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

#include "AdminItem.h"
#include "Presentation.cpp"
#include "ReadItem.cpp"
#include "SelectionList.h"
#include "SpecificationItem.cpp"

class AdminImperative
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundVirtualListAction_;
	bool isRestart_;

	SpecificationItem *foundVirtualListAssignmentItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

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
						if( adminItem_->writeTextWithPossibleQueryCommands( PRESENTATION_PROMPT_INFO, executionString ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the execution string" );
						}
					}
				else
					{
					adminItem_->initializeAdminWriteVariables();

					switch( specificationWordParameter )
						{
						case WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
							if( adminItem_->writeJustificationReport( specificationWordItem ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write justification for a word" );

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
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read the info file with a singular noun word" );
										}

									// Try to show all knowledge about this specification
									if( adminItem_->writeInfoAboutWord( false, true, true, true, true, true, true, true, NULL, specificationWordItem ) != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write info about a word" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read the info file" );
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The specification word item is undefined" );
						}

					if( !commonVariables_->hasShownMessage &&
					specificationWordItem != NULL )
						{
						if( adminItem_->isImperativeSentence() )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_NO ) == RESULT_OK )
								{
								if( commonVariables_->presentation->writeDiacriticalText( false, false, PRESENTATION_PROMPT_NOTIFICATION, commonVariables_->writeSentenceString ) != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a sentence" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
							}
						else
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_HAVE_ANY_INFO_ABOUT_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_HAVE_ANY_INFO_ABOUT_END ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType addWordToVirtualList( bool isSelection, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWordToVirtualList";

		if( generalizationWordItem != NULL )
			{
			if( generalizationWordItem->addSpecification( true, false, false, false, false, false, false, false, false, false, isSelection, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, specificationWordItem, NULL, NULL, NULL ).result == RESULT_OK )
				{
				if( generalizationWordItem->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, specificationWordItem, NULL, NULL ).result != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign a virtual list word" );
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign a virtual list word" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

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
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a word to a virtual list" );
								}
							else
								{
								if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
								}

							break;

						case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
							switch( prepositionParameter )
								{
								case WORD_PARAMETER_PREPOSITION_OF:		// Head or tail
									if( specificationWordItem->isNounHead() )
										{
										hasFoundHeadOrTail = true;

										if( ( foundVirtualListAssignmentItem_ = relationWordItem->lastActiveNonQuestionAssignmentItem() ) == NULL )
											hasFoundVirtualListAction_ = true;
										else
											{
											if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to dectivate the head of a virtual list" );
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
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to dectivate the tail of a virtual list" );
												}
											}
										}

								// Don't insert a break statement here

								case WORD_PARAMETER_PREPOSITION_FROM:
									if( !hasFoundHeadOrTail )
										{
										if( ( foundVirtualListAssignmentItem_ = relationWordItem->firstAssignmentItem( true, false, false, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
											hasFoundVirtualListAction_ = true;
										else
											{
											if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to dectivate the word in a virtual list" );
											}
										}

									break;

								case WORD_PARAMETER_PREPOSITION_TO:
									if( foundVirtualListAssignmentItem_ == NULL )
										{
										if( !hasFoundVirtualListAction_ )
											{
											if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
											}
										}
									else
										{
										if( addWordToVirtualList( false, foundVirtualListAssignmentItem_->generalizationWordTypeNr(), foundVirtualListAssignmentItem_->specificationWordTypeNr(), relationWordItem, foundVirtualListAssignmentItem_->specificationWordItem() ) != RESULT_OK )
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a word to a virtual list" );
										}

									break;

								default:
									if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION ) != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning about the preposition parameter" );
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
									foundVirtualListAssignmentItem_ = relationWordItem->firstAssignmentItem( true, false, false, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );
								}

							if( foundVirtualListAssignmentItem_ != NULL )
								{
								if( relationWordItem->inactivateActiveAssignment( foundVirtualListAssignmentItem_ ) != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to dectivate the head of a virtual list" );
								}

							break;

						default:
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given imperative parameter isn't a virtual list action" );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

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
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
								}
							}
						}
					while( currentReadItem != endRelationReadItem &&
					( currentReadItem = currentReadItem->nextReadItem() ) != NULL );
					}
				else
					{
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
					}
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

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
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given execution selection item has no relation word item" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given execution selection item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminImperative( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundVirtualListAction_ = false;
		isRestart_ = false;

		foundVirtualListAssignmentItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminImperative" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
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
		return isRestart_;
		}

	ResultType executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgress, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem )
		{
		bool showRelationWarning = false;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperative";

		if( isInitializeVariables )
			{
			hasFoundVirtualListAction_ = false;
			foundVirtualListAssignmentItem_ = NULL;
			}

		switch( imperativeParameter )
			{
			case NO_IMPERATIVE_PARAMETER:	// Selection
				if( adminItem_->assignSelectionSpecification( executionSelectionItem ) != RESULT_OK )
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to assign an imperative selection specification at assignment level ", commonVariables_->currentAssignmentLevel );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				if( executionSelectionItem == NULL )
					{
					if( executeVirtualListImperative( imperativeParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
					}
				else
					{
					if( executeVirtualListSelectionImperative( imperativeParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, executionSelectionItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to execute a selection virtual list imperative" );
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
								if( adminItem_->deleteSentences( false, adminItem_->firstUserSentenceNr() ) == RESULT_OK )
									{
									if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR ) != RESULT_OK )
										return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to write an interface notification about clearing my mind" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the previous sentences" );

								break;

							default:
								if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
									return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to write an interface warning about clearing" );
							}
						}
					else
						showRelationWarning = true;
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
				if( generalizationWordItem != NULL )
					{
					if( startRelationWordReadItem == NULL )
						{
						if( adminItem_->readInfoFile( true, generalizationWordItem->anyWordTypeString() ).result != RESULT_OK )
							return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to read the info help file" );
						}
					else
						showRelationWarning = true;
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( startRelationWordReadItem == NULL )
					{
					if( adminItem_->login( specificationWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to login" );
					}
				else
					showRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
				if( startRelationWordReadItem == NULL )
					{
					switch( specificationWordParameter )
						{
						case WORD_PARAMETER_NOUN_FILE:
							if( adminItem_->readExampleFile( executionString ) != RESULT_OK )
								return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to read an example file" );

							break;

						default:
							if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
								return myWordItem_->startErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I don't know how to perform imperative 'read'. Unknown specification parameter: ", specificationWordParameter );

							return myWordItem_->startErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I don't know how to perform imperative 'read'. Unknown specification parameter: ", specificationWordParameter, " at assignment level ", commonVariables_->currentAssignmentLevel );
						}
					}
				else
					showRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO:
				if( adminItem_->undoLastSentence() == RESULT_OK )
					adminItem_->dontDeletedRollbackInfo();
				else
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to undo the last sentence" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO:
				if( adminItem_->redoLastUndoneSentence() == RESULT_OK )
					adminItem_->dontDeletedRollbackInfo();
				else
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to redo the last undone sentence" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART:
				isRestart_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW:
				if( executeImperativeShow( specificationWordParameter, specificationWordTypeNr, generalizationWordItem, specificationWordItem, executionString ) != RESULT_OK )
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to execute the show imperative" );

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
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to solve a word at assignment level ", commonVariables_->currentAssignmentLevel );
					}
				else
					{
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, generalizationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to write an interface warning about solving" );
					}

				break;

			default:
				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
					return myWordItem_->startErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I don't know how to execute the imperative with word parameter: ", imperativeParameter );

				return myWordItem_->startErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I don't know how to execute the imperative with word parameter: ", imperativeParameter, ", at assignment level ", commonVariables_->currentAssignmentLevel );
			}

		if( showRelationWarning )
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION ) != RESULT_OK )
				return myWordItem_->addErrorInItem( adminItem_->adminListChar( executionListNr ), functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
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
