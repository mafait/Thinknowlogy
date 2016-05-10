/*	Class:			ScoreItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store scoring info during
 *					solving (= assigning) words according the selections
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

class ScoreItem extends Item
	{
	// Private loadable variables

	private short assignmentLevel_;


	protected boolean isMarked;
	protected boolean isChecked;

	protected int oldSatisfiedScore;
	protected int newSatisfiedScore;
	protected int oldDissatisfiedScore;
	protected int newDissatisfiedScore;
	protected int oldNotBlockingScore;
	protected int newNotBlockingScore;
	protected int oldBlockingScore;
	protected int newBlockingScore;

	protected SelectionItem referenceSelectionItem;


	// Private methods

	private ScoreItem possibilityItem( boolean isIncludingThisItem )
		{
		short currentAssignmentLevel = CommonVariables.currentAssignmentLevel;
		ScoreItem searchItem = ( isIncludingThisItem ? this : nextScoreItem() );

		while( searchItem != null &&
		searchItem.assignmentLevel_ >= currentAssignmentLevel )
			{
			if( searchItem.assignmentLevel_ == currentAssignmentLevel )
				return searchItem;

			searchItem = searchItem.nextScoreItem();
			}

		return null;
		}


	// Constructor / deconstructor

	protected ScoreItem( boolean _isChecked, short assignmentLevel, int _oldSatisfiedScore, int _newSatisfiedScore, int _oldDissatisfiedScore, int _newDissatisfiedScore, int _oldNotBlockingScore, int _newNotBlockingScore, int _oldBlockingScore, int _newBlockingScore, SelectionItem _selectionReference, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		assignmentLevel_ = assignmentLevel;

		// Protected loadable variables

		isMarked = false;
		isChecked = _isChecked;

		oldSatisfiedScore = _oldSatisfiedScore;
		newSatisfiedScore = _newSatisfiedScore;
		oldDissatisfiedScore = _oldDissatisfiedScore;
		newDissatisfiedScore = _newDissatisfiedScore;
		oldNotBlockingScore = _oldNotBlockingScore;
		newNotBlockingScore = _newNotBlockingScore;
		oldBlockingScore = _oldBlockingScore;
		newBlockingScore = _newBlockingScore;

		referenceSelectionItem = _selectionReference;
		}


	// Protected virtual methods

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( assignmentLevel_ == queryParameter ||
				oldSatisfiedScore == queryParameter ||
				newSatisfiedScore == queryParameter ||
				oldDissatisfiedScore == queryParameter ||
				newDissatisfiedScore == queryParameter ||
				oldNotBlockingScore == queryParameter ||
				newNotBlockingScore == queryParameter ||
				oldBlockingScore == queryParameter ||
				newBlockingScore == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( assignmentLevel_ > Constants.NO_ASSIGNMENT_LEVEL ||
				oldSatisfiedScore > Constants.NO_SCORE ||
				newSatisfiedScore > Constants.NO_SCORE ||
				oldDissatisfiedScore > Constants.NO_SCORE ||
				newDissatisfiedScore > Constants.NO_SCORE ||
				oldNotBlockingScore > Constants.NO_SCORE ||
				newNotBlockingScore > Constants.NO_SCORE ||
				oldBlockingScore > Constants.NO_SCORE ||
				newBlockingScore > Constants.NO_SCORE ) ) );
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( referenceSelectionItem == null ? false :
				( querySentenceNr == Constants.NO_SENTENCE_NR ? true : referenceSelectionItem.creationSentenceNr() == querySentenceNr ) &&
				( queryItemNr == Constants.NO_ITEM_NR ? true : referenceSelectionItem.itemNr() == queryItemNr ) );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		return ( nextSortItem != null &&
				// Descending assignmentLevel_
				assignmentLevel_ > ( (ScoreItem)nextSortItem ).assignmentLevel_ );
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		baseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( isMarked )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "Marked" );

		if( isChecked )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "Checked" );

		if( oldSatisfiedScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "oldSatisfiedScore:" + oldSatisfiedScore );

		if( newSatisfiedScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "newSatisfiedScore:" + newSatisfiedScore );

		if( oldDissatisfiedScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "oldDissatisfiedScore:" + oldDissatisfiedScore );

		if( newDissatisfiedScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "newDissatisfiedScore:" + newDissatisfiedScore );

		if( oldNotBlockingScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "oldNotBlockingScore:" + oldNotBlockingScore );

		if( newNotBlockingScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "newNotBlockingScore:" + newNotBlockingScore );

		if( oldBlockingScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "oldBlockingScore:" + oldBlockingScore );

		if( newBlockingScore > Constants.NO_SCORE )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "newBlockingScore:" + newBlockingScore );

		if( referenceSelectionItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "referenceSelectionItem" + Constants.QUERY_REF_ITEM_START_CHAR + referenceSelectionItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + referenceSelectionItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected boolean hasOldSatisfiedScore()
		{
		return ( oldSatisfiedScore > Constants.NO_SCORE );
		}

	protected boolean hasNewSatisfiedScore()
		{
		return ( newSatisfiedScore > Constants.NO_SCORE );
		}

	protected boolean hasOldDissatisfiedScore()
		{
		return ( oldDissatisfiedScore > Constants.NO_SCORE );
		}

	protected boolean hasNewDissatisfiedScore()
		{
		return ( newDissatisfiedScore > Constants.NO_SCORE );
		}

	protected boolean hasOldNotBlockingScore()
		{
		return ( oldNotBlockingScore > Constants.NO_SCORE );
		}

	protected boolean hasNewNotBlockingScore()
		{
		return ( newNotBlockingScore > Constants.NO_SCORE );
		}

	protected boolean hasOldBlockingScore()
		{
		return ( oldBlockingScore > Constants.NO_SCORE );
		}

	protected boolean hasNewBlockingScore()
		{
		return ( newBlockingScore > Constants.NO_SCORE );
		}

	protected short assignmentLevel()
		{
		return assignmentLevel_;
		}

	protected ScoreItem firstPossibilityItem()
		{
		return possibilityItem( true );
		}

	protected ScoreItem nextPossibilityItem()
		{
		return possibilityItem( false );
		}

	protected ScoreItem nextScoreItem()
		{
		return (ScoreItem)nextItem;
		}

	protected SelectionItem scoreReference()
		{
		return referenceSelectionItem;
		}
	};

/*************************************************************************
 *	"The Lord rules over the floodwaters.
 *	The Lord reigns as king forever." (Psalm 29:10)
 *************************************************************************/
