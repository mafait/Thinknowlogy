/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

#include "CollectionList.cpp"
#include "ContextList.cpp"
#include "GeneralizationList.cpp"
#include "GrammarList.cpp"
#include "InterfaceList.cpp"
#include "JustificationList.cpp"
#include "MultipleWordList.cpp"
#include "SelectionList.cpp"
#include "WordQuestion.cpp"
#include "WordSpecification.cpp"
#include "WordTypeList.cpp"
#include "WordWrite.cpp"


	// Private common functions

	unsigned short WordItem::nUsers()
		{
		unsigned short nUsers = 0;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *predefinedNounUserWordItem = globalVariables()->predefinedNounUserWordItem;

		if( predefinedNounUserWordItem != NULL &&
		( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	nUsers++;
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
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

	char *WordItem::selectedLanguageNameString( unsigned short languageNr )
		{
		WordItem *foundLanguageWordItem;

		if( ( foundLanguageWordItem = languageWordItem( languageNr ) ) != NULL )
			return foundLanguageWordItem->anyWordTypeString();

		return (char *)NO_LANGUAGE_NAME_FOUND_STRING;
		}

	char *WordItem::selectedUserNameString( unsigned short userNr )
		{
		unsigned short numberOfUsers;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;

		if( userNr == NO_USER_NR )
			return (char *)ADMIN_USER_NAME_STRING;

		if( ( numberOfUsers = nUsers() ) > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
				userNr == numberOfUsers-- )
					return generalizationWordItem->anyWordTypeString();
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return (char *)NO_USER_NAME_FOUND_STRING;
		}


	// Private error functions

	signed char WordItem::addErrorWithWordListNr( unsigned short wordListNr, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addError( wordListChar( wordListNr ), functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	BoolResultType WordItem::startBoolResultErrorInWord( const char *functionNameString, const char *errorString )
		{
		BoolResultType boolResult;

		boolResult.result = startError( functionNameString, NULL, anyWordTypeString(), errorString );
		return boolResult;
		}

	CollectionResultType WordItem::addCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addCollectionResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	CollectionResultType WordItem::startCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startCollectionResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	GrammarResultType WordItem::startGrammarResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startGrammarResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	JustificationResultType WordItem::startJustificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startJustificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WordEndingResultType WordItem::startWordEndingResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startWordEndingResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	WordTypeResultType WordItem::startWordTypeResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startWordTypeResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}


	// Private assignment functions

	signed char WordItem::inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundActiveAssignmentItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *relatedSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "inactivateAssignment";

		if( ( foundAssignmentItem = firstActiveNonQuestionAssignmentItem( ( isAmbiguousRelationContext ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == NULL )
			{
			if( relationContextNr == NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( relatedResult = findRelatedSpecification( true, true, false, isExclusiveSpecification, false, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem ) ).result != RESULT_OK )
						return addErrorInWord( functionNameString, NULL, "I failed to find a related assignment" );

					relatedSpecificationItem = relatedResult.relatedSpecificationItem;

					if( relatedSpecificationItem != NULL &&
					// Skip specification
					relatedSpecificationItem->isUserAssignment() &&

					( ( !isNegative &&
					// Only option left
					!isSelfGenerated ) ||

					!relatedSpecificationItem->isNegative() ) )
						{
						if( inactivateActiveAssignment( relatedSpecificationItem ) != RESULT_OK )
							return addErrorInWord( functionNameString, NULL, "I failed to inactivate a related active assignment" );
						}
					}
				}
			else	// Has relation context
				{
				if( ( foundActiveAssignmentItem = firstAssignmentItem( true, false, false, isNegative, isSelfGenerated, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
					{
					if( ( foundAssignmentItem = firstAssignmentItem( false, true, false, isNegative, isSelfGenerated, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
					// First archive an inactive assignment
					archiveInactiveAssignment( foundAssignmentItem ) != RESULT_OK )
						return addErrorInWord( functionNameString, NULL, "I failed to archive an inactive assignment" );

					// Now inactivate the found active assignment
					if( inactivateActiveAssignment( foundActiveAssignmentItem ) != RESULT_OK )
						return addErrorInWord( functionNameString, NULL, "I failed to inactivate an active assignment" );
					}
				}
			}
		else
			{
			if( isAmbiguousRelationContext ||

			( !foundAssignmentItem->isUniqueUserRelation() &&
			foundAssignmentItem->isNegative() != isNegative ) )
				{
				if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
					return addErrorInWord( functionNameString, NULL, "I failed to inactivate a negative active assignment" );
				}
			}

		return RESULT_OK;
		}

	signed char WordItem::inactivateCurrentAssignments()
		{
		SpecificationItem *activeAssignmentItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "inactivateCurrentAssignments";

		while( ( activeAssignmentItem = firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			if( inactivateActiveAssignment( activeAssignmentItem ) != RESULT_OK )
				return addErrorInWord( functionNameString, NULL, "I failed to inactive an active assignment item" );
			}

		return RESULT_OK;
		}

	SpecificationItem *WordItem::firstActiveNonQuestionAssignmentItem( unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		if( assignmentList_ != NULL )
			return assignmentList_->firstActiveNonQuestionAssignmentItem( relationContextNr, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		if( assignmentList_ != NULL )
			return assignmentList_->firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );

		return NULL;
		}

	SpecificationItem *WordItem::firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_->firstAssignmentItem( false, false, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->firstAssignmentItem( true, false, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				return assignmentList_->firstAssignmentItem( false, true, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	CreateAndAssignResultType WordItem::assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString )
		{
		bool hasDifferentAssumptionLevel = false;
		bool isAmbiguous = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		SpecificationItem *createdAssignmentItem;
		SpecificationItem *foundAssignmentItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSpecification";

		if( !isQuestion &&
		inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) != RESULT_OK )
			return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to inactivate an assignment" );

		if( ( foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == NULL &&
		// Didn't find a self-generated assignment. Now try to find a confirmed assignment
		isSelfGenerated &&
		!isSpecificationGeneralization )
			foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, false, questionParameter, relationContextNr, specificationWordItem );

		if( foundAssignmentItem != NULL )
			{
			createAndAssignResult.foundSpecificationItem = foundAssignmentItem;

			if( foundAssignmentItem->assumptionLevel() != assumptionLevel )
				hasDifferentAssumptionLevel = true;

			// Ambiguous if assignment has different tense: active, inactive or archived.
			if( foundAssignmentItem->isInactiveAssignment() != isInactiveAssignment ||
			foundAssignmentItem->isArchivedAssignment() != isArchivedAssignment )
				isAmbiguous = true;
			}

		if( isAmbiguous ||
		foundAssignmentItem == NULL ||

		( !isSelfGenerated &&
		hasDifferentAssumptionLevel ) ||

		// Archived assignment without relation will be
		// replaced by archived assignment with relation
		( isArchivedAssignment &&
		relationContextNr > NO_CONTEXT_NR &&
		!foundAssignmentItem->hasRelationContext() ) )
			{
			if( ( createAndAssignResult = createSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false, isCharacteristicFor, false, false, false, ( generalizationContextNr == NO_CONTEXT_NR && specificationContextNr == NO_CONTEXT_NR ? isEveryGeneralization : false ), isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, false, isUniqueUserRelation, false, globalVariables()->currentAssignmentLevel, assumptionLevel, globalVariables()->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL, NULL ) ).result != RESULT_OK )
				{
				if( specificationWordItem == NULL )
					return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to create an assignment with specification string \"", specificationString, "\"" );

				return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to create an assignment with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}

			if( ( createdAssignmentItem = createAndAssignResult.createdSpecificationItem ) == NULL )
				{
				if( specificationWordItem == NULL )
					return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "I couldn't create an assignment item with specification string \"", specificationString, "\"" );

				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "I couldn't create an assignment item with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}

			if( foundAssignmentItem != NULL )
				{
				if( hasDifferentAssumptionLevel )
					{
					if( !isQuestion &&
					replaceOrDeleteSpecification( foundAssignmentItem, ( foundAssignmentItem->isSelfGenerated() ? createdAssignmentItem : NULL ) ) != RESULT_OK )
						return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to replace or delete an ambiguous assignment" );
					}
				else
					{
					if( foundAssignmentItem->isActiveItem() )
						{
						if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
							return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to inactivate an active assignment" );
						}
					else
						{
						if( foundAssignmentItem->isInactiveAssignment() &&
						// Avoid archiving inactive assignments that are created during this sentence
						!foundAssignmentItem->hasCurrentInactiveSentenceNr() &&

						archiveInactiveAssignment( foundAssignmentItem ) != RESULT_OK )
							return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to archive an inactive assignment" );
						}
					}
				}
			}

		return createAndAssignResult;
		}


	// Private collection functions

	unsigned short WordItem::highestCollectionOrderNrInWord( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->highestCollectionOrderNr( collectionNr );

		return NO_ORDER_NR;
		}

	unsigned int WordItem::highestCollectionNrInWord()
		{
		if( collectionList_ != NULL )
			return collectionList_->highestCollectionNr();

		return NO_COLLECTION_NR;
		}


	// Private context functions

	bool WordItem::hasContextInWord( unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		if( contextList_ != NULL )
			return contextList_->hasContext( contextNr, spanishAmbiguousCollectionNr, specificationWordItem );

		return false;
		}

	bool WordItem::isContextSubsetInWord( unsigned int fullSetContextNr, unsigned int subsetContextNr )
		{
		if( contextList_ != NULL )
			return contextList_->isContextSubset( fullSetContextNr, subsetContextNr );

		return false;
		}


	// Private grammar functions

	void WordItem::markGrammarAsChoiceEnd()
		{
		if( grammarList_ != NULL )
			grammarList_->markAsChoiceEnd();
		}

	void WordItem::markGrammarAsOptionEnd()
		{
		if( grammarList_ != NULL )
			grammarList_->markAsOptionEnd();
		}

	signed char WordItem::checkForDuplicateGrammarDefinition()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateGrammarDefinition";

		if( grammarList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList_->checkForDuplicateGrammarDefinition();
		}

	signed char WordItem::checkGrammar()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkGrammar";

		if( grammarList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		if( grammarList_->isCheckingGrammarNeeded() )
			return grammarList_->checkGrammar();

		// Checking is not needed
		return RESULT_OK;
		}

	signed char WordItem::linkLaterDefinedGrammarWords()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "linkLaterDefinedGrammarWords";

		if( grammarList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		return grammarList_->linkLaterDefinedGrammarWords();
		}

	signed char WordItem::shrinkMergedWordsInWriteSentence()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "shrinkMergedWordsInWriteSentence";

		if( grammarList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The grammar list isn't created yet" );

		if( grammarList_->hasMergedWords() )
			return grammarList_->shrinkMergedWordsInWriteSentence();

		// Language has no merged words
		return RESULT_OK;
		}

	char *WordItem::grammarString( unsigned short wordTypeNr )
		{
		if( grammarList_ != NULL )
			return grammarList_->grammarStringInList( wordTypeNr );

		return NULL;
		}

	GrammarItem *WordItem::firstReadingGrammarItem()
		{
		if( grammarList_ != NULL )
			return grammarList_->firstReadingGrammarItem();

		return NULL;
		}

	GrammarItem *WordItem::firstWritingGrammarItem( bool isQuestion )
		{
		if( grammarList_ != NULL )
			return grammarList_->firstWritingGrammarItem( isQuestion );

		return NULL;
		}

	BoolResultType WordItem::expandMergedWordsInReadSentence( char *readUserSentenceString )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "expandMergedWordsInReadSentence";

		if( grammarList_ == NULL )
			return startBoolResultError( functionNameString, NULL, "The grammar list isn't created yet" );

		if( grammarList_->hasMergedWords() )
			{
			// Current language has merged words
			boolResult.booleanValue = true;
			boolResult.result = grammarList_->expandMergedWordsInReadSentence( readUserSentenceString );
			}

		return boolResult;
		}

	GrammarResultType WordItem::createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createGrammarItem";

		if( grammarList_ == NULL )
			{
			if( isAdminWord() )
				return startGrammarResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a grammar list" );

			if( ( grammarList_ = new GrammarList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startGrammarResultErrorInWord( functionNameString, NULL, "I failed to create a grammar list" );

			wordListArray_[WORD_GRAMMAR_LIST] = grammarList_;
			}

		return grammarList_->createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );
		}

	GrammarResultType WordItem::findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		GrammarResultType grammarResult;

		if( grammarList_ != NULL )
			return grammarList_->findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return grammarResult;
		}

	WordEndingResultType WordItem::analyzeWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString )
		{
		WordEndingResultType wordEndingResult;

		if( grammarList_ != NULL )
			return grammarList_->analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return wordEndingResult;
		}


	// Private interface functions

	signed char WordItem::checkInterface( unsigned short interfaceParameter, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkInterface";

		if( interfaceList_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have an interface list" );

			// Create list
			if( ( interfaceList_ = new InterfaceList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create an interface list" );

			wordListArray_[WORD_INTERFACE_LIST] = interfaceList_;
			}

		return interfaceList_->checkInterface( interfaceParameter, interfaceString );
		}

	signed char WordItem::createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createInterfaceItem";

		if( interfaceList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The interface list isn't created yet" );

		return interfaceList_->createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );
		}

	const char *WordItem::interfaceString_( unsigned short interfaceParameter )
		{
		if( interfaceList_ != NULL )
			return interfaceList_->interfaceString( interfaceParameter );

		return NULL;
		}


	// Private specification functions

	bool WordItem::isAuthorizedForChanges( char *authorizationKey )
		{
		return ( authorizationKey_ == NULL ||
				authorizationKey_ == authorizationKey );
		}

	signed char WordItem::checkSpecificationForUsageInWord( SpecificationItem *unusedSpecificationItem )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->checkSpecificationForUsage( false, false, false, unusedSpecificationItem ) == RESULT_OK &&
		assignmentList_->checkSpecificationForUsage( false, true, false, unusedSpecificationItem ) == RESULT_OK &&
		assignmentList_->checkSpecificationForUsage( false, true, false, unusedSpecificationItem ) == RESULT_OK )
			assignmentList_->checkSpecificationForUsage( false, false, true, unusedSpecificationItem );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL &&
		specificationList_->checkSpecificationForUsage( false, false, false, unusedSpecificationItem ) == RESULT_OK )
			specificationList_->checkSpecificationForUsage( false, false, true, unusedSpecificationItem );

		if( globalVariables()->result == RESULT_OK &&
		justificationList_ != NULL )
			justificationList_->checkSpecificationForUsage( unusedSpecificationItem );

		return globalVariables()->result;
		}

	// Private word type functions

	bool WordItem::isSingularNounWord()
		{
		return hasWordType( false, WORD_TYPE_NOUN_SINGULAR );
		}


	// Constructor (AdminItem)

	WordItem::WordItem()
		{
		// Private constructed variables

		isChineseLanguage_ = false;
		isSpanishLanguage_ = false;
		isNounWordSpanishAmbiguous_ = false;

		highestSentenceNrInWord_ = NO_SENTENCE_NR;

		authorizationKey_ = NULL;

		wordQuestion_ = NULL;
		wordSpecification_ = NULL;
		wordWrite_ = NULL;

		assignmentList_ = NULL;
		collectionList_ = NULL;
		contextList_ = NULL;
		generalizationList_ = NULL;
		grammarList_ = NULL;
		interfaceList_ = NULL;
		justificationList_ = NULL;
		multipleWordList_ = NULL;
		writeList_ = NULL;
		specificationList_ = NULL;
		wordTypeList_ = NULL;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = NULL;

		strcpy( wordItemNameString_, EMPTY_STRING );

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
		nextPossessiveNounWordItem = NULL;
		nextSpecificationWordItem = NULL;
		nextTouchedWordItem = NULL;
		nextUserProperNounWordItem = NULL;
		}


	// Constructor

	WordItem::WordItem( bool isLanguageWord, unsigned short wordParameter, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordItem", globalVariables, inputOutput, myList, this );

		// Private constructed variables

		isChineseLanguage_ = false;
		isSpanishLanguage_ = false;
		isNounWordSpanishAmbiguous_ = false;

		highestSentenceNrInWord_ = NO_SENTENCE_NR;

		authorizationKey_ = NULL;

		wordQuestion_ = NULL;
		wordSpecification_ = NULL;
		wordWrite_ = NULL;

		assignmentList_ = NULL;
		collectionList_ = NULL;
		contextList_ = NULL;
		generalizationList_ = NULL;
		grammarList_ = NULL;
		interfaceList_ = NULL;
		justificationList_ = NULL;
		multipleWordList_ = NULL;
		writeList_ = NULL;
		specificationList_ = NULL;
		wordTypeList_ = NULL;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = NULL;

		strcpy( wordItemNameString_, EMPTY_STRING );

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
		nextPossessiveNounWordItem = NULL;
		nextSpecificationWordItem = NULL;
		nextTouchedWordItem = NULL;
		nextUserProperNounWordItem = NULL;
		}

	WordItem::~WordItem()
		{
		if( wordQuestion_ != NULL )
			delete wordQuestion_;
		if( wordSpecification_ != NULL )
			delete wordSpecification_;
		if( wordWrite_ != NULL )
			delete wordWrite_;

		if( assignmentList_ != NULL )
			delete assignmentList_;
		if( collectionList_ != NULL )
			delete collectionList_;
		if( contextList_ != NULL )
			delete contextList_;
		if( generalizationList_ != NULL )
			delete generalizationList_;
		if( grammarList_ != NULL )
			delete grammarList_;
		if( interfaceList_ != NULL )
			delete interfaceList_;
		if( justificationList_ != NULL )
			delete justificationList_;
		if( multipleWordList_ != NULL )
			delete multipleWordList_;
		if( specificationList_ != NULL )
			delete specificationList_;
		if( wordTypeList_ != NULL )
			delete wordTypeList_;
		if( writeList_ != NULL )
			delete writeList_;
		}


	// Protected functions

	signed char WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	signed char WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		sprintf( tempString, "%s%u", errorString, number );
		return addErrorInWord( functionNameString, moduleNameString, tempString );
		}

	signed char WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	signed char WordItem::addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	signed char WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	signed char WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		sprintf( tempString, "%s%u", errorString, number );
		return startErrorInWord( functionNameString, moduleNameString, tempString );
		}

	signed char WordItem::startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	signed char WordItem::startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	CreateAndAssignResultType WordItem::addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	CreateAndAssignResultType WordItem::addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	CreateAndAssignResultType WordItem::addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	CreateAndAssignResultType WordItem::startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	CreateAndAssignResultType WordItem::startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	CreateAndAssignResultType WordItem::startCreateAndAssignResultSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startCreateAndAssignResultSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	RelatedResultType WordItem::addRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addRelatedResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	RelatedResultType WordItem::startRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startRelatedResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SpecificationResultType WordItem::addSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	SpecificationResultType WordItem::startSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	UserSpecificationResultType WordItem::addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addUserSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	UserSpecificationResultType WordItem::addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addUserSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

	UserSpecificationResultType WordItem::startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startUserSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	UserSpecificationResultType WordItem::startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startUserSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}


	// Protected virtual item functions

	void WordItem::checkForUsage()
		{
		if( assignmentList_ != NULL &&
		assignmentList_->checkWordItemForUsage( false, false, this ) == RESULT_OK &&
		assignmentList_->checkWordItemForUsage( true, false, this ) == RESULT_OK )
			assignmentList_->checkWordItemForUsage( false, true, this );

		if( globalVariables()->result == RESULT_OK &&
		collectionList_ != NULL )
			collectionList_->checkWordItemForUsage( this );

		if( globalVariables()->result == RESULT_OK &&
		contextList_ != NULL )
			contextList_->checkWordItemForUsage( this );

		if( globalVariables()->result == RESULT_OK &&
		generalizationList_ != NULL )
			generalizationList_->checkWordItemForUsage( this );

		if( globalVariables()->result == RESULT_OK &&
		multipleWordList_ != NULL )
			multipleWordList_->checkWordItemForUsage( this );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->checkWordItemForUsage( false, false, this );

		if( globalVariables()->result == RESULT_OK &&
		globalVariables()->adminConditionList != NULL )
			globalVariables()->adminConditionList->checkWordItemForUsage( this );

		if( globalVariables()->result == RESULT_OK &&
		globalVariables()->adminActionList != NULL )
			globalVariables()->adminActionList->checkWordItemForUsage( this );

		if( globalVariables()->result == RESULT_OK &&
		globalVariables()->adminAlternativeList != NULL )
			globalVariables()->adminAlternativeList->checkWordItemForUsage( this );
		}

	bool WordItem::hasParameter( unsigned int queryParameter )
		{
		return ( wordParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				wordParameter_ > NO_WORD_PARAMETER ) );
		}

	bool WordItem::isSorted( Item *nextSortItem )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.

		// Add new word at the end of the list
		return false;
		}

	BoolResultType WordItem::findMatchingWordReferenceString( char *searchString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findMatchingWordReferenceString";

		if( wordTypeList_ == NULL )
			return startBoolResultErrorInWord( functionNameString, "The word type list isn't created yet" );

		return wordTypeList_->findMatchingWordReferenceString( searchString );
		}

	char *WordItem::itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		itemBaseToString( queryWordTypeNr );


		queryString = globalVariables()->queryString;

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

		if( highestSentenceNrInWord_ > NO_WORD_PARAMETER )
			{
			sprintf( tempString, "%chighestSentenceNrInWord:%u", QUERY_SEPARATOR_CHAR, highestSentenceNrInWord_ );
			strcat( queryString, tempString );
			}

		if( wordParameter_ > NO_WORD_PARAMETER )
			{
			sprintf( tempString, "%cwordParameter:%u", QUERY_SEPARATOR_CHAR, wordParameter_ );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected common functions

	void WordItem::setCurrentLanguageAsChinese()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			currentLanguageWordItem->isChineseLanguage_ = true;
		}

	void WordItem::setHighestSentenceNr( unsigned int currentSentenceNr )
		{
		if( currentSentenceNr > highestSentenceNrInWord_ )
			highestSentenceNrInWord_ = currentSentenceNr;
		}

	bool WordItem::isAdjectiveAssigned()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

	bool WordItem::isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

	bool WordItem::isAdjectiveComparison()
		{
		return ( wordParameter_ >= WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER &&
				wordParameter_ <= WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER );
		}

	bool WordItem::isAdjectiveComparisonEqual()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_EQUAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_SAME );
		}

	bool WordItem::isAdjectiveComparisonLess()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_EARLIER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LESS ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LOWER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER );
		}

	bool WordItem::isAdjectiveComparisonMore()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_HIGHER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LARGER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LATER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_MORE );
		}

	bool WordItem::isAdjectiveEven()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVEN );
		}

	bool WordItem::isAdjectiveOdd()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ODD );
		}

	bool WordItem::isAdjectiveOddOrEven()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ODD ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVEN );
		}

	bool WordItem::isAdminWord()
		{
		return ( myList() == NULL );
		}

	bool WordItem::isAuthorizationRequiredForChanges()
		{
		return ( authorizationKey_ != NULL );
		}

	bool WordItem::isChineseCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;

		return ( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL &&
				currentLanguageWordItem->isChineseLanguage_ );
		}

	bool WordItem::isSpanishCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;

		return ( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL &&
				currentLanguageWordItem->isSpanishLanguage_ );
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

	bool WordItem::isChineseReversedImperativeNoun()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_HEAD ||
				wordParameter_ == WORD_PARAMETER_NOUN_NUMBER ||
				wordParameter_ == WORD_PARAMETER_NOUN_TAIL ||
				wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
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

	bool WordItem::isNounValue()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
		}

	bool WordItem::isBasicVerb()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IS ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_WAS );
		}

	bool WordItem::isImperativeVerbDisplay()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY );
		}

	bool WordItem::isImperativeVerbDisplayLoginOrRead()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ );
		}

	bool WordItem::isImperativeVerbUndoOrRedo()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO );
		}

	unsigned short WordItem::userNr( WordItem *userWordItem )
		{
		unsigned short userNr = nUsers();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;
		WordItem *predefinedNounUserWordItem = globalVariables()->predefinedNounUserWordItem;

		if( predefinedNounUserWordItem != NULL &&
		( currentGeneralizationItem = predefinedNounUserWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
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

		return NO_USER_NR;
		}

	unsigned short WordItem::wordParameter()
		{
		return wordParameter_;
		}

	unsigned int WordItem::highestSentenceNrInWord()
		{
		return highestSentenceNrInWord_;
		}

	signed char WordItem::assignChangePermissions( char *authorizationKey )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignChangePermissions";

		if( authorizationKey_ == NULL )
			authorizationKey_ = authorizationKey;
		else
			{
			if( authorizationKey_ != authorizationKey )
				return startErrorInWord( functionNameString, NULL, "The authorization key is already assigned" );
			}

		return RESULT_OK;
		}

	signed char WordItem::checkStructuralIntegrityInWord()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkStructuralIntegrityInWord";

		// Check assignments for replaced or deleted justifications
		if( assignmentList_ != NULL &&
		assignmentList_->checkForReplacedOrDeletedJustification( false, false ) == RESULT_OK &&
		assignmentList_->checkForReplacedOrDeletedJustification( false, false ) == RESULT_OK &&
		assignmentList_->checkForReplacedOrDeletedJustification( false, true ) == RESULT_OK )
			assignmentList_->checkForDeletedJustificationInReplacedSpecification();

		// Check for unused contexts
		if( globalVariables()->result == RESULT_OK &&
		contextList_ != NULL &&
		contextList_->checkForUnusedRelationContext() != RESULT_OK )
			return startErrorInWord( functionNameString, NULL, "I failed to check for unused relation contexts" );

		if( globalVariables()->result == RESULT_OK &&
		justificationList_ != NULL &&
		justificationList_->checkForReplacedOrDeletedSpecification() == RESULT_OK &&
		justificationList_->checkForUnreferencedReplacedJustifications() == RESULT_OK )
			justificationList_->replaceOrDeleteUnreferencedJustifications();

		// Check specifications for replaced or deleted justifications
		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL &&
		specificationList_->checkForReplacedOrDeletedJustification( false, false ) == RESULT_OK )
			specificationList_->checkForDeletedJustificationInReplacedSpecification();

		return globalVariables()->result;
		}

	signed char WordItem::markWordAsFemale()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markWordAsFemale";

		if( isMale_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already masculine" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnMasculineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );

		isFemale_ = true;
		isMale_ = false;

		return RESULT_OK;
		}

	signed char WordItem::markWordAsFeminine()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markWordAsFeminine";

		if( isMasculineWord_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already masculine" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnMasculineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );

		isFeminineWord_ = true;
		isMasculineWord_ = false;

		return RESULT_OK;
		}

	signed char WordItem::markWordAsMale()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markWordAsMale";

		if( isFemale_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already feminine" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnFeminineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );

		isFemale_ = false;
		isMale_ = true;

		return RESULT_OK;
		}

	signed char WordItem::markWordAsMasculine()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markWordAsMasculine";

		if( isFeminineWord_ &&
		!hasCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already feminine" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnFeminineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );

		isFeminineWord_ = false;
		isMasculineWord_ = true;

		return RESULT_OK;
		}

	char *WordItem::currentLanguageNameString()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->anyWordTypeString();

		return NULL;
		}

	char *WordItem::languageNameString( unsigned short languageNr )
		{
		WordItem *predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = globalVariables()->predefinedNounLanguageWordItem ) != NULL )
			return predefinedNounLanguageWordItem->selectedLanguageNameString( languageNr );

		return NULL;
		}

	char *WordItem::userNameString( unsigned short userNr )
		{
		WordItem *predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = globalVariables()->predefinedNounUserWordItem ) != NULL )
			return predefinedNounUserWordItem->selectedUserNameString( userNr );

		return NULL;
		}

	char *WordItem::wordTypeNameString( unsigned short wordTypeNr )
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->grammarString( wordTypeNr );

		return NULL;
		}

	WordItem *WordItem::languageWordItem( unsigned short languageNr )
		{
		unsigned short numberOfLanguages = nLanguages();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( numberOfLanguages > 0 &&
		( currentGeneralizationItem = firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
				currentGeneralizationWordItem->isLanguageWord() &&
				languageNr == numberOfLanguages-- )
					return currentGeneralizationItem->generalizationWordItem();
				}
			while( numberOfLanguages > 0 &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return NULL;
		}

	WordItem *WordItem::nextWordItem()
		{
		return (WordItem *)nextItem;
		}

	WordItem *WordItem::predefinedWordItem( unsigned short wordParameter )
		{
		WordItem *currentPredefinedWordItem;
		WordItem *lastPredefinedWordItem = globalVariables()->lastPredefinedWordItem;

		if( wordParameter > NO_WORD_PARAMETER &&
		( currentPredefinedWordItem = globalVariables()->firstPredefinedWordItem ) != NULL )
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
		if( assignmentList_ != NULL )
			return assignmentList_->nActiveAssignments();

		return 0;
		}

	signed char WordItem::archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "archiveInactivateAssignment";

		if( assignmentList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

		if( inactiveAssignmentItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given inactive assignment item is undefined" );

		if( !inactiveAssignmentItem->isInactiveAssignment() )
			return startErrorInWord( functionNameString, NULL, "The given assignment item isn't inactive" );

		if( assignmentList_->archiveItem( inactiveAssignmentItem ) != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to archive the given inactive assignment item" );

		inactiveAssignmentItem->clearStoredSentenceString();

		return RESULT_OK;
		}

	signed char WordItem::createNewAssignmentLevelInWord()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createNewAssignmentLevelInWord";

		if( assignmentList_ != NULL &&
		assignmentList_->createNewAssignmentLevelInList() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to create assignment level ", ( globalVariables()->currentAssignmentLevel + 1 ) );

		return RESULT_OK;
		}

	signed char WordItem::deleteAssignmentLevelInWord()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAssignmentLevelInWord";

		if( assignmentList_ != NULL &&
		assignmentList_->deleteAssignmentLevelInList() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to delete assignment level ", globalVariables()->currentAssignmentLevel );

		return RESULT_OK;
		}

	signed char WordItem::inactivateActiveAssignment( SpecificationItem *activeAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "inactivateActiveAssignment";

		if( assignmentList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

		if( activeAssignmentItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given active assignment item is undefined" );

		if( !activeAssignmentItem->isActiveAssignment() )
			return startErrorInWord( functionNameString, NULL, "The given active assignment item isn't active" );

		if( assignmentList_->inactivateItem( activeAssignmentItem ) != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to inactivate an active assignment" );

		activeAssignmentItem->clearStoredSentenceString();

		return RESULT_OK;
		}

	SpecificationItem *WordItem::firstActiveNumeralAssignmentItem()
		{
		if( assignmentList_ != NULL )
			return assignmentList_->firstActiveNumeralAssignmentItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstActiveStringAssignmentItem()
		{
		if( assignmentList_ != NULL )
			return assignmentList_->firstActiveStringAssignmentItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstNonQuestionActiveAssignmentItem()
		{
		if( assignmentList_ != NULL )
			return assignmentList_->firstAssignmentItem( false, false, false, false );

		return NULL;
		}

	SpecificationItem *WordItem::lastActiveNonQuestionAssignmentItem()
		{
		if( assignmentList_ != NULL )
			return assignmentList_->lastActiveNonQuestionAssignmentItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstAssignmentItem( bool isPossessive, bool isQuestion, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL &&
		// Active assignment
		( foundSpecificationItem = assignmentList_->firstAssignmentItem( false, false, isPossessive, isQuestion, relationContextNr, specificationWordItem ) ) == NULL &&
		// Inactive assignment
		( foundSpecificationItem = assignmentList_->firstAssignmentItem( true, false, isPossessive, isQuestion, relationContextNr, specificationWordItem ) ) == NULL )
			// Archived assignment
			return assignmentList_->firstAssignmentItem( false, true, isPossessive, isQuestion, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstNonPossessiveActiveAssignmentItem( WordItem *relationWordItem )
		{
		if( assignmentList_ != NULL )
			return assignmentList_->firstRelationSpecificationItem( false, relationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstQuestionAssignmentItem()
		{
		SpecificationItem *firstAssignmentItem = NULL;

		if( assignmentList_ != NULL &&
		// Active assignment
		( firstAssignmentItem = assignmentList_->firstAssignmentItem( false, false, false, true ) ) == NULL )
			// Archived assignment
			return assignmentList_->firstAssignmentItem( false, false, true, true );

		return firstAssignmentItem;
		}

	SpecificationItem *WordItem::firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_->firstNonQuestionAssignmentItem( false, false, isNegative, isPossessive, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->firstNonQuestionAssignmentItem( true, false, isNegative, isPossessive, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				return assignmentList_->firstNonQuestionAssignmentItem( false, true, isNegative, isPossessive, specificationWordItem );
			}

		return foundSpecificationItem;
		}

	CreateAndAssignResultType WordItem::assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *authorizationKey )
		{
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *foundSpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSpecification";

		// Not authorized to assign this (generalization) word
		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != NULL &&
		// No user is logged in yet
		globalVariables()->currentUserNr != NO_USER_NR &&
		// Not authorized to assign this specification word
		!specificationWordItem->isAuthorizedForChanges( authorizationKey ) ) )
			return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "You are not authorized to assign this word" );

		if( assignmentList_ == NULL )
			{
			if( isAdminWord() )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "The admin word item cannot have an assignment list" );

			// Create assignment list
			if( ( assignmentList_ = new SpecificationList( WORD_ASSIGNMENT_LIST_SYMBOL, globalVariables(), inputOutput(), this ) ) == NULL )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to create an assignment list" );

			wordListArray_[WORD_ASSIGNMENT_LIST] = assignmentList_;
			}

		if( isAssignedOrClear )
			{
			if( questionParameter > NO_QUESTION_PARAMETER )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "A question can only be answered, not be cleared" );

			if( inactivateCurrentAssignments() != RESULT_OK )
				return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to inactivate my current assignments" );
			}
		else
			{
			// Find the specification of the assignment
			if( ( foundSpecificationItem = firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) ) == NULL )
				{
				if( specificationWordItem == NULL )
					return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "String \"", specificationString, "\" isn't one of my specifications" );

				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "Word \"", specificationWordItem->anyWordTypeString(), "\" isn't one of my specifications" );
				}

			if( foundSpecificationItem->isExclusiveNonPossessiveGeneralization() &&
			assignmentList_->inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, specificationWordItem ) != RESULT_OK )
				return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to inactivate or archive current generalization assignments" );

			if( foundSpecificationItem->isValueSpecification() )
				{
				if( specificationWordItem == NULL )
					return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "The given specification word item is undefined" );

				// Assign specification by value
				if( ( currentAssignmentItem = specificationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
					{
					do	{
						if( ( createAndAssignResult = assignSpecification( false, currentAssignmentItem->isInactiveAssignment(), currentAssignmentItem->isArchivedAssignment(), currentAssignmentItem->isCharacteristicFor(), currentAssignmentItem->isEveryGeneralization(), currentAssignmentItem->isExclusiveGeneralization(), currentAssignmentItem->isExclusiveSpecification(), currentAssignmentItem->isNegative(), currentAssignmentItem->isPartOf(), currentAssignmentItem->isPossessive(), currentAssignmentItem->isSpecificationGeneralization(), currentAssignmentItem->isUniqueUserRelation(), currentAssignmentItem->assumptionLevel(), currentAssignmentItem->prepositionParameter(), currentAssignmentItem->questionParameter(), currentAssignmentItem->generalizationWordTypeNr(), currentAssignmentItem->specificationWordTypeNr(), currentAssignmentItem->relationWordTypeNr(), currentAssignmentItem->specificationCollectionNr(), currentAssignmentItem->generalizationContextNr(), currentAssignmentItem->specificationContextNr(), currentAssignmentItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, currentAssignmentItem->nContextRelations(), firstJustificationItem, currentAssignmentItem->specificationWordItem(), currentAssignmentItem->specificationString() ) ).result != RESULT_OK )
							return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}
					while( ( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
					}
				}
			else
				{
				if( ( createAndAssignResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem->isCharacteristicFor(), foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->isExclusiveGeneralization(), foundSpecificationItem->isExclusiveSpecification(), isNegative, foundSpecificationItem->isPartOf(), isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem->generalizationWordTypeNr(), foundSpecificationItem->specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem->specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
					{
					if( specificationWordItem == NULL )
						return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to assign specification string \"", specificationString, "\"" );

					return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return createAndAssignResult;
		}


	// Protected cleanup functions

	void WordItem::clearReplacingInfoInWord()
		{
		if( justificationList_ != NULL )
			justificationList_->clearReplacingInfo();

		if( assignmentList_ != NULL )
			assignmentList_->clearReplacingInfo();

		if( specificationList_ != NULL )
			specificationList_->clearReplacingInfo();
		}

	void WordItem::rebuildQuickAccessWordLists()
		{
		nextAssignmentWordItem = NULL;
		nextCollectionWordItem = NULL;
		nextContextWordItem = NULL;
		nextPossessiveNounWordItem = NULL;
		nextSpecificationWordItem = NULL;
		nextTouchedWordItem = NULL;
		nextUserProperNounWordItem = NULL;

		if( assignmentList_ != NULL )
			assignmentList_->addToAssignmentWordQuickAccessList();

		if( collectionList_ != NULL )
			collectionList_->addToCollectionWordQuickAccessList();

		if( contextList_ != NULL )
			contextList_->addToContextWordQuickAccessList();

		if( specificationList_ != NULL )
			{
			if( isSingularNounWord() )
				{
				if( specificationList_->firstPossessiveSpecificationItem() != NULL )
					specificationList_->addToPossessiveNounWordQuickAccessList();
				}
			else
				{
				if( isProperNounWord() &&
				hasUserNr() )
					specificationList_->addToUserProperNounWordQuickAccessList();
				}

			specificationList_->addToSpecificationWordQuickAccessList();
			}
		}

	unsigned int WordItem::highestCurrentSentenceItemNrInWord( unsigned int currentSentenceNr, unsigned int currentSentenceItemNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// Efficiency: Only select lists with current sentence number or higher
			currentWordList->highestSentenceNrInList() >= currentSentenceNr )
				currentSentenceItemNr = currentWordList->highestCurrentSentenceItemNrInList( currentSentenceNr, currentSentenceItemNr );
			}

		return currentSentenceItemNr;
		}

	unsigned int WordItem::highestFoundSentenceNrInWord( bool isIncludingTemporaryLists, unsigned int highestFoundSentenceNr, unsigned int maxSentenceNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; ( wordListNr < NUMBER_OF_WORD_LISTS &&
											// Efficiency: Only select word lists with higher sentence number
											highestFoundSentenceNr < highestSentenceNrInWord_ &&
											highestFoundSentenceNr < maxSentenceNr ); wordListNr++ )
			{
			if( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != NULL &&

			// Skip temporary lists
			( isIncludingTemporaryLists ||
			!currentWordList->isTemporaryList() ) )
				highestFoundSentenceNr = currentWordList->highestFoundSentenceNrInList( highestFoundSentenceNr, maxSentenceNr );
			}

		return highestFoundSentenceNr;
		}

	signed char WordItem::decrementItemNrRangeInWord( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementItemNrRangeInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// Efficiency: Only select lists with decrement sentence number of higher
			currentWordList->highestSentenceNrInList() >= decrementSentenceNr &&

			currentWordList->decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to decrement item number range" );
			}

		return RESULT_OK;
		}

	signed char WordItem::decrementSentenceNrsInWord( unsigned int startSentenceNr )
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementSentenceNrsInWord";

		if( startSentenceNr <= NO_SENTENCE_NR )
			return startError( functionNameString, NULL, "The given start sentence number is undefined" );

		if( highestSentenceNrInWord_ == startSentenceNr )
			highestSentenceNrInWord_--;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST &&
			( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// Efficiency: Only select lists with start sentence number of higher
			currentWordList->highestSentenceNrInList() >= startSentenceNr &&

			currentWordList->decrementSentenceNrsInList( startSentenceNr ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
			}

		return RESULT_OK;
		}

	signed char WordItem::deleteSentencesInWord( unsigned int lowestSentenceNr )
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteSentencesInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// Efficiency: Only select lists with lowest sentence number of higher
			currentWordList->highestSentenceNrInList() >= lowestSentenceNr &&

			currentWordList->deleteSentencesInList( lowestSentenceNr ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to delete sentences in one of my lists" );
			}

		return RESULT_OK;
		}

	signed char WordItem::removeFirstRangeOfDeletedItemsInWord()
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeFirstRangeOfDeletedItemsInWord";

		for( unsigned short wordListNr = 0; ( wordListNr < NUMBER_OF_WORD_LISTS && globalVariables()->nDeletedItems == 0 ); wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			currentWordList->removeFirstRangeOfDeletedItemsInList() != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to remove the first deleted items" );
			}

		return RESULT_OK;
		}

	signed char WordItem::redoCurrentSentenceInWord()
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "redoCurrentSentenceInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			!currentWordList->isTemporaryList() &&
			currentWordList->redoCurrentSentenceInList() != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to redo the current sentence" );
			}

		return RESULT_OK;
		}

	signed char WordItem::undoCurrentSentenceInWord()
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "undoCurrentSentenceInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			!currentWordList->isTemporaryList() &&
			currentWordList->undoCurrentSentenceInList() != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to undo the current sentence" );
			}

		return RESULT_OK;
		}


	// Protected collection functions

	bool WordItem::hasCollection()
		{
		return ( collectionList_ != NULL &&
				collectionList_->hasActiveItems() );
		}

	bool WordItem::hasCollectionNr( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->hasCollectionNr( collectionNr );

		return false;
		}

	bool WordItem::hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem )
		{
		if( collectionList_ != NULL )
			return collectionList_->hasCollectionNr( collectionNr, commonWordItem );

		return false;
		}

	bool WordItem::hasNonExclusiveCollection( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->hasNonExclusiveCollection( collectionNr );

		return false;
		}

	bool WordItem::isCompoundCollection( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->isCompoundCollection( collectionNr );

		return false;
		}

	bool WordItem::isCollectionSpanishAmbiguous( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->isCollectionSpanishAmbiguous( collectionNr );

		return false;
		}

	bool WordItem::isNonCompoundCollection( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->isNonCompoundCollection( collectionNr );

		return false;
		}

	bool WordItem::isNounWordSpanishAmbiguous()
		{
		return isNounWordSpanishAmbiguous_;
		}

	unsigned short WordItem::highestCollectionOrderNrInCollectionWords( unsigned int collectionNr )
		{
		unsigned short highestCollectionOrderNr = NO_ORDER_NR;
		unsigned short tempCollectionOrderNr;
		WordItem *currentCollectionWordItem;

		if( collectionNr > NO_COLLECTION_NR &&
		( currentCollectionWordItem = globalVariables()->firstCollectionWordItem ) != NULL )
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
		if( collectionList_ != NULL )
			return collectionList_->collectionNr( collectionWordTypeNr );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::collectionNr( WordItem *commonWordItem )
		{
		if( collectionList_ != NULL )
			return collectionList_->collectionNr( commonWordItem );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::collectionNrByCompoundGeneralizationWordInWord( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		if( collectionList_ != NULL )
			return collectionList_->collectionNrByCompoundGeneralizationWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::compoundCollectionNr()
		{
		if( collectionList_ != NULL )
			return collectionList_->compoundCollectionNr();

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::nonCompoundCollectionNr()
		{
		if( collectionList_ != NULL )
			return collectionList_->nonCompoundCollectionNr();

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->nonCompoundCollectionNr( compoundCollectionNr );

		return NO_COLLECTION_NR;
		}

	unsigned int WordItem::highestCollectionNrInCollectionWords()
		{
		unsigned int highestCollectionNr = NO_COLLECTION_NR;
		unsigned int tempCollectionNr;
		WordItem *currentCollectionWordItem;

		if( ( currentCollectionWordItem = globalVariables()->firstCollectionWordItem ) != NULL )
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

	WordItem *WordItem::collectionWordItem( unsigned int compoundCollectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->collectionWordItem( compoundCollectionNr );

		return NULL;
		}

	WordItem *WordItem::commonWordItem( unsigned int collectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->commonWordItem( collectionNr );

		return NULL;
		}

	WordItem *WordItem::compoundGeneralizationWordItem( unsigned int compoundCollectionNr )
		{
		if( collectionList_ != NULL )
			return collectionList_->compoundGeneralizationWordItem( compoundCollectionNr );

		return NULL;
		}

	WordItem *WordItem::feminineCollectionWordItem()
		{
		if( collectionList_ != NULL )
			return collectionList_->feminineCollectionWordItem();

		return NULL;
		}

	WordItem *WordItem::masculineCollectionWordItem()
		{
		if( collectionList_ != NULL )
			return collectionList_->masculineCollectionWordItem();

		return NULL;
		}

	BoolResultType WordItem::findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		BoolResultType boolResult;

		if( collectionList_ != NULL )
			return collectionList_->findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return boolResult;
		}

	CollectionResultType WordItem::createCollection( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createCollection";

		if( commonWordItem == this )
			{
			isNounWordSpanishAmbiguous_ = true;

			if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
				currentLanguageWordItem->isSpanishLanguage_ = true;
			}

		if( collectionList_ == NULL )
			{
			if( isAdminWord() )
				return startCollectionResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a collection list" );

			// Create list
			if( ( collectionList_ = new CollectionList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startCollectionResultErrorInWord( functionNameString, NULL, "I failed to create a collection list" );

			wordListArray_[WORD_COLLECTION_LIST] = collectionList_;
			}

		return collectionList_->createCollection( isExclusiveSpecification, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
		}


	// Protected context functions

	bool WordItem::hasContextCurrentlyBeenUpdatedInWord( unsigned int contextNr )
		{
		if( contextList_ != NULL )
			return contextList_->hasContextCurrentlyBeenUpdated( contextNr );

		return false;
		}

	bool WordItem::hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem )
		{
		if( contextList_ != NULL )
			return contextList_->hasContext( contextNr, specificationWordItem );

		return false;
		}

	bool WordItem::hasFoundContextInAllWords( unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		WordItem *currentContextWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->hasContextInWord( contextNr, spanishAmbiguousCollectionNr, specificationWordItem ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return false;
		}

	bool WordItem::isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr )
		{
		if( contextList_ != NULL )
			return ( contextList_->hasContext( firstContextNr ) == contextList_->hasContext( secondContextNr ) );

		return true;
		}

	bool WordItem::isContextSubsetInContextWords( unsigned int fullSetContextNr, unsigned int subsetContextNr )
		{
		WordItem *currentContextWordItem;

		if( fullSetContextNr > NO_CONTEXT_NR &&
		subsetContextNr > NO_CONTEXT_NR )
			{
			if( fullSetContextNr == subsetContextNr )
				return true;

			if( ( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
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

	unsigned short WordItem::contextWordTypeNr( unsigned int contextNr )
		{
		ContextItem *foundContextItem;

		if( ( foundContextItem = contextItem( contextNr ) ) != NULL )
			return foundContextItem->contextWordTypeNr();

		return NO_WORD_TYPE_NR;
		}

	unsigned int WordItem::contextNr( WordItem *specificationWordItem )
		{
		if( contextList_ != NULL )
			return contextList_->contextNr( specificationWordItem );

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::contextNr( unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		if( contextList_ != NULL )
			return contextList_->contextNr( spanishAmbiguousCollectionNr, specificationWordItem );

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::highestContextNrInWord()
		{
		if( contextList_ != NULL )
			return contextList_->highestContextNr();

		return NO_CONTEXT_NR;
		}

	unsigned int WordItem::nContextWords( unsigned int contextNr, WordItem *specificationWordItem )
		{
		unsigned int nContextWords = 0;
		WordItem *currentContextWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
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

	signed char WordItem::addContext( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addContext";

		if( contextList_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a context list" );

			// Create list
			if( ( contextList_ = new ContextList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a context list" );

			wordListArray_[WORD_CONTEXT_LIST] = contextList_;
			}

		return contextList_->addContext( contextWordTypeNr, specificationWordTypeNr, contextNr, spanishAmbiguousCollectionNr, specificationWordItem );
		}

	ContextItem *WordItem::firstActiveContextItem()
		{
		if( contextList_ != NULL )
			return contextList_->firstActiveContextItem();

		return NULL;
		}

	ContextItem *WordItem::contextItem( unsigned int contextNr )
		{
		if( contextList_ != NULL )
			return contextList_->contextItem( contextNr );

		return NULL;
		}

	ContextItem *WordItem::contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		if( contextList_ != NULL )
			return contextList_->contextItem( isCompoundCollectionSpanishAmbiguous, nContextWords, spanishAmbiguousCollectionNr, specificationWordItem );

		return NULL;
		}


	// Protected database connection functions
/*
	signed char WordItem::storeChangesInFutureDatabaseInWord()
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "storeChangesInFutureDatabaseInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// No need to store items of temporary lists
			!currentWordList->isTemporaryList() &&
			currentWordList->storeChangesInFutureDatabaseInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to store changes of a list in the database" );
			}

		return RESULT_OK;
		}
*/

	// Protected generalization functions

	signed char WordItem::createGeneralizationItem( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createGeneralizationItem";

		if( generalizationList_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a generalization list" );

			// Create list
			if( ( generalizationList_ = new GeneralizationList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a generalization list" );

			wordListArray_[WORD_GENERALIZATION_LIST] = generalizationList_;
			}

		return generalizationList_->createGeneralizationItem( isLanguageWord_, isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem );
		}

	GeneralizationItem *WordItem::firstGeneralizationItem()
		{
		if( generalizationList_ != NULL )
			return generalizationList_->firstActiveGeneralizationItem();

		return NULL;
		}

	GeneralizationItem *WordItem::firstNounSpecificationGeneralizationItem()
		{
		if( generalizationList_ != NULL )
			return generalizationList_->firstGeneralizationItem( true, true, false );

		return NULL;
		}

	GeneralizationItem *WordItem::firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage )
		{
		if( generalizationList_ != NULL )
			return generalizationList_->firstGeneralizationItem( false, isOnlySelectingCurrentLanguage, false );

		return NULL;
		}

	GeneralizationItem *WordItem::firstRelationGeneralizationItem()
		{
		if( generalizationList_ != NULL )
			return generalizationList_->firstGeneralizationItem( false, false, true );

		return NULL;
		}

	BoolResultType WordItem::findGeneralization( bool isRelation, WordItem *generalizationWordItem )
		{
		BoolResultType boolResult;

		if( generalizationList_ != NULL )
			return generalizationList_->findGeneralization( isRelation, generalizationWordItem );

		return boolResult;
		}


	// Protected grammar functions

	void WordItem::checkGrammarForUsageInWord( GrammarItem *unusedGrammarItem )
		{
		if( grammarList_ != NULL )
			grammarList_->checkGrammarItemForUsage( unusedGrammarItem );

		if( globalVariables()->result == RESULT_OK &&
		writeList_ != NULL )
			writeList_->checkGrammarItemForUsage( unusedGrammarItem );
		}

	void WordItem::markGrammarOfCurrentLanguageAsChoiceEnd()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			currentLanguageWordItem->markGrammarAsChoiceEnd();
		}

	void WordItem::markGrammarOfCurrentLanguageAsOptionEnd()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			currentLanguageWordItem->markGrammarAsOptionEnd();
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
		WordItem *predefinedNounLanguageWordItem = globalVariables()->predefinedNounLanguageWordItem;

		if( predefinedNounLanguageWordItem != NULL &&
		( currentGeneralizationItem = predefinedNounLanguageWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( generalizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
				generalizationWordItem->isLanguageWord() )
					nLanguages++;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return nLanguages;
		}

	signed char WordItem::checkForDuplicateGrammarDefinitionInCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateGrammarDefinitionInCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->checkForDuplicateGrammarDefinition();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	signed char WordItem::checkGrammarOfCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkGrammarOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->checkGrammar();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	signed char WordItem::linkLaterDefinedGrammarWordsInCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "linkLaterDefinedGrammarWordsInCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->linkLaterDefinedGrammarWords();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	signed char WordItem::shrinkMergedWordsInWriteSentenceOfCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "shrinkMergedWordsInWriteSentenceOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->shrinkMergedWordsInWriteSentence();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	GrammarItem *WordItem::firstCurrentLanguageReadingGrammarItem()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->firstReadingGrammarItem();

		return NULL;
		}

	GrammarItem *WordItem::firstCurrentLanguageWritingGrammarItem( bool isQuestion )
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->firstWritingGrammarItem( isQuestion );

		return NULL;
		}

	BoolResultType WordItem::expandMergedWordsInReadSentenceOfCurrentLanguage( char *readUserSentenceString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "shrinkMergedWordsInWriteSentence";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->expandMergedWordsInReadSentence( readUserSentenceString );

		return startBoolResultError( functionNameString, NULL, "The current language word is undefined" );
		}

	GrammarResultType WordItem::createGrammarItemForCurrentLanguage( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createGrammarItemForCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );

		return startGrammarResultErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	GrammarResultType WordItem::findGrammarOfCurrentLanguage( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findGrammarOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return startGrammarResultErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	WordEndingResultType WordItem::analyzeWordEndingWithCurrentLanguage( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString )
		{
		WordItem *currentLanguageWordItem;
		WordEndingResultType wordEndingResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "analyzeWordEndingWithCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->analyzeWordEnding( grammarParameter, originalWordStringLength, originalWordString );

		return startWordEndingResultErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}


	// Protected interface functions

	void WordItem::checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( justificationList_ != NULL )
			justificationList_->checkSpecificationForUsage( unusedSpecificationItem );

		if( globalVariables()->result == RESULT_OK &&
		// Check this word
		checkSpecificationForUsageInWord( unusedSpecificationItem ) == RESULT_OK &&
		( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
			{
			do	{
				// Also check generalization words
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					currentGeneralizationWordItem->checkSpecificationForUsageInWord( unusedSpecificationItem );
				}
			while( globalVariables()->result == RESULT_OK &&
			( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
			}
		}

	signed char WordItem::checkInterfaceOfCurrentLanguage( unsigned short interfaceParameter, char *interfaceString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkInterfaceOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->checkInterface( interfaceParameter, interfaceString );

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	signed char WordItem::createInterfaceForCurrentLanguage( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createInterfaceForCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

	const char *WordItem::interfaceString( unsigned short interfaceParameter )
		{
		const char *foundInterfaceString = NULL;
		WordItem *currentLanguageWordItem;
		WordItem *firstLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			foundInterfaceString = currentLanguageWordItem->interfaceString_( interfaceParameter );

		if( foundInterfaceString == NULL &&

		// The current language doesn't have the requested interface string.
		// Now try to get interface string of the first language (=English).
		( currentLanguageWordItem == NULL ||
		( firstLanguageWordItem = globalVariables()->firstWordItem ) == NULL ||
		firstLanguageWordItem == currentLanguageWordItem ||
		( foundInterfaceString = firstLanguageWordItem->interfaceString_( interfaceParameter ) ) == NULL ) )
			return NO_INTERFACE_STRING_FOUND_STRING;

		return foundInterfaceString;
		}


	// Protected justification functions

	void WordItem::clearJustificationHasBeenWritten()
		{
		if( justificationList_ != NULL )
			justificationList_->clearJustificationHasBeenWritten();
		}

	bool WordItem::hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->hasJustification( primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem );

		return false;
		}

	bool WordItem::hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		if( justificationList_ != NULL )
			return justificationList_->hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();

		return false;
		}

	signed char WordItem::attachJustificationInWord( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "attachJustificationInWord";

		if( justificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->attachJustification( attachJustificationItem, involvedSpecificationItem );
		}

	signed char WordItem::replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceJustification";

		if( justificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->replaceJustification( false, obsoleteJustificationItem, replacingJustificationItem, involvedSpecificationItem, NULL );
		}

	signed char WordItem::replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteJustification";

		if( justificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->replaceOrDeleteJustification( obsoleteJustificationItem );
		}

	signed char WordItem::replaceOrDeleteUnreferencedJustifications()
		{
		if( justificationList_ != NULL )
			return justificationList_->replaceOrDeleteUnreferencedJustifications();

		return RESULT_OK;
		}

	signed char WordItem::updateSpecificationsInJustificationInWord( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->updateSpecificationsInJustifications( isMainWord, obsoleteSpecificationItem, replacingSpecificationItem );

		return RESULT_OK;
		}

	signed char WordItem::writeRelatedJustificationSpecifications( unsigned short justificationTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeRelatedJustificationSpecifications";

		if( justificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->writeRelatedJustificationSpecifications( justificationTypeNr );
		}

	JustificationResultType WordItem::addJustification( bool hasFeminineOrMasculineProperNounEnding, bool isForcingNewJustification, bool isIncrementingOrderNr, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addJustification";

		if( justificationList_ == NULL )
			{
			if( isAdminWord() )
				return startJustificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a justification list" );

			if( ( justificationList_ = new JustificationList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startJustificationResultErrorInWord( functionNameString, NULL, "I failed to create a justification list" );

			wordListArray_[WORD_JUSTIFICATION_LIST] = justificationList_;
			}

		return justificationList_->addJustification( hasFeminineOrMasculineProperNounEnding, isForcingNewJustification, isIncrementingOrderNr, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem );
		}

	JustificationResultType WordItem::copyJustification( bool isForcingNewJustification, SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyJustification";

		if( justificationList_ == NULL )
			return startJustificationResultErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->copyJustification( isForcingNewJustification, newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );
		}

	JustificationItem *WordItem::correctedAssumptionByOppositeQuestionJustificationItem()
		{
		if( justificationList_ != NULL )
			return justificationList_->correctedAssumptionByOppositeQuestionJustificationItem();

		return NULL;
		}

	JustificationItem *WordItem::negativeAssumptionOrConclusionJustificationItem( SpecificationItem *anotherPrimarySpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, bool isPossessiveSecondarySpecification, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->olderComplexJustificationItem( hasSecondarySpecificationWithoutRelationContext, isPossessiveSecondarySpecification, justificationTypeNr, secondarySpecificationCollectionNr, primarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::possessiveReversibleAssumptionJustificationItem( WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->possessiveReversibleAssumptionJustificationItem( generalizationWordItem, secondarySpecificationWordItem );

		return NULL;
		}

	JustificationItem *WordItem::primarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->primarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, primarySpecificationItem );

		return NULL;
		}

	JustificationItem *WordItem::secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->secondarySpecificationJustificationItem( isSelectingOlderItemOnly, justificationTypeNr, secondarySpecificationItem );

		return NULL;
		}

	SpecificationItem *WordItem::suggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		if( justificationList_ != NULL )
			return justificationList_->suggestiveQuestionAssumptionSecondarySpecificationItem();

		return NULL;
		}


	// Protected multiple word functions

	bool WordItem::isMultipleWord()
		{
		return ( multipleWordList_ != NULL &&
				multipleWordList_->hasActiveItems() );
		}

	unsigned short WordItem::matchingMultipleWordParts( char *sentenceString )
		{
		if( multipleWordList_ != NULL )
			return multipleWordList_->matchingMultipleWordParts( sentenceString );

		return 0;
		}

	signed char WordItem::addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addMultipleWord";

		if( multipleWordList_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a multiple word list" );

			// Create list
			if( ( multipleWordList_ = new MultipleWordList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a multiple word list" );

			wordListArray_[WORD_MULTIPLE_WORD_LIST] = multipleWordList_;
			}

		return multipleWordList_->addMultipleWord( nWordParts, wordTypeNr, multipleWordItem );
		}


	// Protected query functions

	void WordItem::countQuery()
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->countQueryInList();
			}
		}

	void WordItem::clearQuerySelections()
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->clearQuerySelectionsInList();
			}
		}

	void WordItem::itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			}
		}

	void WordItem::listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListString );
			}
		}

	void WordItem::parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );
			}
		}

	void WordItem::wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );
			}
		}

	bool WordItem::hasActiveQuestionWithCompoundCollection()
		{
		if( specificationList_ != NULL )
			return specificationList_->hasActiveQuestionWithCompoundCollection();

		return false;
		}

	signed char WordItem::displayQueryResultInWord( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displayQueryResultInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			currentWordList->displayQueryResultInList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to display the items" );
			}

		return RESULT_OK;
		}

	signed char WordItem::stringQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString )
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "stringQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			currentWordList->stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query strings" );
			}

		return RESULT_OK;
		}

	signed char WordItem::wordQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *wordNameString )
		{
		bool hasFoundMatchingString;
		List *currentWordList;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "wordQueryInWord";

		if( ( boolResult = findMatchingWordReferenceString( wordNameString ) ).result != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to find words" );

		hasFoundMatchingString = boolResult.booleanValue;

		if( hasFoundMatchingString )
			{
			if( isSelectingOnFind &&
			!isSelectedByQuery )
				{
				isSelectedByQuery = true;
				globalVariables()->hasFoundQuery = true;
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
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->wordQueryInList( ( hasFoundMatchingString && isSelectingOnFind ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems );
			}

		return RESULT_OK;
		}

	signed char WordItem::wordReferenceQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		bool isAssignmentOrSpecificationList;
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "wordReferenceQueryInWord";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				{
				isAssignmentOrSpecificationList = ( wordListNr == WORD_ASSIGNMENT_LIST ||
													wordListNr == WORD_SPECIFICATION_LIST );

				if( currentWordList->wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, ( isAssignmentOrSpecificationList && isSelectingAttachedJustifications ), ( isAssignmentOrSpecificationList && isSelectingJustificationSpecifications), wordReferenceNameString ) != RESULT_OK )
					return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to query word references" );
				}
			}

		return RESULT_OK;
		}


	// Protected question functions

	bool WordItem::hasCurrentlyAnsweredSelfGeneratedQuestion()
		{
		if( wordQuestion_ != NULL )
			return wordQuestion_->hasCurrentlyAnsweredSelfGeneratedQuestion();

		return false;
		}

	bool WordItem::hasFoundAnswerToQuestion()
		{
		if( wordQuestion_ != NULL )
			return wordQuestion_->hasFoundAnswerToQuestion();

		return false;
		}

	signed char WordItem::findAnswerToNewUserQuestion()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswerToNewUserQuestion";

		if( wordQuestion_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word question module" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word question module" );
			}

		return wordQuestion_->findAnswerToNewUserQuestion();
		}

	signed char WordItem::findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswersToQuestions";

		if( wordQuestion_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word question module" );

			// Create supporting module
			if( ( wordQuestion_ = new WordQuestion( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word question module" );
			}

		return wordQuestion_->findAnswersToQuestions( compoundSpecificationCollectionNr, answerSpecificationItem );
		}

	signed char WordItem::writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeAnswerToQuestion";

		if( wordQuestion_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word question module isn't created yet" );

		return wordQuestion_->writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );
		}


	// Protected specification functions

	void WordItem::checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->checkJustificationForUsage( false, false, false, unusedJustificationItem ) == RESULT_OK &&
		assignmentList_->checkJustificationForUsage( true, false, false, unusedJustificationItem ) == RESULT_OK &&
		assignmentList_->checkJustificationForUsage( false, true, false, unusedJustificationItem ) == RESULT_OK )
			assignmentList_->checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL &&
		specificationList_->checkJustificationForUsage( false, false, false, unusedJustificationItem ) == RESULT_OK )
			specificationList_->checkJustificationForUsage( false, false, true, unusedJustificationItem );

		if( globalVariables()->result == RESULT_OK &&
		justificationList_ != NULL )
			justificationList_->checkJustificationForUsage( unusedJustificationItem );
		}

	void WordItem::initializeVariablesInWord()
		{
		isUserGeneralizationWord = false;
		isUserSpecificationWord = false;
		isUserRelationWord = false;
		isWordCheckedForSolving = false;

		nextTouchedWordItem = NULL;

		if( assignmentList_ != NULL )
			{
			assignmentList_->initializeSpecificationVariables( false, false );
			assignmentList_->initializeSpecificationVariables( true, false );
			assignmentList_->initializeSpecificationVariables( false, true );
			}

		if( justificationList_ != NULL )
			justificationList_->initializeJustificationVariables();

		if( specificationList_ != NULL )
			specificationList_->initializeSpecificationVariables( false, false );

		if( wordQuestion_ != NULL )
			wordQuestion_->initializeWordQuestionVariables();

		if( wordSpecification_ != NULL )
			wordSpecification_->initializeWordSpecificationVariables();
		}

	bool WordItem::hadOnceAnsweredSelfGeneratedQuestion()
		{
		if( specificationList_ != NULL )
			return specificationList_->hadOnceAnsweredSelfGeneratedQuestion();

		return false;
		}

	bool WordItem::hasAnyUserSpecification()
		{
		if( specificationList_ != NULL )
			return specificationList_->hasAnyUserSpecification();

		return false;
		}

	bool WordItem::hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();

		return false;
		}

	bool WordItem::hasCurrentlyConfirmedSpecificationButNoRelation()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasCurrentlyConfirmedSpecificationButNoRelation();

		return false;
		}

	bool WordItem::hasCurrentlyConfirmedSpecification()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasCurrentlyConfirmedSpecification();

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

	bool WordItem::hasDisplayedMoreSpecificSpecification()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasDisplayedMoreSpecificSpecification();

		return false;
		}

	bool WordItem::hasDisplayedMoreSpecificNonExclusiveSpecification()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasDisplayedMoreSpecificNonExclusiveSpecification();

		return false;
		}

	bool WordItem::hasDisplayedMoreSpecificQuestion()
		{
		if( wordSpecification_ != NULL )
			return wordSpecification_->hasDisplayedMoreSpecificQuestion();

		return false;
		}

	bool WordItem::hasMultipleSpecificationWordsWithSameSentenceNr( unsigned int creationSentenceNr, unsigned int notThisItemNr, unsigned int specificationCollectionNr )
		{
		if( specificationList_ != NULL )
			return specificationList_->hasMultipleSpecificationWordsWithSameSentenceNr( creationSentenceNr, notThisItemNr, specificationCollectionNr );

		return false;
		}

	bool WordItem::hasPartOfSpecification()
		{
		if( specificationList_ != NULL )
			return specificationList_->hasPartOfSpecification();

		return false;
		}

	bool WordItem::hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationItem );

		return true;
		}

	bool WordItem::hasRelationContextInSpecificationsInWord( unsigned int relationContextNr )
		{
		if( assignmentList_ != NULL &&

		( assignmentList_->hasRelationContextInSpecifications( false, false, false, relationContextNr ) ||
		assignmentList_->hasRelationContextInSpecifications( true, false, false, relationContextNr ) ||
		assignmentList_->hasRelationContextInSpecifications( false, true, false, relationContextNr ) ||
		assignmentList_->hasRelationContextInSpecifications( false, false, true, relationContextNr ) ) )
			return true;

		if( specificationList_ != NULL &&

		( specificationList_->hasRelationContextInSpecifications( false, false, false, relationContextNr ) ||
		specificationList_->hasRelationContextInSpecifications( false, false, true, relationContextNr ) ) )
			return true;

		return false;
		}

	bool WordItem::isJustificationInUse( JustificationItem *unusedJustificationItem )
		{
		if( unusedJustificationItem != NULL )
			{
			if( unusedJustificationItem->isReplacedItem() )
				return true;

			if( assignmentList_ != NULL )
				{
				if( assignmentList_->isJustificationInUse( false, false, unusedJustificationItem ) )
					return true;

				if( assignmentList_->isJustificationInUse( true, false, unusedJustificationItem ) )
					return true;

				if( assignmentList_->isJustificationInUse( false, true, unusedJustificationItem ) )
					return true;
				}

			if( specificationList_ != NULL &&
			specificationList_->isJustificationInUse( false, false, unusedJustificationItem ) )
				return true;
			}

		return false;
		}

	unsigned int WordItem::nRemainingSpecificationWordsForWriting( bool isAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, unsigned int creationSentenceNr )
		{
		if( isAssignment )
			{
			if( assignmentList_ != NULL )
				return assignmentList_->nRemainingSpecificationWordsForWriting( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, creationSentenceNr );
			}
		else
			{
			if( specificationList_ != NULL )
				return specificationList_->nRemainingSpecificationWordsForWriting( false, isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, creationSentenceNr );
			}

		return 0;
		}

	signed char WordItem::changeJustificationOfNegativeAssumptions( SpecificationItem *secondarySpecificationItem )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem ) == RESULT_OK &&
		assignmentList_->changeJustificationOfNegativeAssumptions( true, false, secondarySpecificationItem ) == RESULT_OK )
			assignmentList_->changeJustificationOfNegativeAssumptions( false, true, secondarySpecificationItem );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->changeJustificationOfNegativeAssumptions( false, false, secondarySpecificationItem );

		return globalVariables()->result;
		}

	signed char WordItem::checkForSpecificationConflict( bool isArchivedAssignment, bool isGeneralizationProperNoun, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForSpecificationConflict";

		if( wordSpecification_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );
		}

	signed char WordItem::clearStoredSentenceStringWithUnknownPluralNoun( const char *unknownPluralNounString, WordItem *specificationWordItem )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->clearStoredSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem ) == RESULT_OK &&
		assignmentList_->clearStoredSentenceStringWithUnknownPluralNoun( true, false, unknownPluralNounString, specificationWordItem ) == RESULT_OK )
			assignmentList_->clearStoredSentenceStringWithUnknownPluralNoun( false, true, unknownPluralNounString, specificationWordItem );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->clearStoredSentenceStringWithUnknownPluralNoun( false, false, unknownPluralNounString, specificationWordItem );

		return globalVariables()->result;
		}

	signed char WordItem::collectGeneralizations( bool isExclusiveGeneralization, unsigned int generalizationCollectionNr )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->collectGeneralizations( false, false, isExclusiveGeneralization, generalizationCollectionNr ) == RESULT_OK &&
		assignmentList_->collectGeneralizations( true, false, isExclusiveGeneralization, generalizationCollectionNr ) == RESULT_OK )
			assignmentList_->collectGeneralizations( false, true, isExclusiveGeneralization, generalizationCollectionNr );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->collectGeneralizations( false, false, isExclusiveGeneralization, generalizationCollectionNr );

		return globalVariables()->result;
		}

	signed char WordItem::collectSpecificationsInWord( bool isExclusiveSpecification, bool isQuestion, unsigned int specificationCollectionNr )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->collectSpecificationsInList( false, false, isExclusiveSpecification, isQuestion, specificationCollectionNr ) == RESULT_OK &&
		assignmentList_->collectSpecificationsInList( true, false, isExclusiveSpecification, isQuestion, specificationCollectionNr ) == RESULT_OK )
			assignmentList_->collectSpecificationsInList( false, true, isExclusiveSpecification, isQuestion, specificationCollectionNr );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->collectSpecificationsInList( false, false, isExclusiveSpecification, isQuestion, specificationCollectionNr );

		return globalVariables()->result;
		}

	signed char WordItem::confirmSpecificationButNotItsRelation( SpecificationItem *confirmationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "confirmSpecificationButNotItsRelation";

		if( specificationList_ != NULL )
			return specificationList_->confirmSpecificationButNotItsRelation( confirmationSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I have no specifications" );
		}

	signed char WordItem::copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyAndReplaceSpecificationItem";

		if( originalSpecificationItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given original specification item is undefined" );

		if( originalSpecificationItem->isAssignment() )
			{
			if( assignmentList_ == NULL )
				return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList_->copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveGeneralization, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
			}

		if( specificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My specification list isn't created yet" );

		return specificationList_->copyAndReplaceSpecificationItem( isNewAnsweredQuestion, isNewExclusiveGeneralization, isNewExclusiveSpecification, newGeneralizationCollectionNr, newSpecificationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
		}

	signed char WordItem::recalculateAssumptionsInWord()
		{
		if( assignmentList_ != NULL )
			// Recalculate archived assumptions
			assignmentList_->recalculateAssumptions( false, true );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->recalculateAssumptions( false, false );

		return globalVariables()->result;
		}

	signed char WordItem::replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isAssignment() )
			{
			if( assignmentList_ == NULL )
				return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList_->replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );
			}

		if( specificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The specification list isn't created yet" );

		return specificationList_->replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem );
		}

	signed char WordItem::updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		bool isIncludingReplacedItems = ( obsoleteJustificationItem != NULL &&
										obsoleteJustificationItem->hasCurrentCreationSentenceNr() );

		if( assignmentList_ != NULL &&
		assignmentList_->updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList_->updateJustificationInSpecifications( true, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList_->updateJustificationInSpecifications( false, true, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			{
			if( isIncludingReplacedItems )
				assignmentList_->updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL &&
		specificationList_->updateJustificationInSpecifications( false, false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			{
			if( isIncludingReplacedItems )
				specificationList_->updateJustificationInSpecifications( false, false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );
			}

		return globalVariables()->result;
		}

	signed char WordItem::updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->updateSpecificationsInJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem );

		return RESULT_OK;
		}

	signed char WordItem::writeConfirmedSpecification( unsigned short interfaceParameter, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeConfirmedSpecification";

		if( wordSpecification_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->writeConfirmedSpecification( interfaceParameter, writeSpecificationItem );
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingArchivedAssignments &&
		assignmentList_ != NULL )
			foundSpecificationItem = assignmentList_->bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, specificationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, specificationCollectionNr, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, specificationWordItem, relationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->bestMatchingRelationContextNrSpecificationItem( true, isNegative, isPossessive, specificationWordItem, relationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->bestMatchingRelationContextNrSpecificationItem( false, isNegative, isPossessive, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingAnsweredQuestions, bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem );

			if( isIncludingInactiveAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->bestMatchingRelationContextNrSpecificationItem( isAllowingEmptyRelationContext, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingArchivedAssignments &&
		assignmentList_ != NULL )
			foundSpecificationItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationCollectionNr, specificationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationCollectionNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyGeneralizationContext, bool isAllowingEmptyRelationContext, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyGeneralizationContext, isAllowingEmptyRelationContext, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyGeneralizationContext, isAllowingEmptyRelationContext, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyGeneralizationContext, isAllowingEmptyRelationContext, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstActiveQuestionSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstActiveSpecificationItem( false, true );

		return NULL;
		}

	SpecificationItem *WordItem::firstAssignmentOrSpecificationItem( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			foundSpecificationItem = assignmentList_->firstSpecificationItem( isNegative, isPossessive, relationContextNr, specificationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->firstSpecificationItem( isNegative, isPossessive, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstAssignmentOrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			{
			if( isIncludingActiveAssignments )
				foundSpecificationItem = assignmentList_->firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem );

			if( isIncludingArchivedAssignments &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = assignmentList_->firstSpecificationItem( true, isNegative, isPossessive, isQuestion, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstExclusiveSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstExclusiveSpecificationItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstExclusiveSpecificationItem( WordItem *specificationWordItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->firstExclusiveSpecificationItem( specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstFeminineOrMasculineSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstFeminineOrMasculineSpecificationItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstNonCompoundUserSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstNonCompoundUserSpecificationItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstNonCompoundCollectionSpecificationItem( unsigned int compoundSpecificationCollectionNr )
		{
		if( specificationList_ != NULL )
			return specificationList_->firstNonCompoundCollectionSpecificationItem( compoundSpecificationCollectionNr );

		return NULL;
		}

	SpecificationItem *WordItem::firstNonPossessiveDefinitionSpecificationItem( bool isIncludingAdjectives )
		{
		if( specificationList_ != NULL )
			return specificationList_->firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives );

		return NULL;
		}

	SpecificationItem *WordItem::firstNonQuestionSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstActiveSpecificationItem( false, false );

		return NULL;
		}

	SpecificationItem *WordItem::firstPossessiveSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstPossessiveSpecificationItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstPrimarySpecificationCandidateOfQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification )
		{
		SpecificationItem *firstSelectedSpecificationItem;

		if( ( firstSelectedSpecificationItem = firstSpecificationItem( false, false, false, false ) ) != NULL )
			return firstSelectedSpecificationItem->primarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, true );

		return NULL;
		}

	SpecificationItem *WordItem::firstRecentlyAnsweredQuestionSpecificationItem()
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL &&
		( foundSpecificationItem = assignmentList_->firstRecentlyAnsweredQuestionSpecificationItem( false ) ) == NULL )
			foundSpecificationItem = assignmentList_->firstRecentlyAnsweredQuestionSpecificationItem( true );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return foundSpecificationItem = specificationList_->firstRecentlyAnsweredQuestionSpecificationItem( false );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstRelationSpecificationItem()
		{
		if( specificationList_ != NULL )
			return specificationList_->firstRelationSpecificationItem();

		return NULL;
		}

	SpecificationItem *WordItem::firstSelectedRelationSpecificationItem( bool isAssignment, bool isArchivedAssignment )
		{
		if( isAssignment )
			{
			if( assignmentList_ != NULL )
				return assignmentList_->firstSelectedRelationSpecificationItem( isArchivedAssignment );
			}
		else
			{
			if( specificationList_ != NULL )
				return specificationList_->firstSelectedRelationSpecificationItem( false );
			}

		return NULL;
		}

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *moreRecentSpecificationItem;
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL &&
		( foundSpecificationItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem ) ) == NULL )
			foundSpecificationItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( true, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem );

		if( specificationList_ != NULL &&
		( moreRecentSpecificationItem = specificationList_->firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem ) ) != NULL )
			{
			if( foundSpecificationItem == NULL ||

			// Prefer more recent specification over assignment
			( foundSpecificationItem->hasRelationContext() == moreRecentSpecificationItem->hasRelationContext() &&
			foundSpecificationItem->originalSentenceNr() < moreRecentSpecificationItem->originalSentenceNr() ) )
				return moreRecentSpecificationItem;
			}

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL &&
		( foundSpecificationItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == NULL &&

		isIncludingArchivedAssignments )
			foundSpecificationItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, true, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( isIncludingArchivedAssignments &&
		assignmentList_ != NULL )
			foundSpecificationItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelationContext, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		return ( isAssignment ? firstAssignmentItem( isInactiveAssignment, isArchivedAssignment, isQuestion ) :
				( isQuestion ? firstActiveQuestionSpecificationItem() : firstNonQuestionSpecificationItem() ) );
		}

	SpecificationItem *WordItem::firstSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		if( isIncludingAnsweredQuestions &&
		questionParameter > NO_QUESTION_PARAMETER )
			{
			if( isAssignment )
				{
				if( assignmentList_ != NULL )
					return assignmentList_->firstAssignmentItem( true, isInactiveAssignment, isArchivedAssignment, questionParameter );
				}
			else
				{
				if( specificationList_ != NULL )
					return specificationList_->firstActiveSpecificationItem( true, questionParameter );
				}
			}
		else
			{
			if( isAssignment )
				{
				if( assignmentList_ != NULL )
					{
					if( isArchivedAssignment )
						return assignmentList_->firstAssignmentItem( false, false, true, questionParameter );

					if( isInactiveAssignment )
						return assignmentList_->firstAssignmentItem( false, true, false, questionParameter );

					return assignmentList_->firstAssignmentItem( false, false, false, questionParameter );
					}
				}
			else
				{
				if( questionParameter == NO_QUESTION_PARAMETER )
					return firstNonQuestionSpecificationItem();

				if( specificationList_ != NULL )
					return specificationList_->firstActiveSpecificationItem( false, questionParameter );
				}
			}

		return NULL;
		}

	SpecificationItem *WordItem::firstSpecificationItem( bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::firstUnhiddenSpanishSpecificationItem()
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL )
			foundSpecificationItem = assignmentList_->firstUnhiddenSpanishSpecificationItem( true );

		if( specificationList_ != NULL )
			return specificationList_->firstUnhiddenSpanishSpecificationItem( false );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL &&
		( foundSpecificationItem = assignmentList_->firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( ( foundSpecificationItem = assignmentList_->firstUserSpecificationItem( true, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ) == NULL )
				foundSpecificationItem = assignmentList_->firstUserSpecificationItem( false, true, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );
			}

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->firstUserSpecificationItem( false, false, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem );

		return foundSpecificationItem;
		}

	SpecificationItem *WordItem::noRelationContextSpecificationItem( bool isPossessive, bool isSelfGenerated, WordItem *specificationWordItem )
		{
		if( specificationList_ != NULL &&

		( assignmentList_ == NULL ||
		// Skip if archived assignment with relation context is found
		assignmentList_->firstSpecificationItem( true, false, isPossessive, false, specificationWordItem ) == NULL ) )
			return specificationList_->noRelationContextSpecificationItem( isPossessive, isSelfGenerated, specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::partOfSpecificationItem( WordItem *specificationWordItem )
		{
		if( specificationList_ != NULL )
			return specificationList_->partOfSpecificationItem( specificationWordItem );

		return NULL;
		}

	SpecificationItem *WordItem::sameUserQuestionSpecificationItem( unsigned short questionParameter )
		{
		SpecificationItem *foundSpecificationItem = NULL;

		if( assignmentList_ != NULL &&
		( foundSpecificationItem = assignmentList_->sameUserQuestionSpecificationItem( false, questionParameter ) ) == NULL )
			foundSpecificationItem = assignmentList_->sameUserQuestionSpecificationItem( true, questionParameter );

		if( foundSpecificationItem == NULL &&
		specificationList_ != NULL )
			return specificationList_->sameUserQuestionSpecificationItem( false, questionParameter );

		return foundSpecificationItem;
		}

	WordItem *WordItem::feminineOrMasculineCommonWordItem( bool isFeminineWord )
		{
		if( specificationList_ != NULL )
			return specificationList_->feminineOrMasculineCommonWordItem( isFeminineWord );

		return NULL;
		}

	BoolResultType WordItem::findQuestionToBeAdjustedByCompoundCollection( unsigned int questionSpecificationCollectionNr, WordItem *primarySpecificationWordItem )
		{
		BoolResultType boolResult;

		if( specificationList_ != NULL )
			return specificationList_->findQuestionToBeAdjustedByCompoundCollection( questionSpecificationCollectionNr, primarySpecificationWordItem );

		return boolResult;
		}

	CollectionResultType WordItem::collectSpecifications( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentSpecificationWordItem = NULL;
		WordItem *foundSpecificationWordItem = NULL;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectSpecifications";

		if( generalizationWordItem == NULL )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given generalization word item is undefined" );

		if( generalizationWordItem == this &&
		// Accept Spanish ambiguous definition
		!isNounWordSpanishAmbiguous() )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given generalization word item is the same as my word" );

		if( collectionWordItem == NULL )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given collected word item is undefined" );

		if( collectionWordItem == this )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given collected word item is the same as my word" );

		if( collectionWordTypeNr != WORD_TYPE_NOUN_SINGULAR )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given collection word type isn't a singular noun" );

		if( commonWordTypeNr != WORD_TYPE_NOUN_SINGULAR )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given common word type isn't a singular noun" );

		if( ( currentSpecificationItem = firstExclusiveSpecificationItem() ) != NULL )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem &&
				( foundSpecificationItem = currentSpecificationWordItem->firstNonQuestionSpecificationItem() ) != NULL &&
				( foundSpecificationWordItem = foundSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( ( boolResult = foundSpecificationWordItem->findCollection( false, collectionWordItem, this ) ).result != RESULT_OK )
						return addCollectionResultErrorInWord( functionNameString, NULL, "I failed to find out if word \"", collectionWordItem->anyWordTypeString(), "\" is collected with word \"", foundSpecificationWordItem->anyWordTypeString(), "\"" );

					// No collection found
					if( !boolResult.booleanValue )
						{
						if( ( collectionResult = foundSpecificationWordItem->createCollection( false, collectionWordTypeNr, commonWordTypeNr, NO_COLLECTION_NR, collectionWordItem, this, NULL ) ).result != RESULT_OK )
							return addCollectionResultErrorInWord( functionNameString, NULL, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", collectionWordItem->anyWordTypeString(), "\"" );

						if( collectionWordItem->createCollection( false, collectionWordTypeNr, commonWordTypeNr, collectionResult.createdCollectionNr, foundSpecificationWordItem, this, NULL ).result != RESULT_OK )
							return addCollectionResultErrorInWord( functionNameString, NULL, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", collectionWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( foundSpecificationWordItem == NULL &&
			( currentSpecificationItem = currentSpecificationItem->nextExclusiveNounSpecificationItem() ) != NULL );
			}

		collectionResult.foundGeneralizationWordItem = currentSpecificationWordItem;
		return collectionResult;
		}

	CreateAndAssignResultType WordItem::addSpecificationInWord( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *authorizationKey )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSpecificationInWord";

		if( !isAuthorizedForChanges( authorizationKey ) ||

		( specificationWordItem != NULL &&
		// No user is logged in yet
		globalVariables()->currentUserNr > NO_USER_NR &&
		!specificationWordItem->isAuthorizedForChanges( authorizationKey ) ) )
			return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "You are not authorized to add this word" );

		if( wordSpecification_ == NULL )
			{
			if( isAdminWord() )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a word specification module" );

			// Create supporting module
			if( ( wordSpecification_ = new WordSpecification( globalVariables(), inputOutput(), this ) ) == NULL )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to create my word specification module" );
			}

		return wordSpecification_->addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );
		}

	CreateAndAssignResultType WordItem::copySpecificationItem( unsigned int specificationCollectionNr, JustificationItem *firstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copySpecificationItem";

		if( originalSpecificationItem == NULL )
			return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "The given original specification item is undefined" );

		return createSpecificationItem( originalSpecificationItem->isAssignment(), originalSpecificationItem->isInactiveAssignment(), originalSpecificationItem->isArchivedAssignment(), originalSpecificationItem->isAnsweredQuestion(), originalSpecificationItem->isCharacteristicFor(), originalSpecificationItem->isConcludedAssumption(), originalSpecificationItem->isConditional(), originalSpecificationItem->isCorrectedAssumption(), originalSpecificationItem->isEveryGeneralization(), originalSpecificationItem->isExclusiveGeneralization(), originalSpecificationItem->isExclusiveSpecification(), originalSpecificationItem->isNegative(), originalSpecificationItem->isPartOf(), originalSpecificationItem->isPossessive(), originalSpecificationItem->isSpecific(), originalSpecificationItem->isSpecificationGeneralization(), originalSpecificationItem->isUncountableGeneralizationNoun(), originalSpecificationItem->isUniqueUserRelation(), originalSpecificationItem->isValueSpecification(), originalSpecificationItem->assignmentLevel(), originalSpecificationItem->assumptionLevel(), originalSpecificationItem->languageNr(), originalSpecificationItem->prepositionParameter(), originalSpecificationItem->questionParameter(), originalSpecificationItem->generalizationWordTypeNr(), originalSpecificationItem->specificationWordTypeNr(), originalSpecificationItem->relationWordTypeNr(), specificationCollectionNr, originalSpecificationItem->generalizationContextNr(), originalSpecificationItem->specificationContextNr(), originalSpecificationItem->relationContextNr(), originalSpecificationItem->originalSentenceNr(), originalSpecificationItem->activeSentenceNr(), originalSpecificationItem->inactiveSentenceNr(), originalSpecificationItem->archivedSentenceNr(), originalSpecificationItem->nContextRelations(), firstJustificationItem, originalSpecificationItem->specificationWordItem(), originalSpecificationItem->specificationString(), originalSpecificationItem->storedSentenceString(), originalSpecificationItem->storedSentenceWithOnlyOneSpecificationString() );
		}

	CreateAndAssignResultType WordItem::createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isCharacteristicFor, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createSpecificationItem";

		if( isAssignment )
			{
			if( assignmentList_ == NULL )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList_->createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isCharacteristicFor, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( isExclusiveGeneralization ? collectionNr( specificationWordItem ) : NO_COLLECTION_NR ), specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceString, storedSentenceWithOnlyOneSpecificationString );
			}

		if( specificationList_ == NULL )
			{
			if( isAdminWord() )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a specification list" );

			// Create specification list
			if( ( specificationList_ = new SpecificationList( WORD_SPECIFICATION_LIST_SYMBOL, globalVariables(), inputOutput(), this ) ) == NULL )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to create a specification list" );

			wordListArray_[WORD_SPECIFICATION_LIST] = specificationList_;
			}

		return specificationList_->createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isCharacteristicFor, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_COLLECTION_NR, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceString, storedSentenceWithOnlyOneSpecificationString );
		}

	RelatedResultType WordItem::findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ == NULL )
			return startRelatedResultErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );
		}

	RelatedResultType WordItem::findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ == NULL )
			return startRelatedResultErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->findRelatedSpecification( false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );
		}


	// Protected word type functions

	void WordItem::clearGeneralizationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList_ != NULL )
			wordTypeList_->clearGeneralizationWriteLevel( isLanguageWord_, currentWriteLevel );
		}

	void WordItem::clearSpecificationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList_ != NULL )
			wordTypeList_->clearSpecificationWriteLevel( currentWriteLevel );
		}

	void WordItem::clearRelationWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		if( contextList_ != NULL &&
		wordTypeList_ != NULL &&
		contextList_->hasContext( contextNr ) )
			wordTypeList_->clearRelationWriteLevel( currentWriteLevel );
		}

	bool WordItem::hasAnyWordType()
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->hasAnyWordType();

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

	bool WordItem::hasFeminineProperNounEnding()
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL )
			return foundWordTypeItem->hasFeminineWordEnding();

		return false;
		}

	bool WordItem::hasFeminineOrMasculineProperNounEnding()
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL )
			return foundWordTypeItem->hasFeminineOrMasculineWordEnding();

		return false;
		}

	bool WordItem::hasMasculineProperNounEnding()
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL )
			return foundWordTypeItem->hasMasculineWordEnding();

		return false;
		}

	bool WordItem::hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr )
		{
		if( wordTypeList_ != NULL )
			return ( wordTypeList_->activeWordTypeItem( isAllowingDifferentNoun, isLanguageWord_, wordTypeNr ) != NULL );

		return false;
		}

	bool WordItem::isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( true, isLanguageWord_, specificationWordTypeNr ) ) != NULL )
			return foundWordTypeItem->isSpecificationWordAlreadyWritten();

		return false;
		}

	bool WordItem::isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( true, isLanguageWord_, relationWordTypeNr ) ) != NULL )
			return foundWordTypeItem->isRelationWordAlreadyWritten();

		return false;
		}

	bool WordItem::isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectAdjective( adjectiveParameter );

		return false;
		}

	bool WordItem::isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectDefiniteArticle( definiteArticleParameter );

		return false;
		}

	bool WordItem::isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&

		( ( !isFeminineWord_ &&
		!isMasculineWord_ ) ||

		( isFeminineWord_ &&
		isFeminineArticleParameter( indefiniteArticleParameter ) ) ||

		( isMasculineWord_ &&
		isMasculineArticleParameter( indefiniteArticleParameter ) ) ) &&

		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->isCorrectIndefiniteArticle( true, indefiniteArticleParameter );

		return false;
		}

	bool WordItem::isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *hideKey )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->isCorrectHiddenWordType( wordTypeNr, compareString, hideKey );

		return false;
		}

	bool WordItem::isNumeralWordType()
		{
		return hasWordType( false, WORD_TYPE_NUMERAL );
		}

	bool WordItem::isProperNounPrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		WordTypeItem *foundWordTypeItem;

		if( wordTypeList_ != NULL &&
		( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL )
			return foundWordTypeItem->isProperNounPrecededByDefiniteArticle( definiteArticleParameter );

		return false;
		}

	bool WordItem::isProperNounWord()
		{
		return hasWordType( false, WORD_TYPE_PROPER_NOUN );
		}

	signed char WordItem::deleteAllWordTypesOfCurrentSentence()
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->deleteAllWordTypesOfCurrentSentence();

		return RESULT_OK;
		}

	signed char WordItem::deleteWordType( WordTypeItem *deleteWordTypeItem )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->deleteWordType( deleteWordTypeItem );

		return RESULT_OK;
		}

	signed char WordItem::hideWordType( unsigned short wordTypeNr, char *authorizationKey )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->hideWordTypeItem( wordTypeNr, authorizationKey );

		return RESULT_OK;
		}

	signed char WordItem::markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->markGeneralizationWordTypeAsWritten( isLanguageWord_, wordTypeNr );
		}

	signed char WordItem::markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->markSpecificationWordTypeAsWritten( wordTypeNr );
		}

	signed char WordItem::markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->markRelationWordTypeAsWritten( wordTypeNr );
		}

	WordTypeItem *WordItem::activeWordTypeItem( unsigned short wordTypeNr )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr );

		return NULL;
		}

	WordTypeItem *WordItem::activeWordTypeItem( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr );

		return NULL;
		}

	char *WordItem::activeWordTypeString( unsigned short wordTypeNr )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->activeWordTypeString( isLanguageWord_, wordTypeNr );

		return NULL;
		}

	char *WordItem::anyWordTypeString()
		{
		return wordTypeString( true, NO_WORD_TYPE_NR );
		}

	char *WordItem::singularNounString()
		{
		return activeWordTypeString( WORD_TYPE_NOUN_SINGULAR );
		}

	char *WordItem::wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		char *wordTypeString;

		if( wordTypeList_ == NULL ||
		( wordTypeString = wordTypeList_->wordTypeString( ( isCheckingAllLanguages || isLanguageWord_ ), wordTypeNr ) ) == NULL )
			{
			sprintf( wordItemNameString_, "%c%u%c%u%c", QUERY_ITEM_START_CHAR, creationSentenceNr(), QUERY_SEPARATOR_CHAR, itemNr(), QUERY_ITEM_END_CHAR );
			return wordItemNameString_;
			}

		return wordTypeString;
		}

	WordTypeResultType WordItem::addWordType( bool isMultipleWord, bool isProperNounPrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addWordType";

		if( wordTypeList_ == NULL )
			{
			if( isAdminWord() )
				return startWordTypeResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a word type list" );

			// Create list
			if( ( wordTypeList_ = new WordTypeList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startWordTypeResultErrorInWord( functionNameString, NULL, "I failed to create a word type list" );

			wordListArray_[WORD_TYPE_LIST] = wordTypeList_;
			}

		return wordTypeList_->addWordType( isLanguageWord_, isMultipleWord, isProperNounPrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString );
		}

	WordResultType WordItem::findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString )
		{
		WordResultType wordResult;

		if( wordTypeList_ != NULL )
			return wordTypeList_->findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString );

		return wordResult;
		}


	// Protected write functions

	void WordItem::deleteTemporaryWriteList()
		{
		if( writeList_ != NULL )
			writeList_->deleteTemporaryList();
		}

	signed char WordItem::createWriteWord( bool isSkipped, unsigned short grammarLevel, GrammarItem *selectedGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createWriteWord";

		if( writeList_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a write list" );

			// Create list
			if( ( writeList_ = new WriteList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create a write list" );

			wordListArray_[WORD_WRITE_LIST] = writeList_;
			}

		return writeList_->createWriteItem( isSkipped, grammarLevel, selectedGrammarItem );
		}

	signed char WordItem::writeJustificationSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeJustificationSpecification";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, justificationSpecificationItem );
		}

	signed char WordItem::writeSelectedRelationInfo( bool isActiveAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedRelationInfo";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeSelectedRelationInfo( isActiveAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );
		}

	signed char WordItem::writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem )
		{
		return writeSelectedSpecification( false, isCheckingUserSentenceForIntegrity, true, true, false, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, writeSpecificationItem );
		}

	signed char WordItem::writeSelectedSpecification( bool isAdjustedAssumption, bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isJustification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedSpecification";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word write module" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeSelectedSpecification( isAdjustedAssumption, isCheckingUserSentenceForIntegrity, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isJustification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );
		}

	signed char WordItem::writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedSpecificationInfo";

		if( wordWrite_ == NULL )
			{
			if( isAdminWord() )
				return startErrorInWord( functionNameString, NULL, "The admin word item cannot have a word write module" );

			// Create supporting module
			if( ( wordWrite_ = new WordWrite( globalVariables(), inputOutput(), this ) ) == NULL )
				return startErrorInWord( functionNameString, NULL, "I failed to create my word write module" );
			}

		return wordWrite_->writeSelectedSpecificationInfo( isAssignment, isInactiveAssignment, isArchivedAssignment, writeWordItem );
		}

	signed char WordItem::writeSpecificationSentence( bool isAssignment, bool isArchivedAssignment, bool isCheckingUserSentenceForIntegrity, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationSentence";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeSpecificationSentence( isAssignment, isArchivedAssignment, isCheckingUserSentenceForIntegrity, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );
		}

	signed char WordItem::writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, bool isReplacedBySpecificationWithRelation, bool wasHiddenSpanishSpecification, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeUpdatedSpecification";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeUpdatedSpecification( isAdjustedSpecification, isCorrectedAssumptionByKnowledge, isCorrectedAssumptionByOppositeQuestion, isReplacedBySpecificationWithRelation, wasHiddenSpanishSpecification, writeSpecificationItem );
		}

	WriteItem *WordItem::firstActiveWriteItem()
		{
		if( writeList_ != NULL )
			return writeList_->firstActiveWriteItem();

		return NULL;
		}

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
