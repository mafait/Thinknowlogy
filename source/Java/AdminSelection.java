/*
 *	Class:			AdminSelection
 *	Supports class:	AdminItem
 *	Purpose:		To process selections
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

class AdminSelection
	{
	// Private constructible variables

	private SelectionItem firstSelectionItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte removeDuplicateSelection()
		{
		if( adminItem_.conditionList != null )
			{
			if( adminItem_.actionList != null )
				{
				if( adminItem_.conditionList.deleteActiveItemsWithCurrentSentenceNr() == Constants.RESULT_OK )
					{
					if( adminItem_.actionList.deleteActiveItemsWithCurrentSentenceNr() == Constants.RESULT_OK )
						{
						if( adminItem_.alternativeList != null )
							{
							if( adminItem_.alternativeList.deleteActiveItemsWithCurrentSentenceNr() != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to remove the alternative of a selection" );
							}
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to remove the action of a selection" );
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to remove the condition of a selection" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The action list isn't created yet" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The condition list isn't created yet" );

		return Constants.RESULT_OK;
		}

	private SelectionItem firstCondition()
		{
		if( adminItem_.conditionList != null )
			return adminItem_.conditionList.firstActiveSelectionItem();

		return null;
		}


	// Constructor / deconstructor

	protected AdminSelection( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		firstSelectionItem_ = null;

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

	protected byte checkForDuplicateSelection()
		{
		SelectionResultType selectionResult;
		boolean hasFoundDuplicateSelection = false;
		int duplicateConditionSentenceNr;

		if( adminItem_.conditionList != null )
			{
			if( adminItem_.actionList != null )
				{
				if( ( selectionResult = adminItem_.conditionList.checkDuplicateCondition() ).result == Constants.RESULT_OK )
					{
					if( ( duplicateConditionSentenceNr = selectionResult.duplicateConditionSentenceNr ) > Constants.NO_SENTENCE_NR )
						{
						if( ( selectionResult = adminItem_.actionList.checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result == Constants.RESULT_OK )
							{
							if( selectionResult.hasFoundDuplicateSelection )
								{
								if( adminItem_.alternativeList == null )
									hasFoundDuplicateSelection = true;
								else
									{
									if( ( selectionResult = adminItem_.alternativeList.checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result == Constants.RESULT_OK )
										{
										if( selectionResult.hasFoundDuplicateSelection )
											hasFoundDuplicateSelection = true;
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );
									}
								}

							if( hasFoundDuplicateSelection )
								{
								if( removeDuplicateSelection() != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to remove a duplicate selection" );
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check if the action selection part is duplicate" );
						}
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check if the condition selection part is duplicate" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The action list isn't created yet" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The condition list isn't created yet" );

		return Constants.RESULT_OK;
		}

	protected byte createSelectionPart( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short selectionListNr, short imperativeParameter, short prepositionParameter, short specificationWordParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SelectionResultType selectionResult;

		if( generalizationWordItem != null ||
		specificationString != null )
			{
			switch( selectionListNr )
				{
				case Constants.ADMIN_CONDITION_LIST:
					if( adminItem_.conditionList == null )
						{
						// Create list
						if( ( adminItem_.conditionList = new SelectionList( Constants.ADMIN_CONDITION_LIST_SYMBOL, myWordItem_ ) ) != null )
							{
							CommonVariables.adminConditionList = adminItem_.conditionList;
							adminItem_.adminListArray[Constants.ADMIN_CONDITION_LIST] = adminItem_.conditionList;
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to create an admin condition list" );
						}

					if( ( selectionResult = adminItem_.conditionList.createSelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == Constants.RESULT_OK )
						{
						if( firstSelectionItem_ == null )
							firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin condition list" );

					break;

				case Constants.ADMIN_ACTION_LIST:
					if( adminItem_.actionList == null )
						{
						// Create list
						if( ( adminItem_.actionList = new SelectionList( Constants.ADMIN_ACTION_LIST_SYMBOL, myWordItem_ ) ) != null )
							{
							CommonVariables.adminActionList = adminItem_.actionList;
							adminItem_.adminListArray[Constants.ADMIN_ACTION_LIST] = adminItem_.actionList;
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to create an admin action list" );
						}

					if( ( selectionResult = adminItem_.actionList.createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == Constants.RESULT_OK )
						{
						if( firstSelectionItem_ == null )
							firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin action list" );

					break;

				case Constants.ADMIN_ALTERNATIVE_LIST:
					if( adminItem_.alternativeList == null )
						{
						// Create list
						if( ( adminItem_.alternativeList = new SelectionList( Constants.ADMIN_ALTERNATIVE_LIST_SYMBOL, myWordItem_ ) ) != null )
							{
							CommonVariables.adminAlternativeList = adminItem_.alternativeList;
							adminItem_.adminListArray[Constants.ADMIN_ALTERNATIVE_LIST] = adminItem_.alternativeList;
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to create an admin alternative list" );
						}

					if( ( selectionResult = adminItem_.alternativeList.createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == Constants.RESULT_OK )
						{
						if( firstSelectionItem_ == null )
							firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin alternative list" );

					break;

				default:
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given list number is invalid: " + selectionListNr );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word or specification string is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte executeSelection( int endSolveProgress, SelectionItem actionSelectionItem )
		{
		SelectionResultType selectionResult;
		boolean isNewStart;
		boolean isSatisfied;
		boolean isWaitingForNewStart;
		boolean isWaitingForNewLevel;
		boolean waitForExecution;
		boolean doneLastExecution;
		boolean isInitializeVariables;
		short executionLevel;
		short executionListNr;
		short selectionLevel;
		short nSelectionExecutions = 0;
		int executionSentenceNr;
		WordItem conditionWordItem;
		SelectionItem conditionSelectionItem;
		SelectionItem executionSelectionItem;

		do	{
			doneLastExecution = false;
			CommonVariables.isAssignmentChanged = false;

			if( ( conditionSelectionItem = firstCondition() ) != null )
				{
				isSatisfied = false;
				isWaitingForNewStart = false;
				isWaitingForNewLevel = false;
				waitForExecution = false;
				executionLevel = conditionSelectionItem.selectionLevel();
				executionSentenceNr = conditionSelectionItem.activeSentenceNr();

				do	{
					if( conditionSelectionItem == null ||
					executionSentenceNr != conditionSelectionItem.activeSentenceNr() )
						{
						executionListNr = ( isSatisfied ? Constants.ADMIN_ACTION_LIST : Constants.ADMIN_ALTERNATIVE_LIST );
						executionSelectionItem = null;

						if( isSatisfied )
							{
							if( adminItem_.actionList != null )
								executionSelectionItem = adminItem_.actionList.executionStartEntry( executionLevel, executionSentenceNr );
							}
						else
							{
							if( adminItem_.alternativeList != null )
								executionSelectionItem = adminItem_.alternativeList.executionStartEntry( executionLevel, executionSentenceNr );
							}

						if( executionSelectionItem != null )
							{
							isInitializeVariables = true;

							do	{
								if( adminItem_.executeImperative( isInitializeVariables, executionListNr, executionSelectionItem.imperativeParameter(), executionSelectionItem.specificationWordParameter(), executionSelectionItem.specificationWordTypeNr(), endSolveProgress, executionSelectionItem.specificationString(), executionSelectionItem.generalizationWordItem(), executionSelectionItem.specificationWordItem(), null, null, executionSelectionItem, actionSelectionItem ) == Constants.RESULT_OK )
									isInitializeVariables = false;
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute an imperative" );
								}
							while( !adminItem_.hasRequestedRestart() &&
							!CommonVariables.hasShownWarning &&
							( executionSelectionItem = executionSelectionItem.nextExecutionItem( executionLevel, executionSentenceNr ) ) != null );
							}

						if( conditionSelectionItem != null )	// Found new condition
							{
							isSatisfied = false;
							isWaitingForNewStart = false;
							isWaitingForNewLevel = false;
							waitForExecution = false;
							executionLevel = conditionSelectionItem.selectionLevel();
							}
						}

					if( conditionSelectionItem == null )
						doneLastExecution = true;
					else
						{
						isNewStart = conditionSelectionItem.isNewStart();
						selectionLevel = conditionSelectionItem.selectionLevel();

						if( isNewStart &&
						isWaitingForNewStart &&
						executionLevel == selectionLevel )	// Found new start
							isWaitingForNewStart = false;

						if( isWaitingForNewLevel &&
						executionLevel != selectionLevel )	// Found new level
							isWaitingForNewLevel = false;

						if( !isWaitingForNewStart &&
						!isWaitingForNewLevel &&
						!waitForExecution )
							{
							if( !isNewStart &&
							!isSatisfied &&
							executionLevel == selectionLevel )
								isWaitingForNewStart = true;	// Skip checking of this condition part and wait for a new start to come on this level
							else
								{
								if( isNewStart &&
								isSatisfied &&
								executionLevel == selectionLevel )
									isWaitingForNewLevel = true;		// Skip checking of this condition part and wait for a new level to come
								else
									{
									if( executionLevel != selectionLevel &&
									isSatisfied != conditionSelectionItem.isAction() )
										waitForExecution = true;	// Skip checking of this condition and wait for the next condition sentence number to come
									else
										{
										conditionWordItem = conditionSelectionItem.generalizationWordItem();

										if( conditionWordItem != null )
											{
											if( ( selectionResult = adminItem_.checkCondition( conditionSelectionItem ) ).result == Constants.RESULT_OK )
												{
												isSatisfied = selectionResult.isConditionSatisfied;
												executionLevel = selectionLevel;
												executionSentenceNr = conditionSelectionItem.activeSentenceNr();
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check the condition of word \"" + conditionWordItem.anyWordTypeString() + "\"" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined condition word" );
										}
									}
								}
							}

						conditionSelectionItem = conditionSelectionItem.nextSelectionItem();
						}
					}
				while( !doneLastExecution &&
				!adminItem_.hasRequestedRestart() &&
				!CommonVariables.hasShownWarning );
				}
			}
		while( !adminItem_.hasRequestedRestart() &&
		!CommonVariables.hasShownWarning &&
		CommonVariables.isAssignmentChanged &&
		++nSelectionExecutions < Constants.MAX_SELECTION_EXECUTIONS );

		if( CommonVariables.isAssignmentChanged &&
		nSelectionExecutions == Constants.MAX_SELECTION_EXECUTIONS )
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "There is probably an endless loop in the selections" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"Praise his name with dancing,
 *	accompanied by tambourine and harp.
 *	For the Lord delights in his people;
 *	he crowns the humble with victory." (Psalm 149:3-4)
 *
 *************************************************************************/
