/*	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
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

class SpecificationItem extends Item
	{
	// Private constructible variables

	private short nAssumptionLevelRecalculations_;

	// Private loadable variables

	private boolean isAnsweredQuestion_;
	private boolean isConcludedAssumption_;
	private boolean isConditional_;
	private boolean isCorrectedAssumption_;
	private boolean isEveryGeneralization_;
	private boolean isExclusiveSpecification_;
	private boolean isGeneralizationAssignment_;
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


	// Private constructible variables

	private short specificationStringWriteLevel_;

	private int lastCheckedAssumptionLevelItemNr_;


	// Private question methods

	private SpecificationItem newUserQuestion( boolean isIncludingThisItem )
		{
		SpecificationItem searchItem = ( isIncludingThisItem ? this : nextSelectedSpecificationItem() );

		while( searchItem != null )
			{
			if( !searchItem.isAnsweredQuestion_ &&
			!searchItem.isOlderItem() &&
			searchItem.isUserQuestion() )
				return searchItem;

			searchItem = searchItem.nextSelectedSpecificationItem();
			}

		return searchItem;
		}


	// Private specification methods

	private static boolean hasContextMaleWordInAllWords( int contextNr, WordItem specificationWordItem )
		{
		WordItem currentWordItem;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( currentWordItem.isMale() &&
				currentWordItem.hasContextInWord( contextNr, specificationWordItem ) )
					return true;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return false;
		}

	private SpecificationResultType calculateAssumptionLevel( boolean isNeedingRecalculation )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		short highestAssumptionLevel;
		short lowestAssumptionLevel = Constants.MAX_LEVEL;
		int tempAssumptionLevel;
		int nJustificationRelationWords;
		int nSpecificationRelationWords;
		JustificationItem currentJustificationItem;
		JustificationItem nextJustificationItem = firstJustificationItem_;

		if( !isSpecificationGeneralization_ &&
		isSelfGeneratedAssumption() )
			{
			if( ++nAssumptionLevelRecalculations_ < Constants.MAX_ASSUMPTION_LEVEL_RECALCULATIONS )
				{
				if( isNeedingRecalculation ||
				assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL )
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
									if( ( specificationResult = currentJustificationItem.getCombinedAssumptionLevel() ).result == Constants.RESULT_OK )
										{
										if( ( tempAssumptionLevel = ( specificationResult.combinedAssumptionLevel + currentJustificationItem.justificationAssumptionGrade() ) ) > highestAssumptionLevel )
											{
											if( tempAssumptionLevel < Constants.MAX_LEVEL )
												highestAssumptionLevel = (short)tempAssumptionLevel;
											else
												startSystemError( 1, null, "Assumption level overflow" );
											}
										}
									else
										addError( 1, null, "I failed to get the combined assumption level" );
									}
								}
							while( CommonVariables.result == Constants.RESULT_OK &&
							( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						highestAssumptionLevel < lowestAssumptionLevel &&

						( nJustificationRelationWords == nSpecificationRelationWords ||

						( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL &&
						// To avoid looping: calculation A, calculation B, calculation A, etc.
						lastCheckedAssumptionLevelItemNr_ == CommonVariables.currentItemNr ) ) )
							lowestAssumptionLevel = highestAssumptionLevel;
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					( nextJustificationItem = nextJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != null );

					if( CommonVariables.result == Constants.RESULT_OK )
						{
						if( lowestAssumptionLevel < Constants.MAX_LEVEL )
							{
							nAssumptionLevelRecalculations_ = 0;

							if( assumptionLevel_ != lowestAssumptionLevel )
								{
								assumptionLevel_ = lowestAssumptionLevel;

								if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
									// Clear sentence write buffer if assumption level is changed
									lastWrittenSentenceStringBuffer = null;
								}
							}

						lastCheckedAssumptionLevelItemNr_ = CommonVariables.currentItemNr;
						}
					}
				}
			else
				startError( 1, null, "There is probably an endless loop in the assumption level calculation of my specification, because the number of iterations is: " + nAssumptionLevelRecalculations_ );
			}

		specificationResult.assumptionLevel = assumptionLevel_;
		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}


	// Protected constructible variables

	protected boolean hasSpecificationBeenWrittenAsAnswer;
	protected boolean hasSpecificationBeenWrittenAsConflict;

	protected SpecificationItem replacingSpecificationItem;

	protected StringBuffer lastWrittenSentenceStringBuffer;
	protected StringBuffer lastWrittenSentenceWithOneSpecificationOnlyStringBuffer;


	// Constructor / deconstructor

	protected SpecificationItem( boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isLanguageWord, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, myList, myWordItem );

		// Private constructible variables

		nAssumptionLevelRecalculations_ = 0;

		// Private loadable variables

		isAnsweredQuestion_ = isAnsweredQuestion;
		isConcludedAssumption_ = isConcludedAssumption;
		isConditional_ = isConditional;
		isCorrectedAssumption_ = isCorrectedAssumption;
		isEveryGeneralization_ = isEveryGeneralization;
		isExclusiveSpecification_ = isExclusiveSpecification;
		isGeneralizationAssignment_ = isGeneralizationAssignment;
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

		specificationString_ = ( specificationString == null ? null : specificationString );


		// Private constructible variables

		specificationStringWriteLevel_ = Constants.NO_WRITE_LEVEL;
		lastCheckedAssumptionLevelItemNr_ = Constants.NO_ITEM_NR;

		// Protected constructible variables

		hasSpecificationBeenWrittenAsAnswer = false;
		hasSpecificationBeenWrittenAsConflict = false;

		replacingSpecificationItem = null;

		lastWrittenSentenceStringBuffer = null;
		lastWrittenSentenceWithOneSpecificationOnlyStringBuffer = null;
		}


	// Protected virtual methods

	protected void clearReplacingItem()
		{
		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.hasCurrentCreationSentenceNr() )
			replacingSpecificationItem = null;
		}

	protected void selectingAttachedJustifications( boolean isSelectingJustificationSpecifications )
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			searchItem.isSelectedByQuery = true;

			if( isSelectingJustificationSpecifications )
				searchItem.selectingJustificationSpecifications();

			searchItem = searchItem.attachedJustificationItem();
			}
		}

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( specificationString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( specificationString_ );
			}
		}

	protected void showWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( specificationWordItem_ != null &&
		( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasFoundParameter( int queryParameter )
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

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
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

	protected boolean hasFoundWordType( short queryWordTypeNr )
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
				creationSentenceNr() > nextSortItem.creationSentenceNr() ) ) );
		}

	protected byte checkForUsage()
		{
		return myWordItem().checkSpecificationForUsageOfInvolvedWords( this );
		}

	protected ReferenceResultType findMatchingWordReferenceString( String queryString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( specificationWordItem_ != null )
			{
			if( ( referenceResult = specificationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addError( 1, null, "I failed to find a matching word reference string for the specification word" );
			}

		return referenceResult;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String wordString;
		String languageNameString = myWordItem().languageNameString( languageNr_ );
		String generalizationWordTypeString = myWordItem().wordTypeNameString( generalizationWordTypeNr_ );
		String specificationWordTypeString = myWordItem().wordTypeNameString( specificationWordTypeNr_ );
		String relationWordTypeString = ( relationWordTypeNr_ == Constants.WORD_TYPE_UNDEFINED ? null : myWordItem().wordTypeNameString( relationWordTypeNr_ ) );

		baseToStringBuffer( queryWordTypeNr );

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

		if( isExclusiveSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecification" );

		if( isGeneralizationAssignment_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGeneralizationAssignment" );

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
/*
		if( lastCheckedAssumptionLevelItemNr_ > Constants.NO_ITEM_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "lastCheckedAssumptionLevelItemNr:" + lastCheckedAssumptionLevelItemNr_ );
*/
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


	// Protected assignment methods

	protected boolean isAssignment()
		{
		return ( myList().isAssignmentList() );
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

	protected boolean isGeneralizationAssignment()
		{
		return isGeneralizationAssignment_;
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
		return myWordItem().nContextWordsInAllWords( relationContextNr_, specificationWordItem_ );
		}

	protected SpecificationItem getAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem )
		{
		SpecificationItem searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != null )
			{
			// Skip replaced assignments
			if( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem.isAnsweredQuestion_ )
				return searchItem;

			searchItem = searchItem.nextAssignmentItemWithCurrentLevel();
			}

		return null;
		}

	protected SpecificationItem getAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		SpecificationItem searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != null )
			{
			if( searchItem.isQuestion() == isQuestion &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem.isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem.nextAssignmentItemWithCurrentLevel();
			}

		return null;
		}

	protected SpecificationItem getAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		SpecificationItem searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != null )
			{
			if( searchItem.questionParameter_ == questionParameter &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem.isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem.nextAssignmentItemWithCurrentLevel();
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

	protected void initializeSpecificationVariables()
		{
		// Private constructible variables

		lastCheckedAssumptionLevelItemNr_ = Constants.NO_ITEM_NR;

		// Protected constructible variables

		hasSpecificationBeenWrittenAsAnswer = false;
		hasSpecificationBeenWrittenAsConflict = false;
		}

	protected void markAsGeneralizationAssignment()
		{
		isGeneralizationAssignment_ = true;

		// Clear sentence write buffer
		lastWrittenSentenceStringBuffer = null;
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

	protected boolean hasExclusiveGeneralizationCollection()
		{
		return ( isExclusiveSpecification_ &&
				generalizationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasFoundReplacedOrDeletedJustification( boolean isAllowingNewerReplaceOrDeleteSentenceNr )
		{
		int currentSentenceNr = CommonVariables.currentSentenceNr;
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.isReplacedOrDeletedItem() &&

			( !isAllowingNewerReplaceOrDeleteSentenceNr ||

			( searchItem.replacedSentenceNr() < currentSentenceNr &&
			searchItem.deletedSentenceNr() < currentSentenceNr ) ) )
				return true;

			searchItem = searchItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasFoundDeletedJustification()
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.isDeletedItem() )
				return true;

			searchItem = searchItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasFoundJustification( boolean isIncludingReplacingJustification, JustificationItem searchJustificationItem )
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( searchJustificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem == searchJustificationItem ||

				( isIncludingReplacingJustification &&
				searchItem.replacingJustificationItem == searchJustificationItem ) )
					return true;

				searchItem = searchItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean hasFoundJustificationOfSameType( short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		int searchPrimarySpecificationCollectionNr;
		int searchSecondarySpecificationCollectionNr;
		int primarySpecificationCollectionNr = ( primarySpecificationItem == null ? Constants.NO_COLLECTION_NR : primarySpecificationItem.specificationCollectionNr() );
		int secondarySpecificationCollectionNr = ( secondarySpecificationItem == null ? Constants.NO_COLLECTION_NR : secondarySpecificationItem.specificationCollectionNr() );
		JustificationItem searchItem = firstJustificationItem_;
		SpecificationItem searchPrimarySpecificationItem;
		SpecificationItem searchSecondarySpecificationItem;

		while( searchItem != null )
			{
			if( searchItem.justificationTypeNr() == justificationTypeNr )
				{
				searchPrimarySpecificationItem = searchItem.primarySpecificationItem();
				searchSecondarySpecificationItem = searchItem.secondarySpecificationItem();

				searchPrimarySpecificationCollectionNr = ( searchPrimarySpecificationItem == null ? Constants.NO_COLLECTION_NR : searchPrimarySpecificationItem.specificationCollectionNr() );
				searchSecondarySpecificationCollectionNr = ( searchSecondarySpecificationItem == null ? Constants.NO_COLLECTION_NR : searchSecondarySpecificationItem.specificationCollectionNr() );

				if( searchPrimarySpecificationCollectionNr == primarySpecificationCollectionNr &&
				searchSecondarySpecificationCollectionNr == secondarySpecificationCollectionNr )
					return true;
				}

			searchItem = searchItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasNewInformation()
		{
		return ( !isOlderItem() ||
				// Has been inactivated during this sentence
				hasCurrentInactiveSentenceNr() ||
				// Has been archived during this sentence
				hasCurrentArchivedSentenceNr() ||

				( relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ != null &&
				myWordItem().hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr_, specificationWordItem_ ) ) );
		}

	protected boolean hasPrepositionParameter()
		{
		return ( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER );
		}

	protected boolean hasRelationContext()
		{
		return ( relationContextNr_ > Constants.NO_CONTEXT_NR );
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

	protected boolean hasUserSpecificationWordItem()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isUserSpecificationWord );
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
		SpecificationItem searchItem;
		WordItem currentSpecificationWordItem;
		WordItem foundWordItem = null;

		if( specificationWordItem_ != null &&
		isSpecificationNoun() &&
		( searchItem = myWordItem().firstSelectedSpecificationItem( isIncludingAnsweredQuestions, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != null )
			{
			do	{
				currentSpecificationWordItem = ( searchItem == this ? specificationWordItem_ : searchItem.relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) );

				if( currentSpecificationWordItem != null &&
				!currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
					foundWordItem = currentSpecificationWordItem;
				}
			while( foundWordItem == null &&
			( searchItem = searchItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions ) ) != null );

			return ( foundWordItem == null ? true : ( isDefiniteArticleParameter( articleParameter ) ? foundWordItem.isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem.isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	protected boolean isCorrectRelationArticle( short definiteArticleParameter )
		{
		boolean hasFoundUnwrittenWord = false;
		// Start to search first word in method contextWordInAllWords
		WordItem currentRelationWordItem = null;

		if( isRelationProperName() )
			{
			do	{
				if( ( currentRelationWordItem = relationWordItem( currentRelationWordItem ) ) != null )
					{
					if( !currentRelationWordItem.isRelationWordTypeAlreadyWritten( Constants.WORD_TYPE_PROPER_NAME ) )
						hasFoundUnwrittenWord = true;
					}
				}
			while( !hasFoundUnwrittenWord &&
			currentRelationWordItem != null );

			return ( currentRelationWordItem != null &&
					currentRelationWordItem.isProperNamePrecededByDefiniteArticle( definiteArticleParameter ) );
			}

		return false;
		}

	protected boolean isEveryGeneralization()
		{
		return isEveryGeneralization_;
		}

	protected boolean isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	protected boolean isHiddenSpanishSpecification()
		{
		return ( isSpecificationWordSpanishAmbiguous() &&
				!isNegative_ &&
				!isQuestion() &&
				isSelfGenerated() &&
				hasSpecificationNonCompoundCollection() &&
				myWordItem().isProperName() &&

				// Not possessive specification not in a male word
				( ( !isPossessive_ &&

				( isCorrectedAssumption_ ||
				!myWordItem().isMale() ) ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.compoundCollectionNr( specificationWordTypeNr_ ) > Constants.NO_COLLECTION_NR &&
				!hasContextMaleWordInAllWords( relationContextNr_, specificationWordItem_ ) ) ) );
		}

	protected boolean wasHiddenSpanishSpecification()
		{
		return ( lastWrittenSentenceStringBuffer == null &&
				isSpecificationWordSpanishAmbiguous() &&
				hasRelationContext() &&
				hasSpecificationNonCompoundCollection() );
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

	protected boolean isMatchingGeneralizationContextNr( boolean isAllowingEmptyContextResult, int generalizationContextNr )
		{
		// Empty subset
		if( generalizationContextNr == Constants.NO_CONTEXT_NR ||
		// Same set
		generalizationContextNr_ == generalizationContextNr ||

		( isAllowingEmptyContextResult &&
		generalizationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInAllWords( generalizationContextNr_, generalizationContextNr );
		}

	protected boolean isMatchingSpecificationContextNr( boolean isAllowingEmptyContextResult, int specificationContextNr )
		{
		// Empty subset
		if( specificationContextNr == Constants.NO_CONTEXT_NR ||
		// Same set
		specificationContextNr_ == specificationContextNr ||

		( isAllowingEmptyContextResult &&
		specificationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInAllWords( specificationContextNr_, specificationContextNr );
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

		return myWordItem().isContextSubsetInAllWords( relationContextNr_, relationContextNr );
		}

	protected boolean hasPrimarySpecificationJustification( SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( primarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.primarySpecificationItem() == primarySpecificationItem )
					return true;

				searchItem = searchItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean isSelfGeneratedAssumption()
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( !isConcludedAssumption_ &&
		questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != null )
			{
			if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
				return true;

			while( searchItem != null )
				{
				// At least one justification is an assumption
				if( searchItem.isAssumptionJustification() )
					return true;

				searchItem = searchItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean isSelfGeneratedConclusion()
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != null )
			{
			if( isConcludedAssumption_ )
				return true;

			while( searchItem != null )
				{
				// At least one justification is an assumption. So, it is not a conclusion
				if( searchItem.isAssumptionJustification() )
					return false;

				searchItem = searchItem.attachedJustificationItem();
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

	protected boolean isPossessiveReversibleConclusion()
		{
		return ( firstJustificationItem_ != null &&
				firstJustificationItem_.isPossessiveReversibleConclusion() );
		}

	protected boolean isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	protected boolean isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				!isAssignment() &&
				myWordItem().nContextWordsInAllWords( relationContextNr_, specificationWordItem_ ) > 1 );
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

	protected boolean isGeneralizationNoun()
		{
		return isSingularOrPluralNoun( generalizationWordTypeNr_ );
		}

	protected boolean isGeneralizationProperName()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean isSpecificationNoun()
		{
		return isSingularOrPluralNoun( specificationWordTypeNr_ );
		}

	protected boolean isSpecificationSingularNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isSpecificationPluralNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isRelationProperName()
		{
		return ( relationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME );
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

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				specificationWordItem != null &&
				specificationWordItem_ == specificationWordItem &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, int generalizationCollectionNr, int specificationCollectionNr, int relationContextNr )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				hasSpecificationCollection() &&
				generalizationCollectionNr_ == generalizationCollectionNr &&
				specificationCollectionNr_ == specificationCollectionNr &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	protected boolean isRelatedSpecification( boolean isNegative, boolean isPossessive, int generalizationCollectionNr, int specificationCollectionNr, int compoundSpecificationCollectionNr, WordItem specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				generalizationCollectionNr_ == generalizationCollectionNr &&

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

	protected short prepositionParameter()
		{
		return prepositionParameter_;
		}

	protected short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	protected short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	protected short relationWordTypeNr()
		{
		return relationWordTypeNr_;
		}

	protected short highestAttachedJustificationOrderNr( short justificationTypeNr )
		{
		short highestOrderNr = Constants.NO_ORDER_NR;
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.orderNr > highestOrderNr &&
			searchItem.justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchItem.orderNr;

			searchItem = searchItem.attachedJustificationItem();
			}

		return highestOrderNr;
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

	protected int nInvolvedSpecificationWords( boolean isCheckingForUserSpecificationWords )
		{
		boolean isSelfGeneratedSpecification = isSelfGenerated();
		int _creationSentenceNr = creationSentenceNr();
		int nInvolvedSpecificationWords = 0;
		SpecificationItem searchItem = myWordItem().firstSelectedSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchItem != null )
			{
			if( searchItem == this ||

			( searchItem.creationSentenceNr() == _creationSentenceNr &&
			searchItem.isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) ) )
				{
				if( isCheckingForUserSpecificationWords &&
				!searchItem.hasUserSpecificationWordItem() )
					return 0;

				nInvolvedSpecificationWords++;
				}

			searchItem = searchItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return nInvolvedSpecificationWords;
		}

	protected byte attachJustificationToSpecification( JustificationItem attachJustificationItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem firstJustificationItem;
		SpecificationItem createdSpecificationItem;

		if( attachJustificationItem != null )
			{
			if( attachJustificationItem.isActiveItem() )
				{
				if( !isReplacedOrDeletedItem() )
					{
					if( ( firstJustificationItem = firstJustificationItem_ ) != null )
						{
						if( hasCurrentCreationSentenceNr() )
							{
							if( changeFirstJustification( attachJustificationItem ) == Constants.RESULT_OK )
								{
								if( !myWordItem().hasCorrectedAssumptionByKnowledge() )
									{
									if( attachJustificationItem.attachJustification( firstJustificationItem, this ) != Constants.RESULT_OK )
										return addError( 1, null, "I failed to attach the first justification item of myself, to the given attached justification item" );
									}
								}
							else
								return addError( 1, null, "I failed to change my first justification item" );
							}
						else
							{
							if( ( specificationResult = myWordItem().createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isConcludedAssumption_, isConditional_, isCorrectedAssumption_, isEveryGeneralization_, isExclusiveSpecification_, isGeneralizationAssignment_, isNegative_, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr_, prepositionParameter_, questionParameter_, generalizationWordTypeNr_, specificationWordTypeNr_, relationWordTypeNr_, generalizationCollectionNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, lastWrittenSentenceStringBuffer ) ).result == Constants.RESULT_OK )
								{
								if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
									{
									if( attachJustificationItem.attachJustification( firstJustificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
										{
										if( myWordItem().replaceOrDeleteSpecification( this, createdSpecificationItem ) != Constants.RESULT_OK )
											return addError( 1, null, "I failed to replace or delete a specification" );
										}
									else
										return addError( 1, null, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );
									}
								else
									return startError( 1, null, "I couldn't create a specification" );
								}
							else
								return addError( 1, null, "I failed to create a copy of myself with a different first justification item" );
							}
						}
					else
						return startError( 1, null, "I have no justification items" );
					}
				else
					return startError( 1, null, "I am replaced or deleted" );
				}
			else
				return startError( 1, null, "The given attached justification item isn't active" );
			}
		else
			return startError( 1, null, "The given attached justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte changeFirstJustification( JustificationItem replacingJustificationItem )
		{
		if( replacingJustificationItem != null )
			{
			if( replacingJustificationItem.isActiveItem() )
				{
				if( hasCurrentCreationSentenceNr() )
					firstJustificationItem_ = replacingJustificationItem;
				else
					return startError( 1, null, "It isn't allowed to change an older item afterwards" );
				}
			else
				return startError( 1, null, "The given replacing justification item isn't active" );
			}
		else
			return startError( 1, null, "The given replacing justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte collectSpecification( boolean isCollectGeneralization, boolean isExclusiveSpecification, int collectionNr )
		{
		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			if( hasCurrentCreationSentenceNr() )
				{
				if( isExclusiveSpecification )
					isExclusiveSpecification_ = true;

				if( isCollectGeneralization )
					{
					if( generalizationCollectionNr_ == Constants.NO_COLLECTION_NR )
						generalizationCollectionNr_ = collectionNr;
					else
						return startError( 1, null, "The generalization is already collected" );
					}
				else
					{
					if( specificationCollectionNr_ == Constants.NO_COLLECTION_NR )
						specificationCollectionNr_ = collectionNr;
					else
						return startError( 1, null, "The specification is already collected" );
					}
				}
			else
				return startError( 1, null, "It isn't allowed to change an older item afterwards" );
			}
		else
			return startError( 1, null, "The given collection number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte markAsConcludedAssumption()
		{
		if( isSelfGeneratedAssumption() )
			{
			isConcludedAssumption_ = true;
			assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

			// Clear sentence write buffer to lose the uncertainty word written in the sentence
			lastWrittenSentenceStringBuffer = null;
			}
		else
			return startError( 1, null, "I am not a self-generated assumption" );

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationStringAsWritten()
		{
		if( CommonVariables.currentWriteLevel < Constants.MAX_LEVEL )
			{
			if( specificationStringWriteLevel_ == Constants.NO_WRITE_LEVEL )
				specificationStringWriteLevel_ = ++CommonVariables.currentWriteLevel;
			else
				return startError( 1, null, "My write level is already assigned" );
			}
		else
			return startSystemError( 1, null, "Current write word level overflow" );

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumption()
		{
		SpecificationResultType specificationResult;
		boolean isAdjustedSpecification = false;
		short previousAssumptionLevel;

		if( ( specificationResult = getAssumptionLevel() ).result == Constants.RESULT_OK )
			{
			previousAssumptionLevel = specificationResult.assumptionLevel;

			if( firstJustificationItem_ != null &&
			firstJustificationItem_.hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition() )
				{
				if( markAsConcludedAssumption() == Constants.RESULT_OK )
					isAdjustedSpecification = true;
				else
					return addError( 1, null, "I failed to mark myself as a concluded assumption" );
				}
			else
				{
				// Avoid corrected assumption to be concluded if Spanish ambiguous
				if( isCorrectedAssumption_ ||
				specificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
				!myWordItem().hasCorrectedAssumptionByKnowledge() )
					{
					if( ( specificationResult = recalculateAssumptionLevel() ).result == Constants.RESULT_OK )
						{
						if( isOlderItem() &&
						specificationResult.assumptionLevel != previousAssumptionLevel &&

						// Avoid updates of high uncertainty mapping,
						// from 'maybe' of one level to 'maybe' of another level
						( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
						specificationResult.assumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
							{
							if( specificationResult.assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
								{
								if( markAsConcludedAssumption() == Constants.RESULT_OK )
									isAdjustedSpecification = true;
								else
									return addError( 1, null, "After recalculation, I failed to mark myself as concluded assumption" );
								}
							else
								{
								if( !isPossessive_ &&
								specificationResult.assumptionLevel < previousAssumptionLevel &&
								myWordItem().isUserGeneralizationWord )
									isAdjustedSpecification = true;
								}
							}
						}
					else
						return addError( 1, null, "I failed to recalculate the assumption level" );
					}
				}

			if( isAdjustedSpecification &&
			!isHiddenSpanishSpecification() )
				{
				// Write adjusted specification
				if( myWordItem().writeUpdatedSpecification( true, false, false, false, this ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to write an adjusted specification" );
				}
			}
		else
			return addError( 1, null, "I failed to get the assumption level" );

		return Constants.RESULT_OK;
		}

	protected byte writeSpecificationConflict()
		{
		if( myWordItem().writeSelectedSpecification( false, false, this ) == Constants.RESULT_OK )
			{
			if( CommonVariables.writtenSentenceStringBuffer != null &&
			CommonVariables.writtenSentenceStringBuffer.length() > 0 )
				{
				if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, ( isOlderItem() ? Constants.INTERFACE_LISTING_CONFLICT : Constants.INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) == Constants.RESULT_OK )
					{
					if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) == Constants.RESULT_OK )
						hasSpecificationBeenWrittenAsConflict = true;
					else
						return addError( 1, null, "I failed to write the conflict sentence" );
					}
				else
					return addError( 1, null, "I failed to write an interface warning" );
				}
			else
				return addError( 1, null, "I couldn't write the conflicting specification" );
			}
		else
			return addError( 1, null, "I failed to write the conflicting specification" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType getAssumptionLevel()
		{
		return calculateAssumptionLevel( false );
		}

	protected SpecificationResultType recalculateAssumptionLevel()
		{
		nAssumptionLevelRecalculations_ = 0;
		return calculateAssumptionLevel( true );
		}

	protected JustificationItem answeredQuestionJustificationItem()
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.hasPrimaryAnsweredQuestion() )
				return searchItem;

			searchItem = searchItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem differentAssumptionLevelSpecificationSubstitutionJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		short primaryAssumptionLevel;
		JustificationItem searchItem = firstJustificationItem_;
		SpecificationItem searchPrimarySpecificationItem;

		if( primarySpecificationItem != null &&
		secondarySpecificationItem != null )
			{
			primaryAssumptionLevel = primarySpecificationItem.assumptionLevel();

			while( searchItem != null )
				{
				if( searchItem.isSpecificationSubstitutionAssumption() &&
				searchItem.isOlderItem() &&
				searchItem.secondarySpecificationItem() == secondarySpecificationItem &&
				( searchPrimarySpecificationItem = searchItem.primarySpecificationItem() ) != null )
					{
					if( searchPrimarySpecificationItem.assumptionLevel() != primaryAssumptionLevel )
						return searchItem;
					}

				searchItem = searchItem.attachedJustificationItem();
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
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.justificationTypeNr() == justificationTypeNr )
				return searchItem;

			searchItem = searchItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem olderJustificationItem( short justificationTypeNr )
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.isOlderItem() &&
			searchItem.justificationTypeNr() == justificationTypeNr )
				return searchItem;

			searchItem = searchItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem primaryOrSecondarySpecificationJustificationItem( SpecificationItem referenceSpecificationItem )
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( referenceSpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.primarySpecificationItem() == referenceSpecificationItem ||
				searchItem.secondarySpecificationItem() == referenceSpecificationItem )
					return searchItem;

				searchItem = searchItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem nextSpecificationItem()
		{
		return (SpecificationItem)nextItem;
		}

	protected SpecificationItem nextAssignmentOrSpecificationItem()
		{
		return ( isAssignment() ? nextAssignmentItemWithCurrentLevel() : nextSpecificationItem() );
		}

	protected SpecificationItem updatedSpecificationItem()
		{
		SpecificationItem updatedSpecificationItem;
		SpecificationItem searchItem = this;

		while( ( updatedSpecificationItem = searchItem.replacingSpecificationItem ) != null )
			searchItem = updatedSpecificationItem;

		return searchItem;
		}

	protected SpecificationItem getSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		SpecificationItem searchItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchItem != null )
			{
			if( searchItem.isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem.languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem.isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem getSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		SpecificationItem searchItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchItem != null )
			{
			if( searchItem.questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem.languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem.isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, isQuestion() ) : getSpecificationItem( false, false, isQuestion() ) );
		}

	protected SpecificationItem nextSelectedQuestionParameterSpecificationItem()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, questionParameter_ ) : getSpecificationItem( false, false, questionParameter_ ) );
		}

	protected SpecificationItem nextSelectedQuestionParameterSpecificationItem( boolean isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? getAssignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) : getSpecificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	protected SpecificationItem recentlyAnsweredQuestionSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchItem = ( isAssignment() ? getAssignmentItem( true, isIncludingThisItem, questionParameter_ ) : getSpecificationItem( true, isIncludingThisItem, questionParameter_ ) );

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			searchItem.isAnsweredQuestion_ &&
			searchItem.isSelfGenerated() )
				return searchItem;

			searchItem = searchItem.nextSelectedQuestionParameterSpecificationItem( true );
			}

		return null;
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

	protected WordItem specificationWordItem()
		{
		return specificationWordItem_;
		}

	protected WordItem relationWordItem()
		{
		return relationWordItem( null );
		}

	protected WordItem relationWordItem( WordItem previousWordItem )
		{
		return myWordItem().contextWordItemInAllWords( relationContextNr_, specificationWordItem_, previousWordItem );
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
		if( hasSpecificationCollection() &&
		isExclusiveSpecification_ == isExclusiveSpecification &&
		isNegative_ == isNegative &&
		isPossessive_ == isPossessive &&
		isSelfGenerated() == isSelfGeneratedSpecification &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&
		specificationContextNr_ == specificationContextNr &&
		relationContextNr_ == relationContextNr &&

		( assumptionLevel_ == assumptionLevel ||
		isQuestion() ) &&

		( specificationWordTypeNr == Constants.WORD_TYPE_UNDEFINED ||
		specificationWordTypeNr_ == specificationWordTypeNr ) )
			return specificationWordItem_;

		return null;
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
	};

/*************************************************************************
 *	"All he does is just and good,
 *	and all his commandments are trustworthy." (Psalm 111:7)
 *************************************************************************/
