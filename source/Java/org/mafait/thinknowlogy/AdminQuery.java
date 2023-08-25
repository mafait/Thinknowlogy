/*	Class:			AdminQuery
 *	Supports class:	AdminItem
 *	Purpose:		Processing queries
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

class AdminQuery
	{
	// Private constructed variables

	private int queryCommandStringPosition_ = 0;
	private int queryItemNr_ = Constants.NO_ITEM_NR;
	private int querySentenceNr_ = Constants.NO_SENTENCE_NR;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private static boolean isDisplayingTotal()
		{
		int nFoundCategories = 0;

		if( GlobalVariables.nActiveQueryItems > 0 )
			nFoundCategories++;

		if( GlobalVariables.nInactiveQueryItems > 0 )
			nFoundCategories++;

		if( GlobalVariables.nArchivedQueryItems > 0 )
			nFoundCategories++;

		if( GlobalVariables.nReplacedQueryItems > 0 )
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

	private int nTotalCountQuery()
		{
		GlobalVariables.nActiveQueryItems = 0;
		GlobalVariables.nInactiveQueryItems = 0;
		GlobalVariables.nArchivedQueryItems = 0;
		GlobalVariables.nReplacedQueryItems = 0;

		adminItem_.countQuery();

		return ( GlobalVariables.nActiveQueryItems +
				GlobalVariables.nInactiveQueryItems +
				GlobalVariables.nArchivedQueryItems +
				GlobalVariables.nReplacedQueryItems );
		}

	private byte getIdFromQuery( boolean hasEndChar, String queryCommandString, char startChar, char endChar )
		{
		int queryCommandStringLength;

		querySentenceNr_ = Constants.NO_SENTENCE_NR;
		queryItemNr_ = Constants.NO_ITEM_NR;

		if( queryCommandString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is undefined" );

		if( ( queryCommandStringLength = queryCommandString.length() ) == 0 )
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is empty" );

		if( queryCommandStringPosition_ <= 0 ||
		queryCommandStringPosition_ >= queryCommandStringLength )
			return adminItem_.startError( 1, moduleNameString_, "The query command string position is not within the range of the given query command string" );

		if( queryCommandString.charAt( queryCommandStringPosition_ ) != startChar )
			return adminItem_.startError( 1, moduleNameString_, "The given command doesn't start with character '" + startChar + "', but with '" + queryCommandString.charAt( queryCommandStringPosition_ ) + "'" );

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
					if( !Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find a number in the query string" );

					while( queryCommandStringPosition_ < queryCommandStringLength &&
					Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) &&
					querySentenceNr_ <= Constants.MAX_SENTENCE_NR / 10 )
						{
						querySentenceNr_ = ( querySentenceNr_ * 10 ) + ( queryCommandString.charAt( queryCommandStringPosition_ ) - '0' );
						queryCommandStringPosition_++;
						}
					}

				if( hasEndChar &&
				queryCommandStringPosition_ < queryCommandStringLength &&
				queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_SEPARATOR_CHAR &&
				++queryCommandStringPosition_ < queryCommandStringLength &&
				queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
					{
					if( !Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find the item number in the query string" );

					while( queryCommandStringPosition_ < queryCommandStringLength &&
					Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) &&
					queryItemNr_ <= Constants.MAX_SENTENCE_NR / 10 )
						{
						queryItemNr_ = ( queryItemNr_ * 10 ) + ( queryCommandString.charAt( queryCommandStringPosition_ ) - '0' );
						queryCommandStringPosition_++;
						}
					}
				}
			}

		if( hasEndChar )
			{
			if( queryCommandStringPosition_ >= queryCommandStringLength ||
			queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
				return adminItem_.startError( 1, moduleNameString_, "The given query string is corrupt" );

			queryCommandStringPosition_++;
			}

		return Constants.RESULT_OK;
		}

	private byte getNameFromQuery( String queryCommandString, StringBuffer nameStringBuffer, char startChar, char endChar )
		{
		int queryCommandStringLength;
		int startSourceStringPosition;

		if( queryCommandString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is undefined" );

		if( nameStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given name string buffer is undefined" );

		if( ( queryCommandStringLength = queryCommandString.length() ) == 0 )
			return adminItem_.startError( 1, moduleNameString_, "The given query command string is empty" );

		if( queryCommandStringPosition_ <= 0 ||
		queryCommandStringPosition_ >= queryCommandStringLength )
			return adminItem_.startError( 1, moduleNameString_, "The query command string position is not within the range of the given query command string" );

		if( queryCommandString.charAt( queryCommandStringPosition_ ) != startChar )
			return adminItem_.startError( 1, moduleNameString_, "The given name doesn't start with character '" + startChar + "', but with '" + queryCommandString.charAt( 0 ) + "'" );

		if( queryCommandStringPosition_ + 1 >= queryCommandStringLength )
			return adminItem_.startError( 1, moduleNameString_, "The name in the given query string is corrupt" );

		queryCommandStringPosition_++;

		if( queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
			{
			startSourceStringPosition = queryCommandStringPosition_;

			while( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
			queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
				queryCommandStringPosition_++;

			if( queryCommandString.charAt( queryCommandStringPosition_ ) != endChar )
				return adminItem_.startError( 1, moduleNameString_, "The name in the given query string is corrupt" );

			nameStringBuffer.append( queryCommandString.substring( startSourceStringPosition, queryCommandStringPosition_ ) );
			}

		queryCommandStringPosition_++;

		return Constants.RESULT_OK;
		}

	private byte itemQuery( boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSuppressingMessage, StringBuffer textStringBuffer )
		{
		if( textStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given text string buffer is undefined" );

		adminItem_.itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR );

		if( !isSuppressingMessage &&
		nTotalCountQuery() == 0 )
			textStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );

		return Constants.RESULT_OK;
		}

	// Constructor

	protected AdminQuery( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected QueryResultType executeQuery( boolean isSuppressingMessage, boolean isReturningToPosition, boolean isWritingQueryResult, short promptTypeNr, int queryCommandStringStartPosition, String queryCommandString )
		{
		boolean isDisplayingCount = false;
		boolean isEndOfQuery = false;
		boolean isFirstInstruction = true;
		boolean isInvalidChar = false;
		boolean isOnlyDisplayingStrings = false;
		boolean isOnlyDisplayingWords = false;
		boolean isOnlyDisplayingWordReferences = false;
		boolean isReturnQueryToPosition = isReturningToPosition;
		boolean isSelectingActiveItems = true;
		boolean isSelectingInactiveItems = true;
		boolean isSelectingArchivedItems = true;
		boolean isSelectingReplacedItems = true;
		boolean isSelectingAttachedJustifications = false;
		boolean isSelectingJustificationSpecifications = false;
		short queryWordTypeNr = Constants.NO_WORD_TYPE_NR;
		int listStringPosition;
		int nameStringBufferLength;
		int nTotalCount = 0;
		int queryCommandStringLength;
		int queryWidth = Constants.NO_CENTER_WIDTH;
		StringBuffer queryStringBuffer;
		StringBuffer nameStringBuffer = new StringBuffer();
		QueryResultType queryResult = new QueryResultType();

		if( queryCommandString == null )
			return adminItem_.startQueryResultError( 1, moduleNameString_, "The given query command string is undefined" );

		if( ( queryCommandStringLength = queryCommandString.length() ) == 0 )
			return adminItem_.startQueryResultError( 1, moduleNameString_, "The given query command string is empty" );

		if( queryCommandStringStartPosition <= 0 ||
		queryCommandStringStartPosition >= queryCommandStringLength )
			return adminItem_.startQueryResultError( 1, moduleNameString_, "The given query command string start position is not within the range of the given query command string" );

		querySentenceNr_ = Constants.NO_SENTENCE_NR;
		queryItemNr_ = Constants.NO_ITEM_NR;

		queryCommandStringPosition_ = queryCommandStringStartPosition;

		GlobalVariables.queryStringBuffer = new StringBuffer();
		queryStringBuffer = GlobalVariables.queryStringBuffer;

		GlobalVariables.hasFoundQuery = false;

		adminItem_.clearQuerySelections();

		while( !isEndOfQuery &&
		GlobalVariables.queryStringBuffer.length() == 0 &&
		queryCommandStringPosition_ < queryCommandStringLength )
			{
			switch( queryCommandString.charAt( queryCommandStringPosition_ ) )
				{
				case Constants.QUERY_ITEM_START_CHAR:
					if( getIdFromQuery( true, queryCommandString, Constants.QUERY_ITEM_START_CHAR, Constants.QUERY_ITEM_END_CHAR ) != Constants.RESULT_OK )
						return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get an identification from the item" );

					adminItem_.itemQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, false, querySentenceNr_, queryItemNr_ );

					isFirstInstruction = false;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_ITEM_WAS_FOUND ) );

					break;

				case Constants.QUERY_REF_ITEM_START_CHAR:
					if( getIdFromQuery( true, queryCommandString, Constants.QUERY_REF_ITEM_START_CHAR, Constants.QUERY_REF_ITEM_END_CHAR ) != Constants.RESULT_OK )
						return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a reference identification from the item" );

					adminItem_.itemQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, true, querySentenceNr_, queryItemNr_ );

					isFirstInstruction = false;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_REFERENCE_ITEM_WAS_FOUND ) );

					break;

				case Constants.QUERY_LIST_START_CHAR:
					nameStringBuffer = new StringBuffer();

					if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_LIST_START_CHAR, Constants.QUERY_LIST_END_CHAR ) != Constants.RESULT_OK )
						return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a list string from the text" );

					listStringPosition = 0;
					nameStringBufferLength = nameStringBuffer.length();

					// Check list characters for existence
					do	{
						if( nameStringBufferLength > 0 &&
						!isWordListChar( nameStringBuffer.charAt( listStringPosition ) ) &&
						!isAdminListChar( nameStringBuffer.charAt( listStringPosition ) ) )
							{
							isInvalidChar = true;
							queryStringBuffer.append( adminItem_.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_INVALID_CHARACTER_IN_LIST ) );
							}
						}
					while( !isInvalidChar &&
					++listStringPosition < nameStringBufferLength );

					// All list characters are valid
					if( !isInvalidChar )
						{
						adminItem_.listQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, nameStringBuffer );

						isFirstInstruction = false;

						if( !isSuppressingMessage &&
						nTotalCountQuery() == 0 )
							queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_LIST_WAS_FOUND ) );
						}

					break;

				case Constants.QUERY_WORD_START_CHAR:
					if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
					queryCommandString.charAt( queryCommandStringPosition_ + 1 ) != Constants.QUERY_CHAR )
						{
						nameStringBuffer = new StringBuffer();

						if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_WORD_START_CHAR, Constants.QUERY_WORD_END_CHAR ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a word name from the query specification" );

						if( nameStringBuffer.length() == 0 )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_CHAR )
								queryStringBuffer.append( adminItem_.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_WORD_SPECIFICATION ) );
							else
								{
								isOnlyDisplayingWords = true;
								isReturnQueryToPosition = true;
								}
							}
						else
							{
							if( adminItem_.wordQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, nameStringBuffer.toString() ) != Constants.RESULT_OK )
								return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to query words" );

							isFirstInstruction = false;

							if( !isSuppressingMessage &&
							nTotalCountQuery() == 0 )
								queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_WAS_FOUND ) );
							}
						}
					else
						{
						isOnlyDisplayingWords = true;
						isReturnQueryToPosition = false;
						queryCommandStringPosition_++;
						}

					break;

				case Constants.QUERY_WORD_REFERENCE_START_CHAR:
					if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
					queryCommandString.charAt( queryCommandStringPosition_ + 1 ) != Constants.QUERY_CHAR )
						{
						nameStringBuffer = new StringBuffer();

						if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_WORD_REFERENCE_START_CHAR, Constants.QUERY_WORD_REFERENCE_END_CHAR ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a word reference name from the query specification" );

						if( nameStringBuffer.length() == 0 )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_CHAR )
								queryStringBuffer.append( adminItem_.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_WORD_REFERENCE ) );
							else
								{
								isReturnQueryToPosition = true;
								isOnlyDisplayingWordReferences = true;
								}
							}
						else
							{
							if( adminItem_.wordReferenceQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, nameStringBuffer.toString() ) != Constants.RESULT_OK )
								return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to query word references" );

							isFirstInstruction = false;

							if( !isSuppressingMessage &&
							nTotalCountQuery() == 0 )
								queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_REFERENCE_WAS_FOUND ) );
							}
						}
					else
						{
						isReturnQueryToPosition = false;
						isOnlyDisplayingWordReferences = true;
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

						if( getNameFromQuery( queryCommandString, nameStringBuffer, Constants.QUERY_STRING_START_CHAR, Constants.QUERY_STRING_END_CHAR ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a string from the query specification" );

						if( nameStringBuffer.length() == 0 )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString.charAt( queryCommandStringPosition_ ) != Constants.QUERY_CHAR )
								queryStringBuffer.append( adminItem_.interfaceString( isSuppressingMessage ? Constants.INTERFACE_QUERY_ERROR : Constants.INTERFACE_QUERY_EMPTY_STRING_SPECIFICATION ) );
							else
								{
								isOnlyDisplayingStrings = true;
								isReturnQueryToPosition = true;
								}
							}
						else
							{
							if( adminItem_.stringQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, nameStringBuffer.toString() ) != Constants.RESULT_OK )
								return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to query strings" );

							isFirstInstruction = false;

							if( !isSuppressingMessage &&
							nTotalCountQuery() == 0 )
								queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_STRING_WAS_FOUND ) );
							}
						}
					else
						{
						isOnlyDisplayingStrings = true;
						isReturnQueryToPosition = false;
						queryCommandStringPosition_++;
						}

					break;

				case Constants.QUERY_WORD_TYPE_CHAR:
					querySentenceNr_ = Constants.NO_SENTENCE_NR;

					if( getIdFromQuery( false, queryCommandString, Constants.QUERY_WORD_TYPE_CHAR, Constants.QUERY_WORD_TYPE_CHAR ) != Constants.RESULT_OK )
						return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a word type" );

					if( queryItemNr_ != Constants.NO_ITEM_NR )
						return adminItem_.startQueryResultError( 1, moduleNameString_, "The given parameter is undefined" );

					adminItem_.wordTypeQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, (short)querySentenceNr_ );

					isFirstInstruction = false;
					// Remember given word type number
					queryWordTypeNr = (short)querySentenceNr_;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_TYPE_WAS_FOUND ) );

					break;

				case Constants.QUERY_PARAMETER_CHAR:
					querySentenceNr_ = Constants.NO_SENTENCE_NR;

					if( getIdFromQuery( false, queryCommandString, Constants.QUERY_PARAMETER_CHAR, Constants.QUERY_PARAMETER_CHAR ) != Constants.RESULT_OK )
						return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to get a parameter" );

					if( queryItemNr_ != Constants.NO_ITEM_NR )
						return adminItem_.startQueryResultError( 1, moduleNameString_, "The given parameter is undefined" );

					adminItem_.parameterQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, querySentenceNr_ );

					isFirstInstruction = false;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_PARAMETER_WAS_FOUND ) );

					break;

				case Constants.QUERY_ACTIVE_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems )
						{
						isSelectingInactiveItems = false;
						isSelectingArchivedItems = false;
						isSelectingReplacedItems = false;
						}
					else
						isSelectingActiveItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to execute an item query of active items" );
						}

					break;

				case Constants.QUERY_INACTIVE_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems )
						{
						isSelectingActiveItems = false;
						isSelectingArchivedItems = false;
						isSelectingReplacedItems = false;
						}
					else
						isSelectingInactiveItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to execute an item query of inactive items" );
						}

					break;

				case Constants.QUERY_ARCHIVED_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems )
						{
						isSelectingActiveItems = false;
						isSelectingInactiveItems = false;
						isSelectingReplacedItems = false;
						}
					else
						isSelectingArchivedItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to execute an item query of archived items" );
						}

					break;

				case Constants.QUERY_REPLACED_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems )
						{
						isSelectingActiveItems = false;
						isSelectingInactiveItems = false;
						isSelectingArchivedItems = false;
						}
					else
						isSelectingReplacedItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSuppressingMessage, queryStringBuffer ) != Constants.RESULT_OK )
							return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to execute an item query of archived items" );
						}

					break;

				case Constants.QUERY_DELETED_CHAR:
					// Deleted items are cleaned up
					isSelectingActiveItems = false;
					isSelectingInactiveItems = false;
					isSelectingArchivedItems = false;
					isSelectingReplacedItems = false;

					queryCommandStringPosition_++;

					break;

				case Constants.QUERY_COUNT_CHAR:
					isDisplayingCount = true;
					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString.charAt( queryCommandStringPosition_ ) == Constants.QUERY_CHAR )
						adminItem_.itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR );

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
					if( !Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) )
						return adminItem_.startQueryResultError( 1, moduleNameString_, "I found an illegal character '" + queryCommandString.charAt( queryCommandStringPosition_ ) + "' in the query" );

					while( queryCommandStringPosition_ < queryCommandStringLength &&
					Character.isDigit( queryCommandString.charAt( queryCommandStringPosition_ ) ) &&
					queryWidth <= Constants.MAX_SENTENCE_NR / 10 )
						{
						queryWidth = ( queryWidth * 10 + queryCommandString.charAt( queryCommandStringPosition_ ) - '0' );
						queryCommandStringPosition_++;
						}
				}
			}

		if( queryStringBuffer.length() == 0 )
			{
			// No query executed yet
			if( isFirstInstruction )
				adminItem_.itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, false, Constants.NO_SENTENCE_NR, Constants.NO_ITEM_NR );

			if( isDisplayingCount )
				{
				nTotalCount = nTotalCountQuery();

				if( isSuppressingMessage )
					queryStringBuffer.append( nTotalCount );
				else
					{
					if( nTotalCount == 0 )
						queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
					else
						{
						if( isDisplayingTotal() )
							queryStringBuffer.append( "total:" + nTotalCount );

						if( GlobalVariables.nActiveQueryItems > 0 )
							{
							if( queryStringBuffer.length() > 0 )
								queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

							queryStringBuffer.append( "active:" + GlobalVariables.nActiveQueryItems );
							}

						if( GlobalVariables.nInactiveQueryItems > 0 )
							{
							if( queryStringBuffer.length() > 0 )
								queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

							queryStringBuffer.append( "inactive:" + GlobalVariables.nInactiveQueryItems );
							}

						if( GlobalVariables.nArchivedQueryItems > 0 )
							{
							if( queryStringBuffer.length() > 0 )
								queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

							queryStringBuffer.append( "archived:" + GlobalVariables.nArchivedQueryItems );
							}

						if( GlobalVariables.nReplacedQueryItems > 0 )
							{
							if( queryStringBuffer.length() > 0 )
								queryStringBuffer.append( Constants.QUERY_SEPARATOR_SPACE_STRING );

							queryStringBuffer.append( "replaced:" + GlobalVariables.nReplacedQueryItems );
							}
						}
					}
				}
			else
				{
				GlobalVariables.hasFoundQuery = false;

				if( adminItem_.displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
					return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to display the query result" );

				if( !isSuppressingMessage &&
				!GlobalVariables.hasFoundQuery &&
				queryStringBuffer.length() == 0 )
					{
					if( isOnlyDisplayingWords )
						queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_WORDS_WERE_FOUND ) );
					else
						{
						if( isOnlyDisplayingWordReferences )
							queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_WORD_REFERENCES_WERE_FOUND ) );
						else
							{
							if( isOnlyDisplayingStrings )
								queryStringBuffer.append( adminItem_.interfaceString( Constants.INTERFACE_QUERY_NO_STRINGS_WERE_FOUND ) );
							}
						}
					}
				}
			}

		if( isDisplayingCount &&
		nTotalCount > 0 &&
		queryWidth == Constants.NO_CENTER_WIDTH &&
		!GlobalVariables.hasFoundQuery )
			GlobalVariables.queryStringBuffer = new StringBuffer();
		else
			{
			if( isWritingQueryResult &&

			( queryWidth > Constants.NO_CENTER_WIDTH ||
			queryStringBuffer.length() > 0 ) &&

			// Display comment on empty query
			InputOutput.writeText( ( !isSuppressingMessage && !GlobalVariables.hasFoundQuery && queryWidth == Constants.NO_CENTER_WIDTH ), promptTypeNr, queryWidth, queryStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addQueryResultError( 1, moduleNameString_, "I failed to write the query result" );
			}

		queryResult.queryCommandStringPosition = queryCommandStringPosition_;
		return queryResult;
		}
	}

/*************************************************************************
 *	"Honor the Lord, you heavenly beings;
 *	honor the Lord for his glory and strength." (Psalm 29:1)
 *************************************************************************/
