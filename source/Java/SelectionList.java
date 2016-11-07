/*	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		To store selection items
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

class SelectionList extends List
	{
	// Private selection methods

	private int getLowerSentenceNr( int duplicateSentenceNr )
		{
		Item searchWordItem = firstActiveItem();
		int lowerSentenceNr = Constants.NO_SENTENCE_NR;

		while( searchWordItem != null )
			{
			if( searchWordItem.activeSentenceNr() < duplicateSentenceNr &&
			searchWordItem.activeSentenceNr() > lowerSentenceNr )
				lowerSentenceNr = searchWordItem.activeSentenceNr();

			searchWordItem = searchWordItem.nextItem;
			}

		return lowerSentenceNr;
		}


	// Constructor

	protected SelectionList( char _listChar, WordItem myWordItem )
		{
		initializeListVariables( _listChar, myWordItem );
		}


	// Protected methods

	protected void clearConditionChecksForSolving( short selectionLevel, int conditionSentenceNr )
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != null )
			{
			if( searchSelectionItem.selectionLevel() < selectionLevel &&
			searchSelectionItem.activeSentenceNr() == conditionSentenceNr )
				searchSelectionItem.isConditionCheckedForSolving = false;

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}
		}

	protected byte checkSelectionItemForUsage( SelectionItem unusedSelectionItem )
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		if( unusedSelectionItem == null )
			return startError( 1, null, "The given unused justification item is undefined" );

		while( searchSelectionItem != null )
			{
			if( searchSelectionItem.nextExecutionItem() == unusedSelectionItem )
				return startError( 1, null, "The reference selection item is still in use" );

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		if( unusedWordItem == null )
			return startError( 1, null, "The given unused word item is undefined" );

		while( searchSelectionItem != null )
			{
			if( searchSelectionItem.generalizationWordItem() == unusedWordItem )
				return startError( 1, null, "The generalization word item is still in use" );

			if( searchSelectionItem.specificationWordItem() == unusedWordItem )
				return startError( 1, null, "The specification word item is still in use" );

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != null )
			{
			if( searchSelectionItem.hasCurrentCreationSentenceNr() )
				{
				if( searchSelectionItem.storeFileItemInFutureDatabase( isCondition, isAction, isAlternative ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a selection item in the database" );
				}

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
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
					return addSelectionResultError( 1, null, "I failed to check if the alternative selection part is duplicate" );
				}
			}
		while( !selectionResult.hasFoundDuplicateSelection &&
		selectionResult.duplicateConditionSentenceNr > Constants.NO_SENTENCE_NR );

		return selectionResult;
		}

	protected SelectionResultType checkDuplicateSelectionPart( int duplicateConditionSentenceNr )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		SelectionItem currentSelectionItem = null;
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		if( duplicateConditionSentenceNr <= Constants.NO_SENTENCE_NR )
			return startSelectionResultError( 1, null, "The given duplicate sentence number is undefined" );

		if( duplicateConditionSentenceNr >= CommonVariables.currentSentenceNr )
			return startSelectionResultError( 1, null, "The given duplicate sentence number is equal or higher than the current sentence number" );

		selectionResult.hasFoundDuplicateSelection = true;

		while( searchSelectionItem != null &&
		searchSelectionItem.activeSentenceNr() >= duplicateConditionSentenceNr )
			{
			if( searchSelectionItem.activeSentenceNr() == duplicateConditionSentenceNr )
				{
				currentSelectionItem = firstActiveSelectionItem();

				do	{
					if( currentSelectionItem.isAction() == searchSelectionItem.isAction() &&
					currentSelectionItem.isAssignedOrClear() == searchSelectionItem.isAssignedOrClear() &&
					currentSelectionItem.isNegative() == searchSelectionItem.isNegative() &&
					currentSelectionItem.isNewStart() == searchSelectionItem.isNewStart() &&
					currentSelectionItem.selectionLevel() == searchSelectionItem.selectionLevel() &&
					currentSelectionItem.generalizationWordItem() == searchSelectionItem.generalizationWordItem() &&
					currentSelectionItem.specificationWordItem() == searchSelectionItem.specificationWordItem() )
						{
						if( currentSelectionItem.specificationString() != null &&
						searchSelectionItem.specificationString() != null )
							selectionResult.hasFoundDuplicateSelection = ( currentSelectionItem.specificationString().equals( searchSelectionItem.specificationString() ) );
						else
							{
							if( currentSelectionItem.specificationString() != null ||
							searchSelectionItem.specificationString() != null )
								selectionResult.hasFoundDuplicateSelection = false;
							}

						if( selectionResult.hasFoundDuplicateSelection )
							{
							currentSelectionItem = ( currentSelectionItem.nextSelectionItem() != null &&
													currentSelectionItem.nextSelectionItem().hasCurrentCreationSentenceNr() ? currentSelectionItem.nextSelectionItem() : null );

							searchSelectionItem = ( searchSelectionItem.nextSelectionItem() != null &&
													searchSelectionItem.nextSelectionItem().activeSentenceNr() == duplicateConditionSentenceNr ? searchSelectionItem.nextSelectionItem() : null );
							}
						else
							searchSelectionItem = null;
						}
					else
						{
						selectionResult.hasFoundDuplicateSelection = false;
						searchSelectionItem = null;
						}
					}
				while( selectionResult.hasFoundDuplicateSelection &&
				currentSelectionItem != null &&
				searchSelectionItem != null );
				}
			else
				searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return selectionResult;
		}

	protected SelectionResultType createSelectionItem( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short imperativeVerbParameter, short prepositionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		if( generalizationWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		generalizationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startSelectionResultError( 1, null, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= Constants.NO_WORD_TYPE_NR &&
		specificationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startSelectionResultError( 1, null, "The given specification word type number is undefined or out of bounds" );

		if( ( selectionResult.lastCreatedSelectionItem = new SelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString, this, myWordItem() ) ) == null )
			return startSelectionResultError( 1, null, "I failed to create a selection item" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, selectionResult.lastCreatedSelectionItem ) != Constants.RESULT_OK )
			return addSelectionResultError( 1, null, "I failed to add an active selection item" );

		return selectionResult;
		}

	protected SelectionResultType findFirstExecutionItem( WordItem solveWordItem )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		SelectionItem firstSelectionItem;

		if( ( firstSelectionItem = firstActiveSelectionItem() ) != null )
			{
			if( firstSelectionItem.findNextExecutionSelectionItem( true, solveWordItem ) != Constants.RESULT_OK )
				return addSelectionResultError( 1, null, "I failed to find the first execution selection item" );

			selectionResult.firstExecutionItem = firstSelectionItem.nextExecutionItem();
			}

		return selectionResult;
		}

	protected SelectionItem executionStartEntry( short executionLevel, int executionSentenceNr )
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != null &&
		searchSelectionItem.activeSentenceNr() >= executionSentenceNr )
			{
			if( searchSelectionItem.activeSentenceNr() == executionSentenceNr &&
			searchSelectionItem.selectionLevel() == executionLevel )
				return searchSelectionItem;

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return null;
		}

	protected SelectionItem firstActiveSelectionItem()
		{
		return (SelectionItem)firstActiveItem();
		}

	protected SelectionItem firstConditionSelectionItem( int conditionSentenceNr )
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != null &&
		searchSelectionItem.activeSentenceNr() >= conditionSentenceNr )
			{
			if( searchSelectionItem.activeSentenceNr() == conditionSentenceNr )
				return searchSelectionItem;

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"Who can be compared with the Lord or God,
 *	who is enthroned on high?" (Psalm 113:5)
 *************************************************************************/
