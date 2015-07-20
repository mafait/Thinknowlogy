/*
 *	Class:			CommonVariables
 *	Purpose:		To hold the common variables
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
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

#include <string.h>
#include "Constants.h"

// Class declarations needed by some compilers
class Presentation;
class ScoreList;
class SelectionList;
class SpecificationItem;
class WordItem;

class CommonVariables
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminCleanup;
	friend class AdminCollection;
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminLanguage;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class AdminReasoning;
	friend class AdminSelection;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class AdminWriteJustification;
	friend class AdminWriteSpecification;
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
	friend class InterfaceItem;
	friend class InterfaceList;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class ListCleanup;
	friend class ListQuery;
	friend class MultipleWordItem;
	friend class MultipleWordList;
	friend class Presentation;
	friend class ReadItem;
	friend class ReadList;
	friend class ScoreItem;
	friend class ScoreList;
	friend class SelectionItem;
	friend class SelectionList;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordAssignment;
	friend class WordCleanup;
	friend class WordCollection;
	friend class WordItem;
	friend class WordList;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordType;
	friend class WordTypeItem;
	friend class WordTypeList;
	friend class WordWrite;
	friend class WordWriteSentence;
	friend class WordWriteWords;
	friend class WriteItem;
	friend class WriteList;

	// Protected common variables

	bool hasFoundAnswerToQuestion;
	bool hasFoundQuery;
	bool hasShownMessage;
	bool hasShownWarning;

	bool isAssignmentChanged;
	bool isDontIncrementCurrentSentenceNr;
	bool isFirstAnswerToQuestion;
	bool isQuestionAlreadyAnswered;
	bool isUserQuestion;

	ResultType result;

	unsigned short currentAssignmentLevel;
	unsigned short currentLanguageNr;
	unsigned short currentUserNr;
	unsigned short currentWriteLevel;

	unsigned short matchingWordTypeNr;

	unsigned int nDeletedItems;
	unsigned int nActiveQueryItems;
	unsigned int nInactiveQueryItems;
	unsigned int nArchivedQueryItems;
	unsigned int nReplacedQueryItems;
	unsigned int nDeletedQueryItems;

	unsigned int nUserGeneralizationWords;
	unsigned int nUserSpecificationWords;
	unsigned int nUserRelationWords;

	unsigned int currentSentenceNr;
	unsigned int highestInUseSentenceNr;
	unsigned int removeSentenceNr;

	unsigned int currentItemNr;
	unsigned int removeStartItemNr;

	Presentation *presentation;

	ScoreList *adminScoreList;

	SelectionList *adminConditionList;
	SelectionList *adminActionList;
	SelectionList *adminAlternativeList;

	WordItem *currentLanguageWordItem;
	WordItem *firstWordItem;
	WordItem *lastPredefinedWordItem;
	WordItem *predefinedNounLanguageWordItem;
	WordItem *predefinedNounUserWordItem;

	char queryString[MAX_SENTENCE_STRING_LENGTH];
	char writeSentenceString[MAX_SENTENCE_STRING_LENGTH];


	protected:
	// Constructor / deconstructor

	CommonVariables()
		{
		// Protected common variables

		hasFoundAnswerToQuestion = false;
		hasFoundQuery = false;
		hasShownMessage = false;
		hasShownWarning = false;

		isAssignmentChanged = false;
		isDontIncrementCurrentSentenceNr = false;
		isFirstAnswerToQuestion = false;
		isQuestionAlreadyAnswered = false;
		isUserQuestion = false;

		result = RESULT_OK;

		currentAssignmentLevel = NO_ASSIGNMENT_LEVEL;
		currentLanguageNr = NO_LANGUAGE_NR;
		currentUserNr = NO_USER_NR;
		currentWriteLevel = NO_WRITE_LEVEL;

		matchingWordTypeNr = WORD_TYPE_UNDEFINED;

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
		highestInUseSentenceNr = NO_SENTENCE_NR;
		removeSentenceNr = NO_SENTENCE_NR;

		currentItemNr = NO_ITEM_NR;
		removeStartItemNr = NO_ITEM_NR;

		presentation = NULL;

		adminScoreList = NULL;

		adminConditionList = NULL;
		adminActionList = NULL;
		adminAlternativeList = NULL;

		currentLanguageWordItem = NULL;
		firstWordItem = NULL;
		lastPredefinedWordItem = NULL;
		predefinedNounLanguageWordItem = NULL;
		predefinedNounUserWordItem = NULL;

		strcpy( queryString, EMPTY_STRING );
		strcpy( writeSentenceString, EMPTY_STRING );
		}
	};

/*************************************************************************
 *	"Honor the Lord for the glory of his name.
 *	Worship the Lord in the splendor of his holiness." (Psalm 29:2)
 *************************************************************************/
