/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		Storing and processing word information
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
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

		return ( ( foundLanguageWordItem = languageWordItem( languageNr ) ) != NULL ?
				foundLanguageWordItem->anyWordTypeString() : (char *)NO_LANGUAGE_NAME_FOUND_STRING );
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

	signed char WordItem::inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundActiveAssignmentItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *relatedSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "inactivateAssignment";

		if( ( foundAssignmentItem = firstActiveNonQuestionAssignmentItem( relationCollectionNr, specificationWordItem, ( isAmbiguousRelationContext ? NULL : relationWordItem ) ) ) == NULL ||

		// Typical for Spanish
		// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
		( !isPossessive &&
		foundAssignmentItem->isSpecificationWordSpanishAmbiguous() &&
		!foundAssignmentItem->hasSpecificationCollection() ) )
			{
			if( relationWordItem == NULL )
				{
				if( isExclusiveSpecification )
					{
					if( ( relatedResult = findRelatedSpecification( true, true, false, isExclusiveSpecification, false, isPossessive, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, NO_COLLECTION_NR, generalizationContextNr, specificationWordItem, NULL ) ).result != RESULT_OK )
						return addErrorInWord( functionNameString, NULL, "I failed to find a related assignment" );

					relatedSpecificationItem = relatedResult.relatedSpecificationItem;

					if( relatedSpecificationItem != NULL &&
					// Skip specification
					relatedSpecificationItem->isUserAssignment() &&

						( !isNegative ||
						!relatedSpecificationItem->isNegative() ) )
						{
						if( inactivateActiveAssignment( relatedSpecificationItem ) != RESULT_OK )
							return addErrorInWord( functionNameString, NULL, "I failed to inactivate a related active assignment" );
						}
					}
				}
			else	// Has relation word
				{
				if( ( foundActiveAssignmentItem = firstAssignmentItem( true, false, false, isNegative, isSelfGenerated, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, specificationWordItem, NULL ) ) != NULL )
					{
					if( ( foundAssignmentItem = firstAssignmentItem( false, true, false, isNegative, isSelfGenerated, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, specificationWordItem, NULL ) ) != NULL &&
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
			if( !foundAssignmentItem->isUniqueUserRelation() &&
			foundAssignmentItem->isNegative() != isNegative )
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

	SpecificationItem *WordItem::firstActiveNonQuestionAssignmentItem( unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->firstActiveNonQuestionAssignmentItem( relationCollectionNr, specificationWordItem, relationWordItem ) );
		}

	SpecificationItem *WordItem::firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( assignmentList_ != NULL &&

				( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->firstAssignmentItem( false, false, isNegative, isSelfGenerated, questionParameter, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				( !isIncludingInactiveAssignments ||
				( foundAssignmentItem = assignmentList_->firstAssignmentItem( true, false, isNegative, isSelfGenerated, questionParameter, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				isIncludingArchivedAssignments ? assignmentList_->firstAssignmentItem( false, true, isNegative, isSelfGenerated, questionParameter, relationCollectionNr, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

	CreateAndAssignResultType WordItem::assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
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
		inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) != RESULT_OK )
			return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to inactivate an assignment" );

		if( ( foundAssignmentItem = firstAssignmentItem( true, true, true, isNegative, isSelfGenerated, questionParameter, relationCollectionNr, specificationWordItem, relationWordItem ) ) != NULL )
			{
			createAndAssignResult.createdOrFoundSpecificationItem = foundAssignmentItem;
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

		// Test file: "James was a father - James was the father of Peter"
		( relationWordItem != NULL &&
		!foundAssignmentItem->hasRelationWord() ) )
			{
			if( ( createAndAssignResult = createSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false, isCharacteristicFor, false, false, ( generalizationContextNr == NO_CONTEXT_NR && specificationContextNr == NO_CONTEXT_NR ? isEveryGeneralization : false ), isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, false, isUniqueUserRelation, false, globalVariables()->currentAssignmentLevel, assumptionLevel, globalVariables()->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL, NULL ) ).result != RESULT_OK )
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
					// Replace assignment with different assumption level
					replaceOrDeleteSpecification( foundAssignmentItem, ( foundAssignmentItem->isSelfGeneratedSpecification() ? createdAssignmentItem : NULL ) ) != RESULT_OK )
						return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to replace or delete an assignment with different assumption level" );
					}
				else
					{
					if( isAmbiguous )
						{
						if( foundAssignmentItem->isActiveItem() )
							{
							// Inactivate active assignment
							if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
								return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to inactivate an active assignment" );
							}
						else
							{
							if( foundAssignmentItem->isInactiveAssignment() &&
							// Avoid archiving inactive assignments that are created during this sentence
							!foundAssignmentItem->hasCurrentInactiveSentenceNr() &&
							// Archive inactive assignment
							archiveInactiveAssignment( foundAssignmentItem ) != RESULT_OK )
								return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to archive an inactive assignment" );
							}
						}
					}
				}
			}

		return createAndAssignResult;
		}


	// Private collection functions

	unsigned short WordItem::highestCollectionOrderNrInWord( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->highestCollectionOrderNr( collectionNr ) : NO_ORDER_NR );
		}

	unsigned int WordItem::highestCollectionNrInWord()
		{
		return ( collectionList_ != NULL ?
				collectionList_->highestCollectionNr() : NO_COLLECTION_NR );
		}


	// Private context functions

//Java_private_final
	ContextItem *WordItem::contextItem( unsigned int contextNr )
		{
		return ( contextList_ != NULL ?
				contextList_->contextItem( contextNr ) : NULL );
		}

	WordItem *WordItem::_nextCollectionWordItem( bool isIncludingThisItem, unsigned int collectionNr )
		{
		WordItem *searchCollectionWordItem = ( isIncludingThisItem ? this : nextCollectionWordItem );

		// Do for all context words
		while( searchCollectionWordItem != NULL )
			{
			if( searchCollectionWordItem->hasCollectionNr( collectionNr ) )
				return searchCollectionWordItem;

			searchCollectionWordItem = searchCollectionWordItem->nextCollectionWordItem;
			}

		return NULL;
		}


	// Private generalization functions

//Java_private_final
	GeneralizationItem *WordItem::firstGeneralizationItem()
		{
		return ( generalizationList_ != NULL ?
				generalizationList_->firstActiveGeneralizationItem() : NULL );
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
		if( grammarList_ != NULL &&
		grammarList_->hasMergedWords() )
			return grammarList_->shrinkMergedWordsInWriteSentence();

		// Language has no merged words
		return RESULT_OK;
		}

	char *WordItem::grammarString( unsigned short wordTypeNr )
		{
		return ( grammarList_ != NULL ?
				grammarList_->grammarStringInList( wordTypeNr ) : NULL );
		}

	GrammarItem *WordItem::firstReadingGrammarItem()
		{
		return ( grammarList_ != NULL ?
				grammarList_->firstReadingGrammarItem() : NULL );
		}

	GrammarItem *WordItem::firstWritingGrammarItem( bool isQuestion )
		{
		return ( grammarList_ != NULL ?
				grammarList_->firstWritingGrammarItem( isQuestion ) : NULL );
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
		return ( interfaceList_ != NULL ?
				interfaceList_->interfaceString( interfaceParameter ) : NULL );
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

	SpecificationItem *WordItem::firstNonNegativeUserSpecificationItem( unsigned short specificationWordTypeNr )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonNegativeUserSpecificationItem( specificationWordTypeNr ) );
		}


	// Constructor of AdminItem

	WordItem::WordItem()
		{
		// This array is not used by AdminItem. Nevertheless, initialize.
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = NULL;
		}


	// Constructor of words

	WordItem::WordItem( bool isLanguageWord, unsigned short wordParameter, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordItem", globalVariables, inputOutput, myList, this );

		// Private constructed variables

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			wordListArray_[wordListNr] = NULL;

		// Private initialized variables

		isFeminineWord_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_FEMININE );
		isMasculineWord_ = ( wordParameter == WORD_PARAMETER_ADJECTIVE_MASCULINE );

		isLanguageWord_ = isLanguageWord;

		wordParameter_ = wordParameter;
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

//Java_protected_final
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

//Java_protected_final
	CreateAndAssignResultType WordItem::startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	CreateAndAssignResultType WordItem::startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startCreateAndAssignResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

//Java_protected_final
	CreateAndAssignResultType WordItem::startCreateAndAssignResultSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startCreateAndAssignResultSystemError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

//Java_protected_final
	RelatedResultType WordItem::addRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addRelatedResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

//Java_protected_final
	RelatedResultType WordItem::startRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startRelatedResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

//Java_protected_final
	UserSpecificationResultType WordItem::addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addUserSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), errorString );
		}

	UserSpecificationResultType WordItem::addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addUserSpecificationResultError( functionNameString, moduleNameString, anyWordTypeString(), tempString );
		}

//Java_protected_final
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

	bool WordItem::isSorted( Item *_nextSortItem )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.

		// Add new word at the end of the list
		return false;
		}

	BoolResultType WordItem::findMatchingWordReferenceString( char *searchString )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findMatchingWordReferenceString";

		if( wordTypeList_ != NULL )
			return wordTypeList_->findMatchingWordReferenceString( searchString );

		boolResult.result = startError( functionNameString, NULL, anyWordTypeString(), "The word type list isn't created yet" );
		return boolResult;
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

		if( hasFeminineArticle_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isFeminine" );
			}

		if( hasMasculineArticle_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isMasculine" );
			}

		if( isFeminineWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isFeminine" );
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

//Java_protected_static_final
	void WordItem::setCurrentLanguageAsChinese()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			currentLanguageWordItem->isChineseLanguage_ = true;
		}

