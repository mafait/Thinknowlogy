/*
 *	Class:			AdminImperative
 *	Supports class:	AdminItem
 *	Purpose:		To execute imperative words
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

class AdminImperative
	{
	// Private constructible variables

	private boolean hasFoundVirtualListAction_;
	private boolean isRestart_;

	private SpecificationItem foundVirtualListAssignmentItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte executeImperativeShow( short specificationWordParameter, short specificationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem, String executionString )
		{
		FileResultType fileResult;
		String singularNounString;

		if( generalizationWordItem != null )
			{
			if( executionString != null ||
			specificationWordItem != null )
				{
				if( executionString != null &&
				specificationWordTypeNr == Constants.WORD_TYPE_TEXT )
					{
					if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
						{
						if( adminItem_.writeTextWithPossibleQueryCommands( Constants.PRESENTATION_PROMPT_INFO, executionString ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the execution string" );
						}
					}
				else
					{
					adminItem_.initializeAdminWriteVariables();

					switch( specificationWordParameter )
						{
						case Constants.WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
							if( adminItem_.writeJustificationReport( specificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write justification for a word" );

							break;

						default:
							// Try to read the info file with the specification name
							if( specificationWordItem != null )
								{
								if( ( fileResult = adminItem_.readInfoFile( false, specificationWordItem.activeWordTypeString( specificationWordTypeNr ) ) ).result == Constants.RESULT_OK )
									{
									if( fileResult.createdFileItem == null &&
									specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
									( singularNounString = specificationWordItem.singularNounString() ) != null )
										{
										if( adminItem_.readInfoFile( false, singularNounString ).result != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read the info file with a singular noun word" );
										}

									// Try to show all knowledge about this specification
									if( adminItem_.writeInfoAboutWord( false, true, true, true, true, true, true, true, null, specificationWordItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write info about a word" );
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read the info file" );
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "The specification word item is undefined" );
						}

					if( !CommonVariables.hasShownMessage &&
					specificationWordItem != null )
						{
						if( adminItem_.isImperativeSentence() )
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_NO ) == Constants.RESULT_OK )
								{
								if( Presentation.writeDiacriticalText( false, false, Constants.PRESENTATION_PROMPT_NOTIFICATION, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a sentence" );
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
							}
						else
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_HAVE_ANY_INFO_ABOUT_START, specificationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_HAVE_ANY_INFO_ABOUT_END ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			else
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte addWordToVirtualList( boolean isSelection, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( generalizationWordItem != null )
			{
			if( generalizationWordItem.addSpecification( true, false, false, false, false, false, false, false, false, false, isSelection, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, specificationWordItem, null, null, null ).result == Constants.RESULT_OK )
				{
				if( generalizationWordItem.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, specificationWordItem, null, null ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign a virtual list word" );
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign a virtual list word" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListImperative( short imperativeParameter, short prepositionParameter, short specificationWordTypeNr, short relationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundHeadOrTail = false;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( relationWordItem != null )
					{
					switch( imperativeParameter )
						{
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
							if( !specificationWordItem.isNounHead() &&
							!specificationWordItem.isNounTail() )
								{
								if( addWordToVirtualList( false, relationWordTypeNr, specificationWordTypeNr, relationWordItem, specificationWordItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a word to a virtual list" );
								}
							else
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
								}

							break;

						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
							switch( prepositionParameter )
								{
								case Constants.WORD_PARAMETER_PREPOSITION_OF:		// Head or tail
									if( specificationWordItem.isNounHead() )
										{
										hasFoundHeadOrTail = true;

										if( ( foundVirtualListAssignmentItem_ = relationWordItem.lastActiveNonQuestionAssignmentItem() ) == null )
											hasFoundVirtualListAction_ = true;
										else
											{
											if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to dectivate the head of a virtual list" );
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
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to dectivate the tail of a virtual list" );
												}
											}
										}

								// Don't insert a break statement here

								case Constants.WORD_PARAMETER_PREPOSITION_FROM:
									if( !hasFoundHeadOrTail )
										{
										if( ( foundVirtualListAssignmentItem_ = relationWordItem.firstAssignmentItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) == null )
											hasFoundVirtualListAction_ = true;
										else
											{
											if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to dectivate the word in a virtual list" );
											}
										}

									break;

								case Constants.WORD_PARAMETER_PREPOSITION_TO:
									if( foundVirtualListAssignmentItem_ == null )
										{
										if( !hasFoundVirtualListAction_ )
											{
											if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
											}
										}
									else
										{
										if( addWordToVirtualList( false, foundVirtualListAssignmentItem_.generalizationWordTypeNr(), foundVirtualListAssignmentItem_.specificationWordTypeNr(), relationWordItem, foundVirtualListAssignmentItem_.specificationWordItem() ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a word to a virtual list" );
										}

									break;

								default:
									if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning about the preposition parameter" );
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
									foundVirtualListAssignmentItem_ = relationWordItem.firstAssignmentItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );
								}

							if( foundVirtualListAssignmentItem_ != null )
								{
								if( relationWordItem.inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to dectivate the head of a virtual list" );
								}

							break;

						default:
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given imperative parameter isn't a virtual list action" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListImperative( short imperativeParameter, short specificationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem, ReadItem startRelationWordReadItem, ReadItem endRelationReadItem )
		{
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		ReadItem currentReadItem = startRelationWordReadItem;
		WordItem relationWordItem;

		if( generalizationWordItem != null )
			{
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
								if( executeVirtualListImperative( imperativeParameter, prepositionParameter, specificationWordTypeNr, currentReadItem.wordTypeNr(), generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute a virtual list imperative" );
								}
							}
						}
					while( currentReadItem != endRelationReadItem &&
					( currentReadItem = currentReadItem.nextReadItem() ) != null );
					}
				else
					{
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
					}
				}
			else
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListSelectionImperative( short imperativeParameter, short specificationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem, SelectionItem executionSelectionItem )
		{
		WordItem relationWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( executionSelectionItem != null )
					{
					if( ( relationWordItem = executionSelectionItem.relationWordItem() ) != null )
						{
						if( executeVirtualListImperative( imperativeParameter, executionSelectionItem.prepositionParameter(), specificationWordTypeNr, executionSelectionItem.relationWordTypeNr(), generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute a virtual list imperative" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given execution selection item has no relation word item" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given execution selection item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminImperative( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		hasFoundVirtualListAction_ = false;
		isRestart_ = false;

		foundVirtualListAssignmentItem_ = null;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
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
		return isRestart_;
		}

	protected byte executeImperative( boolean isInitializeVariables, short executionListNr, short imperativeParameter, short specificationWordParameter, short specificationWordTypeNr, int endSolveProgress, String executionString, WordItem generalizationWordItem, WordItem specificationWordItem, ReadItem startRelationWordReadItem, ReadItem endRelationReadItem, SelectionItem executionSelectionItem, SelectionItem actionSelectionItem )
		{
		boolean showRelationWarning = false;

		if( isInitializeVariables )
			{
			hasFoundVirtualListAction_ = false;
			foundVirtualListAssignmentItem_ = null;
			}

		switch( imperativeParameter )
			{
			case Constants.NO_IMPERATIVE_PARAMETER:	// Selection
				if( adminItem_.assignSelectionSpecification( executionSelectionItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to assign an imperative selection specification at assignment level " + CommonVariables.currentAssignmentLevel );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				if( executionSelectionItem == null )
					{
					if( executeVirtualListImperative( imperativeParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to execute a virtual list imperative" );
					}
				else
					{
					if( executeVirtualListSelectionImperative( imperativeParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, executionSelectionItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to execute a selection virtual list imperative" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
				if( generalizationWordItem != null )
					{
					if( startRelationWordReadItem == null )
						{
						switch( specificationWordParameter )
							{
							case Constants.WORD_PARAMETER_NOUN_MIND:
								if( adminItem_.deleteSentences( false, adminItem_.firstUserSentenceNr() ) == Constants.RESULT_OK )
									{
									if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to write an interface notification about clearing my mind" );
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the previous sentences" );

								break;

							default:
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to write an interface warning about clearing" );
							}
						}
					else
						showRelationWarning = true;
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
				if( generalizationWordItem != null )
					{
					if( startRelationWordReadItem == null )
						{
						if( adminItem_.readInfoFile( true, generalizationWordItem.anyWordTypeString() ).result != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to read the info help file" );
						}
					else
						showRelationWarning = true;
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( startRelationWordReadItem == null )
					{
					if( adminItem_.login( specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to login" );
					}
				else
					showRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
				if( startRelationWordReadItem == null )
					{
					switch( specificationWordParameter )
						{
						case Constants.WORD_PARAMETER_NOUN_FILE:
							if( adminItem_.readExampleFile( executionString ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to read an example file" );

							break;

						default:
							if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
								return myWordItem_.startErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I don't know how to perform imperative 'read'. Unknown specification parameter: " + specificationWordParameter );

							return myWordItem_.startErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I don't know how to perform imperative 'read'. Unknown specification parameter: " + specificationWordParameter + " at assignment level " + CommonVariables.currentAssignmentLevel );
						}
					}
				else
					showRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO:
				if( adminItem_.undoLastSentence() == Constants.RESULT_OK )
					adminItem_.dontDeletedRollbackInfo();
				else
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to undo the last sentence" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO:
				if( adminItem_.redoLastUndoneSentence() == Constants.RESULT_OK )
					adminItem_.dontDeletedRollbackInfo();
				else
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to redo the last undone sentence" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART:
				isRestart_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW:
				if( executeImperativeShow( specificationWordParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, executionString ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to execute the show imperative" );

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
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to solve a word at assignment level " + CommonVariables.currentAssignmentLevel );
					}
				else
					{
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to write an interface warning about solving" );
					}

				break;

			default:
				if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					return myWordItem_.startErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I don't know how to execute the imperative with word parameter: " + imperativeParameter );

				return myWordItem_.startErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I don't know how to execute the imperative with word parameter: " + imperativeParameter + ", at assignment level " + CommonVariables.currentAssignmentLevel );
			}

		if( showRelationWarning )
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( adminItem_.adminListChar( executionListNr ), 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"He has given me a new song to sing,
 *	a hymn of praise to our God.
 *	Many will see what he has done and be amazed.
 *	They will put their trust in the Lord." (Psalm 40:3)
 *
 *************************************************************************/
