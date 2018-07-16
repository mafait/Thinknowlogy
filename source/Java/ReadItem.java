/*	Class:			ReadItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about the read words of a sentence
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

class ReadItem extends Item
	{
	// Private initialized variables

	private boolean isUncountableGeneralizationNoun_;

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

	protected ReadItem( boolean isUncountableGeneralizationNoun, short wordOrderNr, short wordParameter, short wordTypeNr, int readStringLength, String _readString, WordItem readWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		isUncountableGeneralizationNoun_ = isUncountableGeneralizationNoun;

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
		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( readString != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( readString );
			}
		}

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( readWordItem_ != null &&
		( wordString = readWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( wordString );
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

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

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

	protected boolean isAdjectiveAssigned()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	protected boolean isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_EMPTY );
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

	protected boolean isArticle()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_ARTICLE );
		}

	protected boolean isChineseReversedImperativeNoun()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_HEAD ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_NUMBER ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_TAIL ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_VALUE );
		}

	protected boolean isConjunction()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_CONJUNCTION );
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

	protected boolean isFrenchPreposition()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_FRENCH_A );
		}

	protected boolean isGeneralizationWord()
		{
		return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_WORD );
		}

	protected boolean isImperativeNoun()
		{
		return ( wordParameter_ == Constants.NO_WORD_PARAMETER ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_HEAD ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_LANGUAGE ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_MIND ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_NUMBER ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_TAIL ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_USER );
		}

	protected boolean isMatchingReadWordTypeNr( short wordTypeNr )
		{
		return isMatchingWordType( wordTypeNr_, wordTypeNr );
		}

	protected boolean isNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isNonChineseNumeral()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NUMERAL &&
				wordParameter_ != Constants.WORD_PARAMETER_NUMERAL_CHINESE_ALL );
		}

	protected boolean isNumeralBoth()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NUMERAL_BOTH );
		}

	protected boolean isNegative()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_ADJECTIVE_NO ||
				wordParameter_ == Constants.WORD_PARAMETER_ADVERB_NOT ||
				wordParameter_ == Constants.WORD_PARAMETER_ADVERB_FRENCH_PAS );
		}

	protected boolean isNounHeadOrTail()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_HEAD ||
				wordParameter_ == Constants.WORD_PARAMETER_NOUN_TAIL );
		}

	protected boolean isNounValue()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_VALUE );
		}

	protected boolean isNounValueAhead()
		{
		ReadItem lookingAheadReadItem;

		return ( ( lookingAheadReadItem = this.nextReadItem() ) != null &&
				( lookingAheadReadItem = lookingAheadReadItem.nextReadItem() ) != null &&
				lookingAheadReadItem.isNounValue() );
		}

	protected boolean isNounPartOf()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_NOUN_PART );
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

	protected boolean isProperNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_PROPER_NOUN );
		}

	protected boolean isQuestionMark()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK );
		}

	protected boolean isRelationWord()
		{
		// To avoid triggering on the article before a proper noun preceded by defined article
		return ( wordTypeNr_ != Constants.WORD_TYPE_ARTICLE &&
				grammarParameter == Constants.GRAMMAR_RELATION_WORD );
		}

	protected boolean isSeparator()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_COMMA ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_COLON ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK ||
				wordParameter_ == Constants.WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_QUESTION_MARK );
		}

	protected boolean isSingularNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isSkippingChineseIntegrityCheckWords()
		{
		switch( wordTypeNr_ )
			{
			case Constants.WORD_TYPE_SYMBOL:
			case Constants.WORD_TYPE_PREPOSITION:
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_SPECIFICATION );

			case Constants.WORD_TYPE_NUMERAL:
				return ( grammarParameter == Constants.GRAMMAR_SPECIFICATION_WORD );

			case Constants.WORD_TYPE_ARTICLE:
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_PART );

			case Constants.WORD_TYPE_CONJUNCTION:
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_PART ||
						grammarParameter == Constants.GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION );

			case Constants.WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE:
				return ( grammarParameter == Constants.GRAMMAR_RELATION_PART );
			}

		return false;
		}

	protected boolean isSkippingIntegrityCheckWords()
		{
		switch( wordTypeNr_ )
			{
			case Constants.WORD_TYPE_SYMBOL:
				// Skip extra comma in sentence that isn't written.
				// See grammar file for: '( symbolComma )'
				// Example: "A creature is an animal, fungus, human-being, micro-organism, or plant."
				return ( grammarParameter == Constants.GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION ||
						grammarParameter == Constants.GRAMMAR_SENTENCE_CONJUNCTION );

			case Constants.WORD_TYPE_NUMERAL:
				// Skip on different order of specification words with a numeral
				// Example: "John has 2 sons and a daughter"
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_SPECIFICATION );

			case Constants.WORD_TYPE_ADJECTIVE:
						// Typical for Spanish
				return ( grammarParameter == Constants.GRAMMAR_SPECIFICATION_WORD ||
						// Typical for French
						grammarParameter == Constants.GRAMMAR_RELATION_PART );

			case Constants.WORD_TYPE_ADVERB:
				// Skip mismatch in uncertainty
				return ( grammarParameter == Constants.GRAMMAR_VERB );

			case Constants.WORD_TYPE_ARTICLE:
						// Skip missing indefinite article, because of a plural noun
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_SPECIFICATION ||
						// Skip wrong indefinite article
						// Example: "An horse is incorrect."
						grammarParameter == Constants.GRAMMAR_GENERALIZATION_PART ||
						// Typical for Dutch
						// Skip wrong definite article
						// Example: "De paard is onjuist."
						grammarParameter == Constants.GRAMMAR_GENERALIZATION_ASSIGNMENT );

			case Constants.WORD_TYPE_CONJUNCTION:
						// Skip question entered with singular verb, but written with plural verb
						// Example: "Expert is a user and his password is expert123."
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_SPECIFICATION ||
						// Skip linked conjunctions
						// Example: "Guest is a user and has no password."
						grammarParameter == Constants.GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION ||
						// Skip sentence conjunctions
						// Example: "Expert is a user and his password is expert123."
						grammarParameter == Constants.GRAMMAR_SENTENCE_CONJUNCTION );

			case Constants.WORD_TYPE_NOUN_SINGULAR:
				return ( grammarParameter == Constants.GRAMMAR_GENERALIZATION_WORD ||
						grammarParameter == Constants.GRAMMAR_SPECIFICATION_WORD );

			case Constants.WORD_TYPE_NOUN_PLURAL:
				// Skip plural noun if singular noun is entered
				// Example: entered: "Is Joe a child?", written: "Are Paul and Joe children?"
				return ( grammarParameter == Constants.GRAMMAR_SPECIFICATION_WORD );

			case Constants.WORD_TYPE_VERB_SINGULAR:
				return ( grammarParameter == Constants.GRAMMAR_VERB );

			case Constants.WORD_TYPE_VERB_PLURAL:
				// Skip plural question verb if singular verb is entered
				// Example: entered: "Is Joe a child?", written: "Are Paul and Joe children?"
				return ( grammarParameter == Constants.GRAMMAR_QUESTION_VERB );
			}

		return false;
		}

	protected boolean isSpecificationWord()
		{
		return ( grammarParameter == Constants.GRAMMAR_SPECIFICATION_WORD );
		}

	protected boolean isSymbol()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_SYMBOL );
		}

	protected boolean isUncountableGeneralizationNoun()
		{
		return isUncountableGeneralizationNoun_;
		}

	protected boolean isVerb()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_VERB_SINGULAR ||
				wordTypeNr_ == Constants.WORD_TYPE_VERB_PLURAL );
		}

	protected boolean isVirtualListPreposition()
		{
		return ( wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_FROM ||
				wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_TO ||
				wordParameter_ == Constants.WORD_PARAMETER_PREPOSITION_OF );
		}

	protected boolean isText()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_TEXT );
		}

	protected short readAheadChineseImperativeVerbParameter()
		{
		ReadItem searchReadItem = this;

		if( wordParameter_ == Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CHINESE_PUT )
			{
			while( ( searchReadItem = searchReadItem.nextReadItem() ) != null )
				{
				switch( searchReadItem.wordParameter() )
					{
					case Constants.WORD_PARAMETER_PREPOSITION_CHINESE_VERB_ADD:
						return Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD;

					case Constants.WORD_PARAMETER_PREPOSITION_CHINESE_VERB_MOVE:
						return Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE;

					case Constants.WORD_PARAMETER_PREPOSITION_CHINESE_VERB_REMOVE:
						return Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE;
					}
				}
			}

		return Constants.NO_WORD_PARAMETER;
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

	protected WordItem lookAheadRelationWordItem()
		{
		ReadItem searchReadItem = this;

		while( searchReadItem != null )
			{
			if( searchReadItem.isRelationWord() )
				return searchReadItem.readWordItem();

			searchReadItem = searchReadItem.nextReadItem();
			}

		return null;
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
