/*
 *	Class:			WordAssignment
 *	Supports class:	WordItem
 *	Purpose:		To assign specifications
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

	private byte inactivateOrArchiveCurrentGeneralizationAssignments( boolean isNegative, boolean isPossessive, short questionParameter, int relationContextNr, WordItem specificationWordItem )
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
							currentGeneralizationWordItem != myWordItem_ )		// Skip my word for activate assignments, because this is the one to be assigned
								foundActiveAssignmentItem = currentGeneralizationWordItem.firstAssignmentItem( true, false, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

							if( foundInactiveAssignmentItem == null )		// Allow to find an inactive assignment in my word
								foundInactiveAssignmentItem = currentGeneralizationWordItem.firstAssignmentItem( false, true, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );
						}
					while( ( foundActiveAssignmentItem == null ||
					foundInactiveAssignmentItem == null ) &&

					( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
					}

				if( foundActiveAssignmentItem != null )		// Only archive an inactive assignment when an active assignment is being inactivated
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
											return myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an inactive assignment without generalization word item" );
										}
									else
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an inactive assignment without generalization collection number" );
									}
								}
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an active assignment" );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an active assignment without generalization word item" );
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an active assignment without generalization collection number" );
					}
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "Processing a noun value specification isn't implemented yet" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte inactivateAssignment( boolean isAmbiguousRelationContext, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundActiveAssignmentItem;
		SpecificationItem relatedSpecificationItem;

		if( ( foundAssignmentItem = myWordItem_.firstActiveAssignmentItem( isAmbiguousRelationContext, isPossessive, Constants.NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) == null )
			{
			if( relationContextNr == Constants.NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( specificationResult = myWordItem_.findRelatedSpecification( true, true, false, false, isExclusiveSpecification, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
						{
						relatedSpecificationItem = specificationResult.relatedSpecificationItem;

						if( relatedSpecificationItem != null &&
						relatedSpecificationItem.isAssignment() &&		// Skip specification

						( ( !isNegative &&
						!isSelfGenerated ) ||							// Only option left

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
			else	// With relation context
				{
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

			( !foundAssignmentItem.isUniqueRelation() &&
			foundAssignmentItem.isNegative() != isNegative ) )
				{
				if( inactivateActiveAssignment( foundAssignmentItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate a negative active assignment" );
				}
			}

		return Constants.RESULT_OK;
		}

	private SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isGeneralizationAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString )
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
			if( inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) != Constants.RESULT_OK )
				myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an assignment" );
			}

		if( CommonVariables.result == Constants.RESULT_OK )
			{
			if( ( foundAssignmentItem = myWordItem_.firstAssignmentItem( true, true, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == null )
				{
				if( isSelfGenerated )	// Found no self-generated assignment. Now try to find a confirmed assignment.
					foundAssignmentItem = myWordItem_.firstAssignmentItem( true, true, true, isNegative, isPossessive, false, questionParameter, relationContextNr, specificationWordItem );
				}

			if( foundAssignmentItem != null )
				{
				if( foundAssignmentItem.assumptionLevel() != assumptionLevel )
					hasDifferentAssumptionLevel = true;

				if( foundAssignmentItem.isInactiveAssignment() != isInactiveAssignment ||	// Ambiguous when has different tense (time)
				foundAssignmentItem.isArchivedAssignment() != isArchivedAssignment )		// (active, inactive or archive)
					isAmbiguous = true;
				}

			if( isAmbiguous ||
			foundAssignmentItem == null ||

			( !isSelfGenerated &&
			hasDifferentAssumptionLevel ) )
				{
				if( ( specificationResult = myWordItem_.createSpecification( true, isInactiveAssignment, isArchivedAssignment, false, false, false, false, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, false, CommonVariables.currentAssignmentLevel, assumptionLevel, CommonVariables.currentGrammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null ) ).result == Constants.RESULT_OK )
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

	private SpecificationResultType assignSpecificationByValue( short questionParameter, JustificationItem firstJustificationItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		SpecificationItem currentAssignmentItem;

		if( specificationWordItem != null )
			{
			if( ( currentAssignmentItem = specificationWordItem.firstActiveAssignmentItem( questionParameter ) ) != null )
				{
				do	{
					if( ( specificationResult = assignSpecification( false, currentAssignmentItem.isInactiveAssignment(), currentAssignmentItem.isArchivedAssignment(), currentAssignmentItem.isEveryGeneralization(), currentAssignmentItem.isExclusiveSpecification(), currentAssignmentItem.isGeneralizationAssignment(), currentAssignmentItem.isNegative(), currentAssignmentItem.isPartOf(), currentAssignmentItem.isPossessive(), currentAssignmentItem.isSpecificationGeneralization(), currentAssignmentItem.isUniqueRelation(), currentAssignmentItem.assumptionLevel(), currentAssignmentItem.prepositionParameter(), currentAssignmentItem.questionParameter(), currentAssignmentItem.generalizationWordTypeNr(), currentAssignmentItem.specificationWordTypeNr(), currentAssignmentItem.relationWordTypeNr(), currentAssignmentItem.generalizationCollectionNr(), currentAssignmentItem.specificationCollectionNr(), currentAssignmentItem.generalizationContextNr(), currentAssignmentItem.specificationContextNr(), currentAssignmentItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, currentAssignmentItem.nContextRelations(), firstJustificationItem, currentAssignmentItem.specificationWordItem(), currentAssignmentItem.specificationString() ) ).result != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to assign specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				( currentAssignmentItem = currentAssignmentItem.nextSpecificationItemWithSameQuestionParameter() ) != null );
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
		if( !myWordItem_.isAdminWord() )
			{
			if( myWordItem_.assignmentList != null )
				{
				if( myWordItem_.assignmentList.createNewAssignmentLevel() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create assignment level " + ( CommonVariables.currentAssignmentLevel + 1 ) );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The admin word item cannot have assignments" );

		return Constants.RESULT_OK;
		}

	protected byte inactivateActiveAssignment( SpecificationItem activeAssignmentItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem replacingAssignmentItem;

		if( activeAssignmentItem != null )
			{
			if( activeAssignmentItem.isActiveAssignment() )
				{
				if( myWordItem_.assignmentList != null )
					{
					if( activeAssignmentItem.hasInactiveSentenceNr() )		// Has been inactive before
						{
						if( ( specificationResult = myWordItem_.copySpecification( true, false, activeAssignmentItem.isAnsweredQuestion(), activeAssignmentItem.isExclusiveSpecification(), activeAssignmentItem.assignmentLevel(), activeAssignmentItem.generalizationCollectionNr(), activeAssignmentItem.specificationCollectionNr(), activeAssignmentItem.relationContextNr(), activeAssignmentItem.firstJustificationItem(), activeAssignmentItem ) ).result == Constants.RESULT_OK )
							{
							if( ( replacingAssignmentItem = specificationResult.createdSpecificationItem ) != null )
								{
								if( myWordItem_.replaceOrDeleteSpecification( activeAssignmentItem, replacingAssignmentItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the given active assignment" );
								}
							else
								return myWordItem_.startErrorInWord( 1, moduleNameString_, "The last created assignment item is undefined" );
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to copy an active assignment item to an inactive assignment item" );
						}
					else
						{
						if( myWordItem_.assignmentList.inactivateItem( false, activeAssignmentItem ) == Constants.RESULT_OK )
							activeAssignmentItem.markAsGeneralizationAssignment();
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate an active assignment" );
						}
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
		if( inactiveAssignmentItem != null )
			{
			if( inactiveAssignmentItem.isInactiveAssignment() )
				{
				if( myWordItem_.assignmentList != null )
					{
					if( myWordItem_.assignmentList.archiveItem( inactiveAssignmentItem ) == Constants.RESULT_OK )
						inactiveAssignmentItem.markAsGeneralizationAssignment();
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to archive an inactive assignment" );
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
					( currentSpecificationItem = currentSpecificationItem.nextSpecificationItemWithSameQuestionParameter() ) != null );
					}
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "I have more than one assignments at assignment level " + CommonVariables.currentAssignmentLevel );

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
					myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an undefined assignment word at assignment level " + CommonVariables.currentAssignmentLevel );
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "I have more than one assignments at assignment level " + CommonVariables.currentAssignmentLevel );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString )
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
			if( ( foundSpecificationItem = myWordItem_.firstAssignmentOrSpecificationItem( false, false, false, ( isArchivedAssignment ? isNegative : false ), isPossessive, questionParameter, specificationWordItem ) ) != null )
				{
				if( foundSpecificationItem.hasExclusiveGeneralizationCollection() )
					{
					if( inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to inactivate or archive current generalization assignments" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( foundSpecificationItem.isValueSpecification() )
						{
						if( ( specificationResult = assignSpecificationByValue( questionParameter, firstJustificationItem, specificationWordItem ) ).result != Constants.RESULT_OK )
							myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to assign the value of a specification word" );
						}
					else
						{
						if( ( specificationResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.isExclusiveSpecification(), ( foundSpecificationItem.isExclusiveSpecification() && foundSpecificationItem.isGeneralizationProperName() ), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem.generalizationWordTypeNr(), foundSpecificationItem.specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
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
 *
 *	"The Lord has made the heavens his throne;
 *	from there he rules over everything." (Psalm 103:19)
 *
 *************************************************************************/
