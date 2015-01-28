/*
 *	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
#include "WordSpecification.cpp"
#include "WordType.cpp"
#include "WordWrite.cpp"
#include "WordWriteSentence.cpp"
#include "WordWriteWords.cpp"


	// Private common functions

	bool WordItem::isCorrectFeminineOrMasculineArticleWord( bool isFeminineWord, bool isMasculineWord, unsigned short articleParameter )
		{
		if( ( isFeminineWord ||
		isMasculineWord ) &&

		( isFeminineWord_ ||
		isMasculineWord_ ) )
			{
			return( !hasFeminineAndMasculineArticle( articleParameter ) ||

					( isFeminineWord_ &&
					isFeminineArticleParameter( articleParameter ) ) ||

					( isMasculineWord_ &&
					isMasculineArticleParameter( articleParameter ) ) );
			}

		// If not defined
		return true;
		}

	unsigned short WordItem::nUsers()
		{
		unsigned short nUsers = 0;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = commonVariables_->predefinedNounUserWordItem ) != NULL )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem() ) != NULL )
				{
				do	nUsers++;
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return nUsers;
		}

	char *WordItem::grammarLanguageNameStringInWord( unsigned short languageNr )
		{
		WordItem *grammarLanguageWordItem = grammarLanguageWordItemInWord( languageNr );

		if( grammarLanguageWordItem != NULL )
			return grammarLanguageWordItem->anyWordTypeString();

		return (char *)NO_GRAMMAR_NAME_FOUND_STRING;
		}

	WordItem *WordItem::grammarLanguageWordItemInWord( unsigned short languageNr )
		{
		unsigned short nLanguages = nGrammarLanguages();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( nLanguages > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					if( currentGeneralizationWordItem->isGrammarLanguageWord() &&
					languageNr == nLanguages-- )
						return currentGeneralizationItem->generalizationWordItem();
					}
				}
			while( nLanguages > 0 &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return NULL;
		}

	char *WordItem::userNameStringInWord( unsigned short userNr )
		{
		unsigned short numberOfUsers = nUsers();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;

		if( numberOfUsers > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					if( userNr == numberOfUsers-- )
						return generalizationWordItem->anyWordTypeString();
					}
				}
			while( userNr > NO_USER_NR &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
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

	unsigned int WordItem::collectionNrByCompoundGeneralizationWordInWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		if( collectionList != NULL )
			return collectionList->collectionNrByCompoundGeneralizationWord( collectionWordTypeNr, compoundGeneralizationWordItem );

		return NO_COLLECTION_NR;
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

	bool WordItem::isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr )
		{
		if( contextList != NULL )
			return ( contextList->hasContext( firstContextNr ) == contextList->hasContext( secondContextNr ) );

		return true;
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

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL &&
		specificationList->checkSpecificationItemForUsage( false, false, false, unusedSpecificationItem ) == RESULT_OK )
			specificationList->checkSpecificationItemForUsage( false, false, true, unusedSpecificationItem );

		if( commonVariables_->result == RESULT_OK &&
		justificationList != NULL )
			justificationList->checkSpecificationItemForUsage( unusedSpecificationItem );

		return commonVariables_->result;
		}


	// Constructor / deconstructor for AdminItem

	WordItem::WordItem()
		{
		// Private constructible variables

		changeKey_ = NULL;

		wordAssignment_ = NULL;
		wordCleanup_ = NULL;
		wordCollection_ = NULL;
		wordQuestion_ = NULL;
		wordSpecification_ = NULL;
		wordType_ = NULL;
		wordWrite_ = NULL;
		wordWriteSentence_ = NULL;
		wordWriteWords_ = NULL;

		// Private loadable variables

		isFeminineWord_ = false;
		isMasculineWord_ = false;
		isLanguageWord_ = false;

		wordParameter_ = NO_WORD_PARAMETER;

		// Protected constructible variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

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


	// Constructor / deconstructor

	WordItem::WordItem( bool isLanguageWord, unsigned short wordParameter, CommonVariables *commonVariables, List *myList )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordItem", commonVariables, myList, this );

		// Private constructible variables

		changeKey_ = NULL;

		wordAssignment_ = NULL;
		wordCleanup_ = NULL;
		wordCollection_ = NULL;
		wordQuestion_ = NULL;
		wordSpecification_ = NULL;
		wordType_ = NULL;
		wordWrite_ = NULL;
		wordWriteSentence_ = NULL;
		wordWriteWords_ = NULL;

		// Private loadable variables

		isFeminineWord_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMasculineWord_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_MASCULINE );
		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;

		// Protected constructible variables

		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordCheckedForSolving = false;
		isWordTouchedDuringCurrentSentence = false;

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
		if( ( wordCleanup_ = new WordCleanup( commonVariables_, this ) ) == NULL )
			startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, anyWordTypeString(), "I failed to create my word cleanup module" );
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

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addErrorInItem( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return addErrorInWord( functionNameString, moduleNameString, tempString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addErrorInItem( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	ResultType WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addErrorInItem( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	ResultType WordItem::addErrorInWord( char listChar, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addErrorInItem( listChar, functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startErrorInItem( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	ResultType WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return startErrorInWord( functionNameString, moduleNameString, tempString );
		}

	ResultType WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startErrorInItem( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	ResultType WordItem::startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSystemErrorInItem( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}


	// Protected virtual item functions

	void WordItem::showWords( bool isReturnQueryToPosition )
		{
		if( wordTypeList != NULL &&
		isSelectedByQuery )
			wordTypeList->showWords( isReturnQueryToPosition );
		}

	bool WordItem::hasFoundParameter( unsigned int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				wordParameter_ > NO_WORD_PARAMETER ) );
		}

	bool WordItem::isSorted( Item *nextSortItem )
		{
		// This is a virtual function. Therefore the given variables are unreferenced

		return false;	// Add at the end of the list
		}

	ReferenceResultType WordItem::findMatchingWordReferenceString( char *searchString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( wordTypeList != NULL )
			return wordTypeList->findMatchingWordReferenceString( searchString );

		referenceResult.result = startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );
		return referenceResult;
		}

	ResultType WordItem::checkForUsage()
		{
		if( assignmentList != NULL &&
		assignmentList->checkWordItemForUsage( false, false, this ) == RESULT_OK &&
		assignmentList->checkWordItemForUsage( true, false, this ) == RESULT_OK )
			assignmentList->checkWordItemForUsage( false, true, this );

		if( commonVariables_->result == RESULT_OK &&
		collectionList != NULL )
			collectionList->checkWordItemForUsage( this );

		if( commonVariables_->result == RESULT_OK &&
		contextList != NULL )
			contextList->checkWordItemForUsage( this );

		if( commonVariables_->result == RESULT_OK &&
		generalizationList != NULL )
			generalizationList->checkWordItemForUsage( this );

		if( commonVariables_->result == RESULT_OK &&
		multipleWordList != NULL )
			multipleWordList->checkWordItemForUsage( this );

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->checkWordItemForUsage( false, false, this );

		if( commonVariables_->result == RESULT_OK &&
		commonVariables_->adminConditionList != NULL )
			commonVariables_->adminConditionList->checkWordItemForUsage( this );

		if( commonVariables_->result == RESULT_OK &&
		commonVariables_->adminActionList != NULL )
			commonVariables_->adminActionList->checkWordItemForUsage( this );

		if( commonVariables_->result == RESULT_OK &&
		commonVariables_->adminAlternativeList != NULL )
			commonVariables_->adminAlternativeList->checkWordItemForUsage( this );

		return commonVariables_->result;
		}

	char *WordItem::toString( unsigned short queryWordTypeNr )
		{
		Item::toString( queryWordTypeNr );

		sprintf( tempString, "%c%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_WORD_START_CHAR, wordTypeString( true, queryWordTypeNr ), QUERY_WORD_END_CHAR );
		strcat( commonVariables_->queryString, tempString );

		if( isFeminineWord_ )
			{
			strcat( commonVariables_->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables_->queryString, "isFeminine" );
			}

		if( isMasculineWord_ )
			{
			strcat( commonVariables_->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables_->queryString, "isMasculine" );
			}

		if( isLanguageWord_ )
			{
			strcat( commonVariables_->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables_->queryString, "isLanguageWord" );
			}

		if( isNeedingAuthorizationForChanges() )
			{
			strcat( commonVariables_->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables_->queryString, "isNeedingAuthorizationForChanges" );
			}

		if( wordParameter_ > NO_WORD_PARAMETER )
			{
			sprintf( tempString, "%cwordParameter:%u", QUERY_SEPARATOR_CHAR, wordParameter_ );
			strcat( commonVariables_->queryString, tempString );
			}

		return commonVariables_->queryString;
		}


	// Protected common functions

	bool WordItem::isNeedingAuthorizationForChanges()
		{
		return ( changeKey_ != NULL );
		}

	bool WordItem::hasItems()
		{
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL &&
			wordListArray[wordListNr]->hasItems() )
				return true;
			}

		return false;
		}

	bool WordItem::isAdminWord()
		{
		return ( myList() == NULL );
		}

	bool WordItem::isFeminineWord()
		{
		return isFeminineWord_;
		}

	bool WordItem::isMasculineWord()
		{
		return isMasculineWord_;
		}

	bool WordItem::isFeminineOrMasculineWord()
		{
		return ( isFeminineWord_ ||
				isMasculineWord_ );
		}

	bool WordItem::isAdjectiveAssignedOrClear()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_CLEAR ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	bool WordItem::isAdjectiveClear()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_CLEAR );
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

	bool WordItem::isNounExpert()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_EXPERT );
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

	bool WordItem::isVerbImperativeLogin()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN );
		}

	bool WordItem::isVerbImperativeReadOrShow()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SHOW );
		}

	bool WordItem::isPredefinedWord()
		{
		return ( wordParameter_ > NO_WORD_PARAMETER );
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

		if( ( predefinedNounUserWordItem = commonVariables_->predefinedNounUserWordItem ) != NULL )
			{
			if( ( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem() ) != NULL )
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

	ResultType WordItem::assignChangePermissions( void *changeKey )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignChangePermissions";

		if( changeKey_ == NULL )
			changeKey_ = changeKey;
		else
			{
			if( changeKey_ != changeKey )
				return startErrorInWord( functionNameString, NULL, "The change key is already assigned" );
			}

		return RESULT_OK;
		}

	ResultType WordItem::markWordAsFeminine()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordAsFeminine";

		if( !isMasculineWord_ ||
		hasCorrectedAssumption() )
			{
			if( wordTypeList != NULL &&
			wordTypeList->checkWordTypesOnMasculineParameters() == RESULT_OK )
				{
				isFeminineWord_ = true;
				isMasculineWord_ = false;
				}
			else
				return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );
			}
		else
			return startErrorInWord( functionNameString, NULL, "I am already masculine" );

		return RESULT_OK;
		}

	ResultType WordItem::markWordAsMasculine()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordAsMasculine";

		if( !isFeminineWord_ ||
		hasCorrectedAssumption() )
			{
			if( wordTypeList != NULL &&
			wordTypeList->checkWordTypesOnFeminineParameters() == RESULT_OK )
				{
				isFeminineWord_ = false;
				isMasculineWord_ = true;
				}
			else
				return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );
			}
		else
			return startErrorInWord( functionNameString, NULL, "I am already feminine" );

		return RESULT_OK;
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

			case WORD_INTERFACE_LANGUAGE_LIST:
				return WORD_INTERFACE_LANGUAGE_LIST_SYMBOL;

			case WORD_JUSTIFICATION_LIST:
				return WORD_JUSTIFICATION_LIST_SYMBOL;

			case WORD_MULTIPLE_WORD_LIST:
				return WORD_MULTIPLE_WORD_LIST_SYMBOL;

			case WORD_GRAMMAR_LANGUAGE_LIST:
				return WORD_GRAMMAR_LANGUAGE_LIST_SYMBOL;

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

	char *WordItem::grammarLanguageNameString( unsigned short languageNr )
		{
		if( commonVariables_->predefinedNounGrammarLanguageWordItem != NULL )
			return commonVariables_->predefinedNounGrammarLanguageWordItem->grammarLanguageNameStringInWord( languageNr );

		return NULL;
		}

	char *WordItem::userNameString( unsigned short userNr )
		{
		if( commonVariables_->predefinedNounUserWordItem != NULL )
			return commonVariables_->predefinedNounUserWordItem->userNameStringInWord( userNr );

		return NULL;
		}

	char *WordItem::wordTypeNameString( unsigned short wordTypeNr )
		{
		WordItem *currentGrammarLanguageWordItem;

		if( ( currentGrammarLanguageWordItem = commonVariables_->currentGrammarLanguageWordItem ) != NULL )
			return currentGrammarLanguageWordItem->grammarStringInWord( wordTypeNr );

		return NULL;
		}

	WordItem *WordItem::grammarLanguageWordItem( unsigned short languageNr )
		{
		if( commonVariables_->predefinedNounGrammarLanguageWordItem != NULL )
			return commonVariables_->predefinedNounGrammarLanguageWordItem->grammarLanguageWordItemInWord( languageNr );

		return NULL;
		}

	WordItem *WordItem::nextWordItem()
		{
		return (WordItem *)nextItem;
		}

	WordItem *WordItem::predefinedWordItem( unsigned short wordParameter )
		{
		WordItem *currentWordItem;

		if( wordParameter > NO_WORD_PARAMETER &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( !currentWordItem->isMultipleWord() &&
				currentWordItem->wordParameter() == wordParameter )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
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

	ResultType WordItem::createNewAssignmentLevel()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevel";

		if( wordAssignment_ != NULL ||
		// Create supporting module
		( wordAssignment_ = new WordAssignment( commonVariables_, this ) ) != NULL )
			return wordAssignment_->createNewAssignmentLevel();

		return startErrorInWord( functionNameString, NULL, "I failed to create my word assignment module" );
		}

	ResultType WordItem::deleteAssignmentLevelInWord()
		{
		if( assignmentList != NULL )
			return assignmentList->deleteAssignmentLevelInList();

		return RESULT_OK;
		}

	ResultType WordItem::inactivateActiveAssignment( SpecificationItem *activeAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateActiveAssignment";

		if( wordAssignment_ != NULL )
			return wordAssignment_->inactivateActiveAssignment( activeAssignmentItem );

		return startErrorInWord( functionNameString, NULL, "My word assignment module isn't created yet" );
		}

	ResultType WordItem::archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "archiveInactivateAssignment";

		if( wordAssignment_ != NULL )
			return wordAssignment_->archiveInactiveAssignment( inactiveAssignmentItem );

		return startErrorInWord( functionNameString, NULL, "My word assignment module isn't created yet" );
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

	SpecificationResultType WordItem::assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, void *changeKey )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( isAuthorizedForChanges( changeKey ) )
			{
			if( specificationWordItem == NULL ||
			specificationWordItem->isAuthorizedForChanges( changeKey ) )
				{
				if( wordAssignment_ != NULL ||
				// Create supporting module
				( wordAssignment_ = new WordAssignment( commonVariables_, this ) ) != NULL )
					return wordAssignment_->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString );

				startErrorInWord( functionNameString, NULL, "I failed to create my word assignment module" );
				}
			else
				startErrorInWord( functionNameString, NULL, "You are not authorized to assign the given specification" );
			}
		else
			startErrorInWord( functionNameString, NULL, "You are not authorized to assign this word" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType WordItem::findAssignmentByRelationWord( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;

		if( assignmentList != NULL )
			return assignmentList->findAssignmentItemByRelationWord( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isPossessive, questionParameter, relationWordItem );

		return specificationResult;
		}

	SpecificationItem *WordItem::firstActiveAssignmentItem( unsigned short questionParameter )
		{
		if( assignmentList != NULL )
			return assignmentList->firstAssignmentItem( false, false, false, questionParameter );

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

	void WordItem::deleteRollbackInfo()
		{
		if( wordCleanup_ != NULL )
			wordCleanup_->deleteRollbackInfo();
		}

	void WordItem::deleteTemporaryWriteList()
		{
		if( writeList != NULL )
			writeList->deleteTemporaryList();
		}

	void WordItem::getHighestInUseSentenceNrInWord( bool isIncludingDeletedItems, bool isIncludingLanguageAssignments, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		if( wordCleanup_ != NULL )
			wordCleanup_->getHighestInUseSentenceNr( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, isLanguageWord_, highestSentenceNr );
		}

	void WordItem::setCurrentItemNrInWord()
		{
		if( wordCleanup_ != NULL )
			wordCleanup_->setCurrentItemNr( isLanguageWord_ );
		}

	unsigned int WordItem::highestSentenceNr()
		{
		unsigned int tempSentenceNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL &&
			( tempSentenceNr = wordListArray[wordListNr]->highestSentenceNrInList() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;
			}

		return highestSentenceNr;
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

	ResultType WordItem::deleteSentencesInWord( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentencesInWord";

		if( wordCleanup_ != NULL )
			return wordCleanup_->deleteSentences( isAvailableForRollback, isLanguageWord_, lowestSentenceNr );

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::redoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		if( wordCleanup_ != NULL )
			return wordCleanup_->redoCurrentSentence();

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::removeFirstRangeOfDeletedItems()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItems";

		if( wordCleanup_ != NULL )
			return wordCleanup_->removeFirstRangeOfDeletedItems( isLanguageWord_ );

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word cleanup module" );
		}

	ResultType WordItem::rollbackDeletedRedoInfo()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "rollbackDeletedRedoInfo";

		if( wordCleanup_ != NULL )
			return wordCleanup_->rollbackDeletedRedoInfo();

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

	bool WordItem::isCollectedWithItself()
		{
		if( collectionList != NULL &&
		isNoun() )
			return collectionList->isCollectedWithItself();

		return false;
		}

	bool WordItem::isCompoundCollection( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->isCompoundCollection( collectionNr );

		return false;
		}

	bool WordItem::isCompoundCollection( unsigned int collectionNr, WordItem *commonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->isCompoundCollection( collectionNr, commonWordItem );

		return false;
		}

	bool WordItem::isExclusiveCollection( unsigned int collectionNr )
		{
		if( collectionList != NULL )
			return collectionList->isExclusiveCollection( collectionNr );

		return false;
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
		WordItem *currentWordItem;

		if( collectionNr > NO_COLLECTION_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( tempCollectionOrderNr = currentWordItem->highestCollectionOrderNrInWord( collectionNr ) ) > highestCollectionOrderNr )
					highestCollectionOrderNr = tempCollectionOrderNr;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return highestCollectionOrderNr;
		}

	unsigned int WordItem::collectionNr( unsigned short specificationWordTypeNr )
		{
		unsigned int foundCompoundCollectionNr;

		// Compound collection
		if( ( foundCompoundCollectionNr = compoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
			return foundCompoundCollectionNr;

		// Non-compound collection
		return nonCompoundCollectionNr( specificationWordTypeNr );
		}

	unsigned int WordItem::collectionNr( unsigned short collectionWordTypeNr, WordItem *commonWordItem )
		{
		if( collectionList != NULL )
			return collectionList->collectionNr( collectionWordTypeNr, commonWordItem );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::collectionNrInAllWords( unsigned int contextNr )
		{
		ContextItem *foundContextItem;
		WordItem *currentWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( foundContextItem = currentWordItem->contextItemInWord( contextNr ) ) != NULL )
					return currentWordItem->collectionNr( foundContextItem->contextWordTypeNr(), foundContextItem->specificationWordItem() );
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::collectionNrByCompoundGeneralizationWordInAllWords( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int collectionNr;
		WordItem *currentWordItem;

		if( collectionWordTypeNr > WORD_TYPE_UNDEFINED &&
		compoundGeneralizationWordItem != NULL &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( collectionNr = currentWordItem->collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::compoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		if( collectionList != NULL )
			return collectionList->compoundCollectionNr( collectionWordTypeNr );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::nCollectionsInAllWords( unsigned int collectionNr )
		{
		unsigned int nCollections = 0;
		WordItem *currentWordItem;

		if( collectionNr > NO_COLLECTION_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( currentWordItem->hasCollectionNr( collectionNr ) )
					nCollections++;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return nCollections;
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
		WordItem *currentWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( nonCompoundCollectionNr = currentWordItem->nonCompoundCollectionNrInWord( compoundCollectionNr ) ) > NO_COLLECTION_NR )
					return nonCompoundCollectionNr;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::highestCollectionNrInAllWords()
		{
		unsigned int tempCollectionNr;
		unsigned int highestCollectionNr = NO_COLLECTION_NR;
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( tempCollectionNr = currentWordItem->highestCollectionNrInWord() ) > highestCollectionNr )
					highestCollectionNr = tempCollectionNr;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return highestCollectionNr;
		}

	CollectionResultType WordItem::addCollection( bool isExclusiveSpecification, bool isSpecificationGeneralization, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem, char *collectionString )
		{
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollection";

		if( wordCollection_ != NULL ||
		// Create supporting module
		( wordCollection_ = new WordCollection( commonVariables_, this ) ) != NULL )
			return wordCollection_->addCollection( isExclusiveSpecification, isSpecificationGeneralization, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, collectionString );

		collectionResult.result = startErrorInWord( functionNameString, NULL, "I failed to create my word collection module" );
		return collectionResult;
		}

	CollectionResultType WordItem::addCollectionByExclusiveSpecification( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollectionByExclusiveSpecification";

		if( wordCollection_ != NULL ||
		// Create supporting module
		( wordCollection_ = new WordCollection( commonVariables_, this ) ) != NULL )
			return wordCollection_->addCollectionByExclusiveSpecification( isExclusiveSpecification, collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );

		collectionResult.result = startErrorInWord( functionNameString, NULL, "I failed to create my word collection module" );
		return collectionResult;
		}

	CollectionResultType WordItem::addCollectionByGeneralization( bool isExclusiveSpecification, bool isExclusiveGeneralization, bool isQuestion, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollectionByGeneralization";

		if( wordCollection_ != NULL ||
		// Create supporting module
		( wordCollection_ = new WordCollection( commonVariables_, this ) ) != NULL )
			return wordCollection_->addCollectionByGeneralization( isExclusiveSpecification, isExclusiveGeneralization, isQuestion, collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem );

		collectionResult.result = startErrorInWord( functionNameString, NULL, "I failed to create my word collection module" );
		return collectionResult;
		}

	CollectionResultType WordItem::findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionResultType collectionResult;

		if( collectionList != NULL )
			return collectionList->findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return collectionResult;
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

	void WordItem::clearGeneralizationContextWriteLevelInWord( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		if( contextList != NULL )
			contextList->clearGeneralizationContextWriteLevel( currentWriteLevel, contextNr );
		}

	void WordItem::clearSpecificationContextWriteLevelInWord( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		if( contextList != NULL )
			contextList->clearSpecificationContextWriteLevel( currentWriteLevel, contextNr );
		}

	void WordItem::clearRelationContextWriteLevelInWord( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		if( contextList != NULL )
			contextList->clearRelationContextWriteLevel( currentWriteLevel, contextNr );
		}

	bool WordItem::hasContextCurrentlyBeenUpdatedInAllWords( unsigned int contextNr, WordItem *specificationWordItem )
		{
		WordItem *currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( currentWordItem->hasContextCurrentlyBeenUpdatedInWord( contextNr, specificationWordItem ) )
					return true;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return false;
		}

	bool WordItem::hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->hasContext( contextNr, specificationWordItem );

		return false;
		}

	bool WordItem::isContextSimilarInAllWords( unsigned int firstContextNr, unsigned int secondContextNr )
		{
		WordItem *currentWordItem;

		if( firstContextNr > NO_CONTEXT_NR &&
		secondContextNr > NO_CONTEXT_NR &&
		firstContextNr != secondContextNr &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( !currentWordItem->isContextSimilarInWord( firstContextNr, secondContextNr ) )
					return false;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return true;
		}

	bool WordItem::isContextSubsetInAllWords( unsigned int fullSetContextNr, unsigned int subsetContextNr )
		{
		WordItem *currentWordItem;

		if( fullSetContextNr > NO_CONTEXT_NR &&
		subsetContextNr > NO_CONTEXT_NR )
			{
			if( fullSetContextNr == subsetContextNr )
				return true;

			if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
				{
				// Do for all words
				do	{
					if( currentWordItem->isContextSubsetInWord( fullSetContextNr, subsetContextNr ) )
						return true;
					}
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
				}
			}

		return false;
		}

	unsigned short WordItem::contextWordTypeNrInWord( unsigned int contextNr )
		{
		ContextItem *foundContextItem;

		if( ( foundContextItem = contextItemInWord( contextNr ) ) != NULL )
			return foundContextItem->contextWordTypeNr();

		return WORD_TYPE_UNDEFINED;
		}

	unsigned int WordItem::contextNrInWord( WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextNr( specificationWordItem );

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::contextNrInWord( unsigned int nContextWords, WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextNr( nContextWords, specificationWordItem );

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
		WordItem *currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( currentWordItem->hasContextInWord( contextNr, specificationWordItem ) )
					nContextWords++;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return nContextWords;
		}

	ResultType WordItem::addContext( bool isQuestion, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addContext";

		if( !isAdminWord() )
			{
			if( contextList == NULL )
				{
				// Create list
				if( ( contextList = new ContextList( commonVariables_, this ) ) != NULL )
					wordListArray[WORD_CONTEXT_LIST] = contextList;
				else
					return startErrorInWord( functionNameString, NULL, "I failed to create a context list" );
				}

			return contextList->addContext( isQuestion, contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem );
			}

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have context" );
		}

	ContextItem *WordItem::firstActiveContextItem()
		{
		if( contextList != NULL )
			return contextList->firstActiveContextItem();

		return NULL;
		}

	ContextItem *WordItem::contextItemInWord( unsigned int contextNr )
		{
		if( contextList != NULL )
			return contextList->contextItem( contextNr );

		return NULL;
		}

	ContextItem *WordItem::contextItemInWord( WordItem *specificationWordItem )
		{
		if( contextList != NULL )
			return contextList->contextItem( specificationWordItem );

		return NULL;
		}

	WordItem *WordItem::contextWordItemInAllWords( unsigned int contextNr, WordItem *specificationWordItem, WordItem *previousWordItem )
		{
		WordItem *currentWordItem;

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR &&
		// Do in all (remaining) words
		( currentWordItem = ( previousWordItem == NULL ? commonVariables_->firstWordItem : previousWordItem->nextWordItem() ) ) != NULL )
			{
			do	{
				if( currentWordItem->hasContextInWord( contextNr, specificationWordItem ) )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NULL;
		}


	// Protected database connection functions
/*
	ResultType WordItem::storeWordItemInFutureDatabase()
		{
		// Not fully implemented yet
		if( assignmentList != NULL )
			return assignmentList->storeChangesInFutureDatabase();

		if( collectionList != NULL )
			return collectionList->storeChangesInFutureDatabase();

		if( contextList != NULL )
			return contextList->storeChangesInFutureDatabase();

		if( generalizationList != NULL )
			return generalizationList->storeChangesInFutureDatabase();

		if( grammarList != NULL )
			return grammarList->storeChangesInFutureDatabase();

		if( interfaceList != NULL )
			return interfaceList->storeChangesInFutureDatabase();

		if( justificationList != NULL )
			return justificationList->storeChangesInFutureDatabase();

		if( multipleWordList != NULL )
			return multipleWordList->storeChangesInFutureDatabase();

		if( specificationList != NULL )
			return specificationList->storeChangesInFutureDatabase();

		if( wordTypeList != NULL )
			return wordTypeList->storeChangesInFutureDatabase();

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

	ResultType WordItem::createGeneralization( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGeneralization";

		if( !isAdminWord() )
			{
			if( generalizationList == NULL )
				{
				// Create list
				if( ( generalizationList = new GeneralizationList( commonVariables_, this ) ) != NULL )
					wordListArray[WORD_GENERALIZATION_LIST] = generalizationList;
				else
					return startErrorInWord( functionNameString, NULL, "I failed to create a generalization list" );
				}

			return generalizationList->createGeneralization( isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem );
			}

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have generalizations" );
		}

	GeneralizationItem *WordItem::firstGeneralizationItem()
		{
		if( generalizationList != NULL )
			return generalizationList->firstActiveGeneralizationItem();

		return NULL;
		}

	GeneralizationItem *WordItem::firstSpecificationGeneralizationItem()
		{
		if( generalizationList != NULL )
			return generalizationList->firstGeneralizationItem( false );

		return NULL;
		}

	GeneralizationItem *WordItem::firstRelationGeneralizationItem()
		{
		if( generalizationList != NULL )
			return generalizationList->firstGeneralizationItem( true );

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

	bool WordItem::isCheckingGrammarNeeded()
		{
		if( grammarList != NULL )
			return grammarList->isCheckingGrammarNeeded();

		return false;
		}

	bool WordItem::isGrammarLanguageWord()
		{
		return ( grammarList != NULL );
		}

	bool WordItem::isLanguageWord()
		{
		return isLanguageWord_;
		}

	unsigned short WordItem::guideByGrammarStringWordTypeNr( char *guideByGrammarString )
		{
		if( grammarList != NULL )
			return grammarList->guideByGrammarStringWordTypeNr( guideByGrammarString );

		return WORD_TYPE_UNDEFINED;
		}

	unsigned short WordItem::nGrammarLanguages()
		{
		unsigned short nLanguages = 0;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;
		WordItem *predefinedNounGrammarLanguageWordItem;

		if( ( predefinedNounGrammarLanguageWordItem = commonVariables_->predefinedNounGrammarLanguageWordItem ) != NULL )
			{
			if( ( currentGeneralizationItem = predefinedNounGrammarLanguageWordItem->firstSpecificationGeneralizationItem() ) != NULL )
				{
				do	{
					if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
						{
						if( generalizationWordItem->isGrammarLanguageWord() )
							nLanguages++;
						}
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return nLanguages;
		}

	GrammarResultType WordItem::checkOnWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString )
		{
		GrammarResultType grammarResult;

		if( grammarList != NULL )
			return grammarList->checkOnWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return grammarResult;
		}

	GrammarResultType WordItem::createGrammar( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		GrammarResultType grammarResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGrammar";

		if( !isAdminWord() )
			{
			if( grammarList == NULL )
				{
				if( ( grammarList = new GrammarList( commonVariables_, this ) ) != NULL )
					wordListArray[WORD_GRAMMAR_LANGUAGE_LIST] = grammarList;
				else
					{
					grammarResult.result = startErrorInWord( functionNameString, NULL, "I failed to create a grammar list" );
					return grammarResult;
					}
				}

			return grammarList->createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );
			}

		grammarResult.result = startErrorInWord( functionNameString, NULL, "The admin word item cannot have grammar" );
		return grammarResult;
		}

	GrammarResultType WordItem::findGrammar( bool ignoreValue, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		GrammarResultType grammarResult;

		if( grammarList != NULL )
			return grammarList->findGrammarItem( ignoreValue, grammarParameter, grammarStringLength, grammarString );

		return grammarResult;
		}

	GrammarResultType WordItem::checkForDuplicateGrammarDefinition()
		{
		GrammarResultType grammarResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateGrammarDefinition";

		if( grammarList != NULL )
			return grammarList->checkForDuplicateGrammarDefinition();

		grammarResult.result = startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );
		return grammarResult;
		}

	ResultType WordItem::checkGrammar()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammar";

		if( grammarList != NULL )
			return grammarList->checkGrammar();

		return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );
		}

	ResultType WordItem::checkGrammarForUsageInWord( GrammarItem *unusedGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammarForUsageInWord";

		if( grammarList != NULL )
			{
			if( grammarList->checkGrammarItemForUsage( unusedGrammarItem ) == RESULT_OK )
				{
				if( writeList != NULL )
					return writeList->checkGrammarItemForUsage( unusedGrammarItem );
				}
			}
		else
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return RESULT_OK;
		}

	ResultType WordItem::linkLaterDefinedGrammarWords()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "linkLaterDefinedGrammarWords";

		if( grammarList != NULL )
			return grammarList->linkLaterDefinedGrammarWords();

		return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );
		}

	GrammarItem *WordItem::startOfGrammarItem()
		{
		if( grammarList != NULL )
			return grammarList->startOfGrammarItem();

		return NULL;
		}


	// Protected interface functions

	bool WordItem::isInterfaceLanguageWord()
		{
		return ( interfaceList != NULL );
		}

	ResultType WordItem::checkInterface( unsigned short interfaceParameter, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkInterface";

		if( !isAdminWord() )
			{
			if( interfaceList == NULL )
				{
				// Create list
				if( ( interfaceList = new InterfaceList( commonVariables_, this ) ) != NULL )
					wordListArray[WORD_INTERFACE_LANGUAGE_LIST] = interfaceList;
				else
					return startErrorInWord( functionNameString, NULL, "I failed to create an interface list" );
				}

			return interfaceList->checkInterface( interfaceParameter, interfaceString );
			}

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have interfacing" );
		}

	ResultType WordItem::createInterface( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createInterface";

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

	JustificationResultType WordItem::addJustification( bool isForcingNewJustification, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addJustification";

		if( !isAdminWord() )
			{
			if( justificationList == NULL )
				{
				if( ( justificationList = new JustificationList( commonVariables_, myWordItem() ) ) != NULL )
					wordListArray[WORD_JUSTIFICATION_LIST] = justificationList;
				else
					{
					justificationResult.result = startErrorInWord( functionNameString, NULL, "I failed to create a justification list" );
					return justificationResult;
					}
				}

			return justificationList->addJustification( true, isForcingNewJustification, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem );
			}

		justificationResult.result = startErrorInWord( functionNameString, NULL, "The admin word item cannot have justification" );
		return justificationResult;
		}

	JustificationResultType WordItem::copyJustification( bool isForcingNewJustification, SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyJustification";

		if( !isAdminWord() )
			{
			if( justificationList != NULL )
				return justificationList->copyJustification( isForcingNewJustification, newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );

			justificationResult.result = startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
			}
		else
			justificationResult.result = startErrorInWord( functionNameString, NULL, "The admin word item cannot have justification" );

		return justificationResult;
		}

	ResultType WordItem::attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustification";

		if( justificationList != NULL )
			return justificationList->attachJustification( attachJustificationItem, involvedSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
		}

	ResultType WordItem::checkForConfirmedJustifications( SpecificationItem *secondarySpecificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForConfirmedJustifications";

		if( justificationList != NULL )
			return justificationList->checkForConfirmedJustifications( secondarySpecificationItem, involvedSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
		}

	ResultType WordItem::checkForUnreferencedJustifications()
		{
		if( justificationList != NULL )
			return justificationList->checkForUnreferencedJustifications();

		return RESULT_OK;
		}

	ResultType WordItem::checkJustificationsForReplacedSpecifications()
		{
		if( justificationList != NULL )
			return justificationList->checkForReplacedOrDeletedSpecifications();

		return RESULT_OK;
		}

	ResultType WordItem::checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationForUsageOfInvolvedWords";

		if( justificationList != NULL )
			justificationList->checkSpecificationItemForUsage( unusedSpecificationItem );

		if( commonVariables_->result == RESULT_OK )
			{
			if( checkSpecificationForUsageInWord( unusedSpecificationItem ) == RESULT_OK )
				{
				if( ( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
							currentGeneralizationWordItem->checkSpecificationForUsageInWord( unusedSpecificationItem );
						else
							return startErrorInWord( functionNameString, NULL, "I found an undefined generalization word" );
						}
					while( commonVariables_->result == RESULT_OK &&
					( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::replaceJustification( bool hasConfirmedSpecification, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustification";

		if( justificationList != NULL )
			return justificationList->replaceJustification( hasConfirmedSpecification, false, false, obsoleteJustificationItem, replacingJustificationItem, involvedSpecificationItem, NULL );

		return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
		}

	ResultType WordItem::replaceObsoleteJustificationsOfAdjustedQuestion( JustificationItem *replacingJustificationItem, SpecificationItem *adjustedQuestionSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceObsoleteJustificationsOfAdjustedQuestion";

		if( justificationList != NULL )
			return justificationList->replaceObsoleteJustificationsOfAdjustedQuestion( replacingJustificationItem, adjustedQuestionSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
		}

	ResultType WordItem::replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteJustification";

		if( justificationList != NULL )
			return justificationList->replaceOrDeleteJustification( obsoleteJustificationItem );

		return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
		}

	ResultType WordItem::updateSpecificationsInJustificationInWord( bool hasConfirmedSpecificationInMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		if( justificationList != NULL &&
		justificationList->updateSpecificationsInJustifications( hasConfirmedSpecificationInMainWord, true, obsoleteSpecificationItem, replacingSpecificationItem ) == RESULT_OK )
			{
			if( obsoleteSpecificationItem != NULL &&
			obsoleteSpecificationItem->isDeletedItem() )
				return justificationList->updateSpecificationsInJustifications( hasConfirmedSpecificationInMainWord, false, obsoleteSpecificationItem, replacingSpecificationItem );
			}

		return commonVariables_->result;
		}

	ResultType WordItem::writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelatedJustificationSpecifications";

		if( justificationList != NULL )
			return justificationList->writeRelatedJustificationSpecifications( justificationTypeNr, specificationCollectionNr );

		return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );
		}

	JustificationItem *WordItem::negativeAssumptionOrConclusionJustificationItem( SpecificationItem *anotherPrimarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::obsoleteAssumptionJustificationItem()
		{
		if( justificationList != NULL )
			return justificationList->obsoleteAssumptionJustificationItem();

		return NULL;
		}

	JustificationItem *WordItem::oldCorrectedAssumptionByOppositeQuestionJustificationItem()
		{
		if( justificationList != NULL )
			return justificationList->oldCorrectedAssumptionByOppositeQuestionJustificationItem();

		return NULL;
		}

	JustificationItem *WordItem::secondarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->secondarySpecificationJustificationItem( justificationTypeNr, secondarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem *secondarySpecificationItem )
		{
		if( justificationList != NULL )
			return justificationList->specificationSubstitutionPartOfAssumptionJustificationItem( secondarySpecificationItem );

		return NULL;
		}

	SpecificationItem *WordItem::oldSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		if( justificationList != NULL )
			return justificationList->oldSuggestiveQuestionAssumptionSecondarySpecificationItem();

		return NULL;
		}


	// Protected multiple word functions

	bool WordItem::isMultipleWord()
		{
		return ( ( multipleWordList != NULL &&
				multipleWordList->hasItems() ) ||

				( wordTypeList != NULL &&
				wordTypeList->hasMultipleWordType() ) );
		}

	bool WordItem::isMultipleNounWordStartingWith( char *sentenceString )
		{
		if( multipleWordList != NULL )
			return multipleWordList->isMultipleNounWordStartingWith( sentenceString );

		return false;
		}

	ResultType WordItem::addMultipleWord( unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( !isAdminWord() )
			{
			if( multipleWordList == NULL )
				{
				// Create list
				if( ( multipleWordList = new MultipleWordList( commonVariables_, this ) ) != NULL )
					wordListArray[WORD_MULTIPLE_WORD_LIST] = multipleWordList;
				else
					return startErrorInWord( functionNameString, NULL, "I failed to create a multiple word list" );
				}

			return multipleWordList->addMultipleWord( wordTypeNr, multipleWordItem );
			}

		return startErrorInWord( functionNameString, NULL, "The admin word item cannot have multiple words" );
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

	ResultType WordItem::itemQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->itemQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to query items" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::listQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *queryListString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->listQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListString ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to do a list query list" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::wordTypeQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned short queryWordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->wordTypeQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to query word types" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::parameterQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned int queryParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->parameterQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to query parameters" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::wordQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordNameString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInWord";

		if( ( referenceResult = findMatchingWordReferenceString( wordNameString ) ).result == RESULT_OK )
			{
			if( referenceResult.hasFoundMatchingStrings )
				{
				if( isSelectOnFind &&
				!isSelectedByQuery )
					{
					isSelectedByQuery = true;
					commonVariables_->hasFoundQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				isSelectedByQuery )
					isSelectedByQuery = false;
				}

			for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
				{
				if( wordListArray[wordListNr] != NULL )
					{
					if( wordListArray[wordListNr]->wordQueryInList( ( isSelectOnFind && referenceResult.hasFoundMatchingStrings ), isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems ) != RESULT_OK )
						return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to query word items" );
					}
				}
			}
		else
			return addErrorInWord( functionNameString, NULL, "I failed to find words" );

		return RESULT_OK;
		}

	ResultType WordItem::wordReferenceQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordReferenceNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->wordReferenceQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to query word references" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::stringQueryInWord( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->stringQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to query strings" );
				}
			}

		return RESULT_OK;
		}

	ResultType WordItem::showQueryResultInWord( bool showOnlyWords, bool showOnlyWordReferences, bool showOnlyStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResultInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListArray[wordListNr] != NULL )
				{
				if( wordListArray[wordListNr]->showQueryResultInList( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
					return addErrorInWord( wordListChar( wordListNr ), functionNameString, NULL, "I failed to show the items" );
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

		if( wordQuestion_ != NULL ||
		// Create supporting module
		( wordQuestion_ = new WordQuestion( commonVariables_, this ) ) != NULL )
			return wordQuestion_->findAnswerToNewUserQuestion();

		return startErrorInWord( functionNameString, NULL, "I failed to create my word question module" );
		}

	ResultType WordItem::findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswersToQuestions";

		if( wordQuestion_ != NULL ||
		// Create supporting module
		( wordQuestion_ = new WordQuestion( commonVariables_, this ) ) != NULL )
			return wordQuestion_->findAnswersToQuestions( compoundSpecificationCollectionNr, answerSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word question module" );
		}

	ResultType WordItem::writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeAnswerToQuestion";

		if( wordQuestion_ != NULL )
			return wordQuestion_->writeAnswerToQuestion( true, isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "My word question module isn't created yet" );
		}

	SpecificationResultType WordItem::findQuestionToBeAdjustedByCompoundCollection( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCompoundCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( wordQuestion_ != NULL )
			return wordQuestion_->findQuestionToBeAdjustedByCompoundCollection( isNegative, isPossessive, questionParameter, specificationCompoundCollectionNr, relationContextNr, specificationWordItem );

		specificationResult.result = startErrorInWord( functionNameString, NULL, "My word question module isn't created yet" );
		return specificationResult;
		}


	// Protected selection functions

	ResultType WordItem::checkSelectionForUsageInWord( SelectionItem *unusedSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionForUsageInWord";

		if( commonVariables_->adminScoreList != NULL )
			{
			if( commonVariables_->adminScoreList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin score list for selection item usage" );
			}

		if( commonVariables_->adminConditionList != NULL )
			{
			if( commonVariables_->adminConditionList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin condition list for selection item usage" );
			}

		if( commonVariables_->adminActionList != NULL )
			{
			if( commonVariables_->adminActionList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin action list for selection item usage" );
			}

		if( commonVariables_->adminAlternativeList != NULL )
			{
			if( commonVariables_->adminAlternativeList->checkSelectionItemForUsage( unusedSelectionItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to check the admin alternative list for selection item usage" );
			}

		return RESULT_OK;
		}


	// Protected specification functions

	void WordItem::initializeVariablesInWord()
		{
		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;

		isWordTouchedDuringCurrentSentence = false;

		if( justificationList != NULL )
			justificationList->initializeVariables();

		if( assignmentList != NULL )
			{
			assignmentList->initializeVariables( false, false );
			assignmentList->initializeVariables( true, false );
			assignmentList->initializeVariables( false, true );
			}

		if( specificationList != NULL )
			specificationList->initializeVariables( false, false );

		if( wordQuestion_ != NULL )
			wordQuestion_->clearFoundAnswerToQuestion();

		if( wordSpecification_ != NULL )
			wordSpecification_->initializeWordSpecificationVariables();
		}

	bool WordItem::hasAnsweredQuestion( unsigned int secondarySpecificationCollectionNr )
		{
		if( specificationList != NULL )
			return specificationList->hasAnsweredQuestion( secondarySpecificationCollectionNr );

		return false;
		}

	bool WordItem::hasConfirmedSpecification()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasConfirmedSpecification();

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

	bool WordItem::isAuthorizedForChanges( void *changeKey )
		{
		return ( changeKey_ == NULL ||
				changeKey_ == changeKey );
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

	unsigned int WordItem::userSpecificationCollectionNr()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->userSpecificationCollectionNr();

		return NO_COLLECTION_NR;
		}

	ResultType WordItem::changeJustificationOfNegativeAssumptions( SpecificationItem *secondarySpecificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem ) == RESULT_OK &&
		assignmentList->changeJustificationOfNegativeAssumptions( true, false, secondarySpecificationItem ) == RESULT_OK )
			assignmentList->changeJustificationOfNegativeAssumptions( false, true, secondarySpecificationItem );

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL )
			return specificationList->changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem );

		return commonVariables_->result;
		}

	ResultType WordItem::checkForSpecificationConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForSpecificationConflict";

		if( wordSpecification_ != NULL )
			return wordSpecification_->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString );

		return startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );
		}

	ResultType WordItem::checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->checkJustificationForUsage( false, false, false, unusedJustificationItem ) == RESULT_OK &&
		assignmentList->checkJustificationForUsage( true, false, false, unusedJustificationItem ) == RESULT_OK &&
		assignmentList->checkJustificationForUsage( false, true, false, unusedJustificationItem ) == RESULT_OK )
			assignmentList->checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL &&
		specificationList->checkJustificationForUsage( false, false, false, unusedJustificationItem ) == RESULT_OK )
			specificationList->checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( commonVariables_->result == RESULT_OK &&
		justificationList != NULL )
			justificationList->checkJustificationForUsage( unusedJustificationItem );

		// Clear error to be able to restart after justification error
		return RESULT_OK;
		}

	ResultType WordItem::checkSpecificationsForReplacedOrDeletedJustifications()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationsForReplacedOrDeletedJustifications";

		if( assignmentList != NULL )
			{
			if( !assignmentList->hasFoundReplacedOrDeletedJustification( false, false ) )
				{
				if( !assignmentList->hasFoundReplacedOrDeletedJustification( false, false ) )
					{
					if( !assignmentList->hasFoundReplacedOrDeletedJustification( false, true ) )
						{
						if( assignmentList->hasFoundDeletedJustificationInReplacedSpecification() )
							return startErrorInWord( functionNameString, NULL, "I found a deleted justification in one of my replaced assignments" );
						}
					else
						return startErrorInWord( functionNameString, NULL, "I found a replaced or deleted justification in one of my archived assignments" );
					}
				else
					return startErrorInWord( functionNameString, NULL, "I found a replaced or deleted justification in one of my inactive assignments" );
				}
			else
				return startErrorInWord( functionNameString, NULL, "I found a replaced or deleted justification in one of my active assignments" );
			}

		if( specificationList != NULL )
			{
			if( !specificationList->hasFoundReplacedOrDeletedJustification( false, false ) )
				{
				if( specificationList->hasFoundDeletedJustificationInReplacedSpecification() )
					return startErrorInWord( functionNameString, NULL, "I found a deleted justification in one of my replaced specifications" );
				}
			else
				return startErrorInWord( functionNameString, NULL, "I found a replaced or deleted justification in one of my specifications" );
			}

		return RESULT_OK;
		}

	ResultType WordItem::collectGeneralizationsOrSpecifications( bool isExclusiveSpecification, bool isGeneralizationCollection, bool isQuestion, unsigned int collectionNr )
		{
		if( assignmentList != NULL &&
		assignmentList->collectGeneralizationsOrSpecifications( false, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr ) == RESULT_OK &&
		assignmentList->collectGeneralizationsOrSpecifications( true, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr ) == RESULT_OK )
			assignmentList->collectGeneralizationsOrSpecifications( false, true, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr );

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL )
			return specificationList->collectGeneralizationsOrSpecifications( false, false, isExclusiveSpecification, isGeneralizationCollection, isQuestion, collectionNr );

		return commonVariables_->result;
		}

	ResultType WordItem::confirmSpecificationButNoRelation( SpecificationItem *confirmedSpecificationItem, SpecificationItem *confirmationSpecificationItem )
		{
		if( assignmentList != NULL &&
		assignmentList->confirmSpecificationButNoRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem ) == RESULT_OK &&
		assignmentList->confirmSpecificationButNoRelation( true, false, confirmedSpecificationItem, confirmationSpecificationItem ) == RESULT_OK )
			assignmentList->confirmSpecificationButNoRelation( false, true, confirmedSpecificationItem, confirmationSpecificationItem );

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL )
			specificationList->confirmSpecificationButNoRelation( false, false, confirmedSpecificationItem, confirmationSpecificationItem );

		return commonVariables_->result;
		}

	ResultType WordItem::recalculateAssumptions()
		{
		if( assignmentList != NULL &&
		assignmentList->recalculateAssumptions( false, false ) == RESULT_OK &&
		assignmentList->recalculateAssumptions( true, false ) == RESULT_OK )
			assignmentList->recalculateAssumptions( false, true );

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL )
			return specificationList->recalculateAssumptions( false, false );

		return commonVariables_->result;
		}

	ResultType WordItem::replaceOlderSpecificationByConcludedAssumption( unsigned int oldRelationContextNr, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOlderSpecificationByConcludedAssumption";

		if( specificationList != NULL )
			return specificationList->replaceOlderSpecificationByConcludedAssumption( oldRelationContextNr, replacingSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "The specification list isn't created yet" );
		}

	ResultType WordItem::replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem != NULL )
			{
			if( obsoleteSpecificationItem->isAssignment() )
				{
				if( assignmentList != NULL )
					return assignmentList->replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );

				return startErrorInWord( functionNameString, NULL, "The assignment list isn't created yet" );
				}

			if( specificationList != NULL )
				return specificationList->replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );

			return startErrorInWord( functionNameString, NULL, "The specification list isn't created yet" );
			}

		return startErrorInWord( functionNameString, NULL, "The given obsolete specification item is undefined" );
		}

	ResultType WordItem::updateJustificationInSpecifications( bool hasConfirmedSpecification, bool isExclusiveSpecification, bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		bool isIncludingReplacedItems = ( obsoleteJustificationItem != NULL &&
										obsoleteJustificationItem->hasCurrentCreationSentenceNr() );

		if( assignmentList != NULL &&
		assignmentList->updateJustificationInSpecifications( hasConfirmedSpecification, false, false, false, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList->updateJustificationInSpecifications( hasConfirmedSpecification, true, false, false, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList->updateJustificationInSpecifications( hasConfirmedSpecification, false, true, false, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			{
			if( isIncludingReplacedItems )
				assignmentList->updateJustificationInSpecifications( hasConfirmedSpecification, false, false, true, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		if( commonVariables_->result == RESULT_OK &&
		specificationList != NULL &&
		specificationList->updateJustificationInSpecifications( hasConfirmedSpecification, false, false, false, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			{
			if( isIncludingReplacedItems )
				return specificationList->updateJustificationInSpecifications( hasConfirmedSpecification, false, false, true, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		return commonVariables_->result;
		}

	ResultType WordItem::updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool hasConfirmedSpecificationInMainWord = hasConfirmedSpecification();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateSpecificationItemsInJustificationOfInvolvedWords";

		// Update specifications in justifications of this word
		if( updateSpecificationsInJustificationInWord( hasConfirmedSpecificationInMainWord, obsoleteSpecificationItem, replacingSpecificationItem ) == RESULT_OK )
			{
			if( ( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
				{
				// Update specifications in justifications of involved words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
						{
						if( currentGeneralizationWordItem->updateSpecificationsInJustificationInWord( hasConfirmedSpecificationInMainWord, obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
							return addErrorInWord( functionNameString, NULL, "I failed to update the justification items in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						return startErrorInWord( functionNameString, NULL, "I found an undefined generalization word" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
				}
			}
		else
			return addErrorInWord( functionNameString, NULL, "I failed to update my justification items" );

		return RESULT_OK;
		}

	SpecificationResultType WordItem::addSpecification( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, bool isUserSentenceMixOfFeminineAndMasculineSpecificationWords, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, void *changeKey )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( isAuthorizedForChanges( changeKey ) )
			{
			if( specificationWordItem == NULL ||
			specificationWordItem->isAuthorizedForChanges( changeKey ) )
				{
				if( wordSpecification_ != NULL ||
				// Create supporting module
				( wordSpecification_ = new WordSpecification( commonVariables_, this ) ) != NULL )
					return wordSpecification_->addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, isUserSentenceMixOfFeminineAndMasculineSpecificationWords, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );

				startErrorInWord( functionNameString, NULL, "I failed to create my word specification module" );
				}
			else
				startErrorInWord( functionNameString, NULL, "You are not authorized to add the given specification" );
			}
		else
			startErrorInWord( functionNameString, NULL, "You are not authorized to add this word" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType WordItem::copySpecification( bool isNewInactiveAssignment, bool isNewArchivedAssignment, bool isNewAnsweredQuestion, bool isNewExclusiveSpecification, unsigned short newAssignmentLevel, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, unsigned int newRelationContextNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copySpecification";

		if( !isAdminWord() )
			{
			if( originalSpecificationItem != NULL )
				{
				if( originalSpecificationItem->isAssignment() )
					{
					if( assignmentList != NULL )
						return assignmentList->copySpecification( isNewInactiveAssignment, isNewArchivedAssignment, isNewAnsweredQuestion, isNewExclusiveSpecification, newAssignmentLevel, newGeneralizationCollectionNr, newSpecificationCollectionNr, newRelationContextNr, newFirstJustificationItem, originalSpecificationItem );

					startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );
					}
				else
					{
					if( specificationList != NULL )
						return specificationList->copySpecification( isNewInactiveAssignment, isNewArchivedAssignment, isNewAnsweredQuestion, isNewExclusiveSpecification, newAssignmentLevel, newGeneralizationCollectionNr, newSpecificationCollectionNr, newRelationContextNr, newFirstJustificationItem, originalSpecificationItem );

					startErrorInWord( functionNameString, NULL, "My specification list isn't created yet" );
					}
				}
			else
				startErrorInWord( functionNameString, NULL, "The given original specification item is undefined" );
			}
		else
			startErrorInWord( functionNameString, NULL, "The admin word item cannot have specifications" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType WordItem::createSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short grammarLanguageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *writtenSentenceString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSpecification";

		if( !isAdminWord() )
			{
			if( isAssignment )
				{
				if( assignmentList == NULL )
					{
					// Create assignment list
					if( ( assignmentList = new SpecificationList( WORD_ASSIGNMENT_LIST_SYMBOL, commonVariables_, this ) ) != NULL )
						wordListArray[WORD_ASSIGNMENT_LIST] = assignmentList;
					else
						startErrorInWord( functionNameString, NULL, "I failed to create an assignment list" );
					}

				if( commonVariables_->result == RESULT_OK )
					return assignmentList->createSpecification( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assignmentLevel, assumptionLevel, grammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceString );
				}
			else
				{
				if( specificationList == NULL )
					{
					// Create specification list
					if( ( specificationList = new SpecificationList( WORD_SPECIFICATION_LIST_SYMBOL, commonVariables_, this ) ) != NULL )
						wordListArray[WORD_SPECIFICATION_LIST] = specificationList;
					else
						startErrorInWord( functionNameString, NULL, "I failed to create a specification list" );
					}

				if( commonVariables_->result == RESULT_OK )
					return specificationList->createSpecification( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assignmentLevel, assumptionLevel, grammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, writtenSentenceString );
				}
			}
		else
			startErrorInWord( functionNameString, NULL, "The admin word item cannot have specifications" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType WordItem::findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ != NULL )
			return wordSpecification_->findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );

		specificationResult.result = startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );
		return specificationResult;
		}

	SpecificationResultType WordItem::findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ != NULL )
			return wordSpecification_->findRelatedSpecification( false, false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString );

		specificationResult.result = startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );
		return specificationResult;
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

	SpecificationItem *WordItem::firstActiveAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingAssignments &&
		assignmentList != NULL )
			foundSpecificationItem = assignmentList->firstSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, specificationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, questionParameter, generalizationContextNr, specificationContextNr, specificationWordItem );

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

	SpecificationItem *WordItem::firstNonQuestionAssignmentOrSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList->firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList->firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstNonQuestionAssignmentOrSpecificationItem( isAllowingEmptyContextResult, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
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

	SpecificationItem *WordItem::firstRelationSpecificationItem( bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		if( specificationList != NULL )
			return specificationList->firstRelationSpecificationItem( isNegative, isPossessive, questionParameter, specificationWordItem );

		return NULL;
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

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList != NULL &&
		( foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem ) ) == NULL )
			{
			if( isIncludingArchivedAssignments )
				foundSpecificationItem = assignmentList->firstSelfGeneratedCheckSpecificationItem( true, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList != NULL )
			return specificationList->firstSelfGeneratedCheckSpecificationItem( false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem );

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

	SpecificationItem *WordItem::firstSpecificationItem( bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		if( specificationList != NULL )
			return specificationList->firstSpecificationItem( false, isPossessive, questionParameter, specificationWordItem );

		return NULL;
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

	void WordItem::clearRelationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList != NULL )
			wordTypeList->clearRelationWriteLevel( currentWriteLevel );
		}

	bool WordItem::hasFeminineProperNameEnding()
		{
		if( wordTypeList != NULL )
			return wordTypeList->hasFeminineWordEnding( WORD_TYPE_PROPER_NAME );

		return false;
		}

	bool WordItem::hasMasculineProperNameEnding()
		{
		if( wordTypeList != NULL )
			return wordTypeList->hasMasculineWordEnding( WORD_TYPE_PROPER_NAME );

		return false;
		}

	bool WordItem::hasFeminineSingularNounEnding()
		{
		if( wordTypeList != NULL )
			return wordTypeList->hasFeminineWordEnding( WORD_TYPE_NOUN_SINGULAR );

		return false;
		}

	bool WordItem::hasMasculineSingularNounEnding()
		{
		if( wordTypeList != NULL )
			return wordTypeList->hasMasculineWordEnding( WORD_TYPE_NOUN_SINGULAR );

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
			return oppositeArticleWordItem->hasWordType( WORD_TYPE_ARTICLE );

		return false;
		}

	bool WordItem::hasWordType( unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return ( wordTypeList->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) != NULL );

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

	bool WordItem::isCorrectDefiniteArticle( bool isFeminineWord, bool isMasculineWord, unsigned short definiteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		isCorrectFeminineOrMasculineArticleWord( isFeminineWord, isMasculineWord, definiteArticleParameter ) &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectDefiniteArticle( definiteArticleParameter );

		return false;
		}

	bool WordItem::isCorrectIndefiniteArticle( bool isFeminineWord, bool isMasculineWord, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		isCorrectFeminineOrMasculineArticleWord( isFeminineWord, isMasculineWord, indefiniteArticleParameter ) &&
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

	bool WordItem::isMultipleWord( unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->isMultipleWord( wordTypeNr );

		return false;
		}

	bool WordItem::isNumeralWordType()
		{
		return hasWordType( WORD_TYPE_NUMERAL );
		}

	bool WordItem::isNoun()
		{
		return ( isSingularNoun() ||
				isPluralNoun() );
		}

	bool WordItem::isSingularNoun()
		{
		return hasWordType( WORD_TYPE_NOUN_SINGULAR );
		}

	bool WordItem::isPluralNoun()
		{
		return hasWordType( WORD_TYPE_NOUN_PLURAL );
		}

	bool WordItem::isProperName()
		{
		return hasWordType( WORD_TYPE_PROPER_NAME );
		}

	bool WordItem::isProperNamePrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList != NULL &&
		( foundWordTypeItem = wordTypeList->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NAME ) ) != NULL )
			return foundWordTypeItem->isProperNamePrecededByDefiniteArticle( definiteArticleParameter );

		return false;
		}

	ResultType WordItem::hideWordType( unsigned short wordTypeNr, void *authorizationKey )
		{
		if( wordTypeList != NULL )
			return wordTypeList->hideWordTypeItem( wordTypeNr, authorizationKey );

		return RESULT_OK;
		}

	ResultType WordItem::deleteWordType( unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->deleteWordType( wordTypeNr );

		return RESULT_OK;
		}

	ResultType WordItem::markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeList != NULL )
			return wordTypeList->markGeneralizationWordTypeAsWritten( isLanguageWord_, wordTypeNr );

		return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );
		}

	ResultType WordItem::markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeList != NULL )
			return wordTypeList->markSpecificationWordTypeAsWritten( wordTypeNr );

		return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );
		}

	ResultType WordItem::markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeList != NULL )
			return wordTypeList->markRelationWordTypeAsWritten( wordTypeNr );

		return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );
		}

	WordResultType WordItem::addWordType( bool isMultipleWord, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWordType";

		if( wordType_ == NULL &&
		// Create supporting module
		( wordType_ = new WordType( commonVariables_, this ) ) == NULL )
			{
			wordResult.result = startErrorInWord( functionNameString, NULL, "I failed to create my word type module" );
			return wordResult;
			}

		return wordType_->addWordType( isLanguageWord_, isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
		}

	WordResultType WordItem::findWordType( bool isForcingToCheckAllLanguages, unsigned short wordTypeNr, char *wordTypeString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordType";

		if( wordType_ != NULL )
			return wordType_->findWordType( isForcingToCheckAllLanguages, wordTypeNr, wordTypeString );

		wordResult.result = startErrorInWord( functionNameString, NULL, "My word type module isn't created yet" );
		return wordResult;
		}

	WordResultType WordItem::findWordTypeInAllWords( bool isForcingToCheckAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordTypeInAllWords";

		if( wordType_ == NULL &&
		// Create supporting module
		( wordType_ = new WordType( commonVariables_, this ) ) == NULL )
			{
			wordResult.result = startErrorInWord( functionNameString, NULL, "I failed to create my word type module" );
			return wordResult;
			}

		return wordType_->findWordTypeInAllWords( isForcingToCheckAllLanguages, wordTypeNr, wordTypeString, previousWordItem );
		}

	WordTypeItem *WordItem::activeWordTypeItem( bool isForcingToCheckAllLanguages, unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->activeWordTypeItem( false, ( isForcingToCheckAllLanguages || isLanguageWord_ ), wordTypeNr );

		return NULL;
		}

	char *WordItem::anyWordTypeString()
		{
		if( wordType_ != NULL )
			return wordType_->wordTypeString( true, WORD_TYPE_UNDEFINED );

		return NULL;
		}

	char *WordItem::activeWordTypeString( unsigned short wordTypeNr )
		{
		if( wordTypeList != NULL )
			return wordTypeList->activeWordTypeString( isLanguageWord_, wordTypeNr );

		return NULL;
		}

	char *WordItem::wordTypeString( bool isForcingToCheckAllLanguages, unsigned short wordTypeNr )
		{
		if( wordType_ != NULL )
			return wordType_->wordTypeString( ( isForcingToCheckAllLanguages || isLanguageWord_ ), wordTypeNr );

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

		if( wordWrite_ != NULL ||
		// Create supporting module
		( wordWrite_ = new WordWrite( commonVariables_, this ) ) != NULL )
			return wordWrite_->writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, justificationSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
		}

	ResultType WordItem::writeSelectedSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecification";

		if( wordWrite_ != NULL ||
		// Create supporting module
		( wordWrite_ = new WordWrite( commonVariables_, this ) ) != NULL )
			return wordWrite_->writeSelectedSpecification( false, true, true, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, writeSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
		}

	ResultType WordItem::writeSelectedSpecification( bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecification";

		if( wordWrite_ != NULL ||
		// Create supporting module
		( wordWrite_ = new WordWrite( commonVariables_, this ) ) != NULL )
			return wordWrite_->writeSelectedSpecification( false, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
		}

	ResultType WordItem::writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecificationInfo";

		if( wordWrite_ != NULL ||
		// Create supporting module
		( wordWrite_ = new WordWrite( commonVariables_, this ) ) != NULL )
			return wordWrite_->writeSelectedSpecificationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
		}

	ResultType WordItem::writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedRelationInfo";

		if( wordWrite_ != NULL ||
		// Create supporting module
		( wordWrite_ = new WordWrite( commonVariables_, this ) ) != NULL )
			return wordWrite_->writeSelectedRelationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, isLanguageWord_, isQuestion, writeWordItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
		}

	ResultType WordItem::writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUpdatedSpecification";

		if( wordWrite_ != NULL ||
		// Create supporting module
		( wordWrite_ = new WordWrite( commonVariables_, this ) ) != NULL )
			return wordWrite_->writeUpdatedSpecification( isAdjustedSpecification, isCorrectedAssumptionByKnowledge, isCorrectedAssumptionByOppositeQuestion, writeSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
		}


	// Protected write sentence functions

	ResultType WordItem::selectGrammarToWriteSentence( bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectGrammarToWriteSentence";

		if( wordWriteSentence_ != NULL ||
		// Create supporting module
		( wordWriteSentence_ = new WordWriteSentence( commonVariables_, this ) ) != NULL )
			return wordWriteSentence_->selectGrammarToWriteSentence( false, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I failed to create my word write sentence module" );
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
		WriteResultType writeResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeWordsToSentence";

		if( wordWriteWords_ != NULL ||
		// Create supporting module
		( wordWriteWords_ = new WordWriteWords( commonVariables_, this ) ) != NULL )
			return wordWriteWords_->writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem );

		writeResult.result = startErrorInWord( functionNameString, NULL, "I failed to create my word write words module" );
		return writeResult;
		}

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
