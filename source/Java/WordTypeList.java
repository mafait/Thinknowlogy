/*
 *	Class:			WordTypeList
 *	Parent class:	List
 *	Purpose:		To store word type items
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class WordTypeList extends List
	{
	// Private constructible variables

	private WordTypeItem foundWordTypeItem_;


	// Private methods

	private static void showWords( boolean isReturnQueryToPosition, WordTypeItem searchItem )
		{
		String wordTypeString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		while( searchItem != null )
			{
			if( ( wordTypeString = searchItem.itemString() ) != null )
				{
				if( CommonVariables.hasFoundQuery ||
				CommonVariables.queryStringBuffer.length() > 0 )
					CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

				CommonVariables.hasFoundQuery = true;

				// Don't show status with active items
				if( !searchItem.isActiveItem() )
					CommonVariables.queryStringBuffer.append( searchItem.statusChar() );

				CommonVariables.queryStringBuffer.append( wordTypeString );
				}

			searchItem = searchItem.nextWordTypeItem();
			}
		}

	private WordTypeItem wordTypeString( boolean isAllowingNounDifference, boolean isCheckingAllLanguages, short wordTypeNr, WordTypeItem searchItem )
		{
		while( searchItem != null )
			{
			if( wordTypeNr == Constants.WORD_TYPE_UNDEFINED ||
			searchItem.wordTypeNr() == wordTypeNr ||

			( isAllowingNounDifference &&
			searchItem.isSingularOrPluralNounWordType() &&
			myWordItem().isSingularOrPluralNoun( searchItem.wordTypeNr() ) ) )
				return searchItem;

			if( foundWordTypeItem_ == null )
				foundWordTypeItem_ = searchItem;

			searchItem = ( isCheckingAllLanguages ? searchItem.nextWordTypeItem() : searchItem.nextCurrentLanguageWordTypeItem() );
			}

		return null;
		}

	private WordTypeItem firstActiveWordTypeItem()
		{
		return (WordTypeItem)firstActiveItem();
		}

	private WordTypeItem firstReplacedWordTypeItem()
		{
		return (WordTypeItem)firstReplacedItem();
		}

	private WordTypeItem firstDeletedWordTypeItem()
		{
		return (WordTypeItem)firstDeletedItem();
		}

	private WordTypeItem firstActiveCurrentLanguageWordTypeItem()
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		WordTypeItem searchItem = firstActiveWordTypeItem();

		while( searchItem != null &&
		searchItem.wordTypeLanguageNr() < currentLanguageNr )
			searchItem = searchItem.nextWordTypeItem();

		return ( searchItem != null && searchItem.wordTypeLanguageNr() == currentLanguageNr ? searchItem : null );
		}

	private WordTypeItem firstReplacedCurrentLanguageWordTypeItem()
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		WordTypeItem searchItem = firstReplacedWordTypeItem();

		while( searchItem != null &&
		searchItem.wordTypeLanguageNr() < currentLanguageNr )
			searchItem = searchItem.nextWordTypeItem();

		return ( searchItem != null && searchItem.wordTypeLanguageNr() == currentLanguageNr ? searchItem : null );
		}

	private WordTypeItem firstDeletedCurrentLanguageWordTypeItem()
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		WordTypeItem searchItem = firstDeletedWordTypeItem();

		while( searchItem != null &&
		searchItem.wordTypeLanguageNr() < currentLanguageNr )
			searchItem = searchItem.nextWordTypeItem();

		return ( searchItem != null && searchItem.wordTypeLanguageNr() == currentLanguageNr ? searchItem : null );
		}


	// Constructor / deconstructor

	protected WordTypeList( WordItem myWordItem )
		{
		foundWordTypeItem_ = null;

		initializeListVariables( Constants.WORD_TYPE_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void showWords( boolean isReturnQueryToPosition )
		{
		showWords( isReturnQueryToPosition, firstActiveWordTypeItem() );
		showWords( isReturnQueryToPosition, firstReplacedWordTypeItem() );
		showWords( isReturnQueryToPosition, firstDeletedWordTypeItem() );
		}

	protected void clearGeneralizationWriteLevel( boolean isLanguageWord, short currentWriteLevel )
		{
		WordTypeItem searchItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );

		while( searchItem != null )
			{
			searchItem.clearGeneralizationWriteLevel( currentWriteLevel );
			searchItem = ( isLanguageWord ? searchItem.nextWordTypeItem() : searchItem.nextCurrentLanguageWordTypeItem() );
			}
		}

	protected void clearSpecificationWriteLevel( short currentWriteLevel )
		{
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			searchItem.clearSpecificationWriteLevel( currentWriteLevel );
			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}
		}

	protected void clearRelationWriteLevel( short currentWriteLevel )
		{
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			searchItem.clearRelationWriteLevel( currentWriteLevel );
			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}
		}

	protected boolean hasFeminineProperNameEnding()
		{
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			if( searchItem.hasFeminineProperNameEnding() )
				return true;

			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	protected boolean hasMasculineProperNameEnding()
		{
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			if( searchItem.hasMasculineProperNameEnding() )
				return true;

			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	protected boolean isCorrectHiddenWordType( short wordTypeNr, String compareString, String authorizationKey )
		{
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			if( searchItem.wordTypeNr() == wordTypeNr &&
			searchItem.isCorrectHiddenWordType( compareString, authorizationKey ) )
				return true;

			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	protected byte checkWordTypesOnFeminineParameters()
		{
		short interfaceParameter;
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			interfaceParameter = Constants.NO_INTERFACE_PARAMETER;

			if( searchItem.hasFeminineDefiniteArticleParameter() &&
			searchItem.hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchItem.hasFeminineIndefiniteArticleParameter() &&
				searchItem.hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > Constants.NO_INTERFACE_PARAMETER )
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an interface notification" );
				}

			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordTypesOnMasculineParameters()
		{
		short interfaceParameter;
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			interfaceParameter = Constants.NO_INTERFACE_PARAMETER;

			if( searchItem.hasMasculineDefiniteArticleParameter() &&
			searchItem.hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchItem.hasMasculineIndefiniteArticleParameter() &&
				searchItem.hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > Constants.NO_INTERFACE_PARAMETER )
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an interface notification" );
				}

			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteWordType( short wordTypeNr )
		{
		boolean hasFoundWordType = false;
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null &&
		!hasFoundWordType )
			{
			if( searchItem.wordTypeNr() == wordTypeNr )
				{
				if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
					hasFoundWordType = true;
				else
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete an active item" );
				}
			else
				searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( 1, null, myWordItem().anyWordTypeString(), "I coundn't find the given word type" );

		return Constants.RESULT_OK;
		}

	protected byte hideWordTypeItem( short wordTypeNr, String authorizationKey )
		{
		boolean hasFoundWordType = false;
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != null &&
		!hasFoundWordType )
			{
			if( searchItem.wordTypeNr() == wordTypeNr )
				{
				if( searchItem.hideWordType( authorizationKey ) == Constants.RESULT_OK )
					hasFoundWordType = true;
				else
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to hide a word type" );
				}
			else
				searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( 1, null, myWordItem().anyWordTypeString(), "I coundn't find the given word type" );

		return Constants.RESULT_OK;
		}

	protected byte markGeneralizationWordTypeAsWritten( boolean isLanguageWord, short wordTypeNr )
		{
		boolean hasFoundWordTypeNr = false;
		boolean isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem pluralNounWordTypeItem = null;
		WordTypeItem singularNounWordTypeItem = null;
		WordTypeItem searchItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );

		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != null &&
			!isWordTypeAlreadyMarkedAsWritten )
				{
				if( searchItem.isGeneralizationWordAlreadyWritten() )
					isWordTypeAlreadyMarkedAsWritten = true;
				else
					{
					if( searchItem.isSingularNoun() )
						singularNounWordTypeItem = searchItem;
					else
						{
						if( searchItem.isPluralNoun() )
							pluralNounWordTypeItem = searchItem;
						}

					if( searchItem.wordTypeNr() == wordTypeNr )
						{
						if( searchItem.markGeneralizationWordTypeAsWritten() == Constants.RESULT_OK )
							hasFoundWordTypeNr = true;
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a word as written" );
						}
					}

				searchItem = ( isLanguageWord ? searchItem.nextWordTypeItem() : searchItem.nextCurrentLanguageWordTypeItem() );
				}

			if( !isWordTypeAlreadyMarkedAsWritten )
				{
				if( hasFoundWordTypeNr )
					{
					// If singular noun - also set plural noun
					if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
					pluralNounWordTypeItem != null )
						{
						if( pluralNounWordTypeItem.markGeneralizationWordTypeAsWritten() != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a plural noun word as written" );
						}
					else
						{
						// If plural noun - also set singular noun
						if( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						singularNounWordTypeItem != null )
							{
							if( singularNounWordTypeItem.markGeneralizationWordTypeAsWritten() != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a singular noun word as written" );
							}
						}
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find the given word type number: " + wordTypeNr );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is already marked as written: " + wordTypeNr );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is undefined or out of bounds: " + wordTypeNr );

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationWordTypeAsWritten( short wordTypeNr )
		{
		boolean hasFoundWordTypeNr = false;
		boolean isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem pluralNounWordTypeItem = null;
		WordTypeItem singularNounWordTypeItem = null;
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != null &&
			!isWordTypeAlreadyMarkedAsWritten )
				{
				if( searchItem.isSpecificationWordAlreadyWritten() )
					isWordTypeAlreadyMarkedAsWritten = true;
				else
					{
					if( searchItem.isSingularNoun() )
						singularNounWordTypeItem = searchItem;
					else
						{
						if( searchItem.isPluralNoun() )
							pluralNounWordTypeItem = searchItem;
						}

					if( searchItem.wordTypeNr() == wordTypeNr )
						{
						if( searchItem.markSpecificationWordTypeAsWritten() == Constants.RESULT_OK )
							hasFoundWordTypeNr = true;
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a word as written" );
						}
					}

				searchItem = searchItem.nextCurrentLanguageWordTypeItem();
				}

			if( !isWordTypeAlreadyMarkedAsWritten )
				{
				if( hasFoundWordTypeNr )
					{
					// If singular noun - also set plural noun
					if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
					pluralNounWordTypeItem != null )
						{
						if( pluralNounWordTypeItem.markSpecificationWordTypeAsWritten() != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a plural noun word as written" );
						}
					else
						{
						// If plural noun - also set singular noun
						if( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						singularNounWordTypeItem != null )
							{
							if( singularNounWordTypeItem.markSpecificationWordTypeAsWritten() != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a singular noun word as written" );
							}
						}
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find the given word type number: " + wordTypeNr );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is already marked as written: " + wordTypeNr );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is undefined or out of bounds: " + wordTypeNr );

		return Constants.RESULT_OK;
		}

	protected byte markRelationWordTypeAsWritten( short wordTypeNr )
		{
		boolean hasFoundWordTypeNr = false;
		boolean isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem pluralNounWordTypeItem = null;
		WordTypeItem singularNounWordTypeItem = null;
		WordTypeItem searchItem = firstActiveCurrentLanguageWordTypeItem();

		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != null &&
			!isWordTypeAlreadyMarkedAsWritten )
				{
				if( !searchItem.isRelationWordAlreadyWritten() )
					{
					if( searchItem.isSingularNoun() )
						singularNounWordTypeItem = searchItem;
					else
						{
						if( searchItem.isPluralNoun() )
							pluralNounWordTypeItem = searchItem;
						}

					if( searchItem.wordTypeNr() == wordTypeNr )
						{
						if( searchItem.markRelationWordTypeAsWritten() == Constants.RESULT_OK )
							hasFoundWordTypeNr = true;
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a word as written" );
						}
					}

				searchItem = searchItem.nextCurrentLanguageWordTypeItem();
				}

			if( !isWordTypeAlreadyMarkedAsWritten )
				{
				if( hasFoundWordTypeNr )
					{
					// If singular noun - also set plural noun
					if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
					pluralNounWordTypeItem != null )
						{
						if( pluralNounWordTypeItem.markRelationWordTypeAsWritten() != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a plural noun word as written" );
						}
					else
						{
						// If plural noun - also set singular noun
						if( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						singularNounWordTypeItem != null )
							{
							if( singularNounWordTypeItem.markRelationWordTypeAsWritten() != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark a singular noun word as written" );
							}
						}
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find the given word type number: " + wordTypeNr );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is already marked as written: " + wordTypeNr );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is undefined or out of bounds: " + wordTypeNr );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		WordTypeItem searchItem = firstActiveWordTypeItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeWordTypeItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store a word type item in the database" );
				}

			searchItem = searchItem.nextWordTypeItem();
			}

		searchItem = firstReplacedWordTypeItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeWordTypeItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced word type item in the database" );
				}

			searchItem = searchItem.nextWordTypeItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected ReferenceResultType findMatchingWordReferenceString( String searchString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();
		WordTypeItem searchItem = firstActiveWordTypeItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		!referenceResult.hasFoundMatchingStrings &&
		searchItem != null )
			{
			if( searchItem.itemString() != null )
				{
				if( ( referenceResult = compareStrings( searchString, searchItem.itemString() ) ).result == Constants.RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						CommonVariables.matchingWordTypeNr = searchItem.wordTypeNr();
					}
				else
					addError( 1, null, myWordItem().anyWordTypeString(), "I failed to compare an active word type string with the query string" );
				}

			searchItem = searchItem.nextWordTypeItem();
			}

		searchItem = firstReplacedWordTypeItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		!referenceResult.hasFoundMatchingStrings &&
		searchItem != null )
			{
			if( searchItem.itemString() != null )
				{
				if( ( referenceResult = compareStrings( searchString, searchItem.itemString() ) ).result == Constants.RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						CommonVariables.matchingWordTypeNr = searchItem.wordTypeNr();
					}
				else
					addError( 1, null, myWordItem().anyWordTypeString(), "I failed to compare a deleted word type string with the query string" );
				}

			searchItem = searchItem.nextWordTypeItem();
			}

		searchItem = firstDeletedWordTypeItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		!referenceResult.hasFoundMatchingStrings &&
		searchItem != null )
			{
			if( searchItem.itemString() != null )
				{
				if( ( referenceResult = compareStrings( searchString, searchItem.itemString() ) ).result == Constants.RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						CommonVariables.matchingWordTypeNr = searchItem.wordTypeNr();
					}
				else
					addError( 1, null, myWordItem().anyWordTypeString(), "I failed to compare a deleted word type string with the query string" );
				}

			searchItem = searchItem.nextWordTypeItem();
			}

		referenceResult.result = CommonVariables.result;
		return referenceResult;
		}

	protected WordResultType createWordTypeItem( boolean hasFeminineWordEnding, boolean hasMasculineWordEnding, boolean isProperNamePrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeNr, int wordLength, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();

		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( wordTypeString != null )
				{
				if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
					{
					if( ( wordResult.createdWordTypeItem = new WordTypeItem( hasFeminineWordEnding, hasMasculineWordEnding, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, CommonVariables.currentLanguageNr, wordTypeNr, wordLength, wordTypeString, this, myWordItem() ) ) != null )
						{
						if( addItemToList( Constants.QUERY_ACTIVE_CHAR, wordResult.createdWordTypeItem ) != Constants.RESULT_OK )
							addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active word type item" );
						}
					else
						startError( 1, null, myWordItem().anyWordTypeString(), "The created word type item is undefined" );
					}
				else
					startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
				}
			else
				startError( 1, null, myWordItem().anyWordTypeString(), "The given wordTypeString is undefined" );
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}

	protected String wordTypeString( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		foundWordTypeItem_ = null;

		// Try to find word type of the current language
		if( ( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == null &&
		( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstReplacedCurrentLanguageWordTypeItem() ) ) == null &&
		( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstDeletedCurrentLanguageWordTypeItem() ) ) == null )
			{
			// Not found in current language. Now, try all languages
			if( isCheckingAllLanguages &&
			( foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() ) ) == null )
				{
				if( ( foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstReplacedWordTypeItem() ) ) == null )
					foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstDeletedWordTypeItem() );
				}
			}

		return ( foundWordTypeItem == null ? ( foundWordTypeItem_ == null ? null : foundWordTypeItem_.itemString() ) : foundWordTypeItem.itemString() );
		}

	protected String activeWordTypeString( boolean isLanguageWord, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		if( ( foundWordTypeItem = activeWordTypeItem( false, isLanguageWord, wordTypeNr ) ) != null )
			return foundWordTypeItem.itemString();

		return null;
		}

	protected WordTypeItem activeWordTypeItem( boolean isAllowingNounDifference, boolean isCheckingAllLanguages, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		// Try to find word type of the current language
		if( ( foundWordTypeItem = wordTypeString( isAllowingNounDifference, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == null )
			{
			// Not found in current language. Now, try all languages
			if( isCheckingAllLanguages )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() );
			}

		return foundWordTypeItem;
		}
	};

/*************************************************************************
 *	"He lifts the poor from the dust
 *	and needy from the garbage dump.
 *	He sets them among princes,
 *	even princes of his own people!" (Psalm 113:7-8)
 *************************************************************************/
