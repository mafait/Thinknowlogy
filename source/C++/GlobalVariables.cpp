/*	Class:			GlobalVariables
 *	Purpose:		To hold the global variables
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

#include <string.h>
#include "Constants.h"

// Class declarations
class SelectionList;
class WordItem;

class GlobalVariables
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

	// Protected global variables

	bool hasDisplayedIntegrityWarning = false;
	bool hasDisplayedMessage = false;
	bool hasDisplayedWarning = false;
	bool hasFoundAnswerToQuestion = false;
	bool hasFoundQuery = false;
	bool isAssignmentChanged = false;
	bool isConflictingQuestion = false;
	bool isFirstAnswerToQuestion = false;
	bool isQuestionAlreadyAnswered = false;

	signed char result = RESULT_OK;

	unsigned short currentAssignmentLevel = NO_ASSIGNMENT_LEVEL;
	unsigned short currentLanguageNr = NO_LANGUAGE_NR;
	unsigned short currentUserNr = NO_USER_NR;
	unsigned short currentWriteLevel = NO_WRITE_LEVEL;

	unsigned int currentSentenceNr = 1;		// First sentence
	unsigned int currentSentenceItemNr = NO_ITEM_NR;

	unsigned int nActiveQueryItems = 0;
	unsigned int nInactiveQueryItems = 0;
	unsigned int nArchivedQueryItems = 0;
	unsigned int nReplacedQueryItems = 0;

	unsigned int nDeletedItems = 0;

	unsigned int nUserGeneralizationWords = 0;
	unsigned int nUserSpecificationWords = 0;
	unsigned int nUserRelationWords = 0;

	unsigned int removeSentenceNr = NO_SENTENCE_NR;
	unsigned int removeStartItemNr = NO_ITEM_NR;

	SelectionList *adminConditionList = NULL;
	SelectionList *adminActionList = NULL;
	SelectionList *adminAlternativeList = NULL;

	WordItem *currentLanguageWordItem = NULL;
	WordItem *firstAssignmentWordItem = NULL;
	WordItem *firstCollectionWordItem = NULL;
	WordItem *firstContextWordItem = NULL;
	WordItem *firstPossessiveNounWordItem = NULL;
	WordItem *firstPredefinedWordItem = NULL;
	WordItem *firstSpecificationWordItem = NULL;
	WordItem *firstTouchedWordItem = NULL;
	WordItem *firstUserProperNounWordItem = NULL;
	WordItem *firstWordItem = NULL;
	WordItem *lastCollectionWordItem = NULL;
	WordItem *lastContextWordItem = NULL;
	WordItem *lastPredefinedWordItem = NULL;
	WordItem *predefinedNounLanguageWordItem = NULL;
	WordItem *predefinedNounUserWordItem = NULL;

	char learnedFromUserString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char queryString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char writtenSentenceString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char writtenUserSentenceString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	};

/*************************************************************************
 *	"Honor the Lord for the glory of his name.
 *	Worship the Lord in the splendor of his holiness." (Psalm 29:2)
 *************************************************************************/
