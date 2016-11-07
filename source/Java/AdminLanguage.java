/*	Class:			AdminLanguage
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign the languages
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

class AdminLanguage
	{
	// Private constructed variables

	private WordItem foundLanguageWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;
	private String languageKey_;


	// Private methods

	private byte findLanguageByName( String languageNameString )
		{
		WordResultType wordResult;
		foundLanguageWordItem_ = null;

		if( languageNameString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given language name string is undefined" );

		// Initially the language name words are not linked to
		// the language defintion word. So, search in all words
		if( ( wordResult = adminItem_.findWordTypeInAllWords( true, Constants.WORD_TYPE_PROPER_NAME, languageNameString.toString(), null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find a language word" );

		foundLanguageWordItem_ = wordResult.foundWordItem;
		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminLanguage( AdminItem adminItem )
		{
		String errorString = null;

		foundLanguageWordItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();
		// The address of the class is the key - not the content of the string
		languageKey_ = moduleNameString_;

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

	protected byte authorizeLanguageWord( WordItem authorizationWordItem )
		{
		if( authorizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given authorization word item is undefined" );

		if( authorizationWordItem.assignChangePermissions( languageKey_ ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign my language change permissions to a word" );

		return Constants.RESULT_OK;
		}

	protected byte createLanguage( String languageNameString )
		{
		WordResultType wordResult;
		WordItem languageWordItem;
		WordItem predefinedNounLanguageWordItem;

		if( findLanguageByName( languageNameString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find the language" );

		if( foundLanguageWordItem_ == null )
			{
			CommonVariables.currentLanguageNr++;

			if( ( wordResult = adminItem_.addWord( true, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, languageNameString.length(), languageNameString ) ).result != Constants.RESULT_OK )
				{
				// Restore old language
				CommonVariables.currentLanguageNr--;
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a language word" );
				}

			if( ( languageWordItem = wordResult.createdWordItem ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The last created word item is undefined" );

			if( adminItem_.authorizeLanguageWord( languageWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the created language word" );

			CommonVariables.currentLanguageWordItem = languageWordItem;

			if( adminItem_.startupLanguageNameString == null )
				adminItem_.startupLanguageNameString = languageWordItem.anyWordTypeString();

			if( ( predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem ) != null )
				{
				if( createLanguageSpecification( predefinedNounLanguageWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to create a language specification" );
				}
			}
		else
			CommonVariables.currentLanguageWordItem = foundLanguageWordItem_;

		return Constants.RESULT_OK;
		}

	protected byte createLanguageSpecification( WordItem languageNounWordItem )
		{
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

		if( languageNounWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given language noun word item is undefined" );

		if( currentLanguageWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );

		if( !currentLanguageWordItem.isLanguageWord() )
			return adminItem_.startError( 1, moduleNameString_, "The current language word isn't a language word" );

		if( adminItem_.addLanguageSpecification( currentLanguageWordItem, languageNounWordItem ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a new language specification" );

		return Constants.RESULT_OK;
		}

	protected byte assignLanguage( short newLanguageNr )
		{
		WordItem languageWordItem;

		if( ( languageWordItem = adminItem_.languageWordItem( newLanguageNr ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the requested language" );

		CommonVariables.currentLanguageNr = newLanguageNr;
		CommonVariables.currentLanguageWordItem = languageWordItem;

		if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, CommonVariables.currentLanguageWordItem, CommonVariables.predefinedNounLanguageWordItem, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language with authorization" );

		return Constants.RESULT_OK;
		}

	protected byte assignLanguage( String languageNameString )
		{
		boolean hasFoundLanguage = false;
		SpecificationItem languageSpecificationItem;
		WordItem predefinedNounLanguageWordItem;

		if( findLanguageByName( languageNameString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find the language" );

		if( foundLanguageWordItem_ != null )
			{
			hasFoundLanguage = true;
			predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem;

			if( ( languageSpecificationItem = foundLanguageWordItem_.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounLanguageWordItem ) ) != null )
				CommonVariables.currentLanguageNr = languageSpecificationItem.languageNr();

			if( CommonVariables.currentLanguageWordItem != foundLanguageWordItem_ )
				{
				if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundLanguageWordItem_, predefinedNounLanguageWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );

				CommonVariables.currentLanguageWordItem = foundLanguageWordItem_;
				}

			if( !hasFoundLanguage )
				return adminItem_.startError( 1, moduleNameString_, "The given name isn't a grammar nor an language" );
			}
		else
			{
			if( !adminItem_.isSystemStartingUp() )
				return adminItem_.startError( 1, moduleNameString_, "I couldn't find the requested language" );
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( specificationResult = generalizationWordItem.addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, languageKey_ ) ).result != Constants.RESULT_OK )
			return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a specification with authorization" );

		return specificationResult;
		}

	protected SpecificationResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, languageKey_ ) ).result != Constants.RESULT_OK )
			return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to assign a specification with authorization" );

		return specificationResult;
		}
	};

/*************************************************************************
 *	"The Sovereign Lord has given me his words of wisdom,
 *	so that I know how to comfort the weary.
 *	Morning by morning he wakens me
 *	and opens my understanding to his will." (Isaiah 50:4)
 *************************************************************************/
