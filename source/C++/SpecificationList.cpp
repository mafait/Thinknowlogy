/*	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		To store specification items
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
			if( createSpecificationItem( isInactiveAssignment, isArchivedAssignment, searchSpecificationItem->isAnsweredQuestion(), searchSpecificationItem->isCharacteristicFor(), searchSpecificationItem->isConcludedAssumption(), searchSpecificationItem->isConditional(), searchSpecificationItem->isCorrectedAssumption(), searchSpecificationItem->isEveryGeneralization(), searchSpecificationItem->isExclusiveGeneralization(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecific(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUncountableGeneralizationNoun(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->isValueSpecification(), (unsigned short)(searchSpecificationItem->assignmentLevel() + 1 ), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->languageNr(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->generalizationWordTypeNr(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationCollectionNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), searchSpecificationItem->firstJustificationItem(), searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), NULL, NULL ).result != RESULT_OK )
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
				( isInactiveAssignment ? firstInactiveSpecificationItem() : firstActiveSpecificationItem() ) );
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
			searchSpecificationItem->isSelfGenerated() == isSelfGenerated &&
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
		// Word order is not important
		myWordItem()->nextAssignmentWordItem = globalVariables()->firstAssignmentWordItem;
		globalVariables()->firstAssignmentWordItem = myWordItem();
		}

	void addToPossessiveNounWordQuickAccessList()
		{
		// Word order is not important
		myWordItem()->nextPossessiveNounWordItem = globalVariables()->firstPossessiveNounWordItem;
		globalVariables()->firstPossessiveNounWordItem = myWordItem();
		}

	void addToUserProperNounWordQuickAccessList()
		{
		// Word order is not important
		myWordItem()->nextUserProperNounWordItem = globalVariables()->firstUserProperNounWordItem;
		globalVariables()->firstUserProperNounWordItem = myWordItem();
		}

	void addToSpecificationWordQuickAccessList()
		{
		// Word order is not important
		myWordItem()->nextSpecificationWordItem = globalVariables()->firstSpecificationWordItem;
		globalVariables()->firstSpecificationWordItem = myWordItem();
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

	bool hasPartOfSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPartOf() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		unsigned int primarySpecificationCollectionNr;
		unsigned int searchSpecificationCollectionNr;
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( false, false );

		if( primarySpecificationItem != NULL )
			{
			primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem != primarySpecificationItem &&
				!searchSpecificationItem->isPossessive() &&
				!searchSpecificationItem->isPartOf() )
					{
					searchSpecificationCollectionNr = searchSpecificationItem->specificationCollectionNr();

					// Don't skip creation of current question
					if( ( searchSpecificationCollectionNr == primarySpecificationCollectionNr ||
					searchSpecificationCollectionNr == exclusiveSecondarySpecificationCollectionNr ) &&

					( searchSpecificationItem->isNegative() ||
					searchSpecificationItem->isOlderItem() ) )
						return false;
					}

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
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

	signed char changeJustificationOfNegativeAssumptions( bool isInactiveAssignment, bool isArchivedAssignment, SpecificationItem *secondarySpecificationItem )
		{
		unsigned int specificationCollectionNr;
		JustificationItem *firstJustificationItem;
		JustificationItem *foundOrCreatedJustificationItem;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem *generalizationWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeJustificationOfNegativeAssumptions";

		if( secondarySpecificationItem == NULL )
			return startError( functionNameString, "The given secondary specification item is undefined" );

		specificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() &&
			searchSpecificationItem->isSelfGeneratedAssumption() &&
			searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
			( firstJustificationItem = searchSpecificationItem->firstJustificationItem() ) != NULL &&
			firstJustificationItem->isOlderItem() )
				{
				if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, firstJustificationItem->justificationTypeNr(), firstJustificationItem->orderNr, firstJustificationItem->originalSentenceNr(), firstJustificationItem->primarySpecificationItem(), firstJustificationItem->anotherPrimarySpecificationItem(), secondarySpecificationItem, NULL, firstJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
					return addError( functionNameString, "I failed to add a justification item" );

				foundOrCreatedJustificationItem = ( justificationResult.createdJustificationItem == NULL ? justificationResult.foundJustificationItem :
																											justificationResult.createdJustificationItem );

				if( firstJustificationItem != foundOrCreatedJustificationItem )
					{
					if( generalizationWordItem->replaceJustification( firstJustificationItem, foundOrCreatedJustificationItem, searchSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a justification of a negative assumption" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
					}
				else
					searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char checkForDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != NULL &&
		searchSpecificationItem->checkForDeletedJustification() == RESULT_OK )
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();

		return globalVariables()->result;
		}

	signed char checkForReplacedOrDeletedJustification( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != NULL &&
		searchSpecificationItem->checkForReplacedOrDeletedJustification() == RESULT_OK )
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();

		return globalVariables()->result;
		}

	signed char checkJustificationForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, "The given unused justification item is undefined" );

		while( searchSpecificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem->hasJustification( unusedJustificationItem ) )
				{
				strcpy( errorString, "\nI found a justification item that is still in use:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
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
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkSpecificationForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, "The given unused specification item is undefined" );

		while( searchSpecificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem->replacingSpecificationItem == unusedSpecificationItem )
				{
				strcpy( errorString, "\nI found a replacing specification item that is still in use:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\treplacingSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
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
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectGeneralizations";

		if( generalizationCollectionNr <= NO_COLLECTION_NR )
			return startError( functionNameString, "The given generalization collection number is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( ( isExclusiveGeneralization ||
			searchSpecificationItem->isGeneralizationNoun() ) &&

			!searchSpecificationItem->hasGeneralizationCollection() &&
			generalizationWordItem->hasCollectionNr( generalizationCollectionNr, searchSpecificationItem->specificationWordItem() ) )
				{
				if( searchSpecificationItem->hasCurrentCreationSentenceNr() )
					{
					if( searchSpecificationItem->collectGeneralization( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
						return addError( functionNameString, "I failed to collect the generalization of a specification" );

					searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
					}
				else
					{
					if( copyAndReplaceSpecificationItem( searchSpecificationItem->isAnsweredQuestion(), isExclusiveGeneralization, searchSpecificationItem->isExclusiveSpecification(), generalizationCollectionNr, searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->firstJustificationItem(), searchSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to copy and replace an older specification" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
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
					if( searchSpecificationItem->collectSpecification( isExclusiveSpecification, specificationCollectionNr ) != RESULT_OK )
						return addError( functionNameString, "I failed to collect a specification" );

					searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
					}
				else
					{
					if( copyAndReplaceSpecificationItem( searchSpecificationItem->isAnsweredQuestion(), false, isExclusiveSpecification, searchSpecificationItem->generalizationCollectionNr(), specificationCollectionNr, searchSpecificationItem->firstJustificationItem(), searchSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to copy and replace an older specification" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char confirmSpecificationButNotItsRelation( SpecificationItem *confirmationSpecificationItem )
		{
		bool isConfirmedPrimarySpecification;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *confirmedSpecificationItem = firstActiveSpecificationItem();
		SpecificationItem *searchSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		WordItem *confirmedSpecificationWordItem;
		WordItem *generalizationWordItem = myWordItem();
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
			confirmedSpecificationItem->isSelfGenerated() &&
			confirmedSpecificationItem->specificationWordItem() == confirmedSpecificationWordItem )
				{
				if( myWordItem()->writeConfirmedSpecification( ( confirmedSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NOT_ITS_RELATION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NOT_ITS_RELATION ), confirmedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write a confirmed specification" );

				// Skip possessive Spanish replacements. To be implemented later
				if( !confirmedSpecificationItem->isPossessive() )
					{
					searchSpecificationItem = firstActiveSpecificationItem();

					while( searchSpecificationItem != NULL )
						{
						if( !searchSpecificationItem->hasRelationContext() &&
						!searchSpecificationItem->isAnsweredQuestion() &&
						( obsoleteJustificationItem = searchSpecificationItem->primaryOrSecondarySpecificationJustificationItem( confirmedSpecificationItem ) ) != NULL )
							{
							isConfirmedPrimarySpecification = ( obsoleteJustificationItem->primarySpecificationItem() == confirmedSpecificationItem );
							selectedPrimarySpecificationItem = ( isConfirmedPrimarySpecification ? confirmationSpecificationItem :
																									obsoleteJustificationItem->primarySpecificationItem() );
							selectedSecondarySpecificationItem = ( isConfirmedPrimarySpecification ? obsoleteJustificationItem->secondarySpecificationItem() :
																									confirmationSpecificationItem );

							if( ( justificationResult = generalizationWordItem->copyJustification( false, selectedPrimarySpecificationItem, selectedSecondarySpecificationItem, obsoleteJustificationItem->attachedJustificationItem(), obsoleteJustificationItem ) ).result != RESULT_OK )
								return addError( functionNameString, "I failed to copy the primary specification justification item with a different primary specification item" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
								return startError( functionNameString, "I couldn't create a justification item" );

							if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, confirmationSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, "I failed to replace the given obsolete justification item by a created justification item" );

							searchSpecificationItem = firstActiveSpecificationItem();
							}
						else
							searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
						}
					}
				}

			confirmedSpecificationItem = confirmedSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		SpecificationItem *createdSpecificationItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyAndReplaceSpecificationItem";

		if( originalSpecificationItem == NULL )
			return startError( functionNameString, "The given original specification item is undefined" );

		if( ( createAndAssignResult = createSpecificationItem( originalSpecificationItem->isInactiveAssignment(), originalSpecificationItem->isArchivedAssignment(), isNewAnsweredQuestion, originalSpecificationItem->isCharacteristicFor(), originalSpecificationItem->isConcludedAssumption(), originalSpecificationItem->isConditional(), originalSpecificationItem->isCorrectedAssumption(), originalSpecificationItem->isEveryGeneralization(), isNewExclusiveGeneralization, isNewExclusiveSpecification, originalSpecificationItem->isNegative(), originalSpecificationItem->isPartOf(), originalSpecificationItem->isPossessive(), originalSpecificationItem->isSpecific(), originalSpecificationItem->isSpecificationGeneralization(), originalSpecificationItem->isUncountableGeneralizationNoun(), originalSpecificationItem->isUniqueUserRelation(), originalSpecificationItem->isValueSpecification(), originalSpecificationItem->assignmentLevel(), originalSpecificationItem->assumptionLevel(), originalSpecificationItem->languageNr(), originalSpecificationItem->prepositionParameter(), originalSpecificationItem->questionParameter(), originalSpecificationItem->generalizationWordTypeNr(), originalSpecificationItem->specificationWordTypeNr(), originalSpecificationItem->relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem->generalizationContextNr(), originalSpecificationItem->specificationContextNr(), originalSpecificationItem->relationContextNr(), originalSpecificationItem->originalSentenceNr(), originalSpecificationItem->activeSentenceNr(), originalSpecificationItem->inactiveSentenceNr(), originalSpecificationItem->archivedSentenceNr(), originalSpecificationItem->nContextRelations(), newFirstJustificationItem, originalSpecificationItem->specificationWordItem(), originalSpecificationItem->specificationString(), originalSpecificationItem->storedSentenceString(), originalSpecificationItem->storedSentenceWithOnlyOneSpecificationString() ) ).result != RESULT_OK )
			return addError( functionNameString, "I failed to create a specification item with different parameters" );

		if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return startError( functionNameString, "I couldn't create a specification item" );

		if( replaceOrDeleteSpecification( originalSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to replace or delete the given specification item" );

		return RESULT_OK;
		}

	signed char recalculateAssumptions( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptions";

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isSelfGeneratedAssumption() &&
			// Don't calculate hidden Spanish assumptions
			!searchSpecificationItem->isHiddenSpanishSpecification() &&
			searchSpecificationItem->recalculateAssumption() != RESULT_OK )
				return addError( functionNameString, "I failed to recalculate an assumption" );

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool isAssignment = isAssignmentList();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, "The given obsolete specification item is already replaced or deleted" );

		if( obsoleteSpecificationItem == replacingSpecificationItem )
			return startError( functionNameString, "The given obsolete specification item and the given replacing specification item are the same specification item" );

		if( obsoleteSpecificationItem->isAssignment() != isAssignment )
			return startError( functionNameString, "The given obsolete specification item is an assignment item and I am a specification list, or the given obsolete specification item is a specification item and I am an assignment list" );

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isAssignment() != isAssignment )
			return startError( functionNameString, "The given replacing specification item is an assignment item and I am a specification list, or the given replacing specification item is a specification item and I am an assignment list" );

		// Don't replace a normal specification by a hidden Spanish specification
		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isHiddenSpanishSpecification() &&
		!obsoleteSpecificationItem->isHiddenSpanishSpecification() )
			return startError( functionNameString, "The given replacing specification item is a hidden specification, while the given obsolete specification item isn't hidden" );

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

		if( updateSpecificationsInJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to update the specifications in the justification of involved words" );

		return RESULT_OK;
		}

	signed char updateJustificationInSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		bool isForcingNewJustification;
		bool isSameJustification;
		bool isSelectedExclusiveSpecification;
		unsigned int relationContextNr;
		JustificationItem *attachedPredecessorOfOldJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		SpecificationItem *updatedSecondarySpecificationItem;
		SpecificationItem *userAssignmentItem;
		WordItem *generalizationWordItem = myWordItem();
		WordItem *secondaryGeneralizationWordItem;
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
						if( searchSpecificationItem->changeFirstJustification( false, replacingJustificationItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to change the first justification item of a specification item" );

						searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
						}
					else
						{
						createdSpecificationItem = NULL;

						if( replacingJustificationItem != NULL )
							{
							isSelectedExclusiveSpecification = ( isExclusiveGeneralization ||
																searchSpecificationItem->isExclusiveSpecification() );
							relationContextNr = ( isExclusiveGeneralization ? NO_CONTEXT_NR : searchSpecificationItem->relationContextNr() );

							if( ( createAndAssignResult = createSpecificationItem( searchSpecificationItem->isInactiveAssignment(), isArchivedAssignment, searchSpecificationItem->isAnsweredQuestion(), searchSpecificationItem->isCharacteristicFor(), searchSpecificationItem->isConcludedAssumption(), searchSpecificationItem->isConditional(), searchSpecificationItem->isCorrectedAssumption(), searchSpecificationItem->isEveryGeneralization(), isExclusiveGeneralization, isSelectedExclusiveSpecification, searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecific(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUncountableGeneralizationNoun(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->isValueSpecification(), searchSpecificationItem->assignmentLevel(), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->languageNr(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->generalizationWordTypeNr(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationCollectionNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), relationContextNr, searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), replacingJustificationItem, searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), searchSpecificationItem->storedSentenceString(), searchSpecificationItem->storedSentenceWithOnlyOneSpecificationString() ) ).result != RESULT_OK )
								return addError( functionNameString, "I failed to copy the search specification item" );

							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
								return startError( functionNameString, "I couldn't copy the search specification item" );
							}

						if( replaceOrDeleteSpecification( searchSpecificationItem, createdSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to replace or delete a specification" );

						if( isExclusiveGeneralization &&
						generalizationWordItem->assignSpecification( false, false, true, false, searchSpecificationItem->isNegative(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->nContextRelations(), replacingJustificationItem, searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), NULL ).result != RESULT_OK )
							return addError( functionNameString, "I failed to create an assignment" );

						searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
						}
					}
				else
					{
					// Check attached justification items
					if( ( attachedPredecessorOfOldJustificationItem = firstJustificationItem->attachedPredecessorOfOldJustificationItem( obsoleteJustificationItem ) ) == NULL )
						searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
					else
						{
						if( attachedPredecessorOfOldJustificationItem->hasCurrentCreationSentenceNr() )
							{
							isSameJustification = ( attachedPredecessorOfOldJustificationItem == replacingJustificationItem );

							if( attachedPredecessorOfOldJustificationItem->changeAttachedJustification( isSameJustification ? NULL : replacingJustificationItem ) != RESULT_OK )
								return addError( functionNameString, "I failed to change the attached justification item of a justification item" );

							searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
							}
						else
							{
							if( attachedPredecessorOfOldJustificationItem->isReplacedItem() )
								searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
							else
								{
								isForcingNewJustification = false;

								if( ( updatedSecondarySpecificationItem = attachedPredecessorOfOldJustificationItem->updatedSecondarySpecificationItem() ) != NULL &&
								updatedSecondarySpecificationItem->isUserSpecification() &&
								// Secondary user specification has no relation
								!updatedSecondarySpecificationItem->hasRelationContext() &&
								attachedPredecessorOfOldJustificationItem->isReversibleAssumptionOrConclusion() &&
								( secondaryGeneralizationWordItem = updatedSecondarySpecificationItem->generalizationWordItem() ) != NULL &&
								// Get user specification assignment
								( userAssignmentItem = secondaryGeneralizationWordItem->firstAssignmentItem( true, true, true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, updatedSecondarySpecificationItem->specificationWordItem() ) ) != NULL )
									{
									// Secondary user specification without relation
									updatedSecondarySpecificationItem = userAssignmentItem;

									if( replacingJustificationItem != NULL &&
									replacingJustificationItem->hasFeminineOrMasculineProperNounEnding() )
										{
										isForcingNewJustification = true;
										replacingJustificationItem = NULL;
										}
									}

								if( ( justificationResult = generalizationWordItem->copyJustification( isForcingNewJustification, attachedPredecessorOfOldJustificationItem->updatedPrimarySpecificationItem(), updatedSecondarySpecificationItem, replacingJustificationItem, attachedPredecessorOfOldJustificationItem ) ).result != RESULT_OK )
									return addError( functionNameString, "I failed to copy the attached predecessor of obsolete justification item with a different first justification item" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return startError( functionNameString, "I couldn't copy an attached predecessor of an old justification" );

								if( generalizationWordItem->replaceJustification( attachedPredecessorOfOldJustificationItem, createdJustificationItem, searchSpecificationItem ) != RESULT_OK )
									return addError( functionNameString, "I failed to replace the attached predecessor of obsolete justification item by a created justification item" );

								searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
								}
							}
						}
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	signed char updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool hasObsoleteRelationContext;
		bool hasReplacingRelationContext = ( replacingSpecificationItem != NULL &&
											replacingSpecificationItem->hasRelationContext() );
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateSpecificationItemsInJustificationOfInvolvedWords";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		hasObsoleteRelationContext = obsoleteSpecificationItem->hasRelationContext();

		// Update specifications in justifications of this word
		if( generalizationWordItem->updateSpecificationsInJustificationInWord( true, obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to update the specifications in my justification items" );

		// Typical for Spanish. Condition block excludes some updates in involved words
		if( ( !hasObsoleteRelationContext ||
		hasReplacingRelationContext ||
		replacingSpecificationItem == NULL ) &&

		( currentGeneralizationItem = generalizationWordItem->firstGeneralizationItem() ) != NULL )
			{
			// Do for all generalization words
			// Update specifications in justifications of involved words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return startSystemError( functionNameString, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem->updateSpecificationsInJustificationInWord( false, obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to update the specifications in the justification items in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
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
				relationWordItem->hasContextInWord( relationContextNr, specificationWordItem ) ) ) )
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

		if( specificationWordItem != NULL )
			{
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
					searchSpecificationItem->specificationWordItem() == specificationWordItem &&
					!searchSpecificationItem->isHiddenSpanishSpecification() )
						{
						nFoundRelationContextWords = searchSpecificationItem->nRelationContextWords();
						foundSpecificationItem = searchSpecificationItem;
						}
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

	SpecificationItem *firstExclusiveSpecificationItem()
		{
		SpecificationItem *firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL ?
				firstSpecificationItem->exclusiveSpecificationItem( true ) : NULL );
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
			if( ( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&
			specificationWordItem->isFemaleOrMale() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
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

		if( compoundSpecificationCollectionNr > NO_COLLECTION_NR )
			{
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
			}

		return NULL;
		}

	SpecificationItem *firstNonPossessiveDefinitionSpecificationItem( bool isIncludingAdjectives )
		{
		SpecificationItem *firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL ?
				firstSpecificationItem->nonPossessiveDefinitionSpecificationItem( isIncludingAdjectives, true ) : NULL );
		}

	SpecificationItem *firstPossessiveSpecificationItem()
		{
		SpecificationItem *firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL ?
				firstSpecificationItem->possessiveSpecificationItem( true ) : NULL );
		}

	SpecificationItem *firstRecentlyAnsweredQuestionSpecificationItem( bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( true, false, isArchivedAssignment, true );

		return ( searchSpecificationItem != NULL ?
				searchSpecificationItem->recentlyAnsweredQuestionSpecificationItem( true ) : NULL );
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
				relationWordItem->hasContextInWord( relationContextNr, searchSpecificationItem->specificationWordItem() ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelectedRelationSpecificationItem( bool isArchivedAssignment )
		{
		SpecificationItem *firstSelectedSpecificationItem;

		return ( ( firstSelectedSpecificationItem = firstSpecificationItem( false, isArchivedAssignment ) ) != NULL ?
				firstSelectedSpecificationItem->selectedRelationSpecificationItem( true ) : NULL );
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isIncludingAnsweredQuestions, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->isSelfGenerated() == isSelfGenerated &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
					{
					if( searchSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification )
						return searchSpecificationItem;

					if( foundSpecificationItem == NULL )
						foundSpecificationItem = searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
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
				relationWordItem->hasContextInWord( relationContextNr, specificationWordItem ) ) ) )
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
				searchSpecificationItem->isSelfGenerated() == isSelfGenerated &&
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
				!searchSpecificationItem->isPartOf() &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->isSelfGenerated() == isSelfGenerated &&
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
		WordItem *generalizationWordItem = myWordItem();
		WordItem *specificationWordItem;

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isExclusiveSpecification() &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL &&
			specificationWordItem->isFemaleOrMale() &&
			( commonWordItem = specificationWordItem->commonWordItem( searchSpecificationItem->specificationCollectionNr() ) ) != NULL )
				{
				if( commonWordItem == generalizationWordItem )
					{
					if( specificationWordItem->isFemale() == isFeminineWord )
						return specificationWordItem;
					}
				else
					{
					if( ( isFeminineWord &&
					commonWordItem->isFemale() ) ||

					( !isFeminineWord &&
					commonWordItem->isMale() ) )
						return commonWordItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	BoolResultType findQuestionToBeAdjustedByCompoundCollection( unsigned int questionSpecificationCollectionNr, WordItem *primarySpecificationWordItem )
		{
		SpecificationItem *adjustedQuestionCompoundSpecificationItem;
		SpecificationItem *lastAdjustedQuestionCompoundSpecificationItem = NULL;
		WordItem *commonWordItem;
		WordItem *currentCollectionWordItem;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( primarySpecificationWordItem == NULL )
			return startBoolResultError( functionNameString, "The given primary specification word item is undefined" );

		if( questionSpecificationCollectionNr <= NO_COLLECTION_NR )
			return startBoolResultError( functionNameString, "The given question specification collection number is undefined" );

		if( ( currentCollectionWordItem = globalVariables()->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( commonWordItem = currentCollectionWordItem->commonWordItem( questionSpecificationCollectionNr ) ) != NULL &&
				commonWordItem != primarySpecificationWordItem &&
				( adjustedQuestionCompoundSpecificationItem = firstSpecificationItem( false, false, false, true, currentCollectionWordItem ) ) != NULL )
					{
					if( replaceOrDeleteSpecification( adjustedQuestionCompoundSpecificationItem, NULL ) != RESULT_OK )
						return addBoolResultError( functionNameString, "I failed to replace or delete a question part" );

					lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem;
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != NULL )
			{
			// Write last instance of adjusted compound question
			if( myWordItem()->writeUpdatedSpecification( true, false, false, false, false, lastAdjustedQuestionCompoundSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
				return addBoolResultError( functionNameString, "I failed to write an adjusted compound question" );

			boolResult.booleanValue = true;
			}

		return boolResult;
		}

	CreateAndAssignResultType createSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isCharacteristicFor, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString )
		{
		bool isAssignment = isAssignmentList();
		WordItem *generalizationWordItem = myWordItem();
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
					generalizationWordItem->hasUserNr() )
						addToUserProperNounWordQuickAccessList();
					}

				addToSpecificationWordQuickAccessList();
				}
			}

		if( ( createAndAssignResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isCharacteristicFor, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, generalizationWordItem->isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceString, storedSentenceWithOnlyOneSpecificationString, globalVariables(), inputOutput(), this, generalizationWordItem ) ) == NULL )
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
