/*	Class:			SelectionItem
 *	Parent class:	Item
 *	Purpose:		To store the selection structure
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

class SelectionItem extends Item
	{
	// Private constructed variables

	private SelectionItem nextExecutionItem_;


	// Private initialized variables

	private boolean isAction_;
	private boolean isAssignedOrClear_;
	private boolean isInactive_;
	private boolean isArchived_;
	private boolean isFirstComparisonPart_;
	private boolean isNewStart_;
	private boolean isNegative_;
	private boolean isPossessive_;
	private boolean isSpecificationGeneralization_;
	private boolean isUniqueUserRelation_;
	private boolean isValueSpecification_;

	private short assumptionLevel_;
	private short selectionLevel_;
	private short imperativeVerbParameter_;
	private short prepositionParameter_;

	private short generalizationWordTypeNr_;
	private short specificationWordTypeNr_;
	private short relationWordTypeNr_;

	private int generalizationContextNr_;
	private int specificationContextNr_;
	private int relationContextNr_;

	private int nContextRelations_;

	private WordItem generalizationWordItem_;
	private WordItem specificationWordItem_;
	private WordItem relationWordItem_;

	private String specificationString_;


	// Protected constructed variables

	protected boolean isConditionCheckedForSolving;


	// Constructor

	protected SelectionItem( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short imperativeVerbParameter, short prepositionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private constructed variables

		nextExecutionItem_ = null;

		// Private initialized variables

		isAction_ = isAction;
		isAssignedOrClear_ = isAssignedOrClear;
		isNewStart_ = isNewStart;

		isInactive_ = isInactiveAssignment;
		isArchived_ = isArchivedAssignment;

		isFirstComparisonPart_ = isFirstComparisonPart;

		isNegative_ = isNegative;
		isPossessive_ = isPossessive;
		isSpecificationGeneralization_ = isSpecificationGeneralization;
		isUniqueUserRelation_ = isUniqueUserRelation;
		isValueSpecification_ = isValueSpecification;

		assumptionLevel_ = assumptionLevel;
		selectionLevel_ = selectionLevel;

		imperativeVerbParameter_ = imperativeVerbParameter;
		prepositionParameter_ = prepositionParameter;

		generalizationWordTypeNr_ = generalizationWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;
		relationWordTypeNr_ = relationWordTypeNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nContextRelations_ = nContextRelations;

		specificationString_ = ( specificationString == null ? null : specificationString );

		generalizationWordItem_ = generalizationWordItem;
		specificationWordItem_ = specificationWordItem;
		relationWordItem_ = relationWordItem;


		// Protected constructed variables

		isConditionCheckedForSolving = false;
		}


	// Protected virtual methods

	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( specificationString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( specificationString_ );
			}
		}

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		StringBuffer queryStringBuffer;
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( generalizationWordItem_ != null &&
		( wordString = generalizationWordItem_.wordTypeString( true, generalizationWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			queryStringBuffer.append( wordString );
			}

		if( specificationWordItem_ != null &&
		( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery ||
			queryStringBuffer.length() > 0 )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			queryStringBuffer.append( wordString );
			}

		if( relationWordItem_ != null &&
		( wordString = relationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery ||
			queryStringBuffer.length() > 0 )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( selectionLevel_ == queryParameter ||
				imperativeVerbParameter_ == queryParameter ||
				prepositionParameter_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nContextRelations_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( selectionLevel_ > Constants.NO_SELECTION_LEVEL ||
				imperativeVerbParameter_ > Constants.NO_IMPERATIVE_PARAMETER ||
				prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER ||
				generalizationContextNr_ > Constants.NO_CONTEXT_NR ||
				specificationContextNr_ > Constants.NO_CONTEXT_NR ||
				relationContextNr_ > Constants.NO_CONTEXT_NR ||
				nContextRelations_ > 0 ) ) );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( generalizationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : generalizationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : generalizationWordItem_.itemNr() == queryItemNr ) ) ||

				( specificationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : specificationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : specificationWordItem_.itemNr() == queryItemNr ) ) ||

				( relationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : relationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : relationWordItem_.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( generalizationWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr ||
				relationWordTypeNr_ == queryWordTypeNr );
		}

	protected byte checkForUsage()
		{
		return myWordItem().checkSelectionForUsageInWord( this );
		}

	protected StringResultType findMatchingWordReferenceString( String queryString )
		{
		StringResultType stringResult = new StringResultType();

		if( generalizationWordItem_ != null )
			{
			if( ( stringResult = generalizationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				return addStringResultError( 1, null, specificationString_, "I failed to find a matching word reference string for the generalization word" );
			}

		if( !stringResult.hasFoundMatchingStrings &&
		specificationWordItem_ != null )
			{
			if( ( stringResult = specificationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				return addStringResultError( 1, null, specificationString_, "I failed to find a matching word reference string for the specification word" );
			}

		if( !stringResult.hasFoundMatchingStrings &&
		relationWordItem_ != null )
			{
			if( ( stringResult = relationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				return addStringResultError( 1, null, specificationString_, "I failed to find a matching word reference string for the relation word" );
			}

		return stringResult;
		}

	protected String itemString()
		{
		return specificationString_;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String wordString;
		String generalizationWordTypeString = myWordItem().wordTypeNameString( generalizationWordTypeNr_ );
		String specificationWordTypeString = myWordItem().wordTypeNameString( specificationWordTypeNr_ );
		String relationWordTypeString = myWordItem().wordTypeNameString( relationWordTypeNr_ );

		baseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( isAction_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAction" );

		if( isAssignedOrClear_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAssignedOrClear" );

		if( isNewStart_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNewStart" );

		if( isInactive_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isInactiveAssignment" );

		if( isArchived_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isArchivedAssignment" );

		if( isFirstComparisonPart_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFirstComparisonPart" );

		if( isNegative_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegative" );

		if( isPossessive_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessive" );

		if( isSpecificationGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralization" );

		if( isUniqueUserRelation_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueUserRelation" );

		if( isValueSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isValueSpecification" );

		if( isConditionCheckedForSolving )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isConditionCheckedForSolving" );

		if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assumptionLevel:" + assumptionLevel_ );

		if( selectionLevel_ > Constants.NO_SELECTION_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "selectionLevel:" + selectionLevel_ );

		if( imperativeVerbParameter_ > Constants.NO_IMPERATIVE_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "imperativeVerbParameter:" + imperativeVerbParameter_ );

		if( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "prepositionParameter:" + prepositionParameter_ );

		if( generalizationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationContextNr:" + generalizationContextNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordType:" + ( generalizationWordTypeString == null ? Constants.EMPTY_STRING : generalizationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + generalizationWordTypeNr_ );

		if( generalizationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + generalizationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + generalizationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = generalizationWordItem_.wordTypeString( true, generalizationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( specificationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationContextNr:" + specificationContextNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordType:" + ( specificationWordTypeString == null ? Constants.EMPTY_STRING : specificationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + specificationWordTypeNr_ );

		if( specificationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + specificationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + specificationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( relationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationContextNr:" + relationContextNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordType:" + ( relationWordTypeString == null ? Constants.EMPTY_STRING : relationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + relationWordTypeNr_ );

		if( relationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + relationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + relationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = relationWordItem_.wordTypeString( true, relationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( nContextRelations_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nContextRelations:" + nContextRelations_ );

		if( specificationString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationString:" + Constants.QUERY_STRING_START_CHAR + specificationString_ + Constants.QUERY_STRING_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected boolean isNumeralRelation()
		{
		return ( relationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isAction()
		{
		return isAction_;
		}

	protected boolean isAssignedOrClear()
		{
		return isAssignedOrClear_;
		}

	protected boolean isNewStart()
		{
		return isNewStart_;
		}

	protected boolean isInactiveAssignment()
		{
		return isInactive_;
		}

	protected boolean isArchivedAssignment()
		{
		return isArchived_;
		}

	protected boolean isFirstComparisonPart()
		{
		return isFirstComparisonPart_;
		}

	protected boolean isNegative()
		{
		return isNegative_;
		}

	protected boolean isPossessive()
		{
		return isPossessive_;
		}

	protected boolean isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	protected boolean isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	protected boolean isValueSpecification()
		{
		return isValueSpecification_;
		}

	protected short assumptionLevel()
		{
		return assumptionLevel_;
		}

	protected short selectionLevel()
		{
		return selectionLevel_;
		}

	protected short imperativeVerbParameter()
		{
		return imperativeVerbParameter_;
		}

	protected short prepositionParameter()
		{
		return prepositionParameter_;
		}

	protected short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	protected short relationWordTypeNr()
		{
		return relationWordTypeNr_;
		}

	protected int generalizationContextNr()
		{
		return generalizationContextNr_;
		}

	protected int specificationContextNr()
		{
		return specificationContextNr_;
		}

	protected int relationContextNr()
		{
		return relationContextNr_;
		}

	protected int nContextRelations()
		{
		return nContextRelations_;
		}

	protected byte findNextExecutionSelectionItem( WordItem solveGeneralizationWordItem )
		{
		return findNextExecutionSelectionItem( false, solveGeneralizationWordItem );
		}

	protected byte findNextExecutionSelectionItem( boolean isIncludingThisItem, WordItem solveWordItem )
		{
		SelectionItem searchSelectionItem = ( isIncludingThisItem ? this : nextSelectionItem() );

		nextExecutionItem_ = null;

		if( solveWordItem == null )
			return startError( 1, null, specificationString_, "The given solve word item is undefined" );

		while( searchSelectionItem != null &&
		nextExecutionItem_ == null )
			{
			if( searchSelectionItem.generalizationWordItem_ == solveWordItem &&
			searchSelectionItem.specificationWordItem_ != null )
				nextExecutionItem_ = searchSelectionItem;
			else
				searchSelectionItem = searchSelectionItem.nextSelectionItem();
			}

		return Constants.RESULT_OK;
		}

	protected String specificationString()
		{
		return specificationString_;
		}

	protected SelectionItem nextExecutionItem()
		{
		return nextExecutionItem_;
		}

	protected SelectionItem nextSelectionItem()
		{
		return (SelectionItem)nextItem;
		}

	protected SelectionItem nextConditionItem( short executionLevel, int conditionSentenceNr )
		{
		if( nextSelectionItem() != null &&
		nextSelectionItem().selectionLevel_ <= executionLevel &&
		nextSelectionItem().creationSentenceNr() == conditionSentenceNr )
			return nextSelectionItem();

		return null;
		}

	protected SelectionItem nextExecutionItem( short executionLevel, int executionSentenceNr )
		{
		if( nextSelectionItem() != null &&
		nextSelectionItem().selectionLevel_ == executionLevel &&
		nextSelectionItem().creationSentenceNr() == executionSentenceNr )
			return nextSelectionItem();

		return null;
		}

	protected WordItem generalizationWordItem()
		{
		return generalizationWordItem_;
		}

	protected WordItem specificationWordItem()
		{
		return specificationWordItem_;
		}

	protected WordItem relationWordItem()
		{
		return relationWordItem_;
		}
	};

/*************************************************************************
 *	"Light shines in the darkness for the godly.
 *	They are generous, compassionate, and righteous." (Psalm 112:4)
 *************************************************************************/
