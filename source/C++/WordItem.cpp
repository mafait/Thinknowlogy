/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
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

#include "ContextList.cpp"
#include "GeneralizationList.cpp"
#include "GrammarList.cpp"
#include "InterfaceList.cpp"
#include "JustificationList.cpp"
#include "MultipleWordList.cpp"
#include "ScoreList.h"
#include "SelectionList.h"
#include "WordAssignment.cpp"
#include "WordCleanup.cpp"
#include "WordCollection.cpp"
#include "WordQuestion.cpp"
#include "WordSelectionCondition.cpp"
#include "WordSpecification.cpp"
#include "WordType.cpp"
#include "WordWrite.cpp"
#include "WordWriteSentence.cpp"
#include "WordWriteWords.cpp"


	// Private common functions

	bool WordItem::isCorrectFeminineOrMasculineArticleWord( unsigned short articleParameter )
		{
		if( isFeminineWord_ ||
		isMasculineWord_ )
			{
			return( !hasFeminineAndMasculineArticle( articleParameter ) ||

					( isFeminineWord_ &&
					isFeminineArticleParameter( articleParameter ) ) ||

					( isMasculineWord_ &&
					isMasculineArticleParameter( articleParameter ) ) );
			}

		// Not defined
		return true;
		}

	unsigned short WordItem::nUsers()
		{
		unsigned short nUsers = 0;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = commonVariables()->predefinedNounUserWordItem ) != NULL )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
				{
				do	nUsers++;
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return nUsers;
		}

	char WordItem::wordListChar( unsigned short wordListNr )
		{
		switch( wordListNr )
			{
			case WORD_ASSIGNMENT_LIST:
				return WORD_ASSIGNMENT_LIST_SYMBOL;

			case WORD_COLLECTION_LIST:
				return WORD_COLLECTION_LIST_SYMBOL;

			case WORD_GENERALIZATION_LIST:
				return WORD_GENERALIZATION_LIST_SYMBOL;

			case WORD_INTERFACE_LIST:
				return WORD_INTERFACE_LIST_SYMBOL;

			case WORD_JUSTIFICATION_LIST:
				return WORD_JUSTIFICATION_LIST_SYMBOL;

			case WORD_MULTIPLE_WORD_LIST:
				return WORD_MULTIPLE_WORD_LIST_SYMBOL;

			case WORD_GRAMMAR_LIST:
				return WORD_GRAMMAR_LIST_SYMBOL;

			case WORD_WRITE_LIST:
				return WORD_WRITE_LIST_SYMBOL;

			case WORD_SPECIFICATION_LIST:
				return WORD_SPECIFICATION_LIST_SYMBOL;

			case WORD_TYPE_LIST:
				return WORD_TYPE_LIST_SYMBOL;

			case WORD_CONTEXT_LIST:
				return WORD_CONTEXT_LIST_SYMBOL;
			}

		return SYMBOL_QUESTION_MARK;
		}

	char *WordItem::languageNameStringInWord( unsigned short languageNr )
		{
		WordItem *languageWordItem = languageWordItemInWord( languageNr );

		if( languageWordItem != NULL )
			return languageWordItem->anyWordTypeString();

		return (char *)NO_LANGUAGE_NAME_FOUND_STRING;
		}

	WordItem *WordItem::languageWordItemInWord( unsigned short languageNr )
		{
		unsigned short numberOfLanguages = nLanguages();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( numberOfLanguages > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					if( currentGeneralizationWordItem->isLanguageWord() &&
					languageNr == numberOfLanguages-- )
						return currentGeneralizationItem->generalizationWordItem();
					}
				}
			while( numberOfLanguages > 0 &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return NULL;
		}

	char *WordItem::userNameStringInWord( unsigned short userNr )
		{
		unsigned short numberOfUsers = nUsers();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;

		if( userNr == NO_USER_NR )
			return (char *)ADMIN_USER_NAME_STRING;

		if( numberOfUsers > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					if( userNr == numberOfUsers-- )
						return generalizationWordItem->anyWordTypeString();
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return (char *)NO_USER_NAME_FOUND_STRING;
		}


	// Private assignment functions

	SpecificationItem *WordItem::firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		if( assignmentList != NULL )
			return assignmentList->firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );

		return NULL;
		}


	// Private collection functions

	unsigned short WordItem::highestCollectionOrderNrInWord( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->highestCollectionOrderNr( collectionNr );

		return NO_ORDER_NR;
		}

	unsigned int WordItem::highestCollectionNrInWord()
		{
		if( collectionList != NULL )
			return collectionList->highestCollectionNr();

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr )
		{
		if( collectionList != NULL )
			return collectionList->nonCompoundCollectionNr( compoundCollectionNr );

		return NO_COLLECTION_NR;
		}


	// Private context functions

	bool WordItem::hasContextCurrentlyBeenUpdatedInWord( unsigned int contextNr, WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->hasContextCurrentlyBeenUpdated( contextNr, specificationWordItem );

		return false;
		}

	bool WordItem::isContextSubsetInWord( unsigned int fullSetContextNr, unsigned int subsetContextNr )
		{
		if( contextList != NULL )
			return contextList->isContextSubset( fullSetContextNr, subsetContextNr );

		return false;
		}


	// Private grammar functions

	char *WordItem::grammarStringInWord( unsigned short wordTypeNr )
		{
		if( grammarList != NULL )
			return grammarList->grammarStringInList( wordTypeNr );

		return NULL;
		}


	// Private specification functions

	ResultType WordItem::checkSpecificationForUsageInWord( SpecificationItem *unusedSpecificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->checkSpecificationItemForUsage( false, false, false, unusedSpecificationItem ) == RESULT_OK &&
		assignmentList->checkSpecificationItemForUsage( false, true, false, unusedSpecificationItem ) == RESULT_OK &&
		assignmentList->checkSpecificationItemForUsage( false, true, false, unusedSpecificationItem ) == RESULT_OK )
			assignmentList->checkSpecificationItemForUsage( false, false, true, unusedSpecificationItem );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL &&
		specificationList->checkSpecificationItemForUsage( false, false, false, unusedSpecificationItem ) == RESULT_OK )
			specificationList->checkSpecificationItemForUsage( false, false, true, unusedSpecificationItem );

		if( commonVariables()->result == RESULT_OK &&
		justificationList != NULL )
			justificationList->checkSpecificationItemForUsage( unusedSpecificationItem );

		return commonVariables()->result;
		}


	// Constructor for AdminItem

	WordItem::WordItem()
		{
		// Private constructed variables

		isNounWordSpanishAmbiguous_ = false;

		authorizationKey_ = NULL;

		wordAssignment_ = NULL;
		wordCleanup_ = NULL;
		wordCollection_ = NULL;
		wordQuestion_ = NULL;
		wordSelectionCondition_ = NULL;
		wordSpecification_ = NULL;
		wordType_ = NULL;
		wordWrite_ = NULL;
		wordWriteSentence_ = NULL;
		wordWriteWords_ = NULL;

		// Private initialized variables

		isFemale_ = false;
		isFeminineWord_ = false;
		isMale_ = false;
		isMasculineWord_ = false;
		isLanguageWord_ = false;

		wordParameter_ = NO_WORD_PARAMETER;

		// Protected constructed variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;

		predefinedMultipleWordNr = 0;

		nextAssignmentWordItem = NULL;
		nextCollectionWordItem = NULL;
		nextContextWordItem = NULL;
		nextSpecificationWordItem = NULL;
		nextTouchedDuringCurrentSentenceWordItem = NULL;

		assignmentList = NULL;
		collectionList = NULL;
		contextList = NULL;
		generalizationList = NULL;
		grammarList = NULL;
		interfaceList = NULL;
		justificationList = NULL;
		multipleWordList = NULL;
		writeList = NULL;
		specificationList = NULL;
		wordTypeList = NULL;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray[wordListNr] = NULL;
		}


	// Constructor

	WordItem::WordItem( bool isLanguageWord, unsigned short wordParameter, CommonVariables *commonVariables, List *myList )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordItem", commonVariables, myList, this );

		// Private constructed variables

		isNounWordSpanishAmbiguous_ = false;

		authorizationKey_ = NULL;

		wordAssignment_ = NULL;
		wordCleanup_ = NULL;
		wordCollection_ = NULL;
		wordQuestion_ = NULL;
		wordSelectionCondition_ = NULL;
		wordSpecification_ = NULL;
		wordType_ = NULL;
		wordWrite_ = NULL;
		wordWriteSentence_ = NULL;
		wordWriteWords_ = NULL;

		// Private initialized variables

		isFemale_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_FEMININE );
		isFeminineWord_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMale_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isMasculineWord_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;

		// Protected constructed variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;

		predefinedMultipleWordNr = 0;

		nextAssignmentWordItem = NULL;
		nextCollectionWordItem = NULL;
		nextContextWordItem = NULL;
		nextSpecificationWordItem = NULL;
		nextTouchedDuringCurrentSentenceWordItem = NULL;

		assignmentList = NULL;
		collectionList = NULL;
		contextList = NULL;
		generalizationList = NULL;
		grammarList = NULL;
		interfaceList = NULL;
		justificationList = NULL;
		multipleWordList = NULL;
		writeList = NULL;
		specificationList = NULL;
		wordTypeList = NULL;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray[wordListNr] = NULL;

		// Always create the word cleanup module
		if( ( wordCleanup_ = new WordCleanup( commonVariables, this ) ) == NULL )
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, anyWordTypeString(), "I failed to create my word cleanup module" );
		}

	WordItem::~WordItem()
		{
		if( wordAssignment_ != NULL )
			delete wordAssignment_;
		if( wordCleanup_ != NULL )
			delete wordCleanup_;
		if( wordCollection_ != NULL )
			delete wordCollection_;
		if( wordQuestion_ != NULL )
			delete wordQuestion_;
		if( wordSelectionCondition_ != NULL )
			delete wordSelectionCondition_;
		if( wordSpecification_ != NULL )
			delete wordSpecification_;
		if( wordType_ != NULL )
			delete wordType_;
		if( wordWrite_ != NULL )
			delete wordWrite_;
		if( wordWriteSentence_ != NULL )
			delete wordWriteSentence_;
		if( wordWriteWords_ != NULL )
			delete wordWriteWords_;

		if( assignmentList != NULL )
			delete assignmentList;
		if( collectionList != NULL )
			delete collectionList;
		if( contextList != NULL )
			delete contextList;
		if( generalizationList != NULL )
			delete generalizationList;
		if( grammarList != NULL )
			delete grammarList;
		if( interfaceList != NULL )
			delete interfaceList;
		if( justificationList != NULL )
			delete justificationList;
		if( multipleWordList != NULL )
			delete multipleWordList;
		if( specificationList != NULL )
			delete specificationList;
		if( wordTypeList != NULL )
			delete wordTypeList;
		if( writeList != NULL )
			delete writeList;
		}


	// Protected functions
	CollectionResultType WordItem::addCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addCollectionResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	CollectionResultType WordItem::addCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return Item::addCollectionResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	CollectionResultType WordItem::startCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startCollectionResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	CollectionResultType WordItem::startCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return Item::startCollectionResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	CollectionResultType WordItem::startCollectionResultSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startCollectionResultSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	GrammarResultType WordItem::startGrammarResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startGrammarResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	JustificationResultType WordItem::startJustificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startJustificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return addErrorInWord( functionNameString, moduleNameString, tempString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return Item::addError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return Item::addError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	ResultType WordItem::addErrorWithWordListNr( unsigned short wordListNr, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addError( wordListChar( wordListNr ), functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return startErrorInWord( functionNameString, moduleNameString, tempString );
		}

	ResultType WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	ResultType WordItem::startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SelectionResultType WordItem::addSelectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addSelectionResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SelectionResultType WordItem::startSelectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startSelectionResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SpecificationResultType WordItem::addSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SpecificationResultType WordItem::addSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return Item::addSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	SpecificationResultType WordItem::addSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return Item::addSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	SpecificationResultType WordItem::startSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SpecificationResultType WordItem::startSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return Item::startSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	SpecificationResultType WordItem::startSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return Item::startSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	SpecificationResultType WordItem::startSpecificationResultSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startSpecificationResultSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WordResultType WordItem::addWordResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addWordResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WordResultType WordItem::startWordResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startWordResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WriteResultType WordItem::addWriteResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::addWriteResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WriteResultType WordItem::startWriteResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return Item::startWriteResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WriteResultType WordItem::startWriteResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return Item::startWriteResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}


	// Protected virtual item functions

	bool WordItem::hasParameter( unsigned int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				wordParameter_ > NO_WORD_PARAMETER ) );
		}

	bool WordItem::isSorted( Item *nextSortItem )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.

		// Always add at the end of the list
		return false;
		}

	ResultType WordItem::checkForUsage()
		{
		if( assignmentList != NULL &&
		assignmentList->checkWordItemForUsage( false, false, this ) == RESULT_OK &&
		assignmentList->checkWordItemForUsage( true, false, this ) == RESULT_OK )
			assignmentList->checkWordItemForUsage( false, true, this );

		if( commonVariables()->result == RESULT_OK &&
		collectionList != NULL )
			collectionList->checkWordItemForUsage( this );

		if( commonVariables()->result == RESULT_OK &&
		contextList != NULL )
			contextList->checkWordItemForUsage( this );

		if( commonVariables()->result == RESULT_OK &&
		generalizationList != NULL )
			generalizationList->checkWordItemForUsage( this );

		if( commonVariables()->result == RESULT_OK &&
		multipleWordList != NULL )
			multipleWordList->checkWordItemForUsage( this );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->checkWordItemForUsage( false, false, this );

		if( commonVariables()->result == RESULT_OK &&
		commonVariables()->adminConditionList != NULL )
			commonVariables()->adminConditionList->checkWordItemForUsage( this );

		if( commonVariables()->result == RESULT_OK &&
		commonVariables()->adminActionList != NULL )
			commonVariables()->adminActionList->checkWordItemForUsage( this );

		if( commonVariables()->result == RESULT_OK &&
		commonVariables()->adminAlternativeList != NULL )
			commonVariables()->adminAlternativeList->checkWordItemForUsage( this );

		return commonVariables()->result;
		}

	StringResultType WordItem::findMatchingWordReferenceString( char *searchString )
		{
		StringResultType stringResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( wordTypeList != NULL )
			return wordTypeList->findMatchingWordReferenceString( searchString );

		stringResult.result = startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );
		return stringResult;
		}

	char *WordItem::toString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		Item::toString( queryWordTypeNr );


		queryString = commonVariables()->queryString;

		sprintf( tempString, "%c%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_WORD_START_CHAR, wordTypeString( true, queryWordTypeNr ), QUERY_WORD_END_CHAR );
		strcat( queryString, tempString );

		if( isAuthorizationRequiredForChanges() )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAuthorizationRequiredForChanges" );
			}

		if( isFemale_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isFemale" );
			}

		if( isFeminineWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isFeminine" );
			}

		if( isMale_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isMale" );
			}

		if( isMasculineWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isMasculine" );
			}

		if( isLanguageWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isLanguageWord" );
			}

		if( isNounWordSpanishAmbiguous_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNounWordSpanishAmbiguous" );
			}

		if( wordParameter_ > NO_WORD_PARAMETER )
			{
			sprintf( tempString, "%cwordParameter:%u", QUERY_SEPARATOR_CHAR, wordParameter_ );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected common functions

	bool WordItem::isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	bool WordItem::isAdjectiveEmpty()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	bool WordItem::isAdjectiveNo()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_NO );
		}

	bool WordItem::isAdjectiveComparison()
		{
		return ( isAdjectiveComparisonLess() ||
				isAdjectiveComparisonEqual() ||
				isAdjectiveComparisonMore() );
		}

	bool WordItem::isAdjectiveComparisonLess()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EARLIER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_LESS ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_LOWER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_SMALLER );
		}

	bool WordItem::isAdjectiveComparisonEqual()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EQUAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_SAME );
		}

	bool WordItem::isAdjectiveComparisonMore()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_BIGGER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_HIGHER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_LARGER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_LATER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_MORE );
		}

	bool WordItem::isAdjectiveOdd()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ODD );
		}

	bool WordItem::isAdjectiveEven()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVEN );
		}

	bool WordItem::isAdjectiveOddOrEven()
		{
		return ( isAdjectiveOdd() ||
				isAdjectiveEven() );
		}

	bool WordItem::isAdminWord()
		{
		return ( myList() == NULL );
		}

	bool WordItem::isAuthorizationRequiredForChanges()
		{
		return ( authorizationKey_ != NULL );
		}

	bool WordItem::isFemale()
		{
		return isFemale_;
		}

	bool WordItem::isMale()
		{
		return isMale_;
		}

	bool WordItem::isFemaleOrMale()
		{
		return ( isFemale_ ||
				isMale_ );
		}

	bool WordItem::isNounHead()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_HEAD );
		}

	bool WordItem::isNounTail()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_TAIL );
		}

	bool WordItem::isNounNumber()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_NUMBER );
		}

	bool WordItem::isNounPassword()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_PASSWORD );
		}

	bool WordItem::isNounUser()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_USER );
		}

	bool WordItem::isNounValue()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
		}

	bool WordItem::isBasicVerb()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IS ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_WAS ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_CAN_BE ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_HAS ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_HAD ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_CAN_HAVE ||

				wordParameter_ == WORD_PARAMETER_PLURAL_VERB_ARE ||
				wordParameter_ == WORD_PARAMETER_PLURAL_VERB_WERE ||
				wordParameter_ == WORD_PARAMETER_PLURAL_VERB_CAN_BE ||
				wordParameter_ == WORD_PARAMETER_PLURAL_VERB_HAVE ||
				wordParameter_ == WORD_PARAMETER_PLURAL_VERB_HAD ||
				wordParameter_ == WORD_PARAMETER_PLURAL_VERB_CAN_HAVE );
		}

	bool WordItem::isVerbImperativeDisplayOrLoginOrRead()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ );
		}

	bool WordItem::isVerbUndoOrRedo()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO );
		}

	bool WordItem::isUserDefinedWord()
		{
		return ( wordParameter_ == NO_WORD_PARAMETER );
		}

	unsigned short WordItem::wordParameter()
		{
		return wordParameter_;
		}

	unsigned short WordItem::getUserNr( WordItem *userWordItem )
		{
		unsigned short userNr = nUsers();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;
		WordItem *predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = commonVariables()->predefinedNounUserWordItem ) != NULL )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
				{
				do	{
					if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
						{
						if( generalizationWordItem == userWordItem )
							return userNr;

						userNr--;
						}
					}
				while( userNr > NO_USER_NR &&
				( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return NO_USER_NR;
		}

	ResultType WordItem::assignChangePermissions( void *authorizationKey )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignChangePermissions";

		if( authorizationKey_ == NULL )
			authorizationKey_ = authorizationKey;
		else
			{
			if( authorizationKey_ != authorizationKey )
				return startErrorInWord( functionNameString, NULL, "The authorization key is already assigned" );
			}

		return RESULT_OK;
		}

	ResultType WordItem::checkWordForStructuralIntegrity( bool isAllowingNewerReplaceOrDeleteSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordForStructuralIntegrity";

		// Check assignments for replaced or deleted justifications
		if( assignmentList != NULL )
			{
			if( assignmentList->hasReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, false ) )
				return startErrorInWord( functionNameString, NULL, "I have found a replaced or deleted justification in one of my active assignments" );

			if( assignmentList->hasReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, false ) )
				return startErrorInWord( functionNameString, NULL, "I have found a replaced or deleted justification in one of my inactive assignments" );

			if( assignmentList->hasReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, true ) )
				return startErrorInWord( functionNameString, NULL, "I have found a replaced or deleted justification in one of my archived assignments" );

			if( assignmentList->hasFoundDeletedJustificationInReplacedSpecification() )
				return startErrorInWord( functionNameString, NULL, "I have found a deleted justification in one of my replaced assignments" );
			}

		// Check for unused contexts
		if( contextList != NULL &&
		contextList->checkForUnusedContext()!= RESULT_OK )
			return startErrorInWord( functionNameString, NULL, "I failed to check for an unused context" );

		if( justificationList != NULL )
			{
			// Check justifications for replaced specifications
			if( justificationList->checkForReplacedOrDeletedSpecification() != RESULT_OK )
				return startErrorInWord( functionNameString, NULL, "I failed to check the justifications for a replaced or deleted specification" );

			// Check justifications for unreferenced justifications
			if( justificationList->checkForUnreferencedJustification() != RESULT_OK )
				return startErrorInWord( functionNameString, NULL, "I failed to check justifications for an unreferenced justification" );
			}

		// Check specifications for replaced or deleted justifications
		if( specificationList != NULL )
			{
			if( specificationList->hasReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr, false, false ) )
				return startErrorInWord( functionNameString, NULL, "I have found a replaced or deleted justification in one of my specifications" );

			if( specificationList->hasFoundDeletedJustificationInReplacedSpecification() )
				return startErrorInWord( functionNameString, NULL, "I have found a deleted justification in one of my replaced specifications" );
			}

		return RESULT_OK;
		}

	ResultType WordItem::markWordAsFemale()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordAsFemale";

		if( isMale_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already masculine" );

		if( wordTypeList == NULL ||
		wordTypeList->checkWordTypesOnMasculineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );

		isFemale_ = true;
		isMale_ = false;

		return RESULT_OK;
		}

	ResultType WordItem::markWordAsFeminine()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordAsFeminine";

		if( isMasculineWord_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already masculine" );

		if( wordTypeList == NULL ||
		wordTypeList->checkWordTypesOnMasculineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );

		isFeminineWord_ = true;
		isMasculineWord_ = false;

		return RESULT_OK;
		}

	ResultType WordItem::markWordAsMale()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordAsMale";

		if( isFemale_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already feminine" );

		if( wordTypeList == NULL ||
		wordTypeList->checkWordTypesOnFeminineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );

		isFemale_ = false;
		isMale_ = true;

		return RESULT_OK;
		}

	ResultType WordItem::markWordAsMasculine()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordAsMasculine";

		if( isFeminineWord_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already feminine" );

		if( wordTypeList == NULL ||
		wordTypeList->checkWordTypesOnFeminineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );

		isFeminineWord_ = false;
		isMasculineWord_ = true;

		return RESULT_OK;
		}

	char *WordItem::languageNameString( unsigned short languageNr )
		{
		WordItem *predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = commonVariables()->predefinedNounLanguageWordItem ) != NULL )
			return predefinedNounLanguageWordItem->languageNameStringInWord( languageNr );

		return NULL;
		}

	char *WordItem::userNameString( unsigned short userNr )
		{
		WordItem *predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = commonVariables()->predefinedNounUserWordItem ) != NULL )
			return predefinedNounUserWordItem->userNameStringInWord( userNr );

		return NULL;
		}

	char *WordItem::wordTypeNameString( unsigned short wordTypeNr )
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = commonVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->grammarStringInWord( wordTypeNr );

		return NULL;
		}

	WordItem *WordItem::languageWordItem( unsigned short languageNr )
		{
		WordItem *predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = commonVariables()->predefinedNounLanguageWordItem ) != NULL )
			return predefinedNounLanguageWordItem->languageWordItemInWord( languageNr );

		return NULL;
		}

	WordItem *WordItem::nextWordItem()
		{
		return (WordItem *)nextItem;
		}

	WordItem *WordItem::predefinedWordItem( unsigned short wordParameter )
		{
		WordItem *currentPredefinedWordItem;
		WordItem *lastPredefinedWordItem = commonVariables()->lastPredefinedWordItem;

		if( wordParameter > NO_WORD_PARAMETER &&
		( currentPredefinedWordItem = commonVariables()->firstPredefinedWordItem ) != NULL )
			{
			// Do for all predefined words
			do	{
				if( currentPredefinedWordItem->wordParameter() == wordParameter &&
				!currentPredefinedWordItem->isMultipleWord() )
					return currentPredefinedWordItem;
				}
			while( currentPredefinedWordItem != lastPredefinedWordItem &&
			( currentPredefinedWordItem = currentPredefinedWordItem->nextWordItem() ) != NULL );
			}

		return NULL;
		}


	// Protected assignment functions

	unsigned int WordItem::nActiveAssignments()
		{
		if( assignmentList != NULL )
			return assignmentList->nActiveAssignments();

		return 0;
		}

	ResultType WordItem::createNewAssignmentLevelInWord()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevelInWord";

		if( assignmentList != NULL &&
		assignmentList->createNewAssignmentLevelInList() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to create assignment level ", ( commonVariables()->currentAssignmentLevel + 1 ) );

		return RESULT_OK;
		}

	ResultType WordItem::deleteAssignmentLevelInWord()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAssignmentLevelInWord";

		if( assignmentList != NULL &&
		assignmentList->deleteAssignmentLevelInList() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to delete assignment level ", commonVariables()->currentAssignmentLevel );

		return RESULT_OK;
		}

	ResultType WordItem::inactivateActiveAssignment( SpecificationItem *activeAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateActiveAssignment";

		if( wordAssignment_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word assignment module isn't created yet" );

		return wordAssignment_->inactivateActiveAssignment( activeAssignmentItem );
		}

	ResultType WordItem::archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "archiveInactivateAssignment";

		if( wordAssignment_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word assignment module isn't created yet" );

		return wordAssignment_->archiveInactiveAssignment( inactiveAssignmentItem );
		}

	SpecificationResultType WordItem::getAssignmentOrderNr()
		{
		SpecificationResultType specificationResult;

		if( wordAssignment_ != NULL )
			return wordAssignment_->getAssignmentOrderNr();

		return specificationResult;
		}

	SpecificationResultType WordItem::getAssignmentWordParameter()
		{
		SpecificationResultType specificationResult;

		if( wordAssignment_ != NULL )
			return wordAssignment_->getAssignmentWordParameter();

		return specificationResult;
		}

	SpecificationResultType WordItem::assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, void *authorizationKey )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != NULL &&
		// No user is logged in yet
		commonVariables()->currentUserNr != NO_USER_NR &&
		!specificationWordItem->isAuthorizedForChanges( authorizationKey ) ) )
			return startSpecificationResultErrorInWord( functionNameString, NULL, "You are not authorized to assign this word" );

		if( wordAssignment_ == NULL )
			{
			if( isAdminWord() )
				return startSpecificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have assignments" );

			// Create supporting module
			if( ( wordAssignment_ = new WordAssignment( commonVariables(), this ) ) == NULL )
				return startSpecificationResultErrorInWord( functionNameString, NULL, "I failed to create my word assignment module" );
			}

		return wordAssignment_->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString );
		}

	SpecificationItem *WordItem::firstActiveAssignmentItem( unsigned short questionParameter )
		{
		if( assignmentList != NULL )
			return assignmentList->firstAssignmentItem( false, false, false, questionParameter );

		return NULL;
		}

	SpecificationItem *WordItem::firstActiveAssignmentItem( bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem )
		{
		if( assignmentList != NULL )
			return assignmentList->firstRelationSpecificationItem( false, false, isPossessive, questionParameter, relationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstActiveNumeralAssignmentItem()
		{
		if( assignmentList != NULL )
			return assignmentList->firstActiveNumeralAssignmentItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstActiveStringAssignmentItem()
		{
		if( assignmentList != NULL )
			return assignmentList->firstActiveStringAssignmentItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstNonQuestionActiveAssignmentItem()
		{
		if( assignmentList != NULL )
			return assignmentList->firstAssignmentItem( false, false, false, false );

		return NULL;
		}

	SpecificationItem *WordItem::lastActiveNonQuestionAssignmentItem()
		{
		if( assignmentList != NULL )
			return assignmentList->lastActiveNonQuestionAssignmentItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstActiveNonQuestionAssignmentItem( bool isDifferentRelationContext, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		if( assignmentList != NULL )
			return assignmentList->firstActiveNonQuestionAssignmentItem( isDifferentRelationContext, isPossessive, relationContextNr, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList->firstAssignmentItem( false, false, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->firstAssignmentItem( true, false, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				return assignmentList->firstAssignmentItem( false, true, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList->firstAssignmentItem( false, false, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->firstAssignmentItem( true, false, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				return assignmentList->firstAssignmentItem( false, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList->firstAssignmentItem( false, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->firstAssignmentItem( true, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				return assignmentList->firstAssignmentItem( false, true, isNegative, isPossessive, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstQuestionAssignmentItem()
		{
		SpecificationItem *firstAssignmentItem = NULL;

		if( assignmentList != NULL &&
		// Active assignment
		( firstAssignmentItem = assignmentList->firstAssignmentItem( false, false, false, true ) ) == NULL )
			{
			// Inactive assignment
			if( ( firstAssignmentItem = assignmentList->firstAssignmentItem( false, true, false, true ) ) == NULL )
				// Archived assignment
				firstAssignmentItem = assignmentList->firstAssignmentItem( false, false, true, true );
			}

		return firstAssignmentItem;
		}

	SpecificationItem *WordItem::firstRecentlyReplacedAssignmentItem( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		if( assignmentList != NULL )
			return assignmentList->firstRecentlyReplacedAssignmentItem( isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

		return NULL;
		}


	// Protected cleanup functions

	void WordItem::clearReplacingInfoInWord()
		{
		if( wordCleanup_ != NULL )
			wordCleanup_->clearReplacingInfo();
		}

	void WordItem::deleteTemporaryWriteList()
		{
		if( writeList != NULL )
			writeList->deleteTemporaryList();
		}

	void WordItem::getHighestInUseSentenceNrInWord( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		if( wordCleanup_ != NULL )
			wordCleanup_->getHighestInUseSentenceNr( isIncludingDeletedItems, isIncludingTemporaryLists, isLanguageWord_, highestSentenceNr );
		}

	void WordItem::rebuildQuickAccessLists()
		{
		nextAssignmentWordItem = NULL;
		nextCollectionWordItem = NULL;
		nextContextWordItem = NULL;
		nextSpecificationWordItem = NULL;

		if( assignmentList != NULL )
			assignmentList->addToAssignmentWordQuickAccessList();

		if( collectionList != NULL )
			collectionList->addToCollectionWordQuickAccessList();

		if( contextList != NULL )
			contextList->addToContextWordQuickAccessList();

		if( specificationList != NULL )
			specificationList->addToSpecificationWordQuickAccessList();
		}

	void WordItem::setCurrentItemNrInWord()
		{
		if( wordCleanup_ != NULL )
			wordCleanup_->setCurrentItemNr( isLanguageWord_ );
		}

	ResultType WordItem::decrementItemNrRangeInWord( unsigned int startSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRangeInWord";

		if( wordCleanup_ != NULL )
			return wordCleanup_->decrementItemNrRange( isLanguageWord_, startSentenceNr, decrementItemNr, decrementOffset );

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::decrementSentenceNrsInWord( unsigned int startSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrsInWord";

		if( wordCleanup_ != NULL )
			return wordCleanup_->decrementSentenceNrs( isLanguageWord_, startSentenceNr );

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::deleteSentencesInWord( unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentencesInWord";

		if( wordCleanup_ != NULL )
			return wordCleanup_->deleteSentences( isLanguageWord_, lowestSentenceNr );

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::redoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		if( wordCleanup_ != NULL )
			return wordCleanup_->redoCurrentSentence();

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::removeFirstRangeOfDeletedItemsInWord()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItems";

		if( wordCleanup_ != NULL )
			return wordCleanup_->removeFirstRangeOfDeletedItems( isLanguageWord_ );

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::undoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoCurrentSentence";

		if( wordCleanup_ != NULL )
			return wordCleanup_->undoCurrentSentence();

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}


	// Protected collection functions

	bool WordItem::hasCollection()
		{
		return ( collectionList != NULL &&
				collectionList->hasItems() );
		}

	bool WordItem::hasCollectionNr( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->hasCollectionNr( collectionNr );

		return false;
		}

	bool WordItem::hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->hasCollectionNr( collectionNr, commonWordItem );

		return false;
		}

	bool WordItem::hasNonExclusiveCollection( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->hasNonExclusiveCollection( collectionNr );

		return false;
		}

	bool WordItem::isCompoundCollection( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->isCompoundCollection( collectionNr );

		return false;
		}

	bool WordItem::isCollectionSpanishAmbiguous( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->isCollectionSpanishAmbiguous( collectionNr );

		return false;
		}

	bool WordItem::isCompoundCollection( unsigned int collectionNr, WordItem *commonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->isCompoundCollection( collectionNr, commonWordItem );

		return false;
		}

	bool WordItem::isNonCompoundCollection( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->isNonCompoundCollection( collectionNr );

		return false;
		}

	bool WordItem::isNounWordSpanishAmbiguous()
		{
		return isNounWordSpanishAmbiguous_;
		}

	bool WordItem::isSpanishCurrentLanguage()
		{
		WordItem *currentLanguageWordItem = commonVariables()->currentLanguageWordItem;

		return ( currentLanguageWordItem != NULL &&
				currentLanguageWordItem->isNounWordSpanishAmbiguous_ );
		}

	unsigned short WordItem::collectionOrderNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->collectionOrderNr( collectionNr, collectionWordItem, commonWordItem );

		return NO_ORDER_NR;
		}

	unsigned short WordItem::highestCollectionOrderNrInAllWords( unsigned int collectionNr )
		{
		unsigned short tempCollectionOrderNr;
		unsigned short highestCollectionOrderNr = NO_ORDER_NR;
		WordItem *currentCollectionWordItem;

		if( collectionNr > NO_COLLECTION_NR &&
		( currentCollectionWordItem = commonVariables()->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( tempCollectionOrderNr = currentCollectionWordItem->highestCollectionOrderNrInWord( collectionNr ) ) > highestCollectionOrderNr )
					highestCollectionOrderNr = tempCollectionOrderNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return highestCollectionOrderNr;
		}

	unsigned int WordItem::collectionNr( unsigned short collectionWordTypeNr )
		{
		unsigned int foundCompoundCollectionNr;

		// Compound collection
		if( ( foundCompoundCollectionNr = compoundCollectionNr( collectionWordTypeNr ) ) > NO_COLLECTION_NR )
			return foundCompoundCollectionNr;

		// Non-compound collection
		return nonCompoundCollectionNr( collectionWordTypeNr );
		}

	unsigned int WordItem::collectionNr( unsigned short collectionWordTypeNr, WordItem *commonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->collectionNr( collectionWordTypeNr, commonWordItem );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::collectionNrByCompoundGeneralizationWordInWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		if( collectionList != NULL )
			return collectionList->collectionNrByCompoundGeneralizationWord( collectionWordTypeNr, compoundGeneralizationWordItem );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::compoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		if( collectionList != NULL )
			return collectionList->compoundCollectionNr( collectionWordTypeNr );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::nonCompoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		if( collectionList != NULL )
			return collectionList->nonCompoundCollectionNr( collectionWordTypeNr );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::nonCompoundCollectionNrInAllWords( unsigned int compoundCollectionNr )
		{
		unsigned int nonCompoundCollectionNr;
		WordItem *currentCollectionWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		( currentCollectionWordItem = commonVariables()->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( nonCompoundCollectionNr = currentCollectionWordItem->nonCompoundCollectionNrInWord( compoundCollectionNr ) ) > NO_COLLECTION_NR )
					return nonCompoundCollectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::highestCollectionNrInAllWords()
		{
		unsigned int tempCollectionNr;
		unsigned int highestCollectionNr = NO_COLLECTION_NR;
		WordItem *currentCollectionWordItem;

		if( ( currentCollectionWordItem = commonVariables()->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( tempCollectionNr = currentCollectionWordItem->highestCollectionNrInWord() ) > highestCollectionNr )
					highestCollectionNr = tempCollectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return highestCollectionNr;
		}

	CollectionResultType WordItem::addCollection( bool isExclusiveSpecification, bool isSpecificationGeneralization, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollection";

		if( commonWordItem == this &&
		isNounWordType( commonWordTypeNr ) )
			{
			isNounWordSpanishAmbiguous_ = true;

			if( ( currentLanguageWordItem = commonVariables()->currentLanguageWordItem ) != NULL )
				currentLanguageWordItem->isNounWordSpanishAmbiguous_ = true;
			}

		if( wordCollection_ == NULL )
			{
			if( isAdminWord() )
				return startCollectionResultErrorInWord( functionNameString, NULL, "The admin word item cannot have collections" );

			// Create supporting module
			if( ( wordCollection_ = new WordCollection( commonVariables(), this ) ) == NULL )
				return startCollectionResultErrorInWord( functionNameString, NULL, "I failed to create my word collection module" );
			}

		return wordCollection_->addCollection( isExclusiveSpecification, isSpecificationGeneralization, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
		}

	CollectionResultType WordItem::addCollectionByGeneralization( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollectionByGeneralization";

		if( wordCollection_ == NULL )
			{
			if( isAdminWord() )
				return startCollectionResultErrorInWord( functionNameString, NULL, "The admin word item cannot have collections" );

			// Create supporting module
			if( ( wordCollection_ = new WordCollection( commonVariables(), this ) ) == NULL )
				return startCollectionResultErrorInWord( functionNameString, NULL, "I failed to create my word collection module" );
			}

		return wordCollection_->addCollectionByGeneralization( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );
		}

	CollectionResultType WordItem::findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionResultType collectionResult;

		if( collectionList != NULL )
			return collectionList->findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return collectionResult;
		}

	ResultType WordItem::addCollectionByGeneralization( bool isExclusiveGeneralization, bool isQuestion, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollectionByGeneralization";

		if( wordCollection_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have collections" );

			// Create supporting module
			if( ( wordCollection_ = new WordCollection( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word collection module" );
			}

		return wordCollection_->addCollectionByGeneralization( isExclusiveGeneralization, isQuestion, collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );
		}

	WordItem *WordItem::collectionWordItem( unsigned int compoundCollectionNr, WordItem *notThisCommonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->collectionWordItem( compoundCollectionNr, notThisCommonWordItem );

		return NULL;
		}

	WordItem *WordItem::commonWordItem( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->commonWordItem( collectionNr );

		return NULL;
		}

	WordItem *WordItem::compoundGeneralizationWordItem( unsigned int compoundCollectionNr )
		{
		if( collectionList != NULL )
			return collectionList->compoundGeneralizationWordItem( compoundCollectionNr );

		return NULL;
		}

	WordItem *WordItem::feminineCollectionWordItem()
		{
		if( collectionList != NULL )
			return collectionList->feminineCollectionWordItem();

		return NULL;
		}


	// Protected context functions

	bool WordItem::hasContextCurrentlyBeenUpdatedInAllWords( unsigned int contextNr, WordItem *specificationWordItem )
		{
		WordItem *currentContextWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = commonVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->hasContextCurrentlyBeenUpdatedInWord( contextNr, specificationWordItem ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return false;
		}

	bool WordItem::hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->hasContext( contextNr, specificationWordItem );

		return false;
		}

	bool WordItem::isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr )
		{
		if( contextList != NULL )
			return ( contextList->hasContext( firstContextNr ) == contextList->hasContext( secondContextNr ) );

		return true;
		}

	bool WordItem::isContextSubsetInAllWords( unsigned int fullSetContextNr, unsigned int subsetContextNr )
		{
		WordItem *currentContextWordItem;

		if( fullSetContextNr > NO_CONTEXT_NR &&
		subsetContextNr > NO_CONTEXT_NR )
			{
			if( fullSetContextNr == subsetContextNr )
				return true;

			if( ( currentContextWordItem = commonVariables()->firstContextWordItem ) != NULL )
				{
				// Do for all context words
				do	{
					if( currentContextWordItem->isContextSubsetInWord( fullSetContextNr, subsetContextNr ) )
						return true;
					}
				while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
				}
			}

		return false;
		}

	unsigned short WordItem::contextWordTypeNrInWord( unsigned int contextNr )
		{
		ContextItem *foundContextItem;

		if( ( foundContextItem = contextItem( contextNr ) ) != NULL )
			return foundContextItem->contextWordTypeNr();

		return NO_WORD_TYPE_NR;
		}

	unsigned int WordItem::contextNr( WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextNr( specificationWordItem );

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::contextNr( bool isCompoundCollectionSpanishAmbiguous, WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem );

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::highestContextNrInWord()
		{
		if( contextList != NULL )
			return contextList->highestContextNr();

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::nContextWordsInAllWords( unsigned int contextNr, WordItem *specificationWordItem )
		{
		unsigned int nContextWords = 0;
		WordItem *currentContextWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = commonVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->hasContextInWord( contextNr, specificationWordItem ) )
					nContextWords++;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return nContextWords;
		}

	ResultType WordItem::addContext( bool isCompoundCollectionSpanishAmbiguous, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addContext";

		if( contextList == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have contexts" );

			// Create list
			if( ( contextList = new ContextList( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a context list" );

			wordListArray[WORD_CONTEXT_LIST] = contextList;
			}

		return contextList->addContext( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem );
		}

	ContextItem *WordItem::firstActiveContextItem()
		{
		if( contextList != NULL )
			return contextList->firstActiveContextItem();

		return NULL;
		}

	ContextItem *WordItem::contextItem( unsigned int contextNr )
		{
		if( contextList != NULL )
			return contextList->contextItem( contextNr );

		return NULL;
		}

	ContextItem *WordItem::contextItem( WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextItem( specificationWordItem );

		return NULL;
		}

	ContextItem *WordItem::contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextItem( isCompoundCollectionSpanishAmbiguous, nContextWords, specificationWordItem );

		return NULL;
		}

	WordItem *WordItem::contextWordItemInAllWords( unsigned int contextNr, WordItem *specificationWordItem, WordItem *previousContextWordItem )
		{
		WordItem *currentContextWordItem;

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR &&
		// Do in all (remaining) words
		( currentContextWordItem = ( previousContextWordItem == NULL ? commonVariables()->firstContextWordItem : previousContextWordItem->nextContextWordItem ) ) != NULL )
			{
			do	{
				if( currentContextWordItem->hasContextInWord( contextNr, specificationWordItem ) )
					return currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return NULL;
		}


	// Protected database connection functions
/*
	ResultType WordItem::storeWordItemInFutureDatabase()
		{
		if( assignmentList != NULL &&
		assignmentList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my assignment list in a future database" );

		if( collectionList != NULL &&
		collectionList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my collection list in a future database" );

		if( contextList != NULL &&
		contextList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my context list in a future database" );

		if( generalizationList != NULL &&
		generalizationList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my generalization list in a future database" );

		if( grammarList != NULL &&
		grammarList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my grammar list in a future database" );

		if( interfaceList != NULL &&
		interfaceList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my interface list in a future database" );

		if( justificationList != NULL &&
		justificationList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my justification list in a future database" );

		if( multipleWordList != NULL &&
		multipleWordList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my multiple word list in a future database" );

		if( specificationList != NULL &&
		specificationList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my specification list in a future database" );

		if( wordTypeList != NULL &&
		wordTypeList->storeChangesInFutureDatabase() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to store my word type list in a future database" );

		// Write list isn't stored, because it is a temporary list

		return RESULT_OK;
		}
*/

	// Protected generalization functions

	GeneralizationResultType WordItem::findGeneralization( bool isRelation, WordItem *generalizationWordItem )
		{
		GeneralizationResultType generalizationResult;

		if( generalizationList != NULL )
			return generalizationList->findGeneralization( isRelation, generalizationWordItem );

		return generalizationResult;
		}

	ResultType WordItem::createGeneralizationItem( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGeneralizationItem";

		if( generalizationList == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have generalizations" );

			// Create list
			if( ( generalizationList = new GeneralizationList( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a generalization list" );

			wordListArray[WORD_GENERALIZATION_LIST] = generalizationList;
			}

		return generalizationList->createGeneralizationItem( isLanguageWord_, isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem );
		}

	GeneralizationItem *WordItem::firstGeneralizationItem()
		{
		if( generalizationList != NULL )
			return generalizationList->firstActiveGeneralizationItem();

		return NULL;
		}

	GeneralizationItem *WordItem::firstNounSpecificationGeneralizationItem()
		{
		if( generalizationList != NULL )
			return generalizationList->firstGeneralizationItem( true, true, false );

		return NULL;
		}

	GeneralizationItem *WordItem::firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage )
		{
		if( generalizationList != NULL )
			return generalizationList->firstGeneralizationItem( false, isOnlySelectingCurrentLanguage, false );

		return NULL;
		}

	GeneralizationItem *WordItem::firstRelationGeneralizationItem()
		{
		if( generalizationList != NULL )
			return generalizationList->firstGeneralizationItem( false, false, true );

		return NULL;
		}


	// Protected grammar functions

	void WordItem::markAsChoiceEnd( unsigned int choiceEndItemNr )
		{
		if( grammarList != NULL )
			grammarList->markAsChoiceEnd( choiceEndItemNr );
		}

	void WordItem::markAsOptionEnd()
		{
		if( grammarList != NULL )
			grammarList->markAsOptionEnd();
		}

	bool WordItem::isNeededToCheckGrammar()
		{
		if( grammarList != NULL )
			return grammarList->isNeededToCheckGrammar();

		return false;
		}

	bool WordItem::isLanguageWithMergedWords()
		{
		if( grammarList != NULL )
			return grammarList->hasMergedWords();

		return false;
		}

	bool WordItem::isLanguageWord()
		{
		return isLanguageWord_;
		}

	unsigned short WordItem::nLanguages()
		{
		unsigned short nLanguages = 0;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;
		WordItem *predefinedNounLanguageWordItem = commonVariables()->predefinedNounLanguageWordItem;

		if( predefinedNounLanguageWordItem != NULL &&
		( currentGeneralizationItem = predefinedNounLanguageWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					if( generalizationWordItem->isLanguageWord() )
						nLanguages++;
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return nLanguages;
		}

	GrammarResultType WordItem::analyzeWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString )
		{
		GrammarResultType grammarResult;

		if( grammarList != NULL )
			return grammarList->analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return grammarResult;
		}

	GrammarResultType WordItem::createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGrammarItem";

		if( grammarList == NULL )
			{
			if( isAdminWord() )
				return startGrammarResultErrorInWord( functionNameString, NULL, "The admin word item cannot have grammar" );

			if( ( grammarList = new GrammarList( commonVariables(), this ) ) == NULL )
				return startGrammarResultErrorInWord( functionNameString, NULL, "I failed to create a grammar list" );

			wordListArray[WORD_GRAMMAR_LIST] = grammarList;
			}

		return grammarList->createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );
		}

	GrammarResultType WordItem::findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		GrammarResultType grammarResult;

		if( grammarList != NULL )
			return grammarList->findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return grammarResult;
		}

	ResultType WordItem::checkForDuplicateGrammarDefinition()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateGrammarDefinition";

		if( grammarList == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList->checkForDuplicateGrammarDefinition();
		}

	ResultType WordItem::checkGrammar()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammar";

		if( grammarList == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList->checkGrammar();
		}

	ResultType WordItem::checkGrammarForUsageInWord( GrammarItem *unusedGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammarForUsageInWord";

		if( grammarList == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		if( grammarList->checkGrammarItemForUsage( unusedGrammarItem ) == RESULT_OK )
			{
			if( writeList != NULL )
				return writeList->checkGrammarItemForUsage( unusedGrammarItem );
			}

		return RESULT_OK;
		}

	ResultType WordItem::linkLaterDefinedGrammarWords()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "linkLaterDefinedGrammarWords";

		if( grammarList == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList->linkLaterDefinedGrammarWords();
		}

	ResultType WordItem::expandMergedWordsInReadSentence( char *readUserSentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "expandMergedWordsInReadSentence";

		if( grammarList == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList->expandMergedWordsInReadSentence( readUserSentenceString );
		}

	ResultType WordItem::shrinkMergedWordsInWriteSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "shrinkMergedWordsInWriteSentence";

		if( grammarList == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList->shrinkMergedWordsInWriteSentence();
		}

	GrammarItem *WordItem::firstGrammarItem()
		{
		if( grammarList != NULL )
			return grammarList->firstGrammarItem();

		return NULL;
		}

	char *WordItem::guideByGrammarString( GrammarItem *startGrammarItem )
		{
		if( grammarList != NULL )
			return grammarList->guideByGrammarString( startGrammarItem );

		return NULL;
		}


	// Protected interface functions

	ResultType WordItem::checkInterface( unsigned short interfaceParameter, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkInterface";

		if( interfaceList == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have interface texts" );

			// Create list
			if( ( interfaceList = new InterfaceList( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create an interface list" );

			wordListArray[WORD_INTERFACE_LIST] = interfaceList;
			}

		return interfaceList->checkInterface( interfaceParameter, interfaceString );
		}

	ResultType WordItem::createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createInterfaceItem";

		if( interfaceList != NULL )
			return interfaceList->createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );

		return startErrorInWord( functionNameString, NULL, "The interface list isn't created yet" );
		}

	const char *WordItem::interfaceString( unsigned short interfaceParameter )
		{
		if( interfaceList != NULL )
			return interfaceList->interfaceString( interfaceParameter );

		return INTERFACE_STRING_NOT_AVAILABLE;
		}


	// Protected justification functions

	void WordItem::clearJustificationHasBeenWritten()
		{
		if( justificationList != NULL )
			justificationList->clearJustificationHasBeenWritten();
		}

	JustificationResultType WordItem::addJustification( bool hasFeminineOrMasculineProperNameEnding, bool isForcingNewJustification, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addJustification";

		if( justificationList == NULL )
			{
			if( isAdminWord() )
				return startJustificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have justifications" );

			if( ( justificationList = new JustificationList( commonVariables(), this ) ) == NULL )
				return startJustificationResultErrorInWord( functionNameString, NULL, "I failed to create a justification list" );

			wordListArray[WORD_JUSTIFICATION_LIST] = justificationList;
			}

		return justificationList->addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem );
		}

	JustificationResultType WordItem::copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyJustificationItem";

		if( justificationList == NULL )
			return startJustificationResultErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList->copyJustificationItem( newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );
		}

	ResultType WordItem::attachJustificationInWord( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustificationInWord";

		if( justificationList == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList->attachJustification( attachJustificationItem, involvedSpecificationItem );
		}

	ResultType WordItem::checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationForUsageOfInvolvedWords";

		if( justificationList != NULL )
			justificationList->checkSpecificationItemForUsage( unusedSpecificationItem );

		if( commonVariables()->result == RESULT_OK &&
		checkSpecificationForUsageInWord( unusedSpecificationItem ) == RESULT_OK )
			{
			if( ( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
				{
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return startErrorInWord( functionNameString, NULL, "I have found an undefined generalization word" );

					currentGeneralizationWordItem->checkSpecificationForUsageInWord( unusedSpecificationItem );
					}
				while( commonVariables()->result == RESULT_OK &&
				( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
				}
			}

		return commonVariables()->result;
		}

	ResultType WordItem::replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustification";

		if( justificationList == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList->replaceJustification( false, obsoleteJustificationItem, replacingJustificationItem, involvedSpecificationItem, NULL );
		}

	ResultType WordItem::replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteJustification";

		if( justificationList == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList->replaceOrDeleteJustification( obsoleteJustificationItem );
		}

	ResultType WordItem::replaceOrDeleteObsoleteJustifications()
		{
		JustificationItem *obsoleteJustificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteObsoleteJustifications";

		if( justificationList != NULL )
			{
			while( ( obsoleteJustificationItem = justificationList->obsoleteJustificationItem() ) != NULL )
				{
				if( replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
					return addErrorInWord( functionNameString, NULL, "I failed to replace an obsolete justification item" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::updateSpecificationsInJustificationInWord( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->updateSpecificationsInJustifications( isMainWord, obsoleteSpecificationItem, replacingSpecificationItem );

		return RESULT_OK;
		}

	ResultType WordItem::writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelatedJustificationSpecifications";

		if( justificationList == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList->writeRelatedJustificationSpecifications( justificationTypeNr, specificationCollectionNr );
		}

	JustificationItem *WordItem::correctedAssumptionByOppositeQuestionJustificationItem()
		{
		if( justificationList != NULL )
			return justificationList->correctedAssumptionByOppositeQuestionJustificationItem();

		return NULL;
		}

	JustificationItem *WordItem::negativeAssumptionOrConclusionJustificationItem( SpecificationItem *anotherPrimarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->olderComplexJustificationItem( hasSecondarySpecificationWithoutRelationContext, justificationTypeNr, secondarySpecificationCollectionNr, primarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::possessiveReversibleAssumptionJustificationItem( WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem )
		{
		if( justificationList != NULL )
			return justificationList->possessiveReversibleAssumptionJustificationItem( generalizationWordItem, secondarySpecificationWordItem );

		return NULL;
		}

	JustificationItem *WordItem::primarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->primarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, primarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->secondarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, secondarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem *secondarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->specificationSubstitutionPartOfAssumptionJustificationItem( secondarySpecificationItem );

		return NULL;
		}

	SpecificationItem *WordItem::olderSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		if( justificationList != NULL )
			return justificationList->olderSuggestiveQuestionAssumptionSecondarySpecificationItem();

		return NULL;
		}


	// Protected multiple word functions

	bool WordItem::isMultipleWord()
		{
		return ( multipleWordList != NULL &&
				multipleWordList->hasItems() );
		}

	unsigned short WordItem::matchingMultipleSingularNounWordParts( char *sentenceString )
		{
		if( multipleWordList != NULL )
			return multipleWordList->matchingMultipleSingularNounWordParts( sentenceString );

		return 0;
		}

	ResultType WordItem::addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( multipleWordList == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have multiple words" );

			// Create list
			if( ( multipleWordList = new MultipleWordList( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a multiple word list" );

			wordListArray[WORD_MULTIPLE_WORD_LIST] = multipleWordList;
			}

		return multipleWordList->addMultipleWord( nWordParts, wordTypeNr, multipleWordItem );
		}


	// Protected query functions

	void WordItem::countQuery()
		{
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				wordListArray[wordListNr]->countQueryInList();
			}
		}

	void WordItem::clearQuerySelections()
		{
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				wordListArray[wordListNr]->clearQuerySelectionsInList();
			}
		}

	bool WordItem::hasActiveQuestionWithCompoundCollection()
		{
		if( specificationList != NULL )
			return specificationList->hasActiveQuestionWithCompoundCollection();

		return false;
		}

	ResultType WordItem::itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query items" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to do a list query list" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query word types" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query parameters" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::wordQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString )
		{
		StringResultType stringResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInWord";

		if( ( stringResult = findMatchingWordReferenceString( wordNameString ) ).result != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to find words" );

		if( stringResult.hasFoundMatchingStrings )
			{
			if( isSelectingOnFind &&
			!isSelectedByQuery )
				{
				isSelectedByQuery = true;
				commonVariables()->hasFoundQuery = true;
				}
			}
		else
			{
			if( !isSelectingOnFind &&
			isSelectedByQuery )
				isSelectedByQuery = false;
			}

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->wordQueryInList( ( isSelectingOnFind && stringResult.hasFoundMatchingStrings ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query word items" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::wordReferenceQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query word references" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::stringQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query strings" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::displayQueryResultInWord( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayQueryResultInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->displayQueryResultInList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to display the items" );
				}
			}

		return RESULT_OK;
		}


	// Protected question functions

	bool WordItem::hasFoundAnswerToQuestion()
		{
		if( wordQuestion_ != NULL )
			return wordQuestion_->hasFoundAnswerToQuestion();

		return false;
		}

	ResultType WordItem::findAnswerToNewUserQuestion()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswerToNewUserQuestion";

		if( wordQuestion_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have questions" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word question module" );
			}

		return wordQuestion_->findAnswerToNewUserQuestion();
		}

	ResultType WordItem::findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswersToQuestions";

		if( wordQuestion_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have questions" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word question module" );
			}

		return wordQuestion_->findAnswersToQuestions( compoundSpecificationCollectionNr, answerSpecificationItem );
		}

	ResultType WordItem::writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeAnswerToQuestion";

		if( wordQuestion_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word question module isn't created yet" );

		return wordQuestion_->writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );
		}

	SpecificationResultType WordItem::findQuestionToBeAdjustedByCompoundCollection( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCompoundCollectionNr, unsigned int relationContextNr, SpecificationItem *replacingSpecificationItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( wordQuestion_ == NULL )
			return startSpecificationResultErrorInWord( functionNameString, NULL, "My word question module isn't created yet" );

		return wordQuestion_->findQuestionToBeAdjustedByCompoundCollection( isNegative, isPossessive, questionParameter, specificationCompoundCollectionNr, relationContextNr, replacingSpecificationItem, specificationWordItem );
		}


	// Protected selection functions

	ResultType WordItem::checkSelectionForUsageInWord( SelectionItem *unusedSelectionItem )
		{
		ScoreList *adminScoreList;
		SelectionList *adminSelectionList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionForUsageInWord";

		if( ( adminScoreList = commonVariables()->adminScoreList ) != NULL )
			{
			if( adminScoreList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin score list for selection item usage" );
			}

		if( ( adminSelectionList = commonVariables()->adminConditionList ) != NULL )
			{
			// Admin condition list
			if( adminSelectionList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin condition list for selection item usage" );
			}

		if( ( adminSelectionList = commonVariables()->adminActionList ) != NULL )
			{
			// Admin action list
			if( adminSelectionList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin action list for selection item usage" );
			}

		if( ( adminSelectionList = commonVariables()->adminAlternativeList ) != NULL )
			{
			// Admin alternative list
			if( adminSelectionList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin alternative list for selection item usage" );
			}

		return RESULT_OK;
		}


	// Protected selection condition functions

	SelectionResultType WordItem::checkSelectionCondition( SelectionItem *conditionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkDuplicateCondition";

		if( wordSelectionCondition_ == NULL &&
		// Create supporting module
		( wordSelectionCondition_ = new WordSelectionCondition( commonVariables(), this ) ) == NULL )
			return startSelectionResultErrorInWord( functionNameString, NULL, "I failed to create my word selection condition module" );

		return wordSelectionCondition_->checkSelectionCondition( conditionSelectionItem );
		}


	// Protected specification functions

	void WordItem::initializeVariablesInWord()
		{
		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;
		isWordCheckedForSolving = false;

		nextTouchedDuringCurrentSentenceWordItem = NULL;

		if( justificationList != NULL )
			justificationList->initializeJustificationVariables();

		if( assignmentList != NULL )
			{
			assignmentList->initializeSpecificationVariables( false, false );
			assignmentList->initializeSpecificationVariables( true, false );
			assignmentList->initializeSpecificationVariables( false, true );
			}

		if( specificationList != NULL )
			specificationList->initializeSpecificationVariables( false, false );

		if( wordQuestion_ != NULL )
			wordQuestion_->clearFoundAnswerToQuestion();

		if( wordSpecification_ != NULL )
			wordSpecification_->initializeWordSpecificationVariables();
		}

	bool WordItem::hasAnsweredSelfGeneratedQuestion()
		{
		if( specificationList != NULL )
			return specificationList->hasAnsweredSelfGeneratedQuestion();

		return false;
		}

	bool WordItem::hasAnyUserSpecification()
		{
		if( specificationList != NULL )
			return specificationList->hasAnyUserSpecification();

		return false;
		}

	bool WordItem::hasConfirmedSpecification()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasConfirmedSpecification();

		return false;
		}

	bool WordItem::hasContextInSpecificationsInWord( unsigned int contextNr )
		{
		if( assignmentList != NULL &&

		( assignmentList->hasContextInSpecifications( false, false, false, contextNr ) ||
		assignmentList->hasContextInSpecifications( true, false, false, contextNr ) ||
		assignmentList->hasContextInSpecifications( false, true, false, contextNr ) ||
		assignmentList->hasContextInSpecifications( false, false, true, contextNr ) ) )
			return true;

		if( specificationList != NULL &&

		( specificationList->hasContextInSpecifications( false, false, false, contextNr ) ||
		specificationList->hasContextInSpecifications( false, false, true, contextNr ) ) )
			return true;

		return false;
		}

	bool WordItem::hasContextInSpecificationsInAllWords( unsigned int contextNr )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
			{
			do	{
				currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem();

				if( currentGeneralizationWordItem != NULL &&
				currentGeneralizationWordItem->hasContextInSpecificationsInWord( contextNr ) )
					return true;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
			}

		return false;
		}

	bool WordItem::hasCorrectedAssumption()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasCorrectedAssumption();

		return false;
		}

	bool WordItem::hasCorrectedAssumptionByKnowledge()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasCorrectedAssumptionByKnowledge();

		return false;
		}

	bool WordItem::hasCorrectedAssumptionByOppositeQuestion()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasCorrectedAssumptionByOppositeQuestion();

		return false;
		}

	bool WordItem::hasDisplayedMoreSpecificRelatedQuestion()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasDisplayedMoreSpecificRelatedQuestion();

		return false;
		}

	bool WordItem::hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		if( specificationList != NULL )
			return specificationList->hasNonPossessiveAndNonQuestionPartOfSpecification();

		return false;
		}

	bool WordItem::hasPossessiveButNonQuestionSpecification()
		{
		if( specificationList != NULL )
			return specificationList->hasPossessiveButNonQuestionSpecification();

		return false;
		}

	bool WordItem::isAuthorizedForChanges( void *authorizationKey )
		{
		return ( authorizationKey_ == NULL ||
				authorizationKey_ == authorizationKey );
		}

	bool WordItem::isJustificationInUse( JustificationItem *unusedJustificationItem )
		{
		bool isIncludingReplacedItems = ( unusedJustificationItem != NULL &&
										unusedJustificationItem->isReplacedItem() );

		if( assignmentList != NULL )
			{
			if( assignmentList->isJustificationInUse( false, false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList->isJustificationInUse( true, false, false, unusedJustificationItem ) )
				return true;

			if( assignmentList->isJustificationInUse( false, true, false, unusedJustificationItem ) )
				return true;

			if( isIncludingReplacedItems &&
			assignmentList->isJustificationInUse( false, false, true, unusedJustificationItem ) )
				return true;
			}

		if( specificationList != NULL )
			{
			if( specificationList->isJustificationInUse( false, false, false, unusedJustificationItem ) )
				return true;

			if( isIncludingReplacedItems &&
			specificationList->isJustificationInUse( false, false, true, unusedJustificationItem ) )
				return true;
			}

		return false;
		}

	ResultType WordItem::changeJustificationOfNegativeAssumptions( SpecificationItem *secondarySpecificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem ) == RESULT_OK &&
		assignmentList->changeJustificationOfNegativeAssumptions( true, false, secondarySpecificationItem ) == RESULT_OK )
			assignmentList->changeJustificationOfNegativeAssumptions( false, true, secondarySpecificationItem );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem );

		return commonVariables()->result;
		}

	ResultType WordItem::checkForSpecificationConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForSpecificationConflict";

		if( wordSpecification_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem );
		}

	ResultType WordItem::checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->checkJustificationForUsage( false, false, false, unusedJustificationItem ) == RESULT_OK &&
		assignmentList->checkJustificationForUsage( true, false, false, unusedJustificationItem ) == RESULT_OK &&
		assignmentList->checkJustificationForUsage( false, true, false, unusedJustificationItem ) == RESULT_OK )
			assignmentList->checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL &&
		specificationList->checkJustificationForUsage( false, false, false, unusedJustificationItem ) == RESULT_OK )
			specificationList->checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( commonVariables()->result == RESULT_OK &&
		justificationList != NULL )
			justificationList->checkJustificationForUsage( unusedJustificationItem );

		return commonVariables()->result;
		}

	ResultType WordItem::clearLastWrittenSentenceStringWithUnknownPluralNoun( const char *unknownPluralNounString, WordItem *specificationWordItem )
		{
		if( assignmentList != NULL &&
		assignmentList->clearLastWrittenSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem ) == RESULT_OK &&
		assignmentList->clearLastWrittenSentenceStringWithUnknownPluralNoun( true, false, unknownPluralNounString, specificationWordItem ) == RESULT_OK )
			assignmentList->clearLastWrittenSentenceStringWithUnknownPluralNoun( false, true, unknownPluralNounString, specificationWordItem );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->clearLastWrittenSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem );

		return commonVariables()->result;
		}

	ResultType WordItem::collectGeneralizationsOrSpecifications( bool isExclusiveSpecification, bool isGeneralizationCollection, bool isQuestion, unsigned int collectionNr )
		{
		if( assignmentList != NULL &&
		assignmentList->collectGeneralizationsOrSpecifications( false, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr ) == RESULT_OK &&
		assignmentList->collectGeneralizationsOrSpecifications( true, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr ) == RESULT_OK )
			assignmentList->collectGeneralizationsOrSpecifications( false, true, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->collectGeneralizationsOrSpecifications( false, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr );

		return commonVariables()->result;
		}

	ResultType WordItem::confirmSpecificationButNotItsRelation( SpecificationItem *confirmedSpecificationItem, SpecificationItem *confirmationSpecificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->confirmSpecificationButNotItsRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem ) == RESULT_OK &&
		assignmentList->confirmSpecificationButNotItsRelation( true, false, confirmedSpecificationItem, confirmationSpecificationItem ) == RESULT_OK )
			assignmentList->confirmSpecificationButNotItsRelation( false, true, confirmedSpecificationItem, confirmationSpecificationItem );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->confirmSpecificationButNotItsRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem );

		return commonVariables()->result;
		}

	ResultType WordItem::copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyAndReplaceSpecificationItem";

		if( originalSpecificationItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given original specification item is undefined" );

		if( originalSpecificationItem->isAssignment() )
			{
			if( assignmentList == NULL )
				return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList->copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
			}

		if( specificationList == NULL )
			return startErrorInWord( functionNameString, NULL, "My specification list isn't created yet" );

		return specificationList->copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
		}

	ResultType WordItem::recalculateAssumptionsInAllTouchedWords()
		{
		WordItem *currentTouchedWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "recalculateAssumptionsInAllTouchedWords";

		if( ( currentTouchedWordItem = commonVariables()->firstTouchedWordItem ) != NULL )
			{
			// Do for all words touched during current sentence
			do	{
				if( currentTouchedWordItem->recalculateAssumptions() != RESULT_OK )
					return addErrorInWord( functionNameString, NULL, "I failed to recalculate the assumptions of word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedDuringCurrentSentenceWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType WordItem::recalculateAssumptions()
		{
		if( assignmentList != NULL &&
		assignmentList->recalculateAssumptions( false, false ) == RESULT_OK &&
		assignmentList->recalculateAssumptions( true, false ) == RESULT_OK )
			assignmentList->recalculateAssumptions( false, true );

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->recalculateAssumptions( false, false );

		return commonVariables()->result;
		}

	ResultType WordItem::replaceOrDeleteSpecification( bool isAllowingReplacedObsoleteSpecification, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isAssignment() )
			{
			if( assignmentList == NULL )
				return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList->replaceOrDeleteSpecification( false, obsoleteSpecificationItem, replacingSpecificationItem );
			}

		if( specificationList == NULL )
			return startErrorInWord( functionNameString, NULL, "The specification list isn't created yet" );

		return specificationList->replaceOrDeleteSpecification( isAllowingReplacedObsoleteSpecification, obsoleteSpecificationItem, replacingSpecificationItem );
		}

	ResultType WordItem::updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		bool isIncludingReplacedItems = ( obsoleteJustificationItem != NULL &&
										obsoleteJustificationItem->hasCurrentCreationSentenceNr() );

		if( assignmentList != NULL &&
		assignmentList->updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList->updateJustificationInSpecifications( true, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList->updateJustificationInSpecifications( false, true, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			{
			if( isIncludingReplacedItems )
				assignmentList->updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		if( commonVariables()->result == RESULT_OK &&
		specificationList != NULL &&
		specificationList->updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			{
			if( isIncludingReplacedItems )
				specificationList->updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		return commonVariables()->result;
		}

	ResultType WordItem::updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateSpecificationItemsInJustificationOfInvolvedWords";

		// Update specifications in justifications of this word
		if( updateSpecificationsInJustificationInWord( true, obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to update my justification items" );

		if( ( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
			{
			// Update specifications in justifications of involved words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return startErrorInWord( functionNameString, NULL, "I have found an undefined generalization word" );

				if( currentGeneralizationWordItem->updateSpecificationsInJustificationInWord( false, obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
					return addErrorInWord( functionNameString, NULL, "I failed to update the justification items in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	SpecificationResultType WordItem::addSpecificationInWord( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, void *authorizationKey )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationInWord";

		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != NULL &&
		// No user is logged in yet
		commonVariables()->currentUserNr > NO_USER_NR &&
		!specificationWordItem->isAuthorizedForChanges( authorizationKey ) ) )
			return startSpecificationResultErrorInWord( functionNameString, NULL, "You are not authorized to add this word" );

		if( wordSpecification_ == NULL )
			{
			if( isAdminWord() )
				return startSpecificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have specifications" );

			// Create supporting module
			if( ( wordSpecification_ = new WordSpecification( commonVariables(), this ) ) == NULL )
				return startSpecificationResultErrorInWord( functionNameString, NULL, "I failed to create my word specification module" );
			}

		return wordSpecification_->addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );
		}

	SpecificationResultType WordItem::createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *writtenSentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSpecificationItem";

		if( isAssignment )
			{
			if( assignmentList == NULL )
				{
				if( isAdminWord() )
					return startSpecificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have assignments" );

				// Create assignment list
				if( ( assignmentList = new SpecificationList( WORD_ASSIGNMENT_LIST_SYMBOL, commonVariables(), this ) ) == NULL )
					return startSpecificationResultErrorInWord( functionNameString, NULL, "I failed to create an assignment list" );

				wordListArray[WORD_ASSIGNMENT_LIST] = assignmentList;
				}

			return assignmentList->createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceString );
			}

		if( specificationList == NULL )
			{
			if( isAdminWord() )
				return startSpecificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have specifications" );

			// Create specification list
			if( ( specificationList = new SpecificationList( WORD_SPECIFICATION_LIST_SYMBOL, commonVariables(), this ) ) == NULL )
				return startSpecificationResultErrorInWord( functionNameString, NULL, "I failed to create a specification list" );

			wordListArray[WORD_SPECIFICATION_LIST] = specificationList;
			}

		return specificationList->createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceString );
		}

	SpecificationResultType WordItem::findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ == NULL )
			return startSpecificationResultErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );
		}

	SpecificationResultType WordItem::findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ == NULL )
			return startSpecificationResultErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->findRelatedSpecification( false, false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem ) ) == NULL )
			foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingAssignments &&
		assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAnsweredQuestions, bool isIncludingAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingAssignments &&
		assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( isIncludingInactiveAssignments )
				foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyContextResult, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstActiveQuestionSpecificationItem()
		{
		if( specificationList != NULL )
			return specificationList->firstActiveSpecificationItem( false, true );

		return NULL;
		}

	SpecificationItem *WordItem::firstActiveAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingAssignments &&
		assignmentList != NULL )
			foundSpecificationItem = assignmentList->firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstAnsweredQuestionAssignmentItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			{
			foundSpecificationItem = assignmentList->firstAnsweredQuestionAssignmentItem( false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->firstAnsweredQuestionAssignmentItem( true, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingAssignments &&
		assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == NULL )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList->firstSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstAssignmentOrSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstSpecificationItem( false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList->firstSpecificationItem( true, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSpecificationItem( false, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstFeminineOrMasculineSpecificationItem()
		{
		if( specificationList != NULL )
			return specificationList->firstFeminineOrMasculineSpecificationItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstNonCompoundCollectionSpecificationItem( unsigned int compoundSpecificationCollectionNr )
		{
		if( specificationList != NULL )
			return specificationList->firstNonCompoundCollectionSpecificationItem( compoundSpecificationCollectionNr );

		return NULL;
		}

	SpecificationItem *WordItem::firstNonQuestionSpecificationItem()
		{
		if( specificationList != NULL )
			return specificationList->firstActiveSpecificationItem( false, false );

		return NULL;
		}

	SpecificationItem *WordItem::firstRecentlyAnsweredQuestionSpecificationItem( unsigned short questionParameter )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstRecentlyAnsweredQuestionSpecificationItem( false, questionParameter ) ) == NULL )
			foundSpecificationItem = assignmentList->firstRecentlyAnsweredQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return foundSpecificationItem = specificationList->firstRecentlyAnsweredQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstRelationSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			foundSpecificationItem = assignmentList->firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		if( foundSpecificationItem == NULL &&
		!isInactiveAssignment &&
		!isArchivedAssignment &&
		specificationList != NULL )
			return specificationList->firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSelectedSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		return ( isAssignment ? firstAssignmentItem( isInactiveAssignment, isArchivedAssignment, isQuestion ) :
				( isQuestion ? firstActiveQuestionSpecificationItem() : firstNonQuestionSpecificationItem() ) );
		}

	SpecificationItem *WordItem::firstSelectedSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		if( isIncludingAnsweredQuestions &&
		questionParameter > NO_QUESTION_PARAMETER )
			{
			if( isAssignment )
				{
				if( assignmentList != NULL )
					return assignmentList->firstAssignmentItem( true, isInactiveAssignment, isArchivedAssignment, questionParameter );
				}
			else
				{
				if( specificationList != NULL )
					return specificationList->firstActiveSpecificationItem( true, questionParameter );
				}
			}
		else
			{
			if( isAssignment )
				{
				if( assignmentList != NULL )
					{
					if( isArchivedAssignment )
						return assignmentList->firstAssignmentItem( false, false, true, questionParameter );

					if( isInactiveAssignment )
						return assignmentList->firstAssignmentItem( false, true, false, questionParameter );

					return firstActiveAssignmentItem( questionParameter );
					}
				}
			else
				{
				if( questionParameter == NO_QUESTION_PARAMETER )
					return firstNonQuestionSpecificationItem();

				if( specificationList != NULL )
					return specificationList->firstActiveSpecificationItem( false, questionParameter );
				}
			}

		return NULL;
		}

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == NULL )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingAssignments &&
		assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *moreRecentSpecificationItem;
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) == NULL )
			foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( true, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem );

		if( specificationList != NULL &&
		( moreRecentSpecificationItem = specificationList->firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, specificationWordItem ) ) != NULL )
			{
			if( foundSpecificationItem == NULL ||

			// Prefer more recent specification over assignment
			( foundSpecificationItem->hasRelationContext() == moreRecentSpecificationItem->hasRelationContext() &&
			foundSpecificationItem->originalSentenceNr() < moreRecentSpecificationItem->originalSentenceNr() ) )
				foundSpecificationItem = moreRecentSpecificationItem;
			}

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		if( specificationList != NULL )
			return specificationList->firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstUnhiddenSpecificationItem( unsigned int relationContextNr )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			foundSpecificationItem = assignmentList->firstUnhiddenSpecificationItem( true, relationContextNr );

		if( specificationList != NULL )
			return specificationList->firstUnhiddenSpecificationItem( false, relationContextNr );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( ( foundSpecificationItem = assignmentList->firstUserSpecificationItem( true, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == NULL )
				foundSpecificationItem = assignmentList->firstUserSpecificationItem( false, true, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::sameUserQuestionSpecificationItem( unsigned short questionParameter )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->sameUserQuestionSpecificationItem( false, questionParameter ) ) == NULL )
			foundSpecificationItem = assignmentList->sameUserQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->sameUserQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	WordItem *WordItem::feminineOrMasculineCommonWordItem( bool isFeminineWord )
		{
		if( specificationList != NULL )
			return specificationList->feminineOrMasculineCommonWordItem( isFeminineWord );

		return NULL;
		}


	// Protected word type functions

	void WordItem::clearGeneralizationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList != NULL )
			wordTypeList->clearGeneralizationWriteLevel( isLanguageWord_, currentWriteLevel );
		}

	void WordItem::clearSpecificationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList != NULL )
			wordTypeList->clearSpecificationWriteLevel( currentWriteLevel );
		}

	void WordItem::clearRelationWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		if( contextList != NULL &&
		wordTypeList != NULL &&
		contextList->hasContext( contextNr ) )
			wordTypeList->clearRelationWriteLevel( currentWriteLevel );
		}

	bool WordItem::hasAnyWordType()
		{
		if( wordTypeList != NULL )
			return wordTypeList->hasAnyWordType();

		return false;
		}

	bool WordItem::hasFeminineAndMasculineArticle( unsigned short articleParameter )
		{
		unsigned short oppositeArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		WordItem *oppositeArticleWordItem;

		switch( articleParameter )
			{
			// Plural phonetic vowel article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE;
				break;

			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE;
				break;

			// Singular phonetic vowel article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE;
				break;

			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE;
				break;

			// Plural article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE;
				break;

			case WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE;
				break;

			// Singular article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE;
				break;

			case WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE;
				break;

			// Plural article
			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE;
				break;

			case WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE;
				break;

			// Singular article
			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE;
				break;

			case WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE:
				oppositeArticleParameter = WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE;
				break;
			}

		if( ( oppositeArticleWordItem = predefinedWordItem( oppositeArticleParameter ) ) != NULL )
			return oppositeArticleWordItem->hasWordType( false, WORD_TYPE_ARTICLE );

		return false;
		}

	bool WordItem::hasFeminineProperNameEnding()
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NAME ) ) != NULL )
			return foundWordTypeItem->hasFeminineWordEnding();

		return false;
		}

	bool WordItem::hasMasculineProperNameEnding()
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NAME ) ) != NULL )
			return foundWordTypeItem->hasMasculineWordEnding();

		return false;
		}

	bool WordItem::hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return ( wordTypeList->activeWordTypeItem( isAllowingDifferentNoun, isLanguageWord_, wordTypeNr ) != NULL );

		return false;
		}

	bool WordItem::isGeneralizationWordTypeAlreadyWritten( unsigned short generalizationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, generalizationWordTypeNr ) ) != NULL )
			return foundWordTypeItem->isGeneralizationWordAlreadyWritten();

		return false;
		}

	bool WordItem::isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( true, isLanguageWord_, specificationWordTypeNr ) ) != NULL )
			return foundWordTypeItem->isSpecificationWordAlreadyWritten();

		return false;
		}

	bool WordItem::isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( true, isLanguageWord_, relationWordTypeNr ) ) != NULL )
			return foundWordTypeItem->isRelationWordAlreadyWritten();

		return false;
		}

	bool WordItem::isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectAdjective( adjectiveParameter );

		return false;
		}

	bool WordItem::isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		isCorrectFeminineOrMasculineArticleWord( definiteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectDefiniteArticle( definiteArticleParameter );

		return false;
		}

	bool WordItem::isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		isCorrectFeminineOrMasculineArticleWord( indefiniteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectIndefiniteArticle( true, indefiniteArticleParameter );

		return false;
		}

	bool WordItem::isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, void *hideKey )
		{
		if( wordTypeList != NULL )
			return wordTypeList->isCorrectHiddenWordType( wordTypeNr, compareString, hideKey );

		return false;
		}

	bool WordItem::isNumeralWordType()
		{
		return hasWordType( false, WORD_TYPE_NUMERAL );
		}

	bool WordItem::isSingularNoun()
		{
		return hasWordType( false, WORD_TYPE_NOUN_SINGULAR );
		}

	bool WordItem::isPluralNoun()
		{
		return hasWordType( false, WORD_TYPE_NOUN_PLURAL );
		}

	bool WordItem::isProperName()
		{
		return hasWordType( false, WORD_TYPE_PROPER_NAME );
		}

	bool WordItem::isProperNamePrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NAME ) ) != NULL )
			return foundWordTypeItem->isProperNamePrecededByDefiniteArticle( definiteArticleParameter );

		return false;
		}

	ResultType WordItem::deleteAllWordTypesOfCurrentSentence()
		{
		if( wordTypeList != NULL )
			return wordTypeList->deleteAllWordTypesOfCurrentSentence();

		return RESULT_OK;
		}

	ResultType WordItem::deleteWordType( WordTypeItem *deleteWordTypeItem )
		{
		if( wordTypeList != NULL )
			return wordTypeList->deleteWordType( deleteWordTypeItem );

		return RESULT_OK;
		}

	ResultType WordItem::hideWordType( unsigned short wordTypeNr, void *authorizationKey )
		{
		if( wordTypeList != NULL )
			return wordTypeList->hideWordTypeItem( wordTypeNr, authorizationKey );

		return RESULT_OK;
		}

	ResultType WordItem::markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeList == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList->markGeneralizationWordTypeAsWritten( isLanguageWord_, wordTypeNr );
		}

	ResultType WordItem::markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeList == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList->markSpecificationWordTypeAsWritten( wordTypeNr );
		}

	ResultType WordItem::markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeList == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList->markRelationWordTypeAsWritten( wordTypeNr );
		}

	WordResultType WordItem::addWordType( bool isMultipleWord, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWordType";

		if( wordType_ == NULL )
			{
			if( isAdminWord() )
				return startWordResultErrorInWord( functionNameString, NULL, "The admin word item cannot have word types" );

			// Create supporting module
			if( ( wordType_ = new WordType( commonVariables(), this ) ) == NULL )
				return startWordResultErrorInWord( functionNameString, NULL, "I failed to create my word type module" );
			}

		return wordType_->addWordType( isLanguageWord_, isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
		}

	WordResultType WordItem::findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordType";

		if( wordType_ == NULL )
			return startWordResultErrorInWord( functionNameString, NULL, "My word type module isn't created yet" );

		return wordType_->findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString );
		}

	WordTypeItem *WordItem::activeWordTypeItem( unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->activeWordTypeItem( false, isLanguageWord_, wordTypeNr );

		return NULL;
		}

	WordTypeItem *WordItem::activeWordTypeItem( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr );

		return NULL;
		}

	char *WordItem::anyWordTypeString()
		{
		if( wordType_ != NULL )
			return wordType_->wordTypeString( true, NO_WORD_TYPE_NR );

		return NULL;
		}

	char *WordItem::activeWordTypeString( unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->activeWordTypeString( isLanguageWord_, wordTypeNr );

		return NULL;
		}

	char *WordItem::wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		if( wordType_ != NULL )
			return wordType_->wordTypeString( ( isCheckingAllLanguages || isLanguageWord_ ), wordTypeNr );

		return NULL;
		}

	char *WordItem::singularNounString()
		{
		return activeWordTypeString( WORD_TYPE_NOUN_SINGULAR );
		}


	// Protected write functions

	ResultType WordItem::writeJustificationSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, justificationSpecificationItem );
		}

	ResultType WordItem::writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecification";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeSelectedSpecification( false, isCheckingUserSentenceForIntegrity, true, true, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, writeSpecificationItem );
		}

	ResultType WordItem::writeSelectedSpecification( bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecification";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeSelectedSpecification( false, false, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );
		}

	ResultType WordItem::writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecificationInfo";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeSelectedSpecificationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );
		}

	ResultType WordItem::writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedRelationInfo";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeSelectedRelationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );
		}

	ResultType WordItem::writeUpdatedSpecification( bool isAdjustedSpecification, bool isConcludedSpanishAmbiguousAssumption, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, bool isRecalculatingAssumptionsInAllTouchedWords, bool isReplacedBySpecificationWithRelation, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUpdatedSpecification";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeUpdatedSpecification( isAdjustedSpecification, isConcludedSpanishAmbiguousAssumption, isCorrectedAssumptionByKnowledge, isCorrectedAssumptionByOppositeQuestion, isRecalculatingAssumptionsInAllTouchedWords, isReplacedBySpecificationWithRelation, writeSpecificationItem );
		}


	// Protected write sentence functions

	ResultType WordItem::selectGrammarToWriteSentence( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectGrammarToWriteSentence";

		if( wordWriteSentence_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have sentences" );

			// Create supporting module
			if( ( wordWriteSentence_ = new WordWriteSentence( commonVariables(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write sentence module" );
			}

		return wordWriteSentence_->selectGrammarToWriteSentence( isCheckingUserSentenceForIntegrity, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );
		}


	// Protected write words functions

	void WordItem::initializeWordWriteWordsVariables()
		{
		if( wordWriteWords_ != NULL )
			wordWriteWords_->initializeWordWriteWordsVariables();
		}

	void WordItem::initializeWordWriteWordsSpecificationVariables( bool isQuestion, size_t startWordPosition )
		{
		if( wordWriteWords_ != NULL )
			wordWriteWords_->initializeWordWriteWordsSpecificationVariables( isQuestion, startWordPosition );
		}

	WriteResultType WordItem::writeWordsToSentence( bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, GrammarItem *definitionGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeWordsToSentence";

		if( wordWriteWords_ == NULL )
			{
			if( isAdminWord() )
				return startWriteResultErrorInWord( functionNameString, NULL, "The admin word item cannot have written words" );

			// Create supporting module
			if( ( wordWriteWords_ = new WordWriteWords( commonVariables(), this ) ) == NULL )
				return startWriteResultErrorInWord( functionNameString, NULL, "I failed to create my word write words module" );
			}

		return wordWriteWords_->writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem );
		}

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
