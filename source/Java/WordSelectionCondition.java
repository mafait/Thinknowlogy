/*	Class:			WordSelectionCondition
 *	Supports class:	AdminItem
 *	Purpose:		To check the condition of selections
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

class WordSelectionCondition
	{
	// Private constructed variables

	private boolean hasFoundScoringAssignment_;
	private boolean isConditionSatisfied_;

	private int oldSatisfiedScore_;
	private int newSatisfiedScore_;
	private int oldDissatisfiedScore_;
	private int newDissatisfiedScore_;
	private int oldNotBlockingScore_;
	private int newNotBlockingScore_;
	private int oldBlockingScore_;
	private int newBlockingScore_;

	private SpecificationItem comparisonAssignmentItem_;
	private SpecificationItem firstComparisonAssignmentItem_;
	private SpecificationItem secondComparisonAssignmentItem_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private boolean hasFoundScoringAssignment( boolean isBlocking )
		{
		SpecificationItem currentAssignmentItem;

		if( ( currentAssignmentItem = myWordItem_.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			do	{
				if( !currentAssignmentItem.isNegative() )
					{
					if( isBlocking )
						{
						if( currentAssignmentItem.isOlderItem() )
							oldBlockingScore_++;
						else
							newBlockingScore_++;
						}
					else
						{
						if( currentAssignmentItem.isOlderItem() )
							oldNotBlockingScore_++;
						else
							newNotBlockingScore_++;
						}

					return true;
					}
				}
			while( ( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
			}

		return false;
		}

	private static boolean isNumeralString( String checkString )
		{
		int stringLength;
		int position = 0;

		if( checkString != null &&
		( stringLength = checkString.length() ) > 0 )
			{
			while( position < stringLength &&
			Character.isDigit( checkString.charAt( position ) ) )
				position++;

			return ( position == stringLength );
			}

		return false;
		}

	private byte getComparisonAssignment( boolean isNumeralRelation, WordItem specificationWordItem, WordItem relationWordItem )
		{
		comparisonAssignmentItem_ = null;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( isNumeralRelation )
			comparisonAssignmentItem_ = specificationWordItem.firstActiveNumeralAssignmentItem();
		else
			{
			if( relationWordItem == null )
				comparisonAssignmentItem_ = specificationWordItem.firstActiveStringAssignmentItem();
			else
				{
				if( specificationWordItem.isNounHead() )
					comparisonAssignmentItem_ = relationWordItem.lastActiveNonQuestionAssignmentItem();
				else
					{
					if( specificationWordItem.isNounTail() )
						comparisonAssignmentItem_ = relationWordItem.firstNonQuestionActiveAssignmentItem();
					else
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte checkComparison( boolean isFirstComparisonPart, boolean isNegative, boolean isNumeralRelation, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		int comparisonResult = 0;
		int firstNumeral = 0;
		int secondNumeral = 0;
		String firstString = null;
		String secondString = null;
		WordItem firstSpecificationWordItem;
		WordItem secondSpecificationWordItem;
		WordItem comparisonAssignmentSpecificationWordItem = null;

		isConditionSatisfied_ = false;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// First part
		if( isFirstComparisonPart )
			{
			if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed get the first comparison assignment" );

			// Allow the second part of the comparison to be checked
			isConditionSatisfied_ = true;

			firstComparisonAssignmentItem_ = comparisonAssignmentItem_;
			secondComparisonAssignmentItem_ = null;
			}
		else
			{
			if( !isNumeralRelation &&
			// Second part
			specificationString == null )
				{
				if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed get a comparison assignment" );

				secondComparisonAssignmentItem_ = comparisonAssignmentItem_;

				firstSpecificationWordItem = ( firstComparisonAssignmentItem_ == null ? null : firstComparisonAssignmentItem_.specificationWordItem() );
				secondSpecificationWordItem = ( secondComparisonAssignmentItem_ == null ? null : secondComparisonAssignmentItem_.specificationWordItem() );

				firstString = ( firstSpecificationWordItem == null ? null : firstSpecificationWordItem.anyWordTypeString() );
				secondString = ( secondSpecificationWordItem == null ? null : secondSpecificationWordItem.anyWordTypeString() );
				}
			else
				{
				// Numeral or specification string
				if( getComparisonAssignment( isNumeralRelation, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed get the first comparison assignment" );

				if( comparisonAssignmentItem_ != null )
					comparisonAssignmentSpecificationWordItem = comparisonAssignmentItem_.specificationWordItem();

				firstString = ( comparisonAssignmentItem_ == null ? null : ( isNumeralRelation ? ( comparisonAssignmentSpecificationWordItem == null ? null : comparisonAssignmentSpecificationWordItem.anyWordTypeString() ) : comparisonAssignmentItem_.specificationString() ) );
				secondString = ( isNumeralRelation ? ( relationWordItem == null ? null : relationWordItem.anyWordTypeString() ) : specificationString );
				}

			if( firstString == null ||
			secondString == null )
				comparisonResult = ( firstString == null && secondString == null ? 0 : ( firstString == null ? -1 : 1 ) );
			else
				{
				if( isNumeralString( firstString ) &&
				isNumeralString( secondString ) )
					{
					firstNumeral = Integer.parseInt( firstString );
					secondNumeral = Integer.parseInt( secondString );

					comparisonResult = ( firstNumeral == secondNumeral ? 0 : ( firstNumeral < secondNumeral ? -1 : 1 ) );
					}
				else
					comparisonResult = firstString.compareTo( secondString );
				}

			if( myWordItem_.isAdjectiveComparisonLess() )
				isConditionSatisfied_ = ( comparisonResult < 0 ? !isNegative : isNegative );
			else
				{
				if( myWordItem_.isAdjectiveComparisonEqual() )
					isConditionSatisfied_ = ( comparisonResult == 0 ? !isNegative : isNegative );
				else
					{
					if( !myWordItem_.isAdjectiveComparisonMore() )
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "Word \"" + myWordItem_.anyWordTypeString() + "\" isn't comparison word" );

					isConditionSatisfied_ = ( comparisonResult > 0 ? !isNegative : isNegative );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte checkForOddOrEven( boolean isNegative, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int nAssignments;

		isConditionSatisfied_ = false;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given relation word item is undefined" );

		if( specificationWordItem.isNounNumber() )
			{
			nAssignments = relationWordItem.nActiveAssignments();

			if( myWordItem_.isAdjectiveOdd() )
				isConditionSatisfied_ = ( nAssignments % 2 == 1 ? !isNegative : isNegative );
			else
				{
				if( !myWordItem_.isAdjectiveEven() )
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "Word \"" + myWordItem_.anyWordTypeString() + "\" isn't about odd or even" );

				isConditionSatisfied_ = ( nAssignments % 2 == 0 ? !isNegative : isNegative );
				}
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	private byte findScoringAssignment( boolean isPossessive, boolean isSatisfiedScore, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem currentAssignmentItem;

		hasFoundScoringAssignment_ = false;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentAssignmentItem = myWordItem_.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			do	{
				if( currentAssignmentItem.isRelatedSpecification( false, isPossessive, relationContextNr, specificationWordItem ) )
					{
					hasFoundScoringAssignment_ = true;

					if( isSatisfiedScore )
						{
						if( currentAssignmentItem.isOlderItem() )
							oldSatisfiedScore_++;
						else
							newSatisfiedScore_++;
						}
					else
						{
						if( currentAssignmentItem.isOlderItem() )
							oldDissatisfiedScore_++;
						else
							newDissatisfiedScore_++;
						}
					}
				}
			while( !hasFoundScoringAssignment_ &&
			( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte checkConditionByValue( boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		boolean isSatisfiedScore;
		SpecificationItem foundAssignmentItem;
		SpecificationItem currentSpecificationItem;

		isConditionSatisfied_ = true;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			do	{
				foundAssignmentItem = specificationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, isPossessive, currentSpecificationItem.relationContextNr(), currentSpecificationItem.specificationWordItem() );
				isSatisfiedScore = ( isNegative == ( foundAssignmentItem == null || foundAssignmentItem.isNegative() ) );

				if( findScoringAssignment( isPossessive, isSatisfiedScore, currentSpecificationItem.relationContextNr(), currentSpecificationItem.specificationWordItem() ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a scoring assignment" );

				if( hasFoundScoringAssignment_ != isSatisfiedScore )
					isConditionSatisfied_ = false;
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected WordSelectionCondition( WordItem myWordItem )
		{
		String errorString = null;

		hasFoundScoringAssignment_ = false;
		isConditionSatisfied_ = false;

		oldSatisfiedScore_ = Constants.NO_SCORE;
		newSatisfiedScore_ = Constants.NO_SCORE;
		oldDissatisfiedScore_ = Constants.NO_SCORE;
		newDissatisfiedScore_ = Constants.NO_SCORE;
		oldNotBlockingScore_ = Constants.NO_SCORE;
		newNotBlockingScore_ = Constants.NO_SCORE;
		oldBlockingScore_ = Constants.NO_SCORE;
		newBlockingScore_ = Constants.NO_SCORE;

		comparisonAssignmentItem_ = null;
		firstComparisonAssignmentItem_ = null;
		secondComparisonAssignmentItem_ = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected SelectionResultType checkSelectionCondition( SelectionItem conditionSelectionItem )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		boolean isNegative;
		WordItem specificationWordItem;

		isConditionSatisfied_ = false;

		oldSatisfiedScore_ = Constants.NO_SCORE;
		newSatisfiedScore_ = Constants.NO_SCORE;
		oldDissatisfiedScore_ = Constants.NO_SCORE;
		newDissatisfiedScore_ = Constants.NO_SCORE;
		oldNotBlockingScore_ = Constants.NO_SCORE;
		newNotBlockingScore_ = Constants.NO_SCORE;
		oldBlockingScore_ = Constants.NO_SCORE;
		newBlockingScore_ = Constants.NO_SCORE;

		if( conditionSelectionItem == null )
			return myWordItem_.startSelectionResultErrorInWord( 1, moduleNameString_, "The given condition selection item is undefined" );

		if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) == null )
			return myWordItem_.startSelectionResultErrorInWord( 1, moduleNameString_, "The given condition selection item has no specification word" );

		isNegative = conditionSelectionItem.isNegative();

		if( conditionSelectionItem.isAssignedOrClear() )
			{
			if( specificationWordItem.isAdjectiveEmpty() )
				// Adjective "clear"
				isConditionSatisfied_ = ( hasFoundScoringAssignment( !isNegative ) ? isNegative : !isNegative );
			else
				// Adjective "assigned"
				isConditionSatisfied_ = ( hasFoundScoringAssignment( isNegative ) ? !isNegative : isNegative );
			}
		else
			{
			if( myWordItem_.isAdjectiveComparison() )
				{
				if( checkComparison( conditionSelectionItem.isFirstComparisonPart(), isNegative, conditionSelectionItem.isNumeralRelation(), specificationWordItem, conditionSelectionItem.relationWordItem(), conditionSelectionItem.specificationString() ) != Constants.RESULT_OK )
					return myWordItem_.addSelectionResultErrorInWord( 1, moduleNameString_, "I failed to check a comparison" );
				}
			else
				{
				if( myWordItem_.isAdjectiveOddOrEven() )
					{
					if( checkForOddOrEven( isNegative, specificationWordItem, conditionSelectionItem.relationWordItem() ) != Constants.RESULT_OK )
						return myWordItem_.addSelectionResultErrorInWord( 1, moduleNameString_, "I failed to check for odd or even" );
					}
				else
					{
					if( conditionSelectionItem.isValueSpecification() )
						{
						if( checkConditionByValue( isNegative, conditionSelectionItem.isPossessive(), specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addSelectionResultErrorInWord( 1, moduleNameString_, "I failed to check the condition of a specification by value" );
						}
					else
						{
						if( findScoringAssignment( conditionSelectionItem.isPossessive(), !isNegative, conditionSelectionItem.relationContextNr(), specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addSelectionResultErrorInWord( 1, moduleNameString_, "I failed to find a scoring assignment" );

						if( hasFoundScoringAssignment_ != isNegative )
							isConditionSatisfied_ = true;
						}
					}
				}
			}

		selectionResult.isConditionSatisfied = isConditionSatisfied_;

		selectionResult.oldSatisfiedScore = oldSatisfiedScore_;
		selectionResult.newSatisfiedScore = newSatisfiedScore_;
		selectionResult.oldDissatisfiedScore = oldDissatisfiedScore_;
		selectionResult.newDissatisfiedScore = newDissatisfiedScore_;
		selectionResult.oldNotBlockingScore = oldNotBlockingScore_;
		selectionResult.newNotBlockingScore = newNotBlockingScore_;
		selectionResult.oldBlockingScore = oldBlockingScore_;
		selectionResult.newBlockingScore = newBlockingScore_;

		return selectionResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *
 *	Praise God in his sanctuary;
 *	praise him in his mighty heaven!" (Psalm 150:1)
 *************************************************************************/