//Java_protected_final
	void WordItem::setHighestSentenceNr( unsigned int currentSentenceNr )
		{
		if( currentSentenceNr > highestSentenceNrInWord_ )
			highestSentenceNrInWord_ = currentSentenceNr;
		}

//Java_protected_final
	bool WordItem::isAdjectiveAssigned()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED );
		}

//Java_protected_final
	bool WordItem::isAdjectiveAssignedOrEmpty()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ASSIGNED ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EMPTY );
		}

//Java_protected_final
	bool WordItem::isAdjectiveComparison()
		{
		return ( wordParameter_ >= WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER &&
				wordParameter_ <= WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER );
		}

//Java_protected_final
	bool WordItem::isAdjectiveComparisonEqual()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_EQUAL ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_SAME );
		}

//Java_protected_final
	bool WordItem::isAdjectiveComparisonLess()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_EARLIER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LESS ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LOWER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER );
		}

//Java_protected_final
	bool WordItem::isAdjectiveComparisonMore()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_HIGHER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LARGER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_LATER ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_COMPARISON_MORE );
		}

//Java_protected_final
	bool WordItem::isAdjectiveEven()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVEN );
		}

//Java_protected_final
	bool WordItem::isAdjectiveOdd()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ODD );
		}

//Java_protected_final
	bool WordItem::isAdjectiveOddOrEven()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_ODD ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_EVEN );
		}

//Java_protected_final
	bool WordItem::isAdjectiveFeminineOrMasculine()
		{
		return ( wordParameter_ == WORD_PARAMETER_ADJECTIVE_FEMININE ||
				wordParameter_ == WORD_PARAMETER_ADJECTIVE_MASCULINE );
		}

//Java_protected_final
	bool WordItem::isAdminWord()
		{
		return ( myList() == NULL );
		}

//Java_protected_final
	bool WordItem::isAuthorizationRequiredForChanges()
		{
		return ( authorizationKey_ != NULL );
		}

//Java_protected_static_final
	bool WordItem::isChineseCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;

		return ( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL &&
				currentLanguageWordItem->isChineseLanguage_ );
		}

//Java_protected_static_final
	bool WordItem::isSpanishCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;

		return ( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL &&
				currentLanguageWordItem->isSpanishLanguage_ );
		}

//Java_protected_final
	bool WordItem::isFeminineWord()
		{
		return isFeminineWord_;
		}

//Java_protected_final
	bool WordItem::isMasculineWord()
		{
		return isMasculineWord_;
		}

//Java_protected_final
	bool WordItem::isFeminineOrMasculineWord()
		{
		return ( isFeminineWord_ ||
				isMasculineWord_ );
		}

//Java_protected_final
	bool WordItem::isChineseReversedImperativeNoun()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_HEAD ||
				wordParameter_ == WORD_PARAMETER_NOUN_NUMBER ||
				wordParameter_ == WORD_PARAMETER_NOUN_TAIL ||
				wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
		}

//Java_protected_final
	bool WordItem::isNounHead()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_HEAD );
		}

//Java_protected_final
	bool WordItem::isNounTail()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_TAIL );
		}

//Java_protected_final
	bool WordItem::isNounNumber()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_NUMBER );
		}

//Java_protected_final
	bool WordItem::isNounPassword()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_PASSWORD );
		}

//Java_protected_final
	bool WordItem::isNounValue()
		{
		return ( wordParameter_ == WORD_PARAMETER_NOUN_VALUE );
		}

//Java_protected_final
	bool WordItem::isBasicVerb()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IS ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_WAS );
		}

//Java_protected_final
	bool WordItem::isImperativeVerbDisplay()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY );
		}

//Java_protected_final
	bool WordItem::isImperativeVerbDisplayLoginOrRead()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ );
		}

//Java_protected_final
	bool WordItem::isImperativeVerbUndoOrRedo()
		{
		return ( wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO ||
				wordParameter_ == WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO );
		}

//Java_protected_static_final
	unsigned short WordItem::userNr( WordItem *userWordItem )
		{
		unsigned short userNr = nUsers();
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;
		WordItem *predefinedNounUserWordItem;

		if( ( predefinedNounUserWordItem = globalVariables()->predefinedNounUserWordItem ) != NULL &&
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

//Java_protected_final
	unsigned short WordItem::wordParameter()
		{
		return wordParameter_;
		}

//Java_protected_final
	unsigned int WordItem::highestSentenceNrInWord()
		{
		return highestSentenceNrInWord_;
		}

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::checkStructuralIntegrityInWord()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkStructuralIntegrityInWord";

		if( assignmentList_ != NULL &&
		// Check assignments for structural integrity
		assignmentList_->checkStructuralIntegrityOfSpecifications( false, false ) == RESULT_OK &&
		assignmentList_->checkStructuralIntegrityOfSpecifications( false, false ) == RESULT_OK &&
		assignmentList_->checkStructuralIntegrityOfSpecifications( false, true ) == RESULT_OK )
			assignmentList_->checkForDeletedJustificationInReplacedSpecification();

		if( globalVariables()->result == RESULT_OK &&
		justificationList_ != NULL &&
		justificationList_->checkForReplacedOrDeletedSpecifications() == RESULT_OK &&
		justificationList_->checkForUnreferencedReplacedJustifications() == RESULT_OK )
			justificationList_->cleanupUnreferencedJustifications();

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL &&
		// Check specifications for structural integrity
		specificationList_->checkStructuralIntegrityOfSpecifications( false, false ) == RESULT_OK )
			specificationList_->checkForDeletedJustificationInReplacedSpecification();

		return globalVariables()->result;
		}

