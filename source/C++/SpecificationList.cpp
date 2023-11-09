﻿/*	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		Storing specification items
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

#include "GeneralizationItem.cpp"
#include "SpecificationItem.cpp"

class SpecificationList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isMarkedAsSpecificationWord_ = false;


	// Private assignment functions

	signed char createNewAssignmentLevel( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createNewAssignmentLevel";

		while( searchSpecificationItem != NULL &&
		searchSpecificationItem->assignmentLevel() >= currentAssignmentLevel )
			{
			// Assignments of current level
			if( createSpecificationItem( isInactiveAssignment, isArchivedAssignment, searchSpecificationItem->isAnsweredQuestion(), searchSpecificationItem->isCharacteristicFor(), searchSpecificationItem->isConditional(), searchSpecificationItem->isCorrectedSpecification(), searchSpecificationItem->isEveryGeneralization(), searchSpecificationItem->isExclusiveGeneralization(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecific(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUncountableGeneralizationNoun(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->isValueSpecification(), (unsigned short)(searchSpecificationItem->assignmentLevel() + 1 ), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->languageNr(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->generalizationWordTypeNr(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationCollectionNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), searchSpecificationItem->firstJustificationItem(), searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), NULL, NULL ).result != RESULT_OK )
				return addError( functionNameString, "I failed to copy an assignment item with incremented assignment level" );

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char deleteAssignmentLevelInList( SpecificationItem *searchSpecificationItem )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAssignmentLevelInList";

		while( searchSpecificationItem != NULL &&
		searchSpecificationItem->assignmentLevel() >= currentAssignmentLevel )
			{
			// Assignments of current level
			if( deleteItem( searchSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to delete an item" );

			searchSpecificationItem = nextSpecificationListItem();
			}

		return RESULT_OK;
		}

	signed char updateReplacedSpecifications( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		SpecificationItem *searchSpecificationItem = firstReplacedSpecificationItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateReplacedSpecifications";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, "The given replaced specification item is a replaced or deleted item" );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->replacingSpecificationItem == obsoleteSpecificationItem )
				searchSpecificationItem->replacingSpecificationItem = replacingSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}


	// Private specification functions

	void clearReplacingInfo( SpecificationItem *searchSpecificationItem )
		{
		while( searchSpecificationItem != NULL )
			{
			searchSpecificationItem->clearReplacingInfo();
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}
		}

	void deleteSpecificationList( SpecificationItem *searchSpecificationItem )
		{
		SpecificationItem *deleteSpecificationItem;

		while( searchSpecificationItem != NULL )
			{
			deleteSpecificationItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			delete deleteSpecificationItem;
			}
		}

	SpecificationItem *firstActiveSpecificationItem()
		{
		return (SpecificationItem *)firstActiveItem();
		}

	SpecificationItem *firstInactiveSpecificationItem()
		{
		return (SpecificationItem *)firstInactiveItem();
		}

	SpecificationItem *firstArchivedSpecificationItem()
		{
		return (SpecificationItem *)firstArchivedItem();
		}

	SpecificationItem *firstReplacedSpecificationItem()
		{
		return (SpecificationItem *)firstReplacedItem();
		}

	SpecificationItem *firstSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		return ( isArchivedAssignment ? firstArchivedSpecificationItem() :
				( isInactiveAssignment ? firstInactiveSpecificationItem() :
										firstActiveSpecificationItem() ) );
		}

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion ) :
									firstActiveSpecificationItem( isIncludingAnsweredQuestions, isQuestion ) );
		}

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter ) :
									firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

	SpecificationItem *nextSpecificationListItem()
		{
		return (SpecificationItem *)nextListItem();
		}


	protected:
	// Constructor

	SpecificationList( char _listChar, GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( _listChar, "SpecificationList", globalVariables, inputOutput, myWordItem );
		}

	~SpecificationList()
		{
		deleteSpecificationList( firstActiveSpecificationItem() );
		deleteSpecificationList( firstInactiveSpecificationItem() );
		deleteSpecificationList( firstArchivedSpecificationItem() );
		deleteSpecificationList( firstReplacedSpecificationItem() );
		deleteSpecificationList( (SpecificationItem *)firstDeletedItem() );
		}


	// Protected assignment functions

	unsigned int nActiveAssignments()
		{
		unsigned int nItems = 0;
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != NULL )
			{
			nItems++;
			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return nItems;
		}

	signed char createNewAssignmentLevelInList()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createNewAssignmentLevelInList";

		if( globalVariables()->currentAssignmentLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, "Assignment level overflow" );

		if( createNewAssignmentLevel( false, false ) != RESULT_OK )
			return addError( functionNameString, "I failed to create an active assignment level" );

		if( createNewAssignmentLevel( true, false ) != RESULT_OK )
			return addError( functionNameString, "I failed to create an inactive assignment level" );

		if( createNewAssignmentLevel( false, true ) != RESULT_OK )
			return addError( functionNameString, "I failed to create an archive assignment level" );

		return RESULT_OK;
		}

	signed char deleteAssignmentLevelInList()
		{
		SpecificationItem *searchSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAssignmentLevelInList";

		if( globalVariables()->currentAssignmentLevel <= NO_ASSIGNMENT_LEVEL )
			return startError( functionNameString, "The current assignment level is undefined" );

		if( deleteAssignmentLevelInList( firstActiveSpecificationItem() ) == RESULT_OK )
			{
			if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != NULL )
				deleteAssignmentLevelInList( searchSpecificationItem );

			if( globalVariables()->result == RESULT_OK &&
			( searchSpecificationItem = firstArchivedSpecificationItem() ) != NULL )
				deleteAssignmentLevelInList( searchSpecificationItem );
			}

		return globalVariables()->result;
		}

	signed char inactivateOrArchiveCurrentGeneralizationAssignments( bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundActiveAssignmentItem = NULL;
		SpecificationItem *foundInactiveAssignmentItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundActiveGeneralizationWordItem;
		WordItem *foundInactiveGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "inactivateOrArchiveCurrentGeneralizationAssignments";

		if( specificationWordItem == NULL )
			return startError( functionNameString, "The given specification word item is undefined" );

		if( specificationWordItem->isNounValue() )
			return startError( functionNameString, "Inactivating or archiving a noun value specification isn't implemented yet" );

		if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return startSystemError( functionNameString, "I found an undefined generalization word" );

				if( foundActiveAssignmentItem == NULL &&
				// Skip my word for activate assignments, because this is the one to be assigned
				currentGeneralizationWordItem != myWordItem() )
					foundActiveAssignmentItem = currentGeneralizationWordItem->firstNonQuestionAssignmentItem( true, false, false, isNegative, isPossessive, specificationWordItem );

				// Allow to find an inactive assignment in my word
				if( foundInactiveAssignmentItem == NULL )
					foundInactiveAssignmentItem = currentGeneralizationWordItem->firstNonQuestionAssignmentItem( false, true, false, isNegative, isPossessive, specificationWordItem );
				}
			while( ( foundActiveAssignmentItem == NULL ||
			foundInactiveAssignmentItem == NULL ) &&

			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		// Only archive an inactive assignment if an active assignment is being inactivated
		if( foundActiveAssignmentItem != NULL )
			{
			if( ( foundActiveGeneralizationWordItem = foundActiveAssignmentItem->generalizationWordItem() ) == NULL )
				return startError( functionNameString, "I found an active assignment without generalization word item" );

			if( foundActiveGeneralizationWordItem->inactivateActiveAssignment( foundActiveAssignmentItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to inactivate an active assignment" );

			if( foundInactiveAssignmentItem != NULL )
				{
				if( ( foundInactiveGeneralizationWordItem = foundInactiveAssignmentItem->generalizationWordItem() ) == NULL )
					return startError( functionNameString, "I found an inactive assignment without generalization word item" );

				if( foundInactiveGeneralizationWordItem->archiveInactiveAssignment( foundInactiveAssignmentItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to archive an inactive assignment" );
				}
			}

		return RESULT_OK;
		}

	SpecificationItem *firstActiveNumeralAssignmentItem()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isSpecificationNumeral() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstActiveStringAssignmentItem()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->specificationString() != NULL )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstActiveNonQuestionAssignmentItem( unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->specificationWordItem() == specificationWordItem &&

			( relationContextNr == NO_CONTEXT_NR ||
			searchSpecificationItem->relationContextNr() == relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		// This is the first assignment
		SpecificationItem *firstAssignmentItem;

		// Now get the first valid assignment
		return ( ( firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) ) != NULL ?
				firstAssignmentItem->assignmentItem( isIncludingAnsweredQuestions, true, isQuestion ) : NULL );
		}

	SpecificationItem *firstAssignmentItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		SpecificationItem *firstAssignmentItem;

		return ( ( firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) ) != NULL ?
				firstAssignmentItem->assignmentItem( isIncludingAnsweredQuestions, true, questionParameter ) : NULL );
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, bool isQuestion, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			searchSpecificationItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isSelfGeneratedSpecification() == isSelfGenerated &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			searchSpecificationItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonQuestionAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *lastActiveNonQuestionAssignmentItem()
		{
		SpecificationItem *lastAssignmentItem = NULL;
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			lastAssignmentItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return lastAssignmentItem;
		}


	// Protected specification functions

	void addToAssignmentWordQuickAccessList()
		{
		WordItem *_myWordItem = myWordItem();

		// Word order is not important
		_myWordItem->nextAssignmentWordItem = globalVariables()->firstAssignmentWordItem;
		globalVariables()->firstAssignmentWordItem = _myWordItem;
		}

	void addToPossessiveNounWordQuickAccessList()
		{
		WordItem *_myWordItem = myWordItem();

		// Word order is not important
		_myWordItem->nextPossessiveNounWordItem = globalVariables()->firstPossessiveNounWordItem;
		globalVariables()->firstPossessiveNounWordItem = _myWordItem;
		}

	void addToUserDefinedProperNounWordQuickAccessList()
		{
		WordItem *_myWordItem = myWordItem();

		// Word order is not important
		_myWordItem->nextUserDefinedProperNounWordItem = globalVariables()->firstUserDefinedProperNounWordItem;
		globalVariables()->firstUserDefinedProperNounWordItem = _myWordItem;
		}

	void addToSpecificationWordQuickAccessList()
		{
		WordItem *_myWordItem = myWordItem();

		// Word order is not important
		_myWordItem->nextSpecificationWordItem = globalVariables()->firstSpecificationWordItem;
		globalVariables()->firstSpecificationWordItem = _myWordItem;
		}

	void clearReplacingInfo()
		{
		SpecificationItem *searchSpecificationItem;

		if( ( searchSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != NULL )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstArchivedSpecificationItem() ) != NULL )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstReplacedSpecificationItem() ) != NULL )
			clearReplacingInfo( searchSpecificationItem );
		}

	void initializeSpecificationVariables( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != NULL )
			{
			searchSpecificationItem->initializeSpecificationVariables();
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}
		}

	bool hasActiveQuestionWithCompoundCollection()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, true );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasCompoundSpecificationCollection() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hadOnceAnsweredSelfGeneratedQuestion()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isAnsweredSelfGeneratedQuestion() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasAnyUserSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasUserNr() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasRelationContextInSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedSpecification, unsigned int relationContextNr )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedSpecification ? firstReplacedSpecificationItem() :
																				firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( relationContextNr > NO_CONTEXT_NR )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->relationContextNr() == relationContextNr )
					return true;

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
			}

		return false;
		}

	bool hasMultipleSpecificationWordsWithSameSentenceNr( unsigned int creationSentenceNr, unsigned int skipThisItemNr, unsigned int specificationCollectionNr )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != NULL &&
		searchSpecificationItem->creationSentenceNr() >= creationSentenceNr )
			{
			if( !searchSpecificationItem->hasRelationContext() &&
			!searchSpecificationItem->isNegative() &&
			!searchSpecificationItem->isPossessive() &&
			searchSpecificationItem->itemNr() != skipThisItemNr &&
			searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasNegativeSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, unsigned int primarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		unsigned int searchSpecificationCollectionNr;
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem != primarySpecificationItem &&
			searchSpecificationItem->isOlderItem() &&
			!searchSpecificationItem->isPartOf() &&
			!searchSpecificationItem->isPossessive() &&

			// Don't skip creation of current question
			( ( ( searchSpecificationCollectionNr = searchSpecificationItem->specificationCollectionNr() ) == primarySpecificationCollectionNr ||
			searchSpecificationCollectionNr == exclusiveSecondarySpecificationCollectionNr ) ) )
				return false;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return true;
		}

	bool isJustificationInUse( bool isInactiveAssignment, bool isArchivedAssignment, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedJustificationItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->hasJustification( unusedJustificationItem ) )
					return true;

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
			}

		return false;
		}

	unsigned int nRemainingSpecificationWordsForWriting( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, unsigned int creationSentenceNr )
		{
		unsigned int nRemainingWords = 0;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );
		WordItem *specificationWordItem;

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr ) != NULL &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&

			// Typical for Chinese
			( creationSentenceNr == NO_SENTENCE_NR ||
			searchSpecificationItem->creationSentenceNr() == creationSentenceNr ) &&

			!specificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) )
				nRemainingWords++;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return nRemainingWords;
		}

	signed char checkForDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning &&
		searchSpecificationItem->checkForDeletedJustification() == RESULT_OK )
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();

		return globalVariables()->result;
		}

	signed char checkStructuralIntegrityOfSpecifications( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning &&
		// Check specification for replaced or deleted justifications
		searchSpecificationItem->checkForReplacedOrDeletedJustification() == RESULT_OK &&

		( !searchSpecificationItem->isSelfGeneratedSpecification() ||

		// Check self-generated specification for circular reasoning
		( !globalVariables()->hasDisplayedIntegrityWarning &&
		searchSpecificationItem->checkForCircularReasoning() == RESULT_OK &&

		( !globalVariables()->hasDisplayedIntegrityWarning &&

		( !searchSpecificationItem->hasRelationContext() ||
		searchSpecificationItem->isHiddenSpanishSpecification() ||
		// Check non-hidden, self-generated specification with relation context for justifications covering the relation context
		searchSpecificationItem->checkForJustificationsCoveringRelationContext() == RESULT_OK ) ) ) ) )
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();

		return globalVariables()->result;
		}

	signed char checkJustificationForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, "The given unused justification item is undefined" );

		while( searchSpecificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem->hasJustification( unusedJustificationItem ) )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found a justification item that is still in use:\n\tSpecificationItem: " );
				strcat( errorString, searchSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, unusedJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char checkSpecificationForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, SpecificationItem *unusedSpecificationItem )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkSpecificationForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, "The given unused specification item is undefined" );

		while( searchSpecificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem->replacingSpecificationItem == unusedSpecificationItem )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found a replacing specification item that is still in use:\n\tSpecificationItem: " );
				strcat( errorString, searchSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\treplacingSpecificationItem: " );
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char checkWordItemForUsage( bool isInactiveAssignment, bool isArchivedAssignment, WordItem *unusedWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->specificationWordItem() == unusedWordItem )
				return startError( functionNameString, "The specification word item is still in use" );

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char clearStoredSentenceStringWithUnknownPluralNoun( bool isInactiveAssignment, bool isArchivedAssignment, const char *unknownPluralNounString, WordItem *specificationWordItem )
		{
		size_t unknownPluralNounStringLength;
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char *storedSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "clearStoredSentenceStringWithUnknownPluralNoun";

		if( unknownPluralNounString == NULL )
			return startError( functionNameString, "The given unknown plural noun string is undefined" );

		if( specificationWordItem == NULL )
			return startError( functionNameString, "The given specification word item is undefined" );

		unknownPluralNounStringLength = strlen( unknownPluralNounString );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			( storedSentenceString = searchSpecificationItem->storedSentenceString() ) != NULL &&
			strlen( storedSentenceString ) > unknownPluralNounStringLength &&
			strncmp( storedSentenceString, unknownPluralNounString, unknownPluralNounStringLength ) > 0 )
				searchSpecificationItem->clearStoredSentenceString();

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char collectGeneralizations( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveGeneralization, unsigned int generalizationCollectionNr )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectGeneralizations";

		if( generalizationCollectionNr <= NO_COLLECTION_NR )
			return startError( functionNameString, "The given generalization collection number is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( ( isExclusiveGeneralization ||
			// Definition specification
			searchSpecificationItem->isGeneralizationNoun() ) &&

			!searchSpecificationItem->hasGeneralizationCollection() )
				{
				if( searchSpecificationItem->hasCurrentCreationSentenceNr() )
					{
					if( searchSpecificationItem->collectGeneralization( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
						return addError( functionNameString, "I failed to collect the generalization of a specification" );
					}
				else
					{
					// Copy and replace specification
					if( copyAndReplaceSpecification( searchSpecificationItem->isAnsweredQuestion(), isExclusiveGeneralization, generalizationCollectionNr, searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->firstJustificationItem(), searchSpecificationItem ).result != RESULT_OK )
						return addError( functionNameString, "I failed to copy and replace an older specification" );
					}
				}
																							// First assignment or specification
			searchSpecificationItem = ( searchSpecificationItem->isReplacedOrDeletedItem() ? firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false ) :
																							// Next assignment or specification
																							searchSpecificationItem->nextSelectedSpecificationItem() );
			}

		return RESULT_OK;
		}

	signed char collectSpecificationsInList( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isQuestion, unsigned int specificationCollectionNr )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectSpecificationsInList";

		if( specificationCollectionNr <= NO_COLLECTION_NR )
			return startError( functionNameString, "The given specification collection number is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->hasSpecificationCollection() &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&
			specificationWordItem->hasCollectionNr( specificationCollectionNr ) )
				{
				if( searchSpecificationItem->hasCurrentCreationSentenceNr() )
					{
					// Collect specification
					if( searchSpecificationItem->collectSpecification( isExclusiveSpecification, specificationCollectionNr ) != RESULT_OK )
						return addError( functionNameString, "I failed to collect a specification" );
					}
				else
					{
					// Copy and replace specification
					if( copyAndReplaceSpecification( searchSpecificationItem->isAnsweredQuestion(), false, searchSpecificationItem->generalizationCollectionNr(), specificationCollectionNr, searchSpecificationItem->firstJustificationItem(), searchSpecificationItem ).result != RESULT_OK )
						return addError( functionNameString, "I failed to copy and replace an older specification" );
					}
				}
																							// First specification
			searchSpecificationItem = ( searchSpecificationItem->isReplacedOrDeletedItem() ? firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion ) :
																							// Next specification
																							searchSpecificationItem->nextSelectedSpecificationItem() );
			}

		return RESULT_OK;
		}

	signed char confirmSpecificationButNotItsRelation( SpecificationItem *confirmationSpecificationItem )
		{
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *confirmedSpecificationItem = firstActiveSpecificationItem();
		SpecificationItem *searchSpecificationItem;
		WordItem *confirmedSpecificationWordItem;
		WordItem *_myWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "confirmSpecificationButNotItsRelation";

		if( confirmationSpecificationItem == NULL )
			return startError( functionNameString, "The given confirmation specification item is undefined" );

		if( !confirmationSpecificationItem->isUserSpecification() )
			return startError( functionNameString, "The given confirmation specification item isn't a user specification" );

		if( ( confirmedSpecificationWordItem = confirmationSpecificationItem->specificationWordItem() ) == NULL )
			return startError( functionNameString, "The given confirmation specification item has no specification word" );

		while( confirmedSpecificationItem != NULL )
			{
			if( confirmedSpecificationItem->hasRelationContext() &&
			confirmedSpecificationItem->isSelfGeneratedSpecification() &&
			confirmedSpecificationItem->specificationWordItem() == confirmedSpecificationWordItem )
				{
				if( _myWordItem->writeConfirmedSpecification( ( confirmedSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NONE_OF_ITS_RELATIONS : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NONE_OF_ITS_RELATIONS ), confirmedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write a confirmed specification" );

				// Skip possessive Spanish replacements. To be implemented later
				if( !confirmedSpecificationItem->isPossessive() )
					{
					searchSpecificationItem = firstActiveSpecificationItem();

					while( searchSpecificationItem != NULL )
						{
						if( !searchSpecificationItem->hasRelationContext() &&
						!searchSpecificationItem->isAnsweredQuestion() &&
						( obsoleteJustificationItem = searchSpecificationItem->primarySpecificationJustificationItem( confirmedSpecificationItem ) ) != NULL )
							{
							// Copy justification
							if( ( justificationResult = _myWordItem->copyJustificationItem( confirmationSpecificationItem, obsoleteJustificationItem->secondarySpecificationItem(), obsoleteJustificationItem->attachedJustificationItem(), obsoleteJustificationItem ) ).result != RESULT_OK )
								return addError( functionNameString, "I failed to copy the primary specification justification item with a different primary specification item" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
								return startError( functionNameString, "I couldn't create a justification item" );

							// Replace justification
							if( _myWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
								return addError( functionNameString, "I failed to replace the given obsolete justification item by a created justification item" );
							}
																										// First specification
						searchSpecificationItem = ( searchSpecificationItem->isReplacedOrDeletedItem() ? firstActiveSpecificationItem() : 
																										// Next specification
																										searchSpecificationItem->nextSpecificationItem() );
						}
					}
				}

			confirmedSpecificationItem = confirmedSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char recalculateAssumptionLevels( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionLevels";

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isSelfGeneratedSpecification() )
				{
				if( searchSpecificationItem->isSelfGeneratedAssumption() &&
				// Don't calculate hidden Spanish assumptions
				!searchSpecificationItem->isHiddenSpanishSpecification() )
					{
					// Recalculate assumption level
					if( searchSpecificationItem->recalculateAssumptionLevel( true ) != RESULT_OK )
						return addError( functionNameString, "I failed to recalculate the assumption level of a specification" );
					}
				else
					{
					if( searchSpecificationItem->hasCurrentCreationSentenceNr() &&
					// Remove obsolete assumption justifications
					searchSpecificationItem->removeObsoleteAssumptionJustifications( true, false ) != RESULT_OK )
						return addError( functionNameString, "I failed to remove obsolete assumption justifications from a specification" );
					}
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char removeObsoleteAssumptionJustificationsFromPartOfSpecifications()
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, false, false );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeObsoleteAssumptionJustificationsFromPartOfSpecifications";

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPartOf() &&
			searchSpecificationItem->isOlderItem() &&
			searchSpecificationItem->hasCurrentCreationSentenceNr() &&
			searchSpecificationItem->removeObsoleteAssumptionJustifications( false, false ) != RESULT_OK )
				return addError( functionNameString, "I failed to remove obsolete assumption justifications" );

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, "The given obsolete specification item is already replaced or deleted" );

		if( obsoleteSpecificationItem == replacingSpecificationItem )
			return startError( functionNameString, "The given obsolete specification item and the given replacing specification item are the same specification item" );

		if( replacingSpecificationItem != NULL )
			{
			if( replacingSpecificationItem->isReplacedOrDeletedItem() )
				return startError( functionNameString, "The given replaced specification item is a replaced or deleted item" );

			// Don't replace a normal specification by a hidden Spanish specification
			if( replacingSpecificationItem->isHiddenSpanishSpecification() &&
			!obsoleteSpecificationItem->isHiddenSpanishSpecification() )
				return startError( functionNameString, "The given replacing specification item is a hidden specification, while the given obsolete specification item isn't hidden" );
			}

		if( updateReplacedSpecifications( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to update the replacing specification item of the archive specification items" );

		obsoleteSpecificationItem->replacingSpecificationItem = replacingSpecificationItem;

		if( obsoleteSpecificationItem->hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to delete a specification item" );
			}
		else
			{
			if( replaceItem( obsoleteSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to replace a specification item" );
			}

		// Update specification of justifications of involved words
		if( myWordItem()->updateSpecificationOfJustificationsOfInvolvedWords( true, obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to update the specifications in the justification of involved words" );

		return RESULT_OK;
		}

	signed char updateJustificationInSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		bool isSelectedExclusiveSpecification;
		unsigned int relationContextNr;
		JustificationItem *attachedPredecessorOfObsoleteJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		WordItem *_myWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateJustificationInSpecifications";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		while( searchSpecificationItem != NULL )
			{
			// Select self-generated specifications
			if( ( firstJustificationItem = searchSpecificationItem->firstJustificationItem() ) != NULL )
				{
				// Check first justification
				if( firstJustificationItem == obsoleteJustificationItem )
					{
					// Allowed to change the current justification, without creating a new one
					if( searchSpecificationItem->hasCurrentCreationSentenceNr() )
						{
						if( replacingJustificationItem != NULL &&
						searchSpecificationItem->changeFirstJustification( false, replacingJustificationItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to change the first justification item of a specification item" );
						}
					else
						{
						createdSpecificationItem = NULL;

						if( replacingJustificationItem != NULL )
							{
							isSelectedExclusiveSpecification = ( isExclusiveGeneralization ||
																searchSpecificationItem->isExclusiveSpecification() );
							relationContextNr = ( isExclusiveGeneralization ? NO_CONTEXT_NR : searchSpecificationItem->relationContextNr() );

							if( ( createAndAssignResult = createSpecificationItem( searchSpecificationItem->isInactiveAssignment(), isArchivedAssignment, searchSpecificationItem->isAnsweredQuestion(), searchSpecificationItem->isCharacteristicFor(), searchSpecificationItem->isConditional(), searchSpecificationItem->isCorrectedSpecification(), searchSpecificationItem->isEveryGeneralization(), isExclusiveGeneralization, isSelectedExclusiveSpecification, searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecific(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUncountableGeneralizationNoun(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->isValueSpecification(), searchSpecificationItem->assignmentLevel(), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->languageNr(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->generalizationWordTypeNr(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationCollectionNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), relationContextNr, searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), replacingJustificationItem, searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), searchSpecificationItem->storedSentenceString(), searchSpecificationItem->storedSentenceWithOnlyOneSpecificationString() ) ).result != RESULT_OK )
								return addError( functionNameString, "I failed to copy the search specification item" );

							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
								return startError( functionNameString, "I couldn't copy the search specification item" );
							}

						if( replaceOrDeleteSpecification( searchSpecificationItem, createdSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to replace or delete a specification" );

						if( isExclusiveGeneralization &&
						_myWordItem->assignSpecification( false, false, true, false, searchSpecificationItem->isNegative(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->nContextRelations(), replacingJustificationItem, searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), NULL ).result != RESULT_OK )
							return addError( functionNameString, "I failed to create an assignment" );
						}
					}
				else
					{
					// Check attached justification items
					if( ( attachedPredecessorOfObsoleteJustificationItem = firstJustificationItem->attachedPredecessorOfObsoleteJustificationItem( obsoleteJustificationItem ) ) != NULL )
						{
						if( attachedPredecessorOfObsoleteJustificationItem->hasCurrentCreationSentenceNr() )
							{
							if( attachedPredecessorOfObsoleteJustificationItem->changeAttachedJustification( replacingJustificationItem ) != RESULT_OK )
								return addError( functionNameString, "I failed to change the attached justification item of a justification item" );
							}
						else
							{
							if( replacingJustificationItem != NULL )
								{
								// Copy justification
								if( ( justificationResult = _myWordItem->copyJustificationItem( attachedPredecessorOfObsoleteJustificationItem->updatedPrimarySpecificationItem(), attachedPredecessorOfObsoleteJustificationItem->updatedSecondarySpecificationItem(), replacingJustificationItem, attachedPredecessorOfObsoleteJustificationItem ) ).result != RESULT_OK )
									return addError( functionNameString, "I failed to copy the attached predecessor of obsolete justification item with a different first justification item" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return startError( functionNameString, "I couldn't copy an attached predecessor of an old justification" );

								// Replace justification
								if( _myWordItem->replaceJustification( attachedPredecessorOfObsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
									return addError( functionNameString, "I failed to replace the attached predecessor of obsolete justification item by a created justification item" );
								}
							}
						}
					}
				}
																							// First specification
			searchSpecificationItem = ( searchSpecificationItem->isReplacedOrDeletedItem() ? firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) : 
																							// Next specification
																							searchSpecificationItem->nextSpecificationItem() );
			}

		return RESULT_OK;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int nCurrentRelationContextWords;
		unsigned int nFoundRelationContextWords = 0;
		unsigned int relationContextNr;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&

				( relationWordItem == NULL ||

				( ( relationContextNr = searchSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
				relationWordItem->hasContextInWord( relationContextNr ) ) ) )
					{
					if( ( isPossessive &&
					!isSpecificationWordSpanishAmbiguous ) ||

					( !hasRelationWord &&
					isSpecificationWordSpanishAmbiguous ) )
						return searchSpecificationItem;

					nCurrentRelationContextWords = searchSpecificationItem->nRelationContextWords();

					if( nFoundRelationContextWords == 0 ||
					nCurrentRelationContextWords == nFoundRelationContextWords + 1 ||

					// Current specification has less relation words than the found specification
					( nCurrentRelationContextWords < nFoundRelationContextWords &&
					nCurrentRelationContextWords + 1 != nFoundRelationContextWords ) )
						{
						nFoundRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		unsigned int nFoundRelationContextWords = 0;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > NO_COLLECTION_NR &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
					return searchSpecificationItem;

				if( nFoundRelationContextWords == 0 &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					{
					nFoundRelationContextWords = searchSpecificationItem->nRelationContextWords();
					foundSpecificationItem = searchSpecificationItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestAssumptionLevelSpecificationItem( bool isPossessive, WordItem *specificationWordItem )
		{
		unsigned short bestAssumptionLevel = MAX_LEVEL;
		unsigned short currentAssumptionLevel;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, false, false );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->hasRelationContext() &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				( currentAssumptionLevel = searchSpecificationItem->assumptionLevel() ) < bestAssumptionLevel )
					{
					bestAssumptionLevel = currentAssumptionLevel;
					foundSpecificationItem = searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		unsigned int nCurrentRelationContextWords;
		unsigned int nFoundRelationContextWords = 0;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					{
					nCurrentRelationContextWords = searchSpecificationItem->nRelationContextWords();

					if( nFoundRelationContextWords == 0 ||
					nCurrentRelationContextWords == nFoundRelationContextWords + 1 ||

					// Current specification has less relation words than the found specification
					( nCurrentRelationContextWords < nFoundRelationContextWords &&
					nCurrentRelationContextWords + 1 != nFoundRelationContextWords ) )
						{
						nFoundRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isQuestion, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				searchSpecificationItem->isMatchingRelationContextNr( isAllowingEmptyRelationContext, relationContextNr ) )
					{
					if( isQuestion ||
					searchSpecificationItem->relationContextNr() == relationContextNr )
						return searchSpecificationItem;

					foundSpecificationItem = searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		// The given specification word item can be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > NO_COLLECTION_NR &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
					return searchSpecificationItem;

				if( foundSpecificationItem == NULL &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					{
					if( specificationCollectionNr == NO_COLLECTION_NR )
						return searchSpecificationItem;

					// Wait for matching specification collection, if defined
					foundSpecificationItem = searchSpecificationItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyGeneralizationContext, bool isAllowingEmptyRelationContext, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->isMatchingGeneralizationContextNr( isAllowingEmptyGeneralizationContext, generalizationContextNr ) &&
			searchSpecificationItem->isMatchingRelationContextNr( isAllowingEmptyRelationContext, relationContextNr ) &&

			( specificationCollectionNr == NO_COLLECTION_NR ||
			searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr ) )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > NO_COLLECTION_NR &&
				!searchSpecificationItem->isSpecificationGeneralization() )
					return searchSpecificationItem;

				if( foundSpecificationItem == NULL &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					{
					if( specificationCollectionNr == NO_COLLECTION_NR )
						return searchSpecificationItem;

					// Wait for matching specification collection
					foundSpecificationItem = searchSpecificationItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return foundSpecificationItem;
		}

	SpecificationItem *firstActiveSpecificationItem( bool isIncludingAnsweredQuestions, bool isQuestion )
		{
		SpecificationItem *firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL ?
				firstSpecificationItem->specificationItem( isIncludingAnsweredQuestions, true, isQuestion ) : NULL );
		}

	SpecificationItem *firstActiveSpecificationItem( bool isIncludingAnsweredQuestions, unsigned short questionParameter )
		{
		SpecificationItem *firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL ?
				firstSpecificationItem->specificationItem( isIncludingAnsweredQuestions, true, questionParameter ) : NULL );
		}

	SpecificationItem *firstAdjectiveSpecificationItem( bool isNegative, bool isQuestion )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, isQuestion );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isSpecificationAdjective() &&
			searchSpecificationItem->isNegative() == isNegative )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstExclusiveSpecificationItem( bool isIncludingAdjectives )
		{
		SpecificationItem *firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			return firstSpecificationItem->exclusiveSpecificationItem( isIncludingAdjectives, true );

		return NULL;
		}

	SpecificationItem *firstExclusiveSpecificationItem( WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isExclusiveSpecification() &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstFeminineOrMasculineSpecificationItem()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem *specificationWordItem;

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isNegative() &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&
			specificationWordItem->isFeminineOrMasculineWord() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNegativeSpecificationItem()
		{
		SpecificationItem *firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			return firstSpecificationItem->negativeSpecificationItem( true );

		return NULL;
		}

	SpecificationItem *firstNonCollectedSpecificationItem( bool isSelfGenerated, WordItem *excludeSpecificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->hasSpecificationCollection() &&
			!searchSpecificationItem->isNegative() &&
			searchSpecificationItem->isSelfGeneratedSpecification() == isSelfGenerated &&
			searchSpecificationItem->specificationWordItem() != excludeSpecificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonCompoundUserSpecificationItem()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasNonCompoundSpecificationCollection() &&
			searchSpecificationItem->isUserSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonCompoundCollectionSpecificationItem( unsigned int compoundSpecificationCollectionNr )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem *specificationWordItem;

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasSpecificationCollection() &&
			!searchSpecificationItem->isNegative() &&
			!searchSpecificationItem->isPossessive() &&
			searchSpecificationItem->hasNonCompoundSpecificationCollection() &&
			searchSpecificationItem->specificationCollectionNr() != compoundSpecificationCollectionNr &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&
			specificationWordItem->hasCollectionNr( compoundSpecificationCollectionNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonExclusiveSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, isQuestion );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isExclusiveSpecification() &&
			searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonNegativeNonPossessiveDefinitionSpecificationItem()
		{
		SpecificationItem *firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			return firstSpecificationItem->nonNegativeNonPossessiveDefinitionSpecificationItem( true );

		return NULL;
		}

	SpecificationItem *firstPossessiveSpecificationItem()
		{
		SpecificationItem *firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			return firstSpecificationItem->possessiveSpecificationItem( true );

		return NULL;
		}

	SpecificationItem *firstNonNegativeUserSpecificationItem( unsigned short specificationWordTypeNr )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isNegative() &&
			searchSpecificationItem->isUserSpecification() &&
			searchSpecificationItem->specificationWordTypeNr() == specificationWordTypeNr )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonQuestionSpecificationItem( bool isAllowingEmptyRelationContext, bool isArchivedAssignment )
		{
		SpecificationItem *firstSelectedSpecificationItem;

		if( ( firstSelectedSpecificationItem = firstSpecificationItem( false, isArchivedAssignment ) ) != NULL )
			return firstSelectedSpecificationItem->nonQuestionSpecificationItem( isAllowingEmptyRelationContext, true );

		return NULL;
		}

	SpecificationItem *firstOlderNonPossessiveNonQuestionSpecificationItem( bool isArchivedAssignment )
		{
		SpecificationItem *firstSelectedSpecificationItem;

		if( ( firstSelectedSpecificationItem = firstSpecificationItem( false, isArchivedAssignment ) ) != NULL )
			return firstSelectedSpecificationItem->olderNonPossessiveNonQuestionSpecificationItem( true );

		return NULL;
		}

	SpecificationItem *firstRelationSpecificationItem( bool isPossessive, WordItem *relationWordItem )
		{
		unsigned int relationContextNr;
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		if( relationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isPossessive() == isPossessive &&
				( relationContextNr = searchSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
				relationWordItem->hasContextInWord( relationContextNr ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isIncludingAnsweredQuestions, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification &&
				searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->isSelfGeneratedSpecification() == isSelfGenerated &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		unsigned int relationContextNr;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isSelfGeneratedAssumption() == isSelfGeneratedAssumption &&
				searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&

				( ( relationWordItem == NULL &&

				( isAllowingEmptyRelationContext ||
				searchSpecificationItem->hasRelationContext() ) ) ||

				( relationWordItem != NULL &&
				( relationContextNr = searchSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
				relationWordItem->hasContextInWord( relationContextNr ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->isSelfGeneratedSpecification() == isSelfGenerated &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				searchSpecificationItem->isMatchingRelationContextNr( isAllowingEmptyRelationContext, relationContextNr ) &&

				( searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr ||

				( relationContextNr == NO_CONTEXT_NR &&
				specificationWordItem->isCompoundCollection( specificationCollectionNr ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSpecificationItem( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, false, false );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( !searchSpecificationItem->isSpecificationGeneralization() &&
				searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->relationContextNr() == relationContextNr &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->isSpecificationGeneralization() == isSpecificationGeneralization &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, isQuestion );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstUnhiddenSpanishSpecificationItem( bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( false, isArchivedAssignment );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->wasHiddenSpanishSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstUserSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isUserSpecification() &&
			searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->isMatchingRelationContextNr( false, relationContextNr ) &&

			( searchSpecificationItem->specificationWordItem() == specificationWordItem ||

			( specificationCollectionNr > NO_COLLECTION_NR &&
			searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr ) ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *noRelationContextSpecificationItem( bool isPossessive, bool isSelfGenerated, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( !searchSpecificationItem->hasRelationContext() &&
				!searchSpecificationItem->isNegative() &&
				searchSpecificationItem->isOlderItem() &&
				!searchSpecificationItem->isPartOf() &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->isSelfGeneratedSpecification() == isSelfGenerated &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *partOfSpecificationItem( WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isPartOf() &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *sameUserQuestionSpecificationItem( bool isArchivedAssignment, unsigned short questionParameter )
		{
		unsigned int nUserSpecificationWords = globalVariables()->nUserSpecificationWords;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isUserQuestion() &&
			searchSpecificationItem->nInvolvedSpecificationWords() == nUserSpecificationWords )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	WordItem *feminineOrMasculineCommonWordItem( bool isFeminineWord )
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem *commonWordItem;
		WordItem *specificationWordItem;
		WordItem *_myWordItem = myWordItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isExclusiveSpecification() &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&
			( commonWordItem = specificationWordItem->commonWordItem( searchSpecificationItem->specificationCollectionNr() ) ) != NULL )
				{
				if( commonWordItem == _myWordItem )
					{
					if( specificationWordItem->isFeminineWord() == isFeminineWord )
						return specificationWordItem;
					}
				else
					{
					if( ( isFeminineWord &&
					commonWordItem->isFeminineWord() ) ||

					( !isFeminineWord &&
					commonWordItem->isMasculineWord() ) )
						return commonWordItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	CreateAndAssignResultType copyAndReplaceSpecification( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		SpecificationItem *createdSpecificationItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyAndReplaceSpecification";

		if( originalSpecificationItem == NULL )
			return startCreateAndAssignResultError( functionNameString, "The given original specification item is undefined" );

		if( ( createAndAssignResult = createSpecificationItem( originalSpecificationItem->isInactiveAssignment(), originalSpecificationItem->isArchivedAssignment(), isNewAnsweredQuestion, originalSpecificationItem->isCharacteristicFor(), originalSpecificationItem->isConditional(), originalSpecificationItem->isCorrectedSpecification(), originalSpecificationItem->isEveryGeneralization(), isNewExclusiveGeneralization, originalSpecificationItem->isExclusiveSpecification(), originalSpecificationItem->isNegative(), originalSpecificationItem->isPartOf(), originalSpecificationItem->isPossessive(), originalSpecificationItem->isSpecific(), originalSpecificationItem->isSpecificationGeneralization(), originalSpecificationItem->isUncountableGeneralizationNoun(), originalSpecificationItem->isUniqueUserRelation(), originalSpecificationItem->isValueSpecification(), originalSpecificationItem->assignmentLevel(), originalSpecificationItem->assumptionLevel(), originalSpecificationItem->languageNr(), originalSpecificationItem->prepositionParameter(), originalSpecificationItem->questionParameter(), originalSpecificationItem->generalizationWordTypeNr(), originalSpecificationItem->specificationWordTypeNr(), originalSpecificationItem->relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem->generalizationContextNr(), originalSpecificationItem->specificationContextNr(), originalSpecificationItem->relationContextNr(), originalSpecificationItem->originalSentenceNr(), originalSpecificationItem->activeSentenceNr(), originalSpecificationItem->inactiveSentenceNr(), originalSpecificationItem->archivedSentenceNr(), originalSpecificationItem->nContextRelations(), newFirstJustificationItem, originalSpecificationItem->specificationWordItem(), originalSpecificationItem->specificationString(), originalSpecificationItem->storedSentenceString(), originalSpecificationItem->storedSentenceWithOnlyOneSpecificationString() ) ).result != RESULT_OK )
			return addCreateAndAssignResultError( functionNameString, "I failed to create a specification item with different parameters" );

		if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return startCreateAndAssignResultError( functionNameString, "I couldn't create a specification item" );

		if( replaceOrDeleteSpecification( originalSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return addCreateAndAssignResultError( functionNameString, "I failed to replace or delete the given specification item" );

		return createAndAssignResult;
		}

	CreateAndAssignResultType createSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isCharacteristicFor, bool isConditional, bool isCorrectedSpecification, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString )
		{
		bool isAssignment = isAssignmentList();
		WordItem *_myWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createSpecificationItem";

		if( generalizationWordTypeNr <= NO_WORD_TYPE_NR ||
		generalizationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startCreateAndAssignResultError( functionNameString, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= NO_WORD_TYPE_NR ||
		specificationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startCreateAndAssignResultError( functionNameString, "The given specification word type number is undefined or out of bounds" );

		if( firstJustificationItem != NULL &&
		!firstJustificationItem->isActiveItem() )
			return startCreateAndAssignResultError( functionNameString, "The given first justification item isn't active" );

		if( specificationWordItem != NULL &&
		specificationWordItem->isNounValue() )
			return startCreateAndAssignResultError( functionNameString, "The given specification word item is a value word" );

		if( !isMarkedAsSpecificationWord_ )
			{
			isMarkedAsSpecificationWord_ = true;

			if( isAssignment )
				addToAssignmentWordQuickAccessList();
			else
				{
				if( generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR )
					{
					if( isPossessive ||
					isCharacteristicFor )
						addToPossessiveNounWordQuickAccessList();
					}
				else
					{
					if( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN &&
					_myWordItem->hasUserNr() )
						addToUserDefinedProperNounWordQuickAccessList();
					}

				addToSpecificationWordQuickAccessList();
				}
			}

		if( ( createAndAssignResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isCharacteristicFor, isConditional, isCorrectedSpecification, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, _myWordItem->isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceString, storedSentenceWithOnlyOneSpecificationString, globalVariables(), inputOutput(), this, _myWordItem ) ) == NULL )
			return startCreateAndAssignResultError( functionNameString, "I failed to create a specification item" );

		if( addItemToList( ( isArchivedAssignment ? QUERY_ARCHIVED_CHAR : ( isInactiveAssignment ? QUERY_INACTIVE_CHAR : QUERY_ACTIVE_CHAR ) ), createAndAssignResult.createdSpecificationItem ) != RESULT_OK )
			return addCreateAndAssignResultError( functionNameString, "I failed to add an assignment item" );

		if( isAssignment &&
		assignmentLevel == NO_ASSIGNMENT_LEVEL &&
		originalSentenceNr == globalVariables()->currentSentenceNr )
			globalVariables()->isAssignmentChanged = true;

		return createAndAssignResult;
		}
	};

/*************************************************************************
 *	"Good comes to those who lend money generously
 *	and conduct their business fairly.
 *	Such people will not be overcome by evil.
 *	Those who are righteous will be long remembered" (Psalm 112:5-6)
 *************************************************************************/
