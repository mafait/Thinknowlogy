/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		Storing and processing word information
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

class WordItem extends Item
	{
	// Private constructed variables

	private boolean isChineseLanguage_ = false;
	private boolean isNounWordSpanishAmbiguous_ = false;
	private boolean isSpanishLanguage_ = false;

	private int highestSentenceNrInWord_ = Constants.NO_SENTENCE_NR;

	private	String authorizationKey_ = null;

	// Word item supporting modules
	private WordQuestion wordQuestion_ = null;
	private WordSpecification wordSpecification_ = null;
	private WordWrite wordWrite_ = null;

	// Word item lists
	private SpecificationList assignmentList_ = null;
	private CollectionList collectionList_ = null;
	private ContextList contextList_ = null;
	private GeneralizationList generalizationList_ = null;
	private GrammarList grammarList_ = null;
	private InterfaceList interfaceList_ = null;
	private JustificationList justificationList_ = null;
	private MultipleWordList multipleWordList_ = null;
	private SpecificationList specificationList_ = null;
	private WordTypeList wordTypeList_ = null;
	private WriteList writeList_ = null;

	// Word item lists array
	private List[] wordListArray_ = new List[Constants.NUMBER_OF_WORD_LISTS];


	// Private initialized variables

	private boolean hasFeminineArticle_ = false;
	private boolean isFeminineWord_ = false;
	private boolean hasMasculineArticle_ = false;
	private boolean isMasculineWord_ = false;

	private boolean isLanguageWord_ = false;

	private short wordParameter_ = Constants.NO_WORD_PARAMETER;


	// Private common methods

	private static short nUsers()
		{
		short nUsers = 0;
		GeneralizationItem currentGeneralizationItem;
		WordItem predefinedNounUserWordItem = GlobalVariables.predefinedNounUserWordItem;

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

	private String selectedLanguageNameString( short languageNr )
		{
		WordItem foundLanguageWordItem;

		return ( ( foundLanguageWordItem = languageWordItem( languageNr ) ) != null ?
				foundLanguageWordItem.anyWordTypeString() : Constants.NO_LANGUAGE_NAME_FOUND_STRING );
		}

	private String selectedUserNameString( short userNr )
		{
		short numberOfUsers;
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;

		if( userNr == Constants.NO_USER_NR )
			return Constants.ADMIN_USER_NAME_STRING;

		if( ( numberOfUsers = nUsers() ) > 0 &&
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

	private WordEndingResultType startWordEndingResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startWordEndingResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	private WordTypeResultType startWordTypeResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startWordTypeResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}


	// Private assignment methods

	private byte inactivateAssignment( boolean isAmbiguousRelationContext, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundActiveAssignmentItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem relatedSpecificationItem;
		RelatedResultType relatedResult;

		if( ( foundAssignmentItem = firstActiveNonQuestionAssignmentItem( ( isAmbiguousRelationContext ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == null )
			{
			if( relationContextNr == Constants.NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( relatedResult = findRelatedSpecification( true, true, false, isExclusiveSpecification, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to find a related assignment" );

					relatedSpecificationItem = relatedResult.relatedSpecificationItem;

					if( relatedSpecificationItem != null &&
					// Skip specification
					relatedSpecificationItem.isUserAssignment() &&

					( !isNegative ||
					!relatedSpecificationItem.isNegative() ) )
						{
						if( inactivateActiveAssignment( relatedSpecificationItem ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to inactivate a related active assignment" );
						}
					}
				}
			else	// Has relation context
				{
				if( ( foundActiveAssignmentItem = firstAssignmentItem( true, false, false, isNegative, isSelfGenerated, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
					{
					if( ( foundAssignmentItem = firstAssignmentItem( false, true, false, isNegative, isSelfGenerated, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
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

	private SpecificationItem firstActiveNonQuestionAssignmentItem( int relationContextNr, WordItem specificationWordItem )
		{
		return ( assignmentList_ == null ? null : assignmentList_.firstActiveNonQuestionAssignmentItem( relationContextNr, specificationWordItem ) );
		}

	private SpecificationItem firstAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isSelfGenerated, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( assignmentList_ != null &&

				( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_.firstAssignmentItem( false, false, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == null ) &&

				( !isIncludingInactiveAssignments ||
				( foundAssignmentItem = assignmentList_.firstAssignmentItem( true, false, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == null ) &&

				isIncludingArchivedAssignments ? assignmentList_.firstAssignmentItem( false, true, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	private CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString )
		{
		boolean hasDifferentAssumptionLevel = false;
		boolean hasDifferentRelationContext = false;
		boolean isAmbiguous = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		SpecificationItem createdAssignmentItem;
		SpecificationItem foundAssignmentItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( !isQuestion &&
		inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
			return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate an assignment" );

		if( ( foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == null &&
		// Didn't find a self-generated assignment. Now try to find a confirmed assignment
		isSelfGenerated &&
		!isSpecificationGeneralization )
			foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, false, questionParameter, relationContextNr, specificationWordItem );

		if( foundAssignmentItem != null )
			{
			createAndAssignResult.createdOrFoundSpecificationItem = foundAssignmentItem;
			createAndAssignResult.foundSpecificationItem = foundAssignmentItem;

			if( foundAssignmentItem.assumptionLevel() != assumptionLevel )
				hasDifferentAssumptionLevel = true;

			if( foundAssignmentItem.relationContextNr() != relationContextNr )
				hasDifferentRelationContext = true;

			// Ambiguous if assignment has different tense: active, inactive or archived.
			if( foundAssignmentItem.isInactiveAssignment() != isInactiveAssignment ||
			foundAssignmentItem.isArchivedAssignment() != isArchivedAssignment )
				isAmbiguous = true;
			}

		if( hasDifferentRelationContext ||
		isAmbiguous ||
		foundAssignmentItem == null ||

		( !isSelfGenerated &&
		hasDifferentAssumptionLevel ) )
			{
			if( ( createAndAssignResult = createSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false, isCharacteristicFor, false, false, ( generalizationContextNr == Constants.NO_CONTEXT_NR && specificationContextNr == Constants.NO_CONTEXT_NR ? isEveryGeneralization : false ), isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, false, isUniqueUserRelation, false, GlobalVariables.currentAssignmentLevel, assumptionLevel, GlobalVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null, null ) ).result != Constants.RESULT_OK )
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
					// Replace assignment with different assumption level
					replaceOrDeleteSpecification( foundAssignmentItem, ( foundAssignmentItem.isSelfGeneratedSpecification() ? createdAssignmentItem : null ) ) != Constants.RESULT_OK )
						return addCreateAndAssignResultErrorInWord( 1, null, "I failed to replace or delete an assignment with different assumption level" );
					}
				else
					{
					if( isAmbiguous )
						{
						if( foundAssignmentItem.isActiveItem() )
							{
							// Inactivate active assignment
							if( inactivateActiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
								return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate an active assignment" );
							}
						else
							{
							if( foundAssignmentItem.isInactiveAssignment() &&
							// Avoid archiving inactive assignments that are created during this sentence
							!foundAssignmentItem.hasCurrentInactiveSentenceNr() &&
							// Archive inactive assignment
							archiveInactiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
								return addCreateAndAssignResultErrorInWord( 1, null, "I failed to archive an inactive assignment" );
							}
						}
					}
				}
			}

		return createAndAssignResult;
		}


	// Private collection methods

	private short highestCollectionOrderNrInWord( int collectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.highestCollectionOrderNr( collectionNr ) : Constants.NO_ORDER_NR );
		}

	private int highestCollectionNrInWord()
		{
		return ( collectionList_ != null ?
				collectionList_.highestCollectionNr() : Constants.NO_COLLECTION_NR );
		}


	// Private context methods

	private boolean isContextSubsetInWord( int fullSetContextNr, int subsetContextNr )
		{
		return ( contextList_ != null ?
				contextList_.isContextSubset( fullSetContextNr, subsetContextNr ) : false );
		}

	private WordItem _nextContextWordItem( boolean isIncludingThisItem, int contextNr )
		{
		WordItem searchContextWordItem = ( isIncludingThisItem ? this : nextContextWordItem );

		// Do for all context words
		while( searchContextWordItem != null )
			{
			if( searchContextWordItem.contextItem( contextNr ) != null )
				return searchContextWordItem;

			searchContextWordItem = searchContextWordItem.nextContextWordItem;
			}

		return null;
		}


	// Private grammar methods

	private void markGrammarAsChoiceEnd()
		{
		if( grammarList_ != null )
			grammarList_.markAsChoiceEnd();
		}

	private void markGrammarAsOptionEnd()
		{
		if( grammarList_ != null )
			grammarList_.markAsOptionEnd();
		}

	private byte checkForDuplicateGrammarDefinition()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.checkForDuplicateGrammarDefinition();
		}

	private byte checkGrammar()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		if( grammarList_.isCheckingGrammarNeeded() )
			return grammarList_.checkGrammar();

		// Checking is not needed
		return Constants.RESULT_OK;
		}

	private byte linkLaterDefinedGrammarWords()
		{
		if( grammarList_ == null )
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return grammarList_.linkLaterDefinedGrammarWords();
		}

	private byte shrinkMergedWordsInWriteSentence()
		{
		if( grammarList_ != null &&
		grammarList_.hasMergedWords() )
			return grammarList_.shrinkMergedWordsInWriteSentence();

		// Language has no merged words
		return Constants.RESULT_OK;
		}

	private String grammarString( short wordTypeNr )
		{
		return ( grammarList_ != null ?
				grammarList_.grammarStringInList( wordTypeNr ) : null );
		}

	private GrammarItem firstReadingGrammarItem()
		{
		return ( grammarList_ != null ?
				grammarList_.firstReadingGrammarItem() : null );
		}

	private GrammarItem firstWritingGrammarItem( boolean isQuestion )
		{
		return ( grammarList_ != null ?
				grammarList_.firstWritingGrammarItem( isQuestion ) : null );
		}

	private BoolResultType expandMergedWordsInReadSentence( StringBuffer readUserSentenceStringBuffer )
		{
		BoolResultType boolResult = new BoolResultType();
		if( grammarList_ == null )
			return startBoolResultError( 1, null, "The grammar list isn't created yet" );

		if( grammarList_.hasMergedWords() )
			{
			// Current language has merged words
			boolResult.booleanValue = true;
			boolResult.result = grammarList_.expandMergedWordsInReadSentence( readUserSentenceStringBuffer );
			}

		return boolResult;
		}

	private GrammarResultType createGrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
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

	private GrammarResultType findGrammar( boolean isIgnoringGrammarParameter, short grammarParameter, int grammarStringLength, String grammarString )
		{
		if( grammarList_ != null )
			return grammarList_.findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return new GrammarResultType();
		}

	private WordEndingResultType analyzeWordEnding( short grammarParameter, int originalWordStringLength, String originalWordString )
		{
		if( grammarList_ != null )
			return grammarList_.analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return new WordEndingResultType();
		}


	// Private interface methods

	private byte checkInterface( short interfaceParameter, String interfaceString )
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

	private byte createInterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString )
		{
		if( interfaceList_ == null )
			return startErrorInWord( 1, null, "The interface list isn't created yet" );

		return interfaceList_.createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );
		}

	private String interfaceString_( short interfaceParameter )
		{
		return ( interfaceList_ != null ?
				interfaceList_.interfaceString( interfaceParameter ) : null );
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

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		specificationList_.checkSpecificationForUsage( false, false, false, unusedSpecificationItem ) == Constants.RESULT_OK )
			specificationList_.checkSpecificationForUsage( false, false, true, unusedSpecificationItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		justificationList_ != null )
			justificationList_.checkSpecificationForUsage( unusedSpecificationItem );

		return GlobalVariables.result;
		}

	private SpecificationItem firstNonNegativeUserSpecificationItem( short specificationWordTypeNr )
		{
		return ( specificationList_ == null ? null : specificationList_.firstNonNegativeUserSpecificationItem( specificationWordTypeNr ) );
		}


	// Private word type methods

	private boolean isSingularNounWord()
		{
		return hasWordType( false, Constants.WORD_TYPE_NOUN_SINGULAR );
		}


	// Protected constructed variables

	protected boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = false;

	protected boolean isUserGeneralizationWord = false;
	protected boolean isUserSpecificationWord = false;
	protected boolean isUserRelationWord = false;

	protected boolean isWordCheckedForSolving = false;

	protected short predefinedMultipleWordNr = 0;

	protected WordItem nextAssignmentWordItem = null;
	protected WordItem nextCollectionWordItem = null;
	protected WordItem nextContextWordItem = null;
	protected WordItem nextPossessiveNounWordItem = null;
	protected WordItem nextSpecificationWordItem = null;
	protected WordItem nextTouchedWordItem = null;
	protected WordItem nextUserDefinedProperNounWordItem = null;


	// Constructor of AdminItem

	protected WordItem()
		{
		// This array is not used by AdminItem. Nevertheless, initialize.
		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = null;
		}


	// Constructor of words

	protected WordItem( boolean isLanguageWord, short wordParameter, List myList )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, this );

		// Private constructed variables

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = null;

		// Private initialized variables

		isFeminineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMasculineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );

		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;
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
	protected byte startSystemErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startSystemError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
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

	protected UserSpecificationResultType addUserSpecificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addUserSpecificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected UserSpecificationResultType startUserSpecificationResultErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startUserSpecificationResultError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}


	// Protected virtual item methods

	@Override
	protected void checkForUsage()
		{
		if( assignmentList_ != null &&
		assignmentList_.checkWordItemForUsage( false, false, this ) == Constants.RESULT_OK &&
		assignmentList_.checkWordItemForUsage( true, false, this ) == Constants.RESULT_OK )
			assignmentList_.checkWordItemForUsage( false, true, this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		collectionList_ != null )
			collectionList_.checkWordItemForUsage( this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		contextList_ != null )
			contextList_.checkWordItemForUsage( this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		generalizationList_ != null )
			generalizationList_.checkWordItemForUsage( this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		multipleWordList_ != null )
			multipleWordList_.checkWordItemForUsage( this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.checkWordItemForUsage( false, false, this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		GlobalVariables.adminConditionList != null )
			GlobalVariables.adminConditionList.checkWordItemForUsage( this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		GlobalVariables.adminActionList != null )
			GlobalVariables.adminActionList.checkWordItemForUsage( this );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		GlobalVariables.adminAlternativeList != null )
			GlobalVariables.adminAlternativeList.checkWordItemForUsage( this );
		}

	@Override
	protected boolean hasParameter( int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				wordParameter_ > Constants.NO_WORD_PARAMETER ) );
		}

	@Override
	protected boolean isSorted( Item _nextSortItem )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.

		// Add new word at the end of the list
		return false;
		}

	@Override
	protected BoolResultType findMatchingWordReferenceString( String searchString )
		{
		if( wordTypeList_ == null )
			return startBoolResultErrorInWord( 1, "The word type list isn't created yet" );

		return wordTypeList_.findMatchingWordReferenceString( searchString );
		}

	@Override
	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + Constants.QUERY_WORD_START_CHAR + wordTypeString( true, queryWordTypeNr ) + Constants.QUERY_WORD_END_CHAR );

		if( isAuthorizationRequiredForChanges() )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAuthorizationRequiredForChanges" );

		if( hasFeminineArticle_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFeminineNounWord" );

		if( hasMasculineArticle_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMasculineNounWord" );

		if( isFeminineWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFeminineWord" );

		if( isMasculineWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMasculineWord" );

		if( isLanguageWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isNounWordSpanishAmbiguous_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNounWordSpanishAmbiguous" );

		if( highestSentenceNrInWord_ > Constants.NO_WORD_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "highestSentenceNrInWord:" + highestSentenceNrInWord_ );

		if( wordParameter_ > Constants.NO_WORD_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordParameter:" + wordParameter_ );

		return queryStringBuffer;
		}


	// Protected common methods

	protected void setCurrentLanguageAsChinese()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			currentLanguageWordItem.isChineseLanguage_ = true;
		}

	protected void setHighestSentenceNr( int currentSentenceNr )
		{
		if( currentSentenceNr > highestSentenceNrInWord_ )
			highestSentenceNrInWord_ = currentSentenceNr;
		}

	protected boolean isAdjectiveAssigned()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	protected boolean isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	protected boolean isAdjectiveComparison()
		{
		return ( wordParameter_ >= Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER &&
				wordParameter_ <= Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER );
		}

	protected boolean isAdjectiveComparisonEqual()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_EQUAL ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_SAME );
		}

	protected boolean isAdjectiveComparisonLess()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_EARLIER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_LESS ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_LOWER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER );
		}

	protected boolean isAdjectiveComparisonMore()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_HIGHER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_LARGER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_LATER ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_COMPARISON_MORE );
		}

	protected boolean isAdjectiveEven()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EVEN );
		}

	protected boolean isAdjectiveOdd()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ODD );
		}

	protected boolean isAdjectiveOddOrEven()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ODD ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EVEN );
		}

	protected boolean isAdjectiveFeminineOrMasculine()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );
		}

	protected boolean isAdminWord()
		{
		return ( myList() == null );
		}

	protected boolean isAuthorizationRequiredForChanges()
		{
		return ( authorizationKey_ != null );
		}

	protected boolean isChineseCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		return ( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null &&
				currentLanguageWordItem.isChineseLanguage_ );
		}

	protected boolean isSpanishCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		return ( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null &&
				currentLanguageWordItem.isSpanishLanguage_ );
		}

	protected boolean isFeminineWord()
		{
		return isFeminineWord_;
		}

	protected boolean isMasculineWord()
		{
		return isMasculineWord_;
		}

	protected boolean isFeminineOrMasculineWord()
		{
		return ( isFeminineWord_ ||
				isMasculineWord_ );
		}

	protected boolean isChineseReversedImperativeNoun()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_HEAD ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_NUMBER ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_TAIL ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_VALUE );
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
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_WAS );
		}

	protected boolean isImperativeVerbDisplay()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY );
		}

	protected boolean isImperativeVerbDisplayLoginOrRead()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ );
		}

	protected boolean isImperativeVerbUndoOrRedo()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO );
		}

	protected short userNr( WordItem userWordItem )
		{
		short userNr = nUsers();
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;
		WordItem predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = GlobalVariables.predefinedNounUserWordItem ) != null &&
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

	protected int highestSentenceNrInWord()
		{
		return highestSentenceNrInWord_;
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
		if( assignmentList_ != null &&
		// Check assignments for structural integrity
		assignmentList_.checkStructuralIntegrityOfSpecifications( false, false ) == Constants.RESULT_OK &&
		assignmentList_.checkStructuralIntegrityOfSpecifications( false, false ) == Constants.RESULT_OK &&
		assignmentList_.checkStructuralIntegrityOfSpecifications( false, true ) == Constants.RESULT_OK )
			assignmentList_.checkForDeletedJustificationInReplacedSpecification();

		// Check for unused contexts
		if( GlobalVariables.result == Constants.RESULT_OK &&
		contextList_ != null &&
		contextList_.checkForUnusedRelationContext() != Constants.RESULT_OK )
			return startErrorInWord( 1, null, "I failed to check for unused relation contexts" );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		justificationList_ != null &&
		justificationList_.checkForReplacedOrDeletedSpecifications() == Constants.RESULT_OK &&
		justificationList_.checkForUnreferencedReplacedJustifications() == Constants.RESULT_OK )
			justificationList_.cleanupUnreferencedJustifications();

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		// Check specifications for structural integrity
		specificationList_.checkStructuralIntegrityOfSpecifications( false, false ) == Constants.RESULT_OK )
			specificationList_.checkForDeletedJustificationInReplacedSpecification();

		return GlobalVariables.result;
		}

	protected byte markWordAsFeminine()
		{
		if( isMasculineWord_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already marked as masculine" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnMasculineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on masculine parameters" );

		isFeminineWord_ = true;

		return Constants.RESULT_OK;
		}

	protected byte markWordAsMasculine()
		{
		if( isFeminineWord_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already marked as feminine" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnFeminineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on feminine parameters" );

		isMasculineWord_ = true;

		return Constants.RESULT_OK;
		}

	protected byte relateSingularNounWithFeminineArticle()
		{
		if( hasMasculineArticle_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already related to a masculine article" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnMasculineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on masculine parameters" );

		hasFeminineArticle_ = true;

		return Constants.RESULT_OK;
		}

	protected byte relateSingularNounWithMasculineArticle()
		{
		if( hasFeminineArticle_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( 1, null, "I am already related to a feminine article" );

		if( wordTypeList_ == null ||
		wordTypeList_.checkWordTypesOnFeminineParameters() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to check the word types on feminine parameters" );

		hasMasculineArticle_ = true;

		return Constants.RESULT_OK;
		}

	protected String currentLanguageNameString()
		{
		WordItem currentLanguageWordItem;

		return ( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null ?
				currentLanguageWordItem.anyWordTypeString() : null );
		}

	protected String languageNameString( short languageNr )
		{
		WordItem predefinedNounLanguageWordItem;

		return ( ( predefinedNounLanguageWordItem = GlobalVariables.predefinedNounLanguageWordItem ) != null ?
				predefinedNounLanguageWordItem.selectedLanguageNameString( languageNr ) : null );
		}

	protected String userNameString( short userNr )
		{
		WordItem predefinedNounUserWordItem = GlobalVariables.predefinedNounUserWordItem;

		return ( predefinedNounUserWordItem != null ?
				predefinedNounUserWordItem.selectedUserNameString( userNr ) : null );
		}

	protected String wordTypeNameString( short wordTypeNr )
		{
		WordItem currentLanguageWordItem;

		return ( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null ?
				currentLanguageWordItem.grammarString( wordTypeNr ) : null );
		}

	protected WordItem languageWordItem( short languageNr )
		{
		short numberOfLanguages;
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( ( numberOfLanguages = nLanguages() ) > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
				currentGeneralizationWordItem.isLanguageWord() &&
				languageNr == numberOfLanguages-- )
					return currentGeneralizationItem.generalizationWordItem();
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
		WordItem lastPredefinedWordItem = GlobalVariables.lastPredefinedWordItem;

		if( wordParameter > Constants.NO_WORD_PARAMETER &&
		( currentPredefinedWordItem = GlobalVariables.firstPredefinedWordItem ) != null )
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
		return ( assignmentList_ == null ? 0 : assignmentList_.nActiveAssignments() );
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

		inactiveAssignmentItem.clearStoredSentenceStringBuffer();

		return Constants.RESULT_OK;
		}

	protected byte createNewAssignmentLevelInWord()
		{
		if( assignmentList_ != null &&
		assignmentList_.createNewAssignmentLevelInList() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to create assignment level " + ( GlobalVariables.currentAssignmentLevel + 1 ) );

		return Constants.RESULT_OK;
		}

	protected byte deleteAssignmentLevelInWord()
		{
		if( assignmentList_ != null &&
		assignmentList_.deleteAssignmentLevelInList() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to delete assignment level " + GlobalVariables.currentAssignmentLevel );

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

		activeAssignmentItem.clearStoredSentenceStringBuffer();

		return Constants.RESULT_OK;
		}

	protected SpecificationItem firstActiveNumeralAssignmentItem()
		{
		return ( assignmentList_ == null ? null : assignmentList_.firstActiveNumeralAssignmentItem() );
		}

	protected SpecificationItem firstActiveStringAssignmentItem()
		{
		return ( assignmentList_ == null ? null : assignmentList_.firstActiveStringAssignmentItem() );
		}

	protected SpecificationItem firstNonQuestionActiveAssignmentItem()
		{
		return ( assignmentList_ == null ? null : assignmentList_.firstAssignmentItem( false, false, false, false ) );
		}

	protected SpecificationItem lastActiveNonQuestionAssignmentItem()
		{
		return ( assignmentList_ == null ? null : assignmentList_.lastActiveNonQuestionAssignmentItem() );
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( assignmentList_ == null ? null : assignmentList_.firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion ) );
		}

	protected SpecificationItem firstAssignmentItem( boolean isPossessive, boolean isQuestion, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( assignmentList_ != null &&
				// Active assignment
				( foundAssignmentItem = assignmentList_.firstAssignmentItem( false, false, isPossessive, isQuestion, relationContextNr, specificationWordItem ) ) == null &&
				// Inactive assignment
				( foundAssignmentItem = assignmentList_.firstAssignmentItem( true, false, isPossessive, isQuestion, relationContextNr, specificationWordItem ) ) == null ?
				// Archived assignment
				assignmentList_.firstAssignmentItem( false, true, isPossessive, isQuestion, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem firstNonPossessiveActiveAssignmentItem( WordItem relationWordItem )
		{
		return ( assignmentList_ == null ? null : assignmentList_.firstRelationSpecificationItem( false, relationWordItem ) );
		}

	protected SpecificationItem firstQuestionAssignmentItem()
		{
		SpecificationItem foundAssignmentItem = null;

		return ( assignmentList_ != null &&
				// Active assignment
				( foundAssignmentItem = assignmentList_.firstAssignmentItem( false, false, false, true ) ) == null ?
				// Archived assignment
				assignmentList_.firstAssignmentItem( false, false, true, true ) : foundAssignmentItem );
		}

	protected SpecificationItem firstNonQuestionAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( assignmentList_ != null &&

				( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_.firstNonQuestionAssignmentItem( false, false, isNegative, isPossessive, specificationWordItem ) ) == null ) &&

				( !isIncludingInactiveAssignments ||
				( foundAssignmentItem = assignmentList_.firstNonQuestionAssignmentItem( true, false, isNegative, isPossessive, specificationWordItem ) ) == null ) &&

				isIncludingArchivedAssignments ? assignmentList_.firstNonQuestionAssignmentItem( false, true, isNegative, isPossessive, specificationWordItem ) :
												foundAssignmentItem );
		}

	protected CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, String authorizationKey )
		{
		SpecificationItem currentAssignmentItem;
		SpecificationItem foundSpecificationItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		// Not authorized to assign this (generalization) word
		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != null &&
		// No user is logged in yet
		GlobalVariables.currentUserNr != Constants.NO_USER_NR &&
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
			if( ( foundSpecificationItem = firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) ) != null &&
			// Typical for Spanish: Spanish ambiguous past tense assignment
			!isNegative &&
			foundSpecificationItem.isNegative() )
				foundSpecificationItem = firstSpecificationItem( false, false, false, specificationWordItem );

			if( foundSpecificationItem == null )
				return startCreateAndAssignResultErrorInWord( 1, null, ( specificationWordItem == null ? "String \"" + specificationString + "\" isn't one of my specifications" : "Word \"" + specificationWordItem.anyWordTypeString() + "\" isn't one of my specifications" ) );

			if( foundSpecificationItem.isExclusiveNonPossessiveGeneralization() &&
			assignmentList_.inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, specificationWordItem ) != Constants.RESULT_OK )
				return addCreateAndAssignResultErrorInWord( 1, null, "I failed to inactivate or archive current generalization assignments" );

			if( foundSpecificationItem.isValueSpecification() )
				{
				if( specificationWordItem == null )
					return startCreateAndAssignResultErrorInWord( 1, null, "The given specification word item is undefined" );

				// Assign specification by value
				if( ( currentAssignmentItem = specificationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
					{
					do	{
						if( ( createAndAssignResult = assignSpecification( false, currentAssignmentItem.isInactiveAssignment(), currentAssignmentItem.isArchivedAssignment(), currentAssignmentItem.isCharacteristicFor(), currentAssignmentItem.isEveryGeneralization(), currentAssignmentItem.isExclusiveGeneralization(), currentAssignmentItem.isExclusiveSpecification(), currentAssignmentItem.isNegative(), currentAssignmentItem.isPartOf(), currentAssignmentItem.isPossessive(), currentAssignmentItem.isSpecificationGeneralization(), currentAssignmentItem.isUniqueUserRelation(), currentAssignmentItem.assumptionLevel(), currentAssignmentItem.prepositionParameter(), currentAssignmentItem.questionParameter(), currentAssignmentItem.generalizationWordTypeNr(), currentAssignmentItem.specificationWordTypeNr(), currentAssignmentItem.relationWordTypeNr(), currentAssignmentItem.specificationCollectionNr(), currentAssignmentItem.generalizationContextNr(), currentAssignmentItem.specificationContextNr(), currentAssignmentItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, currentAssignmentItem.nContextRelations(), firstJustificationItem, currentAssignmentItem.specificationWordItem(), currentAssignmentItem.specificationString() ) ).result != Constants.RESULT_OK )
							return addCreateAndAssignResultErrorInWord( 1, null, "I failed to assign specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
						}
					while( ( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
					}
				}
			else
				{
				if( ( createAndAssignResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem.isCharacteristicFor(), foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.isExclusiveGeneralization(), foundSpecificationItem.isExclusiveSpecification(), isNegative, foundSpecificationItem.isPartOf(), isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem.generalizationWordTypeNr(), foundSpecificationItem.specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem.specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
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

	protected void rebuildQuickAccessWordLists()
		{
		nextAssignmentWordItem = null;
		nextCollectionWordItem = null;
		nextContextWordItem = null;
		nextPossessiveNounWordItem = null;
		nextSpecificationWordItem = null;
		nextTouchedWordItem = null;
		nextUserDefinedProperNounWordItem = null;

		if( assignmentList_ != null )
			assignmentList_.addToAssignmentWordQuickAccessList();

		if( collectionList_ != null )
			collectionList_.addToCollectionWordQuickAccessList();

		if( contextList_ != null )
			contextList_.addToContextWordQuickAccessList();

		if( specificationList_ != null )
			{
			if( isSingularNounWord() )
				{
				if( specificationList_.firstPossessiveSpecificationItem() != null )
					specificationList_.addToPossessiveNounWordQuickAccessList();
				}
			else
				{
				if( isProperNounWord() &&
				hasUserNr() )
					specificationList_.addToUserDefinedProperNounWordQuickAccessList();
				}

			specificationList_.addToSpecificationWordQuickAccessList();
			}
		}

	protected int highestCurrentSentenceItemNrInWord( int currentSentenceNr, int currentSentenceItemNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			// Efficiency: Only select lists with current sentence number or higher
			currentWordList.highestSentenceNrInList() >= currentSentenceNr )
				currentSentenceItemNr = currentWordList.highestCurrentSentenceItemNrInList( currentSentenceNr, currentSentenceItemNr );
			}

		return currentSentenceItemNr;
		}

	protected int highestFoundSentenceNrInWord( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestFoundSentenceNr, int maxSentenceNr )
		{
		List currentWordList;

		for( short wordListNr = 0; ( wordListNr < Constants.NUMBER_OF_WORD_LISTS &&
									// Efficiency: Only select word lists with higher sentence number
									highestFoundSentenceNr < highestSentenceNrInWord_ &&
									highestFoundSentenceNr < maxSentenceNr ); wordListNr++ )
			{
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&

			// Skip temporary lists
			( isIncludingTemporaryLists ||
			!currentWordList.isTemporaryList() ) )
				highestFoundSentenceNr = currentWordList.highestFoundSentenceNrInList( isIncludingDeletedItems, highestFoundSentenceNr, maxSentenceNr );
			}

		return highestFoundSentenceNr;
		}

	protected byte decrementItemNrRangeInWord( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			// Efficiency: Only select lists with decrement sentence number of higher
			currentWordList.highestSentenceNrInList() >= decrementSentenceNr &&

			currentWordList.decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to decrement item number range" );
			}

		return Constants.RESULT_OK;
		}

	protected byte decrementSentenceNrsInWord( int startSentenceNr )
		{
		List currentWordList;

		if( startSentenceNr <= Constants.NO_SENTENCE_NR )
			return startError( 1, null, "The given start sentence number is undefined" );

		if( highestSentenceNrInWord_ == startSentenceNr )
			highestSentenceNrInWord_--;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != null &&
			// Efficiency: Only select lists with start sentence number of higher
			currentWordList.highestSentenceNrInList() >= startSentenceNr &&

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
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			// Efficiency: Only select lists with lowest sentence number of higher
			currentWordList.highestSentenceNrInList() >= lowestSentenceNr &&

			currentWordList.deleteSentencesInList( lowestSentenceNr ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to delete sentences in one of my lists" );
			}

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItemsInWord()
		{
		List currentWordList;

		for( short wordListNr = 0; ( wordListNr < Constants.NUMBER_OF_WORD_LISTS && GlobalVariables.nDeletedItems == 0 ); wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
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
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
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
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
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
		return ( collectionList_ != null ?
				collectionList_.hasCollectionNr( collectionNr ) : false );
		}

	protected boolean isCompoundCollection( int collectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.isCompoundCollection( collectionNr ) : false );
		}

	protected boolean isExclusiveCollection( int collectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.isExclusiveCollection( collectionNr ) : false );
		}

	protected boolean isNonCompoundCollection( int collectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.isNonCompoundCollection( collectionNr ) : false );
		}

	protected boolean isNonExclusiveCollection( int collectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.isNonExclusiveCollection( collectionNr ) : false );
		}

	protected boolean isNounWordSpanishAmbiguous()
		{
		return isNounWordSpanishAmbiguous_;
		}

	protected short highestCollectionOrderNrInCollectionWords( int collectionNr )
		{
		short highestCollectionOrderNr = Constants.NO_ORDER_NR;
		short tempCollectionOrderNr;
		WordItem currentCollectionWordItem;

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
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
		return ( collectionList_ != null ?
				collectionList_.collectionNr( collectionWordTypeNr ) : Constants.NO_COLLECTION_NR );
		}

	protected int collectionNr( WordItem commonWordItem )
		{
		return ( collectionList_ != null ?
				collectionList_.collectionNr( commonWordItem ) : Constants.NO_COLLECTION_NR );
		}

	protected int collectionNrByCompoundGeneralizationWordInWord( boolean isExclusiveSpecification, short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		return ( collectionList_ != null ?
				collectionList_.collectionNrByCompoundGeneralizationWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) : Constants.NO_COLLECTION_NR );
		}

	protected int compoundCollectionNr()
		{
		return ( collectionList_ != null ?
				collectionList_.compoundCollectionNr() : Constants.NO_COLLECTION_NR );
		}

	protected int nonCompoundCollectionNr()
		{
		return ( collectionList_ != null ?
				collectionList_.nonCompoundCollectionNr() : Constants.NO_COLLECTION_NR );
		}

	protected int nonCompoundCollectionNrInWord( int compoundCollectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.nonCompoundCollectionNr( compoundCollectionNr ) : Constants.NO_COLLECTION_NR );
		}

	protected int highestCollectionNrInCollectionWords()
		{
		int highestCollectionNr = Constants.NO_COLLECTION_NR;
		int tempCollectionNr;
		WordItem currentCollectionWordItem;

		if( ( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
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
		return ( collectionList_ != null ?
				collectionList_.collectionWordItem( compoundCollectionNr ) : null );
		}

	protected WordItem commonWordItem( int collectionNr )
		{
		return ( collectionList_ != null ?
				collectionList_.commonWordItem( collectionNr ) : null );
		}

	protected WordItem compoundGeneralizationWordItem( int compoundCollectionNr )
		{
		return ( compoundCollectionNr > Constants.NO_COLLECTION_NR &&
				collectionList_ != null ?
				collectionList_.compoundGeneralizationWordItem( compoundCollectionNr ) : null );
		}

	protected WordItem feminineCollectionWordItem()
		{
		return ( collectionList_ != null ?
				collectionList_.feminineCollectionWordItem() : null );
		}

	protected WordItem masculineCollectionWordItem()
		{
		return ( collectionList_ != null ?
				collectionList_.masculineCollectionWordItem() : null );
		}

	protected BoolResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		if( collectionList_ != null )
			return collectionList_.findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return new BoolResultType();
		}

	protected CollectionResultType createCollectionItem( boolean isExclusiveSpecification, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		WordItem currentLanguageWordItem;

		if( commonWordItem == this )
			{
			isNounWordSpanishAmbiguous_ = true;

			if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
				currentLanguageWordItem.isSpanishLanguage_ = true;
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

		return collectionList_.createCollectionItem( isExclusiveSpecification, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
		}


	// Protected context methods

	protected boolean hasContextCurrentlyBeenUpdatedInWord( int contextNr )
		{
		return ( contextList_ != null ?
				contextList_.hasContextCurrentlyBeenUpdated( contextNr ) : false );
		}

	protected boolean hasContextInWord( int contextNr )
		{
		return ( contextList_ != null ?
				contextList_.hasContext( contextNr ) : false );
		}

	protected boolean hasContextInWord( int contextNr, WordItem specificationWordItem )
		{
		return ( contextList_ != null ?
				contextList_.hasContext( contextNr, specificationWordItem ) : false );
		}

	protected boolean isContextSimilarInWord( int firstContextNr, int secondContextNr )
		{
		return ( contextList_ != null ?
				( contextList_.hasContext( firstContextNr ) == contextList_.hasContext( secondContextNr ) ) : true );
		}

	protected boolean isContextSubsetInContextWords( int fullSetContextNr, int subsetContextNr )
		{
		WordItem currentContextWordItem;

		if( fullSetContextNr > Constants.NO_CONTEXT_NR &&
		subsetContextNr > Constants.NO_CONTEXT_NR )
			{
			if( fullSetContextNr == subsetContextNr )
				return true;

			if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
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

		return ( ( foundContextItem = contextItem( contextNr ) ) != null ?
				foundContextItem.contextWordTypeNr() : Constants.NO_WORD_TYPE_NR );
		}

	protected int contextNr( WordItem specificationWordItem )
		{
		return ( contextList_ != null ?
				contextList_.contextNr( specificationWordItem ) : Constants.NO_CONTEXT_NR );
		}

	protected int contextNr( int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
		{
		return ( contextList_ != null ?
				contextList_.contextNr( spanishAmbiguousCollectionNr, specificationWordItem ) : Constants.NO_CONTEXT_NR );
		}

	protected int highestContextNrInWord()
		{
		return ( contextList_ != null ?
				contextList_.highestContextNr() : Constants.NO_CONTEXT_NR );
		}

	protected int nContextWords( int contextNr )
		{
		int nContextWords = 0;
		WordItem currentContextWordItem;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = firstContextWordItem( contextNr ) ) != null )
			{
			// Do for all context words with given context
			do	nContextWords++;
			while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( contextNr ) ) != null );
			}

		return nContextWords;
		}

	protected byte addContext( short contextWordTypeNr, short specificationWordTypeNr, int contextNr, int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
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

		return contextList_.addContext( contextWordTypeNr, specificationWordTypeNr, contextNr, spanishAmbiguousCollectionNr, specificationWordItem );
		}

	protected ContextItem firstActiveContextItem()
		{
		return ( contextList_ != null ?
				contextList_.firstActiveContextItem() : null );
		}

	protected ContextItem contextItem( int contextNr )
		{
		return ( contextList_ != null ?
				contextList_.contextItem( contextNr ) : null );
		}

	protected ContextItem contextItem( boolean isCompoundCollectionSpanishAmbiguous, int nContextWords, int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
		{
		return ( contextList_ != null ?
				contextList_.contextItem( isCompoundCollectionSpanishAmbiguous, nContextWords, spanishAmbiguousCollectionNr, specificationWordItem ) : null );
		}

	protected WordItem firstContextWordItem( int contextNr )
		{
		WordItem currentContextWordItem;

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			return currentContextWordItem._nextContextWordItem( true, contextNr );

		return null;
		}

	protected WordItem _nextContextWordItem( int contextNr )
		{
		return _nextContextWordItem( false, contextNr );
		}


	// Protected database connection methods
/*
	protected byte storeChangesInWordInFutureDatabase()
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			// No need to store items of temporary lists
			!currentWordList.isTemporaryList() &&
			currentWordList.storeChangesInListInFutureDatabase( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
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
		return ( generalizationList_ != null ?
				generalizationList_.firstActiveGeneralizationItem() : null );
		}

	protected GeneralizationItem firstNounSpecificationGeneralizationItem()
		{
		return ( generalizationList_ != null ?
				generalizationList_.firstGeneralizationItem( true, true, false, false ) : null );
		}

	protected GeneralizationItem firstProperNounSpecificationGeneralizationItem()
		{
		return ( generalizationList_ != null ?
				generalizationList_.firstGeneralizationItem( true, false, true, false ) : null );
		}

	protected GeneralizationItem firstSpecificationGeneralizationItem( boolean isOnlySelectingCurrentLanguage )
		{
		return ( generalizationList_ != null ?
				generalizationList_.firstGeneralizationItem( isOnlySelectingCurrentLanguage, false, false, false ) : null );
		}

	protected GeneralizationItem firstRelationGeneralizationItem()
		{
		return ( generalizationList_ != null ?
				generalizationList_.firstGeneralizationItem( false, false, false, true ) : null );
		}

	protected BoolResultType findGeneralization( boolean isRelation, WordItem generalizationWordItem )
		{
		if( generalizationList_ != null )
			return generalizationList_.findGeneralization( isRelation, generalizationWordItem );

		return new BoolResultType();
		}


	// Protected grammar methods

	protected void markGrammarOfCurrentLanguageAsChoiceEnd()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			currentLanguageWordItem.markGrammarAsChoiceEnd();
		}

	protected void markGrammarOfCurrentLanguageAsOptionEnd()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			currentLanguageWordItem.markGrammarAsOptionEnd();
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
		WordItem predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = GlobalVariables.predefinedNounLanguageWordItem ) != null &&
		( currentGeneralizationItem = predefinedNounLanguageWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
				generalizationWordItem.isLanguageWord() )
					nLanguages++;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return nLanguages;
		}

	protected byte checkForDuplicateGrammarDefinitionInCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.checkForDuplicateGrammarDefinition();

		return startErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected byte checkGrammarOfCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.checkGrammar();

		return startErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected byte linkLaterDefinedGrammarWordsInCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.linkLaterDefinedGrammarWords();

		return startErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected byte shrinkMergedWordsInWriteSentenceOfCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.shrinkMergedWordsInWriteSentence();

		return startErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected GrammarItem firstCurrentLanguageReadingGrammarItem()
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.firstReadingGrammarItem();

		return null;
		}

	protected GrammarItem firstCurrentLanguageWritingGrammarItem( boolean isQuestion )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.firstWritingGrammarItem( isQuestion );

		return null;
		}

	protected BoolResultType expandMergedWordsInReadSentenceOfCurrentLanguage( StringBuffer readUserSentenceStringBuffer )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.expandMergedWordsInReadSentence( readUserSentenceStringBuffer );

		return startBoolResultError( 1, null, "The current language word is undefined" );
		}

	protected GrammarResultType createGrammarItemForCurrentLanguage( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );

		return startGrammarResultErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected GrammarResultType findGrammarOfCurrentLanguage( boolean isIgnoringGrammarParameter, short grammarParameter, int grammarStringLength, String grammarString )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return startGrammarResultErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected WordEndingResultType analyzeWordEndingWithCurrentLanguage( short grammarParameter, int originalWordStringLength, String originalWordString )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return startWordEndingResultErrorInWord( 1, null, "The current language word is undefined" );
		}


	// Protected interface methods

	protected void checkSpecificationForUsageOfInvolvedWords( SpecificationItem unusedSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( justificationList_ != null )
			justificationList_.checkSpecificationForUsage( unusedSpecificationItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		// Check this word
		checkSpecificationForUsageInWord( unusedSpecificationItem ) == Constants.RESULT_OK &&
		( currentGeneralizationItem = firstGeneralizationItem() ) != null )
			{
			do	{
				// Also check generalization words
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					currentGeneralizationWordItem.checkSpecificationForUsageInWord( unusedSpecificationItem );
				}
			while( GlobalVariables.result == Constants.RESULT_OK &&
			( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
			}
		}

	protected byte checkInterfaceOfCurrentLanguage( short interfaceParameter, String interfaceString )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.checkInterface( interfaceParameter, interfaceString );

		return startErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected byte createInterfaceForCurrentLanguage( short interfaceParameter, int interfaceStringLength, String interfaceString )
		{
		WordItem currentLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			return currentLanguageWordItem.createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );

		return startErrorInWord( 1, null, "The current language word is undefined" );
		}

	protected String interfaceString( short interfaceParameter )
		{
		String foundInterfaceString = null;
		WordItem currentLanguageWordItem;
		WordItem firstLanguageWordItem;

		if( ( currentLanguageWordItem = GlobalVariables.currentLanguageWordItem ) != null )
			foundInterfaceString = currentLanguageWordItem.interfaceString_( interfaceParameter );

		if( foundInterfaceString == null &&

		// The current language doesn't have the requested interface string.
		// Now try to get interface string of the first language (=English).
		( currentLanguageWordItem == null ||
		interfaceParameter < Constants.INTERFACE_CONSOLE_INITIAL_MESSAGE_DEMO_BUTTONS ||
		( firstLanguageWordItem = GlobalVariables.firstWordItem ) == null ||
		firstLanguageWordItem == currentLanguageWordItem ||
		( foundInterfaceString = firstLanguageWordItem.interfaceString_( interfaceParameter ) ) == null ) )
			return Constants.NO_INTERFACE_STRING_FOUND_STRING;

		return foundInterfaceString;
		}


	// Protected justification methods

	protected void clearJustificationHasBeenWritten()
		{
		if( justificationList_ != null )
			justificationList_.clearJustificationHasBeenWritten();
		}

	protected boolean hasJustification( SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		return ( justificationList_ != null ?
				justificationList_.hasJustification( primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) : false );
		}

	protected boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		return ( justificationList_ != null ?
				justificationList_.hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection() : false );
		}

	protected byte cleanupUnreferencedJustifications()
		{
		if( justificationList_ != null )
			return justificationList_.cleanupUnreferencedJustifications();

		return Constants.RESULT_OK;
		}

	protected byte replaceJustification( JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.replaceJustification( false, obsoleteJustificationItem, replacingJustificationItem );
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.replaceOrDeleteJustification( obsoleteJustificationItem );
		}

	protected byte updateSpecificationOfJustificationsInWord( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( justificationList_ != null )
			return justificationList_.updateSpecificationOfJustifications( obsoleteSpecificationItem, replacingSpecificationItem );

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, short orderNr, int secondarySpecificationCollectionNr )
		{
		if( justificationList_ == null )
			return startErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.writeRelatedJustificationSpecifications( justificationTypeNr, orderNr, secondarySpecificationCollectionNr );
		}

	protected JustificationResultType copyJustificationItem( SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		if( justificationList_ == null )
			return startJustificationResultErrorInWord( 1, null, "The justification list isn't created yet" );

		return justificationList_.copyJustificationItem( newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );
		}

	protected JustificationResultType createJustificationItem( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem additionalProperNounSpecificationItem, JustificationItem attachedJustificationItem )
		{
		if( justificationList_ == null )
			{
			if( isAdminWord() )
				return startJustificationResultErrorInWord( 1, null, "The admin word item cannot have a justification list" );

			if( ( justificationList_ = new JustificationList( this ) ) == null )
				return startJustificationResultErrorInWord( 1, null, "I failed to create a justification list" );

			wordListArray_[Constants.WORD_JUSTIFICATION_LIST] = justificationList_;
			}

		return justificationList_.createJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem, attachedJustificationItem );
		}

	protected JustificationItem foundJustificationItem( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		return ( justificationList_ != null ?
				justificationList_.foundJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) : null );
		}

	protected JustificationItem primarySpecificationJustificationItem( short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		return ( justificationList_ != null ?
				justificationList_.primarySpecificationJustificationItem( justificationTypeNr, primarySpecificationItem ) : null );
		}

	protected JustificationItem primaryAndSecondarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		return ( justificationList_ != null ?
				justificationList_.primaryAndSecondarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, primarySpecificationItem, secondarySpecificationItem ) : null );
		}

	protected JustificationItem secondarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem secondarySpecificationItem )
		{
		return ( justificationList_ != null ?
				justificationList_.secondarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, secondarySpecificationItem ) : null );
		}

	protected SpecificationItem suggestiveQuestionAssumptionSpecificationItem()
		{
		return ( justificationList_ != null ?
				justificationList_.suggestiveQuestionAssumptionSpecificationItem() : null );
		}


	// Protected multiple word methods

	protected boolean isMultipleWord()
		{
		return ( multipleWordList_ != null &&
				multipleWordList_.hasActiveItems() );
		}

	protected short matchingMultipleWordParts( String sentenceString )
		{
		return ( multipleWordList_ != null ?
				multipleWordList_.matchingMultipleWordParts( sentenceString ) : 0 );
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

	protected void itemQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			}
		}

	protected void listQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, StringBuffer queryListStringBuffer )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListStringBuffer );
			}
		}

	protected void parameterQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, int queryParameter )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );
			}
		}

	protected void wordTypeQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, short queryWordTypeNr )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				currentWordList.wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );
			}
		}

	protected boolean hasActiveQuestionWithCompoundCollection()
		{
		return ( specificationList_ == null ? false : specificationList_.hasActiveQuestionWithCompoundCollection() );
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

	protected byte stringQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String queryString )
		{
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			currentWordList.stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString ) != Constants.RESULT_OK )
				return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query strings" );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String wordNameString )
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
				GlobalVariables.hasFoundQuery = true;
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
				currentWordList.wordQueryInList( ( hasFoundMatchingString && isSelectingOnFind ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		boolean isAssignmentOrSpecificationList;
		List currentWordList;

		for( short wordListNr = 0; wordListNr < Constants.NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null )
				{
				isAssignmentOrSpecificationList = ( wordListNr == Constants.WORD_ASSIGNMENT_LIST ||
													wordListNr == Constants.WORD_SPECIFICATION_LIST );

				if( currentWordList.wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, ( isAssignmentOrSpecificationList && isSelectingAttachedJustifications ), ( isAssignmentOrSpecificationList && isSelectingJustificationSpecifications), wordReferenceNameString ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query word references" );
				}
			}

		return Constants.RESULT_OK;
		}


	// Protected question methods

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

	protected byte findAnswersToQuestions( int questionSpecificationCollectionNr, SpecificationItem answerSpecificationItem )
		{
		if( wordQuestion_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a word question module" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create my word question module" );
			}

		return wordQuestion_.findAnswersToQuestions( questionSpecificationCollectionNr, answerSpecificationItem );
		}

	protected byte writeAnswerToQuestion( boolean isNegativeAnswer, boolean isPositiveAnswer, boolean isUncertainAboutRelation, SpecificationItem answerSpecificationItem )
		{
		if( wordQuestion_ == null )
			return startErrorInWord( 1, null, "My word question module isn't created yet" );

		return wordQuestion_.writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );
		}


	// Protected specification methods

	protected void addMyWordToTouchedDuringCurrentSentenceList()
		{
		if( wordSpecification_ != null )
			wordSpecification_.addMyWordToTouchedDuringCurrentSentenceList();
		}

	protected void checkJustificationForUsageInWord( JustificationItem unusedJustificationItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.checkJustificationForUsage( false, false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.checkJustificationForUsage( true, false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.checkJustificationForUsage( false, true, false, unusedJustificationItem ) == Constants.RESULT_OK )
			assignmentList_.checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null &&
		specificationList_.checkJustificationForUsage( false, false, false, unusedJustificationItem ) == Constants.RESULT_OK )
			specificationList_.checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		justificationList_ != null )
			justificationList_.checkJustificationForUsage( unusedJustificationItem );
		}

	protected void initializeVariablesInWord()
		{
		hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = false;

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;
		isWordCheckedForSolving = false;

		nextTouchedWordItem = null;

		if( assignmentList_ != null )
			{
			assignmentList_.initializeSpecificationVariables( false, false );
			assignmentList_.initializeSpecificationVariables( true, false );
			assignmentList_.initializeSpecificationVariables( false, true );
			}

		if( specificationList_ != null )
			specificationList_.initializeSpecificationVariables( false, false );

		if( wordSpecification_ != null )
			wordSpecification_.initializeWordSpecificationVariables();
		}

	protected boolean hadOnceAnsweredSelfGeneratedQuestion()
		{
		return ( specificationList_ == null ? false : specificationList_.hadOnceAnsweredSelfGeneratedQuestion() );
		}

	protected boolean hasAnyUserSpecification()
		{
		return ( specificationList_ == null ? false : specificationList_.hasAnyUserSpecification() );
		}

	protected boolean hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() : false );
		}

	protected boolean hasCurrentlyConfirmedSpecificationButNoRelation()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyConfirmedSpecificationButNoRelation() : false );
		}

	protected boolean hasCurrentlyConfirmedSpecification()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyConfirmedSpecification() : false );
		}

	protected boolean hasCurrentlyCorrectedAssumption()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyCorrectedAssumption() : false );
		}

	protected boolean hasCurrentlyCorrectedAssumptionByKnowledge()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyCorrectedAssumptionByKnowledge() : false );
		}

	protected boolean hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() : false );
		}

	protected boolean hasCurrentlyMoreSpecificSpecification()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.hasCurrentlyMoreSpecificSpecification() : false );
		}

	protected boolean hasMultipleSpecificationWordsWithSameSentenceNr( int creationSentenceNr, int skipThisItemNr, int specificationCollectionNr )
		{
		return ( specificationList_ == null ? false : specificationList_.hasMultipleSpecificationWordsWithSameSentenceNr( creationSentenceNr, skipThisItemNr, specificationCollectionNr ) );
		}

	protected boolean hasNegativeSpecification()
		{
		return ( specificationList_ == null ? false : specificationList_.hasNegativeSpecification() );
		}

	protected boolean hasPossiblyGapInKnowledge( int exclusiveSecondarySpecificationCollectionNr, int primarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		return ( specificationList_ == null ? true : specificationList_.hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationCollectionNr, primarySpecificationItem ) );
		}

	protected boolean hasRelationContextInSpecificationsInWord( int relationContextNr )
		{
		return ( ( assignmentList_ != null &&

				( assignmentList_.hasRelationContextInSpecifications( false, false, false, relationContextNr ) ||
				assignmentList_.hasRelationContextInSpecifications( true, false, false, relationContextNr ) ||
				assignmentList_.hasRelationContextInSpecifications( false, true, false, relationContextNr ) ||
				assignmentList_.hasRelationContextInSpecifications( false, false, true, relationContextNr ) ) ) ||

				( specificationList_ != null &&

				( specificationList_.hasRelationContextInSpecifications( false, false, false, relationContextNr ) ||
				specificationList_.hasRelationContextInSpecifications( false, false, true, relationContextNr ) ) ) );
		}

	protected boolean isJustificationInUse( JustificationItem unusedJustificationItem )
		{
		return ( unusedJustificationItem != null &&

				( unusedJustificationItem.isReplacedItem() ||

				( assignmentList_ != null &&

				( assignmentList_.isJustificationInUse( false, false, unusedJustificationItem ) ||
				assignmentList_.isJustificationInUse( true, false, unusedJustificationItem ) ||
				assignmentList_.isJustificationInUse( false, true, unusedJustificationItem ) ) ) ||

				( specificationList_ != null &&
				specificationList_.isJustificationInUse( false, false, unusedJustificationItem ) ) ) );
		}

	protected int nRemainingSpecificationWordsForWriting( boolean isAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedSpecification, short assumptionLevel, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, int creationSentenceNr )
		{
		if( isAssignment )
			return ( assignmentList_ == null ? 0 : assignmentList_.nRemainingSpecificationWordsForWriting( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, creationSentenceNr ) );

		return ( specificationList_ == null ? 0 : specificationList_.nRemainingSpecificationWordsForWriting( false, isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, creationSentenceNr ) );
		}

	protected int userSpecificationCollectionNr()
		{
		return ( wordSpecification_ != null ?
				wordSpecification_.userSpecificationCollectionNr() : Constants.NO_COLLECTION_NR );
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isUserSpecificationWordSpanishAmbiguous, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( wordSpecification_ == null )
			return startErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, isUserSpecificationWordSpanishAmbiguous, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem );
		}

	protected byte clearStoredSentenceStringWithUnknownPluralNoun( String unknownPluralNounString, WordItem specificationWordItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.clearStoredSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem ) == Constants.RESULT_OK &&
		assignmentList_.clearStoredSentenceStringWithUnknownPluralNoun( true, false, unknownPluralNounString, specificationWordItem ) == Constants.RESULT_OK )
			assignmentList_.clearStoredSentenceStringWithUnknownPluralNoun( false, true, unknownPluralNounString, specificationWordItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.clearStoredSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem );

		return GlobalVariables.result;
		}

	protected byte collectGeneralizations( boolean isExclusiveGeneralization, int generalizationCollectionNr )
		{
		if( assignmentList_ != null &&
		assignmentList_.collectGeneralizations( false, false, isExclusiveGeneralization, generalizationCollectionNr ) == Constants.RESULT_OK &&
		assignmentList_.collectGeneralizations( true, false, isExclusiveGeneralization, generalizationCollectionNr ) == Constants.RESULT_OK )
			assignmentList_.collectGeneralizations( false, true, isExclusiveGeneralization, generalizationCollectionNr );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.collectGeneralizations( false, false, isExclusiveGeneralization, generalizationCollectionNr );

		return GlobalVariables.result;
		}

	protected byte collectSpecificationsInWord( boolean isExclusiveSpecification, boolean isQuestion, int specificationCollectionNr )
		{
		if( assignmentList_ != null &&
		assignmentList_.collectSpecificationsInList( false, false, isExclusiveSpecification, isQuestion, specificationCollectionNr ) == Constants.RESULT_OK &&
		assignmentList_.collectSpecificationsInList( true, false, isExclusiveSpecification, isQuestion, specificationCollectionNr ) == Constants.RESULT_OK )
			assignmentList_.collectSpecificationsInList( false, true, isExclusiveSpecification, isQuestion, specificationCollectionNr );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.collectSpecificationsInList( false, false, isExclusiveSpecification, isQuestion, specificationCollectionNr );

		return GlobalVariables.result;
		}

	protected byte confirmSpecificationButNotItsRelation( SpecificationItem confirmationSpecificationItem )
		{
		if( specificationList_ != null )
			return specificationList_.confirmSpecificationButNotItsRelation( confirmationSpecificationItem );

		return startErrorInWord( 1, null, "I have no specifications" );
		}

	protected byte recalculateAssumptionLevelsInWord()
		{
		if( assignmentList_ != null )
			// Recalculate archived assumptions
			assignmentList_.recalculateAssumptionLevels( false, true );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			specificationList_.recalculateAssumptionLevels( false, false );

		return GlobalVariables.result;
		}

	protected byte removeObsoleteAssumptionJustificationsFromPartSpecificationsInWord()
		{
		if( specificationList_ != null )
			specificationList_.removeObsoleteAssumptionJustificationsFromPartOfSpecifications();

		return GlobalVariables.result;
		}

	protected byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( obsoleteSpecificationItem == null )
			return startErrorInWord( 1, null, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem.isAssignment() )
			{
			if( assignmentList_ == null )
				return startErrorInWord( 1, null, "My assignment list isn't created yet" );

			return assignmentList_.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );
			}

		if( specificationList_ == null )
			return startErrorInWord( 1, null, "The specification list isn't created yet" );

		return specificationList_.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );
		}

	protected byte updateJustificationInSpecifications( boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		if( assignmentList_ != null &&
		assignmentList_.updateJustificationInSpecifications( false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK &&
		assignmentList_.updateJustificationInSpecifications( true, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
			assignmentList_.updateJustificationInSpecifications( false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		specificationList_ != null )
			return specificationList_.updateJustificationInSpecifications( false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );

		return GlobalVariables.result;
		}

	protected byte updateSpecificationOfJustificationsOfInvolvedWords( boolean isIncludingUpdateOfCurrentWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( obsoleteSpecificationItem == null )
			return startErrorInWord( 1, null, "The given obsolete specification item is undefined" );

		if( isIncludingUpdateOfCurrentWord &&
		// Update specifications in justifications of this word
		updateSpecificationOfJustificationsInWord( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to update the specifications in my justification items" );

		// Typical for Spanish
		// Condition block excludes some updates in involved words
		if( ( replacingSpecificationItem == null ||
		replacingSpecificationItem.hasRelationContext() ||
		!obsoleteSpecificationItem.hasRelationContext() ) &&

		( currentGeneralizationItem = firstGeneralizationItem() ) != null )
			{
			// Do for all generalization words
			// Update specifications in justifications of involved words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return startSystemErrorInWord( 1, null, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem.updateSpecificationOfJustificationsInWord( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
					return addErrorInWord( 1, null, "I failed to update the specifications in the justification items in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeConfirmedSpecification( short interfaceParameter, SpecificationItem writeSpecificationItem )
		{
		if( wordSpecification_ == null )
			return startErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.writeConfirmedSpecification( interfaceParameter, writeSpecificationItem );
		}

	protected SpecificationItem bestAssumptionLevelSpecificationItem( boolean isPossessive, WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.bestAssumptionLevelSpecificationItem( isPossessive, specificationWordItem ) );
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( !isIncludingArchivedAssignments ||
				assignmentList_ == null ||
				( foundAssignmentItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, specificationWordItem ) ) == null ) &&

				specificationList_ != null ?
				specificationList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isNegative, boolean isPossessive, int specificationCollectionNr, WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) );
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isIncludingActiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, specificationWordItem, relationWordItem ) ) == null ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, specificationWordItem, relationWordItem ) ) == null ) ) ) &&

				specificationList_ != null ?
					specificationList_.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isIncludingAnsweredQuestions, boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isQuestion, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null ) &&

				( !isIncludingInactiveAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null ) ) ) &&

				specificationList_ != null ?
					specificationList_.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( !isIncludingArchivedAssignments ||
				assignmentList_ == null ||
				( foundAssignmentItem = assignmentList_.bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationCollectionNr, specificationWordItem ) ) == null ) &&

				specificationList_ != null ?
					specificationList_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationCollectionNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isAllowingEmptyGeneralizationContext, boolean isAllowingEmptyRelationContext, boolean isIncludingActiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyGeneralizationContext, isAllowingEmptyRelationContext, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ) == null ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyGeneralizationContext, isAllowingEmptyRelationContext, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ) == null ) ) ) &&

				specificationList_ != null ?
					specificationList_.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyGeneralizationContext, isAllowingEmptyRelationContext, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem firstActiveQuestionSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstActiveSpecificationItem( false, true ) );
		}

	protected SpecificationItem firstAdjectiveSpecificationItem( boolean isNegative )
		{
		return ( specificationList_ == null ? null : specificationList_.firstAdjectiveSpecificationItem( isNegative ) );
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||
				( foundAssignmentItem = assignmentList_.firstSpecificationItem( isNegative, isPossessive, relationContextNr, specificationWordItem ) ) == null ) &&

				specificationList_ != null ?
					specificationList_.firstSpecificationItem( isNegative, isPossessive, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingActiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isQuestion, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_.firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) ) == null ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_.firstSpecificationItem( true, isNegative, isPossessive, isQuestion, specificationWordItem ) ) == null ) ) ) &&

				specificationList_ != null ?
					specificationList_.firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem firstExclusiveSpecificationItem( boolean isIncludingAdjectives )
		{
		return ( specificationList_ == null ? null : specificationList_.firstExclusiveSpecificationItem( isIncludingAdjectives ) );
		}

	protected SpecificationItem firstExclusiveSpecificationItem( WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.firstExclusiveSpecificationItem( specificationWordItem ) );
		}

	protected SpecificationItem firstFeminineOrMasculineSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstFeminineOrMasculineSpecificationItem() );
		}

	protected SpecificationItem firstNegativeSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstNegativeSpecificationItem() );
		}

	protected SpecificationItem firstNonCollectedSpecificationItem( boolean isSelfGenerated, WordItem excludeSpecificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.firstNonCollectedSpecificationItem( isSelfGenerated, excludeSpecificationWordItem ) );
		}

	protected SpecificationItem firstNonCompoundUserSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstNonCompoundUserSpecificationItem() );
		}

	protected SpecificationItem firstNonCompoundCollectionSpecificationItem( int compoundSpecificationCollectionNr )
		{
		return ( specificationList_ == null ? null : specificationList_.firstNonCompoundCollectionSpecificationItem( compoundSpecificationCollectionNr ) );
		}

	protected SpecificationItem firstNonExclusiveSpecificationItem( boolean isNegative, boolean isPossessive, boolean isQuestion, WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.firstNonExclusiveSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) );
		}

	protected SpecificationItem firstNonNegativeNonPossessiveDefinitionSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstNonNegativeNonPossessiveDefinitionSpecificationItem() );
		}

	protected SpecificationItem firstNonQuestionSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstActiveSpecificationItem( false, false ) );
		}

	protected SpecificationItem firstNonQuestionSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isAssignment, boolean isArchivedAssignment )
		{
		if( isAssignment )
			return ( assignmentList_ == null ? null : assignmentList_.firstNonQuestionSpecificationItem( isAllowingEmptyRelationContext, isArchivedAssignment ) );

		return ( specificationList_ == null ? null : specificationList_.firstNonQuestionSpecificationItem( isAllowingEmptyRelationContext, false ) );
		}

	protected SpecificationItem firstOlderNonPossessiveNonQuestionSpecificationItem( boolean isArchivedAssignment )
		{
		SpecificationItem foundAssignmentItem;

		if( assignmentList_ != null &&
		( foundAssignmentItem = assignmentList_.firstOlderNonPossessiveNonQuestionSpecificationItem( isArchivedAssignment ) ) != null )
			return foundAssignmentItem;

		return ( specificationList_ == null ? null : specificationList_.firstOlderNonPossessiveNonQuestionSpecificationItem( false ) );
		}

	protected SpecificationItem firstPossessiveSpecificationItem()
		{
		return ( specificationList_ == null ? null : specificationList_.firstPossessiveSpecificationItem() );
		}

	protected SpecificationItem firstCandidateForQuestionSpecificationItem( boolean isAllowingSpanishPossessiveSpecification )
		{
		SpecificationItem firstSelectedSpecificationItem;

		return ( ( firstSelectedSpecificationItem = firstSpecificationItem( false, false, false, false ) ) != null ?
				firstSelectedSpecificationItem.candidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, true ) : null );
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				( ( foundAssignmentItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == null &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == null ) ) ) &&

				specificationList_ != null ?
					specificationList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;
		SpecificationItem moreRecentSpecificationItem;

		if( specificationList_ != null &&
		( moreRecentSpecificationItem = specificationList_.firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem ) ) != null )
			{
			if( assignmentList_ != null &&
			( foundAssignmentItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem ) ) == null )
				foundAssignmentItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( true, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem );

			if( foundAssignmentItem == null ||

			// Prefer more recent specification over assignment
			( foundAssignmentItem.hasRelationContext() == moreRecentSpecificationItem.hasRelationContext() &&
			foundAssignmentItem.originalSentenceNr() < moreRecentSpecificationItem.originalSentenceNr() ) )
				return moreRecentSpecificationItem;
			}

		return foundAssignmentItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( !isIncludingArchivedAssignments ||
				assignmentList_ == null ||
				( foundAssignmentItem = assignmentList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null ) &&

				specificationList_ != null ?
					specificationList_.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem firstSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( isAssignment ? firstAssignmentItem( isInactiveAssignment, isArchivedAssignment, isQuestion ) :
				( isQuestion ? firstActiveQuestionSpecificationItem() : firstNonQuestionSpecificationItem() ) );
		}

	protected SpecificationItem firstSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		if( isAssignment )
			return ( assignmentList_ != null ?
					( isArchivedAssignment ? assignmentList_.firstAssignmentItem( isIncludingAnsweredQuestions, false, true, questionParameter ) :
					( isInactiveAssignment ? assignmentList_.firstAssignmentItem( isIncludingAnsweredQuestions, true, false, questionParameter ) :
											assignmentList_.firstAssignmentItem( isIncludingAnsweredQuestions, false, false, questionParameter ) ) ) : null );

		return ( specificationList_ == null ? null : specificationList_.firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

	protected SpecificationItem firstSpecificationItem( boolean isNegative, boolean isPossessive, boolean isQuestion, WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) );
		}

	protected SpecificationItem firstSpecificationItem( boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) );
		}

	protected SpecificationItem firstUnhiddenSpanishSpecificationItem()
		{
		return ( specificationList_ != null ?
					specificationList_.firstUnhiddenSpanishSpecificationItem( false ) :
				( assignmentList_ != null ?
					assignmentList_.firstUnhiddenSpanishSpecificationItem( true ) : null ) );
		}

	protected SpecificationItem firstUserSpecificationItem( boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				// Active assignment
				( ( foundAssignmentItem = assignmentList_.firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null &&
				// Inactive assignment
				( foundAssignmentItem = assignmentList_.firstUserSpecificationItem( true, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null &&
				// Archived assignment
				( foundAssignmentItem = assignmentList_.firstUserSpecificationItem( false, true, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null ) ) &&

				specificationList_ != null ?
					specificationList_.firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) : foundAssignmentItem );
		}

	protected SpecificationItem noRelationContextSpecificationItem( boolean isPossessive, boolean isSelfGenerated, WordItem specificationWordItem )
		{
		return ( specificationList_ != null &&

				( assignmentList_ == null ||
				// Skip if archived assignment with relation context is found
				assignmentList_.firstSpecificationItem( true, false, isPossessive, false, specificationWordItem ) == null ) ?
					specificationList_.noRelationContextSpecificationItem( isPossessive, isSelfGenerated, specificationWordItem ) : null );
		}

	protected SpecificationItem partOfSpecificationItem( WordItem specificationWordItem )
		{
		return ( specificationList_ == null ? null : specificationList_.partOfSpecificationItem( specificationWordItem ) );
		}

	protected SpecificationItem sameUserQuestionSpecificationItem( short questionParameter )
		{
		SpecificationItem foundAssignmentItem = null;

		return ( ( assignmentList_ == null ||

				// Active assignment
				( ( foundAssignmentItem = assignmentList_.sameUserQuestionSpecificationItem( false, questionParameter ) ) == null &&
				// Archived assignment
				( foundAssignmentItem = assignmentList_.sameUserQuestionSpecificationItem( true, questionParameter ) ) == null ) ) &&

				specificationList_ != null ?
					specificationList_.sameUserQuestionSpecificationItem( false, questionParameter ) : foundAssignmentItem );
		}

	protected WordItem feminineOrMasculineCommonWordItem( boolean isFeminineWord )
		{
		return ( specificationList_ == null ? null : specificationList_.feminineOrMasculineCommonWordItem( isFeminineWord ) );
		}

	protected CollectionResultType collectExclusiveSpecificationWords( short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		SpecificationItem currentSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem currentSpecificationWordItem = null;
		WordItem foundSpecificationWordItem = null;
		BoolResultType boolResult;
		CollectionResultType collectionResult = new CollectionResultType();

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

		if( collectionWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR &&
		collectionWordTypeNr != Constants.WORD_TYPE_ADJECTIVE )
			return startCollectionResultErrorInWord( 1, null, "The given collection word type isn't a singular noun or an adjective" );

		if( commonWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR )
			return startCollectionResultErrorInWord( 1, null, "The given common word type isn't a singular noun" );

		if( ( currentSpecificationItem = firstExclusiveSpecificationItem( true ) ) != null )
			{
			do	{
				if( currentSpecificationItem.hasNonCompoundSpecificationCollection() &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem &&
				( foundSpecificationItem = currentSpecificationWordItem.firstNonNegativeUserSpecificationItem( collectionWordTypeNr ) ) != null &&
				( foundSpecificationWordItem = foundSpecificationItem.specificationWordItem() ) != null )
					{
					if( ( boolResult = foundSpecificationWordItem.findCollection( false, collectionWordItem, this ) ).result != Constants.RESULT_OK )
						return addCollectionResultErrorInWord( 1, null, "I failed to find out if word \"" + collectionWordItem.anyWordTypeString() + "\" is collected with word \"" + foundSpecificationWordItem.anyWordTypeString() + "\"" );

					// No collection found
					if( !boolResult.booleanValue )
						{
						// Each collection comes in pairs
						if( ( collectionResult = foundSpecificationWordItem.createCollectionItem( true, collectionWordTypeNr, commonWordTypeNr, Constants.NO_COLLECTION_NR, collectionWordItem, this, null ) ).result != Constants.RESULT_OK )
							return addCollectionResultErrorInWord( 1, null, "I failed to collect the first word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with the second word \"" + collectionWordItem.anyWordTypeString() + "\"" );

						if( collectionWordItem.createCollectionItem( true, collectionWordTypeNr, commonWordTypeNr, collectionResult.createdCollectionNr, foundSpecificationWordItem, this, null ).result != Constants.RESULT_OK )
							return addCollectionResultErrorInWord( 1, null, "I failed to collect the second word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with the first word \"" + collectionWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( foundSpecificationWordItem == null &&
			( currentSpecificationItem = currentSpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		collectionResult.foundGeneralizationWordItem = currentSpecificationWordItem;
		return collectionResult;
		}

	protected CreateAndAssignResultType addSpecificationInWord( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString, String authorizationKey )
		{
		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != null &&
		// No user is logged in yet
		GlobalVariables.currentUserNr > Constants.NO_USER_NR &&
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

		return wordSpecification_.addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );
		}

	protected CreateAndAssignResultType copyAndReplaceSpecification( boolean isNewAnsweredQuestion, boolean isNewExclusiveGeneralization, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		if( originalSpecificationItem == null )
			return startCreateAndAssignResultErrorInWord( 1, null, "The given original specification item is undefined" );

		if( originalSpecificationItem.isAssignment() )
			{
			if( assignmentList_ == null )
				return startCreateAndAssignResultErrorInWord( 1, null, "My assignment list isn't created yet" );

			return assignmentList_.copyAndReplaceSpecification( isNewAnsweredQuestion, isNewExclusiveGeneralization, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
			}

		if( specificationList_ == null )
			return startCreateAndAssignResultErrorInWord( 1, null, "My specification list isn't created yet" );

		return specificationList_.copyAndReplaceSpecification( isNewAnsweredQuestion, isNewExclusiveGeneralization, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
		}

	protected CreateAndAssignResultType createSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isCharacteristicFor, boolean isConditional, boolean isCorrectedSpecification, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer storedSentenceStringBuffer, StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer )
		{
		if( isAssignment )
			{
			if( assignmentList_ == null )
				return startCreateAndAssignResultErrorInWord( 1, null, "My assignment list isn't created yet" );

			return assignmentList_.createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isCharacteristicFor, isConditional, isCorrectedSpecification, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( isExclusiveGeneralization ? collectionNr( specificationWordItem ) : Constants.NO_COLLECTION_NR ), specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceStringBuffer, storedSentenceWithOnlyOneSpecificationStringBuffer );
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

		return specificationList_.createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isCharacteristicFor, isConditional, isCorrectedSpecification, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_COLLECTION_NR, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceStringBuffer, storedSentenceWithOnlyOneSpecificationStringBuffer );
		}

	protected RelatedResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		if( wordSpecification_ == null )
			return startRelatedResultErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );
		}

	protected RelatedResultType findRelatedSpecification( boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( wordSpecification_ == null )
			return startRelatedResultErrorInWord( 1, null, "My word specification module isn't created yet" );

		return wordSpecification_.findRelatedSpecification( false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );
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

	protected void clearRelationWriteLevel( short currentWriteLevel )
		{
		if( wordTypeList_ != null )
			wordTypeList_.clearRelationWriteLevel( currentWriteLevel );
		}

	protected boolean hasAnyWordType()
		{
		return ( wordTypeList_ != null ?
				wordTypeList_.hasAnyWordType() : false );
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

		return ( ( oppositeArticleWordItem = predefinedWordItem( oppositeArticleParameter ) ) != null ?
				oppositeArticleWordItem.hasWordType( false, Constants.WORD_TYPE_ARTICLE ) : false );
		}

	protected boolean hasFeminineProperNounEnding()
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NOUN ) ) != null ?
					foundWordTypeItem.hasFeminineWordEnding() : false );
		}

	protected boolean hasFeminineOrMasculineProperNounEnding()
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NOUN ) ) != null ?
					foundWordTypeItem.hasFeminineOrMasculineWordEnding() : false );
		}

	protected boolean hasMasculineProperNounEnding()
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NOUN ) ) != null ?
					foundWordTypeItem.hasMasculineWordEnding() : false );
		}

	protected boolean hasWordType( boolean isAllowingDifferentNoun, short wordTypeNr )
		{
		return ( wordTypeList_ != null ?
				( wordTypeList_.activeWordTypeItem( isAllowingDifferentNoun, isLanguageWord_, wordTypeNr ) != null ) : false );
		}

	protected boolean isSpecificationWordTypeAlreadyWritten( short specificationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( true, isLanguageWord_, specificationWordTypeNr ) ) != null ?
					foundWordTypeItem.isSpecificationWordAlreadyWritten() : false );
		}

	protected boolean isRelationWordTypeAlreadyWritten( short relationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( true, isLanguageWord_, relationWordTypeNr ) ) != null ?
					foundWordTypeItem.isRelationWordAlreadyWritten() : false );
		}

	protected boolean isCorrectAdjective( short adjectiveParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null ?
					foundWordTypeItem.isCorrectAdjective( adjectiveParameter ) : false );
		}

	protected boolean isCorrectDefiniteArticle( short definiteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null ?
					foundWordTypeItem.isCorrectDefiniteArticle( definiteArticleParameter ) : false );
		}

	protected boolean isCorrectIndefiniteArticle( short indefiniteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				
				( ( !hasFeminineArticle_ &&
				!hasMasculineArticle_ ) ||

				( hasFeminineArticle_ &&
				isFeminineArticleParameter( indefiniteArticleParameter ) ) ||

				( hasMasculineArticle_ &&
				isMasculineArticleParameter( indefiniteArticleParameter ) ) ) &&

				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null ?
					foundWordTypeItem.isCorrectIndefiniteArticle( true, indefiniteArticleParameter ) : false );
		}

	protected boolean isCorrectHiddenWordType( short wordTypeNr, String compareString, String hideKey )
		{
		return ( wordTypeList_ != null ?
				wordTypeList_.isCorrectHiddenWordType( wordTypeNr, compareString, hideKey ) : false );
		}

	protected boolean isMatchingChineseWord( String wordString )
		{
		return ( wordTypeList_ != null ?
				wordTypeList_.isMatchingChineseWord( wordString ) : false );
		}

	protected boolean isNumeralWordType()
		{
		return hasWordType( false, Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isProperNounPrecededByDefiniteArticle( short definiteArticleParameter )
		{
		WordTypeItem foundWordTypeItem;

		return ( wordTypeList_ != null &&
				( foundWordTypeItem = wordTypeList_.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NOUN ) ) != null ?
					foundWordTypeItem.isProperNounPrecededByDefiniteArticle( definiteArticleParameter ) : false );
		}

	protected boolean isProperNounWord()
		{
		return hasWordType( false, Constants.WORD_TYPE_PROPER_NOUN );
		}

	protected int bestMatchingChineseWordLength( String wordString )
		{
		return ( wordTypeList_ != null ?
				wordTypeList_.bestMatchingChineseWordLength( wordString ) : 0 );
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
		return ( wordTypeList_ != null ?
				wordTypeList_.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) : null );
		}

	protected WordTypeItem activeWordTypeItem( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		return ( wordTypeList_ != null ?
				wordTypeList_.activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr ) : null );
		}

	protected String activeWordTypeString( short wordTypeNr )
		{
		return ( wordTypeList_ != null ?
				wordTypeList_.activeWordTypeString( isLanguageWord_, wordTypeNr ) : null );
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
		String wordTypeString;

		if( wordTypeList_ == null ||
		( wordTypeString = wordTypeList_.wordTypeString( ( isCheckingAllLanguages || isLanguageWord_ ), wordTypeNr ) ) == null )
			return ( Constants.QUERY_ITEM_START_STRING + creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + itemNr() + Constants.QUERY_ITEM_END_CHAR );

		return wordTypeString;
		}

	protected WordTypeResultType addWordType( boolean isMultipleWord, boolean isProperNounPrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeNr, int wordLength, String wordTypeString )
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

		return wordTypeList_.addWordType( isLanguageWord_, isMultipleWord, isProperNounPrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
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

	protected byte writeJustificationSpecificationInWord( boolean isWritingCurrentSpecificationWordOnly, SpecificationItem justificationSpecificationItem )
		{
		if( wordWrite_ == null )
			return startErrorInWord( 1, null, "My word write module isn't created yet" );

		return wordWrite_.writeJustificationSpecificationInWord( isWritingCurrentSpecificationWordOnly, justificationSpecificationItem );
		}

	protected byte writeSelectedRelationInfo( boolean isActiveAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		if( wordWrite_ == null )
			return startErrorInWord( 1, null, "My word write module isn't created yet" );

		return wordWrite_.writeSelectedRelationInfo( isActiveAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );
		}

	protected byte writeSelectedSpecification( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, SpecificationItem writeSpecificationItem )
		{
		return writeSelectedSpecification( isCheckingUserSentenceForIntegrity, true, true, false, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem );
		}

	protected byte writeSelectedSpecification( boolean isCheckingUserSentenceForIntegrity, boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isJustification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a word write module" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create my word write module" );
			}

		return wordWrite_.writeSelectedSpecification( isCheckingUserSentenceForIntegrity, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isJustification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );
		}

	protected byte writeSelectedSpecificationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, WordItem writeWordItem )
		{
		if( wordWrite_ == null )
			{
			if( isAdminWord() )
				return startErrorInWord( 1, null, "The admin word item cannot have a word write module" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( this ) ) == null )
				return startErrorInWord( 1, null, "I failed to create my word write module" );
			}

		return wordWrite_.writeSelectedSpecificationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, writeWordItem );
		}

	protected byte writeSpecificationSentence( boolean isAssignment, boolean isArchivedAssignment, boolean isCheckingUserSentenceForIntegrity, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ == null )
			return startErrorInWord( 1, null, "My word write module isn't created yet" );

		return wordWrite_.writeSpecificationSentence( isAssignment, isArchivedAssignment, isCheckingUserSentenceForIntegrity, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );
		}

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isCorrectedSpecificationByKnowledge, boolean isCorrectedSpecificationByOppositeSuggestiveQuestion, boolean isForcingResponseNotBeingFirstSpecification, boolean isReplacedBySpecificationWithRelation, boolean isWritingCurrentSpecificationWordOnly, boolean wasHiddenSpanishSpecification, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ == null )
			return startErrorInWord( 1, null, "My word write module isn't created yet" );

		return wordWrite_.writeUpdatedSpecification( isAdjustedSpecification, isCorrectedSpecificationByKnowledge, isCorrectedSpecificationByOppositeSuggestiveQuestion, isForcingResponseNotBeingFirstSpecification, isReplacedBySpecificationWithRelation, isWritingCurrentSpecificationWordOnly, wasHiddenSpanishSpecification, writeSpecificationItem );
		}

	protected WriteItem firstActiveWriteItem()
		{
		return ( writeList_ != null ?
				writeList_.firstActiveWriteItem() : null );
		}
	}

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
