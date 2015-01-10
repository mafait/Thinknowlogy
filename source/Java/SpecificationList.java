/*
 *	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		To store specification items
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class SpecificationList extends List
	{
	// Private assignment methods

	private byte createNewAssignmentLevel( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( searchItem != null &&
		searchItem.assignmentLevel() > CommonVariables.currentAssignmentLevel )
			searchItem = searchItem.nextAssignmentItemWithCurrentLevel();

		while( searchItem != null )
			{
			if( copySpecification( isInactiveAssignment, isArchivedAssignment, searchItem.isAnsweredQuestion(), searchItem.isExclusiveSpecification(), (short)(searchItem.assignmentLevel() + 1 ), searchItem.generalizationCollectionNr(), searchItem.specificationCollectionNr(), searchItem.relationContextNr(), searchItem.firstJustificationItem(), searchItem ).result == Constants.RESULT_OK )
				searchItem = searchItem.nextAssignmentItemWithCurrentLevel();
			else
				return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy an assignment item with incremented assignment level" );
			}

		return Constants.RESULT_OK;
		}

	private byte updateReplacedSpecifications( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		SpecificationItem searchItem = firstReplacedSpecificationItem();

		if( obsoleteSpecificationItem != null )
			{
			if( replacingSpecificationItem == null ||
			!replacingSpecificationItem.isReplacedItem() )
				{
				while( searchItem != null )
					{
					if( searchItem.replacingSpecificationItem == obsoleteSpecificationItem )
						searchItem.replacingSpecificationItem = replacingSpecificationItem;

					searchItem = searchItem.nextSpecificationItem();
					}
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given replaced specification item is a replaced item" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete specification item is undefined" );

		return Constants.RESULT_OK;
		}


	// Private specification methods

	private byte changeOlderSpecification( boolean isExclusiveSpecification, int generalizationCollectionNr, int specificationCollectionNr, SpecificationItem olderSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		SpecificationItem createdSpecificationItem = null;

		if( olderSpecificationItem != null )
			{
			if( olderSpecificationItem.isOlderItem() )
				{
				if( ( isExclusiveSpecification ||
				!olderSpecificationItem.isExclusiveSpecification() ) ||

				( generalizationCollectionNr > Constants.NO_COLLECTION_NR ||
				!olderSpecificationItem.hasGeneralizationCollection() ) ||

				( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				!olderSpecificationItem.hasSpecificationCollection() ) )
					{
					if( ( specificationResult = copySpecification( olderSpecificationItem.isInactiveAssignment(), olderSpecificationItem.isArchivedAssignment(), olderSpecificationItem.isAnsweredQuestion(), ( isExclusiveSpecification || olderSpecificationItem.isExclusiveSpecification() ), olderSpecificationItem.assignmentLevel(), ( generalizationCollectionNr > Constants.NO_COLLECTION_NR ? generalizationCollectionNr : olderSpecificationItem.generalizationCollectionNr() ), ( specificationCollectionNr > Constants.NO_COLLECTION_NR ? specificationCollectionNr : olderSpecificationItem.specificationCollectionNr() ), olderSpecificationItem.relationContextNr(), olderSpecificationItem.firstJustificationItem(), olderSpecificationItem ) ).result == Constants.RESULT_OK )
						{
						if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
							{
							if( replaceOrDeleteSpecification( olderSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace or delete the given specification" );
							}
						else
							startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't create a specification" );
						}
					else
						return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy the older specification with different exclusive indicator and/or different collection numbers" );
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find any changing parameter" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given older specification item isn't old" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given older specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private SpecificationItem firstActiveSpecificationItem()
		{
		return (SpecificationItem)firstActiveItem();
		}

	private SpecificationItem firstInactiveSpecificationItem()
		{
		return (SpecificationItem)firstInactiveItem();
		}

	private SpecificationItem firstArchivedSpecificationItem()
		{
		return (SpecificationItem)firstArchivedItem();
		}

	private SpecificationItem firstReplacedSpecificationItem()
		{
		return (SpecificationItem)firstReplacedItem();
		}

	private SpecificationItem firstSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		return ( isArchivedAssignment ? firstArchivedSpecificationItem() : ( isInactiveAssignment ? firstInactiveSpecificationItem() : firstActiveSpecificationItem() ) );
		}

	private SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion ) : firstActiveSpecificationItem( isIncludingAnsweredQuestions, isQuestion ) );
		}

	private SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter ) : firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

	private SpecificationItem nextSpecificationListItem()
		{
		return (SpecificationItem)nextListItem();
		}


	// Constructor / deconstructor

	protected SpecificationList( char _listChar, WordItem myWordItem )
		{
		initializeListVariables( _listChar, myWordItem );
		}


	// Protected assignment methods

	protected int nActiveAssignments()
		{
		int nItems = 0;
		SpecificationItem searchItem = firstAssignmentItem( false, false, false, false );

		while( searchItem != null )
			{
			nItems++;
			searchItem = searchItem.nextNonQuestionAssignmentItem();
			}

		return nItems;
		}

	protected byte createNewAssignmentLevel()
		{
		if( CommonVariables.currentAssignmentLevel < Constants.MAX_LEVEL )
			{
			if( createNewAssignmentLevel( false, false ) == Constants.RESULT_OK )
				{
				if( createNewAssignmentLevel( true, false ) == Constants.RESULT_OK )
					{
					if( createNewAssignmentLevel( false, true ) != Constants.RESULT_OK )
						return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to create an archive assignment level" );
					}
				else
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to create an inactive assignment level" );
				}
			else
				return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to create an active assignment level" );
			}
		else
			return startSystemError( 1, null, myWordItem().anyWordTypeString(), "Assignment level overflow" );

		return Constants.RESULT_OK;
		}

	protected byte deleteAssignmentLevelInList()
		{
		SpecificationItem searchItem = firstActiveSpecificationItem();

		if( CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL )
			{
			while( searchItem != null &&
			searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
				{
				if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
					{
					if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
						searchItem = nextSpecificationListItem();
					else
						return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete an active item" );
					}
				else
					searchItem = searchItem.nextSpecificationItem();
				}

			searchItem = firstInactiveSpecificationItem();

			while( searchItem != null &&
			searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
				{
				if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
					{
					if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
						searchItem = nextSpecificationListItem();
					else
						return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete an inactive item" );
					}
				else
					searchItem = searchItem.nextSpecificationItem();
				}

			searchItem = firstArchivedSpecificationItem();

			while( searchItem != null &&
			searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
				{
				if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
					{
					if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
						searchItem = nextSpecificationListItem();
					else
						return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete an archived item" );
					}
				else
					searchItem = searchItem.nextSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The current assignment level is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType findAssignmentItemByRelationWord( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		SpecificationItem searchItem = firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( relationWordItem != null )
			{
			while( searchItem != null &&
			specificationResult.foundSpecificationItem == null )
				{
				if( searchItem.isPossessive() == isPossessive &&
				relationWordItem.hasContextInWord( searchItem.relationContextNr(), searchItem.specificationWordItem() ) )
					specificationResult.foundSpecificationItem = searchItem;
				else
					searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
				}
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given relation context word is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem firstNumeralAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		SpecificationItem currentAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( currentAssignmentItem != null &&
		// This is the first assignment. Now get the first valid assignment.
		( currentAssignmentItem = currentAssignmentItem.getAssignmentItem( false, true, isQuestion ) ) != null )
			{
			do	{
				if( currentAssignmentItem.isSpecificationNumeral() )
					return currentAssignmentItem;
				}
			while( ( currentAssignmentItem = currentAssignmentItem.getAssignmentItem( false, false, isQuestion ) ) != null );
			}

		return null;
		}

	protected SpecificationItem firstStringAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		SpecificationItem currentAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( currentAssignmentItem != null &&
		// This is the first assignment. Now get the first valid assignment.
		( currentAssignmentItem = currentAssignmentItem.getAssignmentItem( false, true, isQuestion ) ) != null )
			{
			do	{
				if( currentAssignmentItem.specificationString() != null )
					return currentAssignmentItem;
				}
			while( ( currentAssignmentItem = currentAssignmentItem.getAssignmentItem( false, false, isQuestion ) ) != null );
			}

		return null;
		}

	protected SpecificationItem firstActiveAssignmentItem( boolean isDifferentRelationContext, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentItem( false, false, false, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem &&

			( ( !isDifferentRelationContext &&
			searchItem.relationContextNr() == relationContextNr ) ||

			( isDifferentRelationContext &&
			searchItem.hasRelationContext() &&
			searchItem.relationContextNr() != relationContextNr ) ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		// This is the first assignment
		SpecificationItem firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		// Now get the first valid assignment
		return ( firstAssignmentItem == null ? null : firstAssignmentItem.getAssignmentItem( isIncludingAnsweredQuestions, true, isQuestion ) );
		}

	protected SpecificationItem lastAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		SpecificationItem lastAssignmentItem = null;
		SpecificationItem currentAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( currentAssignmentItem != null &&
		// This is the first assignment. Now get the first valid assignment.
		( currentAssignmentItem = currentAssignmentItem.getAssignmentItem( isIncludingAnsweredQuestions, true, isQuestion ) ) != null )
			{
			do	lastAssignmentItem = currentAssignmentItem;
			while( ( currentAssignmentItem = currentAssignmentItem.getAssignmentItem( isIncludingAnsweredQuestions, false, isQuestion ) ) != null );
			}

		return lastAssignmentItem;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		SpecificationItem firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		return ( firstAssignmentItem == null ? null : firstAssignmentItem.getAssignmentItem( isIncludingAnsweredQuestions, true, questionParameter ) );
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstAnsweredQuestionAssignmentItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentItem( true, false, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isAnsweredQuestion() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( true );
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.isSelfGenerated() == isSelfGenerated &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstRecentlyReplacedAssignmentItem( boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstReplacedSpecificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentReplacedSentenceNr() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.questionParameter() == questionParameter &&
			searchItem.relationContextNr() == relationContextNr &&
			searchItem.specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem.nextSpecificationItem();
			}

		return null;
		}


	// Protected specification methods

	protected void initializeVariables( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchItem != null )
			{
			searchItem.clearLastCheckedAssumptionLevelItemNr();
			searchItem.hasAlreadyBeenWrittenAsAnswer = false;
			searchItem.hasAlreadyBeenWrittenAsConflict = false;

			searchItem = searchItem.nextSpecificationItem();
			}
		}

	protected boolean hasActiveQuestionWithCompoundCollection()
		{
		SpecificationItem searchItem = firstActiveSpecificationItem( false, true );

		while( searchItem != null )
			{
			if( searchItem.hasSpecificationCompoundCollection() )
				return true;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return false;
		}

	protected boolean hasAnsweredQuestion( int secondarySpecificationCollectionNr )
		{
		JustificationItem firstJustificationItem;
		SpecificationItem searchItem = firstSpecificationItem( false, false );

		if( secondarySpecificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.isAnsweredQuestion() &&
				searchItem.specificationCollectionNr() == secondarySpecificationCollectionNr &&
				( firstJustificationItem = searchItem.firstJustificationItem() ) != null )
					{
					if( firstJustificationItem.hasPrimaryUserSpecification() )
						return true;
					}

				searchItem = searchItem.nextSpecificationItem();
				}
			}

		return false;
		}

	protected boolean hasFoundReplacedOrDeletedJustification( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchItem != null )
			{
			if( searchItem.hasFoundReplacedOrDeletedJustification() )
				return true;

			searchItem = searchItem.nextSpecificationItem();
			}

		return false;
		}

	protected boolean hasFoundDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem searchItem = firstReplacedSpecificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasFoundDeletedJustification() )
				return true;

			searchItem = searchItem.nextSpecificationItem();
			}

		return false;
		}

	protected boolean hasNonPossessiveAndNonQuestionPartOfSpecification()
		{
		SpecificationItem searchItem = firstActiveSpecificationItem( false, false );

		while( searchItem != null )
			{
			if( searchItem.isPartOf() &&
			!searchItem.isPossessive() )
				return true;

			searchItem = searchItem.nextNonQuestionSpecificationItem();
			}

		return false;
		}

	protected boolean hasPossessiveButNonQuestionSpecification()
		{
		SpecificationItem searchItem = firstActiveSpecificationItem( false, false );

		while( searchItem != null )
			{
			if( searchItem.isPossessive() )
				return true;

			searchItem = searchItem.nextNonQuestionSpecificationItem();
			}

		return false;
		}

	protected boolean isJustificationInUse( boolean isInactiveAssignment, boolean isArchivedAssignment, JustificationItem unusedJustificationItem )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedJustificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.hasFoundJustification( unusedJustificationItem ) )
					return true;

				searchItem = searchItem.nextSpecificationItem();
				}
			}

		return false;
		}

	protected byte changeJustificationOfNegativeAssumptions( boolean isInactiveAssignment, boolean isArchivedAssignment, SpecificationItem secondarySpecificationItem )
		{
		JustificationResultType justificationResult;
		int specificationCollectionNr;
		JustificationItem firstJustificationItem;
		JustificationItem foundOrCreatedJustificationItem;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		if( secondarySpecificationItem != null )
			{
			specificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

			while( searchItem != null )
				{
				if( searchItem.isNegative() &&
				searchItem.isSelfGeneratedAssumption() &&
				searchItem.specificationCollectionNr() == specificationCollectionNr &&
				( firstJustificationItem = searchItem.firstJustificationItem() ) != null )
					{
					if( firstJustificationItem.isOlderItem() )
						{
						if( ( justificationResult = myWordItem().addJustification( false, firstJustificationItem.justificationTypeNr(), firstJustificationItem.orderNr, firstJustificationItem.originalSentenceNr(), firstJustificationItem.primarySpecificationItem(), firstJustificationItem.anotherPrimarySpecificationItem(), secondarySpecificationItem, null, firstJustificationItem.attachedJustificationItem() ) ).result == Constants.RESULT_OK )
							{
							foundOrCreatedJustificationItem = ( justificationResult.createdJustificationItem == null ? justificationResult.foundJustificationItem : justificationResult.createdJustificationItem );

							if( firstJustificationItem != foundOrCreatedJustificationItem )
								{
								if( myWordItem().replaceJustification( false, firstJustificationItem, foundOrCreatedJustificationItem, searchItem ) == Constants.RESULT_OK )
									searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace a justification of a negative assumption" );
								}
							else
								searchItem = searchItem.nextSelectedSpecificationItem();
							}
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add a justification item" );
						}
					else
						searchItem = searchItem.nextSelectedSpecificationItem();
					}
				else
					searchItem = searchItem.nextSelectedSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkJustificationForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, JustificationItem unusedJustificationItem )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedJustificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.hasFoundJustification( unusedJustificationItem ) )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused justification item is still in use" );

				searchItem = searchItem.nextSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationItemForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, SpecificationItem unusedSpecificationItem )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedSpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.replacingSpecificationItem == unusedSpecificationItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "I found a replacing specification item that is still in use" );

				searchItem = searchItem.nextSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, WordItem unusedWordItem )
		{
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.specificationWordItem() == unusedWordItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The specification word item is still in use" );

				searchItem = searchItem.nextSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralizationsOrSpecifications( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationCollection, boolean isQuestion, int collectionNr )
		{
		boolean isCollectGeneralization;
		boolean isCollectSpecification;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
		WordItem specificationWordItem;

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( ( specificationWordItem = searchItem.specificationWordItem() ) != null )
					{
					isCollectGeneralization = ( isGeneralizationCollection &&
											!searchItem.hasGeneralizationCollection() &&
											myWordItem().hasCollectionNr( collectionNr, specificationWordItem ) );

					isCollectSpecification = ( !isGeneralizationCollection &&
											!searchItem.hasSpecificationCollection() &&
											specificationWordItem.hasCollectionNr( collectionNr ) );

					if( isCollectGeneralization ||
					isCollectSpecification )
						{
						if( searchItem.hasCurrentCreationSentenceNr() )
							{
							if( searchItem.collectSpecification( isCollectGeneralization, isExclusiveSpecification, collectionNr ) == Constants.RESULT_OK )
								searchItem = searchItem.nextSelectedSpecificationItem();
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to collect a specification" );
							}
						else
							{
							if( changeOlderSpecification( isExclusiveSpecification, ( isCollectGeneralization ? collectionNr : Constants.NO_COLLECTION_NR ), ( isCollectSpecification ? collectionNr : Constants.NO_COLLECTION_NR ), searchItem ) == Constants.RESULT_OK )
								searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to collect an older specification" );
							}
						}
					else
						searchItem = searchItem.nextSelectedSpecificationItem();
					}
				else
					searchItem = searchItem.nextSelectedSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given collection number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte confirmSpecificationButNoRelation( boolean isInactiveAssignment, boolean isArchivedAssignment, SpecificationItem confirmedSpecificationItem, SpecificationItem confirmationSpecificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem createdJustificationItem;
		JustificationItem primarySpecificationJustificationItem;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		if( confirmedSpecificationItem != null )
			{
			if( confirmedSpecificationItem.isSelfGenerated() )
				{
				if( confirmationSpecificationItem != null )
					{
					if( confirmationSpecificationItem.isUserSpecification() )
						{
						while( searchItem != null )
							{
							if( !searchItem.hasRelationContext() &&
							( primarySpecificationJustificationItem = searchItem.primarySpecificationJustificationItem( confirmedSpecificationItem ) ) != null )
								{
								if( ( justificationResult = myWordItem().copyJustification( false, confirmationSpecificationItem, primarySpecificationJustificationItem.attachedJustificationItem(), primarySpecificationJustificationItem ) ).result == Constants.RESULT_OK )
									{
									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
										{
										if( myWordItem().replaceJustification( true, primarySpecificationJustificationItem, createdJustificationItem, confirmationSpecificationItem ) == Constants.RESULT_OK )
											searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
										else
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to archive the attached justification item before the archived justification item in my word" );
										}
									else
										return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't create a justification item" );
									}
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy the primary specification Justification item with a different primary specification item" );
								}
							else
								searchItem = searchItem.nextSelectedSpecificationItem();
							}
						}
					else
						return startError( 1, null, myWordItem().anyWordTypeString(), "The given confirmation specification item isn't a user specification" );
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given confirmation specification item is undefined" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given confirmed specification but not its relation(s) specification item isn't self-generated" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given confirmed specification but not its relation(s) specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptions( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isOnlyRecalculateSpecificationsWithoutSpecificationCollection )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchItem != null )
			{
			if( searchItem.isSelfGeneratedAssumption() &&
			!searchItem.hasFoundJustificationWithReplacedPrimarySpecification() &&

			( !isOnlyRecalculateSpecificationsWithoutSpecificationCollection ||
			!searchItem.hasSpecificationCollection() ) )
				{
				if( searchItem.recalculateAssumption() != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to recalculate an assumption" );
				}

			searchItem = searchItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOlderSpecificationByConcludedAssumption( int oldRelationContextNr, SpecificationItem replacingSpecificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem firstJustificationItem;
		SpecificationItem searchItem = firstActiveSpecificationItem();

		if( replacingSpecificationItem != null )
			{
			if( replacingSpecificationItem.isSelfGeneratedAssumption() )
				{
				while( searchItem != null )
					{
					if( searchItem.isOlderItem() &&
					searchItem.relationContextNr() == oldRelationContextNr &&
					( firstJustificationItem = searchItem.firstJustificationItem() ) != null )
						{
						if( replacingSpecificationItem.markAsConcludedAssumption( false ) == Constants.RESULT_OK )
							{
							// Write adjusted specification
							if( myWordItem().writeUpdatedSpecification( true, false, false, replacingSpecificationItem ) == Constants.RESULT_OK )
								{
								if( replaceOrDeleteSpecification( searchItem, replacingSpecificationItem ) == Constants.RESULT_OK )
									{
									if( !firstJustificationItem.hasCurrentActiveSentenceNr() )
										{
										if( ( justificationResult = myWordItem().copyJustification( true, firstJustificationItem.primarySpecificationItem(), firstJustificationItem.attachedJustificationItem(), firstJustificationItem ) ).result == Constants.RESULT_OK )
											{
											if( justificationResult.createdJustificationItem != null )
												{
												if( myWordItem().replaceJustification( false, firstJustificationItem, justificationResult.createdJustificationItem, searchItem ) == Constants.RESULT_OK )
													firstJustificationItem = justificationResult.createdJustificationItem;
												else
													return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the search justification item" );
												}
											else
												return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't create a justification item" );
											}
										else
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to force a copy of a justification item" );
										}

									if( myWordItem().attachJustification( firstJustificationItem, replacingSpecificationItem ) == Constants.RESULT_OK )
										searchItem = nextSpecificationListItem();
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to attach a justification item" );
									}
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace or delete a specification" );
								}
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an adjusted specification" );
							}
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark the replacing specification as concluded assumption" );
						}
					else
						searchItem = searchItem.nextSpecificationItem();
					}
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item is not a self-generated assumption" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		boolean isSelfGeneratedQuestion;
		boolean isUserSpecification;
		boolean isAssignment = isAssignmentList();
		JustificationItem attachedJustificationItem;
		JustificationItem currentJustificationItem;
		JustificationItem obsoleteJustificationItem;

		if( obsoleteSpecificationItem != null )
			{
			if( !obsoleteSpecificationItem.isReplacedOrDeletedItem() )
				{
				if( obsoleteSpecificationItem != replacingSpecificationItem )
					{
					if( obsoleteSpecificationItem.isAssignment() == isAssignment )
						{
						if( replacingSpecificationItem == null ||
						replacingSpecificationItem.isAssignment() == isAssignment )
							{
							obsoleteSpecificationItem.replacingSpecificationItem = replacingSpecificationItem;

							if( updateReplacedSpecifications( obsoleteSpecificationItem, replacingSpecificationItem ) == Constants.RESULT_OK )
								{
								if( obsoleteSpecificationItem.hasCurrentCreationSentenceNr() )
									{
									if( deleteItem( false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete a specification item" );
									}
								else
									{
									if( replaceItem( obsoleteSpecificationItem ) != Constants.RESULT_OK )
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace a specification item" );
									}

								if( myWordItem().updateSpecificationsInJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem ) == Constants.RESULT_OK )
									{
									// Check for confirmed assumption/conclusion
									if( replacingSpecificationItem != null )
										{
										isSelfGeneratedQuestion = replacingSpecificationItem.isSelfGeneratedQuestion();
										isUserSpecification = replacingSpecificationItem.isUserSpecification();

										if( ( isSelfGeneratedQuestion ||
										isUserSpecification ) &&

										( currentJustificationItem = obsoleteSpecificationItem.firstJustificationItem() ) != null )
											{
											do	{
												// Remove (replace) justification (from confirmed assumption/conclusion)
												attachedJustificationItem = currentJustificationItem.attachedJustificationItem();

												if( !myWordItem().isJustificationInUse( currentJustificationItem ) &&

												( currentJustificationItem.hasPrimarySpecification() ||
												currentJustificationItem.hasSecondaryAssignment() ||
												currentJustificationItem.hasSecondaryUserSpecification() ||
												currentJustificationItem.isBackFiredPossessiveConditionalSpecificationAssumption() ) )
													{
													if( isUserSpecification )
														{
														if( myWordItem().replaceJustification( true, currentJustificationItem, null, obsoleteSpecificationItem ) != Constants.RESULT_OK )
															return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to remove a justification from a confirmed specification" );
														}
													else
														{
														if( currentJustificationItem.isActiveItem() )
															{
															if( myWordItem().replaceOrDeleteJustification( currentJustificationItem ) != Constants.RESULT_OK )
																return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete the current justification item" );
															}
														}
													}
												}
											while( ( currentJustificationItem = attachedJustificationItem ) != null );

											if( isSelfGeneratedQuestion &&
											( obsoleteJustificationItem = myWordItem().generalizationAssumptionJustificationItem( replacingSpecificationItem ) ) != null )
												{
												if( myWordItem().replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
													return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete an obsolete justification item" );
												}
											}
										}
									}
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to update the specifications in the justification of involved words" );
								}
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to update the replacing specification item of the archive specification items" );
							}
						else
							return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item is an assignment item and I am a specification list, or the given replacing specification item is a specification item and I am an assignment list" );
						}
					else
						return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete specification item is an assignment item and I am a specification list, or the given obsolete specification item is a specification item and I am an assignment list" );
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete specification item and the given replacing specification item are the same specification item" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete specification item is already replaced or deleted" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete specification item is undefined" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDataBase()
		{
		// Not fully implemented yet
		boolean isAssignment = isAssignmentList();
		SpecificationItem searchItem = firstActiveSpecificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeSpecificationItemInFutureDataBase( isAssignment ) != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store an active specification item in the database" );
				}

			searchItem = searchItem.nextSpecificationItem();
			}

		searchItem = firstInactiveSpecificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeSpecificationItemInFutureDataBase( isAssignment ) != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store an inactive specification item in the database" );
				}

			searchItem = searchItem.nextSpecificationItem();
			}

		searchItem = firstArchivedSpecificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeSpecificationItemInFutureDataBase( isAssignment ) != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store an archived specification item in the database" );
				}

			searchItem = searchItem.nextSpecificationItem();
			}

		searchItem = firstReplacedSpecificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeSpecificationItemInFutureDataBase( isAssignment ) != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced specification item in the database" );
				}

			searchItem = searchItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected byte updateJustificationInSpecifications( boolean hasConfirmedSpecification, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isExclusiveGeneralization, JustificationItem oldJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isSameJustification;
		JustificationItem attachedPredecessorOfOldJustificationItem;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( oldJustificationItem != null )
			{
			while( searchItem != null )
				{
				if( ( firstJustificationItem = searchItem.firstJustificationItem() ) != null )
					{
					if( firstJustificationItem == oldJustificationItem )
						{
						if( searchItem.hasCurrentCreationSentenceNr() )	// Allowed to change without replacing this one and creating a new one
							{
							if( searchItem.changeFirstJustification( replacingJustificationItem ) == Constants.RESULT_OK )
								searchItem = searchItem.nextSpecificationItem();
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the first justification item of a specification item" );
							}
						else
							{
							createdSpecificationItem = null;

							if( replacingJustificationItem != null )
								{
								if( ( specificationResult = copySpecification( searchItem.isInactiveAssignment(), isArchivedAssignment, searchItem.isAnsweredQuestion(), ( isExclusiveSpecification || searchItem.isExclusiveSpecification() ), searchItem.assignmentLevel(), searchItem.generalizationCollectionNr(), searchItem.specificationCollectionNr(), ( isExclusiveGeneralization ? Constants.NO_CONTEXT_NR : searchItem.relationContextNr() ), replacingJustificationItem, searchItem ) ).result == Constants.RESULT_OK )
									{
									if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
										return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't create a specification" );
									}
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy the search specification item" );
								}

							if( replaceOrDeleteSpecification( searchItem, createdSpecificationItem ) == Constants.RESULT_OK )
								{
								if( isExclusiveGeneralization )
									{
									if( myWordItem().assignSpecification( false, false, true, false, searchItem.isNegative(), searchItem.isPartOf(), searchItem.isPossessive(), searchItem.isSpecificationGeneralization(), searchItem.isUniqueRelation(), searchItem.assumptionLevel(), searchItem.prepositionParameter(), searchItem.questionParameter(), searchItem.relationWordTypeNr(), searchItem.generalizationContextNr(), searchItem.specificationContextNr(), searchItem.relationContextNr(), searchItem.originalSentenceNr(), searchItem.activeSentenceNr(), searchItem.inactiveSentenceNr(), searchItem.archivedSentenceNr(), searchItem.nContextRelations(), replacingJustificationItem, searchItem.specificationWordItem(), null, searchItem.specificationString() ).result != Constants.RESULT_OK )
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to create an assignment" );
									}
								else
									{
									if( hasConfirmedSpecification &&
									replacingJustificationItem != null &&
									createdSpecificationItem != null &&
									createdSpecificationItem.isSelfGeneratedAssumption() )
										{
										if( myWordItem().recalculateAssumptions( replacingJustificationItem.isSpecificationSubstitutionAssumption() ) != Constants.RESULT_OK )
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to recalculate my assumptions" );
										}
									}

								searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
								}
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace or delete a specification" );
							}
						}
					else	// Check attached justification items
						{
						if( ( attachedPredecessorOfOldJustificationItem = firstJustificationItem.attachedPredecessorOfOldJustificationItem( oldJustificationItem ) ) == null )
							searchItem = searchItem.nextSpecificationItem();
						else
							{
							if( attachedPredecessorOfOldJustificationItem.hasCurrentCreationSentenceNr() )
								{
								isSameJustification = ( attachedPredecessorOfOldJustificationItem == replacingJustificationItem );

								if( attachedPredecessorOfOldJustificationItem.changeAttachedJustification( isSameJustification ? null : replacingJustificationItem ) == Constants.RESULT_OK )
									{
									if( isSameJustification )
										{
										if( myWordItem().replaceJustification( hasConfirmedSpecification, oldJustificationItem, replacingJustificationItem, searchItem ) != Constants.RESULT_OK )
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the old justification item" );
										}

									searchItem = searchItem.nextSpecificationItem();
									}
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the attached justification item of a justification item" );
								}
							else
								{
								if( replacingJustificationItem != null &&
								myWordItem().isJustificationInUse( replacingJustificationItem ) )
									{
									if( myWordItem().replaceJustification( hasConfirmedSpecification, attachedPredecessorOfOldJustificationItem, replacingJustificationItem, searchItem ) == Constants.RESULT_OK )
										searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to archive the attached justification item before the archived justification item in my word" );
									}
								else
									{
									if( ( justificationResult = myWordItem().copyJustification( false, attachedPredecessorOfOldJustificationItem.primarySpecificationItem(), replacingJustificationItem, attachedPredecessorOfOldJustificationItem ) ).result == Constants.RESULT_OK )
										{
										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
											{
											if( myWordItem().replaceJustification( hasConfirmedSpecification, attachedPredecessorOfOldJustificationItem, createdJustificationItem, searchItem ) == Constants.RESULT_OK )
												searchItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to archive the attached justification item before the archived justification item in my word" );
											}
										else
											searchItem = searchItem.nextSpecificationItem();
										}
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy the attached predecessor of old justification item with a different first justification item" );
									}
								}
							}
						}
					}
				else
					searchItem = searchItem.nextSpecificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given old justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType copySpecification( boolean isNewInactiveAssignment, boolean isNewArchivedAssignment, boolean isNewAnsweredQuestion, boolean isNewExclusiveSpecification, short newAssignmentLevel, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, int newRelationContextNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( originalSpecificationItem != null )
			return createSpecification( isNewInactiveAssignment, isNewArchivedAssignment, isNewAnsweredQuestion, originalSpecificationItem.isConcludedAssumption(), originalSpecificationItem.isConditional(), originalSpecificationItem.isCorrectedAssumption(), originalSpecificationItem.isEveryGeneralization(), isNewExclusiveSpecification, originalSpecificationItem.isGeneralizationAssignment(), originalSpecificationItem.isNegative(), originalSpecificationItem.isPartOf(), originalSpecificationItem.isPossessive(), originalSpecificationItem.isSpecificationGeneralization(), originalSpecificationItem.isUniqueRelation(), originalSpecificationItem.isValueSpecification(), newAssignmentLevel, originalSpecificationItem.assumptionLevel(), originalSpecificationItem.grammarLanguageNr(), originalSpecificationItem.prepositionParameter(), originalSpecificationItem.questionParameter(), originalSpecificationItem.generalizationWordTypeNr(), originalSpecificationItem.specificationWordTypeNr(), originalSpecificationItem.relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem.generalizationContextNr(), originalSpecificationItem.specificationContextNr(), newRelationContextNr, originalSpecificationItem.originalSentenceNr(), originalSpecificationItem.activeSentenceNr(), originalSpecificationItem.inactiveSentenceNr(), originalSpecificationItem.archivedSentenceNr(), originalSpecificationItem.nContextRelations(), newFirstJustificationItem, originalSpecificationItem.specificationWordItem(), originalSpecificationItem.specificationString(), originalSpecificationItem.lastWrittenSentenceStringBuffer );

		specificationResult.result = startError( 1, null, myWordItem().anyWordTypeString(), "The given original specification item is undefined" );
		return specificationResult;
		}

	protected SpecificationResultType createSpecification( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short grammarLanguageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer writtenSentenceStringBuffer )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isAssignment = isAssignmentList();

		if( generalizationWordTypeNr >= Constants.WORD_TYPE_UNDEFINED &&
		generalizationWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( specificationWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
			specificationWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
				{
				if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
					{
					if( firstJustificationItem == null ||
					firstJustificationItem.isActiveItem() )
						{
						if( specificationWordItem == null ||
						!specificationWordItem.isNounValue() )
							{
							if( ( specificationResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, myWordItem().isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assignmentLevel, assumptionLevel, grammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : Constants.NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : Constants.NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : Constants.NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, this, myWordItem() ) ) != null )
								{
								if( isArchivedAssignment )
									{
									if( addItemToList( Constants.QUERY_ARCHIVED_CHAR, specificationResult.createdSpecificationItem ) != Constants.RESULT_OK )
										addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an archive assignment item" );
									}
								else
									{
									if( isInactiveAssignment )
										{
										if( addItemToList( Constants.QUERY_INACTIVE_CHAR, specificationResult.createdSpecificationItem ) != Constants.RESULT_OK )
											addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an inactive assignment item" );
										}
									else
										{
										if( addItemToList( Constants.QUERY_ACTIVE_CHAR, specificationResult.createdSpecificationItem ) != Constants.RESULT_OK )
											addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active specification item" );
										}
									}

								if( CommonVariables.result == Constants.RESULT_OK )
									{
									if( isAssignment &&
									assignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
									originalSentenceNr == CommonVariables.currentSentenceNr )
										CommonVariables.isAssignmentChanged = true;

									if( writtenSentenceStringBuffer != null )
										specificationResult.createdSpecificationItem.lastWrittenSentenceStringBuffer = new StringBuffer( writtenSentenceStringBuffer );
									}
								}
							else
								startError( 1, null, myWordItem().anyWordTypeString(), "I failed to create a specification item" );
							}
						else
							startError( 1, null, myWordItem().anyWordTypeString(), "The given specification word item is a value word" );
						}
					else
						startError( 1, null, myWordItem().anyWordTypeString(), "The given first justification item isn't active" );
					}
				else
					startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
				}
			else
				startError( 1, null, myWordItem().anyWordTypeString(), "The given specification word type number is undefined or out of bounds" );
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given generalization word type number is undefined or out of bounds" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isSpecificationCollectedWithItself;
		boolean hasRelationContext = ( relationWordItem != null );
		int nCurrentRelationContextWords;
		int nMinimumRelationContextWords = Constants.MAX_NUMBER;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

			while( searchItem != null )
				{
				if( searchItem.isNegative() == isNegative &&
				searchItem.isPossessive() == isPossessive &&
				searchItem.specificationWordItem() == specificationWordItem &&

				( relationWordItem == null ||
				relationWordItem.hasContextInWord( searchItem.relationContextNr(), specificationWordItem ) ) )
					{
					if( !hasRelationContext ||

					( isPossessive &&
					!isSpecificationCollectedWithItself ) )
						return searchItem;

					nCurrentRelationContextWords = searchItem.nRelationContextWords();

					if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

					( nCurrentRelationContextWords < nMinimumRelationContextWords &&
					nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
						{
						nMinimumRelationContextWords = nCurrentRelationContextWords;
						foundSpecificationItem = searchItem;
						}
					}

				searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		int nCurrentRelationContextWords;
		int nMinimumRelationContextWords = Constants.MAX_NUMBER;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingRelationContextNr( true, relationContextNr ) )
				{
				if( searchItem.isExclusiveSpecification() ||

				( relationContextNr > Constants.NO_CONTEXT_NR &&
				searchItem.relationContextNr() == relationContextNr ) )
					return searchItem;

				nCurrentRelationContextWords = searchItem.nRelationContextWords();

				if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

				( nCurrentRelationContextWords < nMinimumRelationContextWords &&
				nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
					{
					nMinimumRelationContextWords = nCurrentRelationContextWords;
					foundSpecificationItem = searchItem;
					}
				}

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		int nCurrentRelationContextWords;
		int nMinimumRelationContextWords = Constants.MAX_NUMBER;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationCollectionNr() == specificationCollectionNr &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingGeneralizationContextNr( isAllowingEmptyContextResult, generalizationContextNr ) &&
			searchItem.isMatchingSpecificationContextNr( isAllowingEmptyContextResult, specificationContextNr ) &&
			searchItem.isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) )
				{
				if( !hasRelationContext ||
				!searchItem.hasRelationContext() ||
				searchItem.relationContextNr() == relationContextNr )
					return searchItem;

				nCurrentRelationContextWords = searchItem.nRelationContextWords();

				if( nCurrentRelationContextWords == nMinimumRelationContextWords + 1 ||

				( nCurrentRelationContextWords < nMinimumRelationContextWords &&
				nCurrentRelationContextWords + 1 != nMinimumRelationContextWords ) )
					{
					nMinimumRelationContextWords = nCurrentRelationContextWords;
					foundSpecificationItem = searchItem;
					}
				}

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstActiveSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isQuestion )
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.getSpecificationItem( isIncludingAnsweredQuestions, true, isQuestion );

		return null;
		}

	protected SpecificationItem firstActiveSpecificationItem( boolean isIncludingAnsweredQuestions, short questionParameter )
		{
		SpecificationItem firstSpecificationItem;

		if( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null )
			return firstSpecificationItem.getSpecificationItem( isIncludingAnsweredQuestions, true, questionParameter );

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( !searchItem.isSpecificationGeneralization() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.generalizationContextNr() == generalizationContextNr &&
			searchItem.specificationContextNr() == specificationContextNr &&
			searchItem.specificationWordItem() == specificationWordItem &&

			( ( relationContextNr == Constants.NO_CONTEXT_NR &&
			!searchItem.hasRelationContext() ) ||

			myWordItem().isContextSubsetInAllWords( relationContextNr, searchItem.relationContextNr() ) ) )
				return searchItem;

			searchItem = searchItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isAllowingEmptyContextResult, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int generalizationContextNr, int specificationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isExclusiveSpecification() == isExclusiveSpecification &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingGeneralizationContextNr( isAllowingEmptyContextResult, generalizationContextNr ) &&
			searchItem.isMatchingSpecificationContextNr( isAllowingEmptyContextResult, specificationContextNr ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstFeminineOrMasculineSpecificationItem()
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, false, false, false );

		while( searchItem != null )
			{
			if( !searchItem.isNegative() &&
			!searchItem.isPossessive() &&
			searchItem.hasFeminineOrMasculineSpecificationWord() )
				return searchItem;

			searchItem = searchItem.nextNonQuestionSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonCompoundCollectionSpecificationItem( int compoundSpecificationCollectionNr )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, false, false, false );
		WordItem specificationWordItem;

		if( compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.hasSpecificationCollection() &&
				!searchItem.isNegative() &&
				!searchItem.isPossessive() &&
				searchItem.specificationCollectionNr() != compoundSpecificationCollectionNr &&
				( specificationWordItem = searchItem.specificationWordItem() ) != null )
					{
					if( !searchItem.hasSpecificationCompoundCollection() &&
					specificationWordItem.hasCollectionNr( compoundSpecificationCollectionNr ) )
						return searchItem;
					}

				searchItem = searchItem.nextNonQuestionSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstNonQuestionAssignmentOrSpecificationItem( boolean isAllowingEmptyContextResult, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.isMatchingGeneralizationContextNr( isAllowingEmptyContextResult, generalizationContextNr ) &&
			searchItem.isMatchingSpecificationContextNr( isAllowingEmptyContextResult, specificationContextNr ) &&
			searchItem.isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) &&

			( searchItem.specificationWordItem() == specificationWordItem ||

			( !searchItem.isSpecificationGeneralization() &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			searchItem.specificationCollectionNr() == specificationCollectionNr ) ) )
				return searchItem;

			searchItem = searchItem.nextNonQuestionSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstRecentlyAnsweredQuestionSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, questionParameter );

		if( searchItem != null )
			return searchItem.recentlyAnsweredQuestionSpecificationItem( true );

		return null;
		}

	protected SpecificationItem firstRelationSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.hasRelationContext() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption &&
			searchItem.specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyContextResult, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( !searchItem.isHiddenSpecification() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.isSelfGenerated() == isSelfGenerated &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isMatchingRelationContextNr( isAllowingEmptyContextResult, relationContextNr ) &&

			( searchItem.specificationCollectionNr() == specificationCollectionNr ||

			// If specification collection doesn't match,
			// check for compound collection to avoid duplicates
			( ( relationContextNr == Constants.NO_CONTEXT_NR ||
			!searchItem.hasRelationContext() ) &&

			specificationWordItem != null &&
			specificationWordItem.isCompoundCollection( specificationCollectionNr ) ) ) )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter();
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( !searchItem.isHiddenSpecification() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.specificationWordTypeNr() == specificationWordTypeNr &&
			searchItem.isSelfGenerated() == isSelfGenerated &&
			searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.specificationCollectionNr() == specificationCollectionNr )
				{
				if( searchItem.isExclusiveSpecification() == isExclusiveSpecification )
					return searchItem;

				if( foundSpecificationItem == null )
					foundSpecificationItem = searchItem;
				}

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstUserSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchItem != null )
			{
			if( searchItem.isUserSpecification() &&
			searchItem.isNegative() == isNegative &&
			searchItem.isPossessive() == isPossessive &&
			searchItem.isMatchingRelationContextNr( true, relationContextNr ) &&

			( searchItem.specificationWordItem() == specificationWordItem ||

			( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			searchItem.specificationCollectionNr() == specificationCollectionNr ) ) )
				return searchItem;

			searchItem = searchItem.nextNonQuestionSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem justificationSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, JustificationItem findJustificationItem )
		{
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchItem != null )
			{
			if( searchItem.hasFoundJustification( findJustificationItem ) )
				return searchItem;

			searchItem = searchItem.nextNonQuestionSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem sameQuestionSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isSelfGenerated, short questionParameter )
		{
		int nUserSpecificationWords = CommonVariables.nUserSpecificationWords;
		SpecificationItem searchItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchItem != null )
			{
			if( searchItem.isSelfGenerated() == isSelfGenerated &&
			searchItem.nInvolvedSpecificationWords( true, true ) == nUserSpecificationWords )
				return searchItem;

			searchItem = searchItem.nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions );
			}

		return null;
		}
	};

/*************************************************************************
 *
 *	"Good comes to those who lend money generously
 *	and conduct their business fairly.
 *	Such people will not be overcome by evil.
 *	Those who are righteous will be long remembered" (Psalm 112:5-6)
 *
 *************************************************************************/
