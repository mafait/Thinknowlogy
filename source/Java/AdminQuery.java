/*	Class:			AdminQuery
 *	Supports class:	AdminItem
 *	Purpose:		To process queries
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

class AdminQuery
	{
	// Private constructible variables

	private int queryItemNr_;
	private int querySentenceNr_;
	private int queryCommandStringPosition_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private void clearQuerySelections()
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.clearQuerySelectionsInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.clearQuerySelectionsInList();
			}
		}

	private static boolean isNeedingToShowTotal()
		{
		int nFoundCategories = 0;

		if( CommonVariables.nActiveQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nInactiveQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nArchivedQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nReplacedQueryItems > 0 )
			nFoundCategories++;

		if( CommonVariables.nDeletedQueryItems > 0 )
			nFoundCategories++;

		return ( nFoundCategories > 1 );
		}

	private static boolean isAdminListChar( char queryListChar )
		{
		return ( queryListChar == Constants.ADMIN_FILE_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_READ_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_SCORE_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_WORD_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_CONDITION_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_ACTION_LIST_SYMBOL ||
				queryListChar == Constants.ADMIN_ALTERNATIVE_LIST_SYMBOL );
		}

	private static boolean isWordListChar( char queryListChar )
		{
		return ( queryListChar == Constants.WORD_ASSIGNMENT_LIST_SYMBOL ||
				queryListChar == Constants.WORD_COLLECTION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_GENERALIZATION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_INTERFACE_LIST_SYMBOL ||
				queryListChar == Constants.WORD_JUSTIFICATION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_MULTIPLE_WORD_LIST_SYMBOL ||
				queryListChar == Constants.WORD_GRAMMAR_LIST_SYMBOL ||
				queryListChar == Constants.WORD_WRITE_LIST_SYMBOL ||
				queryListChar == Constants.WORD_SPECIFICATION_LIST_SYMBOL ||
				queryListChar == Constants.WORD_TYPE_LIST_SYMBOL ||
				queryListChar == Constants.WORD_CONTEXT_LIST_SYMBOL );
		}

	private static int nTotalCount()
		{
		return ( CommonVariables.nActiveQueryItems +
				CommonVariables.nInactiveQueryItems +
				CommonVariables.nArchivedQueryItems +
				CommonVariables.nReplacedQueryItems +
				CommonVariables.nDeletedQueryItems );
		}

	private int countQuery()
		{
		List currentAdminList;
		WordList wordList;

		CommonVariables.nActiveQueryItems = 0;
		CommonVariables.nInactiveQueryItems = 0;
		CommonVariables.nArchivedQueryItems = 0;
		CommonVariables.nReplacedQueryItems = 0;
		CommonVariables.nDeletedQueryItems = 0;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.countQueryInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.countQueryInList();
			}

		return nTotalCount();
		}

	private byte itemQuery( boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSuppressingMessage, StringBuffer textStringBuffer )
		{
		WordItem currentLanguageWordItem;

		if( textStringBuffer != null )
			{
			if( itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR ) == Constants.RESULT_OK )
				{
				if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
					{
					if( !isSuppressingMessage &&
					countQuery() == 0 )
						textStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );
				}
			else
				return adminItem_.addError( 1, moduleNameString_, "I failed to query items" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given text string buffer is undefined" );

		return Constants.RESULT_OK;
		}

	private byte showQueryResult( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, ( queryWordTypeNr == Constants.WORD_TYPE_UNDEFINED ? CommonVariables.matchingWordTypeNr : queryWordTypeNr ), queryWidth ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to show the query result in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.showQueryResultInList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to show the query result" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte getIdFromQuery( boolean hasEndChar, String queryCommandString, char startChar, char endChar )
		{
		int queryCommandStringLength;

		querySentenceNr_ = Constants.NO_SENTENCE_NR;
		queryItemNr_ = Constants.NO_ITEM_NR;

		if( queryCommandString != null )
			{
			if( ( queryCommandStringLength = queryCommandString.length() ) > 0 )
				{
				if( queryCommandStringPosition_ > 0 &&
				queryCommandStringPosition_ < queryCommandStringLength )
					{
					if( queryCommandString.charAt( queryCommandStringPosition_ ) == startChar )
						{
						queryCommandStringPosition_++;

						if( queryCommandStringPosition_ < queryCommandStringLength &&
						queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
							{
							if( queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.SYMBOL_ASTERISK )
								{
								queryCommandStringPosition_++;
								querySentenceNr_ = Constants.MAX_SENTENCE_NR;
								}
							else
								{
								if( queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_SEPARATOR_CHAR )
									{
									if( Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) )
										{
										while( queryCommandStringPosition_ < queryCommandStringLength &&
										Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) &&
										querySentenceNr_ <= Constants.MAX_SENTENCE_NR / 10 )
											{
											querySentenceNr_ = ( querySentenceNr_ * 10 ) + ( queryCommandString.charAt( queryCommandStringPosition_ ) - '0' );
											queryCommandStringPosition_++;
											}
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "I couldn't find a number in the query string" );
									}

								if( hasEndChar &&
								queryCommandStringPosition_ < queryCommandStringLength &&
								queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_SEPARATOR_CHAR )
									{
									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ < queryCommandStringLength &&
									queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
										{
										if( Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) )
											{
											while( queryCommandStringPosition_ < queryCommandStringLength &&
											Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) &&
											queryItemNr_ <= Constants.MAX_SENTENCE_NR / 10 )
												{
												queryItemNr_ = ( queryItemNr_ * 10 ) + ( queryCommandString.charAt( queryCommandStringPosition_ ) - '0' );
												queryCommandStringPosition_++;
												}
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "I couldn't find the item number in the query string" );
										}
									}
								}
							}

						if( hasEndChar )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString.charAt( queryCommandStringPosition_ ) == endChar )
								queryCommandStringPosition_++;
							else
								return adminItem_.startError( 1, moduleNameString_, "The given query string is corrupt" );
							}
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The given command doesn't start with character '" + startChar + "', but with '" + queryCommandString.charAt( queryCommandStringPosition_ ) + "'" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The query command string position is not within the range of the given query command string" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given query command string is empty" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is undefined" );

		return Constants.RESULT_OK;
		}

	private byte getNameFromQuery( String queryCommandString, StringBuffer nameStringBuffer, char startChar, char endChar )
		{
		int queryCommandStringLength;
		int startSourceStringPosition;

		if( queryCommandString != null )
			{
			if( nameStringBuffer != null )
				{
				if( ( queryCommandStringLength = queryCommandString.length() ) > 0 )
					{
					if( queryCommandStringPosition_ > 0 &&
					queryCommandStringPosition_ < queryCommandStringLength )
						{
						if( queryCommandString.charAt( queryCommandStringPosition_ ) == startChar )
							{
							if( queryCommandStringPosition_ + 1 < queryCommandStringLength )
								{
								queryCommandStringPosition_++;

								if( queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
									{
									startSourceStringPosition = queryCommandStringPosition_;

									while( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
									queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
										queryCommandStringPosition_++;

									if( queryCommandString.charAt( queryCommandStringPosition_ ) == endChar )
										nameStringBuffer.append( queryCommandString.substring( startSourceStringPosition, queryCommandStringPosition_ ) );
									else
										return adminItem_.startError( 1, moduleNameString_, "The name in the given query string is corrupt" );
									}

								queryCommandStringPosition_++;
								}
							else
								return adminItem_.startError( 1, moduleNameString_, "The name in the given query string is corrupt" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The given name doesn't start with character '" + startChar + "', but with '" + queryCommandString.charAt( 0 ) + "'" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The query command string position is not within the range of the given query command string" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given query command string is empty" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given name string buffer is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is undefined" );

		return Constants.RESULT_OK;
		}

	private byte itemQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query item numbers in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to query item numbers in" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte listQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to do a list query module" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte wordTypeQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query word types in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to word types" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte parameterQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query parameters in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to parameters" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte wordQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query the words in my word list" );
			}

		return Constants.RESULT_OK;
		}

	private byte wordReferenceQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query word references in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, false, wordReferenceNameString ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to query word references" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte stringQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to query strings in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				{
				if( currentAdminList.stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to query strings" );
				}
			}

		return Constants.RESULT_OK;
		}

	// Constructor / deconstructor

	protected AdminQuery( AdminItem adminItem )
		{
		String errorString = null;

		queryItemNr_ = Constants.NO_ITEM_NR;
		querySentenceNr_ = Constants.NO_SENTENCE_NR;
		queryCommandStringPosition_ = 1;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void initializeQueryStringPosition()
		{
		queryCommandStringPosition_ = 1;
		}

	protected byte writeInfoTextWithPossibleQueryCommands( String textString )
		{
		boolean hasFoundNewLine = false;
		int textStringLength;
		int previousPosition;
		int position = 0;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer writeStringBuffer = new StringBuffer();

		if( textString != null )
			{
			textStringLength = textString.length();

			if( textString.charAt( 0 ) == Constants.SYMBOL_DOUBLE_QUOTE )
				position++;

			while( position < textStringLength &&
			textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE )
				{
				if( textString.charAt( position ) == Constants.QUERY_CHAR )
					{
					if( ++position < textStringLength )
						{
						previousPosition = position;
						queryCommandStringPosition_ = position;

						if( executeQuery( true, false, true, Constants.PRESENTATION_PROMPT_INFO, textString ) == Constants.RESULT_OK )
							position = queryCommandStringPosition_;
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to execute query \"" + textString.substring( previousPosition ) + "\"" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The text string ended with a query character" );
					}
				else
					{
					if( textString.charAt( position ) == Constants.SYMBOL_BACK_SLASH )
						{
						if( ++position < textStringLength )
							{
							if( ( textChar = Presentation.convertDiacriticalChar( textString.charAt( position ) ) ) == Constants.NEW_LINE_CHAR )
								hasFoundNewLine = true;
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The text string ended with a diacritical sign" );
						}
					else
						textChar = textString.charAt( position );

					position++;
					writeStringBuffer.append( textChar );
					}

				if( hasFoundNewLine ||

				( position < textStringLength &&
				textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE &&
				textString.charAt( position ) == Constants.QUERY_CHAR &&
				writeStringBuffer.length() > 0 ) )
					{
					if( Presentation.writeText( false, Constants.PRESENTATION_PROMPT_INFO, Constants.NO_CENTER_WIDTH, writeStringBuffer ) == Constants.RESULT_OK )
						{
						hasFoundNewLine = false;
						writeStringBuffer = new StringBuffer();
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to write a character" );
					}
				}

			if( writeStringBuffer.length() > 0 )
				{
				if( Presentation.writeText( false, Constants.PRESENTATION_PROMPT_INFO, Constants.NO_CENTER_WIDTH, writeStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the last characters" );
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given text string is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte executeQuery( boolean isSuppressingMessage, boolean isReturningToPosition, boolean isWritingQueryResult, short promptTypeNr, String queryCommandString )
		{
		boolean isEndOfQuery = false;
		boolean isFirstInstruction = true;
		boolean isInvalidChar = false;
		boolean isOnlyShowingStrings = false;
		boolean isOnlyShowingWords = false;
		boolean isOnlyShowingWordReferences = false;
		boolean isReturnQueryToPosition = isReturningToPosition;
		boolean isSelectingActiveItems = true;
		boolean isSelectingInactiveItems = true;
		boolean isSelectingArchivedItems = true;
		boolean isSelectingReplacedItems = true;
		boolean isSelectingDeletedItems = true;
		boolean isShowingCount = false;
		boolean isSelectingAttachedJustifications = false;
		boolean isSelectingJustificationSpecifications = false;
		short queryWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		int nTotal;
		int listStringPosition;
		int nameStringBufferLength;
		int queryCommandStringLength;
		int queryWidth = Constants.NO_CENTER_WIDTH;
		WordItem currentLanguageWordItem;
		StringBuffer queryStringBuffer;
		StringBuffer nameStringBuffer = new StringBuffer();

		CommonVariables.queryStringBuffer = new StringBuffer();

		if( queryCommandString != null )
			{
			if( ( queryCommandStringLength = queryCommandString.length() ) > 0 )
				{
				if( queryCommandStringPosition_ > 0 &&
				queryCommandStringPosition_ < queryCommandStringLength )
					{
					if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
						{
						clearQuerySelections();

						querySentenceNr_ = Constants.NO_SENTENCE_NR;
						queryItemNr_ = Constants.NO_ITEM_NR;
						queryStringBuffer = CommonVariables.queryStringBuffer;

						CommonVariables.hasFoundQuery = false;
						CommonVariables.matchingWordTypeNr = Constants.WORD_TYPE_UNDEFINED;

						while( !isEndOfQuery &&
						CommonVariables.queryStringBuffer.length() == 0 &&
						queryCommandStringPosition_ < queryCommandStringLength )
							{
							switch( queryCommandString.charAt( queryCommandStringPosition_ ) )
								{
								case Constants.QUERY_ITEM_START_CHAR:
									if( getIdFromQuery( true, queryCommandString, Constants.QUERY_ITEM_START_CHAR, Constants.QUERY_ITEM_END_CHAR ) == Constants.RESULT_OK )
										{
										if( itemQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, querySentenceNr_, queryItemNr_ ) == Constants.RESULT_OK )
											{
											isFirstInstruction = false;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_ITEM_WAS_FOUND ) );
											}
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to query items" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to get an identification from the item" );

									break;

								case Constants.QUERY_REF_ITEM_START_CHAR:
									if( getIdFromQuery( true, queryCommandString, Constants.QUERY_REF_ITEM_START_CHAR, Constants.QUERY_REF_ITEM_END_CHAR ) == Constants.RESULT_OK )
										{
										if( itemQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, true, querySentenceNr_, queryItemNr_ ) == Constants.RESULT_OK )
											{
											isFirstInstruction = false;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_REFERENCE_ITEM_WAS_FOUND ) );
											}
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to query item references" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to get a reference identification from the item" );

									break;

								case Constants.QUERY_LIST_START_CHAR:
									nameStringBuffer = new StringBuffer();

									if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_LIST_START_CHAR, Constants.QUERY_LIST_END_CHAR ) == Constants.RESULT_OK )
										{
										listStringPosition = 0;
										nameStringBufferLength = nameStringBuffer.length();

										// Check list characters for existence
										do	{
											if( nameStringBufferLength > 0 &&
											!isWordListChar( nameStringBuffer.charAt( listStringPosition ) ) &&
											!isAdminListChar( nameStringBuffer.charAt( listStringPosition ) ) )
												{
												isInvalidChar = true;
												queryStringBuffer.append( currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_INVALID_CHARACTER_IN_LIST ) );
												}
											}
										while( !isInvalidChar &&
										++listStringPosition < nameStringBufferLength );

										// All list characters are valid
										if( !isInvalidChar )
											{
											if( listQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, nameStringBuffer ) == Constants.RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_LIST_WAS_FOUND ) );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to query lists" );
											}
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to get a list string from the text" );

									break;

								case Constants.QUERY_WORD_START_CHAR:
									if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
									queryCommandString.charAt( queryCommandStringPosition_ + 1 ) != Constants.QUERY_CHAR )
										{
										nameStringBuffer = new StringBuffer();

										if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_WORD_START_CHAR, Constants.QUERY_WORD_END_CHAR ) == Constants.RESULT_OK )
											{
											if( nameStringBuffer.length() == 0 )
												{
												if( queryCommandStringPosition_ < queryCommandStringLength &&
												queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_CHAR )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_WORD_SPECIFICATION ) );
												else
													{
													isOnlyShowingWords = true;
													isReturnQueryToPosition = true;
													}
												}
											else
												{
												if( wordQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, nameStringBuffer.toString() ) == Constants.RESULT_OK )
													{
													isFirstInstruction = false;

													if( !isSuppressingMessage &&
													countQuery() == 0 )
														queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_WAS_FOUND ) );
													}
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to query words" );
												}
											}
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to get a word name from the query specification" );
										}
									else
										{
										isOnlyShowingWords = true;
										isReturnQueryToPosition = false;
										queryCommandStringPosition_++;
										}

									break;

								case Constants.QUERY_WORD_REFERENCE_START_CHAR:
									if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
									queryCommandString.charAt( queryCommandStringPosition_ + 1 ) != Constants.QUERY_CHAR )
										{
										nameStringBuffer = new StringBuffer();

										if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_WORD_REFERENCE_START_CHAR, Constants.QUERY_WORD_REFERENCE_END_CHAR ) == Constants.RESULT_OK )
											{
											if( nameStringBuffer.length() == 0 )
												{
												if( queryCommandStringPosition_ < queryCommandStringLength &&
												queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_CHAR )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_WORD_REFERENCE ) );
												else
													{
													isReturnQueryToPosition = true;
													isOnlyShowingWordReferences = true;
													}
												}
											else
												{
												if( wordReferenceQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, nameStringBuffer.toString() ) == Constants.RESULT_OK )
													{
													isFirstInstruction = false;

													if( !isSuppressingMessage &&
													countQuery() == 0 )
														queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_REFERENCE_WAS_FOUND ) );
													}
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to query word references" );
												}
											}
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to get a word reference name from the query specification" );
										}
									else
										{
										isReturnQueryToPosition = false;
										isOnlyShowingWordReferences = true;
										queryCommandStringPosition_++;
										}

									break;

								// Escape character for string
								case Constants.SYMBOL_BACK_SLASH:
									if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
									queryCommandString.charAt( queryCommandStringPosition_ + 1 ) != Constants.QUERY_CHAR )
										queryCommandStringPosition_++;

									// Don't insert a break statement here

								case Constants.QUERY_STRING_START_CHAR:
									if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
									queryCommandString.charAt( queryCommandStringPosition_ + 1 ) != Constants.QUERY_CHAR )
										{
										nameStringBuffer = new StringBuffer();

										if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_STRING_START_CHAR, Constants.QUERY_STRING_END_CHAR ) == Constants.RESULT_OK )
											{
											if( nameStringBuffer.length() == 0 )
												{
												if( queryCommandStringPosition_ < queryCommandStringLength &&
												queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_CHAR )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_STRING_SPECIFICATION ) );
												else
													{
													isOnlyShowingStrings = true;
													isReturnQueryToPosition = true;
													}
												}
											else
												{
												if( stringQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, nameStringBuffer.toString() ) == Constants.RESULT_OK )
													{
													isFirstInstruction = false;

													if( !isSuppressingMessage &&
													countQuery() == 0 )
														queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_STRING_WAS_FOUND ) );
													}
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to query strings" );
												}
											}
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to get a string from the query specification" );
										}
									else
										{
										isOnlyShowingStrings = true;
										isReturnQueryToPosition = false;
										queryCommandStringPosition_++;
										}

									break;

								case Constants.QUERY_WORD_TYPE_CHAR:
									querySentenceNr_ = Constants.NO_SENTENCE_NR;

									if( getIdFromQuery( false, queryCommandString, Constants.QUERY_WORD_TYPE_CHAR, Constants.QUERY_WORD_TYPE_CHAR ) == Constants.RESULT_OK )
										{
										if( queryItemNr_ == Constants.NO_ITEM_NR )
											{
											if( wordTypeQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, (short)querySentenceNr_ ) == Constants.RESULT_OK )
												{
												isFirstInstruction = false;
												// Remember given word type number
												queryWordTypeNr = (short)querySentenceNr_;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_TYPE_WAS_FOUND ) );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to query word types" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "The given parameter is undefined" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to get a word type" );

									break;

								case Constants.QUERY_PARAMETER_CHAR:
									querySentenceNr_ = Constants.NO_SENTENCE_NR;

									if( getIdFromQuery( false, queryCommandString, Constants.QUERY_PARAMETER_CHAR, Constants.QUERY_PARAMETER_CHAR ) == Constants.RESULT_OK )
										{
										if( queryItemNr_ == Constants.NO_ITEM_NR )
											{
											if( parameterQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, querySentenceNr_ ) == Constants.RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_PARAMETER_WAS_FOUND ) );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to query parameters" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "The given parameter is undefined" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to get a parameter" );

									break;

								case Constants.QUERY_ACTIVE_CHAR:
									// Initially
									if( isSelectingActiveItems &&
									isSelectingInactiveItems &&
									isSelectingArchivedItems &&
									isSelectingReplacedItems &&
									isSelectingDeletedItems )
										{
										isSelectingInactiveItems = false;
										isSelectingArchivedItems = false;
										isSelectingReplacedItems = false;
										isSelectingDeletedItems = false;
										}
									else
										isSelectingActiveItems = true;

									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ >= queryCommandStringLength ||
									// End of query
									queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
										{
										if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to execute an item query of active items" );
										}

									break;

								case Constants.QUERY_INACTIVE_CHAR:
									// Initially
									if( isSelectingActiveItems &&
									isSelectingInactiveItems &&
									isSelectingArchivedItems &&
									isSelectingReplacedItems &&
									isSelectingDeletedItems )
										{
										isSelectingActiveItems = false;
										isSelectingArchivedItems = false;
										isSelectingReplacedItems = false;
										isSelectingDeletedItems = false;
										}
									else
										isSelectingInactiveItems = true;

									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ >= queryCommandStringLength ||
									// End of query
									queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
										{
										if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to execute an item query of inactive items" );
										}

									break;

								case Constants.QUERY_ARCHIVED_CHAR:
									// Initially
									if( isSelectingActiveItems &&
									isSelectingInactiveItems &&
									isSelectingArchivedItems &&
									isSelectingReplacedItems &&
									isSelectingDeletedItems )
										{
										isSelectingActiveItems = false;
										isSelectingInactiveItems = false;
										isSelectingReplacedItems = false;
										isSelectingDeletedItems = false;
										}
									else
										isSelectingArchivedItems = true;

									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ >= queryCommandStringLength ||
									// End of query
									queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
										{
										if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to execute an item query of archived items" );
										}

									break;

								case Constants.QUERY_REPLACED_CHAR:
									// Initially
									if( isSelectingActiveItems &&
									isSelectingInactiveItems &&
									isSelectingArchivedItems &&
									isSelectingReplacedItems &&
									isSelectingDeletedItems )
										{
										isSelectingActiveItems = false;
										isSelectingInactiveItems = false;
										isSelectingArchivedItems = false;
										isSelectingDeletedItems = false;
										}
									else
										isSelectingReplacedItems = true;

									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ >= queryCommandStringLength ||
									// End of query
									queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
										{
										if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to execute an item query of archived items" );
										}

									break;

								case Constants.QUERY_DELETED_CHAR:
									// Initially
									if( isSelectingActiveItems &&
									isSelectingInactiveItems &&
									isSelectingArchivedItems &&
									isSelectingReplacedItems &&
									isSelectingDeletedItems )
										{
										isSelectingActiveItems = false;
										isSelectingInactiveItems = false;
										isSelectingArchivedItems = false;
										isSelectingReplacedItems = false;
										}
									else
										isSelectingDeletedItems = true;

									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ >= queryCommandStringLength ||
									// End of query
									queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
										{
										if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to execute an item query of deleted items" );
										}

									break;

								case Constants.QUERY_COUNT_CHAR:
									isShowingCount = true;
									queryCommandStringPosition_++;

									if( queryCommandStringPosition_ >= queryCommandStringLength ||
									// End of query
									queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
										{
										if( itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to query items" );
										}

									break;

								case Constants.QUERY_SELECTING_ATTACHED_JUSTIFICATIONS_CHAR:
									if( isSelectingAttachedJustifications )
										// Second asterisk
										isSelectingJustificationSpecifications = true;
									else
										// First asterisk
										isSelectingAttachedJustifications = true;

									queryCommandStringPosition_++;

									break;

								case Constants.QUERY_CHAR:
									isEndOfQuery = true;
									queryCommandStringPosition_++;

									break;

								default:
									// Set query width parameter
									if( Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) )
										{
										while( queryCommandStringPosition_ < queryCommandStringLength &&
										Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) &&
										queryWidth <= Constants.MAX_SENTENCE_NR / 10 )
											{
											queryWidth = ( queryWidth * 10 + queryCommandString.charAt( queryCommandStringPosition_ ) - '0' );
											queryCommandStringPosition_++;
											}
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "I have found an illegal character '" + queryCommandString.charAt( queryCommandStringPosition_ ) + "' in the query" );
								}
							}

						if( queryStringBuffer.length() == 0 )
							{
							// No query executed yet
							if( isFirstInstruction )
								{
								if( itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to query items" );
								}

							if( isShowingCount )
								{
								nTotal = countQuery();

								if( isSuppressingMessage )
									queryStringBuffer.append( nTotal );
								else
									{
									if( nTotal == 0 )
										queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
									else
										{
										if( isNeedingToShowTotal() )
											queryStringBuffer.append( "total:" + nTotal );

										if( CommonVariables.nActiveQueryItems > 0 )
											{
											if( queryStringBuffer.length() > 0 )
												queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

											queryStringBuffer.append( "active:" + CommonVariables.nActiveQueryItems );
											}

										if( CommonVariables.nInactiveQueryItems > 0 )
											{
											if( queryStringBuffer.length() > 0 )
												queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

											queryStringBuffer.append( "inactive:" + CommonVariables.nInactiveQueryItems );
											}

										if( CommonVariables.nArchivedQueryItems > 0 )
											{
											if( queryStringBuffer.length() > 0 )
												queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

											queryStringBuffer.append( "archived:" + CommonVariables.nArchivedQueryItems );
											}

										if( CommonVariables.nReplacedQueryItems > 0 )
											{
											if( queryStringBuffer.length() > 0 )
												queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

											queryStringBuffer.append( "replaced:" + CommonVariables.nReplacedQueryItems );
											}

										if( CommonVariables.nDeletedQueryItems > 0 )
											{
											if( queryStringBuffer.length() > 0 )
												queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

											queryStringBuffer.append( "deleted:" + CommonVariables.nDeletedQueryItems );
											}
										}
									}
								}
							else
								{
								CommonVariables.hasFoundQuery = false;

								if( showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == Constants.RESULT_OK )
									{
									if( !isSuppressingMessage &&
									!CommonVariables.hasFoundQuery &&
									queryStringBuffer.length() == 0 )
										{
										if( isOnlyShowingWords )
											queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORDS_WERE_FOUND ) );
										else
											{
											if( isOnlyShowingWordReferences )
												queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_REFERENCES_WERE_FOUND ) );
											else
												{
												if( isOnlyShowingStrings )
													queryStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_QUERY_NO_STRINGS_WERE_FOUND ) );
												}
											}
										}
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to show the query result" );
								}
							}

						// Show comment on empty query
						if( nTotalCount() == 0 ||
						CommonVariables.hasFoundQuery ||
						queryWidth > Constants.NO_CENTER_WIDTH )
							{
							if( isWritingQueryResult &&

							( queryWidth > Constants.NO_CENTER_WIDTH ||
							queryStringBuffer.length() > 0 ) )
								{
								if( Presentation.writeText( ( !isSuppressingMessage && !CommonVariables.hasFoundQuery && queryWidth == Constants.NO_CENTER_WIDTH ), promptTypeNr, queryWidth, queryStringBuffer ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write the query result" );
								}
							}
						else
							CommonVariables.queryStringBuffer = new StringBuffer();
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The query command string position is not within the range of the given query command string" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given query command string is empty" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Honor the Lord, you heavenly beings;
 *	honor the Lord for his glory and strength." (Psalm 29:1)
 *************************************************************************/
