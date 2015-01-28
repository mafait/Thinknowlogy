/*
 *	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
	private boolean isUniqueRelation_;
	private boolean isValueSpecification_;

	private short assignmentLevel_;
	private short assumptionLevel_;
	private short grammarLanguageNr_;
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

	private boolean hasExclusiveSpecificationSubstitutionAssumption()
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.isExclusiveSpecificationSubstitutionAssumption() )
				return true;

			searchItem = searchItem.attachedJustificationItem();
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
					specificationResult.hasPerformedRecalculation = true;
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
												startSystemErrorInItem( 1, null, myWordItem().anyWordTypeString(), "Assumption level overflow" );
											}
										}
									else
										addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to get the combined assumption level" );
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
				startErrorInItem( 1, null, "There is probably an endless loop in the assumption level calculation of my specification, because the number of iterations is: " + nAssumptionLevelRecalculations_ );
			}

		specificationResult.assumptionLevel = assumptionLevel_;
		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}


	// Protected constructible variables

	protected boolean hasAlreadyBeenWrittenAsAnswer;
	protected boolean hasAlreadyBeenWrittenAsConflict;

	protected SpecificationItem replacingSpecificationItem;

	protected StringBuffer lastWrittenSentenceStringBuffer;
	protected StringBuffer lastWrittenSentenceWithOneSpecificationOnlyStringBuffer;


	// Constructor / deconstructor

	protected SpecificationItem( boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isLanguageWord, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short grammarLanguageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, List myList, WordItem myWordItem )
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
		isUniqueRelation_ = isUniqueRelation;
		isValueSpecification_ = isValueSpecification;

		assignmentLevel_ = assignmentLevel;
		assumptionLevel_ = assumptionLevel;
		grammarLanguageNr_ = grammarLanguageNr;
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

		hasAlreadyBeenWrittenAsAnswer = false;
		hasAlreadyBeenWrittenAsConflict = false;

		replacingSpecificationItem = null;

		lastWrittenSentenceStringBuffer = null;
		lastWrittenSentenceWithOneSpecificationOnlyStringBuffer = null;
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( specificationString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			if( !isActiveItem() )	// Show status if not active
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
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			if( !isActiveItem() )	// Show status if not active
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
				addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to find a matching word reference string for the specification word" );
			}

		return referenceResult;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		String wordString;
		String languageNameString = myWordItem().grammarLanguageNameString( grammarLanguageNr_ );
		String generalizationWordTypeString = myWordItem().wordTypeNameString( generalizationWordTypeNr_ );
		String specificationWordTypeString = myWordItem().wordTypeNameString( specificationWordTypeNr_ );
		String relationWordTypeString = ( relationWordTypeNr_ == Constants.WORD_TYPE_UNDEFINED ? null : myWordItem().wordTypeNameString( relationWordTypeNr_ ) );

		baseToStringBuffer( queryWordTypeNr );

		if( grammarLanguageNr_ > Constants.NO_LANGUAGE_NR )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + ( languageNameString == null ? ( "grammarLanguageNr:" + grammarLanguageNr_ ) : ( "grammarLanguage:" + languageNameString ) ) );
			}

		if( isAnsweredQuestion_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAnsweredQuestion" );

		if( isConcludedAssumption_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isConcludedAssumption" );

		if( isConditional_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isConditional" );

		if( isCorrectedAssumption_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isCorrectedAssumption" );

		if( isEveryGeneralization_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isEveryGeneralization" );

		if( isExclusiveSpecification_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecification" );

		if( isGeneralizationAssignment_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGeneralizationAssignment" );

		if( isLanguageWord_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isNegative_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegative" );

		if( isPartOf_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPartOf" );

		if( isPossessive_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessive" );

		if( isSpecificationGeneralization_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralization" );

		if( isUniqueRelation_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueRelation" );

		if( isValueSpecification_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isValueSpecification" );

		if( assignmentLevel_ > Constants.NO_ASSIGNMENT_LEVEL )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assignmentLevel:" + assignmentLevel_ );

		if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assumptionLevel:" + assumptionLevel_ );

		if( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationStringWriteLevel:" + specificationStringWriteLevel_ );

		if( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "prepositionParameter:" + prepositionParameter_ );

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "questionParameter:" + questionParameter_ );
/*
		if( lastCheckedAssumptionLevelItemNr_ > Constants.NO_ITEM_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "lastCheckedAssumptionLevelItemNr:" + lastCheckedAssumptionLevelItemNr_ );
*/
		if( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationCollectionNr:" + generalizationCollectionNr_ );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordType:" + ( generalizationWordTypeString == null ? Constants.EMPTY_STRING : generalizationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + generalizationWordTypeNr_ );

		if( generalizationContextNr_ > Constants.NO_CONTEXT_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationContextNr:" + generalizationContextNr_ );

		if( specificationCollectionNr_ > Constants.NO_COLLECTION_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationCollectionNr:" + specificationCollectionNr_ );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordType:" + ( specificationWordTypeString == null ? Constants.EMPTY_STRING : specificationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + specificationWordTypeNr_ );

		if( specificationContextNr_ > Constants.NO_CONTEXT_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationContextNr:" + specificationContextNr_ );

		if( specificationWordItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + specificationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + specificationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
				CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( relationWordTypeString != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordType:" + relationWordTypeString + Constants.QUERY_WORD_TYPE_STRING + relationWordTypeNr_ );

		if( relationContextNr_ > Constants.NO_CONTEXT_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationContextNr:" + relationContextNr_ );

		if( nContextRelations_ > Constants.NO_CONTEXT_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nContextRelations:" + nContextRelations_ );

		if( firstJustificationItem_ != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "firstJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + firstJustificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + firstJustificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( replacingSpecificationItem != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacingSpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + replacingSpecificationItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + replacingSpecificationItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( specificationString_ != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationString:" + Constants.QUERY_STRING_START_CHAR + specificationString_ + Constants.QUERY_STRING_END_CHAR );

		return CommonVariables.queryStringBuffer;
		}


	// Protected assignment methods

	protected boolean isAssignment()
		{
		return ( myList().isAssignmentList() );
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

	protected short grammarLanguageNr()
		{
		return grammarLanguageNr_;
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
		short currentGrammarLanguageNr = CommonVariables.currentGrammarLanguageNr;
		SpecificationItem searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != null )
			{
			if( searchItem.isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem.grammarLanguageNr_ == currentGrammarLanguageNr ) &&

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
		short currentGrammarLanguageNr = CommonVariables.currentGrammarLanguageNr;
		SpecificationItem searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == CommonVariables.currentAssignmentLevel ? this : null ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != null )
			{
			if( searchItem.questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem.grammarLanguageNr_ == currentGrammarLanguageNr ) &&

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

	protected void clearLastCheckedAssumptionLevelItemNr()
		{
		lastCheckedAssumptionLevelItemNr_ = Constants.NO_ITEM_NR;
		}

	protected void clearSpecificationStringWriteLevel( short currentWriteLevel )
		{
		if( specificationStringWriteLevel_ > currentWriteLevel )
			specificationStringWriteLevel_ = Constants.NO_WRITE_LEVEL;
		}

	protected void markAsGeneralizationAssignment()
		{
		isGeneralizationAssignment_ = true;

		// Clear sentence write buffer
		lastWrittenSentenceStringBuffer = null;
		}

	protected boolean hasAssumptionLevel()
		{
		return ( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL );
		}

	protected boolean hasNewInformation()
		{
		return ( !isOlderItem() ||
				hasCurrentInactiveSentenceNr() ||	// Has been inactivated during this sentence
				hasCurrentArchivedSentenceNr() ||	// Has been archived during this sentence

				( relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ != null &&
				myWordItem().hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr_, specificationWordItem_ ) ) );
		}

	protected boolean hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasExclusiveGeneralizationCollection()
		{
		return ( isExclusiveSpecification_ &&
				generalizationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasExclusiveSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isExclusiveCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasFeminineOrMasculineSpecificationWord()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isFeminineOrMasculineWord() );
		}

	protected boolean hasFoundReplacedOrDeletedJustification()
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.isReplacedOrDeletedItem() )
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

	protected boolean hasFoundJustificationWithReplacedPrimarySpecification()
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.hasReplacedPrimarySpecification() )
				return true;

			searchItem = searchItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasFoundJustification( boolean isIncludingReplacingJustification, JustificationItem searchJustificationItem )
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem == searchJustificationItem ||

			( isIncludingReplacingJustification &&
			searchItem.replacingJustificationItem == searchJustificationItem ) )
				return true;

			searchItem = searchItem.attachedJustificationItem();
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

		while( searchItem != null )
			{
			if( searchItem.justificationTypeNr() == justificationTypeNr )
				{
				searchPrimarySpecificationCollectionNr = ( searchItem.primarySpecificationItem() == null ? Constants.NO_COLLECTION_NR : searchItem.primarySpecificationItem().specificationCollectionNr() );
				searchSecondarySpecificationCollectionNr = ( searchItem.secondarySpecificationItem() == null ? Constants.NO_COLLECTION_NR : searchItem.secondarySpecificationItem().specificationCollectionNr() );

				if( searchPrimarySpecificationCollectionNr == primarySpecificationCollectionNr &&
				searchSecondarySpecificationCollectionNr == secondarySpecificationCollectionNr )
					return true;
				}

			searchItem = searchItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasGeneralizationContext()
		{
		return ( generalizationContextNr_ > Constants.NO_CONTEXT_NR );
		}

	protected boolean hasHiddenSpecificationWord()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.activeWordTypeString( specificationWordTypeNr_ ) == null );
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

	protected boolean hasSpecificationContext()
		{
		return ( specificationContextNr_ > Constants.NO_CONTEXT_NR );
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

	protected boolean isCorrectSpecificationArticle( boolean isFeminineWord, boolean isMasculineWord, short articleParameter )
		{
		boolean hasFoundUnwrittenWord = false;
		boolean isIncludingAnsweredQuestions = isAnsweredQuestion();
		boolean _isSelfGenerated = isSelfGenerated();
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem foundWordItem = null;

		if( isSpecificationNoun() &&
		( currentSpecificationItem = myWordItem().firstSelectedSpecificationItem( isIncludingAnsweredQuestions, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != null )
			{
			do	{
				if( currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, _isSelfGenerated, assumptionLevel_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) )
					{
					if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
						{
						if( !currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
							{
							hasFoundUnwrittenWord = true;
							foundWordItem = currentSpecificationWordItem;
							}
						}
					}
				else	// No more specifications
					foundWordItem = specificationWordItem_;
				}
			while( !hasFoundUnwrittenWord &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions ) ) != null );

			return ( foundWordItem == null ? true : ( isDefiniteArticleParameter( articleParameter ) ? foundWordItem.isCorrectDefiniteArticle( isFeminineWord, isMasculineWord, articleParameter, specificationWordTypeNr_ ) : foundWordItem.isCorrectIndefiniteArticle( isFeminineWord, isMasculineWord, articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	protected boolean isCorrectRelationArticle( short definiteArticleParameter )
		{
		boolean hasFoundUnwrittenWord = false;
		WordItem currentRelationWordItem = null;	// Start to search first word in method contextWordInAllWords

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

	protected boolean isHiddenSpecification()
		{
		return ( !isNegative_ &&
				isSelfGeneratedAssumption() &&
				isSpecificationWordCollectedWithItself() );
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
		if( generalizationContextNr == Constants.NO_CONTEXT_NR ||			// Empty subset
		generalizationContextNr_ == generalizationContextNr ||	// Same set

		( isAllowingEmptyContextResult &&
		generalizationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInAllWords( generalizationContextNr_, generalizationContextNr );
		}

	protected boolean isMatchingSpecificationContextNr( boolean isAllowingEmptyContextResult, int specificationContextNr )
		{
		if( specificationContextNr == Constants.NO_CONTEXT_NR ||			// Empty subset
		specificationContextNr_ == specificationContextNr ||	// Same set

		( isAllowingEmptyContextResult &&
		specificationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInAllWords( specificationContextNr_, specificationContextNr );
		}

	protected boolean isMatchingRelationContextNr( boolean isAllowingEmptyContextResult, int relationContextNr )
		{
		if( relationContextNr == Constants.NO_CONTEXT_NR ||		// Empty subset
		relationContextNr_ == relationContextNr ||		// Same set

		( isAllowingEmptyContextResult &&
		relationContextNr_ == Constants.NO_CONTEXT_NR ) )
			return true;

		return myWordItem().isContextSubsetInAllWords( relationContextNr_, relationContextNr );
		}

	protected boolean isSelfGeneratedAssumption()
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( !isConcludedAssumption_ &&
		firstJustificationItem_ != null )
			{
			if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL &&
			questionParameter_ == Constants.NO_QUESTION_PARAMETER )
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

		if( isConcludedAssumption_ )
			return true;

		if( firstJustificationItem_ != null )
			{
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

	protected boolean isSpecificationWordCollectedWithItself()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isCollectedWithItself() );
		}

	protected boolean isUniqueRelation()
		{
		return isUniqueRelation_;
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

	protected boolean isGeneralizationPluralNoun()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationNoun()
		{
		return isSingularOrPluralNoun( specificationWordTypeNr_ );
		}

	protected boolean isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isRelationNoun()
		{
		return isSingularOrPluralNoun( relationWordTypeNr_ );
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

				( specificationCollectionNr_ == specificationCollectionNr ||

				( compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == compoundSpecificationCollectionNr ) ) &&

				( hasSpecificationCollection() ||
				specificationWordItem_ == specificationWordItem ) );
		}

	protected boolean isRelatedSpecification( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean _isSelfGenerated, short assumptionLevel, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr )
		{
		return ( hasSpecificationCollection() &&
				isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGenerated() == _isSelfGenerated &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				specificationContextNr_ == specificationContextNr &&
				relationContextNr_ == relationContextNr &&

				( assumptionLevel_ == assumptionLevel ||
				isQuestion() ) );
		}

	protected boolean isRelatedSpecification( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr )
		{
		return ( ( isIgnoringExclusive ||
				isExclusiveSpecification_ == isExclusiveSpecification ) &&

				( isIgnoringNegative ||
				isNegative_ == isNegative ) &&

				isPossessive_ == isPossessive &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				specificationContextNr_ == specificationContextNr &&

				( ( !isCheckingRelationContext &&
				!isHiddenSpecification() ) ||

				relationContextNr_ == relationContextNr ) );
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

	protected int nInvolvedSpecificationWords( boolean isCheckingForSameCreationSentenceNr, boolean isCheckingForUserSpecificationWords )
		{
		boolean _isSelfGenerated = isSelfGenerated();
		int _creationSentenceNr = creationSentenceNr();
		int nInvolvedSpecificationWords = 0;
		SpecificationItem searchItem = myWordItem().firstSelectedSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchItem != null )
			{
			if( searchItem == this ||

			( ( !isCheckingForSameCreationSentenceNr ||
			searchItem.creationSentenceNr() == _creationSentenceNr ) &&

			searchItem.isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, _isSelfGenerated, assumptionLevel_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) ) )
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

	protected byte addFeminineOrMasculineContext( WordItem contextWordItem )
		{
		int relationContextNr;
		WordItem specificationWordItem;
		JustificationItem searchItem = firstJustificationItem_;
		SpecificationItem primarySpecificationItem;

		if( contextWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isSpecificationSubstitutionAssumption() &&
				( primarySpecificationItem = searchItem.primarySpecificationItem() ) != null )
					{
					if( primarySpecificationItem.hasCurrentActiveSentenceNr() &&
					( relationContextNr = primarySpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
					( specificationWordItem = primarySpecificationItem.specificationWordItem() ) != null )
						{
						if( specificationWordItem.isFeminineOrMasculineWord() &&
						// If context doesn't exist
						!contextWordItem.hasContextInWord( relationContextNr, specificationWordItem ) )
							{
							if( contextWordItem.addContext( false, primarySpecificationItem.relationWordTypeNr(), primarySpecificationItem.specificationWordTypeNr(), relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
								return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to add a context" );
							}
						}
					}

				searchItem = searchItem.attachedJustificationItem();
				}
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given context word item is undefined" );

		return Constants.RESULT_OK;
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
								if( !firstJustificationItem.isDeletedItem() )
									{
									if( attachJustificationItem.attachJustification( firstJustificationItem, this ) != Constants.RESULT_OK )
										return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to attach the first justification item to the given attached justification item" );
									}
								}
							else
								return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to change my first justification item" );
							}
						else
							{
							if( ( specificationResult = myWordItem().createSpecification( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isConcludedAssumption_, isConditional_, isCorrectedAssumption_, isEveryGeneralization_, isExclusiveSpecification_, isGeneralizationAssignment_, isNegative_, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, grammarLanguageNr_, prepositionParameter_, questionParameter_, generalizationWordTypeNr_, specificationWordTypeNr_, relationWordTypeNr_, generalizationCollectionNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, lastWrittenSentenceStringBuffer ) ).result == Constants.RESULT_OK )
								{
								if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
									{
									if( attachJustificationItem.attachJustification( firstJustificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
										{
										if( myWordItem().replaceOrDeleteSpecification( this, createdSpecificationItem ) != Constants.RESULT_OK )
											return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to replace or delete a specification" );
										}
									else
										return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to attach the first justification item to the given attached justification item of the created specification item" );
									}
								else
									return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I couldn't create a specification" );
								}
							else
								return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to create a copy of myself with a different first justification item" );
							}
						}
					else
						return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I have no justification items" );
					}
				else
					return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I am replaced or deleted" );
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item isn't active" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item is undefined" );

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
					return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item isn't active" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item is undefined" );

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
						return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The generalization is already collected" );
					}
				else
					{
					if( specificationCollectionNr_ == Constants.NO_COLLECTION_NR )
						specificationCollectionNr_ = collectionNr;
					else
						return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The specification is already collected" );
					}
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given collection number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte markAsConcludedAssumption( boolean isSkippingSelfGeneratedAssumptionCheck )
		{
		if( isSkippingSelfGeneratedAssumptionCheck ||
		isSelfGeneratedAssumption() )
			{
			isConcludedAssumption_ = true;
			assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

			// Clear sentence write buffer to lose the uncertainty word
			lastWrittenSentenceStringBuffer = null;
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I am not a self-generated assumption" );

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationStringAsWritten()
		{
		if( CommonVariables.currentWriteLevel < Constants.MAX_LEVEL )
			{
			if( specificationStringWriteLevel_ == Constants.NO_WRITE_LEVEL )
				specificationStringWriteLevel_ = ++CommonVariables.currentWriteLevel;
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My write level is already assigned" );
			}
		else
			return startSystemErrorInItem( 1, null, myWordItem().anyWordTypeString(), "Current write word level overflow" );

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
				if( markAsConcludedAssumption( true ) == Constants.RESULT_OK )
					isAdjustedSpecification = true;
				else
					return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to mark myself as a concluded assumption" );
				}
			else
				{
				if( !specificationResult.hasPerformedRecalculation &&

				// Avoid corrected assumption to be concluded if collected with itself
				( isCorrectedAssumption_ ||
				specificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
				!myWordItem().hasCorrectedAssumptionByKnowledge() ) )
					{
					if( ( specificationResult = recalculateAssumptionLevel() ).result == Constants.RESULT_OK )
						{
						if( isOlderItem() &&
						specificationResult.assumptionLevel != previousAssumptionLevel &&

						// Avoid updates of high uncertainty mapping,
						// from "may be" of one level to "may be" of another level
						( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
						specificationResult.assumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
							{
							if( specificationResult.assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
								{
								if( markAsConcludedAssumption( true ) == Constants.RESULT_OK )
									{
									if( ( specificationResult = myWordItem().findRelatedSpecification( false, this ) ).result == Constants.RESULT_OK )
										{
										if( specificationResult.relatedSpecificationItem == null ||
										specificationResult.relatedSpecificationItem.isConcludedAssumption() )
											isAdjustedSpecification = true;
										}
									else
										return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to find out if a search specification is related" );
									}
								else
									return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "After recalculation, I failed to mark myself as concluded assumption" );
								}
							else
								{
								if( specificationResult.assumptionLevel < previousAssumptionLevel &&
								myWordItem().isUserGeneralizationWord &&

								( relationContextNr_ == Constants.NO_CONTEXT_NR ||
								!hasExclusiveSpecificationSubstitutionAssumption() ) )
									isAdjustedSpecification = true;
								}
							}
						}
					else
						return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to recalculate the assumption level" );
					}
				}

			if( isAdjustedSpecification )
				{
				// Write adjusted specification
				if( myWordItem().writeUpdatedSpecification( true, false, false, this ) != Constants.RESULT_OK )
					return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to write an adjusted specification" );
				}
			}
		else
			return addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to get the assumption level" );

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

	protected JustificationItem firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	protected JustificationItem olderGeneralizationAssumptionBySpecificationJustificationItem()
		{
		JustificationItem searchItem = firstJustificationItem_;

		while( searchItem != null )
			{
			if( searchItem.isOlderItem() &&
			searchItem.isGeneralizationAssumption() )
				return searchItem;

			searchItem = searchItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem oppositePossessiveConditionalSpecificationAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( secondarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isOppositePossessiveConditionalSpecificationAssumption() &&
				searchItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem.attachedJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem possessiveReversibleAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstJustificationItem_;

		if( secondarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isPossessiveReversibleAssumption() &&
				searchItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem.attachedJustificationItem();
				}
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
		return (SpecificationItem)nextItem ;
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
		short currentGrammarLanguageNr = CommonVariables.currentGrammarLanguageNr;
		SpecificationItem searchItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchItem != null )
			{
			if( searchItem.isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem.grammarLanguageNr_ == currentGrammarLanguageNr ) &&

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
		short currentGrammarLanguageNr = CommonVariables.currentGrammarLanguageNr;
		SpecificationItem searchItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchItem != null )
			{
			if( searchItem.questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem.grammarLanguageNr_ == currentGrammarLanguageNr ) &&

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
			if( commonWordItem.isCollectedWithItself() )
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
