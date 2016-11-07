/*	Class:			AdminImperative
 *	Supports class:	AdminItem
 *	Purpose:		To execute imperative words
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

class AdminImperative
	{
	// Private constructed variables

	private boolean hasFoundVirtualListAction_;
	private boolean hasRequestedRestart_;

	private SpecificationItem foundVirtualListAssignmentItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte addWordToVirtualList( boolean isSelection, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem.addSpecificationInWord( true, false, false, false, false, false, false, false, false, false, isSelection, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, specificationWordItem, null, null, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a virtual list specification" );

		if( generalizationWordItem.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, specificationWordItem, null, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign a virtual list word" );

		return Constants.RESULT_OK;
		}

	private byte executeImperativeDisplay( short executionNounWordParameter, short executionWordTypeNr, WordItem imperativeVerbWordItem, WordItem executionWordItem, String executionString )
		{
		FileResultType fileResult;
		String singularNounString;

		if( imperativeVerbWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given imperative verb word item is undefined" );

		if( executionString != null ||
		executionWordItem != null )
			{
			if( executionString != null &&
			executionWordTypeNr == Constants.WORD_TYPE_TEXT )
				{
				if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					{
					if( adminItem_.writeInfoTextWithPossibleQueryCommands( executionString ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the execution string" );
					}
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

					default:
						// Try to read the info file with the specification name
						if( executionWordItem == null )
							return adminItem_.startError( 1, moduleNameString_, "The specification word item is undefined" );

						if( ( fileResult = adminItem_.readInfoFile( false, executionWordItem.activeWordTypeString( executionWordTypeNr ) ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to read the info file" );

						if( fileResult.createdFileItem == null &&
						executionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						( singularNounString = executionWordItem.singularNounString() ) != null )
							{
							if( adminItem_.readInfoFile( false, singularNounString ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to read the info file with a singular noun word" );
							}

						// Try to display all knowledge about this specification
						if( adminItem_.writeInfoAboutWord( false, true, true, true, true, true, true, true, executionWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write info about a word" );
					}

				if( !CommonVariables.hasDisplayedMessage &&
				executionWordItem != null )
					{
					if( adminItem_.isImperativeSentence() &&
					CommonVariables.writtenSentenceStringBuffer != null &&
					CommonVariables.writtenSentenceStringBuffer.length() > 0 )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_NO ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I have no' interface notification" );

						if( Presentation.writeDiacriticalText( false, false, Constants.PRESENTATION_PROMPT_NOTIFICATION, CommonVariables.writtenSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write a sentence" );
						}
					else
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_HAVE_NO_INFO_ABOUT_START, executionWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_HAVE_NO_INFO_ABOUT_END ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListImperative( short imperativeVerbParameter, short prepositionParameter, short specificationWordTypeNr, short relationWordTypeNr, WordItem imperativeVerbWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundHeadOrTail = false;

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
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
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

							if( ( foundVirtualListAssignmentItem_ = relationWordItem.lastActiveNonQuestionAssignmentItem() ) == null )
								hasFoundVirtualListAction_ = true;
							else
								{
								if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to dectivate the head of a virtual list" );
								}
							}
						else
							{
							if( specificationWordItem.isNounTail() )
								{
								hasFoundHeadOrTail = true;

								if( ( foundVirtualListAssignmentItem_ = relationWordItem.firstNonQuestionActiveAssignmentItem() ) == null )
									hasFoundVirtualListAction_ = true;
								else
									{
									if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to dectivate the tail of a virtual list" );
									}
								}
							}

					// Don't insert a break statement here

					case Constants.WORD_PARAMETER_PREPOSITION_FROM:
						if( !hasFoundHeadOrTail )
							{
							if( ( foundVirtualListAssignmentItem_ = relationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, Constants.NO_CONTEXT_NR, specificationWordItem ) ) == null )
								hasFoundVirtualListAction_ = true;
							else
								{
								if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to dectivate the word in a virtual list" );
								}
							}

						break;

					case Constants.WORD_PARAMETER_PREPOSITION_TO:
						if( foundVirtualListAssignmentItem_ == null )
							{
							if( !hasFoundVirtualListAction_ )
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
								}
							}
						else
							{
							if( addWordToVirtualList( false, foundVirtualListAssignmentItem_.generalizationWordTypeNr(), foundVirtualListAssignmentItem_.specificationWordTypeNr(), relationWordItem, foundVirtualListAssignmentItem_.specificationWordItem() ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a word to a virtual list" );
							}

						break;

					default:
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the preposition parameter" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				foundVirtualListAssignmentItem_ = null;

				if( specificationWordItem.isNounHead() )
					foundVirtualListAssignmentItem_ = relationWordItem.lastActiveNonQuestionAssignmentItem();
				else
					{
					if( specificationWordItem.isNounTail() )
						foundVirtualListAssignmentItem_ = relationWordItem.firstNonQuestionActiveAssignmentItem();
					else
						foundVirtualListAssignmentItem_ = relationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, Constants.NO_CONTEXT_NR, specificationWordItem );
					}

				if( foundVirtualListAssignmentItem_ != null )
					{
					if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to dectivate the head of a virtual list" );
					}

				break;

			default:
				return adminItem_.startError( 1, moduleNameString_, "The given imperative parameter isn't a virtual list action" );
			}

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListImperative( short imperativeVerbParameter, short specificationWordTypeNr, WordItem imperativeVerbWordItem, WordItem specificationWordItem, ReadItem startRelationWordReadItem, ReadItem endRelationReadItem )
		{
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		ReadItem currentReadItem = startRelationWordReadItem;
		WordItem relationWordItem;

		if( imperativeVerbWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem != null )
			{
			if( startRelationWordReadItem != null )
				{
				do	{
					if( currentReadItem.isPreposition() )
						prepositionParameter = currentReadItem.wordParameter();
					else
						{
						if( currentReadItem.isRelationWord() &&
						( relationWordItem = currentReadItem.readWordItem() ) != null )
							{
							if( executeVirtualListImperative( imperativeVerbParameter, prepositionParameter, specificationWordTypeNr, currentReadItem.wordTypeNr(), imperativeVerbWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to execute a virtual list imperative" );
							}
						}
					}
				while( currentReadItem != endRelationReadItem &&
				( currentReadItem = currentReadItem.nextReadItem() ) != null );
				}
			else
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
				}
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
			}

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListSelectionImperative( short imperativeVerbParameter, short prepositionParameter, short specificationWordTypeNr, short relationWordTypeNr, WordItem imperativeVerbWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( imperativeVerbWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( executeVirtualListImperative( imperativeVerbParameter, prepositionParameter, specificationWordTypeNr, relationWordTypeNr, imperativeVerbWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to execute a virtual list imperative" );

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminImperative( AdminItem adminItem )
		{
		String errorString = null;

		hasFoundVirtualListAction_ = false;
		hasRequestedRestart_ = false;

		foundVirtualListAssignmentItem_ = null;

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

	protected boolean hasRequestedRestart()
		{
		return hasRequestedRestart_;
		}

	protected byte executeImperative( boolean isInitializeVariables, short executionListNr, short imperativeVerbParameter, short executionNounWordParameter, short specificationWordTypeNr, int endSolveProgress, String executionString, WordItem imperativeVerbWordItem, WordItem specificationWordItem, ReadItem startRelationWordReadItem, ReadItem endRelationReadItem, SelectionItem executionSelectionItem, SelectionItem actionSelectionItem )
		{
		boolean isDisplayingRelationWarning = false;

		if( isInitializeVariables )
			{
			hasFoundVirtualListAction_ = false;
			foundVirtualListAssignmentItem_ = null;
			}

		switch( imperativeVerbParameter )
			{
			// Selection
			case Constants.NO_IMPERATIVE_PARAMETER:
				if( adminItem_.assignSelectionSpecification( executionSelectionItem ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to assign an imperative selection specification at assignment level " + CommonVariables.currentAssignmentLevel );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				if( executionSelectionItem == null )
					{
					if( executeVirtualListImperative( imperativeVerbParameter, specificationWordTypeNr, imperativeVerbWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to execute a virtual list imperative" );
					}
				else
					{
					if( executeVirtualListSelectionImperative( imperativeVerbParameter, executionSelectionItem.prepositionParameter(), specificationWordTypeNr, executionSelectionItem.relationWordTypeNr(), imperativeVerbWordItem, specificationWordItem, executionSelectionItem.relationWordItem() ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to execute a selection virtual list imperative" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
				if( imperativeVerbWordItem == null )
					return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "The given imperative verb word item is undefined" );

				if( startRelationWordReadItem == null )
					{
					switch( executionNounWordParameter )
						{
						case Constants.WORD_PARAMETER_NOUN_MIND:
							if( adminItem_.deleteSentences( adminItem_.firstSentenceNrOfCurrentUser() ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to delete the knowledge entered by the current user" );

							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write the 'my mind is clear' interface notification about clearing my mind" );

							break;

						default:
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write an interface warning about clearing" );
						}
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
				if( imperativeVerbWordItem == null )
					return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "The given imperative verb word item is undefined" );

				if( startRelationWordReadItem == null )
					{
					if( adminItem_.readInfoFile( true, imperativeVerbWordItem.anyWordTypeString() ).result != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to read the info help file" );
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( startRelationWordReadItem == null )
					{
					if( adminItem_.login( specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to login" );
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
				if( startRelationWordReadItem == null )
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
							Presentation.redirectOutputToTestFile( null );

							if( CommonVariables.result == Constants.RESULT_OK &&
							!Console.isTestingCanceled() )
								{
								if( adminItem_.compareOutputFileAgainstReferenceFile( executionString ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to compare the test file against the reference file" );
								}

							// Continue redirecting to test results
							Presentation.redirectOutputToTestFile( adminItem_.currentWriteFile() );

							// Now check the result
							if( CommonVariables.result != Constants.RESULT_OK )
								{
								Presentation.clearProgress();
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to read a test file" );
								}

							break;

						default:
							if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
								return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: " + executionNounWordParameter );

							return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: " + executionNounWordParameter + " at assignment level " + CommonVariables.currentAssignmentLevel );
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
					adminItem_.clearCurrentSolveProgress();

					if( adminItem_.solveWord( endSolveProgress, specificationWordItem, actionSelectionItem ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to solve a word at assignment level " + CommonVariables.currentAssignmentLevel );
					}
				else
					{
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write an interface warning about solving" );
					}

				break;

			default:
				if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute the imperative with word parameter: " + imperativeVerbParameter );

				return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute the imperative with word parameter: " + imperativeVerbParameter + ", at assignment level " + CommonVariables.currentAssignmentLevel );
			}

		if( isDisplayingRelationWarning )
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION ) != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"He has given me a new song to sing,
 *	a hymn of praise to our God.
 *	Many will see what he has done and be amazed.
 *	They will put their trust in the Lord." (Psalm 40:3)
 *************************************************************************/
