/*	Class:		GlobalVariables
 *	Purpose:	To hold the global variables
 *	Version:	Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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

class GlobalVariables
	{
	protected static boolean hasConfirmedAnySpecification;
	protected static boolean hasDisplayedIntegrityWarning;
	protected static boolean hasDisplayedMessage;
	protected static boolean hasDisplayedWarning;
	protected static boolean hasFoundAnswerToQuestion;
	protected static boolean hasFoundQuery;
	protected static boolean isAssignmentChanged;
	protected static boolean isConflictingQuestion;
	protected static boolean isFirstAnswerToQuestion;
	protected static boolean isQuestionAlreadyAnswered;
	protected static boolean isSystemStartingUp;

	protected static byte result;

	protected static short currentAssignmentLevel;
	protected static short currentLanguageNr;
	protected static short currentUserNr;
	protected static short currentWriteLevel;

	protected static int currentSentenceNr;
	protected static int currentSentenceItemNr;

	protected static int nActiveQueryItems;
	protected static int nInactiveQueryItems;
	protected static int nArchivedQueryItems;
	protected static int nReplacedQueryItems;

	protected static int nDeletedItems;

	protected static int nUserGeneralizationWords;
	protected static int nUserSpecificationWords;
	protected static int nUserRelationWords;

	protected static int removeSentenceNr;
	protected static int removeStartItemNr;

	protected static int nConstructedSentences;
	protected static int nReadSentences;

	protected static int nCreatedJustificationItems;
	protected static int nCreatedSpecificationItems;
	protected static int nCreatedReadItems;
	protected static int nCreatedWriteItems;
	protected static int nTotalCreatedItems;

	protected static int nReplacedJustificationItems;
	protected static int nReplacedSpecificationItems;
	protected static int nTotalReplacedItems;

	protected static int nDeletedJustificationItems;
	protected static int nDeletedSpecificationItems;
	protected static int nDeletedReadItems;
	protected static int nTotalDeletedItems;

	// Used for developer statistics
	protected static int nIdleReasoningCalls;
	protected static int nUsefulReasoningCalls;
	protected static int nTotalReasoningCalls;

	// Private methods
	protected static int nIdleCalls_askQuestions1;
	protected static int nIdleCalls_askQuestions2;
	protected static int nIdleCalls_askQuestionsWithQuestionAsPrimarySpecification;
	protected static int nIdleCalls_askSimpleQuestions;
	protected static int nIdleCalls_askSpanishQuestionAboutGeneralizationWord;
	protected static int nIdleCalls_askSpanishQuestionAboutRelationWord;
	protected static int nIdleCalls_drawNegativeAdjectiveConclusions;
	protected static int nIdleCalls_drawOnlyOptionLeftProperNounConclusion;
	protected static int nIdleCalls_drawProperNounPartOfConclusions;
	protected static int nIdleCalls_drawPossessiveReversibleConclusions;
	protected static int nIdleCalls_drawSelectedReversibleConclusions;
	protected static int nIdleCalls_makeOppositePossessiveSpecificationAssumption;

	protected static int nUsefulCalls_askQuestions1;
	protected static int nUsefulCalls_askQuestions2;
	protected static int nUsefulCalls_askQuestionsWithQuestionAsPrimarySpecification;
	protected static int nUsefulCalls_askSimpleQuestions;
	protected static int nUsefulCalls_askSpanishQuestionAboutGeneralizationWord;
	protected static int nUsefulCalls_askSpanishQuestionAboutRelationWord;
	protected static int nUsefulCalls_drawNegativeAdjectiveConclusions;
	protected static int nUsefulCalls_drawOnlyOptionLeftProperNounConclusion;
	protected static int nUsefulCalls_drawProperNounPartOfConclusions;
	protected static int nUsefulCalls_drawPossessiveReversibleConclusions;
	protected static int nUsefulCalls_drawSelectedReversibleConclusions;
	protected static int nUsefulCalls_makeOppositePossessiveSpecificationAssumption;

	// Protected methods
	protected static int nIdleCalls_askSpecificationSubstitutionQuestionOrDrawNegativeConclusion;
	protected static int nIdleCalls_drawCompoundSpecificationSubstitutionConclusion;
	protected static int nIdleCalls_drawProperNounPartOfConclusionsInProperNounWords;
	protected static int nIdleCalls_drawSimpleNegativeConclusions;
	protected static int nIdleCalls_drawSpecificationGeneralizationConclusion;
	protected static int nIdleCalls_drawSpecificationSubstitutionConclusionOrAskQuestion;
	protected static int nIdleCalls_makeExclusiveSpecificationSubstitutionAssumption;
	protected static int nIdleCalls_makeGeneralizationAssumption;
	protected static int nIdleCalls_makeIndirectlyAnsweredQuestionAssumption;
	protected static int nIdleCalls_drawDefinitionSpecificationSubstitutionConclusion;

	protected static int nUsefulCalls_askSpecificationSubstitutionQuestionOrDrawNegativeConclusion;
	protected static int nUsefulCalls_drawCompoundSpecificationSubstitutionConclusion;
	protected static int nUsefulCalls_drawProperNounPartOfConclusionsInProperNounWords;
	protected static int nUsefulCalls_drawSimpleNegativeConclusions;
	protected static int nUsefulCalls_drawSpecificationGeneralizationConclusion;
	protected static int nUsefulCalls_drawSpecificationSubstitutionConclusionOrAskQuestion;
	protected static int nUsefulCalls_makeExclusiveSpecificationSubstitutionAssumption;
	protected static int nUsefulCalls_makeGeneralizationAssumption;
	protected static int nUsefulCalls_makeIndirectlyAnsweredQuestionAssumption;
	protected static int nUsefulCalls_drawDefinitionSpecificationSubstitutionConclusion;


	protected static SelectionList adminConditionList;
	protected static SelectionList adminActionList;
	protected static SelectionList adminAlternativeList;

	protected static WordItem currentLanguageWordItem;
	protected static WordItem firstAssignmentWordItem;
	protected static WordItem firstCollectionWordItem;
	protected static WordItem firstContextWordItem;
	protected static WordItem firstPossessiveNounWordItem;
	protected static WordItem firstPredefinedWordItem;
	protected static WordItem firstProperNounWordItem;
	protected static WordItem firstSpecificationWordItem;
	protected static WordItem firstTouchedWordItem;
	protected static WordItem firstUserDefinedProperNounWordItem;
	protected static WordItem firstWordItem;
	protected static WordItem lastCollectionWordItem;
	protected static WordItem lastContextWordItem;
	protected static WordItem lastPredefinedWordItem;
	protected static WordItem predefinedNounLanguageWordItem;
	protected static WordItem predefinedNounUserWordItem;

	protected static StringBuffer currentPathStringBuffer;
	protected static StringBuffer interfaceLanguageStringBuffer;
	protected static StringBuffer learnedFromUserStringBuffer;
	protected static StringBuffer queryStringBuffer;
	protected static StringBuffer readUserSentenceStringBuffer;
	protected static StringBuffer writtenSentenceStringBuffer;
	protected static StringBuffer writtenUserSentenceStringBuffer;


	// Protected methods

	protected static void initialize()
		{
		hasConfirmedAnySpecification = false;
		hasDisplayedIntegrityWarning = false;
		hasDisplayedMessage = false;
		hasDisplayedWarning = false;
		hasFoundAnswerToQuestion = false;
		hasFoundQuery = false;
		isAssignmentChanged = false;
		isConflictingQuestion = false;
		isFirstAnswerToQuestion = false;
		isQuestionAlreadyAnswered = false;
		isSystemStartingUp = true;

		result = Constants.RESULT_OK;

		currentAssignmentLevel = Constants.NO_ASSIGNMENT_LEVEL;
		currentLanguageNr = Constants.NO_LANGUAGE_NR;
		currentUserNr = Constants.NO_USER_NR;
		currentWriteLevel = Constants.NO_WRITE_LEVEL;

		currentSentenceNr = 1;	// First sentence
		currentSentenceItemNr = Constants.NO_ITEM_NR;

		nActiveQueryItems = 0;
		nInactiveQueryItems = 0;
		nArchivedQueryItems = 0;
		nReplacedQueryItems = 0;

		nDeletedItems = 0;

		nUserGeneralizationWords = 0;
		nUserSpecificationWords = 0;
		nUserRelationWords = 0;

		removeSentenceNr = Constants.NO_SENTENCE_NR;
		removeStartItemNr = Constants.NO_ITEM_NR;

		// Used for developer statistics
		nConstructedSentences = 0;
		nReadSentences = 0;

		nCreatedJustificationItems = 0;
		nCreatedSpecificationItems = 0;
		nCreatedReadItems = 0;
		nCreatedWriteItems = 0;
		nTotalCreatedItems = 0;

		nReplacedJustificationItems = 0;
		nReplacedSpecificationItems = 0;
		nTotalReplacedItems = 0;

		nDeletedJustificationItems = 0;
		nDeletedSpecificationItems = 0;
		nDeletedReadItems = 0;
		nTotalDeletedItems = 0;

		nIdleReasoningCalls = 0;
		nUsefulReasoningCalls = 0;
		nTotalReasoningCalls = 0;

		// Private methods
		nIdleCalls_askQuestions1 = 0;
		nIdleCalls_askQuestions2 = 0;
		nIdleCalls_askQuestionsWithQuestionAsPrimarySpecification = 0;
		nIdleCalls_askSimpleQuestions = 0;
		nIdleCalls_askSpanishQuestionAboutGeneralizationWord = 0;
		nIdleCalls_askSpanishQuestionAboutRelationWord = 0;
		nIdleCalls_drawNegativeAdjectiveConclusions = 0;
		nIdleCalls_drawOnlyOptionLeftProperNounConclusion = 0;
		nIdleCalls_drawProperNounPartOfConclusions = 0;
		nIdleCalls_drawPossessiveReversibleConclusions = 0;
		nIdleCalls_drawSelectedReversibleConclusions = 0;
		nIdleCalls_makeOppositePossessiveSpecificationAssumption = 0;

		nUsefulCalls_askQuestions1 = 0;
		nUsefulCalls_askQuestions2 = 0;
		nUsefulCalls_askQuestionsWithQuestionAsPrimarySpecification = 0;
		nUsefulCalls_askSimpleQuestions = 0;
		nUsefulCalls_askSpanishQuestionAboutGeneralizationWord = 0;
		nUsefulCalls_askSpanishQuestionAboutRelationWord = 0;
		nUsefulCalls_drawNegativeAdjectiveConclusions = 0;
		nUsefulCalls_drawOnlyOptionLeftProperNounConclusion = 0;
		nUsefulCalls_drawProperNounPartOfConclusions = 0;
		nUsefulCalls_drawPossessiveReversibleConclusions = 0;
		nUsefulCalls_drawSelectedReversibleConclusions = 0;
		nUsefulCalls_makeOppositePossessiveSpecificationAssumption = 0;

		// Protected methods
		nIdleCalls_askSpecificationSubstitutionQuestionOrDrawNegativeConclusion = 0;
		nIdleCalls_drawCompoundSpecificationSubstitutionConclusion = 0;
		nIdleCalls_drawProperNounPartOfConclusionsInProperNounWords = 0;
		nIdleCalls_drawSimpleNegativeConclusions = 0;
		nIdleCalls_drawSpecificationGeneralizationConclusion = 0;
		nIdleCalls_drawSpecificationSubstitutionConclusionOrAskQuestion = 0;
		nIdleCalls_makeExclusiveSpecificationSubstitutionAssumption = 0;
		nIdleCalls_makeGeneralizationAssumption = 0;
		nIdleCalls_makeIndirectlyAnsweredQuestionAssumption = 0;
		nIdleCalls_drawDefinitionSpecificationSubstitutionConclusion = 0;

		nUsefulCalls_askSpecificationSubstitutionQuestionOrDrawNegativeConclusion = 0;
		nUsefulCalls_drawCompoundSpecificationSubstitutionConclusion = 0;
		nUsefulCalls_drawProperNounPartOfConclusionsInProperNounWords = 0;
		nUsefulCalls_drawSimpleNegativeConclusions = 0;
		nUsefulCalls_drawSpecificationGeneralizationConclusion = 0;
		nUsefulCalls_drawSpecificationSubstitutionConclusionOrAskQuestion = 0;
		nUsefulCalls_makeExclusiveSpecificationSubstitutionAssumption = 0;
		nUsefulCalls_makeGeneralizationAssumption = 0;
		nUsefulCalls_makeIndirectlyAnsweredQuestionAssumption = 0;
		nUsefulCalls_drawDefinitionSpecificationSubstitutionConclusion = 0;


		adminConditionList = null;
		adminActionList = null;
		adminAlternativeList = null;

		currentLanguageWordItem = null;
		firstAssignmentWordItem = null;
		firstCollectionWordItem = null;
		firstContextWordItem = null;
		firstPossessiveNounWordItem = null;
		firstPredefinedWordItem = null;
		firstProperNounWordItem = null;
		firstSpecificationWordItem = null;
		firstTouchedWordItem = null;
		firstUserDefinedProperNounWordItem = null;
		firstWordItem = null;
		lastCollectionWordItem = null;
		lastContextWordItem = null;
		lastPredefinedWordItem = null;
		predefinedNounLanguageWordItem = null;
		predefinedNounUserWordItem = null;

		/* Don't initialize 'currentPathStringBuffer' here.
		Otherwise it will destroy the initialization by class Thinknowlogy
		This variable will be set in the main() method of class Thinknowlogy */
//		currentPathStringBuffer = null;

		interfaceLanguageStringBuffer = null;
		learnedFromUserStringBuffer = null;
		queryStringBuffer = null;
		readUserSentenceStringBuffer = null;
		writtenSentenceStringBuffer = null;
		writtenUserSentenceStringBuffer = null;
		}
	}

/*************************************************************************
 *	"Honor the Lord for the glory of his name.
 *	Worship the Lord in the splendor of his holiness." (Psalm 29:2)
 *************************************************************************/
