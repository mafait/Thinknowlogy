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

class AdminAuthorization
	{
	// Private constructible variables

	private boolean isDeveloperUser_;
	private boolean isExpertUser_;

	private int firstSentenceNrOfCurrentUser_;

	private WordItem currentUserWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;
	private String authorizationKey_;


	// Constructor / deconstructor

	protected AdminAuthorization( AdminItem adminItem )
		{
		String errorString = null;

		isDeveloperUser_ = false;
		isExpertUser_ = false;

		firstSentenceNrOfCurrentUser_ = Constants.NO_SENTENCE_NR;

		currentUserWordItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();
		// The address of the class is the key, not the content of the string
		authorizationKey_ = moduleNameString_;

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected boolean isDeveloperUser()
		{
		return isDeveloperUser_;
		}

	protected boolean isExpertUser()
		{
		return isExpertUser_;
		}

	protected int firstSentenceNrOfCurrentUser()
		{
		return ( firstSentenceNrOfCurrentUser_ + 1 );
		}

	protected byte authorizeWord( WordItem authorizationWordItem )
		{
		if( authorizationWordItem != null )
			{
			if( adminItem_.isSystemStartingUp() )
				{
				if( authorizationWordItem.assignChangePermissions( authorizationKey_ ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign my authorization permissions to a word" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "You are not authorized to authorize the given word" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given authorization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte login( WordItem specificationWordItem )
		{
		WordResultType wordResult;
		boolean isCorrectPassword = false;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem passwordAssignmentItem;
		WordItem currentGeneralizationWordItem;
		WordItem passwordSpecificationWordItem;
		WordItem predefinedNounPasswordWordItem;
		WordItem predefinedNounUserWordItem;
		WordItem foundUserWordItem = null;
		StringBuffer readUserNameStringBuffer = new StringBuffer();
		StringBuffer readPasswordStringBuffer = new StringBuffer();

		isDeveloperUser_ = false;
		isExpertUser_ = false;

		if( ( predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem ) != null )
			{
			if( ( predefinedNounPasswordWordItem = adminItem_.predefinedNounPasswordWordItem() ) != null )
				{
				// No user name is given
				if( specificationWordItem == null &&
				// Get first user without password
				( currentGeneralizationItem = predefinedNounPasswordWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
							{
							// Select first user in the current language
							if( currentGeneralizationWordItem.activeWordTypeItem( false, Constants.WORD_TYPE_PROPER_NAME ) != null )
								specificationWordItem = currentGeneralizationWordItem;
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );
						}
					while( specificationWordItem == null &&
					( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
					}

				// No user name is given, no user without password is found
				if( specificationWordItem == null )
					{
					// Ask user name
					if( adminItem_.getUserInput( false, false, true, false, predefinedNounUserWordItem.singularNounString(), readUserNameStringBuffer ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to read to user name" );
					}
				else
					readUserNameStringBuffer.append( specificationWordItem.anyWordTypeString() );

				if( readUserNameStringBuffer.length() > 0 &&
				// Find user word
				( currentGeneralizationItem = predefinedNounUserWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
							{
							if( ( wordResult = currentGeneralizationWordItem.findWordType( true, Constants.WORD_TYPE_PROPER_NAME, readUserNameStringBuffer.toString() ) ).result == Constants.RESULT_OK )
								{
								if( wordResult.foundWordTypeItem != null )
									foundUserWordItem = currentGeneralizationWordItem;
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to find the user name" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );
						}
					while( foundUserWordItem == null &&
					( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
					}

				if( foundUserWordItem != null )
					{
					if( foundUserWordItem == currentUserWordItem_ )
						{
						// Already logged in as given user
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START, readUserNameStringBuffer.toString(), Constants.INTERFACE_CONSOLE_LOGIN_END ) == Constants.RESULT_OK )
							isCorrectPassword = true;
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'already logged in' interface notification" );
						}
					else
						{
						if( ( predefinedNounPasswordWordItem = adminItem_.predefinedNounPasswordWordItem() ) != null )
							{
							if( ( passwordAssignmentItem = predefinedNounPasswordWordItem.firstActiveAssignmentItem( false, Constants.NO_QUESTION_PARAMETER, foundUserWordItem ) ) == null )
								{
								// No password assignment found. Now, check explicitly if no password is required
								if( foundUserWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, true, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounPasswordWordItem ) != null )
									// No password required
									isCorrectPassword = true;
								}
							else
								{
								// Ask password
								if( adminItem_.getUserInput( false, true, true, false, predefinedNounPasswordWordItem.singularNounString() + " (" + readUserNameStringBuffer + ")", readPasswordStringBuffer ) == Constants.RESULT_OK )
									{
									if( ( passwordSpecificationWordItem = passwordAssignmentItem.specificationWordItem() ) != null )
										{
										if( passwordSpecificationWordItem.isCorrectHiddenWordType( passwordAssignmentItem.specificationWordTypeNr(), readPasswordStringBuffer.toString(), authorizationKey_ ) )
											isCorrectPassword = true;
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "The password assignment specification item is undefined" );
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to read to password" );
								}

							if( isCorrectPassword )
								{
								if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundUserWordItem, predefinedNounUserWordItem, null ).result == Constants.RESULT_OK )
									{
									firstSentenceNrOfCurrentUser_ = CommonVariables.currentSentenceNr;
									currentUserWordItem_ = foundUserWordItem;
									CommonVariables.currentUserNr = predefinedNounUserWordItem.getUserNr( foundUserWordItem );

									if( adminItem_.predefinedNounDeveloperWordItem() != null &&
									foundUserWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, adminItem_.predefinedNounDeveloperWordItem() ) != null )
										isDeveloperUser_ = true;

									if( adminItem_.predefinedNounExpertWordItem() != null &&
									foundUserWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, adminItem_.predefinedNounExpertWordItem() ) != null )
										isExpertUser_ = true;
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to assign the user" );
								}
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The predefined password noun word item is undefined" );
						}
					}

				if( !isCorrectPassword ||
				foundUserWordItem == null )
					{
					if( !adminItem_.isSystemStartingUp() )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_LOGIN_FAILED ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'login failed' interface notification" );
						}
					else
						return adminItem_.startSystemError( 1, moduleNameString_, "The user name or it's password isn't correct" );
					}
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The predefined password noun word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The predefined user noun word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		WordItem predefinedVerbLoginWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( generalizationWordItem.isNounPassword() ||
				specificationWordItem.isNounDeveloper() ||
				specificationWordItem.isNounExpert() ||
				specificationWordItem.isNounUser() )
					{
					if( generalizationWordItem.isNounPassword() )
						{
						if( specificationWordItem.hideWordType( specificationWordTypeNr, authorizationKey_ ) != Constants.RESULT_OK )
							adminItem_.addError( 1, moduleNameString_, "I failed to hide a password" );
						}
					else
						{
						if( !isNegative &&
						specificationWordItem.isNounUser() &&
						questionParameter == Constants.NO_QUESTION_PARAMETER &&
						// Create a login for this user
						( predefinedVerbLoginWordItem = adminItem_.predefinedVerbLoginWordItem() ) != null )
							{
							if( ( specificationResult = predefinedVerbLoginWordItem.addSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_VERB_SINGULAR, generalizationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, specificationCollectionNr, Constants.NO_CONTEXT_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, nContextRelations, firstJustificationItem, generalizationWordItem, null, null, authorizationKey_ ) ).result != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to add a specification with authorization" );
							}
						}
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( ( specificationResult = generalizationWordItem.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, authorizationKey_ ) ).result != Constants.RESULT_OK )
						adminItem_.addError( 1, moduleNameString_, "I failed to add a specification with authorization" );
					}
				}
			else
				adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem != null )
			{
			if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, authorizationKey_ ) ).result != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to assign a specification with authorization" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected String currentUserName()
		{
		return ( currentUserWordItem_ == null ? null : currentUserWordItem_.anyWordTypeString() );
		}
	};

/*************************************************************************
 *	"We thank you, O God!
 *	We give thanks because you are near.
 *	People everywhere tell your wonderful deeds." (Psalm 75:1)
 *************************************************************************/
