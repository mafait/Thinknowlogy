/*	Class:			WordAssignment
 *	Supports class:	WordItem
 *	Purpose:		To assign specifications
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

#include "GeneralizationItem.cpp"
#include "SpecificationList.cpp"

class WordAssignment
	{
	friend class WordItem;

	// Private constructed variables

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

	ResultType inactivateOrArchiveCurrentGeneralizationAssignments( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundActiveAssignmentItem = NULL;
		SpecificationItem *foundInactiveAssignmentItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundActiveGeneralizationWordItem;
		WordItem *foundInactiveGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateOrArchiveCurrentGeneralizationAssignments";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( specificationWordItem->isNounValue() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Inactivating or archiving a noun value specification isn't implemented yet" );

		if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

				if( foundActiveAssignmentItem == NULL &&
				// Skip my word for activate assignments, because this is the one to be assigned
				currentGeneralizationWordItem != myWordItem_ )
					foundActiveAssignmentItem = currentGeneralizationWordItem->firstNonQuestionAssignmentItem( true, false, false, isNegative, isPossessive, relationContextNr, specificationWordItem );

				// Allow to find an inactive assignment in my word
				if( foundInactiveAssignmentItem == NULL )
					foundInactiveAssignmentItem = currentGeneralizationWordItem->firstNonQuestionAssignmentItem( false, true, false, isNegative, isPossessive, relationContextNr, specificationWordItem );
				}
			while( ( foundActiveAssignmentItem == NULL ||
			foundInactiveAssignmentItem == NULL ) &&

			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		// Only archive an inactive assignment if an active assignment is being inactivated
		if( foundActiveAssignmentItem != NULL )
			{
			if( !foundActiveAssignmentItem->hasGeneralizationCollection() )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have found an active assignment without generalization collection number" );

			if( ( foundActiveGeneralizationWordItem = foundActiveAssignmentItem->generalizationWordItem() ) == NULL )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have found an active assignment without generalization word item" );

			if( foundActiveGeneralizationWordItem->inactivateActiveAssignment( foundActiveAssignmentItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );

			if( foundInactiveAssignmentItem != NULL )
				{
				if( !foundInactiveAssignmentItem->hasGeneralizationCollection() )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have found an inactive assignment without generalization collection number" );

				if( ( foundInactiveGeneralizationWordItem = foundInactiveAssignmentItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have found an inactive assignment without generalization word item" );

				if( foundInactiveGeneralizationWordItem->archiveInactiveAssignment( foundInactiveAssignmentItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to archive an inactive assignment" );
				}
			}

		return RESULT_OK;
		}

	ResultType inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *foundActiveAssignmentItem;
		SpecificationItem *relatedSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateAssignment";

		if( ( foundAssignmentItem = myWordItem_->firstActiveNonQuestionAssignmentItem( isAmbiguousRelationContext, isPossessive, relationContextNr, specificationWordItem ) ) == NULL )
			{
			if( relationContextNr == NO_CONTEXT_NR )
				{
				if( isExclusiveSpecification )
					{
					if( ( specificationResult = myWordItem_->findRelatedSpecification( true, true, false, isExclusiveSpecification, false, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, specificationWordItem ) ).result != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related assignment" );

					relatedSpecificationItem = specificationResult.relatedSpecificationItem;

					if( relatedSpecificationItem != NULL &&
					// Skip specification
					relatedSpecificationItem->isUserAssignment() &&

					( ( !isNegative &&
					// Only option left
					!isSelfGenerated ) ||

					!relatedSpecificationItem->isNegative() ) )
						{
						if( inactivateActiveAssignment( relatedSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate a related active assignment" );
						}
					}
				}
			else
				{
				// With relation context
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

			( !foundAssignmentItem->isUniqueUserRelation() &&
			foundAssignmentItem->isNegative() != isNegative ) )
				{
				if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate a negative active assignment" );
				}
			}

		return RESULT_OK;
		}

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString )
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
			if( inactivateAssignment( isAmbiguousRelationContext, isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an assignment" );
			}

		if( ( foundAssignmentItem = myWordItem_->firstAssignmentItem( true, true, true, isNegative, isPossessive, isSelfGenerated, questionParameter, relationContextNr, specificationWordItem ) ) == NULL )
			{
			// Didn't find a self-generated assignment. Now try to find a confirmed assignment
			if( isSelfGenerated &&
			!isSpecificationGeneralization )
				foundAssignmentItem = myWordItem_->firstAssignmentItem( true, true, true, isNegative, isPossessive, false, questionParameter, relationContextNr, specificationWordItem );
			}

		if( foundAssignmentItem != NULL )
			{
			specificationResult.foundSpecificationItem = foundAssignmentItem;

			if( foundAssignmentItem->assumptionLevel() != assumptionLevel )
				hasDifferentAssumptionLevel = true;

			// Ambiguous if assignment has different tense (time): active, inactive or archived.
			if( foundAssignmentItem->isInactiveAssignment() != isInactiveAssignment ||
			foundAssignmentItem->isArchivedAssignment() != isArchivedAssignment )
				isAmbiguous = true;
			}

		if( isAmbiguous ||
		foundAssignmentItem == NULL ||

		( !isSelfGenerated &&
		hasDifferentAssumptionLevel ) )
			{
			if( ( specificationResult = myWordItem_->createSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false, false, false, false, isEveryGeneralization, isExclusiveSpecification, isGeneralizationAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, commonVariables_->currentAssignmentLevel, assumptionLevel, commonVariables_->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL ) ).result == RESULT_OK )
				{
				if( ( createdAssignmentItem = specificationResult.createdSpecificationItem ) != NULL )
					{
					if( foundAssignmentItem != NULL )
						{
						if( hasDifferentAssumptionLevel )
							{
							if( !isQuestion )
								{
								if( myWordItem_->replaceOrDeleteSpecification( false, foundAssignmentItem, ( foundAssignmentItem->isSelfGenerated() ? createdAssignmentItem : NULL ) ) != RESULT_OK )
									return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an ambiguous assignment" );
								}
							}
						else
							{
							if( foundAssignmentItem->isActiveItem() )
								{
								if( inactivateActiveAssignment( foundAssignmentItem ) != RESULT_OK )
									return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );
								}
							else
								{
								if( foundAssignmentItem->isInactiveAssignment() &&
								// Avoid archiving inactive assignments that are created during this sentence
								!foundAssignmentItem->hasCurrentInactiveSentenceNr() )
									{
									if( archiveInactiveAssignment( foundAssignmentItem ) != RESULT_OK )
										return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to archive an inactive assignment" );
									}
								}
							}
						}
					}
				else
					{
					if( specificationWordItem == NULL )
						return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't create an assignment item with specification string \"", specificationString, "\"" );

					return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't create an assignment item with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			else
				{
				if( specificationWordItem == NULL )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to create an assignment with specification string \"", specificationString, "\"" );

				return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to create an assignment with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return specificationResult;
		}

	SpecificationResultType assignSpecificationByValue( JustificationItem *firstJustificationItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *currentAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecificationByValue";

		if( specificationWordItem == NULL )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentAssignmentItem = specificationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			do	{
				if( ( specificationResult = assignSpecification( false, currentAssignmentItem->isInactiveAssignment(), currentAssignmentItem->isArchivedAssignment(), currentAssignmentItem->isEveryGeneralization(), currentAssignmentItem->isExclusiveSpecification(), currentAssignmentItem->isGeneralizationAssignment(), currentAssignmentItem->isNegative(), currentAssignmentItem->isPartOf(), currentAssignmentItem->isPossessive(), currentAssignmentItem->isSpecificationGeneralization(), currentAssignmentItem->isUniqueUserRelation(), currentAssignmentItem->assumptionLevel(), currentAssignmentItem->prepositionParameter(), currentAssignmentItem->questionParameter(), currentAssignmentItem->generalizationWordTypeNr(), currentAssignmentItem->specificationWordTypeNr(), currentAssignmentItem->relationWordTypeNr(), currentAssignmentItem->generalizationCollectionNr(), currentAssignmentItem->specificationCollectionNr(), currentAssignmentItem->generalizationContextNr(), currentAssignmentItem->specificationContextNr(), currentAssignmentItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, currentAssignmentItem->nContextRelations(), firstJustificationItem, currentAssignmentItem->specificationWordItem(), currentAssignmentItem->specificationString() ) ).result != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return specificationResult;
		}


	protected:
		// Constructor

	WordAssignment( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordAssignment" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

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

	ResultType inactivateActiveAssignment( SpecificationItem *activeAssignmentItem )
		{
		SpecificationList *assignmentList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateActiveAssignment";

		if( activeAssignmentItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given active assignment item is undefined" );

		if( !activeAssignmentItem->isActiveAssignment() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given active assignment item isn't active" );

		if( ( assignmentList = myWordItem_->assignmentList ) == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "My assignment list isn't created yet" );

		if( assignmentList->inactivateItem( activeAssignmentItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate an active assignment" );

		activeAssignmentItem->markAsGeneralizationAssignment();

		return RESULT_OK;
		}

	ResultType archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem )
		{
		SpecificationList *assignmentList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "archiveInactiveAssignment";

		if( inactiveAssignmentItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given inactive assignment item is undefined" );

		if( !inactiveAssignmentItem->isInactiveAssignment() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given assignment item isn't inactive" );

		if( ( assignmentList = myWordItem_->assignmentList ) == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "My assignment list isn't created yet" );

		if( assignmentList->archiveItem( inactiveAssignmentItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to archive the given inactive assignment item" );

		inactiveAssignmentItem->markAsGeneralizationAssignment();

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

		if( myWordItem_->nActiveAssignments() > 1 )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I have more than one assignment at assignment level ", commonVariables_->currentAssignmentLevel );

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
				( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}

		return specificationResult;
		}

	SpecificationResultType getAssignmentWordParameter()
		{
		SpecificationResultType specificationResult;
		SpecificationItem *assignmentItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getAssignmentWordParameter";

		if( myWordItem_->nActiveAssignments() > 1 )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I have more than one assignment at assignment level ", commonVariables_->currentAssignmentLevel );

		if( ( assignmentItem = myWordItem_->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			if( ( specificationWordItem = assignmentItem->specificationWordItem() ) == NULL )
				return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I have found an undefined assignment word at assignment level ", commonVariables_->currentAssignmentLevel );

			specificationResult.assignmentParameter = specificationWordItem->wordParameter();
			}

		return specificationResult;
		}

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *foundSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( isAssignedOrClear )
			{
			if( questionParameter > NO_QUESTION_PARAMETER )
				return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "A question can only be answered, not be cleared" );

			if( inactivateCurrentAssignments() != RESULT_OK )
				return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate my current assignments" );
			}
		else
			{
			// Find the specification of the assignment
			if( ( foundSpecificationItem = myWordItem_->firstSpecificationItem( isPossessive, isSpecificationGeneralization, questionParameter, specificationWordItem ) ) != NULL )
				{
				if( foundSpecificationItem->hasExclusiveGeneralizationCollection() )
					{
					if( inactivateOrArchiveCurrentGeneralizationAssignments( isNegative, isPossessive, relationContextNr, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to inactivate or archive current generalization assignments" );
					}

				if( foundSpecificationItem->isValueSpecification() )
					{
					if( ( specificationResult = assignSpecificationByValue( firstJustificationItem, specificationWordItem ) ).result != RESULT_OK )
						return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to assign the value of a specification word" );
					}
				else
					{
					if( ( specificationResult = assignSpecification( isAmbiguousRelationContext, isInactiveAssignment, isArchivedAssignment, foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->isExclusiveSpecification(), ( foundSpecificationItem->isExclusiveSpecification() && foundSpecificationItem->isGeneralizationProperName() ), isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, foundSpecificationItem->generalizationWordTypeNr(), foundSpecificationItem->specificationWordTypeNr(), relationWordTypeNr, foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
						{
						if( specificationWordItem == NULL )
							return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to assign specification string \"", specificationString, "\"" );

						return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to assign specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			else
				{
				if( specificationWordItem == NULL )
					return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "String \"", specificationString, "\" isn't one of my specifications" );

				return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "Word \"", specificationWordItem->anyWordTypeString(), "\" isn't one of my specifications" );
				}
			}

		return specificationResult;
		}
	};

/*************************************************************************
 *	"The Lord has made the heavens his throne;
 *	from there he rules over everything." (Psalm 103:19)
 *************************************************************************/
