/*
 *	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class WordItem extends Item
	{
	// Private constructible variables

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

	private boolean isFeminineWord_;
	private boolean isMasculineWord_;
	private boolean isLanguageWord_;

	private short wordParameter_;


	// Private common methods

	private boolean isCorrectFeminineOrMasculineArticleWord( boolean isFeminineWord, boolean isMasculineWord, short articleParameter )
		{
		if( ( isFeminineWord ||
		isMasculineWord ) &&

		( isFeminineWord_ ||
		isMasculineWord_ ) )
			{
			return( !hasFeminineAndMasculineArticle( articleParameter ) ||

					( isFeminineWord_ &&
					isFeminineArticleParameter( articleParameter ) ) ||

					( isMasculineWord_ &&
					isMasculineArticleParameter( articleParameter ) ) );
			}

		// If not defined
		return true;
		}

	private static short nUsers()
		{
		short nUsers = 0;
		GeneralizationItem currentGeneralizationItem;
		WordItem predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem ) != null )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem() ) != null )
				{
				do	nUsers++;
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
				}
			}

		return nUsers;
		}

	private String grammarLanguageNameStringInWord( short languageNr )
		{
		WordItem grammarLanguageWordItem = grammarLanguageWordItemInWord( languageNr );

		if( grammarLanguageWordItem != null )
			return grammarLanguageWordItem.anyWordTypeString();

		return Constants.NO_GRAMMAR_NAME_FOUND_STRING;
		}

	private WordItem grammarLanguageWordItemInWord( short languageNr )
		{
		short nLanguages = nGrammarLanguages();
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( nLanguages > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( currentGeneralizationWordItem.isGrammarLanguageWord() &&
					languageNr == nLanguages-- )
						return currentGeneralizationItem.generalizationWordItem();
					}
				}
			while( nLanguages > 0 &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return null;
		}

	private String userNameStringInWord( short userNr )
		{
		short numberOfUsers = nUsers();
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;

		if( numberOfUsers > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem() ) != null )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( userNr == numberOfUsers-- )
						return generalizationWordItem.anyWordTypeString();
					}
				}
			while( userNr > Constants.NO_USER_NR &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
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

	private int collectionNrByCompoundGeneralizationWordInWord( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		if( collectionList != null )
			return collectionList.collectionNrByCompoundGeneralizationWord( collectionWordTypeNr, compoundGeneralizationWordItem );

		return Constants.NO_COLLECTION_NR;
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

	private boolean isContextSimilarInWord( int firstContextNr, int secondContextNr )
		{
		if( contextList != null )
			return ( contextList.hasContext( firstContextNr ) == contextList.hasContext( secondContextNr ) );

		return true;
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
		assignmentList.checkSpecificationItemForUsage( false, false, unusedSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList.checkSpecificationItemForUsage( true, false, unusedSpecificationItem ) == Constants.RESULT_OK )
			assignmentList.checkSpecificationItemForUsage( false, true, unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			specificationList.checkSpecificationItemForUsage( false, false, unusedSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList != null )
			justificationList.checkSpecificationItemForUsage( unusedSpecificationItem );

		return Constants.RESULT_OK;
		}


	// Protected constructible variables

	protected boolean isUserGeneralizationWord;
	protected boolean isUserSpecificationWord;
	protected boolean isUserRelationWord;

	protected boolean isWordCheckedForSolving;
	protected boolean isWordTouchedDuringCurrentSentence;

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

		isFeminineWord_ = false;
		isMasculineWord_ = false;
		isLanguageWord_ = false;

		wordParameter_ = Constants.NO_WORD_PARAMETER;

		// Protected constructible variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

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

		isFeminineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMasculineWord_ = ( wordParameter == Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;

		// Protected constructible variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

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
			startSystemErrorInItem( 1, null, anyWordTypeString(), "I failed to create my word cleanup module" );
		}


	// Protected methods

	protected byte addErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return addErrorInItem( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected byte addErrorInWord( char listChar, int methodLevel, String moduleNameString, String errorString )
		{
		return addErrorInItem( listChar, ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}

	protected byte startErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startErrorInItem( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}
	protected byte startSystemErrorInWord( int methodLevel, String moduleNameString, String errorString )
		{
		return startSystemErrorInItem( ( methodLevel + 1 ), moduleNameString, anyWordTypeString(), errorString );
		}


	// Protected virtual item methods

	protected void showWords( boolean isReturnQueryToPosition )
		{
		if( wordTypeList != null &&
		isSelectedByQuery )
			wordTypeList.showWords( isReturnQueryToPosition );
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				wordParameter_ > Constants.NO_WORD_PARAMETER ) );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		// This is a virtual method. Therefore the given variables are unreferenced

		return false;	// Add at the end of the list
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

		return Constants.RESULT_OK;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		baseToStringBuffer( queryWordTypeNr );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + Constants.QUERY_WORD_START_CHAR + wordTypeString( true, queryWordTypeNr ) + Constants.QUERY_WORD_END_CHAR );

		if( isFeminineWord_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFeminine" );

		if( isMasculineWord_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMasculine" );

		if( isLanguageWord_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isNeedingAuthorizationForChanges() )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNeedingAuthorizationForChanges" );

		if( wordParameter_ > Constants.NO_WORD_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordParameter:" + wordParameter_ );

		return CommonVariables.queryStringBuffer;
		}


	// Protected common methods

	protected boolean isNeedingAuthorizationForChanges()
		{
		return ( changeKey_ != null );
		}

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

	protected boolean isAdminWord()
		{
		return ( myList() == null );
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

	protected boolean isAdjectiveAssignedOrClear()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_CLEAR ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	protected boolean isAdjectiveClear()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_CLEAR );
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

	protected boolean isVerbImperativeLogin()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN );
		}

	protected boolean isVerbImperativeShow()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW );
		}

	protected boolean isPredefinedWord()
		{
		return ( wordParameter_ > Constants.NO_WORD_PARAMETER );
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
			if( ( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem() ) != null )
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

	protected char wordListChar( short wordListNr )
		{
		switch( wordListNr )
			{
			case Constants.WORD_ASSIGNMENT_LIST:
				return Constants.WORD_ASSIGNMENT_LIST_SYMBOL;

			case Constants.WORD_COLLECTION_LIST:
				return Constants.WORD_COLLECTION_LIST_SYMBOL;

			case Constants.WORD_GENERALIZATION_LIST:
				return Constants.WORD_GENERALIZATION_LIST_SYMBOL;

			case Constants.WORD_INTERFACE_LANGUAGE_LIST:
				return Constants.WORD_INTERFACE_LANGUAGE_LIST_SYMBOL;

			case Constants.WORD_JUSTIFICATION_LIST:
				return Constants.WORD_JUSTIFICATION_LIST_SYMBOL;

			case Constants.WORD_MULTIPLE_WORD_LIST:
				return Constants.WORD_MULTIPLE_WORD_LIST_SYMBOL;

			case Constants.WORD_GRAMMAR_LANGUAGE_LIST:
				return Constants.WORD_GRAMMAR_LANGUAGE_LIST_SYMBOL;

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

	protected String grammarLanguageNameString( short languageNr )
		{
		if( CommonVariables.predefinedNounGrammarLanguageWordItem != null )
			return CommonVariables.predefinedNounGrammarLanguageWordItem.grammarLanguageNameStringInWord( languageNr );

		return null;
		}

	protected String userNameString( short userNr )
		{
		if( CommonVariables.predefinedNounUserWordItem != null )
			return CommonVariables.predefinedNounUserWordItem.userNameStringInWord( userNr );

		return null;
		}

	protected String wordTypeNameString( short wordTypeNr )
		{
		WordItem currentGrammarLanguageWordItem;

		if( ( currentGrammarLanguageWordItem = CommonVariables.currentGrammarLanguageWordItem ) != null )
			return currentGrammarLanguageWordItem.grammarStringInWord( wordTypeNr );

		return null;
		}

	protected WordItem grammarLanguageWordItem( short languageNr )
		{
		if( CommonVariables.predefinedNounGrammarLanguageWordItem != null )
			return CommonVariables.predefinedNounGrammarLanguageWordItem.grammarLanguageWordItemInWord( languageNr );

		return null;
		}

	protected WordItem nextWordItem()
		{
		return (WordItem)nextItem;
		}

	protected WordItem predefinedWordItem( short wordParameter )
		{
		WordItem currentWordItem;

		if( wordParameter > Constants.NO_WORD_PARAMETER &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( !currentWordItem.isMultipleWord() &&
				currentWordItem.wordParameter() == wordParameter )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
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

	protected byte createNewAssignmentLevel()
		{
		if( wordAssignment_ != null ||
		// Create supporting module
		( wordAssignment_ = new WordAssignment( this ) ) != null )
			return wordAssignment_.createNewAssignmentLevel();

		return startErrorInWord( 1, null, "I failed to create my word assignment module" );
		}

	protected byte deleteAssignmentLevelInWord()
		{
		if( assignmentList != null )
			return assignmentList.deleteAssignmentLevelInList();

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

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, String changeKey )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isAuthorizedForChanges( changeKey ) )
			{
			if( specificationWordItem == null ||
			specificationWordItem.isAuthorizedForChanges( changeKey ) )
				{
				if( wordAssignment_ != null ||
				// Create supporting module
				( wordAssignment_ = new WordAssignment( this ) ) != null )
					return wordAssignment_.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString );

				startErrorInWord( 1, null, "I failed to create my word assignment module" );
				}
			else
				startErrorInWord( 1, null, "You are not authorized to assign the given specification" );
			}
		else
			startErrorInWord( 1, null, "You are not authorized to assign this word" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType findAssignmentByRelationWord( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		if( assignmentList != null )
			return assignmentList.findAssignmentItemByRelationWord( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		return new SpecificationResultType();
		}

	protected SpecificationItem firstActiveNumeralAssignmentItem()
		{
		if( assignmentList != null )
			return assignmentList.firstNumeralAssignmentItem( false, false, false );

		return null;
		}

	protected SpecificationItem firstActiveStringAssignmentItem()
		{
		if( assignmentList != null )
			return assignmentList.firstStringAssignmentItem( false, false, false );

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
			return assignmentList.lastAssignmentItem( false, false, false, false );

		return null;
		}

	protected SpecificationItem firstActiveAssignmentItem( short questionParameter )
		{
		if( assignmentList != null )
			return assignmentList.firstAssignmentItem( false, false, false, questionParameter );

		return null;
		}

	protected SpecificationItem firstActiveAssignmentItem( boolean isDifferentRelationContext, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		if( assignmentList != null )
			return assignmentList.firstActiveAssignmentItem( isDifferentRelationContext, isPossessive, questionParameter, relationContextNr, specificationWordItem );

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

	protected SpecificationItem firstAssignmentItem( boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList.firstAssignmentItem( false, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstAssignmentItem( true, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				return assignmentList.firstAssignmentItem( false, true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
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

	protected void deleteRollbackInfo()
		{
		if( wordCleanup_ != null )
			wordCleanup_.deleteRollbackInfo();
		}

	protected void deleteTemporaryWriteList()
		{
		if( writeList != null )
			writeList.deleteTemporaryList();
		}

	protected void getHighestInUseSentenceNrInWord( boolean isIncludingDeletedItems, boolean isIncludingLanguageAssignments, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		if( wordCleanup_ != null )
			wordCleanup_.getHighestInUseSentenceNr( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr );
		}

	protected void setCurrentItemNrInWord()
		{
		if( wordCleanup_ != null )
			wordCleanup_.setCurrentItemNr();
		}

	protected int highestSentenceNr()
		{
		int tempSentenceNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;

		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null &&
			( tempSentenceNr = wordListArray[wordListNr].highestSentenceNrInList() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;
			}

		return highestSentenceNr;
		}

	protected byte deleteSentencesInWord( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		if( wordCleanup_ != null )
			return wordCleanup_.deleteSentences( isAvailableForRollback, lowestSentenceNr );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte decrementSentenceNrsInWord( int startSentenceNr )
		{
		if( wordCleanup_ != null )
			return wordCleanup_.decrementSentenceNrs( startSentenceNr );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte decrementItemNrRangeInWord( int startSentenceNr, int decrementItemNr, int decrementOffset )
		{
		if( wordCleanup_ != null )
			return wordCleanup_.decrementItemNrRange( startSentenceNr, decrementItemNr, decrementOffset );

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte removeFirstRangeOfDeletedItems()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.removeFirstRangeOfDeletedItems();

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte rollbackDeletedRedoInfo()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.rollbackDeletedRedoInfo();

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte undoCurrentSentence()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.undoCurrentSentence();

		return startErrorInWord( 1, null, "The admin word item cannot have a word cleanup module" );
		}

	protected byte redoCurrentSentence()
		{
		if( wordCleanup_ != null )
			return wordCleanup_.redoCurrentSentence();

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

	protected boolean isCollectedWithItself()
		{
		if( collectionList != null )
			return collectionList.isCollectedWithItself();

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.isCompoundCollection( collectionNr );

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr, WordItem commonWordItem )
		{
		if( collectionList != null )
			return collectionList.isCompoundCollection( collectionNr, commonWordItem );

		return false;
		}

	protected boolean isExclusiveCollection( int collectionNr )
		{
		if( collectionList != null )
			return collectionList.isExclusiveCollection( collectionNr );

		return false;
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
			// Do for all words
			do	{
				if( ( tempCollectionOrderNr = currentWordItem.highestCollectionOrderNrInWord( collectionNr ) ) > highestCollectionOrderNr )
					highestCollectionOrderNr = tempCollectionOrderNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return highestCollectionOrderNr;
		}

	protected int collectionNr( short specificationWordTypeNr )
		{
		int foundCompoundCollectionNr;

		// Compound collection
		if( ( foundCompoundCollectionNr = compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
			return foundCompoundCollectionNr;

		// Non-compound collection
		return nonCompoundCollectionNr( specificationWordTypeNr );
		}

	protected int collectionNr( short collectionWordTypeNr, WordItem commonWordItem )
		{
		if( collectionList != null )
			return collectionList.collectionNr( collectionWordTypeNr, commonWordItem );

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrInAllWords( int contextNr )
		{
		ContextItem foundContextItem;
		WordItem currentWordItem;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( foundContextItem = currentWordItem.contextItemInWord( contextNr ) ) != null )
					return currentWordItem.collectionNr( foundContextItem.contextWordTypeNr(), foundContextItem.specificationWordItem() );
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrByCompoundGeneralizationWordInAllWords( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		WordItem currentWordItem;

		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		compoundGeneralizationWordItem != null &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( collectionNr = currentWordItem.collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int compoundCollectionNr( short collectionWordTypeNr )
		{
		if( collectionList != null )
			return collectionList.compoundCollectionNr( collectionWordTypeNr );

		return Constants.NO_COLLECTION_NR;
		}

	protected int nCollectionsInAllWords( int collectionNr )
		{
		int nCollections = 0;
		WordItem currentWordItem;

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.hasCollectionNr( collectionNr ) )
					nCollections++;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return nCollections;
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
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
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
			// Do for all words
			do	{
				if( ( tempCollectionNr = currentWordItem.highestCollectionNrInWord() ) > highestCollectionNr )
					highestCollectionNr = tempCollectionNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return highestCollectionNr;
		}

	protected CollectionResultType addCollection( boolean isExclusiveSpecification, boolean isSpecificationGeneralization, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem, String collectionString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		if( wordCollection_ != null ||
		// Create supporting module
		( wordCollection_ = new WordCollection( this ) ) != null )
			return wordCollection_.addCollection( isExclusiveSpecification, isSpecificationGeneralization, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, collectionString );

		collectionResult.result = startErrorInWord( 1, null, "I failed to create my word collection module" );
		return collectionResult;
		}

	protected CollectionResultType addCollectionByExclusiveSpecification( boolean isExclusiveSpecification, short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		if( wordCollection_ != null ||
		// Create supporting module
		( wordCollection_ = new WordCollection( this ) ) != null )
			return wordCollection_.addCollectionByExclusiveSpecification( isExclusiveSpecification, collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );

		collectionResult.result = startErrorInWord( 1, null, "I failed to create my word collection module" );
		return collectionResult;
		}

	protected CollectionResultType addCollectionByGeneralization( boolean isExclusiveSpecification, boolean isExclusiveGeneralization, boolean isQuestion, short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		if( wordCollection_ != null ||
		// Create supporting module
		( wordCollection_ = new WordCollection( this ) ) != null )
			return wordCollection_.addCollectionByGeneralization( isExclusiveSpecification, isExclusiveGeneralization, isQuestion, collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );

		collectionResult.result = startErrorInWord( 1, null, "I failed to create my word collection module" );
		return collectionResult;
		}

	protected CollectionResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		if( collectionList != null )
			return collectionList.findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return new CollectionResultType();
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


	// Protected context methods

	protected void clearGeneralizationContextWriteLevelInWord( short currentWriteLevel, int contextNr )
		{
		if( contextList != null )
			contextList.clearGeneralizationContextWriteLevel( currentWriteLevel, contextNr );
		}

	protected void clearSpecificationContextWriteLevelInWord( short currentWriteLevel, int contextNr )
		{
		if( contextList != null )
			contextList.clearSpecificationContextWriteLevel( currentWriteLevel, contextNr );
		}

	protected void clearRelationContextWriteLevelInWord( short currentWriteLevel, int contextNr )
		{
		if( contextList != null )
			contextList.clearRelationContextWriteLevel( currentWriteLevel, contextNr );
		}

	protected boolean hasContextCurrentlyBeenUpdatedInAllWords( int contextNr, WordItem specificationWordItem )
		{
		WordItem currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.hasContextCurrentlyBeenUpdatedInWord( contextNr, specificationWordItem ) )
					return true;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return false;
		}

	protected boolean hasContextInWord( int contextNr, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.hasContext( contextNr, specificationWordItem );

		return false;
		}

	protected boolean isCurrentContextInAllWords( int contextNr, WordItem specificationWordItem )
		{
		int tempSentenceNr;
		int currentSentenceNr = CommonVariables.currentSentenceNr;
		WordItem currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( tempSentenceNr = currentWordItem.contextSentenceNr( contextNr, specificationWordItem ) ) > Constants.NO_SENTENCE_NR )
					{
					if( tempSentenceNr != currentSentenceNr )
						return false;
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );

			return true;
			}

		return false;
		}

	protected boolean isContextSimilarInAllWords( int firstContextNr, int secondContextNr )
		{
		WordItem currentWordItem;

		if( firstContextNr > Constants.NO_CONTEXT_NR &&
		secondContextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( !currentWordItem.isContextSimilarInWord( firstContextNr, secondContextNr ) )
					return false;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

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

			if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
				{
				// Do for all words
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

		if( ( foundContextItem = contextItemInWord( contextNr ) ) != null )
			return foundContextItem.contextWordTypeNr();

		return Constants.WORD_TYPE_UNDEFINED;
		}

	protected int contextNrInWord( WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextNr( specificationWordItem );

		return Constants.NO_CONTEXT_NR;
		}

	protected int contextNrInWord( int nContextWords, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextNr( nContextWords, specificationWordItem );

		return Constants.NO_CONTEXT_NR;
		}

	protected int contextSentenceNr( int contextNr, WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextSentenceNr( contextNr, specificationWordItem );

		return Constants.NO_SENTENCE_NR;
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

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.hasContextInWord( contextNr, specificationWordItem ) )
					nContextWords++;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return nContextWords;
		}

	protected byte addContext( boolean isQuestion, short contextWordTypeNr, short specificationWordTypeNr, int contextNr, WordItem specificationWordItem )
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

			return contextList.addContext( isQuestion, contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have context" );
		}

	protected byte copyContextInAllWords( short contextWordTypeNr, short specificationWordTypeNr, int oldContextNr, int newContextNr, WordItem specificationWordItem )
		{
		WordItem currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( oldContextNr > Constants.NO_CONTEXT_NR &&
		newContextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.hasContextInWord( oldContextNr, specificationWordItem ) )
					{
					if( currentWordItem.addContext( false, contextWordTypeNr, specificationWordTypeNr, newContextNr, specificationWordItem ) != Constants.RESULT_OK )
						return addErrorInWord( 1, null, "I failed to copy a context in word \"" + currentWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected ContextItem firstActiveContextItem()
		{
		if( contextList != null )
			return contextList.firstActiveContextItem();

		return null;
		}

	protected ContextItem contextItemInWord( int contextNr )
		{
		if( contextList != null )
			return contextList.contextItem( contextNr );

		return null;
		}

	protected ContextItem contextItemInWord( WordItem specificationWordItem )
		{
		if( contextList != null )
			return contextList.contextItem( specificationWordItem );

		return null;
		}

	protected WordItem contextWordItemInAllWords( int contextNr, WordItem specificationWordItem, WordItem previousWordItem )
		{
		WordItem currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR &&
		// Do in all (remaining) words
		( currentWordItem = ( previousWordItem == null ? CommonVariables.firstWordItem : previousWordItem.nextWordItem() ) ) != null )
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
	protected byte storeWordItemInFutureDataBase()
		{
		// Not fully implemented yet
		if( assignmentList != null )
			return assignmentList.storeChangesInFutureDataBase();

		if( collectionList != null )
			return collectionList.storeChangesInFutureDataBase();

		if( contextList != null )
			return contextList.storeChangesInFutureDataBase();

		if( generalizationList != null )
			return generalizationList.storeChangesInFutureDataBase();

		if( grammarList != null )
			return grammarList.storeChangesInFutureDataBase();

		if( interfaceList != null )
			return interfaceList.storeChangesInFutureDataBase();

		if( justificationList != null )
			return justificationList.storeChangesInFutureDataBase();

		if( multipleWordList != null )
			return multipleWordList.storeChangesInFutureDataBase();

		if( specificationList != null )
			return specificationList.storeChangesInFutureDataBase();

		if( wordTypeList != null )
			return wordTypeList.storeChangesInFutureDataBase();

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

	protected byte createGeneralization( boolean isRelation, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem )
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

			return generalizationList.createGeneralization( isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have generalizations" );
		}

	protected GeneralizationItem firstGeneralizationItem()
		{
		if( generalizationList != null )
			return generalizationList.firstActiveGeneralizationItem();

		return null;
		}

	protected GeneralizationItem firstSpecificationGeneralizationItem()
		{
		if( generalizationList != null )
			return generalizationList.firstGeneralizationItem( false );

		return null;
		}

	protected GeneralizationItem firstRelationGeneralizationItem()
		{
		if( generalizationList != null )
			return generalizationList.firstGeneralizationItem( true );

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

	protected boolean isCheckingGrammarNeeded()
		{
		if( grammarList != null )
			return grammarList.isCheckingGrammarNeeded();

		return false;
		}

	protected boolean isGrammarLanguageWord()
		{
		return ( grammarList != null );
		}

	protected boolean isLanguageWord()
		{
		return isLanguageWord_;
		}

	protected short guideByGrammarStringWordTypeNr( String guideByGrammarString )
		{
		if( grammarList != null )
			return grammarList.guideByGrammarStringWordTypeNr( guideByGrammarString );

		return Constants.WORD_TYPE_UNDEFINED;
		}

	protected short nGrammarLanguages()
		{
		short nLanguages = 0;
		GeneralizationItem currentGeneralizationItem;
		WordItem generalizationWordItem;
		WordItem predefinedNounGrammarLanguageWordItem;

		if( ( predefinedNounGrammarLanguageWordItem = CommonVariables.predefinedNounGrammarLanguageWordItem ) != null )
			{
			if( ( currentGeneralizationItem = predefinedNounGrammarLanguageWordItem.firstSpecificationGeneralizationItem() ) != null )
				{
				do	{
					if( ( generalizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
						{
						if( generalizationWordItem.isGrammarLanguageWord() )
							nLanguages++;
						}
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
				}
			}

		return nLanguages;
		}

	protected GrammarResultType checkOnWordEnding( short grammarParameter, int originalWordStringLength, String originalWordString )
		{
		if( grammarList != null )
			return grammarList.checkOnWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return new GrammarResultType();
		}

	protected GrammarResultType createGrammar( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		if( !isAdminWord() )
			{
			if( grammarList == null )
				{
				if( ( grammarList = new GrammarList( this ) ) != null )
					wordListArray[Constants.WORD_GRAMMAR_LANGUAGE_LIST] = grammarList;
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

	protected GrammarItem startOfGrammarItem()
		{
		if( grammarList != null )
			return grammarList.startOfGrammarItem();

		return null;
		}


	// Protected interface methods

	protected boolean isInterfaceLanguageWord()
		{
		return ( interfaceList != null );
		}

	protected byte checkInterface( short interfaceParameter, String interfaceString )
		{
		if( !isAdminWord() )
			{
			if( interfaceList == null )
				{
				// Create list
				if( ( interfaceList = new InterfaceList( this ) ) != null )
					wordListArray[Constants.WORD_INTERFACE_LANGUAGE_LIST] = interfaceList;
				else
					return startErrorInWord( 1, null, "I failed to create an interface list" );
				}

			return interfaceList.checkInterface( interfaceParameter, interfaceString );
			}

		return startErrorInWord( 1, null, "The admin word item cannot have interfacing" );
		}

	protected byte createInterface( short interfaceParameter, int interfaceStringLength, String interfaceString )
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

	protected JustificationResultType addJustification( boolean isForcingNewJustification, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		if( !isAdminWord() )
			{
			if( justificationList == null )
				{
				if( ( justificationList = new JustificationList( myWordItem() ) ) != null )
					wordListArray[Constants.WORD_JUSTIFICATION_LIST] = justificationList;
				else
					{
					justificationResult.result = startErrorInWord( 1, null, "I failed to create a justification list" );
					return justificationResult;
					}
				}

			return justificationList.addJustification( true, isForcingNewJustification, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem );
			}

		justificationResult.result = startErrorInWord( 1, null, "The admin word item cannot have justification" );
		return justificationResult;
		}

	protected JustificationResultType copyJustification( boolean isForcingNewJustification, SpecificationItem newPrimarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		if( !isAdminWord() )
			{
			if( justificationList != null )
				return justificationList.copyJustification( isForcingNewJustification, newPrimarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );

			justificationResult.result = startErrorInWord( 1, null, "The justification list isn't created yet" );
			}
		else
			justificationResult.result = startErrorInWord( 1, null, "The admin word item cannot have justification" );

		return justificationResult;
		}

	protected byte attachJustification( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.attachJustification( attachJustificationItem, involvedSpecificationItem );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte checkForConfirmedJustifications( SpecificationItem secondarySpecificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.checkForConfirmedJustifications( secondarySpecificationItem, involvedSpecificationItem );

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
							{
							if( currentGeneralizationWordItem.checkSpecificationForUsageInWord( unusedSpecificationItem ) != Constants.RESULT_OK )
								return addErrorInWord( 1, null, "I failed to check the specifications in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" for its usage" );
							}
						else
							return startErrorInWord( 1, null, "I found an undefined generalization word" );
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
					}
				}
			else
				return addErrorInWord( 1, null, "I failed to check my specifications" );
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceJustification( boolean hasConfirmedSpecification, JustificationItem oldJustificationItem, JustificationItem replacingJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.replaceJustification( hasConfirmedSpecification, false, false, oldJustificationItem, replacingJustificationItem, involvedSpecificationItem, null );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte replaceObsoleteJustificationsOfAdjustedQuestion( JustificationItem replacingJustificationItem, SpecificationItem adjustedQuestionSpecificationItem )
		{
		if( justificationList != null )
			return justificationList.replaceObsoleteJustificationsOfAdjustedQuestion( replacingJustificationItem, adjustedQuestionSpecificationItem );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( justificationList != null )
			return justificationList.replaceOrDeleteJustification( obsoleteJustificationItem );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected byte updateSpecificationsInJustificationInWord( boolean hasConfirmedSpecificationInMainWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( justificationList != null )
			{
			if( justificationList.updateSpecificationsInJustifications( hasConfirmedSpecificationInMainWord, true, obsoleteSpecificationItem, replacingSpecificationItem ) == Constants.RESULT_OK )
				return justificationList.updateSpecificationsInJustifications( hasConfirmedSpecificationInMainWord, false, obsoleteSpecificationItem, replacingSpecificationItem );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, int specificationCollectionNr )
		{
		if( justificationList != null )
			return justificationList.writeRelatedJustificationSpecifications( justificationTypeNr, specificationCollectionNr );

		return startErrorInWord( 1, null, "The justification list isn't created yet" );
		}

	protected JustificationItem generalizationAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.generalizationAssumptionJustificationItem( secondarySpecificationItem );

		return null;
		}

	protected JustificationItem negativeAssumptionOrConclusionJustificationItem( SpecificationItem anotherPrimarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem );

		return null;
		}

	protected JustificationItem obsoleteAssumptionJustificationItem()
		{
		if( justificationList != null )
			return justificationList.obsoleteAssumptionJustificationItem();

		return null;
		}

	protected JustificationItem oldCorrectedAssumptionByOppositeQuestionJustificationItem()
		{
		if( justificationList != null )
			return justificationList.oldCorrectedAssumptionByOppositeQuestionJustificationItem();

		return null;
		}

	protected JustificationItem questionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.questionJustificationItem( secondarySpecificationItem );

		return null;
		}

	protected JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		if( justificationList != null )
			return justificationList.specificationSubstitutionPartOfAssumptionJustificationItem( secondarySpecificationItem );

		return null;
		}

	protected SpecificationItem oldSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		if( justificationList != null )
			return justificationList.oldSuggestiveQuestionAssumptionSecondarySpecificationItem();

		return null;
		}


	// Protected multiple word methods

	protected boolean isMultipleWord()
		{
		return ( ( multipleWordList != null &&
				multipleWordList.hasItems() ) ||

				( wordTypeList != null &&
				wordTypeList.hasMultipleWordType() ) );
		}

	protected boolean isMultipleNounWordStartingWith( String sentenceString )
		{
		if( multipleWordList != null )
			return multipleWordList.isMultipleNounWordStartingWith( sentenceString );

		return false;
		}

	protected byte addMultipleWord( short wordTypeNr, WordItem multipleWordItem )
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

			return multipleWordList.addMultipleWord( wordTypeNr, multipleWordItem );
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

	protected byte itemQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].itemQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to query items" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte listQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, StringBuffer queryListStringBuffer )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].listQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to do a list query list" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte wordTypeQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, short queryWordTypeNr )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].wordTypeQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to query word types" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte parameterQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, int queryParameter )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].parameterQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to query parameters" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte wordQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordNameString )
		{
		ReferenceResultType referenceResult;

		if( ( referenceResult = findMatchingWordReferenceString( wordNameString ) ).result == Constants.RESULT_OK )
			{
			if( referenceResult.hasFoundMatchingStrings )
				{
				if( isSelectOnFind &&
				!isSelectedByQuery )
					{
					isSelectedByQuery = true;
					CommonVariables.hasFoundQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				isSelectedByQuery )
					isSelectedByQuery = false;
				}

			for( short wordListNr : Constants.WordLists )
				{
				if( wordListArray[wordListNr] != null )
					{
					if( wordListArray[wordListNr].wordQueryInList( ( isSelectOnFind && referenceResult.hasFoundMatchingStrings ), isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems ) != Constants.RESULT_OK )
						return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to query word items" );
					}
				}
			}
		else
			return addErrorInWord( 1, null, "I failed to find words" );

		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordReferenceNameString )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].wordReferenceQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to query word references" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte stringQueryInWord( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String queryString )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].stringQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to query strings" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte showQueryResultInWord( boolean showOnlyWords, boolean showOnlyWordReferences, boolean showOnlyStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( wordListArray[wordListNr] != null )
				{
				if( wordListArray[wordListNr].showQueryResultInList( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), 1, null, "I failed to show the items" );
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
			return wordQuestion_.writeAnswerToQuestion( true, isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );

		return startErrorInWord( 1, null, "My word question module isn't created yet" );
		}

	protected SpecificationResultType findQuestionToBeAdjustedByCompoundCollection( boolean isNegative, boolean isPossessive, short questionParameter, int specificationCompoundCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( wordQuestion_ != null )
			return wordQuestion_.findQuestionToBeAdjustedByCompoundCollection( isNegative, isPossessive, questionParameter, specificationCompoundCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		specificationResult.result = startErrorInWord( 1, null, "My word question module isn't created yet" );
		return specificationResult;
		}


	// Protected selection methods

	protected byte checkSelectionForUsageInWord( SelectionItem unusedSelectionItem )
		{
		if( CommonVariables.adminScoreList != null )
			{
			if( CommonVariables.adminScoreList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "The admin score list isn't created yet" );
			}

		if( CommonVariables.adminConditionList != null )
			{
			if( CommonVariables.adminConditionList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "The admin condition list isn't created yet" );
			}

		if( CommonVariables.adminActionList != null )
			{
			if( CommonVariables.adminActionList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "The admin action list isn't created yet" );
			}

		if( CommonVariables.adminAlternativeList != null )
			{
			if( CommonVariables.adminAlternativeList.checkSelectionItemForUsage( unusedSelectionItem ) != Constants.RESULT_OK )
				return addErrorInWord( 1, null, "The admin alternative list isn't created yet" );
			}

		return Constants.RESULT_OK;
		}


	// Protected specification methods

	protected void initializeVariablesInWord()
		{
		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordTouchedDuringCurrentSentence = false;

		if( justificationList != null )
			justificationList.initializeVariables();

		if( assignmentList != null )
			{
			assignmentList.initializeVariables( false, false );
			assignmentList.initializeVariables( true, false );
			assignmentList.initializeVariables( false, true );
			}

		if( specificationList != null )
			specificationList.initializeVariables( false, false );

		if( wordQuestion_ != null )
			wordQuestion_.clearFoundAnswerToQuestion();

		if( wordSpecification_ != null )
			wordSpecification_.initializeWordSpecificationVariables();
		}

	protected boolean hasAnsweredQuestion( int secondarySpecificationCollectionNr )
		{
		if( specificationList != null )
			return specificationList.hasAnsweredQuestion( secondarySpecificationCollectionNr );

		return false;
		}

	protected boolean hasConfirmedSpecification()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasConfirmedSpecification();

		return false;
		}

	protected boolean hasConfirmedSpecificationButNoRelation()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.hasConfirmedSpecificationButNoRelation();

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

	protected boolean isAuthorizedForChanges( String changeKey )
		{
		return ( changeKey_ == null ||
				changeKey_ == changeKey );
		}

	protected boolean isJustificationInUse( JustificationItem unusedJustificationItem )
		{
		if( assignmentList != null )
			{
			if( assignmentList.isJustificationInUse( false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList.isJustificationInUse( true, false, unusedJustificationItem ) )
				return true;

			if( assignmentList.isJustificationInUse( false, true, unusedJustificationItem ) )
				return true;
			}

		if( specificationList != null &&
		specificationList.isJustificationInUse( false, false, unusedJustificationItem ) )
			return true;

		return false;
		}

	protected int userSpecificationCollectionNr()
		{
		if( wordSpecification_ != null )
			return wordSpecification_.userSpecificationCollectionNr();

		return Constants.NO_COLLECTION_NR;
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

		return Constants.RESULT_OK;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		if( wordSpecification_ != null )
			return wordSpecification_.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString );

		return startErrorInWord( 1, null, "My word specification module isn't created yet" );
		}

	protected byte checkJustificationForUsageInWord( JustificationItem unusedJustificationItem )
		{
		if( assignmentList != null &&
		assignmentList.checkJustificationForUsage( false, false, unusedJustificationItem ) == Constants.RESULT_OK &&
		assignmentList.checkJustificationForUsage( true, false, unusedJustificationItem ) == Constants.RESULT_OK )
			assignmentList.checkJustificationForUsage( false, true, unusedJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			specificationList.checkJustificationForUsage( false, false, unusedJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		justificationList != null )
			justificationList.checkJustificationForUsage( unusedJustificationItem );

		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationsForReplacedJustifications()
		{
		if( assignmentList != null )
			{
			if( !assignmentList.hasFoundReplacedOrDeletedJustification( false, false ) )
				{
				if( !assignmentList.hasFoundReplacedOrDeletedJustification( false, false ) )
					{
					if( !assignmentList.hasFoundReplacedOrDeletedJustification( false, true ) )
						{
						if( assignmentList.hasFoundDeletedJustificationInReplacedSpecification() )
							return startErrorInWord( 1, null, "I found a deleted justification in one of my replaced assignments" );
						}
					else
						return startErrorInWord( 1, null, "I found a replaced or deleted justification in one of my archived assignments" );
					}
				else
					return startErrorInWord( 1, null, "I found a replaced or deleted justification in one of my inactive assignments" );
				}
			else
				return startErrorInWord( 1, null, "I found a replaced or deleted justification in one of my active assignments" );
			}

		if( specificationList != null )
			{
			if( !specificationList.hasFoundReplacedOrDeletedJustification( false, false ) )
				{
				if( specificationList.hasFoundDeletedJustificationInReplacedSpecification() )
					return startErrorInWord( 1, null, "I found a deleted justification in one of my replaced specifications" );
				}
			else
				return startErrorInWord( 1, null, "I found a replaced or deleted justification in one of my specifications" );
			}

		return Constants.RESULT_OK;
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

		return Constants.RESULT_OK;
		}

	protected byte confirmSpecificationButNoRelation( SpecificationItem confirmedSpecificationItem, SpecificationItem confirmationSpecificationItem )
		{
		if( assignmentList != null &&
		assignmentList.confirmSpecificationButNoRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK &&
		assignmentList.confirmSpecificationButNoRelation( true, false, confirmedSpecificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK )
			assignmentList.confirmSpecificationButNoRelation( false, true, confirmedSpecificationItem, confirmationSpecificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			specificationList.confirmSpecificationButNoRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem );

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptions( boolean isOnlyRecalculateSpecificationsWithoutSpecificationCollection )
		{
		if( assignmentList != null &&
		assignmentList.recalculateAssumptions( false, false, isOnlyRecalculateSpecificationsWithoutSpecificationCollection ) == Constants.RESULT_OK &&
		assignmentList.recalculateAssumptions( true, false, isOnlyRecalculateSpecificationsWithoutSpecificationCollection ) == Constants.RESULT_OK )
			assignmentList.recalculateAssumptions( false, true, isOnlyRecalculateSpecificationsWithoutSpecificationCollection );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			return specificationList.recalculateAssumptions( false, false, isOnlyRecalculateSpecificationsWithoutSpecificationCollection );

		return Constants.RESULT_OK;
		}

	protected byte replaceOlderSpecificationByConcludedAssumption( int oldRelationContextNr, SpecificationItem replacingSpecificationItem )
		{
		if( specificationList != null )
			return specificationList.replaceOlderSpecificationByConcludedAssumption( oldRelationContextNr, replacingSpecificationItem );

		return startErrorInWord( 1, null, "The specification list isn't created yet" );
		}

	protected byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( obsoleteSpecificationItem != null )
			{
			if( obsoleteSpecificationItem.isAssignment() )
				{
				if( assignmentList != null )
					return assignmentList.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );

				return startErrorInWord( 1, null, "The assignment list isn't created yet" );
				}

			if( specificationList != null )
				return specificationList.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );

			return startErrorInWord( 1, null, "The specification list isn't created yet" );
			}

		return startErrorInWord( 1, null, "The given obsolete specification item is undefined" );
		}

	protected byte updateJustificationInSpecifications( boolean hasConfirmedSpecification, boolean isExclusiveSpecification, boolean isExclusiveGeneralization, JustificationItem oldJustificationItem, JustificationItem replacingJustificationItem )
		{
		if( assignmentList != null &&
		assignmentList.updateJustificationInSpecifications( hasConfirmedSpecification, false, false, isExclusiveSpecification, isExclusiveGeneralization, oldJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK &&
		assignmentList.updateJustificationInSpecifications( hasConfirmedSpecification, true, false, isExclusiveSpecification, isExclusiveGeneralization, oldJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
			assignmentList.updateJustificationInSpecifications( hasConfirmedSpecification, false, true, isExclusiveSpecification, isExclusiveGeneralization, oldJustificationItem, replacingJustificationItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationList != null )
			return specificationList.updateJustificationInSpecifications( hasConfirmedSpecification, false, false, isExclusiveSpecification, isExclusiveGeneralization, oldJustificationItem, replacingJustificationItem );

		return Constants.RESULT_OK;
		}

	protected byte updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		boolean hasConfirmedSpecificationInMainWord = hasConfirmedSpecification();
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		// Update specifications in justifications of this word
		if( updateSpecificationsInJustificationInWord( hasConfirmedSpecificationInMainWord, obsoleteSpecificationItem, replacingSpecificationItem ) == Constants.RESULT_OK )
			{
			if( ( currentGeneralizationItem = firstGeneralizationItem() ) != null )
				{
				// Update specifications in justifications of involved words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
						{
						if( currentGeneralizationWordItem.updateSpecificationsInJustificationInWord( hasConfirmedSpecificationInMainWord, obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
							return addErrorInWord( 1, null, "I failed to update the justification items in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						return startErrorInWord( 1, null, "I found an undefined generalization word" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
				}
			}
		else
			return addErrorInWord( 1, null, "I failed to update my justification items" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isUserSpecificationExclusivelyFeminineOrMasculine, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString, String changeKey )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isAuthorizedForChanges( changeKey ) )
			{
			if( specificationWordItem == null ||
			specificationWordItem.isAuthorizedForChanges( changeKey ) )
				{
				if( wordSpecification_ != null ||
				// Create supporting module
				( wordSpecification_ = new WordSpecification( this ) ) != null )
					return wordSpecification_.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );

				startErrorInWord( 1, null, "I failed to create my word specification module" );
				}
			else
				startErrorInWord( 1, null, "You are not authorized to add the given specification" );
			}
		else
			startErrorInWord( 1, null, "You are not authorized to add this word" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType copySpecification( boolean isNewInactiveAssignment, boolean isNewArchivedAssignment, boolean isNewAnsweredQuestion, boolean isNewExclusiveSpecification, short newAssignmentLevel, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, int newRelationContextNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( !isAdminWord() )
			{
			if( originalSpecificationItem != null )
				{
				if( originalSpecificationItem.isAssignment() )
					{
					if( assignmentList != null )
						return assignmentList.copySpecification( isNewInactiveAssignment, isNewArchivedAssignment, isNewAnsweredQuestion, isNewExclusiveSpecification, newAssignmentLevel, newGeneralizationCollectionNr, newSpecificationCollectionNr, newRelationContextNr, newFirstJustificationItem, originalSpecificationItem );

					startErrorInWord( 1, null, "My assignment list isn't created yet" );
					}
				else
					{
					if( specificationList != null )
						return specificationList.copySpecification( isNewInactiveAssignment, isNewArchivedAssignment, isNewAnsweredQuestion, isNewExclusiveSpecification, newAssignmentLevel, newGeneralizationCollectionNr, newSpecificationCollectionNr, newRelationContextNr, newFirstJustificationItem, originalSpecificationItem );

					startErrorInWord( 1, null, "My specification list isn't created yet" );
					}
				}
			else
				startErrorInWord( 1, null, "The given original specification item is undefined" );
			}
		else
			startErrorInWord( 1, null, "The admin word item cannot have specifications" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType createSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short grammarLanguageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer writtenSentenceStringBuffer )
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
					return assignmentList.createSpecification( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assignmentLevel, assumptionLevel, grammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceStringBuffer );
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
					return specificationList.createSpecification( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assignmentLevel, assumptionLevel, grammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceStringBuffer );
				}
			}
		else
			startErrorInWord( 1, null, "The admin word item cannot have specifications" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingRelationContext, boolean isCheckingSelfGeneratedAssumption, SpecificationItem searchSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( wordSpecification_ != null )
			return wordSpecification_.findRelatedSpecification( isCheckingRelationContext, isCheckingSelfGeneratedAssumption, searchSpecificationItem );

		specificationResult.result = startErrorInWord( 1, null, "My word specification module isn't created yet" );
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( wordSpecification_ != null )
			return wordSpecification_.findRelatedSpecification( false, false, isIgnoringNegative, isIncludingAssignments, isIncludingInactiveAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString );

		specificationResult.result = startErrorInWord( 1, null, "My word specification module isn't created yet" );
		return specificationResult;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem ) ) == null )
			{
			if( ( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( true, false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem ) ) == null )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( false, true, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingRelationContextNrSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingRelationContextNrSpecificationItem( isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAnsweredQuestions, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

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
			foundSpecificationItem = assignmentList.firstSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem );

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstActiveAssignmentOrSpecificationItem( boolean isIncludingAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null )
			foundSpecificationItem = assignmentList.firstSpecificationItem( false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, specificationWordItem );

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, specificationWordItem );

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

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.firstSpecificationItem( true, false, isNegative, isPossessive, questionParameter, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstSpecificationItem( false, true, isNegative, isPossessive, questionParameter, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			foundSpecificationItem = assignmentList.firstSpecificationItem( false, false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstSpecificationItem( true, false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstSpecificationItem( false, true, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSpecificationItem( false, false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

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

	protected SpecificationItem firstNonQuestionAssignmentOrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingActiveAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList.firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, true, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, false, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstRecentlyAnsweredQuestionSpecificationItem( short questionParameter )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstRecentlyAnsweredQuestionSpecificationItem( false, false, questionParameter ) ) == null )
			{
			if( ( foundSpecificationItem = assignmentList.firstRecentlyAnsweredQuestionSpecificationItem( true, false, questionParameter ) ) == null )
				foundSpecificationItem = assignmentList.firstRecentlyAnsweredQuestionSpecificationItem( false, true, questionParameter );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return foundSpecificationItem = specificationList.firstRecentlyAnsweredQuestionSpecificationItem( false, false, questionParameter );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstRelationSpecificationItem( boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstRelationSpecificationItem( true, false, false, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.firstRelationSpecificationItem( true, true, false, isNegative, isPossessive, questionParameter, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstRelationSpecificationItem( true, false, true, isNegative, isPossessive, questionParameter, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstRelationSpecificationItem( true, false, false, isNegative, isPossessive, questionParameter, specificationWordItem );

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

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, isPossessive, isSelfGeneratedAssumption, questionParameter, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( true, false, isNegative, isPossessive, isSelfGeneratedAssumption, questionParameter, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( false, true, isNegative, isPossessive, isSelfGeneratedAssumption, questionParameter, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, isPossessive, isSelfGeneratedAssumption, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == null &&
		specificationList != null )
			return specificationList.firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem moreRecentSpecificationItem;
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isIncludingAnsweredQuestions, false, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isIncludingAnsweredQuestions, true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.firstSelfGeneratedCheckSpecificationItem( isIncludingAnsweredQuestions, false, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem );
			}

		if( specificationList != null &&
		( moreRecentSpecificationItem = specificationList.firstSelfGeneratedCheckSpecificationItem( isIncludingAnsweredQuestions, false, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) != null )
			{
			if( foundSpecificationItem == null ||

			// Prefer more recent specification over assignment
			( foundSpecificationItem.hasRelationContext() == moreRecentSpecificationItem.hasRelationContext() &&
			foundSpecificationItem.originalSentenceNr() < moreRecentSpecificationItem.originalSentenceNr() ) )
				foundSpecificationItem = moreRecentSpecificationItem;
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSpecificationItem( boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		if( specificationList != null )
			return specificationList.firstSpecificationItem( false, isPossessive, questionParameter, specificationWordItem );

		return null;
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

	protected SpecificationItem justificationSpecificationItem( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, JustificationItem findJustificationItem )
		{
		if( isAssignment )
			{
			if( assignmentList != null )
				return assignmentList.justificationSpecificationItem( isInactiveAssignment, isArchivedAssignment, findJustificationItem );
			}
		else
			{
			if( specificationList != null )
				return specificationList.justificationSpecificationItem( false, false, findJustificationItem );
			}

		return null;
		}

	protected SpecificationItem sameQuestionSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingAssignments, boolean isIncludingInactiveAssignments, boolean isIncludingArchivedAssignments, boolean isSelfGenerated, short questionParameter )
		{
		SpecificationItem foundSpecificationItem = null;

		if( isIncludingAssignments &&
		assignmentList != null &&
		( foundSpecificationItem = assignmentList.sameQuestionSpecificationItem( isIncludingAnsweredQuestions, false, false, isSelfGenerated, questionParameter ) ) == null )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList.sameQuestionSpecificationItem( isIncludingAnsweredQuestions, true, false, isSelfGenerated, questionParameter );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == null )
				foundSpecificationItem = assignmentList.sameQuestionSpecificationItem( isIncludingAnsweredQuestions, false, true, isSelfGenerated, questionParameter );
			}

		if( specificationList != null )
			return specificationList.sameQuestionSpecificationItem( isIncludingAnsweredQuestions, false, false, isSelfGenerated, questionParameter );

		return foundSpecificationItem;
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

	protected boolean hasFeminineProperNameEnding()
		{
		if( wordTypeList != null )
			return wordTypeList.hasFeminineWordEnding( Constants.WORD_TYPE_PROPER_NAME );

		return false;
		}

	protected boolean hasMasculineProperNameEnding()
		{
		if( wordTypeList != null )
			return wordTypeList.hasMasculineWordEnding( Constants.WORD_TYPE_PROPER_NAME );

		return false;
		}

	protected boolean hasFeminineSingularNounEnding()
		{
		if( wordTypeList != null )
			return wordTypeList.hasFeminineWordEnding( Constants.WORD_TYPE_NOUN_SINGULAR );

		return false;
		}

	protected boolean hasMasculineSingularNounEnding()
		{
		if( wordTypeList != null )
			return wordTypeList.hasMasculineWordEnding( Constants.WORD_TYPE_NOUN_SINGULAR );

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
			return oppositeArticleWordItem.hasWordType( Constants.WORD_TYPE_ARTICLE );

		return false;
		}

	protected boolean hasWordType( short wordTypeNr )
		{
		if( wordTypeList != null )
			return ( wordTypeList.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) != null );

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

	protected boolean isCorrectDefiniteArticle( boolean isFeminineWord, boolean isMasculineWord, short definiteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		isCorrectFeminineOrMasculineArticleWord( isFeminineWord, isMasculineWord, definiteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != null )
			return foundWordTypeItem.isCorrectDefiniteArticle( definiteArticleParameter );

		return false;
		}

	protected boolean isCorrectIndefiniteArticle( boolean isFeminineWord, boolean isMasculineWord, short indefiniteArticleParameter, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		isCorrectFeminineOrMasculineArticleWord( isFeminineWord, isMasculineWord, indefiniteArticleParameter ) &&
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

	protected boolean isMultipleWord( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.isMultipleWord( wordTypeNr );

		return false;
		}

	protected boolean isNumeralWordType()
		{
		return hasWordType( Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isNoun()
		{
		return ( isSingularNoun() ||
				isPluralNoun() );
		}

	protected boolean isSingularNoun()
		{
		return hasWordType( Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isPluralNoun()
		{
		return hasWordType( Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isProperName()
		{
		return hasWordType( Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean isProperNamePrecededByDefiniteArticle( short definiteArticleParameter )
		{
		WordTypeItem foundWordTypeItem;

		if( wordTypeList != null &&
		( foundWordTypeItem = wordTypeList.activeWordTypeItem( false, isLanguageWord_, Constants.WORD_TYPE_PROPER_NAME ) ) != null )
			return foundWordTypeItem.isProperNamePrecededByDefiniteArticle( definiteArticleParameter );

		return false;
		}

	protected byte hideWordType( short wordTypeNr, String authorizationKey )
		{
		if( wordTypeList != null )
			return wordTypeList.hideWordTypeItem( wordTypeNr, authorizationKey );

		return Constants.RESULT_OK;
		}

	protected byte deleteWordType( short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.deleteWordType( wordTypeNr );

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

		return wordType_.addWordType( isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
		}

	protected WordResultType findWordType( boolean isForcingToCheckAllLanguages, short wordTypeNr, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();

		if( wordType_ != null )
			return wordType_.findWordType( isForcingToCheckAllLanguages, wordTypeNr, wordTypeString );

		wordResult.result = startErrorInWord( 1, null, "My word type module isn't created yet" );
		return wordResult;
		}

	protected WordResultType findWordTypeInAllWords( boolean isForcingToCheckAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		WordResultType wordResult = new WordResultType();

		if( wordType_ == null &&
		// Create supporting module
		( wordType_ = new WordType( this ) ) == null )
			{
			wordResult.result = startErrorInWord( 1, null, "I failed to create my word type module" );
			return wordResult;
			}

		return wordType_.findWordTypeInAllWords( isForcingToCheckAllLanguages, wordTypeNr, wordTypeString, previousWordItem );
		}

	protected WordTypeItem activeWordTypeItem( boolean isForcingToCheckAllLanguages, short wordTypeNr )
		{
		if( wordTypeList != null )
			return wordTypeList.activeWordTypeItem( false, ( isForcingToCheckAllLanguages || isLanguageWord_ ), wordTypeNr );

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

	protected String wordTypeString( boolean isForcingToCheckAllLanguages, short wordTypeNr )
		{
		if( wordType_ != null )
			return wordType_.wordTypeString( ( isForcingToCheckAllLanguages || isLanguageWord_ ), wordTypeNr );

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

	protected byte writeSelectedSpecification( boolean isWritingCurrentSpecificationWordOnly, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeSelectedSpecification( false, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}

	protected byte writeSelectedSpecification( boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeSelectedSpecification( false, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );

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

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isCorrectedAssumptionByKnowledge, boolean isCorrectedAssumptionByOppositeQuestion, SpecificationItem writeSpecificationItem )
		{
		if( wordWrite_ != null ||
		// Create supporting module
		( wordWrite_ = new WordWrite( this ) ) != null )
			return wordWrite_.writeUpdatedSpecification( isAdjustedSpecification, isCorrectedAssumptionByKnowledge, isCorrectedAssumptionByOppositeQuestion, writeSpecificationItem );

		return startErrorInWord( 1, null, "I failed to create my word write module" );
		}


	// Protected write sentence methods

	protected byte selectGrammarToWriteSentence( boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		if( wordWriteSentence_ != null ||
		// Create supporting module
		( wordWriteSentence_ = new WordWriteSentence( this ) ) != null )
			return wordWriteSentence_.selectGrammarToWriteSentence( false, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );

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
 *
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *
 *************************************************************************/