//Java_protected_final
	signed char WordItem::markWordAsFeminine()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markWordAsFeminine";

		if( isMasculineWord_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already marked as masculine" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnMasculineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );

		isFeminineWord_ = true;

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::markWordAsMasculine()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markWordAsMasculine";

		if( isFeminineWord_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already marked as feminine" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnFeminineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );

		isMasculineWord_ = true;

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::relateSingularNounWithFeminineArticle()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "relateSingularNounWithFeminineArticle";

		if( hasMasculineArticle_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already related to a masculine article" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnMasculineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on masculine parameters" );

		hasFeminineArticle_ = true;

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::relateSingularNounWithMasculineArticle()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "relateSingularNounWithMasculineArticle";

		if( hasFeminineArticle_ &&
		!hasCurrentlyCorrectedAssumption() )
			return startErrorInWord( functionNameString, NULL, "I am already related to a feminine article" );

		if( wordTypeList_ == NULL ||
		wordTypeList_->checkWordTypesOnFeminineParameters() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to check the word types on feminine parameters" );

		hasMasculineArticle_ = true;

		return RESULT_OK;
		}

//Java_protected_static_final
	char *WordItem::currentLanguageNameString()
		{
		WordItem *currentLanguageWordItem;

		return ( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL ?
				currentLanguageWordItem->anyWordTypeString() : NULL );
		}

//Java_protected_static_final
	char *WordItem::languageNameString( unsigned short languageNr )
		{
		WordItem *predefinedNounLanguageWordItem;

		return ( ( predefinedNounLanguageWordItem = globalVariables()->predefinedNounLanguageWordItem ) != NULL ?
				predefinedNounLanguageWordItem->selectedLanguageNameString( languageNr ) : NULL );
		}

//Java_protected_static_final
	char *WordItem::userNameString( unsigned short userNr )
		{
		WordItem *predefinedNounUserWordItem = globalVariables()->predefinedNounUserWordItem;

		return ( predefinedNounUserWordItem != NULL ?
				predefinedNounUserWordItem->selectedUserNameString( userNr ) : NULL );
		}

//Java_protected_static_final
	char *WordItem::wordTypeNameString( unsigned short wordTypeNr )
		{
		WordItem *currentLanguageWordItem;

		return ( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL ?
				currentLanguageWordItem->grammarString( wordTypeNr ) : NULL );
		}

//Java_protected_final
	WordItem *WordItem::languageWordItem( unsigned short languageNr )
		{
		unsigned short numberOfLanguages;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( ( numberOfLanguages = nLanguages() ) > 0 &&
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

//Java_protected_final
	WordItem *WordItem::nextWordItem()
		{
		return (WordItem *)nextItem;
		}

//Java_protected_static_final
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

//Java_protected_final
	unsigned int WordItem::nActiveAssignments()
		{
		return ( assignmentList_ == NULL ? 0 : assignmentList_->nActiveAssignments() );
		}

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::createNewAssignmentLevelInWord()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createNewAssignmentLevelInWord";

		if( assignmentList_ != NULL &&
		assignmentList_->createNewAssignmentLevelInList() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to create assignment level ", ( globalVariables()->currentAssignmentLevel + 1 ) );

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::deleteAssignmentLevelInWord()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAssignmentLevelInWord";

		if( assignmentList_ != NULL &&
		assignmentList_->deleteAssignmentLevelInList() != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to delete assignment level ", globalVariables()->currentAssignmentLevel );

		return RESULT_OK;
		}

//Java_protected_final
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

//Java_protected_final
	SpecificationItem *WordItem::firstActiveNumeralAssignmentItem()
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->firstActiveNumeralAssignmentItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstActiveStringAssignmentItem()
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->firstActiveStringAssignmentItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonQuestionActiveAssignmentItem()
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->firstAssignmentItem( false, false, false, false ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::lastActiveNonQuestionAssignmentItem()
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->lastActiveNonQuestionAssignmentItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstAssignmentItem( bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( assignmentList_ != NULL &&
				// Active assignment
				( foundAssignmentItem = assignmentList_->firstAssignmentItem( false, false, isPossessive, isQuestion, specificationWordItem ) ) == NULL &&
				// Inactive assignment
				( foundAssignmentItem = assignmentList_->firstAssignmentItem( true, false, isPossessive, isQuestion, specificationWordItem ) ) == NULL ?
				// Archived assignment
				assignmentList_->firstAssignmentItem( false, true, isPossessive, isQuestion, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonPossessiveActiveAssignmentItem( WordItem *relationWordItem )
		{
		return ( assignmentList_ == NULL ? NULL : assignmentList_->firstRelationSpecificationItem( false, relationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( assignmentList_ != NULL &&

				( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->firstNonQuestionAssignmentItem( false, false, isNegative, isPossessive, specificationWordItem ) ) == NULL ) &&

				( !isIncludingInactiveAssignments ||
				( foundAssignmentItem = assignmentList_->firstNonQuestionAssignmentItem( true, false, isNegative, isPossessive, specificationWordItem ) ) == NULL ) &&

				isIncludingArchivedAssignments ? assignmentList_->firstNonQuestionAssignmentItem( false, true, isNegative, isPossessive, specificationWordItem ) :
												foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstQuestionAssignmentItem()
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( assignmentList_ != NULL &&
				// Active assignment
				( foundAssignmentItem = assignmentList_->firstAssignmentItem( false, false, false, true ) ) == NULL ?
				// Archived assignment
				assignmentList_->firstAssignmentItem( false, false, true, true ) : foundAssignmentItem );
		}

//Java_protected_final
	CreateAndAssignResultType WordItem::assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *authorizationKey )
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
			if( ( foundSpecificationItem = firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) ) != NULL &&
			// Typical for Spanish: Spanish ambiguous past tense assignment
			!isNegative &&
			foundSpecificationItem->isNegative() )
				foundSpecificationItem = firstSpecificationItem( false, false, false, specificationWordItem );

			if( foundSpecificationItem == NULL )
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
						if( ( createAndAssignResult = assignSpecification( false, currentAssignmentItem->isInactiveAssignment(), currentAssignmentItem->isArchivedAssignment(), currentAssignmentItem->isCharacteristicFor(), currentAssignmentItem->isEveryGeneralization(), currentAssignmentItem->isExclusiveGeneralization(), currentAssignmentItem->isExclusiveSpecification(), currentAssignmentItem->isNegative(), currentAssignmentItem->isPartOf(), currentAssignmentItem->isPossessive(), currentAssignmentItem->isSpecificationGeneralization(), currentAssignmentItem->isUniqueUserRelation(), currentAssignmentItem->assumptionLevel(), currentAssignmentItem->prepositionParameter(), currentAssignmentItem->questionParameter(), currentAssignmentItem->generalizationWordTypeNr(), currentAssignmentItem->specificationWordTypeNr(), currentAssignmentItem->relationWordTypeNr(), currentAssignmentItem->specificationCollectionNr(), relationCollectionNr, currentAssignmentItem->generalizationContextNr(), currentAssignmentItem->specificationContextNr(), currentAssignmentItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, currentAssignmentItem->nEnteredRelationWords(), firstJustificationItem, currentAssignmentItem->specificationWordItem(), currentAssignmentItem->relationWordItem(), currentAssignmentItem->specificationString() ) ).result != RESULT_OK )
							return addCreateAndAssignResultErrorInWord( functionNameString, NULL, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}
					while( ( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
					}
				}
			else
				{
				if( ( createAndAssignResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem->isCharacteristicFor(), foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->isExclusiveGeneralization(), foundSpecificationItem->isExclusiveSpecification(), isNegative, foundSpecificationItem->isPartOf(), isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem->generalizationWordTypeNr(), foundSpecificationItem->specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem->specificationCollectionNr(), relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
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

//Java_protected_final
	void WordItem::clearReplacingInfoInWord()
		{
		if( justificationList_ != NULL )
			justificationList_->clearReplacingInfo();

		if( assignmentList_ != NULL )
			assignmentList_->clearReplacingInfo();

		if( specificationList_ != NULL )
			specificationList_->clearReplacingInfo();
		}

//Java_protected_final
	void WordItem::rebuildQuickAccessWordLists()
		{
		nextAssignmentWordItem = NULL;
		nextCollectionWordItem = NULL;
		nextContextWordItem = NULL;
		nextPossessiveNounWordItem = NULL;
		nextProperNounWordItem = NULL;
		nextSpecificationWordItem = NULL;
		nextTouchedWordItem = NULL;
		nextUserDefinedProperNounWordItem = NULL;

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
				if( isProperNounWord() )
					{
					specificationList_->addToProperNounWordQuickAccessList();

					if( hasUserNr() )
						specificationList_->addToUserDefinedProperNounWordQuickAccessList();
					}
				}

			specificationList_->addToSpecificationWordQuickAccessList();
			}
		}

//Java_protected_final
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

//Java_protected_final
	unsigned int WordItem::highestFoundSentenceNrInWord( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, bool isIncludingWordTypeList, unsigned int highestFoundSentenceNr, unsigned int maxSentenceNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; ( wordListNr < NUMBER_OF_WORD_LISTS &&
									// Efficiency: Only select word lists with higher sentence number
									highestFoundSentenceNr < highestSentenceNrInWord_ &&
									highestFoundSentenceNr < maxSentenceNr ); wordListNr++ )
			{
			if( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST &&

				// Skip temporary lists
				( isIncludingWordTypeList ||
				wordListNr != WORD_TYPE_LIST ) &&

			( currentWordList = wordListArray_[wordListNr] ) != NULL &&

				// Skip temporary lists
				( isIncludingTemporaryLists ||
				!currentWordList->isTemporaryList() ) )

				highestFoundSentenceNr = currentWordList->highestFoundSentenceNrInList( isIncludingDeletedItems, highestFoundSentenceNr, maxSentenceNr );
			}

		return highestFoundSentenceNr;
		}

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
	bool WordItem::hasCollection()
		{
		return ( collectionList_ != NULL &&
				collectionList_->hasActiveItems() );
		}

//Java_protected_final
	bool WordItem::hasCollectionNr( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->hasCollectionNr( collectionNr ) : false );
		}

//Java_protected_final
	bool WordItem::hasCollectionCurrentlyBeenUpdated( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->hasCollectionCurrentlyBeenUpdated( collectionNr ) : false );
		}

//Java_protected_final
	bool WordItem::hasSimilarRelationCollection( unsigned int firstCollectionNr, unsigned int secondCollectionNr )
		{
		return ( collectionList_ != NULL ?
				( collectionList_->hasCollectionNr( firstCollectionNr ) == collectionList_->hasCollectionNr( secondCollectionNr ) ) : true );
		}

//Java_protected_final
	bool WordItem::isCompoundCollection( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->isCompoundCollection( collectionNr ) : false );
		}

//Java_protected_final
	bool WordItem::isExclusiveCollection( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->isExclusiveCollection( collectionNr ) : false );
		}

//Java_protected_final
	bool WordItem::isNonCompoundCollection( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->isNonCompoundCollection( collectionNr ) : false );
		}

//Java_protected_final
	bool WordItem::isNonExclusiveCollection( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->isNonExclusiveCollection( collectionNr ) : false );
		}

//Java_protected_final
	bool WordItem::isNounWordSpanishAmbiguous()
		{
		return isNounWordSpanishAmbiguous_;
		}

//Java_protected_static_final
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

//Java_protected_final
	unsigned int WordItem::collectionNr()
		{
		return ( collectionList_ != NULL ?
				collectionList_->collectionNr() : NO_COLLECTION_NR );
		}

//Java_protected_final
	unsigned int WordItem::collectionNr( WordItem *commonWordItem )
		{
		return ( collectionList_ != NULL ?
				collectionList_->collectionNr( commonWordItem ) : NO_COLLECTION_NR );
		}

//Java_protected_final
	unsigned int WordItem::collectionNrByCompoundGeneralizationWordInWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		return ( collectionList_ != NULL ?
				collectionList_->collectionNrByCompoundGeneralizationWord( collectionWordTypeNr, compoundGeneralizationWordItem ) : NO_COLLECTION_NR );
		}

//Java_protected_final
	unsigned int WordItem::compoundCollectionNr()
		{
		return ( collectionList_ != NULL ?
				collectionList_->compoundCollectionNr() : NO_COLLECTION_NR );
		}

//Java_protected_static_final
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

//Java_protected_final
	unsigned int WordItem::nonCompoundCollectionNr()
		{
		return ( collectionList_ != NULL ?
				collectionList_->nonCompoundCollectionNr() : NO_COLLECTION_NR );
		}

//Java_protected_final
	unsigned int WordItem::nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->nonCompoundCollectionNr( compoundCollectionNr ) : NO_COLLECTION_NR );
		}

