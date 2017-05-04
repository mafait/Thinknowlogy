/*	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
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

class SpecificationItem extends Item
	{
	// Private initialized variables

	private boolean isAnsweredQuestion_;
	private boolean isConcludedAssumption_;
	private boolean isConditional_;
	private boolean isCorrectedAssumption_;
	private boolean isEveryGeneralization_;
	private boolean isExclusiveGeneralization_;
	private boolean isExclusiveSpecification_;
	private boolean isLanguageWord_;
	private boolean isNegative_;
	private boolean isPartOf_;
	private boolean isPossessive_;
	private boolean isSpecificationGeneralization_;
	private boolean isUniqueUserRelation_;
	private boolean isValueSpecification_;

	private short assignmentLevel_;
	private short assumptionLevel_;
	private short languageNr_;
	private short prepositionParameter_;
	private short questionParameter_;

	private short generalizationWordTypeNr_;
	private short specificationWordTypeNr_;
	private short relationWordTypeNr_;

	private int generalizationCollectionNr_;
	private int specificationCollectionNr_;

	private int generalizationContextNr_;
	private int specificationContextNr_;
	private int relationContextNr_;

	private int nContextRelations_;

	private JustificationItem firstJustificationItem_;

	private WordItem specificationWordItem_;

	private String specificationString_;


	// Private constructed variables

	private boolean hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_;
	private boolean hasSpecificationBeenWrittenAsConflict_;

	private short specificationStringWriteLevel_;


	// Private question methods

	private SpecificationItem newUserQuestion( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSelectedSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.isAnsweredQuestion_ &&
			!searchSpecificationItem.isOlderItem() &&
			searchSpecificationItem.isUserQuestion() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return searchSpecificationItem;
		}


	// Private specification methods

	private boolean hasContextMaleWordInContextWords()
		{
		boolean isSkippingReverseCheck = ( !isOlderItem() ||
										isSelfGeneratedConclusion() ||
										!hasOnlyOneRelationWord() );
		WordItem currentContextWordItem;
		WordItem generalizationWordItem = myWordItem();

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		specificationWordItem_ != null &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem.isMale() &&
				currentContextWordItem.hasContextInWord( relationContextNr_, specificationWordItem_ ) &&

				( isSkippingReverseCheck ||
				currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, false, false, specificationWordItem_, generalizationWordItem ) == null ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
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

				generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME &&
				questionParameter_ == Constants.NO_QUESTION_PARAMETER &&

				( !isExclusiveSpecification_ ||
				!isSelfGenerated() ) &&

				specificationWordItem_ != null &&
				!isHiddenSpanishSpecification() );
		}

	private boolean isSpecificationAdjective()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_ADJECTIVE );
		}

	private byte recalculateAssumptionsInContextWords( int contextNr, WordItem specificationWordItem )
		{
		WordItem currentContextWordItem;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
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

	private static WordItem contextWordItemInContextWords( int contextNr, WordItem specificationWordItem, WordItem previousContextWordItem )
		{
		WordItem currentContextWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		// Do in all (remaining) words
		( currentContextWordItem = ( previousContextWordItem == null ? CommonVariables.firstContextWordItem : previousContextWordItem.nextContextWordItem ) ) != null )
			{
			do	{
				if( currentContextWordItem.hasContextInWord( contextNr, specificationWordItem ) )
					return currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return null;
		}


	// Protected constructed variables

	protected boolean hasSpecificationBeenWrittenAsAnswer;

	protected SpecificationItem replacingSpecificationItem;

	protected StringBuffer lastWrittenSentenceStringBuffer;
	protected StringBuffer lastWrittenSentenceWithOneSpecificationOnlyStringBuffer;


	// Constructor

	protected SpecificationItem( boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isLanguageWord, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, myList, myWordItem );

		// Private initialized variables

		isAnsweredQuestion_ = isAnsweredQuestion;
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
		isSpecificationGeneralization_ = isSpecificationGeneralization;
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

		hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = false;
		hasSpecificationBeenWrittenAsConflict_ = false;

		specificationStringWriteLevel_ = Constants.NO_WRITE_LEVEL;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;

		replacingSpecificationItem = null;

		lastWrittenSentenceStringBuffer = null;
		lastWrittenSentenceWithOneSpecificationOnlyStringBuffer = null;
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
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( specificationWordItem_ != null &&
		( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
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
		StringBuffer queryStringBuffer;
		String wordString;
		String languageNameString = myWordItem().languageNameString( languageNr_ );
		String generalizationWordTypeString = myWordItem().wordTypeNameString( generalizationWordTypeNr_ );
		String specificationWordTypeString = myWordItem().wordTypeNameString( specificationWordTypeNr_ );
		String relationWordTypeString = ( relationWordTypeNr_ == Constants.NO_WORD_TYPE_NR ? null : myWordItem().wordTypeNameString( relationWordTypeNr_ ) );

		itemBaseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( languageNr_ > Constants.NO_LANGUAGE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + ( languageNameString == null ? ( "languageNr:" + languageNr_ ) : ( "language:" + languageNameString ) ) );

		if( isAnsweredQuestion_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAnsweredQuestion" );

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

		if( isSpecificationGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralization" );

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

		if( lastWrittenSentenceStringBuffer != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "lastWrittenSentenceStringBuffer:" + Constants.QUERY_STRING_START_CHAR + lastWrittenSentenceStringBuffer + Constants.QUERY_STRING_END_CHAR );

		if( lastWrittenSentenceWithOneSpecificationOnlyStringBuffer != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "lastWrittenSentenceWithOneSpecificationOnlyStringBuffer:" + Constants.QUERY_STRING_START_CHAR + lastWrittenSentenceWithOneSpecificationOnlyStringBuffer + Constants.QUERY_STRING_END_CHAR );

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
		return myWordItem().nContextWordsInContextWords( relationContextNr_, specificationWordItem_ );
		}

	protected SpecificationItem assignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isQuestion() == isQuestion &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithCurrentLevel();
			}

		return null;
		}

	protected SpecificationItem assignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.questionParameter_ == questionParameter &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithCurrentLevel();
			}

		return null;
		}

	protected SpecificationItem nextAssignmentItemWithCurrentLevel()
		{
		SpecificationItem nextAssignmentItem = (SpecificationItem)nextItem;

		if( nextAssignmentItem != null &&
		nextAssignmentItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
			return nextAssignmentItem;

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

	protected SpecificationItem firstNewUserQuestion()
		{
		return newUserQuestion( true );
		}

	protected SpecificationItem nextNewUserQuestion()
		{
		return newUserQuestion( false );
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

	protected void incrementAssumptionLevel()
		{
		if( assumptionLevel_ < Constants.MAX_LEVEL )
			assumptionLevel_++;
		}

	protected void initializeSpecificationVariables()
		{
		// Private constructed variables

		hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = false;
		hasSpecificationBeenWrittenAsConflict_ = false;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;
		}

	protected boolean hasAnsweredQuestionInJustification()
		{
		SpecificationItem secondarySpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( firstJustificationItem_ != null &&
		firstJustificationItem_.attachedJustificationItem() == null &&
		firstJustificationItem_.isNegativeAssumptionOrConclusion() &&
		( secondarySpecificationItem = firstJustificationItem_.secondarySpecificationItem() ) != null )
			{
			if( secondarySpecificationItem.isAnsweredQuestion_ &&
			( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
				{
				if( !secondarySpecificationWordItem.isUserSpecificationWord )
					return true;
				}
			}

		return false;
		}

	protected boolean hasAssumptionLevel()
		{
		return ( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL );
		}

	protected boolean hasJustification( boolean isIncludingReplacingJustification, JustificationItem referenceJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( referenceJustificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem == referenceJustificationItem ||

				( isIncludingReplacingJustification &&
				searchJustificationItem.replacingJustificationItem == referenceJustificationItem ) )
					return true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean hasJustificationOfSameType( short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		int searchPrimarySpecificationCollectionNr;
		int searchSecondarySpecificationCollectionNr;
		int primarySpecificationCollectionNr = ( primarySpecificationItem == null ? Constants.NO_COLLECTION_NR : primarySpecificationItem.specificationCollectionNr() );
		int secondarySpecificationCollectionNr = ( secondarySpecificationItem == null ? Constants.NO_COLLECTION_NR : secondarySpecificationItem.specificationCollectionNr() );
		JustificationItem searchJustificationItem = firstJustificationItem_;
		SpecificationItem searchPrimarySpecificationItem;
		SpecificationItem searchSecondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				{
				searchPrimarySpecificationItem = searchJustificationItem.primarySpecificationItem();
				searchSecondarySpecificationItem = searchJustificationItem.secondarySpecificationItem();

				searchPrimarySpecificationCollectionNr = ( searchPrimarySpecificationItem == null ? Constants.NO_COLLECTION_NR : searchPrimarySpecificationItem.specificationCollectionNr() );
				searchSecondarySpecificationCollectionNr = ( searchSecondarySpecificationItem == null ? Constants.NO_COLLECTION_NR : searchSecondarySpecificationItem.specificationCollectionNr() );

				if( searchPrimarySpecificationCollectionNr == primarySpecificationCollectionNr &&
				searchSecondarySpecificationCollectionNr == secondarySpecificationCollectionNr )
					return true;
				}

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
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
				hasRelationContextCurrentlyBeenUpdated() );
		}

	protected boolean hasRelationContextCurrentlyBeenUpdated()
		{
		WordItem currentContextWordItem;

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		specificationWordItem_ != null &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem.hasContextCurrentlyBeenUpdatedInWord( relationContextNr_, specificationWordItem_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return false;
		}

	protected boolean hasOnlyOneRelationWord()
		{
		return ( myWordItem().nContextWordsInContextWords( relationContextNr_, specificationWordItem_ ) == 1 );
		}

	protected boolean hasPrepositionParameter()
		{
		return ( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER );
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

	protected boolean hasRelationContext()
		{
		return ( relationContextNr_ > Constants.NO_CONTEXT_NR );
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

	protected boolean hasSpecificationCompoundCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isCompoundCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasSpecificationNonCompoundCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isNonCompoundCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasSpecificationNonExclusiveCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.hasNonExclusiveCollection( specificationCollectionNr_ ) );
		}

	protected boolean isAnsweredSelfGeneratedQuestion()
		{
		return ( isAnsweredQuestion_ &&
				firstJustificationItem_ != null );
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

	protected boolean isCorrectSpecificationArticle( short articleParameter )
		{
		boolean isIncludingAnsweredQuestions = isAnsweredQuestion();
		boolean isSelfGeneratedSpecification = isSelfGenerated();
		SpecificationItem searchSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem foundWordItem = null;

		if( specificationWordItem_ != null &&
		isSpecificationNoun() &&
		( searchSpecificationItem = myWordItem().firstSpecificationItem( isIncludingAnsweredQuestions, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != null )
			{
			do	{
				currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ : searchSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) );

				if( currentSpecificationWordItem != null &&
				!currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
					foundWordItem = currentSpecificationWordItem;
				}
			while( foundWordItem == null &&
			( searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions ) ) != null );

			return ( foundWordItem == null ? true : ( isDefiniteArticleParameter( articleParameter ) ? foundWordItem.isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem.isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	protected boolean isCorrectArticleOfFirstUnwrittenProperNameRelationWord( short definiteArticleParameter )
		{
		WordItem firstUnwrittenRelationWordItem = unwrittenProperNameRelationWordItem( null );

		return ( firstUnwrittenRelationWordItem != null &&
				firstUnwrittenRelationWordItem.isProperNamePrecededByDefiniteArticle( definiteArticleParameter ) );
		}

	protected boolean isExclusiveNonPossessiveGeneralizationAssignment()
		{
		return ( isAssignment() &&
				isExclusiveNonPossessiveGeneralization() );
		}

	protected boolean isExclusiveNonPossessiveGeneralization()
		{
		return ( isExclusiveGeneralization_ &&
				!isPossessive_ &&
				questionParameter_ == Constants.NO_QUESTION_PARAMETER );
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
		return isNounWordType( generalizationWordTypeNr_ );
		}

	protected boolean isGeneralizationProperName()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean isHiddenSpanishSpecification()
		{
		return ( isSpecificationWordSpanishAmbiguous() &&
				hasSpecificationNonCompoundCollection() &&
				!isNegative_ &&
				isSelfGenerated() &&
				!isQuestion() &&
				isGeneralizationProperName() &&

				// Not possessive specification not in a male word
				( ( !isPossessive_ &&

				( isCorrectedAssumption_ ||
				!myWordItem().isMale() ) ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.compoundCollectionNr( specificationWordTypeNr_ ) > Constants.NO_COLLECTION_NR &&
				!hasContextMaleWordInContextWords() ) ) );
		}

	protected boolean isMatchingGeneralizationContextNr( boolean isAllowingEmptyContextResult, int generalizationContextNr )
		{
		// Empty subset
		if( generalizationContextNr == Constants.NO_CONTEXT_NR ||
		// Same set
		generalizationContextNr_ == generalizationContextNr ||

		( isAllowingEmptyContextResult &&
		generalizationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInContextWords( generalizationContextNr_, generalizationContextNr );
		}

	protected boolean isMatchingRelationContextNr( boolean isAllowingEmptyContextResult, int relationContextNr )
		{
		// Empty subset
		if( relationContextNr == Constants.NO_CONTEXT_NR ||
		// Same set
		relationContextNr_ == relationContextNr ||

		( isAllowingEmptyContextResult &&
		relationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInContextWords( relationContextNr_, relationContextNr );
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

	protected boolean isPossessiveReversibleConclusion()
		{
		return ( firstJustificationItem_ != null &&
				firstJustificationItem_.isPossessiveReversibleConclusion() );
		}

	protected boolean isRelatedQuestion( boolean isExclusiveSpecification, int specificationCollectionNr, int subsetRelationContextNr )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				specificationCollectionNr_ == specificationCollectionNr &&
				isMatchingRelationContextNr( true, subsetRelationContextNr ) );
		}

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, short generalizationWordTypeNr )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				generalizationWordTypeNr_ == generalizationWordTypeNr );
		}

	protected boolean isRelatedSpecification( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem != null &&
				specificationWordItem_ == specificationWordItem );
		}

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr )
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				specificationCollectionNr_ == specificationCollectionNr &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				specificationWordItem != null &&
				specificationWordItem_ == specificationWordItem &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, int specificationCollectionNr, int compoundSpecificationCollectionNr, WordItem specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&

				( ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr ) ||

				( compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

				( specificationCollectionNr_ == compoundSpecificationCollectionNr ||
				specificationWordItem_ == specificationWordItem ) ) ) );
		}

	protected boolean isRelatedSpecification( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedSpecification, short assumptionLevel, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr )
		{
		return ( relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) != null );
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
				// At least one justification is an assumption. So, it is not a conclusion
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

	protected boolean isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	protected boolean isSpecificationNoun()
		{
		return isNounWordType( specificationWordTypeNr_ );
		}

	protected boolean isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isSpecificationPluralNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				!isAssignment() &&
				myWordItem().nContextWordsInContextWords( relationContextNr_, specificationWordItem_ ) > 1 );
		}

	protected boolean isSpecificationWordSpanishAmbiguous()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isNounWordSpanishAmbiguous() );
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
		return ( lastWrittenSentenceStringBuffer == null &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				isSpecificationWordSpanishAmbiguous() &&
				hasSpecificationNonCompoundCollection() &&
				!isHiddenSpanishSpecification() );
		}

	protected short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	protected short highestAttachedJustificationOrderNr( short justificationTypeNr )
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

	protected int nInvolvedSpecificationWords()
		{
		boolean isSelfGeneratedSpecification = isSelfGenerated();
		int _creationSentenceNr = creationSentenceNr();
		int nInvolvedSpecificationWords = 0;
		SpecificationItem searchSpecificationItem = myWordItem().firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem == this ||

			( searchSpecificationItem.creationSentenceNr() == _creationSentenceNr &&
			searchSpecificationItem.isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) ) )
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
		JustificationItem firstJustificationItem;
		SpecificationItem createdSpecificationItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

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
			if( changeFirstJustification( attachJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to change my first justification item" );

			if( !myWordItem().hasCorrectedAssumptionByKnowledge() &&
			attachJustificationItem.attachJustification( firstJustificationItem, this ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to attach the first justification item of myself to the given attached justification item" );
			}
		else
			{
			if( ( createAndAssignResult = myWordItem().createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isConcludedAssumption_, isConditional_, isCorrectedAssumption_, isEveryGeneralization_, isExclusiveGeneralization_, isExclusiveSpecification_, isNegative_, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr_, prepositionParameter_, questionParameter_, generalizationWordTypeNr_, specificationWordTypeNr_, relationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, lastWrittenSentenceStringBuffer ) ).result != Constants.RESULT_OK )
				return addError( 1, null, "I failed to create a copy of myself with a different first justification item" );

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
				return startError( 1, null, "I couldn't create a specification" );

			if( attachJustificationItem.attachJustification( firstJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );

			if( myWordItem().replaceOrDeleteSpecification( false, this, createdSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to replace or delete a specification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte changeFirstJustification( JustificationItem replacingJustificationItem )
		{
		if( replacingJustificationItem == null )
			return startError( 1, null, "The given replacing justification item is undefined" );

		if( !replacingJustificationItem.isActiveItem() )
			return startError( 1, null, "The given replacing justification item isn't active" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		firstJustificationItem_ = replacingJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null &&
		!CommonVariables.hasDisplayedIntegrityWarning )
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
		!CommonVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.isReplacedOrDeletedItem() )
				{
				if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted justification item:\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
					return startError( 1, null, "I failed to write an interface warning" );
				}

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

	protected byte markAsConcludedAssumption( boolean isSkippingCheck )
		{
		if( !isSkippingCheck &&
		!isSelfGeneratedAssumption() &&
		!wasHiddenSpanishSpecification() )
			return startError( 1, null, "I am not a self-generated assumption" );

		isConcludedAssumption_ = true;
		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

		// Clear sentence write buffer to lose the uncertainty word written in the sentence
		lastWrittenSentenceStringBuffer = null;

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationStringAsWritten()
		{
		if( CommonVariables.currentWriteLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, null, "Current write word level overflow" );

		if( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL )
			return startError( 1, null, "My write level is already assigned" );

		specificationStringWriteLevel_ = ++CommonVariables.currentWriteLevel;

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumption()
		{
		boolean isOlderAssumption;
		boolean hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean isAdjustedSpecification = false;
		short previousAssumptionLevel = assumptionLevel_;

		if( firstJustificationItem_ != null &&
		firstJustificationItem_.hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition() )
			{
			if( markAsConcludedAssumption( false ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to mark myself as a concluded assumption" );

			isAdjustedSpecification = true;
			}
		else
			{
			isOlderAssumption = isOlderItem();

			if( specificationCollectionNr_ == Constants.NO_COLLECTION_NR ||

			// Avoid corrected assumption to be concluded
			( !isCorrectedAssumption_ &&

			( !isOlderAssumption ||
			// Avoid older corrected assumption to be concluded
			!myWordItem().hasCorrectedAssumptionByKnowledge() ) ) )
				{
				if( recalculateAssumptionLevel().result != Constants.RESULT_OK )
					return addError( 1, null, "I failed to recalculate my assumption level" );

				if( isOlderAssumption &&
				assumptionLevel_ != previousAssumptionLevel &&

				// Avoid updates of high uncertainty mapping,
				// from 'maybe' of one level to 'maybe' of another level
				( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
				assumptionLevel_ < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
					{
					if( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL )
						{
						// Skip check on confirmed specification
						if( markAsConcludedAssumption( !isSelfGeneratedAssumption() ) != Constants.RESULT_OK )
							return addError( 1, null, "After recalculation, I failed to mark myself as a concluded assumption" );

						if( !hasRelationContext ||
						isPossessive_ )
							isAdjustedSpecification = true;
						}
					else
						{
						if( !isPossessive_ ||

						( hasRelationContext &&
						hasSpecificationCompoundCollection() ) )
							isAdjustedSpecification = true;
						}
					}
				}
			}

		if( isAdjustedSpecification )
			{
			if( isNegative_ &&
			assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
				hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = true;

			// Write adjusted specification
			if( myWordItem().writeUpdatedSpecification( true, false, false, false, false, this ) != Constants.RESULT_OK )
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

		return Constants.RESULT_OK;
		}

	protected byte writeSpecificationConflict()
		{
		if( myWordItem().writeSelectedSpecification( false, false, this ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write the conflicting specification" );

		if( CommonVariables.writtenSentenceStringBuffer == null ||
		CommonVariables.writtenSentenceStringBuffer.length() == 0 )
			return startError( 1, null, "The written sentence string buffer is empty" );

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WARNING, ( isOlderItem() ? Constants.INTERFACE_LISTING_CONFLICT : Constants.INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write an interface warning" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
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
		return ( specificationWordItem_ == null ? null : specificationWordItem_.activeWordTypeString( specificationWordTypeNr_ ) );
		}

	protected JustificationItem answeredQuestionJustificationItem()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasPrimaryAnsweredQuestion() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem differentAssumptionLevelSpecificationSubstitutionJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		short primaryAssumptionLevel;
		JustificationItem searchJustificationItem = firstJustificationItem_;
		SpecificationItem searchPrimarySpecificationItem;

		if( primarySpecificationItem != null &&
		secondarySpecificationItem != null )
			{
			primaryAssumptionLevel = primarySpecificationItem.assumptionLevel();

			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isSpecificationSubstitutionAssumption() &&
				searchJustificationItem.isOlderItem() &&
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem &&
				( searchPrimarySpecificationItem = searchJustificationItem.primarySpecificationItem() ) != null )
					{
					if( searchPrimarySpecificationItem.assumptionLevel() != primaryAssumptionLevel )
						return searchJustificationItem;
					}

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

	protected JustificationItem olderJustificationItem( short justificationTypeNr )
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

	protected JustificationItem primaryOrSecondarySpecificationJustificationItem( SpecificationItem referenceSpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( referenceSpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.primarySpecificationItem() == referenceSpecificationItem ||
				searchJustificationItem.secondarySpecificationItem() == referenceSpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem exclusiveNounSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isExclusiveSpecification() &&
			!searchSpecificationItem.isNegative() &&
			searchSpecificationItem.isSpecificationNoun() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
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

			( searchSpecificationItem.isSpecificationNoun() ||

			( isIncludingAdjectives &&
			searchSpecificationItem.isSpecificationAdjective() ) ) &&

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

	protected SpecificationItem relationSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasRelationContext() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem specificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
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
		short currentLanguageNr = CommonVariables.currentLanguageNr;
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

	protected SpecificationItem nextAssignmentOrSpecificationItem()
		{
		return ( isAssignment() ? nextAssignmentItemWithCurrentLevel() : nextSpecificationItem() );
		}

	protected SpecificationItem nextExclusiveNounSpecificationItem()
		{
		return exclusiveNounSpecificationItem( false );
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

	protected SpecificationItem nextRelationSpecificationItem()
		{
		return relationSpecificationItem( false );
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
		SpecificationItem searchSpecificationItem = ( isAssignment() ? assignmentItem( true, isIncludingThisItem, questionParameter_ ) : specificationItem( true, isIncludingThisItem, questionParameter_ ) );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasCurrentCreationSentenceNr() &&
			searchSpecificationItem.isAnsweredQuestion_ &&
			searchSpecificationItem.isSelfGenerated() )
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

	protected WordItem collectedWithItselfCommonWordItem()
		{
		WordItem commonWordItem;

		if( specificationWordItem_ != null &&
		( commonWordItem = specificationWordItem_.commonWordItem( specificationCollectionNr_ ) ) != null )
			{
			if( commonWordItem.isNounWordSpanishAmbiguous() )
				return commonWordItem;
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

		// Check all relation words
		if( ( firstRelationWordItem = contextWordItemInContextWords( relationContextNr_, specificationWordItem_, null ) ) != null )
			{
			// Found another relation word
			if( contextWordItemInContextWords( relationContextNr_, specificationWordItem_, firstRelationWordItem ) != null )
				return null;
			}

		return firstRelationWordItem;
		}

	protected WordItem relatedSpecificationWordItem( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr )
		{
		if( ( isIgnoringExclusive ||
		isExclusiveSpecification_ == isExclusiveSpecification ) &&

		( isIgnoringNegative ||
		isNegative_ == isNegative ) &&

		isPossessive_ == isPossessive &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&
		specificationContextNr_ == specificationContextNr &&

		( ( !isCheckingRelationContext &&
		!isHiddenSpanishSpecification() ) ||

		relationContextNr_ == relationContextNr ) )
			return specificationWordItem_;

		return null;
		}

	protected WordItem relatedSpecificationWordItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedSpecification, short assumptionLevel, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr )
		{
		if( isExclusiveSpecification_ == isExclusiveSpecification &&
		isNegative_ == isNegative &&
		isPossessive_ == isPossessive &&
		isSelfGenerated() == isSelfGeneratedSpecification &&
		specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&
		specificationContextNr_ == specificationContextNr &&
		relationContextNr_ == relationContextNr &&

		( assumptionLevel_ == assumptionLevel ||
		isQuestion() ) &&

		( specificationWordTypeNr == Constants.NO_WORD_TYPE_NR ||
		specificationWordTypeNr_ == specificationWordTypeNr ) )
			return specificationWordItem_;

		return null;
		}

	protected WordItem specificationWordItem()
		{
		return specificationWordItem_;
		}

	protected WordItem unwrittenProperNameRelationWordItem( WordItem previousWordItem )
		{
		// Start with previous word
		WordItem currentRelationWordItem = previousWordItem;

		// Check all relation words
		while( ( currentRelationWordItem = contextWordItemInContextWords( relationContextNr_, specificationWordItem_, currentRelationWordItem ) ) != null )
			{
			if( !currentRelationWordItem.isRelationWordTypeAlreadyWritten( Constants.WORD_TYPE_PROPER_NAME ) )
				return currentRelationWordItem;
			}

		return null;
		}

	protected ShortResultType recalculateAssumptionLevel()
		{
		short highestAssumptionLevel;
		short lowestAssumptionLevel = Constants.MAX_LEVEL;
		int tempAssumptionLevel;
		int nJustificationRelationWords;
		int nSpecificationRelationWords;
		JustificationItem currentJustificationItem;
		JustificationItem nextJustificationItem = firstJustificationItem_;
		ShortResultType shortResult = new ShortResultType();

		if( !isSpecificationGeneralization_ &&
		isSelfGeneratedAssumption() )
			{
			nSpecificationRelationWords = nRelationContextWords();

			do	{
				nJustificationRelationWords = 0;
				highestAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
				currentJustificationItem = nextJustificationItem;

				if( currentJustificationItem != null )
					{
					do	{
						nJustificationRelationWords += currentJustificationItem.nJustificationContextRelations( relationContextNr_, nSpecificationRelationWords );

						if( currentJustificationItem.isAssumptionJustification() )
							{
							if( ( shortResult = currentJustificationItem.getCombinedAssumptionLevel() ).result != Constants.RESULT_OK )
								return addShortResultError( 1, null, null, "I failed to get the combined assumption level" );

							// Add combined assumption level and justification grade
							if( ( tempAssumptionLevel = ( shortResult.shortValue + currentJustificationItem.justificationAssumptionGrade() ) ) > highestAssumptionLevel )
								{
								if( tempAssumptionLevel >= Constants.MAX_LEVEL )
									return startShortResultSystemError( 1, "Assumption level overflow" );

								highestAssumptionLevel = (short)tempAssumptionLevel;
								}
							}
						}
					while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );
					}

				if( highestAssumptionLevel < lowestAssumptionLevel &&
				nJustificationRelationWords == nSpecificationRelationWords )
					lowestAssumptionLevel = highestAssumptionLevel;
				}
			while( ( nextJustificationItem = nextJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != null );

			if( lowestAssumptionLevel < Constants.MAX_LEVEL &&
			// Assumption level has changed
			assumptionLevel_ != lowestAssumptionLevel )
				{
				assumptionLevel_ = lowestAssumptionLevel;

				if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL &&
				isOlderItem() )
					// Clear sentence write buffer
					lastWrittenSentenceStringBuffer = null;
				}
			}

		shortResult.shortValue = assumptionLevel_;
		return shortResult;
		}
	};

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his covenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
