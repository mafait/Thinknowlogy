/*
 *	Class:			AdminLanguage
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign the languages
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

#include "AdminItem.h"
#include "SpecificationItem.cpp"

class AdminLanguage
	{
	friend class AdminItem;

	// Private constructible variables

	WordItem *foundLanguageWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
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
			if( ( wordResult = adminItem_->findWordTypeInAllWords( true, WORD_TYPE_PROPER_NAME, languageNameString, NULL ) ).result == RESULT_OK )
				foundLanguageWordItem_ = wordResult.foundWordItem;
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a language word" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given language name string is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminLanguage( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		foundLanguageWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminLanguage" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
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
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign my language change permissions to a word" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given authorization word item is undefined" );

		return RESULT_OK;
		}

	ResultType createLanguage( char *languageNameString )
		{
		WordResultType wordResult;
		WordItem *languageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createLanguage";

		if( findLanguageByName( languageNameString ) == RESULT_OK )
			{
			if( foundLanguageWordItem_ == NULL )
				{
				commonVariables_->currentLanguageNr++;

				if( ( wordResult = adminItem_->addWord( true, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_PROPER_NAME, strlen( languageNameString ), languageNameString ) ).result == RESULT_OK )
					{
					if( ( languageWordItem = wordResult.createdWordItem ) != NULL )
						{
						if( adminItem_->authorizeLanguageWord( languageWordItem ) == RESULT_OK )
							{
							commonVariables_->currentLanguageWordItem = languageWordItem;

							if( adminItem_->startupLanguageNameString == NULL ||
							strlen( adminItem_->startupLanguageNameString ) == 0 )
								adminItem_->startupLanguageNameString = languageWordItem->anyWordTypeString();

							if( commonVariables_->predefinedNounLanguageWordItem != NULL )
								{
								if( createLanguageSpecification( commonVariables_->predefinedNounLanguageWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a language specification" );
								}
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to authorize the created language word" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The last created word item is undefined" );
					}
				else
					{
					// Restore old language
					commonVariables_->currentLanguageNr--;
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a language word" );
					}
				}
			else
				commonVariables_->currentLanguageWordItem = foundLanguageWordItem_;
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the language" );

		return RESULT_OK;
		}

	ResultType createLanguageSpecification( WordItem *languageNounWordItem )
		{
		WordItem *currentLanguageWordItem = commonVariables_->currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createLanguageSpecification";

		if( currentLanguageWordItem != NULL )
			{
			if( currentLanguageWordItem->isLanguageWord() )
				{
				if( languageNounWordItem != NULL )
					{
					if( adminItem_->addSpecification( true, false, false, false, false, false, false, false, false, false, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_ASSUMPTION_LEVEL, WORD_TYPE_PROPER_NAME, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, currentLanguageWordItem, languageNounWordItem, NULL, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a new language specification" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given language noun word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given language word isn't a language word" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The current language word item is undefined" );

		return RESULT_OK;
		}

	ResultType assignLanguage( unsigned short newLanguageNr )
		{
		WordItem *languageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignLanguage";

		if( ( languageWordItem = adminItem_->languageWordItem( newLanguageNr ) ) != NULL )
			{
			commonVariables_->currentLanguageNr = newLanguageNr;
			commonVariables_->currentLanguageWordItem = languageWordItem;

			if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, commonVariables_->currentLanguageWordItem, commonVariables_->predefinedNounLanguageWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language with authorization" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the requested language" );

		return RESULT_OK;
		}

	ResultType assignLanguage( char *languageNameString )
		{
		bool hasFoundLanguage = false;
		SpecificationItem *languageSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignLanguage";

		if( findLanguageByName( languageNameString ) == RESULT_OK )
			{
			if( foundLanguageWordItem_ != NULL )
				{
				hasFoundLanguage = true;

				if( ( languageSpecificationItem = foundLanguageWordItem_->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, commonVariables_->predefinedNounLanguageWordItem ) ) != NULL )
					commonVariables_->currentLanguageNr = languageSpecificationItem->languageNr();

				if( commonVariables_->currentLanguageWordItem != foundLanguageWordItem_ )
					{
					if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, foundLanguageWordItem_, commonVariables_->predefinedNounLanguageWordItem, NULL ).result == RESULT_OK )
						commonVariables_->currentLanguageWordItem = foundLanguageWordItem_;
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language" );
					}

				if( !hasFoundLanguage )
					return adminItem_->startError( functionNameString, moduleNameString_, "The given name isn't a grammar nor an language" );
				}
			else
				{
				if( !adminItem_->isSystemStartingUp() )
					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the requested language" );
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the language" );

		return RESULT_OK;
		}

	SpecificationResultType addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( generalizationWordItem != NULL )
			{
			if( ( specificationResult = generalizationWordItem->addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, this ) ).result != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecificationWithAuthorization";

		if( generalizationWordItem != NULL )
			{
			if( ( specificationResult = generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, this ) ).result != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a specification with authorization" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"The Sovereign Lord has given me his words of wisdom,
 *	so that I know how to comfort the weary.
 *	Morning by morning he wakens me
 *	and opens my understanding to his will." (Psalm 50:4)
 *************************************************************************/