//Java_protected_static_final
	unsigned int WordItem::nRelationWords( unsigned int relationCollectionNr, WordItem *relationWordItem )
		{
		unsigned int nCollectionWords = 0;
		WordItem *currentCollectionWordItem;

		if( relationWordItem != NULL )
			{
			// Relation word
			nCollectionWords++;

			if( relationCollectionNr > NO_COLLECTION_NR &&
			( currentCollectionWordItem = firstCollectionWordItem( relationCollectionNr ) ) != NULL )
				{
				// Do for all collection words
				do	{
					if( currentCollectionWordItem != relationWordItem )
						nCollectionWords++;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr ) ) != NULL );
				}
			}

		return nCollectionWords;
		}

//Java_protected_final
	WordItem *WordItem::collectionWordItem( unsigned int compoundCollectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->collectionWordItem( compoundCollectionNr ) : NULL );
		}

//Java_protected_final
	WordItem *WordItem::commonWordItem( unsigned int collectionNr )
		{
		return ( collectionList_ != NULL ?
				collectionList_->commonWordItem( collectionNr ) : NULL );
		}

//Java_protected_final
	WordItem *WordItem::compoundGeneralizationWordItem( unsigned int compoundCollectionNr )
		{
		return ( compoundCollectionNr > NO_COLLECTION_NR &&
				collectionList_ != NULL ?
				collectionList_->compoundGeneralizationWordItem( compoundCollectionNr ) : NULL );
		}

//Java_protected_final
	WordItem *WordItem::feminineCollectionWordItem()
		{
		return ( collectionList_ != NULL ?
				collectionList_->feminineCollectionWordItem() : NULL );
		}

//Java_protected_final
	WordItem *WordItem::masculineCollectionWordItem()
		{
		return ( collectionList_ != NULL ?
				collectionList_->masculineCollectionWordItem() : NULL );
		}

//Java_protected_static_final
	WordItem *WordItem::firstCollectionWordItem( unsigned int collectionNr )
		{
		WordItem *currentCollectionWordItem;

		if( ( currentCollectionWordItem = globalVariables()->firstCollectionWordItem ) != NULL )
			return currentCollectionWordItem->_nextCollectionWordItem( true, collectionNr );

		return NULL;
		}

//Java_protected_final
	WordItem *WordItem::_nextCollectionWordItem( unsigned int collectionNr )
		{
		return _nextCollectionWordItem( false, collectionNr );
		}

//Java_protected_final
	BoolResultType WordItem::findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		BoolResultType boolResult;

		if( collectionList_ != NULL )
			return collectionList_->findCollection( isAllowingDifferentCommonWord, collectionWordItem, commonWordItem );

		return boolResult;
		}

//Java_protected_final
	CollectionResultType WordItem::createCollectionItem( bool isCheckingForSameCollectionNumber, bool isExclusiveSpecification, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createCollectionItem";

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

		return collectionList_->createCollectionItem( isCheckingForSameCollectionNumber, isExclusiveSpecification, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
		}


	// Protected context functions

//Java_protected_final
	unsigned short WordItem::contextWordTypeNr( unsigned int contextNr )
		{
		ContextItem *foundContextItem;

		return ( ( foundContextItem = contextItem( contextNr ) ) != NULL ?
				foundContextItem->contextWordTypeNr() : NO_WORD_TYPE_NR );
		}

//Java_protected_final
	unsigned int WordItem::contextNr( WordItem *specificationWordItem )
		{
		return ( contextList_ != NULL ?
				contextList_->contextNr( specificationWordItem ) : NO_CONTEXT_NR );
		}

//Java_protected_final
	unsigned int WordItem::highestContextNrInWord()
		{
		return ( contextList_ != NULL ?
				contextList_->highestContextNr() : NO_CONTEXT_NR );
		}

//Java_protected_final
	signed char WordItem::addContext( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
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

		return contextList_->addContext( contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem );
		}


	// Protected database connection functions
/*
//Java_protected_final
	signed char WordItem::storeChangesInWordInFutureDatabase()
		{
		List *currentWordList;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "storeChangesInWordInFutureDatabase";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// No need to store items of temporary lists
			!currentWordList->isTemporaryList() &&
			currentWordList->storeChangesInListInFutureDatabase( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
				return addErrorWithWordListNr( wordListNr, functionNameString, NULL, "I failed to store changes of a list in the database" );
			}

		return RESULT_OK;
		}
*/

	// Protected generalization functions

//Java_protected_final
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

//Java_protected_final
	GeneralizationItem *WordItem::firstNounSpecificationGeneralizationItem()
		{
		return ( generalizationList_ != NULL ?
				generalizationList_->firstGeneralizationItem( true, true, false, false ) : NULL );
		}

//Java_protected_final
	GeneralizationItem *WordItem::firstProperNounSpecificationGeneralizationItem()
		{
		return ( generalizationList_ != NULL ?
				generalizationList_->firstGeneralizationItem( true, false, true, false ) : NULL );
		}

//Java_protected_final
	GeneralizationItem *WordItem::firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage )
		{
		return ( generalizationList_ != NULL ?
				generalizationList_->firstGeneralizationItem( isOnlySelectingCurrentLanguage, false, false, false ) : NULL );
		}

//Java_protected_final
	GeneralizationItem *WordItem::firstRelationGeneralizationItem()
		{
		return ( generalizationList_ != NULL ?
				generalizationList_->firstGeneralizationItem( false, false, false, true ) : NULL );
		}

//Java_protected_final
	BoolResultType WordItem::findGeneralization( bool isRelation, WordItem *generalizationWordItem )
		{
		BoolResultType boolResult;

		if( generalizationList_ != NULL )
			return generalizationList_->findGeneralization( isRelation, generalizationWordItem );

		return boolResult;
		}


	// Protected grammar functions

//Java_protected_static_final
	void WordItem::markGrammarOfCurrentLanguageAsChoiceEnd()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			currentLanguageWordItem->markGrammarAsChoiceEnd();
		}

//Java_protected_static_final
	void WordItem::markGrammarOfCurrentLanguageAsOptionEnd()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			currentLanguageWordItem->markGrammarAsOptionEnd();
		}

//Java_protected_final
	bool WordItem::isLanguageWord()
		{
		return isLanguageWord_;
		}

//Java_protected_static_final
	unsigned short WordItem::nLanguages()
		{
		unsigned short nLanguages = 0;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *generalizationWordItem;
		WordItem *predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = globalVariables()->predefinedNounLanguageWordItem ) != NULL &&
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

//Java_protected_final
	signed char WordItem::checkForDuplicateGrammarDefinitionInCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateGrammarDefinitionInCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->checkForDuplicateGrammarDefinition();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
	signed char WordItem::checkGrammarOfCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkGrammarOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->checkGrammar();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
	signed char WordItem::linkLaterDefinedGrammarWordsInCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "linkLaterDefinedGrammarWordsInCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->linkLaterDefinedGrammarWords();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
	signed char WordItem::shrinkMergedWordsInWriteSentenceOfCurrentLanguage()
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "shrinkMergedWordsInWriteSentenceOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->shrinkMergedWordsInWriteSentence();

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_static_final
	GrammarItem *WordItem::firstCurrentLanguageReadingGrammarItem()
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->firstReadingGrammarItem();

		return NULL;
		}

//Java_protected_static_final
	GrammarItem *WordItem::firstCurrentLanguageWritingGrammarItem( bool isQuestion )
		{
		WordItem *currentLanguageWordItem;

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->firstWritingGrammarItem( isQuestion );

		return NULL;
		}

