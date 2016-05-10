/*	Class:			AdminImperative
 *	Supports class:	AdminItem
 *	Purpose:		To execute imperative words
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

#include "AdminItem.h"
#include "ReadItem.cpp"
#include "SelectionList.h"

class AdminImperative
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundVirtualListAction_;
	bool hasRequestedRestart_;

	SpecificationItem *foundVirtualListAssignmentItem_;

	char executionString_[MAX_SENTENCE_STRING_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType addWordToVirtualList( bool isSelection, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWordToVirtualList";

		if( generalizationWordItem != NULL )
			{
			if( generalizationWordItem->addSpecification( true, false, false, false, false, false, false, false, false, false, isSelection, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, specificationWordItem, NULL, NULL, NULL ).result == RESULT_OK )
				{
				if( generalizationWordItem->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, specificationWordItem, NULL, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a virtual list word" );
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a virtual list specification" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType executeImperativeShow( unsigned short specificationWordParameter, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *executionString )
		{
		FileResultType fileResult;
		char *singularNounString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperativeShow";

		if( generalizationWordItem != NULL )
			{
			if( executionString != NULL ||
			specificationWordItem != NULL )
				{
				if( executionString != NULL &&
				specificationWordTypeNr == WORD_TYPE_TEXT )
					{
					if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
						{
						if( adminItem_->writeInfoTextWithPossibleQueryCommands( executionString ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the execution string" );
						}
					}
				else
					{
					adminItem_->initializeAdminWriteVariables();

					switch( specificationWordParameter )
						{
						case WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
							if( adminItem_->writeJustificationReport( specificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write justification for a word" );

							break;

						default:
							// Try to read the info file with the specification name
							if( specificationWordItem != NULL )
								{
								if( ( fileResult = adminItem_->readInfoFile( false, specificationWordItem->activeWordTypeString( specificationWordTypeNr ) ) ).result == RESULT_OK )
									{
									if( fileResult.createdFileItem == NULL &&
									specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
									( singularNounString = specificationWordItem->singularNounString() ) != NULL )
										{
										if( adminItem_->readInfoFile( false, singularNounString ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read the info file with a singular noun word" );
										}

									// Try to show all knowledge about this specification
									if( adminItem_->writeInfoAboutWord( false, true, true, true, true, true, true, true, specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write info about a word" );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read the info file" );
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "The specification word item is undefined" );
						}

					if( !commonVariables_->hasShownMessage &&
					specificationWordItem != NULL )
						{
						if( adminItem_->isImperativeSentence() &&
						strlen( commonVariables_->writtenSentenceString ) > 0 )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_NO ) == RESULT_OK )
								{
								if( commonVariables_->presentation->writeDiacriticalText( false, false, PRESENTATION_PROMPT_NOTIFICATION, commonVariables_->writtenSentenceString ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a sentence" );
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have no' interface notification" );
							}
						else
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_HAVE_NO_INFO_ABOUT_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_HAVE_NO_INFO_ABOUT_END ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType executeVirtualListImperative( unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundHeadOrTail = false;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeVirtualListImperative";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					switch( imperativeParameter )
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
								if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
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

										if( ( foundVirtualListAssignmentItem_ = relationWordItem->lastActiveNonQuestionAssignmentItem() ) == NULL )
											hasFoundVirtualListAction_ = true;
										else
											{
											if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the head of a virtual list" );
											}
										}
									else
										{
										if( specificationWordItem->isNounTail() )
											{
											hasFoundHeadOrTail = true;

											if( ( foundVirtualListAssignmentItem_ = relationWordItem->firstNonQuestionActiveAssignmentItem() ) == NULL )
												hasFoundVirtualListAction_ = true;
											else
												{
												if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the tail of a virtual list" );
												}
											}
										}

								// Don't insert a break statement here

								case WORD_PARAMETER_PREPOSITION_FROM:
									if( !hasFoundHeadOrTail )
										{
										if( ( foundVirtualListAssignmentItem_ = relationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
											hasFoundVirtualListAction_ = true;
										else
											{
											if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the word in a virtual list" );
											}
										}

									break;

								case WORD_PARAMETER_PREPOSITION_TO:
									if( foundVirtualListAssignmentItem_ == NULL )
										{
										if( !hasFoundVirtualListAction_ )
											{
											if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
											}
										}
									else
										{
										if( addWordToVirtualList( false, foundVirtualListAssignmentItem_->generalizationWordTypeNr(), foundVirtualListAssignmentItem_->specificationWordTypeNr(), relationWordItem, foundVirtualListAssignmentItem_->specificationWordItem() ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a word to a virtual list" );
										}

									break;

								default:
									if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the preposition parameter" );
								}

							break;

						case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
							foundVirtualListAssignmentItem_ = NULL;

							if( specificationWordItem->isNounHead() )
								foundVirtualListAssignmentItem_ = relationWordItem->lastActiveNonQuestionAssignmentItem();
							else
								{
								if( specificationWordItem->isNounTail() )
									foundVirtualListAssignmentItem_ = relationWordItem->firstNonQuestionActiveAssignmentItem();
								else
									foundVirtualListAssignmentItem_ = relationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, specificationWordItem );
								}

							if( foundVirtualListAssignmentItem_ != NULL )
								{
								if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the head of a virtual list" );
								}

							break;

						default:
							return adminItem_->startError( functionNameString, moduleNameString_, "The given imperative parameter isn't a virtual list action" );
						}
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType executeVirtualListImperative( unsigned short imperativeParameter, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem )
		{
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		ReadItem *currentReadItem = startRelationWordReadItem;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeVirtualListImperative";

		if( generalizationWordItem != NULL )
			{
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
							( relationWordItem = currentReadItem->readWordItem() ) != NULL )
								{
								if( executeVirtualListImperative( imperativeParameter, prepositionParameter, specificationWordTypeNr, currentReadItem->wordTypeNr(), generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
								}
							}
						}
					while( currentReadItem != endRelationReadItem &&
					( currentReadItem = currentReadItem->nextReadItem() ) != NULL );
					}
				else
					{
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
					}
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType executeVirtualListSelectionImperative( unsigned short imperativeParameter, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SelectionItem *executionSelectionItem )
		{
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeVirtualListSelectionImperative";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( executionSelectionItem != NULL )
					{
					if( ( relationWordItem = executionSelectionItem->relationWordItem() ) != NULL )
						{
						if( executeVirtualListImperative( imperativeParameter, executionSelectionItem->prepositionParameter(), specificationWordTypeNr, executionSelectionItem->relationWordTypeNr(), generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given execution selection item has no relation word item" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given execution selection item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminImperative( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundVirtualListAction_ = false;
		hasRequestedRestart_ = false;

		foundVirtualListAssignmentItem_ = NULL;

		strcpy( executionString_, EMPTY_STRING );

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminImperative" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	bool hasRequestedRestart()
		{
		return hasRequestedRestart_;
		}

	ResultType executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgress, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem )
		{
		bool isShowingRelationWarning = false;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperative";

		if( isInitializeVariables )
			{
			hasFoundVirtualListAction_ = false;
			foundVirtualListAssignmentItem_ = NULL;
			}

		switch( imperativeParameter )
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
					if( executeVirtualListImperative( imperativeParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
					}
				else
					{
					if( executeVirtualListSelectionImperative( imperativeParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, executionSelectionItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to execute a selection virtual list imperative" );
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
				if( generalizationWordItem != NULL )
					{
					if( startRelationWordReadItem == NULL )
						{
						switch( specificationWordParameter )
							{
							case WORD_PARAMETER_NOUN_MIND:
								if( adminItem_->deleteSentences( adminItem_->firstSentenceNrOfCurrentUser() ) == RESULT_OK )
									{
									if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write the 'my mind is clear' interface notification about clearing my mind" );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the knowledge entered by the current user" );

								break;

							default:
								if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write an interface warning about clearing" );
							}
						}
					else
						isShowingRelationWarning = true;
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
				if( generalizationWordItem != NULL )
					{
					if( startRelationWordReadItem == NULL )
						{
						if( adminItem_->readInfoFile( true, generalizationWordItem->anyWordTypeString() ).result != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to read the info help file" );
						}
					else
						isShowingRelationWarning = true;
					}
				else
					return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "The given generalization word item is undefined" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( startRelationWordReadItem == NULL )
					{
					if( adminItem_->login( specificationWordItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to login" );
					}
				else
					isShowingRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
				if( startRelationWordReadItem == NULL )
					{
					switch( specificationWordParameter )
						{
						case WORD_PARAMETER_NOUN_FILE:
							if( adminItem_->readExampleFile( executionString ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to read an example file" );

							break;

						case WORD_PARAMETER_NOUN_TEST_FILE:
							if( executionString != NULL &&
							strlen( executionString ) > 0 )
								{
								// Store file name before closing the test file
								strcpy( executionString_, executionString );

								// Postpone result check. Close test file first
								adminItem_->readTestFile( executionString_ );

								// Stop redirecting to test results
								commonVariables_->presentation->redirectOutputToTestFile( NULL );

								// Test file is closed. Now check the result
								if( commonVariables_->result == RESULT_OK )
									{
										if( adminItem_->compareOutputFileAgainstReferenceFile( executionString_ ) == RESULT_OK )
											// Continue redirecting to test results
											commonVariables_->presentation->redirectOutputToTestFile( adminItem_->currentWriteFile() );
										else
											return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to compare the test file against the reference file" );
									}
								else
									{
									commonVariables_->presentation->clearProgress();
									return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to read a test file" );
									}
								}
							else
								return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "The given execution string is undefined or empty" );

							break;

						default:
							if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
								return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: ", specificationWordParameter );

							return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: ", specificationWordParameter, " at assignment level ", commonVariables_->currentAssignmentLevel );
						}
					}
				else
					isShowingRelationWarning = true;

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

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW:
				if( executeImperativeShow( specificationWordParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, executionString ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to execute the show imperative" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE:
				if( executionSelectionItem != NULL ||

				( executionListNr == NO_LIST_NR &&
				specificationWordItem != NULL &&
				specificationWordItem->wordParameter() != WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE ) )
					{
					adminItem_->deleteTemporaryScoreList();
					adminItem_->clearCurrentSolveProgress();

					if( adminItem_->solveWord( endSolveProgress, specificationWordItem, actionSelectionItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to solve a word at assignment level ", commonVariables_->currentAssignmentLevel );
					}
				else
					{
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write an interface warning about solving" );
					}

				break;

			default:
				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
					return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute the imperative with word parameter: ", imperativeParameter );

				return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute the imperative with word parameter: ", imperativeParameter, ", at assignment level ", commonVariables_->currentAssignmentLevel );
			}

		if( isShowingRelationWarning )
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"He has given me a new song to sing,
 *	a hymn of praise to our God.
 *	Many will see what he has done and be amazed.
 *	They will put their trust in the Lord." (Psalm 40:3)
 *************************************************************************/
