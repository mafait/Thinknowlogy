/*
 *	Class:			WordWriteWords
 *	Supports class:	WordItem
 *	Purpose:		To write the words of the sentences
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
	bool hasFoundSpecificationWord_;
	bool hasFoundQuestionVerb_;
	bool hasFoundSingleSpecificationWord_;
	bool hasFoundSpecificationGeneralizationVerb_;

	bool isSkipClearWriteLevel_;
	bool isSpecificationWaitingForConjunction_;
	bool isRelationWaitingForConjunction_;
	bool isUnknownPluralOfNoun_;

	unsigned short assumptionLevel_;

	size_t generalizationStartWordPosition_;
	size_t specificationStartWordPosition_;

	SpecificationItem *lastFoundSpecificationItem_;

	WordItem *lastFoundRelationWordItem_;

	char *writeWordString_;

	char lastSpecificationString_[MAX_SENTENCE_STRING_LENGTH];
	char previousSpecificationString_[MAX_SENTENCE_STRING_LENGTH];

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isSpecificationReasoningWordType( unsigned short specificationWordTypeNr )
		{
		return ( specificationWordTypeNr == WORD_TYPE_NUMERAL ||
				specificationWordTypeNr == WORD_TYPE_LETTER_SMALL ||
				specificationWordTypeNr == WORD_TYPE_LETTER_CAPITAL ||
				specificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL );
		}

	ResultType checkAssumptionLevel( SpecificationItem *writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		unsigned short previousAssumptionLevel;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkAssumptionLevel";

		assumptionLevel_ = NO_ASSUMPTION_LEVEL;

		if( writeSpecificationItem != NULL )
			{
			if( !writeSpecificationItem->isSelfGeneratedQuestion() &&

			( writeSpecificationItem->hasAssumptionLevel() ||
			writeSpecificationItem->isSelfGeneratedAssumption() ) )
				{
				if( ( specificationResult = writeSpecificationItem->getAssumptionLevel() ).result == RESULT_OK )
					{
					if( specificationResult.hasPerformedRecalculation )
						assumptionLevel_ = specificationResult.assumptionLevel;
					else
						{
						if( ( previousAssumptionLevel = specificationResult.assumptionLevel ) > NO_ASSUMPTION_LEVEL )
							{
							if( ( specificationResult = writeSpecificationItem->recalculateAssumptionLevel() ).result == RESULT_OK )
								{
								if( specificationResult.assumptionLevel == previousAssumptionLevel )
									assumptionLevel_ = specificationResult.assumptionLevel;
								else
									return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The assumption level of the given write specification item has changed during the process, but it isn't recalculated. So, this specification may have a recalculation or update issue" );
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to recalculate the assumption level" );
							}
						}
					}
				else
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to get the assumption level" );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}

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

	ResultType writeSpecificationWordToSentence( bool isSpecificationGeneralization, bool isWordTypeNumeral, bool isWordTypePluralNoun, bool isWordTypeSingularNoun, bool isWritingCurrentSpecificationWordOnly, unsigned short grammarWordTypeNr, SpecificationItem *writeSpecificationItem )
		{
		bool isAnsweredQuestion;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isSelfGenerated;
		bool hasFoundUnwrittenWordType = false;
		bool isNumberOfRelations = false;
		unsigned short assumptionLevel;
		unsigned short specificationWordTypeNr;
		unsigned short writeWordTypeNr = grammarWordTypeNr;
		unsigned int nContextRelations;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *lastFoundSpecificationWordItem;
		char nContextRelationsString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationWordToSentence";

		hasFoundSpecificationWord_ = false;
		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordString_ = NULL;

		if( writeSpecificationItem != NULL )
			{
			isAnsweredQuestion = writeSpecificationItem->isAnsweredQuestion();

			if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAnsweredQuestion, writeSpecificationItem->isAssignment(), writeSpecificationItem->isInactiveAssignment(), writeSpecificationItem->isArchivedAssignment(), writeSpecificationItem->questionParameter() ) ) != NULL )
				{
				isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
				isNegative = writeSpecificationItem->isNegative();
				isPossessive = writeSpecificationItem->isPossessive();
				isSelfGenerated = writeSpecificationItem->isSelfGenerated();

				assumptionLevel = writeSpecificationItem->assumptionLevel();
				specificationWordTypeNr = writeSpecificationItem->specificationWordTypeNr();

				specificationCollectionNr = writeSpecificationItem->specificationCollectionNr();
				generalizationContextNr = writeSpecificationItem->generalizationContextNr();
				specificationContextNr = writeSpecificationItem->specificationContextNr();
				relationContextNr = writeSpecificationItem->relationContextNr();

				do	{
					if( currentSpecificationItem == writeSpecificationItem ||

					( !isWritingCurrentSpecificationWordOnly &&
					currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) )
						{
						if( currentSpecificationItem != writeSpecificationItem )
							hasFoundUnwrittenWordType = true;

						// Specification string
						if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) == NULL )
							{
							if( !currentSpecificationItem->isSpecificationStringAlreadyWritten() )
								{
								if( currentSpecificationItem->markSpecificationStringAsWritten() == RESULT_OK )
									{
									hasFoundSpecificationWord_ = true;
									lastFoundSpecificationItem_ = currentSpecificationItem;
									writeWordString_ = currentSpecificationItem->specificationString();

									if( hasFoundAllSpecificationWordsBeforeConjunction_ )
										{
										hasFoundAllSpecificationWords_ = true;
										hasFoundAllSpecificationWordsBeforeConjunction_ = false;
										isSkipClearWriteLevel_ = false;
										}
									}
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification string \"", currentSpecificationItem->specificationString(), "\" as written" );
								}
							}
						else	// Specification word
							{
							if( ( nContextRelations = currentSpecificationItem->nContextRelations() ) == 0 )
								{
								if( isPossessive &&
								specificationStartWordPosition_ == 0 &&		// To avoid looping in numbers
								currentSpecificationItem->hasRelationContext() &&
								isSpecificationReasoningWordType( grammarWordTypeNr ) )
									nContextRelations = myWordItem_->nContextWordsInAllWords( currentSpecificationItem->relationContextNr(), currentSpecificationWordItem );
								}

							if( ( nContextRelations == 0 &&
							specificationWordTypeNr == grammarWordTypeNr ) ||

							( nContextRelations == 1 &&
							isWordTypeSingularNoun ) ||

							( nContextRelations > 1 &&

							( isWordTypeNumeral ||
							isWordTypePluralNoun ) ) )
								{
								if( ( writeWordString_ = currentSpecificationWordItem->activeWordTypeString( grammarWordTypeNr ) ) == NULL )
									{
									if( isWordTypeNumeral )
										{
										if( specificationStartWordPosition_ == 0 )		// To avoid looping in numbers
											{
											// The word 'number' needs to be converted to the number of relations
											isNumberOfRelations = true;
											sprintf( nContextRelationsString, "%u", nContextRelations );
											writeWordString_ = nContextRelationsString;
											}
										}
									else
										{
										if( isWordTypePluralNoun &&
										( writeWordString_ = currentSpecificationWordItem->singularNounString() ) != NULL )
											// The plural noun is unknown, but the singular is known. So, force a singular noun, but with a remark
											isUnknownPluralOfNoun_ = true;		// Force as singular noun
										else	// Must be hidden word type
											writeWordString_ = currentSpecificationWordItem->anyWordTypeString();
										}
									}

								if( !isNumberOfRelations &&
								writeWordString_ != NULL )
									{
									if( isUnknownPluralOfNoun_ )
										writeWordTypeNr = WORD_TYPE_NOUN_SINGULAR;

									if( currentSpecificationWordItem->isSpecificationWordTypeAlreadyWritten( writeWordTypeNr ) )
										writeWordString_ = NULL;
									else
										{
										if( currentSpecificationWordItem->markSpecificationWordTypeAsWritten( writeWordTypeNr ) == RESULT_OK )
											{
											hasFoundSpecificationWord_ = true;
											lastFoundSpecificationItem_ = currentSpecificationItem;

											if( hasFoundAllSpecificationWordsBeforeConjunction_ )
												{
												hasFoundAllSpecificationWords_ = true;
												isSkipClearWriteLevel_ = false;
												}
											}
										else
											return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\" as written" );
										}
									}
								}
							}
						}
					}
				while( writeWordString_ == NULL &&
				( currentSpecificationItem = currentSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != NULL );
				}

			if( !hasFoundSpecificationWord_ &&
			!hasFoundAllSpecificationWords_ &&
			!hasFoundAllSpecificationWordsBeforeConjunction_ &&
			lastFoundSpecificationItem_ != NULL &&
			lastFoundSpecificationItem_->specificationWordTypeNr() == grammarWordTypeNr )
				{
				if( hasFoundUnwrittenWordType )
					{
					if( strlen( previousSpecificationString_ ) > 0 )
						{
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
						isSpecificationWaitingForConjunction_ = true;

						if( !isSpecificationGeneralization )
							isSkipClearWriteLevel_ = true;

						strcpy( commonVariables_->writeSentenceString, previousSpecificationString_ );

						if( ( lastFoundSpecificationWordItem = lastFoundSpecificationItem_->specificationWordItem() ) == NULL )		// Specification string
							lastFoundSpecificationItem_->clearSpecificationStringWriteLevel( NO_WRITE_LEVEL );
						else
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

	ResultType writeRelationContextWordToSentence( unsigned short grammarWordTypeNr, SpecificationItem *writeSpecificationItem )
		{
		WordItem *currentRelationWordItem = NULL;		// Start to search first word in function contextWordInAllWords
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelationContextWordToSentence";

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
			writeSpecificationItem->relationWordItem( currentRelationWordItem ) == NULL )	// Look ahead: This is the last relation word of the relation context
				{
				isRelationWaitingForConjunction_ = true;
				writeWordString_ = NULL;
				currentRelationWordItem->clearRelationWriteLevel( NO_WRITE_LEVEL );
				}

			lastFoundRelationWordItem_ = currentRelationWordItem;
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	WordWriteWords( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasFoundGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		hasFoundQuestionVerb_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;

		isSkipClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isRelationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		assumptionLevel_ = NO_ASSUMPTION_LEVEL;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		lastFoundSpecificationItem_ = NULL;
		lastFoundRelationWordItem_ = NULL;
		writeWordString_ = NULL;

		strcpy( lastSpecificationString_, EMPTY_STRING );
		strcpy( previousSpecificationString_, EMPTY_STRING );

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordWriteWords" );

		if( commonVariables_ != NULL )
			{
		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

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
		hasFoundSpecificationWord_ = false;
		hasFoundQuestionVerb_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;

		isSkipClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isRelationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		assumptionLevel_ = NO_ASSUMPTION_LEVEL;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		lastFoundSpecificationItem_ = NULL;
		lastFoundRelationWordItem_ = NULL;
		writeWordString_ = NULL;

		strcpy( lastSpecificationString_, EMPTY_STRING );
		strcpy( previousSpecificationString_, EMPTY_STRING );
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
		WriteResultType writeResult;
		bool hasRelationContext;
		bool isAssignment;
		bool isActiveAssignment;
		bool isInactiveAssignment;
		bool isArchivedAssignment;
		bool isCheckFeminineAndMasculineWord;
		bool isConditional;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPartOf;
		bool isPossessive;
		bool isQuestion;
		bool isGeneralization;
		bool isSpecification;
		bool isRelation;
		bool isUniqueRelation;
		bool isSpecificationGeneralization;
		bool isWordTypeNumeral;
		bool isWordTypeSingularNoun;
		bool isWordTypePluralNoun;
		bool isInsertSeparator = true;
		bool isGeneralizationFeminineWord = false;
		bool isGeneralizationMasculineWord = false;
		bool isSpecificationFeminineWord = false;
		bool isSpecificationMasculineWord = false;
		unsigned short definitionGrammarParameter;
		unsigned short definitionGrammarWordTypeNr;
		unsigned short generalizationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		unsigned int pronounContextNr = NO_CONTEXT_NR;
		WordItem *predefinedWordItem;
		WordItem *pronounWordItem;
		WordItem *specificationWordItem;
		char *predefinedWordString = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeWordsToSentence";

		hasFoundGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordString_ = NULL;

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
					isNegative = writeSpecificationItem->isNegative();
					isPartOf = writeSpecificationItem->isPartOf();
					isPossessive = writeSpecificationItem->isPossessive();
					isQuestion = writeSpecificationItem->isQuestion();
					isSpecificationGeneralization = writeSpecificationItem->isSpecificationGeneralization();
					isUniqueRelation = writeSpecificationItem->isUniqueRelation();

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
					generalizationWordTypeNr = writeSpecificationItem->generalizationWordTypeNr();

					generalizationContextNr = writeSpecificationItem->generalizationContextNr();
					specificationContextNr = writeSpecificationItem->specificationContextNr();
					relationContextNr = writeSpecificationItem->relationContextNr();

					if( definitionGrammarParameter > NO_GRAMMAR_PARAMETER &&
					( predefinedWordItem = myWordItem_->predefinedWordItem( definitionGrammarParameter ) ) != NULL )
						predefinedWordString = predefinedWordItem->wordTypeString( true, definitionGrammarWordTypeNr );

					isCheckFeminineAndMasculineWord = ( !isExclusiveSpecification &&
														!isPossessive &&
														!isQuestion );

					if( isCheckFeminineAndMasculineWord )
						{
						isGeneralizationFeminineWord = myWordItem_->isFeminineWord();
						isGeneralizationMasculineWord = myWordItem_->isMasculineWord();
						}

					if( ( specificationWordItem = writeSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( isCheckFeminineAndMasculineWord )
							{
							isSpecificationFeminineWord = specificationWordItem->isFeminineWord();
							isSpecificationMasculineWord = specificationWordItem->isMasculineWord();
							}
						}

					switch( definitionGrammarParameter )
						{
						// Symbols
						case WORD_PARAMETER_SYMBOL_COMMA:
						case WORD_PARAMETER_SYMBOL_COLON:
						case WORD_PARAMETER_SYMBOL_QUESTION_MARK:
						case WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED:
							isInsertSeparator = false;
							writeWordString_ = predefinedWordString;
							break;

						case WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK:
						case WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED:
							break;	// Skip these words

						// Ambiguity
						case WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:		// Typically for Dutch: in zowel ... als ...
							isRelationWaitingForConjunction_ = false;
							isSpecificationWaitingForConjunction_ = false;

							// Don't insert a break statement here

						case WORD_PARAMETER_NUMERAL_BOTH:				// Typically for English: in both ... and ...
						case WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:	// Typically for Dutch: in zowel ... als ...
							if( writeSpecificationItem->isSpecificationWithStaticAmiguity() )
								writeWordString_ = predefinedWordString;

							break;

						// Adjectives
						case WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR:
						case WORD_PARAMETER_ADJECTIVE_CALLED_PLURAL:
							writeWordString_ = predefinedWordString;
							break;

						case WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
						case WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE:
							if( writeSpecificationItem->isEveryGeneralization() &&
							!writeSpecificationItem->isSelfGeneratedAssumption() &&
							myWordItem_->isCorrectAdjective( definitionGrammarParameter, generalizationWordTypeNr ) )
								writeWordString_ = predefinedWordString;

							break;

						case WORD_PARAMETER_ADJECTIVE_NO:
						case WORD_PARAMETER_ADVERB_NOT:
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
							writeSpecificationItem->hasCurrentActiveSentenceNr() &&		// Only show the word "new" during the current sentence
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
							isUniqueRelation )
								writeWordString_ = predefinedWordString;

							break;

						case WORD_PARAMETER_ADVERB_AS:
							break;	// Skip these words

						case WORD_PARAMETER_ADVERB_ASSUMPTION_MAY_BE:
							if( checkAssumptionLevel( writeSpecificationItem ) == RESULT_OK )
								{
								if( assumptionLevel_ >= NUMBER_OF_ASSUMPTION_LEVELS )
									writeWordString_ = predefinedWordString;
								}
							else
								myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'may-be'" );

							break;

						case WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
							if( checkAssumptionLevel( writeSpecificationItem ) == RESULT_OK )
								{
								if( assumptionLevel_ == 2 )
									writeWordString_ = predefinedWordString;
								}
							else
								myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'possibly'" );

							break;

						case WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
							if( checkAssumptionLevel( writeSpecificationItem ) == RESULT_OK )
								{
								if( assumptionLevel_ == 1 )
									writeWordString_ = predefinedWordString;
								}
							else
								myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the assumption level for 'probably'" );

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
							writeSpecificationItem->isGeneralizationNoun() &&
							myWordItem_->isCorrectIndefiniteArticle( isGeneralizationFeminineWord, isGeneralizationMasculineWord, definitionGrammarParameter, generalizationWordTypeNr ) ) ||

							// Specification nouns
							( isSpecification &&
							writeSpecificationItem->isSpecificationNoun() &&
							writeSpecificationItem->isCorrectSpecificationArticle( isSpecificationFeminineWord, isSpecificationMasculineWord, definitionGrammarParameter ) ) ) )
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
							isUniqueRelation ) &&

							// Generalization noun
							( ( isGeneralization &&
							writeSpecificationItem->isGeneralizationNoun() &&
							myWordItem_->isCorrectDefiniteArticle( isGeneralizationFeminineWord, isGeneralizationMasculineWord, definitionGrammarParameter, generalizationWordTypeNr ) ) ||

							// Specification nouns
							( isSpecification &&
							writeSpecificationItem->isGeneralizationProperName() &&
							writeSpecificationItem->isSpecificationNoun() &&
							writeSpecificationItem->isCorrectSpecificationArticle( isSpecificationFeminineWord, isSpecificationMasculineWord, definitionGrammarParameter ) ) ) ) ||

							// Generalization proper name preceded-by-defined-article
							( isGeneralization &&
							writeSpecificationItem->isGeneralizationProperName() &&
							myWordItem_->isProperNamePrecededByDefiniteArticle( definitionGrammarParameter ) ) ||

							// Relation proper names preceded-by-defined-article
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
								isRelationWaitingForConjunction_ = false;
								isSpecificationWaitingForConjunction_ = false;
								writeWordString_ = predefinedWordString;
								}

							break;

						case WORD_PARAMETER_CONJUNCTION_OR:
							if( isExclusiveSpecification )
								{
								isRelationWaitingForConjunction_ = false;
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
						case WORD_PARAMETER_PREPOSITION_FOR:
						case WORD_PARAMETER_PREPOSITION_FROM:
						case WORD_PARAMETER_PREPOSITION_IN:
						case WORD_PARAMETER_PREPOSITION_TO:
						case WORD_PARAMETER_PREPOSITION_OF:
							if( !writeSpecificationItem->hasPrepositionParameter() ||
							writeSpecificationItem->prepositionParameter() == definitionGrammarParameter )
								writeWordString_ = predefinedWordString;

							break;

						// Verbs
						case WORD_PARAMETER_SINGULAR_VERB_IS:
						case WORD_PARAMETER_PLURAL_VERB_ARE:
							if( !isConditional &&
							!isPossessive &&

							( !isAssignment ||		// Specification
							isActiveAssignment ) &&

							( isQuestion ||
							hasFoundAllSpecificationWords_ ||
							!isSpecificationGeneralization ) )
								{
								writeWordString_ = predefinedWordString;

								if( isQuestion )
									{
									hasFoundQuestionVerb_ = true;
									isInsertSeparator = false;		// Specific for Spanish
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
									isInsertSeparator = false;		// Specific for Spanish
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
										// Force current tense, because the word 'previous' is used
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
									isInsertSeparator = false;		// Specific for Spanish
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
									isInsertSeparator = false;		// Specific for Spanish
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
						case WORD_PARAMETER_ADJECTIVE_CLEAR:
						case WORD_PARAMETER_ADJECTIVE_DONE:
						case WORD_PARAMETER_ADJECTIVE_DEFENSIVE:
						case WORD_PARAMETER_ADJECTIVE_EXCLUSIVE:
						case WORD_PARAMETER_ADJECTIVE_INVERTED:
						case WORD_PARAMETER_ADJECTIVE_MASCULINE:
						case WORD_PARAMETER_ADJECTIVE_FEMININE:

						// Nouns
						case WORD_PARAMETER_NOUN_DEVELOPER:
						case WORD_PARAMETER_NOUN_EXPERT:
						case WORD_PARAMETER_NOUN_FILE:
						case WORD_PARAMETER_NOUN_GRAMMAR_LANGUAGE:
						case WORD_PARAMETER_NOUN_INTERFACE_LANGUAGE:
						case WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
						case WORD_PARAMETER_NOUN_HEAD:
						case WORD_PARAMETER_NOUN_TAIL:
						case WORD_PARAMETER_NOUN_MIND:
						case WORD_PARAMETER_NOUN_NUMBER:
						case WORD_PARAMETER_NOUN_PASSWORD:
						case WORD_PARAMETER_NOUN_SOLVE_LEVEL:
						case WORD_PARAMETER_NOUN_SOLVE_METHOD:
						case WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
						case WORD_PARAMETER_NOUN_USER:
							if( ( !isQuestion ||
							hasFoundQuestionVerb_ ) &&

							( !isRelationWaitingForConjunction_ &&
							!isSpecificationWaitingForConjunction_ ) )
								{
								if( isGeneralization )
									{
									// Matching generalization word type
									if( generalizationWordTypeNr == definitionGrammarWordTypeNr )
										{
										// Generalization
										if( writeGeneralizationWordToSentence( ( !isPartOf && isSpecificationGeneralization && !hasFoundSingleSpecificationWord_ ), definitionGrammarWordTypeNr ) != RESULT_OK )
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
										// Matching specification word type
										writeSpecificationItem->specificationWordTypeNr() == definitionGrammarWordTypeNr )
											{
											// Specification
											if( writeSpecificationWordToSentence( isSpecificationGeneralization, isWordTypeNumeral, isWordTypePluralNoun, isWordTypeSingularNoun, isWritingCurrentSpecificationWordOnly, definitionGrammarWordTypeNr, writeSpecificationItem ) != RESULT_OK )
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
											if( writeRelationContextWordToSentence( definitionGrammarWordTypeNr, writeSpecificationItem ) != RESULT_OK )
												myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a relation context word to the sentence" );
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
							if( strlen( commonVariables_->writeSentenceString ) == 0 )
								strcpy( commonVariables_->writeSentenceString, writeWordString_ );
							else
								{
								isSkipClearWriteLevel_ = false;

								if( isInsertSeparator )
									strcat( commonVariables_->writeSentenceString, SPACE_STRING );

								strcat( commonVariables_->writeSentenceString, writeWordString_ );
								}

							if( isUnknownPluralOfNoun_ )
								{
								if( commonVariables_->currentInterfaceLanguageWordItem != NULL )
									{
									strcat( commonVariables_->writeSentenceString, commonVariables_->currentInterfaceLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) );
									strcat( commonVariables_->writeSentenceString, writeWordString_ );
									strcat( commonVariables_->writeSentenceString, commonVariables_->currentInterfaceLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
									}
								else
									myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current interface language word item is undefined" );
								}

							if( hasFoundGeneralizationWord_ )
								generalizationStartWordPosition_ = strlen( commonVariables_->writeSentenceString );
							else
								{
								if( hasFoundSpecificationWord_ )
									{
									specificationStartWordPosition_ = strlen( commonVariables_->writeSentenceString );

									// To recovery an unsuccessful specification match
									strcpy( previousSpecificationString_, lastSpecificationString_ );
									strcpy( lastSpecificationString_, commonVariables_->writeSentenceString );
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
		writeResult.isSkipClearWriteLevel = isSkipClearWriteLevel_;
		writeResult.result = commonVariables_->result;
		return writeResult;
		}
	};

/*************************************************************************
 *	"Your word is a lamp to guide my feet
 *	and a light for my path." (Psalm 119:105)
 *************************************************************************/