//Java_protected_final
	BoolResultType WordItem::expandMergedWordsInReadSentenceOfCurrentLanguage( char *readUserSentenceString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "expandMergedWordsInReadSentenceOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->expandMergedWordsInReadSentence( readUserSentenceString );

		return startBoolResultError( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
	GrammarResultType WordItem::createGrammarItemForCurrentLanguage( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createGrammarItemForCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->createGrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem );

		return startGrammarResultErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
	GrammarResultType WordItem::findGrammarOfCurrentLanguage( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findGrammarOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->findGrammar( isIgnoringGrammarParameter, grammarParameter, grammarStringLength, grammarString );

		return startGrammarResultErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::checkInterfaceOfCurrentLanguage( unsigned short interfaceParameter, char *interfaceString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkInterfaceOfCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->checkInterface( interfaceParameter, interfaceString );

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_final
	signed char WordItem::createInterfaceForCurrentLanguage( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createInterfaceForCurrentLanguage";

		if( ( currentLanguageWordItem = globalVariables()->currentLanguageWordItem ) != NULL )
			return currentLanguageWordItem->createInterfaceItem( interfaceParameter, interfaceStringLength, interfaceString );

		return startErrorInWord( functionNameString, NULL, "The current language word is undefined" );
		}

//Java_protected_static_final
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

//Java_protected_final
	void WordItem::clearJustificationHasBeenWritten()
		{
		if( justificationList_ != NULL )
			justificationList_->clearJustificationHasBeenWritten();
		}

//Java_protected_final
	bool WordItem::hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		return ( justificationList_ != NULL ?
				justificationList_->hasJustification( primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) : false );
		}

//Java_protected_final
	bool WordItem::hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		return ( justificationList_ != NULL ?
				justificationList_->hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection() : false );
		}

//Java_protected_final
	signed char WordItem::replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceJustification";

		if( justificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->replaceJustification( false, obsoleteJustificationItem, replacingJustificationItem );
		}

//Java_protected_final
	signed char WordItem::replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteJustification";

		if( justificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->replaceOrDeleteJustification( obsoleteJustificationItem );
		}

//Java_protected_final
	signed char WordItem::updateSpecificationOfJustificationsInWord( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		if( justificationList_ != NULL )
			return justificationList_->updateSpecificationOfJustifications( obsoleteSpecificationItem, replacingSpecificationItem );

		return RESULT_OK;
		}

//Java_protected_final
	JustificationResultType WordItem::copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyJustificationItem";

		if( justificationList_ == NULL )
			return startJustificationResultErrorInWord( functionNameString, NULL, "The justification list isn't created yet" );

		return justificationList_->copyJustificationItem( newPrimarySpecificationItem, newSecondarySpecificationItem, newAttachedJustificationItem, originalJustificationItem );
		}

//Java_protected_final
	JustificationResultType WordItem::createJustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, JustificationItem *attachedJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createJustificationItem";

		if( justificationList_ == NULL )
			{
			if( isAdminWord() )
				return startJustificationResultErrorInWord( functionNameString, NULL, "The admin word item cannot have a justification list" );

			if( ( justificationList_ = new JustificationList( globalVariables(), inputOutput(), this ) ) == NULL )
				return startJustificationResultErrorInWord( functionNameString, NULL, "I failed to create a justification list" );

			wordListArray_[WORD_JUSTIFICATION_LIST] = justificationList_;
			}

		return justificationList_->createJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem, attachedJustificationItem );
		}

//Java_protected_final
	JustificationItem *WordItem::foundJustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		return ( justificationList_ != NULL ?
				justificationList_->foundJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) : NULL );
		}

//Java_protected_final
	JustificationItem *WordItem::primarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem )
		{
		return ( justificationList_ != NULL ?
				justificationList_->primarySpecificationJustificationItem( justificationTypeNr, primarySpecificationItem ) : NULL );
		}

//Java_protected_final
	JustificationItem *WordItem::primaryAndSecondarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		return ( justificationList_ != NULL ?
				justificationList_->primaryAndSecondarySpecificationJustificationItem( justificationTypeNr, primarySpecificationItem, secondarySpecificationItem ) : NULL );
		}

//Java_protected_final
	JustificationItem *WordItem::secondarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		return ( justificationList_ != NULL ?
				justificationList_->secondarySpecificationJustificationItem( justificationTypeNr, secondarySpecificationItem ) : NULL );
		}

//Java_protected_final
	SpecificationItem *WordItem::suggestiveQuestionAssumptionSpecificationItem()
		{
		return ( justificationList_ != NULL ?
				justificationList_->suggestiveQuestionAssumptionSpecificationItem() : NULL );
		}


	// Protected multiple word functions

//Java_protected_final
	bool WordItem::isMultipleWord()
		{
		return ( multipleWordList_ != NULL &&
				multipleWordList_->hasActiveItems() );
		}

//Java_protected_final
	unsigned short WordItem::matchingMultipleWordParts( char *sentenceString )
		{
		return ( multipleWordList_ != NULL ?
				multipleWordList_->matchingMultipleWordParts( sentenceString ) : 0 );
		}

//Java_protected_final
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

	void WordItem::clearQuerySelections()
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->clearQuerySelectionsInList();
			}
		}

	void WordItem::countQuery()
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->countQueryInList();
			}
		}

//Java_protected_final
	void WordItem::itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			}
		}

//Java_protected_final
	void WordItem::listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListString );
			}
		}

//Java_protected_final
	void WordItem::parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );
			}
		}

//Java_protected_final
	void WordItem::wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr )
		{
		List *currentWordList;

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL )
				currentWordList->wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );
			}
		}

//Java_protected_final
	bool WordItem::hasActiveQuestionWithCompoundCollection()
		{
		return ( specificationList_ == NULL ? false : specificationList_->hasActiveQuestionWithCompoundCollection() );
		}

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::findAnswersToQuestions( unsigned int questionSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
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

		return wordQuestion_->findAnswersToQuestions( questionSpecificationCollectionNr, answerSpecificationItem );
		}

//Java_protected_final
	signed char WordItem::writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeAnswerToQuestion";

		if( wordQuestion_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word question module isn't created yet" );

		return wordQuestion_->writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem );
		}


	// Protected specification functions

//Java_protected_final
	void WordItem::addMyWordToTouchedDuringCurrentSentenceList()
		{
		if( wordSpecification_ != NULL )
			wordSpecification_->addMyWordToTouchedDuringCurrentSentenceList();
		}

//Java_protected_final
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

//Java_protected_final
	void WordItem::initializeVariablesInWord()
		{
		hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = false;

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

		if( specificationList_ != NULL )
			specificationList_->initializeSpecificationVariables( false, false );

		if( wordSpecification_ != NULL )
			wordSpecification_->initializeWordSpecificationVariables();
		}

//Java_protected_final
	bool WordItem::hadOnceAnsweredSelfGeneratedQuestion()
		{
		return ( specificationList_ == NULL ? false : specificationList_->hadOnceAnsweredSelfGeneratedQuestion() );
		}

//Java_protected_final
	bool WordItem::hasAnyUserSpecification()
		{
		return ( specificationList_ == NULL ? false : specificationList_->hasAnyUserSpecification() );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() : false );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyConfirmedSpecificationButNoRelation()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyConfirmedSpecificationButNoRelation() : false );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyConfirmedSpecification()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyConfirmedSpecification() : false );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyCorrectedAssumption()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyCorrectedAssumption() : false );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyCorrectedAssumptionByKnowledge()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyCorrectedAssumptionByKnowledge() : false );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() : false );
		}

//Java_protected_final
	bool WordItem::hasCurrentlyMoreSpecificSpecification()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->hasCurrentlyMoreSpecificSpecification() : false );
		}

//Java_protected_final
	bool WordItem::hasMultipleSpecificationWordsWithSameSentenceNr( unsigned int creationSentenceNr, unsigned int skipThisItemNr, unsigned int specificationCollectionNr )
		{
		return ( specificationList_ == NULL ? false : specificationList_->hasMultipleSpecificationWordsWithSameSentenceNr( creationSentenceNr, skipThisItemNr, specificationCollectionNr ) );
		}

//Java_protected_final
	bool WordItem::hasNegativeSpecification()
		{
		return ( specificationList_ == NULL ? false : specificationList_->hasNegativeSpecification() );
		}

//Java_protected_final
	bool WordItem::hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, unsigned int primarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		return ( specificationList_ == NULL ? true : specificationList_->hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationCollectionNr, primarySpecificationItem ) );
		}

//Java_protected_final
	bool WordItem::isJustificationInUse( JustificationItem *unusedJustificationItem )
		{
		return ( unusedJustificationItem != NULL &&

					( unusedJustificationItem->isReplacedItem() ||

					( assignmentList_ != NULL &&

					( assignmentList_->isJustificationInUse( false, false, unusedJustificationItem ) ||
					assignmentList_->isJustificationInUse( true, false, unusedJustificationItem ) ||
					assignmentList_->isJustificationInUse( false, true, unusedJustificationItem ) ) ) ||

					( specificationList_ != NULL &&
					specificationList_->isJustificationInUse( false, false, unusedJustificationItem ) ) ) );
		}

//Java_protected_final
	unsigned int WordItem::nRemainingSpecificationWordsForWriting( bool isAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int creationSentenceNr, WordItem *relationWordItem )
		{
		if( isAssignment )
			return ( assignmentList_ == NULL ? 0 : assignmentList_->nRemainingSpecificationWordsForWriting( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, creationSentenceNr, relationWordItem ) );

		return ( specificationList_ == NULL ? 0 : specificationList_->nRemainingSpecificationWordsForWriting( false, isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, creationSentenceNr, relationWordItem ) );
		}

//Java_protected_final
	unsigned int WordItem::userSpecificationCollectionNr()
		{
		return ( wordSpecification_ != NULL ?
				wordSpecification_->userSpecificationCollectionNr() : NO_COLLECTION_NR );
		}

