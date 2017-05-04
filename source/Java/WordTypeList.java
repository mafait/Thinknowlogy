/*	Class:			WordTypeList
 *	Parent class:	List
 *	Purpose:		To store word type items
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

class WordTypeList extends List
	{
	// Private constructed variables

	private boolean hasFeminineWordEnding_;
	private boolean hasMasculineWordEnding_;

	private WordTypeItem foundWordTypeItem_;


	// Private methods

	private byte checkOnFeminineAndMasculineWordEnding( boolean isSingularNoun, String wordString )
		{
		WordEndingResultType wordEndingResult = new WordEndingResultType();
		WordItem currentLanguageWordItem;

		if( wordString == null )
			return startError( 1, "The given word string is undefined" );

		if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) == null )
			return startError( 1, "The current language word item is undefined" );

		if( ( wordEndingResult = currentLanguageWordItem.analyzeWordEnding( ( isSingularNoun ? Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING : Constants.WORD_FEMININE_PROPER_NAME_ENDING ), 0, wordString ) ).result != Constants.RESULT_OK )
			return addError( 1, "I failed to check on feminine word ending" );

		if( wordEndingResult.hasFoundWordEnding )
			{
			hasFeminineWordEnding_ = true;

			if( isSingularNoun &&
			myWordItem().markWordAsFeminine() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark my word as feminine" );
			}
		else
			{
			if( ( wordEndingResult = currentLanguageWordItem.analyzeWordEnding( ( isSingularNoun ? Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING : Constants.WORD_MASCULINE_PROPER_NAME_ENDING ), 0, wordString ) ).result != Constants.RESULT_OK )
				return addError( 1, "I failed to check on masculine word ending" );

			if( wordEndingResult.hasFoundWordEnding )
				{
				hasMasculineWordEnding_ = true;

				if( isSingularNoun &&
				myWordItem().markWordAsMasculine() != Constants.RESULT_OK )
					return addError( 1, "I failed to mark my word as masculine" );
				}
			}

		return Constants.RESULT_OK;
		}

	private WordTypeItem nextWordTypeListItem()
		{
		return (WordTypeItem)nextListItem();
		}

	private WordTypeItem wordTypeString( boolean isAllowingDifferentNoun, boolean isCheckingAllLanguages, short wordTypeNr, WordTypeItem searchWordTypeItem )
		{
		while( searchWordTypeItem != null )
			{
			if( wordTypeNr == Constants.NO_WORD_TYPE_NR ||
			searchWordTypeItem.wordTypeNr() == wordTypeNr ||

			( isAllowingDifferentNoun &&
			searchWordTypeItem.isNoun() ) )
				return searchWordTypeItem;

			if( foundWordTypeItem_ == null )
				foundWordTypeItem_ = searchWordTypeItem;

			searchWordTypeItem = ( isCheckingAllLanguages ? searchWordTypeItem.nextWordTypeItem() : searchWordTypeItem.nextCurrentLanguageWordTypeItem() );
			}

		return null;
		}

	private WordTypeItem firstActiveWordTypeItem()
		{
		return (WordTypeItem)firstActiveItem();
		}

	private WordTypeItem firstDeletedWordTypeItem()
		{
		return (WordTypeItem)firstDeletedItem();
		}

	private WordTypeItem firstActiveCurrentLanguageWordTypeItem()
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		WordTypeItem searchWordTypeItem = firstActiveWordTypeItem();

		while( searchWordTypeItem != null &&
		searchWordTypeItem.wordTypeLanguageNr() < currentLanguageNr )
			searchWordTypeItem = searchWordTypeItem.nextWordTypeItem();

		return ( searchWordTypeItem != null && searchWordTypeItem.wordTypeLanguageNr() == currentLanguageNr ? searchWordTypeItem : null );
		}

	private WordTypeItem firstDeletedCurrentLanguageWordTypeItem()
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		WordTypeItem searchWordTypeItem = firstDeletedWordTypeItem();

		while( searchWordTypeItem != null &&
		searchWordTypeItem.wordTypeLanguageNr() < currentLanguageNr )
			searchWordTypeItem = searchWordTypeItem.nextWordTypeItem();

		return ( searchWordTypeItem != null && searchWordTypeItem.wordTypeLanguageNr() == currentLanguageNr ? searchWordTypeItem : null );
		}


	// Constructor

	protected WordTypeList( WordItem myWordItem )
		{
		// Private constructed variables

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		foundWordTypeItem_ = null;

		initializeListVariables( Constants.WORD_TYPE_LIST_SYMBOL, "WordTypeList", myWordItem );
		}


	// Protected methods

	protected void clearGeneralizationWriteLevel( boolean isLanguageWord, short currentWriteLevel )
		{
		WordTypeItem searchWordTypeItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );

		while( searchWordTypeItem != null )
			{
			searchWordTypeItem.clearGeneralizationWriteLevel( currentWriteLevel );
			searchWordTypeItem = ( isLanguageWord ? searchWordTypeItem.nextWordTypeItem() : searchWordTypeItem.nextCurrentLanguageWordTypeItem() );
			}
		}

	protected void clearSpecificationWriteLevel( short currentWriteLevel )
		{
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != null )
			{
			searchWordTypeItem.clearSpecificationWriteLevel( currentWriteLevel );
			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}
		}

	protected void clearRelationWriteLevel( short currentWriteLevel )
		{
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != null )
			{
			searchWordTypeItem.clearRelationWriteLevel( currentWriteLevel );
			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}
		}

	protected boolean hasAnyWordType()
		{
		return ( firstActiveWordTypeItem() != null );
		}

	protected boolean isCorrectHiddenWordType( short wordTypeNr, String compareString, String authorizationKey )
		{
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != null )
			{
			if( searchWordTypeItem.isCorrectHiddenWordType( wordTypeNr, compareString, authorizationKey ) )
				return true;

			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	protected byte checkWordTypesOnFeminineParameters()
		{
		short interfaceParameter;
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != null )
			{
			interfaceParameter = Constants.NO_INTERFACE_PARAMETER;

			if( searchWordTypeItem.hasFeminineDefiniteArticleParameter() &&
			searchWordTypeItem.hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchWordTypeItem.hasFeminineIndefiniteArticleParameter() &&
				searchWordTypeItem.hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > Constants.NO_INTERFACE_PARAMETER &&
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter, searchWordTypeItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
				return addError( 1, "I failed to write an interface notification about the use of an article with a feminine noun" );

			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordTypesOnMasculineParameters()
		{
		short interfaceParameter;
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != null )
			{
			interfaceParameter = Constants.NO_INTERFACE_PARAMETER;

			if( searchWordTypeItem.hasMasculineDefiniteArticleParameter() &&
			searchWordTypeItem.hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchWordTypeItem.hasMasculineIndefiniteArticleParameter() &&
				searchWordTypeItem.hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > Constants.NO_INTERFACE_PARAMETER &&
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter, searchWordTypeItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
				return addError( 1, "I failed to write an interface notification about the use of an article with a masculine noun" );

			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteWordType( WordTypeItem deleteWordTypeItem )
		{
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		if( deleteWordTypeItem == null )
			return startError( 1, "The given delete word type item is undefined" );

		if( deleteItem( searchWordTypeItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to delete an active item" );

		return Constants.RESULT_OK;
		}

	protected byte deleteAllWordTypesOfCurrentSentence()
		{
		WordTypeItem searchWordTypeItem = firstActiveWordTypeItem();

		while( searchWordTypeItem != null )
			{
			if( searchWordTypeItem.hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( searchWordTypeItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to delete a word type item of this sentence" );

				searchWordTypeItem = nextWordTypeListItem();
				}
			else
				searchWordTypeItem = searchWordTypeItem.nextWordTypeItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte hideWordTypeItem( short wordTypeNr, String authorizationKey )
		{
		boolean hasWordType = false;
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != null &&
		!hasWordType )
			{
			if( searchWordTypeItem.wordTypeNr() == wordTypeNr )
				{
				if( searchWordTypeItem.hideWordType( authorizationKey ) != Constants.RESULT_OK )
					return addError( 1, "I failed to hide a word type" );

				hasWordType = true;
				}
			else
				searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}

		if( !hasWordType )
			return startError( 1, "I coundn't find the given word type" );

		return Constants.RESULT_OK;
		}

	protected byte markGeneralizationWordTypeAsWritten( boolean isLanguageWord, short wordTypeNr )
		{
		boolean hasWordTypeNr = false;
		boolean isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem pluralNounWordTypeItem = null;
		WordTypeItem singularNounWordTypeItem = null;
		WordTypeItem searchWordTypeItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given word type number is undefined or out of bounds: " + wordTypeNr );

		while( searchWordTypeItem != null &&
		!isWordTypeAlreadyMarkedAsWritten )
			{
			if( searchWordTypeItem.isGeneralizationWordAlreadyWritten() )
				isWordTypeAlreadyMarkedAsWritten = true;
			else
				{
				if( searchWordTypeItem.isSingularNoun() )
					singularNounWordTypeItem = searchWordTypeItem;
				else
					{
					if( searchWordTypeItem.isPluralNoun() )
						pluralNounWordTypeItem = searchWordTypeItem;
					}

				if( searchWordTypeItem.wordTypeNr() == wordTypeNr )
					{
					if( searchWordTypeItem.markGeneralizationWordTypeAsWritten() != Constants.RESULT_OK )
						return addError( 1, "I failed to mark a word as written" );

					hasWordTypeNr = true;
					}
				}

			searchWordTypeItem = ( isLanguageWord ? searchWordTypeItem.nextWordTypeItem() : searchWordTypeItem.nextCurrentLanguageWordTypeItem() );
			}

		if( isWordTypeAlreadyMarkedAsWritten )
			return startError( 1, "The given word type number is already marked as written: " + wordTypeNr );

		if( !hasWordTypeNr )
			return startError( 1, "I couldn't find the given word type number: " + wordTypeNr );

		// If singular noun - also set plural noun
		if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
		pluralNounWordTypeItem != null )
			{
			if( pluralNounWordTypeItem.markGeneralizationWordTypeAsWritten() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark a plural noun word as written" );
			}
		else
			{
			// If plural noun - also set singular noun
			if( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
			singularNounWordTypeItem != null &&
			singularNounWordTypeItem.markGeneralizationWordTypeAsWritten() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark a singular noun word as written" );
			}

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationWordTypeAsWritten( short wordTypeNr )
		{
		boolean hasWordTypeNr = false;
		boolean isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem pluralNounWordTypeItem = null;
		WordTypeItem singularNounWordTypeItem = null;
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given word type number is undefined or out of bounds: " + wordTypeNr );

		while( searchWordTypeItem != null &&
		!isWordTypeAlreadyMarkedAsWritten )
			{
			if( searchWordTypeItem.isSpecificationWordAlreadyWritten() )
				isWordTypeAlreadyMarkedAsWritten = true;
			else
				{
				if( searchWordTypeItem.isSingularNoun() )
					singularNounWordTypeItem = searchWordTypeItem;
				else
					{
					if( searchWordTypeItem.isPluralNoun() )
						pluralNounWordTypeItem = searchWordTypeItem;
					}

				if( searchWordTypeItem.wordTypeNr() == wordTypeNr )
					{
					if( searchWordTypeItem.markSpecificationWordTypeAsWritten() != Constants.RESULT_OK )
						return addError( 1, "I failed to mark a word as written" );

					hasWordTypeNr = true;
					}
				}

			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}

		if( isWordTypeAlreadyMarkedAsWritten )
			return startError( 1, "The given word type number is already marked as written: " + wordTypeNr );

		if( !hasWordTypeNr )
			return startError( 1, "I couldn't find the given word type number: " + wordTypeNr );

		// If singular noun - also set plural noun
		if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
		pluralNounWordTypeItem != null )
			{
			if( pluralNounWordTypeItem.markSpecificationWordTypeAsWritten() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark a plural noun word as written" );
			}
		else
			{
			// If plural noun - also set singular noun
			if( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
			singularNounWordTypeItem != null &&
			singularNounWordTypeItem.markSpecificationWordTypeAsWritten() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark a singular noun word as written" );
			}

		return Constants.RESULT_OK;
		}

	protected byte markRelationWordTypeAsWritten( short wordTypeNr )
		{
		boolean hasWordTypeNr = false;
		boolean isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem pluralNounWordTypeItem = null;
		WordTypeItem singularNounWordTypeItem = null;
		WordTypeItem searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given word type number is undefined or out of bounds: " + wordTypeNr );

		while( searchWordTypeItem != null &&
		!isWordTypeAlreadyMarkedAsWritten )
			{
			if( !searchWordTypeItem.isRelationWordAlreadyWritten() )
				{
				if( searchWordTypeItem.isSingularNoun() )
					singularNounWordTypeItem = searchWordTypeItem;
				else
					{
					if( searchWordTypeItem.isPluralNoun() )
						pluralNounWordTypeItem = searchWordTypeItem;
					}

				if( searchWordTypeItem.wordTypeNr() == wordTypeNr )
					{
					if( searchWordTypeItem.markRelationWordTypeAsWritten() != Constants.RESULT_OK )
						return addError( 1, "I failed to mark a word as written" );

					hasWordTypeNr = true;
					}
				}

			searchWordTypeItem = searchWordTypeItem.nextCurrentLanguageWordTypeItem();
			}

		if( isWordTypeAlreadyMarkedAsWritten )
			return startError( 1, "The given word type number is already marked as written: " + wordTypeNr );

		if( !hasWordTypeNr )
			return startError( 1, "I couldn't find the given word type number: " + wordTypeNr );

		// If singular noun - also set plural noun
		if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
		pluralNounWordTypeItem != null )
			{
			if( pluralNounWordTypeItem.markRelationWordTypeAsWritten() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark a plural noun word as written" );
			}
		else
			{
			// If plural noun - also set singular noun
			if( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
			singularNounWordTypeItem != null &&
			singularNounWordTypeItem.markRelationWordTypeAsWritten() != Constants.RESULT_OK )
				return addError( 1, "I failed to mark a singular noun word as written" );
			}

		return Constants.RESULT_OK;
		}

	protected String wordTypeString( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		WordTypeItem searchWordTypeItem;
		WordTypeItem foundWordTypeItem = null;

		foundWordTypeItem_ = null;

		// Try to find word type of the current language
		if( ( searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem() ) != null )
			foundWordTypeItem = wordTypeString( false, false, wordTypeNr, searchWordTypeItem );

		if( foundWordTypeItem == null &&
		( searchWordTypeItem = firstDeletedCurrentLanguageWordTypeItem() ) != null )
			foundWordTypeItem = wordTypeString( false, false, wordTypeNr, searchWordTypeItem );

		// Not found in current language. Now, try all languages
		if( isCheckingAllLanguages &&
		foundWordTypeItem == null )
			{
			if( ( searchWordTypeItem = firstActiveWordTypeItem() ) != null )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, searchWordTypeItem );

			if( foundWordTypeItem == null &&
			( searchWordTypeItem = firstDeletedWordTypeItem() ) != null )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, searchWordTypeItem );
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

	protected WordTypeItem activeWordTypeItem( boolean isAllowingDifferentNoun, boolean isCheckingAllLanguages, short wordTypeNr )
		{
		WordTypeItem foundWordTypeItem;

		// Try to find word type of the current language
		if( ( foundWordTypeItem = wordTypeString( isAllowingDifferentNoun, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == null )
			{
			// Not found in current language. Now, try all languages
			if( isCheckingAllLanguages )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() );
			}

		return foundWordTypeItem;
		}

	protected BoolResultType findMatchingWordReferenceString( String searchString )
		{
		boolean hasFoundMatchingString = false;
		WordTypeItem searchWordTypeItem = firstActiveWordTypeItem();
		String itemString;
		BoolResultType boolResult = new BoolResultType();

		while( !hasFoundMatchingString &&
		searchWordTypeItem != null )
			{
			if( ( itemString = searchWordTypeItem.itemString() ) != null )
				{
				if( ( boolResult = compareStrings( searchString, itemString ) ).result != Constants.RESULT_OK )
					return addBoolResultError( 1, "I failed to compare an active word type string with the query string" );

				// Matching string
				if( boolResult.booleanValue )
					{
					hasFoundMatchingString = true;
					CommonVariables.matchingWordTypeNr = searchWordTypeItem.wordTypeNr();
					}
				}

			searchWordTypeItem = searchWordTypeItem.nextWordTypeItem();
			}

		searchWordTypeItem = firstDeletedWordTypeItem();

		while( !hasFoundMatchingString &&
		searchWordTypeItem != null )
			{
			if( ( itemString = searchWordTypeItem.itemString() ) != null )
				{
				if( ( boolResult = compareStrings( searchString, itemString ) ).result != Constants.RESULT_OK )
					return addBoolResultError( 1, "I failed to compare a deleted word type string with the query string" );

				// Matching string
				if( boolResult.booleanValue )
					{
					hasFoundMatchingString = true;
					CommonVariables.matchingWordTypeNr = searchWordTypeItem.wordTypeNr();
					}
				}

			searchWordTypeItem = searchWordTypeItem.nextWordTypeItem();
			}

		return boolResult;
		}

	protected WordTypeResultType addWordType( boolean isLanguageWord, boolean isMultipleWord, boolean isProperNamePrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeNr, int wordLength, String wordTypeString )
		{
		WordTypeResultType wordTypeResult = new WordTypeResultType();
		WordResultType wordResult = new WordResultType();
		boolean isSingularNoun;

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startWordTypeResultError( 1, "The given word type number is undefined or out of bounds" );

		if( wordTypeString == null )
			return startWordTypeResultError( 1, "The given wordTypeString is undefined" );

		// Check if word type already exists
		if( ( wordResult = findWordType( false, wordTypeNr, wordTypeString ) ).result != Constants.RESULT_OK )
			return addWordTypeResultError( 1, "I failed to find the given word type" );

		// Add word type if it doesn't exist yet
		if( wordResult.foundWordTypeItem == null )
			{
			isSingularNoun = ( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );

			if( isSingularNoun ||

			( !isLanguageWord &&
			wordTypeNr == Constants.WORD_TYPE_PROPER_NAME ) )
				{
				if( checkOnFeminineAndMasculineWordEnding( isSingularNoun, wordTypeString ) != Constants.RESULT_OK )
					return addWordTypeResultError( 1, "I failed to check the string on feminine or masculine word ending" );
				}

			if( ( wordTypeResult.wordTypeItem = new WordTypeItem( ( hasFeminineWordEnding_ && !isMultipleWord ), hasMasculineWordEnding_, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, CommonVariables.currentLanguageNr, wordTypeNr, wordLength, wordTypeString, this, myWordItem() ) ) == null )
				return startWordTypeResultError( 1, "The created word type item is undefined" );

			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, wordTypeResult.wordTypeItem ) != Constants.RESULT_OK )
				return addWordTypeResultError( 1, "I failed to add an active word type item" );
			}

		return wordTypeResult;
		}

	protected WordResultType findWordType( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();
		int currentWordTypeStringLength;
		int wordTypeStringLength;
		String currentWordTypeString;
		WordTypeItem currentWordTypeItem;

		if( wordTypeString == null )
			return startWordResultError( 1, "The given word type string is undefined" );

		if( ( wordTypeStringLength = wordTypeString.length() ) == 0 )
			return startWordResultError( 1, "The given word type string is empty" );

		if( ( currentWordTypeItem = activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr ) ) != null )
			{
			do	{
				// Skip hidden word type
				if( ( currentWordTypeString = currentWordTypeItem.itemString() ) != null )
					{
					if( ( currentWordTypeStringLength = currentWordTypeString.length() ) == 0 )
						return startWordResultError( 1, "The active word type string is empty" );

					if( wordTypeStringLength == currentWordTypeStringLength &&
					wordTypeString.equals( currentWordTypeString ) )
						{
						wordResult.foundWordItem = myWordItem();
						wordResult.foundWordTypeItem = currentWordTypeItem;
						}
					}
				}
			while( wordResult.foundWordItem == null &&
			( currentWordTypeItem = currentWordTypeItem.nextWordTypeItem( wordTypeNr ) ) != null );
			}

		return wordResult;
		}
	};

/*************************************************************************
 *	"He lifts the poor from the dust
 *	and needy from the garbage dump.
 *	He sets them among princes,
 *	even princes of his own people!" (Psalm 113:7-8)
 *************************************************************************/
