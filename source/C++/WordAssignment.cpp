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

#include "GeneralizationItem.cpp"
#include "SpecificationList.cpp"

class WordAssignment
	{
	friend class WordItem;

	// Private constructible variables

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;

	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType inactivateCurrentAssignments()
		{
		SpecificationItem *activeAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateCurrentAssignments";

		while( ( activeAssignmentItem = myWordItem_->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			if( inactivateActiveAssignment( activeAssignmentItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactive an active assignment item" );
			}

		return RESULT_OK;
		}

	ResultType inactivateOrArchiveCurrentGeneralizationAssignments( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundActiveAssignmentItem = NULL;
		SpecificationItem *foundInactiveAssignmentItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundActiveGeneralizationWordItem;
		WordItem *foundInactiveGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateOrArchiveCurrentGeneralizationAssignments";

		if( specificationWordItem != NULL )
			{
			if( !specificationWordItem->isNounValue() )
				{
				if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem() ) != NULL )
					{
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
							{
							if( foundActiveAssignmentItem == NULL &&
							currentGeneralizationWordItem != myWordItem_ )		// Skip my word for activate assignments, because this is the one to be assigned
								foundActiveAssignmentItem = currentGeneralizationWordItem->firstAssignmentItem( true, false, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );

							if( foundInactiveAssignmentItem == NULL )		// Allow to find an inactive assignment in my word
								foundInactiveAssignmentItem = currentGeneralizationWordItem->firstAssignmentItem( false, true, false, isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem );
							}
						else
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );
						}
					while( ( foundActiveAssignmentItem == NULL ||
					foundInactiveAssignmentItem == NULL ) &&

					( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
					}

				if( foundActiveAssignmentItem != NULL )		// Only archive an inactive assignment when an active assignment is being inactivated
					{
					if( foundActiveAssignmentItem->hasGeneralizationCollection() )
						{
						if( ( foundActiveGeneralizationWordItem = foundActiveAssignmentItem->generalizationWordItem() ) != NULL )
							{
							if( foundActiveGeneralizationWordItem->inactivateActiveAssignment( foundActiveAssignmentItem ) == RESULT_OK )
								{
								if( foundInactiveAssignmentItem != NULL )
									{
									if( foundInactiveAssignmentItem->hasGeneralizationCollection() )
										{
										if( ( foundInactiveGeneralizationWordItem = foundInactiveAssignmentItem->generalizationWordItem() ) != NULL )
											{
											if( foundInactiveGeneralizationWordItem->archiveInactiveAssignment( foundInactiveAssignmentItem ) != RESULT_OK )
												return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to archive an inactive assignment" );
											}
										else
											return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an inactive assignment without generalization word item" );
										}
									else
										return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an inactive assignment without generalization collection number" );
									}
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );
							}
						else
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an active assignment without generalization word item" );
						}
					else
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an active assignment without generalization collection number" );
					}
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Processing a noun value specification isn't implemented yet" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		return RESULT_OK;
		}

	ResultType inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *foundActiveAssignmentItem;
		SpecificationItem *relatedSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateAssignment";

		if( ( foundAssignmentItem = myWordItem_->firstActiveAssignmentItem( isAmbiguousRelationContext, isPossessive, NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( relationContextNr == NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( specificationResult = myWordItem_->findRelatedSpecification( true, true, false, false, isExclusiveSpecification, false, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, specificationWordItem, specificationString ) ).result == RESULT_OK )
						{
						relatedSpecificationItem = specificationResult.relatedSpecificationItem;

						if( relatedSpecificationItem != NULL &&
						relatedSpecificationItem->isAssignment() &&		// Skip specification

						( ( !isNegative &&
						!isSelfGenerated ) ||							// Only option left

						!relatedSpecificationItem->isNegative() ) )
							{
							if( inactivateActiveAssignment( relatedSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate a related active assignment" );
							}
						}
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related assignment" );
					}
				}
			else	// With relation context
				{
				if( ( foundActiveAssignmentItem = myWordItem_->firstAssignmentItem( true, false, false, isNegative, isPossessive, isSelfGenerated, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
					{
					if( ( foundAssignmentItem = myWordItem_->firstAssignmentItem( false, true, false, isNegative, isPossessive, isSelfGenerated, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
						{
						// First archive an inactive assignment
						if( archiveInactiveAssignment( foundAssignmentItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to archive an inactive assignment" );
						}

					// Now inactivate the found active assignment
					if( inactivateActiveAssignment( foundActiveAssignmentItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );
					}
				}
			}
		else
			{
			if( isAmbiguousRelationContext ||

			( !foundAssignmentItem->isUniqueRelation() &&
			foundAssignmentItem->isNegative() != isNegative ) )
				{
				if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate a negative active assignment" );
				}
			}

		return RESULT_OK;
		}

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		bool hasDifferentAssumptionLevel = false;
		bool isAmbiguous = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		SpecificationItem *createdAssignmentItem;
		SpecificationItem *foundAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( !isQuestion )
			{
			if( inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) != RESULT_OK )
				myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an assignment" );
			}

		if( commonVariables_->result == RESULT_OK )
			{
			if( ( foundAssignmentItem = myWordItem_->firstAssignmentItem( true, true, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == NULL )
				{
				if( isSelfGenerated )	// Found no self-generated assignment. Now try to find a confirmed assignment.
					foundAssignmentItem = myWordItem_->firstAssignmentItem( true, true, true, isNegative, isPossessive, false, questionParameter, relationContextNr, specificationWordItem );
				}

			if( foundAssignmentItem != NULL )
				{
				if( foundAssignmentItem->assumptionLevel() != assumptionLevel )
					hasDifferentAssumptionLevel = true;

				if( foundAssignmentItem->isInactiveAssignment() != isInactiveAssignment ||	// Ambiguous when has different tense (time)
				foundAssignmentItem->isArchivedAssignment() != isArchivedAssignment )		// (active, inactive or archive)
					isAmbiguous = true;
				}

			if( isAmbiguous ||
			foundAssignmentItem == NULL ||

			( !isSelfGenerated &&
			hasDifferentAssumptionLevel ) )
				{
				if( ( specificationResult = myWordItem_->createSpecification( true, isInactiveAssignment, isArchivedAssignment, false, false, false, false, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, false, commonVariables_->currentAssignmentLevel, assumptionLevel, commonVariables_->currentGrammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL ) ).result == RESULT_OK )
					{
					if( ( createdAssignmentItem = specificationResult.createdSpecificationItem ) != NULL )
						{
						if( foundAssignmentItem != NULL )
							{
							if( hasDifferentAssumptionLevel )
								{
								if( !isQuestion )
									{
									if( myWordItem_->replaceOrDeleteSpecification( foundAssignmentItem, ( foundAssignmentItem->isSelfGenerated() ? createdAssignmentItem : NULL ) ) != RESULT_OK )
										myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an ambiguous assignment" );
									}
								}
							else
								{
								if( foundAssignmentItem->isActiveItem() )
									{
									if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
										myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );
									}
								else
									{
									if( foundAssignmentItem->isInactiveAssignment() &&
									// Avoid archiving inactive assignments that are created during this sentence
									!foundAssignmentItem->hasCurrentInactiveSentenceNr() )
										{
										if( archiveInactiveAssignment( foundAssignmentItem ) != RESULT_OK )
											myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to archive an inactive assignment" );
										}
									}
								}
							}
						}
					else
						{
						if( specificationWordItem == NULL )
							myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't create an assignment item with specification string \"", specificationString, "\"" );
						else
							myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't create an assignment item with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					{
					if( specificationWordItem == NULL )
						myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create an assignment with specification string \"", specificationString, "\"" );
					else
						myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create an assignment with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType assignSpecificationByValue( unsigned short questionParameter, JustificationItem *firstJustificationItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *currentAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecificationByValue";

		if( specificationWordItem != NULL )
			{
			if( ( currentAssignmentItem = specificationWordItem->firstActiveAssignmentItem( questionParameter ) ) != NULL )
				{
				do	{
					if( ( specificationResult = assignSpecification( false, currentAssignmentItem->isInactiveAssignment(), currentAssignmentItem->isArchivedAssignment(), currentAssignmentItem->isEveryGeneralization(), currentAssignmentItem->isExclusiveSpecification(), currentAssignmentItem->isGeneralizationAssignment(), currentAssignmentItem->isNegative(), currentAssignmentItem->isPartOf(), currentAssignmentItem->isPossessive(), currentAssignmentItem->isSpecificationGeneralization(), currentAssignmentItem->isUniqueRelation(), currentAssignmentItem->assumptionLevel(), currentAssignmentItem->prepositionParameter(), currentAssignmentItem->questionParameter(), currentAssignmentItem->generalizationWordTypeNr(), currentAssignmentItem->specificationWordTypeNr(), currentAssignmentItem->relationWordTypeNr(), currentAssignmentItem->generalizationCollectionNr(), currentAssignmentItem->specificationCollectionNr(), currentAssignmentItem->generalizationContextNr(), currentAssignmentItem->specificationContextNr(), currentAssignmentItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, currentAssignmentItem->nContextRelations(), firstJustificationItem, currentAssignmentItem->specificationWordItem(), currentAssignmentItem->specificationString() ) ).result != RESULT_OK )
						myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				while( commonVariables_->result == RESULT_OK &&
				( currentAssignmentItem = currentAssignmentItem->nextSpecificationItemWithSameQuestionParameter() ) != NULL );
				}
			}
		else
			myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}


	protected:
		// Constructor / deconstructor

	WordAssignment( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordAssignment" );

		if( commonVariables_ != NULL )
			{
		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType createNewAssignmentLevel()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewAssignmentLevel";

		if( !myWordItem_->isAdminWord() )
			{
			if( myWordItem_->assignmentList != NULL )
				{
				if( myWordItem_->assignmentList->createNewAssignmentLevel() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create assignment level ", ( commonVariables_->currentAssignmentLevel + 1 ) );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The admin word item cannot have assignments" );

		return RESULT_OK;
		}

	ResultType inactivateActiveAssignment( SpecificationItem *activeAssignmentItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *replacingAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateActiveAssignment";

		if( activeAssignmentItem != NULL )
			{
			if( activeAssignmentItem->isActiveAssignment() )
				{
				if( myWordItem_->assignmentList != NULL )
					{
					if( activeAssignmentItem->hasInactiveSentenceNr() )		// Has been inactive before
						{
						if( ( specificationResult = myWordItem_->copySpecification( true, false, activeAssignmentItem->isAnsweredQuestion(), activeAssignmentItem->isExclusiveSpecification(), activeAssignmentItem->assignmentLevel(), activeAssignmentItem->generalizationCollectionNr(), activeAssignmentItem->specificationCollectionNr(), activeAssignmentItem->relationContextNr(), activeAssignmentItem->firstJustificationItem(), activeAssignmentItem ) ).result == RESULT_OK )
							{
							if( ( replacingAssignmentItem = specificationResult.createdSpecificationItem ) != NULL )
								{
								if( myWordItem_->replaceOrDeleteSpecification( activeAssignmentItem, replacingAssignmentItem ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the given active assignment" );
								}
							else
								return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The last created assignment item is undefined" );
							}
						else
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to copy an active assignment item to an inactive assignment item" );
						}
					else
						{
						if( myWordItem_->assignmentList->inactivateItem( false, activeAssignmentItem ) == RESULT_OK )
							activeAssignmentItem->markAsGeneralizationAssignment();
						else
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );
						}
					}
				else
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The assignment list isn't created yet" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given assignment item isn't active" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given active assignment item is undefined" );

		return RESULT_OK;
		}

	ResultType archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "archiveInactiveAssignment";

		if( inactiveAssignmentItem != NULL )
			{
			if( inactiveAssignmentItem->isInactiveAssignment() )
				{
				if( myWordItem_->assignmentList != NULL )
					{
					if( myWordItem_->assignmentList->archiveItem( inactiveAssignmentItem ) == RESULT_OK )
						inactiveAssignmentItem->markAsGeneralizationAssignment();
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to archive an inactive assignment" );
					}
				else
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The assignment list isn't created yet" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given assignment item isn't inactive" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given inactive assignment item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType getAssignmentOrderNr()
		{
		SpecificationResultType specificationResult;
		unsigned short specificationNr = 0;
		SpecificationItem *orderAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		WordItem *assignmentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getAssignmentOrderNr";

		if( myWordItem_->nActiveAssignments() <= 1 )
			{
			if( ( orderAssignmentItem = myWordItem_->firstNonQuestionActiveAssignmentItem() ) != NULL )
				{
				if( ( assignmentWordItem = orderAssignmentItem->specificationWordItem() ) != NULL &&
				( currentSpecificationItem = myWordItem_->firstNonQuestionSpecificationItem() ) != NULL )
					{
					do	{
						specificationNr++;

						if( currentSpecificationItem->specificationWordItem() == assignmentWordItem )
							specificationResult.assignmentOrderNr = specificationNr;
						}
					while( specificationResult.assignmentOrderNr == NO_ORDER_NR &&
					( currentSpecificationItem = currentSpecificationItem->nextSpecificationItemWithSameQuestionParameter() ) != NULL );
					}
				}
			}
		else
			myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have more than one assignments at assignment level ", commonVariables_->currentAssignmentLevel );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType getAssignmentWordParameter()
		{
		SpecificationResultType specificationResult;
		SpecificationItem *assignmentItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getAssignmentWordParameter";

		if( myWordItem_->nActiveAssignments() <= 1 )
			{
			if( ( assignmentItem = myWordItem_->firstNonQuestionActiveAssignmentItem() ) != NULL )
				{
				if( ( specificationWordItem = assignmentItem->specificationWordItem() ) != NULL )
					specificationResult.assignmentParameter = specificationWordItem->wordParameter();
				else
					myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an undefined assignment word at assignment level ", commonVariables_->currentAssignmentLevel );
				}
			}
		else
			myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have more than one assignments at assignment level ", commonVariables_->currentAssignmentLevel );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *foundSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( isAssignedOrClear )
			{
			if( questionParameter == NO_QUESTION_PARAMETER )
				{
				if( inactivateCurrentAssignments() != RESULT_OK )
					myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate my current assignments" );
				}
			else
				myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "A question can only be answered, not be cleared" );
			}
		else
			{
			// Find the specification of the assignment
			if( ( foundSpecificationItem = myWordItem_->firstAssignmentOrSpecificationItem( false, false, false, ( isArchivedAssignment ? isNegative : false ), isPossessive, questionParameter, specificationWordItem ) ) != NULL )
				{
				if( foundSpecificationItem->hasExclusiveGeneralizationCollection() )
					{
					if( inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, questionParameter, relationContextNr, specificationWordItem ) != RESULT_OK )
						myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate or archive current generalization assignments" );
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( foundSpecificationItem->isValueSpecification() )
						{
						if( ( specificationResult = assignSpecificationByValue( questionParameter, firstJustificationItem, specificationWordItem ) ).result != RESULT_OK )
							myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to assign the value of a specification word" );
						}
					else
						{
						if( ( specificationResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->isExclusiveSpecification(), ( foundSpecificationItem->isExclusiveSpecification() && foundSpecificationItem->isGeneralizationProperName() ), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem->generalizationWordTypeNr(), foundSpecificationItem->specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
							{
							if( specificationWordItem == NULL )
								myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to assign specification string \"", specificationString, "\"" );
							else
								myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			else
				{
				if( specificationWordItem == NULL )
					myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "String \"", specificationString, "\" isn't one of my specifications" );
				else
					myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Word \"", specificationWordItem->anyWordTypeString(), "\" isn't one of my specifications" );
				}
			}

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}
	};

/*************************************************************************
 *
 *	"The Lord has made the heavens his throne;
 *	from there he rules over everything." (Psalm 103:19)
 *
 *************************************************************************/
