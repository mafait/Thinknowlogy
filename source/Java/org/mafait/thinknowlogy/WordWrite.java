﻿/*	Class:			WordWrite
 *	Supports class:	WordItem
 *	Purpose:		Writing specifications as sentences
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

class WordWrite
	{
	// Private constructed variables

	private boolean hasFoundAllRelationWords_ = false;
	private boolean hasFoundAllRelationWordsBeforeConjunction_ = false;
	private boolean hasFoundAllSpecificationWords_ = false;
	private boolean hasFoundAllSpecificationWordsBeforeConjunction_ = false;
	private boolean hasFoundWordToWrite_ = false;
	private boolean hasGeneralizationWord_ = false;
	private boolean hasFoundSingleSpecificationWord_ = false;
	private boolean hasFoundSpecificationGeneralizationVerb_ = false;
	private boolean hasFoundSpecificationWord_ = false;
	private boolean isChineseCurrentLanguage_ = false;
	private boolean isSkippingClearWriteLevel_ = false;
	private boolean isSpecificationWaitingForConjunction_ = false;
	private boolean isUnknownPluralOfNoun_ = false;

	private short writeWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private int generalizationStartWordPosition_ = 0;
	private int specificationStartWordPosition_ = 0;

	private SpecificationItem lastFoundSpecificationItem_ = null;

	private WordItem lastFoundRelationWordItem_ = null;

	private String writeWordString_ = null;

	private String moduleNameString_ = this.getClass().getName();

	private StringBuffer lastSpecificationStringBuffer_ = null;
	private StringBuffer previousSpecificationStringBuffer_ = null;

	// Private initialized variables

	private WordItem myWordItem_ = null;


	// Private methods

	private void initializePrivateWriteVariables()
		{
		hasFoundAllRelationWords_ = false;
		hasFoundAllRelationWordsBeforeConjunction_ = false;
		hasFoundAllSpecificationWords_ = false;
		hasFoundAllSpecificationWordsBeforeConjunction_ = false;
		hasGeneralizationWord_ = false;
		hasFoundSingleSpecificationWord_ = false;
		hasFoundSpecificationGeneralizationVerb_ = false;
		hasFoundSpecificationWord_ = false;
		isChineseCurrentLanguage_ = myWordItem_.isChineseCurrentLanguage();
		isSkippingClearWriteLevel_ = false;
		isSpecificationWaitingForConjunction_ = false;
		isUnknownPluralOfNoun_ = false;

		generalizationStartWordPosition_ = 0;
		specificationStartWordPosition_ = 0;

		lastFoundSpecificationItem_ = null;

		lastFoundRelationWordItem_ = null;

		writeWordString_ = null;

		lastSpecificationStringBuffer_ = null;
		previousSpecificationStringBuffer_ = null;

		GlobalVariables.currentWriteLevel = Constants.NO_WRITE_LEVEL;
		GlobalVariables.writtenSentenceStringBuffer = new StringBuffer();

		myWordItem_.deleteTemporaryWriteList();
		}

	private byte checkAssumptionLevel( SpecificationItem writeSpecificationItem )
		{
		short previousAssumptionLevel;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		previousAssumptionLevel = writeSpecificationItem.assumptionLevel();

		// Calculate assumption level of write specification
		if( writeSpecificationItem.calculateAssumptionLevel() != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to calculate the assumption level of the write specification" );

		if( writeSpecificationItem.isOlderItem() &&
		// There is a difference between recalculated assumption level and previous assumption level
		// So, the assumption level is not recalculated after a change during the process
		writeSpecificationItem.assumptionLevel() != previousAssumptionLevel &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe assumption level of the following specification item has changed during the process, but it isn't recalculated.\nSo, this specification may have a recalculation or update issue.\n\tPrevious assumption level: " + previousAssumptionLevel + ", recalculated assumption level: " + writeSpecificationItem.assumptionLevel() + ";\n\tSpecificationItem: " + writeSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n\n" ) != Constants.RESULT_OK )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	private byte cleanupWriteInfo( boolean isWritingCurrentSpecificationWordOnly, short startWriteLevel, int startWordPosition, SpecificationItem clearSpecificationItem )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( clearSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given clear specification item is undefined" );

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() <= startWordPosition )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string buffer" );

		GlobalVariables.writtenSentenceStringBuffer = new StringBuffer( startWordPosition > 0 ? GlobalVariables.writtenSentenceStringBuffer.substring( 0, startWordPosition ) : Constants.EMPTY_STRING );

		if( GlobalVariables.currentWriteLevel > startWriteLevel )
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
		boolean isExclusiveSpecification;
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

		if( GlobalVariables.currentWriteLevel < currentWriteLevel )
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
				( specificationCollectionNr = clearSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( currentSpecificationItem = myWordItem_.firstSpecificationItem( false, clearSpecificationItem.isAssignment(), clearSpecificationItem.isInactiveAssignment(), clearSpecificationItem.isArchivedAssignment(), clearSpecificationItem.questionParameter() ) ) != null )
					{
					// Clear specification write level of related specification words
					isExclusiveSpecification = clearSpecificationItem.isExclusiveSpecification();
					isNegative = clearSpecificationItem.isNegative();
					isPossessive = clearSpecificationItem.isPossessive();
					isSelfGenerated = clearSpecificationItem.isSelfGeneratedSpecification();
					assumptionLevel = clearSpecificationItem.assumptionLevel();
					generalizationContextNr = clearSpecificationItem.generalizationContextNr();

					do	{
						if( currentSpecificationItem != clearSpecificationItem &&
						( currentSpecificationWordItem = currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, Constants.NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, relationContextNr ) ) != null )
							currentSpecificationWordItem.clearSpecificationWriteLevel( currentWriteLevel );
						}
					while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedQuestionParameterSpecificationItem() ) != null );
					}
				}
			else
				{
				if( ( currentContextWordItem = myWordItem_.firstContextWordItem( relationContextNr ) ) != null )
					{
					if( currentWriteLevel == Constants.NO_WRITE_LEVEL )
						{
						hasFoundAllRelationWords_ = false;
						hasFoundAllRelationWordsBeforeConjunction_ = false;
						}

					// Do for all context words with relation context
					// Clear relation context write level
					do	currentContextWordItem.clearRelationWriteLevel( currentWriteLevel );
					while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( relationContextNr ) ) != null );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte writeGeneralizationWordToSentence( boolean isPluralSpecificationGeneralization, short grammarDefinitionWordTypeNr )
		{
		short writeWordTypeNr = grammarDefinitionWordTypeNr;

		hasGeneralizationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		if( ( writeWordString_ = myWordItem_.activeWordTypeString( isPluralSpecificationGeneralization && !isChineseCurrentLanguage_ ? Constants.WORD_TYPE_NOUN_PLURAL : grammarDefinitionWordTypeNr ) ) == null &&
		( writeWordString_ = myWordItem_.singularNounString() ) != null )
			{
			if( !isChineseCurrentLanguage_ )
				isUnknownPluralOfNoun_ = true;

			writeWordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;
			}

		if( writeWordString_ != null )
			{
			if( myWordItem_.markGeneralizationWordTypeAsWritten( writeWordTypeNr ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a word type of a generalization word as written" );

			hasGeneralizationWord_ = true;
			}

		return Constants.RESULT_OK;
		}

	private byte writeCurrentSpecificationWordToSentence( boolean isPossessive, short grammarDefinitionWordTypeNr, short specificationWordTypeNr, int relationContextNr, SpecificationItem writeSpecificationItem, WordItem specificationWordItem )
		{
		boolean hasFoundNumberOfRelations = false;
		boolean isWordTypeNumeral = ( grammarDefinitionWordTypeNr == Constants.WORD_TYPE_NUMERAL );
		boolean isWordTypePluralNoun = ( grammarDefinitionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL );
		int nContextRelations = 0;
		String specificationWordString;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( isPossessive &&
		// Number of relation words of user specification item
		( nContextRelations = writeSpecificationItem.nContextRelations() ) == 0 )
			// Calculated number of relation words of a self-generated possessive specification
			nContextRelations = myWordItem_.nContextWords( relationContextNr );

		// No relation word
		if( ( ( nContextRelations == 0 ||
		// One relation word
		nContextRelations == 1 ) &&

		grammarDefinitionWordTypeNr == specificationWordTypeNr ) ||

		// Multiple relation words
		( nContextRelations > 1 &&

		( isWordTypeNumeral ||
		isWordTypePluralNoun ) ) )
			{
			if( ( specificationWordString = specificationWordItem.activeWordTypeString( grammarDefinitionWordTypeNr ) ) == null )
				{
				if( isWordTypeNumeral )
					{
					if( !isChineseCurrentLanguage_ ||
					// Typical for Chinese: no counting of parents, children, and so on, in sentence with relation.
					relationContextNr == Constants.NO_CONTEXT_NR )
						{
						// The grammar definition word 'number' needs to be converted to the number of relation words
						hasFoundNumberOfRelations = true;
						writeWordString_ = ( Constants.EMPTY_STRING + nContextRelations );
						}
					}
				else
					{
					if( isWordTypePluralNoun )
						{
						if( ( writeWordString_ = specificationWordItem.singularNounString() ) != null )
							{
							if( !isChineseCurrentLanguage_ )
								// The plural noun is unknown, but the singular noun is known.
								// So, force a singular noun, but add a 'plural noun is unknown' text.
								isUnknownPluralOfNoun_ = true;

							writeWordTypeNr_ = Constants.WORD_TYPE_NOUN_SINGULAR;
							}
						}
					else
						// Accept a hidden word type (password) in user sentence
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

				// Typical for Chinese
				if( isChineseCurrentLanguage_ &&
				// Chinese test files: "Boiling point" and "Condensation point"
				writeSpecificationItem.isUncountableGeneralizationNoun() )
					hasFoundAllSpecificationWordsBeforeConjunction_ = true;

				if( hasFoundAllSpecificationWordsBeforeConjunction_ )
					{
					hasFoundAllSpecificationWords_ = true;
					isSkippingClearWriteLevel_ = false;
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationWordToSentence( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, boolean isSpecificationGeneralization, boolean isWritingCurrentSpecificationWordOnly, short assumptionLevel, short grammarDefinitionWordTypeNr, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, SpecificationItem writeSpecificationItem, WordItem specificationWordItem )
		{
		boolean hasSkippedDifferentSpecification = false;
		boolean isAnsweredQuestion;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem lastFoundSpecificationWordItem;

		hasFoundSingleSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = grammarDefinitionWordTypeNr;
		writeWordString_ = null;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( isWritingCurrentSpecificationWordOnly )
			{
			if( !specificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) &&
			// Writing current specification word only
			writeCurrentSpecificationWordToSentence( isPossessive, grammarDefinitionWordTypeNr, specificationWordTypeNr, relationContextNr, writeSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word only to the sentence" );
			}
		else
			{
			isAnsweredQuestion = writeSpecificationItem.isAnsweredQuestion();

			if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( isAnsweredQuestion, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) ) != null )
				{
				do	{
					if( ( currentSpecificationItem == writeSpecificationItem ||
					currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, Constants.NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, relationContextNr ) != null ) &&

					( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
						{
						if( currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( currentSpecificationItem.specificationWordTypeNr() ) )
							{
							if( currentSpecificationItem != writeSpecificationItem )
								hasSkippedDifferentSpecification = true;
							}
						else
							{
							// Write current specification word
							if( writeCurrentSpecificationWordToSentence( isPossessive, grammarDefinitionWordTypeNr, currentSpecificationItem.specificationWordTypeNr(), relationContextNr, currentSpecificationItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the current specification word to the sentence" );
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
		lastFoundSpecificationItem_.specificationWordTypeNr() == grammarDefinitionWordTypeNr )
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

					// Recovery after unsuccessful grammar path
					GlobalVariables.writtenSentenceStringBuffer = previousSpecificationStringBuffer_;

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

	private byte writeRelationWordToSentence( short grammarDefinitionWordTypeNr, SpecificationItem writeSpecificationItem )
		{
		WordItem firstUnwrittenRelationWordItem;

		writeWordString_ = null;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( ( firstUnwrittenRelationWordItem = writeSpecificationItem.firstUnwrittenRelationWordItem( grammarDefinitionWordTypeNr, null ) ) == null )
			{
			hasFoundAllRelationWords_ = true;
			hasFoundAllRelationWordsBeforeConjunction_ = true;
			}
		else
			{
			if( ( writeWordString_ = firstUnwrittenRelationWordItem.activeWordTypeString( grammarDefinitionWordTypeNr ) ) == null )
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "Relation context word \"" + firstUnwrittenRelationWordItem.anyWordTypeString() + "\" doesn't have the requested word type" );

			if( firstUnwrittenRelationWordItem.markRelationWordTypeAsWritten( grammarDefinitionWordTypeNr ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark relation context word \"" + firstUnwrittenRelationWordItem.anyWordTypeString() + "\" as written" );

			if( hasFoundAllRelationWordsBeforeConjunction_ )
				hasFoundAllRelationWords_ = true;

			if( lastFoundRelationWordItem_ != null &&
			lastFoundRelationWordItem_ != firstUnwrittenRelationWordItem &&
			// Look ahead: Was this the last unwritten relation word?
			writeSpecificationItem.firstUnwrittenRelationWordItem( grammarDefinitionWordTypeNr, firstUnwrittenRelationWordItem ) == null )
				{
				hasFoundAllRelationWordsBeforeConjunction_ = true;
				writeWordString_ = null;
				}
			}

		lastFoundRelationWordItem_ = firstUnwrittenRelationWordItem;

		return Constants.RESULT_OK;
		}

	private byte writeWordToSentence( boolean isWritingCurrentSpecificationWordOnly, short answerParameter, GrammarItem definitionGrammarItem, SpecificationItem writeSpecificationItem )
		{
		boolean hasRelationContext;
		boolean isAssignment;
		boolean isActiveAssignment;
		boolean isArchivedAssignment;
		boolean isCharacteristicFor;
		boolean isChineseCurrentLanguage = isChineseCurrentLanguage_;
		boolean isConditional;
		boolean isEveryGeneralization;
		boolean isExclusiveSpecification;
		boolean isGeneralization;
		boolean isGeneralizationNoun;
		boolean isGeneralizationProperNoun;
		boolean isInactiveAssignment;
		boolean isNegative;
		boolean isPartOf;
		boolean isPossessive;
		boolean isQuestion;
		boolean isRelation;
		boolean isSelfGenerated;
		boolean isSelfGeneratedAssumption;
		boolean isSelfGeneratedQuestion;
		boolean isSpecification;
		boolean isSpecificationGeneralization;
		boolean isSpecificationNoun;
		boolean isSpecificationPluralNoun;
		boolean isUncountableGeneralizationNoun;
		boolean isUniqueUserRelation;
		// Typical for Chinese: No spaces between characters
		boolean isInsertingSeparator = !isChineseCurrentLanguage;
		short assumptionLevel;
		short generalizationWordTypeNr;
		short grammarDefinitionParameter;
		short grammarDefinitionWordTypeNr;
		short prepositionParameter;
		short questionParameter;
		short specificationWordTypeNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		int nRemainingSpecificationWordsForWriting;
		int pronounContextNr = Constants.NO_CONTEXT_NR;
		int specificationCollectionNr;
		StringBuffer writtenSentenceStringBuffer;
		String predefinedWordString = null;
		WordItem predefinedWordItem;
		WordItem pronounWordItem;
		WordItem singleRelationWordItem;
		WordItem specificationWordItem;

		hasGeneralizationWord_ = false;
		hasFoundSpecificationWord_ = false;
		isUnknownPluralOfNoun_ = false;

		writeWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		writeWordString_ = null;

		if( definitionGrammarItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given definition grammar item is undefined" );

		if( ( grammarDefinitionWordTypeNr = definitionGrammarItem.grammarWordTypeNr() ) <= Constants.NO_WORD_TYPE_NR )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given grammar definition has no word type" );

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		hasRelationContext = writeSpecificationItem.hasRelationContext();
		isAssignment = writeSpecificationItem.isAssignment();
		isActiveAssignment = writeSpecificationItem.isActiveAssignment();
		isArchivedAssignment = writeSpecificationItem.isArchivedAssignment();
		isCharacteristicFor = writeSpecificationItem.isCharacteristicFor();
		isConditional = writeSpecificationItem.isConditional();
		isEveryGeneralization = writeSpecificationItem.isEveryGeneralization();
		isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
		isGeneralizationNoun = writeSpecificationItem.isGeneralizationNoun();
		isGeneralizationProperNoun = writeSpecificationItem.isGeneralizationProperNoun();
		isInactiveAssignment = writeSpecificationItem.isInactiveAssignment();
		isNegative = writeSpecificationItem.isNegative();
		isPartOf = writeSpecificationItem.isPartOf();
		isPossessive = writeSpecificationItem.isPossessive();
		isQuestion = writeSpecificationItem.isQuestion();
		isSelfGenerated = writeSpecificationItem.isSelfGeneratedSpecification();
		isSelfGeneratedAssumption = writeSpecificationItem.isSelfGeneratedAssumption();
		isSelfGeneratedQuestion = writeSpecificationItem.isSelfGeneratedQuestion();
		isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();
		isSpecificationNoun = writeSpecificationItem.isSpecificationNoun();
		isSpecificationPluralNoun = writeSpecificationItem.isSpecificationPluralNoun();
		isUncountableGeneralizationNoun = writeSpecificationItem.isUncountableGeneralizationNoun();
		isUniqueUserRelation = writeSpecificationItem.isUniqueUserRelation();

		isGeneralization = ( generalizationStartWordPosition_ == 0 &&

							( !isSpecificationGeneralization ||

							( hasFoundAllSpecificationWords_ &&
							hasFoundSpecificationGeneralizationVerb_ ) ) );

		isRelation = ( hasRelationContext &&
						!isGeneralization &&

						( specificationStartWordPosition_ > 0 ||

						// Typical for Chinese: Relation before specification
						( isChineseCurrentLanguage &&
						!isPossessive &&
						!hasFoundAllRelationWords_ ) ) );

		isSpecification = ( !isGeneralization &&
							!isRelation );

		grammarDefinitionParameter = definitionGrammarItem.grammarParameter();
		assumptionLevel = writeSpecificationItem.assumptionLevel();
		generalizationWordTypeNr = writeSpecificationItem.generalizationWordTypeNr();
		prepositionParameter = writeSpecificationItem.prepositionParameter();
		questionParameter = writeSpecificationItem.questionParameter();
		specificationWordTypeNr = writeSpecificationItem.specificationWordTypeNr();
		specificationCollectionNr = writeSpecificationItem.specificationCollectionNr();
		generalizationContextNr = writeSpecificationItem.generalizationContextNr();
		specificationContextNr = writeSpecificationItem.specificationContextNr();
		relationContextNr = writeSpecificationItem.relationContextNr();
		specificationWordItem = writeSpecificationItem.specificationWordItem();

		if( grammarDefinitionParameter > Constants.NO_GRAMMAR_PARAMETER &&
		( predefinedWordItem = myWordItem_.predefinedWordItem( grammarDefinitionParameter ) ) != null )
			predefinedWordString = predefinedWordItem.wordTypeString( true, grammarDefinitionWordTypeNr );

		switch( grammarDefinitionParameter )
			{
			// Symbols
			case Constants.WORD_PARAMETER_SYMBOL_COMMA:
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
				isGeneralizationNoun ) ||

				( !isWritingCurrentSpecificationWordOnly &&

				( ( ( !isAssignment ||
				isExclusiveSpecification ) ) ||

				( !isQuestion &&
				!writeSpecificationItem.isExclusiveGeneralization() ) ) &&

				// Efficiency: Only write comma if there are more than one specification words remaining
				( ( nRemainingSpecificationWordsForWriting = myWordItem_.nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, Constants.NO_SENTENCE_NR ) ) > 1 ||

				( nRemainingSpecificationWordsForWriting == 0 &&
				!hasRelationContext &&
				!isNegative &&
				!hasFoundAllSpecificationWords_ &&

				// Exceptions
				( isCharacteristicFor ||
				isInactiveAssignment ||
				isSpecificationGeneralization ||

				// Test file: "Same and similar questions"
				( isQuestion &&
				isExclusiveSpecification ) ||

				// Typical for French
				// Typical for Spanish
				( isPossessive &&
				!isSelfGenerated ) ) ) ) ) ) ) )
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

			case Constants.WORD_PARAMETER_SYMBOL_COLON:
			case Constants.WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_QUESTION_MARK:
				isInsertingSeparator = false;
				writeWordString_ = predefinedWordString;
				break;

			case Constants.WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK:
			case Constants.WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK:
				break;

			case Constants.WORD_PARAMETER_SYMBOL_QUESTION_MARK:
				if( isQuestion )
					{
					isInsertingSeparator = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			case Constants.WORD_PARAMETER_SYMBOL_CHINESE_NEGATIVE_SEPARATOR:
				if( isNegative &&
				!hasFoundAllSpecificationWords_ )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;

					if( !hasFoundAllSpecificationWordsBeforeConjunction_ &&
					myWordItem_.nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, false, true, isPossessive, isSelfGenerated, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, ( isSelfGenerated ? Constants.NO_SENTENCE_NR : writeSpecificationItem.creationSentenceNr() ) ) == 1 )
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					}

				break;

			case Constants.WORD_PARAMETER_SYMBOL_CHINESE_QUESTION_SEPARATOR:
				if( isQuestion &&
				isExclusiveSpecification &&
				!hasFoundAllSpecificationWords_ )
					{
					// Typical for Chinese
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;

					if( myWordItem_.nRemainingSpecificationWordsForWriting( isActiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, ( isSelfGenerated ? Constants.NO_SENTENCE_NR : writeSpecificationItem.creationSentenceNr() ) ) == 1 )
						hasFoundAllSpecificationWordsBeforeConjunction_ = true;
					}

				break;

			case Constants.WORD_PARAMETER_NUMERAL_CHINESE_ALL:
				if( isGeneralizationNoun &&
				!isCharacteristicFor &&

				( isEveryGeneralization ||
				isSpecificationGeneralization ) )
					writeWordString_ = predefinedWordString;

				break;

			// Adjectives
			case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_NEUTRAL:
				if( hasRelationContext &&
				!isUncountableGeneralizationNoun )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE:
				if( hasRelationContext &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem.singleRelationWordItem() ) != null )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE:
				if( hasRelationContext &&
				// Has only one relation word
				( singleRelationWordItem = writeSpecificationItem.singleRelationWordItem() ) != null &&

				( singleRelationWordItem.isMasculineWord() ||
				!singleRelationWordItem.hasFeminineProperNounEnding() ) )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_CHARACTERISTIC:
				if( isCharacteristicFor )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL:
			case Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE:
				if( isActiveAssignment &&
				writeSpecificationItem.isExclusiveNonPossessiveGeneralizationAssignment() )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
			case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE:
			// Skip writing 'every' in combination with uncertainty
				if( isEveryGeneralization &&
				!isSelfGeneratedAssumption &&
				myWordItem_.isCorrectAdjective( grammarDefinitionParameter, generalizationWordTypeNr ) )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL:
			case Constants.WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE:
				if( isActiveAssignment &&
				// Only display the word "new" during the current sentence
				writeSpecificationItem.hasCurrentCreationSentenceNr() &&

				( writeSpecificationItem.isExclusiveSpecification() ||

				( relationContextNr > Constants.NO_CONTEXT_NR &&
				myWordItem_.hasCurrentlyConfirmedSpecification() &&
				// Has only one relation word
				myWordItem_.nContextWords( relationContextNr ) == 1 ) ) )
					writeWordString_ = predefinedWordString;

				break;
/*
			case Constants.WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL:
			case Constants.WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE:
				break;
*/
			case Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL:
			case Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE:
				if( isInactiveAssignment )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_SPECIFIC:
				if( writeSpecificationItem.isSpecific() )
					writeWordString_ = predefinedWordString;

				break;

			// Adverbs
			case Constants.WORD_PARAMETER_ADJECTIVE_NO:
			case Constants.WORD_PARAMETER_ADVERB_FRENCH_PAS:
				if( isNegative )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADVERB_NOT:
				if( isNegative &&

				( !isChineseCurrentLanguage ||
				// Typical for Chinese
				!isUniqueUserRelation ) )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADVERB_ANYMORE:
			case Constants.WORD_PARAMETER_ADVERB_CHINESE_NEVER_AGAIN:
				if( isNegative &&
				isUniqueUserRelation )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				if( isSelfGeneratedAssumption &&
				!myWordItem_.hasCurrentlyCorrectedAssumption() )
					{
					// Check if the assumption level is still correct
					if( checkAssumptionLevel( writeSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the assumption level for 'probably'" );

					assumptionLevel = writeSpecificationItem.assumptionLevel();
					}
				else
					{
					if( isSelfGeneratedQuestion )
						// Self-generated question shouldn't display its assumption level
						assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
					}

				if( ( assumptionLevel == Constants.ASSUMPTION_LEVEL_PROBABLY &&
				grammarDefinitionParameter == Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY ) ||

				( assumptionLevel == Constants.ASSUMPTION_LEVEL_POSSIBLY &&
				grammarDefinitionParameter == Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY ) ||

				( assumptionLevel >= Constants.ASSUMPTION_LEVEL_MAYBE &&
				grammarDefinitionParameter == Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE ) )
					writeWordString_ = predefinedWordString;

				break;

			// Interjections
			case Constants.WORD_PARAMETER_INTERJECTION_YES:
			case Constants.WORD_PARAMETER_INTERJECTION_NO:
				if( grammarDefinitionParameter == answerParameter )
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
				if( ( isGeneralization &&
				isGeneralizationNoun &&
				!isUncountableGeneralizationNoun &&

				( !isChineseCurrentLanguage ||

				( isEveryGeneralization &&
				!isSelfGeneratedAssumption &&
				!isSpecificationGeneralization &&
				!writeSpecificationItem.isSpecific() ) ) &&

				myWordItem_.isCorrectIndefiniteArticle( grammarDefinitionParameter, generalizationWordTypeNr ) ) ||

				// Specification nouns
				( isSpecification &&
				isSpecificationNoun &&

				( ( !isChineseCurrentLanguage ||
				isGeneralizationProperNoun ||

				( ( !isEveryGeneralization ||
				!isPossessive ) &&

				!isSpecificationGeneralization ) ) &&

				( !isSpecificationPluralNoun ||
				grammarDefinitionParameter != Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) &&

				writeSpecificationItem.isCorrectSpecificationArticle( false, isWritingCurrentSpecificationWordOnly, grammarDefinitionParameter ) ) ) )
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
				!isArchivedAssignment &&
				isGeneralizationNoun &&
				myWordItem_.isCorrectDefiniteArticle( grammarDefinitionParameter, generalizationWordTypeNr ) ) ||

				// Specification nouns
				( isSpecification &&
				isGeneralizationProperNoun &&

				( !isChineseCurrentLanguage ||
				!hasRelationContext ) &&

				writeSpecificationItem.isCorrectSpecificationArticle( true, isWritingCurrentSpecificationWordOnly, grammarDefinitionParameter ) ) ) ) ||

				// Generalization proper name preceded by defined article
				( isGeneralization &&
				isGeneralizationProperNoun &&
				myWordItem_.isProperNounPrecededByDefiniteArticle( grammarDefinitionParameter ) ) ||

				// Relation proper names preceded by defined article
				( isRelation &&
				writeSpecificationItem.isCorrectArticleOfFirstUnwrittenProperNounRelationWord( grammarDefinitionParameter ) ) )
					writeWordString_ = predefinedWordString;

				break;

			// Conjunctions
			case Constants.WORD_PARAMETER_CONJUNCTION_AND:
				if( isInactiveAssignment ||
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

			case Constants.WORD_PARAMETER_CONJUNCTION_OR:
				if( isExclusiveSpecification &&
				!hasFoundAllSpecificationWords_ )
					{
					isSpecificationWaitingForConjunction_ = false;
					writeWordString_ = predefinedWordString;
					}

				break;

			// Ambiguity
			// Typical for Dutch: in zowel ... als ...
			case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
				isSpecificationWaitingForConjunction_ = false;

				// Don't insert a break statement here

			// Typical for English: '... in both ... and ...'
			case Constants.WORD_PARAMETER_NUMERAL_BOTH:
			// Typical for Dutch: in zowel ... als ...
			case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:
				if( writeSpecificationItem.isSpecificationWithStaticAmiguity() )
					writeWordString_ = predefinedWordString;

				break;

			// Nouns
			case Constants.WORD_PARAMETER_NOUN_PART:
				if( isPartOf )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_NOUN_VALUE:
				break;

			case Constants.WORD_PARAMETER_NOUN_CHINESE_IS_IT_TRUE:
				if( isQuestion &&
				!isExclusiveSpecification )
					writeWordString_ = predefinedWordString;

				break;

			// Singular pronouns
			case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_I_ME_MY_MINE:
			case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_INFORMAL:
			case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_FORMAL:
			case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_HE_HIM_HIS_HIS:
			case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_SHE_HER_HER_HERS:
			case Constants.WORD_PARAMETER_SINGULAR_PRONOUN_IT_ITS_ITS_ITS:

				// Don't insert a break statement here

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
/*						else
							{
							if( isRelation )
								pronounContextNr = relationContextNr;
							}
*/						}

					if( pronounContextNr > Constants.NO_CONTEXT_NR )
						{
						if( ( pronounWordItem = myWordItem_.predefinedWordItem( grammarDefinitionParameter ) ) == null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find the pronoun word with the definition grammar parameter" );

						if( pronounWordItem.contextWordTypeNr( pronounContextNr ) == grammarDefinitionWordTypeNr )
							writeWordString_ = predefinedWordString;
						}
					}

				break;

			case Constants.WORD_PARAMETER_PREPOSITION_ABOUT:
			case Constants.WORD_PARAMETER_PREPOSITION_FROM:
			case Constants.WORD_PARAMETER_PREPOSITION_IN:
			case Constants.WORD_PARAMETER_PREPOSITION_TO:
			case Constants.WORD_PARAMETER_PREPOSITION_FOR:
			case Constants.WORD_PARAMETER_PREPOSITION_FRENCH_A:
				if( prepositionParameter == grammarDefinitionParameter ||
				prepositionParameter == Constants.NO_PREPOSITION_PARAMETER )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_PREPOSITION_OF:
				if( ( prepositionParameter == grammarDefinitionParameter ||

				( isGeneralizationNoun ||
				!isSpecificationPluralNoun ) ) &&

				( !isChineseCurrentLanguage ||

				// Typical for Chinese
				( isSpecification &&

				( isGeneralizationNoun ||
				hasRelationContext ) ) ) )
					writeWordString_ = predefinedWordString;

				break;

			// Verbs
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IS:
			case Constants.WORD_PARAMETER_PLURAL_VERB_ARE:
				if( ( !isPossessive ||

				// Typical for Chinese
				( isChineseCurrentLanguage &&
				// Chinese test files: "Boiling point" and "Condensation point"
				isUncountableGeneralizationNoun &&
				hasFoundAllSpecificationWords_ ) ) &&

				( !isAssignment ||
				isActiveAssignment ) &&

				( !isSpecificationGeneralization ||
				isQuestion ||
				hasFoundAllSpecificationWords_ ) &&

				!isConditional )
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

			case Constants.WORD_PARAMETER_SINGULAR_VERB_WAS:
			case Constants.WORD_PARAMETER_PLURAL_VERB_WERE:
				if( !isPossessive &&

				( isInactiveAssignment ||
				isArchivedAssignment ) )
					{
					if( isQuestion )
						// Typical for Spanish: No separator after inverted question mark
						isInsertingSeparator = false;

					if( isSpecificationGeneralization )
						hasFoundSpecificationGeneralizationVerb_ = true;

					if( isQuestion ||
					isArchivedAssignment ||
					// Test file: "Connect Four"
					writeSpecificationItem.hasSpecificationCollection() )
						writeWordString_ = predefinedWordString;
					else
						{
						// Force current tense, because the word "previous" is used
						if( ( predefinedWordItem = myWordItem_.predefinedWordItem( grammarDefinitionParameter == Constants.WORD_PARAMETER_SINGULAR_VERB_WAS ? Constants.WORD_PARAMETER_SINGULAR_VERB_IS : Constants.WORD_PARAMETER_PLURAL_VERB_ARE ) ) == null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find the predefined word with word parameter " + grammarDefinitionParameter );

						if( ( writeWordString_ = predefinedWordItem.wordTypeString( true, grammarDefinitionWordTypeNr ) ) == null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't get the word type string from the predefined word with word parameter " + grammarDefinitionParameter );
						}
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_BE:
			case Constants.WORD_PARAMETER_PLURAL_VERB_CAN_BE:
				if( isConditional )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_HAS:
			case Constants.WORD_PARAMETER_PLURAL_VERB_HAVE:
				if( isPossessive &&
				!isArchivedAssignment )
					writeWordString_ = predefinedWordString;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_HAD:
			case Constants.WORD_PARAMETER_PLURAL_VERB_HAD:
				if( isPossessive &&
				isArchivedAssignment )
					writeWordString_ = predefinedWordString;

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
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CHINESE_PUT:
				break;

			case Constants.NO_GRAMMAR_PARAMETER:

				// Don't insert a break statement here

			// Adjectives
			case Constants.WORD_PARAMETER_ADJECTIVE_ASSIGNED:
			case Constants.WORD_PARAMETER_ADJECTIVE_BUSY:
			case Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE:
			case Constants.WORD_PARAMETER_ADJECTIVE_DONE:
			case Constants.WORD_PARAMETER_ADJECTIVE_EMPTY:
			case Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE:
			case Constants.WORD_PARAMETER_ADJECTIVE_FEMININE:
			case Constants.WORD_PARAMETER_ADJECTIVE_INVERTED:
			case Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE:
			case Constants.WORD_PARAMETER_ADJECTIVE_CHINESE_DRAW:
			case Constants.WORD_PARAMETER_ADJECTIVE_CHINESE_HIGH:
			case Constants.WORD_PARAMETER_ADJECTIVE_CHINESE_LOW:

				// Don't insert a break statement here

			// Nouns
			case Constants.WORD_PARAMETER_NOUN_DEVELOPER:
			case Constants.WORD_PARAMETER_NOUN_EXPERT:
			case Constants.WORD_PARAMETER_NOUN_FILE:
			case Constants.WORD_PARAMETER_NOUN_HEAD:
			case Constants.WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
			case Constants.WORD_PARAMETER_NOUN_LANGUAGE:
			case Constants.WORD_PARAMETER_NOUN_MIND:
			case Constants.WORD_PARAMETER_NOUN_NUMBER:
			case Constants.WORD_PARAMETER_NOUN_PASSWORD:
			case Constants.WORD_PARAMETER_NOUN_SOLVE_LEVEL:
			case Constants.WORD_PARAMETER_NOUN_SOLVE_METHOD:
			case Constants.WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
			case Constants.WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
			case Constants.WORD_PARAMETER_NOUN_TAIL:
			case Constants.WORD_PARAMETER_NOUN_TEST_FILE:
			case Constants.WORD_PARAMETER_NOUN_USER:
			case Constants.WORD_PARAMETER_NOUN_INFORMATION:
			case Constants.WORD_PARAMETER_NOUN_CHINESE_MARSUPIAL:
			case Constants.WORD_PARAMETER_NOUN_CHINESE_SAILBOAT:
			case Constants.WORD_PARAMETER_NOUN_CHINESE_TURN:
				if( !isSpecificationWaitingForConjunction_ )
					{
					if( isGeneralization )
						{
						// Matching generalization word type
						if( generalizationWordTypeNr == grammarDefinitionWordTypeNr &&
						writeGeneralizationWordToSentence( ( isSpecificationGeneralization && !hasFoundSingleSpecificationWord_ && !isCharacteristicFor && !isPartOf ), grammarDefinitionWordTypeNr ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the generalization word to the sentence" );
						}
					else
						{
						if( isSpecification )
							{
							if( specificationWordItem == null )
								{
								// Specification string
								if( grammarDefinitionWordTypeNr == Constants.WORD_TYPE_TEXT &&
								// Typical for Chinese
								!writeSpecificationItem.isSpecificationStringAlreadyWritten() )
									{
									if( writeSpecificationItem.markSpecificationStringAsWritten() != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark specification string \"" + writeSpecificationItem.specificationString() + "\" as written" );

									writeWordString_ = new String( Constants.SYMBOL_DOUBLE_QUOTE + writeSpecificationItem.specificationString() + Constants.SYMBOL_DOUBLE_QUOTE );
									}
								}
							else
								{
								// Matching specification word type
								if( ( specificationWordTypeNr == grammarDefinitionWordTypeNr ||
								grammarDefinitionWordTypeNr == Constants.WORD_TYPE_NUMERAL ||
								grammarDefinitionWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR ||
								grammarDefinitionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ) &&

								// Specification word
								writeSpecificationWordToSentence( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, assumptionLevel, grammarDefinitionWordTypeNr, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, writeSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification word to the sentence" );
								}
							}
						else
							{
							if( !hasFoundAllRelationWords_ &&
							// Matching relation word type
							writeSpecificationItem.relationWordTypeNr() == grammarDefinitionWordTypeNr &&

							// Relation context
							writeRelationWordToSentence( grammarDefinitionWordTypeNr, writeSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a relation word to the sentence" );
							}
						}
					}

				break;

			default:
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "I don't know how to handle the grammar definition with grammar parameter: " + grammarDefinitionParameter );
			}

		if( writeWordString_ != null )
			{
			if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
			writtenSentenceStringBuffer.length() == 0 )
				GlobalVariables.writtenSentenceStringBuffer = new StringBuffer( writeWordString_ );
			else
				{
				isSkippingClearWriteLevel_ = false;

				if( isInsertingSeparator )
					GlobalVariables.writtenSentenceStringBuffer.append( Constants.SPACE_STRING );

				GlobalVariables.writtenSentenceStringBuffer.append( writeWordString_ );
				}

			writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer;

			if( isUnknownPluralOfNoun_ )
				writtenSentenceStringBuffer.append( myWordItem_.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) + writeWordString_ + myWordItem_.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING ) );

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
		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given my word item is undefined.\n" );
			}
		}


	// Protected methods

	protected byte writeJustificationSpecificationInWord( boolean isWritingCurrentSpecificationWordOnly, SpecificationItem justificationSpecificationItem )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( justificationSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given justification specification item is undefined" );

		if( justificationSpecificationItem.isReplacedOrDeletedItem() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given justification specification item is a replaced or deleted item" );

		if( writeSelectedSpecification( false, true, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, justificationSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected specification with justification" );

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the selected specification with justification" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a justification sentence" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedRelationInfo( boolean isActiveAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		int relationContextNr;
		SpecificationItem currentSpecificationItem = null;
		StringBuffer writtenSentenceStringBuffer;

		if( writeWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( isActiveAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != null )
			{
			do	{
				if( ( relationContextNr = currentSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
				writeWordItem.hasContextInWord( relationContextNr ) &&
				!currentSpecificationItem.isHiddenSpanishSpecification() )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected specification" );

					if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
					writtenSentenceStringBuffer.length() > 0 )
						{
						if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isQuestion ? Constants.INTERFACE_LISTING_RELATED_QUESTIONS : Constants.INTERFACE_LISTING_RELATED_INFORMATION ) ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a related header" );

						if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecification( boolean isCheckingUserSentenceForIntegrity, boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isJustification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		boolean hasAssignment = false;
		boolean hasRelationContext;
		boolean hasSpecificationCollection;
		boolean hasCompoundSpecificationCollection;
		boolean isArchivedAssignment;
		boolean isAssignment;
		boolean isCombinedSpecification = false;
		boolean isExclusiveSpecification;
		boolean isFirstRelatedSpecification = false;
		boolean isGeneralizationNoun;
		boolean isLastCompoundSpecification = false;
		boolean isLastRelatedSpecification = false;
		boolean isNegative;
		boolean isPossessive;
		boolean isOlderItem;
		boolean isQuestion;
		boolean isSelfGeneratedSingularDefinition = false;
		boolean isSelfGeneratedSpecification;
		boolean isSpecificationGeneralization;
		boolean isSpecificationPluralNoun;
		boolean isUserSpecification;
		boolean isWritingSentenceWithOnlyOneSpecification;
		SpecificationItem foundAssignmentItem;
		SpecificationItem relatedSpecificationItem = null;
		WordItem specificationWordItem;
		StringBuffer storedSentenceStringBuffer;
		StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer = null;
		StringBuffer writtenSentenceStringBuffer;
		RelatedResultType relatedResult;

		GlobalVariables.learnedFromUserStringBuffer = new StringBuffer();
		GlobalVariables.writtenSentenceStringBuffer = new StringBuffer();

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( writeSpecificationItem.isDeletedItem() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is deleted" );

		if( isJustification &&
		writeSpecificationItem.isAnsweredQuestion() &&
		writeSpecificationItem.isSelfGeneratedQuestion() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is obsolete. It is an answered, self-generated question" );

		if( writeSpecificationItem.isHiddenSpanishSpecification() )
			{
			if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI can't write a hidden specification:\n\tSpecificationItem: " + writeSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
			}
		else
			{
			hasRelationContext = writeSpecificationItem.hasRelationContext();
			hasSpecificationCollection = writeSpecificationItem.hasSpecificationCollection();
			hasCompoundSpecificationCollection = writeSpecificationItem.hasCompoundSpecificationCollection();
			isArchivedAssignment = writeSpecificationItem.isArchivedAssignment();
			isAssignment = writeSpecificationItem.isAssignment();
			isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
			isGeneralizationNoun = writeSpecificationItem.isGeneralizationNoun();
			isNegative = writeSpecificationItem.isNegative();
			isOlderItem = writeSpecificationItem.isOlderItem();
			isPossessive = writeSpecificationItem.isPossessive();
			isQuestion = writeSpecificationItem.isQuestion();
			isSelfGeneratedSpecification = writeSpecificationItem.isSelfGeneratedSpecification();
			isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();
			isSpecificationPluralNoun = writeSpecificationItem.isSpecificationPluralNoun();
			isUserSpecification = writeSpecificationItem.isUserSpecification();

			if( ( specificationWordItem = writeSpecificationItem.specificationWordItem() ) != null )
				{
				if( hasSpecificationCollection &&
				!isWritingCurrentSpecificationWordOnly &&
				answerParameter == Constants.NO_ANSWER_PARAMETER )
					{
					if( ( relatedResult = myWordItem_.findRelatedSpecification( true, writeSpecificationItem ) ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

					isFirstRelatedSpecification = relatedResult.isFirstRelatedSpecification;
					isLastRelatedSpecification = relatedResult.isLastRelatedSpecification;

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
					relatedSpecificationItem.isOlderItem() )
						{
						if( !hasCompoundSpecificationCollection &&
						!isNegative &&
						isWritingCurrentSentenceOnly &&

						( !isSpecificationGeneralization ||
						isLastRelatedSpecification ) )
							isCombinedSpecification = true;
						}
					}

				if( !isAssignment &&
				( foundAssignmentItem = myWordItem_.firstAssignmentItem( isPossessive, isQuestion, writeSpecificationItem.relationContextNr(), specificationWordItem ) ) != null &&

				( foundAssignmentItem.hasRelationContext() ||
				!foundAssignmentItem.isArchivedAssignment() ||
				foundAssignmentItem.isSelfGeneratedSpecification() ) )
					hasAssignment = true;

				if( hasCompoundSpecificationCollection &&
				!isFirstRelatedSpecification &&
				isLastRelatedSpecification )
					isLastCompoundSpecification = true;

				if( isGeneralizationNoun &&
				!isSpecificationPluralNoun &&
				writeSpecificationItem.isSelfGeneratedConclusionOrAssumption() )
					isSelfGeneratedSingularDefinition = true;

				if( !hasAssignment &&
				!hasRelationContext &&
				!isExclusiveSpecification &&

				( hasCompoundSpecificationCollection ||
				isNegative ) &&

				( writeSpecificationItem.isSelfGeneratedConclusion() ||

				( !isGeneralizationNoun &&
				writeSpecificationItem.isSelfGeneratedAssumption() &&
				!writeSpecificationItem.hasAdditionalDefinitionSpecificationJustification() ) ) )
					isWritingCurrentSpecificationWordOnly = true;
				}

			if( isCombinedSpecification ||
			isWritingCurrentSpecificationWordOnly ||

			// Self-generated singular definition
			( isSelfGeneratedSingularDefinition &&

			( isLastCompoundSpecification ||
			isForcingResponseNotBeingFirstSpecification ||

			( !hasCompoundSpecificationCollection &&
			isFirstRelatedSpecification ) ||

			( !isExclusiveSpecification &&

			( !isChineseCurrentLanguage_ ||
			// Typical for Chinese
			!isPossessive ||
			GlobalVariables.nUserSpecificationWords == 1 ) ) ) ) ||

			// Not self-generated singular definition
			( !isSelfGeneratedSingularDefinition &&

			( isForcingResponseNotBeingFirstSpecification ||

			( ( isFirstRelatedSpecification ||
			relatedSpecificationItem == null ) &&

			( isForcingResponseNotBeingAssignment ||
			// Test file: "Connect-Four - Display information about the set"
			writeSpecificationItem.isConditional() ||

			// Test file: "US presidents"
			( hasRelationContext ||

			// Past tense
			( !hasAssignment &&
			isSelfGeneratedSpecification ) ||

			// Test file: "Connect-Four - Display information about the set"
			( !isArchivedAssignment &&

			( !hasAssignment ||
			// Test file: "Correcting invalidated assumption (by opposite suggestive question)"
			writeSpecificationItem.isCorrectedSpecification() ) ) ) ) ) ) ) )
				{
				isWritingSentenceWithOnlyOneSpecification = ( isWritingCurrentSpecificationWordOnly &&
															( storedSentenceWithOnlyOneSpecificationStringBuffer = writeSpecificationItem.storedSentenceWithOnlyOneSpecificationStringBuffer() ) != null );

				// Efficiency: Use the stored sentence if still up-to-date
				if( ( storedSentenceStringBuffer = writeSpecificationItem.storedSentenceStringBuffer() ) != null &&
				answerParameter == Constants.NO_ANSWER_PARAMETER &&

				( isQuestion ||
				isWritingSentenceWithOnlyOneSpecification ||

				// No relation context
				( !hasRelationContext &&

				( !hasSpecificationCollection ||
				isExclusiveSpecification ||
				writeSpecificationItem.isSpecificationAdjective() ||

				( ( !isJustification ||
				isArchivedAssignment ) &&

				!isSpecificationGeneralization &&
				!isSpecificationPluralNoun ) ||

				// User specification
				( isUserSpecification &&

				( !hasCompoundSpecificationCollection ||
				!isWritingCurrentSpecificationWordOnly ) ) ||

				// Multiple specification
				( !isSpecificationPluralNoun &&
				!myWordItem_.hasMultipleSpecificationWordsWithSameSentenceNr( writeSpecificationItem.creationSentenceNr(), writeSpecificationItem.itemNr(), writeSpecificationItem.specificationCollectionNr() ) ) ) ) ||

				// Relation context
				( hasRelationContext &&
				!writeSpecificationItem.hasRelationContextCurrentlyBeenUpdated() ) ) )
					{
					// Use the stored sentence
					if( isCheckingUserSentenceForIntegrity )
						GlobalVariables.writtenUserSentenceStringBuffer = storedSentenceStringBuffer;

					GlobalVariables.writtenSentenceStringBuffer = ( isWritingSentenceWithOnlyOneSpecification ? storedSentenceWithOnlyOneSpecificationStringBuffer : storedSentenceStringBuffer );
					}
				else
					{
					// Efficiency: Use the stored sentence of a related specification
					if( isOlderItem &&

					// Non-compound user question
					( !isQuestion ||
					!hasCompoundSpecificationCollection ||
					!isSelfGeneratedSpecification ) &&

					relatedSpecificationItem != null &&
					// Avoid mixing user specification with self-generated specification
					relatedSpecificationItem.isUserSpecification() == isUserSpecification &&
					// Avoid mixing different assumption levels
					relatedSpecificationItem.assumptionLevel() == writeSpecificationItem.assumptionLevel() &&
					( storedSentenceStringBuffer = relatedSpecificationItem.storedSentenceStringBuffer() ) != null )
						// Use the stored sentence of a related specification

						GlobalVariables.writtenSentenceStringBuffer = new StringBuffer( storedSentenceStringBuffer );
					else
						{
						if( writeSpecificationSentence( isAssignment, isArchivedAssignment, isCheckingUserSentenceForIntegrity, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, Constants.NO_GRAMMAR_LEVEL, myWordItem_.firstCurrentLanguageWritingGrammarItem( isQuestion ), writeSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification sentence" );

						// Used for developer statistics
						GlobalVariables.nConstructedSentences++;

						// Under certain conditions, the sentence will be stored, in order to be re-used if needed
						if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
						( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
						writtenSentenceStringBuffer.length() > 0 )
							{
							if( isWritingCurrentSpecificationWordOnly )
								writeSpecificationItem.storedWrittenSentenceStringBufferWithOneSpecificationOnly();
							else
								{
								if( !hasRelationContext ||
								!isAssignment ||
								!isExclusiveSpecification ||
								isOlderItem ||
								writeSpecificationItem.isInactiveAssignment() )
									writeSpecificationItem.storeWrittenSentenceStringBuffer();
								}
							}
						}
					}

				if( writeSpecificationItem.userNr() != GlobalVariables.currentUserNr &&
				( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
				writtenSentenceStringBuffer.length() > 0 )
					GlobalVariables.learnedFromUserStringBuffer = new StringBuffer( myWordItem_.interfaceString( Constants.INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_START ) + myWordItem_.userNameString( writeSpecificationItem.userNr() ) + myWordItem_.interfaceString( Constants.INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_END ) );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecificationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, WordItem writeWordItem )
		{
		SpecificationItem currentSpecificationItem;
		StringBuffer writtenSentenceStringBuffer;

		if( writeWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, false ) ) != null )
			{
			do	{
				if( currentSpecificationItem.specificationWordItem() == writeWordItem )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected specification" );

					if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
					writtenSentenceStringBuffer.length() > 0 )
						{
						if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WRITE, Constants.INTERFACE_LISTING_SPECIFICATIONS ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a related header" );

						if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSpecificationSentence( boolean isAssignment, boolean isArchivedAssignment, boolean isCheckingUserSentenceForIntegrity, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		boolean isChineseCurrentLanguage = isChineseCurrentLanguage_;
		boolean isChoice;
		boolean isChoiceEnd;
		boolean isOption;
		boolean isSkippingThisChoiceOrOptionPart;
		boolean isSkippingNextChoiceOrOptionParts;
		boolean isStillSuccessful;
		short startWriteLevel = GlobalVariables.currentWriteLevel;
		// Take the current write sentence string buffer length as start position
		int startWordPosition = ( GlobalVariables.writtenSentenceStringBuffer == null ? 0 : GlobalVariables.writtenSentenceStringBuffer.length() );
		GrammarItem definitionGrammarItem = selectedGrammarItem;
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
			initializePrivateWriteVariables();

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
				// Efficiency: Only select useful grammar definitions
				if( definitionGrammarItem.isUsefulGrammarDefinition( isAssignment, isArchivedAssignment, isChineseCurrentLanguage, isPossessive, isQuestion, isSpecificationGeneralization ) )
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

							if( writeSpecificationSentence( isAssignment, isArchivedAssignment, false, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, (short)( grammarLevel + 1 ), selectedGrammarItem.definitionGrammarItem, writeSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a specification sentence at grammar level: " + ( grammarLevel + 1 ) );

							if( !hasFoundWordToWrite_ )
								// Failed, try next part
								isSkippingThisChoiceOrOptionPart = true;
							}

						isChoiceEnd = selectedGrammarItem.isChoiceEnd;

						if( isChoiceEnd ||
						selectedGrammarItem.isOptionEnd )
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

						selectedGrammarItem = selectedGrammarItem.nextGrammarItem();

						if( !isChoice &&
						!isOption &&
						!hasFoundWordToWrite_ )
							isStillSuccessful = false;
						}
					while( !selectedGrammarItem.isDefinitionStart() );

					if( !hasFoundWordToWrite_ &&
					!isSkippingClearWriteLevel_ &&
					// The sentence has grown
					( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
					writtenSentenceStringBuffer.length() > startWordPosition &&
					// Clean up write info
					cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to cleanup the write info" );
					}
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

			if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
			writtenSentenceStringBuffer.length() > 0 )
				{
				if( isCheckingUserSentenceForIntegrity )
					GlobalVariables.writtenUserSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer;

				// Typical for French: Merged words
				if( myWordItem_.shrinkMergedWordsInWriteSentenceOfCurrentLanguage() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to shrink the merged words in the write sentence string" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isCorrectedSpecificationByKnowledge, boolean isCorrectedSpecificationByOppositeSuggestiveQuestion, boolean isForcingResponseNotBeingFirstSpecification, boolean isReplacedBySpecificationWithRelation, boolean isWritingCurrentSpecificationWordOnly, boolean wasHiddenSpanishSpecification, SpecificationItem writeSpecificationItem )
		{
		short interfaceParameter;
		StringBuffer writtenSentenceStringBuffer;

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( writeSelectedSpecification( false, true, isForcingResponseNotBeingFirstSpecification, false, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the given specification item" );

		// No sentence written
		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given specification item" );

		interfaceParameter = ( isReplacedBySpecificationWithRelation ?
							// Replaced by specification with relation
							( writeSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_EARLIER_ASSUMPTIONS_THAT_HAVE_RELATION_WORDS_NOW :
																					Constants.INTERFACE_LISTING_MY_EARLIER_CONCLUSIONS_THAT_HAVE_RELATION_WORDS_NOW ) :

							// Adjusted question
							( isAdjustedSpecification ? ( writeSpecificationItem.isQuestion() ? Constants.INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS :
							// Adjusted or concluded assumptions
							( writeSpecificationItem.isSelfGeneratedConclusion() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_CONCLUDED :
																					Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_ADJUSTED ) ) :

							// Corrected assumption
							( isCorrectedSpecificationByKnowledge ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE :
							( isCorrectedSpecificationByOppositeSuggestiveQuestion ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_SUGGESTIVE_QUESTION :

							// Typical for Spanish: Not hidden anymore
							( wasHiddenSpanishSpecification ? ( writeSpecificationItem.hasAssumptionLevel() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_NOT_HIDDEN_ANYMORE :
																												Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_NOT_HIDDEN_ANYMORE ) :

							// Answered questions
							( writeSpecificationItem.isSelfGeneratedSpecification() ? Constants.INTERFACE_LISTING_MY_QUESTIONS_THAT_HAVE_BEEN_ANSWERED :
																						Constants.INTERFACE_LISTING_YOUR_QUESTIONS_THAT_HAVE_BEEN_ANSWERED ) ) ) ) ) );

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a header" );

		writeSpecificationItem.hasAdjustedSpecificationBeenWritten = true;

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );

		return Constants.RESULT_OK;
		}
	}

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
