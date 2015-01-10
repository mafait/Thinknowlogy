/*
 *	Class:			AdminAuthorization
 *	Supports class:	AdminItem
 *	Purpose:		To handle authorization
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

class AdminAuthorization
	{
	// Private constructible variables

	private boolean hasUserPasswordAssigned_;
	private boolean hasUserEnteredCorrectPassword_;
	private boolean isExpertUser_;

	private int loginSentenceNr_;

	private SpecificationItem passwordAssignmentItem_;

	private WordItem currentUserWordItem_;
	private WordItem foundUserWordItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;
	private String authorizationKey_;


	// Private methods

	private byte findUserWord( String userNameString )
		{
		WordResultType wordResult;
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		foundUserWordItem_ = null;

		if( userNameString != null )
			{
			if( CommonVariables.predefinedNounUserWordItem != null )
				{
				if( ( currentGeneralizationItem = CommonVariables.predefinedNounUserWordItem.firstSpecificationGeneralizationItem() ) != null )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
							{
							if( ( wordResult = currentGeneralizationWordItem.findWordType( false, Constants.WORD_TYPE_PROPER_NAME, userNameString ) ).result == Constants.RESULT_OK )
								{
								if( wordResult.foundWordTypeItem != null )
									foundUserWordItem_ = currentGeneralizationWordItem;
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find the user name" );
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
					}
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The predefined user noun word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user name string is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkUserForPasswordAssignment( WordItem userWordItem )
		{
		SpecificationResultType specificationResult;
		WordItem predefinedNounPasswordWordItem;

		hasUserPasswordAssigned_ = true;
		passwordAssignmentItem_ = null;

		if( userWordItem != null )
			{
			if( ( predefinedNounPasswordWordItem = adminItem_.predefinedNounPasswordWordItem() ) != null )
				{
				if( ( specificationResult = predefinedNounPasswordWordItem.findAssignmentByRelationWord( false, false, false, false, Constants.NO_QUESTION_PARAMETER, userWordItem ) ).result == Constants.RESULT_OK )
					{
					if( ( passwordAssignmentItem_ = specificationResult.foundSpecificationItem ) == null )
						{
						// No assignment found. So, check for explict negative password specification
						if( userWordItem.firstNonQuestionAssignmentOrSpecificationItem( false, false, false, false, true, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounPasswordWordItem ) != null )
							hasUserPasswordAssigned_ = false;
						}
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find the password assignment" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The predefined password noun word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkPassword( String enteredPasswordString )
		{
		hasUserEnteredCorrectPassword_ = false;		// Fail safe

		if( enteredPasswordString != null )
			{
			if( passwordAssignmentItem_ != null )
				{
				if( passwordAssignmentItem_.specificationWordItem() != null )
					{

					if( passwordAssignmentItem_.specificationWordItem().isCorrectHiddenWordType( passwordAssignmentItem_.specificationWordTypeNr(), enteredPasswordString, authorizationKey_ ) )
						hasUserEnteredCorrectPassword_ = true;
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The password assignment specification item is undefined" );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given password string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminAuthorization( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		hasUserPasswordAssigned_ = true;
		hasUserEnteredCorrectPassword_ = false;
		isExpertUser_ = false;

		loginSentenceNr_ = Constants.NO_SENTENCE_NR;

		passwordAssignmentItem_ = null;
		currentUserWordItem_ = null;
		foundUserWordItem_ = null;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();
		authorizationKey_ = moduleNameString_;		// The address of the class is the key - not the content of the string

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

	protected boolean isExpertUser()
		{
		return isExpertUser_;
		}

	protected int firstUserSentenceNr()
		{
		return ( loginSentenceNr_ + 1 );
		}

	protected byte authorizeWord( WordItem authorizationWordItem )
		{
		if( authorizationWordItem != null )
			{
			if( adminItem_.isSystemStartingUp() )
				{
				if( authorizationWordItem.assignChangePermissions( authorizationKey_ ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign my authorization permissions to a word" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "You are not authorized to authorize the given word" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given authorization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte login( WordItem specificationWordItem )
		{
		WordItem predefinedNounPasswordWordItem;
		WordItem predefinedNounUserWordItem;
		StringBuffer readUserNameStringBuffer = new StringBuffer();
		StringBuffer readPasswordStringBuffer = new StringBuffer();

		hasUserPasswordAssigned_ = true;
		hasUserEnteredCorrectPassword_ = false;

		passwordAssignmentItem_ = null;
		foundUserWordItem_ = null;

		if( ( predefinedNounUserWordItem = CommonVariables.predefinedNounUserWordItem ) != null )
			{
			if( ( predefinedNounPasswordWordItem = adminItem_.predefinedNounPasswordWordItem() ) != null )
				{
				if( specificationWordItem == null )		// No user name is given
					{
					if( adminItem_.getUserInput( false, false, true, false, predefinedNounUserWordItem.singularNounString(), readUserNameStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read to user name" );
					}
				else
					readUserNameStringBuffer.append( specificationWordItem.anyWordTypeString() );

				if( readUserNameStringBuffer.length() > 0 )
					{
					if( findUserWord( readUserNameStringBuffer.toString() ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find the user" );
					}

				if( foundUserWordItem_ != null &&
				foundUserWordItem_ == currentUserWordItem_ )
					{
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START, readUserNameStringBuffer.toString(), Constants.INTERFACE_CONSOLE_LOGIN_END ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
					}
				else
					{
					if( foundUserWordItem_ != null )
						{
						if( checkUserForPasswordAssignment( foundUserWordItem_ ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check the user for password assignment" );
						}

					if( foundUserWordItem_ != null &&
					!hasUserPasswordAssigned_ )
						hasUserEnteredCorrectPassword_ = true;		// User has no password
					else
						{
						if( adminItem_.getUserInput( false, true, true, false, predefinedNounPasswordWordItem.singularNounString(), readPasswordStringBuffer ) == Constants.RESULT_OK )
							{
							if( foundUserWordItem_ != null )	// Only check password if user was found
								{
								if( checkPassword( readPasswordStringBuffer.toString() ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check the user and password" );
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read to password" );
						}

					if( hasUserEnteredCorrectPassword_ &&
					foundUserWordItem_ != null )
						{
						isExpertUser_ = false;

						if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundUserWordItem_, predefinedNounUserWordItem, null ).result == Constants.RESULT_OK )
							{
							currentUserWordItem_ = foundUserWordItem_;
							loginSentenceNr_ = CommonVariables.currentSentenceNr;
							CommonVariables.firstUserSentenceNr = loginSentenceNr_;
							CommonVariables.currentUserNr = predefinedNounUserWordItem.getUserNr( foundUserWordItem_ );
							if( adminItem_.predefinedNounExpertWordItem() != null &&
							foundUserWordItem_.firstNonQuestionAssignmentOrSpecificationItem( false, false, false, false, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, adminItem_.predefinedNounExpertWordItem() ) != null )
								isExpertUser_ = true;
/*
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_LOGIN_WELCOME, readUserNameStringBuffer.toString(), Constants.INTERFACE_CONSOLE_LOGIN_END ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
*/							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the user" );
						}
					else
						{
						if( !adminItem_.isSystemStartingUp() )
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_LOGIN_FAILED ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
							}
						else
							return myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "The user name or it's password isn't correct" );
						}
					}
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The predefined password noun word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The predefined user noun word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isUserSpecificationExclusivelyFeminineOrMasculine, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		WordItem predefinedVerbLoginWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( generalizationWordItem.isNounPassword() ||
				specificationWordItem.isNounExpert() ||
				specificationWordItem.isNounUser() ||
				generalizationWordItem.isVerbImperativeLogin() )
					{
					if( generalizationWordItem.isNounPassword() )
						{
						if( specificationWordItem.hideWordType( specificationWordTypeNr, authorizationKey_ ) != Constants.RESULT_OK )
							myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to hide a password" );
						}
					else
						{
						if( !isNegative &&
						specificationWordItem.isNounUser() &&
						questionParameter == Constants.NO_QUESTION_PARAMETER &&
						( predefinedVerbLoginWordItem = adminItem_.predefinedVerbLoginWordItem() ) != null )		// Create a login for this user
							{
							if( ( specificationResult = predefinedVerbLoginWordItem.addSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_VERB_SINGULAR, generalizationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, specificationCollectionNr, Constants.NO_CONTEXT_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, firstJustificationItem, generalizationWordItem, null, null, authorizationKey_ ) ).result != Constants.RESULT_OK )
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification with authorization" );
							}
						}
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( ( specificationResult = generalizationWordItem.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, authorizationKey_ ) ).result != Constants.RESULT_OK )
						myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification with authorization" );
					}
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
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
			if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, authorizationKey_ ) ).result != Constants.RESULT_OK )
				myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign a specification with authorization" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected String currentUserName()
		{
		return ( currentUserWordItem_ == null ? null : currentUserWordItem_.anyWordTypeString() );
		}
	};

/*************************************************************************
 *
 *	"We thank you, O God!
 *	We give thanks because you are near.
 *	People everywhere tell your wonderful deeds." (Psalm 75:1)
 *
 *************************************************************************/
