/*	Class:			ReadItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about the read words of a sentence
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

class ReadItem extends Item
	{
	// Private initialized variables

	private short wordOrderNr_;
	private short wordParameter_;
	private short wordTypeNr_;

	private WordItem readWordItem_;


	// Protected constructed variables

	protected boolean hasWordPassedIntegrityCheckOfStoredUserSentence;
	protected boolean isMarkedBySetGrammarParameter;

	protected short grammarParameter;

	protected GrammarItem definitionGrammarItem;


	// Protected initialized variables

	protected String readString;


	// Constructor

	protected ReadItem( short wordOrderNr, short wordParameter, short wordTypeNr, int readStringLength, String _readString, WordItem readWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );


		// Private initialized variables

		wordOrderNr_ = wordOrderNr;
		wordParameter_ = wordParameter;
		wordTypeNr_ = wordTypeNr;

		readWordItem_ = readWordItem;


		// Protected constructed variables

		hasWordPassedIntegrityCheckOfStoredUserSentence = false;
		isMarkedBySetGrammarParameter = false;

		grammarParameter = Constants.NO_GRAMMAR_PARAMETER;

		definitionGrammarItem = null;

		// Protected initialized variables

		readString = ( _readString == null ? null : _readString.substring( 0, readStringLength ) );
		}


	// Protected virtual methods

	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( readString != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( readString );
			}
		}

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( readWordItem_ != null &&
		( wordString = readWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( grammarParameter == queryParameter ||
				wordOrderNr_ == queryParameter ||
				wordParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( grammarParameter > Constants.NO_GRAMMAR_PARAMETER ||
				wordOrderNr_ > Constants.NO_ORDER_NR ||
				wordParameter_ > Constants.NO_WORD_PARAMETER ) ) );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( readWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : readWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : readWordItem_.itemNr() == queryItemNr ) ) ||

				( definitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : definitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : definitionGrammarItem.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		ReadItem nextSortReadItem = (ReadItem)nextSortItem;

		// Remark: All read items should have the same creationSentenceNr
		return ( nextSortItem != null &&

				// 1) Ascending wordOrderNr_
				( wordOrderNr_ < nextSortReadItem.wordOrderNr_ ||

				// 2) Descending wordTypeNr_
				( wordOrderNr_ == nextSortReadItem.wordOrderNr_ &&
				wordTypeNr_ > nextSortReadItem.wordTypeNr_ ) ) );
		}

	protected String itemString()
		{
		return readString;
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String wordString;
		String wordTypeString = myWordItem().wordTypeNameString( wordTypeNr_ );

		itemBaseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( hasWordPassedIntegrityCheckOfStoredUserSentence )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasWordPassedIntegrityCheckOfStoredUserSentence" );

		if( isMarkedBySetGrammarParameter )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMarkedBySetGrammarParameter" );

		if( wordOrderNr_ > Constants.NO_ORDER_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordOrderNr:" + wordOrderNr_ );

		if( wordParameter_ > Constants.NO_WORD_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordParameter:" + wordParameter_ );

		if( grammarParameter > Constants.NO_GRAMMAR_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarParameter:" + grammarParameter );

		if( readString != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "readString:" + Constants.QUERY_STRING_START_CHAR + readString + Constants.QUERY_STRING_END_CHAR );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordType:" + ( wordTypeString == null ? Constants.EMPTY_STRING : wordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + wordTypeNr_ );

		if( readWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "readWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + readWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + readWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = readWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( definitionGrammarItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + definitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + definitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
		}

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		if( readWordItem_ != null )
			return readWordItem_.findMatchingWordReferenceString( queryString );

		return new BoolResultType();
		}


	// Protected methods

	protected boolean hasFoundRelationWordInThisList( WordItem relationWordItem )
		{
		ReadItem searchReadItem = this;

		if( relationWordItem != null )
			{
			while( searchReadItem != null )
				{
				if( searchReadItem.isRelationWord() &&
				searchReadItem.readWordItem() == relationWordItem )
					return true;

				searchReadItem = searchReadItem.nextReadItem();
				}
			}

		return false;
		}

	protected boolean isSymbol()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_SYMBOL );
		}

	protected boolean isNumeral()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isAdjective()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ADJECTIVE );
		}

	protected boolean isArticle()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ARTICLE );
		}

	protected boolean isNoun()
		{
		return isNounWordType( wordTypeNr_ );
		}

	protected boolean isSingularNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isMatchingReadWordTypeNr( short wordTypeNr )
		{
		return isMatchingWordType( wordTypeNr_, wordTypeNr );
		}

	protected boolean isDeterminerOrPronoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_SUBJECTIVE ||
				wordTypeNr_ == Constants.WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_OBJECTIVE ||

				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ||

				wordTypeNr_ == Constants.WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE ||
				wordTypeNr_ == Constants.WORD_TYPE_PERSONAL_PRONOUN_PLURAL_OBJECTIVE ||

				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL ||
				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL );
		}

	protected boolean isPossessiveDeterminer()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL );
		}

	protected boolean isPreposition()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_PREPOSITION );
		}

	protected boolean isProperName()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME );
		}

	protected boolean isVerb()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_VERB_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_VERB_PLURAL );
		}

	protected boolean isPluralQuestionVerb()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_VERB_PLURAL &&
				grammarParameter == Constants.GRAMMAR_QUESTION_VERB );
		}

	protected boolean isText()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_TEXT );
		}

	protected boolean isNumeralBoth()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NUMERAL_BOTH );
		}

	protected boolean isAdjectiveAssigned()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	protected boolean isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	protected boolean isAdjectiveCalledSingularFeminineOrMasculine()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE );
		}

	protected boolean isAdjectiveEvery()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE );
		}

	protected boolean isAdjectivePrevious()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE );
		}

	protected boolean isNegative()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_NO ||
				wordParameter_ == Constants.WORD_PARAMETER_ADVERB_NOT ||
				wordParameter_ == Constants.WORD_PARAMETER_ADVERB_NOT_FRENCH );
		}

	protected boolean isNounJustificationReport()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT );
		}

	protected boolean isNounInformation()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_INFORMATION );
		}

	protected boolean isNounValue()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_VALUE );
		}

	protected boolean isNounFile()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_FILE ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_TEST_FILE );
		}

	protected boolean isNounPartOf()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_PART );
		}

	protected boolean isSeparator()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_COMMA ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_COLON ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED );
		}

	protected boolean isVirtualListPreposition()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_FROM ||
				wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_TO ||
				wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_OF );
		}

	protected boolean isFrenchPreposition()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_FRENCH_A );
		}

	protected boolean isUserDefined()
		{
		return ( wordParameter_ == Constants.NO_WORD_PARAMETER );
		}

	protected boolean isGeneralizationWord()
		{
		return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_WORD );
		}

	protected boolean isSpecificationWord()
		{
		return ( grammarParameter == Constants.GRAMMAR_SPECIFICATION_WORD );
		}

	protected boolean isRelationWord()
		{
		// To avoid triggering on the article before a proper name preceded by defined article
		return ( wordTypeNr_ != Constants.WORD_TYPE_ARTICLE &&
				grammarParameter == Constants.GRAMMAR_RELATION_WORD );
		}

	protected boolean isLinkedGeneralizationConjunction()
		{
		return ( grammarParameter == Constants.GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION );
		}

	protected boolean isSentenceConjunction()
		{
		return ( grammarParameter == Constants.GRAMMAR_SENTENCE_CONJUNCTION );
		}

	protected short wordOrderNr()
		{
		return wordOrderNr_;
		}

	protected short wordParameter()
		{
		return wordParameter_;
		}

	protected short wordTypeNr()
		{
		return wordTypeNr_;
		}

	protected byte changeReadWord( short newWordTypeNr, WordItem newReadWordItem )
		{
		if( newReadWordItem == null )
			return startError( 1, null, "The given new read word item is undefined" );

		wordTypeNr_ = newWordTypeNr;
		readWordItem_ = newReadWordItem;

		return Constants.RESULT_OK;
		}

	protected String readWordTypeString()
		{
		if( readWordItem_ != null )
			return readWordItem_.activeWordTypeString( wordTypeNr_ );

		return null;
		}

	protected ReadItem firstRelationWordReadItem()
		{
		ReadItem searchReadItem = this;

		while( searchReadItem != null )
			{
			if( searchReadItem.isRelationWord() )
				return searchReadItem;

			searchReadItem = searchReadItem.nextReadItem();
			}

		return null;
		}

	protected ReadItem nextReadItem()
		{
		return (ReadItem)nextItem;
		}

	protected WordItem readWordItem()
		{
		return readWordItem_;
		}

	protected WordTypeItem activeReadWordTypeItem()
		{
		return ( readWordItem_ == null ? null : readWordItem_.activeWordTypeItem( true, wordTypeNr_ ) );
		}
	};

/*************************************************************************
 *	"The godly will see these things and be glad,
 *	while the wicked are struck in silent." (Psalm 107:42)
 *************************************************************************/
