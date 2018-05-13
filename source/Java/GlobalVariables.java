/*	Class:		GlobalVariables
 *	Purpose:	To hold the global variables
 *	Version:	Thinknowlogy 2018r1 (ShangDi 上帝)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

class GlobalVariables
	{
	protected static boolean hasDisplayedIntegrityWarning;
	protected static boolean hasDisplayedMessage;
	protected static boolean hasDisplayedWarning;
	protected static boolean hasFoundAnswerToQuestion;
	protected static boolean hasFoundQuery;
	protected static boolean isAssignmentChanged;
	protected static boolean isFirstAnswerToQuestion;
	protected static boolean isQuestionAlreadyAnswered;

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
	protected static int nDeletedQueryItems;
	protected static int nDeletedItems;

	protected static int nUserGeneralizationWords;
	protected static int nUserSpecificationWords;
	protected static int nUserRelationWords;

	protected static int removeSentenceNr;
	protected static int removeStartItemNr;

	protected static long cleaningTime;
	protected static long openFileTime;
	protected static long selectionExecutionTime;
	protected static long writingTime;

	protected static SelectionList adminConditionList;
	protected static SelectionList adminActionList;
	protected static SelectionList adminAlternativeList;

	protected static WordItem currentLanguageWordItem;
	protected static WordItem firstAssignmentWordItem;
	protected static WordItem firstCollectionWordItem;
	protected static WordItem firstContextWordItem;
	protected static WordItem firstPossessiveNounWordItem;
	protected static WordItem firstPredefinedWordItem;
	protected static WordItem firstSpecificationWordItem;
	protected static WordItem firstTouchedWordItem;
	protected static WordItem firstUserProperNounWordItem;
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
		hasDisplayedIntegrityWarning = false;
		hasDisplayedMessage = false;
		hasDisplayedWarning = false;
		hasFoundAnswerToQuestion = false;
		hasFoundQuery = false;
		isAssignmentChanged = false;
		isFirstAnswerToQuestion = false;
		isQuestionAlreadyAnswered = false;

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
		nDeletedQueryItems = 0;
		nDeletedItems = 0;

		nUserGeneralizationWords = 0;
		nUserSpecificationWords = 0;
		nUserRelationWords = 0;

		removeSentenceNr = Constants.NO_SENTENCE_NR;
		removeStartItemNr = Constants.NO_ITEM_NR;

		cleaningTime = 0;
		openFileTime = 0;
		selectionExecutionTime = 0;
		writingTime = 0;

		adminConditionList = null;
		adminActionList = null;
		adminAlternativeList = null;

		currentLanguageWordItem = null;
		firstAssignmentWordItem = null;
		firstCollectionWordItem = null;
		firstContextWordItem = null;
		firstPossessiveNounWordItem = null;
		firstPredefinedWordItem = null;
		firstSpecificationWordItem = null;
		firstTouchedWordItem = null;
		firstUserProperNounWordItem = null;
		firstWordItem = null;
		lastCollectionWordItem = null;
		lastContextWordItem = null;
		lastPredefinedWordItem = null;
		predefinedNounLanguageWordItem = null;
		predefinedNounUserWordItem = null;

		// Don't initialize 'currentPathStringBuffer' there. 
		// It will be set in the main() method of the Thinknowlogy class.
		// Otherwise it will destroy the initialization by Thinknowlogy
//		currentPathStringBuffer = null;
		interfaceLanguageStringBuffer = null;
		learnedFromUserStringBuffer = null;
		queryStringBuffer = null;
		readUserSentenceStringBuffer = null;
		writtenSentenceStringBuffer = null;
		writtenUserSentenceStringBuffer = null;
		}
	};

/*************************************************************************
 *	"Honor the Lord for the glory of his name.
 *	Worship the Lord in the splendor of his holiness." (Psalm 29:2)
 *************************************************************************/
