/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
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

class WordItem extends Item
	{
	// Private constructed variables

	private boolean isNounWordSpanishAmbiguous_;

	private	String authorizationKey_;

	// Word item supporting modules
	private WordQuestion wordQuestion_;
	private WordSpecification wordSpecification_;
	private WordWrite wordWrite_;

	// Word item lists
	private SpecificationList assignmentList_;
	private CollectionList collectionList_;
	private ContextList contextList_;
	private GeneralizationList generalizationList_;
	private GrammarList grammarList_;
	private InterfaceList interfaceList_;
	private JustificationList justificationList_;
	private MultipleWordList multipleWordList_;
	private SpecificationList specificationList_;
	private WordTypeList wordTypeList_;
	private WriteList writeList_;

	// Word item lists array
	private List[] wordListArray_ = new List[Constants.NUMBER_OF_WORD_LISTS];


	// Private initialized variables

	private boolean isFemale_;
	private boolean isFeminineWord_;
	private boolean isMale_;
	private boolean isMasculineWord_;
	private boolean isLanguageWord_;

	private short wordParameter_;


	// Private common methods

	private boolean isCorrectFeminineOrMasculineArticleWord( short articleParameter )
		{
		if( isFeminineWord_ ||
		isMasculineWord_ )
			{
			return( !hasFeminineAndMasculineArticle( articleParameter ) ||

					( isFeminineWord_ &&
					isFeminineArticleParameter( articleParameter ) ) ||

					( isMasculineWord_ &&
					isMasculineArticleParameter( articleParameter ) ) );
			}

		// Not defined
		return true;
		}

	private static short nUsers()
		{
		short nUsers = 0;
		GeneralizationItem currentGeneralizationItem;
		WordItem predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem;

		if( predefinedNounUserWordItem != null &&
		( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	nUsers++;
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return nUsers;
		}

	private static char wordListChar( short wordListNr )
		{
		switch( wordListNr )
			{
			case Constants.WORD_ASSIGNMENT_LIST:
				return Constants.WORD_ASSIGNMENT_LIST_SYMBOL;

			case Constants.WORD_COLLECTION_LIST:
				return Constants.WORD_COLLECTION_LIST_SYMBOL;

			case Constants.WORD_GENERALIZATION_LIST:
				return Constants.WORD_GENERALIZATION_LIST_SYMBOL;

			case Constants.WORD_INTERFACE_LIST:
				return Constants.WORD_INTERFACE_LIST_SYMBOL;

			case Constants.WORD_JUSTIFICATION_LIST:
				return Constants.WORD_JUSTIFICATION_LIST_SYMBOL;

			case Constants.WORD_MULTIPLE_WORD_LIST:
				return Constants.WORD_MULTIPLE_WORD_LIST_SYMBOL;

			case Constants.WORD_GRAMMAR_LIST:
				return Constants.WORD_GRAMMAR_LIST_SYMBOL;

			case Constants.WORD_WRITE_LIST:
				return Constants.WORD_WRITE_LIST_SYMBOL;

			case Constants.WORD_SPECIFICATION_LIST:
				return Constants.WORD_SPECIFICATION_LIST_SYMBOL;

			case Constants.WORD_TYPE_LIST:
				return Constants.WORD_TYPE_LIST_SYMBOL;

			case Constants.WORD_CONTEXT_LIST:
				return Constants.WORD_CONTEXT_LIST_SYMBOL;
			}

		return Constants.SYMBOL_QUESTION_MARK;
		}

	private String languageNameStringInWord( short languageNr )
		{
		WordItem languageWordItem = languageWordItemInWord( languageNr );

		if( languageWordItem != null )
			return languageWordItem.anyWordTypeString();

		return Constants.NO_LANGUAGE_NAME_FOUND_STRING;
		}

	private String userNameStringInWord( short userNr )
		{
		short numberOfUsers = nUsers();
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;

		if( userNr == Constants.NO_USER_NR )
			return Constants.ADMIN_USER_NAME_STRING;

		if( numberOfUsers > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
				userNr == numberOfUsers-- )
					return generalizationWordItem.anyWordTypeString();
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return Constants.NO_USER_NAME_FOUND_STRING;
		}


	// Private error methods

	private byte addErrorWithWordListNr( short wordListNr, int methodLevel, String moduleNameString, String errorString )
		{
		return addError( wordListChar( wordListNr ), ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	private BoolResultType startBoolResultErrorInWord( int methodLevel, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = startError( 1, null, anyWordTypeString(), errorString );
		return boolResult;
		}

	private CollectionResultType addCollectionResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addCollectionResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	private CollectionResultType startCollectionResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startCollectionResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	private GrammarResultType startGrammarResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startGrammarResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	private JustificationResultType startJustificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startJustificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	private WordTypeResultType startWordTypeResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startWordTypeResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}


	// Private assignment methods

	private byte inactivateAssignment( boolean isAmbiguousRelationContext, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		RelatedResultType relatedResult;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundActiveAssignmentItem;
		SpecificationItem relatedSpecificationItem;

		if( ( foundAssignmentItem = firstActiveNonQuestionAssignmentItem( isAmbiguousRelationContext, isPossessive, relationContextNr, specificationWordItem ) ) == null )
			{
			if( relationContextNr == Constants.NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( relatedResult = findRelatedSpecification( true, true, false, isExclusiveSpecification, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to find a related assignment" );

					relatedSpecificationItem = relatedResult.relatedSpecificationItem;

					if( relatedSpecificationItem != null &&
					// Skip specification
					relatedSpecificationItem.isUserAssignment() &&

					( ( !isNegative &&
					// Only option left
					!isSelfGenerated ) ||

					!relatedSpecificationItem.isNegative() ) )
						{
						if( inactivateActiveAssignment( relatedSpecificationItem ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to inactivate a related active assignment" );
						}
					}
				}
			else
				{
				// Has relation context
				if( ( foundActiveAssignmentItem = firstAssignmentItem( true, false, false, isNegative, isPossessive, isSelfGenerated, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
					{
					if( ( foundAssignmentItem = firstAssignmentItem( false, true, false, isNegative, isPossessive, isSelfGenerated, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
					// First archive an inactive assignment
					archiveInactiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to archive an inactive assignment" );

					// Now inactivate the found active assignment
					if( inactivateActiveAssignment( foundActiveAssignmentItem ) != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to inactivate an active assignment" );
					}
				}
			}
		else
			{
			if( isAmbiguousRelationContext ||

			( !foundAssignmentItem.isUniqueUserRelation() &&
			foundAssignmentItem.isNegative() != isNegative ) )
				{
				if( inactivateActiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
					return addErrorInWord( 1, null, "I failed to inactivate a negative active assignment" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte inactivateCurrentAssignments()
		{
		SpecificationItem activeAssignmentItem;

		while( ( activeAssignmentItem = firstNonQuestionActiveAssignmentItem() ) != null )
			{
			if( inactivateActiveAssignment( activeAssignmentItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to inactive an active assignment item" );
			}

		return Constants.RESULT_OK;
		}

	private SpecificationItem firstActiveNonQuestionAssignmentItem( boolean isDifferentRelationContext, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		if( assignmentList_ != null )
			return assignmentList_.firstActiveNonQuestionAssignmentItem( isDifferentRelationContext, isPossessive, relationContextNr, specificationWordItem );

		return null;
		}

	private SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		if( assignmentList_ != null )
			return assignmentList_.firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );

		return null;
		}

	private CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString )
		{
		boolean hasDifferentAssumptionLevel = false;
		boolean isAmbiguous = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		SpecificationItem createdAssignmentItem;
		SpecificationItem foundAssignmentItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( !isQuestion &&
		inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
			return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate an assignment" );

		foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

		if( foundAssignmentItem == null &&
		// Didn't find a self-generated assignment. Now try to find a confirmed assignment
		isSelfGenerated &&
		!isSpecificationGeneralization )
			foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, isPossessive, false, questionParameter, relationContextNr, specificationWordItem );

		if( foundAssignmentItem != null )
			{
			createAndAssignResult.foundSpecificationItem = foundAssignmentItem;

			if( foundAssignmentItem.assumptionLevel() != assumptionLevel )
				hasDifferentAssumptionLevel = true;

			// Ambiguous if assignment has different tense (time): active, inactive or archived.
			if( foundAssignmentItem.isInactiveAssignment() != isInactiveAssignment ||
			foundAssignmentItem.isArchivedAssignment() != isArchivedAssignment )
				isAmbiguous = true;
			}

		if( isAmbiguous ||
		foundAssignmentItem == null ||

		( !isSelfGenerated &&
		hasDifferentAssumptionLevel ) ||

		// Archived assignment without relation will be
		// replaced by archived assignment with relation
		( isArchivedAssignment &&
		relationContextNr > Constants.NO_CONTEXT_NR &&
		!foundAssignmentItem.hasRelationContext() ) )
			{
			if( ( createAndAssignResult = createSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false, false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, CommonVariables.currentAssignmentLevel, assumptionLevel, CommonVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
				{
				if( specificationWordItem == null )
					return addCreateAndAssignResultErrorInWord( 1, null, "I failed to create an assignment with specification string \"" + specificationString + "\"" );

				return addCreateAndAssignResultErrorInWord( 1, null, "I failed to create an assignment with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}

			if( ( createdAssignmentItem = createAndAssignResult.createdSpecificationItem ) == null )
				{
				if( specificationWordItem == null )
					return startCreateAndAssignResultErrorInWord( 1, null, "I couldn't create an assignment item with specification string \"" + specificationString + "\"" );

				return startCreateAndAssignResultErrorInWord( 1, null, "I couldn't create an assignment item with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}

			if( foundAssignmentItem != null )
				{
				if( hasDifferentAssumptionLevel )
					{
					if( !isQuestion &&
					replaceOrDeleteSpecification( false, foundAssignmentItem, ( foundAssignmentItem.isSelfGenerated() ? createdAssignmentItem : null ) ) != Constants.RESULT_OK )
						return addCreateAndAssignResultErrorInWord( 1, null, "I failed to replace or delete an ambiguous assignment" );
					}
				else
					{
					if( foundAssignmentItem.isActiveItem() )
						{
						if( inactivateActiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
							return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate an active assignment" );
						}
					else
						{
						if( foundAssignmentItem.isInactiveAssignment() &&
						// Avoid archiving inactive assignments that are created during this sentence
						!foundAssignmentItem.hasCurrentInactiveSentenceNr() &&
						archiveInactiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
							return addCreateAndAssignResultErrorInWord( 1, null, "I failed to archive an inactive assignment" );
						}
					}
				}
			}

		return createAndAssignResult;
		}

	private CreateAndAssignResultType assignSpecificationByValue( JustificationItem firstJustificationItem, WordItem specificationWordItem )
		{
		SpecificationItem currentAssignmentItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( specificationWordItem == null )
			return startCreateAndAssignResultErrorInWord( 1, null, "The given specification word item is undefined" );

		if( ( currentAssignmentItem = specificationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			do	{
				if( ( createAndAssignResult = assignSpecification( false, currentAssignmentItem.isInactiveAssignment(), currentAssignmentItem.isArchivedAssignment(), currentAssignmentItem.isEveryGeneralization(), currentAssignmentItem.isExclusiveGeneralization(), currentAssignmentItem.isExclusiveSpecification(), currentAssignmentItem.isNegative(), currentAssignmentItem.isPartOf(), currentAssignmentItem.isPossessive(), currentAssignmentItem.isSpecificationGeneralization(), currentAssignmentItem.isUniqueUserRelation(), currentAssignmentItem.assumptionLevel(), currentAssignmentItem.prepositionParameter(), currentAssignmentItem.questionParameter(), currentAssignmentItem.generalizationWordTypeNr(), currentAssignmentItem.specificationWordTypeNr(), currentAssignmentItem.relationWordTypeNr(), currentAssignmentItem.specificationCollectionNr(), currentAssignmentItem.generalizationContextNr(), currentAssignmentItem.specificationContextNr(), currentAssignmentItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, currentAssignmentItem.nContextRelations(), firstJustificationItem, currentAssignmentItem.specificationWordItem(), currentAssignmentItem.specificationString() ) ).result != Constants.RESULT_OK )
					return addCreateAndAssignResultErrorInWord( 1, null, "I failed to assign specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
			}

		return createAndAssignResult;
		}


	// Private collection methods

	private short highestCollectionOrderNrInWord( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.highestCollectionOrderNr( collectionNr );

		return Constants.NO_ORDER_NR;
		}

	private int highestCollectionNrInWord()
		{
		if( collectionList_ != null )
			return collectionList_.highestCollectionNr();

		return Constants.NO_COLLECTION_NR;
		}


	// Private context methods

	private boolean isContextSubsetInWord( int fullSetContextNr, int subsetContextNr )
		{
		if( contextList_ != null )
			return contextList_.isContextSubset( fullSetContextNr, subsetContextNr );

		return false;
		}


	// Private grammar methods

	private String grammarStringInWord( short wordTypeNr )
		{
		if( grammarList_ != null )
			return grammarList_.grammarStringInList( wordTypeNr );

		return null;
		}


	// Private specification methods

	private boolean isAuthorizedForChanges( String authorizationKey )
		{
		return ( authorizationKey_ == null ||
				authorizationKey_ == authorizationKey );
		}

	private byte checkSpecificationForUsageInWord( SpecificationItem unusedSpecificationItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.checkSpecificationForUsage( false, false, false, unusedSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList_.checkSpecificationForUsage( false, true, false, unusedSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList_.checkSpecificationForUsage( false, true, false, unusedSpecificationItem ) == Constants.RESULT_OK )
			assignmentList_.checkSpecificationForUsage( false, false, true, unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		specificationList_.checkSpecificationForUsage( false, false, false, unusedSpecificationItem ) == Constants.RESULT_OK )
			specificationList_.checkSpecificationForUsage( false, false, true, unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList_ != null )
			justificationList_.checkSpecificationForUsage( unusedSpecificationItem );

		return CommonVariables.result;
		}

	private SpecificationItem firstRelationSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			foundSpecificationItem = assignmentList_.firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		if( foundSpecificationItem == null &&
		!isInactiveAssignment &&
		!isArchivedAssignment &&
		specificationList_ != null )
			return specificationList_.firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		return foundSpecificationItem;
		}

	// Private word type methods

	private boolean isSingularNoun()
		{
		return hasWordType( false, Constants.WORD_TYPE_NOUN_SINGULAR );
		}


	// Protected constructed variables

	protected boolean isUserGeneralizationWord;
	protected boolean isUserSpecificationWord;
	protected boolean isUserRelationWord;

	protected boolean isWordCheckedForSolving;

	protected short predefinedMultipleWordNr;

	protected WordItem nextAssignmentWordItem;
	protected WordItem nextCollectionWordItem;
	protected WordItem nextContextWordItem;
	protected WordItem nextPossessiveNounWordItem;
	protected WordItem nextSpecificationWordItem;
	protected WordItem nextTouchedProperNameWordItem;


	// Constructor (AdminItem)

	protected WordItem()
		{
		// Private constructed variables

		isNounWordSpanishAmbiguous_ = false;

		authorizationKey_ = null;

		wordQuestion_ = null;
		wordSpecification_ = null;
		wordWrite_ = null;

		assignmentList_ = null;
		collectionList_ = null;
		contextList_ = null;
		generalizationList_ = null;
		grammarList_ = null;
		interfaceList_ = null;
		justificationList_ = null;
		multipleWordList_ = null;
		writeList_ = null;
		specificationList_ = null;
		wordTypeList_ = null;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = null;

		// Private initialized variables

		isFemale_ = false;
		isFeminineWord_ = false;
		isMale_ = false;
		isMasculineWord_ = false;
		isLanguageWord_ = false;

		wordParameter_ = Constants.NO_WORD_PARAMETER;

		// Protected constructed variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;

		predefinedMultipleWordNr = 0;

		nextAssignmentWordItem = null;
		nextCollectionWordItem = null;
		nextContextWordItem = null;
		nextPossessiveNounWordItem = null;
		nextSpecificationWordItem = null;
		nextTouchedProperNameWordItem = null;
		}


	// Constructor

	protected WordItem( boolean isLanguageWord, short wordParameter, List myList )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, this );

		// Private constructed variables

		isNounWordSpanishAmbiguous_ = false;

		authorizationKey_ = null;

		wordQuestion_ = null;
		wordSpecification_ = null;
		wordWrite_ = null;

		assignmentList_ = null;
		collectionList_ = null;
		contextList_ = null;
		generalizationList_ = null;
		grammarList_ = null;
		interfaceList_ = null;
		justificationList_ = null;
		multipleWordList_ = null;
		writeList_ = null;
		specificationList_ = null;
		wordTypeList_ = null;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = null;

		// Private initialized variables

		isFemale_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE );
		isFeminineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMale_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isMasculineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;

		// Protected constructed variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;

		predefinedMultipleWordNr = 0;

		nextAssignmentWordItem = null;
		nextCollectionWordItem = null;
		nextContextWordItem = null;
		nextPossessiveNounWordItem = null;
		nextSpecificationWordItem = null;
		nextTouchedProperNameWordItem = null;
		}


	// Protected methods

	protected byte addErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected byte startErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected CreateAndAssignResultType addCreateAndAssignResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addCreateAndAssignResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected CreateAndAssignResultType startCreateAndAssignResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startCreateAndAssignResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected CreateAndAssignResultType startCreateAndAssignResultSystemErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startCreateAndAssignResultSystemError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected RelatedResultType addRelatedResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addRelatedResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected RelatedResultType startRelatedResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startRelatedResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected SpecificationResultType addSpecificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addSpecificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected SpecificationResultType startSpecificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startSpecificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected UserSpecificationResultType addUserSpecificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addUserSpecificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected UserSpecificationResultType startUserSpecificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startUserSpecificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}


	// Protected virtual item methods

	protected void checkForUsage()
		{
		if( assignmentList_ != null &&
		assignmentList_.checkWordItemForUsage( false, false, this ) == Constants.RESULT_OK &&
		assignmentList_.checkWordItemForUsage( true, false, this ) == Constants.RESULT_OK )
			assignmentList_.checkWordItemForUsage( false, true, this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		collectionList_ != null )
			collectionList_.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		contextList_ != null )
			contextList_.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		generalizationList_ != null )
			generalizationList_.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		multipleWordList_ != null )
			multipleWordList_.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.checkWordItemForUsage( false, false, this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		CommonVariables.adminConditionList != null )
			CommonVariables.adminConditionList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		CommonVariables.adminActionList != null )
			CommonVariables.adminActionList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		CommonVariables.adminAlternativeList != null )
			CommonVariables.adminAlternativeList.checkWordItemForUsage( this );
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				wordParameter_ > Constants.NO_WORD_PARAMETER ) );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.

		// Add new word at the end of the list
		return false;
		}

	protected BoolResultType findMatchingWordReferenceString( String searchString )
		{
		if( wordTypeList_ == null )
			return startBoolResultErrorInWord( 1, "The word type list isn't created yet" );

		return wordTypeList_.findMatchingWordReferenceString( searchString );
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + Constants.QUERY_WORD_START_CHAR + wordTypeString( true, queryWordTypeNr ) + Constants.QUERY_WORD_END_CHAR );

		if( isAuthorizationRequiredForChanges() )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAuthorizationRequiredForChanges" );

		if( isFemale_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFemale" );

		if( isFeminineWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFeminine" );

		if( isMale_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMale" );

		if( isMasculineWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMasculine" );

		if( isLanguageWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isNounWordSpanishAmbiguous_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNounWordSpanishAmbiguous" );

		if( wordParameter_ > Constants.NO_WORD_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordParameter:" + wordParameter_ );

		return queryStringBuffer;
		}


	// Protected common methods

	protected boolean isAdjectiveAssigned()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	protected boolean isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	protected boolean isAdjectiveNo()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_NO );
		}

	protected boolean isAdjectiveComparison()
		{
		return ( isAdjectiveComparisonLess() ||
				isAdjectiveComparisonEqual() ||
				isAdjectiveComparisonMore() );
		}

	protected boolean isAdjectiveComparisonLess()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EARLIER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_LESS ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_LOWER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_SMALLER );
		}

	protected boolean isAdjectiveComparisonEqual()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EQUAL ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_SAME );
		}

	protected boolean isAdjectiveComparisonMore()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_BIGGER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_HIGHER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_LARGER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_LATER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_MORE );
		}

	protected boolean isAdjectiveOdd()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ODD );
		}

	protected boolean isAdjectiveEven()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EVEN );
		}

	protected boolean isAdjectiveOddOrEven()
		{
		return ( isAdjectiveOdd() ||
				isAdjectiveEven() );
		}

	protected boolean isAdminWord()
		{
		return ( myList() == null );
		}

	protected boolean isAuthorizationRequiredForChanges()
		{
		return ( authorizationKey_ != null );
		}

	protected boolean isFemale()
		{
		return isFemale_;
		}

	protected boolean isMale()
		{
		return isMale_;
		}

	protected boolean isFemaleOrMale()
		{
		return ( isFemale_ ||
				isMale_ );
		}

	protected boolean isNounHead()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_HEAD );
		}

	protected boolean isNounTail()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_TAIL );
		}

	protected boolean isNounNumber()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_NUMBER );
		}

	protected boolean isNounPassword()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_PASSWORD );
		}

	protected boolean isNounValue()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_VALUE );
		}

	protected boolean isBasicVerb()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IS ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_WAS ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_BE ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_HAS ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_HAD ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_HAVE ||

				wordParameter_ == Constants.WORD_PARAMETER_PLURAL_VERB_ARE ||
				wordParameter_ == Constants.WORD_PARAMETER_PLURAL_VERB_WERE ||
				wordParameter_ == Constants.WORD_PARAMETER_PLURAL_VERB_CAN_BE ||
				wordParameter_ == Constants.WORD_PARAMETER_PLURAL_VERB_HAVE ||
				wordParameter_ == Constants.WORD_PARAMETER_PLURAL_VERB_HAD ||
				wordParameter_ == Constants.WORD_PARAMETER_PLURAL_VERB_CAN_HAVE );
		}

	protected boolean isVerbImperativeDisplayOrLoginOrRead()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ );
		}

	protected boolean isVerbUndoOrRedo()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO );
		}

	protected short userNr( WordItem userWordItem )
		{
		short userNr = nUsers();
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;
		WordItem predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem;

		if( predefinedNounUserWordItem != null &&
		( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( generalizationWordItem == userWordItem )
						return userNr;

					userNr--;
					}
				}
			while( userNr > Constants.NO_USER_NR &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return Constants.NO_USER_NR;
		}

	protected short wordParameter()
		{
		return wordParameter_;
		}

	protected byte assignChangePermissions( String authorizationKey )
		{
		if( authorizationKey_ == null )
			authorizationKey_ = authorizationKey;
		else
			{
			// Not the content of the string, but its address
			// identifies the owner. So, compare the addresses.
			if( authorizationKey_ != authorizationKey )
				return startErrorInWord( 1, null, "The authorization key is already assigned" );
			}

		return Constants.RESULT_OK;
		}

	protected byte checkStructuralIntegrityInWord()
		{
		// Check assignments for replaced or deleted justifications
		if( assignmentList_ != null &&
		assignmentList_.checkForReplacedOrDeletedJustification( false, false ) == Constants.RESULT_OK &&
		assignmentList_.checkForReplacedOrDeletedJustification( false, false ) == Constants.RESULT_OK &&
		assignmentList_.checkForReplacedOrDeletedJustification( false, true ) == Constants.RESULT_OK )
			assignmentList_.checkForDeletedJustificationInReplacedSpecification();

		// Check for unused contexts
		if( CommonVariables.result == Constants.RESULT_OK &&
		contextList_ != null &&
		contextList_.checkForUnusedContext() != Constants.RESULT_OK )
			return startErrorInWord( 1, null, "I failed to check for an unused context" );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList_ != null &&
		justificationList_.checkForReplacedOrDeletedSpecification() == Constants.RESULT_OK )
			justificationList_.checkForUnreferencedJustification();

		// Check specifications for replaced or deleted justifications
		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		specificationList_.checkForReplacedOrDeletedJustification( false, false ) == Constants.RESULT_OK )
			specificationList_.checkForDeletedJustificationInReplacedSpecification();

		return CommonVariables.result;
		}

	protected byte markWordAsFemale()
		{
		if( isMale_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already masculine" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnMasculineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on masculine parameters" );

		isFemale_ = true;
		isMale_ = false;

		return Constants.RESULT_OK;
		}

	protected byte markWordAsFeminine()
		{
		if( isMasculineWord_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already masculine" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnMasculineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on masculine parameters" );

		isFeminineWord_ = true;
		isMasculineWord_ = false;

		return Constants.RESULT_OK;
		}

	protected byte markWordAsMale()
		{
		if( isFemale_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already feminine" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnFeminineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on feminine parameters" );

		isFemale_ = false;
		isMale_ = true;

		return Constants.RESULT_OK;
		}

	protected byte markWordAsMasculine()
		{
		if( isFeminineWord_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already feminine" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnFeminineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on feminine parameters" );

		isFeminineWord_ = false;
		isMasculineWord_ = true;

		return Constants.RESULT_OK;
		}

	protected String languageNameString( short languageNr )
		{
		WordItem predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem ) != null )
			return predefinedNounLanguageWordItem.languageNameStringInWord( languageNr );

		return null;
		}

	protected String userNameString( short userNr )
		{
		WordItem predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem ) != null )
			return predefinedNounUserWordItem.userNameStringInWord( userNr );

		return null;
		}

	protected String wordTypeNameString( short wordTypeNr )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.grammarStringInWord( wordTypeNr );

		return null;
		}

	protected WordItem languageWordItemInWord( short languageNr )
		{
		short numberOfLanguages = nLanguages();
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( numberOfLanguages > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( currentGeneralizationWordItem.isLanguageWord() &&
					languageNr == numberOfLanguages-- )
						return currentGeneralizationItem.generalizationWordItem();
					}
				}
			while( numberOfLanguages > 0 &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return null;
		}

	protected WordItem nextWordItem()
		{
		return (WordItem)nextItem;
		}

	protected WordItem predefinedWordItem( short wordParameter )
		{
		WordItem currentPredefinedWordItem;
		WordItem lastPredefinedWordItem = CommonVariables.lastPredefinedWordItem;

		if( wordParameter > Constants.NO_WORD_PARAMETER &&
		( currentPredefinedWordItem = CommonVariables.firstPredefinedWordItem ) != null )
			{
			// Do for all predefined words
			do	{
				if( currentPredefinedWordItem.wordParameter() == wordParameter &&
				!currentPredefinedWordItem.isMultipleWord() )
					return currentPredefinedWordItem;
				}
			while( currentPredefinedWordItem != lastPredefinedWordItem &&
			( currentPredefinedWordItem = currentPredefinedWordItem.nextWordItem() ) != null );
			}

		return null;
		}


	// Protected assignment methods

	protected int nActiveAssignments()
		{
		if( assignmentList_ != null )
			return assignmentList_.nActiveAssignments();

		return 0;
		}

	protected byte archiveInactiveAssignment( SpecificationItem inactiveAssignmentItem )
		{
		if( assignmentList_ == null )
			return startErrorInWord( 1, null, "My assignment list isn't created yet" );

		if( inactiveAssignmentItem == null )
			return startErrorInWord( 1, null, "The given inactive assignment item is undefined" );

		if( !inactiveAssignmentItem.isInactiveAssignment() )
			return startErrorInWord( 1, null, "The given assignment item isn't inactive" );

		if( assignmentList_.archiveItem( inactiveAssignmentItem ) != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to archive the given inactive assignment item" );

		inactiveAssignmentItem.lastWrittenSentenceStringBuffer = null;

		return Constants.RESULT_OK;
		}

	protected byte createNewAssignmentLevelInWord()
		{
		if( assignmentList_ != null &&
		assignmentList_.createNewAssignmentLevelInList() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to create assignment level " + ( CommonVariables.currentAssignmentLevel + 1 ) );

		return Constants.RESULT_OK;
		}

	protected byte deleteAssignmentLevelInWord()
		{
		if( assignmentList_ != null &&
		assignmentList_.deleteAssignmentLevelInList() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to delete assignment level " + CommonVariables.currentAssignmentLevel );

		return Constants.RESULT_OK;
		}

	protected byte inactivateActiveAssignment( SpecificationItem activeAssignmentItem )
		{
		if( assignmentList_ == null )
			return startErrorInWord( 1, null, "My assignment list isn't created yet" );

		if( activeAssignmentItem == null )
			return startErrorInWord( 1, null, "The given active assignment item is undefined" );

		if( !activeAssignmentItem.isActiveAssignment() )
			return startErrorInWord( 1, null, "The given active assignment item isn't active" );

		if( assignmentList_.inactivateItem( activeAssignmentItem ) != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to inactivate an active assignment" );

		activeAssignmentItem.lastWrittenSentenceStringBuffer = null;

		return Constants.RESULT_OK;
		}

	protected SpecificationItem firstActiveAssignmentItem( boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		if( assignmentList_ != null )
			return assignmentList_.firstRelationSpecificationItem( false, false, isPossessive, questionParameter, relationWordItem );

		return null;
		}

	protected SpecificationItem firstActiveNumeralAssignmentItem()
		{
		if( assignmentList_ != null )
			return assignmentList_.firstActiveNumeralAssignmentItem();

		return null;
		}

	protected SpecificationItem firstActiveStringAssignmentItem()
		{
		if( assignmentList_ != null )
			return assignmentList_.firstActiveStringAssignmentItem();

		return null;
		}

	protected SpecificationItem firstNonQuestionActiveAssignmentItem()
		{
		if( assignmentList_ != null )
			return assignmentList_.firstAssignmentItem( false, false, false, false );

		return null;
		}

	protected SpecificationItem lastActiveNonQuestionAssignmentItem()
		{
		if( assignmentList_ != null )
			return assignmentList_.lastActiveNonQuestionAssignmentItem();

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_.firstAssignmentItem( false, false, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList_.firstAssignmentItem( true, false, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList_.firstAssignmentItem( false, true, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_.firstAssignmentItem( false, false, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList_.firstAssignmentItem( true, false, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList_.firstAssignmentItem( false, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstNonQuestionAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_.firstAssignmentItem( false, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList_.firstAssignmentItem( true, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList_.firstAssignmentItem( false, true, isNegative, isPossessive, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstQuestionAssignmentItem()
		{
		SpecificationItem firstAssignmentItem = null;

		if( assignmentList_ != null &&
		// Active assignment
		( firstAssignmentItem = assignmentList_.firstAssignmentItem( false, false, false, true ) ) == null )
			{
			// Inactive assignment
			if( ( firstAssignmentItem = assignmentList_.firstAssignmentItem( false, true, false, true ) ) == null )
				// Archived assignment
				firstAssignmentItem = assignmentList_.firstAssignmentItem( false, false, true, true );
			}

		return firstAssignmentItem;
		}

	protected CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, String authorizationKey )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		SpecificationItem foundSpecificationItem;
		// Not authorized to assign this (generalization) word
		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != null &&
		// No user is logged in yet
		CommonVariables.currentUserNr != Constants.NO_USER_NR &&
		// Not authorized to assign this specification word
		!specificationWordItem.isAuthorizedForChanges( authorizationKey ) ) )
			return startCreateAndAssignResultErrorInWord( 1, null, "You are not authorized to assign this word" );

		if( assignmentList_ == null )
			{
			if( isAdminWord() )
				return startCreateAndAssignResultErrorInWord( 1, null, "The admin word item cannot have an assignment list" );

			// Create assignment list
			if( ( assignmentList_ = new SpecificationList( Constants.WORD_ASSIGNMENT_LIST_SYMBOL, this ) ) == null )
				return startCreateAndAssignResultErrorInWord( 1, null, "I failed to create an assignment list" );

			wordListArray_[Constants.WORD_ASSIGNMENT_LIST] = assignmentList_;
			}

		if( isAssignedOrClear )
			{
			if( questionParameter > Constants.NO_QUESTION_PARAMETER )
				return startCreateAndAssignResultErrorInWord( 1, null, "A question can only be answered, not be cleared" );

			if( inactivateCurrentAssignments() != Constants.RESULT_OK )
				return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate my current assignments" );
			}
		else
			{
			// Find the specification of the assignment
			if( ( foundSpecificationItem = firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) ) == null )
				return startCreateAndAssignResultErrorInWord( 1, null, ( specificationWordItem == null ? "String \"" + specificationString + "\" isn't one of my specifications" : "Word \"" + specificationWordItem.anyWordTypeString() + "\" isn't one of my specifications" ) );

			if( foundSpecificationItem.isExclusiveNonPossessiveGeneralization() &&
			assignmentList_.inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
				return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate or archive current generalization assignments" );

			if( foundSpecificationItem.isValueSpecification() )
				{
				if( ( createAndAssignResult = assignSpecificationByValue( firstJustificationItem, specificationWordItem ) ).result != Constants.RESULT_OK )
					return addCreateAndAssignResultErrorInWord( 1, null, "I failed to assign the value of a specification word" );
				}
			else
				{
				if( ( createAndAssignResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.isExclusiveGeneralization(), foundSpecificationItem.isExclusiveSpecification(), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem.generalizationWordTypeNr(), foundSpecificationItem.specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem.specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return addCreateAndAssignResultErrorInWord( 1, null, ( specificationWordItem == null ? "I failed to assign specification string \"" + specificationString + "\"" : "I failed to assign specification word \"" + specificationWordItem.anyWordTypeString() + "\"" ) );
				}
			}

		return createAndAssignResult;
		}


	// Protected cleanup methods

	protected void clearReplacingInfoInWord()
		{
		if( justificationList_ != null )
			justificationList_.clearReplacingInfo();

		if( assignmentList_ != null )
			assignmentList_.clearReplacingInfo();

		if( specificationList_ != null )
			specificationList_.clearReplacingInfo();
		}

	protected void getHighestInUseSentenceNrInWord( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		List currentWordList;

		for( short wordListNr = 0; ( wordListNr < Constants.NUMBER_OF_WORD_LISTS && CommonVariables.highestInUseSentenceNr < highestSentenceNr ); wordListNr++ )
			{
			// No need to organize grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null )
				{
				// Skip temporary lists
				if( isIncludingTemporaryLists ||
				!currentWordList.isTemporaryList() )
					currentWordList.getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );
				}
			}
		}

	protected void rebuildQuickAccessWordLists()
		{
		nextAssignmentWordItem = null;
		nextCollectionWordItem = null;
		nextContextWordItem = null;
		nextPossessiveNounWordItem = null;
		nextSpecificationWordItem = null;

		if( assignmentList_ != null )
			assignmentList_.addToAssignmentWordQuickAccessList();

		if( collectionList_ != null )
			collectionList_.addToCollectionWordQuickAccessList();

		if( contextList_ != null )
			contextList_.addToContextWordQuickAccessList();

		if( specificationList_ != null )
			{
			if( isSingularNoun() &&
			specificationList_.firstPossessiveSpecificationItem() != null )
				specificationList_.addToPossessiveNounWordQuickAccessList();

			specificationList_.addToSpecificationWordQuickAccessList();
			}
		}

	protected void setCurrentItemNrInWord()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			// No need to organize grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.setCurrentItemNrInList();
			}
		}

	protected byte decrementItemNrRangeInWord( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			// No need to organize grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to decrement item number range" );
			}

		return Constants.RESULT_OK;
		}

	protected byte decrementSentenceNrsInWord( int startSentenceNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			// No need to organize grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.decrementSentenceNrsInList( startSentenceNr ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteSentencesInWord( int lowestSentenceNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			// No need to organize grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.deleteSentencesInList( lowestSentenceNr ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to delete sentences in one of my lists" );
			}

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItemsInWord()
		{
		List currentWordList;

		for( short wordListNr = 0; ( wordListNr < Constants.NUMBER_OF_WORD_LISTS && CommonVariables.nDeletedItems == 0 ); wordListNr++ )
			{
			// No need to organize grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.removeFirstRangeOfDeletedItemsInList() != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to remove the first deleted items" );
			}

		return Constants.RESULT_OK;
		}

	protected byte redoCurrentSentenceInWord()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			// No need to redo grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			// No need to redo items in temporary lists
			!currentWordList.isTemporaryList() &&
			currentWordList.redoCurrentSentenceInList() != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to redo the current sentence" );
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentenceInWord()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			// No need to undo grammar and interface lists (of language words)
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			// No need to undo items in temporary lists
			!currentWordList.isTemporaryList() &&
			currentWordList.undoCurrentSentenceInList() != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to undo the current sentence" );
			}

		return Constants.RESULT_OK;
		}


	// Protected collection methods

	protected boolean hasCollection()
		{
		return ( collectionList_ != null &&
				collectionList_.hasActiveItems() );
		}

	protected boolean hasCollectionNr( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.hasCollectionNr( collectionNr );

		return false;
		}

	protected boolean hasCollectionNr( int collectionNr, WordItem commonWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.hasCollectionNr( collectionNr, commonWordItem );

		return false;
		}

	protected boolean hasCollectionNr( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.hasCollectionNr( collectionNr, collectionWordItem, commonWordItem );

		return false;
		}

	protected boolean hasNonExclusiveCollection( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.hasNonExclusiveCollection( collectionNr );

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.isCompoundCollection( collectionNr );

		return false;
		}

	protected boolean isCollectionSpanishAmbiguous( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.isCollectionSpanishAmbiguous( collectionNr );

		return false;
		}

	protected boolean isNonCompoundCollection( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.isNonCompoundCollection( collectionNr );

		return false;
		}

	protected boolean isNounWordSpanishAmbiguous()
		{
		return isNounWordSpanishAmbiguous_;
		}

	protected boolean isSpanishCurrentLanguage()
		{
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

		return ( currentLanguageWordItem != null &&
				currentLanguageWordItem.isNounWordSpanishAmbiguous_ );
		}

	protected short collectionOrderNr( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.collectionOrderNr( collectionNr, collectionWordItem, commonWordItem );

		return Constants.NO_ORDER_NR;
		}

	protected short highestCollectionOrderNrInCollectionWords( int collectionNr )
		{
		short tempCollectionOrderNr;
		short highestCollectionOrderNr = Constants.NO_ORDER_NR;
		WordItem currentCollectionWordItem;

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = CommonVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( ( tempCollectionOrderNr = currentCollectionWordItem.highestCollectionOrderNrInWord( collectionNr ) ) > highestCollectionOrderNr )
					highestCollectionOrderNr = tempCollectionOrderNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return highestCollectionOrderNr;
		}

	protected int collectionNr( short collectionWordTypeNr )
		{
		int foundCompoundCollectionNr;

		// Compound collection
		if( ( foundCompoundCollectionNr = compoundCollectionNr( collectionWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
			return foundCompoundCollectionNr;

		// Non-compound collection
		return nonCompoundCollectionNr( collectionWordTypeNr );
		}

	protected int collectionNr( short collectionWordTypeNr, WordItem commonWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.collectionNr( collectionWordTypeNr, commonWordItem );

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrByCompoundGeneralizationWordInWord( boolean isExclusiveSpecification, short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.collectionNrByCompoundGeneralizationWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem );

		return Constants.NO_COLLECTION_NR;
		}

	protected int compoundCollectionNr( short collectionWordTypeNr )
		{
		if( collectionList_ != null )
			return collectionList_.compoundCollectionNr( collectionWordTypeNr );

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNr( short collectionWordTypeNr )
		{
		if( collectionList_ != null )
			return collectionList_.nonCompoundCollectionNr( collectionWordTypeNr );

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNrInWord( int compoundCollectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.nonCompoundCollectionNr( compoundCollectionNr );

		return Constants.NO_COLLECTION_NR;
		}

	protected int highestCollectionNrInCollectionWords()
		{
		int tempCollectionNr;
		int highestCollectionNr = Constants.NO_COLLECTION_NR;
		WordItem currentCollectionWordItem;

		if( ( currentCollectionWordItem = CommonVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( ( tempCollectionNr = currentCollectionWordItem.highestCollectionNrInWord() ) > highestCollectionNr )
					highestCollectionNr = tempCollectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return highestCollectionNr;
		}

	protected WordItem collectionWordItem( int compoundCollectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.collectionWordItem( compoundCollectionNr );

		return null;
		}

	protected WordItem commonWordItem( int collectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.commonWordItem( collectionNr );

		return null;
		}

	protected WordItem compoundGeneralizationWordItem( int compoundCollectionNr )
		{
		if( collectionList_ != null )
			return collectionList_.compoundGeneralizationWordItem( compoundCollectionNr );

		return null;
		}

	protected WordItem feminineCollectionWordItem()
		{
		if( collectionList_ != null )
			return collectionList_.feminineCollectionWordItem();

		return null;
		}

	protected BoolResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return new BoolResultType();
		}

	protected CollectionResultType addCollection( boolean isExclusiveSpecification, boolean isSpecificationGeneralization, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		WordItem currentLanguageWordItem;

		if( commonWordItem == this &&
		isNounWordType( commonWordTypeNr ) )
			{
			isNounWordSpanishAmbiguous_ = true;

			if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
				currentLanguageWordItem.isNounWordSpanishAmbiguous_ = true;
			}

		if( collectionList_ == null )
			{
			if( isAdminWord() )
				return startCollectionResultErrorInWord( 1, null, "The admin word item cannot have a collection list" );

			// Create list
			if( ( collectionList_ = new CollectionList( this ) ) == null )
				return startCollectionResultErrorInWord( 1, null, "I failed to create a collection list" );

			wordListArray_[Constants.WORD_COLLECTION_LIST] = collectionList_;
			}

		return collectionList_.addCollection( isExclusiveSpecification, isSpecificationGeneralization, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
		}


	// Protected context methods

	protected boolean hasContextCurrentlyBeenUpdatedInWord( int contextNr, WordItem specificationWordItem )
		{
		if( contextList_ != null )
			return contextList_.hasContextCurrentlyBeenUpdated( contextNr, specificationWordItem );

		return false;
		}

	protected boolean hasContextInWord( int contextNr, WordItem specificationWordItem )
		{
		if( contextList_ != null )
			return contextList_.hasContext( contextNr, specificationWordItem );

		return false;
		}

	protected boolean isContextSimilarInWord( int firstContextNr, int secondContextNr )
		{
		if( contextList_ != null )
			return ( contextList_.hasContext( firstContextNr ) == contextList_.hasContext( secondContextNr ) );

		return true;
		}

	protected boolean isContextSubsetInContextWords( int fullSetContextNr, int subsetContextNr )
		{
		WordItem currentContextWordItem;

		if( fullSetContextNr > Constants.NO_CONTEXT_NR &&
		subsetContextNr > Constants.NO_CONTEXT_NR )
			{
			if( fullSetContextNr == subsetContextNr )
				return true;

			if( ( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
				{
				// Do for all context words
				do	{
					if( currentContextWordItem.isContextSubsetInWord( fullSetContextNr, subsetContextNr ) )
						return true;
					}
				while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
				}
			}

		return false;
		}

	protected short contextWordTypeNr( int contextNr )
		{
		ContextItem foundContextItem;

		if( ( foundContextItem = contextItem( contextNr ) ) != null )
			return foundContextItem.contextWordTypeNr();

		return Constants.NO_WORD_TYPE_NR;
		}

	protected int contextNr( WordItem specificationWordItem )
		{
		if( contextList_ != null )
			return contextList_.contextNr( specificationWordItem );

		return Constants.NO_CONTEXT_NR;
		}

	protected int contextNr( boolean isCompoundCollectionSpanishAmbiguous, WordItem specificationWordItem )
		{
		if( contextList_ != null )
			return contextList_.contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem );

		return Constants.NO_CONTEXT_NR;
		}

	protected int highestContextNrInWord()
		{
		if( contextList_ != null )
			return contextList_.highestContextNr();

		return Constants.NO_CONTEXT_NR;
		}

	protected int nContextWordsInContextWords( int contextNr, WordItem specificationWordItem )
		{
		int nContextWords = 0;
		WordItem currentContextWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem.hasContextInWord( contextNr, specificationWordItem ) )
					nContextWords++;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return nContextWords;
		}

	protected byte addContext( boolean isCompoundCollectionSpanishAmbiguous, short contextWordTypeNr, short specificationWordTypeNr, int contextNr, WordItem specificationWordItem )
		{
		if( contextList_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a context list" );

			// Create list
			if( ( contextList_ = new ContextList( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create a context list" );

			wordListArray_[Constants.WORD_CONTEXT_LIST] = contextList_;
			}

		return contextList_.addContext( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem );
		}

	protected ContextItem firstActiveContextItem()
		{
		if( contextList_ != null )
			return contextList_.firstActiveContextItem();

		return null;
		}

	protected ContextItem contextItem( int contextNr )
		{
		if( contextList_ != null )
			return contextList_.contextItem( contextNr );

		return null;
		}

	protected ContextItem contextItem( boolean isCompoundCollectionSpanishAmbiguous, int nContextWords, WordItem specificationWordItem )
		{
		if( contextList_ != null )
			return contextList_.contextItem( isCompoundCollectionSpanishAmbiguous, nContextWords, specificationWordItem );

		return null;
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabaseInWord()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			// No need to store items of temporary lists
			!currentWordList.isTemporaryList() &&
			currentWordList.storeChangesInFutureDatabaseInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to store changes of a list in the database" );
			}

		return Constants.RESULT_OK;
		}
*/

	// Protected generalization methods

	protected byte createGeneralizationItem( boolean isRelation, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem )
		{
		if( generalizationList_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a generalization list" );

			// Create list
			if( ( generalizationList_ = new GeneralizationList( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create a generalization list" );

			wordListArray_[Constants.WORD_GENERALIZATION_LIST] = generalizationList_;
			}

		return generalizationList_.createGeneralizationItem( isLanguageWord_, isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem );
		}

	protected GeneralizationItem firstGeneralizationItem()
		{
		if( generalizationList_ != null )
			return generalizationList_.firstActiveGeneralizationItem();

		return null;
		}

	protected GeneralizationItem firstNounSpecificationGeneralizationItem()
		{
		if( generalizationList_ != null )
			return generalizationList_.firstGeneralizationItem( true, true, false );

		return null;
		}

	protected GeneralizationItem firstSpecificationGeneralizationItem( boolean isOnlySelectingCurrentLanguage )
		{
		if( generalizationList_ != null )
			return generalizationList_.firstGeneralizationItem( false, isOnlySelectingCurrentLanguage, false );

		return null;
		}

	protected GeneralizationItem firstRelationGeneralizationItem()
		{
		if( generalizationList_ != null )
			return generalizationList_.firstGeneralizationItem( false, false, true );

		return null;
		}

	protected BoolResultType findGeneralization( boolean isRelation, WordItem generalizationWordItem )
		{
		if( generalizationList_ != null )
			return generalizationList_.findGeneralization( isRelation, generalizationWordItem );

		return new BoolResultType();
		}


	// Protected grammar methods

	protected void checkGrammarForUsageInWord( GrammarItem unusedGrammarItem )
		{
		if( grammarList_ != null )
			grammarList_.checkGrammarItemForUsage( unusedGrammarItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		writeList_ != null )
			writeList_.checkGrammarItemForUsage( unusedGrammarItem );
		}

	protected void markAsChoiceEnd( int choiceEndItemNr )
		{
		if( grammarList_ != null )
			grammarList_.markAsChoiceEnd( choiceEndItemNr );
		}

	protected void markAsOptionEnd()
		{
		if( grammarList_ != null )
			grammarList_.markAsOptionEnd();
		}

	protected boolean isCheckingGrammarNeeded()
		{
		if( grammarList_ != null )
			return grammarList_.isCheckingGrammarNeeded();

		return false;
		}

	protected boolean isLanguageWithMergedWords()
		{
		if( grammarList_ != null )
			return grammarList_.hasMergedWords();

		return false;
		}

	protected boolean isLanguageWord()
		{
		return isLanguageWord_;
		}

	protected short nLanguages()
		{
		short nLanguages = 0;
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;
		WordItem predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem;

		if( predefinedNounLanguageWordItem != null &&
		( currentGeneralizationItem = predefinedNounLanguageWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( generalizationWordItem.isLanguageWord() )
						nLanguages++;
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return nLanguages;
		}

	protected byte checkForDuplicateGrammarDefinition()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.checkForDuplicateGrammarDefinition();
		}

	protected byte checkGrammar()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.checkGrammar();
		}

	protected byte linkLaterDefinedGrammarWords()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.linkLaterDefinedGrammarWords();
		}

	protected byte expandMergedWordsInReadSentence( StringBuffer readUserSentenceStringBuffer )
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.expandMergedWordsInReadSentence( readUserSentenceStringBuffer );
		}

	protected byte shrinkMergedWordsInWriteSentence()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.shrinkMergedWordsInWriteSentence();
		}

	protected GrammarResultType createGrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		if( grammarList_ == null )
			{
			if( isAdminWord() )
				return startGrammarResultErrorInWord( 1, null, "The admin word item cannot have a grammar list" );

			if( ( grammarList_ = new GrammarList( this ) ) == null )
				return startGrammarResultErrorInWord( 1, null, "I failed to create a grammar list" );

			wordListArray_[Constants.WORD_GRAMMAR_LIST] = grammarList_;
			}

		return grammarList_.createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );
		}

	protected GrammarResultType findGrammar( boolean isIgnoringGrammarParameter, short grammarParameter, int grammarStringLength, String grammarString )
		{
		if( grammarList_ != null )
			return grammarList_.findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return new GrammarResultType();
		}

	protected WordEndingResultType analyzeWordEnding( short grammarParameter, int originalWordStringLength, String originalWordString )
		{
		if( grammarList_ != null )
			return grammarList_.analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return new WordEndingResultType();
		}

	protected GrammarItem firstGrammarItem()
		{
		if( grammarList_ != null )
			return grammarList_.firstGrammarItem();

		return null;
		}


	// Protected interface methods

	protected void checkSpecificationForUsageOfInvolvedWords( SpecificationItem unusedSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( justificationList_ != null )
			justificationList_.checkSpecificationForUsage( unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		// Check this word
		checkSpecificationForUsageInWord( unusedSpecificationItem ) == Constants.RESULT_OK &&
		( currentGeneralizationItem = firstGeneralizationItem() ) != null )
			{
			do	{
				// Also check generalization words
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					currentGeneralizationWordItem.checkSpecificationForUsageInWord( unusedSpecificationItem );
				}
			while( CommonVariables.result == Constants.RESULT_OK &&
			( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
			}
		}

	protected byte checkInterface( short interfaceParameter, String interfaceString )
		{
		if( interfaceList_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have an interface list" );

			// Create list
			if( ( interfaceList_ = new InterfaceList( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create an interface list" );

			wordListArray_[Constants.WORD_INTERFACE_LIST] = interfaceList_;
			}

		return interfaceList_.checkInterface( interfaceParameter, interfaceString );
		}

	protected byte createInterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString )
		{
		if( interfaceList_ == null )
			return startErrorInWord( 1, null, "The interface list isn't created yet" );

		return interfaceList_.createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );
		}

	protected String interfaceString( short interfaceParameter )
		{
		if( interfaceList_ != null )
			return interfaceList_.interfaceString( interfaceParameter );

		return Constants.INTERFACE_STRING_NOT_AVAILABLE;
		}


	// Protected justification methods

	protected void clearJustificationHasBeenWritten()
		{
		if( justificationList_ != null )
			justificationList_.clearJustificationHasBeenWritten();
		}

	protected boolean hasJustification( short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.hasJustification( justificationTypeNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem );

		return false;
		}

	protected boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		if( justificationList_ != null )
			return justificationList_.hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();

		return false;
		}

	protected byte attachJustificationInWord( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.attachJustification( attachJustificationItem, involvedSpecificationItem );
		}

	protected byte replaceJustification( JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.replaceJustification( false, obsoleteJustificationItem, replacingJustificationItem, involvedSpecificationItem, null );
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.replaceOrDeleteJustification( obsoleteJustificationItem );
		}

	protected byte replaceOrDeleteObsoleteJustifications()
		{
		if( justificationList_ != null )
			return justificationList_.replaceOrDeleteObsoleteJustifications();

		return Constants.RESULT_OK;
		}

	protected byte updateSpecificationsInJustificationInWord( boolean isMainWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.updateSpecificationsInJustifications( isMainWord, obsoleteSpecificationItem, replacingSpecificationItem );

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, int specificationCollectionNr )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.writeRelatedJustificationSpecifications( justificationTypeNr, specificationCollectionNr );
		}

	protected JustificationResultType addJustification( boolean hasFeminineOrMasculineProperNameEnding, boolean isForcingNewJustification, boolean isIncrementingOrderNr, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		if( justificationList_ == null )
			{
			if( isAdminWord() )
				return startJustificationResultErrorInWord( 1, null, "The admin word item cannot have a justification list" );

			if( ( justificationList_ = new JustificationList( this ) ) == null )
				return startJustificationResultErrorInWord( 1, null, "I failed to create a justification list" );

			wordListArray_[Constants.WORD_JUSTIFICATION_LIST] = justificationList_;
			}

		return justificationList_.addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, isIncrementingOrderNr, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem );
		}

	protected JustificationResultType copyJustification( boolean isForcingNewJustification, SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		if( justificationList_ == null )
			return startJustificationResultErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.copyJustification( isForcingNewJustification, newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );
		}

	protected JustificationItem correctedAssumptionByOppositeQuestionJustificationItem()
		{
		if( justificationList_ != null )
			return justificationList_.correctedAssumptionByOppositeQuestionJustificationItem();

		return null;
		}

	protected JustificationItem negativeAssumptionOrConclusionJustificationItem( SpecificationItem anotherPrimarySpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem );

		return null;
		}

	protected JustificationItem olderComplexJustificationItem( boolean hasSecondarySpecificationWithoutRelationContext, short justificationTypeNr, int secondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.olderComplexJustificationItem( hasSecondarySpecificationWithoutRelationContext, justificationTypeNr, secondarySpecificationCollectionNr, primarySpecificationItem );

		return null;
		}

	protected JustificationItem possessiveReversibleAssumptionJustificationItem( WordItem generalizationWordItem, WordItem secondarySpecificationWordItem )
		{
		if( justificationList_ != null )
			return justificationList_.possessiveReversibleAssumptionJustificationItem( generalizationWordItem, secondarySpecificationWordItem );

		return null;
		}

	protected JustificationItem primarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.primarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, primarySpecificationItem );

		return null;
		}

	protected JustificationItem secondarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem secondarySpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.secondarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, secondarySpecificationItem );

		return null;
		}

	protected SpecificationItem olderSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		if( justificationList_ != null )
			return justificationList_.olderSuggestiveQuestionAssumptionSecondarySpecificationItem();

		return null;
		}


	// Protected multiple word methods

	protected boolean isMultipleWord()
		{
		return ( multipleWordList_ != null &&
				multipleWordList_.hasActiveItems() );
		}

	protected short matchingMultipleSingularNounWordParts( String sentenceString )
		{
		if( multipleWordList_ != null )
			return multipleWordList_.matchingMultipleSingularNounWordParts( sentenceString );

		return 0;
		}

	protected byte addMultipleWord( short nWordParts, short wordTypeNr, WordItem multipleWordItem )
		{
		if( multipleWordList_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a multiple word list" );

			// Create list
			if( ( multipleWordList_ = new MultipleWordList( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create a multiple word list" );

			wordListArray_[Constants.WORD_MULTIPLE_WORD_LIST] = multipleWordList_;
			}

		return multipleWordList_.addMultipleWord( nWordParts, wordTypeNr, multipleWordItem );
		}


	// Protected query methods

	protected void countQuery()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.countQueryInList();
			}
		}

	protected void clearQuerySelections()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.clearQuerySelectionsInList();
			}
		}

	protected void itemQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			}
		}

	protected void listQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer );
			}
		}

	protected void parameterQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter );
			}
		}

	protected void wordTypeQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr );
			}
		}

	protected boolean hasActiveQuestionWithCompoundCollection()
		{
		if( specificationList_ != null )
			return specificationList_.hasActiveQuestionWithCompoundCollection();

		return false;
		}

	protected byte displayQueryResultInWord( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.displayQueryResultInList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to display the items" );
			}

		return Constants.RESULT_OK;
		}

	protected byte stringQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query strings" );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString )
		{
		boolean hasFoundMatchingString;
		List currentWordList;
		BoolResultType boolResult;

		if( ( boolResult = findMatchingWordReferenceString( wordNameString ) ).result != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to find words" );

		hasFoundMatchingString = boolResult.booleanValue;

		if( hasFoundMatchingString )
			{
			if( isSelectingOnFind &&
			!isSelectedByQuery )
				{
				isSelectedByQuery = true;
				CommonVariables.hasFoundQuery = true;
				}
			}
		else
			{
			if( !isSelectingOnFind &&
			isSelectedByQuery )
				isSelectedByQuery = false;
			}

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.wordQueryInList( ( hasFoundMatchingString && isSelectingOnFind ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		boolean isAssignmentOrSpecificationList;
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				{
				isAssignmentOrSpecificationList = ( wordListNr == Constants.WORD_ASSIGNMENT_LIST ||
													wordListNr == Constants.WORD_SPECIFICATION_LIST );

				if( currentWordList.wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, ( isAssignmentOrSpecificationList && isSelectingAttachedJustifications ), ( isAssignmentOrSpecificationList && isSelectingJustificationSpecifications), wordReferenceNameString ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query word references" );
				}
			}

		return Constants.RESULT_OK;
		}


	// Protected question methods

	protected boolean hasFoundAnswerToQuestion()
		{
		if( wordQuestion_ != null )
			return wordQuestion_.hasFoundAnswerToQuestion();

		return false;
		}

	protected byte findAnswerToNewUserQuestion()
		{
		if( wordQuestion_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a word question module" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create my word question module" );
			}

		return wordQuestion_.findAnswerToNewUserQuestion();
		}

	protected byte findAnswersToQuestions( int compoundSpecificationCollectionNr, SpecificationItem answerSpecificationItem )
		{
		if( wordQuestion_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a word question module" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create my word question module" );
			}

		return wordQuestion_.findAnswersToQuestions( compoundSpecificationCollectionNr, answerSpecificationItem );
		}

	protected byte writeAnswerToQuestion( boolean isNegativeAnswer, boolean isPositiveAnswer, boolean isUncertainAboutRelation, SpecificationItem answerSpecificationItem )
		{
		if( wordQuestion_ == null )
			return startErrorInWord( 1, null, "My word question module isn't created yet" );

		return wordQuestion_.writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );
		}


	// Protected specification methods

	protected void checkJustificationForUsageInWord( JustificationItem unusedJustificationItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.checkJustificationForUsage( false, false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.checkJustificationForUsage( true, false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.checkJustificationForUsage( false, true, false, unusedJustificationItem ) == Constants.RESULT_OK )
			assignmentList_.checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		specificationList_.checkJustificationForUsage( false, false, false, unusedJustificationItem ) == Constants.RESULT_OK )
			specificationList_.checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList_ != null )
			justificationList_.checkJustificationForUsage( unusedJustificationItem );
		}

	protected void initializeVariablesInWord()
		{
		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;
		isWordCheckedForSolving = false;

		nextTouchedProperNameWordItem = null;

		if( assignmentList_ != null )
			{
			assignmentList_.initializeSpecificationVariables( false, false );
			assignmentList_.initializeSpecificationVariables( true, false );
			assignmentList_.initializeSpecificationVariables( false, true );
			}

		if( justificationList_ != null )
			justificationList_.initializeJustificationVariables();

		if( specificationList_ != null )
			specificationList_.initializeSpecificationVariables( false, false );

		if( wordQuestion_ != null )
			wordQuestion_.initializeWordQuestionVariables();

		if( wordSpecification_ != null )
			wordSpecification_.initializeWordSpecificationVariables();
		}

	protected boolean hasAnsweredSelfGeneratedQuestion()
		{
		if( specificationList_ != null )
			return specificationList_.hasAnsweredSelfGeneratedQuestion();

		return false;
		}

	protected boolean hasAnyUserSpecification()
		{
		if( specificationList_ != null )
			return specificationList_.hasAnyUserSpecification();

		return false;
		}

	protected boolean hasConfirmedSpecificationButNoRelation()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasConfirmedSpecificationButNoRelation();

		return false;
		}

	protected boolean hasConfirmedSpecification()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasConfirmedSpecification();

		return false;
		}

	protected boolean hasContextInSpecificationsInWord( int contextNr )
		{
		if( assignmentList_ != null &&

		( assignmentList_.hasContextInSpecifications( false, false, false, contextNr ) ||
		assignmentList_.hasContextInSpecifications( true, false, false, contextNr ) ||
		assignmentList_.hasContextInSpecifications( false, true, false, contextNr ) ||
		assignmentList_.hasContextInSpecifications( false, false, true, contextNr ) ) )
			return true;

		if( specificationList_ != null &&

		( specificationList_.hasContextInSpecifications( false, false, false, contextNr ) ||
		specificationList_.hasContextInSpecifications( false, false, true, contextNr ) ) )
			return true;

		return false;
		}

	protected boolean hasCorrectedAssumption()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasCorrectedAssumption();

		return false;
		}

	protected boolean hasCorrectedAssumptionByKnowledge()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasCorrectedAssumptionByKnowledge();

		return false;
		}

	protected boolean hasCorrectedAssumptionByOppositeQuestion()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasCorrectedAssumptionByOppositeQuestion();

		return false;
		}

	protected boolean hasDisplayedMoreSpecificSpecification()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasDisplayedMoreSpecificSpecification();

		return false;
		}

	protected boolean hasDisplayedMoreSpecificNonExclusiveSpecification()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasDisplayedMoreSpecificNonExclusiveSpecification();

		return false;
		}

	protected boolean hasDisplayedMoreSpecificQuestion()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasDisplayedMoreSpecificQuestion();

		return false;
		}

	protected boolean hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		if( specificationList_ != null )
			return specificationList_.hasNonPossessiveAndNonQuestionPartOfSpecification();

		return false;
		}

	protected boolean hasPossessiveButNonQuestionSpecification()
		{
		if( specificationList_ != null )
			return specificationList_.hasPossessiveButNonQuestionSpecification();

		return false;
		}

	protected boolean hasPossiblyGapInKnowledge( int exclusiveSecondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		if( specificationList_ != null )
			return specificationList_.hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationItem );

		return true;
		}

	protected boolean isJustificationInUse( JustificationItem unusedJustificationItem )
		{
		boolean isIncludingReplacedItems = ( unusedJustificationItem != null &&
										unusedJustificationItem.isReplacedItem() );

		if( assignmentList_ != null )
			{
			if( assignmentList_.isJustificationInUse( false, false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList_.isJustificationInUse( true, false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList_.isJustificationInUse( false, true, false, unusedJustificationItem ) )
				return true;

			if( isIncludingReplacedItems &&
			assignmentList_.isJustificationInUse( false, false, true, unusedJustificationItem ) )
				return true;
			}

		if( specificationList_ != null )
			{
			if( specificationList_.isJustificationInUse( false, false, false, unusedJustificationItem ) )
				return true;

			if( isIncludingReplacedItems &&
			specificationList_.isJustificationInUse( false, false, true, unusedJustificationItem ) )
				return true;
			}

		return false;
		}

	protected byte changeJustificationOfNegativeAssumptions( SpecificationItem secondarySpecificationItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem ) == Constants.RESULT_OK &&
		assignmentList_.changeJustificationOfNegativeAssumptions( true, false, secondarySpecificationItem ) == Constants.RESULT_OK )
			assignmentList_.changeJustificationOfNegativeAssumptions( false, true, secondarySpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem );

		return CommonVariables.result;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperName, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( wordSpecification_ == null )
			return startErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem );
		}

	protected byte clearLastWrittenSentenceStringWithUnknownPluralNoun( String unknownPluralNounString, WordItem specificationWordItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.clearLastWrittenSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem ) == Constants.RESULT_OK &&
		assignmentList_.clearLastWrittenSentenceStringWithUnknownPluralNoun( true, false, unknownPluralNounString, specificationWordItem ) == Constants.RESULT_OK )
			assignmentList_.clearLastWrittenSentenceStringWithUnknownPluralNoun( false, true, unknownPluralNounString, specificationWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.clearLastWrittenSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem );

		return CommonVariables.result;
		}

	protected byte collectGeneralizations( boolean isExclusiveGeneralization, int generalizationCollectionNr )
		{
		if( assignmentList_ != null &&
		assignmentList_.collectGeneralizations( false, false, isExclusiveGeneralization, generalizationCollectionNr ) == Constants.RESULT_OK &&
		assignmentList_.collectGeneralizations( true, false, isExclusiveGeneralization, generalizationCollectionNr ) == Constants.RESULT_OK )
			assignmentList_.collectGeneralizations( false, true, isExclusiveGeneralization, generalizationCollectionNr );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.collectGeneralizations( false, false, isExclusiveGeneralization, generalizationCollectionNr );

		return CommonVariables.result;
		}

	protected byte collectSpecificationsInWord( boolean isExclusiveSpecification, boolean isQuestion, int specificationCollectionNr )
		{
		if( assignmentList_ != null &&
		assignmentList_.collectSpecificationsInList( false, false, isExclusiveSpecification, isQuestion, specificationCollectionNr ) == Constants.RESULT_OK &&
		assignmentList_.collectSpecificationsInList( true, false, isExclusiveSpecification, isQuestion, specificationCollectionNr ) == Constants.RESULT_OK )
			assignmentList_.collectSpecificationsInList( false, true, isExclusiveSpecification, isQuestion, specificationCollectionNr );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.collectSpecificationsInList( false, false, isExclusiveSpecification, isQuestion, specificationCollectionNr );

		return CommonVariables.result;
		}

	protected byte confirmSpecificationButNotItsRelation( SpecificationItem confirmedSpecificationItem, SpecificationItem confirmationSpecificationItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.confirmSpecificationButNotItsRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList_.confirmSpecificationButNotItsRelation( true, false, confirmedSpecificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK )
			assignmentList_.confirmSpecificationButNotItsRelation( false, true, confirmedSpecificationItem, confirmationSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.confirmSpecificationButNotItsRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem );

		return CommonVariables.result;
		}

	protected byte copyAndReplaceSpecificationItem( boolean isNewAnsweredQuestion, boolean isNewExclusiveGeneralization, boolean isNewExclusiveSpecification, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		if( originalSpecificationItem == null )
			return startErrorInWord( 1, null, "The given original specification item is undefined" );

		if( originalSpecificationItem.isAssignment() )
			{
			if( assignmentList_ == null )
				return startErrorInWord( 1, null, "My assignment list isn't created yet" );

			return assignmentList_.copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveGeneralization, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
			}

		if( specificationList_ == null )
			return startErrorInWord( 1, null, "My specification list isn't created yet" );

		return specificationList_.copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveGeneralization, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
		}

	protected byte recalculateAssumptionsInWord()
		{
		if( assignmentList_ != null &&
		assignmentList_.recalculateAssumptions( false, false ) == Constants.RESULT_OK &&
		assignmentList_.recalculateAssumptions( true, false ) == Constants.RESULT_OK )
			assignmentList_.recalculateAssumptions( false, true );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.recalculateAssumptions( false, false );

		return CommonVariables.result;
		}

	protected byte replaceOrDeleteSpecification( boolean isAllowingReplacedObsoleteSpecification, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( obsoleteSpecificationItem == null )
			return startErrorInWord( 1, null, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem.isAssignment() )
			{
			if( assignmentList_ == null )
				return startErrorInWord( 1, null, "My assignment list isn't created yet" );

			return assignmentList_.replaceOrDeleteSpecification( false, obsoleteSpecificationItem, replacingSpecificationItem );
			}

		if( specificationList_ == null )
			return startErrorInWord( 1, null, "The specification list isn't created yet" );

		return specificationList_.replaceOrDeleteSpecification( isAllowingReplacedObsoleteSpecification, obsoleteSpecificationItem, replacingSpecificationItem );
		}

	protected byte updateJustificationInSpecifications( boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		boolean isIncludingReplacedItems = ( obsoleteJustificationItem != null &&
										obsoleteJustificationItem.hasCurrentCreationSentenceNr() );

		if( assignmentList_ != null &&
		assignmentList_.updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.updateJustificationInSpecifications( true, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.updateJustificationInSpecifications( false, true, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
			{
			if( isIncludingReplacedItems )
				assignmentList_.updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		specificationList_.updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
			{
			if( isIncludingReplacedItems )
				specificationList_.updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		return CommonVariables.result;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem ) ) == null )
			foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAnsweredQuestions, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingActiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList_.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstActiveQuestionSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstActiveSpecificationItem( false, true );

		return null;
		}

	protected SpecificationItem firstActiveAssignmentOrSpecificationItem( boolean isIncludingAssignments, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList_ != null )
			foundSpecificationItem = assignmentList_.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAnsweredQuestionAssignmentItem( boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			{
			foundSpecificationItem = assignmentList_.firstAnsweredQuestionAssignmentItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList_.firstAnsweredQuestionAssignmentItem( true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList_.firstSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList_.firstSpecificationItem( true, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstExclusiveNounSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstExclusiveNounSpecificationItem();

		return null;
		}

	protected SpecificationItem firstExclusiveSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstExclusiveSpecificationItem();

		return null;
		}

	protected SpecificationItem firstFeminineOrMasculineSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstFeminineOrMasculineSpecificationItem();

		return null;
		}

	protected SpecificationItem firstNonCompoundUserSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstNonCompoundUserSpecificationItem();

		return null;
		}

	protected SpecificationItem firstNonCompoundCollectionSpecificationItem( int compoundSpecificationCollectionNr )
		{
		if( specificationList_ != null )
			return specificationList_.firstNonCompoundCollectionSpecificationItem( compoundSpecificationCollectionNr );

		return null;
		}

	protected SpecificationItem firstNonPossessiveDefinitionSpecificationItem( boolean isIncludingAdjectives )
		{
		if( specificationList_ != null )
			return specificationList_.firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives );

		return null;
		}

	protected SpecificationItem firstNonQuestionSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstActiveSpecificationItem( false, false );

		return null;
		}

	protected SpecificationItem firstPossessiveSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstPossessiveSpecificationItem();

		return null;
		}

	protected SpecificationItem firstPrimarySpecificationCandidateOfQuestionSpecificationItem( boolean isAllowingSpanishPossessiveSpecification )
		{
		SpecificationItem firstSelectedSpecificationItem;

		if( ( firstSelectedSpecificationItem = firstSpecificationItem( false, false, false, false ) ) != null )
			return firstSelectedSpecificationItem.primarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, true );

		return null;
		}

	protected SpecificationItem firstRecentlyAnsweredQuestionSpecificationItem( short questionParameter )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstRecentlyAnsweredQuestionSpecificationItem( false, questionParameter ) ) == null )
			foundSpecificationItem = assignmentList_.firstRecentlyAnsweredQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return foundSpecificationItem = specificationList_.firstRecentlyAnsweredQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstRelationSpecificationItem()
		{
		if( specificationList_ != null )
			return specificationList_.firstRelationSpecificationItem();

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem moreRecentSpecificationItem;
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) == null )
			foundSpecificationItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( true, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem );

		if( specificationList_ != null &&
		( moreRecentSpecificationItem = specificationList_.firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) != null )
			{
			if( foundSpecificationItem == null ||

			// Prefer more recent specification over assignment
			( foundSpecificationItem.hasRelationContext() == moreRecentSpecificationItem.hasRelationContext() &&
			foundSpecificationItem.originalSentenceNr() < moreRecentSpecificationItem.originalSentenceNr() ) )
				foundSpecificationItem = moreRecentSpecificationItem;
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( isAssignment ? firstAssignmentItem( isInactiveAssignment, isArchivedAssignment, isQuestion ) :
				( isQuestion ? firstActiveQuestionSpecificationItem() : firstNonQuestionSpecificationItem() ) );
		}

	protected SpecificationItem firstSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		if( isIncludingAnsweredQuestions &&
		questionParameter > Constants.NO_QUESTION_PARAMETER )
			{
			if( isAssignment )
				{
				if( assignmentList_ != null )
					return assignmentList_.firstAssignmentItem( true, isInactiveAssignment, isArchivedAssignment, questionParameter );
				}
			else
				{
				if( specificationList_ != null )
					return specificationList_.firstActiveSpecificationItem( true, questionParameter );
				}
			}
		else
			{
			if( isAssignment )
				{
				if( assignmentList_ != null )
					{
					if( isArchivedAssignment )
						return assignmentList_.firstAssignmentItem( false, false, true, questionParameter );

					if( isInactiveAssignment )
						return assignmentList_.firstAssignmentItem( false, true, false, questionParameter );

					return assignmentList_.firstAssignmentItem( false, false, false, questionParameter );
					}
				}
			else
				{
				if( questionParameter == Constants.NO_QUESTION_PARAMETER )
					return firstNonQuestionSpecificationItem();

				if( specificationList_ != null )
					return specificationList_.firstActiveSpecificationItem( false, questionParameter );
				}
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, WordItem specificationWordItem )
		{
		if( specificationList_ != null )
			return specificationList_.firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem );

		return null;
		}

	protected SpecificationItem firstUnhiddenSpecificationItem( int relationContextNr )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null )
			foundSpecificationItem = assignmentList_.firstUnhiddenSpecificationItem( true, relationContextNr );

		if( specificationList_ != null )
			return specificationList_.firstUnhiddenSpecificationItem( false, relationContextNr );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstUserSpecificationItem( boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( ( foundSpecificationItem = assignmentList_.firstUserSpecificationItem( true, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
				foundSpecificationItem = assignmentList_.firstUserSpecificationItem( false, true, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem sameUserQuestionSpecificationItem( short questionParameter )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList_ != null &&
		( foundSpecificationItem = assignmentList_.sameUserQuestionSpecificationItem( false, questionParameter ) ) == null )
			foundSpecificationItem = assignmentList_.sameUserQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == null &&
		specificationList_ != null )
			return specificationList_.sameUserQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	protected WordItem feminineOrMasculineCommonWordItem( boolean isFeminineWord )
		{
		if( specificationList_ != null )
			return specificationList_.feminineOrMasculineCommonWordItem( isFeminineWord );

		return null;
		}

	protected CollectionResultType collectSpecifications( short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		BoolResultType boolResult;
		CollectionResultType collectionResult = new CollectionResultType();
		SpecificationItem currentSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem currentSpecificationWordItem = null;
		WordItem foundSpecificationWordItem = null;

		if( generalizationWordItem == null )
			return startCollectionResultErrorInWord( 1, null, "The given generalization word item is undefined" );

		if( generalizationWordItem == this &&
		// Accept Spanish ambiguous definition
		!isNounWordSpanishAmbiguous() )
			return startCollectionResultErrorInWord( 1, null, "The given generalization word item is the same as my word" );

		if( collectionWordItem == null )
			return startCollectionResultErrorInWord( 1, null, "The given collected word item is undefined" );

		if( collectionWordItem == this )
			return startCollectionResultErrorInWord( 1, null, "The given collected word item is the same as my word" );

		if( !isNounWordType( collectionWordTypeNr ) )
			return startCollectionResultErrorInWord( 1, null, "The given collected word type isn't a noun" );

		if( !isNounWordType( commonWordTypeNr ) )
			return startCollectionResultErrorInWord( 1, null, "The given common word type isn't a noun" );

		if( ( currentSpecificationItem = firstExclusiveNounSpecificationItem() ) != null )
			{
			do	{
				if( currentSpecificationItem.isExclusiveSpecification() &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem &&
				( foundSpecificationItem = currentSpecificationWordItem.firstNonQuestionSpecificationItem() ) != null &&
				( foundSpecificationWordItem = foundSpecificationItem.specificationWordItem() ) != null )
					{
					if( ( boolResult = foundSpecificationWordItem.findCollection( false, collectionWordItem, this ) ).result != Constants.RESULT_OK )
						return addCollectionResultErrorInWord( 1, null, "I failed to find out if word \"" + collectionWordItem.anyWordTypeString() + "\" is collected with word \"" + foundSpecificationWordItem.anyWordTypeString() + "\"" );

					// No collection found
					if( !boolResult.booleanValue )
						{
						if( ( collectionResult = foundSpecificationWordItem.addCollection( false, false, collectionWordTypeNr, commonWordTypeNr, Constants.NO_COLLECTION_NR, collectionWordItem, this, null ) ).result != Constants.RESULT_OK )
							return addCollectionResultErrorInWord( 1, null, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + collectionWordItem.anyWordTypeString() + "\"" );

						if( collectionWordItem.addCollection( false, false, collectionWordTypeNr, commonWordTypeNr, collectionResult.createdCollectionNr, foundSpecificationWordItem, this, null ).result != Constants.RESULT_OK )
							return addCollectionResultErrorInWord( 1, null, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + collectionWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( foundSpecificationWordItem == null &&
			( currentSpecificationItem = currentSpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
			}

		collectionResult.foundGeneralizationWordItem = currentSpecificationWordItem;
		return collectionResult;
		}

	protected CreateAndAssignResultType addSpecificationInWord( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString, String authorizationKey )
		{
		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != null &&
		// No user is logged in yet
		CommonVariables.currentUserNr > Constants.NO_USER_NR &&
		!specificationWordItem.isAuthorizedForChanges( authorizationKey ) ) )
			return startCreateAndAssignResultErrorInWord( 1, null, "You are not authorized to add this word" );

		if( wordSpecification_ == null )
			{
			if( isAdminWord() )
				return startCreateAndAssignResultErrorInWord( 1, null, "The admin word item cannot have a word specification module" );

			// Create supporting module
			if( ( wordSpecification_ = new WordSpecification( this ) ) == null )
				return startCreateAndAssignResultErrorInWord( 1, null, "I failed to create my word specification module" );
			}

		return wordSpecification_.addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );
		}

	protected CreateAndAssignResultType createSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer writtenSentenceStringBuffer )
		{
		if( isAssignment )
			{
			if( assignmentList_ == null )
				{
				if( isAdminWord() )
					return startCreateAndAssignResultErrorInWord( 1, null, "The admin word item cannot have an assignment list" );

				// Create assignment list
				if( ( assignmentList_ = new SpecificationList( Constants.WORD_ASSIGNMENT_LIST_SYMBOL, this ) ) == null )
					return startCreateAndAssignResultErrorInWord( 1, null, "I failed to create an assignment list" );

				wordListArray_[Constants.WORD_ASSIGNMENT_LIST] = assignmentList_;
				}

			return assignmentList_.createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( isExclusiveGeneralization ? collectionNr( generalizationWordTypeNr, specificationWordItem ) : Constants.NO_COLLECTION_NR ), specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceStringBuffer );
			}

		if( specificationList_ == null )
			{
			if( isAdminWord() )
				return startCreateAndAssignResultErrorInWord( 1, null, "The admin word item cannot have a specification list" );

			// Create specification list
			if( ( specificationList_ = new SpecificationList( Constants.WORD_SPECIFICATION_LIST_SYMBOL, this ) ) == null )
				return startCreateAndAssignResultErrorInWord( 1, null, "I failed to create a specification list" );

			wordListArray_[Constants.WORD_SPECIFICATION_LIST] = specificationList_;
			}

		return specificationList_.createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_COLLECTION_NR, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceStringBuffer );
		}

	protected RelatedResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		if( wordSpecification_ == null )
			return startRelatedResultErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );
		}

	protected RelatedResultType findRelatedSpecification( boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( wordSpecification_ == null )
			return startRelatedResultErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.findRelatedSpecification( false, false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
		}


	// Protected word type methods

	protected void clearGeneralizationWriteLevel( short currentWriteLevel )
		{
		if( wordTypeList_ != null )
			wordTypeList_.clearGeneralizationWriteLevel( isLanguageWord_, currentWriteLevel );
		}

	protected void clearSpecificationWriteLevel( short currentWriteLevel )
		{
		if( wordTypeList_ != null )
			wordTypeList_.clearSpecificationWriteLevel( currentWriteLevel );
		}

	protected void clearRelationWriteLevel( short currentWriteLevel, int contextNr )
		{
		if( contextList_ != null &&
		wordTypeList_ != null &&
		contextList_.hasContext( contextNr ) )
			wordTypeList_.clearRelationWriteLevel( currentWriteLevel );
		}

	protected boolean hasAnyWordType()
		{
		if( wordTypeList_ != null )
			return wordTypeList_.hasAnyWordType();

		return false;
		}

	protected boolean hasFeminineAndMasculineArticle( short articleParameter )
		{
		short oppositeArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		WordItem oppositeArticleWordItem;

		switch( articleParameter )
			{
			// Plural phonetic vowel article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE;
				break;

			// Singular phonetic vowel article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE;
				break;

			// Plural article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE;
				break;

			// Singular article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE;
				break;

			// Plural article
			case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE;
				break;

			// Singular article
			case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE:
				oppositeArticleParameter = Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE;
				break;
			}

		if( ( oppositeArticleWordItem = predefinedWordItem( oppositeArticleParameter ) ) != null )
			return oppositeArticleWordItem.hasWordType( false, Constants.WORD_TYPE_ARTICLE );

		return false;
		}

	protected boolean hasFeminineProperNameEnding()
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.hasFeminineWordEnding();

		return false;
		}

	protected boolean hasFeminineOrMasculineProperNameEnding()
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.hasFeminineOrMasculineWordEnding();

		return false;
		}

	protected boolean hasMasculineProperNameEnding()
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.hasMasculineWordEnding();

		return false;
		}

	protected boolean hasWordType( boolean isAllowingDifferentNoun, short wordTypeNr )
		{
		if( wordTypeList_ != null )
			return ( wordTypeList_.activeWordTypeItem( isAllowingDifferentNoun, isLanguageWord_, wordTypeNr ) != null );

		return false;
		}

	protected boolean isGeneralizationWordTypeAlreadyWritten( short generalizationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, generalizationWordTypeNr ) ) != null )
			return foundWordTypeItem.isGeneralizationWordAlreadyWritten();

		return false;
		}

	protected boolean isSpecificationWordTypeAlreadyWritten( short specificationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( true, isLanguageWord_, specificationWordTypeNr ) ) != null )
			return foundWordTypeItem.isSpecificationWordAlreadyWritten();

		return false;
		}

	protected boolean isRelationWordTypeAlreadyWritten( short relationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( true, isLanguageWord_, relationWordTypeNr ) ) != null )
			return foundWordTypeItem.isRelationWordAlreadyWritten();

		return false;
		}

	protected boolean isCorrectAdjective( short adjectiveParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectAdjective( adjectiveParameter );

		return false;
		}

	protected boolean isCorrectDefiniteArticle( short definiteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		isCorrectFeminineOrMasculineArticleWord( definiteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectDefiniteArticle( definiteArticleParameter );

		return false;
		}

	protected boolean isCorrectIndefiniteArticle( short indefiniteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		isCorrectFeminineOrMasculineArticleWord( indefiniteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectIndefiniteArticle( true, indefiniteArticleParameter );

		return false;
		}

	protected boolean isCorrectHiddenWordType( short wordTypeNr, String compareString, String hideKey )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.isCorrectHiddenWordType( wordTypeNr, compareString, hideKey );

		return false;
		}

	protected boolean isNumeralWordType()
		{
		return hasWordType( false, Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isProperNamePrecededByDefiniteArticle( short definiteArticleParameter )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList_ != null &&
		( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.isProperNamePrecededByDefiniteArticle( definiteArticleParameter );

		return false;
		}

	protected byte deleteAllWordTypesOfCurrentSentence()
		{
		if( wordTypeList_ != null )
			return wordTypeList_.deleteAllWordTypesOfCurrentSentence();

		return Constants.RESULT_OK;
		}

	protected byte deleteWordType( WordTypeItem deleteWordTypeItem )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.deleteWordType( deleteWordTypeItem );

		return Constants.RESULT_OK;
		}

	protected byte hideWordType( short wordTypeNr, String authorizationKey )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.hideWordTypeItem( wordTypeNr, authorizationKey );

		return Constants.RESULT_OK;
		}

	protected byte markGeneralizationWordTypeAsWritten( short wordTypeNr )
		{
		if( wordTypeList_ == null )
			return startErrorInWord( 1, null, "The word type list isn't created yet" );

		return wordTypeList_.markGeneralizationWordTypeAsWritten( isLanguageWord_, wordTypeNr );
		}

	protected byte markSpecificationWordTypeAsWritten( short wordTypeNr )
		{
		if( wordTypeList_ == null )
			return startErrorInWord( 1, null, "The word type list isn't created yet" );

		return wordTypeList_.markSpecificationWordTypeAsWritten( wordTypeNr );
		}

	protected byte markRelationWordTypeAsWritten( short wordTypeNr )
		{
		if( wordTypeList_ == null )
			return startErrorInWord( 1, null, "The word type list isn't created yet" );

		return wordTypeList_.markRelationWordTypeAsWritten( wordTypeNr );
		}

	protected WordTypeItem activeWordTypeItem( short wordTypeNr )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr );

		return null;
		}

	protected WordTypeItem activeWordTypeItem( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr );

		return null;
		}

	protected String activeWordTypeString( short wordTypeNr )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.activeWordTypeString( isLanguageWord_, wordTypeNr );

		return null;
		}

	protected String anyWordTypeString()
		{
		return wordTypeString( true, Constants.NO_WORD_TYPE_NR );
		}

	protected String singularNounString()
		{
		return activeWordTypeString( Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected String wordTypeString( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		String wordTypeString = wordTypeList_.wordTypeString( ( isCheckingAllLanguages || isLanguageWord_ ), wordTypeNr );

		if( wordTypeString == null )
			return ( Constants.QUERY_ITEM_START_STRING + creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + itemNr() + Constants.QUERY_ITEM_END_CHAR );

		return wordTypeString;
		}

	protected WordTypeResultType addWordType( boolean isMultipleWord, boolean isProperNamePrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeNr, int wordLength, String wordTypeString )
		{
		if( wordTypeList_ == null )
			{
			if( isAdminWord() )
				return startWordTypeResultErrorInWord( 1, null, "The admin word item cannot have a word type list" );

			// Create list
			if( ( wordTypeList_ = new WordTypeList( this ) ) == null )
				return startWordTypeResultErrorInWord( 1, null, "I failed to create a word type list" );

			wordListArray_[Constants.WORD_TYPE_LIST] = wordTypeList_;
			}

		return wordTypeList_.addWordType( isLanguageWord_, isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
		}

	protected WordResultType findWordType( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString )
		{
		if( wordTypeList_ != null )
			return wordTypeList_.findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString );

		return new WordResultType();
		}


	// Protected write methods

	protected void deleteTemporaryWriteList()
		{
		if( writeList_ != null )
			writeList_.deleteTemporaryList();
		}

	protected byte createWriteWord( boolean isSkipped, short grammarLevel, GrammarItem selectedGrammarItem )
		{
		if( writeList_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a write list" );

			// Create list
			if( ( writeList_ = new WriteList( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create a write list" );

			wordListArray_[Constants.WORD_WRITE_LIST] = writeList_;
			}

		return writeList_.createWriteItem( isSkipped, grammarLevel, selectedGrammarItem );
		}

	protected byte writeJustificationSpecification( boolean isWritingCurrentSpecificationWordOnly, SpecificationItem justificationSpecificationItem )
		{
		if( justificationSpecificationItem == null )
			return startErrorInWord( 1, null, "The given justification specification item is undefined" );

		if( justificationSpecificationItem.isReplacedOrDeletedItem() )
			return startErrorInWord( 1, null, "The given justification specification item is a replaced or deleted item" );

		if( writeSelectedSpecification( false, false, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, justificationSpecificationItem ) != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to write a selected specification with justification" );

		if( CommonVariables.writtenSentenceStringBuffer == null ||
		CommonVariables.writtenSentenceStringBuffer.length() == 0 )
			return startErrorInWord( 1, null, "I couldn't write the selected specification with justification" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to write a justification sentence" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedRelationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		SpecificationItem currentSpecificationItem = null;

		if( writeWordItem == null )
			return startErrorInWord( 1, null, "The given write word item is undefined" );

		if( ( currentSpecificationItem = firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != null )
			{
			do	{
				if( writeWordItem.hasContextInWord( currentSpecificationItem.relationContextNr(), currentSpecificationItem.specificationWordItem() ) &&
				firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, currentSpecificationItem.isPossessive(), currentSpecificationItem.questionParameter(), writeWordItem ) != null &&
				!currentSpecificationItem.isHiddenSpanishSpecification() )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to write a selected specification" );

					if( CommonVariables.writtenSentenceStringBuffer != null &&
					CommonVariables.writtenSentenceStringBuffer.length() > 0 )
						{
						if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isQuestion ? Constants.INTERFACE_LISTING_RELATED_QUESTIONS : Constants.INTERFACE_LISTING_RELATED_INFORMATION ) ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to write a related header" );

						if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecification( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, SpecificationItem writeSpecificationItem )
		{
		return writeSelectedSpecification( false, isCheckingUserSentenceForIntegrity, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem );
		}

	protected byte writeSelectedSpecification( boolean isAdjustedAssumption, boolean isCheckingUserSentenceForIntegrity, boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		boolean hasRelationContext;
		boolean hasSpecificationCompoundCollection;
		boolean isAssignment;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isOlderItem;
		boolean isQuestion;
		boolean isSelfGenerated;
		boolean isSpecificationGeneralization;
		boolean isUserSpecification;
		boolean isWritingSentenceWithOneSpecificationOnly;
		boolean hasAssignment = false;
		boolean isCombinedSpecification = false;
		boolean isNegativeCompoundSpecification = false;
		boolean isFirstRelatedSpecification = false;
		boolean isLastCompoundSpecification = false;
		boolean isLastRelatedSpecification = false;
		boolean isSelfGeneratedDefinitionConclusion = false;
		int relationContextNr;
		long startNanoTime;
		SpecificationItem foundAssignmentItem;
		SpecificationItem relatedSpecificationItem = null;
		WordItem currentLanguageWordItem;
		WordItem specificationWordItem;
		RelatedResultType relatedResult = new RelatedResultType();

		CommonVariables.learnedFromUserStringBuffer = new StringBuffer();
		CommonVariables.writtenSentenceStringBuffer = new StringBuffer();

		if( writeSpecificationItem == null )
			return startErrorInWord( 1, null, "The given write specification item is undefined" );

		if( writeSpecificationItem.isDeletedItem() )
			return startErrorInWord( 1, null, "The given write specification item is deleted" );

		if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) == null )
			return startErrorInWord( 1, null, "The current language word item is undefined" );

		if( writeSpecificationItem.isHiddenSpanishSpecification() )
			{
			if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI can't write a hidden specification:\n\tSpecificationItem: " + writeSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startErrorInWord( 1, null, "I failed to write an interface warning" );
			}
		else
			{
			hasRelationContext = writeSpecificationItem.hasRelationContext();
			hasSpecificationCompoundCollection = writeSpecificationItem.hasSpecificationCompoundCollection();
			isAssignment = writeSpecificationItem.isAssignment();
			isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
			isNegative = writeSpecificationItem.isNegative();
			isPossessive = writeSpecificationItem.isPossessive();
			isOlderItem = writeSpecificationItem.isOlderItem();
			isQuestion = writeSpecificationItem.isQuestion();
			isSelfGenerated = writeSpecificationItem.isSelfGenerated();
			isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();
			isUserSpecification = writeSpecificationItem.isUserSpecification();

			relationContextNr = writeSpecificationItem.relationContextNr();

			if( ( specificationWordItem = writeSpecificationItem.specificationWordItem() ) != null )
				{
				if( !isWritingCurrentSpecificationWordOnly &&
				answerParameter == Constants.NO_ANSWER_PARAMETER )
					{
					if( writeSpecificationItem.hasSpecificationCollection() &&
					( relatedResult = findRelatedSpecification( true, writeSpecificationItem ) ).result != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to find a related specification" );

					isFirstRelatedSpecification = relatedResult.isFirstRelatedSpecification;
					isLastRelatedSpecification = relatedResult.isLastRelatedSpecification;
					relatedSpecificationItem = relatedResult.relatedSpecificationItem;

					if( relatedSpecificationItem != null &&
					relatedSpecificationItem.isOlderItem() )
						{
						if( hasSpecificationCompoundCollection )
							{
							if( isNegative &&
							!isOlderItem &&

							( !hasFoundAnswerToQuestion() ||
							isSpanishCurrentLanguage() ||

							( !relatedSpecificationItem.isConcludedAssumption() &&
							!relatedSpecificationItem.hasSpecificationBeenWrittenAsAdjustedNegativeAssumption() ) ) )
								isNegativeCompoundSpecification = true;
							}
						else
							{
							if( isWritingCurrentSentenceOnly &&

							( !isSpecificationGeneralization ||
							isLastRelatedSpecification ) )
								isCombinedSpecification = true;
							}
						}
					}

				if( !isAssignment &&
				( foundAssignmentItem = firstAssignmentItem( true, true, true, isPossessive, writeSpecificationItem.questionParameter(), relationContextNr, specificationWordItem ) ) != null )
					{
					if( foundAssignmentItem.hasRelationContext() ||
					!foundAssignmentItem.isArchivedAssignment() ||
					foundAssignmentItem.isSelfGenerated() )
						hasAssignment = true;
					}

				if( hasSpecificationCompoundCollection &&
				!isFirstRelatedSpecification &&
				isLastRelatedSpecification )
					isLastCompoundSpecification = true;

				if( !isSpecificationGeneralization &&
				writeSpecificationItem.isGeneralizationNoun() &&
				writeSpecificationItem.isSelfGeneratedConclusion() )
					isSelfGeneratedDefinitionConclusion = true;
				}

			if( isCombinedSpecification ||

			// Self-generated
			( isSelfGeneratedDefinitionConclusion &&

			( !isExclusiveSpecification ||
			isLastCompoundSpecification ||
			isForcingResponseNotBeingFirstSpecification ||

			( !hasSpecificationCompoundCollection &&
			isFirstRelatedSpecification ) ) ) ||

			// User specification
			( !isSelfGeneratedDefinitionConclusion &&

			( isForcingResponseNotBeingAssignment ||
			writeSpecificationItem.isConditional() ||
			writeSpecificationItem.isCorrectedAssumption() ||

			( !hasAssignment &&

			( hasRelationContext ||
			isSelfGenerated ||
			!writeSpecificationItem.isArchivedAssignment() ) ) ) &&

			( isNegativeCompoundSpecification ||
			isFirstRelatedSpecification ||
			isForcingResponseNotBeingFirstSpecification ||
			relatedSpecificationItem == null ||

			( !isAdjustedAssumption &&
			isNegative &&
			!isQuestion &&
			writeSpecificationItem.assumptionLevel() != relatedSpecificationItem.assumptionLevel() ) ) ) )
				{
				isWritingSentenceWithOneSpecificationOnly = ( isWritingCurrentSpecificationWordOnly &&
															writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer != null );

				// To increase performance, use the last written sentence if still up-to-date
				if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
				writeSpecificationItem.lastWrittenSentenceStringBuffer != null &&

				( isWritingSentenceWithOneSpecificationOnly ||
				isQuestion ||

				// No relation context
				( !hasRelationContext &&

				( isExclusiveSpecification ||
				isUserGeneralizationWord ||

				( isUserSpecification &&

				( !hasSpecificationCompoundCollection ||
				!isWritingCurrentSpecificationWordOnly ) ) ) ) ||

				// Relation context
				( hasRelationContext &&

				( hasAssignment ||
				!writeSpecificationItem.hasRelationContextCurrentlyBeenUpdated() ) ) ) )
					{
					// Use the last written sentence
					if( isCheckingUserSentenceForIntegrity )
						CommonVariables.writtenUserSentenceStringBuffer = new StringBuffer( writeSpecificationItem.lastWrittenSentenceStringBuffer );

					CommonVariables.writtenSentenceStringBuffer = new StringBuffer( isWritingSentenceWithOneSpecificationOnly ? writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer : writeSpecificationItem.lastWrittenSentenceStringBuffer );
					}
				else
					{
					// To increase performance, use the last written sentence of a related specification
					if( isOlderItem &&

					( !isQuestion ||
					// Non-compound question
					!hasSpecificationCompoundCollection ||
					// User question
					!isSelfGenerated ) &&

					relatedSpecificationItem != null &&
					relatedSpecificationItem.lastWrittenSentenceStringBuffer != null )
						// Use the last written sentence of a related specification
						CommonVariables.writtenSentenceStringBuffer = relatedSpecificationItem.lastWrittenSentenceStringBuffer;
					else
						{
						startNanoTime = System.nanoTime();

						if( writeSpecificationSentence( isCheckingUserSentenceForIntegrity, isWritingCurrentSpecificationWordOnly, answerParameter, Constants.NO_GRAMMAR_LEVEL, currentLanguageWordItem.firstGrammarItem(), writeSpecificationItem ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to write a specification sentence" );

						CommonVariables.writingTime += ( System.nanoTime() - startNanoTime );

						// Under certain conditions, the last written sentence will be stored, in order to be re-used if needed
						if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
						CommonVariables.writtenSentenceStringBuffer != null &&
						CommonVariables.writtenSentenceStringBuffer.length() > 0 )
							{
							if( isWritingCurrentSpecificationWordOnly )
								{
								if( writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer == null )
									writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer = new StringBuffer( CommonVariables.writtenSentenceStringBuffer );
								}
							else
								{
								// Skip self-generated negative compound specifications
								if( ( !isNegative ||
								isUserSpecification ||
								!hasSpecificationCompoundCollection ) &&

								// Skip some exclusive generalization specifications
								( !hasRelationContext ||
								!isAssignment ||
								!isExclusiveSpecification ||
								isOlderItem ||
								!isPossessive ||
								writeSpecificationItem.isInactiveAssignment() ) )
									writeSpecificationItem.lastWrittenSentenceStringBuffer = new StringBuffer( CommonVariables.writtenSentenceStringBuffer );
								}
							}
						}
					}

				if( writeSpecificationItem.userNr() != CommonVariables.currentUserNr &&
				CommonVariables.writtenSentenceStringBuffer != null &&
				CommonVariables.writtenSentenceStringBuffer.length() > 0 )
					CommonVariables.learnedFromUserStringBuffer = new StringBuffer( currentLanguageWordItem.interfaceString( Constants.INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_START ) + userNameString( writeSpecificationItem.userNr() ) + currentLanguageWordItem.interfaceString( Constants.INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_END ) );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecificationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		SpecificationItem currentSpecificationItem;

		if( writeWordItem == null )
			return startErrorInWord( 1, null, "The given write word item is undefined" );

		if( ( currentSpecificationItem = firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != null )
			{
			do	{
				if( currentSpecificationItem.specificationWordItem() == writeWordItem &&
				!currentSpecificationItem.isHiddenSpanishSpecification() )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to write a selected specification" );

					if( CommonVariables.writtenSentenceStringBuffer != null &&
					CommonVariables.writtenSentenceStringBuffer.length() > 0 )
						{
						if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WRITE, ( isQuestion ? Constants.INTERFACE_LISTING_SPECIFICATION_QUESTIONS : Constants.INTERFACE_LISTING_SPECIFICATIONS ) ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to write a related header" );

						if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSpecificationSentence( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a word write module" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create my word write module" );
			}

		return wordWrite_.writeSpecificationSentence( isCheckingUserSentenceForIntegrity, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );
		}

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isConcludedSpanishAmbiguousAssumption, boolean isCorrectedAssumptionByKnowledge, boolean isCorrectedAssumptionByOppositeQuestion, boolean isReplacedBySpecificationWithRelation, SpecificationItem writeSpecificationItem )
		{
		boolean isExclusiveSpecification;
		boolean wasHiddenSpanishSpecification;
		short interfaceParameter;

		if( writeSpecificationItem == null )
			return startErrorInWord( 1, null, "The given write specification item is undefined" );

		isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
		wasHiddenSpanishSpecification = writeSpecificationItem.wasHiddenSpanishSpecification();

		if( writeSelectedSpecification( true, false, true, isExclusiveSpecification, false, false, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem ) != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to write the given specification item" );

		if( CommonVariables.writtenSentenceStringBuffer != null &&
		CommonVariables.writtenSentenceStringBuffer.length() > 0 )
			{
			interfaceParameter = ( isCorrectedAssumptionByKnowledge ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE :
									( isCorrectedAssumptionByOppositeQuestion ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_QUESTION :
									( isReplacedBySpecificationWithRelation ? ( writeSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_EARLIER_ASSUMPTIONS_THAT_HAVE_RELATION_WORDS_NOW : Constants.INTERFACE_LISTING_MY_EARLIER_CONCLUSIONS_THAT_HAVE_RELATION_WORDS_NOW ) :
									( isAdjustedSpecification ? ( writeSpecificationItem.isQuestion() ? Constants.INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS :
									( isConcludedSpanishAmbiguousAssumption || writeSpecificationItem.isConcludedAssumption() ?
									( wasHiddenSpanishSpecification ? Constants.INTERFACE_LISTING_MY_HIDDEN_ASSUMPTIONS_THAT_ARE_CONCLUDED : Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONCLUDED ) :
									( wasHiddenSpanishSpecification ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_NOT_HIDDEN_ANYMORE : Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_ADJUSTED ) ) ) :
									( writeSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_MY_QUESTIONS_THAT_ARE_ANSWERED : Constants.INTERFACE_LISTING_YOUR_QUESTIONS_THAT_ARE_ANSWERED ) ) ) ) );

			if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to write a header" );

			if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to write a sentence" );
			}
		else
			{
			if( isExclusiveSpecification )
				return startErrorInWord( 1, null, "I couldn't write the given specification item" );
			}

		return Constants.RESULT_OK;
		}

	protected WriteItem firstActiveWriteItem()
		{
		if( writeList_ != null )
			return writeList_.firstActiveWriteItem();

		return null;
		}
	};

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
