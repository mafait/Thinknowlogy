/*
 *	Class:			WordAssignment
 *	Supports class:	WordItem
 *	Purpose:		To assign specifications
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class WordAssignment
	{
	// Private constructible variables

	private WordItem myWordItem_;

	private String moduleNameString_;


	// Private methods

	private byte inactivateCurrentAssignments()
		{
		SpecificationItem activeAssignmentItem;

		while( ( activeAssignmentItem = myWordItem_.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			if( inactivateActiveAssignment( activeAssignmentItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactive an active assignment item" );
			}

		return Constants.RESULT_OK;
		}

	private byte inactivateOrArchiveCurrentGeneralizationAssignments( boolean isNegative, boolean isPossessive, int relationContextNr, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundActiveAssignmentItem = null;
		SpecificationItem foundInactiveAssignmentItem = null;
		WordItem currentGeneralizationWordItem;
		WordItem foundActiveGeneralizationWordItem;
		WordItem foundInactiveGeneralizationWordItem;

		if( specificationWordItem != null )
			{
			if( !specificationWordItem.isNounValue() )
				{
				if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem() ) != null )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
							{
							if( foundActiveAssignmentItem == null &&
							// Skip my word for activate assignments, because this is the one to be assigned
							currentGeneralizationWordItem != myWordItem_ )
								foundActiveAssignmentItem = currentGeneralizationWordItem.firstNonQuestionAssignmentItem( true, false, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

							// Allow to find an inactive assignment in my word
							if( foundInactiveAssignmentItem == null )
								foundInactiveAssignmentItem = currentGeneralizationWordItem.firstNonQuestionAssignmentItem( false, true, false, isNegative, isPossessive, relationContextNr, specificationWordItem );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I've found an undefined generalization word" );
						}
					while( ( foundActiveAssignmentItem == null ||
					foundInactiveAssignmentItem == null ) &&

					( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
					}

				// Only archive an inactive assignment if an active assignment is being inactivated
				if( foundActiveAssignmentItem != null )
					{
					if( foundActiveAssignmentItem.hasGeneralizationCollection() )
						{
						if( ( foundActiveGeneralizationWordItem = foundActiveAssignmentItem.generalizationWordItem() ) != null )
							{
							if( foundActiveGeneralizationWordItem.inactivateActiveAssignment( foundActiveAssignmentItem ) == Constants.RESULT_OK )
								{
								if( foundInactiveAssignmentItem != null )
									{
									if( foundInactiveAssignmentItem.hasGeneralizationCollection() )
										{
										if( ( foundInactiveGeneralizationWordItem = foundInactiveAssignmentItem.generalizationWordItem() ) != null )
											{
											if( foundInactiveGeneralizationWordItem.archiveInactiveAssignment( foundInactiveAssignmentItem ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to archive an inactive assignment" );
											}
										else
											return myWordItem_.startErrorInWord( 1, moduleNameString_, "I've found an inactive assignment without generalization word item" );
										}
									else
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I've found an inactive assignment without generalization collection number" );
									}
								}
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an active assignment" );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I've found an active assignment without generalization word item" );
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I've found an active assignment without generalization collection number" );
					}
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "Inactivating or archiving a noun value specification isn't implemented yet" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte inactivateAssignment( boolean isAmbiguousRelationContext, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundActiveAssignmentItem;
		SpecificationItem relatedSpecificationItem;

		if( ( foundAssignmentItem = myWordItem_.firstActiveNonQuestionAssignmentItem( isAmbiguousRelationContext, isPossessive, relationContextNr, specificationWordItem ) ) == null )
			{
			if( relationContextNr == Constants.NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( specificationResult = myWordItem_.findRelatedSpecification( true, true, false, isExclusiveSpecification, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result == Constants.RESULT_OK )
						{
						relatedSpecificationItem = specificationResult.relatedSpecificationItem;

						if( relatedSpecificationItem != null &&
						// Skip specification
						relatedSpecificationItem.isAssignment() &&

						( ( !isNegative &&
						// Only option left
						!isSelfGenerated ) ||

						!relatedSpecificationItem.isNegative() ) )
							{
							if( inactivateActiveAssignment( relatedSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate a related active assignment" );
							}
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related assignment" );
					}
				}
			else
				{
				// With relation context
				if( ( foundActiveAssignmentItem = myWordItem_.firstAssignmentItem( true, false, false, isNegative, isPossessive, isSelfGenerated, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
					{
					if( ( foundAssignmentItem = myWordItem_.firstAssignmentItem( false, true, false, isNegative, isPossessive, isSelfGenerated, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
						{
						// First archive an inactive assignment
						if( archiveInactiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to archive an inactive assignment" );
						}

					// Now inactivate the found active assignment
					if( inactivateActiveAssignment( foundActiveAssignmentItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an active assignment" );
					}
				}
			}
		else
			{
			if( isAmbiguousRelationContext ||

			( !foundAssignmentItem.isUniqueUserRelation() &&
			foundAssignmentItem.isNegative() != isNegative ) )
				{
				if( inactivateActiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate a negative active assignment" );
				}
			}

		return Constants.RESULT_OK;
		}

	private SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasDifferentAssumptionLevel = false;
		boolean isAmbiguous = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		SpecificationItem createdAssignmentItem;
		SpecificationItem foundAssignmentItem;

		if( !isQuestion )
			{
			if( inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
				myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an assignment" );
			}

		if( CommonVariables.result == Constants.RESULT_OK )
			{
			if( ( foundAssignmentItem = myWordItem_.firstAssignmentItem( true, true, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == null )
				{
				// Found no self-generated assignment. Now try to find a confirmed assignment
				if( isSelfGenerated )
					foundAssignmentItem = myWordItem_.firstAssignmentItem( true, true, true, isNegative, isPossessive, false, questionParameter, relationContextNr, specificationWordItem );
				}

			if( foundAssignmentItem != null )
				{
				specificationResult.foundSpecificationItem = foundAssignmentItem;

				if( foundAssignmentItem.assumptionLevel() != assumptionLevel )
					hasDifferentAssumptionLevel = true;

				// Ambiguous if assignment has different tense (time): active, inactive or archived.
				if( foundAssignmentItem.isInactiveAssignment() != isInactiveAssignment ||
				foundAssignmentItem.isArchivedAssignment() != isArchivedAssignment )
					isAmbiguous = true;
				}

			if( isAmbiguous ||
			foundAssignmentItem == null ||

			( !isSelfGenerated &&
			hasDifferentAssumptionLevel ) )
				{
				if( ( specificationResult = myWordItem_.createSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false, false, false, false, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, CommonVariables.currentAssignmentLevel, assumptionLevel, CommonVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null ) ).result == Constants.RESULT_OK )
					{
					if( ( createdAssignmentItem = specificationResult.createdSpecificationItem ) != null )
						{
						if( foundAssignmentItem != null )
							{
							if( hasDifferentAssumptionLevel )
								{
								if( !isQuestion )
									{
									if( myWordItem_.replaceOrDeleteSpecification( foundAssignmentItem, ( foundAssignmentItem.isSelfGenerated() ? createdAssignmentItem : null ) ) != Constants.RESULT_OK )
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an ambiguous assignment" );
									}
								}
							else
								{
								if( foundAssignmentItem.isActiveItem() )
									{
									if( inactivateActiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an active assignment" );
									}
								else
									{
									if( foundAssignmentItem.isInactiveAssignment() &&
									// Avoid archiving inactive assignments that are created during this sentence
									!foundAssignmentItem.hasCurrentInactiveSentenceNr() )
										{
										if( archiveInactiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
											myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to archive an inactive assignment" );
										}
									}
								}
							}
						}
					else
						{
						if( specificationWordItem == null )
							myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't create an assignment item with specification string \"" + specificationString + "\"" );
						else
							myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't create an assignment item with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					if( specificationWordItem == null )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create an assignment with specification string \"" + specificationString + "\"" );
					else
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create an assignment with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	private SpecificationResultType assignSpecificationByValue( JustificationItem firstJustificationItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		SpecificationItem currentAssignmentItem;

		if( specificationWordItem != null )
			{
			if( ( currentAssignmentItem = specificationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
				{
				do	{
					if( ( specificationResult = assignSpecification( false, currentAssignmentItem.isInactiveAssignment(), currentAssignmentItem.isArchivedAssignment(), currentAssignmentItem.isEveryGeneralization(), currentAssignmentItem.isExclusiveSpecification(), currentAssignmentItem.isGeneralizationAssignment(), currentAssignmentItem.isNegative(), currentAssignmentItem.isPartOf(), currentAssignmentItem.isPossessive(), currentAssignmentItem.isSpecificationGeneralization(), currentAssignmentItem.isUniqueUserRelation(), currentAssignmentItem.assumptionLevel(), currentAssignmentItem.prepositionParameter(), currentAssignmentItem.questionParameter(), currentAssignmentItem.generalizationWordTypeNr(), currentAssignmentItem.specificationWordTypeNr(), currentAssignmentItem.relationWordTypeNr(), currentAssignmentItem.generalizationCollectionNr(), currentAssignmentItem.specificationCollectionNr(), currentAssignmentItem.generalizationContextNr(), currentAssignmentItem.specificationContextNr(), currentAssignmentItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, currentAssignmentItem.nContextRelations(), firstJustificationItem, currentAssignmentItem.specificationWordItem(), currentAssignmentItem.specificationString() ) ).result != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to assign specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}


		// Constructor / deconstructor

	protected WordAssignment( WordItem myWordItem )
		{
		String errorString = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte createNewAssignmentLevel()
		{
		SpecificationList assignmentList;

		if( !myWordItem_.isAdminWord() )
			{
			if( ( assignmentList = myWordItem_.assignmentList ) != null )
				{
				if( assignmentList.createNewAssignmentLevel() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create assignment level " + ( CommonVariables.currentAssignmentLevel + 1 ) );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The admin word item cannot have assignments" );

		return Constants.RESULT_OK;
		}

	protected byte inactivateActiveAssignment( SpecificationItem activeAssignmentItem )
		{
		SpecificationList assignmentList;

		if( activeAssignmentItem != null )
			{
			if( activeAssignmentItem.isActiveAssignment() )
				{
				if( ( assignmentList = myWordItem_.assignmentList ) != null )
					{
					if( assignmentList.inactivateItem( activeAssignmentItem ) == Constants.RESULT_OK )
						activeAssignmentItem.markAsGeneralizationAssignment();
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an active assignment" );
					}
				else
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The assignment list isn't created yet" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given assignment item isn't active" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given active assignment item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte archiveInactiveAssignment( SpecificationItem inactiveAssignmentItem )
		{
		SpecificationList assignmentList;

		if( inactiveAssignmentItem != null )
			{
			if( inactiveAssignmentItem.isInactiveAssignment() )
				{
				if( ( assignmentList = myWordItem_.assignmentList ) != null )
					{
					if( assignmentList.archiveItem( inactiveAssignmentItem ) == Constants.RESULT_OK )
						inactiveAssignmentItem.markAsGeneralizationAssignment();
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to archive the given inactive assignment item" );
					}
				else
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The assignment list isn't created yet" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given assignment item isn't inactive" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given inactive assignment item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType getAssignmentOrderNr()
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		short specificationNr = 0;
		SpecificationItem orderAssignmentItem;
		SpecificationItem currentSpecificationItem;
		WordItem assignmentWordItem;

		if( myWordItem_.nActiveAssignments() <= 1 )
			{
			if( ( orderAssignmentItem = myWordItem_.firstNonQuestionActiveAssignmentItem() ) != null )
				{
				if( ( assignmentWordItem = orderAssignmentItem.specificationWordItem() ) != null &&
				( currentSpecificationItem = myWordItem_.firstNonQuestionSpecificationItem() ) != null )
					{
					do	{
						specificationNr++;

						if( currentSpecificationItem.specificationWordItem() == assignmentWordItem )
							specificationResult.assignmentOrderNr = specificationNr;
						}
					while( specificationResult.assignmentOrderNr == Constants.NO_ORDER_NR &&
					( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
					}
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "I've more than one assignments at assignment level " + CommonVariables.currentAssignmentLevel );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType getAssignmentWordParameter()
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		SpecificationItem assignmentItem;
		WordItem specificationWordItem;

		if( myWordItem_.nActiveAssignments() <= 1 )
			{
			if( ( assignmentItem = myWordItem_.firstNonQuestionActiveAssignmentItem() ) != null )
				{
				if( ( specificationWordItem = assignmentItem.specificationWordItem() ) != null )
					specificationResult.assignmentParameter = specificationWordItem.wordParameter();
				else
					myWordItem_.startErrorInWord( 1, moduleNameString_, "I've found an undefined assignment word at assignment level " + CommonVariables.currentAssignmentLevel );
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "I've more than one assignments at assignment level " + CommonVariables.currentAssignmentLevel );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		SpecificationItem foundSpecificationItem;

		if( isAssignedOrClear )
			{
			if( questionParameter == Constants.NO_QUESTION_PARAMETER )
				{
				if( inactivateCurrentAssignments() != Constants.RESULT_OK )
					myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate my current assignments" );
				}
			else
				myWordItem_.startErrorInWord( 1, moduleNameString_, "A question can only be answered, not be cleared" );
			}
		else
			{
			// Find the specification of the assignment
			if( ( foundSpecificationItem = myWordItem_.firstSpecificationItem( isPossessive, questionParameter, specificationWordItem ) ) != null )
				{
				if( foundSpecificationItem.hasExclusiveGeneralizationCollection() )
					{
					if( inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate or archive current generalization assignments" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( foundSpecificationItem.isValueSpecification() )
						{
						if( ( specificationResult = assignSpecificationByValue( firstJustificationItem, specificationWordItem ) ).result != Constants.RESULT_OK )
							myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to assign the value of a specification word" );
						}
					else
						{
						if( ( specificationResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.isExclusiveSpecification(), ( foundSpecificationItem.isExclusiveSpecification() && foundSpecificationItem.isGeneralizationProperName() ), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem.generalizationWordTypeNr(), foundSpecificationItem.specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
							{
							if( specificationWordItem == null )
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to assign specification string \"" + specificationString + "\"" );
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to assign specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				}
			else
				{
				if( specificationWordItem == null )
					myWordItem_.startErrorInWord( 1, moduleNameString_, "String \"" + specificationString + "\" isn't one of my specifications" );
				else
					myWordItem_.startErrorInWord( 1, moduleNameString_, "Word \"" + specificationWordItem.anyWordTypeString() + "\" isn't one of my specifications" );
				}
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"The Lord has made the heavens his throne;
 *	from there he rules over everything." (Psalm 103:19)
 *************************************************************************/
