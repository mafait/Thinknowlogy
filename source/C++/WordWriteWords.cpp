/*	Class:			WordWriteWords
 *	Supports class:	WordItem
 *	Purpose:		To write the words of the sentences
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
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

#include "GrammarItem.cpp"
#include "SpecificationItem.cpp"

class WordWriteWords
	{
	friend class WordItem;

	// Private constructible variables

	bool hasFoundAllSpecificationWords_;
	bool hasFoundAllSpecificationWordsBeforeConjunction_;
	bool hasFoundGeneralizationWord_;
	bool hasFoundQuestionVerb_;
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

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType writeGeneralizationWordToSentence( bool isPluralSpecificationGeneralization, unsigned short grammarWordTypeNr )
		{
		unsigned short writeWordTypeNr = grammarWordTypeNr;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeGeneralizationWordToSentence";

		hasFoundGeneralizationWord_ = false;
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
				if( myWordItem_->markGeneralizationWordTypeAsWritten( writeWordTypeNr ) == RESULT_OK )
					hasFoundGeneralizationWord_ = true;
				else
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a word type of a generalization word as written" );
				}
			}

		return RESULT_OK;
		}

	ResultType writeSpecificationWordToSentence( bool isPossessive, bool isWordTypeNumeral, bool isWordTypePluralNoun, unsigned short grammarWordTypeNr, unsigned int relationContextNr, SpecificationItem *writeSpecificationItem )
		{
		bool isNumberOfRelations = false;
		unsigned short specificationWordTypeNr;
		unsigned int nContextRelations;
		WordItem *specificationWordItem;
		char *specificationWordString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationWordToSentence";

		if( writeSpecificationItem != NULL )
			{
			// Specification string
			if( ( specificationWordItem = writeSpecificationItem->specificationWordItem() ) == NULL )
				{
				if( !writeSpecificationItem->isSpecificationStringAlreadyWritten() )
					{
					if( writeSpecificationItem->markSpecificationStringAsWritten() == RESULT_OK )
						{
						hasFoundSpecificationWord_ = true;
						lastFoundSpecificationItem_ = writeSpecificationItem;
						writeWordString_ = writeSpecificationItem->specificationString();
						}
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification string \"", writeSpecificationItem->specificationString(), "\" as written" );
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
							nContextRelations = myWordItem_->nContextWordsInAllWords( relationContextNr, specificationWordItem );
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
							if( specificationWordItem->markSpecificationWordTypeAsWritten( writeWordTypeNr_ ) == RESULT_OK )
								{
								hasFoundSpecificationWord_ = true;
								lastFoundSpecificationItem_ = writeSpecificationItem;

								if( hasFoundAllSpecificationWordsBeforeConjunction_ )
									{
									hasFoundAllSpecificationWords_ = true;
									isSkippingClearWriteLevel_ = false;
									}
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification word \"", specificationWordItem->anyWordTypeString(), "\" as written" );
							}
						}
					}
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSpecificationWordToSentence( bool isSpecificationGeneralization, bool isWordTypeNumeral, bool isWordTypePluralNoun, bool isWritingCurrentSpecificationWordOnly, unsigned short grammarWordTypeNr, SpecificationItem *writeSpecificationItem )
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

		if( writeSpecificationItem != NULL )
			{
			isPossessive = writeSpecificationItem->isPossessive();
			relationContextNr = writeSpecificationItem->relationContextNr();

			if( isWritingCurrentSpecificationWordOnly )
				{
				if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, writeSpecificationItem ) != RESULT_OK )
					myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word only to the sentence" );
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

				if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAnsweredQuestion, writeSpecificationItem->isAssignment(), writeSpecificationItem->isInactiveAssignment(), writeSpecificationItem->isArchivedAssignment(), writeSpecificationItem->questionParameter() ) ) != NULL )
					{
					do	{
						if( currentSpecificationItem == writeSpecificationItem ||
						currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, WORD_TYPE_UNDEFINED, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
							{
							if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, currentSpecificationItem ) == RESULT_OK )
								{
								if( writeWordString_ == NULL &&
								currentSpecificationItem != writeSpecificationItem )
									{
									hasSkippedDifferentSpecification = true;

									if( writeSpecificationItem->specificationWordTypeNr() != currentSpecificationItem->specificationWordTypeNr() )
										mixedWordTypesSpecificationItem_ = currentSpecificationItem;
									}
								}
							else
								myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word to the sentence" );
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
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeRelationWordToSentence( unsigned short grammarWordTypeNr, SpecificationItem *writeSpecificationItem )
		{
		// Start to search first word in function contextWordInAllWords
		WordItem *currentRelationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelationWordToSentence";

		writeWordString_ = NULL;

		if( writeSpecificationItem != NULL )
			{
			do	{
				if( ( currentRelationWordItem = writeSpecificationItem->relationWordItem( currentRelationWordItem ) ) != NULL )
					{
					if( ( writeWordString_ = currentRelationWordItem->activeWordTypeString( grammarWordTypeNr ) ) != NULL )
						{
						if( currentRelationWordItem->isRelationWordTypeAlreadyWritten( grammarWordTypeNr ) )
							writeWordString_ = NULL;
						else
							{
							if( currentRelationWordItem->markRelationWordTypeAsWritten( grammarWordTypeNr ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark relation context word \"", currentRelationWordItem->anyWordTypeString(), "\" as written" );
							}
						}
					else
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Relation context word \"", currentRelationWordItem->anyWordTypeString(), "\" doesn't have the requested word type" );
					}
				}
			while( writeWordString_ == NULL &&
			currentRelationWordItem != NULL );

			if( currentRelationWordItem != NULL &&
			lastFoundRelationWordItem_ != NULL &&
			lastFoundRelationWordItem_ != currentRelationWordItem &&
			// Look ahead: This is the last relation word of the relation context
			writeSpecificationItem->relationWordItem( currentRelationWordItem ) == NULL )
				writeWordString_ = NULL;

			lastFoundRelationWordItem_ = currentRelationWordItem;
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType checkAssumptionLevel( SpecificationItem *writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		unsigned short previousAssumptionLevel;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkAssumptionLevel";

		if( writeSpecificationItem != NULL )
			{
			if( ( specificationResult = writeSpecificationItem->getAssumptionLevel() ).result == RESULT_OK )
				{
				if( ( previousAssumptionLevel = specificationResult.assumptionLevel ) > NO_ASSUMPTION_LEVEL )
					{
					if( ( specificationResult = writeSpecificationItem->recalculateAssumptionLevel() ).result == RESULT_OK )
						{
						if( specificationResult.assumptionLevel != previousAssumptionLevel )
							myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The assumption level of the given write specification item has changed during the process, but it isn't recalculated. So, this specification may have a recalculation or update issue" );
						}
					else
						myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to recalculate the assumption level" );
					}
				}
			else
				myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to get the assumption level" );
			}
		else
			myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}


	protected:
	// Constructor / deconstructor

	WordWriteWords( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasFoundGeneralizationWord_ = false;
		hasFoundQuestionVerb_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;
		hasFoundSpecificationWord_ = false;

		isSkippingClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = WORD_TYPE_UNDEFINED;

		lastNumeralWordPosition_ = 0;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		mixedWordTypesSpecificationItem_ = NULL;
		lastFoundSpecificationItem_ = NULL;

		lastFoundRelationWordItem_ = NULL;

		writeWordString_ = NULL;

		strcpy( lastSpecificationString_, EMPTY_STRING );
		strcpy( previousSpecificationString_, EMPTY_STRING );

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordWriteWords" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeWordWriteWordsVariables()
		{
		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasFoundGeneralizationWord_ = false;
		hasFoundQuestionVerb_ = false;
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
		}

	void initializeWordWriteWordsSpecificationVariables( bool isQuestion, size_t startWordPosition )
		{
		if( generalizationStartWordPosition_ > startWordPosition )
			generalizationStartWordPosition_ = 0;

		if( specificationStartWordPosition_ > startWordPosition )
			{
			hasFoundAllSpecificationWordsBeforeConjunction_ = false;
			isSpecificationWaitingForConjunction_ = false;

			if( !isQuestion )
				hasFoundSpecificationGeneralizationVerb_ = false;

			specificationStartWordPosition_ = 0;
			}
		}

	WriteResultType writeWordsToSentence( bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, GrammarItem *definitionGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		WriteResultType writeResult;
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
		bool isWordTypeSingularNoun;
		bool isWordTypePluralNoun;
		bool isInsertingSeparator = true;
		unsigned short assumptionLevel;
		unsigned short definitionGrammarParameter;
		unsigned short definitionGrammarWordTypeNr;
		unsigned short generalizationWordTypeNr;
		unsigned short specificationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		unsigned int pronounContextNr = NO_CONTEXT_NR;
		WordItem *predefinedWordItem;
		WordItem *pronounWordItem;
		WordItem *relationWordItem;
		WordItem *specificationWordItem;
		WordItem *currentLanguageWordItem;
		char *writtenSentenceString;
		char *predefinedWordString = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeWordsToSentence";

		hasFoundGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = WORD_TYPE_UNDEFINED;

		writeWordString_ = NULL;
		strcpy( nContextRelationsString_, EMPTY_STRING );

		if( definitionGrammarItem != NULL )
			{
			if( definitionGrammarItem->hasWordType() )
				{
				if( writeSpecificationItem != NULL )
					{
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
					specificationWordTypeNr = writeSpecificationItem->specificationWordTypeNr();

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
							myWordItem_->nContextWordsInAllWords( relationContextNr, specificationWordItem ) == 1 )
								{
								if( ( relationWordItem = writeSpecificationItem->relationWordItem() ) != NULL )
									writeWordString_ = predefinedWordString;
								else
									myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find any relation word" );
								}

							break;

						case WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE:
							if( hasRelationContext &&
							myWordItem_->nContextWordsInAllWords( relationContextNr, specificationWordItem ) == 1 )
								{
								if( ( relationWordItem = writeSpecificationItem->relationWordItem() ) != NULL )
									{
									if( relationWordItem->isMale() ||
									!relationWordItem->hasFeminineProperNameEnding() )
										writeWordString_ = predefinedWordString;
									}
								else
									myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find any relation word" );
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
							if( isActiveAssignment &&
							writeSpecificationItem->isGeneralizationAssignment() )
								writeWordString_ = predefinedWordString;

							break;

						case WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL:
						case WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE:
							if( isActiveAssignment &&
							// Only show the word "new" during the current sentence
							writeSpecificationItem->hasCurrentActiveSentenceNr() &&
							writeSpecificationItem->isGeneralizationAssignment() )
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
								if( ( specificationResult = checkAssumptionLevel( writeSpecificationItem ) ).result == RESULT_OK )
									assumptionLevel = specificationResult.assumptionLevel;
								else
									myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'maybe'" );
								}
							else
								{
								if( isSelfGeneratedQuestion )
									// Self-generated question shouldn't show its assumption level
									assumptionLevel = NO_ASSUMPTION_LEVEL;
								}

							if( assumptionLevel >= NUMBER_OF_ASSUMPTION_LEVELS )
								writeWordString_ = predefinedWordString;

							break;

						case WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
							if( isSelfGeneratedAssumption )
								{
								if( ( specificationResult = checkAssumptionLevel( writeSpecificationItem ) ).result == RESULT_OK )
									assumptionLevel = specificationResult.assumptionLevel;
								else
									myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'possibly'" );
								}
							else
								{
								if( isSelfGeneratedQuestion )
									// Self-generated question shouldn't show its assumption level
									assumptionLevel = NO_ASSUMPTION_LEVEL;
								}

							if( assumptionLevel == 2 )
								writeWordString_ = predefinedWordString;

							break;

						case WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
							if( isSelfGeneratedAssumption )
								{
								if( ( specificationResult = checkAssumptionLevel( writeSpecificationItem ) ).result == RESULT_OK )
									assumptionLevel = specificationResult.assumptionLevel;
								else
									myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'probably'" );
								}
							else
								{
								if( isSelfGeneratedQuestion )
									// Self-generated question shouldn't show its assumption level
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

							( ( specificationWordTypeNr != WORD_TYPE_NOUN_PLURAL &&
							mixedWordTypesSpecificationItem_ == NULL &&
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
							isGeneralizationProperName &&
							isSpecificationNoun &&
							writeSpecificationItem->isCorrectSpecificationArticle( definitionGrammarParameter ) ) ) ) ||

							// Generalization proper name preceded by defined article
							( isGeneralization &&
							isGeneralizationProperName &&
							myWordItem_->isProperNamePrecededByDefiniteArticle( definitionGrammarParameter ) ) ||

							// Relation proper names preceded by defined article
							( isRelation &&
							writeSpecificationItem->isRelationProperName() &&
							writeSpecificationItem->isCorrectRelationArticle( definitionGrammarParameter ) ) )
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
									if( ( pronounWordItem = myWordItem_->predefinedWordItem( definitionGrammarParameter ) ) != NULL )
										{
										if( pronounWordItem->contextWordTypeNrInWord( pronounContextNr ) == definitionGrammarWordTypeNr )
											writeWordString_ = predefinedWordString;
										}
									else
										myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find the pronoun word with the definition grammar parameter" );
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
									hasFoundQuestionVerb_ = true;
									// Typically for Spanish
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
									hasFoundQuestionVerb_ = true;
									// Typically for Spanish
									isInsertingSeparator = false;
									}

								if( isSpecificationGeneralization )
									hasFoundSpecificationGeneralizationVerb_ = true;

								if( isArchivedAssignment ||
								!isExclusiveSpecification ||
								isQuestion ||
								!writeSpecificationItem->hasGeneralizationCollection() )
									writeWordString_ = predefinedWordString;
								else
									{
									if( myWordItem_->firstNonQuestionAssignmentItem( false, true, true, isNegative, isPossessive, relationContextNr, specificationWordItem ) == NULL )
										writeWordString_ = predefinedWordString;
									else
										{
										// Force current tense, because the word "previous" is used
										if( ( predefinedWordItem = myWordItem_->predefinedWordItem( definitionGrammarParameter == WORD_PARAMETER_SINGULAR_VERB_WAS ? WORD_PARAMETER_SINGULAR_VERB_IS : WORD_PARAMETER_PLURAL_VERB_ARE ) ) != NULL )
											{
											if( ( writeWordString_ = predefinedWordItem->wordTypeString( true, definitionGrammarWordTypeNr ) ) == NULL )
												myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't get the word type string from the predefined word with word parameter ", definitionGrammarParameter );
											}
										else
											myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find the predefined word with word parameter ", definitionGrammarParameter );
										}
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
									hasFoundQuestionVerb_ = true;
									// Typically for Spanish
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
									hasFoundQuestionVerb_ = true;
									// Typically for Spanish
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
						case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW:
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
							hasFoundQuestionVerb_ ) &&

							!isSpecificationWaitingForConjunction_ )
								{
								if( isGeneralization )
									{
									// Matching generalization word type
									if( generalizationWordTypeNr == definitionGrammarWordTypeNr )
										{
										// Generalization
										if( writeGeneralizationWordToSentence( ( isSpecificationGeneralization && !isPartOf && !hasFoundSingleSpecificationWord_ ), definitionGrammarWordTypeNr ) != RESULT_OK )
											myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the generalization word to the sentence" );
										}
									}
								else
									{
									if( isSpecification )
										{
										isWordTypeNumeral = definitionGrammarItem->isNumeral();
										isWordTypePluralNoun = definitionGrammarItem->isPluralNoun();
										isWordTypeSingularNoun = definitionGrammarItem->isSingularNoun();

										if( isWordTypeNumeral ||
										isWordTypePluralNoun ||
										isWordTypeSingularNoun ||
										// Matching specification word type
										specificationWordTypeNr == definitionGrammarWordTypeNr )
											{
											// Specification
											if( writeSpecificationWordToSentence( isSpecificationGeneralization, isWordTypeNumeral, isWordTypePluralNoun, isWritingCurrentSpecificationWordOnly, definitionGrammarWordTypeNr, writeSpecificationItem ) != RESULT_OK )
												myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a specification word to the sentence" );
											}
										}
									else
										{
										if( isRelation &&
										// Matching relation word type
										writeSpecificationItem->relationWordTypeNr() == definitionGrammarWordTypeNr )
											{
											// Relation context
											if( writeRelationWordToSentence( definitionGrammarWordTypeNr, writeSpecificationItem ) != RESULT_OK )
												myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a relation word to the sentence" );
											}
										}
									}
								}

							break;

						default:
							myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I don't know how to handle the grammar definition with grammar parameter: ", definitionGrammarParameter );
						}

					if( commonVariables_->result == RESULT_OK )
						{
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
								if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) != NULL )
									{
									strcat( writtenSentenceString, currentLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) );
									strcat( writtenSentenceString, writeWordString_ );
									strcat( writtenSentenceString, currentLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
									}
								else
									myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current language word item is undefined" );
								}

							if( hasFoundGeneralizationWord_ )
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
						}
					}
				else
					myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );
				}
			else
				myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar definition has no word type" );
			}
		else
			myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given definition grammar item is undefined" );

		writeResult.hasFoundWordToWrite = ( writeWordString_ != NULL );
		writeResult.isSkippingClearWriteLevel = isSkippingClearWriteLevel_;
		writeResult.result = commonVariables_->result;
		return writeResult;
		}
	};

/*************************************************************************
 *	"Your word is a lamp to guide my feet
 *	and a light for my path." (Psalm 119:105)
 *************************************************************************/
