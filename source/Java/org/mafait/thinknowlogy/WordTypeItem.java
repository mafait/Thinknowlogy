/*	Class:			WordTypeItem
 *	Parent class:	Item
 *	Purpose:		To store the word types of a word
 *	Version:		Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

class WordTypeItem extends Item
	{
	// Private initialized variables

	private boolean hasFeminineWordEnding_ = false;
	private boolean hasMasculineWordEnding_ = false;
	private boolean isProperNounPrecededByDefiniteArticle_ = false;

	private short adjectiveParameter_ = Constants.NO_ADJECTIVE_PARAMETER;
	private short definiteArticleParameter_ = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
	private short indefiniteArticleParameter_ = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
	private short wordTypeLanguageNr_ = Constants.NO_LANGUAGE_NR;
	private short wordTypeNr_ = Constants.NO_WORD_TYPE_NR;


	// Private constructed variables

	private short generalizationWriteLevel_ = Constants.NO_WRITE_LEVEL;
	private short specificationWriteLevel_ = Constants.NO_WRITE_LEVEL;
	private short relationWriteLevel_ = Constants.NO_WRITE_LEVEL;

	private String hideKey_ = null;
	private String wordTypeString_ = null;


	// Private methods

	private boolean isIndefinitePhoneticVowelArticle( short indefiniteArticleParameter )
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

		return ( phoneticVowelIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
				( phoneticVowelIndefiniteArticleWordItem = myWordItem().predefinedWordItem( phoneticVowelIndefiniteArticleParameter ) ) != null ?
					phoneticVowelIndefiniteArticleWordItem.hasWordType( false, Constants.WORD_TYPE_ARTICLE ) : false );
		}

	private static boolean isIndefiniteArticlePhoneticVowelParameter( short wordParameter )
		{
		return ( wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				wordParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE );
		}


	// Constructor

	protected WordTypeItem( boolean hasFeminineWordEnding, boolean hasMasculineWordEnding, boolean isProperNounPrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeLanguageNr, short wordTypeNr, int wordTypeStringLength, String wordTypeString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		hasFeminineWordEnding_ = hasFeminineWordEnding;
		hasMasculineWordEnding_ = hasMasculineWordEnding;
		isProperNounPrecededByDefiniteArticle_ = isProperNounPrecededByDefiniteArticle;

		adjectiveParameter_ = adjectiveParameter;
		definiteArticleParameter_ = definiteArticleParameter;
		indefiniteArticleParameter_ = indefiniteArticleParameter;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Checking private initialized variables

		if( wordTypeString != null )
			{
			if( wordTypeString.length() > 0 )
				{
				if( wordTypeStringLength > 0 )
					wordTypeString_ = ( Character.isUpperCase( wordTypeString.charAt( 0 ) ) &&
										wordTypeNr != Constants.WORD_TYPE_LETTER_CAPITAL &&
										wordTypeNr != Constants.WORD_TYPE_PROPER_NOUN ? Character.toLowerCase( wordTypeString.charAt( 0 ) ) + wordTypeString.substring( 1, wordTypeStringLength ) : wordTypeString.substring( 0, wordTypeStringLength ) );
				else
					startSystemError( 1, null, null, "The given word type string length is undefined" );
				}
			else
				startSystemError( 1, null, null, "The given word type string is empty" );
			}
		else
			startSystemError( 1, null, null, "The given word type string is undefined" );
		}


	// Protected virtual methods

	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( itemString() != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( itemString() );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( adjectiveParameter_ == queryParameter ||
				definiteArticleParameter_ == queryParameter ||
				indefiniteArticleParameter_ == queryParameter ||
				wordTypeLanguageNr_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( adjectiveParameter_ > Constants.NO_ADJECTIVE_PARAMETER ||
				definiteArticleParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER ||
				indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER ||
				wordTypeLanguageNr_ > Constants.NO_LANGUAGE_NR ) ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
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

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		WordItem thisWordItem = myWordItem();
		StringBuffer queryStringBuffer;
		String wordString;
		String wordTypeString = thisWordItem.wordTypeNameString( wordTypeNr_ );
		String languageNameString = thisWordItem.languageNameString( wordTypeLanguageNr_ );

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( wordTypeLanguageNr_ > Constants.NO_LANGUAGE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordTypeLanguageNr:" + ( languageNameString == null ? wordTypeLanguageNr_ : languageNameString ) );

		if( hideKey_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isHidden" );

		if( hasFeminineWordEnding_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasFeminineWordEnding" );

		if( hasMasculineWordEnding_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasMasculineWordEnding" );

		if( isProperNounPrecededByDefiniteArticle_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isProperNounPrecededByDefiniteArticle" );

		if( adjectiveParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "adjectiveParameter:" + adjectiveParameter_ );

		if( definiteArticleParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definiteArticleParameter:" + definiteArticleParameter_ );

		if( indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "indefiniteArticleParameter:" + indefiniteArticleParameter_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordType:" + ( wordTypeString == null ? Constants.EMPTY_STRING : wordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + wordTypeNr_ );

		if( ( wordString = itemString() ) != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordString:" + Constants.QUERY_STRING_START_CHAR + wordString + Constants.QUERY_STRING_END_CHAR );

		return queryStringBuffer;
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

	protected void clearIndefiniteArticleParameter()
		{
		indefiniteArticleParameter_ = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		}

	protected boolean hasAdjectiveParameter()
		{
		return ( adjectiveParameter_ > Constants.NO_ADJECTIVE_PARAMETER );
		}

	protected boolean hasDefiniteArticleParameter()
		{
		return ( definiteArticleParameter_ > Constants.NO_DEFINITE_ARTICLE_PARAMETER );
		}

	protected boolean hasIndefiniteArticleParameter()
		{
		return ( indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER );
		}

	protected boolean hasFeminineWordEnding()
		{
		return hasFeminineWordEnding_;
		}

	protected boolean hasFeminineOrMasculineWordEnding()
		{
		return ( hasFeminineWordEnding_ ||
				hasMasculineWordEnding_ );
		}

	protected boolean hasMasculineWordEnding()
		{
		return hasMasculineWordEnding_;
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
		boolean hasIndefiniteArticleParameter = ( indefiniteArticleParameter_ > Constants.NO_INDEFINITE_ARTICLE_PARAMETER );
		boolean isStringStartingWithVowel;
		boolean isVowelIndefiniteArticle;

		if( isCheckingForEqualParameters &&
		indefiniteArticleParameter_ == indefiniteArticleParameter )
			return true;

		// Typical for English ('a' or 'an')
		// If undefined, fall back to a simple phonetic vowel rule
		if( isIndefinitePhoneticVowelArticle( hasIndefiniteArticleParameter ? indefiniteArticleParameter_ : indefiniteArticleParameter ) )
			{
			isStringStartingWithVowel = doesStringStartWithPhoneticVowel( itemString() );
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

				( definiteArticleParameter_ == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				indefiniteArticleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ) ||

				( definiteArticleParameter_ == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE &&
				indefiniteArticleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) );
		}

	protected boolean isCorrectHiddenWordType( short wordTypeNr, String compareString, String hideKey )
		{
		return ( wordTypeNr_ == wordTypeNr &&
				hideKey_ == hideKey &&
				compareString != null &&
				wordTypeString_.equals( compareString ) );
		}

	protected boolean isAnswer()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ANSWER );
		}

	protected boolean isDefiniteArticle()
		{
		return isDefiniteArticleParameter( definiteArticleParameter_ );
		}

	protected boolean isNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isProperNounPrecededByDefiniteArticle( short definiteArticleParameter )
		{
		return ( isProperNounPrecededByDefiniteArticle_ &&
				isCorrectDefiniteArticle( definiteArticleParameter ) );
		}

	protected boolean isSingularNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}
/*
	protected boolean isGeneralizationWordAlreadyWritten()
		{
		return ( generalizationWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}
*/
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

	protected byte markGeneralizationWordTypeAsWritten()
		{
		if( GlobalVariables.currentWriteLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, null, itemString(), "Current write word level overflow" );

		if( generalizationWriteLevel_ > Constants.NO_WRITE_LEVEL )
			return startError( 1, null, itemString(), "My write level is already assigned" );

		generalizationWriteLevel_ = ++GlobalVariables.currentWriteLevel;

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationWordTypeAsWritten()
		{
		if( GlobalVariables.currentWriteLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, null, itemString(), "Current write word level overflow" );

		if( specificationWriteLevel_ > Constants.NO_WRITE_LEVEL )
			return startError( 1, null, itemString(), "My write level is already assigned" );

		specificationWriteLevel_ = ++GlobalVariables.currentWriteLevel;

		return Constants.RESULT_OK;
		}

	protected byte markRelationWordTypeAsWritten()
		{
		if( GlobalVariables.currentWriteLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, null, itemString(), "Current write word level overflow" );

		if( relationWriteLevel_ > Constants.NO_WRITE_LEVEL )
			return startError( 1, null, itemString(), "My write level is already assigned" );

		relationWriteLevel_ = ++GlobalVariables.currentWriteLevel;

		return Constants.RESULT_OK;
		}

	protected byte hideWordType( String hideKey )
		{
		if( hideKey_ != null )
			return startError( 1, null, itemString(), "This word type is already hidden" );

		hideKey_ = hideKey;

		return Constants.RESULT_OK;
		}

	protected byte setAdjectiveParameter( short adjectiveParameter )
		{
		if( !isSingularNoun() )
			return startError( 1, null, itemString(), "I am not a singular noun" );

		if( !isAdjectiveParameter( adjectiveParameter ) )
			return startError( 1, null, itemString(), "The given adjective parameter is no adjective parameter" );

		if( adjectiveParameter_ == Constants.NO_ADJECTIVE_PARAMETER )
			adjectiveParameter_ = adjectiveParameter;
		else
			{
			if( adjectiveParameter_ != adjectiveParameter &&
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_WITH_NOUN_START, itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
				return addError( 1, null, itemString(), "I failed to write an interface notification about the use of a different adjective" );
			}

		return Constants.RESULT_OK;
		}

	protected byte setDefiniteArticleParameter( short definiteArticleParameter )
		{
		if( !isSingularNoun() )
			return startError( 1, null, itemString(), "I am not a singular noun" );

		if( !isDefiniteArticleParameter( definiteArticleParameter ) )
			return startError( 1, null, itemString(), "The given definite article parameter is no definite article parameter" );

		if( definiteArticleParameter_ == Constants.NO_DEFINITE_ARTICLE_PARAMETER )
			definiteArticleParameter_ = definiteArticleParameter;
		else
			{
			if( definiteArticleParameter_ != definiteArticleParameter &&
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START, itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
				return addError( 1, null, itemString(), "I failed to write an interface notification about the use of a different defnite article" );
			}

		return Constants.RESULT_OK;
		}

	protected byte setIndefiniteArticleParameter( short indefiniteArticleParameter )
		{
		if( !isSingularNoun() )
			return startError( 1, null, itemString(), "I am not a singular noun" );

		if( !isIndefiniteArticleParameter( indefiniteArticleParameter ) )
			return startError( 1, null, itemString(), "The given indefinite article parameter is no indefinite article parameter" );

		if( indefiniteArticleParameter_ == Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
			indefiniteArticleParameter_ = indefiniteArticleParameter;
		else
			{
			if( indefiniteArticleParameter_ != indefiniteArticleParameter &&

			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
				return addError( 1, null, itemString(), "I failed to write an interface notification about the use of an indefinite article" );
			}

		return Constants.RESULT_OK;
		}

	protected WordTypeItem nextWordTypeItem()
		{
		return (WordTypeItem)nextItem;
		}

	protected WordTypeItem nextCurrentLanguageWordTypeItem()
		{
		WordTypeItem nextCurrentLanguageWordTypeItem = nextWordTypeItem();

		return ( nextCurrentLanguageWordTypeItem != null &&
				nextCurrentLanguageWordTypeItem.wordTypeLanguageNr() == GlobalVariables.currentLanguageNr ? nextCurrentLanguageWordTypeItem : null );
		}
	};

/*************************************************************************
 *	"The Lord gives his people strength.
 *	The Lord blesses them with peace." (Psalm 29:11)
 *************************************************************************/
