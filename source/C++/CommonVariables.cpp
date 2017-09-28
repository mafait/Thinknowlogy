/*	Class:			CommonVariables
 *	Purpose:		To hold the common variables
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#include <string.h>
#include "Constants.h"

// Class declarations
class ScoreList;
class SelectionList;
class WordItem;

class CommonVariables
	{
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class AdminReasoningNew;
	friend class AdminReasoningOld;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class CollectionItem;
	friend class CollectionList;
	friend class ContextItem;
	friend class ContextList;
	friend class FileItem;
	friend class FileList;
	friend class GeneralizationItem;
	friend class GeneralizationList;
	friend class GrammarItem;
	friend class GrammarList;
	friend class InputOutput;
	friend class InterfaceItem;
	friend class InterfaceList;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class MultipleWordItem;
	friend class MultipleWordList;
	friend class ReadItem;
	friend class ReadList;
	friend class ScoreItem;
	friend class ScoreList;
	friend class SelectionItem;
	friend class SelectionList;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordList;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordTypeItem;
	friend class WordTypeList;
	friend class WordWrite;
	friend class WriteItem;
	friend class WriteList;

	// Protected common variables

	bool hasDisplayedArticleNotification;
	bool hasDisplayedIntegrityWarning;
	bool hasDisplayedMessage;
	bool hasDisplayedWarning;
	bool hasFoundAnswerToQuestion;
	bool hasFoundQuery;
	bool isAssignmentChanged;
	bool isFirstAnswerToQuestion;
	bool isQuestionAlreadyAnswered;

	signed char result;

	unsigned short currentAssignmentLevel;
	unsigned short currentLanguageNr;
	unsigned short currentUserNr;
	unsigned short currentWriteLevel;

	unsigned int currentSentenceNr;
	unsigned int currentSentenceItemNr;

	unsigned int nDeletedItems;
	unsigned int nActiveQueryItems;
	unsigned int nInactiveQueryItems;
	unsigned int nArchivedQueryItems;
	unsigned int nReplacedQueryItems;
	unsigned int nDeletedQueryItems;

	unsigned int nUserGeneralizationWords;
	unsigned int nUserSpecificationWords;
	unsigned int nUserRelationWords;

	unsigned int removeSentenceNr;
	unsigned int removeStartItemNr;

	SelectionList *adminConditionList;
	SelectionList *adminActionList;
	SelectionList *adminAlternativeList;

	WordItem *currentLanguageWordItem;
	WordItem *firstAssignmentWordItem;
	WordItem *firstCollectionWordItem;
	WordItem *firstContextWordItem;
	WordItem *firstPossessiveNounWordItem;
	WordItem *firstPredefinedWordItem;
	WordItem *firstSpecificationWordItem;
	WordItem *firstTouchedProperNameWordItem;
	WordItem *firstUserProperNameWordItem;
	WordItem *firstWordItem;
	WordItem *lastCollectionWordItem;
	WordItem *lastContextWordItem;
	WordItem *lastPredefinedWordItem;
	WordItem *predefinedNounLanguageWordItem;
	WordItem *predefinedNounUserWordItem;

	char learnedFromUserString[MAX_SENTENCE_STRING_LENGTH];
	char queryString[MAX_SENTENCE_STRING_LENGTH];
	char writtenSentenceString[MAX_SENTENCE_STRING_LENGTH];
	char writtenUserSentenceString[MAX_SENTENCE_STRING_LENGTH];


	protected:
	// Constructor

	CommonVariables()
		{
		// Protected common variables

		hasDisplayedArticleNotification = false;
		hasDisplayedIntegrityWarning = false;
		hasDisplayedMessage = false;
		hasDisplayedWarning = false;
		hasFoundAnswerToQuestion = false;
		hasFoundQuery = false;
		isAssignmentChanged = false;
		isFirstAnswerToQuestion = false;
		isQuestionAlreadyAnswered = false;

		result = RESULT_OK;

		currentAssignmentLevel = NO_ASSIGNMENT_LEVEL;
		currentLanguageNr = NO_LANGUAGE_NR;
		currentUserNr = NO_USER_NR;
		currentWriteLevel = NO_WRITE_LEVEL;

		currentSentenceNr = 1;	// First sentence
		currentSentenceItemNr = NO_ITEM_NR;

		nDeletedItems = 0;
		nActiveQueryItems = 0;
		nInactiveQueryItems = 0;
		nArchivedQueryItems = 0;
		nReplacedQueryItems = 0;
		nDeletedQueryItems = 0;

		nUserGeneralizationWords = 0;
		nUserSpecificationWords = 0;
		nUserRelationWords = 0;

		removeSentenceNr = NO_SENTENCE_NR;
		removeStartItemNr = NO_ITEM_NR;

		adminConditionList = NULL;
		adminActionList = NULL;
		adminAlternativeList = NULL;

		currentLanguageWordItem = NULL;
		firstAssignmentWordItem = NULL;
		firstCollectionWordItem = NULL;
		firstContextWordItem = NULL;
		firstPossessiveNounWordItem = NULL;
		firstPredefinedWordItem = NULL;
		firstSpecificationWordItem = NULL;
		firstTouchedProperNameWordItem = NULL;
		firstUserProperNameWordItem = NULL;
		firstWordItem = NULL;
		lastCollectionWordItem = NULL;
		lastContextWordItem = NULL;
		lastPredefinedWordItem = NULL;
		predefinedNounLanguageWordItem = NULL;
		predefinedNounUserWordItem = NULL;

		strcpy( learnedFromUserString, EMPTY_STRING );
		strcpy( queryString, EMPTY_STRING );
		strcpy( writtenSentenceString, EMPTY_STRING );
		strcpy( writtenUserSentenceString, EMPTY_STRING );
		}
	};

/*************************************************************************
 *	"Honor the Lord for the glory of his name.
 *	Worship the Lord in the splendor of his holiness." (Psalm 29:2)
 *************************************************************************/
