/*
 *	Class:			WordTypeItem
 *	Parent class:	Item
 *	Purpose:		To store the word types of a word
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

class WordTypeItem extends Item
	{
	// Private loadable variables

	private boolean hasFeminineWordEnding_;
	private boolean hasMasculineWordEnding_;
	private boolean isProperNamePrecededByDefiniteArticle_;

	private short adjectiveParameter_;
	private short definiteArticleParameter_;
	private short indefiniteArticleParameter_;
	private short wordTypeLanguageNr_;
	private short wordTypeNr_;


	// Private constructible variables

	private short generalizationWriteLevel_;
	private short specificationWriteLevel_;
	private short relationWriteLevel_;

	private String wordTypeString_;

	private	String hideKey_;


	// Private methods

	private boolean hasIndefinitePhoneticVowelArticle( short indefiniteArticleParameter )
		{
		short phoneticVowelIndefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem phoneticVowelIndefiniteArticleWordItem;

		switch( indefiniteArticleParameter )
			{
			// Phonetic vowel article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE:
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE:
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE:
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE:
				return true;

			// Plural article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE:
				phoneticVowelIndefiniteArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE:
				phoneticVowelIndefiniteArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE;
				break;

			// Singular article
			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE:
				phoneticVowelIndefiniteArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE;
				break;

			case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE:
				phoneticVowelIndefiniteArticleParameter = Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE;
				break;
			}

		if( phoneticVowelIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
		( phoneticVowelIndefiniteArticleWordItem = myWordItem().predefinedWordItem( phoneticVowelIndefiniteArticleParameter ) ) != null )
			return phoneticVowelIndefiniteArticleWordItem.hasWordType( Constants.WORD_TYPE_ARTICLE );

		return false;
		}

	private static boolean isIndefiniteArticlePhoneticVowelParameter( short wordParameter )
		{
		return ( wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE );
		}

	private WordResultType setAdjectiveParameter( short adjectiveParameter )
		{
		WordResultType wordResult = new WordResultType();

		if( isAdjectiveParameter( adjectiveParameter ) )
			{
			if( isSingularNoun() )
				{
				if( adjectiveParameter_ == Constants.NO_ADJECTIVE_PARAMETER )
					adjectiveParameter_ = adjectiveParameter;
				else
					{
					if( adjectiveParameter_ != adjectiveParameter )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_WITH_NOUN_START, itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == Constants.RESULT_OK )
							wordResult.hasFoundDifferentParameter = true;
						else
							addErrorInItem( 1, null, itemString(), "I failed to write an interface notification" );
						}
					}
				}
			else
				startErrorInItem( 1, null, itemString(), "I am not a singular noun" );
			}
		else
			startErrorInItem( 1, null, itemString(), "The given adjective parameter is not an adjective parameter" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}

	private WordResultType setDefiniteArticleParameter( short definiteArticleParameter )
		{
		WordResultType wordResult = new WordResultType();

		if( isDefiniteArticleParameter( definiteArticleParameter ) )
			{
			if( isSingularNoun() )
				{
				if( definiteArticleParameter_ == Constants.NO_DEFINITE_ARTICLE_PARAMETER )
					definiteArticleParameter_ = definiteArticleParameter;
				else
					{
					if( definiteArticleParameter_ != definiteArticleParameter )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_DEFINITE_ARTICLE_WITH_NOUN_START, itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == Constants.RESULT_OK )
							wordResult.hasFoundDifferentParameter = true;
						else
							addErrorInItem( 1, null, itemString(), "I failed to write an interface notification" );
						}
					}
				}
			else
				startErrorInItem( 1, null, itemString(), "I am not a singular noun" );
			}
		else
			startErrorInItem( 1, null, itemString(), "The given definite article parameter is not a definite article parameter" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}

	private WordResultType setIndefiniteArticleParameter( short indefiniteArticleParameter )
		{
		WordResultType wordResult = new WordResultType();

		if( isIndefiniteArticleParameter( indefiniteArticleParameter ) )
			{
			if( isSingularNoun() )
				{
				if( indefiniteArticleParameter_ == Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
					indefiniteArticleParameter_ = indefiniteArticleParameter;
				else
					{
					if( indefiniteArticleParameter_ != indefiniteArticleParameter )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START, itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == Constants.RESULT_OK )
							wordResult.hasFoundDifferentParameter = true;
						else
							addErrorInItem( 1, null, itemString(), "I failed to write an interface notification" );
						}
					}
				}
			else
				startErrorInItem( 1, null, itemString(), "I am not a singular noun" );
			}
		else
			startErrorInItem( 1, null, itemString(), "The given indefinite article parameter is not an indefinite article parameter" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}


	// Constructor / deconstructor

	protected WordTypeItem( boolean hasFeminineWordEnding, boolean hasMasculineWordEnding, boolean isProperNamePrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeLanguageNr, short wordTypeNr, int wordTypeStringLength, String _wordTypeString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		hasFeminineWordEnding_ = hasFeminineWordEnding;
		hasMasculineWordEnding_ = hasMasculineWordEnding;
		isProperNamePrecededByDefiniteArticle_ = isProperNamePrecededByDefiniteArticle;

		adjectiveParameter_ = adjectiveParameter;
		definiteArticleParameter_ = definiteArticleParameter;
		indefiniteArticleParameter_ = indefiniteArticleParameter;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Private constructible variables

		generalizationWriteLevel_ = Constants.NO_WRITE_LEVEL;
		specificationWriteLevel_ = Constants.NO_WRITE_LEVEL;
		relationWriteLevel_ = Constants.NO_WRITE_LEVEL;

		hideKey_ = null;
		wordTypeString_ = null;

		if( _wordTypeString != null )
			{
			if( _wordTypeString.length() > 0 )
				{
				if( wordTypeStringLength > 0 )
					wordTypeString_ = ( Character.isUpperCase( _wordTypeString.charAt( 0 ) ) && wordTypeNr != Constants.WORD_TYPE_LETTER_CAPITAL && wordTypeNr != Constants.WORD_TYPE_PROPER_NAME ? Character.toLowerCase( _wordTypeString.charAt( 0 ) ) + _wordTypeString.substring( 1, wordTypeStringLength ) : _wordTypeString.substring( 0, wordTypeStringLength ) );
				else
					startSystemErrorInItem( 1, null, null, "The given word type string length is undefined" );
				}
			else
				startSystemErrorInItem( 1, null, null, "The given word type string is empty" );
			}
		else
			startSystemErrorInItem( 1, null, null, "The given word type string is undefined" );
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( itemString() != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( itemString() );
			}
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( adjectiveParameter_ == queryParameter ||
				definiteArticleParameter_ == queryParameter ||
				indefiniteArticleParameter_ == queryParameter ||
				generalizationWriteLevel_ == queryParameter ||
				specificationWriteLevel_ == queryParameter ||
				relationWriteLevel_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( adjectiveParameter_ > Constants.NO_ADJECTIVE_PARAMETER ||
				definiteArticleParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER ||
				indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER ||
				generalizationWriteLevel_ > Constants.NO_WRITE_LEVEL ||
				specificationWriteLevel_ > Constants.NO_WRITE_LEVEL ||
				relationWriteLevel_ > Constants.NO_WRITE_LEVEL ) ) );
		}

	protected boolean hasFoundWordType( short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		return ( nextSortItem != null &&
				// Ascending wordTypeLanguageNr_
				wordTypeLanguageNr_ < ( (WordTypeItem)nextSortItem ).wordTypeLanguageNr_ );
		}

	protected String itemString()
		{
		return ( hideKey_ == null ? wordTypeString_ : null );
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		String wordString;
		String wordTypeString = myWordItem().wordTypeNameString( wordTypeNr_ );
		String languageNameString = myWordItem().languageNameString( wordTypeLanguageNr_ );

		baseToStringBuffer( queryWordTypeNr );

		if( wordTypeLanguageNr_ > Constants.NO_LANGUAGE_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordTypeLanguageNr:" + ( languageNameString == null ? wordTypeLanguageNr_ : languageNameString ) );

		if( hideKey_ != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isHidden" );

		if( hasFeminineWordEnding_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasFeminineWordEnding" );

		if( hasMasculineWordEnding_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasMasculineWordEnding" );

		if( isProperNamePrecededByDefiniteArticle_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isProperNamePrecededByDefiniteArticle" );

		if( adjectiveParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "adjectiveParameter:" + adjectiveParameter_ );

		if( definiteArticleParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definiteArticleParameter:" + definiteArticleParameter_ );

		if( indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "indefiniteArticleParameter:" + indefiniteArticleParameter_ );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordType:" + ( wordTypeString == null ? Constants.EMPTY_STRING : wordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + wordTypeNr_ );

		if( ( wordString = itemString() ) != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordString:" + Constants.QUERY_STRING_START_CHAR + wordString + Constants.QUERY_STRING_END_CHAR );

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected void clearGeneralizationWriteLevel( short currentWriteLevel )
		{
		if( generalizationWriteLevel_ > currentWriteLevel )
			generalizationWriteLevel_ = Constants.NO_WRITE_LEVEL;
		}

	protected void clearSpecificationWriteLevel( short currentWriteLevel )
		{
		if( specificationWriteLevel_ > currentWriteLevel )
			specificationWriteLevel_ = Constants.NO_WRITE_LEVEL;
		}

	protected void clearRelationWriteLevel( short currentWriteLevel )
		{
		if( relationWriteLevel_ > currentWriteLevel )
			relationWriteLevel_ = Constants.NO_WRITE_LEVEL;
		}

	protected boolean hasFeminineProperNameEnding()
		{
		return ( hasFeminineWordEnding_ &&
				wordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean hasMasculineProperNameEnding()
		{
		return ( hasMasculineWordEnding_ &&
				wordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean hasFeminineDefiniteArticleParameter()
		{
		return isFeminineArticleParameter( definiteArticleParameter_ );
		}

	protected boolean hasFeminineIndefiniteArticleParameter()
		{
		return isFeminineArticleParameter( indefiniteArticleParameter_ );
		}

	protected boolean hasMasculineDefiniteArticleParameter()
		{
		return isMasculineArticleParameter( definiteArticleParameter_ );
		}

	protected boolean hasMasculineIndefiniteArticleParameter()
		{
		return isMasculineArticleParameter( indefiniteArticleParameter_ );
		}

	protected boolean hasFeminineAndMasculineDefiniteArticle()
		{
		return myWordItem().hasFeminineAndMasculineArticle( definiteArticleParameter_ );
		}

	protected boolean hasFeminineAndMasculineIndefiniteArticle()
		{
		return myWordItem().hasFeminineAndMasculineArticle( indefiniteArticleParameter_ );
		}

	protected boolean isCorrectAdjective( short adjectiveParameter )
		{
		return ( adjectiveParameter_ == Constants.NO_ADJECTIVE_PARAMETER ||
				adjectiveParameter_ == adjectiveParameter );
		}

	protected boolean isCorrectDefiniteArticle( short definiteArticleParameter )
		{
		return ( definiteArticleParameter_ == Constants.NO_DEFINITE_ARTICLE_PARAMETER ||
				definiteArticleParameter_ == definiteArticleParameter );
		}

	protected boolean isCorrectIndefiniteArticle( boolean isCheckingForEqualParameters, short indefiniteArticleParameter )
		{
		boolean isStringStartingWithVowel;
		boolean isVowelIndefiniteArticle;
		boolean hasIndefiniteArticleParameter = ( indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER );

		if( isCheckingForEqualParameters &&
		indefiniteArticleParameter_ == indefiniteArticleParameter )
			return true;

		// If undefined, fall back to a simple phonetic vowel rule
		if( hasIndefinitePhoneticVowelArticle( hasIndefiniteArticleParameter ? indefiniteArticleParameter_ : indefiniteArticleParameter ) )
			{
			isStringStartingWithVowel = isStartingWithPhoneticVowel( itemString() );
			isVowelIndefiniteArticle = isIndefiniteArticlePhoneticVowelParameter( indefiniteArticleParameter );

			return ( ( !isStringStartingWithVowel &&	// 'a'
					!isVowelIndefiniteArticle ) ||

					( isStringStartingWithVowel &&		// 'an'
					isVowelIndefiniteArticle ) );
			}

		if( hasIndefiniteArticleParameter )
			return !isCheckingForEqualParameters;

		return ( ( definiteArticleParameter_ != Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				definiteArticleParameter_ != Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE ) ||

				( ( definiteArticleParameter_ == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				indefiniteArticleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ) ||

				( definiteArticleParameter_ == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE &&
				indefiniteArticleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) ) );
		}

	protected boolean isProperNamePrecededByDefiniteArticle( short definiteArticleParameter )
		{
		return ( isProperNamePrecededByDefiniteArticle_ &&
				isCorrectDefiniteArticle( definiteArticleParameter ) );
		}

	protected boolean isCorrectHiddenWordType( String compareString, String hideKey )
		{
		if( hideKey_ == hideKey &&
		compareString != null &&
		wordTypeString_.equals( compareString ) )
			return true;

		return false;
		}

	protected boolean isAdjective()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ADJECTIVE );
		}

	protected boolean isArticle()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ARTICLE );
		}

	protected boolean isConjunction()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_CONJUNCTION );
		}

	protected boolean isDefiniteArticle()
		{
		// Filter on articles, because nouns also have a definiteArticleParameter
		return ( wordTypeNr_ == Constants.WORD_TYPE_ARTICLE &&
				isDefiniteArticleParameter( definiteArticleParameter_ ) );
		}

	protected boolean isIndefiniteArticle()
		{
		// Filter on articles, because nouns also have an indefiniteArticleParameter
		return ( wordTypeNr_ == Constants.WORD_TYPE_ARTICLE &&
				isIndefiniteArticleParameter( indefiniteArticleParameter_ ) );
		}

	protected boolean isSingularOrPluralNounWordType()
		{
		return isSingularOrPluralNoun( wordTypeNr_ );
		}

	protected boolean isSingularNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isPluralNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isPossessiveDeterminer()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL );
		}

	protected boolean isPossessivePronoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL );
		}

	protected boolean isPreposition()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_PREPOSITION );
		}

	protected boolean isSymbol()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_SYMBOL );
		}

	protected boolean isNumeral()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isVerb()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_VERB_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_VERB_PLURAL );
		}

	protected boolean isGeneralizationWordAlreadyWritten()
		{
		return ( generalizationWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isSpecificationWordAlreadyWritten()
		{
		return ( specificationWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isRelationWordAlreadyWritten()
		{
		return ( relationWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected short adjectiveParameter()
		{
		return adjectiveParameter_;
		}

	protected short definiteArticleParameter()
		{
		return definiteArticleParameter_;
		}

	protected short indefiniteArticleParameter()
		{
		return indefiniteArticleParameter_;
		}

	protected short wordTypeNr()
		{
		return wordTypeNr_;
		}

	protected short wordTypeLanguageNr()
		{
		return wordTypeLanguageNr_;
		}

	protected byte createNewWordTypeString( String newWordTypeString )
		{
		if( newWordTypeString != null )
			{
			if( newWordTypeString.length() > 0 )
				wordTypeString_ = newWordTypeString;
			else
				return startErrorInItem( 1, null, itemString(), "The given new word type string is empty" );
			}
		else
			return startErrorInItem( 1, null, itemString(), "The given new word type string is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte markGeneralizationWordTypeAsWritten()
		{
		if( CommonVariables.currentWriteLevel < Constants.MAX_LEVEL )
			{
			if( generalizationWriteLevel_ == Constants.NO_WRITE_LEVEL )
				generalizationWriteLevel_ = ++CommonVariables.currentWriteLevel;
			else
				return startErrorInItem( 1, null, itemString(), "My write level is already assigned" );
			}
		else
			return startSystemErrorInItem( 1, null, itemString(), "Current write word level overflow" );

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationWordTypeAsWritten()
		{
		if( CommonVariables.currentWriteLevel < Constants.MAX_LEVEL )
			{
			if( specificationWriteLevel_ == Constants.NO_WRITE_LEVEL )
				specificationWriteLevel_ = ++CommonVariables.currentWriteLevel;
			else
				return startErrorInItem( 1, null, itemString(), "My write level is already assigned" );
			}
		else
			return startSystemErrorInItem( 1, null, itemString(), "Current write word level overflow" );

		return Constants.RESULT_OK;
		}

	protected byte markRelationWordTypeAsWritten()
		{
		if( CommonVariables.currentWriteLevel < Constants.MAX_LEVEL )
			{
			if( relationWriteLevel_ == Constants.NO_WRITE_LEVEL )
				relationWriteLevel_ = ++CommonVariables.currentWriteLevel;
			else
				return startErrorInItem( 1, null, itemString(), "My write level is already assigned" );
			}
		else
			return startSystemErrorInItem( 1, null, itemString(), "Current write word level overflow" );

		return Constants.RESULT_OK;
		}

	protected byte hideWordType( String hideKey )
		{
		if( hideKey_ == null )
			hideKey_ = hideKey;
		else
			return startErrorInItem( 1, null, itemString(), "This word type is already hidden" );

		return Constants.RESULT_OK;
		}

	protected WordResultType setParameter( short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter )
		{
		WordResultType wordResult = new WordResultType();

		if( adjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER )
			{
			if( ( wordResult = setAdjectiveParameter( adjectiveParameter ) ).result != Constants.RESULT_OK )
				addErrorInItem( 1, null, itemString(), "I failed to set my adjective parameter" );
			}
		else
			{
			if( definiteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER )
				{
				if( ( wordResult = setDefiniteArticleParameter( definiteArticleParameter ) ).result != Constants.RESULT_OK )
					addErrorInItem( 1, null, itemString(), "I failed to set my definite article parameter" );
				}
			else
				{
				if( indefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
					{
					if( ( wordResult = setIndefiniteArticleParameter( indefiniteArticleParameter ) ).result != Constants.RESULT_OK )
						addErrorInItem( 1, null, itemString(), "I failed to set the indefinite article parameter of a singular noun" );
					}
				}
			}

		return wordResult;
		}

	protected WordTypeItem nextWordTypeItem()
		{
		return (WordTypeItem)nextItem;
		}

	protected WordTypeItem nextCurrentLanguageWordTypeItem()
		{
		WordTypeItem nextCurrentLanguageWordTypeItem = nextWordTypeItem();

		return ( nextCurrentLanguageWordTypeItem != null &&
				nextCurrentLanguageWordTypeItem.wordTypeLanguageNr() == CommonVariables.currentLanguageNr ? nextCurrentLanguageWordTypeItem : null );
		}

	protected WordTypeItem nextWordTypeItem( short wordTypeNr )
		{
		WordTypeItem searchItem = nextCurrentLanguageWordTypeItem();

		while( searchItem != null )
			{
			if( wordTypeNr == Constants.WORD_TYPE_UNDEFINED ||
			searchItem.wordTypeNr_ == wordTypeNr )
				return searchItem;

			searchItem = searchItem.nextCurrentLanguageWordTypeItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"The Lord gives his people strength.
 *	The Lord blesses them with peace." (Psalm 29:11)
 *************************************************************************/
