/*
 *	Class:			ReadItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about the read words of a sentence
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

#ifndef READITEM
#define READITEM 1
#include "GrammarItem.cpp"

class ReadItem : private Item
	{
	friend class AdminCleanup;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminReadSentence;
	friend class AdminSpecification;
	friend class AdminWriteSpecification;
	friend class ReadList;

	// Private loadable variables

	unsigned short wordOrderNr_;
	unsigned short wordParameter_;
	unsigned short wordTypeNr_;

	WordItem *readWordItem_;


	protected:
	// Protected constructible variables

	bool hasWordPassedGrammarIntegrityCheck;
	bool isMarkedBySetGrammarParameter;
	bool isUnusedReadItem;

	unsigned short grammarParameter;

	GrammarItem *definitionGrammarItem;


	// Protected loadable variables

	char *readString;


	// Constructor / deconstructor

	ReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *_readString, WordItem *readWordItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "ReadItem", commonVariables, myList, myWordItem );


		// Private loadable variables

		wordOrderNr_ = wordOrderNr;
		wordParameter_ = wordParameter;
		wordTypeNr_ = wordTypeNr;

		readWordItem_ = readWordItem;


		// Protected constructible variables

		hasWordPassedGrammarIntegrityCheck = false;
		isMarkedBySetGrammarParameter = false;
		isUnusedReadItem = false;

		grammarParameter = NO_GRAMMAR_PARAMETER;

		definitionGrammarItem = NULL;


		// Protected loadable variables

		readString = NULL;

		if( _readString != NULL )
			{
			if( readStringLength < MAX_SENTENCE_STRING_LENGTH &&
			strlen( _readString ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( readString = new char[readStringLength + 1] ) != NULL )
					{
					strncpy( readString, _readString, readStringLength );
					readString[readStringLength] = NULL_CHAR;
					}
				else
					startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the read string" );
				}
			else
				startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given read string is too long" );
			}
		}

	~ReadItem()
		{
		if( readString != NULL )
			delete readString;
		}


	// Protected virtual functions

	virtual void showString( bool isReturnQueryToPosition )
		{
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( readString != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status when not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, readString );
			}
		}

	virtual void showWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( readWordItem_ != NULL &&
		( wordString = readWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status when not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( grammarParameter == queryParameter ||
				wordOrderNr_ == queryParameter ||
				wordParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( grammarParameter > NO_GRAMMAR_PARAMETER ||
				wordOrderNr_ > NO_ORDER_NR ||
				wordParameter_ > NO_WORD_PARAMETER ) ) );
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( readWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : readWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : readWordItem_->itemNr() == queryItemNr ) ) ||

				( definitionGrammarItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : definitionGrammarItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : definitionGrammarItem->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		ReadItem *nextSortReadItem = (ReadItem *)nextSortItem;

		return ( nextSortItem != NULL &&
				// Remark: All read items should have the same creationSentenceNr

				// 1) Ascending wordOrderNr_
				( wordOrderNr_ < nextSortReadItem->wordOrderNr_ ||

				// 2) Descending wordTypeNr_
				( wordOrderNr_ == nextSortReadItem->wordOrderNr_ &&
				wordTypeNr_ > nextSortReadItem->wordTypeNr_ ) ) );
		}

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( readWordItem_ != NULL )
			{
			if( ( referenceResult = readWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find the word reference for the word reference query" );
			}

		return referenceResult;
		}

	virtual ReferenceResultType findWordReference( WordItem *referenceWordItem )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordReference";

		if( referenceWordItem != NULL )
			{
			if( readWordItem_ == referenceWordItem )
				referenceResult.hasFoundWordReference = true;
			}
		else
			startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given reference word is undefined" );

		return referenceResult;
		}

	virtual char *itemString()
		{
		return readString;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *wordTypeString = myWordItem()->wordTypeNameString( wordTypeNr_ );

		Item::toString( queryWordTypeNr );

		if( isUnusedReadItem )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isUnusedReadItem" );
			}

		if( hasWordPassedGrammarIntegrityCheck )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "hasWordPassedGrammarIntegrityCheck" );
			}

		if( isMarkedBySetGrammarParameter )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isMarkedBySetGrammarParameter" );
			}

		if( wordOrderNr_ > NO_ORDER_NR )
			{
			sprintf( tempString, "%cwordOrderNr:%u", QUERY_SEPARATOR_CHAR, wordOrderNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( wordParameter_ > NO_WORD_PARAMETER )
			{
			sprintf( tempString, "%cwordParameter:%u", QUERY_SEPARATOR_CHAR, wordParameter_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( grammarParameter > NO_GRAMMAR_PARAMETER )
			{
			sprintf( tempString, "%cgrammarParameter:%u", QUERY_SEPARATOR_CHAR, grammarParameter );
			strcat( commonVariables()->queryString, tempString );
			}

		if( readString != NULL )
			{
			sprintf( tempString, "%creadString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, readString, QUERY_STRING_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( wordTypeString == NULL )
			sprintf( tempString, "%cwordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );
		else
			sprintf( tempString, "%cwordType:%s%c%u", QUERY_SEPARATOR_CHAR, wordTypeString, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( readWordItem_ != NULL )
			{
			sprintf( tempString, "%creadWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, readWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, readWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );

			if( ( wordString = readWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( commonVariables()->queryString, tempString );
				}
			}

		if( definitionGrammarItem != NULL )
			{
			sprintf( tempString, "%cdefinitionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, definitionGrammarItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, definitionGrammarItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	bool hasFoundRelationWordInThisList( WordItem *relationWordItem )
		{
		ReadItem *searchItem = this;

		if( relationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isRelationWord() &&
				searchItem->readWordItem() == relationWordItem )
					return true;

				searchItem = searchItem->nextReadItem();
				}
			}

		return false;
		}

	bool isSymbol()
		{
		return ( wordTypeNr_ == WORD_TYPE_SYMBOL );
		}

	bool isNumeral()
		{
		return ( wordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isAdjective()
		{
		return ( wordTypeNr_ == WORD_TYPE_ADJECTIVE );
		}

	bool isArticle()
		{
		return ( wordTypeNr_ == WORD_TYPE_ARTICLE );
		}

	bool isNoun()
		{
		return isSingularOrPluralNoun( wordTypeNr_ );
		}

	bool isSingularNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	bool isPluralNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isMatchingWordTypeNr( unsigned short wordTypeNr )
		{
		return isMatchingWordType( wordTypeNr_, wordTypeNr );
		}

	bool isDeterminerOrPronoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_SUBJECTIVE ||
				wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_OBJECTIVE ||

				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ||

				wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE ||
				wordTypeNr_ == WORD_TYPE_PERSONAL_PRONOUN_PLURAL_OBJECTIVE ||

				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL );
		}

	bool isPossessiveDeterminer()
		{
		return ( wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL );
		}

	bool isPreposition()
		{
		return ( wordTypeNr_ == WORD_TYPE_PREPOSITION );
		}

	bool isProperName()
		{
		return ( wordTypeNr_ == WORD_TYPE_PROPER_NAME );
		}

	bool isVerb()
		{
		return ( wordTypeNr_ == WORD_TYPE_VERB_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_VERB_PLURAL );
		}

	bool isPluralQuestionVerb()
		{
		return ( wordTypeNr_ == WORD_TYPE_VERB_PLURAL &&
				grammarParameter == GRAMMAR_QUESTION_VERB );
		}

	bool isText()
		{
		return ( wordTypeNr_ == WORD_TYPE_TEXT );
		}

	bool isNumeralBoth()
		{
		return ( wordParameter_ == WORD_PARAMETER_NUMERAL_BOTH );
		}

	bool isAdjectiveAssigned()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	bool isAdjectiveAssignedOrClear()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_CLEAR ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	bool isAdjectiveEvery()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE );
		}

	bool isAdjectivePrevious()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE );
		}

	bool isVirtualListPreposition()
		{
		return ( wordParameter_ == WORD_PARAMETER_PREPOSITION_FROM ||
				wordParameter_ == WORD_PARAMETER_PREPOSITION_TO ||
				wordParameter_ == WORD_PARAMETER_PREPOSITION_OF );
		}

	bool isPrepositionIn()
		{
		return ( wordParameter_ == WORD_PARAMETER_PREPOSITION_IN );
		}

	bool isNegative()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_NO ||
				wordParameter_ == WORD_PARAMETER_ADVERB_NOT );
		}

	bool isJustificationReportNoun()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT );
		}

	bool isInformationNoun()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_INFORMATION );
		}

	bool isNounValue()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
		}

	bool isNounFile()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_FILE );
		}

	bool isNounPartOf()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_PART );
		}

	bool isSeparator()
		{
		return ( wordParameter_ == WORD_PARAMETER_SYMBOL_COMMA ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_COLON ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_QUESTION_MARK ||
				wordParameter_ == WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED );
		}

	bool isUserDefined()
		{
		return ( wordParameter_ == NO_WORD_PARAMETER );
		}

	bool isSelection()
		{
		return ( grammarParameter == GRAMMAR_SELECTION );
		}

	bool isImperative()
		{
		return ( grammarParameter == GRAMMAR_IMPERATIVE );
		}

	bool isGeneralizationWord()
		{
		return ( grammarParameter == GRAMMAR_GENERALIZATION_WORD );
		}

	bool isSpecificationWord()
		{
		return ( grammarParameter == GRAMMAR_SPECIFICATION_WORD );
		}

	bool isRelationWord()
		{
		return ( wordTypeNr_ != WORD_TYPE_ARTICLE &&	// To avoid triggering on the article before a proper name preceded-by-defined-article
				grammarParameter == GRAMMAR_RELATION_WORD );
		}

	bool isGeneralizationPart()
		{
		return ( grammarParameter == GRAMMAR_GENERALIZATION_PART ||
				grammarParameter == GRAMMAR_GENERALIZATION_ASSIGNMENT );
		}

	bool isGeneralizationSpecification()
		{
		return ( grammarParameter == GRAMMAR_GENERALIZATION_SPECIFICATION );
		}

	bool isLinkedGeneralizationConjunction()
		{
		return ( grammarParameter == GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION );
		}

	bool isSentenceConjunction()
		{
		return ( grammarParameter == GRAMMAR_SENTENCE_CONJUNCTION );
		}

	unsigned short wordOrderNr()
		{
		return wordOrderNr_;
		}

	unsigned short wordParameter()
		{
		return wordParameter_;
		}

	unsigned short wordTypeNr()
		{
		return wordTypeNr_;
		}

	ResultType changeReadWord( unsigned short newWordTypeNr, WordItem *newReadWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeReadWord";

		if( newReadWordItem != NULL )
			{
			wordTypeNr_ = newWordTypeNr;
			readWordItem_ = newReadWordItem;
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given new read word item is undefined" );

		return RESULT_OK;
		}

	char *readWordTypeString()
		{
		if( readWordItem_ != NULL )
			return readWordItem_->activeWordTypeString( wordTypeNr_ );

		return NULL;
		}

	ReadItem *firstRelationWordReadItem()
		{
		ReadItem *searchItem = this;

		while( searchItem != NULL )
			{
			if( searchItem->isRelationWord() )
				return searchItem;

			searchItem = searchItem->nextReadItem();
			}

		return NULL;
		}

	ReadItem *nextReadItem()
		{
		return (ReadItem *)nextItem;
		}

	WordItem *readWordItem()
		{
		return readWordItem_;
		}

	WordTypeItem *activeReadWordTypeItem()
		{
		return ( readWordItem_ == NULL ? NULL : readWordItem_->activeWordTypeItem( true, wordTypeNr_ ) );
		}
	};
#endif

/*************************************************************************
 *
 *	"The godly will see these things and be glad,
 *	while the wicked are struck in silent." (Psalm 107:42)
 *
 *************************************************************************/
