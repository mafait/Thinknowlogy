/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
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

#include "AdminItem.h"
#include "CollectionResultType.cpp"
#include "ContextItem.cpp"
#include "ReadItem.cpp"
#include "SelectionItem.cpp"

class AdminSpecification
	{
	friend class AdminItem;

	// Private constructed variables

	bool isArchivedAssignment_;

	unsigned short doneSpecificationWordOrderNr_;
	unsigned short linkedGeneralizationWordTypeNr_;

	unsigned int userRelationContextNr_;

	SpecificationItem *simpleUserSpecificationItem_;
	SpecificationItem *userSpecificationItem_;

	WordItem *linkedGeneralizationWordItem_;

	char *previousSpecificationString_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isAuthorizationRequiredForChanges( WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		return( generalizationWordItem != NULL &&
				specificationWordItem != NULL &&
				adminItem_->isSystemStartingUp() &&

				( generalizationWordItem->isAuthorizationRequiredForChanges() ||
				specificationWordItem->isAuthorizationRequiredForChanges() ) );
		}

	ResultType addUserSpecificationWithRelation( bool isAction, bool isAssignedOrClear, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isCheckingExistingUserSpecification;
		bool isFirstComparisonPart = ( selectionListNr == ADMIN_CONDITION_LIST );
		bool isStop = false;
		unsigned short relationWordTypeNr;
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		ReadItem *currentRelationReadItem = startRelationReadItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *mostRecentContextWord;
		WordItem *relationWordItem;
		WordItem *previousRelationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecificationWithRelation";

		if( startRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start relation read item is undefined" );

		if( endRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given end relation read item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isCheckingExistingUserSpecification = ( isPossessive &&
												nContextRelations == 0 &&
												!specificationWordItem->isNounWordSpanishAmbiguous() );

		do	{
			if( currentRelationReadItem->isPreposition() )
				prepositionParameter = currentRelationReadItem->wordParameter();
			else
				{
				if( currentRelationReadItem->isRelationWord() )
					{
					if( ( relationWordItem = currentRelationReadItem->readWordItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined read word" );

					if( specificationWordItem == relationWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "The relation word is the same as the specification word" );

					if( generalizationWordItem == relationWordItem )
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						relationWordTypeNr = currentRelationReadItem->wordTypeNr();

						if( previousRelationWordItem == NULL &&
						commonVariables_->nUserRelationWords == 1 &&
						( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != NULL )
							{
							if( foundSpecificationItem->hasRelationContext() &&
							( mostRecentContextWord = mostRecentContextWordInAllWords( foundSpecificationItem->relationContextNr() ) ) != NULL )
								{
								if( mostRecentContextWord != relationWordItem )
									previousRelationWordItem = mostRecentContextWord;
								}
							}

						if( previousRelationWordItem != NULL )
							{
							if( adminItem_->collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );
							}

						if( selectionListNr == NO_LIST_NR )
							{
							existingSpecificationItem = ( isCheckingExistingUserSpecification ? generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) : NULL );

							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( existingSpecificationItem == NULL ? NO_COLLECTION_NR : existingSpecificationItem->specificationCollectionNr() ), generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NULL, generalizationWordItem, specificationWordItem, relationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( adminItem_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem->isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a selection part having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

							isFirstComparisonPart = false;
							}

						previousRelationWordItem = relationWordItem;
						}
					}
				}
			}
		while( !isStop &&
		!commonVariables_->hasDisplayedWarning &&
		currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	ResultType processJustification( bool hasFoundRelationContext, bool hasRelationWord, bool isUserGeneralizationWord, unsigned short justificationTypeNr, JustificationItem *createdOrFoundJustificationItem, SpecificationItem *foundSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool isAttachingJustification = true;
		bool isDeletingCreatedJustification = false;
		bool isRecalculatingAssumptionsAfterAttachment = false;
		bool isRemovingPreviousJustifications = false;
		bool isSpecificationWordSpanishAmbiguous;
		JustificationResultType justificationResult;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem;
		JustificationItem *existingJustificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processJustification";

		if( createdOrFoundJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created or found justification item is undefined" );

		if( foundSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		isSpecificationWordSpanishAmbiguous = foundSpecificationItem->isSpecificationWordSpanishAmbiguous();
		secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem();

		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				if( relationWordItem != NULL )
					{
					if( secondarySpecificationItem->isSelfGeneratedAssumption() )
						{
						// Skip if justification with self-generated assumption is created,
						// while justification with self-generated conclusion already exists
						if( relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), false, secondarySpecificationItem->specificationWordItem(), generalizationWordItem ) != NULL )
							isDeletingCreatedJustification = true;
						}
					else
						{
						if( secondarySpecificationItem->isSelfGeneratedConclusion() &&
						( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), true, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL )
							{
							if( ( existingJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != NULL )
								{
								if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an existing justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								isAttachingJustification = false;
								}
							}
						else
							{
							if( isUserGeneralizationWord &&
							!foundSpecificationItem->isOlderItem() &&
							foundSpecificationItem->isHiddenSpanishSpecification() &&
							createdOrFoundJustificationItem->hasFeminineOrMasculineProperNameEnding() )
								isAttachingJustification = false;
							}
						}
					}

				break;

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( hasFoundRelationContext &&
				hasRelationWord &&
				primarySpecificationItem != NULL )
					{
					// Confirmation
					if( secondarySpecificationItem->isUserSpecification() )
						{
						if( isUserGeneralizationWord )
							{
							if( ( existingJustificationItem = generalizationWordItem->olderComplexJustificationItem( true, justificationTypeNr, secondarySpecificationItem->specificationCollectionNr(), primarySpecificationItem ) ) == NULL )
								{
								if( ( obsoleteJustificationItem = generalizationWordItem->possessiveReversibleAssumptionJustificationItem( relationWordItem, secondarySpecificationWordItem ) ) == NULL )
									{
									// Skip if justification already exists as an opposite possessive conditional specification assumption
									if( secondarySpecificationItem->isGeneralizationProperName() &&
									generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) != NULL )
										isDeletingCreatedJustification = true;
									}
								else
									{
									if( ( justificationResult = generalizationWordItem->addJustification( false, false, justificationTypeNr, obsoleteJustificationItem->orderNr, obsoleteJustificationItem->originalSentenceNr(), NULL, NULL, secondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification without primary specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find or create a justification without primary specification" );

									if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								{
								// Replace existing justification by the created justification
								if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an existing justification by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								isAttachingJustification = false;
								}
							}
						}
					else
						{
						if( !isUserGeneralizationWord &&
						!isSpecificationWordSpanishAmbiguous &&
						!foundSpecificationItem->isOlderItem() &&
						primarySpecificationItem->isOlderItem() )
							isRecalculatingAssumptionsAfterAttachment = true;
						}
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( !hasRelationWord &&
				primarySpecificationItem != NULL &&
				( existingJustificationItem = foundSpecificationItem->differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) != NULL )
					{
					isAttachingJustification = false;

					if( primarySpecificationItem->assumptionLevel() < existingJustificationItem->primarySpecificationAssumptionLevel() )
						{
						// Replace less certain existing justification by the created justification
						if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						isDeletingCreatedJustification = true;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				if( !hasRelationWord &&
				primarySpecificationItem != NULL &&
				( specificationSubstitutionPartOfAssumptionJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != NULL )
					{
					if( specificationSubstitutionPartOfAssumptionJustificationItem->hasHiddenPrimarySpecification() )
						{
						if( specificationSubstitutionPartOfAssumptionJustificationItem->changePrimarySpecification( primarySpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a hidden primary specification of the attached justification item of the found specification substitution 'part of' assumption justification item" );

						isDeletingCreatedJustification = true;
						}
					}

				break;

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				if( ( !isSpecificationWordSpanishAmbiguous ||
				foundSpecificationItem->isConcludedAssumption() ) &&

				foundSpecificationItem->hasOnlyOneRelationWord() )
					isRemovingPreviousJustifications = true;

				break;

			case JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				if( anotherSecondarySpecificationItem != NULL &&
				foundSpecificationItem->isConcludedAssumption() &&
				( obsoleteJustificationItem = generalizationWordItem->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, secondarySpecificationItem ) ) != NULL )
					{
					if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace negative assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					isAttachingJustification = false;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Generalization word has adjusted question
				if( foundSpecificationItem->isOlderItem() &&
				generalizationWordItem == adminItem_->adjustedQuestionWordItem() )
					{
					// Remove obsolete justifications from adjusted compound question
					isRemovingPreviousJustifications = true;
					strcpy( foundSpecificationItem->lastWrittenSentenceString, EMPTY_STRING );
					}
			}

		if( isDeletingCreatedJustification )
			{
			if( generalizationWordItem->replaceOrDeleteJustification( createdOrFoundJustificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}
		else
			{
			if( isRemovingPreviousJustifications )
				{
				if( generalizationWordItem->copyAndReplaceSpecificationItem( foundSpecificationItem->isAnsweredQuestion(), foundSpecificationItem->isExclusiveSpecification(), foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy and replace a specification with a different first justification item" );
				}
			else
				{
				if( isAttachingJustification )
					{
					// Attach created justification to the found specification
					if( generalizationWordItem->attachJustificationInWord( createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( isRecalculatingAssumptionsAfterAttachment )
						{
						if( generalizationWordItem->recalculateAssumptions() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	WordItem *mostRecentContextWordInAllWords( unsigned int contextNr )
		{
		ContextItem *currentContextItem;
		ContextItem *mostRecentContextItem = NULL;
		WordItem *currentContextWordItem;
		WordItem *mostRecentWordItem = NULL;

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				currentContextItem = currentContextWordItem->contextItem( contextNr );

				if( currentContextItem != NULL &&

				( mostRecentContextItem == NULL ||
				currentContextItem->isMoreRecent( mostRecentContextItem ) ) )
					{
					mostRecentWordItem = currentContextWordItem;
					mostRecentContextItem = currentContextItem;
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return mostRecentWordItem;
		}


	protected:
	// Constructor

	AdminSpecification( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

		userRelationContextNr_ = NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = NULL;
		userSpecificationItem_ = NULL;

		linkedGeneralizationWordItem_ = NULL;

		previousSpecificationString_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminSpecification" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected assignment functions

	ResultType assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSelectionSpecification";

		if( assignmentSelectionItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem->isAssignedOrClear(), assignmentSelectionItem->isInactiveAssignment(), assignmentSelectionItem->isArchivedAssignment(), assignmentSelectionItem->isNegative(), false, assignmentSelectionItem->isPossessive(), assignmentSelectionItem->isSpecificationGeneralization(), assignmentSelectionItem->isUniqueUserRelation(), assignmentSelectionItem->assumptionLevel(), assignmentSelectionItem->prepositionParameter(), NO_QUESTION_PARAMETER, assignmentSelectionItem->relationWordTypeNr(), assignmentSelectionItem->generalizationContextNr(), assignmentSelectionItem->specificationContextNr(), assignmentSelectionItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, assignmentSelectionItem->nContextRelations(), NULL, assignmentSelectionItem->generalizationWordItem(), assignmentSelectionItem->specificationWordItem(), assignmentSelectionItem->specificationString() ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a specification" );

		return RESULT_OK;
		}

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			{
			if( ( specificationResult = adminItem_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
				return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to assign generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", ( specificationWordItem == NULL ? specificationString : specificationWordItem->anyWordTypeString() ), "\" with authorization" );
			}
		else
			{
			if( ( specificationResult = generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL ) ).result != RESULT_OK )
				return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to assign generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", ( specificationWordItem == NULL ? specificationString : specificationWordItem->anyWordTypeString() ), "\"" );
			}

		return specificationResult;
		}


	// Protected specification functions

	void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
		linkedGeneralizationWordItem_ = NULL;
		}

	void initializeAdminSpecificationVariables()
		{
		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

		userRelationContextNr_ = NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = NULL;
		userSpecificationItem_ = NULL;

		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordItem_ = NULL;
		}

	ResultType addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		CollectionResultType collectionResult;
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		bool isGeneralizationProperName;
		bool isGeneralizationReasoningWordType;
		bool isRelationWord;
		bool isSpecificationWord;
		bool hasAddedSpecification = false;
		bool hasRelationWord = ( startRelationReadItem != NULL );
		bool hasLinkedPossessiveDeterminer = false;
		bool isAmbiguousRelationContext = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelection = ( selectionListNr != NO_LIST_NR );
		bool isSkippingThisGeneralizationPart = false;
		bool isSpecificationWordAlreadyAssignedByComparison = false;
		bool isValueSpecificationWord = false;
		bool isWaitingForRelation = false;
		bool isWaitingForText = false;
		unsigned short firstSpecificationWordOrderNr;
		unsigned short generalizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short valueGeneralizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short linkedSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short currentSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short previousSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned int nContextRelations = 0;
		ReadItem *currentReadItem;
		ReadItem *firstRelationReadItem;
		ReadItem *lastGeneralizationReadItem;
		WordItem *generalizationWordItem;
		WordItem *readWordItem;
		WordItem *relationWordItem;
		WordItem *tempSpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *currentSpecificationWordItem = NULL;
		WordItem *linkedSpecificationWordItem = NULL;
		WordItem *previousSpecificationWordItem = NULL;
		WordItem *valueGeneralizationWordItem = NULL;
		char *specificationString = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecifications";

		previousSpecificationString_ = NULL;

		if( generalizationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given generalization read item is undefined" );

		if( ( generalizationWordItem = generalizationReadItem->readWordItem() ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given generalization read item has no read word" );

		if( ( currentReadItem = startSpecificationReadItem ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given start specification read item is undefined" );

		if( endSpecificationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given end specification read item is undefined" );

		isGeneralizationProperName = generalizationReadItem->isProperName();
		generalizationWordTypeNr = generalizationReadItem->wordTypeNr();
		isGeneralizationReasoningWordType = adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr );

		lastGeneralizationReadItem = generalizationReadItem->nextReadItem();

		while( lastGeneralizationReadItem != NULL &&
		!lastGeneralizationReadItem->isGeneralizationWord() )
			{
			if( lastGeneralizationReadItem->isPossessiveDeterminer() )
				hasLinkedPossessiveDeterminer = true;

			lastGeneralizationReadItem = lastGeneralizationReadItem->nextReadItem();
			}

		firstSpecificationWordOrderNr = ( lastGeneralizationReadItem == NULL ? endSpecificationReadItem->wordOrderNr() : (unsigned short)( lastGeneralizationReadItem->wordOrderNr() + 1 ) );

		do	{
			isRelationWord = currentReadItem->isRelationWord();
			isSpecificationWord = currentReadItem->isSpecificationWord();

			if( isSkippingThisGeneralizationPart )
				{
				if( isWaitingForRelation )
					{
					if( isRelationWord ||
					currentReadItem->isText() )
						{
						isSkippingThisGeneralizationPart = false;
						isWaitingForRelation = false;
						isWaitingForText = false;
						}
					}
				else
					{
					if( hasRelationWord &&
					generalizationReadItem->wordOrderNr() < currentReadItem->wordOrderNr() )
						isSkippingThisGeneralizationPart = false;
					}
				}
			else
				{
				if( hasRelationWord &&
				doneSpecificationWordOrderNr_ > currentReadItem->wordOrderNr() )
					isSkippingThisGeneralizationPart = true;
				else
					{
					if( currentReadItem->isGeneralizationWord() )
						{
						if( hasRelationWord ||
						isSelection )
							{
							isSkippingThisGeneralizationPart = true;
							isWaitingForRelation = true;
							}
						}
					else
						{
						if( isSpecificationWord &&
						generalizationWordItem->isAdjectiveComparison() &&
						( tempSpecificationWordItem = currentReadItem->readWordItem() ) != NULL )
							{
							// Skip head and tail in the comparison
							if( !tempSpecificationWordItem->isNounHead() &&
							!tempSpecificationWordItem->isNounTail() )
								{
								isWaitingForText = true;
								isSpecificationWordAlreadyAssignedByComparison = true;
								currentSpecificationWordTypeNr = currentReadItem->wordTypeNr();
								currentSpecificationWordItem = tempSpecificationWordItem;
								}
							}
						}
					}
				}

			if( !isWaitingForText &&
			!isSkippingThisGeneralizationPart &&
			!currentReadItem->isNegative() &&
			// Skip numeral 'both'. Typically for English: in both ... and ...
			!currentReadItem->isNumeralBoth() )
				{
				readWordItem = currentReadItem->readWordItem();
				specificationString = NULL;

				if( currentReadItem->isText() )
					specificationString = currentReadItem->readString;

				if( isSpecificationWordAlreadyAssignedByComparison )
					isSpecificationWordAlreadyAssignedByComparison = false;
				else
					{
					currentSpecificationWordTypeNr = ( isQuestion && currentReadItem->isNoun() ? WORD_TYPE_NOUN_SINGULAR : currentReadItem->wordTypeNr() );
					currentSpecificationWordItem = readWordItem;
					}

				if( isPossessive &&
				currentReadItem->isNumeral() )
					sscanf( currentReadItem->readWordTypeString(), "%u", &nContextRelations );
				else
					{
					if( !isSelection &&
					isSpecificationWord &&
					startRelationReadItem != NULL &&
					!generalizationWordItem->isVerbImperativeDisplayOrLoginOrRead() )
						{
						if( ( contextResult = adminItem_->getRelationContext( isArchivedAssignment, isPossessive, isQuestion, true, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, startRelationReadItem ) ).result != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to get the multiple entry relation context" );

						isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
						userRelationContextNr_ = contextResult.contextNr;
						}

					if( currentReadItem->isNumeral() ||
					currentReadItem->isText() ||

					( isSpecificationWord &&
					!currentReadItem->isPreposition() ) )
						{
						if( isValueSpecificationWord )
							{
							hasAddedSpecification = true;

							if( hasRelationWord )
								return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Adding a value specification with relation isn't implemented" );

							if( isSelection )
								{
								if( adminItem_->createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a value selection item" );
								}

							// Value, but no relation
							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a value specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( currentReadItem->isNounValue() )
								{
								isValueSpecificationWord = true;
								valueGeneralizationWordTypeNr = generalizationWordTypeNr;
								valueGeneralizationWordItem = generalizationWordItem;
								}
							else
								{
								if( isSelection )
									{
									hasAddedSpecification = true;

									if( hasRelationWord )
										{
										// Selection, no value, with relation
										if( addUserSpecificationWithRelation( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, selectionLevel, selectionListNr, imperativeVerbParameter, userAssumptionLevel, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else
										{
										// Selection, no value, no relation
										if( adminItem_->createSelectionPart( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part" );
										}
									}
								else	// No selection
									{
									if( previousSpecificationString_ == NULL &&
									previousSpecificationWordItem != NULL &&
									currentSpecificationWordItem != NULL &&
									currentReadItem->isMatchingReadWordTypeNr( previousSpecificationWordTypeNr ) )
										{
										if( ( collectionResult = adminItem_->collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) ).result != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to collect specification words" );

										if( collectionResult.isAmbiguousCollection )
											{
											if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, NULL, generalizationWordItem, previousSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a specification with an ambiguous specification word to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										}

									if( currentSpecificationWordTypeNr != WORD_TYPE_PREPOSITION )
										{
										previousSpecificationWordItem = currentSpecificationWordItem;
										previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
										previousSpecificationString_ = specificationString;
										}
									}
								}

							if( !isSelection ||
							!currentReadItem->isAdjectiveAssignedOrEmpty() )
								{
								doneSpecificationWordOrderNr_ = currentReadItem->wordOrderNr();
								linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
								linkedSpecificationWordItem = currentSpecificationWordItem;

								if( !currentReadItem->isNounValue() &&
								currentSpecificationWordTypeNr != WORD_TYPE_ADVERB &&
								currentReadItem->wordOrderNr() <= firstSpecificationWordOrderNr )
									{
									hasAddedSpecification = true;

									// Login is already created by during startup
									if( !generalizationWordItem->isVerbImperativeDisplayOrLoginOrRead() )
										{
										if( hasRelationWord )
											{
											// No selection, no value, with relation
											if( addUserSpecificationWithRelation( false, false, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, NO_SELECTION_LEVEL, NO_LIST_NR, imperativeVerbParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a user specification with a relation to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											// No selection, no value, no relation
											if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a specification without relation to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}

										if( isGeneralizationReasoningWordType &&
										!isNegative &&
										!isPartOf &&
										!isQuestion &&
										currentSpecificationWordItem != NULL &&
										adminItem_->isNounWordType( currentSpecificationWordTypeNr ) )
											{
											// Generalizations with noun specifications - without relations
											if( !isPossessive )
												{
												if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, true, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( !commonVariables_->hasDisplayedWarning )
													{
													if( isGeneralizationProperName )
														{
														if( !isExclusiveSpecification &&
														generalizationWordItem->isUserGeneralizationWord &&
														adminItem_->isSpanishCurrentLanguage() )
															{
															if( adminItem_->drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														{
														// Definition specification
														if( ( specificationResult = adminItem_->drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														compoundGeneralizationWordItem = specificationResult.compoundGeneralizationWordItem;
														}
													}
												}

											if( isGeneralizationProperName &&
											!commonVariables_->hasDisplayedWarning &&
											commonVariables_->nUserRelationWords == 1 &&
											startRelationReadItem != NULL &&
											( firstRelationReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL )
												{
												if( ( relationWordItem = firstRelationReadItem->readWordItem() ) != NULL )
													{
													if( adminItem_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, firstRelationReadItem->wordTypeNr(), specificationContextNr, userRelationContextNr_, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
														return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with one relation word, from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										}
									}
								}
							}
						}
					else
						{
						if( isPossessive &&
						currentReadItem->isArticle() )
							nContextRelations = 0;
						}

					if( !commonVariables_->hasDisplayedWarning &&
					currentSpecificationWordItem != NULL )
						{
						if( !isQuestion &&
						linkedSpecificationWordItem != NULL &&

						( isRelationWord ||
						isSpecificationWord ) &&

						( !hasRelationWord ||
						linkedSpecificationWordItem != currentSpecificationWordItem ) )
							{
							if( isGeneralizationProperName &&
							!isValueSpecificationWord &&
							// Skip adjectives
							adminItem_->isNounWordType( linkedSpecificationWordTypeNr ) )
								{
								if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : NULL ) ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );
								}

							if( isRelationWord ||

							( isSpecificationWord &&
							hasLinkedPossessiveDeterminer ) )
								{
								if( !commonVariables_->hasDisplayedWarning &&
								// Linked specification
								currentSpecificationWordItem == linkedSpecificationWordItem )
									{
									if( linkedGeneralizationWordItem_ != NULL )
										return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Linked word \"", linkedGeneralizationWordItem_->anyWordTypeString(), "\" is already assigned" );

									linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
									linkedGeneralizationWordItem_ = generalizationWordItem;
									}

								if( !commonVariables_->hasDisplayedWarning &&
								isGeneralizationProperName &&
								!isQuestion &&
								!isSpecificationGeneralization &&
								userRelationContextNr_ > NO_CONTEXT_NR &&
								commonVariables_->nUserRelationWords > 1 )
									{
									if( adminItem_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								{
								if( !isSelection &&
								currentReadItem->isAdjectiveAssigned() )
									{
									if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem->readWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, currentReadItem->wordOrderNr(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
									}
								}
							}
						}
					}
				}
			}
		while( !commonVariables_->hasDisplayedWarning &&
		currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		if( !hasAddedSpecification )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperName )
			{
			if( adminItem_->recalculateAssumptionsInAllTouchedWords() != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumptions in all words" );
			}

		return RESULT_OK;
		}

	SpecificationResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNameEnding, bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkippingAdditionalConclusionOrAskQuestion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool isUserGeneralizationWord;
		bool hasFoundRelationContext = false;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isAssumption = adminItem_->isAssumption( assumptionJustificationTypeNr );
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		unsigned short justificationTypeNr = assumptionJustificationTypeNr;
		unsigned int createdRelationContextNr;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *foundJustificationItem = NULL;
		JustificationItem *createdOrFoundJustificationItem = NULL;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSelfGeneratedSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( primarySpecificationItem != NULL &&
		primarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem != NULL &&
		anotherPrimarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );

		if( secondarySpecificationItem != NULL &&
		secondarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( anotherSecondarySpecificationItem != NULL &&
		anotherSecondarySpecificationItem->isHiddenSpanishSpecification() )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		// Check assumption
		if( isAssumption &&
		!hasFeminineOrMasculineProperNameEnding &&
		adminItem_->assumptionGrade( ( anotherPrimarySpecificationItem != NULL ), hasFeminineOrMasculineProperNameEnding, ( primarySpecificationItem != NULL && primarySpecificationItem->isGeneralizationProperName() && primarySpecificationItem->isPossessive() ), ( primarySpecificationItem != NULL && primarySpecificationItem->isQuestion() ), assumptionJustificationTypeNr ) == 0 &&

		( primarySpecificationItem == NULL ||

		( !primarySpecificationItem->hasAssumptionLevel() &&
		!primarySpecificationItem->isSelfGeneratedAssumption() ) ) &&

		( anotherPrimarySpecificationItem == NULL ||

		( !anotherPrimarySpecificationItem->hasAssumptionLevel() &&
		!anotherPrimarySpecificationItem->isSelfGeneratedAssumption() ) ) &&

		( secondarySpecificationItem == NULL ||

		( !secondarySpecificationItem->hasAssumptionLevel() &&
		!secondarySpecificationItem->isSelfGeneratedAssumption() ) ) )
			{
			// Conclusion
			if( !adminItem_->isConclusion( conclusionJustificationTypeNr ) )
				return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given conclusion justification type isn't a conclusion" );

			isAssumption = false;
			justificationTypeNr = conclusionJustificationTypeNr;
			}

		if( ( justificationResult = generalizationWordItem->addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, justificationTypeNr, NO_ORDER_NR, commonVariables_->currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add a justification" );

		createdJustificationItem = justificationResult.createdJustificationItem;
		foundJustificationItem = justificationResult.foundJustificationItem;

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == NULL ? foundJustificationItem : createdJustificationItem ) ) == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "I couldn't find or create a justification" );

		if( ( specificationResult = addSpecification( isAssignment, false, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION || justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION ), false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to create a specification" );

		if( !commonVariables_->hasDisplayedWarning )
			{
			createdSpecificationItem = specificationResult.createdSpecificationItem;

			if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
				hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

			if( createdSpecificationItem == NULL )
				{
				if( foundSpecificationItem == NULL )
					return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "I couldn't find or create a specification" );

				if( foundSpecificationItem->isUserSpecification() )
					return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "I have found a user specification, whilst trying to create a self-generated specification" );

				isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

				// A justification has been created, but the self-generated specification already exists
				// So, the justification needs to be added or replaced separately
				if( ( createdJustificationItem == NULL &&

				( !foundSpecificationItem->hasJustification( false, foundJustificationItem ) ||

				( !isPossessive &&
				justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) ) ||

				( createdJustificationItem != NULL &&

				( !isAssumption ||
				foundSpecificationItem->isConcludedAssumption() ||
				foundSpecificationItem->isSelfGeneratedAssumption() ||

				// Conclusion becomes assumption
				( foundSpecificationItem->isSelfGeneratedConclusion() &&

				( justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||

				( !isUserGeneralizationWord &&
				justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
				relationWordItem != NULL &&

				( ( isPossessive &&
				!relationWordItem->hasConfirmedSpecification() ) ||

				( !isPossessive &&

				( relationWordItem->hasAnsweredSelfGeneratedQuestion() ||
				generalizationWordItem->hasAnsweredSelfGeneratedQuestion() ||

				( relationWordItem->hasConfirmedSpecification() &&
				foundSpecificationItem->hasSpecificationCompoundCollection() ) ) ) ) ) ) ) ) ) )
					{
					if( processJustification( hasFoundRelationContext, hasRelationWord, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to process the created justification" );
					}
				}
			else
				{
				createdRelationContextNr = createdSpecificationItem->relationContextNr();

				if( !hasRelationWord ||
				!hasFoundRelationContext ||
				generalizationWordItem->hasConfirmedSpecification() )
					{
					if( hasRelationWord &&
					isGeneralizationProperName )
						{
						if( adminItem_->collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, createdRelationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						if( isAssumption &&
						!isExclusiveSpecification &&
						!createdSpecificationItem->isHiddenSpanishSpecification() )
							{
							// Check self-generated specification for integrity
							if( generalizationWordItem->writeSelectedSpecification( true, true, false, true, NO_ANSWER_PARAMETER, createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to write the self-generated specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to check the writing integrity" );

							if( strlen( commonVariables_->writtenSentenceString ) == 0 )
								return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "Integrity error! I couldn't write the self-generated specification with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
							}
						}
					}
				else
					{
					if( foundSpecificationItem != NULL &&
					foundSpecificationItem->relationContextNr() == createdRelationContextNr )
						return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "Integrity error! A specification was created in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" while a specification was found as well with the same relation context" );
					}
				}

			if( !isNegative &&
			!isPartOf &&
			!isSkippingAdditionalConclusionOrAskQuestion &&
			adminItem_->isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != NULL )
						{
						// Draw a unique relation conclusion
						// Not yet explained in the design
						if( addSelfGeneratedSpecification( false, false, false, isEveryGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, createdSpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					{
					if( createdJustificationItem != NULL ||
					createdSpecificationItem != NULL )
						{
						if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, false, isArchivedAssignment, isExclusiveSpecification, true, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		specificationResult.createdJustificationItem = createdJustificationItem;
		return specificationResult;
		}

	SpecificationResultType addSpecification( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		ContextResultType contextResult;
		SpecificationResultType assignmentResult;
		SpecificationResultType specificationResult;
		bool isAuthorizationRequired;
		bool isDefinitionSpecification;
		bool isSpecificationWordSpanishAmbiguous;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isCompoundCollectionSpanishAmbiguous = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationContextAlreadyDefined = false;
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *replacedAssignmentItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *createdAssignmentItem = NULL;
		SpecificationItem *createdSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given specification word is the administrator" );

		existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

		// Get relation context
		// Exceptions: Conditions and imperative verbs have no relation context
		if( !isConditional &&
		!isSelection &&
		generalizationWordTypeNr != WORD_TYPE_VERB_SINGULAR &&
		specificationWordItem != NULL &&
		relationWordItem != NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( isSpecificationWordSpanishAmbiguous &&
			isPossessive &&
			specificationCollectionNr > NO_COLLECTION_NR &&
			specificationWordItem->isCompoundCollection( specificationCollectionNr ) )
				isCompoundCollectionSpanishAmbiguous = true;

			if( isArchivedAssignment ||
			relationContextNr == NO_CONTEXT_NR ||

			( isPossessive &&
			!isSpecificationWordSpanishAmbiguous &&
			nContextRelations == 0 &&
			specificationCollectionNr == NO_COLLECTION_NR &&

			// Possessive user specification
			( !isSelfGenerated ||

			// Ignoring context in other words
			( existingSpecificationItem != NULL &&
			existingSpecificationItem->hasRelationContext() &&

			( !existingSpecificationItem->hasSpecificationCollection() ||
			existingSpecificationItem->relationContextNr() != relationContextNr ) &&

			( !relationWordItem->hasConfirmedSpecification() ||
			existingSpecificationItem->isSelfGeneratedAssumption() ) ) ) ) )
				{
				if( ( contextResult = adminItem_->getSpecificationRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isPossessive, ( firstJustificationItem != NULL && firstJustificationItem->isAssumptionJustification() ), specificationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to get a relation context" );

				relationContextNr = contextResult.contextNr;
				}
			else
				isRelationContextAlreadyDefined = true;

			if( relationWordItem->addContext( isCompoundCollectionSpanishAmbiguous, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) != RESULT_OK )
				return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );
			}

		// Skip adding specification if already exists
		if( isAssignment ||
		!isRelationContextAlreadyDefined ||
		existingSpecificationItem == NULL ||
		!existingSpecificationItem->isActiveAssignment() )
			{
			// Check for an assignment or a specification with opposite negative indicator
			if( !isAssignment &&
			isArchivedAssignment_ &&
			generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, !isNegative, isPossessive, questionParameter, specificationWordItem ) != NULL )
				isAssignment = true;
			isAuthorizationRequired = isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem );

			if( isAuthorizationRequired )
				{
				if( ( specificationResult = adminItem_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add a specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with authorization" );
				}
			else
				{
				if( ( specificationResult = generalizationWordItem->addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL ) ).result != RESULT_OK )
					return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add an assignment specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				isArchivedAssignment_ = isArchivedAssignment;
				replacedAssignmentItem = specificationResult.replacedAssignmentItem;

				isDefinitionSpecification = ( relationWordItem == NULL &&
											adminItem_->isNounWordType( generalizationWordTypeNr ) &&
											adminItem_->isNounWordType( specificationWordTypeNr ) );

				if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
					{
					if( hasRelationWord &&
					!isSelfGenerated &&
					( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
						{
						userSpecificationItem_ = foundSpecificationItem;

						if( !isQuestion &&
						foundSpecificationItem->hasRelationContext() )
							simpleUserSpecificationItem_ = generalizationWordItem->firstSpecificationItem( isPossessive, false, NO_QUESTION_PARAMETER, specificationWordItem );
						}
					}
				else
					{
					if( !isSelfGenerated )
						{
						userSpecificationItem_ = createdSpecificationItem;

						if( !isQuestion )
							simpleUserSpecificationItem_ = createdSpecificationItem;
						}
					}

				if( !isExclusiveSpecification &&
				!isQuestion &&
				!isSelfGenerated &&
				specificationWordItem != NULL &&
				!generalizationWordItem->hasConfirmedSpecification() )
					{
					if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME )
						{
						if( !isSpecificationGeneralization &&
						createdSpecificationItem != NULL )
							{
							if( isNegative )
								{
								if( createdSpecificationItem->hasSpecificationCollection() )
									{
									if( adminItem_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to make an only-option-left assumption" );
									}
								}
							else
								{
								if( adminItem_->collectGeneralizationWordWithPreviousOne( ( isAssignment || isAuthorizationRequired ), isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
									return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

								if( !isPossessive )
									{
									if( adminItem_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( generalizationWordItem->hasFoundAnswerToQuestion() &&
									!specificationWordItem->isCompoundCollection( createdSpecificationItem->specificationCollectionNr() ) )
										{
										if( adminItem_->drawNegativeConclusionsFromAnsweredQuestions( createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
											return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to draw negative conclusions from answered questions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						}
					else
						{
						if( isDefinitionSpecification )
							{
							if( isSpecificationGeneralization )
								{
								if( isPartOf &&
								!isPossessive &&

								( !isAssignment ||
								isArchivedAssignment ) )
									{
									// Draw a possessive definition conclusion from a specification-generalization 'part of' sentence
									// Not yet explained in the design
									if( addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, false, false, isNegative, false, true, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								{
								if( createdSpecificationItem == NULL )
									{
									if( !isConditional &&
									!isNegative )
										{
										// Check if primary specification already has an assignment
										foundAssignmentItem = generalizationWordItem->firstAssignmentItem( true, true, true, isPossessive, questionParameter, relationContextNr, specificationWordItem );

										if( isAssignment ||
										foundAssignmentItem != NULL )
											{
											if( generalizationWordItem->writeSelectedSpecification( false, true, ( foundAssignmentItem == NULL ? specificationResult.foundSpecificationItem : foundAssignmentItem ) ) != RESULT_OK )
												return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to write a non-proper-name specification of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( strlen( commonVariables_->writtenSentenceString ) == 0 )
												return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );

											if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( foundAssignmentItem == NULL ? INTERFACE_LISTING_SENTENCE_ASSIGNMENT_CONFLICTS_WITH_DEFINITION_SPECIFICATION : INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) != RESULT_OK )
												return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "I failed to write a header" );

											if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
												return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "I failed to write a sentence" );
											}
										}
									}
								else
									{
									if( isPossessive &&

									( !isAssignment ||
									isArchivedAssignment ) )
										{
										// Draw a specification-generalization 'part of' conclusion from a possessive definition sentence
										// Not yet explained in the design
										if( addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, false, false, isNegative, true, false, false, false, true, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
											return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						}
					}

				if( !commonVariables_->hasDisplayedWarning &&
				// Skip for example imperative verbs
				adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) &&

				( ( isAssignment &&
				// At this stage, selections must be stored - rather than executed. So, don't assign them
				!isSelection ) ||

				replacedAssignmentItem != NULL ) )
					{
					if( ( assignmentResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != NULL && specificationWordItem->isAdjectiveAssignedOrEmpty() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
						return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = assignmentResult.createdSpecificationItem ) != NULL )
						{
						if( !isArchivedAssignment_ ||
						specificationResult.createdSpecificationItem != NULL )
							specificationResult.createdSpecificationItem = createdAssignmentItem;

						if( replacedAssignmentItem != NULL )
							{
							if( generalizationWordItem->replaceOrDeleteSpecification( false, replacedAssignmentItem, createdAssignmentItem ) != RESULT_OK )
								return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}

						if( isNegative &&
						!isQuestion &&
						createdAssignmentItem->hasSpecificationCollection() &&
						generalizationWordItem->isUserGeneralizationWord )
							{
							if( adminItem_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem->specificationCollectionNr(), generalizationWordItem ) != RESULT_OK )
								return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
							}
						}

					if( !isSelfGenerated &&
					( tempSpecificationItem = ( createdAssignmentItem == NULL ? assignmentResult.foundSpecificationItem : createdAssignmentItem ) ) != NULL )
						{
						userSpecificationItem_ = tempSpecificationItem;

						if( isArchivedAssignment_ &&
						!isQuestion )
							simpleUserSpecificationItem_ = tempSpecificationItem;
						}
					}

				if( !commonVariables_->hasDisplayedWarning &&
				!isExclusiveSpecification &&
				specificationWordItem != NULL &&
				userSpecificationItem_ != NULL &&

				( !isSelfGenerated ||

				( isDefinitionSpecification &&
				userSpecificationItem_->hasGeneralizationCollection() ) ) )
					{
					if( isQuestion )
						{
						if( hasRelationWord &&

						( createdAssignmentItem != NULL ||

						( createdSpecificationItem != NULL &&
						!generalizationWordItem->hasDisplayedMoreSpecificRelatedQuestion() ) ) )
							{
							if( adminItem_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == NULL ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						{
						if( isDefinitionSpecification &&
						!isSpecificationGeneralization )
							{
							if( adminItem_->drawSpecificationGeneralizationConclusion( isArchivedAssignment, isPossessive, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion" );
							}
						}
					}
				}
			}

		return specificationResult;
		}

	SpecificationItem *simpleUserSpecificationItem()
		{
		return simpleUserSpecificationItem_;
		}

	SpecificationItem *userSpecificationItem()
		{
		return userSpecificationItem_;
		}
	};

/*************************************************************************
 *	"How amazing are the deeds of the Lord!
 *	All who delight in him should ponder them." (Psalm 111:2)
 *************************************************************************/
