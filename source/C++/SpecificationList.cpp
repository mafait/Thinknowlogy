/*	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		To store specification items
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

#ifndef SPECIFICATIONLIST
#define SPECIFICATIONLIST 1
#include "SpecificationItem.cpp"

class SpecificationList : private List
	{
	friend class WordAssignment;
	friend class WordItem;

	// Private constructed variables

	bool isMarkedAsSpecificationWord_;

	// Private assignment functions

	ResultType createNewAssignmentLevel( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevel";

		if( searchSpecificationItem != NULL &&
		searchSpecificationItem->assignmentLevel() > commonVariables()->currentAssignmentLevel )
			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithCurrentLevel();

		while( searchSpecificationItem != NULL )
			{
			if( createSpecificationItem( isInactiveAssignment, isArchivedAssignment, searchSpecificationItem->isAnsweredQuestion(), searchSpecificationItem->isConcludedAssumption(), searchSpecificationItem->isConditional(), searchSpecificationItem->isCorrectedAssumption(), searchSpecificationItem->isEveryGeneralization(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isGeneralizationAssignment(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->isValueSpecification(), (unsigned short)(searchSpecificationItem->assignmentLevel() + 1 ), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->languageNr(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->generalizationWordTypeNr(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationCollectionNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), searchSpecificationItem->firstJustificationItem(), searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), searchSpecificationItem->lastWrittenSentenceString ).result != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to copy an assignment item with incremented assignment level" );

			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithCurrentLevel();
			}

		return RESULT_OK;
		}

	ResultType deleteAssignmentLevelInList( SpecificationItem *searchSpecificationItem )
		{
		unsigned short currentAssignmentLevel = commonVariables()->currentAssignmentLevel;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAssignmentLevelInList";

		while( searchSpecificationItem != NULL &&
		searchSpecificationItem->assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchSpecificationItem->assignmentLevel() == currentAssignmentLevel )
				{
				if( deleteItem( searchSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to delete an item" );

				searchSpecificationItem = nextSpecificationListItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType removeJustificationsFromConfirmedAssumptionOrConclusion( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool isUserSpecification;
		JustificationItem *attachedJustificationItem;
		JustificationItem *currentJustificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeJustificationsFromConfirmedAssumptionOrConclusion";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		isUserSpecification = replacingSpecificationItem->isUserSpecification();

		if( ( currentJustificationItem = obsoleteSpecificationItem->firstJustificationItem() ) == NULL )
			return startError( functionNameString, NULL, "The given obsolete specification item has no justifications" );

		do	{
			// Remember next justification
			attachedJustificationItem = currentJustificationItem->attachedJustificationItem();

			if( currentJustificationItem->isOlderItem() &&
			!generalizationWordItem->isJustificationInUse( currentJustificationItem ) )
				{
				if( isUserSpecification )
					{
					// Remove justification from confirmed assumption/conclusion
					if( generalizationWordItem->replaceJustification( currentJustificationItem, NULL, obsoleteSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to remove a justification from a confirmed specification" );
					}
				else
					{
					// Self-generated question or corrected specification
					if( currentJustificationItem->isActiveItem() &&
					!generalizationWordItem->isJustificationInUse( currentJustificationItem ) )
						{
						if( generalizationWordItem->replaceOrDeleteJustification( currentJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace or delete a justification" );
						}
					}
				}
			}
		while( ( currentJustificationItem = attachedJustificationItem ) != NULL );

		return RESULT_OK;
		}

	ResultType updateReplacedSpecifications( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		SpecificationItem *searchSpecificationItem = firstReplacedSpecificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateReplacedSpecifications";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isReplacedItem() )
			return startError( functionNameString, NULL, "The given replaced specification item is a replaced item" );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->replacingSpecificationItem == obsoleteSpecificationItem )
				searchSpecificationItem->replacingSpecificationItem = replacingSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}


	// Private specification functions

	ResultType confirmSpecificationInJustification( bool isConfirmedPrimarySpecification, JustificationItem *obsoleteJustificationItem, SpecificationItem *confirmationSpecificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *createdJustificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "confirmSpecificationInJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

		if( confirmationSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given confirmation specification item is undefined" );

		if( !confirmationSpecificationItem->isUserSpecification() )
			return startError( functionNameString, NULL, "The given confirmation specification item isn't a user specification" );

		if( ( justificationResult = generalizationWordItem->copyJustificationItem( ( isConfirmedPrimarySpecification ? confirmationSpecificationItem : obsoleteJustificationItem->primarySpecificationItem() ), ( isConfirmedPrimarySpecification ? obsoleteJustificationItem->secondarySpecificationItem() : confirmationSpecificationItem ), obsoleteJustificationItem->attachedJustificationItem(), obsoleteJustificationItem ) ).result != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to copy the primary specification Justification item with a different primary specification item" );

		if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
			return startError( functionNameString, NULL, "I couldn't create a justification item" );

		if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, confirmationSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to replace the given obsolete justification item by a created justification item" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase( bool isAssignment, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasCurrentCreationSentenceNr() )
				{
				if( searchSpecificationItem->storeSpecificationItemInFutureDatabase( isAssignment ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a specification item in the database" );
				}

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}
*/
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
		return ( isArchivedAssignment ? firstArchivedSpecificationItem() : ( isInactiveAssignment ? firstInactiveSpecificationItem() : firstActiveSpecificationItem() ) );
		}

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion ) : firstActiveSpecificationItem( isIncludingAnsweredQuestions, isQuestion ) );
		}

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter ) : firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

	SpecificationItem *nextSpecificationListItem()
		{
		return (SpecificationItem *)nextListItem();
		}


	protected:
	// Constructor

	SpecificationList( char _listChar, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		isMarkedAsSpecificationWord_ = false;

		initializeListVariables( _listChar, "SpecificationList", commonVariables, myWordItem );
		}

	~SpecificationList()
		{
		SpecificationItem *deleteSpecificationItem;
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			deleteSpecificationItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			delete deleteSpecificationItem;
			}

		searchSpecificationItem = firstInactiveSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			deleteSpecificationItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			delete deleteSpecificationItem;
			}

		searchSpecificationItem = firstArchivedSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			deleteSpecificationItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			delete deleteSpecificationItem;
			}

		searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			deleteSpecificationItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			delete deleteSpecificationItem;
			}

		searchSpecificationItem = (SpecificationItem *)firstDeletedItem();

		while( searchSpecificationItem != NULL )
			{
			deleteSpecificationItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			delete deleteSpecificationItem;
			}
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

	ResultType createNewAssignmentLevelInList()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevelInList";

		if( commonVariables()->currentAssignmentLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, "Assignment level overflow" );

		if( createNewAssignmentLevel( false, false ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to create an active assignment level" );

		if( createNewAssignmentLevel( true, false ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to create an inactive assignment level" );

		if( createNewAssignmentLevel( false, true ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to create an archive assignment level" );

		return RESULT_OK;
		}

	ResultType deleteAssignmentLevelInList()
		{
		SpecificationItem *searchSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAssignmentLevelInList";

		if( commonVariables()->currentAssignmentLevel <= NO_ASSIGNMENT_LEVEL )
			return startError( functionNameString, NULL, "The current assignment level is undefined" );

		if( deleteAssignmentLevelInList( firstActiveSpecificationItem() ) == RESULT_OK )
			{
			if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != NULL )
				deleteAssignmentLevelInList( searchSpecificationItem );

			if( ( searchSpecificationItem = firstArchivedSpecificationItem() ) != NULL )
				deleteAssignmentLevelInList( searchSpecificationItem );
			}

		return commonVariables()->result;
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

	SpecificationItem *firstActiveNonQuestionAssignmentItem( bool isDifferentRelationContext, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&

			( ( !isDifferentRelationContext &&
			searchSpecificationItem->relationContextNr() == relationContextNr ) ||

			( isDifferentRelationContext &&
			searchSpecificationItem->hasRelationContext() &&
			searchSpecificationItem->relationContextNr() != relationContextNr ) ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAnsweredQuestionAssignmentItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( true, false, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isAnsweredQuestion() &&
			searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			searchSpecificationItem->relationContextNr() == relationContextNr )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( true );
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion )
		{
		// This is the first assignment
		SpecificationItem *firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		// Now get the first valid assignment
		return ( firstAssignmentItem == NULL ? NULL : firstAssignmentItem->getAssignmentItem( isIncludingAnsweredQuestions, true, isQuestion ) );
		}

	SpecificationItem *firstAssignmentItem( bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter )
		{
		SpecificationItem *firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		return ( firstAssignmentItem == NULL ? NULL : firstAssignmentItem->getAssignmentItem( isIncludingAnsweredQuestions, true, questionParameter ) );
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			searchSpecificationItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			searchSpecificationItem->relationContextNr() == relationContextNr )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->isSelfGenerated() == isSelfGenerated &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			searchSpecificationItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstRecentlyReplacedAssignmentItem( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasCurrentReplacedSentenceNr() &&
			searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->questionParameter() == questionParameter &&
			searchSpecificationItem->relationContextNr() == relationContextNr &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
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
		myWordItem()->nextAssignmentWordItem = commonVariables()->firstAssignmentWordItem;
		commonVariables()->firstAssignmentWordItem = myWordItem();
		}

	void addToSpecificationWordQuickAccessList()
		{
		WordItem *tempWordItem;
		WordItem *lastSpecificationWordItem = commonVariables()->firstSpecificationWordItem;

		if( lastSpecificationWordItem == NULL )
			commonVariables()->firstSpecificationWordItem = myWordItem();
		else
			{
			// Word order is important: Add word at end of specification word list
			while( ( tempWordItem = lastSpecificationWordItem->nextSpecificationWordItem ) != NULL )
				lastSpecificationWordItem = tempWordItem;

			lastSpecificationWordItem->nextSpecificationWordItem = myWordItem();
			}
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
			if( searchSpecificationItem->hasSpecificationCompoundCollection() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasAnsweredSelfGeneratedQuestion()
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
			if( searchSpecificationItem->userNr() > NO_USER_NR )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasContextInSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedSpecification, unsigned int contextNr )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedSpecification ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->generalizationContextNr() == contextNr ||
				searchSpecificationItem->specificationContextNr() == contextNr ||
				searchSpecificationItem->relationContextNr() == contextNr )
					return true;

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
			}

		return false;
		}

	bool hasReplacedOrDeletedJustification( bool isAllowingNewerReplaceOrDeleteSentenceNr, bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr ) )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasFoundDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasDeletedJustification() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPartOf() &&
			!searchSpecificationItem->isPossessive() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasPossessiveButNonQuestionSpecification()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPossessive() )
				return true;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool isJustificationInUse( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedJustificationItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->hasJustification( !isReplacedItem, unusedJustificationItem ) )
					return true;

				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
				}
			}

		return false;
		}

	ResultType changeJustificationOfNegativeAssumptions( bool isInactiveAssignment, bool isArchivedAssignment, SpecificationItem *secondarySpecificationItem )
		{
		JustificationResultType justificationResult;
		unsigned int specificationCollectionNr;
		JustificationItem *firstJustificationItem;
		JustificationItem *foundOrCreatedJustificationItem;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeJustificationOfNegativeAssumptions";

		if( secondarySpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given secondary specification item is undefined" );

		specificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() &&
			searchSpecificationItem->isSelfGeneratedAssumption() &&
			searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
			( firstJustificationItem = searchSpecificationItem->firstJustificationItem() ) != NULL )
				{
				if( firstJustificationItem->isOlderItem() )
					{
					if( ( justificationResult = generalizationWordItem->addJustification( false, false, firstJustificationItem->justificationTypeNr(), firstJustificationItem->orderNr, firstJustificationItem->originalSentenceNr(), firstJustificationItem->primarySpecificationItem(), firstJustificationItem->anotherPrimarySpecificationItem(), secondarySpecificationItem, NULL, firstJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add a justification item" );

					foundOrCreatedJustificationItem = ( justificationResult.createdJustificationItem == NULL ? justificationResult.foundJustificationItem : justificationResult.createdJustificationItem );

					if( firstJustificationItem != foundOrCreatedJustificationItem )
						{
						if( generalizationWordItem->replaceJustification( firstJustificationItem, foundOrCreatedJustificationItem, searchSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace a justification of a negative assumption" );

						searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
						}
					else
						searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
					}
				else
					searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType checkJustificationForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given unused justification item is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasJustification( !isReplacedItem, unusedJustificationItem ) )
				return startError( functionNameString, NULL, "I have found a justification item that is still in use" );

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType checkSpecificationItemForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, SpecificationItem *unusedSpecificationItem )
		{
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationItemForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given unused specification item is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->replacingSpecificationItem == unusedSpecificationItem )
				return startError( functionNameString, NULL, "I have found a replacing specification item that is still in use" );

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType checkWordItemForUsage( bool isInactiveAssignment, bool isArchivedAssignment, WordItem *unusedWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, NULL, "The given unused word item is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->specificationWordItem() == unusedWordItem )
				return startError( functionNameString, NULL, "The specification word item is still in use" );

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType clearLastWrittenSentenceStringWithUnknownPluralNoun( bool isInactiveAssignment, bool isArchivedAssignment, const char *unknownPluralNounString, WordItem *specificationWordItem )
		{
		size_t unknownPluralNounStringLength;
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_LENGTH] = "clearLastWrittenSentenceStringWithUnknownPluralNoun";

		if( unknownPluralNounString == NULL )
			return startError( functionNameString, NULL, "The given unknown plural noun string is undefined" );

		if( specificationWordItem == NULL )
			return startError( functionNameString, NULL, "The given specification word item is undefined" );

		unknownPluralNounStringLength = strlen( unknownPluralNounString );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->specificationWordItem() == specificationWordItem &&
			strlen( searchSpecificationItem->lastWrittenSentenceString ) > 0 &&
			strncmp( searchSpecificationItem->lastWrittenSentenceString, unknownPluralNounString, unknownPluralNounStringLength ) > 0 )
				strcpy( searchSpecificationItem->lastWrittenSentenceString, EMPTY_STRING );

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType collectGeneralizationsOrSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationCollection, bool isQuestion, unsigned int collectionNr )
		{
		bool isCollectGeneralization;
		bool isCollectSpecification;
		bool isNewExclusiveSpecification;
		unsigned int generalizationCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
		WordItem *specificationWordItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectGeneralizationsOrSpecifications";

		if( collectionNr <= NO_COLLECTION_NR )
			return startError( functionNameString, NULL, "The given collection number is undefined" );

		while( searchSpecificationItem != NULL )
			{
			if( ( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL )
				{
				isCollectGeneralization = ( isGeneralizationCollection &&
											!searchSpecificationItem->hasGeneralizationCollection() &&
											generalizationWordItem->hasCollectionNr( collectionNr, specificationWordItem ) );

				isCollectSpecification = ( !isGeneralizationCollection &&
											!searchSpecificationItem->hasSpecificationCollection() &&
											specificationWordItem->hasCollectionNr( collectionNr ) );

				if( isCollectGeneralization ||
				isCollectSpecification )
					{
					if( searchSpecificationItem->hasCurrentCreationSentenceNr() )
						{
						if( searchSpecificationItem->collectSpecification( isCollectGeneralization, isExclusiveSpecification, collectionNr ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to collect a specification" );

						searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
						}
					else
						{
						if( !searchSpecificationItem->isOlderItem() )
							return startError( functionNameString, NULL, "The search item isn't old" );

						isNewExclusiveSpecification = ( isExclusiveSpecification ||
														searchSpecificationItem->isExclusiveSpecification() );

						generalizationCollectionNr = ( isCollectGeneralization ? collectionNr : searchSpecificationItem->generalizationCollectionNr() );
						specificationCollectionNr = ( isCollectSpecification ? collectionNr : searchSpecificationItem->specificationCollectionNr() );

						if( isNewExclusiveSpecification == searchSpecificationItem->isExclusiveSpecification() &&
						generalizationCollectionNr == searchSpecificationItem->generalizationCollectionNr() &&
						specificationCollectionNr == searchSpecificationItem->specificationCollectionNr() )
							return startError( functionNameString, NULL, "I couldn't find any changing parameter" );

						if( copyAndReplaceSpecificationItem( searchSpecificationItem->isAnsweredQuestion(), isNewExclusiveSpecification, generalizationCollectionNr, specificationCollectionNr, searchSpecificationItem->firstJustificationItem(), searchSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to copy and replace an older specification" );

						searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
						}
					}
				else
					searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType confirmSpecificationButNotItsRelation( bool isInactiveAssignment, bool isArchivedAssignment, SpecificationItem *confirmedSpecificationItem, SpecificationItem *confirmationSpecificationItem )
		{
		JustificationItem *foundJustificationItem;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_LENGTH] = "confirmSpecificationButNotItsRelation";

		if( confirmedSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given confirmed specification but not its relation(s) specification item is undefined" );

		if( !confirmedSpecificationItem->isSelfGenerated() )
			return startError( functionNameString, NULL, "The given confirmed specification but not its relation(s) specification item isn't self-generated" );

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->hasRelationContext() &&
			( foundJustificationItem = searchSpecificationItem->primaryOrSecondarySpecificationJustificationItem( confirmedSpecificationItem ) ) != NULL )
				{
				if( confirmSpecificationInJustification( ( foundJustificationItem->primarySpecificationItem() == confirmedSpecificationItem ), foundJustificationItem, confirmationSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to confirm a specification in the found justification" );

				searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *createdSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyAndReplaceSpecificationItem";

		if( originalSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given original specification item is undefined" );

		if( ( specificationResult = createSpecificationItem( originalSpecificationItem->isInactiveAssignment(), originalSpecificationItem->isArchivedAssignment(), isNewAnsweredQuestion, originalSpecificationItem->isConcludedAssumption(), originalSpecificationItem->isConditional(), originalSpecificationItem->isCorrectedAssumption(), originalSpecificationItem->isEveryGeneralization(), isNewExclusiveSpecification, originalSpecificationItem->isGeneralizationAssignment(), originalSpecificationItem->isNegative(), originalSpecificationItem->isPartOf(), originalSpecificationItem->isPossessive(), originalSpecificationItem->isSpecificationGeneralization(), originalSpecificationItem->isUniqueUserRelation(), originalSpecificationItem->isValueSpecification(), originalSpecificationItem->assignmentLevel(), originalSpecificationItem->assumptionLevel(), originalSpecificationItem->languageNr(), originalSpecificationItem->prepositionParameter(), originalSpecificationItem->questionParameter(), originalSpecificationItem->generalizationWordTypeNr(), originalSpecificationItem->specificationWordTypeNr(), originalSpecificationItem->relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem->generalizationContextNr(), originalSpecificationItem->specificationContextNr(), originalSpecificationItem->relationContextNr(), originalSpecificationItem->originalSentenceNr(), originalSpecificationItem->activeSentenceNr(), originalSpecificationItem->inactiveSentenceNr(), originalSpecificationItem->archivedSentenceNr(), originalSpecificationItem->nContextRelations(), newFirstJustificationItem, originalSpecificationItem->specificationWordItem(), originalSpecificationItem->specificationString(), originalSpecificationItem->lastWrittenSentenceString ) ).result != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to create a specification item with different parameters" );

		if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
			return startError( functionNameString, NULL, "I couldn't create a specification item" );

		if( replaceOrDeleteSpecification( false, originalSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to replace or delete the given specification item" );

		return RESULT_OK;
		}

	ResultType recalculateAssumptions( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_LENGTH] = "recalculateAssumptions";

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isSelfGeneratedAssumption() )
				{
				if( searchSpecificationItem->recalculateAssumption() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to recalculate an assumption" );
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType replaceOrDeleteSpecification( bool isAllowingReplacedObsoleteSpecification, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool isAssignment = isAssignmentList();
		JustificationItem *obsoleteJustificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		if( !isAllowingReplacedObsoleteSpecification &&
		obsoleteSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given obsolete specification item is already replaced or deleted" );

		if( obsoleteSpecificationItem == replacingSpecificationItem )
			return startError( functionNameString, NULL, "The given obsolete specification item and the given replacing specification item are the same specification item" );

		if( obsoleteSpecificationItem->isAssignment() != isAssignment )
			return startError( functionNameString, NULL, "The given obsolete specification item is an assignment item and I am a specification list, or the given obsolete specification item is a specification item and I am an assignment list" );

		if( !isAllowingReplacedObsoleteSpecification &&
		replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isAssignment() != isAssignment )
			return startError( functionNameString, NULL, "The given replacing specification item is an assignment item and I am a specification list, or the given replacing specification item is a specification item and I am an assignment list" );

		// Don't replace a normal specification by a hidden specification
		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isHiddenSpanishSpecification() &&
		!obsoleteSpecificationItem->isHiddenSpanishSpecification() &&
		obsoleteSpecificationItem->specificationCollectionNr() == replacingSpecificationItem->specificationCollectionNr() )
			return startError( functionNameString, NULL, "The given replacing specification item is a hidden specification, while the given obsolete specification item isn't hidden" );

		if( updateReplacedSpecifications( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to update the replacing specification item of the archive specification items" );

		obsoleteSpecificationItem->replacingSpecificationItem = replacingSpecificationItem;

		if( obsoleteSpecificationItem->hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to delete a specification item" );
			}
		else
			{
			if( !isAllowingReplacedObsoleteSpecification )
				{
				if( replaceItem( obsoleteSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to replace a specification item" );

				if( replacingSpecificationItem != NULL &&
				( obsoleteJustificationItem = obsoleteSpecificationItem->firstJustificationItem() ) != NULL )
					{
					if( ( replacingSpecificationItem->isUserSpecification() ||

					( replacingSpecificationItem->isSelfGeneratedQuestion() &&
					!replacingSpecificationItem->isAnsweredQuestion() ) ) &&

					!generalizationWordItem->isJustificationInUse( obsoleteJustificationItem ) )
						{
						// Remove justifications from an obsolete specification item
						if( removeJustificationsFromConfirmedAssumptionOrConclusion( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to remove the justifications from a confirmed assumption or conclusion" );
						}
					}
				}
			}

		if( generalizationWordItem->updateSpecificationsInJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to update the specifications in the justification of involved words" );

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->hasAnsweredQuestionInJustification() )
			{
			if( replaceOrDeleteSpecification( false, replacingSpecificationItem, NULL ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to replace or delete a replacing specification with an answered question in its justification" );
			}

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		bool isAssignment = isAssignmentList();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		if( storeChangesInFutureDatabase( isAssignment, firstActiveSpecificationItem() ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to store active specification items in the database" );

		if( storeChangesInFutureDatabase( isAssignment, firstInactiveSpecificationItem() ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to store inactive specification items in the database" );

		if( storeChangesInFutureDatabase( isAssignment, firstArchivedSpecificationItem() ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to store archived specification items in the database" );

		if( storeChangesInFutureDatabase( isAssignment, firstReplacedSpecificationItem() ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to store replaced specification items in the database" );

		return RESULT_OK;
		}
*/
	ResultType updateJustificationInSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplacedItem, bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool isSameJustification;
		JustificationItem *attachedPredecessorOfOldJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateJustificationInSpecifications";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

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
						if( searchSpecificationItem->changeFirstJustification( replacingJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the first justification item of a specification item" );

						searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
						}
					else
						{
						createdSpecificationItem = NULL;

						if( replacingJustificationItem != NULL )
							{
							if( ( specificationResult = createSpecificationItem( searchSpecificationItem->isInactiveAssignment(), isArchivedAssignment, searchSpecificationItem->isAnsweredQuestion(), searchSpecificationItem->isConcludedAssumption(), searchSpecificationItem->isConditional(), searchSpecificationItem->isCorrectedAssumption(), searchSpecificationItem->isEveryGeneralization(), ( isExclusiveGeneralization || searchSpecificationItem->isExclusiveSpecification() ), searchSpecificationItem->isGeneralizationAssignment(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->isValueSpecification(), searchSpecificationItem->assignmentLevel(), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->languageNr(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->generalizationWordTypeNr(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationCollectionNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), ( isExclusiveGeneralization ? NO_CONTEXT_NR : searchSpecificationItem->relationContextNr() ), searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), replacingJustificationItem, searchSpecificationItem->specificationWordItem(), searchSpecificationItem->specificationString(), searchSpecificationItem->lastWrittenSentenceString ) ).result != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to copy the search specification item" );

							if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
								return startError( functionNameString, NULL, "I couldn't copy the search specification item" );
							}

						if( replaceOrDeleteSpecification( false, searchSpecificationItem, createdSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace or delete a specification" );

						if( isExclusiveGeneralization )
							{
							if( generalizationWordItem->assignSpecification( false, false, true, false, searchSpecificationItem->isNegative(), searchSpecificationItem->isPartOf(), searchSpecificationItem->isPossessive(), searchSpecificationItem->isSpecificationGeneralization(), searchSpecificationItem->isUniqueUserRelation(), searchSpecificationItem->assumptionLevel(), searchSpecificationItem->prepositionParameter(), searchSpecificationItem->questionParameter(), searchSpecificationItem->relationWordTypeNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->originalSentenceNr(), searchSpecificationItem->activeSentenceNr(), searchSpecificationItem->inactiveSentenceNr(), searchSpecificationItem->archivedSentenceNr(), searchSpecificationItem->nContextRelations(), replacingJustificationItem, searchSpecificationItem->specificationWordItem(), NULL, searchSpecificationItem->specificationString() ).result != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to create an assignment" );
							}

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
								return addError( functionNameString, NULL, "I failed to change the attached justification item of a justification item" );

							searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
							}
						else
							{
							if( ( justificationResult = generalizationWordItem->copyJustificationItem( attachedPredecessorOfOldJustificationItem->primarySpecificationItem(), attachedPredecessorOfOldJustificationItem->secondarySpecificationItem(), replacingJustificationItem, attachedPredecessorOfOldJustificationItem ) ).result != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to copy the attached predecessor of obsolete justification item with a different first justification item" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL )
								{
								if( generalizationWordItem->replaceJustification( attachedPredecessorOfOldJustificationItem, createdJustificationItem, searchSpecificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to replace the attached predecessor of obsolete justification item by a created justification item" );

								searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
								}
							else
								searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
							}
						}
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}

	SpecificationResultType createSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *writtenSentenceString )
		{
		SpecificationResultType specificationResult;
		bool isAssignment = isAssignmentList();
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSpecificationItem";

		if( generalizationWordTypeNr <= NO_WORD_TYPE_NR &&
		generalizationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startSpecificationResultError( functionNameString, NULL, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= NO_WORD_TYPE_NR &&
		specificationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startSpecificationResultError( functionNameString, NULL, "The given specification word type number is undefined or out of bounds" );

		if( firstJustificationItem != NULL &&
		!firstJustificationItem->isActiveItem() )
			return startSpecificationResultError( functionNameString, NULL, "The given first justification item isn't active" );

		if( specificationWordItem != NULL &&
		specificationWordItem->isNounValue() )
			return startSpecificationResultError( functionNameString, NULL, "The given specification word item is a value word" );

		if( !isMarkedAsSpecificationWord_ )
			{
			isMarkedAsSpecificationWord_ = true;

			if( isAssignment )
				addToAssignmentWordQuickAccessList();
			else
				addToSpecificationWordQuickAccessList();
			}

		if( ( specificationResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, generalizationWordItem->isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, commonVariables(), this, generalizationWordItem ) ) == NULL )
			return startSpecificationResultError( functionNameString, NULL, "I failed to create a specification item" );

		if( addItemToList( ( isArchivedAssignment ? QUERY_ARCHIVED_CHAR : ( isInactiveAssignment ? QUERY_INACTIVE_CHAR : QUERY_ACTIVE_CHAR ) ), specificationResult.createdSpecificationItem ) != RESULT_OK )
			return addSpecificationResultError( functionNameString, NULL, "I failed to add an assignment item" );

		if( isAssignment &&
		assignmentLevel == NO_ASSIGNMENT_LEVEL &&
		originalSentenceNr == commonVariables()->currentSentenceNr )
			commonVariables()->isAssignmentChanged = true;

		if( writtenSentenceString != NULL &&
		strlen( writtenSentenceString ) > 0 )
			strcpy( specificationResult.createdSpecificationItem->lastWrittenSentenceString, writtenSentenceString );

		return specificationResult;
		}

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyContextResult, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->isMatchingGeneralizationContextNr( isAllowingEmptyContextResult, generalizationContextNr ) &&
			searchSpecificationItem->isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > NO_COLLECTION_NR &&
				!searchSpecificationItem->isSpecificationGeneralization() &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
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

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isSpecificationWordSpanishAmbiguous;
		bool hasRelationWord = ( relationWordItem != NULL );
		unsigned int nCurrentRelationContextWords;
		unsigned int nMinimumRelationContextWords = MAX_NUMBER;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&

				( relationWordItem == NULL ||
				relationWordItem->hasContextInWord( searchSpecificationItem->relationContextNr(), specificationWordItem ) ) )
					{
					if( ( isPossessive &&
					!isSpecificationWordSpanishAmbiguous ) ||

					( !hasRelationWord &&
					isSpecificationWordSpanishAmbiguous ) )
						return searchSpecificationItem;

					nCurrentRelationContextWords = searchSpecificationItem->nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		unsigned int nCurrentRelationContextWords;
		unsigned int nMinimumRelationContextWords = MAX_NUMBER;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				searchSpecificationItem->isMatchingRelationContextNr( true, relationContextNr ) )
					{
					nCurrentRelationContextWords = searchSpecificationItem->nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAnsweredQuestions, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		unsigned int nCurrentRelationContextWords;
		unsigned int nMinimumRelationContextWords = MAX_NUMBER;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&
				searchSpecificationItem->isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
					{
					if( !hasRelationContext ||
					!searchSpecificationItem->hasRelationContext() ||
					searchSpecificationItem->relationContextNr() == relationContextNr )
						return searchSpecificationItem;

					nCurrentRelationContextWords = searchSpecificationItem->nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *firstActiveSpecificationItem()
		{
		return (SpecificationItem *)firstActiveItem();
		}

	SpecificationItem *firstActiveSpecificationItem( bool isIncludingAnsweredQuestions, bool isQuestion )
		{
		SpecificationItem *firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			return firstSpecificationItem->getSpecificationItem( isIncludingAnsweredQuestions, true, isQuestion );

		return NULL;
		}

	SpecificationItem *firstActiveSpecificationItem( bool isIncludingAnsweredQuestions, unsigned short questionParameter )
		{
		SpecificationItem *firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != NULL )
			return firstSpecificationItem->getSpecificationItem( isIncludingAnsweredQuestions, true, questionParameter );

		return NULL;
		}

	SpecificationItem *firstFeminineOrMasculineSpecificationItem()
		{
		SpecificationItem *searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem *specificationWordItem;

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isNegative() &&
			!searchSpecificationItem->isPossessive() &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL )
				{
				if( specificationWordItem->isFemaleOrMale() )
					return searchSpecificationItem;
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
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
				searchSpecificationItem->specificationCollectionNr() != compoundSpecificationCollectionNr &&
				( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( searchSpecificationItem->hasSpecificationNonCompoundCollection() &&
					specificationWordItem->hasCollectionNr( compoundSpecificationCollectionNr ) )
						return searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstRecentlyAnsweredQuestionSpecificationItem( bool isArchivedAssignment, unsigned short questionParameter )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( true, false, isArchivedAssignment, questionParameter );

		if( searchSpecificationItem != NULL )
			return searchSpecificationItem->recentlyAnsweredQuestionSpecificationItem( true );

		return NULL;
		}

	SpecificationItem *firstRelationSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( relationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isPossessive() == isPossessive &&
				relationWordItem->hasContextInWord( searchSpecificationItem->relationContextNr(), searchSpecificationItem->specificationWordItem() ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
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

				( isAllowingEmptyContextResult ||
				searchSpecificationItem->hasRelationContext() ) ) ||

				( relationWordItem != NULL &&
				relationWordItem->hasContextInWord( searchSpecificationItem->relationContextNr(), specificationWordItem ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
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
				searchSpecificationItem->isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) &&

				( searchSpecificationItem->specificationCollectionNr() == specificationCollectionNr ||

				// If specification collection doesn't match,
				// check for compound collection to avoid duplicates
				( ( relationContextNr == NO_CONTEXT_NR ||
				!searchSpecificationItem->hasRelationContext() ) &&

				specificationWordItem->isCompoundCollection( specificationCollectionNr ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isIncludingAnsweredQuestions, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->specificationWordTypeNr() == specificationWordTypeNr &&
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

	SpecificationItem *firstSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		WordItem *anyWordItem = myWordItem();
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchSpecificationItem != NULL )
				{
				if( !searchSpecificationItem->isSpecificationGeneralization() &&
				searchSpecificationItem->isNegative() == isNegative &&
				searchSpecificationItem->isPossessive() == isPossessive &&
				searchSpecificationItem->generalizationContextNr() == generalizationContextNr &&
				searchSpecificationItem->specificationContextNr() == specificationContextNr &&
				searchSpecificationItem->specificationWordItem() == specificationWordItem &&

				( ( relationContextNr == NO_CONTEXT_NR &&
				!searchSpecificationItem->hasRelationContext() ) ||

				anyWordItem->isContextSubsetInAllWords( relationContextNr, searchSpecificationItem->relationContextNr() ) ) )
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

	SpecificationItem *firstSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() == isNegative &&
			searchSpecificationItem->isPossessive() == isPossessive &&
			searchSpecificationItem->specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstUnhiddenSpecificationItem( bool isArchivedAssignment, unsigned int relationContextNr )
		{
		SpecificationItem *searchSpecificationItem = firstSpecificationItem( false, isArchivedAssignment );

		if( relationContextNr > NO_CONTEXT_NR )
			{
			while( searchSpecificationItem != NULL )
				{
				if( searchSpecificationItem->relationContextNr() == relationContextNr &&
				!searchSpecificationItem->isHiddenSpanishSpecification() &&
				searchSpecificationItem->wasHiddenSpanishSpecification() )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
				}
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

	SpecificationItem *sameUserQuestionSpecificationItem( bool isArchivedAssignment, unsigned short questionParameter )
		{
		unsigned int nUserSpecificationWords = commonVariables()->nUserSpecificationWords;
		SpecificationItem *searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isUserQuestion() &&
			searchSpecificationItem->nInvolvedSpecificationWords( true ) == nUserSpecificationWords )
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
		WordItem *generalizationWordItem = myWordItem();

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isExclusiveSpecification() &&
			!searchSpecificationItem->isNegative() &&
			!searchSpecificationItem->isPossessive() &&
			( specificationWordItem = searchSpecificationItem->specificationWordItem() ) != NULL )
				{
				if( specificationWordItem->isFemaleOrMale() &&
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
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}
	};
#endif

/*************************************************************************
 *	"Good comes to those who lend money generously
 *	and conduct their business fairly.
 *	Such people will not be overcome by evil.
 *	Those who are righteous will be long remembered" (Psalm 112:5-6)
 *************************************************************************/
