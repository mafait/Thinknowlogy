/*	Class:			WordWrite
 *	Supports class:	WordItem
 *	Purpose:		To write specifications as sentences
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

#include "SpecificationItem.cpp"
#include "WriteList.cpp"

class WordWrite
	{
	friend class WordItem;

	// Private constructed variables

	bool hasFoundAllSpecificationWords_;
	bool hasFoundAllSpecificationWordsBeforeConjunction_;
	bool hasFoundWordToWrite_;
	bool hasGeneralizationWord_;
	bool hasQuestionVerb_;
	bool hasFoundSingleSpecificationWord_;
	bool hasFoundSpecificationGeneralizationVerb_;
	bool hasFoundSpecificationWord_;

	bool isSkippingClearWriteLevel_;
	bool isSpecificationWaitingForConjunction_;
	bool isUnknownPluralOfNoun_;

	unsigned short writeWordTypeNr_;

	size_t lastNumeralWordPosition_;
	size_t generalizationStartWordPosition_;
	size_t specificationStartWordPosition_;

	SpecificationItem *mixedWordTypesSpecificationItem_;
	SpecificationItem *lastFoundSpecificationItem_;

	WordItem *lastFoundRelationWordItem_;

	char *writeWordString_;
	// This string is returned by a function. So, it must be "static".
	char nContextRelationsString_[MAX_SENTENCE_STRING_LENGTH];

	char lastSpecificationString_[MAX_SENTENCE_STRING_LENGTH];
	char previousSpecificationString_[MAX_SENTENCE_STRING_LENGTH];


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;
	WordItem *myWordItem_;


	// Private functions

	void initializeVariables()
		{
		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasGeneralizationWord_ = false;
		hasQuestionVerb_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;
		hasFoundSpecificationWord_ = false;

		isSkippingClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		lastNumeralWordPosition_ = 0;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		mixedWordTypesSpecificationItem_ = NULL;
		lastFoundSpecificationItem_ = NULL;

		lastFoundRelationWordItem_ = NULL;

		writeWordString_ = NULL;

		strcpy( lastSpecificationString_, EMPTY_STRING );
		strcpy( previousSpecificationString_, EMPTY_STRING );
		strcpy( nContextRelationsString_, EMPTY_STRING );

		commonVariables_->currentWriteLevel = NO_WRITE_LEVEL;
		strcpy( commonVariables_->writtenSentenceString, EMPTY_STRING );

		myWordItem_->deleteTemporaryWriteList();
		}

	signed char checkAssumptionLevel( SpecificationItem *writeSpecificationItem )
		{
		unsigned short previousAssumptionLevel;
		ShortResultType shortResult;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkAssumptionLevel";

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		previousAssumptionLevel = writeSpecificationItem->assumptionLevel();

		if( ( shortResult = writeSpecificationItem->recalculateAssumptionLevel() ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the write specification" );

		// Difference between recalculated assumption level and previous assumption level
		// Assumption level is not recalculated after change
		if( shortResult.shortValue != previousAssumptionLevel )
			{
			sprintf( errorString, "\nThe assumption level of the following specification item has changed during the process, but it isn't recalculated.\nSo, this specification may have a recalculation or update issue.\n\tPrevious assumption level: %u, recalculated assumption level: %u;\n\tSpecificationItem: %s.\n\n", previousAssumptionLevel, shortResult.shortValue, writeSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );

			if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char cleanupWriteInfo( bool isWritingCurrentSpecificationWordOnly, unsigned short startWriteLevel, size_t startWordPosition, SpecificationItem *clearSpecificationItem )
		{
		char tempString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupWriteInfo";

		if( strlen( commonVariables_->writtenSentenceString ) <= startWordPosition )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string" );

		strncat( tempString, commonVariables_->writtenSentenceString, startWordPosition );
		strcpy( commonVariables_->writtenSentenceString, tempString );

		if( commonVariables_->currentWriteLevel > startWriteLevel )
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
		bool isAnsweredQuestion;
		bool isExclusiveSpecification = false;
		bool isNegative = false;
		bool isPossessive = false;
		bool isSelfGenerated = false;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "clearWriteLevel";

		if( clearSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given clear specification item is undefined" );

		if( commonVariables_->currentWriteLevel < currentWriteLevel )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given current write level of this wordf is higher than the global current write level" );

		// Clear generalization write level
		myWordItem_->clearGeneralizationWriteLevel( currentWriteLevel );

		if( ( currentSpecificationWordItem = clearSpecificationItem->specificationWordItem() ) != NULL )
			{
			// Clear specification write level of current specification word
			currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );

			if( ( relationContextNr = clearSpecificationItem->relationContextNr() ) == NO_CONTEXT_NR )
				{
				if( !isWritingCurrentSpecificationWordOnly &&
				( specificationCollectionNr = clearSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
					{
					// Clear specification write level of related specification words
					isAnsweredQuestion = clearSpecificationItem->isAnsweredQuestion();
					isExclusiveSpecification = clearSpecificationItem->isExclusiveSpecification();
					isNegative = clearSpecificationItem->isNegative();
					isPossessive = clearSpecificationItem->isPossessive();
					isSelfGenerated = clearSpecificationItem->isSelfGenerated();

					assumptionLevel = clearSpecificationItem->assumptionLevel();

					generalizationContextNr = clearSpecificationItem->generalizationContextNr();
					specificationContextNr = clearSpecificationItem->specificationContextNr();

					if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( isAnsweredQuestion, clearSpecificationItem->isAssignment(), clearSpecificationItem->isInactiveAssignment(), clearSpecificationItem->isArchivedAssignment(), clearSpecificationItem->questionParameter() ) ) != NULL )
						{
						do	{
							if( currentSpecificationItem != clearSpecificationItem &&
							( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) != NULL )
								currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );
							}
						while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != NULL );
						}
					}
				}
			else
				{
				// Clear relation context write level
				if( ( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
					{
					// Do for all context words
					do	currentContextWordItem->clearRelationWriteLevel( currentWriteLevel, relationContextNr );
					while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
					}
				}
			}

		return RESULT_OK;
		}

	signed char writeGeneralizationWordToSentence( bool isPluralSpecificationGeneralization, unsigned short grammarWordTypeNr )
		{
		unsigned short writeWordTypeNr = grammarWordTypeNr;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeGeneralizationWordToSentence";

		hasGeneralizationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		if( ( writeWordString_ = myWordItem_->activeWordTypeString( isPluralSpecificationGeneralization ? WORD_TYPE_NOUN_PLURAL : grammarWordTypeNr ) ) == NULL )
			{
			if( isPluralSpecificationGeneralization &&
			grammarWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
			( writeWordString_ = myWordItem_->singularNounString() ) != NULL )
				{
				isUnknownPluralOfNoun_ = true;
				writeWordTypeNr = WORD_TYPE_NOUN_SINGULAR;
				}
			}

		if( writeWordString_ != NULL )
			{
			if( myWordItem_->isGeneralizationWordTypeAlreadyWritten( writeWordTypeNr ) )
				writeWordString_ = NULL;
			else
				{
				if( myWordItem_->markGeneralizationWordTypeAsWritten( writeWordTypeNr ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a word type of a generalization word as written" );

				hasGeneralizationWord_ = true;
				}
			}

		return RESULT_OK;
		}

	signed char writeSpecificationWordToSentence( bool isPossessive, bool isWordTypeNumeral, bool isWordTypePluralNoun, unsigned short grammarWordTypeNr, unsigned int relationContextNr, SpecificationItem *writeSpecificationItem )
		{
		bool isNumberOfRelations = false;
		unsigned short specificationWordTypeNr;
		unsigned int nContextRelations;
		WordItem *specificationWordItem;
		char *specificationWordString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationWordToSentence";

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		// Specification string
		if( ( specificationWordItem = writeSpecificationItem->specificationWordItem() ) == NULL )
			{
			if( !writeSpecificationItem->isSpecificationStringAlreadyWritten() )
				{
				if( writeSpecificationItem->markSpecificationStringAsWritten() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification string \"", writeSpecificationItem->specificationString(), "\" as written" );

				hasFoundSpecificationWord_ = true;
				lastFoundSpecificationItem_ = writeSpecificationItem;
				writeWordString_ = writeSpecificationItem->specificationString();
				}
			}
		else
			{
			// Specification word
			specificationWordTypeNr = writeSpecificationItem->specificationWordTypeNr();

			if( !specificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) )
				{
				// Number of relation words of user specification item
				if( ( nContextRelations = writeSpecificationItem->nContextRelations() ) == 0 )
					{
					// Calculated number of relation words of a self-generated possessive specification
					if( isPossessive &&
					relationContextNr > NO_CONTEXT_NR )
						nContextRelations = myWordItem_->nContextWordsInContextWords( relationContextNr, specificationWordItem );
					}

				// No relation word
				if( ( ( nContextRelations == 0 ||
				// One relation word
				nContextRelations == 1 ) &&

				grammarWordTypeNr == specificationWordTypeNr ) ||

				// Multiple relation words
				( nContextRelations > 1 &&

				( isWordTypeNumeral ||
				isWordTypePluralNoun ) ) )
					{
					if( ( specificationWordString = specificationWordItem->activeWordTypeString( grammarWordTypeNr ) ) == NULL )
						{
						if( isWordTypeNumeral )
							{
							// To avoid looping in numbers
							if( lastNumeralWordPosition_ == 0 )
								{
								// The word 'number' needs to be converted to the number of relation words
								isNumberOfRelations = true;
								lastNumeralWordPosition_ = specificationStartWordPosition_;
								sprintf( nContextRelationsString_, "%u", nContextRelations );
								writeWordString_ = nContextRelationsString_;
								}
							}
						else
							{
							if( isWordTypePluralNoun )
								{
								if( ( writeWordString_ = specificationWordItem->singularNounString() ) != NULL )
									{
									// The plural noun is unknown, but the singular noun is known.
									// So, force a singular noun, but add a 'plural noun is unknown' text.
									isUnknownPluralOfNoun_ = true;
									writeWordTypeNr_ = WORD_TYPE_NOUN_SINGULAR;
									}
								}
							else
								// Must be hidden word type
								writeWordString_ = specificationWordItem->anyWordTypeString();
							}
						}
					else
						writeWordString_ = specificationWordString;

					if( !isNumberOfRelations &&
					writeWordString_ != NULL )
						{
						if( specificationWordItem->markSpecificationWordTypeAsWritten( writeWordTypeNr_ ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification word \"", specificationWordItem->anyWordTypeString(), "\" as written" );

						hasFoundSpecificationWord_ = true;
						lastFoundSpecificationItem_ = writeSpecificationItem;

						if( hasFoundAllSpecificationWordsBeforeConjunction_ )
							{
							hasFoundAllSpecificationWords_ = true;
							isSkippingClearWriteLevel_ = false;
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char writeSpecificationWordToSentence( bool isSpecificationGeneralization, bool isWordTypeNumeral, bool isWordTypePluralNoun, bool isWritingCurrentSpecificationWordOnly, unsigned short grammarWordTypeNr, SpecificationItem *writeSpecificationItem )
		{
		bool isAnsweredQuestion;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isSelfGenerated;
		bool hasSkippedDifferentSpecification = false;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *lastFoundSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationWordToSentence";

		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = grammarWordTypeNr;

		writeWordString_ = NULL;

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		isPossessive = writeSpecificationItem->isPossessive();
		relationContextNr = writeSpecificationItem->relationContextNr();

		if( isWritingCurrentSpecificationWordOnly )
			{
			if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, writeSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word only to the sentence" );
			}
		else
			{
			isAnsweredQuestion = writeSpecificationItem->isAnsweredQuestion();
			isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
			isNegative = writeSpecificationItem->isNegative();
			isSelfGenerated = writeSpecificationItem->isSelfGenerated();

			assumptionLevel = writeSpecificationItem->assumptionLevel();

			specificationCollectionNr = writeSpecificationItem->specificationCollectionNr();
			generalizationContextNr = writeSpecificationItem->generalizationContextNr();
			specificationContextNr = writeSpecificationItem->specificationContextNr();

			if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( isAnsweredQuestion, writeSpecificationItem->isAssignment(), writeSpecificationItem->isInactiveAssignment(), writeSpecificationItem->isArchivedAssignment(), writeSpecificationItem->questionParameter() ) ) != NULL )
				{
				do	{
					if( currentSpecificationItem == writeSpecificationItem ||
					currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
						{
						if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, currentSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word to the sentence" );

						if( writeWordString_ == NULL &&
						currentSpecificationItem != writeSpecificationItem )
							{
							hasSkippedDifferentSpecification = true;

							if( writeSpecificationItem->specificationWordTypeNr() != currentSpecificationItem->specificationWordTypeNr() )
								mixedWordTypesSpecificationItem_ = currentSpecificationItem;
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
		lastFoundSpecificationItem_->specificationWordTypeNr() == grammarWordTypeNr )
			{
			if( hasSkippedDifferentSpecification )
				{
				if( strlen( previousSpecificationString_ ) > 0 )
					{
					hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					isSpecificationWaitingForConjunction_ = true;

					if( !isSpecificationGeneralization )
						isSkippingClearWriteLevel_ = true;

					lastNumeralWordPosition_ = 0;

					strcpy( commonVariables_->writtenSentenceString, previousSpecificationString_ );

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

	signed char writeRelationWordToSentence( unsigned short grammarWordTypeNr, SpecificationItem *writeSpecificationItem )
		{
		WordItem *firstUnwrittenRelationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelationWordToSentence";

		writeWordString_ = NULL;

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( ( firstUnwrittenRelationWordItem = writeSpecificationItem->unwrittenProperNameRelationWordItem( NULL ) ) != NULL )
			{
			if( ( writeWordString_ = firstUnwrittenRelationWordItem->activeWordTypeString( grammarWordTypeNr ) ) == NULL )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Relation context word \"", firstUnwrittenRelationWordItem->anyWordTypeString(), "\" doesn't have the requested word type" );

			if( firstUnwrittenRelationWordItem->markRelationWordTypeAsWritten( grammarWordTypeNr ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark relation context word \"", firstUnwrittenRelationWordItem->anyWordTypeString(), "\" as written" );

			if( lastFoundRelationWordItem_ != NULL &&
			lastFoundRelationWordItem_ != firstUnwrittenRelationWordItem &&
			// Look ahead: Was this the last unwritten relation word?
			writeSpecificationItem->unwrittenProperNameRelationWordItem( firstUnwrittenRelationWordItem ) == NULL )
				writeWordString_ = NULL;
			}

		lastFoundRelationWordItem_ = firstUnwrittenRelationWordItem;

		return RESULT_OK;
		}

	signed char writeWordToSentence( bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, GrammarItem *definitionGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		bool hasRelationContext;
		bool isAssignment;
		bool isActiveAssignment;
		bool isInactiveAssignment;
		bool isArchivedAssignment;
		bool isConditional;
		bool isExclusiveSpecification;
		bool isGeneralization;
		bool isGeneralizationNoun;
		bool isGeneralizationProperName;
		bool isNegative;
		bool isPartOf;
		bool isPossessive;
		bool isQuestion;
		bool isRelation;
		bool isSelfGeneratedAssumption;
		bool isSelfGeneratedQuestion;
		bool isSpecification;
		bool isSpecificationGeneralization;
		bool isSpecificationNoun;
		bool isUniqueUserRelation;
		bool isWordTypeNumeral;
		bool isWordTypePluralNoun;
		bool isInsertingSeparator = true;
		unsigned short assumptionLevel;
		unsigned short definitionGrammarParameter;
		unsigned short definitionGrammarWordTypeNr;
		unsigned short generalizationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		unsigned int pronounContextNr = NO_CONTEXT_NR;
		char *writtenSentenceString;
		char *predefinedWordString = NULL;
		WordItem *predefinedWordItem;
		WordItem *pronounWordItem;
		WordItem *singleRelationWordItem;
		WordItem *specificationWordItem;
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeWordToSentence";

		hasGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = NO_WORD_TYPE_NR;

		writeWordString_ = NULL;
		strcpy( nContextRelationsString_, EMPTY_STRING );

		if( definitionGrammarItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given definition grammar item is undefined" );

		if( !definitionGrammarItem->hasWordType() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar definition has no word type" );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		hasRelationContext = writeSpecificationItem->hasRelationContext();

		isAssignment = writeSpecificationItem->isAssignment();
		isActiveAssignment = writeSpecificationItem->isActiveAssignment();
		isInactiveAssignment = writeSpecificationItem->isInactiveAssignment();
		isArchivedAssignment = writeSpecificationItem->isArchivedAssignment();

		isConditional = writeSpecificationItem->isConditional();
		isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
		isGeneralizationNoun = writeSpecificationItem->isGeneralizationNoun();
		isGeneralizationProperName = writeSpecificationItem->isGeneralizationProperName();
		isNegative = writeSpecificationItem->isNegative();
		isPartOf = writeSpecificationItem->isPartOf();
		isPossessive = writeSpecificationItem->isPossessive();
		isQuestion = writeSpecificationItem->isQuestion();
		isSelfGeneratedAssumption = writeSpecificationItem->isSelfGeneratedAssumption();
		isSelfGeneratedQuestion = writeSpecificationItem->isSelfGeneratedQuestion();
		isSpecificationGeneralization = writeSpecificationItem->isSpecificationGeneralization();
		isSpecificationNoun = writeSpecificationItem->isSpecificationNoun();
		isUniqueUserRelation = writeSpecificationItem->isUniqueUserRelation();

		isGeneralization = ( generalizationStartWordPosition_ == 0 &&

							( !isSpecificationGeneralization ||

							( hasFoundAllSpecificationWords_ &&
							hasFoundSpecificationGeneralizationVerb_ ) ) );

		isRelation = ( hasRelationContext &&
					!isGeneralization &&
					generalizationStartWordPosition_ > 0 &&
					specificationStartWordPosition_ > 0 );

		isSpecification = ( !isGeneralization &&
							!isRelation );

		definitionGrammarParameter = definitionGrammarItem->grammarParameter();
		definitionGrammarWordTypeNr = definitionGrammarItem->grammarWordTypeNr();

		assumptionLevel = writeSpecificationItem->assumptionLevel();
		generalizationWordTypeNr = writeSpecificationItem->generalizationWordTypeNr();

		generalizationContextNr = writeSpecificationItem->generalizationContextNr();
		specificationContextNr = writeSpecificationItem->specificationContextNr();
		relationContextNr = writeSpecificationItem->relationContextNr();

		specificationWordItem = writeSpecificationItem->specificationWordItem();

		if( definitionGrammarParameter > NO_GRAMMAR_PARAMETER &&
		( predefinedWordItem = myWordItem_->predefinedWordItem( definitionGrammarParameter ) ) != NULL )
			predefinedWordString = predefinedWordItem->wordTypeString( true, definitionGrammarWordTypeNr );

		switch( definitionGrammarParameter )
			{
			// Symbols
			case WORD_PARAMETER_SYMBOL_COMMA:
			case WORD_PARAMETER_SYMBOL_COLON:
			case WORD_PARAMETER_SYMBOL_QUESTION_MARK:
			case WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED:
				isInsertingSeparator = false;
				writeWordString_ = predefinedWordString;
				break;

			case WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK:
			case WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED:
				break;	// Skip these words

			// Ambiguity
			// Typically for Dutch: in zowel ... als ...
			case WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
				isSpecificationWaitingForConjunction_ = false;

				// Don't insert a break statement here

			// Typically for English: in both ... and ...
			case WORD_PARAMETER_NUMERAL_BOTH:
			// Typically for Dutch: in zowel ... als ...
			case WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:
				if( writeSpecificationItem->isSpecificationWithStaticAmiguity() )
					writeWordString_ = predefinedWordString;

				break;

			// Adjectives
			case WORD_PARAMETER_ADJECTIVE_CALLED_PLURAL:
				if( hasRelationContext )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE:
				if( hasRelationContext &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem->singleRelationWordItem() ) != NULL )
					{
					if( singleRelationWordItem->isFemale() ||
					!singleRelationWordItem->hasMasculineProperNameEnding() )
						writeWordString_ = predefinedWordString;
					}

				break;

			case WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE:
				if( hasRelationContext &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem->singleRelationWordItem() ) != NULL )
					{
					if( singleRelationWordItem->isMale() ||
					!singleRelationWordItem->hasFeminineProperNameEnding() )
						writeWordString_ = predefinedWordString;
					}

				break;

			case WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE:
				if( !isSelfGeneratedAssumption &&
				writeSpecificationItem->isEveryGeneralization() &&
				myWordItem_->isCorrectAdjective( definitionGrammarParameter, generalizationWordTypeNr ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_NO:
			case WORD_PARAMETER_ADVERB_NOT:
			case WORD_PARAMETER_ADVERB_NOT_FRENCH:
				if( isNegative )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE:
				if( isInactiveAssignment &&
				!isQuestion )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE:
				if( writeSpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE:
				if( isActiveAssignment &&
				// Only display the word "new" during the current sentence
				writeSpecificationItem->hasCurrentActiveSentenceNr() &&

				( writeSpecificationItem->isExclusiveSpecification() ||

				( relationContextNr > NO_CONTEXT_NR &&
				myWordItem_->hasConfirmedSpecification() &&
				myWordItem_->nContextWordsInContextWords( relationContextNr, specificationWordItem ) == 1 ) ) )
					writeWordString_ = predefinedWordString;

				break;
/*
			case WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL:
			case WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE:
				break;	// Skip these words
*/
			// Adverbs
			case WORD_PARAMETER_ADVERB_ANYMORE:
				if( isNegative &&
				isUniqueUserRelation )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADVERB_AS:
				break;	// Skip these words

			case WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
				if( isSelfGeneratedAssumption )
					{
					if( checkAssumptionLevel( writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'maybe'" );
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
						assumptionLevel = NO_ASSUMPTION_LEVEL;
					}

				if( assumptionLevel >= NUMBER_OF_ASSUMPTION_LEVELS )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
				if( isSelfGeneratedAssumption )
					{
					if( checkAssumptionLevel( writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'possibly'" );
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
						assumptionLevel = NO_ASSUMPTION_LEVEL;
					}

				if( assumptionLevel == 2 )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				if( isSelfGeneratedAssumption )
					{
					if( checkAssumptionLevel( writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'probably'" );
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
						assumptionLevel = NO_ASSUMPTION_LEVEL;
					}

				if( assumptionLevel == 1 )
					writeWordString_ = predefinedWordString;

				break;

			// Interjections
			case WORD_PARAMETER_INTERJECTION_YES:
			case WORD_PARAMETER_INTERJECTION_NO:
				if( definitionGrammarParameter == answerParameter )
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
				if( ( hasRelationContext ||
				!isArchivedAssignment ||
				!isExclusiveSpecification ) &&

				// Generalization noun
				( ( isGeneralization &&
				isGeneralizationNoun &&
				myWordItem_->isCorrectIndefiniteArticle( definitionGrammarParameter, generalizationWordTypeNr ) ) ||

				// Specification nouns
				( isSpecification &&
				isSpecificationNoun &&

				( ( mixedWordTypesSpecificationItem_ == NULL &&
				!writeSpecificationItem->isSpecificationPluralNoun() &&
				writeSpecificationItem->isCorrectSpecificationArticle( definitionGrammarParameter ) ) ||

				( mixedWordTypesSpecificationItem_ != NULL &&
				mixedWordTypesSpecificationItem_->isCorrectSpecificationArticle( definitionGrammarParameter ) ) ) ) ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL:
			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE:
			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE:
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL:
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE:
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE:
				if( ( isAssignment &&

				( !isArchivedAssignment ||
				isUniqueUserRelation ) &&

				// Generalization noun
				( ( isGeneralization &&
				isGeneralizationNoun &&

				( !isArchivedAssignment ||
				!writeSpecificationItem->isEveryGeneralization() ) &&

				myWordItem_->isCorrectDefiniteArticle( definitionGrammarParameter, generalizationWordTypeNr ) ) ||

				// Specification nouns
				( isSpecification &&
				isSpecificationNoun &&
				isGeneralizationProperName &&
				writeSpecificationItem->isCorrectSpecificationArticle( definitionGrammarParameter ) ) ) ) ||

				// Generalization proper name preceded by defined article
				( isGeneralization &&
				isGeneralizationProperName &&
				myWordItem_->isProperNamePrecededByDefiniteArticle( definitionGrammarParameter ) ) ||

				// Relation proper names preceded by defined article
				( isRelation &&
				writeSpecificationItem->isCorrectArticleOfFirstUnwrittenProperNameRelationWord( definitionGrammarParameter ) ) )
					writeWordString_ = predefinedWordString;

				break;

			// Conjunctions
			case WORD_PARAMETER_CONJUNCTION_AND:
				if( isInactiveAssignment ||
				!isExclusiveSpecification ||
				isNegative ||
				isRelation )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			case WORD_PARAMETER_CONJUNCTION_OR:
				if( isExclusiveSpecification )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			// Nouns
			case WORD_PARAMETER_NOUN_INFORMATION:
			case WORD_PARAMETER_NOUN_VALUE:
				break;	// Skip these words

			case WORD_PARAMETER_NOUN_PART:
				if( isPartOf )
					writeWordString_ = predefinedWordString;

				break;

			// Singular pronouns
			case WORD_PARAMETER_SINGULAR_PRONOUN_I_ME_MY_MINE:
			case WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_INFORMAL:
			case WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_FORMAL:
			case WORD_PARAMETER_SINGULAR_PRONOUN_HE_HIM_HIS_HIS:
			case WORD_PARAMETER_SINGULAR_PRONOUN_SHE_HER_HER_HERS:
			case WORD_PARAMETER_SINGULAR_PRONOUN_IT_ITS_ITS_ITS:

			// Plural pronouns
			case WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_FEMININE:
			case WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_MASCULINE:
			case WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_FEMININE:
			case WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_MASCULINE:
			case WORD_PARAMETER_PLURAL_PRONOUN_YOU_ALL:
			case WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_FEMININE:
			case WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_MASCULINE:
				if( !isArchivedAssignment )
					{
					if( isGeneralization )
						pronounContextNr = generalizationContextNr;
					else
						{
						if( isSpecification )
							pronounContextNr = specificationContextNr;
						else
							{
							if( isRelation )
								pronounContextNr = relationContextNr;
							}
						}

					if( pronounContextNr > NO_CONTEXT_NR )
						{
						if( ( pronounWordItem = myWordItem_->predefinedWordItem( definitionGrammarParameter ) ) == NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find the pronoun word with the definition grammar parameter" );

						if( pronounWordItem->contextWordTypeNr( pronounContextNr ) == definitionGrammarWordTypeNr )
							writeWordString_ = predefinedWordString;
						}
					}

				break;

			case WORD_PARAMETER_PREPOSITION_ABOUT:
			case WORD_PARAMETER_PREPOSITION_FROM:
			case WORD_PARAMETER_PREPOSITION_IN:
			case WORD_PARAMETER_PREPOSITION_TO:
			case WORD_PARAMETER_PREPOSITION_OF:
				if( !writeSpecificationItem->hasPrepositionParameter() ||
				writeSpecificationItem->prepositionParameter() == definitionGrammarParameter )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_PREPOSITION_FRENCH_A:
				break;

			// Verbs
			case WORD_PARAMETER_SINGULAR_VERB_IS:
			case WORD_PARAMETER_PLURAL_VERB_ARE:
				if( !isConditional &&
				!isPossessive &&

				( !isAssignment ||
				isActiveAssignment ) &&

				( ( isQuestion &&
				!hasFoundAllSpecificationWords_ ) ||

				( !isQuestion &&

				( hasFoundAllSpecificationWords_ ||
				!isSpecificationGeneralization ) ) ) )
					{
					writeWordString_ = predefinedWordString;

					if( isQuestion )
						{
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
						isInsertingSeparator = false;
						}

					if( isSpecificationGeneralization )
						hasFoundSpecificationGeneralizationVerb_ = true;
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_WAS:
			case WORD_PARAMETER_PLURAL_VERB_WERE:
				if( !isConditional &&
				!isPossessive &&

				( isInactiveAssignment ||
				isArchivedAssignment ) )
					{
					if( isQuestion )
						{
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
						isInsertingSeparator = false;
						}

					if( isSpecificationGeneralization )
						hasFoundSpecificationGeneralizationVerb_ = true;

					if( isQuestion ||
					isArchivedAssignment ||
					!isInactiveAssignment ||

					( !hasRelationContext &&
					writeSpecificationItem->hasSpecificationCollection() ) )
						writeWordString_ = predefinedWordString;
					else
						{
						// Force current tense, because the word "previous" is used
						if( ( predefinedWordItem = myWordItem_->predefinedWordItem( definitionGrammarParameter == WORD_PARAMETER_SINGULAR_VERB_WAS ? WORD_PARAMETER_SINGULAR_VERB_IS : WORD_PARAMETER_PLURAL_VERB_ARE ) ) == NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find the predefined word with word parameter ", definitionGrammarParameter );

						if( ( writeWordString_ = predefinedWordItem->wordTypeString( true, definitionGrammarWordTypeNr ) ) == NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't get the word type string from the predefined word with word parameter ", definitionGrammarParameter );
						}
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_CAN_BE:
			case WORD_PARAMETER_PLURAL_VERB_CAN_BE:
				if( isConditional &&
				!isPossessive &&

				( !isAssignment ||		// Specification
				isActiveAssignment ) )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAD:
			case WORD_PARAMETER_PLURAL_VERB_HAD:
				if( !isConditional &&
				isPossessive &&

				( isInactiveAssignment ||
				isArchivedAssignment ) )
					{
					writeWordString_ = predefinedWordString;

					if( isQuestion )
						{
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
						isInsertingSeparator = false;
						}

					if( isSpecificationGeneralization )
						hasFoundSpecificationGeneralizationVerb_ = true;
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_CAN_HAVE:
			case WORD_PARAMETER_PLURAL_VERB_CAN_HAVE:
				if( isConditional &&
				isPossessive )
					writeWordString_ = predefinedWordString;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAS:
			case WORD_PARAMETER_PLURAL_VERB_HAVE:
				if( !isConditional &&
				isPossessive &&
				!isArchivedAssignment )
					{
					writeWordString_ = predefinedWordString;

					if( isQuestion )
						{
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
						isInsertingSeparator = false;
						}

					if( isSpecificationGeneralization )
						hasFoundSpecificationGeneralizationVerb_ = true;
					}

				break;

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
				break;	// Skip these words

			// Selection words
			case WORD_PARAMETER_SELECTION_IF:
			case WORD_PARAMETER_SELECTION_THEN:
			case WORD_PARAMETER_SELECTION_ELSE:
				break;	// Skip these words

			case NO_GRAMMAR_PARAMETER:

			// Adjectives
			case WORD_PARAMETER_ADJECTIVE_ASSIGNED:
			case WORD_PARAMETER_ADJECTIVE_BUSY:
			case WORD_PARAMETER_ADJECTIVE_DONE:
			case WORD_PARAMETER_ADJECTIVE_DEFENSIVE:
			case WORD_PARAMETER_ADJECTIVE_EMPTY:
			case WORD_PARAMETER_ADJECTIVE_EXCLUSIVE:
			case WORD_PARAMETER_ADJECTIVE_INVERTED:
			case WORD_PARAMETER_ADJECTIVE_MASCULINE:
			case WORD_PARAMETER_ADJECTIVE_FEMININE:

			// Nouns
			case WORD_PARAMETER_NOUN_DEVELOPER:
			case WORD_PARAMETER_NOUN_EXPERT:
			case WORD_PARAMETER_NOUN_FILE:
			case WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
			case WORD_PARAMETER_NOUN_HEAD:
			case WORD_PARAMETER_NOUN_TAIL:
			case WORD_PARAMETER_NOUN_LANGUAGE:
			case WORD_PARAMETER_NOUN_MIND:
			case WORD_PARAMETER_NOUN_NUMBER:
			case WORD_PARAMETER_NOUN_PASSWORD:
			case WORD_PARAMETER_NOUN_SOLVE_LEVEL:
			case WORD_PARAMETER_NOUN_SOLVE_METHOD:
			case WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
			case WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
			case WORD_PARAMETER_NOUN_TEST_FILE:
			case WORD_PARAMETER_NOUN_USER:
				if( ( !isQuestion ||
				hasQuestionVerb_ ) &&

				!isSpecificationWaitingForConjunction_ )
					{
					if( isGeneralization )
						{
						// Matching generalization word type
						if( generalizationWordTypeNr == definitionGrammarWordTypeNr )
							{
							// Generalization
							if( writeGeneralizationWordToSentence( ( isSpecificationGeneralization && !isPartOf && !hasFoundSingleSpecificationWord_ ), definitionGrammarWordTypeNr ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the generalization word to the sentence" );
							}
						}
					else
						{
						if( isSpecification )
							{
							isWordTypeNumeral = definitionGrammarItem->isNumeral();
							isWordTypePluralNoun = definitionGrammarItem->isPluralNoun();

							if( isWordTypeNumeral ||
							isWordTypePluralNoun ||
							definitionGrammarItem->isSingularNoun() ||
							// Matching specification word type
							writeSpecificationItem->specificationWordTypeNr() == definitionGrammarWordTypeNr )
								{
								// Specification
								if( writeSpecificationWordToSentence( isSpecificationGeneralization, isWordTypeNumeral, isWordTypePluralNoun, isWritingCurrentSpecificationWordOnly, definitionGrammarWordTypeNr, writeSpecificationItem ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word to the sentence" );
								}
							}
						else
							{
							if( isRelation &&
							// Matching relation word type
							writeSpecificationItem->relationWordTypeNr() == definitionGrammarWordTypeNr &&
							// Relation context
							writeRelationWordToSentence( definitionGrammarWordTypeNr, writeSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a relation word to the sentence" );
							}
						}
					}

				break;

			default:
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I don't know how to handle the grammar definition with grammar parameter: ", definitionGrammarParameter );
			}

		if( writeWordString_ != NULL )
			{
			if( strlen( commonVariables_->writtenSentenceString ) == 0 )
				strcpy( commonVariables_->writtenSentenceString, writeWordString_ );
			else
				{
				isSkippingClearWriteLevel_ = false;

				if( isInsertingSeparator )
					strcat( commonVariables_->writtenSentenceString, SPACE_STRING );

				strcat( commonVariables_->writtenSentenceString, writeWordString_ );
				}

			writtenSentenceString = commonVariables_->writtenSentenceString;

			if( isUnknownPluralOfNoun_ )
				{
				if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) == NULL )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current language word item is undefined" );

				strcat( writtenSentenceString, currentLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) );
				strcat( writtenSentenceString, writeWordString_ );
				strcat( writtenSentenceString, currentLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
				}

			if( hasGeneralizationWord_ )
				generalizationStartWordPosition_ = strlen( writtenSentenceString );
			else
				{
				if( hasFoundSpecificationWord_ )
					{
					specificationStartWordPosition_ = strlen( writtenSentenceString );

					// Recover an unsuccessful grammar path
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

	WordWrite( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasFoundWordToWrite_ = false;
		hasGeneralizationWord_ = false;
		hasQuestionVerb_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;
		hasFoundSpecificationWord_ = false;

		isSkippingClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = NO_WORD_TYPE_NR;

		lastNumeralWordPosition_ = 0;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		mixedWordTypesSpecificationItem_ = NULL;
		lastFoundSpecificationItem_ = NULL;

		lastFoundRelationWordItem_ = NULL;

		writeWordString_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "WordWrite" );

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given my word item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				myWordItem_->startSystemErrorInWord( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	signed char writeSpecificationSentence( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		bool isChoice;
		bool isOption;
		bool isSkippingThisChoiceOrOptionPart;
		bool isSkippingNextChoiceOrOptionParts;
		bool isStillSuccessful;
		unsigned short startWriteLevel = commonVariables_->currentWriteLevel;
		// Take the current write sentence string length as start position
		size_t startWordPosition = strlen( commonVariables_->writtenSentenceString );
		GrammarItem *definitionGrammarItem = selectedGrammarItem;
		WordItem *currentLanguageWordItem;
		WriteItem *currentWriteItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationSentence";

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
			initializeVariables();

		do	{
			if( !definitionGrammarItem->isDefinitionStart() )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar definition word item isn't a definition start" );

			// Grammar word
			if( definitionGrammarItem->isNewStart() )
				{
				if( writeWordToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a word to a sentence" );

				}
			else
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
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #", ( grammarLevel + 1 ) );

						if( writeSpecificationSentence( false, isWritingCurrentSpecificationWordOnly, answerParameter, ( grammarLevel + 1 ), selectedGrammarItem->definitionGrammarItem, writeSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification sentence at grammar level: ", ( grammarLevel + 1 ) );

						if( !hasFoundWordToWrite_ )
							// Failed, try next part
							isSkippingThisChoiceOrOptionPart = true;
						}

					if( selectedGrammarItem->isChoiceEnd ||
					selectedGrammarItem->isOptionEnd )
						{
						isSkippingThisChoiceOrOptionPart = false;
						isSkippingNextChoiceOrOptionParts = false;

						if( selectedGrammarItem->isChoiceEnd )
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
				while( selectedGrammarItem != NULL &&
				!selectedGrammarItem->isDefinitionStart() );

				if( !hasFoundWordToWrite_ &&
				!isSkippingClearWriteLevel_ &&
				// The sentence has grown
				strlen( commonVariables_->writtenSentenceString ) > startWordPosition &&
				cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write info" );
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

			if( strlen( commonVariables_->writtenSentenceString ) > 0 )
				{
				if( isCheckingUserSentenceForIntegrity )
					strcpy( commonVariables_->writtenUserSentenceString, commonVariables_->writtenSentenceString );

				// Typically for French: Compound words
				currentLanguageWordItem = commonVariables_->currentLanguageWordItem;

				if( currentLanguageWordItem != NULL &&
				currentLanguageWordItem->isLanguageWithMergedWords() &&
				currentLanguageWordItem->shrinkMergedWordsInWriteSentence() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to shrink the compound words in the write sentence string" );
				}
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
