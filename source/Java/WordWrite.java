/*	Class:			WordWrite
 *	Supports class:	WordItem
 *	Purpose:		To write specifications as sentences
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
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

class WordWrite
	{
	// Private constructed variables

	private boolean hasFoundAllSpecificationWords_;
	private boolean hasFoundAllSpecificationWordsBeforeConjunction_;
	private boolean hasFoundWordToWrite_;
	private boolean hasGeneralizationWord_;
	private boolean hasQuestionVerb_;
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


	// Private initialized variables

	private String moduleNameString_;

	private WordItem myWordItem_;


	// Private methods

	private void initializeVariables()
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

		mixedWordTypesSpecificationItem_ = null;
		lastFoundSpecificationItem_ = null;

		lastFoundRelationWordItem_ = null;

		writeWordString_ = null;

		lastSpecificationStringBuffer_ = null;
		previousSpecificationStringBuffer_ = null;

		CommonVariables.currentWriteLevel = Constants.NO_WRITE_LEVEL;
		CommonVariables.writtenSentenceStringBuffer = new StringBuffer();

		myWordItem_.deleteTemporaryWriteList();
		}

	private byte checkAssumptionLevel( SpecificationItem writeSpecificationItem )
		{
		short currentAssumptionLevel;
		short previousAssumptionLevel;
		ShortResultType shortResult;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		previousAssumptionLevel = writeSpecificationItem.assumptionLevel();

		if( myWordItem_.hasCorrectedAssumptionByKnowledge() )
			{
			// Normal calculation will go wrong if corrected assumption is rewritten
			// In other words: If the stored sentence buffer string isn't used
			if( writeSpecificationItem.recalculateAssumption() != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to calculate the assumption level of the write specification" );

			currentAssumptionLevel = writeSpecificationItem.assumptionLevel();
			}
		else
			{
			if( ( shortResult = writeSpecificationItem.calculateAssumptionLevel() ).result != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to calculate the assumption level of the write specification" );

			currentAssumptionLevel = shortResult.shortValue;
			}

		// Difference between recalculated assumption level and previous assumption level
		// Assumption level is not recalculated after change
		if( currentAssumptionLevel != previousAssumptionLevel &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe assumption level of the following specification item has changed during the process, but it isn't recalculated.\nSo, this specification may have a recalculation or update issue.\n\tPrevious assumption level: " + previousAssumptionLevel + ", recalculated assumption level: " + currentAssumptionLevel + ";\n\tSpecificationItem: " + writeSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n\n" ) != Constants.RESULT_OK )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	private byte cleanupWriteInfo( boolean isWritingCurrentSpecificationWordOnly, short startWriteLevel, int startWordPosition, SpecificationItem clearSpecificationItem )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( ( writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() <= startWordPosition )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string buffer" );

		CommonVariables.writtenSentenceStringBuffer = new StringBuffer( startWordPosition > 0 ? CommonVariables.writtenSentenceStringBuffer.substring( 0, startWordPosition ) : Constants.EMPTY_STRING );

		if( CommonVariables.currentWriteLevel > startWriteLevel )
			{
			if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, startWriteLevel, clearSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to cleanup the write levels of the write words" );

			if( generalizationStartWordPosition_ > startWordPosition )
				generalizationStartWordPosition_ = 0;

			if( specificationStartWordPosition_ > startWordPosition )
				{
				hasFoundAllSpecificationWordsBeforeConjunction_ = false;
				isSpecificationWaitingForConjunction_ = false;

				if( !clearSpecificationItem.isQuestion() )
					hasFoundSpecificationGeneralizationVerb_ = false;

				specificationStartWordPosition_ = 0;
				}
			}

		return Constants.RESULT_OK;
		}

	private byte clearWriteLevel( boolean isWritingCurrentSpecificationWordOnly, short currentWriteLevel, SpecificationItem clearSpecificationItem )
		{
		boolean isAnsweredQuestion;
		boolean isExclusiveSpecification = false;
		boolean isNegative = false;
		boolean isPossessive = false;
		boolean isSelfGenerated = false;
		short assumptionLevel;
		int specificationCollectionNr;
		int generalizationContextNr;
		int relationContextNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentContextWordItem;
		WordItem currentSpecificationWordItem;

		if( clearSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given clear specification item is undefined" );

		if( CommonVariables.currentWriteLevel < currentWriteLevel )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given current write level of this wordf is higher than the global current write level" );

		// Clear generalization write level
		myWordItem_.clearGeneralizationWriteLevel( currentWriteLevel );

		if( ( currentSpecificationWordItem = clearSpecificationItem.specificationWordItem() ) != null )
			{
			// Clear specification write level of current specification word
			currentSpecificationWordItem.clearSpecificationWriteLevel( currentWriteLevel );

			if( ( relationContextNr = clearSpecificationItem.relationContextNr() ) == Constants.NO_CONTEXT_NR )
				{
				if( !isWritingCurrentSpecificationWordOnly &&
				( specificationCollectionNr = clearSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
					{
					isAnsweredQuestion = clearSpecificationItem.isAnsweredQuestion();

					if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( isAnsweredQuestion, clearSpecificationItem.isAssignment(), clearSpecificationItem.isInactiveAssignment(), clearSpecificationItem.isArchivedAssignment(), clearSpecificationItem.questionParameter() ) ) != null )
						{
						// Clear specification write level of related specification words
						isExclusiveSpecification = clearSpecificationItem.isExclusiveSpecification();
						isNegative = clearSpecificationItem.isNegative();
						isPossessive = clearSpecificationItem.isPossessive();
						isSelfGenerated = clearSpecificationItem.isSelfGenerated();
						assumptionLevel = clearSpecificationItem.assumptionLevel();
						generalizationContextNr = clearSpecificationItem.generalizationContextNr();

						do	{
							if( currentSpecificationItem != clearSpecificationItem &&
							( currentSpecificationWordItem = currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, Constants.NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, relationContextNr ) ) != null )
								currentSpecificationWordItem.clearSpecificationWriteLevel( currentWriteLevel );
							}
						while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != null );
						}
					}
				}
			else
				{
				// Clear relation context write level
				if( ( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
					{
					// Do for all context words
					do	currentContextWordItem.clearRelationWriteLevel( currentWriteLevel, relationContextNr );
					while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte writeGeneralizationWordToSentence( boolean isPluralSpecificationGeneralization, short grammarWordTypeNr )
		{
		short writeWordTypeNr = grammarWordTypeNr;

		hasGeneralizationWord_ = false;
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
				if( myWordItem_.markGeneralizationWordTypeAsWritten( writeWordTypeNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a word type of a generalization word as written" );

				hasGeneralizationWord_ = true;
				}
			}

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationWordToSentence( boolean isPossessive, boolean isWordTypeNumeral, boolean isWordTypePluralNoun, short grammarWordTypeNr, int relationContextNr, SpecificationItem writeSpecificationItem )
		{
		boolean hasFoundNumberOfRelations = false;
		short specificationWordTypeNr;
		int nContextRelations = 0;
		WordItem specificationWordItem;
		String specificationWordString;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		// Specification string
		if( ( specificationWordItem = writeSpecificationItem.specificationWordItem() ) == null )
			{
			if( !writeSpecificationItem.isSpecificationStringAlreadyWritten() )
				{
				if( writeSpecificationItem.markSpecificationStringAsWritten() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification string \"" + writeSpecificationItem.specificationString() + "\" as written" );

				hasFoundSpecificationWord_ = true;
				lastFoundSpecificationItem_ = writeSpecificationItem;
				writeWordString_ = writeSpecificationItem.specificationString();
				}
			}
		else
			{
			// Specification word
			specificationWordTypeNr = writeSpecificationItem.specificationWordTypeNr();

			if( !specificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) )
				{
				if( isPossessive &&
				// Number of relation words of user specification item
				( nContextRelations = writeSpecificationItem.nContextRelations() ) == 0 )
					// Calculated number of relation words of a self-generated possessive specification
					nContextRelations = myWordItem_.nContextWords( relationContextNr, specificationWordItem );

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
								// The grammar definition word 'number' needs to be converted to the number of relation words
								hasFoundNumberOfRelations = true;
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

					if( !hasFoundNumberOfRelations &&
					writeWordString_ != null )
						{
						if( specificationWordItem.markSpecificationWordTypeAsWritten( writeWordTypeNr_ ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification word \"" + specificationWordItem.anyWordTypeString() + "\" as written" );

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
		int relationContextNr;
		SpecificationItem currentSpecificationItem;
		WordItem lastFoundSpecificationWordItem;

		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = grammarWordTypeNr;

		writeWordString_ = null;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		isPossessive = writeSpecificationItem.isPossessive();
		relationContextNr = writeSpecificationItem.relationContextNr();

		if( isWritingCurrentSpecificationWordOnly )
			{
			if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, writeSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word only to the sentence" );
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

			if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( isAnsweredQuestion, writeSpecificationItem.isAssignment(), writeSpecificationItem.isInactiveAssignment(), writeSpecificationItem.isArchivedAssignment(), writeSpecificationItem.questionParameter() ) ) != null )
				{
				do	{
					if( currentSpecificationItem == writeSpecificationItem ||
					currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, Constants.NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, relationContextNr ) != null )
						{
						if( writeSpecificationWordToSentence( isPossessive, isWordTypeNumeral, isWordTypePluralNoun, grammarWordTypeNr, relationContextNr, currentSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word to the sentence" );

						if( writeWordString_ == null &&
						currentSpecificationItem != writeSpecificationItem )
							{
							hasSkippedDifferentSpecification = true;

							if( writeSpecificationItem.specificationWordTypeNr() != currentSpecificationItem.specificationWordTypeNr() )
								mixedWordTypesSpecificationItem_ = currentSpecificationItem;
							}
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
					lastNumeralWordPosition_ = 0;

					if( !isSpecificationGeneralization )
						isSkippingClearWriteLevel_ = true;

					// Recovery after unsuccessful grammar path
					CommonVariables.writtenSentenceStringBuffer = previousSpecificationStringBuffer_;

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

		return Constants.RESULT_OK;
		}

	private byte writeRelationWordToSentence( short grammarWordTypeNr, SpecificationItem writeSpecificationItem )
		{
		WordItem firstUnwrittenRelationWordItem;

		writeWordString_ = null;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( ( firstUnwrittenRelationWordItem = writeSpecificationItem.unwrittenProperNameRelationWordItem( null ) ) != null )
			{
			if( ( writeWordString_ = firstUnwrittenRelationWordItem.activeWordTypeString( grammarWordTypeNr ) ) == null )
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "Relation context word \"" + firstUnwrittenRelationWordItem.anyWordTypeString() + "\" doesn't have the requested word type" );

			if( firstUnwrittenRelationWordItem.markRelationWordTypeAsWritten( grammarWordTypeNr ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark relation context word \"" + firstUnwrittenRelationWordItem.anyWordTypeString() + "\" as written" );

			if( lastFoundRelationWordItem_ != null &&
			lastFoundRelationWordItem_ != firstUnwrittenRelationWordItem &&
			// Look ahead: Was this the last unwritten relation word?
			writeSpecificationItem.unwrittenProperNameRelationWordItem( firstUnwrittenRelationWordItem ) == null )
				writeWordString_ = null;
			}

		lastFoundRelationWordItem_ = firstUnwrittenRelationWordItem;

		return Constants.RESULT_OK;
		}

	private byte writeWordToSentence( boolean isWritingCurrentSpecificationWordOnly, short answerParameter, GrammarItem definitionGrammarItem, SpecificationItem writeSpecificationItem )
		{
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
		boolean isWordTypePluralNoun;
		boolean isInsertingSeparator = true;
		short assumptionLevel;
		short definitionGrammarParameter;
		short definitionGrammarWordTypeNr;
		short generalizationWordTypeNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		int pronounContextNr = Constants.NO_CONTEXT_NR;
		StringBuffer writtenSentenceStringBuffer;
		String predefinedWordString = null;
		WordItem predefinedWordItem;
		WordItem pronounWordItem;
		WordItem singleRelationWordItem;
		WordItem specificationWordItem;
		WordItem currentLanguageWordItem;

		hasGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		writeWordString_ = null;

		if( definitionGrammarItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given definition grammar item is undefined" );

		if( !definitionGrammarItem.hasWordType() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given grammar definition has no word type" );

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

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
				if( isPartOf ||
				isRelation ||
				answerParameter > Constants.NO_ANSWER_PARAMETER ||

				( isSpecification &&
				!isWritingCurrentSpecificationWordOnly &&
				myWordItem_.nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, writeSpecificationItem.isSelfGenerated(), assumptionLevel, writeSpecificationItem.questionParameter(), writeSpecificationItem.specificationWordTypeNr(), writeSpecificationItem.specificationCollectionNr(), generalizationContextNr, relationContextNr ) != 1 ) )
					{
					isInsertingSeparator = false;
					writeWordString_ = predefinedWordString;
					}
				else
					hasFoundAllSpecificationWordsBeforeConjunction_ = true;

				break;

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
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem.singleRelationWordItem() ) != null )
					{
					if( singleRelationWordItem.isFemale() ||
					!singleRelationWordItem.hasMasculineProperNameEnding() )
						writeWordString_ = predefinedWordString;
					}

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE:
				if( hasRelationContext &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem.singleRelationWordItem() ) != null )
					{
					if( singleRelationWordItem.isMale() ||
					!singleRelationWordItem.hasFeminineProperNameEnding() )
						writeWordString_ = predefinedWordString;
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
				if( writeSpecificationItem.isExclusiveNonPossessiveGeneralizationAssignment() )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL:
			case Constants.WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE:
				if( isActiveAssignment &&
				// Only display the word "new" during the current sentence
				writeSpecificationItem.hasCurrentActiveSentenceNr() &&

				( writeSpecificationItem.isExclusiveSpecification() ||

				( relationContextNr > Constants.NO_CONTEXT_NR &&
				myWordItem_.hasCurrentlyConfirmedSpecification() &&
				myWordItem_.nContextWords( relationContextNr, specificationWordItem ) == 1 ) ) )
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
					if( checkAssumptionLevel( writeSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'maybe'" );
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
						assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
					}

				if( assumptionLevel >= Constants.NUMBER_OF_ASSUMPTION_LEVELS )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
				if( isSelfGeneratedAssumption )
					{
					if( checkAssumptionLevel( writeSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'possibly'" );
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
						assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
					}

				if( assumptionLevel == 2 )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				if( isSelfGeneratedAssumption )
					{
					if( checkAssumptionLevel( writeSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'probably'" );
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
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

				( ( mixedWordTypesSpecificationItem_ == null &&
				!writeSpecificationItem.isSpecificationPluralNoun() &&
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
				isSpecificationNoun &&
				isGeneralizationProperName &&
				writeSpecificationItem.isCorrectSpecificationArticle( definitionGrammarParameter ) ) ) ) ||

				// Generalization proper name preceded by defined article
				( isGeneralization &&
				isGeneralizationProperName &&
				myWordItem_.isProperNamePrecededByDefiniteArticle( definitionGrammarParameter ) ) ||

				// Relation proper names preceded by defined article
				( isRelation &&
				writeSpecificationItem.isCorrectArticleOfFirstUnwrittenProperNameRelationWord( definitionGrammarParameter ) ) )
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
						if( ( pronounWordItem = myWordItem_.predefinedWordItem( definitionGrammarParameter ) ) == null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find the pronoun word with the definition grammar parameter" );

						if( pronounWordItem.contextWordTypeNr( pronounContextNr ) == definitionGrammarWordTypeNr )
							writeWordString_ = predefinedWordString;
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
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
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
					writeSpecificationItem.hasSpecificationCollection() ) )
						writeWordString_ = predefinedWordString;
					else
						{
						// Force current tense, because the word "previous" is used
						if( ( predefinedWordItem = myWordItem_.predefinedWordItem( definitionGrammarParameter == Constants.WORD_PARAMETER_SINGULAR_VERB_WAS ? Constants.WORD_PARAMETER_SINGULAR_VERB_IS : Constants.WORD_PARAMETER_PLURAL_VERB_ARE ) ) == null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find the predefined word with word parameter " + definitionGrammarParameter );

						if( ( writeWordString_ = predefinedWordItem.wordTypeString( true, definitionGrammarWordTypeNr ) ) == null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't get the word type string from the predefined word with word parameter " + definitionGrammarParameter );
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
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
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
						hasQuestionVerb_ = true;
						// Typically for the Spanish language: No separator after inverted question mark
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
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY:
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
				hasQuestionVerb_ ) &&

				!isSpecificationWaitingForConjunction_ )
					{
					if( isGeneralization )
						{
						// Matching generalization word type
						if( generalizationWordTypeNr == definitionGrammarWordTypeNr )
							{
							// Generalization
							if( writeGeneralizationWordToSentence( ( isSpecificationGeneralization && !isPartOf && !hasFoundSingleSpecificationWord_ ), definitionGrammarWordTypeNr ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the generalization word to the sentence" );
							}
						}
					else
						{
						if( isSpecification )
							{
							isWordTypeNumeral = definitionGrammarItem.isNumeral();
							isWordTypePluralNoun = definitionGrammarItem.isPluralNoun();

							if( isWordTypeNumeral ||
							isWordTypePluralNoun ||
							definitionGrammarItem.isSingularNoun() ||
							// Matching specification word type
							writeSpecificationItem.specificationWordTypeNr() == definitionGrammarWordTypeNr )
								{
								// Specification
								if( writeSpecificationWordToSentence( isSpecificationGeneralization, isWordTypeNumeral, isWordTypePluralNoun, isWritingCurrentSpecificationWordOnly, definitionGrammarWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word to the sentence" );
								}
							}
						else
							{
							if( isRelation &&
							// Matching relation word type
							writeSpecificationItem.relationWordTypeNr() == definitionGrammarWordTypeNr &&
							// Relation context
							writeRelationWordToSentence( definitionGrammarWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a relation word to the sentence" );
							}
						}
					}

				break;

			default:
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "I don't know how to handle the grammar definition with grammar parameter: " + definitionGrammarParameter );
			}

		if( writeWordString_ != null )
			{
			if( ( writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer ) == null ||
			writtenSentenceStringBuffer.length() == 0 )
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
				if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) == null )
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The current language word item is undefined" );

				writtenSentenceStringBuffer.append( currentLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) + writeWordString_ + currentLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );
				}

			if( hasGeneralizationWord_ )
				generalizationStartWordPosition_ = writtenSentenceStringBuffer.length();
			else
				{
				if( hasFoundSpecificationWord_ )
					{
					specificationStartWordPosition_ = writtenSentenceStringBuffer.length();

					// Prepare recovery
					previousSpecificationStringBuffer_ = lastSpecificationStringBuffer_;
					lastSpecificationStringBuffer_ = new StringBuffer( writtenSentenceStringBuffer );
					}
				}
			}

		if( writeWordString_ != null )
			hasFoundWordToWrite_ = true;

		return Constants.RESULT_OK;
		}


	// Constructor

	protected WordWrite( WordItem myWordItem )
		{
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

		writeWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		lastNumeralWordPosition_ = 0;
		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		mixedWordTypesSpecificationItem_ = null;
		lastFoundSpecificationItem_ = null;

		lastFoundRelationWordItem_ = null;

		writeWordString_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given my word item is undefined.\n" );
			}
		}


	// Protected methods

	protected byte writeSpecificationSentence( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		boolean isChoice;
		boolean isOption;
		boolean isSkippingThisChoiceOrOptionPart;
		boolean isSkippingNextChoiceOrOptionParts;
		boolean isStillSuccessful;
		short startWriteLevel = CommonVariables.currentWriteLevel;
		// Take the current write sentence string buffer length as start position
		int startWordPosition = ( CommonVariables.writtenSentenceStringBuffer == null ? 0 : CommonVariables.writtenSentenceStringBuffer.length() );
		GrammarItem definitionGrammarItem = selectedGrammarItem;
		WordItem currentLanguageWordItem;
		WriteItem currentWriteItem = null;
		StringBuffer writtenSentenceStringBuffer;

		hasFoundWordToWrite_ = false;

		if( grammarLevel >= Constants.MAX_GRAMMAR_LEVEL )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given grammar level is too high: #" + grammarLevel );

		if( selectedGrammarItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given selected grammar item is undefined" );

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( writeSpecificationItem.isDeletedItem() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is deleted" );

		// Initialize
		if( grammarLevel == Constants.NO_GRAMMAR_LEVEL )
			initializeVariables();

		do	{
			if( !definitionGrammarItem.isDefinitionStart() )
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The grammar definition word item isn't a definition start" );

			// Grammar word
			if( definitionGrammarItem.isNewStart() )
				{
				if( writeWordToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a word to a sentence" );

				}
			else
				{
				// Grammar definition
				if( ( selectedGrammarItem = definitionGrammarItem.nextGrammarItem() ) == null )
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The grammar word item is undefined" );

				isChoice = false;
				isOption = false;
				isStillSuccessful = true;
				isSkippingThisChoiceOrOptionPart = false;
				isSkippingNextChoiceOrOptionParts = false;

				do	{
					if( selectedGrammarItem.isNewStart() )
						{
						if( isChoice ||
						isOption ||
						selectedGrammarItem.isChoiceStart() ||
						selectedGrammarItem.isOptionStart() )
							{
							currentWriteItem = myWordItem_.firstActiveWriteItem();

							while( currentWriteItem != null &&
							( currentWriteItem.grammarLevel() != grammarLevel ||
							currentWriteItem.startOfChoiceOrOptionGrammarItem() != selectedGrammarItem ) )
								currentWriteItem = currentWriteItem.nextWriteItem();

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
									currentWriteItem != null &&
									currentWriteItem.isSkipped )
										currentWriteItem.isSkipped = false;
									}

								if( currentWriteItem == null )
									{
									if( myWordItem_.createWriteWord( ( !isStillSuccessful || isSkippingNextChoiceOrOptionParts ), grammarLevel, selectedGrammarItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a write word" );
									}
								else
									{
									if( currentWriteItem.startOfChoiceOrOptionGrammarItem().activeSentenceNr() != selectedGrammarItem.activeSentenceNr() ||
									currentWriteItem.startOfChoiceOrOptionGrammarItem().itemNr() != selectedGrammarItem.itemNr() )
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the selected grammar path" );

									if( currentWriteItem.grammarLevel() != grammarLevel )
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the grammar level" );

									currentWriteItem = currentWriteItem.nextWriteItem();
									}
								}
							else
								{
								if( selectedGrammarItem.isChoiceStart() )
									isChoice = true;
								else
									isOption = true;

								isSkippingThisChoiceOrOptionPart = false;

								if( currentWriteItem == null )
									{
									if( myWordItem_.createWriteWord( !isStillSuccessful, grammarLevel, selectedGrammarItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a start write word" );
									}
								else
									{
									if( currentWriteItem.startOfChoiceOrOptionGrammarItem().activeSentenceNr() != selectedGrammarItem.activeSentenceNr() ||
									currentWriteItem.startOfChoiceOrOptionGrammarItem().itemNr() != selectedGrammarItem.itemNr() )
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the selected grammar path at the start of a choice or option" );

									if( currentWriteItem.grammarLevel() != grammarLevel )
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the grammar level at the start of a choice or option" );
									}
								}
							}
						}

					if( isStillSuccessful &&
					!isSkippingThisChoiceOrOptionPart &&
					!isSkippingNextChoiceOrOptionParts &&
					!selectedGrammarItem.isSkipOptionForWriting() )
						{
						if( grammarLevel + 1 >= Constants.MAX_GRAMMAR_LEVEL )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #" + ( grammarLevel + 1 ) );

						if( writeSpecificationSentence( false, isWritingCurrentSpecificationWordOnly, answerParameter, (short)( grammarLevel + 1 ), selectedGrammarItem.definitionGrammarItem, writeSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification sentence at grammar level: " + ( grammarLevel + 1 ) );

						if( !hasFoundWordToWrite_ )
							// Failed, try next part
							isSkippingThisChoiceOrOptionPart = true;
						}

					if( selectedGrammarItem.isChoiceEnd ||
					selectedGrammarItem.isOptionEnd )
						{
						isSkippingThisChoiceOrOptionPart = false;
						isSkippingNextChoiceOrOptionParts = false;

						if( selectedGrammarItem.isChoiceEnd )
							isChoice = false;
						else
							{
							isOption = false;

							if( isStillSuccessful )
								hasFoundWordToWrite_ = true;
							}
						}

					selectedGrammarItem = selectedGrammarItem.nextGrammarItem();

					if( !isChoice &&
					!isOption &&
					!hasFoundWordToWrite_ )
						isStillSuccessful = false;
					}
				while( selectedGrammarItem != null &&
				!selectedGrammarItem.isDefinitionStart() );

				if( !hasFoundWordToWrite_ &&
				!isSkippingClearWriteLevel_ &&
				// The sentence has grown
				( writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer ) != null &&
				writtenSentenceStringBuffer.length() > startWordPosition &&

				// Clean up write info
				cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to cleanup the write info" );
				}

			definitionGrammarItem = definitionGrammarItem.nextDefinitionGrammarItem;
			}
		while( !hasFoundWordToWrite_ &&
		definitionGrammarItem != null );

		if( grammarLevel == Constants.NO_GRAMMAR_LEVEL )
			{
			// Cleanup
			myWordItem_.deleteTemporaryWriteList();

			if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, Constants.NO_WRITE_LEVEL, writeSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to clear the write word levels in all words" );

			if( ( writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer ) != null &&
			writtenSentenceStringBuffer.length() > 0 )
				{
				if( isCheckingUserSentenceForIntegrity )
					CommonVariables.writtenUserSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer;

				// Typically for French: Compound words
				currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

				if( currentLanguageWordItem != null &&
				currentLanguageWordItem.isLanguageWithMergedWords() &&
				currentLanguageWordItem.shrinkMergedWordsInWriteSentence() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to shrink the compound words in the write sentence string" );
				}
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
