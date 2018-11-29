/*	Class:			SpecificationList
 *	Parent class:	List
 *	Purpose:		To store specification items
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

package org.mafait.thinknowlogy;

class SpecificationList extends List
	{
	// Private constructed variables

	private boolean isMarkedAsSpecificationWord_ = false;


	// Private assignment methods

	private byte createNewAssignmentLevel( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != null &&
		searchSpecificationItem.assignmentLevel() >= currentAssignmentLevel )
			{
			// Assignments of current level
			if( createSpecificationItem( isInactiveAssignment, isArchivedAssignment, searchSpecificationItem.isAnsweredQuestion(), searchSpecificationItem.isCharacteristicFor(), searchSpecificationItem.isConcludedAssumption(), searchSpecificationItem.isConditional(), searchSpecificationItem.isCorrectedAssumption(), searchSpecificationItem.isEveryGeneralization(), searchSpecificationItem.isExclusiveGeneralization(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPartOf(), searchSpecificationItem.isPossessive(), searchSpecificationItem.isSpecific(), searchSpecificationItem.isSpecificationGeneralization(), searchSpecificationItem.isUncountableGeneralizationNoun(), searchSpecificationItem.isUniqueUserRelation(), searchSpecificationItem.isValueSpecification(), (short)(searchSpecificationItem.assignmentLevel() + 1 ), searchSpecificationItem.assumptionLevel(), searchSpecificationItem.languageNr(), searchSpecificationItem.prepositionParameter(), searchSpecificationItem.questionParameter(), searchSpecificationItem.generalizationWordTypeNr(), searchSpecificationItem.specificationWordTypeNr(), searchSpecificationItem.relationWordTypeNr(), searchSpecificationItem.generalizationCollectionNr(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.originalSentenceNr(), searchSpecificationItem.activeSentenceNr(), searchSpecificationItem.inactiveSentenceNr(), searchSpecificationItem.archivedSentenceNr(), searchSpecificationItem.nContextRelations(), searchSpecificationItem.firstJustificationItem(), searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString(), null, null ).result != Constants.RESULT_OK )
				return addError( 1, "I failed to copy an assignment item with incremented assignment level" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte deleteAssignmentLevelInList( SpecificationItem searchSpecificationItem )
		{
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;

		while( searchSpecificationItem != null &&
		searchSpecificationItem.assignmentLevel() >= currentAssignmentLevel )
			{
			// Assignments of current level
			if( deleteItem( searchSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to delete an item" );

			searchSpecificationItem = nextSpecificationListItem();
			}

		return Constants.RESULT_OK;
		}

	private byte updateReplacedSpecifications( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		SpecificationItem searchSpecificationItem = firstReplacedSpecificationItem();

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, "The given replaced specification item is a replaced or deleted item" );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.replacingSpecificationItem == obsoleteSpecificationItem )
				searchSpecificationItem.replacingSpecificationItem = replacingSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}


	// Private specification methods

	private static void clearReplacingInfo( SpecificationItem searchSpecificationItem )
		{
		while( searchSpecificationItem != null )
			{
			searchSpecificationItem.clearReplacingInfo();
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}
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
		return ( isArchivedAssignment ? firstArchivedSpecificationItem() :
				( isInactiveAssignment ? firstInactiveSpecificationItem() : firstActiveSpecificationItem() ) );
		}

	private SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion ) :
									firstActiveSpecificationItem( isIncludingAnsweredQuestions, isQuestion ) );
		}

	private SpecificationItem firstAssignmentOrSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		return ( isAssignmentList() ? firstAssignmentItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, questionParameter ) :
									firstActiveSpecificationItem( isIncludingAnsweredQuestions, questionParameter ) );
		}

	private SpecificationItem nextSpecificationListItem()
		{
		return (SpecificationItem)nextListItem();
		}


	// Constructor

	protected SpecificationList( char _listChar, WordItem myWordItem )
		{
		initializeListVariables( _listChar, "SpecificationList", myWordItem );
		}


	// Protected assignment methods

	protected int nActiveAssignments()
		{
		int nItems = 0;
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != null )
			{
			nItems++;
			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return nItems;
		}

	protected byte createNewAssignmentLevelInList()
		{
		if( GlobalVariables.currentAssignmentLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, "Assignment level overflow" );

		if( createNewAssignmentLevel( false, false ) != Constants.RESULT_OK )
			return addError( 1, "I failed to create an active assignment level" );

		if( createNewAssignmentLevel( true, false ) != Constants.RESULT_OK )
			return addError( 1, "I failed to create an inactive assignment level" );

		if( createNewAssignmentLevel( false, true ) != Constants.RESULT_OK )
			return addError( 1, "I failed to create an archive assignment level" );

		return Constants.RESULT_OK;
		}

	protected byte deleteAssignmentLevelInList()
		{
		SpecificationItem searchSpecificationItem;

		if( GlobalVariables.currentAssignmentLevel <= Constants.NO_ASSIGNMENT_LEVEL )
			return startError( 1, "The current assignment level is undefined" );

		if( deleteAssignmentLevelInList( firstActiveSpecificationItem() ) == Constants.RESULT_OK )
			{
			if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != null )
				deleteAssignmentLevelInList( searchSpecificationItem );

			if( GlobalVariables.result == Constants.RESULT_OK &&
			( searchSpecificationItem = firstArchivedSpecificationItem() ) != null )
				deleteAssignmentLevelInList( searchSpecificationItem );
			}

		return GlobalVariables.result;
		}

	protected byte inactivateOrArchiveCurrentGeneralizationAssignments( boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundActiveAssignmentItem = null;
		SpecificationItem foundInactiveAssignmentItem = null;
		WordItem currentGeneralizationWordItem;
		WordItem foundActiveGeneralizationWordItem;
		WordItem foundInactiveGeneralizationWordItem;

		if( specificationWordItem == null )
			return startError( 1, "The given specification word item is undefined" );

		if( specificationWordItem.isNounValue() )
			return startError( 1, "Inactivating or archiving a noun value specification isn't implemented yet" );

		if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return startSystemError( 1, "I found an undefined generalization word" );

				if( foundActiveAssignmentItem == null &&
				// Skip my word for activate assignments, because this is the one to be assigned
				currentGeneralizationWordItem != myWordItem() )
					foundActiveAssignmentItem = currentGeneralizationWordItem.firstNonQuestionAssignmentItem( true, false, false, isNegative, isPossessive, specificationWordItem );

				// Allow to find an inactive assignment in my word
				if( foundInactiveAssignmentItem == null )
					foundInactiveAssignmentItem = currentGeneralizationWordItem.firstNonQuestionAssignmentItem( false, true, false, isNegative, isPossessive, specificationWordItem );
				}
			while( ( foundActiveAssignmentItem == null ||
			foundInactiveAssignmentItem == null ) &&

			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		// Only archive an inactive assignment if an active assignment is being inactivated
		if( foundActiveAssignmentItem != null )
			{
			if( ( foundActiveGeneralizationWordItem = foundActiveAssignmentItem.generalizationWordItem() ) == null )
				return startError( 1, "I found an active assignment without generalization word item" );

			if( foundActiveGeneralizationWordItem.inactivateActiveAssignment( foundActiveAssignmentItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to inactivate an active assignment" );

			if( foundInactiveAssignmentItem != null )
				{
				if( ( foundInactiveGeneralizationWordItem = foundInactiveAssignmentItem.generalizationWordItem() ) == null )
					return startError( 1, "I found an inactive assignment without generalization word item" );

				if( foundInactiveGeneralizationWordItem.archiveInactiveAssignment( foundInactiveAssignmentItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to archive an inactive assignment" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem firstActiveNumeralAssignmentItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isSpecificationNumeral() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstActiveStringAssignmentItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationString() != null )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstActiveNonQuestionAssignmentItem( int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, false, false, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationWordItem() == specificationWordItem &&

			( relationContextNr == Constants.NO_CONTEXT_NR ||
			searchSpecificationItem.relationContextNr() == relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion )
		{
		// This is the first assignment
		SpecificationItem firstAssignmentItem;

		// Now get the first valid assignment
		return ( ( firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) ) != null ?
				firstAssignmentItem.assignmentItem( isIncludingAnsweredQuestions, true, isQuestion ) : null );
		}

	protected SpecificationItem firstAssignmentItem( boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, short questionParameter )
		{
		SpecificationItem firstAssignmentItem;

		return ( ( firstAssignmentItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) ) != null ?
				firstAssignmentItem.assignmentItem( isIncludingAnsweredQuestions, true, questionParameter ) : null );
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, boolean isQuestion, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isSelfGenerated, short questionParameter, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			searchSpecificationItem.isMatchingRelationContextNr( true, relationContextNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonQuestionAssignmentItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem lastActiveNonQuestionAssignmentItem()
		{
		SpecificationItem lastAssignmentItem = null;
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			lastAssignmentItem = searchSpecificationItem;
			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return lastAssignmentItem;
		}


	// Protected specification methods

	protected void addToAssignmentWordQuickAccessList()
		{
		WordItem thisWordItem = myWordItem();

		// Word order is not important
		thisWordItem.nextAssignmentWordItem = GlobalVariables.firstAssignmentWordItem;
		GlobalVariables.firstAssignmentWordItem = thisWordItem;
		}

	protected void addToPossessiveNounWordQuickAccessList()
		{
		WordItem thisWordItem = myWordItem();

		// Word order is not important
		thisWordItem.nextPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem;
		GlobalVariables.firstPossessiveNounWordItem = thisWordItem;
		}

	protected void addToUserProperNounWordQuickAccessList()
		{
		WordItem thisWordItem = myWordItem();

		// Word order is not important
		thisWordItem.nextUserProperNounWordItem = GlobalVariables.firstUserProperNounWordItem;
		GlobalVariables.firstUserProperNounWordItem = thisWordItem;
		}

	protected void addToSpecificationWordQuickAccessList()
		{
		WordItem thisWordItem = myWordItem();

		// Word order is not important
		thisWordItem.nextSpecificationWordItem = GlobalVariables.firstSpecificationWordItem;
		GlobalVariables.firstSpecificationWordItem = thisWordItem;
		}

	protected void clearReplacingInfo()
		{
		SpecificationItem searchSpecificationItem;

		if( ( searchSpecificationItem = firstActiveSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstInactiveSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstArchivedSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );

		if( ( searchSpecificationItem = firstReplacedSpecificationItem() ) != null )
			clearReplacingInfo( searchSpecificationItem );
		}

	protected void initializeSpecificationVariables( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != null )
			{
			searchSpecificationItem.initializeSpecificationVariables();
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}
		}

	protected boolean hasActiveQuestionWithCompoundCollection()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, true );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasCompoundSpecificationCollection() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hadOnceAnsweredSelfGeneratedQuestion()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isAnsweredSelfGeneratedQuestion() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return false;
		}

	protected boolean hasAnyUserSpecification()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasUserNr() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasRelationContextInSpecifications( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedSpecification, int relationContextNr )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedSpecification ? firstReplacedSpecificationItem() :
																				firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.relationContextNr() == relationContextNr )
					return true;

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return false;
		}

	protected boolean hasMultipleSpecificationWordsWithSameSentenceNr( int creationSentenceNr, int skipThisItemNr, int specificationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != null &&
		searchSpecificationItem.creationSentenceNr() >= creationSentenceNr )
			{
			if( !searchSpecificationItem.hasRelationContext() &&
			!searchSpecificationItem.isNegative() &&
			!searchSpecificationItem.isPossessive() &&
			searchSpecificationItem.itemNr() != skipThisItemNr &&
			searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasNegativeSpecification()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return false;
		}

	protected boolean hasPartOfSpecification()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPartOf() )
				return true;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return false;
		}

	protected boolean hasPossiblyGapInKnowledge( int exclusiveSecondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		int primarySpecificationCollectionNr;
		int searchSpecificationCollectionNr;
		SpecificationItem searchSpecificationItem = firstSpecificationItem( false, false );

		if( primarySpecificationItem != null )
			{
			primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem != primarySpecificationItem &&
				!searchSpecificationItem.isPossessive() &&
				!searchSpecificationItem.isPartOf() )
					{
					searchSpecificationCollectionNr = searchSpecificationItem.specificationCollectionNr();

					// Don't skip creation of current question
					if( ( searchSpecificationCollectionNr == primarySpecificationCollectionNr ||
					searchSpecificationCollectionNr == exclusiveSecondarySpecificationCollectionNr ) &&

					( searchSpecificationItem.isNegative() ||
					searchSpecificationItem.isOlderItem() ) )
						return false;
					}

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return true;
		}

	protected boolean isJustificationInUse( boolean isInactiveAssignment, boolean isArchivedAssignment, JustificationItem unusedJustificationItem )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedJustificationItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.hasJustification( unusedJustificationItem ) )
					return true;

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return false;
		}

	protected int nRemainingSpecificationWordsForWriting( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedSpecification, short assumptionLevel, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, int creationSentenceNr )
		{
		int nRemainingWords = 0;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );
		WordItem specificationWordItem;

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr ) != null &&
			( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null &&

			// Typical for Chinese
			( creationSentenceNr == Constants.NO_SENTENCE_NR ||
			searchSpecificationItem.creationSentenceNr() == creationSentenceNr ) &&

			!specificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr ) )
				nRemainingWords++;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return nRemainingWords;
		}

	protected byte changeJustificationOfNegativeAssumptions( boolean isInactiveAssignment, boolean isArchivedAssignment, SpecificationItem secondarySpecificationItem )
		{
		int specificationCollectionNr;
		JustificationItem firstJustificationItem;
		JustificationItem foundOrCreatedJustificationItem;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem thisWordItem = myWordItem();
		JustificationResultType justificationResult;

		if( secondarySpecificationItem == null )
			return startError( 1, "The given secondary specification item is undefined" );

		specificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() &&
			searchSpecificationItem.isSelfGeneratedAssumption() &&
			searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
			( firstJustificationItem = searchSpecificationItem.firstJustificationItem() ) != null &&
			firstJustificationItem.isOlderItem() )
				{
				if( ( justificationResult = thisWordItem.addJustification( false, false, false, firstJustificationItem.justificationTypeNr(), firstJustificationItem.orderNr, firstJustificationItem.originalSentenceNr(), firstJustificationItem.primarySpecificationItem(), firstJustificationItem.anotherPrimarySpecificationItem(), secondarySpecificationItem, null, firstJustificationItem.attachedJustificationItem() ) ).result != Constants.RESULT_OK )
					return addError( 1, "I failed to add a justification item" );

				foundOrCreatedJustificationItem = ( justificationResult.createdJustificationItem == null ? justificationResult.foundJustificationItem :
																											justificationResult.createdJustificationItem );

				if( firstJustificationItem != foundOrCreatedJustificationItem )
					{
					if( thisWordItem.replaceJustification( firstJustificationItem, foundOrCreatedJustificationItem, searchSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace a justification of a negative assumption" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
					}
				else
					searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedJustificationInReplacedSpecification()
		{
		SpecificationItem searchSpecificationItem = firstReplacedSpecificationItem();

		while( searchSpecificationItem != null &&
		searchSpecificationItem.checkForDeletedJustification() == Constants.RESULT_OK )
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();

		return GlobalVariables.result;
		}

	protected byte checkForReplacedOrDeletedJustification( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		while( searchSpecificationItem != null &&
		searchSpecificationItem.checkForReplacedOrDeletedJustification() == Constants.RESULT_OK )
			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();

		return GlobalVariables.result;
		}

	protected byte checkJustificationForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, JustificationItem unusedJustificationItem )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedJustificationItem == null )
			return startError( 1, "The given unused justification item is undefined" );

		while( searchSpecificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem.hasJustification( unusedJustificationItem ) &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a justification item that is still in use:\n\tSpecificationItem: " + searchSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + unusedJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, SpecificationItem unusedSpecificationItem )
		{
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );

		if( unusedSpecificationItem == null )
			return startError( 1, "The given unused specification item is undefined" );

		while( searchSpecificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchSpecificationItem.replacingSpecificationItem == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replacing specification item that is still in use:\n\tSpecificationItem: " + searchSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\treplacingSpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( boolean isInactiveAssignment, boolean isArchivedAssignment, WordItem unusedWordItem )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );

		if( unusedWordItem == null )
			return startError( 1, "The given unused word item is undefined" );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationWordItem() == unusedWordItem )
				return startError( 1, "The specification word item is still in use" );

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte clearStoredSentenceStringWithUnknownPluralNoun( boolean isInactiveAssignment, boolean isArchivedAssignment, String unknownPluralNounString, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
		StringBuffer storedSentenceStringBuffer;

		if( unknownPluralNounString == null )
			return startError( 1, "The given unknown plural noun string is undefined" );

		if( specificationWordItem == null )
			return startError( 1, "The given specification word item is undefined" );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.specificationWordItem() == specificationWordItem &&
			( storedSentenceStringBuffer = searchSpecificationItem.storedSentenceStringBuffer() ) != null &&
			storedSentenceStringBuffer.indexOf( unknownPluralNounString ) > 0 )
				searchSpecificationItem.clearStoredSentenceStringBuffer();

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralizations( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveGeneralization, int generalizationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
		WordItem thisWordItem = myWordItem();

		if( generalizationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, "The given generalization collection number is undefined" );

		while( searchSpecificationItem != null )
			{
			if( ( isExclusiveGeneralization ||
			searchSpecificationItem.isGeneralizationNoun() ) &&

			!searchSpecificationItem.hasGeneralizationCollection() &&
			thisWordItem.hasCollectionNr( generalizationCollectionNr, searchSpecificationItem.specificationWordItem() ) )
				{
				if( searchSpecificationItem.hasCurrentCreationSentenceNr() )
					{
					if( searchSpecificationItem.collectGeneralization( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
						return addError( 1, "I failed to collect the generalization of a specification" );

					searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
					}
				else
					{
					// Copy and replace
					if( copyAndReplaceSpecificationItem( searchSpecificationItem.isAnsweredQuestion(), isExclusiveGeneralization, searchSpecificationItem.isExclusiveSpecification(), generalizationCollectionNr, searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.firstJustificationItem(), searchSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to copy and replace an older specification" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte collectSpecificationsInList( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isQuestion, int specificationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
		WordItem specificationWordItem;

		if( specificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, "The given specification collection number is undefined" );

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.hasSpecificationCollection() &&
			( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null &&
			specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
				{
				if( searchSpecificationItem.hasCurrentCreationSentenceNr() )
					{
					if( searchSpecificationItem.collectSpecification( isExclusiveSpecification, specificationCollectionNr ) != Constants.RESULT_OK )
						return addError( 1, "I failed to collect a specification" );

					searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
					}
				else
					{
					// Copy and replace
					if( copyAndReplaceSpecificationItem( searchSpecificationItem.isAnsweredQuestion(), false, isExclusiveSpecification, searchSpecificationItem.generalizationCollectionNr(), specificationCollectionNr, searchSpecificationItem.firstJustificationItem(), searchSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to copy and replace an older specification" );

					searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, isQuestion );
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte confirmSpecificationButNotItsRelation( SpecificationItem confirmationSpecificationItem )
		{
		boolean isConfirmedPrimarySpecification;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem confirmedSpecificationItem = firstActiveSpecificationItem();
		SpecificationItem searchSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		WordItem confirmedSpecificationWordItem;
		WordItem thisWordItem = myWordItem();
		JustificationResultType justificationResult;

		if( confirmationSpecificationItem == null )
			return startError( 1, "The given confirmation specification item is undefined" );

		if( !confirmationSpecificationItem.isUserSpecification() )
			return startError( 1, "The given confirmation specification item isn't a user specification" );

		if( ( confirmedSpecificationWordItem = confirmationSpecificationItem.specificationWordItem() ) == null )
			return startError( 1, "The given confirmation specification item has no specification word" );

		while( confirmedSpecificationItem != null )
			{
			if( confirmedSpecificationItem.hasRelationContext() &&
			confirmedSpecificationItem.isSelfGenerated() &&
			confirmedSpecificationItem.specificationWordItem() == confirmedSpecificationWordItem )
				{
				if( thisWordItem.writeConfirmedSpecification( ( confirmedSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NOT_ITS_RELATION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NOT_ITS_RELATION ), confirmedSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to write a confirmed specification" );

				// Skip possessive Spanish replacements. To be implemented later
				if( !confirmedSpecificationItem.isPossessive() )
					{
					searchSpecificationItem = firstActiveSpecificationItem();

					while( searchSpecificationItem != null )
						{
						if( !searchSpecificationItem.hasRelationContext() &&
						!searchSpecificationItem.isAnsweredQuestion() &&
						( obsoleteJustificationItem = searchSpecificationItem.primaryOrSecondarySpecificationJustificationItem( confirmedSpecificationItem ) ) != null )
							{
							isConfirmedPrimarySpecification = ( obsoleteJustificationItem.primarySpecificationItem() == confirmedSpecificationItem );
							selectedPrimarySpecificationItem = ( isConfirmedPrimarySpecification ? confirmationSpecificationItem :
																									obsoleteJustificationItem.primarySpecificationItem() );
							selectedSecondarySpecificationItem = ( isConfirmedPrimarySpecification ? obsoleteJustificationItem.secondarySpecificationItem() :
																									confirmationSpecificationItem );

							if( ( justificationResult = thisWordItem.copyJustification( false, selectedPrimarySpecificationItem, selectedSecondarySpecificationItem, obsoleteJustificationItem.attachedJustificationItem(), obsoleteJustificationItem ) ).result != Constants.RESULT_OK )
								return addError( 1, "I failed to copy the primary specification justification item with a different primary specification item" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
								return startError( 1, "I couldn't create a justification item" );

							if( thisWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, confirmationSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, "I failed to replace the given obsolete justification item by a created justification item" );

							searchSpecificationItem = firstActiveSpecificationItem();
							}
						else
							searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
						}
					}
				}

			confirmedSpecificationItem = confirmedSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte copyAndReplaceSpecificationItem( boolean isNewAnsweredQuestion, boolean isNewExclusiveGeneralization, boolean isNewExclusiveSpecification, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, JustificationItem newFirstJustificationItem, SpecificationItem originalSpecificationItem )
		{
		SpecificationItem createdSpecificationItem = null;
		CreateAndAssignResultType createAndAssignResult;

		if( originalSpecificationItem == null )
			return startError( 1, "The given original specification item is undefined" );

		if( ( createAndAssignResult = createSpecificationItem( originalSpecificationItem.isInactiveAssignment(), originalSpecificationItem.isArchivedAssignment(), isNewAnsweredQuestion, originalSpecificationItem.isCharacteristicFor(), originalSpecificationItem.isConcludedAssumption(), originalSpecificationItem.isConditional(), originalSpecificationItem.isCorrectedAssumption(), originalSpecificationItem.isEveryGeneralization(), isNewExclusiveGeneralization, isNewExclusiveSpecification, originalSpecificationItem.isNegative(), originalSpecificationItem.isPartOf(), originalSpecificationItem.isPossessive(), originalSpecificationItem.isSpecific(), originalSpecificationItem.isSpecificationGeneralization(), originalSpecificationItem.isUncountableGeneralizationNoun(), originalSpecificationItem.isUniqueUserRelation(), originalSpecificationItem.isValueSpecification(), originalSpecificationItem.assignmentLevel(), originalSpecificationItem.assumptionLevel(), originalSpecificationItem.languageNr(), originalSpecificationItem.prepositionParameter(), originalSpecificationItem.questionParameter(), originalSpecificationItem.generalizationWordTypeNr(), originalSpecificationItem.specificationWordTypeNr(), originalSpecificationItem.relationWordTypeNr(), newGeneralizationCollectionNr, newSpecificationCollectionNr, originalSpecificationItem.generalizationContextNr(), originalSpecificationItem.specificationContextNr(), originalSpecificationItem.relationContextNr(), originalSpecificationItem.originalSentenceNr(), originalSpecificationItem.activeSentenceNr(), originalSpecificationItem.inactiveSentenceNr(), originalSpecificationItem.archivedSentenceNr(), originalSpecificationItem.nContextRelations(), newFirstJustificationItem, originalSpecificationItem.specificationWordItem(), originalSpecificationItem.specificationString(), originalSpecificationItem.storedSentenceStringBuffer(), originalSpecificationItem.storedSentenceWithOnlyOneSpecificationStringBuffer() ) ).result != Constants.RESULT_OK )
			return addError( 1, "I failed to create a specification item with different parameters" );

		if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return startError( 1, "I couldn't create a specification item" );

		if( replaceOrDeleteSpecification( originalSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to replace or delete the given specification item" );

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptions( boolean isInactiveAssignment, boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isSelfGeneratedAssumption() &&
			// Don't calculate hidden Spanish assumptions
			!searchSpecificationItem.isHiddenSpanishSpecification() &&
			searchSpecificationItem.recalculateAssumption() != Constants.RESULT_OK )
				return addError( 1, "I failed to recalculate an assumption" );

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, "The given obsolete specification item is already replaced or deleted" );

		if( obsoleteSpecificationItem == replacingSpecificationItem )
			return startError( 1, "The given obsolete specification item and the given replacing specification item are the same specification item" );

		// Don't replace a normal specification by a hidden Spanish specification
		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.isHiddenSpanishSpecification() &&
		!obsoleteSpecificationItem.isHiddenSpanishSpecification() )
			return startError( 1, "The given replacing specification item is a hidden specification, while the given obsolete specification item isn't hidden" );

		if( updateReplacedSpecifications( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to update the replacing specification item of the archive specification items" );

		obsoleteSpecificationItem.replacingSpecificationItem = replacingSpecificationItem;

		if( obsoleteSpecificationItem.hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to delete a specification item" );
			}
		else
			{
			if( replaceItem( obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to replace a specification item" );
			}

		if( updateSpecificationOfJustificationsOfInvolvedWords( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to update the specifications in the justification of involved words" );

		return Constants.RESULT_OK;
		}

	protected byte updateJustificationInSpecifications( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isReplacedItem, boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		boolean isForcingNewJustification;
		boolean isSameJustification;
		boolean isSelectedExclusiveSpecification;
		int relationContextNr;
		JustificationItem attachedPredecessorOfOldJustificationItem;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem searchSpecificationItem = ( isReplacedItem ? firstReplacedSpecificationItem() :
																		firstSpecificationItem( isInactiveAssignment, isArchivedAssignment ) );
		SpecificationItem updatedSecondarySpecificationItem;
		SpecificationItem userAssignmentItem;
		WordItem secondaryGeneralizationWordItem;
		WordItem thisWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		while( searchSpecificationItem != null )
			{
			// Select self-generated specifications
			if( ( firstJustificationItem = searchSpecificationItem.firstJustificationItem() ) != null )
				{
				// Check first justification
				if( firstJustificationItem == obsoleteJustificationItem )
					{
					// Allowed to change the current justification, without creating a new one
					if( searchSpecificationItem.hasCurrentCreationSentenceNr() )
						{
						if( searchSpecificationItem.changeFirstJustification( false, replacingJustificationItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to change the first justification item of a specification item" );

						searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
						}
					else
						{
						createdSpecificationItem = null;

						if( replacingJustificationItem != null )
							{
							isSelectedExclusiveSpecification = ( isExclusiveGeneralization ||
																searchSpecificationItem.isExclusiveSpecification() );
							relationContextNr = ( isExclusiveGeneralization ? Constants.NO_CONTEXT_NR : searchSpecificationItem.relationContextNr() );

							if( ( createAndAssignResult = createSpecificationItem( searchSpecificationItem.isInactiveAssignment(), isArchivedAssignment, searchSpecificationItem.isAnsweredQuestion(), searchSpecificationItem.isCharacteristicFor(), searchSpecificationItem.isConcludedAssumption(), searchSpecificationItem.isConditional(), searchSpecificationItem.isCorrectedAssumption(), searchSpecificationItem.isEveryGeneralization(), isExclusiveGeneralization, isSelectedExclusiveSpecification, searchSpecificationItem.isNegative(), searchSpecificationItem.isPartOf(), searchSpecificationItem.isPossessive(), searchSpecificationItem.isSpecific(), searchSpecificationItem.isSpecificationGeneralization(), searchSpecificationItem.isUncountableGeneralizationNoun(), searchSpecificationItem.isUniqueUserRelation(), searchSpecificationItem.isValueSpecification(), searchSpecificationItem.assignmentLevel(), searchSpecificationItem.assumptionLevel(), searchSpecificationItem.languageNr(), searchSpecificationItem.prepositionParameter(), searchSpecificationItem.questionParameter(), searchSpecificationItem.generalizationWordTypeNr(), searchSpecificationItem.specificationWordTypeNr(), searchSpecificationItem.relationWordTypeNr(), searchSpecificationItem.generalizationCollectionNr(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), relationContextNr, searchSpecificationItem.originalSentenceNr(), searchSpecificationItem.activeSentenceNr(), searchSpecificationItem.inactiveSentenceNr(), searchSpecificationItem.archivedSentenceNr(), searchSpecificationItem.nContextRelations(), replacingJustificationItem, searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString(), searchSpecificationItem.storedSentenceStringBuffer(), searchSpecificationItem.storedSentenceWithOnlyOneSpecificationStringBuffer() ) ).result != Constants.RESULT_OK )
								return addError( 1, "I failed to copy the search specification item" );

							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
								return startError( 1, "I couldn't copy the search specification item" );
							}

						if( replaceOrDeleteSpecification( searchSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to replace or delete a specification" );

						if( isExclusiveGeneralization &&
						thisWordItem.assignSpecification( false, false, true, false, searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.isSpecificationGeneralization(), searchSpecificationItem.isUniqueUserRelation(), searchSpecificationItem.assumptionLevel(), searchSpecificationItem.prepositionParameter(), searchSpecificationItem.questionParameter(), searchSpecificationItem.relationWordTypeNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.nContextRelations(), replacingJustificationItem, searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString(), null ).result != Constants.RESULT_OK )
							return addError( 1, "I failed to create an assignment" );

						searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
						}
					}
				else
					{
					// Check attached justification items
					if( ( attachedPredecessorOfOldJustificationItem = firstJustificationItem.attachedPredecessorOfOldJustificationItem( obsoleteJustificationItem ) ) == null )
						searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
					else
						{
						if( attachedPredecessorOfOldJustificationItem.hasCurrentCreationSentenceNr() )
							{
							isSameJustification = ( attachedPredecessorOfOldJustificationItem == replacingJustificationItem );

							if( attachedPredecessorOfOldJustificationItem.changeAttachedJustification( isSameJustification ? null : replacingJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, "I failed to change the attached justification item of a justification item" );

							searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
							}
						else
							{
							if( attachedPredecessorOfOldJustificationItem.isReplacedItem() )
								searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
							else
								{
								isForcingNewJustification = false;

								if( ( updatedSecondarySpecificationItem = attachedPredecessorOfOldJustificationItem.updatedSecondarySpecificationItem() ) != null &&
								updatedSecondarySpecificationItem.isUserSpecification() &&
								// Secondary user specification has no relation
								!updatedSecondarySpecificationItem.hasRelationContext() &&
								attachedPredecessorOfOldJustificationItem.isReversibleAssumptionOrConclusion() &&
								( secondaryGeneralizationWordItem = updatedSecondarySpecificationItem.generalizationWordItem() ) != null &&
								// Get user specification assignment
								( userAssignmentItem = secondaryGeneralizationWordItem.firstAssignmentItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, updatedSecondarySpecificationItem.specificationWordItem() ) ) != null )
									{
									// Secondary user specification without relation
									updatedSecondarySpecificationItem = userAssignmentItem;

									if( replacingJustificationItem != null &&
									replacingJustificationItem.hasFeminineOrMasculineProperNounEnding() )
										{
										isForcingNewJustification = true;
										replacingJustificationItem = null;
										}
									}

								if( ( justificationResult = thisWordItem.copyJustification( isForcingNewJustification, attachedPredecessorOfOldJustificationItem.updatedPrimarySpecificationItem(), updatedSecondarySpecificationItem, replacingJustificationItem, attachedPredecessorOfOldJustificationItem ) ).result != Constants.RESULT_OK )
									return addError( 1, "I failed to copy the attached predecessor of obsolete justification item with a different first justification item" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									return startError( 1, "I couldn't copy an attached predecessor of an old justification" );

								if( thisWordItem.replaceJustification( attachedPredecessorOfOldJustificationItem, createdJustificationItem, searchSpecificationItem ) != Constants.RESULT_OK )
									return addError( 1, "I failed to replace the attached predecessor of obsolete justification item by a created justification item" );

								searchSpecificationItem = firstSpecificationItem( isInactiveAssignment, isArchivedAssignment );
								}
							}
						}
					}
				}
			else
				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte updateSpecificationOfJustificationsOfInvolvedWords( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem thisWordItem = myWordItem();

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		// Update specifications in justifications of this word
		if( thisWordItem.updateSpecificationOfJustificationsInWord( true, obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to update the specifications in my justification items" );

		// Typical for Spanish
		// Condition block excludes some updates in involved words
		if( ( replacingSpecificationItem == null ||
		replacingSpecificationItem.hasRelationContext() ||
		!obsoleteSpecificationItem.hasRelationContext() ) &&

		( currentGeneralizationItem = thisWordItem.firstGeneralizationItem() ) != null )
			{
			// Do for all generalization words
			// Update specifications in justifications of involved words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return startSystemError( 1, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem.updateSpecificationOfJustificationsInWord( false, obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to update the specifications in the justification items in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isSpecificationWordSpanishAmbiguous;
		int nCurrentRelationContextWords;
		int nFoundRelationContextWords = 0;
		int relationContextNr;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&

				( relationWordItem == null ||

				( ( relationContextNr = searchSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
				relationWordItem.hasContextInWord( relationContextNr, specificationWordItem ) ) ) )
					{
					if( ( isPossessive &&
					!isSpecificationWordSpanishAmbiguous ) ||

					( !hasRelationWord &&
					isSpecificationWordSpanishAmbiguous ) )
						return searchSpecificationItem;

					nCurrentRelationContextWords = searchSpecificationItem.nRelationContextWords();

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

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isNegative, boolean isPossessive, int specificationCollectionNr, WordItem specificationWordItem )
		{
		int nFoundRelationContextWords = 0;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, false, false );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive )
					{
					// Prefer matching specification collection over matching specification word
					if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
					searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
						return searchSpecificationItem;

					if( nFoundRelationContextWords == 0 &&
					searchSpecificationItem.specificationWordItem() == specificationWordItem &&
					!searchSpecificationItem.isHiddenSpanishSpecification() )
						{
						nFoundRelationContextWords = searchSpecificationItem.nRelationContextWords();
						foundSpecificationItem = searchSpecificationItem;
						}
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		int nCurrentRelationContextWords;
		int nFoundRelationContextWords = 0;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					{
					nCurrentRelationContextWords = searchSpecificationItem.nRelationContextWords();

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

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingRelationContextNrSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isIncludingAnsweredQuestions, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isQuestion, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, isInactiveAssignment, isArchivedAssignment, isQuestion );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.isMatchingRelationContextNr( isAllowingEmptyRelationContext, relationContextNr ) )
					{
					if( isQuestion ||
					searchSpecificationItem.relationContextNr() == relationContextNr )
						return searchSpecificationItem;

					foundSpecificationItem = searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		// The given specification word item can be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					return searchSpecificationItem;

				if( foundSpecificationItem == null &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					{
					if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
						return searchSpecificationItem;

					// Wait for matching specification collection, if defined
					foundSpecificationItem = searchSpecificationItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem bestMatchingSpecificationWordSpecificationItem( boolean isAllowingEmptyGeneralizationContext, boolean isAllowingEmptyRelationContext, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isMatchingGeneralizationContextNr( isAllowingEmptyGeneralizationContext, generalizationContextNr ) &&
			searchSpecificationItem.isMatchingRelationContextNr( isAllowingEmptyRelationContext, relationContextNr ) &&

			( specificationCollectionNr == Constants.NO_COLLECTION_NR ||
			searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr ) )
				{
				// Prefer matching specification collection over matching specification word
				if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				!searchSpecificationItem.isSpecificationGeneralization() )
					return searchSpecificationItem;

				if( foundSpecificationItem == null &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					{
					if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
						return searchSpecificationItem;

					// Wait for matching specification collection 
					foundSpecificationItem = searchSpecificationItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstActiveSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isQuestion )
		{
		SpecificationItem firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null ?
				firstSpecificationItem.specificationItem( isIncludingAnsweredQuestions, true, isQuestion ) : null );
		}

	protected SpecificationItem firstActiveSpecificationItem( boolean isIncludingAnsweredQuestions, short questionParameter )
		{
		SpecificationItem firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null ?
				firstSpecificationItem.specificationItem( isIncludingAnsweredQuestions, true, questionParameter ) : null );
		}

	protected SpecificationItem firstExclusiveSpecificationItem()
		{
		SpecificationItem firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null ?
				firstSpecificationItem.exclusiveSpecificationItem( true ) : null );
		}

	protected SpecificationItem firstExclusiveSpecificationItem( WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isExclusiveSpecification() &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstFeminineOrMasculineSpecificationItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem specificationWordItem;

		while( searchSpecificationItem != null )
			{
			if( ( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null &&
			specificationWordItem.isFemaleOrMale() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonCompoundUserSpecificationItem()
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasNonCompoundSpecificationCollection() &&
			searchSpecificationItem.isUserSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonCompoundCollectionSpecificationItem( int compoundSpecificationCollectionNr )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem specificationWordItem;

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.hasSpecificationCollection() &&
			!searchSpecificationItem.isNegative() &&
			!searchSpecificationItem.isPossessive() &&
			searchSpecificationItem.hasNonCompoundSpecificationCollection() &&
			searchSpecificationItem.specificationCollectionNr() != compoundSpecificationCollectionNr &&
			( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null &&
			specificationWordItem.hasCollectionNr( compoundSpecificationCollectionNr ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstNonPossessiveDefinitionSpecificationItem( boolean isIncludingAdjectives )
		{
		SpecificationItem firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null ?
				firstSpecificationItem.nonPossessiveDefinitionSpecificationItem( isIncludingAdjectives, true ) : null );
		}

	protected SpecificationItem firstPossessiveSpecificationItem()
		{
		SpecificationItem firstSpecificationItem;

		return ( ( firstSpecificationItem = firstActiveSpecificationItem() ) != null ?
				firstSpecificationItem.possessiveSpecificationItem( true ) : null );
		}

	protected SpecificationItem firstRecentlyAnsweredQuestionSpecificationItem( boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( true, false, isArchivedAssignment, true );

		return ( searchSpecificationItem != null ?
				searchSpecificationItem.recentlyAnsweredQuestionSpecificationItem( true ) : null );
		}

	protected SpecificationItem firstRelationSpecificationItem( boolean isPossessive, WordItem relationWordItem )
		{
		int relationContextNr;
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		if( relationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isPossessive() == isPossessive &&
				( relationContextNr = searchSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
				relationWordItem.hasContextInWord( relationContextNr, searchSpecificationItem.specificationWordItem() ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSelectedRelationSpecificationItem( boolean isArchivedAssignment )
		{
		SpecificationItem firstSelectedSpecificationItem;

		return ( ( firstSelectedSpecificationItem = firstSpecificationItem( false, isArchivedAssignment ) ) != null ?
				firstSelectedSpecificationItem.selectedRelationSpecificationItem( true ) : null );
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isIncludingAnsweredQuestions, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, WordItem specificationWordItem )
		{
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( isIncludingAnsweredQuestions, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					{
					if( searchSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification )
						return searchSpecificationItem;

					if( foundSpecificationItem == null )
						foundSpecificationItem = searchSpecificationItem;
					}

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions );
				}
			}

		return foundSpecificationItem;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int relationContextNr;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, false );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption &&
				searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&

				( ( relationWordItem == null &&

				( isAllowingEmptyRelationContext ||
				searchSpecificationItem.hasRelationContext() ) ) ||

				( relationWordItem != null &&
				( relationContextNr = searchSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
				relationWordItem.hasContextInWord( relationContextNr, specificationWordItem ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSelfGeneratedCheckSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, short questionParameter, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem &&
				searchSpecificationItem.isMatchingRelationContextNr( isAllowingEmptyRelationContext, relationContextNr ) &&

				( searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr ||

				( relationContextNr == Constants.NO_CONTEXT_NR &&
				specificationWordItem.isCompoundCollection( specificationCollectionNr ) ) ) )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, false, false );

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( !searchSpecificationItem.isSpecificationGeneralization() &&
				searchSpecificationItem.isNegative() == isNegative &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.relationContextNr() == relationContextNr &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, questionParameter );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isSpecificationGeneralization() == isSpecificationGeneralization &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstSpecificationItem( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isQuestion, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, isQuestion );

		// In case of a specification string, the given specification word item will be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.specificationWordItem() == specificationWordItem )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstUnhiddenSpanishSpecificationItem( boolean isArchivedAssignment )
		{
		SpecificationItem searchSpecificationItem = firstSpecificationItem( false, isArchivedAssignment );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.wasHiddenSpanishSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem firstUserSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, isInactiveAssignment, isArchivedAssignment, false );

		// The given specification word item can be undefined

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isUserSpecification() &&
			searchSpecificationItem.isNegative() == isNegative &&
			searchSpecificationItem.isPossessive() == isPossessive &&
			searchSpecificationItem.isMatchingRelationContextNr( false, relationContextNr ) &&

			( searchSpecificationItem.specificationWordItem() == specificationWordItem ||

			( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			searchSpecificationItem.specificationCollectionNr() == specificationCollectionNr ) ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem noRelationContextSpecificationItem( boolean isPossessive, boolean isSelfGenerated, WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( !searchSpecificationItem.hasRelationContext() &&
				!searchSpecificationItem.isNegative() &&
				searchSpecificationItem.isOlderItem() &&
				!searchSpecificationItem.isPartOf() &&
				searchSpecificationItem.isPossessive() == isPossessive &&
				searchSpecificationItem.isSelfGenerated() == isSelfGenerated &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem partOfSpecificationItem( WordItem specificationWordItem )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem();

		if( specificationWordItem != null )
			{
			while( searchSpecificationItem != null )
				{
				if( searchSpecificationItem.isPartOf() &&
				searchSpecificationItem.specificationWordItem() == specificationWordItem )
					return searchSpecificationItem;

				searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem sameUserQuestionSpecificationItem( boolean isArchivedAssignment, short questionParameter )
		{
		int nUserSpecificationWords = GlobalVariables.nUserSpecificationWords;
		SpecificationItem searchSpecificationItem = firstAssignmentOrSpecificationItem( false, false, isArchivedAssignment, questionParameter );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isUserQuestion() &&
			searchSpecificationItem.nInvolvedSpecificationWords() == nUserSpecificationWords )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return null;
		}

	protected WordItem feminineOrMasculineCommonWordItem( boolean isFeminineWord )
		{
		SpecificationItem searchSpecificationItem = firstActiveSpecificationItem( false, false );
		WordItem commonWordItem;
		WordItem specificationWordItem;
		WordItem thisWordItem = myWordItem();

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isExclusiveSpecification() &&
			( specificationWordItem = searchSpecificationItem.specificationWordItem() ) != null &&
			specificationWordItem.isFemaleOrMale() &&
			( commonWordItem = specificationWordItem.commonWordItem( searchSpecificationItem.specificationCollectionNr() ) ) != null )
				{
				if( commonWordItem == thisWordItem )
					{
					if( specificationWordItem.isFemale() == isFeminineWord )
						return specificationWordItem;
					}
				else
					{
					if( ( isFeminineWord &&
					commonWordItem.isFemale() ) ||

					( !isFeminineWord &&
					commonWordItem.isMale() ) )
						return commonWordItem;
					}
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedSpecificationItem();
			}

		return null;
		}

	protected BoolResultType findQuestionToBeAdjustedByCompoundCollection( int questionSpecificationCollectionNr, WordItem primarySpecificationWordItem )
		{
		SpecificationItem adjustedQuestionCompoundSpecificationItem;
		SpecificationItem lastAdjustedQuestionCompoundSpecificationItem = null;
		WordItem commonWordItem;
		WordItem currentCollectionWordItem;
		BoolResultType boolResult = new BoolResultType();

		if( primarySpecificationWordItem == null )
			return startBoolResultError( 1, "The given primary specification word item is undefined" );

		if( questionSpecificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startBoolResultError( 1, "The given question specification collection number is undefined" );

		if( ( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( ( commonWordItem = currentCollectionWordItem.commonWordItem( questionSpecificationCollectionNr ) ) != null &&
				commonWordItem != primarySpecificationWordItem &&
				( adjustedQuestionCompoundSpecificationItem = firstSpecificationItem( false, false, false, true, currentCollectionWordItem ) ) != null )
					{
					if( replaceOrDeleteSpecification( adjustedQuestionCompoundSpecificationItem, null ) != Constants.RESULT_OK )
						return addBoolResultError( 1, "I failed to replace or delete a question part" );

					lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem;
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != null )
			{
			// Write last instance of adjusted compound question
			if( myWordItem().writeUpdatedSpecification( true, false, false, false, false, lastAdjustedQuestionCompoundSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
				return addBoolResultError( 1, "I failed to write an adjusted compound question" );

			boolResult.booleanValue = true;
			}

		return boolResult;
		}

	protected CreateAndAssignResultType createSpecificationItem( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isAnsweredQuestion, boolean isCharacteristicFor, boolean isConcludedAssumption, boolean isConditional, boolean isCorrectedAssumption, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer storedSentenceStringBuffer, StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer )
		{
		boolean isAssignment = isAssignmentList();
		WordItem thisWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( generalizationWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		generalizationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startCreateAndAssignResultError( 1, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		specificationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startCreateAndAssignResultError( 1, "The given specification word type number is undefined or out of bounds" );

		if( firstJustificationItem != null &&
		!firstJustificationItem.isActiveItem() )
			return startCreateAndAssignResultError( 1, "The given first justification item isn't active" );

		if( specificationWordItem != null &&
		specificationWordItem.isNounValue() )
			return startCreateAndAssignResultError( 1, "The given specification word item is a value word" );

		if( !isMarkedAsSpecificationWord_ )
			{
			isMarkedAsSpecificationWord_ = true;

			if( isAssignment )
				addToAssignmentWordQuickAccessList();
			else
				{
				if( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR )
					{
					if( isPossessive ||
					isCharacteristicFor )
						addToPossessiveNounWordQuickAccessList();
					}
				else
					{
					if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN &&
					thisWordItem.hasUserNr() )
						addToUserProperNounWordQuickAccessList();
					}

				addToSpecificationWordQuickAccessList();
				}
			}

		if( ( createAndAssignResult.createdSpecificationItem = new SpecificationItem( isAnsweredQuestion, isCharacteristicFor, isConcludedAssumption, isConditional, isCorrectedAssumption, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, thisWordItem.isLanguageWord(), isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assignmentLevel, assumptionLevel, languageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, ( isAssignment ? activeSentenceNr : Constants.NO_SENTENCE_NR ), ( isAssignment ? inactiveSentenceNr : Constants.NO_SENTENCE_NR ), ( isAssignment ? archivedSentenceNr : Constants.NO_SENTENCE_NR ), nContextRelations, firstJustificationItem, specificationWordItem, specificationString, storedSentenceStringBuffer, storedSentenceWithOnlyOneSpecificationStringBuffer, this, thisWordItem ) ) == null )
			return startCreateAndAssignResultError( 1, "I failed to create a specification item" );

		if( addItemToList( ( isArchivedAssignment ? Constants.QUERY_ARCHIVED_CHAR : ( isInactiveAssignment ? Constants.QUERY_INACTIVE_CHAR : Constants.QUERY_ACTIVE_CHAR ) ), createAndAssignResult.createdSpecificationItem ) != Constants.RESULT_OK )
			return addCreateAndAssignResultError( 1, "I failed to add an assignment item" );

		if( isAssignment &&
		assignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
		originalSentenceNr == GlobalVariables.currentSentenceNr )
			GlobalVariables.isAssignmentChanged = true;

		return createAndAssignResult;
		}
	};

/*************************************************************************
 *	"Good comes to those who lend money generously
 *	and conduct their business fairly.
 *	Such people will not be overcome by evil.
 *	Those who are righteous will be long remembered" (Psalm 112:5-6)
 *************************************************************************/
