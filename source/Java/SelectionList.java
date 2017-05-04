/*	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		To store selection items
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
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
		initializeListVariables( _listChar, "SelectionList", myWordItem );
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

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		if( unusedWordItem == null )
			return startError( 1, "The given unused word item is undefined" );

		while( searchSelectionItem != null )
			{
			if( searchSelectionItem.generalizationWordItem() == unusedWordItem )
				return startError( 1, "The generalization word item is still in use" );

			if( searchSelectionItem.specificationWordItem() == unusedWordItem )
				return startError( 1, "The specification word item is still in use" );

			searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createSelectionItem( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short imperativeVerbParameter, short prepositionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		if( generalizationWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		generalizationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= Constants.NO_WORD_TYPE_NR &&
		specificationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given specification word type number is undefined or out of bounds" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new SelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an active selection item" );

		return Constants.RESULT_OK;
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

	protected SelectionItem firstSelectionItem( WordItem solveWordItem )
		{
		SelectionItem firstSelectionItem = firstActiveSelectionItem();

		if( firstSelectionItem != null )
			return firstSelectionItem.firstSelectionItem( solveWordItem );

		return null;
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

	protected DuplicateResultType checkDuplicateCondition()
		{
		DuplicateResultType duplicateResult = new DuplicateResultType();

		duplicateResult.duplicateConditionSentenceNr = CommonVariables.currentSentenceNr;

		do	{
			if( ( duplicateResult.duplicateConditionSentenceNr = getLowerSentenceNr( duplicateResult.duplicateConditionSentenceNr ) ) > Constants.NO_SENTENCE_NR )
				{
				if( ( duplicateResult = checkDuplicateSelectionPart( duplicateResult.duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
					return addDuplicateResultError( 1, "I failed to check if the alternative selection part is duplicate" );
				}
			}
		while( !duplicateResult.hasFoundDuplicateSelection &&
		duplicateResult.duplicateConditionSentenceNr > Constants.NO_SENTENCE_NR );

		return duplicateResult;
		}

	protected DuplicateResultType checkDuplicateSelectionPart( int duplicateConditionSentenceNr )
		{
		DuplicateResultType duplicateResult = new DuplicateResultType();
		SelectionItem currentSelectionItem = null;
		SelectionItem searchSelectionItem = firstActiveSelectionItem();

		if( duplicateConditionSentenceNr <= Constants.NO_SENTENCE_NR )
			return startDuplicateResultError( 1, "The given duplicate sentence number is undefined" );

		if( duplicateConditionSentenceNr >= CommonVariables.currentSentenceNr )
			return startDuplicateResultError( 1, "The given duplicate sentence number is equal or higher than the current sentence number" );

		duplicateResult.hasFoundDuplicateSelection = true;

		while( searchSelectionItem != null &&
		searchSelectionItem.activeSentenceNr() >= duplicateConditionSentenceNr )
			{
			if( searchSelectionItem.activeSentenceNr() == duplicateConditionSentenceNr )
				{
				currentSelectionItem = firstActiveSelectionItem();

				do	{
					if( currentSelectionItem.isAction() == searchSelectionItem.isAction() &&
					currentSelectionItem.isAssignedOrEmpty() == searchSelectionItem.isAssignedOrEmpty() &&
					currentSelectionItem.isNegative() == searchSelectionItem.isNegative() &&
					currentSelectionItem.isNewStart() == searchSelectionItem.isNewStart() &&
					currentSelectionItem.selectionLevel() == searchSelectionItem.selectionLevel() &&
					currentSelectionItem.generalizationWordItem() == searchSelectionItem.generalizationWordItem() &&
					currentSelectionItem.specificationWordItem() == searchSelectionItem.specificationWordItem() )
						{
						if( currentSelectionItem.specificationString() != null &&
						searchSelectionItem.specificationString() != null )
							duplicateResult.hasFoundDuplicateSelection = ( currentSelectionItem.specificationString().equals( searchSelectionItem.specificationString() ) );
						else
							{
							if( currentSelectionItem.specificationString() != null ||
							searchSelectionItem.specificationString() != null )
								duplicateResult.hasFoundDuplicateSelection = false;
							}

						if( duplicateResult.hasFoundDuplicateSelection )
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
						duplicateResult.hasFoundDuplicateSelection = false;
						searchSelectionItem = null;
						}
					}
				while( duplicateResult.hasFoundDuplicateSelection &&
				currentSelectionItem != null &&
				searchSelectionItem != null );
				}
			else
				searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return duplicateResult;
		}
	};

/*************************************************************************
 *	"Who can be compared with the Lord or God,
 *	who is enthroned on high?" (Psalm 113:5)
 *************************************************************************/
