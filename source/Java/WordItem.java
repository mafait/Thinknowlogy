/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
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

class WordItem extends Item
	{
	// Private constructible variables

	private boolean isNounWordSpanishAmbiguous_;

	private	String changeKey_;

	private WordAssignment wordAssignment_;
	private WordCleanup wordCleanup_;
	private WordCollection wordCollection_;
	private WordQuestion wordQuestion_;
	private WordSpecification wordSpecification_;
	private WordType wordType_;
	private WordWrite wordWrite_;
	private WordWriteSentence wordWriteSentence_;
	private WordWriteWords wordWriteWords_;


	// Private loadable variables

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
		WordItem predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem ) != null )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
				{
				do	nUsers++;
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
				}
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

	private WordItem languageWordItemInWord( short languageNr )
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
				if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( userNr == numberOfUsers-- )
						return generalizationWordItem.anyWordTypeString();
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return Constants.NO_USER_NAME_FOUND_STRING;
		}


	// Private assignment methods

	private SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		if( assignmentList != null )
			return assignmentList.firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );

		return null;
		}


	// Private collection methods

	private short highestCollectionOrderNrInWord( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.highestCollectionOrderNr( collectionNr );

		return Constants.NO_ORDER_NR;
		}

	private int highestCollectionNrInWord()
		{
		if( collectionList != null )
			return collectionList.highestCollectionNr();

		return Constants.NO_COLLECTION_NR;
		}

	private int nonCompoundCollectionNrInWord( int compoundCollectionNr )
		{
		if( collectionList != null )
			return collectionList.nonCompoundCollectionNr( compoundCollectionNr );

		return Constants.NO_COLLECTION_NR;
		}


	// Private context methods

	private boolean hasContextCurrentlyBeenUpdatedInWord( int contextNr, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.hasContextCurrentlyBeenUpdated( contextNr, specificationWordItem );

		return false;
		}

	private boolean isContextSubsetInWord( int fullSetContextNr, int subsetContextNr )
		{
		if( contextList != null )
			return contextList.isContextSubset( fullSetContextNr, subsetContextNr );

		return false;
		}


	// Private grammar methods

	private String grammarStringInWord( short wordTypeNr )
		{
		if( grammarList != null )
			return grammarList.grammarStringInList( wordTypeNr );

		return null;
		}


	// Private specification methods

	private byte checkSpecificationForUsageInWord( SpecificationItem unusedSpecificationItem )
		{
		if( assignmentList != null &&
		assignmentList.checkSpecificationItemForUsage( false, false, false, unusedSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList.checkSpecificationItemForUsage( false, true, false, unusedSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList.checkSpecificationItemForUsage( false, true, false, unusedSpecificationItem ) == Constants.RESULT_OK )
			assignmentList.checkSpecificationItemForUsage( false, false, true, unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null &&
		specificationList.checkSpecificationItemForUsage( false, false, false, unusedSpecificationItem ) == Constants.RESULT_OK )
			specificationList.checkSpecificationItemForUsage( false, false, true, unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList != null )
			justificationList.checkSpecificationItemForUsage( unusedSpecificationItem );

		return CommonVariables.result;
		}


	// Protected constructible variables

	protected boolean isUserGeneralizationWord;
	protected boolean isUserSpecificationWord;
	protected boolean isUserRelationWord;

	protected boolean isWordCheckedForSolving;
	protected boolean isWordTouchedDuringCurrentSentence;

	protected short predefinedMultipleWordNr;

	protected SpecificationList assignmentList;
	protected CollectionList collectionList;
	protected ContextList contextList;
	protected GeneralizationList generalizationList;
	protected GrammarList grammarList;
	protected InterfaceList interfaceList;
	protected JustificationList justificationList;
	protected MultipleWordList multipleWordList;
	protected SpecificationList specificationList;
	protected WordTypeList wordTypeList;
	protected WriteList writeList;

	protected List[] wordListArray = new List[Constants.NUMBER_OF_WORD_LISTS];


	// Constructor / deconstructor for AdminItem

	protected WordItem()
		{
		// Private constructible variables

		isNounWordSpanishAmbiguous_ = false;

		changeKey_ = null;

		wordAssignment_ = null;
		wordCleanup_ = null;
		wordCollection_ = null;
		wordQuestion_ = null;
		wordSpecification_ = null;
		wordType_ = null;
		wordWrite_ = null;
		wordWriteSentence_ = null;
		wordWriteWords_ = null;

		// Private loadable variables

		isFemale_ = false;
		isFeminineWord_ = false;
		isMale_ = false;
		isMasculineWord_ = false;
		isLanguageWord_ = false;

		wordParameter_ = Constants.NO_WORD_PARAMETER;

		// Protected constructible variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

		predefinedMultipleWordNr = 0;

		assignmentList = null;
		collectionList = null;
		contextList = null;
		generalizationList = null;
		grammarList = null;
		interfaceList = null;
		justificationList = null;
		multipleWordList = null;
		writeList = null;
		specificationList = null;
		wordTypeList = null;

		for( short wordListNr : Constants.WordLists )
			wordListArray[wordListNr] = null;
		}


	// Constructor / deconstructor

	protected WordItem( boolean isLanguageWord, short wordParameter, List myList )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, this );

		// Private constructible variables

		isNounWordSpanishAmbiguous_ = false;

		changeKey_ = null;

		wordAssignment_ = null;
		wordCleanup_ = null;
		wordCollection_ = null;
		wordQuestion_ = null;
		wordSpecification_ = null;
		wordType_ = null;
		wordWrite_ = null;
		wordWriteSentence_ = null;
		wordWriteWords_ = null;

		// Private loadable variables

		isFemale_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE );
		isFeminineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMale_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isMasculineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;

		// Protected constructible variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

		predefinedMultipleWordNr = 0;

		assignmentList = null;
		collectionList = null;
		contextList = null;
		generalizationList = null;
		grammarList = null;
		interfaceList = null;
		justificationList = null;
		multipleWordList = null;
		writeList = null;
		specificationList = null;
		wordTypeList = null;

		for( short wordListNr : Constants.WordLists )
			wordListArray[wordListNr] = null;

		// Always create the word cleanup module
		if( ( wordCleanup_ = new WordCleanup( this ) ) == null )
			startSystemError( 1, null, anyWordTypeString(), "I failed to create my word cleanup module" );
		}


	// Protected methods

	protected byte addErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected byte addErrorWithWordListNr( short wordListNr, int methodLevel, String moduleNameString, String errorString )
		{
		return addError( wordListChar( wordListNr ), ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected byte startErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}
	protected byte startSystemErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startSystemError( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}


	// Protected virtual item methods

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				wordParameter_ > Constants.NO_WORD_PARAMETER ) );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.

		// Add at the end of the list
		return false;
		}

	protected ReferenceResultType findMatchingWordReferenceString( String searchString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( wordTypeList != null )
			return wordTypeList.findMatchingWordReferenceString( searchString );

		referenceResult.result = startErrorInWord( 1, null, "The word type list isn't created yet" );
		return referenceResult;
		}

	protected byte checkForUsage()
		{
		if( assignmentList != null &&
		assignmentList.checkWordItemForUsage( false, false, this ) == Constants.RESULT_OK &&
		assignmentList.checkWordItemForUsage( true, false, this ) == Constants.RESULT_OK )
			assignmentList.checkWordItemForUsage( false, true, this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		collectionList != null )
			collectionList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		contextList != null )
			contextList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		generalizationList != null )
			generalizationList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		multipleWordList != null )
			multipleWordList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			specificationList.checkWordItemForUsage( false, false, this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		CommonVariables.adminConditionList != null )
			CommonVariables.adminConditionList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		CommonVariables.adminActionList != null )
			CommonVariables.adminActionList.checkWordItemForUsage( this );

		if( CommonVariables.result == Constants.RESULT_OK &&
		CommonVariables.adminAlternativeList != null )
			CommonVariables.adminAlternativeList.checkWordItemForUsage( this );

		return CommonVariables.result;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		baseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + Constants.QUERY_WORD_START_CHAR + wordTypeString( true, queryWordTypeNr ) + Constants.QUERY_WORD_END_CHAR );

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

		if( isNeedingAuthorizationForChanges() )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNeedingAuthorizationForChanges" );

		if( isNounWordSpanishAmbiguous_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNounWordSpanishAmbiguous" );

		if( wordParameter_ > Constants.NO_WORD_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordParameter:" + wordParameter_ );

		return queryStringBuffer;
		}


	// Protected common methods

	protected boolean hasItems()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null &&
			wordListArray[wordListNr].hasItems() )
				return true;
			}

		return false;
		}

	protected boolean isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	protected boolean isAdjectiveEmpty()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EMPTY );
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

	protected boolean isNeedingAuthorizationForChanges()
		{
		return ( changeKey_ != null );
		}

	protected boolean isNounDeveloper()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_DEVELOPER );
		}

	protected boolean isNounExpert()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_EXPERT );
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

	protected boolean isNounUser()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_USER );
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

	protected boolean isVerbImperativeLoginOrReadOrShow()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW );
		}

	protected boolean isVerbUndoOrRedo()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO ||
				wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO );
		}

	protected boolean isUserDefinedWord()
		{
		return ( wordParameter_ == Constants.NO_WORD_PARAMETER );
		}

	protected short wordParameter()
		{
		return wordParameter_;
		}

	protected short getUserNr( WordItem userWordItem )
		{
		short userNr = nUsers();
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;
		WordItem predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem ) != null )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
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
			}

		return Constants.NO_USER_NR;
		}

	protected byte assignChangePermissions( String changeKey )
		{
		if( changeKey_ == null )
			changeKey_ = changeKey;
		else
			{
			// Not the content of the string, but its address
			// identifies the owner. So, compare the addresses.
			if( changeKey_ != changeKey )
				return startErrorInWord( 1, null, "The change key is already assigned" );
			}

		return Constants.RESULT_OK;
		}

	protected byte markWordAsFemale()
		{
		if( !isMale_ ||
		hasCorrectedAssumption() )
			{
			if( wordTypeList != null &&
			wordTypeList.checkWordTypesOnMasculineParameters() == Constants.RESULT_OK )
				{
				isFemale_ = true;
				isMale_ = false;
				}
			else
				return addErrorInWord( 1, null, "I failed to check the word types on masculine parameters" );
			}
		else
			return startErrorInWord( 1, null, "I am already masculine" );

		return Constants.RESULT_OK;
		}

	protected byte markWordAsFeminine()
		{
		if( !isMasculineWord_ ||
		hasCorrectedAssumption() )
			{
			if( wordTypeList != null &&
			wordTypeList.checkWordTypesOnMasculineParameters() == Constants.RESULT_OK )
				{
				isFeminineWord_ = true;
				isMasculineWord_ = false;
				}
			else
				return addErrorInWord( 1, null, "I failed to check the word types on masculine parameters" );
			}
		else
			return startErrorInWord( 1, null, "I am already masculine" );

		return Constants.RESULT_OK;
		}

	protected byte markWordAsMale()
		{
		if( !isFemale_ ||
		hasCorrectedAssumption() )
			{
			if( wordTypeList != null &&
			wordTypeList.checkWordTypesOnFeminineParameters() == Constants.RESULT_OK )
				{
				isFemale_ = false;
				isMale_ = true;
				}
			else
				return addErrorInWord( 1, null, "I failed to check the word types on feminine parameters" );
			}
		else
			return startErrorInWord( 1, null, "I am already feminine" );

		return Constants.RESULT_OK;
		}

	protected byte markWordAsMasculine()
		{
		if( !isFeminineWord_ ||
		hasCorrectedAssumption() )
			{
			if( wordTypeList != null &&
			wordTypeList.checkWordTypesOnFeminineParameters() == Constants.RESULT_OK )
				{
				isFeminineWord_ = false;
				isMasculineWord_ = true;
				}
			else
				return addErrorInWord( 1, null, "I failed to check the word types on feminine parameters" );
			}
		else
			return startErrorInWord( 1, null, "I am already feminine" );

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

	protected WordItem languageWordItem( short languageNr )
		{
		WordItem predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem ) != null )
			return predefinedNounLanguageWordItem.languageWordItemInWord( languageNr );

		return null;
		}

	protected WordItem nextWordItem()
		{
		return (WordItem)nextItem;
		}

	protected WordItem predefinedWordItem( short wordParameter )
		{
		WordItem currentWordItem;
		WordItem lastPredefinedWordItem = CommonVariables.lastPredefinedWordItem;

		if( wordParameter > Constants.NO_WORD_PARAMETER &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( currentWordItem.wordParameter() == wordParameter &&
				!currentWordItem.isMultipleWord() )
					return currentWordItem;
				}
			while( currentWordItem != lastPredefinedWordItem &&
			( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return null;
		}


	// Protected assignment methods

	protected int nActiveAssignments()
		{
		if( assignmentList != null )
			return assignmentList.nActiveAssignments();

		return 0;
		}

	protected byte createNewAssignmentLevelInWord()
		{
		if( assignmentList != null &&
		assignmentList.createNewAssignmentLevelInList() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to create assignment level " + ( CommonVariables.currentAssignmentLevel + 1 ) );

		return Constants.RESULT_OK;
		}

	protected byte deleteAssignmentLevelInWord()
		{
		if( assignmentList != null &&
		assignmentList.deleteAssignmentLevelInList() != Constants.RESULT_OK )
			return addErrorInWord( 1, null, "I failed to delete assignment level " + CommonVariables.currentAssignmentLevel );

		return Constants.RESULT_OK;
		}

	protected byte inactivateActiveAssignment( SpecificationItem activeAssignmentItem )
		{
		if( wordAssignment_ != null )
			return wordAssignment_.inactivateActiveAssignment( activeAssignmentItem );

		return startErrorInWord( 1, null, "My word assignment module isn't created yet" );
		}

	protected byte archiveInactiveAssignment( SpecificationItem inactiveAssignmentItem )
		{
		if( wordAssignment_ != null )
			return wordAssignment_.archiveInactiveAssignment( inactiveAssignmentItem );

		return startErrorInWord( 1, null, "My word assignment module isn't created yet" );
		}

	protected SpecificationResultType getAssignmentOrderNr()
		{
		if( wordAssignment_ != null )
			return wordAssignment_.getAssignmentOrderNr();

		return new SpecificationResultType();
		}

	protected SpecificationResultType getAssignmentWordParameter()
		{
		if( wordAssignment_ != null )
			return wordAssignment_.getAssignmentWordParameter();

		return new SpecificationResultType();
		}

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, String changeKey )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isAuthorizedForChanges( changeKey ) &&

		( specificationWordItem == null ||
		// No user is logged in yet
		CommonVariables.currentUserNr == Constants.NO_USER_NR ||
		specificationWordItem.isAuthorizedForChanges( changeKey ) ) )
			{
			if( wordAssignment_ != null ||
			// Create supporting module
			( wordAssignment_ = new WordAssignment( this ) ) != null )
				return wordAssignment_.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString );

			startErrorInWord( 1, null, "I failed to create my word assignment module" );
			}
		else
			startErrorInWord( 1, null, "You are not authorized to assign this word" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem firstActiveAssignmentItem( short questionParameter )
		{
		if( assignmentList != null )
			return assignmentList.firstAssignmentItem( false, false, false, questionParameter );

		return null;
		}

	protected SpecificationItem firstActiveAssignmentItem( boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		if( assignmentList != null )
			return assignmentList.firstRelationSpecificationItem( false, false, isPossessive, questionParameter, relationWordItem );

		return null;
		}

	protected SpecificationItem firstActiveNumeralAssignmentItem()
		{
		if( assignmentList != null )
			return assignmentList.firstActiveNumeralAssignmentItem();

		return null;
		}

	protected SpecificationItem firstActiveStringAssignmentItem()
		{
		if( assignmentList != null )
			return assignmentList.firstActiveStringAssignmentItem();

		return null;
		}

	protected SpecificationItem firstNonQuestionActiveAssignmentItem()
		{
		if( assignmentList != null )
			return assignmentList.firstAssignmentItem( false, false, false, false );

		return null;
		}

	protected SpecificationItem lastActiveNonQuestionAssignmentItem()
		{
		if( assignmentList != null )
			return assignmentList.lastActiveNonQuestionAssignmentItem();

		return null;
		}

	protected SpecificationItem firstActiveNonQuestionAssignmentItem( boolean isDifferentRelationContext, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		if( assignmentList != null )
			return assignmentList.firstActiveNonQuestionAssignmentItem( isDifferentRelationContext, isPossessive, relationContextNr, specificationWordItem );

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList.firstAssignmentItem( false, false, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstAssignmentItem( true, false, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList.firstAssignmentItem( false, true, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList.firstAssignmentItem( false, false, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstAssignmentItem( true, false, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList.firstAssignmentItem( false, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstNonQuestionAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList.firstAssignmentItem( false, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstAssignmentItem( true, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList.firstAssignmentItem( false, true, isNegative, isPossessive, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstQuestionAssignmentItem()
		{
		SpecificationItem firstAssignmentItem = null;

		if( assignmentList != null &&
		// Active assignment
		( firstAssignmentItem = assignmentList.firstAssignmentItem( false, false, false, true ) ) == null )
			{
			// Inactive assignment
			if( ( firstAssignmentItem = assignmentList.firstAssignmentItem( false, true, false, true ) ) == null )
				// Archived assignment
				firstAssignmentItem = assignmentList.firstAssignmentItem( false, false, true, true );
			}

		return firstAssignmentItem;
		}

	protected SpecificationItem firstRecentlyReplacedAssignmentItem( boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		if( assignmentList != null )
			return assignmentList.firstRecentlyReplacedAssignmentItem( isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

		return null;
		}


	// Protected cleanup methods

	protected void deleteTemporaryWriteList()
		{
		if( writeList != null )
			writeList.deleteTemporaryList();
		}

	protected void getHighestInUseSentenceNrInWord( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		if( wordCleanup_ != null )
			wordCleanup_.getHighestInUseSentenceNr( isIncludingDeletedItems, isIncludingTemporaryLists, isLanguageWord_, highestSentenceNr );
		}

	protected void setCurrentItemNrInWord()
		{
		if( wordCleanup_ != null )
			wordCleanup_.setCurrentItemNr( isLanguageWord_ );
		}

	protected byte clearReplacedInfoInWord()
		{
		if( wordCleanup_ != null )
			wordCleanup_.clearReplacedInfo();

		return CommonVariables.result;
		}

	protected byte decrementItemNrRangeInWord( int startSentenceNr, int decrementItemNr, int decrementOffset )
		{
		if( wordCleanup_ != null )
			return wordCleanup_.decrementItemNrRange( isLanguageWord_, startSentenceNr, decrementItemNr, decrementOffset );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte decrementSentenceNrsInWord( int startSentenceNr )
		{
		if( wordCleanup_ != null )
			return wordCleanup_.decrementSentenceNrs( isLanguageWord_, startSentenceNr );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte deleteSentencesInWord( int lowestSentenceNr )
		{
		if( wordCleanup_ != null )
			return wordCleanup_.deleteSentences( isLanguageWord_, lowestSentenceNr );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte redoCurrentSentence()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.redoCurrentSentence();

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte removeFirstRangeOfDeletedItemsInWord()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.removeFirstRangeOfDeletedItems( isLanguageWord_ );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte undoCurrentSentence()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.undoCurrentSentence();

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}


	// Protected collection methods

	protected boolean hasCollection()
		{
		return ( collectionList != null &&
				collectionList.hasItems() );
		}

	protected boolean hasCollectionNr( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.hasCollectionNr( collectionNr );

		return false;
		}

	protected boolean hasCollectionNr( int collectionNr, WordItem commonWordItem )
		{
		if( collectionList != null )
			return collectionList.hasCollectionNr( collectionNr, commonWordItem );

		return false;
		}

	protected boolean hasNonExclusiveCollection( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.hasNonExclusiveCollection( collectionNr );

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.isCompoundCollection( collectionNr );

		return false;
		}

	protected boolean isCollectionSpanishAmbiguous( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.isCollectionSpanishAmbiguous( collectionNr );

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr, WordItem commonWordItem )
		{
		if( collectionList != null )
			return collectionList.isCompoundCollection( collectionNr, commonWordItem );

		return false;
		}

	protected boolean isNonCompoundCollection( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.isNonCompoundCollection( collectionNr );

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
		if( collectionList != null )
			return collectionList.collectionOrderNr( collectionNr, collectionWordItem, commonWordItem );

		return Constants.NO_ORDER_NR;
		}

	protected short highestCollectionOrderNrInAllWords( int collectionNr )
		{
		short tempCollectionOrderNr;
		short highestCollectionOrderNr = Constants.NO_ORDER_NR;
		WordItem currentWordItem;

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( ( tempCollectionOrderNr = currentWordItem.highestCollectionOrderNrInWord( collectionNr ) ) > highestCollectionOrderNr )
					highestCollectionOrderNr = tempCollectionOrderNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
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
		if( collectionList != null )
			return collectionList.collectionNr( collectionWordTypeNr, commonWordItem );

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrByCompoundGeneralizationWordInWord( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		if( collectionList != null )
			return collectionList.collectionNrByCompoundGeneralizationWord( collectionWordTypeNr, compoundGeneralizationWordItem );

		return Constants.NO_COLLECTION_NR;
		}

	protected int compoundCollectionNr( short collectionWordTypeNr )
		{
		if( collectionList != null )
			return collectionList.compoundCollectionNr( collectionWordTypeNr );

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNr( short collectionWordTypeNr )
		{
		if( collectionList != null )
			return collectionList.nonCompoundCollectionNr( collectionWordTypeNr );

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNrInAllWords( int compoundCollectionNr )
		{
		int nonCompoundCollectionNr;
		WordItem currentWordItem;

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR &&
		( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( ( nonCompoundCollectionNr = currentWordItem.nonCompoundCollectionNrInWord( compoundCollectionNr ) ) > Constants.NO_COLLECTION_NR )
					return nonCompoundCollectionNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int highestCollectionNrInAllWords()
		{
		int tempCollectionNr;
		int highestCollectionNr = Constants.NO_COLLECTION_NR;
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( ( tempCollectionNr = currentWordItem.highestCollectionNrInWord() ) > highestCollectionNr )
					highestCollectionNr = tempCollectionNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return highestCollectionNr;
		}

	protected CollectionResultType addCollection( boolean isExclusiveSpecification, boolean isSpecificationGeneralization, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		WordItem currentLanguageWordItem;

		if( commonWordItem == this &&
		isSingularOrPluralNoun( commonWordTypeNr ) )
			{
			isNounWordSpanishAmbiguous_ = true;

			if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
				currentLanguageWordItem.isNounWordSpanishAmbiguous_ = true;
			}

		if( wordCollection_ != null ||
		// Create supporting module
		( wordCollection_ = new WordCollection( this ) ) != null )
			return wordCollection_.addCollection( isExclusiveSpecification, isSpecificationGeneralization, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );

		collectionResult.result = startErrorInWord( 1, null, "I failed to create my word collection module" );
		return collectionResult;
		}

	protected CollectionResultType addCollectionByGeneralization( short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		if( wordCollection_ != null ||
		// Create supporting module
		( wordCollection_ = new WordCollection( this ) ) != null )
			return wordCollection_.addCollectionByGeneralization( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );

		collectionResult.result = startErrorInWord( 1, null, "I failed to create my word collection module" );
		return collectionResult;
		}

	protected CollectionResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		if( collectionList != null )
			return collectionList.findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return new CollectionResultType();
		}

	protected byte addCollectionByGeneralization( boolean isExclusiveGeneralization, boolean isQuestion, short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		if( wordCollection_ != null ||
		// Create supporting module
		( wordCollection_ = new WordCollection( this ) ) != null )
			return wordCollection_.addCollectionByGeneralization( isExclusiveGeneralization, isQuestion, collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );

		return startErrorInWord( 1, null, "I failed to create my word collection module" );
		}

	protected WordItem collectionWordItem( int compoundCollectionNr, WordItem notThisCommonWordItem )
		{
		if( collectionList != null )
			return collectionList.collectionWordItem( compoundCollectionNr, notThisCommonWordItem );

		return null;
		}

	protected WordItem commonWordItem( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.commonWordItem( collectionNr );

		return null;
		}

	protected WordItem compoundGeneralizationWordItem( int compoundCollectionNr )
		{
		if( collectionList != null )
			return collectionList.compoundGeneralizationWordItem( compoundCollectionNr );

		return null;
		}

	protected WordItem feminineCollectionWordItem()
		{
		if( collectionList != null )
			return collectionList.feminineCollectionWordItem();

		return null;
		}


	// Protected context methods

	protected boolean hasContextCurrentlyBeenUpdatedInAllWords( int contextNr, WordItem specificationWordItem )
		{
		WordItem currentWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( currentWordItem.hasContextCurrentlyBeenUpdatedInWord( contextNr, specificationWordItem ) )
					return true;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return false;
		}

	protected boolean hasContextInWord( int contextNr )
		{
		if( contextList != null )
			return contextList.hasContext( contextNr );

		return false;
		}

	protected boolean hasContextInWord( int contextNr, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.hasContext( contextNr, specificationWordItem );

		return false;
		}

	protected boolean isContextSimilarInWord( int firstContextNr, int secondContextNr )
		{
		if( contextList != null )
			return ( contextList.hasContext( firstContextNr ) == contextList.hasContext( secondContextNr ) );

		return true;
		}

	protected boolean isContextSubsetInAllWords( int fullSetContextNr, int subsetContextNr )
		{
		WordItem currentWordItem;

		if( fullSetContextNr > Constants.NO_CONTEXT_NR &&
		subsetContextNr > Constants.NO_CONTEXT_NR )
			{
			if( fullSetContextNr == subsetContextNr )
				return true;

			if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
				{
				// Do for all active words
				do	{
					if( currentWordItem.isContextSubsetInWord( fullSetContextNr, subsetContextNr ) )
						return true;
					}
				while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}
			}

		return false;
		}

	protected short contextWordTypeNrInWord( int contextNr )
		{
		ContextItem foundContextItem;

		if( ( foundContextItem = contextItem( contextNr ) ) != null )
			return foundContextItem.contextWordTypeNr();

		return Constants.WORD_TYPE_UNDEFINED;
		}

	protected int contextNr( WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextNr( specificationWordItem );

		return Constants.NO_CONTEXT_NR;
		}

	protected int contextNr( boolean isCompoundCollectionSpanishAmbiguous, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem );

		return Constants.NO_CONTEXT_NR;
		}

	protected int highestContextNrInWord()
		{
		if( contextList != null )
			return contextList.highestContextNr();

		return Constants.NO_CONTEXT_NR;
		}

	protected int nContextWordsInAllWords( int contextNr, WordItem specificationWordItem )
		{
		int nContextWords = 0;
		WordItem currentWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( currentWordItem.hasContextInWord( contextNr, specificationWordItem ) )
					nContextWords++;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return nContextWords;
		}

	protected byte addContext( boolean isCompoundCollectionSpanishAmbiguous, short contextWordTypeNr, short specificationWordTypeNr, int contextNr, WordItem specificationWordItem )
		{
		if( !isAdminWord() )
			{
			if( contextList == null )
				{
				// Create list
				if( ( contextList = new ContextList( this ) ) != null )
					wordListArray[Constants.WORD_CONTEXT_LIST] = contextList;
				else
					return startErrorInWord( 1, null, "I failed to create a context list" );
				}

			return contextList.addContext( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have context" );
		}

	protected ContextItem firstActiveContextItem()
		{
		if( contextList != null )
			return contextList.firstActiveContextItem();

		return null;
		}

	protected ContextItem contextItem( int contextNr )
		{
		if( contextList != null )
			return contextList.contextItem( contextNr );

		return null;
		}

	protected ContextItem contextItem( WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextItem( specificationWordItem );

		return null;
		}

	protected ContextItem contextItem( boolean isCompoundCollectionSpanishAmbiguous, int nContextWords, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextItem( isCompoundCollectionSpanishAmbiguous, nContextWords, specificationWordItem );

		return null;
		}

	protected WordItem contextWordItemInAllWords( int contextNr, WordItem specificationWordItem, WordItem previousWordItem )
		{
		WordItem currentWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		// Do in all (remaining) words
		( currentWordItem = ( previousWordItem == null ? CommonVariables.lastPredefinedWordItem : previousWordItem.nextWordItem() ) ) != null )
			{
			do	{
				if( currentWordItem.hasContextInWord( contextNr, specificationWordItem ) )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return null;
		}


	// Protected database connection methods
/*
	protected byte storeWordItemInFutureDatabase()
		{
		if( assignmentList != null )
			return assignmentList.storeChangesInFutureDatabase();

		if( collectionList != null )
			return collectionList.storeChangesInFutureDatabase();

		if( contextList != null )
			return contextList.storeChangesInFutureDatabase();

		if( generalizationList != null )
			return generalizationList.storeChangesInFutureDatabase();

		if( grammarList != null )
			return grammarList.storeChangesInFutureDatabase();

		if( interfaceList != null )
			return interfaceList.storeChangesInFutureDatabase();

		if( justificationList != null )
			return justificationList.storeChangesInFutureDatabase();

		if( multipleWordList != null )
			return multipleWordList.storeChangesInFutureDatabase();

		if( specificationList != null )
			return specificationList.storeChangesInFutureDatabase();

		if( wordTypeList != null )
			return wordTypeList.storeChangesInFutureDatabase();

		// Write list isn't stored, because it is a temporary list

		return Constants.RESULT_OK;
		}
*/

	// Protected generalization methods

	protected GeneralizationResultType findGeneralization( boolean isRelation, WordItem generalizationWordItem )
		{
		if( generalizationList != null )
			return generalizationList.findGeneralization( isRelation, generalizationWordItem );

		return new GeneralizationResultType();
		}

	protected byte createGeneralizationItem( boolean isRelation, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem )
		{
		if( !isAdminWord() )
			{
			if( generalizationList == null )
				{
				// Create list
				if( ( generalizationList = new GeneralizationList( this ) ) != null )
					wordListArray[Constants.WORD_GENERALIZATION_LIST] = generalizationList;
				else
					return startErrorInWord( 1, null, "I failed to create a generalization list" );
				}

			return generalizationList.createGeneralizationItem( isLanguageWord_, isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have generalizations" );
		}

	protected GeneralizationItem firstGeneralizationItem()
		{
		if( generalizationList != null )
			return generalizationList.firstActiveGeneralizationItem();

		return null;
		}

	protected GeneralizationItem firstNounSpecificationGeneralizationItem()
		{
		if( generalizationList != null )
			return generalizationList.firstGeneralizationItem( true, false, false );

		return null;
		}

	protected GeneralizationItem firstSpecificationGeneralizationItem( boolean isOnlySelectingCurrentLanguage )
		{
		if( generalizationList != null )
			return generalizationList.firstGeneralizationItem( false, isOnlySelectingCurrentLanguage, false );

		return null;
		}

	protected GeneralizationItem firstRelationGeneralizationItem()
		{
		if( generalizationList != null )
			return generalizationList.firstGeneralizationItem( false, false, true );

		return null;
		}


	// Protected grammar methods

	protected void markAsChoiceEnd( int choiceEndItemNr )
		{
		if( grammarList != null )
			grammarList.markAsChoiceEnd( choiceEndItemNr );
		}

	protected void markAsOptionEnd()
		{
		if( grammarList != null )
			grammarList.markAsOptionEnd();
		}

	protected boolean isNeededToCheckGrammar()
		{
		if( grammarList != null )
			return grammarList.isNeededToCheckGrammar();

		return false;
		}

	protected boolean isLanguageWithMergedWords()
		{
		if( grammarList != null )
			return grammarList.hasMergedWords();

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

	protected GrammarResultType analyzeWordEnding( short grammarParameter, int originalWordStringLength, String originalWordString )
		{
		if( grammarList != null )
			return grammarList.analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return new GrammarResultType();
		}

	protected GrammarResultType createGrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		if( !isAdminWord() )
			{
			if( grammarList == null )
				{
				if( ( grammarList = new GrammarList( this ) ) != null )
					wordListArray[Constants.WORD_GRAMMAR_LIST] = grammarList;
				else
					{
					grammarResult.result = startErrorInWord( 1, null, "I failed to create a grammar list" );
					return grammarResult;
					}
				}

			return grammarList.createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );
			}

		grammarResult.result = startErrorInWord( 1, null, "The admin word item cannot have grammar" );
		return grammarResult;
		}

	protected GrammarResultType findGrammar( boolean ignoreValue, short grammarParameter, int grammarStringLength, String grammarString )
		{
		if( grammarList != null )
			return grammarList.findGrammarItem( ignoreValue, grammarParameter, grammarStringLength, grammarString );

		return new GrammarResultType();
		}

	protected GrammarResultType checkForDuplicateGrammarDefinition()
		{
		GrammarResultType grammarResult = new GrammarResultType();

		if( grammarList != null )
			return grammarList.checkForDuplicateGrammarDefinition();

		grammarResult.result = startErrorInWord( 1, null, "The grammar list isn't created yet" );
		return grammarResult;
		}

	protected byte checkGrammar()
		{
		if( grammarList != null )
			return grammarList.checkGrammar();

		return startErrorInWord( 1, null, "The grammar list isn't created yet" );
		}

	protected byte checkGrammarForUsageInWord( GrammarItem unusedGrammarItem )
		{
		if( grammarList != null )
			{
			if( grammarList.checkGrammarItemForUsage( unusedGrammarItem ) == Constants.RESULT_OK )
				{
				if( writeList != null )
					return writeList.checkGrammarItemForUsage( unusedGrammarItem );
				}
			}
		else
			return startErrorInWord( 1, null, "The grammar list isn't created yet" );

		return Constants.RESULT_OK;
		}

	protected byte linkLaterDefinedGrammarWords()
		{
		if( grammarList != null )
			return grammarList.linkLaterDefinedGrammarWords();

		return startErrorInWord( 1, null, "The grammar list isn't created yet" );
		}

	protected byte expandMergedWordsInReadSentence( StringBuffer readUserSentenceStringBuffer )
		{
		if( grammarList != null )
			return grammarList.expandMergedWordsInReadSentence( readUserSentenceStringBuffer );

		return startErrorInWord( 1, null, "The grammar list isn't created yet" );
		}

	protected byte shrinkMergedWordsInWriteSentence()
		{
		if( grammarList != null )
			return grammarList.shrinkMergedWordsInWriteSentence();

		return startErrorInWord( 1, null, "The grammar list isn't created yet" );
		}

	protected GrammarItem firstGrammarItem()
		{
		if( grammarList != null )
			return grammarList.firstGrammarItem();

		return null;
		}


	// Protected interface methods

	protected byte checkInterface( short interfaceParameter, String interfaceString )
		{
		if( !isAdminWord() )
			{
			if( interfaceList == null )
				{
				// Create list
				if( ( interfaceList = new InterfaceList( this ) ) != null )
					wordListArray[Constants.WORD_INTERFACE_LIST] = interfaceList;
				else
					return startErrorInWord( 1, null, "I failed to create an interface list" );
				}

			return interfaceList.checkInterface( interfaceParameter, interfaceString );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have interfacing" );
		}

	protected byte createInterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString )
		{
		if( interfaceList != null )
			return interfaceList.createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );

		return startErrorInWord( 1, null, "The interface list isn't created yet" );
		}

	protected String interfaceString( short interfaceParameter )
		{
		if( interfaceList != null )
			return interfaceList.interfaceString( interfaceParameter );

		return Constants.INTERFACE_STRING_NOT_AVAILABLE;
		}


	// Protected justification methods

	protected void clearJustificationHasBeenWritten()
		{
		if( justificationList != null )
			justificationList.clearJustificationHasBeenWritten();
		}

	protected JustificationResultType addJustification( boolean hasFeminineOrMasculineProperNameEnding, boolean isForcingNewJustification, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		if( !isAdminWord() )
			{
			if( justificationList == null )
				{
				if( ( justificationList = new JustificationList( this ) ) != null )
					wordListArray[Constants.WORD_JUSTIFICATION_LIST] = justificationList;
				else
					{
					justificationResult.result = startErrorInWord( 1, null, "I failed to create a justification list" );
					return justificationResult;
					}
				}

			return justificationList.addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem );
			}

		justificationResult.result = startErrorInWord( 1, null, "The admin word item cannot have justification" );
		return justificationResult;
		}

	protected JustificationResultType copyJustificationItem( SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		if( justificationList != null )
			return justificationList.copyJustificationItem( newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );

		justificationResult.result = startErrorInWord( 1, null, "The justification list isn't created yet" );
		return justificationResult;
		}

	protected byte attachJustification( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.attachJustification( attachJustificationItem, involvedSpecificationItem );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte checkForUnreferencedJustifications()
		{
		if( justificationList != null )
			return justificationList.checkForUnreferencedJustifications();

		return Constants.RESULT_OK;
		}

	protected byte checkJustificationsForReplacedSpecifications()
		{
		if( justificationList != null )
			return justificationList.checkForReplacedOrDeletedSpecifications();

		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationForUsageOfInvolvedWords( SpecificationItem unusedSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( justificationList != null )
			justificationList.checkSpecificationItemForUsage( unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK )
			{
			if( checkSpecificationForUsageInWord( unusedSpecificationItem ) == Constants.RESULT_OK )
				{
				if( ( currentGeneralizationItem = firstGeneralizationItem() ) != null )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
							currentGeneralizationWordItem.checkSpecificationForUsageInWord( unusedSpecificationItem );
						else
							return startErrorInWord( 1, null, "I have found an undefined generalization word" );
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceJustification( JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.replaceJustification( false, obsoleteJustificationItem, replacingJustificationItem, involvedSpecificationItem, null );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( justificationList != null )
			return justificationList.replaceOrDeleteJustification( obsoleteJustificationItem );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte replaceOrDeleteObsoleteJustifications()
		{
		JustificationItem obsoleteJustificationItem;

		if( justificationList != null )
			{
			while( ( obsoleteJustificationItem = justificationList.obsoleteJustificationItem() ) != null )
				{
				if( replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
					return addErrorInWord( 1, null, "I failed to replace an obsolete justification item" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte updateSpecificationsInJustificationInWord( boolean isMainWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.updateSpecificationsInJustifications( isMainWord, obsoleteSpecificationItem, replacingSpecificationItem );

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, int specificationCollectionNr )
		{
		if( justificationList != null )
			return justificationList.writeRelatedJustificationSpecifications( justificationTypeNr, specificationCollectionNr );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected JustificationItem correctedAssumptionByOppositeQuestionJustificationItem()
		{
		if( justificationList != null )
			return justificationList.correctedAssumptionByOppositeQuestionJustificationItem();

		return null;
		}

	protected JustificationItem negativeAssumptionOrConclusionJustificationItem( SpecificationItem anotherPrimarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem );

		return null;
		}

	protected JustificationItem olderComplexJustificationItem( boolean hasSecondarySpecificationWithoutRelationContext, short justificationTypeNr, int secondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.olderComplexJustificationItem( hasSecondarySpecificationWithoutRelationContext, justificationTypeNr, secondarySpecificationCollectionNr, primarySpecificationItem );

		return null;
		}

	protected JustificationItem possessiveReversibleAssumptionJustificationItem( WordItem secondarySpecificationWordItem )
		{
		if( justificationList != null )
			return justificationList.possessiveReversibleAssumptionJustificationItem( secondarySpecificationWordItem );

		return null;
		}

	protected JustificationItem primarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.primarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, primarySpecificationItem );

		return null;
		}

	protected JustificationItem secondarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem secondarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.secondarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, secondarySpecificationItem );

		return null;
		}

	protected JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.specificationSubstitutionPartOfAssumptionJustificationItem( secondarySpecificationItem );

		return null;
		}

	protected SpecificationItem olderSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		if( justificationList != null )
			return justificationList.olderSuggestiveQuestionAssumptionSecondarySpecificationItem();

		return null;
		}


	// Protected multiple word methods

	protected boolean isMultipleWord()
		{
		return ( multipleWordList != null &&
				multipleWordList.hasItems() );
		}

	protected short matchingMultipleSingularNounWordParts( String sentenceString )
		{
		if( multipleWordList != null )
			return multipleWordList.matchingMultipleSingularNounWordParts( sentenceString );

		return 0;
		}

	protected byte addMultipleWord( short nWordParts, short wordTypeNr, WordItem multipleWordItem )
		{
		if( !isAdminWord() )
			{
			if( multipleWordList == null )
				{
				// Create list
				if( ( multipleWordList = new MultipleWordList( this ) ) != null )
					wordListArray[Constants.WORD_MULTIPLE_WORD_LIST] = multipleWordList;
				else
					return startErrorInWord( 1, null, "I failed to create a multiple word list" );
				}

			return multipleWordList.addMultipleWord( nWordParts, wordTypeNr, multipleWordItem );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have multiple words" );
		}


	// Protected query methods

	protected void countQuery()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				wordListArray[wordListNr].countQueryInList();
			}
		}

	protected void clearQuerySelections()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				wordListArray[wordListNr].clearQuerySelectionsInList();
			}
		}

	protected boolean hasActiveQuestionWithCompoundCollection()
		{
		if( specificationList != null )
			return specificationList.hasActiveQuestionWithCompoundCollection();

		return false;
		}

	protected byte itemQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query items" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte listQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to do a list query list" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte wordTypeQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query word types" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte parameterQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query parameters" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte wordQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString )
		{
		ReferenceResultType referenceResult;

		if( ( referenceResult = findMatchingWordReferenceString( wordNameString ) ).result == Constants.RESULT_OK )
			{
			if( referenceResult.hasFoundMatchingStrings )
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

			for( short wordListNr : Constants.WordLists )
				{
				if( wordListArray[wordListNr] != null )
					{
					if( wordListArray[wordListNr].wordQueryInList( ( isSelectingOnFind && referenceResult.hasFoundMatchingStrings ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems ) != Constants.RESULT_OK )
						return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query word items" );
					}
				}
			}
		else
			return addErrorInWord( 1, null, "I failed to find words" );

		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query word references" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte stringQueryInWord( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to query strings" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte showQueryResultInWord( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].showQueryResultInList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
					return addErrorWithWordListNr( wordListNr, 1, null, "I failed to show the items" );
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
		if( wordQuestion_ != null ||
		// Create supporting module
		( wordQuestion_ = new WordQuestion( this ) ) != null )
			return wordQuestion_.findAnswerToNewUserQuestion();

		return startErrorInWord( 1, null, "I failed to create my word question module" );
		}

	protected byte findAnswersToQuestions( int compoundSpecificationCollectionNr, SpecificationItem answerSpecificationItem )
		{
		if( wordQuestion_ != null ||
		// Create supporting module
		( wordQuestion_ = new WordQuestion( this ) ) != null )
			return wordQuestion_.findAnswersToQuestions( compoundSpecificationCollectionNr, answerSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word question module" );
		}

	protected byte writeAnswerToQuestion( boolean isNegativeAnswer, boolean isPositiveAnswer, boolean isUncertainAboutRelation, SpecificationItem answerSpecificationItem )
		{
		if( wordQuestion_ != null )
			return wordQuestion_.writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );

		return startErrorInWord( 1, null, "My word question module isn't created yet" );
		}

	protected SpecificationResultType findQuestionToBeAdjustedByCompoundCollection( boolean isNegative, boolean isPossessive, short questionParameter, int specificationCompoundCollectionNr, int relationContextNr, SpecificationItem replacingSpecificationItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( wordQuestion_ != null )
			return wordQuestion_.findQuestionToBeAdjustedByCompoundCollection( isNegative, isPossessive, questionParameter, specificationCompoundCollectionNr, relationContextNr, replacingSpecificationItem, specificationWordItem );

		specificationResult.result = startErrorInWord( 1, null, "My word question module isn't created yet" );
		return specificationResult;
		}


	// Protected selection methods

	protected byte checkSelectionForUsageInWord( SelectionItem unusedSelectionItem )
		{
		if( CommonVariables.adminScoreList != null )
			{
			if( CommonVariables.adminScoreList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to check the admin score list for selection item usage" );
			}

		if( CommonVariables.adminConditionList != null )
			{
			if( CommonVariables.adminConditionList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to check the admin condition list for selection item usage" );
			}

		if( CommonVariables.adminActionList != null )
			{
			if( CommonVariables.adminActionList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to check the admin action list for selection item usage" );
			}

		if( CommonVariables.adminAlternativeList != null )
			{
			if( CommonVariables.adminAlternativeList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "I failed to check the admin alternative list for selection item usage" );
			}

		return Constants.RESULT_OK;
		}


	// Protected specification methods

	protected void initializeVariablesInWord()
		{
		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

		if( justificationList != null )
			justificationList.initializeJustificationVariables();

		if( assignmentList != null )
			{
			assignmentList.initializeSpecificationVariables( false, false );
			assignmentList.initializeSpecificationVariables( true, false );
			assignmentList.initializeSpecificationVariables( false, true );
			}

		if( specificationList != null )
			specificationList.initializeSpecificationVariables( false, false );

		if( wordQuestion_ != null )
			wordQuestion_.clearFoundAnswerToQuestion();

		if( wordSpecification_ != null )
			wordSpecification_.initializeWordSpecificationVariables();
		}

	protected boolean hasAnsweredQuestion()
		{
		if( specificationList != null )
			return specificationList.hasAnsweredQuestion();

		return false;
		}

	protected boolean hasConfirmedSpecification()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasConfirmedSpecification();

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

	protected boolean hasFoundAnyUserSpecification()
		{
		if( specificationList != null )
			return specificationList.hasFoundAnyUserSpecification();

		return false;
		}

	protected boolean hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		if( specificationList != null )
			return specificationList.hasNonPossessiveAndNonQuestionPartOfSpecification();

		return false;
		}

	protected boolean hasPossessiveButNonQuestionSpecification()
		{
		if( specificationList != null )
			return specificationList.hasPossessiveButNonQuestionSpecification();

		return false;
		}

	protected boolean hasShownMoreSpecificRelatedQuestion()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasShownMoreSpecificRelatedQuestion();

		return false;
		}

	protected boolean isAuthorizedForChanges( String changeKey )
		{
		return ( changeKey_ == null ||
				changeKey_ == changeKey );
		}

	protected boolean isJustificationInUse( JustificationItem unusedJustificationItem )
		{
		boolean isIncludingReplacedItems = ( unusedJustificationItem != null &&
										unusedJustificationItem.isReplacedItem() );

		if( assignmentList != null )
			{
			if( assignmentList.isJustificationInUse( false, false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList.isJustificationInUse( true, false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList.isJustificationInUse( false, true, false, unusedJustificationItem ) )
				return true;

			if( isIncludingReplacedItems &&
			assignmentList.isJustificationInUse( false, false, true, unusedJustificationItem ) )
				return true;
			}

		if( specificationList != null )
			{
			if( specificationList.isJustificationInUse( false, false, false, unusedJustificationItem ) )
				return true;

			if( isIncludingReplacedItems &&
			specificationList.isJustificationInUse( false, false, true, unusedJustificationItem ) )
				return true;
			}

		return false;
		}

	protected byte changeJustificationOfNegativeAssumptions( SpecificationItem secondarySpecificationItem )
		{
		if( assignmentList != null &&
		assignmentList.changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem ) == Constants.RESULT_OK &&
		assignmentList.changeJustificationOfNegativeAssumptions( true, false, secondarySpecificationItem ) == Constants.RESULT_OK )
			assignmentList.changeJustificationOfNegativeAssumptions( false, true, secondarySpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			return specificationList.changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem );

		return CommonVariables.result;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( wordSpecification_ != null )
			return wordSpecification_.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem );

		return startErrorInWord( 1, null, "My word specification module isn't created yet" );
		}

	protected byte checkJustificationForUsageInWord( JustificationItem unusedJustificationItem )
		{
		if( assignmentList != null &&
		assignmentList.checkJustificationForUsage( false, false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList.checkJustificationForUsage( true, false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList.checkJustificationForUsage( false, true, false, unusedJustificationItem ) == Constants.RESULT_OK )
			assignmentList.checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null &&
		specificationList.checkJustificationForUsage( false, false, false, unusedJustificationItem ) == Constants.RESULT_OK )
			specificationList.checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList != null )
			justificationList.checkJustificationForUsage( unusedJustificationItem );

		// Clear error to be able to restart after justification error
		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationsForReplacedOrDeletedJustifications( boolean isAllowingNewerReplaceOrDeleteSentenceNr )
		{
		if( assignmentList != null )
			{
			if( !assignmentList.hasFoundReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, false ) )
				{
				if( !assignmentList.hasFoundReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, false ) )
					{
					if( !assignmentList.hasFoundReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, true ) )
						{
						if( assignmentList.hasFoundDeletedJustificationInReplacedSpecification() )
							return startErrorInWord( 1, null, "I have found a deleted justification in one of my replaced assignments" );
						}
					else
						return startErrorInWord( 1, null, "I have found a replaced or deleted justification in one of my archived assignments" );
					}
				else
					return startErrorInWord( 1, null, "I have found a replaced or deleted justification in one of my inactive assignments" );
				}
			else
				return startErrorInWord( 1, null, "I have found a replaced or deleted justification in one of my active assignments" );
			}

		if( specificationList != null )
			{
			if( !specificationList.hasFoundReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, false ) )
				{
				if( specificationList.hasFoundDeletedJustificationInReplacedSpecification() )
					return startErrorInWord( 1, null, "I have found a deleted justification in one of my replaced specifications" );
				}
			else
				return startErrorInWord( 1, null, "I have found a replaced or deleted justification in one of my specifications" );
			}

		return Constants.RESULT_OK;
		}

	protected byte clearLastWrittenSentenceStringWithUnknownPluralNoun( String unknownPluralNounString, WordItem specificationWordItem )
		{
		if( assignmentList != null &&
		assignmentList.clearLastWrittenSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem ) == Constants.RESULT_OK &&
		assignmentList.clearLastWrittenSentenceStringWithUnknownPluralNoun( true, false, unknownPluralNounString, specificationWordItem ) == Constants.RESULT_OK )
			assignmentList.clearLastWrittenSentenceStringWithUnknownPluralNoun( false, true, unknownPluralNounString, specificationWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			return specificationList.clearLastWrittenSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem );

		return CommonVariables.result;
		}

	protected byte collectGeneralizationsOrSpecifications( boolean isExclusiveSpecification, boolean isGeneralizationCollection, boolean isQuestion, int collectionNr )
		{
		if( assignmentList != null &&
		assignmentList.collectGeneralizationsOrSpecifications( false, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr ) == Constants.RESULT_OK &&
		assignmentList.collectGeneralizationsOrSpecifications( true, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr ) == Constants.RESULT_OK )
			assignmentList.collectGeneralizationsOrSpecifications( false, true, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			return specificationList.collectGeneralizationsOrSpecifications( false, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr );

		return CommonVariables.result;
		}

	protected byte confirmSpecificationButNotItsRelation( SpecificationItem confirmedSpecificationItem, SpecificationItem confirmationSpecificationItem )
		{
		if( assignmentList != null &&
		assignmentList.confirmSpecificationButNotItsRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList.confirmSpecificationButNotItsRelation( true, false, confirmedSpecificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK )
			assignmentList.confirmSpecificationButNotItsRelation( false, true, confirmedSpecificationItem, confirmationSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			specificationList.confirmSpecificationButNotItsRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem );

		return CommonVariables.result;
		}

	protected byte copyAndReplaceSpecificationItem( boolean isNewAnsweredQuestion, boolean isNewExclusiveSpecification, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		if( originalSpecificationItem != null )
			{
			if( originalSpecificationItem.isAssignment() )
				{
				if( assignmentList != null )
					return assignmentList.copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );

				return startErrorInWord( 1, null, "My assignment list isn't created yet" );
				}

			if( specificationList != null )
				return specificationList.copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );

			return startErrorInWord( 1, null, "My specification list isn't created yet" );
			}

		return startErrorInWord( 1, null, "The given original specification item is undefined" );
		}

	protected byte recalculateAssumptionsInAllTouchedWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( currentWordItem.isWordTouchedDuringCurrentSentence )
					{
					if( currentWordItem.recalculateAssumptions() != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to recalculate the assumptions of word \"" + currentWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptions()
		{
		if( assignmentList != null &&
		assignmentList.recalculateAssumptions( false, false ) == Constants.RESULT_OK &&
		assignmentList.recalculateAssumptions( true, false ) == Constants.RESULT_OK )
			assignmentList.recalculateAssumptions( false, true );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			return specificationList.recalculateAssumptions( false, false );

		return CommonVariables.result;
		}

	protected byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( obsoleteSpecificationItem != null )
			{
			if( obsoleteSpecificationItem.isAssignment() )
				{
				if( assignmentList != null )
					return assignmentList.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );

				return startErrorInWord( 1, null, "My assignment list isn't created yet" );
				}

			if( specificationList != null )
				return specificationList.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );

			return startErrorInWord( 1, null, "The specification list isn't created yet" );
			}

		return startErrorInWord( 1, null, "The given obsolete specification item is undefined" );
		}

	protected byte updateJustificationInSpecifications( boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		boolean isIncludingReplacedItems = ( obsoleteJustificationItem != null &&
										obsoleteJustificationItem.hasCurrentCreationSentenceNr() );

		if( assignmentList != null &&
		assignmentList.updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK &&
		assignmentList.updateJustificationInSpecifications( true, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK &&
		assignmentList.updateJustificationInSpecifications( false, true, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
			{
			if( isIncludingReplacedItems )
				assignmentList.updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null &&
		specificationList.updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
			{
			if( isIncludingReplacedItems )
				return specificationList.updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		return CommonVariables.result;
		}

	protected byte updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		// Update specifications in justifications of this word
		if( updateSpecificationsInJustificationInWord( true, obsoleteSpecificationItem, replacingSpecificationItem ) == Constants.RESULT_OK )
			{
			if( ( currentGeneralizationItem = firstGeneralizationItem() ) != null )
				{
				// Update specifications in justifications of involved words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
						{
						if( currentGeneralizationWordItem.updateSpecificationsInJustificationInWord( false, obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to update the justification items in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						return startErrorInWord( 1, null, "I have found an undefined generalization word" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
				}
			}
		else
			return addErrorInWord( 1, null, "I failed to update my justification items" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString, String changeKey )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isAuthorizedForChanges( changeKey ) &&

		( specificationWordItem == null ||
		// No user is logged in yet
		CommonVariables.currentUserNr == Constants.NO_USER_NR ||
		specificationWordItem.isAuthorizedForChanges( changeKey ) ) )
			{
			if( wordSpecification_ != null ||
			// Create supporting module
			( wordSpecification_ = new WordSpecification( this ) ) != null )
				return wordSpecification_.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );

			startErrorInWord( 1, null, "I failed to create my word specification module" );
			}
		else
			startErrorInWord( 1, null, "You are not authorized to add this word" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType createSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer writtenSentenceStringBuffer )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( !isAdminWord() )
			{
			if( isAssignment )
				{
				if( assignmentList == null )
					{
					// Create assignment list
					if( ( assignmentList = new SpecificationList( Constants.WORD_ASSIGNMENT_LIST_SYMBOL, this ) ) != null )
						wordListArray[Constants.WORD_ASSIGNMENT_LIST] = assignmentList;
					else
						startErrorInWord( 1, null, "I failed to create an assignment list" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					return assignmentList.createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceStringBuffer );
				}
			else
				{
				if( specificationList == null )
					{
					// Create specification list
					if( ( specificationList = new SpecificationList( Constants.WORD_SPECIFICATION_LIST_SYMBOL, this ) ) != null )
						wordListArray[Constants.WORD_SPECIFICATION_LIST] = specificationList;
					else
						startErrorInWord( 1, null, "I failed to create a specification list" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					return specificationList.createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceStringBuffer );
				}
			}
		else
			startErrorInWord( 1, null, "The admin word item cannot have specifications" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( wordSpecification_ != null )
			return wordSpecification_.findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );

		specificationResult.result = startErrorInWord( 1, null, "My word specification module isn't created yet" );
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( wordSpecification_ != null )
			return wordSpecification_.findRelatedSpecification( false, false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		specificationResult.result = startErrorInWord( 1, null, "My word specification module isn't created yet" );
		return specificationResult;
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingActiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem ) ) == null )
			foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAnsweredQuestions, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstActiveQuestionSpecificationItem()
		{
		if( specificationList != null )
			return specificationList.firstActiveSpecificationItem( false, true );

		return null;
		}

	protected SpecificationItem firstActiveAssignmentOrSpecificationItem( boolean isIncludingAssignments, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null )
			foundSpecificationItem = assignmentList.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAnsweredQuestionAssignmentItem( boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			foundSpecificationItem = assignmentList.firstAnsweredQuestionAssignmentItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstAnsweredQuestionAssignmentItem( true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList.firstSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList.firstSpecificationItem( true, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstFeminineOrMasculineSpecificationItem()
		{
		if( specificationList != null )
			return specificationList.firstFeminineOrMasculineSpecificationItem();

		return null;
		}

	protected SpecificationItem firstNonCompoundCollectionSpecificationItem( int compoundSpecificationCollectionNr )
		{
		if( specificationList != null )
			return specificationList.firstNonCompoundCollectionSpecificationItem( compoundSpecificationCollectionNr );

		return null;
		}

	protected SpecificationItem firstNonQuestionSpecificationItem()
		{
		if( specificationList != null )
			return specificationList.firstActiveSpecificationItem( false, false );

		return null;
		}

	protected SpecificationItem firstRecentlyAnsweredQuestionSpecificationItem( short questionParameter )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstRecentlyAnsweredQuestionSpecificationItem( false, questionParameter ) ) == null )
			foundSpecificationItem = assignmentList.firstRecentlyAnsweredQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == null &&
		specificationList != null )
			return foundSpecificationItem = specificationList.firstRecentlyAnsweredQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstRelationSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			foundSpecificationItem = assignmentList.firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		if( foundSpecificationItem == null &&
		!isInactiveAssignment &&
		!isArchivedAssignment &&
		specificationList != null )
			return specificationList.firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelectedSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( isAssignment ? firstAssignmentItem( isInactiveAssignment, isArchivedAssignment, isQuestion ) :
				( isQuestion ? firstActiveQuestionSpecificationItem() : firstNonQuestionSpecificationItem() ) );
		}

	protected SpecificationItem firstSelectedSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		if( isIncludingAnsweredQuestions &&
		questionParameter > Constants.NO_QUESTION_PARAMETER )
			{
			if( isAssignment )
				{
				if( assignmentList != null )
					return assignmentList.firstAssignmentItem( true, isInactiveAssignment, isArchivedAssignment, questionParameter );
				}
			else
				{
				if( specificationList != null )
					return specificationList.firstActiveSpecificationItem( true, questionParameter );
				}
			}
		else
			{
			if( isAssignment )
				{
				if( assignmentList != null )
					{
					if( isArchivedAssignment )
						return assignmentList.firstAssignmentItem( false, false, true, questionParameter );

					if( isInactiveAssignment )
						return assignmentList.firstAssignmentItem( false, true, false, questionParameter );

					return firstActiveAssignmentItem( questionParameter );
					}
				}
			else
				{
				if( questionParameter == Constants.NO_QUESTION_PARAMETER )
					return firstNonQuestionSpecificationItem();

				if( specificationList != null )
					return specificationList.firstActiveSpecificationItem( false, questionParameter );
				}
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem moreRecentSpecificationItem;
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) == null )
			foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( true, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem );

		if( specificationList != null &&
		( moreRecentSpecificationItem = specificationList.firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) != null )
			{
			if( foundSpecificationItem == null ||

			// Prefer more recent specification over assignment
			( foundSpecificationItem.hasRelationContext() == moreRecentSpecificationItem.hasRelationContext() &&
			foundSpecificationItem.originalSentenceNr() < moreRecentSpecificationItem.originalSentenceNr() ) )
				foundSpecificationItem = moreRecentSpecificationItem;
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSpecificationItem( boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, WordItem specificationWordItem )
		{
		if( specificationList != null )
			return specificationList.firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem );

		return null;
		}

	protected SpecificationItem firstUnhiddenSpecificationItem( int relationContextNr )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			foundSpecificationItem = assignmentList.firstUnhiddenSpecificationItem( true, relationContextNr );

		if( specificationList != null )
			return specificationList.firstUnhiddenSpecificationItem( false, relationContextNr );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstUserSpecificationItem( boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( ( foundSpecificationItem = assignmentList.firstUserSpecificationItem( true, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
				foundSpecificationItem = assignmentList.firstUserSpecificationItem( false, true, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem sameUserQuestionSpecificationItem( short questionParameter )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.sameUserQuestionSpecificationItem( false, questionParameter ) ) == null )
			foundSpecificationItem = assignmentList.sameUserQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.sameUserQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	protected WordItem feminineOrMasculineCommonWordItem( boolean isFeminineWord )
		{
		if( specificationList != null )
			return specificationList.feminineOrMasculineCommonWordItem( isFeminineWord );

		return null;
		}


	// Protected word type methods

	protected void clearGeneralizationWriteLevel( short currentWriteLevel )
		{
		if( wordTypeList != null )
			wordTypeList.clearGeneralizationWriteLevel( isLanguageWord_, currentWriteLevel );
		}

	protected void clearSpecificationWriteLevel( short currentWriteLevel )
		{
		if( wordTypeList != null )
			wordTypeList.clearSpecificationWriteLevel( currentWriteLevel );
		}

	protected void clearRelationWriteLevel( short currentWriteLevel )
		{
		if( wordTypeList != null )
			wordTypeList.clearRelationWriteLevel( currentWriteLevel );
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

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.hasFeminineWordEnding();

		return false;
		}

	protected boolean hasMasculineProperNameEnding()
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.hasMasculineWordEnding();

		return false;
		}

	protected boolean hasWordType( boolean isAllowingDifferentNoun, short wordTypeNr )
		{
		if( wordTypeList != null )
			return ( wordTypeList.activeWordTypeItem( isAllowingDifferentNoun, isLanguageWord_, wordTypeNr ) != null );

		return false;
		}

	protected boolean isGeneralizationWordTypeAlreadyWritten( short generalizationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, generalizationWordTypeNr ) ) != null )
			return foundWordTypeItem.isGeneralizationWordAlreadyWritten();

		return false;
		}

	protected boolean isSpecificationWordTypeAlreadyWritten( short specificationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( true, isLanguageWord_, specificationWordTypeNr ) ) != null )
			return foundWordTypeItem.isSpecificationWordAlreadyWritten();

		return false;
		}

	protected boolean isRelationWordTypeAlreadyWritten( short relationWordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( true, isLanguageWord_, relationWordTypeNr ) ) != null )
			return foundWordTypeItem.isRelationWordAlreadyWritten();

		return false;
		}

	protected boolean isCorrectAdjective( short adjectiveParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectAdjective( adjectiveParameter );

		return false;
		}

	protected boolean isCorrectDefiniteArticle( short definiteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		isCorrectFeminineOrMasculineArticleWord( definiteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectDefiniteArticle( definiteArticleParameter );

		return false;
		}

	protected boolean isCorrectIndefiniteArticle( short indefiniteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		isCorrectFeminineOrMasculineArticleWord( indefiniteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectIndefiniteArticle( true, indefiniteArticleParameter );

		return false;
		}

	protected boolean isCorrectHiddenWordType( short wordTypeNr, String compareString, String hideKey )
		{
		if( wordTypeList != null )
			return wordTypeList.isCorrectHiddenWordType( wordTypeNr, compareString, hideKey );

		return false;
		}

	protected boolean isNumeralWordType()
		{
		return hasWordType( false, Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isSingularNoun()
		{
		return hasWordType( false, Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isPluralNoun()
		{
		return hasWordType( false, Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isProperName()
		{
		return hasWordType( false, Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean isProperNamePrecededByDefiniteArticle( short definiteArticleParameter )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.isProperNamePrecededByDefiniteArticle( definiteArticleParameter );

		return false;
		}

	protected byte deleteWordType( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.deleteWordType( wordTypeNr );

		return Constants.RESULT_OK;
		}

	protected byte hideWordType( short wordTypeNr, String authorizationKey )
		{
		if( wordTypeList != null )
			return wordTypeList.hideWordTypeItem( wordTypeNr, authorizationKey );

		return Constants.RESULT_OK;
		}

	protected byte markGeneralizationWordTypeAsWritten( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.markGeneralizationWordTypeAsWritten( isLanguageWord_, wordTypeNr );

		return startErrorInWord( 1, null, "The word type list isn't created yet" );
		}

	protected byte markSpecificationWordTypeAsWritten( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.markSpecificationWordTypeAsWritten( wordTypeNr );

		return startErrorInWord( 1, null, "The word type list isn't created yet" );
		}

	protected byte markRelationWordTypeAsWritten( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.markRelationWordTypeAsWritten( wordTypeNr );

		return startErrorInWord( 1, null, "The word type list isn't created yet" );
		}

	protected WordResultType addWordType( boolean isMultipleWord, boolean isProperNamePrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeNr, int wordLength, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();

		if( wordType_ == null &&
		// Create supporting module
		( wordType_ = new WordType( this ) ) == null )
			{
			wordResult.result = startErrorInWord( 1, null, "I failed to create my word type module" );
			return wordResult;
			}

		return wordType_.addWordType( isLanguageWord_, isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
		}

	protected WordResultType findWordType( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();

		if( wordType_ != null )
			return wordType_.findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString );

		wordResult.result = startErrorInWord( 1, null, "My word type module isn't created yet" );
		return wordResult;
		}

	protected WordTypeItem activeWordTypeItem( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.activeWordTypeItem( false, isLanguageWord_, wordTypeNr );

		return null;
		}

	protected WordTypeItem activeWordTypeItem( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr );

		return null;
		}

	protected String anyWordTypeString()
		{
		if( wordType_ != null )
			return wordType_.wordTypeString( true, Constants.WORD_TYPE_UNDEFINED );

		return null;
		}

	protected String activeWordTypeString( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.activeWordTypeString( isLanguageWord_, wordTypeNr );

		return null;
		}

	protected String wordTypeString( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		if( wordType_ != null )
			return wordType_.wordTypeString( ( isCheckingAllLanguages || isLanguageWord_ ), wordTypeNr );

		return null;
		}

	protected String singularNounString()
		{
		return activeWordTypeString( Constants.WORD_TYPE_NOUN_SINGULAR );
		}


	// Protected write methods

	protected byte writeJustificationSpecification( boolean isWritingCurrentSpecificationWordOnly, SpecificationItem justificationSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, justificationSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}

	protected byte writeSelectedSpecification( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeSelectedSpecification( false, isCheckingUserSentenceForIntegrity, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}

	protected byte writeSelectedSpecification( boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeSelectedSpecification( false, false, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}

	protected byte writeSelectedSpecificationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeSelectedSpecificationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}

	protected byte writeSelectedRelationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeSelectedRelationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isConcludedAssumption, boolean isCorrectedAssumptionByKnowledge, boolean isCorrectedAssumptionByOppositeQuestion, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeUpdatedSpecification( isAdjustedSpecification, isConcludedAssumption, isCorrectedAssumptionByKnowledge, isCorrectedAssumptionByOppositeQuestion, writeSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}


	// Protected write sentence methods

	protected byte selectGrammarToWriteSentence( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		if( wordWriteSentence_ != null ||
		// Create supporting module
		( wordWriteSentence_ = new WordWriteSentence( this ) ) != null )
			return wordWriteSentence_.selectGrammarToWriteSentence( isCheckingUserSentenceForIntegrity, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write sentence module" );
		}


	// Protected write words methods

	protected void initializeWordWriteWordsVariables()
		{
		if( wordWriteWords_ != null )
			wordWriteWords_.initializeWordWriteWordsVariables();
		}

	protected void initializeWordWriteWordsSpecificationVariables( boolean isQuestion, int startWordPosition )
		{
		if( wordWriteWords_ != null )
			wordWriteWords_.initializeWordWriteWordsSpecificationVariables( isQuestion, startWordPosition );
		}

	protected WriteResultType writeWordsToSentence( boolean isWritingCurrentSpecificationWordOnly, short answerParameter, GrammarItem definitionGrammarItem, SpecificationItem writeSpecificationItem )
		{
		WriteResultType writeResult = new WriteResultType();

		if( wordWriteWords_ != null ||
		// Create supporting module
		( wordWriteWords_ = new WordWriteWords( this ) ) != null )
			return wordWriteWords_.writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem );

		writeResult.result = startErrorInWord( 1, null, "I failed to create my word write words module" );
		return writeResult;
		}
	};

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
