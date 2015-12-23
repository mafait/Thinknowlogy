/*
 *	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		To store selection items
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

class SelectionList extends List
	{
	// Private selection methods

	private int getLowerSentenceNr( int duplicateSentenceNr )
		{
		Item searchItem = firstActiveItem();
		int lowerSentenceNr = Constants.NO_SENTENCE_NR;

		while( searchItem != null )
			{
			if( searchItem.activeSentenceNr() < duplicateSentenceNr &&
			searchItem.activeSentenceNr() > lowerSentenceNr )
				lowerSentenceNr = searchItem.activeSentenceNr();

			searchItem = searchItem.nextItem;
			}

		return lowerSentenceNr;
		}


	// Constructor / deconstructor

	protected SelectionList( char _listChar, WordItem myWordItem )
		{
		initializeListVariables( _listChar, myWordItem );
		}


	// Protected virtual methods

	protected ReferenceResultType findWordReference( WordItem referenceWordItem )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();
		SelectionItem searchItem = firstActiveSelectionItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null &&
		!referenceResult.hasFoundWordReference )
			{
			if( ( referenceResult = searchItem.findWordReference( referenceWordItem ) ).result == Constants.RESULT_OK )
				searchItem = searchItem.nextSelectionItem();
			else
				addError( 1, null, "I failed to check for a reference word item in an active selection item" );
			}

		return referenceResult;
		}


	// Protected methods

	protected void clearConditionChecksForSolving( short selectionLevel, int conditionSentenceNr )
		{
		SelectionItem searchItem = firstActiveSelectionItem();

		while( searchItem != null )
			{
			if( searchItem.selectionLevel() < selectionLevel &&
			searchItem.activeSentenceNr() == conditionSentenceNr )
				searchItem.isConditionCheckedForSolving = false;

			searchItem = searchItem.nextSelectionItem();
			}
		}

	protected byte checkSelectionItemForUsage( SelectionItem unusedSelectionItem )
		{
		SelectionItem searchItem = firstActiveSelectionItem();

		if( unusedSelectionItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.nextExecutionItem() == unusedSelectionItem )
					return startError( 1, null, "The reference selection item is still in use" );

				searchItem = searchItem.nextSelectionItem();
				}
			}
		else
			return startError( 1, null, "The given unused justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		SelectionItem searchItem = firstActiveSelectionItem();

		if( unusedWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.generalizationWordItem() == unusedWordItem )
					return startError( 1, null, "The generalization word item is still in use" );

				if( searchItem.specificationWordItem() == unusedWordItem )
					return startError( 1, null, "The specification word item is still in use" );

				searchItem = searchItem.nextSelectionItem();
				}
			}
		else
			return startError( 1, null, "The given unused word item is undefined" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		SelectionItem searchItem = firstActiveSelectionItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeFileItemInFutureDatabase( isCondition, isAction, isAlternative ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a selection item in the database" );
				}

			searchItem = searchItem.nextSelectionItem();
			}

		searchItem = firstReplacedSelectionItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeFileItemInFutureDatabase( isCondition, isAction, isAlternative ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to modify a replaced selection item in the database" );
				}

			searchItem = searchItem.nextSelectionItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected SelectionResultType checkDuplicateCondition()
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.duplicateConditionSentenceNr = CommonVariables.currentSentenceNr;

		do	{
			if( ( selectionResult.duplicateConditionSentenceNr = getLowerSentenceNr( selectionResult.duplicateConditionSentenceNr ) ) > Constants.NO_SENTENCE_NR )
				{
				if( ( selectionResult = checkDuplicateSelectionPart( selectionResult.duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
					addError( 1, null, "I failed to check if the alternative selection part is duplicate" );
				}
			}
		while( CommonVariables.result == Constants.RESULT_OK &&
		!selectionResult.hasFoundDuplicateSelection &&
		selectionResult.duplicateConditionSentenceNr > Constants.NO_SENTENCE_NR );

		selectionResult.result = CommonVariables.result;
		return selectionResult;
		}

	protected SelectionResultType checkDuplicateSelectionPart( int duplicateConditionSentenceNr )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		SelectionItem currentSelectionItem = null;
		SelectionItem searchItem = firstActiveSelectionItem();

		if( duplicateConditionSentenceNr > Constants.NO_SENTENCE_NR )
			{
			if( duplicateConditionSentenceNr < CommonVariables.currentSentenceNr )
				{
				selectionResult.hasFoundDuplicateSelection = true;

				while( searchItem != null &&
				searchItem.activeSentenceNr() >= duplicateConditionSentenceNr )
					{
					if( searchItem.activeSentenceNr() == duplicateConditionSentenceNr )
						{
						currentSelectionItem = firstActiveSelectionItem();

						do	{
							if( currentSelectionItem.isAction() == searchItem.isAction() &&
							currentSelectionItem.isAssignedOrClear() == searchItem.isAssignedOrClear() &&
							currentSelectionItem.isNegative() == searchItem.isNegative() &&
							currentSelectionItem.isNewStart() == searchItem.isNewStart() &&
							currentSelectionItem.selectionLevel() == searchItem.selectionLevel() &&
							currentSelectionItem.generalizationWordItem() == searchItem.generalizationWordItem() &&
							currentSelectionItem.specificationWordItem() == searchItem.specificationWordItem() )
								{
								if( currentSelectionItem.specificationString() != null &&
								searchItem.specificationString() != null )
									selectionResult.hasFoundDuplicateSelection = ( currentSelectionItem.specificationString().equals( searchItem.specificationString() ) );
								else
									{
									if( currentSelectionItem.specificationString() != null ||
									searchItem.specificationString() != null )
										selectionResult.hasFoundDuplicateSelection = false;
									}

								if( selectionResult.hasFoundDuplicateSelection )
									{
									currentSelectionItem = ( currentSelectionItem.nextSelectionItem() != null &&
															currentSelectionItem.nextSelectionItem().hasCurrentCreationSentenceNr() ? currentSelectionItem.nextSelectionItem() : null );

									searchItem = ( searchItem.nextSelectionItem() != null &&
												searchItem.nextSelectionItem().activeSentenceNr() == duplicateConditionSentenceNr ? searchItem.nextSelectionItem() : null );
									}
								else
									searchItem = null;
								}
							else
								{
								selectionResult.hasFoundDuplicateSelection = false;
								searchItem = null;
								}
							}
						while( selectionResult.hasFoundDuplicateSelection &&
						currentSelectionItem != null &&
						searchItem != null );
						}
					else
						searchItem = searchItem.nextSelectionItem();
					}
				}
			else
				startError( 1, null, "The given duplicate sentence number is equal or higher than the current sentence number" );
			}
		else
			startError( 1, null, "The given duplicate sentence number is undefined" );

		selectionResult.result = CommonVariables.result;
		return selectionResult;
		}

	protected SelectionResultType createSelectionItem( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short imperativeParameter, short prepositionParameter, short specificationWordParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		if( generalizationWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		generalizationWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( specificationWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
			specificationWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
				{
				if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
					{
					if( ( selectionResult.lastCreatedSelectionItem = new SelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString, this, myWordItem() ) ) != null )
						{
						if( addItemToList( Constants.QUERY_ACTIVE_CHAR, selectionResult.lastCreatedSelectionItem ) != Constants.RESULT_OK )
							addError( 1, null, "I failed to add an active selection item" );
						}
					else
						startError( 1, null, "I failed to create a selection item" );
					}
				else
					startError( 1, null, "The current item number is undefined" );
				}
			else
				startError( 1, null, "The given specification word type number is undefined or out of bounds" );
			}
		else
			startError( 1, null, "The given generalization word type number is undefined or out of bounds" );

		selectionResult.result = CommonVariables.result;
		return selectionResult;
		}

	protected SelectionResultType findFirstExecutionItem( WordItem solveWordItem )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		SelectionItem firstSelectionItem;

		if( ( firstSelectionItem = firstActiveSelectionItem() ) != null )
			{
			if( firstSelectionItem.findNextExecutionSelectionItem( true, solveWordItem ) == Constants.RESULT_OK )
				selectionResult.firstExecutionItem = firstSelectionItem.nextExecutionItem();
			else
				addError( 1, null, "I failed to find the first execution selection item" );
			}

		selectionResult.result = CommonVariables.result;
		return selectionResult;
		}

	protected SelectionItem executionStartEntry( short executionLevel, int executionSentenceNr )
		{
		SelectionItem searchItem = firstActiveSelectionItem();

		while( searchItem != null &&
		searchItem.activeSentenceNr() >= executionSentenceNr )
			{
			if( searchItem.activeSentenceNr() == executionSentenceNr &&
			searchItem.selectionLevel() == executionLevel )
				return searchItem;

			searchItem = searchItem.nextSelectionItem();
			}

		return null;
		}

	protected SelectionItem firstActiveSelectionItem()
		{
		return (SelectionItem)firstActiveItem();
		}

	protected SelectionItem firstConditionSelectionItem( int conditionSentenceNr )
		{
		SelectionItem searchItem = firstActiveSelectionItem();

		while( searchItem != null &&
		searchItem.activeSentenceNr() >= conditionSentenceNr )
			{
			if( searchItem.activeSentenceNr() == conditionSentenceNr )
				return searchItem;

			searchItem = searchItem.nextSelectionItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"Who can be compared with the Lord or God,
 *	who is enthroned on high?" (Psalm 113:5)
 *************************************************************************/
