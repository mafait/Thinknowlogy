/*	Class:		Item
 *	Purpose:	Base class for the knowledge structure
 *	Version:	Thinknowlogy 2017r2 (Science as it should be)
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

class Item
	{
	// Private constructed variables
	private short userNr_;
	private int activeSentenceNr_;
	private int inactiveSentenceNr_;
	private int originalSentenceNr_;
	private int creationSentenceNr_;
	private int archivedSentenceNr_;
	private int replacedSentenceNr_;
	private int itemNr_;
	private char statusChar_;
	// Private initialized variables
	private List myList_;
	private WordItem myWordItem_;
	private String moduleNameString_;
	// Protected constructed variables
	protected boolean isSelectedByQuery;
	protected boolean isSelectedByJustificationQuery;
	protected char previousStatusChar;
	protected Item previousItem;
	protected Item nextItem;
	protected StringBuffer debugStringBuffer;

	// Private methods

	private String myWordTypeString( short queryWordTypeNr )
		{
		String wordTypeString = null;

		if( myList_ != null &&
		// Don't display my word string if the item is in an admin list
		!myList_.isAdminList() &&
		myWordItem_ != null &&
		( wordTypeString = myWordItem_.wordTypeString( true, queryWordTypeNr ) ) == null )
			wordTypeString = myWordItem_.anyWordTypeString();

		return wordTypeString;
		}

	// Constructor

	protected Item()
		{
		// Private constructed variables

		userNr_ = Constants.NO_USER_NR;

		originalSentenceNr_ = Constants.NO_SENTENCE_NR;
		creationSentenceNr_ = CommonVariables.currentSentenceNr;

		activeSentenceNr_ = Constants.NO_SENTENCE_NR;
		inactiveSentenceNr_ = Constants.NO_SENTENCE_NR;
		archivedSentenceNr_ = Constants.NO_SENTENCE_NR;
		replacedSentenceNr_ = Constants.NO_SENTENCE_NR;

		itemNr_ = Constants.NO_ITEM_NR;

		statusChar_ = Constants.QUERY_ACTIVE_CHAR;

		// Private initialized variables
		moduleNameString_ = this.getClass().getName();

		myList_ = null;
		myWordItem_ = null;

		// Protected constructed variables

		isSelectedByQuery = false;
		isSelectedByJustificationQuery = false;

		previousStatusChar = Constants.QUERY_ACTIVE_CHAR;

		nextItem = null;
		previousItem = null;
		}


	// Protected error methods

	protected byte addError( int methodLevel, String moduleNameString, String errorString )
		{
		return addError( ( methodLevel + 1 ), moduleNameString, ( myWordItem_ == null || myWordItem_.isAdminWord() ? null : myWordItem_.anyWordTypeString() ), errorString );
		}

	protected byte addError( int methodLevel, String moduleNameString, String wordItemString, String errorString )
		{
		InputOutput.displayError( Constants.SYMBOL_QUESTION_MARK, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), wordItemString, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}
	protected byte addError( char listChar, int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		InputOutput.displayError( listChar, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), wordNameString, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte startError( int methodLevel, String moduleNameString, String errorString )
		{
		addError( ( methodLevel + 1 ), moduleNameString, null, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;
		return Constants.RESULT_ERROR;
		}

	protected byte startError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;
		return Constants.RESULT_ERROR;
		}

	protected byte startSystemError( int methodLevel, String moduleNameString, String errorString )
		{
		return startSystemError( ( methodLevel + 1 ), moduleNameString, null, errorString );
		}

	protected byte startSystemError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		char textChar;
		int errorStringPosition = 0;
		StringBuffer tempStringBuffer = new StringBuffer();

		while( errorStringPosition < errorString.length() )
			{
			if( errorString.charAt( errorStringPosition ) == Constants.SYMBOL_BACK_SLASH )
				{
				errorStringPosition++;

				if( errorStringPosition < errorString.length() &&
				( textChar = InputOutput.convertDiacriticalChar( errorString.charAt( errorStringPosition ) ) ) != Constants.NEW_LINE_CHAR )
					tempStringBuffer.append( textChar );
				}
			else
				tempStringBuffer.append( errorString.charAt( errorStringPosition ) );

			errorStringPosition++;
			}

		addError( ( methodLevel + 1 ), moduleNameString, wordNameString, tempStringBuffer.toString() );

		CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
		return Constants.RESULT_SYSTEM_ERROR;
		}

	protected BoolResultType addBoolResultError( int methodLevel, String moduleNameString, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = addError( ( methodLevel + 1 ), moduleNameString, null, errorString );
		return boolResult;
		}

	protected BoolResultType addBoolResultError( int methodLevel, String moduleNameString, String wordItemString, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordItemString, errorString );
		return boolResult;
		}

	protected BoolResultType startBoolResultError( int methodLevel, String moduleNameString, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return boolResult;
		}

	protected BoolResultType startBoolResultError( int methodLevel, String moduleNameString, String wordTypeNameString, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordTypeNameString, errorString );
		return boolResult;
		}

	protected BoolResultType startBoolResultSystemError( int methodLevel, String moduleNameString, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = startSystemError( ( methodLevel + 1 ), moduleNameString, errorString );
		return boolResult;
		}

	protected CollectionResultType addCollectionResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return collectionResult;
		}

	protected CollectionResultType startCollectionResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return collectionResult;
		}

	protected CompoundResultType addCompoundResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CompoundResultType compoundResult = new CompoundResultType();

		compoundResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return compoundResult;
		}

	protected ConditionResultType addConditionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ConditionResultType conditionResult = new ConditionResultType();

		conditionResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return conditionResult;
		}

	protected ConditionResultType startConditionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ConditionResultType conditionResult = new ConditionResultType();

		conditionResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return conditionResult;
		}

	protected CompoundResultType startCompoundResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CompoundResultType compoundResult = new CompoundResultType();

		compoundResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return compoundResult;
		}

	protected ContextResultType addContextResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ContextResultType contextResult = new ContextResultType();

		contextResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return contextResult;
		}

	protected ContextResultType startContextResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ContextResultType contextResult = new ContextResultType();

		contextResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return contextResult;
		}

	protected ContextResultType startContextResultSystemError( int methodLevel, String moduleNameString, String errorString )
		{
		ContextResultType contextResult = new ContextResultType();

		contextResult.result = startSystemError( ( methodLevel + 1 ), moduleNameString, errorString );
		return contextResult;
		}

	protected CreateAndAssignResultType addCreateAndAssignResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return createAndAssignResult;
		}

	protected CreateAndAssignResultType addCreateAndAssignResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return createAndAssignResult;
		}

	protected CreateAndAssignResultType startCreateAndAssignResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return createAndAssignResult;
		}

	protected CreateAndAssignResultType startCreateAndAssignResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return createAndAssignResult;
		}

	protected CreateAndAssignResultType startCreateAndAssignResultSystemError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = startSystemError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return createAndAssignResult;
		}

	protected CreateReadWordResultType addCreateReadWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CreateReadWordResultType createReadWordResult = new CreateReadWordResultType();

		createReadWordResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return createReadWordResult;
		}

	protected CreateReadWordResultType startCreateReadWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CreateReadWordResultType createReadWordResult = new CreateReadWordResultType();

		createReadWordResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return createReadWordResult;
		}

	protected DuplicateResultType startDuplicateResultError( int methodLevel, String errorString )
		{
		DuplicateResultType duplicateResult = new DuplicateResultType();

		duplicateResult.result = startError( 1, null, errorString );
		return duplicateResult;
		}

	protected FileResultType addFileResultError( int methodLevel, String moduleNameString, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return fileResult;
		}

	protected FileResultType startFileResultError( int methodLevel, String moduleNameString, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return fileResult;
		}

	protected FindSpecificationResultType startFindSpecificationResultError( int methodLevel, String moduleNameString, String errorString )
		{
		FindSpecificationResultType findSpecificationResult = new FindSpecificationResultType();

		findSpecificationResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return findSpecificationResult;
		}

	protected GrammarResultType startGrammarResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		grammarResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return grammarResult;
		}

	protected JustificationResultType startJustificationResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		justificationResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return justificationResult;
		}

	protected QueryResultType addQueryResultError( int methodLevel, String moduleNameString, String errorString )
		{
		QueryResultType queryResult = new QueryResultType();

		queryResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return queryResult;
		}

	protected QueryResultType startQueryResultError( int methodLevel, String moduleNameString, String errorString )
		{
		QueryResultType queryResult = new QueryResultType();

		queryResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return queryResult;
		}
	protected ReadWordResultType startReadWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ReadWordResultType readWordResult = new ReadWordResultType();

		readWordResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return readWordResult;
		}

	protected RelatedResultType addRelatedResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		relatedResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return relatedResult;
		}

	protected RelatedResultType startRelatedResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		relatedResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return relatedResult;
		}

	protected SelectionResultType startSelectionResultError( int methodLevel, String errorString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.result = startError( 1, null, errorString );
		return selectionResult;
		}

	protected ShortResultType addShortResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		ShortResultType shortResult = new ShortResultType();

		shortResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return shortResult;
		}

	protected ShortResultType startShortResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ShortResultType shortResult = new ShortResultType();

		shortResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return shortResult;
		}

	protected ShortResultType startShortResultSystemError( int methodLevel, String errorString )
		{
		ShortResultType shortResult = new ShortResultType();

		shortResult.result = startSystemError( 1, null, errorString );
		return shortResult;
		}

	protected SpecificationResultType addSpecificationResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		specificationResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return specificationResult;
		}

	protected SpecificationResultType startSpecificationResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		specificationResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return specificationResult;
		}

	protected UserSpecificationResultType addUserSpecificationResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		userSpecificationResult.result = addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return userSpecificationResult;
		}

	protected UserSpecificationResultType startUserSpecificationResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		userSpecificationResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return userSpecificationResult;
		}

	protected WordResultType addWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		WordResultType wordResult = new WordResultType();

		wordResult.result = addError( ( methodLevel + 1 ), moduleNameString, errorString );
		return wordResult;
		}

	protected WordResultType startWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		WordResultType wordResult = new WordResultType();

		wordResult.result = startError( ( methodLevel + 1 ), moduleNameString, errorString );
		return wordResult;
		}

	protected WordTypeResultType startWordTypeResultError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		WordTypeResultType wordTypeResult = new WordTypeResultType();

		wordTypeResult.result = startError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );
		return wordTypeResult;
		}


	// Protected virtual methods

	protected void checkForUsage()
		{
		// This is a virtual method. Therefore, it has no body.
		}

	protected void displayString( boolean isReturnQueryToPosition )
		{
		// This is a virtual method. Therefore, it has no body, and the given variables are unreferenced.
		}

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		// This is a virtual method. Therefore, it has no body, and the given variables are unreferenced.
		}

	protected void selectingAttachedJustifications( boolean isSelectingJustificationSpecifications )
		{
		// This is a virtual method. Therefore, it has no body, and the given variables are unreferenced.
		}

	protected void selectingJustificationSpecifications()
		{
		// This is a virtual method. Therefore, it has no body.
		}

	protected boolean hasParameter( int queryParameter )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.
		return false;
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.
		return false;
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.
		return false;
		}

	protected boolean isSorted( Item nextSortItem )
		{
		return ( nextSortItem != null &&
				// Default: Descending creationSentenceNr_
				creationSentenceNr_ > nextSortItem.creationSentenceNr_ );
		}

	protected String itemString()
		{
		// This is a virtual method
		return null;
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.
		return null;
		}

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.
		return new BoolResultType();
		}


	// Protected common methods

	protected void clearArchivedSentenceNr()
		{
		archivedSentenceNr_ = Constants.NO_SENTENCE_NR;
		}

	protected void clearReplacedSentenceNr()
		{
		replacedSentenceNr_ = Constants.NO_SENTENCE_NR;
		}

	protected void setActiveStatus()
		{
		statusChar_ = Constants.QUERY_ACTIVE_CHAR;
		}

	protected void setArchivedStatus()
		{
		statusChar_ = Constants.QUERY_ARCHIVED_CHAR;
		}

	protected void setInactiveStatus()
		{
		statusChar_ = Constants.QUERY_INACTIVE_CHAR;
		}

	protected void setReplacedStatus()
		{
		statusChar_ = Constants.QUERY_REPLACED_CHAR;
		}

	protected void setDeletedStatus()
		{
		statusChar_ = Constants.QUERY_DELETED_CHAR;
		}

	protected void setActiveSentenceNr()
		{
		if( activeSentenceNr_ == Constants.NO_SENTENCE_NR )
			activeSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setArchivedSentenceNr()
		{
		if( archivedSentenceNr_ == Constants.NO_SENTENCE_NR )
			archivedSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setInactiveSentenceNr()
		{
		if( inactiveSentenceNr_ == Constants.NO_SENTENCE_NR )
			inactiveSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setReplacedSentenceNr()
		{
		if( replacedSentenceNr_ == Constants.NO_SENTENCE_NR )
			replacedSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void displayWords( boolean isReturnQueryToPosition, short queryWordTypeNr )
		{
		String myWordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( ( myWordString = myWordTypeString( queryWordTypeNr ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar_ );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( myWordString );
			}
		}

	// Strictly for initialization of AdminItem
	protected void initializeItemVariables( WordItem myWordItem )
		{
		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			startSystemError( 1, null, "The given my word item is undefined" );
		}

	protected void initializeItemVariables( int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, List myList, WordItem myWordItem )
		{
		// Private initialized variables
		userNr_ = CommonVariables.currentUserNr;
		originalSentenceNr_ = ( originalSentenceNr == Constants.NO_SENTENCE_NR ? CommonVariables.currentSentenceNr : originalSentenceNr );
		activeSentenceNr_ = ( originalSentenceNr == Constants.NO_SENTENCE_NR ? CommonVariables.currentSentenceNr : activeSentenceNr );
		inactiveSentenceNr_ = inactiveSentenceNr;
		archivedSentenceNr_ = archivedSentenceNr;

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) != null )
			{
			if( ( myList_ = myList ) != null )
				{
				if( CommonVariables.currentSentenceItemNr >= Constants.NO_ITEM_NR )
					{
					if( CommonVariables.currentSentenceItemNr < Constants.MAX_ITEM_NR )
						itemNr_ = ++CommonVariables.currentSentenceItemNr;
					else
						startSystemError( 1, null, myWordItem_.anyWordTypeString(), "Current item number overflow" );
					}
				else
					startSystemError( 1, null, myWordItem_.anyWordTypeString(), "The given current item number is less than zero" );
				}
			else
				startSystemError( 1, null, myWordItem_.anyWordTypeString(), "The given my list is undefined" );
			}
		else
			startSystemError( 1, null, null, "The given my word item is undefined" );
		}

	protected boolean hasActiveSentenceNr()
		{
		return ( activeSentenceNr_ > Constants.NO_SENTENCE_NR );
		}

	protected boolean hasInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ > Constants.NO_SENTENCE_NR );
		}

	protected boolean hasCurrentCreationSentenceNr()
		{
		return ( creationSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentOrNewerCreationSentenceNr()
		{
		return ( creationSentenceNr_ >= CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentActiveSentenceNr()
		{
		return ( activeSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentArchivedSentenceNr()
		{
		return ( archivedSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentReplacedSentenceNr()
		{
		return ( replacedSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasSentenceNr( int sentenceNr )
		{
		return ( originalSentenceNr_ == sentenceNr ||
				creationSentenceNr_ == sentenceNr ||
				activeSentenceNr_ == sentenceNr ||
				inactiveSentenceNr_ == sentenceNr ||
				archivedSentenceNr_ == sentenceNr ||
				replacedSentenceNr_ == sentenceNr );
		}

	protected boolean hasUserNr()
		{
		return ( userNr_ > Constants.NO_USER_NR );
		}

	protected boolean isOlderItem()
		{
		return ( originalSentenceNr_ < CommonVariables.currentSentenceNr );
		}

	protected boolean isActiveItem()
		{
		return ( statusChar_ == Constants.QUERY_ACTIVE_CHAR );
		}

	protected boolean isInactiveItem()
		{
		return ( statusChar_ == Constants.QUERY_INACTIVE_CHAR );
		}

	protected boolean isArchivedItem()
		{
		return ( statusChar_ == Constants.QUERY_ARCHIVED_CHAR );
		}

	protected boolean isReplacedItem()
		{
		return ( statusChar_ == Constants.QUERY_REPLACED_CHAR );
		}

	protected boolean isDeletedItem()
		{
		return ( statusChar_ == Constants.QUERY_DELETED_CHAR );
		}

	protected boolean isReplacedOrDeletedItem()
		{
		return ( statusChar_ == Constants.QUERY_REPLACED_CHAR ||
				statusChar_ == Constants.QUERY_DELETED_CHAR );
		}

	protected boolean isMoreRecent( Item checkItem )
		{
		return ( checkItem != null &&

				( creationSentenceNr_ > checkItem.creationSentenceNr_ ||

				( creationSentenceNr_ == checkItem.creationSentenceNr_ &&
				itemNr_ > checkItem.itemNr_ ) ) );
		}

	protected boolean wasActiveBefore()
		{
		return ( previousStatusChar == Constants.QUERY_ACTIVE_CHAR );
		}

	protected boolean wasInactiveBefore()
		{
		return ( previousStatusChar == Constants.QUERY_INACTIVE_CHAR );
		}

	protected boolean wasArchivedBefore()
		{
		return ( previousStatusChar == Constants.QUERY_ARCHIVED_CHAR );
		}

	protected boolean isSpanishCurrentLanguage()
		{
		WordItem currentLanguageWordItem;

		return ( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null &&
				currentLanguageWordItem.isNounWordSpanishAmbiguous() );
		}

	protected short userNr()
		{
		return userNr_;
		}

	protected int activeSentenceNr()
		{
		return activeSentenceNr_;
		}

	protected int inactiveSentenceNr()
		{
		return inactiveSentenceNr_;
		}

	protected int originalSentenceNr()
		{
		return originalSentenceNr_;
		}

	protected int creationSentenceNr()
		{
		return creationSentenceNr_;
		}

	protected int archivedSentenceNr()
		{
		return archivedSentenceNr_;
		}

	protected int replacedSentenceNr()
		{
		return replacedSentenceNr_;
		}

	protected int itemNr()
		{
		return itemNr_;
		}

	protected byte decrementActiveSentenceNr()
		{
		if( activeSentenceNr_ <= Constants.NO_SENTENCE_NR )
			return startError( 1, moduleNameString_, "The active sentence number is undefined" );

		activeSentenceNr_--;

		return Constants.RESULT_OK;
		}

	protected byte decrementInactiveSentenceNr()
		{
		if( inactiveSentenceNr_ <= Constants.NO_SENTENCE_NR )
			return startError( 1, moduleNameString_, "The inactive sentence number is undefined" );

		inactiveSentenceNr_--;

		return Constants.RESULT_OK;
		}

	protected byte decrementOriginalSentenceNr()
		{
		if( originalSentenceNr_ <= Constants.NO_SENTENCE_NR )
			return startError( 1, moduleNameString_, "The original sentence number is undefined" );

		originalSentenceNr_--;

		return Constants.RESULT_OK;
		}

	protected byte decrementCreationSentenceNr()
		{
		if( creationSentenceNr_ <= Constants.NO_SENTENCE_NR )
			return startError( 1, moduleNameString_, "The creation sentence number is undefined" );

		creationSentenceNr_--;

		return Constants.RESULT_OK;
		}

	protected byte decrementArchivedSentenceNr()
		{
		if( archivedSentenceNr_ <= Constants.NO_SENTENCE_NR )
			return startError( 1, moduleNameString_, "The archived sentence number is undefined" );

		archivedSentenceNr_--;

		return Constants.RESULT_OK;
		}

	protected byte decrementReplacedSentenceNr()
		{
		if( replacedSentenceNr_ <= Constants.NO_SENTENCE_NR )
			return startError( 1, moduleNameString_, "The replaced sentence number is undefined" );

		replacedSentenceNr_--;

		return Constants.RESULT_OK;
		}

	protected byte decrementItemNr( int decrementOffset )
		{
		if( decrementOffset >= itemNr_ )
			return startError( 1, moduleNameString_, "The given decrement offset is higher than the item number itself" );

		itemNr_ -= decrementOffset;

		return Constants.RESULT_OK;
		}

	protected char statusChar()
		{
		return statusChar_;
		}

	protected StringBuffer itemBaseToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String myWordString = myWordTypeString( queryWordTypeNr );
		String userNameString = ( myWordItem_ == null ? null : myWordItem_.userNameString( userNr_ ) );
		CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		// Display status if not active
		if( !isActiveItem() )
			queryStringBuffer.append( statusChar_ );

		if( myWordString != null )
			queryStringBuffer.append( Constants.QUERY_WORD_START_CHAR + myWordString + Constants.QUERY_WORD_END_CHAR );

		queryStringBuffer.append( Constants.QUERY_LIST_START_STRING + ( myList_ == null ? Constants.QUERY_NO_LIST_CHAR : myList_.listChar() ) + Constants.QUERY_LIST_END_CHAR );

		queryStringBuffer.append( Constants.QUERY_ITEM_START_STRING + creationSentenceNr_ + Constants.QUERY_SEPARATOR_CHAR + itemNr_ + Constants.QUERY_ITEM_END_CHAR );
/*
		// Don't display. Is always true during a query
		if( isSelectedByQuery )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSelectedByQuery" );
*/
		if( isArchivedItem() ||
		isReplacedItem() )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "previousStatusChar:" + previousStatusChar );

		if( userNr_ > Constants.NO_USER_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "user:" + ( userNameString == null ? userNr_ : userNameString ) );

		if( originalSentenceNr_ > Constants.NO_SENTENCE_NR &&
		originalSentenceNr_ != creationSentenceNr_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "originalSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + originalSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( activeSentenceNr_ > Constants.NO_SENTENCE_NR &&
		activeSentenceNr_ != creationSentenceNr_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "activeSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + activeSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( inactiveSentenceNr_ > Constants.NO_SENTENCE_NR &&
		inactiveSentenceNr_ != creationSentenceNr_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "inactiveSentence:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + inactiveSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( archivedSentenceNr_ > Constants.NO_SENTENCE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "archivedSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + archivedSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( replacedSentenceNr_ > Constants.NO_SENTENCE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacedSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + replacedSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		return queryStringBuffer;
		}

	protected List myList()
		{
		return myList_;
		}

	protected Item tailOfList()
		{
		Item searchItem = nextItem;
		Item previousSearchItem = this;

		while( searchItem != null )
			{
			previousSearchItem = searchItem;
			searchItem = searchItem.nextItem;
			}

		return previousSearchItem;
		}

	protected WordItem myWordItem()
		{
		return myWordItem_;
		}


	// Protected database connection methods
/*
	byte storeChangesInFutureDatabase()
		{
		// Save changes of this item to the database

		return Constants.RESULT_OK;
		}
*/

	// Protected definition methods

	protected boolean doesStringStartWithPhoneticVowel( String textString )
		{
		short index = 0;

		if( textString != null )
			{
			while( index < Constants.NUMBER_OF_PHONETIC_VOWELS )
				{
				if( textString.charAt( 0 ) == Constants.PHONETIC_VOWEL[index] )
					return true;

				index++;
				}
			}

		return false;
		}

	protected boolean isAssumption( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION );
		}

	protected boolean isConclusion( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );
		}

	protected boolean isAdjectiveParameter( short checkParameter )
		{
		return ( checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE );
//				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL ||
//				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE );
		}

	protected boolean isDefiniteArticleParameter( short checkParameter )
		{
		return ( checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	protected boolean isIndefiniteArticleParameter( short checkParameter )
		{
		return ( checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||

				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE );
		}

	protected boolean isFeminineArticleParameter( short articleParameter )
		{
		return ( articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||

				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE );
		}

	protected boolean isMasculineArticleParameter( short articleParameter )
		{
		return ( articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ||

				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	protected boolean isMatchingWordType( short firstWordTypeNr, short secondWordTypeNr )
		{
		return	( firstWordTypeNr == secondWordTypeNr ||

				( isNounWordType( firstWordTypeNr ) &&
				isNounWordType( secondWordTypeNr ) ) );
		}

	protected boolean isNounWordType( short wordTypeNr )
		{
		return ( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isGeneralizationReasoningWordType( short wordTypeNr )
		{
		return ( wordTypeNr == Constants.WORD_TYPE_PROPER_NAME ||
				wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected short assumptionGrade( boolean hasAnotherPrimarySpecification, boolean hasFeminineOrMasculineProperNameEnding, boolean hasPossessivePrimarySpecification, boolean hasPrimaryQuestionSpecification, short justificationTypeNr )
		{
		switch( justificationTypeNr )
			{
			case Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
				return 1;

			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				return 0;

			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				return (short)( hasFeminineOrMasculineProperNameEnding ? 2 : 1 );

			case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				return (short)( hasAnotherPrimarySpecification &&
						hasFeminineOrMasculineProperNameEnding ? 2 : 1 );

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				return (short)( hasFeminineOrMasculineProperNameEnding ? 1 : 0 );

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
				return (short)( hasPossessivePrimarySpecification ? 1 : 0 );

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				return (short)( hasAnotherPrimarySpecification ? 1 : 0 );

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				return (short)( hasPrimaryQuestionSpecification ? 1 : 0 );

			default:
				return 1;
			}
		}
	};

/*************************************************************************
 *	"Give thanks to him who made the heavenly lights-
 *		His faithful love endures forever.
 *	the sun to rule the day
 *		His faithful love endures forever.
 *	and the moon and stars to rule the night.
 *		His faithful love endures forever." (Psalm 136:7-9)
 *************************************************************************/
