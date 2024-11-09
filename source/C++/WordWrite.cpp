/*	Class:			WordWrite
 *	Supports class:	WordItem
 *	Purpose:		Writing specifications as sentences
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

#include "SpecificationItem.cpp"
#include "WriteList.cpp"

class WordWrite
	{
	friend class WordItem;

	// Private constructed variables

	bool hasFoundAllRelationWords_ = false;
	bool hasFoundAllRelationWordsBeforeConjunction_ = false;
	bool hasFoundAllSpecificationWords_ = false;
	bool hasFoundAllSpecificationWordsBeforeConjunction_ = false;
	bool hasFoundWordToWrite_ = false;
	bool hasGeneralizationWord_ = false;
	bool hasFoundSingleSpecificationWord_ = false;
	bool hasFoundSpecificationGeneralizationVerb_ = false;
	bool hasFoundSpecificationWord_ = false;
	bool isChineseCurrentLanguage_ = false;
	bool isSkippingClearWriteLevel_ = false;
	bool isSpecificationWaitingForConjunction_ = false;
	bool isUnknownPluralOfNoun_ = false;

	unsigned short writeWordTypeNr_ = NO_WORD_TYPE_NR;

	size_t generalizationStartWordPosition_ = 0;
	size_t specificationStartWordPosition_ = 0;

	SpecificationItem *lastFoundSpecificationItem_ = NULL;

	WordItem *lastFoundRelationWordItem_ = NULL;

	char *writeWordString_ = NULL;

//Java_private_final
	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "WordWrite";

	char lastSpecificationString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char previousSpecificationString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;

	// This string is returned by a function. So, it must be "static".
	char nEnteredRelationWordsString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;

	// Private initialized variables

	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;
	WordItem *myWordItem_ = NULL;


	// Private functions

	void initializePrivateWriteVariables()
		{
		hasFoundAllRelationWords_ = false;
		hasFoundAllRelationWordsBeforeConjunction_ = false;
		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasGeneralizationWord_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;
		hasFoundSpecificationWord_ = false;
		isChineseCurrentLanguage_ = myWordItem_->isChineseCurrentLanguage();
		isSkippingClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		lastFoundSpecificationItem_ = NULL;

		lastFoundRelationWordItem_ = NULL;

		writeWordString_ = NULL;

		strcpy( lastSpecificationString_, EMPTY_STRING );
		strcpy( previousSpecificationString_, EMPTY_STRING );

		strcpy( nEnteredRelationWordsString_, EMPTY_STRING );

		globalVariables_->currentWriteLevel = NO_WRITE_LEVEL;
		strcpy( globalVariables_->writtenSentenceString, EMPTY_STRING );

		myWordItem_->deleteTemporaryWriteList();
		}

	signed char cleanupWriteInfo( bool isWritingCurrentSpecificationWordOnly, unsigned short startWriteLevel, size_t startWordPosition, SpecificationItem *clearSpecificationItem )
		{
		char tempString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "cleanupWriteInfo";

		if( clearSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given clear specification item is undefined" );

		if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) <= startWordPosition )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string" );

		strncat( tempString, globalVariables_->writtenSentenceString, startWordPosition );
		strcpy( globalVariables_->writtenSentenceString, tempString );

		if( globalVariables_->currentWriteLevel > startWriteLevel )
			{
			if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, startWriteLevel, clearSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write levels of the write words" );

			if( generalizationStartWordPosition_ > startWordPosition )
				generalizationStartWordPosition_ = 0;

			if( specificationStartWordPosition_ > startWordPosition )
				{
				hasFoundAllSpecificationWordsBeforeConjunction_ = false;
				isSpecificationWaitingForConjunction_ = false;

				if( !clearSpecificationItem->isQuestion() )
					hasFoundSpecificationGeneralizationVerb_ = false;

				specificationStartWordPosition_ = 0;
				}
			}

		return RESULT_OK;
		}

	signed char clearWriteLevel( bool isWritingCurrentSpecificationWordOnly, unsigned short currentWriteLevel, SpecificationItem *clearSpecificationItem )
		{
		bool isExclusiveSpecification;
		bool isNegative = false;
		bool isPossessive = false;
		bool isSelfGenerated = false;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int relationCollectionNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentCollectionWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "clearWriteLevel";

		if( clearSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given clear specification item is undefined" );

		if( globalVariables_->currentWriteLevel < currentWriteLevel )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given current write level of this wordf is higher than the global current write level" );

		// Clear generalization write level
		myWordItem_->clearGeneralizationWriteLevel( currentWriteLevel );

		if( ( currentSpecificationWordItem = clearSpecificationItem->specificationWordItem() ) != NULL )
			{
			// Clear specification write level of current specification word
			currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );

			relationCollectionNr = clearSpecificationItem->relationCollectionNr();

			// No relation word
			if( ( relationWordItem = clearSpecificationItem->relationWordItem() ) == NULL )
				{
				if( !isWritingCurrentSpecificationWordOnly &&
				( specificationCollectionNr = clearSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( currentSpecificationItem = myWordItem_->firstSpecificationItem( false, clearSpecificationItem->isAssignment(), clearSpecificationItem->isInactiveAssignment(), clearSpecificationItem->isArchivedAssignment(), clearSpecificationItem->questionParameter() ) ) != NULL )
					{
					// Clear specification write level of related specification words
					isExclusiveSpecification = clearSpecificationItem->isExclusiveSpecification();
					isNegative = clearSpecificationItem->isNegative();
					isPossessive = clearSpecificationItem->isPossessive();
					isSelfGenerated = clearSpecificationItem->isSelfGeneratedSpecification();
					assumptionLevel = clearSpecificationItem->assumptionLevel();
					generalizationContextNr = clearSpecificationItem->generalizationContextNr();

					do	{
						if( currentSpecificationItem != clearSpecificationItem &&
						( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, NO_WORD_TYPE_NR, specificationCollectionNr, relationCollectionNr, generalizationContextNr, relationWordItem ) ) != NULL )
							currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );
						}
					while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedQuestionParameterSpecificationItem() ) != NULL );
					}
				}
			else	// Relation word
				{
				if( currentWriteLevel == NO_WRITE_LEVEL )
					{
					hasFoundAllRelationWords_ = false;
					hasFoundAllRelationWordsBeforeConjunction_ = false;
					}

				// Clear specification write level of relation word
				relationWordItem->clearRelationWriteLevel( currentWriteLevel );

				if( relationCollectionNr > NO_COLLECTION_NR &&
				( currentCollectionWordItem = myWordItem_->firstCollectionWordItem( relationCollectionNr ) ) != NULL )
					{
					// Do for all collection words
					do	// Clear specification write level of collection relation words
						currentCollectionWordItem->clearRelationWriteLevel( currentWriteLevel );
					while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr ) ) != NULL );
					}
				}
			}

		return RESULT_OK;
		}

	signed char writeCurrentSpecificationWordToSentence( bool isPossessive, unsigned short grammarDefinitionWordTypeNr, unsigned short specificationWordTypeNr, unsigned int relationCollectionNr, SpecificationItem *writeSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundNumberOfRelations = false;
		bool isWordTypeNumeral = ( grammarDefinitionWordTypeNr == WORD_TYPE_NUMERAL );
		bool isWordTypePluralNoun = ( grammarDefinitionWordTypeNr == WORD_TYPE_NOUN_PLURAL );
		unsigned int nEnteredRelationWords = 0;
		char *specificationWordString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeCurrentSpecificationWordToSentence";

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( isPossessive &&
		// Number of relation words of user specification item
		( nEnteredRelationWords = writeSpecificationItem->nEnteredRelationWords() ) == 0 )
			// Calculated number of relation words of a self-generated possessive specification
			nEnteredRelationWords = myWordItem_->nRelationWords( relationCollectionNr, relationWordItem );

		// No relation word
		if( ( ( nEnteredRelationWords == 0 ||
		// One relation word
		nEnteredRelationWords == 1 ) &&

		grammarDefinitionWordTypeNr == specificationWordTypeNr ) ||

		// Multiple relation words
		( nEnteredRelationWords > 1 &&

			( isWordTypeNumeral ||
			isWordTypePluralNoun ) ) )
			{
			if( ( specificationWordString = specificationWordItem->activeWordTypeString( grammarDefinitionWordTypeNr ) ) == NULL )
				{
				if( isWordTypeNumeral )
					{
					if( !isChineseCurrentLanguage_ ||
					// Typical for Chinese: no counting of parents, children, and so on, in sentence with relation.
					relationWordItem == NULL )
						{
						// The grammar definition word 'number' needs to be converted to the number of relation words
						hasFoundNumberOfRelations = true;
						sprintf( nEnteredRelationWordsString_, "%u", nEnteredRelationWords );
						writeWordString_ = nEnteredRelationWordsString_;
						}
					}
				else
					{
					if( isWordTypePluralNoun )
						{
						if( ( writeWordString_ = specificationWordItem->singularNounString() ) != NULL )
							{
							if( !isChineseCurrentLanguage_ )
								// The plural noun is unknown, but the singular noun is known.
								// So, force a singular noun, but add a 'plural noun is unknown' text.
								isUnknownPluralOfNoun_ = true;

							writeWordTypeNr_ = WORD_TYPE_NOUN_SINGULAR;
							}
						}
					else
						// Accept a hidden word type (password) in user sentence
						writeWordString_ = specificationWordItem->anyWordTypeString();
					}
				}
			else
				writeWordString_ = specificationWordString;

			if( !hasFoundNumberOfRelations &&
			writeWordString_ != NULL )
				{
				if( specificationWordItem->markSpecificationWordTypeAsWritten( writeWordTypeNr_ ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification word \"", specificationWordItem->anyWordTypeString(), "\" as written" );

				hasFoundSpecificationWord_ = true;
				lastFoundSpecificationItem_ = writeSpecificationItem;

				// Typical for Chinese
				if( isChineseCurrentLanguage_ &&
				// Chinese test files: "Boiling point",
				//						"Condensation point"
				writeSpecificationItem->isUncountableGeneralizationNoun() )
					hasFoundAllSpecificationWordsBeforeConjunction_ = true;

				if( hasFoundAllSpecificationWordsBeforeConjunction_ )
					{
					hasFoundAllSpecificationWords_ = true;
					isSkippingClearWriteLevel_ = false;
					}
				}
			}

		return RESULT_OK;
		}

	signed char writeGeneralizationWordToSentence( bool isPluralSpecificationGeneralization, unsigned short grammarDefinitionWordTypeNr )
		{
		unsigned short writeWordTypeNr = grammarDefinitionWordTypeNr;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeGeneralizationWordToSentence";

		hasGeneralizationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		if( ( writeWordString_ = myWordItem_->activeWordTypeString( isPluralSpecificationGeneralization && !isChineseCurrentLanguage_ ? WORD_TYPE_NOUN_PLURAL : grammarDefinitionWordTypeNr ) ) == NULL &&
		( writeWordString_ = myWordItem_->singularNounString() ) != NULL )
			{
			if( !isChineseCurrentLanguage_ )
				isUnknownPluralOfNoun_ = true;

			writeWordTypeNr = WORD_TYPE_NOUN_SINGULAR;
			}

		if( writeWordString_ != NULL )
			{
			if( myWordItem_->markGeneralizationWordTypeAsWritten( writeWordTypeNr ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a word type of a generalization word as written" );

			hasGeneralizationWord_ = true;
			}

		return RESULT_OK;
		}

	signed char writeRelationWordToSentence( unsigned short grammarDefinitionWordTypeNr, SpecificationItem *writeSpecificationItem )
		{
		WordItem *firstUnwrittenRelationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeRelationWordToSentence";

		writeWordString_ = NULL;

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( ( firstUnwrittenRelationWordItem = writeSpecificationItem->firstUnwrittenRelationWordItem( grammarDefinitionWordTypeNr, NULL ) ) == NULL )
			{
			hasFoundAllRelationWords_ = true;
			hasFoundAllRelationWordsBeforeConjunction_ = true;
			}
		else
			{
			if( ( writeWordString_ = firstUnwrittenRelationWordItem->activeWordTypeString( grammarDefinitionWordTypeNr ) ) == NULL )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Relation context word \"", firstUnwrittenRelationWordItem->anyWordTypeString(), "\" doesn't have the requested word type" );

			if( firstUnwrittenRelationWordItem->markRelationWordTypeAsWritten( grammarDefinitionWordTypeNr ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark relation context word \"", firstUnwrittenRelationWordItem->anyWordTypeString(), "\" as written" );

			if( hasFoundAllRelationWordsBeforeConjunction_ )
				hasFoundAllRelationWords_ = true;

			if( lastFoundRelationWordItem_ != NULL &&
			lastFoundRelationWordItem_ != firstUnwrittenRelationWordItem &&
			// Look ahead: Was this the last unwritten relation word?
			writeSpecificationItem->firstUnwrittenRelationWordItem( grammarDefinitionWordTypeNr, firstUnwrittenRelationWordItem ) == NULL )
				{
				hasFoundAllRelationWordsBeforeConjunction_ = true;
				writeWordString_ = NULL;
				}
			}

		lastFoundRelationWordItem_ = firstUnwrittenRelationWordItem;

		return RESULT_OK;
		}

	signed char writeSpecificationWordToSentence( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, bool isSpecificationGeneralization, bool isWritingCurrentSpecificationWordOnly, unsigned short assumptionLevel, unsigned short grammarDefinitionWordTypeNr, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, SpecificationItem *writeSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasSkippedDifferentSpecification = false;
		bool isAnsweredQuestion;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *lastFoundSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationWordToSentence";

		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = grammarDefinitionWordTypeNr;
		writeWordString_ = NULL;

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( isWritingCurrentSpecificationWordOnly )
			{
			if( !specificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) &&
			// Writing current specification word only
			writeCurrentSpecificationWordToSentence( isPossessive, grammarDefinitionWordTypeNr, specificationWordTypeNr, relationCollectionNr, writeSpecificationItem, specificationWordItem, relationWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word only to the sentence" );
			}
		else
			{
			isAnsweredQuestion = writeSpecificationItem->isAnsweredQuestion();

			if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( isAnsweredQuestion, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) ) != NULL )
				{
				do	{
					if( ( currentSpecificationItem == writeSpecificationItem ||
					currentSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, NO_WORD_TYPE_NR, specificationCollectionNr, relationCollectionNr, generalizationContextNr, relationWordItem ) != NULL ) &&

					( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( currentSpecificationWordItem->isSpecificationWordTypeAlreadyWritten( currentSpecificationItem->specificationWordTypeNr() ) )
							{
							if( currentSpecificationItem != writeSpecificationItem )
								hasSkippedDifferentSpecification = true;
							}
						else
							{
							// Write current specification word
							if( writeCurrentSpecificationWordToSentence( isPossessive, grammarDefinitionWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), relationCollectionNr, currentSpecificationItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the current specification word to the sentence" );
							}
						}
					}
				while( writeWordString_ == NULL &&
				( currentSpecificationItem = currentSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != NULL );
				}
			}

		if( !hasFoundSpecificationWord_ &&
		!hasFoundAllSpecificationWords_ &&
		!hasFoundAllSpecificationWordsBeforeConjunction_ &&
		lastFoundSpecificationItem_ != NULL &&
		lastFoundSpecificationItem_->specificationWordTypeNr() == grammarDefinitionWordTypeNr )
			{
			if( hasSkippedDifferentSpecification )
				{
				if( previousSpecificationString_ != NULL &&
				strlen( previousSpecificationString_ ) > 0 )
					{
					hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					isSpecificationWaitingForConjunction_ = true;

					if( !isSpecificationGeneralization )
						isSkippingClearWriteLevel_ = true;

					// Recovery after unsuccessful grammar path
					strcpy( globalVariables_->writtenSentenceString, previousSpecificationString_ );

					if( ( lastFoundSpecificationWordItem = lastFoundSpecificationItem_->specificationWordItem() ) != NULL )
						lastFoundSpecificationWordItem->clearSpecificationWriteLevel( NO_WRITE_LEVEL );
					}
				}
			else
				{
				if( isSpecificationGeneralization )
					{
					hasFoundAllSpecificationWords_ = true;
					hasFoundSingleSpecificationWord_ = true;
					}
				}
			}

		return RESULT_OK;
		}

	signed char writeWordToSentence( bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, GrammarItem *definitionGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		bool hasRelationWord;
		bool isAssignment;
		bool isActiveAssignment;
		bool isArchivedAssignment;
		bool isCharacteristicFor;
		bool isChineseCurrentLanguage = isChineseCurrentLanguage_;
		bool isConditional;
		bool isEveryGeneralization;
		bool isExclusiveSpecification;
		bool isGeneralization;
		bool isGeneralizationProperNoun;
		bool isInactiveAssignment;
		bool isNegative;
		bool isNounGeneralization;
		bool isNounSpecification;
		bool isPartOf;
		bool isPluralNounSpecification;
		bool isPossessive;
		bool isQuestion;
		bool isRelation;
		bool isSelfGenerated;
		bool isSelfGeneratedAssumption;
		bool isSelfGeneratedQuestion;
		bool isSpecification;
		bool isSpecificationGeneralization;
		bool isSpecificationString = false;
		bool isUncountableGeneralizationNoun;
		bool isUniqueUserRelation;
		// Typical for Chinese: No spaces between characters
		bool isInsertingSeparator = !isChineseCurrentLanguage;
		unsigned short assumptionLevel;
		unsigned short generalizationWordTypeNr;
		unsigned short grammarDefinitionParameter;
		unsigned short grammarDefinitionWordTypeNr;
		unsigned short prepositionParameter;
		unsigned short questionParameter;
		unsigned short specificationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int nRemainingSpecificationWordsForWriting;
		unsigned int pronounContextNr = NO_CONTEXT_NR;
		unsigned int specificationCollectionNr;
		unsigned int relationCollectionNr;
		char *writtenSentenceString;
		char *predefinedWordString = NULL;
		WordItem *predefinedWordItem;
		WordItem *pronounWordItem;
		WordItem *singleRelationWordItem;
		WordItem *specificationWordItem;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeWordToSentence";

		hasGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = NO_WORD_TYPE_NR;

		writeWordString_ = NULL;
		strcpy( nEnteredRelationWordsString_, EMPTY_STRING );

		if( definitionGrammarItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given definition grammar item is undefined" );

		if( ( grammarDefinitionWordTypeNr = definitionGrammarItem->grammarWordTypeNr() ) <= NO_WORD_TYPE_NR )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar definition has no word type" );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		hasRelationWord = writeSpecificationItem->hasRelationWord();
		isAssignment = writeSpecificationItem->isAssignment();
		isActiveAssignment = writeSpecificationItem->isActiveAssignment();
		isArchivedAssignment = writeSpecificationItem->isArchivedAssignment();
		isCharacteristicFor = writeSpecificationItem->isCharacteristicFor();
		isConditional = writeSpecificationItem->isConditional();
		isEveryGeneralization = writeSpecificationItem->isEveryGeneralization();
		isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
		isGeneralizationProperNoun = writeSpecificationItem->isProperNounGeneralization();
		isInactiveAssignment = writeSpecificationItem->isInactiveAssignment();
		isNegative = writeSpecificationItem->isNegative();
		isNounGeneralization = writeSpecificationItem->isNounGeneralization();
		isNounSpecification = writeSpecificationItem->isNounSpecification();
		isPartOf = writeSpecificationItem->isPartOf();
		isPluralNounSpecification = writeSpecificationItem->isPluralNounSpecification();
		isPossessive = writeSpecificationItem->isPossessive();
		isQuestion = writeSpecificationItem->isQuestion();
		isSelfGenerated = writeSpecificationItem->isSelfGeneratedSpecification();
		isSelfGeneratedAssumption = writeSpecificationItem->isSelfGeneratedAssumption();
		isSelfGeneratedQuestion = writeSpecificationItem->isSelfGeneratedQuestion();
		isSpecificationGeneralization = writeSpecificationItem->isSpecificationGeneralization();
		isUncountableGeneralizationNoun = writeSpecificationItem->isUncountableGeneralizationNoun();
		isUniqueUserRelation = writeSpecificationItem->isUniqueUserRelation();

		isGeneralization = ( generalizationStartWordPosition_ == 0 &&

								( !isSpecificationGeneralization ||

								( hasFoundAllSpecificationWords_ &&
								hasFoundSpecificationGeneralizationVerb_ ) ) );

		isRelation = ( hasRelationWord &&
					!isGeneralization &&

						( specificationStartWordPosition_ > 0 ||

						// Typical for Chinese: Relation before specification
						( isChineseCurrentLanguage &&
						!isPossessive &&
						!hasFoundAllRelationWords_ ) ) );

		isSpecification = ( !isGeneralization &&
							!isRelation );

		grammarDefinitionParameter = definitionGrammarItem->grammarParameter();

		assumptionLevel = writeSpecificationItem->assumptionLevel();
		generalizationWordTypeNr = writeSpecificationItem->generalizationWordTypeNr();
		prepositionParameter = writeSpecificationItem->prepositionParameter();
		questionParameter = writeSpecificationItem->questionParameter();
		specificationWordTypeNr = writeSpecificationItem->specificationWordTypeNr();
		specificationCollectionNr = writeSpecificationItem->specificationCollectionNr();
		relationCollectionNr = writeSpecificationItem->relationCollectionNr();
		generalizationContextNr = writeSpecificationItem->generalizationContextNr();
		specificationContextNr = writeSpecificationItem->specificationContextNr();
		specificationWordItem = writeSpecificationItem->specificationWordItem();
		relationWordItem = writeSpecificationItem->relationWordItem();

		if( grammarDefinitionParameter > NO_GRAMMAR_PARAMETER &&
		( predefinedWordItem = myWordItem_->predefinedWordItem( grammarDefinitionParameter ) ) != NULL )
			predefinedWordString = predefinedWordItem->wordTypeString( true, grammarDefinitionWordTypeNr );

		switch( grammarDefinitionParameter )
			{
			// Symbols
			case WORD_PARAMETER_SYMBOL_COMMA:
				// Answer to question: Comma after 'yes'or 'no'
				if( isGeneralization ||

				// Relation
				( isRelation &&
				!isUncountableGeneralizationNoun &&
				!hasFoundAllRelationWordsBeforeConjunction_ ) ||

				// Specification
				( isSpecification &&
				!hasFoundAllSpecificationWordsBeforeConjunction_ &&

					// Part-of specification
					( ( isWritingCurrentSpecificationWordOnly &&
					isNounGeneralization ) ||

					( !isWritingCurrentSpecificationWordOnly &&

						// No specification words remaining
						( ( ( nRemainingSpecificationWordsForWriting = myWordItem_->nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, NO_SENTENCE_NR, relationWordItem ) ) == 0 &&
						!isNegative &&
						!hasFoundAllSpecificationWords_ &&

							( ( isExclusiveSpecification &&

								// Test file: "programming\Connect-Four - Display information about the set"
								( isInactiveAssignment ||
								// Test file: "question answering\family\Same and similar questions"
								isQuestion ) ) ||

							// Test files: "reasoning\Boiling point",
							//				"reasoning\Collect afterwards - Past tense",
							//				"reasoning\Condensation point",
							//				"reasoning\Every car has wheels",
							//				"reasoning\Knowledge of organisms (2)",
							//				"reasoning\Scientific challenge - Confirmation",
							//				"reasoning\Scientific challenge"
							isSpecificationGeneralization ||

							// Typical for French
							// French test files: "raisonnement\famille\Olivier a 2 fils et une fille (avant la définition de famille)",
							//						"raisonnement\famille\Olivier a 2 fils et une fille",

							// Typical for Spanish
							// Spanish test files: "razonamiento\familia\Juan tiene 2 hijos y una hija (antes de la definición de la familia)",
							//						"razonamiento\familia\Juan tiene 2 hijos y una hija",
							( !isAssignment &&
							isGeneralizationProperNoun &&
							!isChineseCurrentLanguage &&
							isPossessive &&
							!isSelfGenerated ) ) ) ||

						// Efficiency: Only write comma if there are more than one specification words remaining
						nRemainingSpecificationWordsForWriting > 1 ) ) ) ) )
					{
					isInsertingSeparator = false;
					writeWordString_ = predefinedWordString;
					}
				else
					{
					if( isSpecification )
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					}

				break;

			case WORD_PARAMETER_SYMBOL_COLON:
			case WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_QUESTION_MARK:
				isInsertingSeparator = false;
				writeWordString_ = predefinedWordString;
				break;

			case WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK:
			case WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK:
				break;

			case WORD_PARAMETER_SYMBOL_QUESTION_MARK:
				if( isQuestion )
					{
					isInsertingSeparator = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			case WORD_PARAMETER_SYMBOL_CHINESE_NEGATIVE_SEPARATOR:
				if( isNegative &&
				!hasFoundAllSpecificationWords_ )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;

					if( !hasFoundAllSpecificationWordsBeforeConjunction_ &&
					myWordItem_->nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, false, true, isPossessive, isSelfGenerated, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, ( isSelfGenerated ? NO_SENTENCE_NR : writeSpecificationItem->creationSentenceNr() ), relationWordItem ) == 1 )
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					}

				break;

			case WORD_PARAMETER_SYMBOL_CHINESE_QUESTION_SEPARATOR:
				if( isQuestion &&
				isExclusiveSpecification &&
				!hasFoundAllSpecificationWords_ )
					{
					// Typical for Chinese
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;

					if( myWordItem_->nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, ( isSelfGenerated ? NO_SENTENCE_NR : writeSpecificationItem->creationSentenceNr() ), relationWordItem ) == 1 )
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					}

				break;

			case WORD_PARAMETER_NUMERAL_CHINESE_ALL:
				if( isNounGeneralization &&
				!isCharacteristicFor &&

				( isEveryGeneralization ||
				isSpecificationGeneralization ) )
					writeWordString_ = predefinedWordString;

				break;

			// Adjectives
			case WORD_PARAMETER_ADJECTIVE_CALLED_NEUTRAL:
				if( hasRelationWord &&
				!isUncountableGeneralizationNoun )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE:
				if( hasRelationWord &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem->singleRelationWordItem() ) != NULL )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE:
				if( hasRelationWord &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem->singleRelationWordItem() ) != NULL &&

				( singleRelationWordItem->isMasculineWord() ||
				!singleRelationWordItem->hasFeminineProperNounEnding() ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_CHARACTERISTIC:
				if( isCharacteristicFor )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE:
				if( isActiveAssignment &&
				writeSpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE:
			// Skip writing 'every' in combination with uncertainty
				if( isEveryGeneralization &&
				!isSelfGeneratedAssumption &&
				myWordItem_->isCorrectAdjective( grammarDefinitionParameter, generalizationWordTypeNr ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE:
				if( isActiveAssignment &&
				// Only display the word "new" during the current sentence
				writeSpecificationItem->hasCurrentCreationSentenceNr() &&

					// Test files: "ambiguity\US presidents",
					//				"reasoning\Scientific challenge"
					( writeSpecificationItem->isExclusiveSpecification() ||

					// Test file: "reasoning\Scientific challenge - Confirmation"
					( relationWordItem != NULL &&
					myWordItem_->hasCurrentlyConfirmedSpecification() &&
					// Has only one relation word
					myWordItem_->nRelationWords( relationCollectionNr, relationWordItem ) == 1 ) ) )
					writeWordString_ = predefinedWordString;

				break;
/*
			case WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE:
				break;
*/
			case WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE:
				if( isInactiveAssignment )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_SPECIFIC:
				if( writeSpecificationItem->isSpecific() )
					writeWordString_ = predefinedWordString;

				break;

			// Adverbs
			case WORD_PARAMETER_ADJECTIVE_NO:
			case WORD_PARAMETER_ADVERB_FRENCH_PAS:
				if( isNegative )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADVERB_NOT:
				if( isNegative &&

				( !isChineseCurrentLanguage ||
				// Typical for Chinese
				!isUniqueUserRelation ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADVERB_ANYMORE:
			case WORD_PARAMETER_ADVERB_CHINESE_NEVER_AGAIN:
				if( isNegative &&
				isUniqueUserRelation )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
			case WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
			case WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				// Don't show assumption level with self-generated questions
				if( !isSelfGeneratedQuestion )
					{
					if( ( assumptionLevel == ASSUMPTION_LEVEL_PROBABLY &&
					grammarDefinitionParameter == WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY ) ||

					( assumptionLevel == ASSUMPTION_LEVEL_POSSIBLY &&
					grammarDefinitionParameter == WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY ) ||

					( assumptionLevel >= ASSUMPTION_LEVEL_MAYBE &&
					grammarDefinitionParameter == WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE ) )
						writeWordString_ = predefinedWordString;
					}

				break;

			// Interjections
			case WORD_PARAMETER_INTERJECTION_YES:
			case WORD_PARAMETER_INTERJECTION_NO:
				if( grammarDefinitionParameter == answerParameter )
					writeWordString_ = predefinedWordString;

				break;

			// Articles
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE:
				// Generalization noun
				if( ( isGeneralization &&
				isNounGeneralization &&
				!isUncountableGeneralizationNoun &&
				myWordItem_->isCorrectIndefiniteArticle( grammarDefinitionParameter, generalizationWordTypeNr ) &&

					( !isChineseCurrentLanguage ||

					// Typical for Chinese
					( isEveryGeneralization &&
					!isSelfGeneratedAssumption &&
					!isSpecificationGeneralization &&
					!writeSpecificationItem->isSpecific() ) ) ) ||

				// Specification nouns
				( isSpecification &&
				isNounSpecification &&
				!isChineseCurrentLanguage &&

					( !isPluralNounSpecification ||
					// Test file: "reasoning\family\John has a daughter and 2 sons"
					grammarDefinitionParameter != WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) &&

				writeSpecificationItem->isCorrectSpecificationArticle( false, isWritingCurrentSpecificationWordOnly, grammarDefinitionParameter ) ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL:
			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE:
			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE:
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL:
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE:
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE:
				// Generalization noun
				if( ( isNounGeneralization &&
				isGeneralization &&
				isAssignment &&
				!isArchivedAssignment &&
				myWordItem_->isCorrectDefiniteArticle( grammarDefinitionParameter, generalizationWordTypeNr ) ) ||

				// Generalization proper noun
				( isGeneralizationProperNoun &&

					// Generalization proper name preceded by defined article
					( ( isGeneralization &&
					myWordItem_->isProperNounPrecededByDefiniteArticle( grammarDefinitionParameter ) ) ||

					// Specification nouns
					( isSpecification &&
					isAssignment &&

						( ( !isChineseCurrentLanguage &&

							( !isArchivedAssignment ||
							isUniqueUserRelation ) ) ||

						// Typical for Chinese
						( !hasRelationWord &&
						!isArchivedAssignment ) ) &&

					writeSpecificationItem->isCorrectSpecificationArticle( true, isWritingCurrentSpecificationWordOnly, grammarDefinitionParameter ) ) ||

					// Relation proper names preceded by defined article
					( isRelation &&
					writeSpecificationItem->isCorrectArticleOfFirstUnwrittenProperNounRelationWord( grammarDefinitionParameter ) ) ) ) )
					writeWordString_ = predefinedWordString;

				break;

			// Conjunctions
			case WORD_PARAMETER_CONJUNCTION_AND:
				// Test file: "programming\Connect-Four - Display information about the set"
				if( isInactiveAssignment ||

				// Test files: "reasoning\Knowledge of organisms (1)",
				//				"reasoning\The only option left - Boat",
				//				"reasoning\The only option left - Option"
				isNegative ||

				// Specification
				( isSpecification &&
				!isExclusiveSpecification &&
				hasFoundAllSpecificationWordsBeforeConjunction_ &&
				!hasFoundAllSpecificationWords_ ) ||

				// Relation
				( isRelation &&
				hasFoundAllRelationWordsBeforeConjunction_ &&
				!hasFoundAllRelationWords_ ) )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			case WORD_PARAMETER_CONJUNCTION_OR:
				if( isExclusiveSpecification &&
				!hasFoundAllSpecificationWords_ )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			// Ambiguity
			// Typical for Dutch: in zowel ... als ...
			case WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
				isSpecificationWaitingForConjunction_ = false;

				// Don't insert a break statement here

			// Typical for English: '... in both ... and ...'
			case WORD_PARAMETER_NUMERAL_BOTH:
			// Typical for Dutch: in zowel ... als ...
			case WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:
				if( writeSpecificationItem->isSpecificationWithStaticAmiguity() )
					writeWordString_ = predefinedWordString;

				break;

			// Nouns
			case WORD_PARAMETER_NOUN_PART:
				if( isPartOf )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_NOUN_VALUE:
				break;

			case WORD_PARAMETER_NOUN_CHINESE_IS_IT_TRUE:
				if( isQuestion &&
				!isExclusiveSpecification )
					writeWordString_ = predefinedWordString;

				break;

			// Singular pronouns
			case WORD_PARAMETER_SINGULAR_PRONOUN_I_ME_MY_MINE:
			case WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_INFORMAL:
			case WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_FORMAL:
			case WORD_PARAMETER_SINGULAR_PRONOUN_HE_HIM_HIS_HIS:
			case WORD_PARAMETER_SINGULAR_PRONOUN_SHE_HER_HER_HERS:
			case WORD_PARAMETER_SINGULAR_PRONOUN_IT_ITS_ITS_ITS:

				// Don't insert a break statement here

			// Plural pronouns
			case WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_FEMININE:
			case WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_MASCULINE:
			case WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_FEMININE:
			case WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_MASCULINE:
			case WORD_PARAMETER_PLURAL_PRONOUN_YOU_ALL:
			case WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_FEMININE:
			case WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_MASCULINE:
				if( !isArchivedAssignment &&

					( isGeneralization ||
					isSpecification ) &&

				( pronounContextNr = ( isGeneralization ? generalizationContextNr :
														specificationContextNr ) ) > NO_CONTEXT_NR )
					{
					if( ( pronounWordItem = myWordItem_->predefinedWordItem( grammarDefinitionParameter ) ) == NULL )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find the pronoun word with the definition grammar parameter" );

					if( pronounWordItem->contextWordTypeNr( pronounContextNr ) == grammarDefinitionWordTypeNr )
						writeWordString_ = predefinedWordString;
					}

				break;

			case WORD_PARAMETER_PREPOSITION_ABOUT:
			case WORD_PARAMETER_PREPOSITION_FROM:
			case WORD_PARAMETER_PREPOSITION_IN:
			case WORD_PARAMETER_PREPOSITION_TO:
			case WORD_PARAMETER_PREPOSITION_FOR:
			case WORD_PARAMETER_PREPOSITION_FRENCH_A:
				if( prepositionParameter == grammarDefinitionParameter ||
				prepositionParameter == NO_PREPOSITION_PARAMETER )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_PREPOSITION_OF:
				if( isNounGeneralization ||
				prepositionParameter == grammarDefinitionParameter ||

				( !isPluralNounSpecification &&

					( !isChineseCurrentLanguage ||

					// Typical for Chinese
					( isSpecification &&
					hasRelationWord ) ) ) )
					writeWordString_ = predefinedWordString;

				break;

			// Verbs
			case WORD_PARAMETER_SINGULAR_VERB_IS:
			case WORD_PARAMETER_PLURAL_VERB_ARE:
				if( !isConditional &&

					( ( !isPossessive &&

						( isActiveAssignment ||

						( !isAssignment &&

							( !isSpecificationGeneralization ||
							isQuestion ||
							hasFoundAllSpecificationWords_ ) ) ) ) ||

					// Typical for Chinese
					( isChineseCurrentLanguage &&
					// Chinese test files: "Boiling point",
					//						"Condensation point"
					isUncountableGeneralizationNoun &&
					hasFoundAllSpecificationWords_ ) ) )
					{
					writeWordString_ = predefinedWordString;

					if( isQuestion &&
					generalizationStartWordPosition_ == 0 )
						// Typical for Spanish: No separator after inverted question mark
						isInsertingSeparator = false;

					if( isSpecificationGeneralization )
						hasFoundSpecificationGeneralizationVerb_ = true;
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_WAS:
			case WORD_PARAMETER_PLURAL_VERB_WERE:
				if( !isPossessive &&

					// Test files: "ambiguity\Nero",
					//				"ambiguity\US presidents",
					//				"question answering\Just a few questions (4)",
					//				"question answering\Just a few questions (5)",
					//				"reasoning\Changed assumption level (1)",
					//				"reasoning\Collect afterwards - Past tense",
					//				"reasoning\James was a father - James was the father of Peter",
					//				"reasoning\Scientific challenge",
					//				among others
					( isArchivedAssignment ||

					// Test files: "ambiguity\US presidents",
					//				"programming\Connect-Four - Display information about the set",
					//				"reasoning\Unique relation"
					isInactiveAssignment ) )
					{
					if( isQuestion )
						// Typical for Spanish: No separator after inverted question mark
						isInsertingSeparator = false;

					if( isSpecificationGeneralization )
						// Test file: "reasoning\Collect afterwards - Past tense"
						hasFoundSpecificationGeneralizationVerb_ = true;

					// Test files: "ambiguity\Nero",
					//				"ambiguity\US presidents",
					//				"question answering\Just a few questions (4)",
					//				"question answering\Just a few questions (5)",
					//				"reasoning\Changed assumption level (1)",
					//				"reasoning\Changed assumption level (2)",
					//				"reasoning\Scientific challenge",
					//				among others
					if( isArchivedAssignment ||

					// Test file: "programming\Connect-Four - Display information about the set"
					writeSpecificationItem->hasSpecificationCollection() )
						writeWordString_ = predefinedWordString;
					else
						{
						// Force current tense, because the word "previous" is used
						// Test file: "ambiguity\US presidents"
						if( ( predefinedWordItem = myWordItem_->predefinedWordItem( grammarDefinitionParameter == WORD_PARAMETER_SINGULAR_VERB_WAS ? WORD_PARAMETER_SINGULAR_VERB_IS : WORD_PARAMETER_PLURAL_VERB_ARE ) ) == NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find the predefined word with word parameter ", grammarDefinitionParameter );

						if( ( writeWordString_ = predefinedWordItem->wordTypeString( true, grammarDefinitionWordTypeNr ) ) == NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't get the word type string from the predefined word with word parameter ", grammarDefinitionParameter );
						}
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_CAN_BE:
			case WORD_PARAMETER_PLURAL_VERB_CAN_BE:
				if( isConditional )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAS:
			case WORD_PARAMETER_PLURAL_VERB_HAVE:
				if( isPossessive &&
				!isArchivedAssignment )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAD:
			case WORD_PARAMETER_PLURAL_VERB_HAD:
				if( isPossessive &&
				isArchivedAssignment )
					writeWordString_ = predefinedWordString;

				break;

			// Predefined imperative verbs
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CHINESE_PUT:
				break;

			case NO_GRAMMAR_PARAMETER:

				// Don't insert a break statement here

			// Predefined adjectives
			case WORD_PARAMETER_ADJECTIVE_ASSIGNED:
			case WORD_PARAMETER_ADJECTIVE_BUSY:
			case WORD_PARAMETER_ADJECTIVE_DEFENSIVE:
			case WORD_PARAMETER_ADJECTIVE_DONE:
			case WORD_PARAMETER_ADJECTIVE_EMPTY:
			case WORD_PARAMETER_ADJECTIVE_EXCLUSIVE:
			case WORD_PARAMETER_ADJECTIVE_FEMININE:
			case WORD_PARAMETER_ADJECTIVE_INVERTED:
			case WORD_PARAMETER_ADJECTIVE_MASCULINE:
			case WORD_PARAMETER_ADJECTIVE_CHINESE_DRAW:
			case WORD_PARAMETER_ADJECTIVE_CHINESE_HIGH:
			case WORD_PARAMETER_ADJECTIVE_CHINESE_LOW:

				// Don't insert a break statement here

			// Predefined nouns
			case WORD_PARAMETER_NOUN_DEVELOPER:
			case WORD_PARAMETER_NOUN_EXPERT:
			case WORD_PARAMETER_NOUN_FILE:
			case WORD_PARAMETER_NOUN_HEAD:
			case WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
			case WORD_PARAMETER_NOUN_LANGUAGE:
			case WORD_PARAMETER_NOUN_MIND:
			case WORD_PARAMETER_NOUN_NUMBER:
			case WORD_PARAMETER_NOUN_PASSWORD:
			case WORD_PARAMETER_NOUN_SOLVE_LEVEL:
			case WORD_PARAMETER_NOUN_SOLVE_METHOD:
			case WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
			case WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
			case WORD_PARAMETER_NOUN_TAIL:
			case WORD_PARAMETER_NOUN_TEST_FILE:
			case WORD_PARAMETER_NOUN_USER:
			case WORD_PARAMETER_NOUN_INFORMATION:
			case WORD_PARAMETER_NOUN_CHINESE_MARSUPIAL:
			case WORD_PARAMETER_NOUN_CHINESE_SAILBOAT:
			case WORD_PARAMETER_NOUN_CHINESE_TURN:
				if( !isSpecificationWaitingForConjunction_ )
					{
					// Generalization word
					if( isGeneralization )
						{
						// Matching generalization word type
						if( generalizationWordTypeNr == grammarDefinitionWordTypeNr &&
						writeGeneralizationWordToSentence( ( isSpecificationGeneralization && !hasFoundSingleSpecificationWord_ && !isCharacteristicFor && !isPartOf ), grammarDefinitionWordTypeNr ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the generalization word to the sentence" );
						}
					else
						{
						// Specification
						if( isSpecification )
							{
							// Specification string
							if( specificationWordItem == NULL )
								{
								if( grammarDefinitionWordTypeNr == WORD_TYPE_TEXT &&
								// Typical for Chinese
								!writeSpecificationItem->isSpecificationStringAlreadyWritten() )
									{
									if( writeSpecificationItem->markSpecificationStringAsWritten() != RESULT_OK )
										return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification string \"", writeSpecificationItem->specificationString(), "\" as written" );

									isSpecificationString = true;
									writeWordString_ = writeSpecificationItem->specificationString();
									}
								}
							else	// Specification word
								{
								// Not found all specification words
								if( ( ( !hasFoundAllSpecificationWords_ &&

										// Matching specification word type
										( specificationWordTypeNr == grammarDefinitionWordTypeNr ||
										// Exceptions
										grammarDefinitionWordTypeNr == WORD_TYPE_NUMERAL ||

										( isPossessive &&

											// Test file: "reasoning\family\John has a daughter and 2 sons"
											( grammarDefinitionWordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
											grammarDefinitionWordTypeNr == WORD_TYPE_NOUN_PLURAL ) ) ) ) ||

									// Found all specification words
									// Test file: "reasoning\Collect afterwards - Past tense"
									( hasFoundAllSpecificationWords_ &&
									isArchivedAssignment &&
									grammarDefinitionWordTypeNr == WORD_TYPE_NOUN_SINGULAR ) ) &&

								// Write specification word to sentence
								writeSpecificationWordToSentence( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, assumptionLevel, grammarDefinitionWordTypeNr, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, writeSpecificationItem, specificationWordItem, relationWordItem ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word to the sentence" );
								}
							}
						else
							{
							// Relation word
							if( !hasFoundAllRelationWords_ &&
							// Matching relation word type
							writeSpecificationItem->relationWordTypeNr() == grammarDefinitionWordTypeNr &&
							// Write relation word to sentence
							writeRelationWordToSentence( grammarDefinitionWordTypeNr, writeSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a relation word to the sentence" );
							}
						}
					}

				break;

			default:
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I don't know how to handle the grammar definition with grammar parameter: ", grammarDefinitionParameter );
			}

		if( writeWordString_ != NULL )
			{
			if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
			strlen( writtenSentenceString ) == 0 )
				strcpy( globalVariables_->writtenSentenceString, writeWordString_ );
			else
				{
				isSkippingClearWriteLevel_ = false;

				if( isInsertingSeparator )
					strcat( globalVariables_->writtenSentenceString, SPACE_STRING );

				if( isSpecificationString )
					strcat( globalVariables_->writtenSentenceString, DOUBLE_QUOTE_STRING );

				strcat( globalVariables_->writtenSentenceString, writeWordString_ );

				if( isSpecificationString )
					strcat( globalVariables_->writtenSentenceString, DOUBLE_QUOTE_STRING );
				}

			writtenSentenceString = globalVariables_->writtenSentenceString;

			if( isUnknownPluralOfNoun_ )
				{
				strcat( writtenSentenceString, myWordItem_->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) );
				strcat( writtenSentenceString, writeWordString_ );
				strcat( writtenSentenceString, myWordItem_->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
				}

			if( hasGeneralizationWord_ )
				generalizationStartWordPosition_ = strlen( writtenSentenceString );
			else
				{
				if( hasFoundSpecificationWord_ )
					{
					specificationStartWordPosition_ = strlen( writtenSentenceString );

					// Prepare recovery
					strcpy( previousSpecificationString_, lastSpecificationString_ );
					strcpy( lastSpecificationString_, writtenSentenceString );
					}
				}
			}

		if( writeWordString_ != NULL )
			hasFoundWordToWrite_ = true;

		return RESULT_OK;
		}


	protected:
	// Constructor

	WordWrite( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given my word item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				myWordItem_->startSystemErrorInWord( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	signed char writeJustificationSpecificationInWord( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem )
		{
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeJustificationSpecificationInWord";

		if( justificationSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given justification specification item is undefined" );

		if( justificationSpecificationItem->isReplacedOrDeletedItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given justification specification item is a replaced or deleted item" );

		if( writeSelectedSpecification( false, true, true, true, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, justificationSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification with justification" );

		if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the selected specification with justification" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE_INDENTED, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a justification sentence" );

		return RESULT_OK;
		}

	signed char writeSelectedRelationInfo( bool isActiveAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		SpecificationItem *currentSpecificationItem = NULL;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedRelationInfo";

		if( writeWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( isActiveAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != NULL )
			{
			do {
				if( currentSpecificationItem->isMatchingRelation( false, NO_COLLECTION_NR, writeWordItem ) &&
				!currentSpecificationItem->isHiddenSpanishSpecification() )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification" );

					if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
					strlen( writtenSentenceString ) > 0 )
						{
						if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isQuestion ? INTERFACE_LISTING_RELATED_QUESTIONS : INTERFACE_LISTING_RELATED_INFORMATION ) ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a related header" );

						if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isJustification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
		{
		bool hasAssignment = false;
		bool hasRelationWord;
		bool hasSpecificationCollection;
		bool hasCompoundSpecificationCollection;
		bool isArchivedAssignment;
		bool isAssignment;
		bool isCombinedSpecification = false;
		bool isExclusiveSpecification;
		bool isFirstRelatedSpecification = false;
		bool isLastCompoundSpecification = false;
		bool isLastRelatedSpecification = false;
		bool isNegative;
		bool isNounGeneralization;
		bool isPluralNounSpecification;
		bool isPossessive;
		bool isOlderItem;
		bool isQuestion;
		bool isSelfGeneratedSingularDefinition = false;
		bool isSelfGeneratedSpecification;
		bool isSpecificationGeneralization;
		bool isUserSpecification;
		bool isWritingSentenceWithOnlyOneSpecification;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *relatedSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char *storedSentenceString;
		char *storedSentenceWithOnlyOneSpecificationString = NULL;
		char *writtenSentenceString;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedSpecification";

		strcpy( globalVariables_->learnedFromUserString, EMPTY_STRING );
		strcpy( globalVariables_->writtenSentenceString, EMPTY_STRING );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( writeSpecificationItem->isDeletedItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is deleted" );

		if( isJustification &&
		writeSpecificationItem->isAnsweredQuestion() &&
		writeSpecificationItem->isSelfGeneratedQuestion() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is obsolete. It is an answered, self-generated question" );

		// Typical for Spanish
		if( writeSpecificationItem->isHiddenSpanishSpecification() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is hidden Spanish ambiguous" );

		hasRelationWord = writeSpecificationItem->hasRelationWord();
		hasSpecificationCollection = writeSpecificationItem->hasSpecificationCollection();
		hasCompoundSpecificationCollection = writeSpecificationItem->hasCompoundSpecificationCollection();
		isArchivedAssignment = writeSpecificationItem->isArchivedAssignment();
		isAssignment = writeSpecificationItem->isAssignment();
		isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
		isNounGeneralization = writeSpecificationItem->isNounGeneralization();
		isNegative = writeSpecificationItem->isNegative();
		isOlderItem = writeSpecificationItem->isOlderItem();
		isPluralNounSpecification = writeSpecificationItem->isPluralNounSpecification();
		isPossessive = writeSpecificationItem->isPossessive();
		isQuestion = writeSpecificationItem->isQuestion();
		isSelfGeneratedSpecification = writeSpecificationItem->isSelfGeneratedSpecification();
		isSpecificationGeneralization = writeSpecificationItem->isSpecificationGeneralization();
		isUserSpecification = writeSpecificationItem->isUserSpecification();

		if( ( specificationWordItem = writeSpecificationItem->specificationWordItem() ) != NULL )
			{
			if( hasSpecificationCollection &&
			!isWritingCurrentSpecificationWordOnly &&
			answerParameter == NO_ANSWER_PARAMETER )
				{
				if( ( relatedResult = myWordItem_->findRelatedSpecification( true, writeSpecificationItem ) ).result != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

				isFirstRelatedSpecification = relatedResult.isFirstRelatedSpecification;
				isLastRelatedSpecification = relatedResult.isLastRelatedSpecification;

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
				relatedSpecificationItem->isOlderItem() &&
				!hasCompoundSpecificationCollection &&
				!isNegative &&
				isWritingCurrentSentenceOnly &&

					( isExclusiveSpecification ||
					isLastRelatedSpecification ) )
					isCombinedSpecification = true;
				}

			if( !hasRelationWord )
				{
				if( !isAssignment )
					{
					// No assignment found
					if( ( foundAssignmentItem = myWordItem_->firstAssignmentItem( isPossessive, isQuestion, specificationWordItem ) ) == NULL )
						{
						if( isNounGeneralization &&
						!isPluralNounSpecification &&
						writeSpecificationItem->isSelfGeneratedConclusionOrAssumption() )
							{
							isSelfGeneratedSingularDefinition = true;

							if( hasCompoundSpecificationCollection &&
							isExclusiveSpecification &&
							!isFirstRelatedSpecification &&
							isLastRelatedSpecification )
								isLastCompoundSpecification = true;
							}
						}
					else	// Found assignment
						{
						// Test files: "conflict\family\John was the father of Paul (before family definition)",
						//				"question answering\Just a few questions (4)",
						//				"question answering\Just a few questions (5)",
						//				"question answering\family\Just a few questions (1)",
						//				"reasoning\Changed assumption level (1)",
						//				"reasoning\Changed assumption level (2)",
						//				"reasoning\Collect afterwards - Past tense",
						//				among others
						if( ( foundAssignmentItem->isSelfGeneratedSpecification() ||

						( !isNounGeneralization &&

							// Test file: "ambiguity\US presidents"
							( foundAssignmentItem->hasRelationWord() ||
							// Language assignments of regression main tests
							!foundAssignmentItem->isArchivedAssignment() ) ) ) )
							hasAssignment = true;
						}
					}

				if( !hasAssignment &&
				!isExclusiveSpecification &&

					( hasCompoundSpecificationCollection ||
					isNegative ) &&

					( writeSpecificationItem->isSelfGeneratedConclusion() ||

					( !isNounGeneralization &&
					writeSpecificationItem->isSelfGeneratedAssumption() &&
					!writeSpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() ) ) )
					isWritingCurrentSpecificationWordOnly = true;
				}
			}

		if( isCombinedSpecification ||
		isWritingCurrentSpecificationWordOnly ||

		// Not self-generated singular definition
		( !isSelfGeneratedSingularDefinition &&

			( isForcingResponseNotBeingFirstSpecification ||
			isForcingResponseNotBeingAssignment ||

				// Not archived assignment
				// Test file: "Connect-Four - Display information about the set"
				( !isArchivedAssignment &&

					( ( !hasAssignment &&

						( isFirstRelatedSpecification ||
						relatedSpecificationItem == NULL ) ) ||

					// Test file: "Correcting invalidated assumption (by opposite suggestive question)"
					( isFirstRelatedSpecification &&
					writeSpecificationItem->isCorrectedSpecification() ) ) ) ||

				// Archived assignment
				( isArchivedAssignment &&

					// Test file: "ambiguity\US presidents"
					( hasRelationWord ||

					// Past tense
					( isSelfGeneratedSpecification &&

						( relatedSpecificationItem == NULL ||
						// Test file: "reasoning\Collect afterwards - Past tense"
						isFirstRelatedSpecification ) ) ) ) ) ) ||

		// Self-generated singular definition
		( isSelfGeneratedSingularDefinition &&

			( isLastCompoundSpecification ||
			isForcingResponseNotBeingFirstSpecification ||

			( !hasCompoundSpecificationCollection &&
			isFirstRelatedSpecification ) ||

			( !isExclusiveSpecification &&

				( !isPossessive ||
				// Test filea: "reasoning\Condensation point",
				//				"reasoning\Scientific challenge",
				//				"reasoning\Scientific challenge - Confirmation"
				// Typical for Chinese
				relatedSpecificationItem == NULL ) ) ) ) )
			{
			isWritingSentenceWithOnlyOneSpecification = ( isWritingCurrentSpecificationWordOnly &&
														( storedSentenceWithOnlyOneSpecificationString = writeSpecificationItem->storedSentenceWithOnlyOneSpecificationString() ) != NULL &&
														strlen( storedSentenceWithOnlyOneSpecificationString ) > 0 );

			// Efficiency: Use the stored sentence if still up-to-date
			if( ( storedSentenceString = writeSpecificationItem->storedSentenceString() ) != NULL &&
			strlen( storedSentenceString ) > 0 &&
			answerParameter == NO_ANSWER_PARAMETER &&

				( isWritingSentenceWithOnlyOneSpecification ||

				// No relation context
				( !hasRelationWord &&

					( !hasSpecificationCollection ||
					isExclusiveSpecification ||
					// Typical for Spanish
					writeSpecificationItem->isAdjectiveSpecification() ||

					// User specification
					( isUserSpecification &&

						( !hasCompoundSpecificationCollection ||
						!isWritingCurrentSpecificationWordOnly ) ) ||

					( !isPluralNounSpecification &&

						// Test file: "conflict\family\No conflict on multiple related specifications"
						( ( !isSpecificationGeneralization &&

							( !isJustification ||
							isArchivedAssignment ) ) ||

						// Multiple specification
						// Test files: "conflict\family\No conflict on multiple related specifications",
						//				"reasoning\Collect afterwards - Past tense",
						//				"reasoning\Collect afterwards - Uncertainty",
						//				"reasoning\Scientific challenge",
						//				"reasoning\Specification-generalization - Mammals (singular)"
						!myWordItem_->hasMultipleSpecificationWordsWithSameSentenceNr( writeSpecificationItem->creationSentenceNr(), writeSpecificationItem->itemNr(), writeSpecificationItem->specificationCollectionNr() ) ) ) ) ) ||

				// Relation context
				( hasRelationWord &&
				!writeSpecificationItem->hasRelationCollectionCurrentlyBeenUpdated() ) ) )
				{
				// Use the stored sentence
				if( isCheckingUserSentenceForIntegrity )
					strcpy( globalVariables_->writtenUserSentenceString, storedSentenceString );

				strcpy( globalVariables_->writtenSentenceString, ( isWritingSentenceWithOnlyOneSpecification ? storedSentenceWithOnlyOneSpecificationString : storedSentenceString ) );
				}
			else
				{
				// Efficiency: Use the stored sentence of a related specification
				if( isOlderItem &&
				relatedSpecificationItem != NULL &&

					// Non-compound user question
					( !hasCompoundSpecificationCollection ||
					!isQuestion ||
					!isSelfGeneratedSpecification ||
					writeSpecificationItem->hasCurrentCreationSentenceNr() ) &&

				// Avoid mixing user specification with self-generated specification
				relatedSpecificationItem->isUserSpecification() == isUserSpecification &&
				// Avoid mixing different assumption levels
				relatedSpecificationItem->assumptionLevel() == writeSpecificationItem->assumptionLevel() &&
				( storedSentenceString = relatedSpecificationItem->storedSentenceString() ) != NULL &&
				strlen( storedSentenceString ) > 0 )
					// Use the stored sentence of a related specification

					strcpy( globalVariables_->writtenSentenceString, storedSentenceString );
				else
					{
					if( writeSpecificationSentence( isAssignment, isArchivedAssignment, isCheckingUserSentenceForIntegrity, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, NO_GRAMMAR_LEVEL, myWordItem_->firstCurrentLanguageWritingGrammarItem( isQuestion ), writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification sentence" );

					// Under certain conditions, the sentence will be stored, in order to be re-used if needed
					if( answerParameter == NO_ANSWER_PARAMETER &&
					( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
					strlen( writtenSentenceString ) > 0 )
						{
						if( isWritingCurrentSpecificationWordOnly )
							writeSpecificationItem->storeWrittenSentenceStringWithOneSpecificationOnly();
						else
							{
							if( !hasRelationWord ||
							!isAssignment ||
							!isExclusiveSpecification ||
							isOlderItem ||
							writeSpecificationItem->isInactiveAssignment() )
								writeSpecificationItem->storeWrittenSentenceString();
							}
						}
					}
				}

			if( writeSpecificationItem->userNr() != globalVariables_->currentUserNr &&
			( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
			strlen( writtenSentenceString ) > 0 )
				{
				strcpy( globalVariables_->learnedFromUserString, myWordItem_->interfaceString( INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_START ) );
				strcat( globalVariables_->learnedFromUserString, myWordItem_->userNameString( writeSpecificationItem->userNr() ) );
				strcat( globalVariables_->learnedFromUserString, myWordItem_->interfaceString( INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_END ) );
				}
			}

		return RESULT_OK;
		}

	signed char writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, WordItem *writeWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedSpecificationInfo";

		if( writeWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, false ) ) != NULL )
			{
			do	{
				if( currentSpecificationItem->specificationWordItem() == writeWordItem )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification" );

					if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
					strlen( writtenSentenceString ) > 0 )
						{
						if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_WRITE, INTERFACE_LISTING_SPECIFICATIONS ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a related header" );

						if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSpecificationSentence( bool isAssignment, bool isArchivedAssignment, bool isCheckingUserSentenceForIntegrity, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		bool isChineseCurrentLanguage = isChineseCurrentLanguage_;
		bool isChoice;
		bool isChoiceEnd;
		bool isOption;
		bool isSkippingThisChoiceOrOptionPart;
		bool isSkippingNextChoiceOrOptionParts;
		bool isStillSuccessful;
		unsigned short startWriteLevel = globalVariables_->currentWriteLevel;
		// Take the current write sentence string length as start position
		size_t startWordPosition = strlen( globalVariables_->writtenSentenceString );
		GrammarItem *definitionGrammarItem = selectedGrammarItem;
		WriteItem *currentWriteItem = NULL;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationSentence";

		hasFoundWordToWrite_ = false;

		if( grammarLevel >= MAX_GRAMMAR_LEVEL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar level is too high: #", grammarLevel );

		if( selectedGrammarItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given selected grammar item is undefined" );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( writeSpecificationItem->isDeletedItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is deleted" );

		// Initialize
		if( grammarLevel == NO_GRAMMAR_LEVEL )
			initializePrivateWriteVariables();

		do	{
			if( !definitionGrammarItem->isDefinitionStart() )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar definition word item isn't a definition start" );

			// Grammar word
			if( definitionGrammarItem->isNewStart() )
				{
				// Write word to sentence
				if( writeWordToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a word to a sentence" );
				}
			else
				{
				// Efficiency: Only select useful grammar definitions
				if( definitionGrammarItem->isUsefulGrammarDefinition( isAssignment, isArchivedAssignment, isChineseCurrentLanguage, isPossessive, isQuestion, isSpecificationGeneralization ) )
					{
					// Grammar definition
					if( ( selectedGrammarItem = definitionGrammarItem->nextGrammarItem() ) == NULL )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar word item is undefined" );

					isChoice = false;
					isOption = false;
					isStillSuccessful = true;
					isSkippingThisChoiceOrOptionPart = false;
					isSkippingNextChoiceOrOptionParts = false;

					do	{
						if( selectedGrammarItem->isNewStart() )
							{
							if( isChoice ||
							isOption ||
							selectedGrammarItem->isChoiceStart() ||
							selectedGrammarItem->isOptionStart() )
								{
								currentWriteItem = myWordItem_->firstActiveWriteItem();

								while( currentWriteItem != NULL &&
								( currentWriteItem->grammarLevel() != grammarLevel ||
								currentWriteItem->startOfChoiceOrOptionGrammarItem() != selectedGrammarItem ) )
									currentWriteItem = currentWriteItem->nextWriteItem();

								if( isChoice ||
								// End of old choice or option - new one starts
								isOption )
									{
									isSkippingThisChoiceOrOptionPart = false;

									if( hasFoundWordToWrite_ )
										isSkippingNextChoiceOrOptionParts = true;
									else
										{
										if( isStillSuccessful &&
										currentWriteItem != NULL &&
										currentWriteItem->isSkipped )
											currentWriteItem->isSkipped = false;
										}

									if( currentWriteItem == NULL )
										{
										// Create write word
										if( myWordItem_->createWriteWord( ( !isStillSuccessful || isSkippingNextChoiceOrOptionParts ), grammarLevel, selectedGrammarItem ) != RESULT_OK )
											return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a write word" );
										}
									else
										{
										if( currentWriteItem->startOfChoiceOrOptionGrammarItem()->activeSentenceNr() != selectedGrammarItem->activeSentenceNr() ||
										currentWriteItem->startOfChoiceOrOptionGrammarItem()->itemNr() != selectedGrammarItem->itemNr() )
											return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the selected grammar path" );

										if( currentWriteItem->grammarLevel() != grammarLevel )
											return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the grammar level" );

										currentWriteItem = currentWriteItem->nextWriteItem();
										}
									}
								else
									{
									if( selectedGrammarItem->isChoiceStart() )
										isChoice = true;
									else
										isOption = true;

									isSkippingThisChoiceOrOptionPart = false;

									if( currentWriteItem == NULL )
										{
										// Create write word
										if( myWordItem_->createWriteWord( !isStillSuccessful, grammarLevel, selectedGrammarItem ) != RESULT_OK )
											return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a start write word" );
										}
									else
										{
										if( currentWriteItem->startOfChoiceOrOptionGrammarItem()->activeSentenceNr() != selectedGrammarItem->activeSentenceNr() ||
										currentWriteItem->startOfChoiceOrOptionGrammarItem()->itemNr() != selectedGrammarItem->itemNr() )
											return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the selected grammar path at the start of a choice or option" );

										if( currentWriteItem->grammarLevel() != grammarLevel )
											return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the grammar level at the start of a choice or option" );
										}
									}
								}
							}

						if( isStillSuccessful &&
						!isSkippingThisChoiceOrOptionPart &&
						!isSkippingNextChoiceOrOptionParts &&
						!selectedGrammarItem->isSkipOptionForWriting() )
							{
							if( grammarLevel + 1 >= MAX_GRAMMAR_LEVEL )
								return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #" + ( grammarLevel + 1 ) );

							// Write specification sentence
							if( writeSpecificationSentence( isAssignment, isArchivedAssignment, false, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, ( grammarLevel + 1 ), selectedGrammarItem->definitionGrammarItem, writeSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification sentence at grammar level: " + ( grammarLevel + 1 ) );

							if( !hasFoundWordToWrite_ )
								// Failed, try next part
								isSkippingThisChoiceOrOptionPart = true;
							}

						isChoiceEnd = selectedGrammarItem->isChoiceEnd;

						if( isChoiceEnd ||
						selectedGrammarItem->isOptionEnd )
							{
							isSkippingThisChoiceOrOptionPart = false;
							isSkippingNextChoiceOrOptionParts = false;

							if( isChoiceEnd )
								isChoice = false;
							else
								{
								isOption = false;

								if( isStillSuccessful )
									hasFoundWordToWrite_ = true;
								}
							}

						selectedGrammarItem = selectedGrammarItem->nextGrammarItem();

						if( !isChoice &&
						!isOption &&
						!hasFoundWordToWrite_ )
							isStillSuccessful = false;
						}
					while( !selectedGrammarItem->isDefinitionStart() );

					if( !hasFoundWordToWrite_ &&
					!isSkippingClearWriteLevel_ &&
					// The sentence has grown
					( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
					strlen( writtenSentenceString ) > startWordPosition &&
					// Clean up write info
					cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write info" );
					}
				}

			definitionGrammarItem = definitionGrammarItem->nextDefinitionGrammarItem;
			}
		while( !hasFoundWordToWrite_ &&
		definitionGrammarItem != NULL );

		if( grammarLevel == NO_GRAMMAR_LEVEL )
			{
			// Cleanup
			myWordItem_->deleteTemporaryWriteList();

			if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, NO_WRITE_LEVEL, writeSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the write word levels in all words" );

			if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
			strlen( writtenSentenceString ) > 0 )
				{
				if( isCheckingUserSentenceForIntegrity )
					strcpy( globalVariables_->writtenUserSentenceString, globalVariables_->writtenSentenceString );

				// Typical for French: Merged words
				if( myWordItem_->shrinkMergedWordsInWriteSentenceOfCurrentLanguage() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to shrink the merged words in the write sentence string" );
				}
			}

		return RESULT_OK;
		}

	signed char writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedSpecificationByKnowledge, bool isCorrectedSpecificationByOppositeSuggestiveQuestion, bool isForcingResponseNotBeingFirstSpecification, bool isReplacedBySpecificationWithRelation, bool isWritingCurrentSpecificationWordOnly, bool wasHiddenSpanishSpecification, SpecificationItem *writeSpecificationItem )
		{
		unsigned short interfaceParameter;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeUpdatedSpecification";

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

			// Write updated specification
		if( writeSelectedSpecification( false, true, isForcingResponseNotBeingFirstSpecification, false, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, writeSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an updated specification" );

		// No sentence written
		if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given specification item" );

		interfaceParameter = ( isReplacedBySpecificationWithRelation ?
							// Replaced by specification with relation
							( writeSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_MY_EARLIER_ASSUMPTIONS_THAT_HAVE_RELATION_WORDS_NOW :
																					INTERFACE_LISTING_MY_EARLIER_CONCLUSIONS_THAT_HAVE_RELATION_WORDS_NOW ) :

							// Adjusted question
							( isAdjustedSpecification ? ( writeSpecificationItem->isQuestion() ? INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS :
							// Adjusted or concluded assumptions
							( writeSpecificationItem->isSelfGeneratedConclusion() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_CONCLUDED :
																					INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_ADJUSTED ) ) :

							// Corrected assumption
							( isCorrectedSpecificationByKnowledge ? INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE :
							( isCorrectedSpecificationByOppositeSuggestiveQuestion ? INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_SUGGESTIVE_QUESTION :

							// Typical for Spanish: Not hidden anymore
							( wasHiddenSpanishSpecification ? ( writeSpecificationItem->hasAssumptionLevel() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_NOT_HIDDEN_ANYMORE :
																												INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_NOT_HIDDEN_ANYMORE ) :

							// Answered questions
							( writeSpecificationItem->isSelfGeneratedSpecification() ? INTERFACE_LISTING_MY_QUESTIONS_THAT_HAVE_BEEN_ANSWERED :
																						INTERFACE_LISTING_YOUR_QUESTIONS_THAT_HAVE_BEEN_ANSWERED ) ) ) ) ) );

		if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a header" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
