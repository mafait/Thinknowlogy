/*	Class:			AdminAuthorization
 *	Supports class:	AdminItem
 *	Purpose:		To handle authorization
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"

class AdminAuthorization
	{
	friend class AdminItem;

	// Private constructible variables

	unsigned int firstSentenceNrOfCurrentUser_;

	WordItem *currentUserWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	protected:
	// Constructor / deconstructor

	AdminAuthorization( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		firstSentenceNrOfCurrentUser_ = NO_SENTENCE_NR;

		currentUserWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminAuthorization" );

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

	unsigned int firstSentenceNrOfCurrentUser()
		{
		return ( firstSentenceNrOfCurrentUser_ + 1 );
		}

	ResultType authorizeWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeWord";

		if( authorizationWordItem != NULL )
			{
			if( adminItem_->isSystemStartingUp() )
				{
				if( authorizationWordItem->assignChangePermissions( this ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign my authorization permissions to a word" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "You are not authorized to authorize the given word" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given authorization word item is undefined" );

		return RESULT_OK;
		}

	ResultType login( WordItem *specificationWordItem )
		{
		WordResultType wordResult;
		bool isCorrectPassword = false;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *passwordAssignmentItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *passwordSpecificationWordItem;
		WordItem *predefinedNounPasswordWordItem;
		WordItem *predefinedNounUserWordItem;
		WordItem *foundUserWordItem = NULL;
		char promptString[MAX_SENTENCE_STRING_LENGTH];
		char readUserNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char readPasswordString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "login";

		if( ( predefinedNounUserWordItem = commonVariables_->predefinedNounUserWordItem ) != NULL )
			{
			if( ( predefinedNounPasswordWordItem = adminItem_->predefinedNounPasswordWordItem() ) != NULL )
				{
				// No user name is given
				if( specificationWordItem == NULL &&
				// Get first user without password
				( currentGeneralizationItem = predefinedNounPasswordWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
							{
							// Select first user in the current language
							if( currentGeneralizationWordItem->activeWordTypeItem( false, WORD_TYPE_PROPER_NAME ) != NULL )
								specificationWordItem = currentGeneralizationWordItem;
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
						}
					while( specificationWordItem == NULL &&
					( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
					}

				// No user name is given, no user without password is found
				if( specificationWordItem == NULL )
					{
					// Ask user name
					if( adminItem_->getUserInput( false, true, false, predefinedNounUserWordItem->singularNounString(), readUserNameString ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read to user name" );
					}
				else
					strcat( readUserNameString, specificationWordItem->anyWordTypeString() );

				if( strlen( readUserNameString ) > 0 &&
				// Find user word
				( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
							{
							if( ( wordResult = currentGeneralizationWordItem->findWordType( true, WORD_TYPE_PROPER_NAME, readUserNameString ) ).result == RESULT_OK )
								{
								if( wordResult.foundWordTypeItem != NULL )
									foundUserWordItem = currentGeneralizationWordItem;
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the user name" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
						}
					while( foundUserWordItem == NULL &&
					( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
					}

				if( foundUserWordItem != NULL )
					{
					if( foundUserWordItem == currentUserWordItem_ )
						{
						// Already logged in as given user
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START, readUserNameString, INTERFACE_CONSOLE_LOGIN_END ) == RESULT_OK )
							isCorrectPassword = true;
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'already logged in' interface notification" );
						}
					else
						{
						if( ( predefinedNounPasswordWordItem = adminItem_->predefinedNounPasswordWordItem() ) != NULL )
							{
							if( ( passwordAssignmentItem = predefinedNounPasswordWordItem->firstActiveAssignmentItem( false, NO_QUESTION_PARAMETER, foundUserWordItem ) ) == NULL )
								{
								// No password assignment found. Now, check explicitly if no password is required
								if( foundUserWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, true, true, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, predefinedNounPasswordWordItem ) != NULL )
									// No password required
									isCorrectPassword = true;
								}
							else
								{
								sprintf( promptString, "%s (%s)", predefinedNounPasswordWordItem->singularNounString(), readUserNameString );

								// Ask password
								if( adminItem_->getUserInput( true, true, false, promptString, readPasswordString ) == RESULT_OK )
									{
									if( ( passwordSpecificationWordItem = passwordAssignmentItem->specificationWordItem() ) != NULL )
										{
										if( passwordSpecificationWordItem->isCorrectHiddenWordType( passwordAssignmentItem->specificationWordTypeNr(), readPasswordString, this ) )
											isCorrectPassword = true;
										}
									else
										return adminItem_->startError( functionNameString, moduleNameString_, "The password assignment specification item is undefined" );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read to password" );
								}

							if( isCorrectPassword )
								{
								if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, foundUserWordItem, predefinedNounUserWordItem, NULL ).result == RESULT_OK )
									{
									firstSentenceNrOfCurrentUser_ = commonVariables_->currentSentenceNr;
									currentUserWordItem_ = foundUserWordItem;
									commonVariables_->currentUserNr = predefinedNounUserWordItem->getUserNr( foundUserWordItem );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the user" );
								}
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The predefined password noun word item is undefined" );
						}
					}

				if( !isCorrectPassword ||
				foundUserWordItem == NULL )
					{
					if( !adminItem_->isSystemStartingUp() )
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_LOGIN_FAILED ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'login failed' interface notification" );
						}
					else
						return adminItem_->startSystemError( functionNameString, moduleNameString_, "The user name or it's password isn't correct" );
					}
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The predefined password noun word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The predefined user noun word item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		WordItem *predefinedVerbLoginWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( generalizationWordItem->isNounPassword() ||
				specificationWordItem->isNounUser() )
					{
					if( generalizationWordItem->isNounPassword() )
						{
						if( specificationWordItem->hideWordType( specificationWordTypeNr, this ) != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to hide a password" );
						}
					else
						{
						if( !isNegative &&
						specificationWordItem->isNounUser() &&
						questionParameter == NO_QUESTION_PARAMETER &&
						// Create a login for this user
						( predefinedVerbLoginWordItem = adminItem_->predefinedVerbLoginWordItem() ) != NULL )
							{
							if( ( specificationResult = predefinedVerbLoginWordItem->addSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_VERB_SINGULAR, generalizationWordTypeNr, WORD_TYPE_UNDEFINED, specificationCollectionNr, NO_CONTEXT_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, nContextRelations, firstJustificationItem, generalizationWordItem, NULL, NULL, this ) ).result != RESULT_OK )
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );
							}
						}
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( ( specificationResult = generalizationWordItem->addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, this ) ).result != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );
					}
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
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
