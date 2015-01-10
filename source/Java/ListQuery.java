/*
 *	Class:			ListQuery
 *	Supports class:	List
 *	Purpose:		To process queries
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

class ListQuery
	{
	// Private constructible variables

	private List myList_;
	private String moduleNameString_;


	// Private methods

	private static void itemQuery( boolean isSelectOnFind, boolean isReferenceQuery, int querySentenceNr, int queryItemNr, Item queryItem )
		{
		while( queryItem != null )
			{
			if( ( isReferenceQuery &&
			queryItem.hasFoundReferenceItemById( querySentenceNr, queryItemNr ) ) ||

			( !isReferenceQuery &&
			( querySentenceNr == Constants.NO_SENTENCE_NR ||
			querySentenceNr == queryItem.creationSentenceNr() ) &&

			( queryItemNr == Constants.NO_SENTENCE_NR ||
			queryItemNr == queryItem.itemNr() ) ) )
				{
				if( isSelectOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void listQuery( boolean isSelectOnFind, Item queryItem )
		{
		while( queryItem != null )
			{
			if( isSelectOnFind )
				{
				if( !queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void wordTypeQuery( boolean isSelectOnFind, short queryWordTypeNr, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.hasFoundWordType( queryWordTypeNr ) )
				{
				if( isSelectOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void parameterQuery( boolean isSelectOnFind, int queryParameter, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.hasFoundParameter( queryParameter ) )
				{
				if( isSelectOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void wordQuery( boolean isSelectOnFind, Item queryItem )
		{
		while( queryItem != null )
			{
			if( isSelectOnFind )
				{
				if( !queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void clearQuerySelections( Item searchItem )
		{
		while( searchItem != null )
			{
			searchItem.isSelectedByQuery = false;
			searchItem = searchItem.nextItem;
			}
		}

	private byte wordReferenceQuery( boolean isSelectOnFind, String wordReferenceNameString, Item queryItem )
		{
		ReferenceResultType referenceResult;

		while( queryItem != null )
			{
			if( ( referenceResult = queryItem.findMatchingWordReferenceString( wordReferenceNameString ) ).result == Constants.RESULT_OK )
				{
				if( referenceResult.hasFoundMatchingStrings )
					{
					if( isSelectOnFind &&
					!queryItem.isSelectedByQuery )
						{
						CommonVariables.hasFoundQuery = true;
						queryItem.isSelectedByQuery = true;
						}
					}
				else
					{
					if( !isSelectOnFind &&
					queryItem.isSelectedByQuery )
						queryItem.isSelectedByQuery = false;
					}

				queryItem = queryItem.nextItem;
				}
			else
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to check the word references" );
			}

		return Constants.RESULT_OK;
		}

	private byte stringQuery( boolean isSelectOnFind, String wordString, Item queryItem )
		{
		ReferenceResultType referenceResult;
		boolean hasFoundString;
		String itemString;

		while( queryItem != null )
			{
			hasFoundString = false;

			if( ( itemString = queryItem.itemString() ) != null )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == Constants.RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to compare two strings" );
				}

			if( !hasFoundString &&
			( itemString = queryItem.extraItemString() ) != null )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == Constants.RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to compare two strings" );
				}

			if( hasFoundString )
				{
				if( isSelectOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte showQueryResult( boolean showOnlyWords, boolean showOnlyWordReferences, boolean showOnlyStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.isSelectedByQuery )
				{
				if( showOnlyWords )
					queryItem.showWords( isReturnQueryToPosition, queryWordTypeNr );
				else
					{
					if( showOnlyWordReferences )
						queryItem.showWordReferences( isReturnQueryToPosition );
					else
						{
						if( showOnlyStrings )
							queryItem.showString( isReturnQueryToPosition );
						else
							{
							if( Presentation.writeText( true, promptTypeNr, queryWidth, queryItem.toStringBuffer( queryWordTypeNr ) ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to show the info of an active item" );
							}
						}
					}
				}

			queryItem = queryItem.nextItem;
			}

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected ListQuery( List myList )
		{
		String errorString = null;

		myList_ = myList;
		moduleNameString_ = this.getClass().getName();

		if( myList_ == null )
			errorString = "The given my list is undefined";

		if( errorString != null )
			{
			if( myList_ != null &&
			myList_.myWordItem() != null )
				myList_.startSystemError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void countQuery()
		{
		Item searchItem = myList_.firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				CommonVariables.nActiveQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				CommonVariables.nInactiveQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				CommonVariables.nArchivedQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				CommonVariables.nReplacedQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstDeletedItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				CommonVariables.nDeletedQueryItems++;

			searchItem = searchItem.nextItem;
			}
		}

	protected void clearQuerySelections()
		{
		clearQuerySelections( myList_.firstActiveItem() );
		clearQuerySelections( myList_.firstInactiveItem() );
		clearQuerySelections( myList_.firstArchivedItem() );
		clearQuerySelections( myList_.firstReplacedItem() );
		clearQuerySelections( myList_.firstDeletedItem() );
		}

	protected void itemQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		if( isSelectActiveItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_.firstActiveItem() );

		if( isSelectInactiveItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_.firstInactiveItem() );

		if( isSelectArchivedItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_.firstArchivedItem() );

		if( isSelectReplacedItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_.firstReplacedItem() );

		if( isSelectDeletedItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_.firstDeletedItem() );
		}

	protected void listQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems )
		{
		if( isSelectActiveItems )
			listQuery( isSelectOnFind, myList_.firstActiveItem() );

		if( isSelectInactiveItems )
			listQuery( isSelectOnFind, myList_.firstInactiveItem() );

		if( isSelectArchivedItems )
			listQuery( isSelectOnFind, myList_.firstArchivedItem() );

		if( isSelectReplacedItems )
			listQuery( isSelectOnFind, myList_.firstReplacedItem() );

		if( isSelectDeletedItems )
			listQuery( isSelectOnFind, myList_.firstDeletedItem() );
		}

	protected void wordTypeQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, short queryWordTypeNr )
		{
		if( isSelectActiveItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_.firstActiveItem() );

		if( isSelectInactiveItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_.firstInactiveItem() );

		if( isSelectArchivedItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_.firstArchivedItem() );

		if( isSelectReplacedItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_.firstReplacedItem() );

		if( isSelectDeletedItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_.firstDeletedItem() );
		}

	protected void parameterQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, int queryParameter )
		{
		if( isSelectActiveItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_.firstActiveItem() );

		if( isSelectInactiveItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_.firstInactiveItem() );

		if( isSelectArchivedItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_.firstArchivedItem() );

		if( isSelectReplacedItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_.firstReplacedItem() );

		if( isSelectDeletedItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_.firstDeletedItem() );
		}

	protected void wordQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems )
		{
		if( isSelectActiveItems )
			wordQuery( isSelectOnFind, myList_.firstActiveItem() );

		if( isSelectInactiveItems )
			wordQuery( isSelectOnFind, myList_.firstInactiveItem() );

		if( isSelectArchivedItems )
			wordQuery( isSelectOnFind, myList_.firstArchivedItem() );

		if( isSelectReplacedItems )
			wordQuery( isSelectOnFind, myList_.firstReplacedItem() );

		if( isSelectDeletedItems )
			wordQuery( isSelectOnFind, myList_.firstDeletedItem() );
		}

	protected ReferenceResultType compareStrings( String searchString, String sourceString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();
		boolean isStop;
		int searchStringPosition = 0;
		int sourceStringPosition = 0;

		if( searchString != null )
			{
			if( sourceString != null )
				{
				if( searchString != sourceString )
					{
					referenceResult.hasFoundMatchingStrings = true;

					while( CommonVariables.result == Constants.RESULT_OK &&
					referenceResult.hasFoundMatchingStrings &&
					searchStringPosition < searchString.length() &&
					sourceStringPosition < sourceString.length() )
						{
						if( searchString.charAt( searchStringPosition ) == Constants.SYMBOL_QUESTION_MARK ||
						searchString.charAt( searchStringPosition ) == sourceString.charAt( sourceStringPosition ) )
							{
							searchStringPosition++;
							sourceStringPosition++;
							}
						else
							{
							if( searchString.charAt( searchStringPosition ) == Constants.SYMBOL_ASTERISK )
								{
								if( ++searchStringPosition < searchString.length() )
									{
									isStop = false;

									while( !isStop &&
									sourceStringPosition < sourceString.length() )
										{
										if( searchString.charAt( searchStringPosition ) == sourceString.charAt( sourceStringPosition ) )
											{
											// Check remaining strings
											if( ( referenceResult = compareStrings( searchString.substring( searchStringPosition ), sourceString.substring( sourceStringPosition ) ) ).result == Constants.RESULT_OK )
												{
												if( referenceResult.hasFoundMatchingStrings )
													{
													isStop = true;
													searchStringPosition++;
													}
												else
													referenceResult.hasFoundMatchingStrings = true;	// Reset indicator

												sourceStringPosition++;
												}
											else
												myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to compare the remaining strings" );
											}
										else
											sourceStringPosition++;					// Skip source characters when not equal
										}
									}
								else
									sourceStringPosition = sourceString.length();	// Empty source string after asterisk
								}
							else
								referenceResult.hasFoundMatchingStrings = false;
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK )
						{
						if( referenceResult.hasFoundMatchingStrings &&
						sourceStringPosition == sourceString.length() )
							{
							// Check search string for extra asterisks
							while( searchStringPosition < searchString.length() &&
							searchString.charAt( searchStringPosition ) == Constants.SYMBOL_ASTERISK )
								searchStringPosition++;		// Skip extra asterisks
							}

						if( searchStringPosition < searchString.length() ||
						sourceStringPosition < sourceString.length() )
							referenceResult.hasFoundMatchingStrings = false;
						}
					}
				else
					myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given strings are the same string" );
				}
			else
				myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given source string is undefined" );
			}
		else
			myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given search string is undefined" );

		referenceResult.result = CommonVariables.result;
		return referenceResult;
		}

	protected byte wordReferenceQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordReferenceNameString )
		{
		if( isSelectActiveItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_.firstActiveItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to check the word references of an active word" );
			}

		if( isSelectInactiveItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_.firstInactiveItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to check the word references of an inactive word" );
			}

		if( isSelectArchivedItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_.firstArchivedItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to check the word references of an archived word" );
			}

		if( isSelectReplacedItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_.firstReplacedItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to check the word references of a replaced word" );
			}

		if( isSelectDeletedItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_.firstDeletedItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to check the word references of a deleted word" );
			}

		return Constants.RESULT_OK;
		}

	protected byte stringQuery( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordString )
		{
		if( isSelectActiveItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_.firstActiveItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to string query an active word" );
			}

		if( isSelectInactiveItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_.firstInactiveItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to string query an inactive word" );
			}

		if( isSelectArchivedItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_.firstArchivedItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to string query an archived word" );
			}

		if( isSelectReplacedItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_.firstReplacedItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to string query a replaced word" );
			}

		if( isSelectDeletedItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_.firstDeletedItem() ) != Constants.RESULT_OK )
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to string query a deleted word" );
			}

		return Constants.RESULT_OK;
		}

	protected byte showQueryResult( boolean showOnlyWords, boolean showOnlyWordReferences, boolean showOnlyStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_.firstActiveItem() ) == Constants.RESULT_OK )
			{
			if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_.firstInactiveItem() ) == Constants.RESULT_OK )
				{
				if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_.firstArchivedItem() ) == Constants.RESULT_OK )
					{
					if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_.firstReplacedItem() ) == Constants.RESULT_OK )
						{
						if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_.firstDeletedItem() ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to show the query result of a deleted word" );
						}
					else
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to show the query result of a replaced word" );
					}
				else
					return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to show the query result of an archived word" );
				}
			else
				return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to show the query result of an inactive word" );
			}
		else
			return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to show the query result of an active word" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"Your name, O Lord, endures forever;
 *	your name, O Lord, is known to every generation." (Psalm 135:13)
 *
 *************************************************************************/
