/*	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
 *	Version:		Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

class SpecificationItem extends Item
	{
	// Private initialized variables

	private boolean isAnsweredQuestion_ = false;
	private boolean isCharacteristicFor_ = false;
	private boolean isConcludedAssumption_ = false;
	private boolean isConditional_ = false;
	private boolean isCorrectedAssumption_ = false;
	private boolean isEveryGeneralization_ = false;
	private boolean isExclusiveGeneralization_ = false;
	private boolean isExclusiveSpecification_ = false;
	private boolean isLanguageWord_ = false;
	private boolean isNegative_ = false;
	private boolean isPartOf_ = false;
	private boolean isPossessive_ = false;
	private boolean isSpecific_ = false;
	private boolean isSpecificationGeneralization_ = false;
	private boolean isUncountableGeneralizationNoun_ = false;
	private boolean isUniqueUserRelation_ = false;
	private boolean isValueSpecification_ = false;

	private short assignmentLevel_ = Constants.NO_ASSIGNMENT_LEVEL;
	private short assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
	private short languageNr_ = Constants.NO_LANGUAGE_NR;
	private short prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
	private short questionParameter_ = Constants.NO_QUESTION_PARAMETER;

	private short generalizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
	private short specificationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
	private short relationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private int generalizationCollectionNr_ = Constants.NO_COLLECTION_NR;
	private int specificationCollectionNr_ = Constants.NO_COLLECTION_NR;

	private int generalizationContextNr_ = Constants.NO_CONTEXT_NR;
	private int specificationContextNr_ = Constants.NO_CONTEXT_NR;
	private int relationContextNr_ = Constants.NO_CONTEXT_NR;

	private int nContextRelations_ = 0;

	private JustificationItem firstJustificationItem_ = null;

	private WordItem specificationWordItem_ = null;

	private String specificationString_ = null;

	private StringBuffer storedSentenceStringBuffer_ = new StringBuffer();
	private StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer_ = new StringBuffer();

	// Private constructed variables

	private boolean hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = false;
	private boolean hasSpecificationBeenWrittenAsConflict_ = false;

	private short specificationStringWriteLevel_ = Constants.NO_WRITE_LEVEL;


	// Private assignment methods

	private SpecificationItem nextAssignmentItemWithAssignmentLevel( short currentAssignmentLevel )
		{
		SpecificationItem nextAssignmentItem;

		return ( ( nextAssignmentItem = nextSpecificationItem() ) != null &&
				nextAssignmentItem.assignmentLevel() == currentAssignmentLevel ? nextAssignmentItem : null );
		}


	// Private specification methods

	private boolean hasContextMaleWordInContextWords()
		{
		WordItem currentContextWordItem;
		WordItem thisWordItem = myWordItem();

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		specificationWordItem_ != null &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != thisWordItem &&
				currentContextWordItem.isMale() &&
				currentContextWordItem.hasContextInWord( relationContextNr_, specificationWordItem_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return false;
		}

	private boolean hasJustificationOfSameType( short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		int primarySpecificationCollectionNr = ( primarySpecificationItem == null ? Constants.NO_COLLECTION_NR : primarySpecificationItem.specificationCollectionNr() );
		int secondarySpecificationCollectionNr = ( secondarySpecificationItem == null ? Constants.NO_COLLECTION_NR : secondarySpecificationItem.specificationCollectionNr() );
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr &&
			searchJustificationItem.secondarySpecificationCollectionNr() == secondarySpecificationCollectionNr )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	private boolean hasUserSpecificationWordItem()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isUserSpecificationWord );
		}

	private boolean isPrimarySpecificationCandidateOfQuestion( boolean isAllowingSpanishPossessiveSpecification )
		{
		return ( !isExclusiveSpecification_ &&
				!isNegative_ &&
				!isPartOf_ &&

				( !isPossessive_ ||
				isAllowingSpanishPossessiveSpecification ) &&

				questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
				!isHiddenSpanishSpecification() );
		}

	private short highestAttachedJustificationOrderNr( short justificationTypeNr )
		{
		short highestOrderNr = Constants.NO_ORDER_NR;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.orderNr > highestOrderNr &&
			searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem.orderNr;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return highestOrderNr;
		}

	private byte recalculateAssumptionsInContextWords( int contextNr, WordItem specificationWordItem )
		{
		WordItem currentContextWordItem;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			do	{
				if( currentContextWordItem.hasContextInWord( contextNr, specificationWordItem ) &&
				currentContextWordItem.recalculateAssumptionsInWord() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to recalculate the assumptions of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private WordItem relationContextWordItemInContextWords( WordItem previousContextWordItem )
		{
		WordItem currentContextWordItem;

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		// Do for all (remaining) context words
		( currentContextWordItem = ( previousContextWordItem == null ? GlobalVariables.firstContextWordItem : previousContextWordItem.nextContextWordItem ) ) != null )
			{
			do	{
				if( currentContextWordItem.hasContextInWord( relationContextNr_, specificationWordItem_ ) )
					return currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return null;
		}


	// Protected constructed variables

	protected boolean hasSpecificationBeenWrittenAsAnswer = false;

	protected SpecificationItem replacingSpecificationItem = null;


	// Constructor

	protected SpecificationItem( boolean isAnsweredQuestion, boolean isCharacteristicFor, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isLanguageWord, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer storedSentenceStringBuffer, StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer, List myList, WordItem myWordItem )
		{
		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, myList, myWordItem );

		// Private initialized variables

		isAnsweredQuestion_ = isAnsweredQuestion;
		isCharacteristicFor_ = isCharacteristicFor;
		isConcludedAssumption_ = isConcludedAssumption;
		isConditional_ = isConditional;
		isCorrectedAssumption_ = isCorrectedAssumption;
		isEveryGeneralization_ = isEveryGeneralization;
		isExclusiveGeneralization_ = isExclusiveGeneralization;
		isExclusiveSpecification_ = isExclusiveSpecification;
		isLanguageWord_ = isLanguageWord;
		isNegative_ = isNegative;
		isPartOf_ = isPartOf;
		isPossessive_ = isPossessive;
		isSpecific_ = isSpecific;
		isSpecificationGeneralization_ = isSpecificationGeneralization;
		isUncountableGeneralizationNoun_ = isUncountableGeneralizationNoun;
		isUniqueUserRelation_ = isUniqueUserRelation;
		isValueSpecification_ = isValueSpecification;

		assignmentLevel_ = assignmentLevel;
		assumptionLevel_ = assumptionLevel;
		languageNr_ = languageNr;
		prepositionParameter_ = prepositionParameter;
		questionParameter_ = questionParameter;

		generalizationWordTypeNr_ = generalizationWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;
		relationWordTypeNr_ = relationWordTypeNr;

		generalizationCollectionNr_ = generalizationCollectionNr;
		specificationCollectionNr_ = specificationCollectionNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nContextRelations_ = nContextRelations;

		firstJustificationItem_ = firstJustificationItem;

		specificationWordItem_ = specificationWordItem;

		specificationString_ = specificationString;


		// Private constructed variables

		storedSentenceStringBuffer_ = storedSentenceStringBuffer;
		storedSentenceWithOnlyOneSpecificationStringBuffer_ = storedSentenceWithOnlyOneSpecificationStringBuffer;
		}


	// Protected virtual methods

	protected void checkForUsage()
		{
		myWordItem().checkSpecificationForUsageOfInvolvedWords( this );
		}

	protected void selectingAttachedJustifications( boolean isSelectingJustificationSpecifications )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			searchJustificationItem.isSelectedByQuery = true;

			if( isSelectingJustificationSpecifications )
				searchJustificationItem.selectingJustificationSpecifications();

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}
		}

	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( specificationString_ != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( specificationString_ );
			}
		}

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( specificationWordItem_ != null &&
		( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( prepositionParameter_ == queryParameter ||
				questionParameter_ == queryParameter ||
				generalizationCollectionNr_ == queryParameter ||
				specificationCollectionNr_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nContextRelations_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER ||
				questionParameter_ > Constants.NO_QUESTION_PARAMETER ||
				generalizationCollectionNr_ > Constants.NO_COLLECTION_NR ||
				specificationCollectionNr_ > Constants.NO_COLLECTION_NR ||
				generalizationContextNr_ > Constants.NO_CONTEXT_NR ||
				specificationContextNr_ > Constants.NO_CONTEXT_NR ||
				relationContextNr_ > Constants.NO_CONTEXT_NR ||
				nContextRelations_ > 0 ) ) );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( firstJustificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : firstJustificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : firstJustificationItem_.itemNr() == queryItemNr ) ) ||

				( specificationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : specificationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : specificationWordItem_.itemNr() == queryItemNr ) ) ||

				( replacingSpecificationItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : replacingSpecificationItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : replacingSpecificationItem.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( generalizationWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr	||
				relationWordTypeNr_ == queryWordTypeNr	);
		}

	protected boolean isSorted( Item nextSortItem )
		{
		SpecificationItem nextSortSpecificationItem = (SpecificationItem)nextSortItem;

		return ( nextSortItem != null &&

				// 1) Assignment needs descending assignmentLevel_
				( assignmentLevel_ > nextSortSpecificationItem.assignmentLevel_ ||

				// 2) Question and specification needs descending creationSentenceNr()
				( assignmentLevel_ == nextSortSpecificationItem.assignmentLevel_ &&
				creationSentenceNr() > nextSortSpecificationItem.creationSentenceNr() ) ) );
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		WordItem thisWordItem = myWordItem();
		String languageNameString = thisWordItem.languageNameString( languageNr_ );
		String generalizationWordTypeString = thisWordItem.wordTypeNameString( generalizationWordTypeNr_ );
		StringBuffer queryStringBuffer;
		String relationWordTypeString = ( relationWordTypeNr_ == Constants.NO_WORD_TYPE_NR ? null : thisWordItem.wordTypeNameString( relationWordTypeNr_ ) );
		String specificationWordTypeString = thisWordItem.wordTypeNameString( specificationWordTypeNr_ );
		String wordString;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( languageNr_ > Constants.NO_LANGUAGE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + ( languageNameString == null ? ( "languageNr:" + languageNr_ ) : ( "language:" + languageNameString ) ) );

		if( isAnsweredQuestion_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAnsweredQuestion" );

		if( isCharacteristicFor_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isCharacteristicFor" );

		if( isConcludedAssumption_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isConcludedAssumption" );

		if( isConditional_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isConditional" );

		if( isCorrectedAssumption_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isCorrectedAssumption" );

		if( isEveryGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isEveryGeneralization" );

		if( isExclusiveGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveGeneralization" );

		if( isExclusiveSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecification" );

		if( isLanguageWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isNegative_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegative" );

		if( isPartOf_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPartOf" );

		if( isPossessive_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessive" );

		if( isSpecific_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecific" );

		if( isSpecificationGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralization" );

		if( isUncountableGeneralizationNoun_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUncountableGeneralizationNoun" );

		if( isUniqueUserRelation_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueUserRelation" );

		if( isValueSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isValueSpecification" );

		if( assignmentLevel_ > Constants.NO_ASSIGNMENT_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assignmentLevel:" + assignmentLevel_ );

		if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assumptionLevel:" + assumptionLevel_ );

		if( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationStringWriteLevel:" + specificationStringWriteLevel_ );

		if( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "prepositionParameter:" + prepositionParameter_ );

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "questionParameter:" + questionParameter_ );

		if( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationCollectionNr:" + generalizationCollectionNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordType:" + ( generalizationWordTypeString == null ? Constants.EMPTY_STRING : generalizationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + generalizationWordTypeNr_ );

		if( generalizationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationContextNr:" + generalizationContextNr_ );

		if( specificationCollectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationCollectionNr:" + specificationCollectionNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordType:" + ( specificationWordTypeString == null ? Constants.EMPTY_STRING : specificationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + specificationWordTypeNr_ );

		if( specificationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationContextNr:" + specificationContextNr_ );

		if( specificationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + specificationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + specificationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( relationWordTypeString != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordType:" + relationWordTypeString + Constants.QUERY_WORD_TYPE_STRING + relationWordTypeNr_ );

		if( relationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationContextNr:" + relationContextNr_ );

		if( nContextRelations_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nContextRelations:" + nContextRelations_ );

		if( firstJustificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "firstJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + firstJustificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + firstJustificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( replacingSpecificationItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacingSpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + replacingSpecificationItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + replacingSpecificationItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( specificationString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationString:" + Constants.QUERY_STRING_START_CHAR + specificationString_ + Constants.QUERY_STRING_END_CHAR );

		if( storedSentenceStringBuffer_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "storedSentenceStringBuffer:" + Constants.QUERY_STRING_START_CHAR + storedSentenceStringBuffer_ + Constants.QUERY_STRING_END_CHAR );

		if( storedSentenceWithOnlyOneSpecificationStringBuffer_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "storedSentenceWithOnlyOneSpecificationStringBuffer:" + Constants.QUERY_STRING_START_CHAR + storedSentenceWithOnlyOneSpecificationStringBuffer_ + Constants.QUERY_STRING_END_CHAR );

		return queryStringBuffer;
		}

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		if( specificationWordItem_ != null )
			return specificationWordItem_.findMatchingWordReferenceString( queryString );

		return new BoolResultType();
		}


	// Protected assignment methods

	protected boolean isAssignment()
		{
		return myList().isAssignmentList();
		}

	protected boolean isUserAssignment()
		{
		return ( isAssignment() &&
				isUserSpecification() );
		}

	protected boolean isActiveAssignment()
		{
		return ( isAssignment() &&
				isActiveItem() );
		}

	protected boolean isInactiveAssignment()
		{
		return ( isAssignment() &&
				isInactiveItem() );
		}

	protected boolean isArchivedAssignment()
		{
		return ( isAssignment() &&
				isArchivedItem() );
		}

	protected short assignmentLevel()
		{
		return assignmentLevel_;
		}

	protected short assumptionLevel()
		{
		return assumptionLevel_;
		}

	protected short languageNr()
		{
		return languageNr_;
		}

	protected int nRelationContextWords()
		{
		return myWordItem().nContextWords( relationContextNr_, specificationWordItem_ );
		}

	protected SpecificationItem assignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == currentAssignmentLevel ? this : null ) : nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isQuestion() == isQuestion &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel );
			}

		return null;
		}

	protected SpecificationItem assignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == currentAssignmentLevel ? this : null ) : nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.questionParameter_ == questionParameter &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel );
			}

		return null;
		}


	// Protected question methods

	protected boolean isAnsweredQuestion()
		{
		return isAnsweredQuestion_;
		}

	protected boolean isQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER );
		}

	protected boolean isUserQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == null );
		}

	protected short questionParameter()
		{
		return questionParameter_;
		}


	// Protected specification methods

	protected void clearReplacingInfo()
		{
		if( hasCurrentReplacedSentenceNr() ||

		( replacingSpecificationItem != null &&
		replacingSpecificationItem.hasCurrentOrNewerCreationSentenceNr() ) )
			{
			replacingSpecificationItem = null;
			clearReplacedSentenceNr();
			}
		}

	protected void clearStoredSentenceStringBuffer()
		{
		storedSentenceStringBuffer_ = null;
		}

	protected void initializeSpecificationVariables()
		{
		// Private constructed variables

		hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = false;
		hasSpecificationBeenWrittenAsConflict_ = false;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;
		}

	protected void storeWrittenSentenceStringBuffer()
		{
		storedSentenceStringBuffer_ = GlobalVariables.writtenSentenceStringBuffer;
		}

	protected void storedWrittenSentenceStringBufferWithOneSpecificationOnly()
		{
		storedSentenceWithOnlyOneSpecificationStringBuffer_ = GlobalVariables.writtenSentenceStringBuffer;
		}

	protected boolean hasAssumptionLevel()
		{
		return ( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL );
		}

	protected boolean hasCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isCompoundCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasJustification( JustificationItem referenceJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( referenceJustificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem == referenceJustificationItem )
					return true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasNewInformation()
		{
		return ( !isOlderItem() ||
				// Has been inactivated during this sentence
				hasCurrentInactiveSentenceNr() ||
				// Has been archived during this sentence
				hasCurrentArchivedSentenceNr() ||

				( relationContextNr_ > Constants.NO_CONTEXT_NR &&
				hasRelationContextCurrentlyBeenUpdated() ) );
		}

	protected boolean hasNonCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isNonCompoundCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasNonExclusiveSpecificationCollection()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isNonExclusiveCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasOnlyOneRelationWord()
		{
		return ( myWordItem().nContextWords( relationContextNr_, specificationWordItem_ ) == 1 );
		}

	protected boolean hasPossessivePrimarySpecificationJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( !searchJustificationItem.hasPossessivePrimarySpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasPrimarySpecificationJustification( SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem )
					return true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean hasSpecificationContext()
		{
		return ( specificationContextNr_ > Constants.NO_CONTEXT_NR );
		}

	protected boolean hasRelationContext()
		{
		return ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		}

	protected boolean hasRelationContextCurrentlyBeenUpdated()
		{
		WordItem currentContextWordItem;
		WordItem thisWordItem = myWordItem();

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != thisWordItem &&
				currentContextWordItem.hasContextCurrentlyBeenUpdatedInWord( relationContextNr_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return false;
		}

	protected boolean hasSpecificationBeenWrittenAsAdjustedNegativeAssumption()
		{
		return hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_;
		}

	protected boolean hasSpecificationBeenWrittenAsConflict()
		{
		return hasSpecificationBeenWrittenAsConflict_;
		}

	protected boolean hasSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean isAnsweredSelfGeneratedQuestion()
		{
		return ( isAnsweredQuestion_ &&
				firstJustificationItem_ != null );
		}

	protected boolean isCharacteristicFor()
		{
		return isCharacteristicFor_;
		}

	protected boolean isConcludedAssumption()
		{
		return isConcludedAssumption_;
		}

	protected boolean isConditional()
		{
		return isConditional_;
		}

	protected boolean isCorrectedAssumption()
		{
		return isCorrectedAssumption_;
		}

	protected boolean isCorrectSpecificationArticle( boolean isDefiniteArticle, short articleParameter )
		{
		boolean isSelfGeneratedSpecification;
		SpecificationItem searchSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem foundWordItem = null;

		if( ( searchSpecificationItem = myWordItem().firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != null )
			{
			isSelfGeneratedSpecification = isSelfGenerated();

			do	{
				if( ( currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ : searchSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, relationContextNr_ ) ) ) != null &&
				!currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
					foundWordItem = currentSpecificationWordItem;
				}
			while( foundWordItem == null &&
			( searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion_ ) ) != null );

			return ( foundWordItem == null ? true : ( isDefiniteArticle ? foundWordItem.isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem.isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	protected boolean isCorrectArticleOfFirstUnwrittenProperNounRelationWord( short definiteArticleParameter )
		{
		WordItem firstProperNounUnwrittenRelationWordItem;

		return ( ( firstProperNounUnwrittenRelationWordItem = firstUnwrittenRelationWordItem( Constants.WORD_TYPE_PROPER_NOUN, null ) ) != null &&
				firstProperNounUnwrittenRelationWordItem.isProperNounPrecededByDefiniteArticle( definiteArticleParameter ) );
		}

	protected boolean isExclusiveNonPossessiveGeneralizationAssignment()
		{
		return ( isAssignment() &&
				isExclusiveNonPossessiveGeneralization() );
		}

	protected boolean isExclusiveNonPossessiveGeneralization()
		{
		return ( isExclusiveGeneralization_ &&
				!isPossessive_ );
		}

	protected boolean isEveryGeneralization()
		{
		return isEveryGeneralization_;
		}

	protected boolean isExclusiveGeneralization()
		{
		return isExclusiveGeneralization_;
		}

	protected boolean isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	protected boolean isGeneralizationNoun()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isGeneralizationProperNoun()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NOUN );
		}

	protected boolean isHiddenSpanishSpecification()
		{
		return ( isSpecificationWordSpanishAmbiguous() &&
				hasNonCompoundSpecificationCollection() &&
				!isNegative_ &&
				isSelfGenerated() &&
				isGeneralizationProperNoun() &&

				// Not possessive specification not in a male word
				( ( !isPossessive_ &&
				!myWordItem().isMale() ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.compoundCollectionNr() > Constants.NO_COLLECTION_NR &&
				!hasContextMaleWordInContextWords() ) ) );
		}

	protected boolean isMatchingGeneralizationContextNr( boolean isAllowingEmptyRelationContext, int generalizationContextNr )
		{
		return ( isAllowingEmptyRelationContext ||
				// Empty subset
				generalizationContextNr == Constants.NO_CONTEXT_NR ||
				// Same set
				generalizationContextNr_ == generalizationContextNr ? true : myWordItem().isContextSubsetInContextWords( generalizationContextNr_, generalizationContextNr ) );
		}

	protected boolean isMatchingRelationContextNr( boolean isAllowingEmptyRelationContext, int relationContextNr )
		{		// Empty subset
		return ( relationContextNr == Constants.NO_CONTEXT_NR ||
				// Same set
				relationContextNr_ == relationContextNr ||

				( isAllowingEmptyRelationContext &&
				relationContextNr_ == Constants.NO_CONTEXT_NR ) ? true : myWordItem().isContextSubsetInContextWords( relationContextNr_, relationContextNr ) );
		}

	protected boolean isNegative()
		{
		return isNegative_;
		}

	protected boolean isPartOf()
		{
		return isPartOf_;
		}

	protected boolean isPossessive()
		{
		return isPossessive_;
		}

	protected boolean isFirstJustificiationReversibleAssumption()
		{
		return ( firstJustificationItem_ != null &&
				firstJustificationItem_.isReversibleAssumption() );
		}

	protected boolean isFirstJustificiationReversibleConclusion()
		{
		return ( firstJustificationItem_ != null &&
				firstJustificationItem_.isReversibleConclusion() );
		}

	protected boolean isRelatedOlderSpecification( int specificationCollectionNr, int compoundSpecificationCollectionNr, WordItem specificationWordItem )
		{
		return ( isOlderItem() &&

				( ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr ) ||

				( compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

				( specificationCollectionNr_ == compoundSpecificationCollectionNr ||
				specificationWordItem_ == specificationWordItem ) ) ) );
		}

	protected boolean isRelatedSpecification( boolean isNegative, WordItem specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				specificationWordItem_ == specificationWordItem );
		}

	protected boolean isRelatedSpecification( boolean isExclusiveSpecification, boolean isPossessive, WordItem specificationWordItem )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isPossessive_ == isPossessive &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ == specificationWordItem );
		}

	protected boolean isSelfGeneratedAssumption()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( !isConcludedAssumption_ &&
		questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != null )
			{
			if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
				return true;

			while( searchJustificationItem != null )
				{
				// At least one justification is an assumption
				if( searchJustificationItem.isAssumptionJustification() )
					return true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean isSelfGeneratedConclusion()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != null )
			{
			if( isConcludedAssumption_ )
				return true;

			while( searchJustificationItem != null )
				{
				// At least one justification is an assumption. So, it is no conclusion
				if( searchJustificationItem.isAssumptionJustification() )
					return false;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}

			return true;
			}

		return false;
		}

	protected boolean isSelfGeneratedQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER &&
				firstJustificationItem_ != null );
		}

	protected boolean isSelfGenerated()
		{
		return ( firstJustificationItem_ != null );
		}

	protected boolean isSpecific()
		{
		return isSpecific_;
		}

	protected boolean isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	protected boolean isSpecificationNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isSpecificationPluralNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationSingularNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				myWordItem().nContextWords( relationContextNr_, specificationWordItem_ ) > 1 );
		}

	protected boolean isSpecificationWordSpanishAmbiguous()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isNounWordSpanishAmbiguous() );
		}

	protected boolean isTriggeringExecutionOfSelections()
		{
		return ( isAssignment() ||
				generalizationWordTypeNr_ == Constants.WORD_TYPE_VERB_SINGULAR );
		}

	protected boolean isUncountableGeneralizationNoun()
		{
		return isUncountableGeneralizationNoun_;
		}

	protected boolean isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	protected boolean isUserSpecification()
		{
		return ( questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == null );
		}

	protected boolean isValueSpecification()
		{
		return isValueSpecification_;
		}

	protected boolean wasHiddenSpanishSpecification()
		{
		JustificationItem attachedJustificationItem;

		return ( relationContextNr_ > Constants.NO_CONTEXT_NR &&
				// Self-generated
				firstJustificationItem_ != null &&
				isOlderItem() &&
				storedSentenceStringBuffer_ == null &&
				isSpecificationWordSpanishAmbiguous() &&
				!isHiddenSpanishSpecification() &&

				( isSelfGeneratedAssumption() ||

				( !isConcludedAssumption_ &&
				firstJustificationItem_.hasPossessiveSecondarySpecification() ) ||

				( isConcludedAssumption_ &&

				( firstJustificationItem_.hasNonPossessivePrimarySpecification() ||

				( firstJustificationItem_.isReversibleConclusion() &&
				( attachedJustificationItem = firstJustificationItem_.attachedJustificationItem() ) != null &&
				attachedJustificationItem.hasNonPossessivePrimarySpecification() ) ) ) ) );
		}

	protected short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
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

	protected int generalizationCollectionNr()
		{
		return generalizationCollectionNr_;
		}

	protected int specificationCollectionNr()
		{
		return specificationCollectionNr_;
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

	protected int nReversibleConclusionJustifications()
		{
		int nReversibleConclusions = 0;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isReversibleConclusion() )
				nReversibleConclusions++;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return nReversibleConclusions;
		}

	protected int nInvolvedSpecificationWords()
		{
		int myCreationSentenceNr = creationSentenceNr();
		int nInvolvedSpecificationWords = 0;
		SpecificationItem searchSpecificationItem = myWordItem().firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem == this ||
			searchSpecificationItem.creationSentenceNr() == myCreationSentenceNr )
				{
				if( !searchSpecificationItem.hasUserSpecificationWordItem() )
					return 0;

				nInvolvedSpecificationWords++;
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return nInvolvedSpecificationWords;
		}

	protected byte attachJustificationToSpecification( JustificationItem attachJustificationItem )
		{
		boolean hasCurrentlyCorrectedAssumptionByKnowledge;
		JustificationItem firstJustificationItem;
		SpecificationItem createdSpecificationItem;
		WordItem thisWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult;

		if( attachJustificationItem == null )
			return startError( 1, null, "The given attached justification item is undefined" );

		if( !attachJustificationItem.isActiveItem() )
			return startError( 1, null, "The given attached justification item isn't active" );

		if( isReplacedOrDeletedItem() )
			return startError( 1, null, "I am replaced or deleted" );

		if( ( firstJustificationItem = firstJustificationItem_ ) == null )
			return startError( 1, null, "I have no justification items" );

		if( hasCurrentCreationSentenceNr() )
			{
			hasCurrentlyCorrectedAssumptionByKnowledge = thisWordItem.hasCurrentlyCorrectedAssumptionByKnowledge();

			if( changeFirstJustification( !hasCurrentlyCorrectedAssumptionByKnowledge, attachJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to change my first justification item" );

			if( !hasCurrentlyCorrectedAssumptionByKnowledge &&
			attachJustificationItem.attachJustification( firstJustificationItem, this ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to attach the first justification item of myself to the given attached justification item" );
			}
		else
			{
			// Copy specification
			if( ( createAndAssignResult = thisWordItem.copySpecificationItem( specificationCollectionNr_, attachJustificationItem, this ) ).result != Constants.RESULT_OK )
				return addError( 1, null, "I failed to copy myself with a different first justification item" );

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
				return startError( 1, null, "I couldn't create a specification" );

			if( attachJustificationItem.attachJustification( firstJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );

			if( thisWordItem.replaceOrDeleteSpecification( this, createdSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to replace or delete a specification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte attachJustificationToSpecificationAdvanced( JustificationItem attachJustificationItem )
		{
		boolean hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean isOlderSpecification = isOlderItem();
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isSkippingUpdateJustification = false;
		short highestOrderNr;
		short justificationTypeNr;
		short newAssumptionLevel;
		short previousAssumptionLevel;
		JustificationItem attachedJustificationItem;
		JustificationItem lastAttachedJustificationItem;
		JustificationItem obsoleteJustificationItem = null;
		SpecificationItem attachedPrimarySpecificationItem;
		SpecificationItem attachedSecondarySpecificationItem;

		if( attachJustificationItem == null )
			return startError( 1, null, "The given attached justification item is undefined" );

		if( !attachJustificationItem.isActiveItem() )
			return startError( 1, null, "The given attached justification item isn't active" );

		if( isReplacedOrDeletedItem() )
			return startError( 1, null, "I am replaced or deleted" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I have no justification items" );

		isPrimarySpecificationWordSpanishAmbiguous = attachJustificationItem.isPrimarySpecificationWordSpanishAmbiguous();
		justificationTypeNr = attachJustificationItem.justificationTypeNr();
		attachedSecondarySpecificationItem = attachJustificationItem.secondarySpecificationItem();

		if( ( attachedPrimarySpecificationItem = attachJustificationItem.primarySpecificationItem() ) != null &&

		( ( !hasRelationContext &&
		!isExclusiveSpecification_ &&
		!attachedPrimarySpecificationItem.isExclusiveSpecification() &&

		( attachedPrimarySpecificationItem.hasRelationContext() ||
		!attachedPrimarySpecificationItem.isPossessive() ) ) ||

		( ( ( isQuestion() &&
		attachedPrimarySpecificationItem.isPossessive() ) ||

		( !isPrimarySpecificationWordSpanishAmbiguous &&

		( attachedPrimarySpecificationItem.isOlderItem()  ||

		( attachedPrimarySpecificationItem.hasRelationContext() &&
		justificationTypeNr != Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) ) ) ) &&

		!hasJustificationOfSameType( justificationTypeNr, attachedPrimarySpecificationItem, attachedSecondarySpecificationItem ) ) ) )
			{
			if( ( highestOrderNr = highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= Constants.MAX_ORDER_NR )
				return startSystemError( 1, null, "Justification order number overflow" );

			attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
			}

		if( attachedPrimarySpecificationItem != null &&
		!attachedPrimarySpecificationItem.isPossessive() &&
		attachedPrimarySpecificationItem.hasRelationContext() )
			obsoleteJustificationItem = firstJustificationItem_.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem.specificationWordItem() );
		else
			{
			// Typical for Spanish
			if( attachedSecondarySpecificationItem != null &&
			attachedSecondarySpecificationItem.isUserSpecification() &&
				
			( isQuestion() ||
			isSpecificationWordSpanishAmbiguous() ) )
				obsoleteJustificationItem = firstJustificationItem_.obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
			}

		if( obsoleteJustificationItem != null )
			{
			if( obsoleteJustificationItem.isOlderItem() )
				{
				if( !hasRelationContext &&

				( !isPrimarySpecificationWordSpanishAmbiguous ||

				( attachedPrimarySpecificationItem != null &&
				attachedPrimarySpecificationItem.hasAssumptionLevel() ) ) )
					isSkippingUpdateJustification = true;
				else
					{
					if( myWordItem().replaceJustification( obsoleteJustificationItem, attachJustificationItem, this ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace the obsolete justification item" );
					}
				}
			else
				obsoleteJustificationItem = null;
			}

		if( obsoleteJustificationItem == null )
			{
			// Find a justification item with current sentence number, but without attached justification item
			lastAttachedJustificationItem = firstJustificationItem_;

			while( ( attachedJustificationItem = lastAttachedJustificationItem.attachedJustificationItem() ) != null )
				lastAttachedJustificationItem = attachedJustificationItem;

			if( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() )
				{
				if( lastAttachedJustificationItem.attachJustification( attachJustificationItem, this ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to attach a justification item" );

				if( ( !hasRelationContext ||
				!isOlderSpecification ) &&

				!isHiddenSpanishSpecification() )
					{
					previousAssumptionLevel = assumptionLevel();

					if( calculateAssumptionLevel() != Constants.RESULT_OK )
						return addError( 1, null, "I failed to calculate the assumption level of the involved specification" );

					if( ( !hasRelationContext &&
					isOlderSpecification &&
					!isPartOf_ &&
					// Difference between recalculated assumption level and previous assumption level
					( newAssumptionLevel = assumptionLevel() ) != previousAssumptionLevel &&

					( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
					// Recalculated assumption level is higher than 'maybe'
					// Skip writing meaningless adjustment (from 'maybe' to 'maybe')
					newAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) ) &&

					// Write adjusted specification
					myWordItem().writeUpdatedSpecification( true, false, false, false, false, this ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to write an adjusted specification" );
					}
				}
			else
				{
				if( !attachJustificationItem.hasAttachedJustification() &&
				// Attach justification to specification
				attachJustificationToSpecification( attachJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to attach the justification item" );
				}
			}
		else
			{
			if( !isSkippingUpdateJustification &&
			myWordItem().updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to update a question justification item by a conclusion justification item" );
			}

		return Constants.RESULT_OK;
		}

	protected byte calculateAssumptionLevel()
		{
		boolean isSpanishCurrentLanguage;
		boolean _isSpecificationWordSpanishAmbiguous = false;
		short highestAssumptionLevel;
		short lowestAssumptionLevel = Constants.MAX_LEVEL;
		int nJustificationRelationWords;
		int nRelationWords;
		int tempAssumptionLevel;
		JustificationItem currentJustificationItem;
		JustificationItem nextJustificationItem = firstJustificationItem_;
		ShortResultType shortResult;

		if( !isSpecificationGeneralization_ &&
		isSelfGeneratedAssumption() )
			{
			isSpanishCurrentLanguage = myWordItem().isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				_isSpecificationWordSpanishAmbiguous = isSpecificationWordSpanishAmbiguous();

			nRelationWords = nRelationContextWords();

			do	{
				highestAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
				nJustificationRelationWords = 0;

				if( ( currentJustificationItem = nextJustificationItem ) != null )
					{
					do	{
						nJustificationRelationWords += currentJustificationItem.nJustificationContextRelations( relationContextNr_, nRelationWords );

						if( currentJustificationItem.isAssumptionJustification() )
							{
							if( ( shortResult = currentJustificationItem.getCombinedAssumptionLevel() ).result != Constants.RESULT_OK )
								return addError( 1, null, "I failed to get the combined assumption level" );

							// Add combined assumption level and justification grade
							if( ( tempAssumptionLevel = ( shortResult.shortValue + currentJustificationItem.justificationAssumptionGrade() ) ) > highestAssumptionLevel )
								{
								if( tempAssumptionLevel >= Constants.MAX_LEVEL )
									return startSystemError( 1, null, "Assumption level overflow" );

								highestAssumptionLevel = (short)tempAssumptionLevel;
								}
							}
						}
					while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );
					}

				if( highestAssumptionLevel < lowestAssumptionLevel &&

				( nJustificationRelationWords == nRelationWords ||

				( highestAssumptionLevel <= assumptionLevel_ &&
				nJustificationRelationWords > nRelationWords ) ) &&

				( !isSpanishCurrentLanguage ||
				// Typical for Spanish
				isNegative_ ||
				// No assumption level
				assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ||

				// First set of justifications
				( lowestAssumptionLevel == Constants.MAX_LEVEL &&
				highestAssumptionLevel == Constants.NO_ASSUMPTION_LEVEL ) ||

				// Assumption level
				( !_isSpecificationWordSpanishAmbiguous &&
				assumptionLevel_ + 2 > highestAssumptionLevel ) ) )
					lowestAssumptionLevel = highestAssumptionLevel;
				}
			while( ( nextJustificationItem = nextJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != null );

			if( lowestAssumptionLevel < Constants.MAX_LEVEL &&
			// Assumption level has changed
			lowestAssumptionLevel != assumptionLevel_ &&

			( lowestAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL ||
			isOlderItem() ) )
				{
				assumptionLevel_ = lowestAssumptionLevel;

				if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL &&
				isOlderItem() )
					// Clear stored sentence buffer to lose the previous assumption level word
					clearStoredSentenceStringBuffer();
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte changeFirstJustification( boolean isCheckingForDuplicates, JustificationItem replacingJustificationItem )
		{
		if( replacingJustificationItem == null )
			return startError( 1, null, "The given replacing justification item is undefined" );

		if( !replacingJustificationItem.isActiveItem() )
			return startError( 1, null, "The given replacing justification item isn't active" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isCheckingForDuplicates &&
		checkJustification( replacingJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to check the given replacing justification item" );

		firstJustificationItem_ = replacingJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.isDeletedItem() &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted justification item in a replaced specification item:\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustification: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, null, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForReplacedOrDeletedJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.isReplacedOrDeletedItem() &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted justification item:\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, null, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkJustification( JustificationItem referenceJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;
		SpecificationItem referencePrimarySpecificationItem;
		SpecificationItem referenceAnotherPrimarySpecificationItem;
		SpecificationItem referenceSecondarySpecificationItem;
		SpecificationItem referenceAnotherSecondarySpecificationItem;

		if( referenceJustificationItem == null )
			return startError( 1, null, "The given reference justification item is undefined" );

		if( searchJustificationItem == null )
			return startError( 1, null, "I have no justification items" );

		referencePrimarySpecificationItem = referenceJustificationItem.primarySpecificationItem();
		referenceAnotherPrimarySpecificationItem = referenceJustificationItem.anotherPrimarySpecificationItem();
		referenceSecondarySpecificationItem = referenceJustificationItem.secondarySpecificationItem();
		referenceAnotherSecondarySpecificationItem = referenceJustificationItem.anotherSecondarySpecificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem == referenceJustificationItem )
				return startError( 1, null, "The given reference justification item is already one of my attached justification items" );

			if( searchJustificationItem.hasJustification( referencePrimarySpecificationItem, referenceAnotherPrimarySpecificationItem, referenceSecondarySpecificationItem, referenceAnotherSecondarySpecificationItem ) )
				return startError( 1, null, "I already have a justificiation item like the given attached justification item" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralization( boolean isExclusiveGeneralization, int generalizationCollectionNr )
		{
		if( generalizationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The given generalization collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveGeneralization )
			isExclusiveGeneralization_ = true;

		if( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The generalization is already collected" );

		generalizationCollectionNr_ = generalizationCollectionNr;

		return Constants.RESULT_OK;
		}

	protected byte collectSpecification( boolean isExclusiveSpecification, int specificationCollectionNr )
		{
		if( specificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The given specification collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveSpecification )
			isExclusiveSpecification_ = true;

		if( specificationCollectionNr_ > Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The specification is already collected" );

		specificationCollectionNr_ = specificationCollectionNr;

		return Constants.RESULT_OK;
		}

	protected byte markAsAnsweredQuestion()
		{
		if( hasCurrentCreationSentenceNr() )
			isAnsweredQuestion_ = true;
		else
			{
			if( myWordItem().copyAndReplaceSpecificationItem( true, isExclusiveGeneralization_, isExclusiveSpecification_, generalizationCollectionNr_, specificationCollectionNr_, firstJustificationItem_, this ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to copy and replace myself as an answered question" );
			}

		return Constants.RESULT_OK;
		}

	protected byte markAsConcludedAssumption()
		{
		if( !isSelfGeneratedAssumption() &&
		!myWordItem().isUserRelationWord )
			return startError( 1, null, "I am not a self-generated assumption" );

		isConcludedAssumption_ = true;
		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

		// Clear stored sentence buffer to lose the uncertainty word written in the sentence
		clearStoredSentenceStringBuffer();

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationStringAsWritten()
		{
		if( GlobalVariables.currentWriteLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, null, "Current write word level overflow" );

		if( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL )
			return startError( 1, null, "My write level is already assigned" );

		specificationStringWriteLevel_ = ++GlobalVariables.currentWriteLevel;

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumption()
		{
		boolean hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean isAdjustedSpecification = false;
		short previousAssumptionLevel = assumptionLevel_;

		if( calculateAssumptionLevel() != Constants.RESULT_OK )
			return addError( 1, null, "I failed to recalculate my assumption level" );

		if( isOlderItem() &&

		( assumptionLevel_ != previousAssumptionLevel ||
		// Typical for Spanish
		assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ) &&

		// Avoid updates of high uncertainty mapping,
		// from 'maybe' of one level to 'maybe' of another level
		( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
		assumptionLevel_ < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
			{
			if( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL )
				{
				if( markAsConcludedAssumption() != Constants.RESULT_OK )
					return addError( 1, null, "After recalculation, I failed to mark myself as a concluded assumption" );

				if( ( !hasRelationContext &&
				previousAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL ) ||

				( isPossessive_ &&
				assumptionLevel_ != previousAssumptionLevel ) )
					isAdjustedSpecification = true;
				}
			else
				{
				if( ( !isPossessive_ &&
				previousAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL ) ||

				( hasRelationContext &&
				hasCompoundSpecificationCollection() &&
				!isSpecificationWordSpanishAmbiguous() ) )
					isAdjustedSpecification = true;
				}

			if( isAdjustedSpecification )
				{
				if( isNegative_ &&
				assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
					hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = true;

				// Write adjusted specification
				if( myWordItem().writeUpdatedSpecification( true, false, false, false, wasHiddenSpanishSpecification(), this ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to write an adjusted specification" );

				if( specificationCollectionNr_ > Constants.NO_COLLECTION_NR )
					{
					if( hasRelationContext )
						{
						if( recalculateAssumptionsInContextWords( relationContextNr_, specificationWordItem_ ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to recalculate the assumptions of words with my relation context" );
						}
					else
						{
						if( myWordItem().recalculateAssumptionsInWord() != Constants.RESULT_OK )
							return addError( 1, null, "I failed to recalculate the assumptions of my word" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSpecificationConflict( boolean isQuestion )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( myWordItem().writeSelectedSpecification( false, false, this ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write the conflicting specification" );

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() == 0 )
			return startError( 1, null, "The written sentence string buffer is empty" );

		if( InputOutput.writeInterfaceText( true, ( isQuestion ? Constants.INPUT_OUTPUT_PROMPT_WRITE : Constants.INPUT_OUTPUT_PROMPT_WARNING ), ( isOlderItem() ? ( isQuestion ? Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_DEFINITION : Constants.INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_DEFINITION ) : Constants.INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write an interface warning" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write the conflict sentence" );

		hasSpecificationBeenWrittenAsConflict_ = true;

		return Constants.RESULT_OK;
		}

	protected String specificationString()
		{
		return specificationString_;
		}

	protected String activeGeneralizationWordTypeString()
		{
		return myWordItem().activeWordTypeString( generalizationWordTypeNr_ );
		}

	protected String activeSpecificationWordTypeString()
		{
		return ( specificationWordItem_ != null ?
				specificationWordItem_.activeWordTypeString( specificationWordTypeNr_ ) : null );
		}

	protected StringBuffer storedSentenceStringBuffer()
		{
		return storedSentenceStringBuffer_;
		}

	protected StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer()
		{
		return storedSentenceWithOnlyOneSpecificationStringBuffer_;
		}

	protected JustificationItem differentAssumptionLevelSpecificationSubstitutionJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		short primaryAssumptionLevel;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != null &&
		secondarySpecificationItem != null )
			{
			primaryAssumptionLevel = primarySpecificationItem.assumptionLevel();

			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isSpecificationSubstitutionAssumption() &&
				searchJustificationItem.isOlderItem() &&
				searchJustificationItem.primarySpecificationAssumptionLevel() != primaryAssumptionLevel &&
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	protected JustificationItem firstJustificationItem( short justificationTypeNr )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstOlderJustificationItem( short justificationTypeNr )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isOlderItem() &&
			searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstOlderJustificationItem( short justificationTypeNr, WordItem secondaryGeneralizationWordItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( secondaryGeneralizationWordItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isOlderItem() &&
				searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.secondaryGeneralizationWordItem() == secondaryGeneralizationWordItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem primaryOrSecondarySpecificationJustificationItem( SpecificationItem primaryOrSecondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( primaryOrSecondarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.primarySpecificationItem() == primaryOrSecondarySpecificationItem ||
				searchJustificationItem.secondarySpecificationItem() == primaryOrSecondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem secondarySpecificationJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( secondarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem exclusiveSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isExclusiveSpecification() &&
			!searchSpecificationItem.isNegative() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nonPossessiveDefinitionSpecificationItem( boolean isIncludingAdjectives, boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.isPossessive() &&

			( isIncludingAdjectives ||
			searchSpecificationItem.isSpecificationNoun() ) &&

			!searchSpecificationItem.isSpecificationGeneralization() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem possessiveSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() &&
			searchSpecificationItem.hasSpecificationCollection() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem primarySpecificationCandidateOfQuestionSpecificationItem( boolean isAllowingSpanishPossessiveSpecification, boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPrimarySpecificationCandidateOfQuestion( isAllowingSpanishPossessiveSpecification ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem selectedRelationSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasRelationContext() &&
			!searchSpecificationItem.isQuestion() &&
			!searchSpecificationItem.isHiddenSpanishSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem specificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		short currentLanguageNr = GlobalVariables.currentLanguageNr;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchSpecificationItem.languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem specificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		short currentLanguageNr = GlobalVariables.currentLanguageNr;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchSpecificationItem.languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nextExclusiveNounSpecificationItem()
		{
		return exclusiveSpecificationItem( false );
		}

	protected SpecificationItem nextExclusiveSpecificationItem()
		{
		return exclusiveSpecificationItem( false );
		}

	protected SpecificationItem nextNonPossessiveDefinitionSpecificationItem( boolean isIncludingAdjectives )
		{
		return nonPossessiveDefinitionSpecificationItem( isIncludingAdjectives, false );
		}

	protected SpecificationItem nextPossessiveSpecificationItem()
		{
		return possessiveSpecificationItem( false );
		}

	protected SpecificationItem nextPrimarySpecificationCandidateOfQuestionSpecificationItem( boolean isAllowingSpanishPossessiveSpecification )
		{
		return primarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, false );
		}

	protected SpecificationItem nextSelectedRelationSpecificationItem()
		{
		return selectedRelationSpecificationItem( false );
		}

	protected SpecificationItem nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, isQuestion() ) : specificationItem( false, false, isQuestion() ) );
		}

	protected SpecificationItem nextSelectedQuestionParameterSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, questionParameter_ ) : specificationItem( false, false, questionParameter_ ) );
		}

	protected SpecificationItem nextSelectedQuestionParameterSpecificationItem( boolean isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? assignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) : specificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	protected SpecificationItem nextSpecificationItem()
		{
		return (SpecificationItem)nextItem;
		}

	protected SpecificationItem recentlyAnsweredQuestionSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = specificationItem( true, isIncludingThisItem, questionParameter_ );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasCurrentCreationSentenceNr() &&
			searchSpecificationItem.isAnsweredQuestion_ )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( true );
			}

		return null;
		}

	protected SpecificationItem updatedSpecificationItem()
		{
		SpecificationItem updatedSpecificationItem;
		SpecificationItem searchSpecificationItem = this;

		while( ( updatedSpecificationItem = searchSpecificationItem.replacingSpecificationItem ) != null )
			searchSpecificationItem = updatedSpecificationItem;

		return searchSpecificationItem;
		}

	protected WordItem firstUnwrittenRelationWordItem( short wordTypeNr, WordItem previousWordItem )
		{
		// Start with previous word
		WordItem currentRelationWordItem = previousWordItem;

		// Check all relation words
		while( ( currentRelationWordItem = relationContextWordItemInContextWords( currentRelationWordItem ) ) != null )
			{
			if( !currentRelationWordItem.isRelationWordTypeAlreadyWritten( wordTypeNr ) )
				return currentRelationWordItem;
			}

		return null;
		}

	protected WordItem generalizationWordItem()
		{
		return myWordItem();
		}

	protected WordItem singleRelationWordItem()
		{
		WordItem firstRelationWordItem;

				// First relation word
		return ( ( firstRelationWordItem = relationContextWordItemInContextWords( null ) ) != null &&
				// No another relation word
				relationContextWordItemInContextWords( firstRelationWordItem ) == null ? firstRelationWordItem : null );
		}

	protected WordItem relatedSpecificationWordItem( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int relationContextNr )
		{
		return ( isPossessive_ == isPossessive &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&

				( isIgnoringExclusive ||
				isExclusiveSpecification_ == isExclusiveSpecification ) &&

				( isIgnoringNegative ||
				isNegative_ == isNegative ) &&

				( relationContextNr_ == relationContextNr ||

				( !isCheckingRelationContext &&
				!isHiddenSpanishSpecification() ) ) ? specificationWordItem_ : null );
		}

	protected WordItem relatedSpecificationWordItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedSpecification, short assumptionLevel, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGenerated() == isSelfGeneratedSpecification &&
				specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				relationContextNr_ == relationContextNr &&

				( assumptionLevel_ == assumptionLevel ||
				isQuestion() ) &&

				( specificationWordTypeNr == Constants.NO_WORD_TYPE_NR ||
				specificationWordTypeNr_ == specificationWordTypeNr ) ? specificationWordItem_ : null );
		}

	protected WordItem specificationWordItem()
		{
		return specificationWordItem_;
		}
	};

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his covenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
