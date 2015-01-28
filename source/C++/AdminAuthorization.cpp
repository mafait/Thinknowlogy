/*
 *	Class:			AdminAuthorization
 *	Supports class:	AdminItem
 *	Purpose:		To handle authorization
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "Presentation.cpp"
#include "SpecificationItem.cpp"

class AdminAuthorization
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasUserPasswordAssigned_;
	bool hasUserEnteredCorrectPassword_;

	unsigned int loginSentenceNr_;

	SpecificationItem *passwordAssignmentItem_;

	WordItem *currentUserWordItem_;
	WordItem *foundUserWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType findUserWord( char *userNameString )
		{
		WordResultType wordResult;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findUserWord";

		foundUserWordItem_ = NULL;

		if( userNameString != NULL )
			{
			if( commonVariables_->predefinedNounUserWordItem != NULL )
				{
				if( ( currentGeneralizationItem = commonVariables_->predefinedNounUserWordItem->firstSpecificationGeneralizationItem() ) != NULL )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
							{
							if( ( wordResult = currentGeneralizationWordItem->findWordType( false, WORD_TYPE_PROPER_NAME, userNameString ) ).result == RESULT_OK )
								{
								if( wordResult.foundWordTypeItem != NULL )
									foundUserWordItem_ = currentGeneralizationWordItem;
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the user name" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
					}
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The predefined user noun word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user name string is undefined" );

		return RESULT_OK;
		}

	ResultType checkUserForPasswordAssignment( WordItem *userWordItem )
		{
		SpecificationResultType specificationResult;
		WordItem *predefinedNounPasswordWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserForPasswordAssignment";

		hasUserPasswordAssigned_ = true;
		passwordAssignmentItem_ = NULL;

		if( userWordItem != NULL )
			{
			if( ( predefinedNounPasswordWordItem = adminItem_->predefinedNounPasswordWordItem() ) != NULL )
				{
				if( ( specificationResult = predefinedNounPasswordWordItem->findAssignmentByRelationWord( false, false, false, false, NO_QUESTION_PARAMETER, userWordItem ) ).result == RESULT_OK )
					{
					if( ( passwordAssignmentItem_ = specificationResult.foundSpecificationItem ) == NULL )
						{
						// No assignment found. So, check for explict negative password specification
						if( userWordItem->firstNonQuestionAssignmentOrSpecificationItem( false, false, false, true, true, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, predefinedNounPasswordWordItem ) != NULL )
							hasUserPasswordAssigned_ = false;
						}
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the password assignment" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The predefined password noun word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user word item is undefined" );

		return RESULT_OK;
		}

	ResultType checkPassword( char *enteredPasswordString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkPassword";
		hasUserEnteredCorrectPassword_ = false;		// Fail safe

		if( enteredPasswordString != NULL )
			{
			if( passwordAssignmentItem_ != NULL )
				{
				if( passwordAssignmentItem_->specificationWordItem() != NULL )
					{

					if( passwordAssignmentItem_->specificationWordItem()->isCorrectHiddenWordType( passwordAssignmentItem_->specificationWordTypeNr(), enteredPasswordString, this ) )
						hasUserEnteredCorrectPassword_ = true;
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The password assignment specification item is undefined" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given password string is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminAuthorization( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasUserPasswordAssigned_ = true;
		hasUserEnteredCorrectPassword_ = false;

		loginSentenceNr_ = NO_SENTENCE_NR;

		passwordAssignmentItem_ = NULL;
		currentUserWordItem_ = NULL;
		foundUserWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminAuthorization" );

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

	unsigned int firstUserSentenceNr()
		{
		return ( loginSentenceNr_ + 1 );
		}

	ResultType authorizeWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeWord";

		if( authorizationWordItem != NULL )
			{
			if( adminItem_->isSystemStartingUp() )
				{
				if( authorizationWordItem->assignChangePermissions( this ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign my authorization permissions to a word" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "You are not authorized to authorize the given word" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given authorization word item is undefined" );

		return RESULT_OK;
		}

	ResultType login( WordItem *specificationWordItem )
		{
		WordItem *predefinedNounPasswordWordItem;
		WordItem *predefinedNounUserWordItem;
		char readUserNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char readPasswordString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "login";

		hasUserPasswordAssigned_ = true;
		hasUserEnteredCorrectPassword_ = false;

		passwordAssignmentItem_ = NULL;
		foundUserWordItem_ = NULL;

		if( ( predefinedNounUserWordItem = commonVariables_->predefinedNounUserWordItem ) != NULL )
			{
			if( ( predefinedNounPasswordWordItem = adminItem_->predefinedNounPasswordWordItem() ) != NULL )
				{
				if( specificationWordItem == NULL )		// No user name is given
					{
					if( adminItem_->getUserInput( false, true, false, predefinedNounUserWordItem->singularNounString(), readUserNameString ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read to user name" );
					}
				else
					strcat( readUserNameString, specificationWordItem->anyWordTypeString() );

				if( strlen( readUserNameString ) > 0 )
					{
					if( findUserWord( readUserNameString ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the user" );
					}

				if( foundUserWordItem_ != NULL &&
				foundUserWordItem_ == currentUserWordItem_ )
					{
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START, readUserNameString, INTERFACE_CONSOLE_LOGIN_END ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
					}
				else
					{
					if( foundUserWordItem_ != NULL )
						{
						if( checkUserForPasswordAssignment( foundUserWordItem_ ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check the user for password assignment" );
						}

					if( foundUserWordItem_ != NULL &&
					!hasUserPasswordAssigned_ )
						hasUserEnteredCorrectPassword_ = true;		// User has no password
					else
						{
						if( adminItem_->getUserInput( true, true, false, predefinedNounPasswordWordItem->singularNounString(), readPasswordString ) == RESULT_OK )
							{
							if( foundUserWordItem_ != NULL )	// Only check password if user was found
								{
								if( checkPassword( readPasswordString ) != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check the user and password" );
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read to password" );
						}

					if( hasUserEnteredCorrectPassword_ &&
					foundUserWordItem_ != NULL )
						{

						if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, foundUserWordItem_, predefinedNounUserWordItem, NULL ).result == RESULT_OK )
							{
							currentUserWordItem_ = foundUserWordItem_;
							loginSentenceNr_ = commonVariables_->currentSentenceNr;
							commonVariables_->firstUserSentenceNr = loginSentenceNr_;
							commonVariables_->currentUserNr = predefinedNounUserWordItem->getUserNr( foundUserWordItem_ );
/*
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_LOGIN_WELCOME, readUserNameString, INTERFACE_CONSOLE_LOGIN_END ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
*/							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the user" );
						}
					else
						{
						if( !adminItem_->isSystemStartingUp() )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_LOGIN_FAILED ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
							}
						else
							return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "The user name or it's password isn't correct" );
						}
					}
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The predefined password noun word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The predefined user noun word item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		WordItem *predefinedVerbLoginWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( generalizationWordItem->isNounPassword() ||
				specificationWordItem->isNounExpert() ||
				specificationWordItem->isNounUser() ||
				generalizationWordItem->isVerbImperativeLogin() )
					{
					if( generalizationWordItem->isNounPassword() )
						{
						if( specificationWordItem->hideWordType( specificationWordTypeNr, this ) != RESULT_OK )
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to hide a password" );
						}
					else
						{
						if( !isNegative &&
						specificationWordItem->isNounUser() &&
						questionParameter == NO_QUESTION_PARAMETER &&
						( predefinedVerbLoginWordItem = adminItem_->predefinedVerbLoginWordItem() ) != NULL )		// Create a login for this user
							{
							if( ( specificationResult = predefinedVerbLoginWordItem->addSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_VERB_SINGULAR, generalizationWordTypeNr, WORD_TYPE_UNDEFINED, specificationCollectionNr, NO_CONTEXT_NR, generalizationContextNr, NO_CONTEXT_NR, nContextRelations, firstJustificationItem, generalizationWordItem, NULL, NULL, this ) ).result != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );
							}
						}
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( ( specificationResult = generalizationWordItem->addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, adminItem_->isUserSentenceMixOfFeminineAndMasculineSpecificationWords(), isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, this ) ).result != RESULT_OK )
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );
					}
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
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

	char *currentUserName()
		{
		return ( currentUserWordItem_ == NULL ? NULL : currentUserWordItem_->anyWordTypeString() );
		}
	};

/*************************************************************************
 *	"We thank you, O God!
 *	We give thanks because you are near.
 *	People everywhere tell your wonderful deeds." (Psalm 75:1)
 *************************************************************************/
