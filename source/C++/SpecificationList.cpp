/*	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		To store specification items
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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
	friend class WordSpecification;

	// Private assignment functions

	ResultType createNewAssignmentLevel( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevel";

		if( searchItem != NULL &&
		searchItem->assignmentLevel() > commonVariables()->currentAssignmentLevel )
			searchItem = searchItem->nextAssignmentItemWithCurrentLevel();

		while( searchItem != NULL )
			{
			if( createSpecificationItem( isInactiveAssignment, isArchivedAssignment, searchItem->isAnsweredQuestion(), searchItem->isConcludedAssumption(), searchItem->isConditional(), searchItem->isCorrectedAssumption(), searchItem->isEveryGeneralization(), searchItem->isExclusiveSpecification(), searchItem->isGeneralizationAssignment(), searchItem->isNegative(), searchItem->isPartOf(), searchItem->isPossessive(), searchItem->isSpecificationGeneralization(), searchItem->isUniqueUserRelation(), searchItem->isValueSpecification(), (unsigned short)(searchItem->assignmentLevel() + 1 ), searchItem->assumptionLevel(), searchItem->languageNr(), searchItem->prepositionParameter(), searchItem->questionParameter(), searchItem->generalizationWordTypeNr(), searchItem->specificationWordTypeNr(), searchItem->relationWordTypeNr(), searchItem->generalizationCollectionNr(), searchItem->specificationCollectionNr(), searchItem->generalizationContextNr(), searchItem->specificationContextNr(), searchItem->relationContextNr(), searchItem->originalSentenceNr(), searchItem->activeSentenceNr(), searchItem->inactiveSentenceNr(), searchItem->archivedSentenceNr(), searchItem->nContextRelations(), searchItem->firstJustificationItem(), searchItem->specificationWordItem(), searchItem->specificationString(), searchItem->lastWrittenSentenceString ).result == RESULT_OK )
				searchItem = searchItem->nextAssignmentItemWithCurrentLevel();
			else
				return addError( functionNameString, NULL, "I failed to copy an assignment item with incremented assignment level" );
			}

		return RESULT_OK;
		}

	ResultType deleteAssignmentLevelInList( SpecificationItem *searchItem )
		{
		unsigned short currentAssignmentLevel = commonVariables()->currentAssignmentLevel;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAssignmentLevelInList";

		while( searchItem != NULL &&
		searchItem->assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchItem->assignmentLevel() == currentAssignmentLevel )
				{
				if( deleteItem( searchItem ) == RESULT_OK )
					searchItem = nextSpecificationListItem();
				else
					return addError( functionNameString, NULL, "I failed to delete an item" );
				}
			else
				searchItem = searchItem->nextSpecificationItem();
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

		if( obsoleteSpecificationItem != NULL )
			{
			if( replacingSpecificationItem != NULL )
				{
				isUserSpecification = replacingSpecificationItem->isUserSpecification();

				if( ( currentJustificationItem = obsoleteSpecificationItem->firstJustificationItem() ) != NULL )
					{
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
					}
				else
					return startError( functionNameString, NULL, "The given obsolete specification item has no justifications" );
				}
			else
				return startError( functionNameString, NULL, "The given replacing specification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		return RESULT_OK;
		}

	ResultType updateReplacedSpecifications( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		SpecificationItem *searchItem = firstReplacedSpecificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateReplacedSpecifications";

		if( obsoleteSpecificationItem != NULL )
			{
			if( replacingSpecificationItem == NULL ||
			!replacingSpecificationItem->isReplacedItem() )
				{
				while( searchItem != NULL )
					{
					if( searchItem->replacingSpecificationItem == obsoleteSpecificationItem )
						searchItem->replacingSpecificationItem = replacingSpecificationItem;

					searchItem = searchItem->nextSpecificationItem();
					}
				}
			else
				return startError( functionNameString, NULL, "The given replaced specification item is a replaced item" );
			}
		else
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		return RESULT_OK;
		}


	// Private specification functions

	ResultType confirmSpecificationInJustification( bool isConfirmedPrimarySpecification, JustificationItem *obsoleteJustificationItem, SpecificationItem *confirmationSpecificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *createdJustificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "confirmSpecificationInJustification";

		if( obsoleteJustificationItem != NULL )
			{
			if( confirmationSpecificationItem != NULL )
				{
				if( confirmationSpecificationItem->isUserSpecification() )
					{
					if( ( justificationResult = generalizationWordItem->copyJustificationItem( ( isConfirmedPrimarySpecification ? confirmationSpecificationItem : obsoleteJustificationItem->primarySpecificationItem() ), ( isConfirmedPrimarySpecification ? obsoleteJustificationItem->secondarySpecificationItem() : confirmationSpecificationItem ), obsoleteJustificationItem->attachedJustificationItem(), obsoleteJustificationItem ) ).result == RESULT_OK )
						{
						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL )
							{
							if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, confirmationSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to replace the given obsolete justification item by a created justification item" );
							}
						else
							return startError( functionNameString, NULL, "I couldn't create a justification item" );
						}
					else
						return addError( functionNameString, NULL, "I failed to copy the primary specification Justification item with a different primary specification item" );
					}
				else
					return startError( functionNameString, NULL, "The given confirmation specification item isn't a user specification" );
				}
			else
				return startError( functionNameString, NULL, "The given confirmation specification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

		return RESULT_OK;
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
	// Constructor / deconstructor

	SpecificationList( char _listChar, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( _listChar, "SpecificationList", commonVariables, myWordItem );
		}

	~SpecificationList()
		{
		SpecificationItem *deleteItem;
		SpecificationItem *searchItem = firstActiveSpecificationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSpecificationItem();
			delete deleteItem;
			}

		searchItem = firstInactiveSpecificationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSpecificationItem();
			delete deleteItem;
			}

		searchItem = firstArchivedSpecificationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSpecificationItem();
			delete deleteItem;
			}

		searchItem = firstReplacedSpecificationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSpecificationItem();
			delete deleteItem;
			}

		searchItem = (SpecificationItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSpecificationItem();
			delete deleteItem;
			}
		}


	// Protected assignment functions

	unsigned int nActiveAssignments()
		{
		unsigned int nItems = 0;
		SpecificationItem *searchItem = firstAssignmentItem( false, false, false, false );

		while( searchItem != NULL )
			{
			nItems++;
			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return nItems;
		}

	ResultType createNewAssignmentLevelInList()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevelInList";

		if( commonVariables()->currentAssignmentLevel < MAX_LEVEL )
			{
			if( createNewAssignmentLevel( false, false ) == RESULT_OK )
				{
				if( createNewAssignmentLevel( true, false ) == RESULT_OK )
					{
					if( createNewAssignmentLevel( false, true ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to create an archive assignment level" );
					}
				else
					return addError( functionNameString, NULL, "I failed to create an inactive assignment level" );
				}
			else
				return addError( functionNameString, NULL, "I failed to create an active assignment level" );
			}
		else
			return startSystemError( functionNameString, NULL, "Assignment level overflow" );

		return RESULT_OK;
		}

	ResultType deleteAssignmentLevelInList()
		{
		SpecificationItem *searchItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAssignmentLevelInList";

		if( commonVariables()->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL )
			{
			if( deleteAssignmentLevelInList( firstActiveSpecificationItem() ) == RESULT_OK )
				{
				if( ( searchItem = firstInactiveSpecificationItem() ) != NULL )
					deleteAssignmentLevelInList( searchItem );

				if( commonVariables()->result == RESULT_OK &&
				( searchItem = firstArchivedSpecificationItem() ) != NULL )
					deleteAssignmentLevelInList( searchItem );
				}
			}
		else
			return startError( functionNameString, NULL, "The current assignment level is undefined" );

		return commonVariables()->result;
		}

	SpecificationItem *firstActiveNumeralAssignmentItem()
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSpecificationNumeral() )
				return searchItem;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstActiveStringAssignmentItem()
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->specificationString() != NULL )
				return searchItem;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstActiveNonQuestionAssignmentItem( bool isDifferentRelationContext, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentItem( false, false, false, false );

		while( searchItem != NULL )
			{
			if( searchItem->isPossessive() == isPossessive &&
			searchItem->specificationWordItem() == specificationWordItem &&

			( ( !isDifferentRelationContext &&
			searchItem->relationContextNr() == relationContextNr ) ||

			( isDifferentRelationContext &&
			searchItem->hasRelationContext() &&
			searchItem->relationContextNr() != relationContextNr ) ) )
				return searchItem;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAnsweredQuestionAssignmentItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentItem( true, false, isArchivedAssignment, questionParameter );

		while( searchItem != NULL )
			{
			if( searchItem->isAnsweredQuestion() &&
			searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->specificationWordItem() == specificationWordItem &&
			searchItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem( true );
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
		SpecificationItem *searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != NULL )
			{
			if( searchItem->isPossessive() == isPossessive &&
			searchItem->specificationWordItem() == specificationWordItem &&
			searchItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchItem != NULL )
			{
			if( searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->specificationWordItem() == specificationWordItem &&
			searchItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != NULL )
			{
			if( searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->isSelfGenerated() == isSelfGenerated &&
			searchItem->specificationWordItem() == specificationWordItem &&
			searchItem->isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstRecentlyReplacedAssignmentItem( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstReplacedSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentReplacedSentenceNr() &&
			searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->questionParameter() == questionParameter &&
			searchItem->relationContextNr() == relationContextNr &&
			searchItem->specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *lastActiveNonQuestionAssignmentItem()
		{
		SpecificationItem *lastAssignmentItem = NULL;
		SpecificationItem *searchItem = firstActiveSpecificationItem();

		while( searchItem != NULL )
			{
			lastAssignmentItem = searchItem;
			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return lastAssignmentItem;
		}


	// Protected specification functions

	void initializeSpecificationVariables( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchItem != NULL )
			{
			searchItem->initializeSpecificationVariables();
			searchItem = searchItem->nextSpecificationItem();
			}
		}

	bool hasActiveQuestionWithCompoundCollection()
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, true );

		while( searchItem != NULL )
			{
			if( searchItem->hasSpecificationCompoundCollection() )
				return true;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasAnsweredQuestion()
		{
		JustificationItem *firstJustificationItem;
		SpecificationItem *searchItem = firstActiveSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->isAnsweredQuestion() &&
			( firstJustificationItem = searchItem->firstJustificationItem() ) != NULL )
				{
				if( firstJustificationItem->hasPrimaryUserSpecification() )
					return true;
				}

			searchItem = searchItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasFoundAnyUserSpecification()
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->userNr() > NO_USER_NR )
				return true;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasFoundReplacedOrDeletedJustification( bool isAllowingNewerReplaceOrDeleteSentenceNr, bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchItem != NULL )
			{
			if( searchItem->hasFoundReplacedOrDeletedJustification( isAllowingNewerReplaceOrDeleteSentenceNr ) )
				return true;

			searchItem = searchItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasFoundDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem *searchItem = firstReplacedSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasFoundDeletedJustification() )
				return true;

			searchItem = searchItem->nextSpecificationItem();
			}

		return false;
		}

	bool hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, false );

		while( searchItem != NULL )
			{
			if( searchItem->isPartOf() &&
			!searchItem->isPossessive() )
				return true;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool hasPossessiveButNonQuestionSpecification()
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, false );

		while( searchItem != NULL )
			{
			if( searchItem->isPossessive() )
				return true;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return false;
		}

	bool isJustificationInUse( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplaced, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchItem = ( isReplaced ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedJustificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->hasFoundJustification( !isReplaced, unusedJustificationItem ) )
					return true;

				searchItem = searchItem->nextSpecificationItem();
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
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeJustificationOfNegativeAssumptions";

		if( secondarySpecificationItem != NULL )
			{
			specificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

			while( searchItem != NULL )
				{
				if( searchItem->isNegative() &&
				searchItem->isSelfGeneratedAssumption() &&
				searchItem->specificationCollectionNr() == specificationCollectionNr &&
				( firstJustificationItem = searchItem->firstJustificationItem() ) != NULL )
					{
					if( firstJustificationItem->isOlderItem() )
						{
						if( ( justificationResult = generalizationWordItem->addJustification( false, false, firstJustificationItem->justificationTypeNr(), firstJustificationItem->orderNr, firstJustificationItem->originalSentenceNr(), firstJustificationItem->primarySpecificationItem(), firstJustificationItem->anotherPrimarySpecificationItem(), secondarySpecificationItem, NULL, firstJustificationItem->attachedJustificationItem() ) ).result == RESULT_OK )
							{
							foundOrCreatedJustificationItem = ( justificationResult.createdJustificationItem == NULL ? justificationResult.foundJustificationItem : justificationResult.createdJustificationItem );

							if( firstJustificationItem != foundOrCreatedJustificationItem )
								{
								if( generalizationWordItem->replaceJustification( firstJustificationItem, foundOrCreatedJustificationItem, searchItem ) == RESULT_OK )
									searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
								else
									return addError( functionNameString, NULL, "I failed to replace a justification of a negative assumption" );
								}
							else
								searchItem = searchItem->nextSelectedSpecificationItem();
							}
						else
							return addError( functionNameString, NULL, "I failed to add a justification item" );
						}
					else
						searchItem = searchItem->nextSelectedSpecificationItem();
					}
				else
					searchItem = searchItem->nextSelectedSpecificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType checkJustificationForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplaced, JustificationItem *unusedJustificationItem )
		{
		SpecificationItem *searchItem = ( isReplaced ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->hasFoundJustification( !isReplaced, unusedJustificationItem ) )
					return startError( functionNameString, NULL, "I have found a justification item that is still in use" );

				searchItem = searchItem->nextSpecificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given unused justification item is undefined" );

		return RESULT_OK;
		}

	ResultType checkSpecificationItemForUsage( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplaced, SpecificationItem *unusedSpecificationItem )
		{
		SpecificationItem *searchItem = ( isReplaced ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationItemForUsage";

		if( unusedSpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->replacingSpecificationItem == unusedSpecificationItem )
					return startError( functionNameString, NULL, "I have found a replacing specification item that is still in use" );

				searchItem = searchItem->nextSpecificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given unused specification item is undefined" );

		return RESULT_OK;
		}

	ResultType checkWordItemForUsage( bool isInactiveAssignment, bool isArchivedAssignment, WordItem *unusedWordItem )
		{
		SpecificationItem *searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->specificationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, "The specification word item is still in use" );

				searchItem = searchItem->nextSpecificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given unused word item is undefined" );

		return RESULT_OK;
		}

	ResultType clearLastWrittenSentenceStringWithUnknownPluralNoun( bool isInactiveAssignment, bool isArchivedAssignment, const char *unknownPluralNounString, WordItem *specificationWordItem )
		{
		size_t unknownPluralNounStringLength;
		SpecificationItem *searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		char functionNameString[FUNCTION_NAME_LENGTH] = "clearLastWrittenSentenceStringWithUnknownPluralNoun";

		if( unknownPluralNounString != NULL )
			{
			if( specificationWordItem != NULL )
				{
				unknownPluralNounStringLength = strlen( unknownPluralNounString );

				while( searchItem != NULL )
					{
					if( searchItem->specificationWordItem() == specificationWordItem &&
					strlen( searchItem->lastWrittenSentenceString ) > 0 &&
					strncmp( searchItem->lastWrittenSentenceString, unknownPluralNounString, unknownPluralNounStringLength ) > 0 )
						strcpy( searchItem->lastWrittenSentenceString, EMPTY_STRING );

					searchItem = searchItem->nextSpecificationItem();
					}
				}
			else
				return startError( functionNameString, NULL, "The given specification word item is undefined" );
			}
		else
			return startError( functionNameString, NULL, "The given unknown plural noun string is undefined" );

		return RESULT_OK;
		}

	ResultType collectGeneralizationsOrSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationCollection, bool isQuestion, unsigned int collectionNr )
		{
		bool isCollectGeneralization;
		bool isCollectSpecification;
		bool isNewExclusiveSpecification;
		unsigned int generalizationCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
		WordItem *specificationWordItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectGeneralizationsOrSpecifications";

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( ( specificationWordItem = searchItem->specificationWordItem() ) != NULL )
					{
					isCollectGeneralization = ( isGeneralizationCollection &&
											!searchItem->hasGeneralizationCollection() &&
											generalizationWordItem->hasCollectionNr( collectionNr, specificationWordItem ) );

					isCollectSpecification = ( !isGeneralizationCollection &&
											!searchItem->hasSpecificationCollection() &&
											specificationWordItem->hasCollectionNr( collectionNr ) );

					if( isCollectGeneralization ||
					isCollectSpecification )
						{
						if( searchItem->hasCurrentCreationSentenceNr() )
							{
							if( searchItem->collectSpecification( isCollectGeneralization, isExclusiveSpecification, collectionNr ) == RESULT_OK )
								searchItem = searchItem->nextSelectedSpecificationItem();
							else
								return addError( functionNameString, NULL, "I failed to collect a specification" );
							}
						else
							{
							if( searchItem->isOlderItem() )
								{
								isNewExclusiveSpecification = ( isExclusiveSpecification ||
																searchItem->isExclusiveSpecification() );

								generalizationCollectionNr = ( isCollectGeneralization ? collectionNr : searchItem->generalizationCollectionNr() );
								specificationCollectionNr = ( isCollectSpecification ? collectionNr : searchItem->specificationCollectionNr() );

								if( isNewExclusiveSpecification != searchItem->isExclusiveSpecification() ||
								generalizationCollectionNr != searchItem->generalizationCollectionNr() ||
								specificationCollectionNr != searchItem->specificationCollectionNr() )
									{
									if( copyAndReplaceSpecificationItem( searchItem->isAnsweredQuestion(), isNewExclusiveSpecification, generalizationCollectionNr, specificationCollectionNr, searchItem->firstJustificationItem(), searchItem ) == RESULT_OK )
										searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
									else
										return addError( functionNameString, NULL, "I failed to copy and replace an older specification" );
									}
								else
									return startError( functionNameString, NULL, "I couldn't find any changing parameter" );
								}
							else
								return startError( functionNameString, NULL, "The search item isn't old" );
							}
						}
					else
						searchItem = searchItem->nextSelectedSpecificationItem();
					}
				else
					searchItem = searchItem->nextSelectedSpecificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given collection number is undefined" );

		return RESULT_OK;
		}

	ResultType confirmSpecificationButNotItsRelation( bool isInactiveAssignment, bool isArchivedAssignment, SpecificationItem *confirmedSpecificationItem, SpecificationItem *confirmationSpecificationItem )
		{
		JustificationItem *foundJustificationItem;
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_LENGTH] = "confirmSpecificationButNotItsRelation";

		if( confirmedSpecificationItem != NULL )
			{
			if( confirmedSpecificationItem->isSelfGenerated() )
				{
				while( searchItem != NULL )
					{
					if( !searchItem->hasRelationContext() &&
					( foundJustificationItem = searchItem->primaryOrSecondarySpecificationJustificationItem( confirmedSpecificationItem ) ) != NULL )
						{
						if( confirmSpecificationInJustification( ( foundJustificationItem->primarySpecificationItem() == confirmedSpecificationItem ), foundJustificationItem, confirmationSpecificationItem ) == RESULT_OK )
							searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
						else
							return addError( functionNameString, NULL, "I failed to confirm a specification in the found justification" );
						}
					else
						searchItem = searchItem->nextSelectedSpecificationItem();
					}
				}
			else
				return startError( functionNameString, NULL, "The given confirmed specification but not its relation(s) specification item isn't self-generated" );
			}
		else
			return startError( functionNameString, NULL, "The given confirmed specification but not its relation(s) specification item is undefined" );

		return RESULT_OK;
		}

	ResultType copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *createdSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyAndReplaceSpecificationItem";

		if( originalSpecificationItem != NULL )
			{
			if( ( specificationResult = createSpecificationItem( originalSpecificationItem->isInactiveAssignment(), originalSpecificationItem->isArchivedAssignment(), isNewAnsweredQuestion, originalSpecificationItem->isConcludedAssumption(), originalSpecificationItem->isConditional(), originalSpecificationItem->isCorrectedAssumption(), originalSpecificationItem->isEveryGeneralization(), isNewExclusiveSpecification, originalSpecificationItem->isGeneralizationAssignment(), originalSpecificationItem->isNegative(), originalSpecificationItem->isPartOf(), originalSpecificationItem->isPossessive(), originalSpecificationItem->isSpecificationGeneralization(), originalSpecificationItem->isUniqueUserRelation(), originalSpecificationItem->isValueSpecification(), originalSpecificationItem->assignmentLevel(), originalSpecificationItem->assumptionLevel(), originalSpecificationItem->languageNr(), originalSpecificationItem->prepositionParameter(), originalSpecificationItem->questionParameter(), originalSpecificationItem->generalizationWordTypeNr(), originalSpecificationItem->specificationWordTypeNr(), originalSpecificationItem->relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem->generalizationContextNr(), originalSpecificationItem->specificationContextNr(), originalSpecificationItem->relationContextNr(), originalSpecificationItem->originalSentenceNr(), originalSpecificationItem->activeSentenceNr(), originalSpecificationItem->inactiveSentenceNr(), originalSpecificationItem->archivedSentenceNr(), originalSpecificationItem->nContextRelations(), newFirstJustificationItem, originalSpecificationItem->specificationWordItem(), originalSpecificationItem->specificationString(), originalSpecificationItem->lastWrittenSentenceString ) ).result == RESULT_OK )
				{
				if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
					{
					if( replaceOrDeleteSpecification( originalSpecificationItem, createdSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace or delete the given specification item" );
					}
				else
					return startError( functionNameString, NULL, "I couldn't create a specification item" );
				}
			else
				return addError( functionNameString, NULL, "I failed to create a specification item with different parameters" );
			}
		else
			return startError( functionNameString, NULL, "The given original specification item is undefined" );

		return RESULT_OK;
		}

	ResultType recalculateAssumptions( bool isInactiveAssignment, bool isArchivedAssignment )
		{
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		char functionNameString[FUNCTION_NAME_LENGTH] = "recalculateAssumptions";

		while( searchItem != NULL )
			{
			if( searchItem->isSelfGeneratedAssumption() )
				{
				if( searchItem->recalculateAssumption() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to recalculate an assumption" );
				}

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return RESULT_OK;
		}

	ResultType replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool isAssignment = isAssignmentList();
		JustificationItem *obsoleteJustificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem != NULL )
			{
			if( !obsoleteSpecificationItem->isReplacedOrDeletedItem() )
				{
				if( obsoleteSpecificationItem != replacingSpecificationItem )
					{
					if( obsoleteSpecificationItem->isAssignment() == isAssignment )
						{
						if( replacingSpecificationItem == NULL ||
						replacingSpecificationItem->isAssignment() == isAssignment )
							{
							// Don't replace a normal specification by a hidden specification
							if( replacingSpecificationItem == NULL ||
							!replacingSpecificationItem->isHiddenSpanishSpecification() ||
							obsoleteSpecificationItem->isHiddenSpanishSpecification() ||
							obsoleteSpecificationItem->specificationCollectionNr() != replacingSpecificationItem->specificationCollectionNr() )
								{
								if( updateReplacedSpecifications( obsoleteSpecificationItem, replacingSpecificationItem ) == RESULT_OK )
									{
									obsoleteSpecificationItem->replacingSpecificationItem = replacingSpecificationItem;

									if( obsoleteSpecificationItem->hasCurrentCreationSentenceNr() )
										{
										if( deleteItem( obsoleteSpecificationItem ) != RESULT_OK )
											return addError( functionNameString, NULL, "I failed to delete a specification item" );
										}
									else
										{
										if( replaceItem( obsoleteSpecificationItem ) == RESULT_OK )
											{
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
										else
											return addError( functionNameString, NULL, "I failed to replace a specification item" );
										}

									if( generalizationWordItem->updateSpecificationsInJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem ) == RESULT_OK )
										{
										if( replacingSpecificationItem != NULL )
											{
											if( obsoleteSpecificationItem->isDeletedItem() &&
											generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() )
												{
												// Remove justifications from an obsolete specification item
												if( removeJustificationsFromConfirmedAssumptionOrConclusion( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
													return addError( functionNameString, NULL, "I failed to check for confirmed assumption or conclusion" );
												}

											if( replacingSpecificationItem->hasAnsweredQuestionInJustification() )
												{
												if( replaceOrDeleteSpecification( replacingSpecificationItem, NULL ) == RESULT_OK )
													{
													if( myWordItem()->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
														return addError( functionNameString, NULL, "I failed to replace obsolete justification items" );
													}
												else
													return addError( functionNameString, NULL, "I failed to replace or delete a replacing specification with an answered question in its justification" );
												}
											}
										}
									else
										return addError( functionNameString, NULL, "I failed to update the specifications in the justification of involved words" );
									}
								else
									return addError( functionNameString, NULL, "I failed to update the replacing specification item of the archive specification items" );
								}
							else
								return startError( functionNameString, NULL, "The given replacing specification item is a hidden specification, while the given obsolete specification item isn't hidden" );
							}
						else
							return startError( functionNameString, NULL, "The given replacing specification item is an assignment item and I am a specification list, or the given replacing specification item is a specification item and I am an assignment list" );
						}
					else
						return startError( functionNameString, NULL, "The given obsolete specification item is an assignment item and I am a specification list, or the given obsolete specification item is a specification item and I am an assignment list" );
					}
				else
					return startError( functionNameString, NULL, "The given obsolete specification item and the given replacing specification item are the same specification item" );
				}
			else
				return startError( functionNameString, NULL, "The given obsolete specification item is already replaced or deleted" );
			}
		else
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		bool isAssignment = isAssignmentList();
		SpecificationItem *searchItem = firstActiveSpecificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeSpecificationItemInFutureDatabase( isAssignment ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store an active specification item in the database" );
				}

			searchItem = searchItem->nextSpecificationItem();
			}

		searchItem = firstInactiveSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeSpecificationItemInFutureDatabase( isAssignment ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store an inactive specification item in the database" );
				}

			searchItem = searchItem->nextSpecificationItem();
			}

		searchItem = firstArchivedSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeSpecificationItemInFutureDatabase( isAssignment ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store an archived specification item in the database" );
				}

			searchItem = searchItem->nextSpecificationItem();
			}

		searchItem = firstReplacedSpecificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeSpecificationItemInFutureDatabase( isAssignment ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to modify a replaced specification item in the database" );
				}

			searchItem = searchItem->nextSpecificationItem();
			}

		return RESULT_OK;
		}
*/
	ResultType updateJustificationInSpecifications( bool isInactiveAssignment, bool isArchivedAssignment, bool isReplaced, bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool isSameJustification;
		JustificationItem *attachedPredecessorOfOldJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *searchItem = ( isReplaced ? firstReplacedSpecificationItem() : firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateJustificationInSpecifications";

		if( obsoleteJustificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				// Select self-generated specifications
				if( ( firstJustificationItem = searchItem->firstJustificationItem() ) != NULL )
					{
					// Check first justification
					if( firstJustificationItem == obsoleteJustificationItem )
						{
						// Allowed to change the current justification, without creating a new one
						if( searchItem->hasCurrentCreationSentenceNr() )
							{
							if( searchItem->changeFirstJustification( replacingJustificationItem ) == RESULT_OK )
								searchItem = searchItem->nextSpecificationItem();
							else
								return addError( functionNameString, NULL, "I failed to change the first justification item of a specification item" );
							}
						else
							{
							createdSpecificationItem = NULL;

							if( replacingJustificationItem != NULL )
								{
								if( ( specificationResult = createSpecificationItem( searchItem->isInactiveAssignment(), isArchivedAssignment, searchItem->isAnsweredQuestion(), searchItem->isConcludedAssumption(), searchItem->isConditional(), searchItem->isCorrectedAssumption(), searchItem->isEveryGeneralization(), ( isExclusiveGeneralization || searchItem->isExclusiveSpecification() ), searchItem->isGeneralizationAssignment(), searchItem->isNegative(), searchItem->isPartOf(), searchItem->isPossessive(), searchItem->isSpecificationGeneralization(), searchItem->isUniqueUserRelation(), searchItem->isValueSpecification(), searchItem->assignmentLevel(), searchItem->assumptionLevel(), searchItem->languageNr(), searchItem->prepositionParameter(), searchItem->questionParameter(), searchItem->generalizationWordTypeNr(), searchItem->specificationWordTypeNr(), searchItem->relationWordTypeNr(), searchItem->generalizationCollectionNr(), searchItem->specificationCollectionNr(), searchItem->generalizationContextNr(), searchItem->specificationContextNr(), ( isExclusiveGeneralization ? NO_CONTEXT_NR : searchItem->relationContextNr() ), searchItem->originalSentenceNr(), searchItem->activeSentenceNr(), searchItem->inactiveSentenceNr(), searchItem->archivedSentenceNr(), searchItem->nContextRelations(), replacingJustificationItem, searchItem->specificationWordItem(), searchItem->specificationString(), searchItem->lastWrittenSentenceString ) ).result == RESULT_OK )
									{
									if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
										return startError( functionNameString, NULL, "I couldn't copy the search specification item" );
									}
								else
									return addError( functionNameString, NULL, "I failed to copy the search specification item" );
								}

							if( replaceOrDeleteSpecification( searchItem, createdSpecificationItem ) == RESULT_OK )
								{
								if( isExclusiveGeneralization )
									{
									if( generalizationWordItem->assignSpecification( false, false, true, false, searchItem->isNegative(), searchItem->isPartOf(), searchItem->isPossessive(), searchItem->isSpecificationGeneralization(), searchItem->isUniqueUserRelation(), searchItem->assumptionLevel(), searchItem->prepositionParameter(), searchItem->questionParameter(), searchItem->relationWordTypeNr(), searchItem->generalizationContextNr(), searchItem->specificationContextNr(), searchItem->relationContextNr(), searchItem->originalSentenceNr(), searchItem->activeSentenceNr(), searchItem->inactiveSentenceNr(), searchItem->archivedSentenceNr(), searchItem->nContextRelations(), replacingJustificationItem, searchItem->specificationWordItem(), NULL, searchItem->specificationString() ).result != RESULT_OK )
										return addError( functionNameString, NULL, "I failed to create an assignment" );
									}

								searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
								}
							else
								return addError( functionNameString, NULL, "I failed to replace or delete a specification" );
							}
						}
					else
						{
						// Check attached justification items
						if( ( attachedPredecessorOfOldJustificationItem = firstJustificationItem->attachedPredecessorOfOldJustificationItem( obsoleteJustificationItem ) ) == NULL )
							searchItem = searchItem->nextSpecificationItem();
						else
							{
							if( attachedPredecessorOfOldJustificationItem->hasCurrentCreationSentenceNr() )
								{
								isSameJustification = ( attachedPredecessorOfOldJustificationItem == replacingJustificationItem );

								if( attachedPredecessorOfOldJustificationItem->changeAttachedJustification( isSameJustification ? NULL : replacingJustificationItem ) == RESULT_OK )
									searchItem = searchItem->nextSpecificationItem();
								else
									return addError( functionNameString, NULL, "I failed to change the attached justification item of a justification item" );
								}
							else
								{
								if( ( justificationResult = generalizationWordItem->copyJustificationItem( attachedPredecessorOfOldJustificationItem->primarySpecificationItem(), attachedPredecessorOfOldJustificationItem->secondarySpecificationItem(), replacingJustificationItem, attachedPredecessorOfOldJustificationItem ) ).result == RESULT_OK )
									{
									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL )
										{
										if( generalizationWordItem->replaceJustification( attachedPredecessorOfOldJustificationItem, createdJustificationItem, searchItem ) == RESULT_OK )
											searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
										else
											return addError( functionNameString, NULL, "I failed to replace the attached predecessor of obsolete justification item by a created justification item" );
										}
									else
										searchItem = searchItem->nextSpecificationItem();
									}
								else
									return addError( functionNameString, NULL, "I failed to copy the attached predecessor of obsolete justification item with a different first justification item" );
								}
							}
						}
					}
				else
					searchItem = searchItem->nextSpecificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType createSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *writtenSentenceString )
		{
		SpecificationResultType specificationResult;
		bool isAssignment = isAssignmentList();
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSpecificationItem";

		if( generalizationWordTypeNr >= WORD_TYPE_UNDEFINED &&
		generalizationWordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( specificationWordTypeNr > WORD_TYPE_UNDEFINED &&
			specificationWordTypeNr < NUMBER_OF_WORD_TYPES )
				{
				if( commonVariables()->currentItemNr < MAX_ITEM_NR )
					{
					if( firstJustificationItem == NULL ||
					firstJustificationItem->isActiveItem() )
						{
						if( specificationWordItem == NULL ||
						!specificationWordItem->isNounValue() )
							{
							if( ( specificationResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, generalizationWordItem->isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, commonVariables(), this, generalizationWordItem ) ) != NULL )
								{
								if( addItemToList( ( isArchivedAssignment ? QUERY_ARCHIVED_CHAR : ( isInactiveAssignment ? QUERY_INACTIVE_CHAR : QUERY_ACTIVE_CHAR ) ), specificationResult.createdSpecificationItem ) == RESULT_OK )
									{
									if( isAssignment &&
									assignmentLevel == NO_ASSIGNMENT_LEVEL &&
									originalSentenceNr == commonVariables()->currentSentenceNr )
										commonVariables()->isAssignmentChanged = true;

									if( writtenSentenceString != NULL &&
									strlen( writtenSentenceString ) > 0 )
										strcpy( specificationResult.createdSpecificationItem->lastWrittenSentenceString, writtenSentenceString );
									}
								else
									addError( functionNameString, NULL, "I failed to add an assignment item" );
								}
							else
								startError( functionNameString, NULL, "I failed to create a specification item" );
							}
						else
							startError( functionNameString, NULL, "The given specification word item is a value word" );
						}
					else
						startError( functionNameString, NULL, "The given first justification item isn't active" );
					}
				else
					startError( functionNameString, NULL, "The current item number is undefined" );
				}
			else
				startError( functionNameString, NULL, "The given specification word type number is undefined or out of bounds" );
			}
		else
			startError( functionNameString, NULL, "The given generalization word type number is undefined or out of bounds" );

		specificationResult.result = commonVariables()->result;
		return specificationResult;
		}

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyContextResult, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchItem != NULL )
			{
			if( searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->isMatchingGeneralizationContextNr( isAllowingEmptyContextResult, generalizationContextNr ) &&
			searchItem->isMatchingSpecificationContextNr( isAllowingEmptyContextResult, specificationContextNr ) &&
			searchItem->isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > NO_COLLECTION_NR &&
				!searchItem->isSpecificationGeneralization() &&
				searchItem->specificationCollectionNr() == specificationCollectionNr )
					return searchItem;

				if( foundSpecificationItem == NULL &&
				searchItem->specificationWordItem() == specificationWordItem )
					{
					// Wait for matching specification collection 
					if( specificationCollectionNr > NO_COLLECTION_NR )
						foundSpecificationItem = searchItem;
					else
						return searchItem;
					}
				}

			searchItem = searchItem->nextSelectedSpecificationItem();
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
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			while( searchItem != NULL )
				{
				if( searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->specificationWordItem() == specificationWordItem &&

				( relationWordItem == NULL ||
				relationWordItem->hasContextInWord( searchItem->relationContextNr(), specificationWordItem ) ) )
					{
					if( ( isPossessive &&
					!isSpecificationWordSpanishAmbiguous ) ||

					( !hasRelationWord &&
					isSpecificationWordSpanishAmbiguous ) )
						return searchItem;

					nCurrentRelationContextWords = searchItem->nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchItem;
						}
					}

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		unsigned int nCurrentRelationContextWords;
		unsigned int nMinimumRelationContextWords = MAX_NUMBER;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->specificationWordItem() == specificationWordItem &&
				searchItem->isMatchingRelationContextNr( true, relationContextNr ) )
					{
					nCurrentRelationContextWords = searchItem->nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchItem;
						}
					}

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
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
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->specificationCollectionNr() == specificationCollectionNr &&
				searchItem->specificationWordItem() == specificationWordItem &&
				searchItem->isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
					{
					if( !hasRelationContext ||
					!searchItem->hasRelationContext() ||
					searchItem->relationContextNr() == relationContextNr )
						return searchItem;

					nCurrentRelationContextWords = searchItem->nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchItem;
						}
					}

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
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
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, false );
		WordItem *specificationWordItem;

		while( searchItem != NULL )
			{
			if( !searchItem->isNegative() &&
			!searchItem->isPossessive() &&
			( specificationWordItem = searchItem->specificationWordItem() ) != NULL )
				{
				if( specificationWordItem->isFemaleOrMale() )
					return searchItem;
				}

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstNonCompoundCollectionSpecificationItem( unsigned int compoundSpecificationCollectionNr )
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, false );
		WordItem *specificationWordItem;

		if( compoundSpecificationCollectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->hasSpecificationCollection() &&
				!searchItem->isNegative() &&
				!searchItem->isPossessive() &&
				searchItem->specificationCollectionNr() != compoundSpecificationCollectionNr &&
				( specificationWordItem = searchItem->specificationWordItem() ) != NULL )
					{
					if( searchItem->hasSpecificationNonCompoundCollection() &&
					specificationWordItem->hasCollectionNr( compoundSpecificationCollectionNr ) )
						return searchItem;
					}

				searchItem = searchItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstRecentlyAnsweredQuestionSpecificationItem( bool isArchivedAssignment, unsigned short questionParameter )
		{
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( true, false, isArchivedAssignment, questionParameter );

		if( searchItem != NULL )
			return searchItem->recentlyAnsweredQuestionSpecificationItem( true );

		return NULL;
		}

	SpecificationItem *firstRelationSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( relationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isPossessive() == isPossessive &&
				relationWordItem->hasContextInWord( searchItem->relationContextNr(), searchItem->specificationWordItem() ) )
					return searchItem;

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isSelfGeneratedAssumption() == isSelfGeneratedAssumption &&
				searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->specificationWordItem() == specificationWordItem &&

				( ( relationWordItem == NULL &&

				( isAllowingEmptyContextResult ||
				searchItem->hasRelationContext() ) ) ||

				( relationWordItem != NULL &&
				relationWordItem->hasContextInWord( searchItem->relationContextNr(), specificationWordItem ) ) ) )
					return searchItem;

				searchItem = searchItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->isSelfGenerated() == isSelfGenerated &&
				searchItem->specificationWordItem() == specificationWordItem &&
				searchItem->isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) &&

				( searchItem->specificationCollectionNr() == specificationCollectionNr ||

				// If specification collection doesn't match,
				// check for compound collection to avoid duplicates
				( ( relationContextNr == NO_CONTEXT_NR ||
				!searchItem->hasRelationContext() ) &&

				specificationWordItem->isCompoundCollection( specificationCollectionNr ) ) ) )
					return searchItem;

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isIncludingAnsweredQuestions, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem )
		{
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->specificationWordTypeNr() == specificationWordTypeNr &&
				searchItem->isSelfGenerated() == isSelfGenerated &&
				searchItem->specificationWordItem() == specificationWordItem &&
				searchItem->specificationCollectionNr() == specificationCollectionNr )
					{
					if( searchItem->isExclusiveSpecification() == isExclusiveSpecification )
						return searchItem;

					if( foundSpecificationItem == NULL )
						foundSpecificationItem = searchItem;
					}

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	SpecificationItem *firstSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		WordItem *anyWordItem = myWordItem();
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isSpecificationGeneralization() &&
				searchItem->isNegative() == isNegative &&
				searchItem->isPossessive() == isPossessive &&
				searchItem->generalizationContextNr() == generalizationContextNr &&
				searchItem->specificationContextNr() == specificationContextNr &&
				searchItem->specificationWordItem() == specificationWordItem &&

				( ( relationContextNr == NO_CONTEXT_NR &&
				!searchItem->hasRelationContext() ) ||

				anyWordItem->isContextSubsetInAllWords( relationContextNr, searchItem->relationContextNr() ) ) )
					return searchItem;

				searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->isPossessive() == isPossessive &&
			searchItem->isSpecificationGeneralization() == isSpecificationGeneralization &&
			searchItem->specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstSpecificationItem( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *firstUnhiddenSpecificationItem( bool isArchivedAssignment, unsigned int relationContextNr )
		{
		SpecificationItem *searchItem = firstSpecificationItem( false, isArchivedAssignment );

		if( relationContextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->relationContextNr() == relationContextNr &&
				!searchItem->isHiddenSpanishSpecification() &&
				searchItem->wasHiddenSpanishSpecification() )
					return searchItem;

				searchItem = searchItem->nextSelectedSpecificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *firstUserSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchItem != NULL )
			{
			if( searchItem->isUserSpecification() &&
			searchItem->isNegative() == isNegative &&
			searchItem->isPossessive() == isPossessive &&
			searchItem->isMatchingRelationContextNr( false, relationContextNr ) &&

			( searchItem->specificationWordItem() == specificationWordItem ||

			( specificationCollectionNr > NO_COLLECTION_NR &&
			searchItem->specificationCollectionNr() == specificationCollectionNr ) ) )
				return searchItem;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *sameUserQuestionSpecificationItem( bool isArchivedAssignment, unsigned short questionParameter )
		{
		unsigned int nUserSpecificationWords = commonVariables()->nUserSpecificationWords;
		SpecificationItem *searchItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		while( searchItem != NULL )
			{
			if( searchItem->isUserQuestion() &&
			searchItem->nInvolvedSpecificationWords( true ) == nUserSpecificationWords )
				return searchItem;

			searchItem = searchItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return NULL;
		}

	WordItem *feminineOrMasculineCommonWordItem( bool isFeminineWord )
		{
		SpecificationItem *searchItem = firstActiveSpecificationItem( false, false );
		WordItem *commonWordItem;
		WordItem *specificationWordItem;
		WordItem *generalizationWordItem = myWordItem();

		while( searchItem != NULL )
			{
			if( searchItem->isExclusiveSpecification() &&
			!searchItem->isNegative() &&
			!searchItem->isPossessive() &&
			( specificationWordItem = searchItem->specificationWordItem() ) != NULL )
				{
				if( specificationWordItem->isFemaleOrMale() &&
				( commonWordItem = specificationWordItem->commonWordItem( searchItem->specificationCollectionNr() ) ) != NULL )
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

			searchItem = searchItem->nextSelectedSpecificationItem();
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