//Java_protected_final
	signed char WordItem::changeSpecification( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, unsigned int newRelationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeSpecification";

		if( originalSpecificationItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given original specification item is undefined" );

		if( originalSpecificationItem->isAssignment() )
			{
			if( assignmentList_ == NULL )
				return startErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList_->changeSpecification( isNewAnsweredQuestion, isNewExclusiveGeneralization, newGeneralizationCollectionNr, newSpecificationCollectionNr, newRelationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
			}

		if( specificationList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My specification list isn't created yet" );

		return specificationList_->changeSpecification( isNewAnsweredQuestion, isNewExclusiveGeneralization, newGeneralizationCollectionNr, newSpecificationCollectionNr, newRelationCollectionNr, newFirstJustificationItem, originalSpecificationItem );
		}

//Java_protected_final
	signed char WordItem::checkForSpecificationConflict( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isUserSpecificationWordSpanishAmbiguous, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForSpecificationConflict";

		if( wordSpecification_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, isUserSpecificationWordSpanishAmbiguous, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem );
		}

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::confirmSpecificationButNotItsRelation( SpecificationItem *confirmationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "confirmSpecificationButNotItsRelation";

		if( specificationList_ != NULL )
			return specificationList_->confirmSpecificationButNotItsRelation( confirmationSpecificationItem );

		return startErrorInWord( functionNameString, NULL, "I have no specifications" );
		}

//Java_protected_final
	signed char WordItem::recalculateAssumptionLevelsInWord( bool isSituationStable )
		{
		if( assignmentList_ != NULL )
			// Recalculate archived assumptions
			assignmentList_->recalculateAssumptionLevels( false, true, isSituationStable );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			specificationList_->recalculateAssumptionLevels( false, false, isSituationStable );

		return globalVariables()->result;
		}

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		if( assignmentList_ != NULL &&
		assignmentList_->updateJustificationInSpecifications( false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK &&
		assignmentList_->updateJustificationInSpecifications( true, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
			assignmentList_->updateJustificationInSpecifications( false, true, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );

		if( globalVariables()->result == RESULT_OK &&
		specificationList_ != NULL )
			return specificationList_->updateJustificationInSpecifications( false, false, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem );

		return globalVariables()->result;
		}

//Java_protected_final
	signed char WordItem::updateSpecificationOfJustificationsOfInvolvedWords( bool isIncludingUpdateOfCurrentWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateSpecificationItemsInJustificationOfInvolvedWords";

		if( obsoleteSpecificationItem == NULL )
			return startErrorInWord( functionNameString, NULL, "The given obsolete specification item is undefined" );

		if( isIncludingUpdateOfCurrentWord &&
		// Update specifications in justifications of this word
		updateSpecificationOfJustificationsInWord( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addErrorInWord( functionNameString, NULL, "I failed to update the specifications in my justification items" );

		// Condition block excludes some updates in involved words
		if( ( replacingSpecificationItem == NULL ||
		replacingSpecificationItem->hasRelationWord() ||
		!obsoleteSpecificationItem->hasRelationWord() ) &&

		( currentGeneralizationItem = firstGeneralizationItem() ) != NULL )
			{
			// Do for all generalization words
			// Update specifications in justifications of involved words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return startSystemErrorInWord( functionNameString, NULL, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem->updateSpecificationOfJustificationsInWord( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
					return addErrorInWord( functionNameString, NULL, "I failed to update the specifications in the justification items in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::writeConfirmedSpecification( unsigned short interfaceParameter, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeConfirmedSpecification";

		if( wordSpecification_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->writeConfirmedSpecification( interfaceParameter, writeSpecificationItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingRelationSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( !isIncludingArchivedAssignments ||
				assignmentList_ == NULL ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( true, isNegative, isPossessive, specificationWordItem ) ) == NULL ) &&

				specificationList_ != NULL ?
				specificationList_->bestMatchingRelationSpecificationItem( false, isNegative, isPossessive, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingRelationSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->bestMatchingRelationSpecificationItem( isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingRelationSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( false, isNegative, isPossessive, specificationWordItem, relationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( true, isNegative, isPossessive, specificationWordItem, relationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingRelationSpecificationItem( false, isNegative, isPossessive, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingRelationSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( false, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( true, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingRelationSpecificationItem( false, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingRelationSpecificationItem( bool isAllowingEmptyRelation, bool isIncludingAnsweredQuestions, bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( isAllowingEmptyRelation, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				( !isIncludingInactiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( isAllowingEmptyRelation, isIncludingAnsweredQuestions, true, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingRelationSpecificationItem( isAllowingEmptyRelation, isIncludingAnsweredQuestions, false, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingRelationSpecificationItem( isAllowingEmptyRelation, isIncludingAnsweredQuestions, false, false, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( !isIncludingArchivedAssignments ||
				assignmentList_ == NULL ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationCollectionNr, specificationWordItem ) ) == NULL ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, questionParameter, specificationCollectionNr, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int generalizationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, generalizationContextNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, generalizationContextNr, specificationWordItem, relationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, generalizationContextNr, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyRelation, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyRelation, false, isNegative, isPossessive, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyRelation, true, isNegative, isPossessive, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->bestMatchingSpecificationWordSpecificationItem( isAllowingEmptyRelation, false, isNegative, isPossessive, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstActiveQuestionSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstActiveSpecificationItem( false, true ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstAdjectiveSpecificationItem( bool isNegative, bool isQuestion )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstAdjectiveSpecificationItem( isNegative, isQuestion ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstAssignmentOrSpecificationItem( bool isNegative, bool isPossessive, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||
				( foundAssignmentItem = assignmentList_->firstSpecificationItem( isNegative, isPossessive, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				specificationList_ != NULL ?
					specificationList_->firstSpecificationItem( isNegative, isPossessive, relationCollectionNr, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstAssignmentOrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( !isIncludingActiveAssignments ||
				( foundAssignmentItem = assignmentList_->firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) ) == NULL ) &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->firstSpecificationItem( true, isNegative, isPossessive, isQuestion, specificationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstCandidateForQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification )
		{
		SpecificationItem *firstSelectedSpecificationItem;

		return ( ( firstSelectedSpecificationItem = firstSpecificationItem( false, false, false, false ) ) != NULL ?
				firstSelectedSpecificationItem->candidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, true ) : NULL );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstCollectionSpecificationItem( bool isPossessive, unsigned int relationCollectionNr )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				// Active assignment
				( ( foundAssignmentItem = assignmentList_->firstCollectionSpecificationItem( false, isPossessive, relationCollectionNr ) ) == NULL &&
				// Archived assignment
				( foundAssignmentItem = assignmentList_->firstCollectionSpecificationItem( true, isPossessive, relationCollectionNr ) ) == NULL ) ) &&
				// Specification
				specificationList_ != NULL ?
					specificationList_->firstCollectionSpecificationItem( false, isPossessive, relationCollectionNr ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstExclusiveSpecificationItem( bool isIncludingAdjectives )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstExclusiveSpecificationItem( isIncludingAdjectives ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstExclusiveSpecificationItem( WordItem *specificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstExclusiveSpecificationItem( specificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstFeminineOrMasculineSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstFeminineOrMasculineSpecificationItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNegativeSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNegativeSpecificationItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonCollectedSpecificationItem( bool isSelfGenerated, WordItem *excludeSpecificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonCollectedSpecificationItem( isSelfGenerated, excludeSpecificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonCompoundUserSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonCompoundUserSpecificationItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonCompoundCollectionSpecificationItem( bool isNegative, bool isPossessive, unsigned int compoundSpecificationCollectionNr )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonCompoundCollectionSpecificationItem( isNegative, isPossessive, compoundSpecificationCollectionNr ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonExclusiveSpecificationItem( bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonExclusiveSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonNegativeNonPosessiveDefinitionSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonNegativeNonPosessiveDefinitionSpecificationItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonQuestionSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstActiveSpecificationItem( false, false ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstNonQuestionSpecificationItem( bool isAllowingEmptyRelation, bool isAssignment, bool isArchivedAssignment )
		{
		if( isAssignment )
			return ( assignmentList_ == NULL ? NULL : assignmentList_->firstNonQuestionSpecificationItem( isAllowingEmptyRelation, isArchivedAssignment ) );

		return ( specificationList_ == NULL ? NULL : specificationList_->firstNonQuestionSpecificationItem( isAllowingEmptyRelation, false ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstOlderNonPossessiveNonQuestionSpecificationItem( bool isArchivedAssignment )
		{
		SpecificationItem *foundAssignmentItem;

		if( assignmentList_ != NULL &&
		( foundAssignmentItem = assignmentList_->firstOlderNonPossessiveNonQuestionSpecificationItem( isArchivedAssignment ) ) != NULL )
			return foundAssignmentItem;

		return ( specificationList_ == NULL ? NULL : specificationList_->firstOlderNonPossessiveNonQuestionSpecificationItem( false ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstPartOfSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstPartOfSpecificationItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstPartOfSpecificationItem( WordItem *specificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstPartOfSpecificationItem( specificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstPossessiveSpecificationItem()
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstPossessiveSpecificationItem() );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;
		SpecificationItem *moreRecentSpecificationItem;

		if( specificationList_ != NULL &&
		( moreRecentSpecificationItem = specificationList_->firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem ) ) != NULL )
			{
			if( assignmentList_ != NULL &&
			( foundAssignmentItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( true, false, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem ) ) == NULL )
				foundAssignmentItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( true, true, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, specificationWordItem );

			if( foundAssignmentItem == NULL ||

			// Prefer more recent specification over assignment
			( foundAssignmentItem->hasRelationWord() == moreRecentSpecificationItem->hasRelationWord() &&
			foundAssignmentItem->originalSentenceNr() < moreRecentSpecificationItem->originalSentenceNr() ) )
				return moreRecentSpecificationItem;
			}

		return foundAssignmentItem;
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelation, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				( ( foundAssignmentItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelation, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == NULL &&

				( !isIncludingArchivedAssignments ||
				( foundAssignmentItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelation, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) ) == NULL ) ) ) &&

				specificationList_ != NULL ?
					specificationList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelation, false, isNegative, isPossessive, isSelfGeneratedAssumption, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelation, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( !isIncludingArchivedAssignments ||
				assignmentList_ == NULL ||
				( foundAssignmentItem = assignmentList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelation, true, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) == NULL ) &&

				specificationList_ != NULL ?
					specificationList_->firstSelfGeneratedCheckSpecificationItem( isAllowingEmptyRelation, false, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSelfGeneratedConclusionSpecificationItem( bool isNegative, bool isPossessive, bool isUserAssignment, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||
				( foundAssignmentItem = assignmentList_->firstSelfGeneratedConclusionSpecificationItem( isNegative, isPossessive, isUserAssignment, specificationWordItem ) ) == NULL ) &&

				specificationList_ != NULL ?
					specificationList_->firstSelfGeneratedConclusionSpecificationItem( isNegative, isPossessive, isUserAssignment, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		return ( isAssignment ? firstAssignmentItem( isInactiveAssignment, isArchivedAssignment, isQuestion ) :
				( isQuestion ? firstActiveQuestionSpecificationItem() : firstNonQuestionSpecificationItem() ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		if( isAssignment )
			return ( assignmentList_ != NULL ?
					( isArchivedAssignment ? assignmentList_->firstAssignmentItem( isIncludingAnsweredQuestions, false, true, questionParameter ) :
					( isInactiveAssignment ? assignmentList_->firstAssignmentItem( isIncludingAnsweredQuestions, true, false, questionParameter ) :
											assignmentList_->firstAssignmentItem( isIncludingAnsweredQuestions, false, false, questionParameter ) ) ) : NULL );

		return ( specificationList_ == NULL ? NULL : specificationList_->firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSpecificationItem( bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstSpecificationItem( false, isNegative, isPossessive, isQuestion, specificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstUnhiddenSpanishSpecificationItem()
		{
		return ( specificationList_ != NULL ?
					specificationList_->firstUnhiddenSpanishSpecificationItem( false ) :

				( assignmentList_ != NULL ?
					assignmentList_->firstUnhiddenSpanishSpecificationItem( true ) : NULL ) );
		}

//Java_protected_final
	SpecificationItem *WordItem::firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				// Active assignment
				( ( foundAssignmentItem = assignmentList_->firstUserSpecificationItem( false, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) == NULL &&
				// Archived assignment
				( foundAssignmentItem = assignmentList_->firstUserSpecificationItem( true, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) == NULL ) ) &&

				specificationList_ != NULL ?
					specificationList_->firstUserSpecificationItem( false, isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) : foundAssignmentItem );
		}

//Java_protected_final
	SpecificationItem *WordItem::noRelationWordSpecificationItem( bool isPossessive, bool isSelfGenerated, WordItem *specificationWordItem )
		{
		return ( specificationList_ != NULL &&

				( assignmentList_ == NULL ||
				// Skip if archived assignment with relation context is found
				assignmentList_->firstSpecificationItem( true, false, isPossessive, false, specificationWordItem ) == NULL ) ?
					specificationList_->noRelationWordSpecificationItem( isPossessive, isSelfGenerated, specificationWordItem ) : NULL );
		}

//Java_protected_final
	SpecificationItem *WordItem::sameUserQuestionSpecificationItem( unsigned short questionParameter )
		{
		SpecificationItem *foundAssignmentItem = NULL;

		return ( ( assignmentList_ == NULL ||

				// Active assignment
				( ( foundAssignmentItem = assignmentList_->sameUserQuestionSpecificationItem( false, questionParameter ) ) == NULL &&
				// Archived assignment
				( foundAssignmentItem = assignmentList_->sameUserQuestionSpecificationItem( true, questionParameter ) ) == NULL ) ) &&

				specificationList_ != NULL ?
					specificationList_->sameUserQuestionSpecificationItem( false, questionParameter ) : foundAssignmentItem );
		}

//Java_protected_final
	WordItem *WordItem::feminineOrMasculineCommonWordItem( bool isFeminineWord )
		{
		return ( specificationList_ == NULL ? NULL : specificationList_->feminineOrMasculineCommonWordItem( isFeminineWord ) );
		}

//Java_protected_final
	CollectionResultType WordItem::collectExclusiveSpecificationWords( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentSpecificationWordItem = NULL;
		WordItem *foundSpecificationWordItem = NULL;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectExclusiveSpecificationWords";

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

		if( collectionWordTypeNr != WORD_TYPE_NOUN_SINGULAR &&
		collectionWordTypeNr != WORD_TYPE_ADJECTIVE )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given collection word type isn't a singular noun or an adjective" );

		if( commonWordTypeNr != WORD_TYPE_NOUN_SINGULAR )
			return startCollectionResultErrorInWord( functionNameString, NULL, "The given common word type isn't a singular noun" );

		if( ( currentSpecificationItem = firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			do	{
				if( currentSpecificationItem->hasNonCompoundSpecificationCollection() &&
				( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem &&
				( foundSpecificationItem = currentSpecificationWordItem->firstNonNegativeUserSpecificationItem( collectionWordTypeNr ) ) != NULL &&
				( foundSpecificationWordItem = foundSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( ( boolResult = foundSpecificationWordItem->findCollection( false, collectionWordItem, this ) ).result != RESULT_OK )
						return addCollectionResultErrorInWord( functionNameString, NULL, "I failed to find out if word \"", collectionWordItem->anyWordTypeString(), "\" is collected with word \"", foundSpecificationWordItem->anyWordTypeString(), "\"" );

					// No collection found
					if( !boolResult.booleanValue )
						{
						// Each collection comes in pairs
						if( ( collectionResult = foundSpecificationWordItem->createCollectionItem( true, true, collectionWordTypeNr, commonWordTypeNr, NO_COLLECTION_NR, collectionWordItem, this, NULL ) ).result != RESULT_OK )
							return addCollectionResultErrorInWord( functionNameString, NULL, "I failed to collect the first word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with the second word \"", collectionWordItem->anyWordTypeString(), "\"" );

						if( collectionWordItem->createCollectionItem( false, true, collectionWordTypeNr, commonWordTypeNr, collectionResult.createdCollectionNr, foundSpecificationWordItem, this, NULL ).result != RESULT_OK )
							return addCollectionResultErrorInWord( functionNameString, NULL, "I failed to collect the second word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with the first word \"", collectionWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( foundSpecificationWordItem == NULL &&
			( currentSpecificationItem = currentSpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		collectionResult.foundGeneralizationWordItem = currentSpecificationWordItem;
		return collectionResult;
		}

//Java_protected_final
	CreateAndAssignResultType WordItem::addSpecificationInWord( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *authorizationKey )
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

		return wordSpecification_->addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString );
		}

//Java_protected_final
	CreateAndAssignResultType WordItem::createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isCharacteristicFor, bool isConditional, bool isCorrectedSpecification, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createSpecificationItem";

		if( isAssignment )
			{
			if( assignmentList_ == NULL )
				return startCreateAndAssignResultErrorInWord( functionNameString, NULL, "My assignment list isn't created yet" );

			return assignmentList_->createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isCharacteristicFor, isConditional, isCorrectedSpecification, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( isExclusiveGeneralization ? collectionNr( specificationWordItem ) : NO_COLLECTION_NR ), specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, storedSentenceString, storedSentenceWithOnlyOneSpecificationString );
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

		return specificationList_->createSpecificationItem( isInactiveAssignment, isArchivedAssignment, isAnsweredQuestion, isCharacteristicFor, isConditional, isCorrectedSpecification, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_COLLECTION_NR, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, storedSentenceString, storedSentenceWithOnlyOneSpecificationString );
		}

//Java_protected_final
	RelatedResultType WordItem::findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ == NULL )
			return startRelatedResultErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->findRelatedSpecification( isCheckingRelationContext, searchSpecificationItem );
		}

//Java_protected_final
	RelatedResultType WordItem::findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( wordSpecification_ == NULL )
			return startRelatedResultErrorInWord( functionNameString, NULL, "My word specification module isn't created yet" );

		return wordSpecification_->findRelatedSpecification( false, isIgnoringNegative, isIncludingAssignments, isIncludingArchivedAssignments, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem );
		}


	// Protected word type functions

//Java_protected_final
	void WordItem::clearGeneralizationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList_ != NULL )
			wordTypeList_->clearGeneralizationWriteLevel( isLanguageWord_, currentWriteLevel );
		}

//Java_protected_final
	void WordItem::clearSpecificationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList_ != NULL )
			wordTypeList_->clearSpecificationWriteLevel( currentWriteLevel );
		}

//Java_protected_final
	void WordItem::clearRelationWriteLevel( unsigned short currentWriteLevel )
		{
		if( wordTypeList_ != NULL )
			wordTypeList_->clearRelationWriteLevel( currentWriteLevel );
		}

//Java_protected_final
	bool WordItem::hasAnyWordType()
		{
		return ( wordTypeList_ != NULL ?
				wordTypeList_->hasAnyWordType() : false );
		}

//Java_protected_static_final
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

		return ( ( oppositeArticleWordItem = predefinedWordItem( oppositeArticleParameter ) ) != NULL ?
				oppositeArticleWordItem->hasWordType( false, WORD_TYPE_ARTICLE ) : false );
		}

//Java_protected_final
	bool WordItem::hasFeminineProperNounEnding()
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL ?
					foundWordTypeItem->hasFeminineWordEnding() : false );
		}

//Java_protected_final
	bool WordItem::hasFeminineOrMasculineProperNounEnding()
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL ?
					foundWordTypeItem->hasFeminineOrMasculineWordEnding() : false );
		}

//Java_protected_final
	bool WordItem::hasMasculineProperNounEnding()
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL ?
					foundWordTypeItem->hasMasculineWordEnding() : false );
		}

//Java_protected_final
	bool WordItem::hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr )
		{
		return ( wordTypeList_ != NULL ?
				( wordTypeList_->activeWordTypeItem( isAllowingDifferentNoun, isLanguageWord_, wordTypeNr ) != NULL ) : false );
		}

//Java_protected_final
	bool WordItem::isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( true, isLanguageWord_, specificationWordTypeNr ) ) != NULL ?
					foundWordTypeItem->isSpecificationWordAlreadyWritten() : false );
		}

//Java_protected_final
	bool WordItem::isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( true, isLanguageWord_, relationWordTypeNr ) ) != NULL ?
					foundWordTypeItem->isRelationWordAlreadyWritten() : false );
		}

//Java_protected_final
	bool WordItem::isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL ?
					foundWordTypeItem->isCorrectAdjective( adjectiveParameter ) : false );
		}

//Java_protected_final
	bool WordItem::isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL ?
					foundWordTypeItem->isCorrectDefiniteArticle( definiteArticleParameter ) : false );
		}

//Java_protected_final
	bool WordItem::isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				
				( ( !hasFeminineArticle_ &&
				!hasMasculineArticle_ ) ||

				( hasFeminineArticle_ &&
				isFeminineArticleParameter( indefiniteArticleParameter ) ) ||

				( hasMasculineArticle_ &&
				isMasculineArticleParameter( indefiniteArticleParameter ) ) ) &&

				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) ) != NULL ?
					foundWordTypeItem->isCorrectIndefiniteArticle( true, indefiniteArticleParameter ) : false );
		}

//Java_protected_final
	bool WordItem::isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *hideKey )
		{
		return ( wordTypeList_ != NULL ?
				wordTypeList_->isCorrectHiddenWordType( wordTypeNr, compareString, hideKey ) : false );
		}

//Java_protected_final
	bool WordItem::isNumeralWordType()
		{
		return hasWordType( false, WORD_TYPE_NUMERAL );
		}

//Java_protected_final
	bool WordItem::isProperNounPrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		WordTypeItem *foundWordTypeItem;

		return ( wordTypeList_ != NULL &&
				( foundWordTypeItem = wordTypeList_->activeWordTypeItem( false, isLanguageWord_, WORD_TYPE_PROPER_NOUN ) ) != NULL ?
					foundWordTypeItem->isProperNounPrecededByDefiniteArticle( definiteArticleParameter ) : false );
		}

//Java_protected_final
	bool WordItem::isProperNounWord()
		{
		return hasWordType( false, WORD_TYPE_PROPER_NOUN );
		}

//Java_protected_final
	bool WordItem::isSingularNounWord()
		{
		return hasWordType( false, WORD_TYPE_NOUN_SINGULAR );
		}

//Java_protected_final
	signed char WordItem::deleteAllWordTypesOfCurrentSentence()
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->deleteAllWordTypesOfCurrentSentence();

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::deleteWordType( WordTypeItem *deleteWordTypeItem )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->deleteWordType( deleteWordTypeItem );

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::hideWordType( unsigned short wordTypeNr, char *authorizationKey )
		{
		if( wordTypeList_ != NULL )
			return wordTypeList_->hideWordTypeItem( wordTypeNr, authorizationKey );

		return RESULT_OK;
		}

//Java_protected_final
	signed char WordItem::markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->markGeneralizationWordTypeAsWritten( isLanguageWord_, wordTypeNr );
		}

//Java_protected_final
	signed char WordItem::markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->markSpecificationWordTypeAsWritten( wordTypeNr );
		}

//Java_protected_final
	signed char WordItem::markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->markRelationWordTypeAsWritten( wordTypeNr );
		}

//Java_protected_final
	signed char WordItem::setAdjectiveParameter( bool isChineseCurrentLanguage, unsigned short adjectiveParameter, WordTypeItem *thisWordTypeItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setAdjectiveParameter";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->setAdjectiveParameter( isChineseCurrentLanguage, adjectiveParameter, wordParameter_, thisWordTypeItem );
		}

//Java_protected_final
	signed char WordItem::setDefiniteArticleParameter( unsigned short definiteArticleParameter, WordTypeItem *thisWordTypeItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setDefiniteArticleParameter";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->setDefiniteArticleParameter( definiteArticleParameter, wordParameter_, thisWordTypeItem );
		}

//Java_protected_final
	signed char WordItem::setIndefiniteArticleParameter( unsigned short indefiniteArticleParameter, WordTypeItem *thisWordTypeItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setIndefiniteArticleParameter";

		if( wordTypeList_ == NULL )
			return startErrorInWord( functionNameString, NULL, "The word type list isn't created yet" );

		return wordTypeList_->setIndefiniteArticleParameter( indefiniteArticleParameter, wordParameter_, thisWordTypeItem );
		}

//Java_protected_final
	WordTypeItem *WordItem::activeWordTypeItem( unsigned short wordTypeNr )
		{
		return ( wordTypeList_ != NULL ?
				wordTypeList_->activeWordTypeItem( false, isLanguageWord_, wordTypeNr ) : NULL );
		}

//Java_protected_final
	WordTypeItem *WordItem::activeWordTypeItem( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		return ( wordTypeList_ != NULL ?
				wordTypeList_->activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr ) : NULL );
		}

//Java_protected_final
	char *WordItem::activeWordTypeString( unsigned short wordTypeNr )
		{
		return ( wordTypeList_ != NULL ?
				wordTypeList_->activeWordTypeString( isLanguageWord_, wordTypeNr ) : NULL );
		}

//Java_protected_final
	char *WordItem::anyWordTypeString()
		{
		return wordTypeString( true, NO_WORD_TYPE_NR );
		}

//Java_protected_final
	char *WordItem::singularNounString()
		{
		return activeWordTypeString( WORD_TYPE_NOUN_SINGULAR );
		}

//Java_protected_final
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

//Java_protected_final
	WordResultType WordItem::findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString )
		{
		WordResultType wordResult;

		if( wordTypeList_ != NULL )
			return wordTypeList_->findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString );

		return wordResult;
		}

//Java_protected_final
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


	// Protected write functions

//Java_protected_final
	void WordItem::deleteTemporaryWriteList()
		{
		if( writeList_ != NULL )
			writeList_->deleteTemporaryList();
		}

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::writeJustificationSpecificationInWord( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeJustificationSpecificationInWord";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeJustificationSpecificationInWord( isWritingCurrentSpecificationWordOnly, justificationSpecificationItem );
		}

//Java_protected_final
	signed char WordItem::writeSelectedRelationInfo( bool isActiveAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedRelationInfo";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeSelectedRelationInfo( isActiveAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion, writeWordItem );
		}

//Java_protected_final
	signed char WordItem::writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem )
		{
		return writeSelectedSpecification( isCheckingUserSentenceForIntegrity, true, true, false, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, writeSpecificationItem );
		}

//Java_protected_final
	signed char WordItem::writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isJustification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
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

		return wordWrite_->writeSelectedSpecification( isCheckingUserSentenceForIntegrity, isForcingResponseNotBeingAssignment, isForcingResponseNotBeingFirstSpecification, isJustification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, answerParameter, writeSpecificationItem );
		}

//Java_protected_final
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

//Java_protected_final
	signed char WordItem::writeSpecificationSentence( bool isAssignment, bool isArchivedAssignment, bool isCheckingUserSentenceForIntegrity, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationSentence";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeSpecificationSentence( isAssignment, isArchivedAssignment, isCheckingUserSentenceForIntegrity, isPossessive, isQuestion, isSpecificationGeneralization, isWritingCurrentSpecificationWordOnly, answerParameter, grammarLevel, selectedGrammarItem, writeSpecificationItem );
		}

//Java_protected_final
	signed char WordItem::writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedSpecificationByKnowledge, bool isCorrectedSpecificationByOppositeSuggestiveQuestion, bool isForcingResponseNotBeingFirstSpecification, bool isReplacedBySpecificationWithRelation, bool isWritingCurrentSpecificationWordOnly, bool wasHiddenSpanishSpecification, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeUpdatedSpecification";

		if( wordWrite_ == NULL )
			return startErrorInWord( functionNameString, NULL, "My word write module isn't created yet" );

		return wordWrite_->writeUpdatedSpecification( isAdjustedSpecification, isCorrectedSpecificationByKnowledge, isCorrectedSpecificationByOppositeSuggestiveQuestion, isForcingResponseNotBeingFirstSpecification, isReplacedBySpecificationWithRelation, isWritingCurrentSpecificationWordOnly, wasHiddenSpanishSpecification, writeSpecificationItem );
		}

//Java_protected_final
	WriteItem *WordItem::firstActiveWriteItem()
		{
		return ( writeList_ != NULL ?
				writeList_->firstActiveWriteItem() : NULL );
		}

/*************************************************************************
 *	"Fear of the Lord is the foundation of true wisdom.
 *	All who obey his commandments will grow in wisdom." (Psalm 111:10)
 *************************************************************************/
