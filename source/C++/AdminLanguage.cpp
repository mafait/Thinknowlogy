/*
 *	Class:			AdminLanguage
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign the grammar and interface languages
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

#include "AdminItem.h"
#include "SpecificationItem.cpp"

class AdminLanguage
	{
	friend class AdminItem;

	// Private constructible variables

	WordItem *foundLanguageWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType findLanguageByName( char *languageNameString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findLanguageByName";
		foundLanguageWordItem_ = NULL;

		if( languageNameString != NULL )
			{
			// Initially the language name words are not linked
			// to the language defintion word. So, search in all words.
			if( ( wordResult = myWordItem_->findWordTypeInAllWords( false, WORD_TYPE_PROPER_NAME, languageNameString, NULL ) ).result == RESULT_OK )
				foundLanguageWordItem_ = wordResult.foundWordItem;
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a language word" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given language name string is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminLanguage( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		foundLanguageWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminLanguage" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType authorizeLanguageWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeLanguageWord";

		if( authorizationWordItem != NULL )
			{
			if( authorizationWordItem->assignChangePermissions( this ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign my language change permissions to a word" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given authorization word item is undefined" );

		return RESULT_OK;
		}

	ResultType createGrammarLanguage( char *languageNameString )
		{
		WordResultType wordResult;
		WordItem *grammarLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGrammarLanguage";

		if( findLanguageByName( languageNameString ) == RESULT_OK )
			{
			if( foundLanguageWordItem_ == NULL )
				{
				commonVariables_->currentGrammarLanguageNr++;

				if( ( wordResult = adminItem_->addWord( true, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_PROPER_NAME, strlen( languageNameString ), languageNameString ) ).result == RESULT_OK )
					{
					if( ( grammarLanguageWordItem = wordResult.createdWordItem ) != NULL )
						{
						if( adminItem_->authorizeLanguageWord( grammarLanguageWordItem ) == RESULT_OK )
							commonVariables_->currentGrammarLanguageWordItem = grammarLanguageWordItem;
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to authorize the created grammar language word" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The last created word item is undefined" );
					}
				else
					{
					commonVariables_->currentGrammarLanguageNr--;		// Restore old grammar language
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a grammar language word" );
					}
				}
			else
				commonVariables_->currentGrammarLanguageWordItem = foundLanguageWordItem_;
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the grammar language" );

		return RESULT_OK;
		}

	ResultType createInterfaceLanguage( char *languageNameString )
		{
		WordResultType wordResult;
		WordItem *interfaceLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createInterfaceLanguage";

		if( findLanguageByName( languageNameString ) == RESULT_OK )
			{
			if( foundLanguageWordItem_ == NULL )
				{
				commonVariables_->currentGrammarLanguageNr++;

				if( ( wordResult = adminItem_->addWord( true, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_PROPER_NAME, strlen( languageNameString ), languageNameString ) ).result == RESULT_OK )
					{
					if( ( interfaceLanguageWordItem = wordResult.createdWordItem ) != NULL )
						{
						if( adminItem_->authorizeLanguageWord( interfaceLanguageWordItem ) == RESULT_OK )
							commonVariables_->currentInterfaceLanguageWordItem = interfaceLanguageWordItem;
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to authorize the created interface language word" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The last created word item is undefined" );
					}
				else
					{
					commonVariables_->currentGrammarLanguageNr--;	// Restore old grammar language number
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add an interface language word" );
					}
				}
			else
				commonVariables_->currentInterfaceLanguageWordItem = foundLanguageWordItem_;
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the grammar language" );

		return RESULT_OK;
		}

	ResultType createLanguageSpecification( WordItem *languageWordItem, WordItem *languageNounWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createLanguageSpecification";

		if( languageWordItem != NULL )
			{
			if( languageWordItem->isProperName() )
				{
				if( languageNounWordItem != NULL )
					{
					if( adminItem_->addSpecification( true, false, false, false, false, false, false, false, false, false, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_ASSUMPTION_LEVEL, WORD_TYPE_PROPER_NAME, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, languageWordItem, languageNounWordItem, NULL, NULL ).result != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a new language specification" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given language noun word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given language word isn't a proper name" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given language word item is undefined" );

		return RESULT_OK;
		}

	ResultType assignGrammarAndInterfaceLanguage( char *languageNameString )
		{
		bool hasFoundLanguage = false;
		SpecificationItem *languageSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignGrammarAndInterfaceLanguage";

		if( findLanguageByName( languageNameString ) == RESULT_OK )
			{
			if( foundLanguageWordItem_ != NULL )
				{
				if( foundLanguageWordItem_->isGrammarLanguageWord() )
					{
					hasFoundLanguage = true;

					if( ( languageSpecificationItem = foundLanguageWordItem_->firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, commonVariables_->predefinedNounGrammarLanguageWordItem ) ) != NULL )
						commonVariables_->currentGrammarLanguageNr = languageSpecificationItem->grammarLanguageNr();

					if( commonVariables_->currentGrammarLanguageWordItem != foundLanguageWordItem_ )
						{
						if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, foundLanguageWordItem_, commonVariables_->predefinedNounGrammarLanguageWordItem, NULL ).result == RESULT_OK )
							commonVariables_->currentGrammarLanguageWordItem = foundLanguageWordItem_;
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the grammar language" );
						}
					}

				if( foundLanguageWordItem_->isInterfaceLanguageWord() )
					{
					hasFoundLanguage = true;

					if( commonVariables_->currentInterfaceLanguageWordItem != foundLanguageWordItem_ )
						{
						if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, foundLanguageWordItem_, adminItem_->predefinedNounInterfaceLanguageWordItem(), NULL ).result == RESULT_OK )
							commonVariables_->currentInterfaceLanguageWordItem = foundLanguageWordItem_;
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the interface language" );
						}
					}

				if( !hasFoundLanguage )
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given name isn't a grammar nor an interface language" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the requested language" );
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the grammar language" );

		return RESULT_OK;
		}

	ResultType assignGrammarAndInterfaceLanguage( unsigned short newLanguageNr )
		{
		WordItem *grammarLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignGrammarAndInterfaceLanguage";

		if( ( grammarLanguageWordItem = myWordItem_->grammarLanguageWordItem( newLanguageNr ) ) != NULL )
			{
			commonVariables_->currentGrammarLanguageNr = newLanguageNr;
			commonVariables_->currentGrammarLanguageWordItem = grammarLanguageWordItem;

			if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, commonVariables_->currentGrammarLanguageWordItem, commonVariables_->predefinedNounGrammarLanguageWordItem, NULL ).result == RESULT_OK )
				{
				// Also assign the interface language, if possible
				if( commonVariables_->currentGrammarLanguageWordItem->isInterfaceLanguageWord() )
					{
					commonVariables_->currentInterfaceLanguageWordItem = commonVariables_->currentGrammarLanguageWordItem;

					if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, commonVariables_->currentInterfaceLanguageWordItem, adminItem_->predefinedNounInterfaceLanguageWordItem(), NULL ).result != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the interface language with authorization" );
					}
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the grammar language with authorization" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the requested language" );

		return RESULT_OK;
		}

	SpecificationResultType addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool isUserSpecificationExclusivelyFeminineOrMasculine, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( generalizationWordItem != NULL )
			{
			if( ( specificationResult = generalizationWordItem->addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, this ) ).result != RESULT_OK )
				myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecificationWithAuthorization";

		if( generalizationWordItem != NULL )
			{
			if( ( specificationResult = generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, this ) ).result != RESULT_OK )
				myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign a specification with authorization" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}
	};

/*************************************************************************
 *
 *	"The Sovereign Lord has given me his words of wisdom,
 *	so that I know how to comfort the weary.
 *	Morning by morning he wakens me
 *	and opens my understanding to his will." (Psalm 50:4)
 *
 *************************************************************************/
