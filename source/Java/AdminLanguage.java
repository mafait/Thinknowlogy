/*
 *	Class:			AdminLanguage
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign the grammar and interface languages
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

class AdminLanguage
	{
	// Private constructible variables

	private WordItem foundLanguageWordItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;
	private String languageKey_;


	// Private methods

	private byte findLanguageByName( String languageNameString )
		{
		WordResultType wordResult;
		foundLanguageWordItem_ = null;

		if( languageNameString != null )
			{
			// Initially the language name words are not linked
			// to the language defintion word. So, search in all words.
			if( ( wordResult = myWordItem_.findWordTypeInAllWords( false, Constants.WORD_TYPE_PROPER_NAME, languageNameString.toString(), null ) ).result == Constants.RESULT_OK )
				foundLanguageWordItem_ = wordResult.foundWordItem;
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a language word" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given language name string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminLanguage( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		foundLanguageWordItem_ = null;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();
		languageKey_ = moduleNameString_;		// The address of the class is the key - not the content of the string

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte authorizeLanguageWord( WordItem authorizationWordItem )
		{
		if( authorizationWordItem != null )
			{
			if( authorizationWordItem.assignChangePermissions( languageKey_ ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign my language change permissions to a word" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given authorization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte createGrammarLanguage( String languageNameString )
		{
		WordResultType wordResult;
		WordItem grammarLanguageWordItem;

		if( findLanguageByName( languageNameString ) == Constants.RESULT_OK )
			{
			if( foundLanguageWordItem_ == null )
				{
				CommonVariables.currentGrammarLanguageNr++;

				if( ( wordResult = adminItem_.addWord( true, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, languageNameString.length(), languageNameString ) ).result == Constants.RESULT_OK )
					{
					if( ( grammarLanguageWordItem = wordResult.createdWordItem ) != null )
						{
						if( adminItem_.authorizeLanguageWord( grammarLanguageWordItem ) == Constants.RESULT_OK )
							CommonVariables.currentGrammarLanguageWordItem = grammarLanguageWordItem;
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the created grammar language word" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The last created word item is undefined" );
					}
				else
					{
					CommonVariables.currentGrammarLanguageNr--;		// Restore old grammar language
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a grammar language word" );
					}
				}
			else
				CommonVariables.currentGrammarLanguageWordItem = foundLanguageWordItem_;
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find the grammar language" );

		return Constants.RESULT_OK;
		}

	protected byte createInterfaceLanguage( String languageNameString )
		{
		WordResultType wordResult;
		WordItem interfaceLanguageWordItem;

		if( findLanguageByName( languageNameString ) == Constants.RESULT_OK )
			{
			if( foundLanguageWordItem_ == null )
				{
				CommonVariables.currentGrammarLanguageNr++;

				if( ( wordResult = adminItem_.addWord( true, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, languageNameString.length(), languageNameString ) ).result == Constants.RESULT_OK )
					{
					if( ( interfaceLanguageWordItem = wordResult.createdWordItem ) != null )
						{
						if( adminItem_.authorizeLanguageWord( interfaceLanguageWordItem ) == Constants.RESULT_OK )
							CommonVariables.currentInterfaceLanguageWordItem = interfaceLanguageWordItem;
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the created interface language word" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The last created word item is undefined" );
					}
				else
					{
					CommonVariables.currentGrammarLanguageNr--;	// Restore old grammar language number
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add an interface language word" );
					}
				}
			else
				CommonVariables.currentInterfaceLanguageWordItem = foundLanguageWordItem_;
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find the grammar language" );

		return Constants.RESULT_OK;
		}

	protected byte createLanguageSpecification( WordItem languageWordItem, WordItem languageNounWordItem )
		{
		if( languageWordItem != null )
			{
			if( languageWordItem.isProperName() )
				{
				if( languageNounWordItem != null )
					{
					if( adminItem_.addSpecification( true, false, false, false, false, false, false, false, false, false, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_ASSUMPTION_LEVEL, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, languageWordItem, languageNounWordItem, null, null ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a new language specification" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given language noun word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given language word isn't a proper name" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given language word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte assignGrammarAndInterfaceLanguage( String languageNameString )
		{
		boolean hasFoundLanguage = false;
		SpecificationItem languageSpecificationItem;

		if( findLanguageByName( languageNameString ) == Constants.RESULT_OK )
			{
			if( foundLanguageWordItem_ != null )
				{
				if( foundLanguageWordItem_.isGrammarLanguageWord() )
					{
					hasFoundLanguage = true;

					if( ( languageSpecificationItem = foundLanguageWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, CommonVariables.predefinedNounGrammarLanguageWordItem ) ) != null )
						CommonVariables.currentGrammarLanguageNr = languageSpecificationItem.grammarLanguageNr();

					if( CommonVariables.currentGrammarLanguageWordItem != foundLanguageWordItem_ )
						{
						if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundLanguageWordItem_, CommonVariables.predefinedNounGrammarLanguageWordItem, null ).result == Constants.RESULT_OK )
							CommonVariables.currentGrammarLanguageWordItem = foundLanguageWordItem_;
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the grammar language" );
						}
					}

				if( foundLanguageWordItem_.isInterfaceLanguageWord() )
					{
					hasFoundLanguage = true;

					if( CommonVariables.currentInterfaceLanguageWordItem != foundLanguageWordItem_ )
						{
						if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundLanguageWordItem_, adminItem_.predefinedNounInterfaceLanguageWordItem(), null ).result == Constants.RESULT_OK )
							CommonVariables.currentInterfaceLanguageWordItem = foundLanguageWordItem_;
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the interface language" );
						}
					}

				if( !hasFoundLanguage )
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given name isn't a grammar nor an interface language" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the requested language" );
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find the grammar language" );

		return Constants.RESULT_OK;
		}

	protected byte assignGrammarAndInterfaceLanguage( short newLanguageNr )
		{
		WordItem grammarLanguageWordItem;

		if( ( grammarLanguageWordItem = myWordItem_.grammarLanguageWordItem( newLanguageNr ) ) != null )
			{
			CommonVariables.currentGrammarLanguageNr = newLanguageNr;
			CommonVariables.currentGrammarLanguageWordItem = grammarLanguageWordItem;

			if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, CommonVariables.currentGrammarLanguageWordItem, CommonVariables.predefinedNounGrammarLanguageWordItem, null ).result == Constants.RESULT_OK )
				{
				// Also assign the interface language, if possible
				if( CommonVariables.currentGrammarLanguageWordItem.isInterfaceLanguageWord() )
					{
					CommonVariables.currentInterfaceLanguageWordItem = CommonVariables.currentGrammarLanguageWordItem;

					if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, CommonVariables.currentInterfaceLanguageWordItem, adminItem_.predefinedNounInterfaceLanguageWordItem(), null ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the interface language with authorization" );
					}
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the grammar language with authorization" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the requested language" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem != null )
			{
			if( ( specificationResult = generalizationWordItem.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, adminItem_.isUserSentenceMixOfFeminineAndMasculineSpecificationWords(), isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, languageKey_ ) ).result != Constants.RESULT_OK )
				myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification with authorization" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem != null )
			{
			if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, languageKey_ ) ).result != Constants.RESULT_OK )
				myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign a specification with authorization" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"The Sovereign Lord has given me his words of wisdom,
 *	so that I know how to comfort the weary.
 *	Morning by morning he wakens me
 *	and opens my understanding to his will." (Psalm 50:4)
 *************************************************************************/
