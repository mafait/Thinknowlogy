/*
 *	Class:		CommonVariables
 *	Purpose:	To hold the common variables
 *	Version:	Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class CommonVariables
	{
	protected static boolean hasFoundAnswerToQuestion;
	protected static boolean hasFoundQuery;
	protected static boolean hasRecalculatedRelationWord;
	protected static boolean hasShownMessage;
	protected static boolean hasShownWarning;

	protected static boolean isAssignmentChanged;
	protected static boolean isDontIncrementCurrentSentenceNr;
	protected static boolean isFirstAnswerToQuestion;
	protected static boolean isQuestionAlreadyAnswered;
	protected static boolean isUserQuestion;

	protected static byte result;

	protected static short currentAssignmentLevel;
	protected static short currentLanguageNr;
	protected static short currentUserNr;
	protected static short currentWriteLevel;

	protected static short matchingWordTypeNr;

	protected static int nDeletedItems;
	protected static int nActiveQueryItems;
	protected static int nInactiveQueryItems;
	protected static int nArchivedQueryItems;
	protected static int nReplacedQueryItems;
	protected static int nDeletedQueryItems;

	protected static int nUserGeneralizationWords;
	protected static int nUserSpecificationWords;
	protected static int nUserRelationWords;

	protected static int currentSentenceNr;
	protected static int highestInUseSentenceNr;
	protected static int removeSentenceNr;

	protected static int currentItemNr;
	protected static int removeStartItemNr;

	protected static ScoreList adminScoreList;

	protected static SelectionList adminConditionList;
	protected static SelectionList adminActionList;
	protected static SelectionList adminAlternativeList;

	protected static WordItem currentLanguageWordItem;
	protected static WordItem firstWordItem;
	protected static WordItem lastPredefinedWordItem;
	protected static WordItem predefinedNounLanguageWordItem;
	protected static WordItem predefinedNounUserWordItem;

	protected static StringBuffer interfaceLanguageStringBuffer;
	protected static StringBuffer queryStringBuffer;
	protected static StringBuffer writeSentenceStringBuffer;

	protected static StringBuffer currentPathStringBuffer;


	// Protected methods

	protected static void initialize()
		{
		hasFoundAnswerToQuestion = false;
		hasFoundQuery = false;
		hasRecalculatedRelationWord = false;
		hasShownMessage = false;
		hasShownWarning = false;

		isAssignmentChanged = false;
		isDontIncrementCurrentSentenceNr = false;
		isFirstAnswerToQuestion = false;
		isQuestionAlreadyAnswered = false;
		isUserQuestion = false;

		result = Constants.RESULT_OK;

		matchingWordTypeNr = Constants.WORD_TYPE_UNDEFINED;

		currentAssignmentLevel = Constants.NO_ASSIGNMENT_LEVEL;
		currentLanguageNr = Constants.NO_LANGUAGE_NR;
		currentUserNr = Constants.NO_USER_NR;
		currentWriteLevel = Constants.NO_WRITE_LEVEL;

		nDeletedItems = 0;
		nActiveQueryItems = 0;
		nInactiveQueryItems = 0;
		nArchivedQueryItems = 0;
		nReplacedQueryItems = 0;
		nDeletedQueryItems = 0;

		nUserGeneralizationWords = 0;
		nUserSpecificationWords = 0;
		nUserRelationWords = 0;

		// First sentence
		currentSentenceNr = 1;
		highestInUseSentenceNr = Constants.NO_SENTENCE_NR;
		removeSentenceNr = Constants.NO_SENTENCE_NR;

		currentItemNr = Constants.NO_ITEM_NR;
		removeStartItemNr = Constants.NO_ITEM_NR;

		adminScoreList = null;

		adminConditionList = null;
		adminActionList = null;
		adminAlternativeList = null;

		currentLanguageWordItem = null;
		firstWordItem = null;
		lastPredefinedWordItem = null;
		predefinedNounLanguageWordItem = null;
		predefinedNounUserWordItem = null;

		interfaceLanguageStringBuffer = null;
		queryStringBuffer = null;
		writeSentenceStringBuffer = null;

		// Don't initialize 'currentPathStringBuffer' there. 
		// It will be set in the main() method of the Thinknowlogy class.
		// Otherwise it will destroy the initialization by Thinknowlogy
//		currentPathStringBuffer = null;
		}
	};

/*************************************************************************
 *	"Honor the Lord for the glory of his name.
 *	Worship the Lord in the splendor of his holiness." (Psalm 29:2)
 *************************************************************************/
