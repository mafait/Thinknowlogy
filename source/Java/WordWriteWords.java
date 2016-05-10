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

class WordWriteWords
	{
	// Private constructible variables

	private boolean hasFoundAllSpecificationWords_;
	private boolean hasFoundAllSpecificationWordsBeforeConjunction_;
	private boolean hasFoundGeneralizationWord_;
	private boolean hasFoundQuestionVerb_;
	private boolean hasFoundSingleSpecificationWord_;
	private boolean hasFoundSpecificationGeneralizationVerb_;
	private boolean hasFoundSpecificationWord_;

	private boolean isSkippingClearWriteLevel_;
	private boolean isSpecificationWaitingForConjunction_;
	private boolean isUnknownPluralOfNoun_;

	private short writeWordTypeNr_;

	private int lastNumeralWordPosition_;
	private int generalizationStartWordPosition_;
	private int specificationStartWordPosition_;

	private SpecificationItem mixedWordTypesSpecificationItem_;
	private SpecificationItem lastFoundSpecificationItem_;

	private WordItem lastFoundRelationWordItem_;

	private String writeWordString_;

	private StringBuffer lastSpecificationStringBuffer_;
	private StringBuffer previousSpecificationStringBuffer_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

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

	private byte writeSpecificationWordToSentence( boolean isPossessive, boolean isWordTypeNumeral, boolean isWordTypePluralNoun, short grammarWordTypeNr, int relationContextNr, SpecificationItem writeSpecificationItem )
		{
		boolean isNumberOfRelations = false;
		short specificationWordTypeNr;
		int nContextRelations;
		WordItem specificationWordItem;
		String specificationWordString;

		if( writeSpecificationItem != null )
			{
			// Specification string
			if( ( specificationWordItem = writeSpecificationItem.specificationWordItem() ) == null )
				{
				if( !writeSpecificationItem.isSpecificationStringAlreadyWritten() )
					{
					if( writeSpecificationItem.markSpecificationStringAsWritten() == Constants.RESULT_OK )
						{
						hasFoundSpecificationWord_ = true;
						lastFoundSpecificationItem_ = writeSpecificationItem;
						writeWordString_ = writeSpecificationItem.specificationString();
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification string \"" + writeSpecificationItem.specificationString() + "\" as written" );
					}
				}
			else
				{
				// Specification word
				specificationWordTypeNr = writeSpecificationItem.specificationWordTypeNr();

				if( !specificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) )
					{
					// Number of relation words of user specification item
					if( ( nContextRelations = writeSpecificationItem.nContextRelations() ) == 0 )
						{
						// Calculated number of relation words of a self-generated possessive specification
						if( isPossessive &&
						relationContextNr > Constants.NO_CONTEXT_NR )
							nContextRelations = myWordItem_.nContextWordsInAllWords( relationContextNr, specificationWordItem );
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
						if( ( specificationWordString = specificationWordItem.activeWordTypeString( grammarWordTypeNr ) ) == null )
							{
							if( isWordTypeNumeral )
								{
								// To avoid looping in numbers
								if( lastNumeralWordPosition_ == 0 )
									{
									// The word 'number' needs to be converted to the number of relation words
									isNumberOfRelations = true;
									lastNumeralWordPosition_ = specificationStartWordPosition_;
									writeWordString_ = ( Constants.EMPTY_STRING + nContextRelations );
									}
								}
							else
								{
								if( isWordTypePluralNoun )
									{
									if( ( writeWordString_ = specificationWordItem.singularNounString() ) != null )
										{
										// The plural noun is unknown, but the singular noun is known.
										// So, force a singular noun, but add a 'plural noun is unknown' text.
										isUnknownPluralOfNoun_ = true;
										writeWordTypeNr_ = Constants.WORD_TYPE_NOUN_SINGULAR;
										}
									}
								else
									// Must be hidden word type
									writeWordString_ = specificationWordItem.anyWordTypeString();
								}
							}
						else
							writeWordString_ = specificationWordString;

						if( !isNumberOfRelations &&
						writeWordString_ != null )
							{
							if( specificationWordItem.markSpecificationWordTypeAsWritten( writeWordTypeNr_ ) == Constants.RESULT_OK )
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
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification word \"" + specificationWordItem.anyWordTypeString() + "\" as written" );
							}
						}
					}
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationWordToSentence( boolean isSpecificationGeneralization, boolean isWordTypeNumeral, boolean isWordTypePluralNoun, boolean isWritingCurrentSpecificationWordOnly, short grammarWordTypeNr, SpecificationItem writeSpecificationItem )
		{
		boolean isAnsweredQuestion;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isSelfGenerated;
		boolean hasSkippedDifferentSpecification = false;
		short assumptionLevel;
		int specificationCollectionNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		SpecificationItem currentSpecificationItem;
		WordItem lastFoundSpecificationWordItem;

		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = grammarWordTypeNr;

		writeWordString_ = null;

		if( writeSpecificationItem != null )
			{
			isPossessive = writeSpecificationItem.isPossessive();
			relationContextNr = writeSpecificationItem.relationContextNr();

			if( isWritingCurrentSpecificationWordOnly )
				{
				if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, writeSpecificationItem ) != Constants.RESULT_OK )
					myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word only to the sentence" );
				}
			else
				{
				isAnsweredQuestion = writeSpecificationItem.isAnsweredQuestion();
				isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
				isNegative = writeSpecificationItem.isNegative();
				isSelfGenerated = writeSpecificationItem.isSelfGenerated();

				assumptionLevel = writeSpecificationItem.assumptionLevel();

				specificationCollectionNr = writeSpecificationItem.specificationCollectionNr();
				generalizationContextNr = writeSpecificationItem.generalizationContextNr();
				specificationContextNr = writeSpecificationItem.specificationContextNr();

				if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAnsweredQuestion, writeSpecificationItem.isAssignment(), writeSpecificationItem.isInactiveAssignment(), writeSpecificationItem.isArchivedAssignment(), writeSpecificationItem.questionParameter() ) ) != null )
					{
					do	{
						if( currentSpecificationItem == writeSpecificationItem ||
						currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, Constants.WORD_TYPE_UNDEFINED, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
							{
							if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, currentSpecificationItem ) == Constants.RESULT_OK )
								{
								if( writeWordString_ == null &&
								currentSpecificationItem != writeSpecificationItem )
									{
									hasSkippedDifferentSpecification = true;

									if( writeSpecificationItem.specificationWordTypeNr() != currentSpecificationItem.specificationWordTypeNr() )
										mixedWordTypesSpecificationItem_ = currentSpecificationItem;
									}
								}
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word to the sentence" );
							}
						}
					while( writeWordString_ == null &&
					( currentSpecificationItem = currentSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != null );
					}
				}

			if( !hasFoundSpecificationWord_ &&
			!hasFoundAllSpecificationWords_ &&
			!hasFoundAllSpecificationWordsBeforeConjunction_ &&
			lastFoundSpecificationItem_ != null &&
			lastFoundSpecificationItem_.specificationWordTypeNr() == grammarWordTypeNr )
				{
				if( hasSkippedDifferentSpecification )
					{
					if( previousSpecificationStringBuffer_ != null &&
					previousSpecificationStringBuffer_.length() > 0 )
						{
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
						isSpecificationWaitingForConjunction_ = true;

						if( !isSpecificationGeneralization )
							isSkippingClearWriteLevel_ = true;

						lastNumeralWordPosition_ = 0;

						CommonVariables.writtenSentenceStringBuffer = new StringBuffer( previousSpecificationStringBuffer_ );

						if( ( lastFoundSpecificationWordItem = lastFoundSpecificationItem_.specificationWordItem() ) != null )
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

	private byte writeRelationWordToSentence( short grammarWordTypeNr, SpecificationItem writeSpecificationItem )
		{
		// Start to search first word in method contextWordInAllWords
		WordItem currentRelationWordItem = null;

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
			// Look ahead: This is the last relation word of the relation context
			writeSpecificationItem.relationWordItem( currentRelationWordItem ) == null )
				writeWordString_ = null;

			lastFoundRelationWordItem_ = currentRelationWordItem;
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private SpecificationResultType checkAssumptionLevel( SpecificationItem writeSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		short previousAssumptionLevel;

		if( writeSpecificationItem != null )
			{
			if( ( specificationResult = writeSpecificationItem.getAssumptionLevel() ).result == Constants.RESULT_OK )
				{
				if( ( previousAssumptionLevel = specificationResult.assumptionLevel ) > Constants.NO_ASSUMPTION_LEVEL )
					{
					if( ( specificationResult = writeSpecificationItem.recalculateAssumptionLevel() ).result == Constants.RESULT_OK )
						{
						if( specificationResult.assumptionLevel != previousAssumptionLevel )
							myWordItem_.startErrorInWord( 1, moduleNameString_, "The assumption level of the given write specification item has changed during the process, but it isn't recalculated. So, this specification may have a recalculation or update issue" );
						}
					else
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to recalculate the assumption level" );
					}
				}
			else
				myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to get the assumption level" );
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}


	// Constructor / deconstructor

	protected WordWriteWords( WordItem myWordItem )
		{
		String errorString = null;

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

		writeWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		lastNumeralWordPosition_ = 0;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		mixedWordTypesSpecificationItem_ = null;
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
				Console.addError( "\nClass:" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nMethod:\t" + errorString + "\nError:\t\t%s.\n" );
				}
			}
		}


	// Protected methods

	protected void initializeWordWriteWordsVariables()
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

		mixedWordTypesSpecificationItem_ = null;
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
		SpecificationResultType specificationResult;
		WriteResultType writeResult = new WriteResultType();
		boolean hasRelationContext;
		boolean isAssignment;
		boolean isActiveAssignment;
		boolean isInactiveAssignment;
		boolean isArchivedAssignment;
		boolean isConditional;
		boolean isExclusiveSpecification;
		boolean isGeneralization;
		boolean isGeneralizationNoun;
		boolean isGeneralizationProperName;
		boolean isNegative;
		boolean isPartOf;
		boolean isPossessive;
		boolean isQuestion;
		boolean isRelation;
		boolean isSelfGeneratedAssumption;
		boolean isSelfGeneratedQuestion;
		boolean isSpecification;
		boolean isSpecificationGeneralization;
		boolean isSpecificationNoun;
		boolean isUniqueUserRelation;
		boolean isWordTypeNumeral;
		boolean isWordTypeSingularNoun;
		boolean isWordTypePluralNoun;
		boolean isInsertingSeparator = true;
		short assumptionLevel;
		short definitionGrammarParameter;
		short definitionGrammarWordTypeNr;
		short generalizationWordTypeNr;
		short specificationWordTypeNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		int pronounContextNr = Constants.NO_CONTEXT_NR;
		WordItem predefinedWordItem;
		WordItem pronounWordItem;
		WordItem relationWordItem;
		WordItem specificationWordItem;
		WordItem currentLanguageWordItem;
		StringBuffer writtenSentenceStringBuffer;
		String predefinedWordString = null;

		hasFoundGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

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
					isGeneralizationNoun = writeSpecificationItem.isGeneralizationNoun();
					isGeneralizationProperName = writeSpecificationItem.isGeneralizationProperName();
					isNegative = writeSpecificationItem.isNegative();
					isPartOf = writeSpecificationItem.isPartOf();
					isPossessive = writeSpecificationItem.isPossessive();
					isQuestion = writeSpecificationItem.isQuestion();
					isSelfGeneratedAssumption = writeSpecificationItem.isSelfGeneratedAssumption();
					isSelfGeneratedQuestion = writeSpecificationItem.isSelfGeneratedQuestion();
					isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();
					isSpecificationNoun = writeSpecificationItem.isSpecificationNoun();
					isUniqueUserRelation = writeSpecificationItem.isUniqueUserRelation();

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

					assumptionLevel = writeSpecificationItem.assumptionLevel();
					generalizationWordTypeNr = writeSpecificationItem.generalizationWordTypeNr();
					specificationWordTypeNr = writeSpecificationItem.specificationWordTypeNr();

					generalizationContextNr = writeSpecificationItem.generalizationContextNr();
					specificationContextNr = writeSpecificationItem.specificationContextNr();
					relationContextNr = writeSpecificationItem.relationContextNr();

					specificationWordItem = writeSpecificationItem.specificationWordItem();

					if( definitionGrammarParameter > Constants.NO_GRAMMAR_PARAMETER &&
					( predefinedWordItem = myWordItem_.predefinedWordItem( definitionGrammarParameter ) ) != null )
						predefinedWordString = predefinedWordItem.wordTypeString( true, definitionGrammarWordTypeNr );

					switch( definitionGrammarParameter )
						{
						// Symbols
						case Constants.WORD_PARAMETER_SYMBOL_COMMA:
						case Constants.WORD_PARAMETER_SYMBOL_COLON:
						case Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK:
						case Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK_INVERTED:
							isInsertingSeparator = false;
							writeWordString_ = predefinedWordString;
							break;

						case Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK:
						case Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK_INVERTED:
							break;	// Skip these words

						// Ambiguity
						// Typically for Dutch: in zowel ... als ...
						case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
							isSpecificationWaitingForConjunction_ = false;

							// Don't insert a break statement here

						// Typically for English: in both ... and ...
						case Constants.WORD_PARAMETER_NUMERAL_BOTH:
						// Typically for Dutch: in zowel ... als ...
						case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:
							if( writeSpecificationItem.isSpecificationWithStaticAmiguity() )
								writeWordString_ = predefinedWordString;

							break;

						// Adjectives
						case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_PLURAL:
							if( hasRelationContext )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE:
							if( hasRelationContext &&
							myWordItem_.nContextWordsInAllWords( relationContextNr, specificationWordItem ) == 1 )
								{
								if( ( relationWordItem = writeSpecificationItem.relationWordItem() ) != null )
									writeWordString_ = predefinedWordString;
								else
									myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find any relation word" );
								}

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE:
							if( hasRelationContext &&
							myWordItem_.nContextWordsInAllWords( relationContextNr, specificationWordItem ) == 1 )
								{
								if( ( relationWordItem = writeSpecificationItem.relationWordItem() ) != null )
									{
									if( relationWordItem.isMale() ||
									!relationWordItem.hasFeminineProperNameEnding() )
										writeWordString_ = predefinedWordString;
									}
								else
									myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find any relation word" );
								}

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
						case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE:
							if( !isSelfGeneratedAssumption &&
							writeSpecificationItem.isEveryGeneralization() &&
							myWordItem_.isCorrectAdjective( definitionGrammarParameter, generalizationWordTypeNr ) )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADJECTIVE_NO:
						case Constants.WORD_PARAMETER_ADVERB_NOT:
						case Constants.WORD_PARAMETER_ADVERB_NOT_FRENCH:
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
							// Only show the word "new" during the current sentence
							writeSpecificationItem.hasCurrentActiveSentenceNr() &&
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
							isUniqueUserRelation )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADVERB_AS:
							break;	// Skip these words

						case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
							if( isSelfGeneratedAssumption )
								{
								if( ( specificationResult = checkAssumptionLevel( writeSpecificationItem ) ).result == Constants.RESULT_OK )
									assumptionLevel = specificationResult.assumptionLevel;
								else
									myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'maybe'" );
								}
							else
								{
								if( isSelfGeneratedQuestion )
									// Self-generated question shouldn't show its assumption level
									assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
								}

							if( assumptionLevel >= Constants.NUMBER_OF_ASSUMPTION_LEVELS )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
							if( isSelfGeneratedAssumption )
								{
								if( ( specificationResult = checkAssumptionLevel( writeSpecificationItem ) ).result == Constants.RESULT_OK )
									assumptionLevel = specificationResult.assumptionLevel;
								else
									myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'possibly'" );
								}
							else
								{
								if( isSelfGeneratedQuestion )
									// Self-generated question shouldn't show its assumption level
									assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
								}

							if( assumptionLevel == 2 )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
							if( isSelfGeneratedAssumption )
								{
								if( ( specificationResult = checkAssumptionLevel( writeSpecificationItem ) ).result == Constants.RESULT_OK )
									assumptionLevel = specificationResult.assumptionLevel;
								else
									myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'probably'" );
								}
							else
								{
								if( isSelfGeneratedQuestion )
									// Self-generated question shouldn't show its assumption level
									assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
								}

							if( assumptionLevel == 1 )
								writeWordString_ = predefinedWordString;

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
							isGeneralizationNoun &&
							myWordItem_.isCorrectIndefiniteArticle( definitionGrammarParameter, generalizationWordTypeNr ) ) ||

							// Specification nouns
							( isSpecification &&
							isSpecificationNoun &&

							( ( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_PLURAL &&
							mixedWordTypesSpecificationItem_ == null &&
							writeSpecificationItem.isCorrectSpecificationArticle( definitionGrammarParameter ) ) ||

							( mixedWordTypesSpecificationItem_ != null &&
							mixedWordTypesSpecificationItem_.isCorrectSpecificationArticle( definitionGrammarParameter ) ) ) ) ) )
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
							isUniqueUserRelation ) &&

							// Generalization noun
							( ( isGeneralization &&
							isGeneralizationNoun &&

							( !isArchivedAssignment ||
							!writeSpecificationItem.isEveryGeneralization() ) &&

							myWordItem_.isCorrectDefiniteArticle( definitionGrammarParameter, generalizationWordTypeNr ) ) ||

							// Specification nouns
							( isSpecification &&
							isGeneralizationProperName &&
							isSpecificationNoun &&
							writeSpecificationItem.isCorrectSpecificationArticle( definitionGrammarParameter ) ) ) ) ||

							// Generalization proper name preceded by defined article
							( isGeneralization &&
							isGeneralizationProperName &&
							myWordItem_.isProperNamePrecededByDefiniteArticle( definitionGrammarParameter ) ) ||

							// Relation proper names preceded by defined article
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
								isSpecificationWaitingForConjunction_ = false;
								writeWordString_ = predefinedWordString;
								}

							break;

						case Constants.WORD_PARAMETER_CONJUNCTION_OR:
							if( isExclusiveSpecification )
								{
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
						case Constants.WORD_PARAMETER_PREPOSITION_FROM:
						case Constants.WORD_PARAMETER_PREPOSITION_IN:
						case Constants.WORD_PARAMETER_PREPOSITION_TO:
						case Constants.WORD_PARAMETER_PREPOSITION_OF:
							if( !writeSpecificationItem.hasPrepositionParameter() ||
							writeSpecificationItem.prepositionParameter() == definitionGrammarParameter )
								writeWordString_ = predefinedWordString;

							break;

						case Constants.WORD_PARAMETER_PREPOSITION_FRENCH_A:
							break;

						// Verbs
						case Constants.WORD_PARAMETER_SINGULAR_VERB_IS:
						case Constants.WORD_PARAMETER_PLURAL_VERB_ARE:
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
									// Typically for Spanish
									isInsertingSeparator = false;
									}

								if( isSpecificationGeneralization )
									hasFoundSpecificationGeneralizationVerb_ = true;

								if( isArchivedAssignment ||
								!isExclusiveSpecification ||
								isQuestion ||
								!writeSpecificationItem.hasGeneralizationCollection() )
									writeWordString_ = predefinedWordString;
								else
									{
									if( myWordItem_.firstNonQuestionAssignmentItem( false, true, true, isNegative, isPossessive, relationContextNr, specificationWordItem ) == null )
										writeWordString_ = predefinedWordString;
									else
										{
										// Force current tense, because the word "previous" is used
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
									// Typically for Spanish
									isInsertingSeparator = false;
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
									// Typically for Spanish
									isInsertingSeparator = false;
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
						case Constants.WORD_PARAMETER_ADJECTIVE_DONE:
						case Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE:
						case Constants.WORD_PARAMETER_ADJECTIVE_EMPTY:
						case Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE:
						case Constants.WORD_PARAMETER_ADJECTIVE_INVERTED:
						case Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE:
						case Constants.WORD_PARAMETER_ADJECTIVE_FEMININE:

						// Nouns
						case Constants.WORD_PARAMETER_NOUN_DEVELOPER:
						case Constants.WORD_PARAMETER_NOUN_EXPERT:
						case Constants.WORD_PARAMETER_NOUN_FILE:
						case Constants.WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
						case Constants.WORD_PARAMETER_NOUN_HEAD:
						case Constants.WORD_PARAMETER_NOUN_TAIL:
						case Constants.WORD_PARAMETER_NOUN_LANGUAGE:
						case Constants.WORD_PARAMETER_NOUN_MIND:
						case Constants.WORD_PARAMETER_NOUN_NUMBER:
						case Constants.WORD_PARAMETER_NOUN_PASSWORD:
						case Constants.WORD_PARAMETER_NOUN_SOLVE_LEVEL:
						case Constants.WORD_PARAMETER_NOUN_SOLVE_METHOD:
						case Constants.WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
						case Constants.WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
						case Constants.WORD_PARAMETER_NOUN_TEST_FILE:
						case Constants.WORD_PARAMETER_NOUN_USER:
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
										if( writeGeneralizationWordToSentence( ( isSpecificationGeneralization && !isPartOf && !hasFoundSingleSpecificationWord_ ), definitionGrammarWordTypeNr ) != Constants.RESULT_OK )
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
										isWordTypeSingularNoun ||
										// Matching specification word type
										specificationWordTypeNr == definitionGrammarWordTypeNr )
											{
											// Specification
											if( writeSpecificationWordToSentence( isSpecificationGeneralization, isWordTypeNumeral, isWordTypePluralNoun, isWritingCurrentSpecificationWordOnly, definitionGrammarWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
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
											if( writeRelationWordToSentence( definitionGrammarWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
												myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a relation word to the sentence" );
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
							if( CommonVariables.writtenSentenceStringBuffer == null ||
							CommonVariables.writtenSentenceStringBuffer.length() == 0 )
								CommonVariables.writtenSentenceStringBuffer = new StringBuffer( writeWordString_ );
							else
								{
								isSkippingClearWriteLevel_ = false;

								if( isInsertingSeparator )
									CommonVariables.writtenSentenceStringBuffer.append( Constants.SPACE_STRING );

								CommonVariables.writtenSentenceStringBuffer.append( writeWordString_ );
								}

							writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer;

							if( isUnknownPluralOfNoun_ )
								{
								if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
									writtenSentenceStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) + writeWordString_ + currentLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
								else
									myWordItem_.startErrorInWord( 1, moduleNameString_, "The current language word item is undefined" );
								}

							if( hasFoundGeneralizationWord_ )
								generalizationStartWordPosition_ = writtenSentenceStringBuffer.length();
							else
								{
								if( hasFoundSpecificationWord_ )
									{
									specificationStartWordPosition_ = writtenSentenceStringBuffer.length();

									// Recover an unsuccessful grammar path
									previousSpecificationStringBuffer_ = lastSpecificationStringBuffer_;
									lastSpecificationStringBuffer_ = new StringBuffer( writtenSentenceStringBuffer );
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
		writeResult.isSkippingClearWriteLevel = isSkippingClearWriteLevel_;
		writeResult.result = CommonVariables.result;
		return writeResult;
		}
	};

/*************************************************************************
 *	"Your word is a lamp to guide my feet
 *	and a light for my path." (Psalm 119:105)
 *************************************************************************/
