/*	Class:			ListQuery
 *	Supports class:	List
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

class ListQuery
	{
	// Private constructible variables

	private List myList_;
	private String moduleNameString_;


	// Private methods

	private static void itemQuery( boolean isSelectingOnFind, boolean isReferenceQuery, int querySentenceNr, int queryItemNr, Item queryItem )
		{
		while( queryItem != null )
			{
			if( ( isReferenceQuery &&
			queryItem.hasFoundReferenceItemById( querySentenceNr, queryItemNr ) ) ||

			( !isReferenceQuery &&

			( querySentenceNr == Constants.NO_SENTENCE_NR ||
			queryItem.hasSentenceNr( querySentenceNr ) ) &&

			( queryItemNr == Constants.NO_SENTENCE_NR ||
			queryItemNr == queryItem.itemNr() ) ) )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void listQuery( boolean isSelectingOnFind, Item queryItem )
		{
		while( queryItem != null )
			{
			if( isSelectingOnFind )
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

	private static void wordTypeQuery( boolean isSelectingOnFind, short queryWordTypeNr, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.hasFoundWordType( queryWordTypeNr ) )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void parameterQuery( boolean isSelectingOnFind, int queryParameter, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.hasFoundParameter( queryParameter ) )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void wordQuery( boolean isSelectingOnFind, Item queryItem )
		{
		while( queryItem != null )
			{
			if( isSelectingOnFind )
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
			searchItem.isSelectedByJustificationQuery = false;

			searchItem = searchItem.nextItem;
			}
		}

	private byte wordReferenceQuery( boolean isSelectingOnFind, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString, Item queryItem )
		{
		ReferenceResultType referenceResult;

		while( queryItem != null )
			{
			if( ( referenceResult = queryItem.findMatchingWordReferenceString( wordReferenceNameString ) ).result == Constants.RESULT_OK )
				{
				if( referenceResult.hasFoundMatchingStrings )
					{
					if( queryItem.isSelectedByQuery )
						{
						if( isSelectingAttachedJustifications )
							queryItem.selectingAttachedJustifications( isSelectingJustificationSpecifications );
						}
					else
						{
						if( isSelectingOnFind )
							{
							CommonVariables.hasFoundQuery = true;
							queryItem.isSelectedByQuery = true;
							}
						}
					}
				else
					{
					if( !isSelectingOnFind &&
					queryItem.isSelectedByQuery )
						queryItem.isSelectedByQuery = false;
					}

				queryItem = queryItem.nextItem;
				}
			else
				return myList_.addError( 1, moduleNameString_, "I failed to check the word references" );
			}

		return Constants.RESULT_OK;
		}

	private byte stringQuery( boolean isSelectingOnFind, String wordString, Item queryItem )
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
					return myList_.addError( 1, moduleNameString_, "I failed to compare two strings" );
				}

			if( !hasFoundString &&
			( itemString = queryItem.virtualGuideByGrammarString() ) != null )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == Constants.RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_.addError( 1, moduleNameString_, "I failed to compare a Guide by Grammar string" );
				}

			if( hasFoundString )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					CommonVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte showQueryResult( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.isSelectedByQuery ||
			queryItem.isSelectedByJustificationQuery )
				{
				if( isOnlyShowingWords )
					queryItem.showWords( isReturnQueryToPosition, queryWordTypeNr );
				else
					{
					if( isOnlyShowingWordReferences )
						queryItem.showWordReferences( isReturnQueryToPosition );
					else
						{
						if( isOnlyShowingStrings )
							queryItem.showString( isReturnQueryToPosition );
						else
							{
							if( Presentation.writeText( true, promptTypeNr, queryWidth, queryItem.toStringBuffer( queryWordTypeNr ) ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, "I failed to show the info of an active item" );
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
				myList_.startSystemError( 1, moduleNameString_, errorString );
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
		Item searchItem;

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_.firstInactiveItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_.firstArchivedItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_.firstReplacedItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_.firstDeletedItem() ) != null )
			clearQuerySelections( searchItem );
		}

	protected void itemQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );
		}

	protected void listQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			listQuery( isSelectingOnFind, searchItem );
		}

	protected void wordTypeQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );
		}

	protected void parameterQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );
		}

	protected void wordQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );
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
													// Reset indicator
													referenceResult.hasFoundMatchingStrings = true;

												sourceStringPosition++;
												}
											else
												myList_.addError( 1, moduleNameString_, "I failed to compare the remaining strings" );
											}
										else
											// Skip source characters if not equal
											sourceStringPosition++;
										}
									}
								else
									// Empty source string after asterisk
									sourceStringPosition = sourceString.length();
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
								// Skip extra asterisks
								searchStringPosition++;
							}

						if( searchStringPosition < searchString.length() ||
						sourceStringPosition < sourceString.length() )
							referenceResult.hasFoundMatchingStrings = false;
						}
					}
				else
					myList_.startError( 1, moduleNameString_, "The given strings are the same string" );
				}
			else
				myList_.startError( 1, moduleNameString_, "The given source string is undefined" );
			}
		else
			myList_.startError( 1, moduleNameString_, "The given search string is undefined" );

		referenceResult.result = CommonVariables.result;
		return referenceResult;
		}

	protected byte wordReferenceQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		return CommonVariables.result;
		}

	protected byte stringQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordString )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_.firstActiveItem() ) != null )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		isSelectingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		return CommonVariables.result;
		}

	protected byte showQueryResult( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		Item searchItem;

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return CommonVariables.result;
		}
	};

/*************************************************************************
 *	"Your name, O Lord, endures forever;
 *	your name, O Lord, is known to every generation." (Psalm 135:13)
 *************************************************************************/
