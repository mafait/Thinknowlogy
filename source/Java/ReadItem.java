/*
 *	Class:			ReadItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about the read words of a sentence
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

class ReadItem extends Item
	{
	// Private loadable variables

	private short wordOrderNr_;
	private short wordParameter_;
	private short wordTypeNr_;

	private WordItem readWordItem_;


	// Protected constructible variables

	protected boolean hasWordPassedIntegrityCheckOfStoredUserSentence;
	protected boolean isMarkedBySetGrammarParameter;
	protected boolean isUnusedReadItem;

	protected short grammarParameter;

	protected GrammarItem definitionGrammarItem;


	// Protected loadable variables

	protected String readString;


	// Constructor / deconstructor

	protected ReadItem( short wordOrderNr, short wordParameter, short wordTypeNr, int readStringLength, String _readString, WordItem readWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );


		// Private loadable variables

		wordOrderNr_ = wordOrderNr;
		wordParameter_ = wordParameter;
		wordTypeNr_ = wordTypeNr;

		readWordItem_ = readWordItem;


		// Protected constructible variables

		hasWordPassedIntegrityCheckOfStoredUserSentence = false;
		isMarkedBySetGrammarParameter = false;
		isUnusedReadItem = false;

		grammarParameter = Constants.NO_GRAMMAR_PARAMETER;

		definitionGrammarItem = null;


		// Protected loadable variables

		if( _readString != null )
			readString = _readString.substring( 0, readStringLength );
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( readString != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( readString );
			}
		}

	protected void showWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( readWordItem_ != null &&
		( wordString = readWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( grammarParameter == queryParameter ||
				wordOrderNr_ == queryParameter ||
				wordParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( grammarParameter > Constants.NO_GRAMMAR_PARAMETER ||
				wordOrderNr_ > Constants.NO_ORDER_NR ||
				wordParameter_ > Constants.NO_WORD_PARAMETER ) ) );
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( readWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : readWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : readWordItem_.itemNr() == queryItemNr ) ) ||

				( definitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : definitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : definitionGrammarItem.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasFoundWordType( short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	protected boolean isSorted( Item nextSortItem )
		{
		ReadItem nextSortReadItem = (ReadItem)nextSortItem;

		return ( nextSortItem != null &&
				// Remark: All read items should have the same creationSentenceNr

				// 1) Ascending wordOrderNr_
				( wordOrderNr_ < nextSortReadItem.wordOrderNr_ ||

				// 2) Descending wordTypeNr_
				( wordOrderNr_ == nextSortReadItem.wordOrderNr_ &&
				wordTypeNr_ > nextSortReadItem.wordTypeNr_ ) ) );
		}

	protected ReferenceResultType findMatchingWordReferenceString( String queryString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( readWordItem_ != null )
			{
			if( ( referenceResult = readWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to find the word reference for the word reference query" );
			}

		return referenceResult;
		}

	protected ReferenceResultType findWordReference( WordItem referenceWordItem )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( referenceWordItem != null )
			{
			if( readWordItem_ == referenceWordItem )
				referenceResult.hasFoundWordReference = true;
			}
		else
			startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given reference word is undefined" );

		return referenceResult;
		}

	protected String itemString()
		{
		return readString;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		String wordString;
		String wordTypeString = myWordItem().wordTypeNameString( wordTypeNr_ );

		baseToStringBuffer( queryWordTypeNr );

		if( isUnusedReadItem )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUnusedReadItem" );

		if( hasWordPassedIntegrityCheckOfStoredUserSentence )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasWordPassedIntegrityCheckOfStoredUserSentence" );

		if( isMarkedBySetGrammarParameter )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isMarkedBySetGrammarParameter" );

		if( wordOrderNr_ > Constants.NO_ORDER_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordOrderNr:" + wordOrderNr_ );

		if( wordParameter_ > Constants.NO_WORD_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordParameter:" + wordParameter_ );

		if( grammarParameter > Constants.NO_GRAMMAR_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarParameter:" + grammarParameter );

		if( readString != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "readString:" + Constants.QUERY_STRING_START_CHAR + readString + Constants.QUERY_STRING_END_CHAR );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordType:" + ( wordTypeString == null ? Constants.EMPTY_STRING : wordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + wordTypeNr_ );

		if( readWordItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "readWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + readWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + readWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = readWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
				CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( definitionGrammarItem != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + definitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + definitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected boolean hasFoundRelationWordInThisList( WordItem relationWordItem )
		{
		ReadItem searchItem = this;

		if( relationWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isRelationWord() &&
				searchItem.readWordItem() == relationWordItem )
					return true;

				searchItem = searchItem.nextReadItem();
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

	protected boolean isArticle()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ARTICLE );
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

	protected boolean isAdjectiveAssignedOrClear()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_CLEAR ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED );
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

	protected boolean isVirtualListPreposition()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_FROM ||
				wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_TO ||
				wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_OF );
		}

	protected boolean isNegative()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_NO ||
				wordParameter_ == Constants.WORD_PARAMETER_ADVERB_NOT );
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
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_FILE );
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

	protected boolean isUserDefined()
		{
		return ( wordParameter_ == Constants.NO_WORD_PARAMETER );
		}

	protected boolean isSelection()
		{
		return ( grammarParameter == Constants.GRAMMAR_SELECTION );
		}

	protected boolean isImperative()
		{
		return ( grammarParameter == Constants.GRAMMAR_IMPERATIVE );
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
		// To avoid triggering on the article before a proper name preceded-by-defined-article
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

		if( newReadWordItem != null )
			{
			wordTypeNr_ = newWordTypeNr;
			readWordItem_ = newReadWordItem;
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given new read word item is undefined" );

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
		ReadItem searchItem = this;

		while( searchItem != null )
			{
			if( searchItem.isRelationWord() )
				return searchItem;

			searchItem = searchItem.nextReadItem();
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
