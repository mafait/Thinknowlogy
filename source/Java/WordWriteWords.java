/*
 *	Class:			WordWriteWords
 *	Supports class:	WordItem
 *	Purpose:		To write the words of the sentences
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

class WordWriteWords
	{
	// Private constructible variables

	private boolean hasFoundAllSpecificationWords_;
	private boolean hasFoundAllSpecificationWordsBeforeConjunction_;
	private boolean hasFoundGeneralizationWord_;
	private boolean hasFoundSpecificationWord_;
	private boolean hasFoundQuestionVerb_;
	private boolean hasFoundSingleSpecificationWord_;
	private boolean hasFoundSpecificationGeneralizationVerb_;

	private boolean isSkipClearWriteLevel_;
	private boolean isSpecificationWaitingForConjunction_;
	private boolean isRelationWaitingForConjunction_;
	private boolean isUnknownPluralOfNoun_;

	private short assumptionLevel_;

	private int generalizationStartWordPosition_;
	private int specificationStartWordPosition_;

	private SpecificationItem lastFoundSpecificationItem_;

	private WordItem lastFoundRelationWordItem_;

	private String writeWordString_;

	private StringBuffer lastSpecificationStringBuffer_;
	private StringBuffer previousSpecificationStringBuffer_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean isSpecificationReasoningWordType( short specificationWordTypeNr )
		{
		return ( specificationWordTypeNr == Constants.WORD_TYPE_NUMERAL ||
				specificationWordTypeNr == Constants.WORD_TYPE_LETTER_SMALL ||
				specificationWordTypeNr == Constants.WORD_TYPE_LETTER_CAPITAL ||
				specificationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	private byte checkAssumptionLevel( SpecificationItem writeSpecificationItem )
		{
		SpecificationResultType specificationResult;

		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

		if( writeSpecificationItem != null )
			{
			if( !writeSpecificationItem.isSelfGeneratedQuestion() &&

			( writeSpecificationItem.hasAssumptionLevel() ||
			writeSpecificationItem.isSelfGeneratedAssumption() ) )
				{
				if( ( specificationResult = writeSpecificationItem.getAssumptionLevel() ).result == Constants.RESULT_OK )
					{
					if( specificationResult.hasPerformedRecalculation )
						assumptionLevel_ = specificationResult.assumptionLevel;
					else
						{
						if( specificationResult.assumptionLevel > Constants.NO_ASSUMPTION_LEVEL )
							{
							if( ( specificationResult = writeSpecificationItem.recalculateAssumptionLevel() ).result == Constants.RESULT_OK )
								assumptionLevel_ = specificationResult.assumptionLevel;
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to recalculate the assumption level" );
							}
						}
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to get the assumption level" );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeGeneralizationWordToSentence( boolean isPluralSpecificationGeneralization, short grammarWordTypeNr )
		{
		short writeWordTypeNr = grammarWordTypeNr;

		hasFoundGeneralizationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		if( ( writeWordString_ = myWordItem_.activeWordTypeString( isPluralSpecificationGeneralization ? Constants.WORD_TYPE_NOUN_PLURAL : grammarWordTypeNr ) ) == null )
			{
			if( isPluralSpecificationGeneralization &&
			grammarWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
			( writeWordString_ = myWordItem_.singularNounString() ) != null )
				{
				isUnknownPluralOfNoun_ = true;
				writeWordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;
				}
			}

		if( writeWordString_ != null )
			{
			if( myWordItem_.isGeneralizationWordTypeAlreadyWritten( writeWordTypeNr ) )
				writeWordString_ = null;
			else
				{
				if( myWordItem_.markGeneralizationWordTypeAsWritten( writeWordTypeNr ) == Constants.RESULT_OK )
					hasFoundGeneralizationWord_ = true;
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a word type of a generalization word as written" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationWordToSentence( boolean isSpecificationGeneralization, boolean isWordTypeNumeral, boolean isWordTypePluralNoun, boolean isWordTypeSingularNoun, boolean isWritingCurrentSpecificationWordOnly, short grammarWordTypeNr, SpecificationItem writeSpecificationItem )
		{
		boolean isAnsweredQuestion;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isSelfGenerated;
		boolean hasFoundUnwrittenWordType = false;
		boolean isNumberOfRelations = false;
		short assumptionLevel;
		short specificationWordTypeNr;
		short writeWordTypeNr = grammarWordTypeNr;
		int nContextRelations;
		int specificationCollectionNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem lastFoundSpecificationWordItem;

		hasFoundSpecificationWord_ = false;
		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordString_ = null;

		if( writeSpecificationItem != null )
			{
			isAnsweredQuestion = writeSpecificationItem.isAnsweredQuestion();

			if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAnsweredQuestion, writeSpecificationItem.isAssignment(), writeSpecificationItem.isInactiveAssignment(), writeSpecificationItem.isArchivedAssignment(), writeSpecificationItem.questionParameter() ) ) != null )
				{
				isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
				isNegative = writeSpecificationItem.isNegative();
				isPossessive = writeSpecificationItem.isPossessive();
				isSelfGenerated = writeSpecificationItem.isSelfGenerated();

				assumptionLevel = writeSpecificationItem.assumptionLevel();
				specificationWordTypeNr = writeSpecificationItem.specificationWordTypeNr();

				specificationCollectionNr = writeSpecificationItem.specificationCollectionNr();
				generalizationContextNr = writeSpecificationItem.generalizationContextNr();
				specificationContextNr = writeSpecificationItem.specificationContextNr();
				relationContextNr = writeSpecificationItem.relationContextNr();

				do	{
					if( currentSpecificationItem == writeSpecificationItem ||

					( !isWritingCurrentSpecificationWordOnly &&
					currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) )
						{
						if( currentSpecificationItem != writeSpecificationItem )
							hasFoundUnwrittenWordType = true;

						// Specification string
						if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) == null )
							{
							if( !currentSpecificationItem.isSpecificationStringAlreadyWritten() )
								{
								if( currentSpecificationItem.markSpecificationStringAsWritten() == Constants.RESULT_OK )
									{
									hasFoundSpecificationWord_ = true;
									lastFoundSpecificationItem_ = currentSpecificationItem;
									writeWordString_ = currentSpecificationItem.specificationString();

									if( hasFoundAllSpecificationWordsBeforeConjunction_ )
										{
										hasFoundAllSpecificationWords_ = true;
										hasFoundAllSpecificationWordsBeforeConjunction_ = false;
										isSkipClearWriteLevel_ = false;
										}
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification string \"" + currentSpecificationItem.specificationString() + "\" as written" );
								}
							}
						else	// Specification word
							{
							if( ( nContextRelations = currentSpecificationItem.nContextRelations() ) == 0 )
								{
								if( isPossessive &&
								specificationStartWordPosition_ == 0 &&		// To avoid looping in numbers
								currentSpecificationItem.hasRelationContext() &&
								isSpecificationReasoningWordType( grammarWordTypeNr ) )
									nContextRelations = myWordItem_.nContextWordsInAllWords( currentSpecificationItem.relationContextNr(), currentSpecificationWordItem );
								}

							if( ( nContextRelations == 0 &&
							specificationWordTypeNr == grammarWordTypeNr ) ||

							( nContextRelations == 1 &&
							isWordTypeSingularNoun ) ||

							( nContextRelations > 1 &&

							( isWordTypeNumeral ||
							isWordTypePluralNoun ) ) )
								{
								if( ( writeWordString_ = currentSpecificationWordItem.activeWordTypeString( grammarWordTypeNr ) ) == null )
									{
									if( isWordTypeNumeral )
										{
										if( specificationStartWordPosition_ == 0 )		// To avoid looping in numbers
											{
											// The word 'number' needs to be converted to the number of relations
											isNumberOfRelations = true;
											writeWordString_ = ( Constants.EMPTY_STRING + nContextRelations );
											}
										}
									else
										{
										if( isWordTypePluralNoun &&
										( writeWordString_ = currentSpecificationWordItem.singularNounString() ) != null )
											// The plural noun is unknown, but the singular is known. So, force a singular noun, but with a remark
											isUnknownPluralOfNoun_ = true;		// Force as singular noun
										else	// Must be hidden word type
											writeWordString_ = currentSpecificationWordItem.anyWordTypeString();
										}
									}

								if( !isNumberOfRelations &&
								writeWordString_ != null )
									{
									if( isUnknownPluralOfNoun_ )
										writeWordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;

									if( currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( writeWordTypeNr ) )
										writeWordString_ = null;
									else
										{
										if( currentSpecificationWordItem.markSpecificationWordTypeAsWritten( writeWordTypeNr ) == Constants.RESULT_OK )
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
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" as written" );
										}
									}
								}
							}
						}
					}
				while( writeWordString_ == null &&
				( currentSpecificationItem = currentSpecificationItem.nextSpecificationItemWithSameQuestionParameter( isAnsweredQuestion ) ) != null );
				}

			if( !hasFoundSpecificationWord_ &&
			!hasFoundAllSpecificationWords_ &&
			!hasFoundAllSpecificationWordsBeforeConjunction_ &&
			lastFoundSpecificationItem_ != null &&
			lastFoundSpecificationItem_.specificationWordTypeNr() == grammarWordTypeNr )
				{
				if( hasFoundUnwrittenWordType )
					{
					if( previousSpecificationStringBuffer_ != null &&
					previousSpecificationStringBuffer_.length() > 0 )
						{
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
						isSpecificationWaitingForConjunction_ = true;

						if( !isSpecificationGeneralization )
							isSkipClearWriteLevel_ = true;

						CommonVariables.writeSentenceStringBuffer = new StringBuffer( previousSpecificationStringBuffer_ );

						if( ( lastFoundSpecificationWordItem = lastFoundSpecificationItem_.specificationWordItem() ) == null )		// Specification string
							lastFoundSpecificationItem_.clearSpecificationStringWriteLevel( Constants.NO_WRITE_LEVEL );
						else
							lastFoundSpecificationWordItem.clearSpecificationWriteLevel( Constants.NO_WRITE_LEVEL );
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
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeRelationContextWordToSentence( short grammarWordTypeNr, SpecificationItem writeSpecificationItem )
		{
		WordItem currentRelationWordItem = null;		// Start to search first word in method contextWordInAllWords

		writeWordString_ = null;

		if( writeSpecificationItem != null )
			{
			do	{
				if( ( currentRelationWordItem = writeSpecificationItem.relationWordItem( currentRelationWordItem ) ) != null )
					{
					if( ( writeWordString_ = currentRelationWordItem.activeWordTypeString( grammarWordTypeNr ) ) != null )
						{
						if( currentRelationWordItem.isRelationWordTypeAlreadyWritten( grammarWordTypeNr ) )
							writeWordString_ = null;
						else
							{
							if( currentRelationWordItem.markRelationWordTypeAsWritten( grammarWordTypeNr ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark relation context word \"" + currentRelationWordItem.anyWordTypeString() + "\" as written" );
							}
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "Relation context word \"" + currentRelationWordItem.anyWordTypeString() + "\" doesn't have the requested word type" );
					}
				}
			while( writeWordString_ == null &&
			currentRelationWordItem != null );

			if( currentRelationWordItem != null &&
			lastFoundRelationWordItem_ != null &&
			lastFoundRelationWordItem_ != currentRelationWordItem &&
			writeSpecificationItem.relationWordItem( currentRelationWordItem ) == null )	// Look ahead: This is the last relation word of the relation context
				{
				isRelationWaitingForConjunction_ = true;
				writeWordString_ = null;
				currentRelationWordItem.clearRelationWriteLevel( Constants.NO_WRITE_LEVEL );
				}

			lastFoundRelationWordItem_ = currentRelationWordItem;
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected WordWriteWords( WordItem myWordItem )
		{
		String errorString = null;

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

		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		lastFoundSpecificationItem_ = null;
		lastFoundRelationWordItem_ = null;
		writeWordString_ = null;

		lastSpecificationStringBuffer_ = null;
		previousSpecificationStringBuffer_ = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + errorString + "\nMethod:\t%s\nError:\t\t%s.\n" );
				}
			}
		}


	// Protected methods

	protected void initializeWordWriteWordsVariables()
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

		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		lastFoundSpecificationItem_ = null;
		lastFoundRelationWordItem_ = null;
		writeWordString_ = null;

		lastSpecificationStringBuffer_ = null;
		previousSpecificationStringBuffer_ = null;
		}

	protected void initializeWordWriteWordsSpecificationVariables( boolean isQuestion, int startWordPosition )
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

	protected WriteResultType writeWordsToSentence( boolean isWritingCurrentSpecificationWordOnly, short answerParameter, GrammarItem definitionGrammarItem, SpecificationItem writeSpecificationItem )
		{
		WriteResultType writeResult = new WriteResultType();
		boolean hasRelationContext;
		boolean isAssignment;
		boolean isActiveAssignment;
		boolean isInactiveAssignment;
		boolean isArchivedAssignment;
		boolean isCheckFeminineAndMasculineWord;
		boolean isConditional;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPartOf;
		boolean isPossessive;
		boolean isQuestion;
		boolean isGeneralization;
		boolean isSpecification;
		boolean isRelation;
		boolean isUniqueRelation;
		boolean isSpecificationGeneralization;
		boolean isWordTypeNumeral;
		boolean isWordTypeSingularNoun;
		boolean isWordTypePluralNoun;
		boolean isInsertSeparator = true;
		boolean isGeneralizationFeminineWord = false;
		boolean isGeneralizationMasculineWord = false;
		boolean isSpecificationFeminineWord = false;
		boolean isSpecificationMasculineWord = false;
		short definitionGrammarParameter;
		short definitionGrammarWordTypeNr;
		short generalizationWordTypeNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		int pronounContextNr = Constants.NO_CONTEXT_NR;
		WordItem predefinedWordItem;
		WordItem pronounWordItem;
		WordItem specificationWordItem;
		String predefinedWordString = null;

		hasFoundGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordString_ = null;

		if( definitionGrammarItem != null )
			{
			if( definitionGrammarItem.hasWordType() )
				{
				if( writeSpecificationItem != null )
					{
					hasRelationContext = writeSpecificationItem.hasRelationContext();

					isAssignment = writeSpecificationItem.isAssignment();
					isActiveAssignment = writeSpecificationItem.isActiveAssignment();
					isInactiveAssignment = writeSpecificationItem.isInactiveAssignment();
					isArchivedAssignment = writeSpecificationItem.isArchivedAssignment();

					isConditional = writeSpecificationItem.isConditional();
					isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
					isNegative = writeSpecificationItem.isNegative();
					isPartOf = writeSpecificationItem.isPartOf();
					isPossessive = writeSpecificationItem.isPossessive();
					isQuestion = writeSpecificationItem.isQuestion();
					isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();
					isUniqueRelation = writeSpecificationItem.isUniqueRelation();

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

					definitionGrammarParameter = definitionGrammarItem.grammarParameter();
					definitionGrammarWordTypeNr = definitionGrammarItem.grammarWordTypeNr();
					generalizationWordTypeNr = writeSpecificationItem.generalizationWordTypeNr();

					generalizationContextNr = writeSpecificationItem.generalizationContextNr();
					specificationContextNr = writeSpecificationItem.specificationContextNr();
					relationContextNr = writeSpecificationItem.relationContextNr();

					if( definitionGrammarParameter > Constants.NO_GRAMMAR_PARAMETER &&
					( predefinedWordItem = myWordItem_.predefinedWordItem( definitionGrammarParameter ) ) != null )
						predefinedWordString = predefinedWordItem.wordTypeString( true, definitionGrammarWordTypeNr );

					isCheckFeminineAndMasculineWord = ( !isExclusiveSpecification &&
														!isPossessive &&
														!isQuestion );

					if( isCheckFeminineAndMasculineWord )
						{
						isGeneralizationFeminineWord = myWordItem_.isFeminineWord();
						isGeneralizationMasculineWord = myWordItem_.isMasculineWord();
						}

					if( ( specificationWordItem = writeSpecificationItem.specificationWordItem() ) != null )
						{
						if( isCheckFeminineAndMasculineWord )
							{
							isSpecificationFeminineWord = specificationWordItem.isFeminineWord();
							isSpecificationMasculineWord = specificationWordItem.isMasculineWord();
							}
						}

					switch( definitionGrammarParameter )
						{
						// Symbols
						case Constants.WORD_PARAMETER_SYMBOL_COMMA:
						case Constants.WORD_PARAMETER_SYMBOL_COLON:
						case Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK:
						case Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED:
							isInsertSeparator = false;
							writeWordString_ = predefinedWordString;
							break;

						case Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK:
						case Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED:
							break;	// Skip these words

						// Ambiguity
						case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:		// Typically for Dutch: in zowel ... als ...
							isRelationWaitingForConjunction_ = false;
							isSpecificationWaitingForConjunction_ = false;

							// Don't insert a break statement here

						case Constants.WORD_PARAMETER_NUMERAL_BOTH:				// Typically for English: in both ... and ...
						case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:	// Typically for Dutch: in zowel ... als ...
							if( writeSpecificationItem.isSpecificationWithStaticAmiguity() )
								writeWordString_ = predefinedWordString;

							break;

						// Adjectives
						case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR:
						case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_PLURAL:
							writeWordString_ = predefinedWordString;
							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
						case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE:
							if( writeSpecificationItem.isEveryGeneralization() &&
							!writeSpecificationItem.isSelfGeneratedAssumption() &&
							myWordItem_.isCorrectAdjective( definitionGrammarParameter, generalizationWordTypeNr ) )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_NO:
						case Constants.WORD_PARAMETER_ADVERB_NOT:
							if( isNegative )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL:
						case Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE:
							if( isInactiveAssignment &&
							!isQuestion )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL:
						case Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE:
							if( isActiveAssignment &&
							writeSpecificationItem.isGeneralizationAssignment() )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL:
						case Constants.WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE:
							if( isActiveAssignment &&
							writeSpecificationItem.hasCurrentActiveSentenceNr() &&		// Only show the word "new" during the current sentence
							writeSpecificationItem.isGeneralizationAssignment() )
								writeWordString_ = predefinedWordString;

							break;
/*
						case Constants.WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL:
						case Constants.WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE:
							break;	// Skip these words
*/
						// Adverbs
						case Constants.WORD_PARAMETER_ADVERB_ANYMORE:
							if( isNegative &&
							isUniqueRelation )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADVERB_AS:
							break;	// Skip these words

						case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_MAY_BE:
							if( checkAssumptionLevel( writeSpecificationItem ) == Constants.RESULT_OK )
								{
								if( assumptionLevel_ >= Constants.NUMBER_OF_ASSUMPTION_LEVELS )
									writeWordString_ = predefinedWordString;
								}
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'may-be'" );

							break;

						case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
							if( checkAssumptionLevel( writeSpecificationItem ) == Constants.RESULT_OK )
								{
								if( assumptionLevel_ == 2 )
									writeWordString_ = predefinedWordString;
								}
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'possibly'" );

							break;

						case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
							if( checkAssumptionLevel( writeSpecificationItem ) == Constants.RESULT_OK )
								{
								if( assumptionLevel_ == 1 )
									writeWordString_ = predefinedWordString;
								}
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'probably'" );

							break;

						// Interjections
						case Constants.WORD_PARAMETER_INTERJECTION_YES:
						case Constants.WORD_PARAMETER_INTERJECTION_NO:
							if( definitionGrammarParameter == answerParameter )
								writeWordString_ = predefinedWordString;

							break;

						// Articles
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE:
						case Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE:
							if( ( hasRelationContext ||
							!isArchivedAssignment ||
							!isExclusiveSpecification ) &&

							// Generalization noun
							( ( isGeneralization &&
							writeSpecificationItem.isGeneralizationNoun() &&
							myWordItem_.isCorrectIndefiniteArticle( isGeneralizationFeminineWord, isGeneralizationMasculineWord, definitionGrammarParameter, generalizationWordTypeNr ) ) ||

							// Specification nouns
							( isSpecification &&
							writeSpecificationItem.isSpecificationNoun() &&
							writeSpecificationItem.isCorrectSpecificationArticle( isSpecificationFeminineWord, isSpecificationMasculineWord, definitionGrammarParameter ) ) ) )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL:
						case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE:
						case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE:
						case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL:
						case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE:
						case Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE:
							if( ( isAssignment &&

							( !isArchivedAssignment ||
							isUniqueRelation ) &&

							// Generalization noun
							( ( isGeneralization &&
							writeSpecificationItem.isGeneralizationNoun() &&
							myWordItem_.isCorrectDefiniteArticle( isGeneralizationFeminineWord, isGeneralizationMasculineWord, definitionGrammarParameter, generalizationWordTypeNr ) ) ||

							// Specification nouns
							( isSpecification &&
							writeSpecificationItem.isGeneralizationProperName() &&
							writeSpecificationItem.isSpecificationNoun() &&
							writeSpecificationItem.isCorrectSpecificationArticle( isSpecificationFeminineWord, isSpecificationMasculineWord, definitionGrammarParameter ) ) ) ) ||

							// Generalization proper name preceded-by-defined-article
							( isGeneralization &&
							writeSpecificationItem.isGeneralizationProperName() &&
							myWordItem_.isProperNamePrecededByDefiniteArticle( definitionGrammarParameter ) ) ||

							// Relation proper names preceded-by-defined-article
							( isRelation &&
							writeSpecificationItem.isRelationProperName() &&
							writeSpecificationItem.isCorrectRelationArticle( definitionGrammarParameter ) ) )
								writeWordString_ = predefinedWordString;

							break;

						// Conjunctions
						case Constants.WORD_PARAMETER_CONJUNCTION_AND:
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

						case Constants.WORD_PARAMETER_CONJUNCTION_OR:
							if( isExclusiveSpecification )
								{
								isRelationWaitingForConjunction_ = false;
								isSpecificationWaitingForConjunction_ = false;
								writeWordString_ = predefinedWordString;
								}

							break;

						// Nouns
						case Constants.WORD_PARAMETER_NOUN_INFORMATION:
						case Constants.WORD_PARAMETER_NOUN_VALUE:
							break;	// Skip these words

						case Constants.WORD_PARAMETER_NOUN_PART:
							if( isPartOf )
								writeWordString_ = predefinedWordString;

							break;

						// Singular pronouns
						case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_I_ME_MY_MINE:
						case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_INFORMAL:
						case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_FORMAL:
						case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_HE_HIM_HIS_HIS:
						case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_SHE_HER_HER_HERS:
						case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_IT_ITS_ITS_ITS:

						// Plural pronouns
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_FEMININE:
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_MASCULINE:
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_FEMININE:
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_MASCULINE:
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_YOU_ALL:
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_FEMININE:
						case Constants.WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_MASCULINE:
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

								if( pronounContextNr > Constants.NO_CONTEXT_NR )
									{
									if( ( pronounWordItem = myWordItem_.predefinedWordItem( definitionGrammarParameter ) ) != null )
										{
										if( pronounWordItem.contextWordTypeNrInWord( pronounContextNr ) == definitionGrammarWordTypeNr )
											writeWordString_ = predefinedWordString;
										}
									else
										myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find the pronoun word with the definition grammar parameter" );
									}
								}

							break;

						case Constants.WORD_PARAMETER_PREPOSITION_ABOUT:
						case Constants.WORD_PARAMETER_PREPOSITION_FOR:
						case Constants.WORD_PARAMETER_PREPOSITION_FROM:
						case Constants.WORD_PARAMETER_PREPOSITION_IN:
						case Constants.WORD_PARAMETER_PREPOSITION_TO:
						case Constants.WORD_PARAMETER_PREPOSITION_OF:
							if( !writeSpecificationItem.hasPrepositionParameter() ||
							writeSpecificationItem.prepositionParameter() == definitionGrammarParameter )
								writeWordString_ = predefinedWordString;

							break;

						// Verbs
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IS:
						case Constants.WORD_PARAMETER_PLURAL_VERB_ARE:
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

						case Constants.WORD_PARAMETER_SINGULAR_VERB_WAS:
						case Constants.WORD_PARAMETER_PLURAL_VERB_WERE:
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

								if( !isExclusiveSpecification ||
								isArchivedAssignment ||
								!writeSpecificationItem.hasGeneralizationCollection() )
									writeWordString_ = predefinedWordString;
								else
									{
									if( myWordItem_.firstAssignmentItem( false, true, true, isNegative, isPossessive, writeSpecificationItem.questionParameter(), relationContextNr, specificationWordItem ) == null )
										writeWordString_ = predefinedWordString;
									else
										{
										// Force current tense, because the word 'previous' is used
										if( ( predefinedWordItem = myWordItem_.predefinedWordItem( definitionGrammarParameter == Constants.WORD_PARAMETER_SINGULAR_VERB_WAS ? Constants.WORD_PARAMETER_SINGULAR_VERB_IS : Constants.WORD_PARAMETER_PLURAL_VERB_ARE ) ) != null )
											{
											if( ( writeWordString_ = predefinedWordItem.wordTypeString( true, definitionGrammarWordTypeNr ) ) == null )
												myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't get the word type string from the predefined word with word parameter " + definitionGrammarParameter );
											}
										else
											myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find the predefined word with word parameter " + definitionGrammarParameter );
										}
									}
								}

							break;

						case Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_BE:
						case Constants.WORD_PARAMETER_PLURAL_VERB_CAN_BE:
							if( isConditional &&
							!isPossessive &&

							( !isAssignment ||		// Specification
							isActiveAssignment ) )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_SINGULAR_VERB_HAD:
						case Constants.WORD_PARAMETER_PLURAL_VERB_HAD:
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

						case Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_HAVE:
						case Constants.WORD_PARAMETER_PLURAL_VERB_CAN_HAVE:
							if( isConditional &&
							isPossessive )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_SINGULAR_VERB_HAS:
						case Constants.WORD_PARAMETER_PLURAL_VERB_HAVE:
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

						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE:
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO:
							break;	// Skip these words

						// Selection words
						case Constants.WORD_PARAMETER_SELECTION_IF:
						case Constants.WORD_PARAMETER_SELECTION_THEN:
						case Constants.WORD_PARAMETER_SELECTION_ELSE:
							break;	// Skip these words

						case Constants.NO_GRAMMAR_PARAMETER:

						// Adjectives
						case Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED:
						case Constants.WORD_PARAMETER_ADJECTIVE_BUSY:
						case Constants.WORD_PARAMETER_ADJECTIVE_CLEAR:
						case Constants.WORD_PARAMETER_ADJECTIVE_DONE:
						case Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE:
						case Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE:
						case Constants.WORD_PARAMETER_ADJECTIVE_INVERTED:
						case Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE:
						case Constants.WORD_PARAMETER_ADJECTIVE_FEMININE:

						// Nouns
						case Constants.WORD_PARAMETER_NOUN_DEVELOPER:
						case Constants.WORD_PARAMETER_NOUN_EXPERT:
						case Constants.WORD_PARAMETER_NOUN_FILE:
						case Constants.WORD_PARAMETER_NOUN_GRAMMAR_LANGUAGE:
						case Constants.WORD_PARAMETER_NOUN_INTERFACE_LANGUAGE:
						case Constants.WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
						case Constants.WORD_PARAMETER_NOUN_HEAD:
						case Constants.WORD_PARAMETER_NOUN_TAIL:
						case Constants.WORD_PARAMETER_NOUN_MIND:
						case Constants.WORD_PARAMETER_NOUN_NUMBER:
						case Constants.WORD_PARAMETER_NOUN_PASSWORD:
						case Constants.WORD_PARAMETER_NOUN_SOLVE_LEVEL:
						case Constants.WORD_PARAMETER_NOUN_SOLVE_METHOD:
						case Constants.WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
						case Constants.WORD_PARAMETER_NOUN_USER:
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
										if( writeGeneralizationWordToSentence( ( !isPartOf && isSpecificationGeneralization && !hasFoundSingleSpecificationWord_ ), definitionGrammarWordTypeNr ) != Constants.RESULT_OK )
											myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the generalization word to the sentence" );
										}
									}
								else
									{
									if( isSpecification )
										{
										isWordTypeNumeral = definitionGrammarItem.isNumeral();
										isWordTypePluralNoun = definitionGrammarItem.isPluralNoun();
										isWordTypeSingularNoun = definitionGrammarItem.isSingularNoun();

										if( isWordTypeNumeral ||
										isWordTypePluralNoun ||
										// Matching specification word type
										writeSpecificationItem.specificationWordTypeNr() == definitionGrammarWordTypeNr )
											{
											// Specification
											if( writeSpecificationWordToSentence( isSpecificationGeneralization, isWordTypeNumeral, isWordTypePluralNoun, isWordTypeSingularNoun, isWritingCurrentSpecificationWordOnly, definitionGrammarWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
												myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word to the sentence" );
											}
										}
									else
										{
										if( isRelation &&
										// Matching relation word type
										writeSpecificationItem.relationWordTypeNr() == definitionGrammarWordTypeNr )
											{
											// Relation context
											if( writeRelationContextWordToSentence( definitionGrammarWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
												myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a relation context word to the sentence" );
											}
										}
									}
								}

							break;

						default:
							myWordItem_.startErrorInWord( 1, moduleNameString_, "I don't know how to handle the grammar definition with grammar parameter: " + definitionGrammarParameter );
						}

					if( CommonVariables.result == Constants.RESULT_OK )
						{
						if( writeWordString_ != null )
							{
							if( CommonVariables.writeSentenceStringBuffer == null )
								CommonVariables.writeSentenceStringBuffer = new StringBuffer( writeWordString_ );
							else
								{
								isSkipClearWriteLevel_ = false;

								if( isInsertSeparator )
									CommonVariables.writeSentenceStringBuffer.append( Constants.SPACE_STRING );

								CommonVariables.writeSentenceStringBuffer.append( writeWordString_ );
								}

							if( isUnknownPluralOfNoun_ )
								{
								if( CommonVariables.currentInterfaceLanguageWordItem != null )
									CommonVariables.writeSentenceStringBuffer.append( CommonVariables.currentInterfaceLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) + writeWordString_ + CommonVariables.currentInterfaceLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
								else
									myWordItem_.startErrorInWord( 1, moduleNameString_, "The current interface language word item is undefined" );
								}

							if( hasFoundGeneralizationWord_ )
								generalizationStartWordPosition_ = CommonVariables.writeSentenceStringBuffer.length();
							else
								{
								if( hasFoundSpecificationWord_ )
									{
									specificationStartWordPosition_ = CommonVariables.writeSentenceStringBuffer.length();

									// To recovery an unsuccessful specification match
									previousSpecificationStringBuffer_ = lastSpecificationStringBuffer_;
									lastSpecificationStringBuffer_ = new StringBuffer( CommonVariables.writeSentenceStringBuffer );
									}
								}
							}
						}
					}
				else
					myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );
				}
			else
				myWordItem_.startErrorInWord( 1, moduleNameString_, "The given grammar definition has no word type" );
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given definition grammar item is undefined" );

		writeResult.hasFoundWordToWrite = ( writeWordString_ != null );
		writeResult.isSkipClearWriteLevel = isSkipClearWriteLevel_;
		writeResult.result = CommonVariables.result;
		return writeResult;
		}
	};

/*************************************************************************
 *
 *	"Your word is a lamp to guide my feet
 *	and a light for my path." (Psalm 119:105)
 *
 *************************************************************************/
