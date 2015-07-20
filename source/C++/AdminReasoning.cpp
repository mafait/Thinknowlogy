/*
 *	Class:			AdminReasoning
 *	Supports class:	AdminItem
 *	Purpose:		To support grammar-based reasoning and to
 *					autonomously ask grammar-based questions
 *	Version:		Thinknowlogy 2015r1beta (Coraz�n)
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

#include "AdminItem.h"
#include "SpecificationItem.cpp"

class AdminReasoning
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasAdjustedQuestion_;
	bool hasFoundConflictWithUserSpecification_;

	SpecificationItem *alternativeAnotherPrimarySpecificationItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType checkForConflictWithUserSpecification( WordItem *generalizationWordItem, WordItem *primarySpecificationWordItem, WordItem *questionSpecificationWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForConflictWithUserSpecification";

		hasFoundConflictWithUserSpecification_ = false;

		if( generalizationWordItem != NULL )
			{
			if( primarySpecificationWordItem != NULL )
				{
				if( questionSpecificationWordItem != NULL )
					{
					if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
						{
						do	{
							if( !currentSpecificationItem->isExclusiveSpecification() &&
							!currentSpecificationItem->isNegative() &&
							!currentSpecificationItem->isPossessive() &&
							// Current sentence
							!currentSpecificationItem->isOlderItem() &&
							// User sentence
							!currentSpecificationItem->isSelfGenerated() &&
							( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
								{
								foundSpecificationItem = currentSpecificationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, questionSpecificationWordItem );

								if( ( foundSpecificationItem == NULL ||

								( !foundSpecificationItem->isExclusiveSpecification() &&
								!foundSpecificationItem->isNegative() ) ) )
									hasFoundConflictWithUserSpecification_ = true;
								}
							}
						while( !hasFoundConflictWithUserSpecification_ &&
						( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given question specification word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType processJustification( bool hasFoundRelationContext, bool hasFoundSelfGeneratedAssumption, bool hasRelationWord, unsigned short justificationTypeNr, JustificationItem *createdOrFoundJustificationItem, SpecificationItem *foundSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool isPossessiveSecondarySpecification;
		bool isAttachingJustification = true;
		bool isDeletingCreatedJustification = false;
		JustificationResultType justificationResult;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem;
		JustificationItem *existingJustificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processJustification";

		if( createdOrFoundJustificationItem != NULL )
			{
			if( foundSpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( hasFoundSelfGeneratedAssumption &&
					secondarySpecificationItem != NULL )
						{
						isPossessiveSecondarySpecification = secondarySpecificationItem->isPossessive();
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
										( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), isPossessiveSecondarySpecification, true, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL )
											{
											if( ( existingJustificationItem = generalizationWordItem->olderSecondarySpecificationJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != NULL )
												{
												if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) == RESULT_OK )
													isAttachingJustification = false;
												else
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an existing justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}

								break;

							case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( hasFoundRelationContext &&
								hasRelationWord &&
								primarySpecificationItem != NULL &&
								generalizationWordItem->isUserGeneralizationWord &&
								// Confirmation
								secondarySpecificationItem->isUserSpecification() )
									{
									if( ( existingJustificationItem = generalizationWordItem->olderPrimarySpecificationJustificationItem( false, true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, secondarySpecificationItem->specificationCollectionNr(), primarySpecificationItem ) ) == NULL )
										{
										if( ( obsoleteJustificationItem = generalizationWordItem->secondaryAssumptionJustificationItem( !isPossessiveSecondarySpecification, secondarySpecificationWordItem ) ) != NULL )
											{
											if( ( justificationResult = generalizationWordItem->addJustification( false, false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, obsoleteJustificationItem->orderNr, obsoleteJustificationItem->originalSentenceNr(), NULL, NULL, secondarySpecificationItem, NULL, NULL ) ).result == RESULT_OK )
												{
												if( justificationResult.createdJustificationItem != NULL )
													{
													if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, justificationResult.createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find or create a justification without primary specification" );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a justification without primary specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											// Skip if justification already exists as an opposite possessive conditional specification assumption
											if( generalizationWordItem->olderSecondarySpecificationJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) != NULL )
												isDeletingCreatedJustification = true;
											}
										}
									else
										{
										// Replace existing justification by the created justification
										if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) == RESULT_OK )
											isAttachingJustification = false;
										else
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an existing justification by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}

								break;

							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( !hasRelationWord &&
								primarySpecificationItem != NULL )
									{
									if( ( existingJustificationItem = foundSpecificationItem->differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) == NULL )
										{
										if( ( obsoleteJustificationItem = foundSpecificationItem->answeredQuestionJustificationItem() ) != NULL )
											{
											if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) == RESULT_OK )
												isAttachingJustification = false;
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										}
									else
										{
										isAttachingJustification = false;

										if( primarySpecificationItem->assumptionLevel() < existingJustificationItem->primarySpecificationAssumptionLevel() )
											{
											// Replace less certain existing justification by the created justification
											if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											isDeletingCreatedJustification = true;
										}
									}

								break;

							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
								if( !hasRelationWord &&
								primarySpecificationItem != NULL &&
								( specificationSubstitutionPartOfAssumptionJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != NULL )
									{
									if( specificationSubstitutionPartOfAssumptionJustificationItem->hasHiddenPrimarySpecification() )
										{
										if( specificationSubstitutionPartOfAssumptionJustificationItem->changePrimarySpecification( primarySpecificationItem ) == RESULT_OK )
											isDeletingCreatedJustification = true;
										else
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace a hidden primary specification of the attached justification item of the found specification substitution 'part of' assumption justification item" );
										}
									}
							}
						}

					if( isDeletingCreatedJustification )
						{
						if( generalizationWordItem->replaceOrDeleteJustification( createdOrFoundJustificationItem ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						if( isAttachingJustification )
							{
							// Attach created justification to the found specification
							if( generalizationWordItem->attachJustification( createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
								adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given found specification item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given created or found justification item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminReasoning( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasAdjustedQuestion_ = false;
		hasFoundConflictWithUserSpecification_ = false;

		alternativeAnotherPrimarySpecificationItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminReasoning" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeAdminReasoningVariables()
		{
		hasAdjustedQuestion_ = false;
		hasFoundConflictWithUserSpecification_ = false;
		}

	ResultType askSpecificationSubstitutionQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isPrimarySpecificationCollectedWithItself;
		bool isSecondarySpecificationCompoundCollection;
		bool isBlockingCommonWordOfCompoundCollection = false;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondaryRelationContextNr;
		unsigned int secondarySpecificationCollectionNr;
		SpecificationItem *adjustedQuestionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpecificationSubstitutionQuestion";

		if( primarySpecificationItem != NULL )
			{
			if( secondarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( ( secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
							{
							if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
								{
								isPrimarySpecificationCollectedWithItself = primarySpecificationItem->isSpecificationWordCollectedWithItself();

								nonCompoundSpecificationCollectionNr = secondarySpecificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );
								secondaryRelationContextNr = secondarySpecificationItem->relationContextNr();
								isSecondarySpecificationCompoundCollection = secondarySpecificationWordItem->isCompoundCollection( secondarySpecificationCollectionNr, specificationWordItem );

								foundSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if already exists as specification
								if( isPrimarySpecificationCollectedWithItself ||
								foundSpecificationItem == NULL ||
								generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() ||

								( isSecondarySpecificationCompoundCollection &&
								foundSpecificationItem->isSelfGeneratedAssumption() ) )
									{
									if( generalizationWordItem->isUserGeneralizationWord )
										{
										if( isSecondarySpecificationCompoundCollection &&
										nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR )
											{
											// Check for conflicts
											if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, secondaryRelationContextNr, secondarySpecificationWordItem ) == RESULT_OK )
												{
												// Check for adjusted compound question
												if( ( specificationResult = generalizationWordItem->findQuestionToBeAdjustedByCompoundCollection( false, false, WORD_PARAMETER_SINGULAR_VERB_IS, secondarySpecificationCollectionNr, secondaryRelationContextNr, specificationWordItem ) ).result == RESULT_OK )
													{
													if( ( adjustedQuestionSpecificationItem = specificationResult.adjustedQuestionSpecificationItem ) != NULL )
														{
														// Write adjusted compound question
														if( generalizationWordItem->writeUpdatedSpecification( true, false, false, adjustedQuestionSpecificationItem ) == RESULT_OK )
															hasAdjustedQuestion_ = true;
														else
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( checkForConflictWithUserSpecification( generalizationWordItem, specificationWordItem, secondarySpecificationWordItem ) == RESULT_OK )
												{
												if( hasFoundConflictWithUserSpecification_ )
													isBlockingCommonWordOfCompoundCollection = true;
												else
													{
													if( nonCompoundSpecificationCollectionNr == NO_COLLECTION_NR )
														nonCompoundSpecificationCollectionNr = adminItem_->nonCompoundCollectionNrInAllWords( secondarySpecificationCollectionNr );

													if( generalizationWordItem->firstUserSpecificationItem( false, false, secondarySpecificationCollectionNr, NO_CONTEXT_NR, NULL ) != NULL ||

													( nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR &&
													generalizationWordItem->firstUserSpecificationItem( false, false, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) != NULL ) )
														isBlockingCommonWordOfCompoundCollection = true;
													}
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a conflict with the user specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										}

									if( !commonVariables_->hasShownWarning &&
									!isBlockingCommonWordOfCompoundCollection &&

									( isExclusiveSpecification ||
									secondarySpecificationItem->isExclusiveSpecification() ||
									generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() ) )
										{
										alternativeAnotherPrimarySpecificationItem_ = NULL;

										if( isPrimarySpecificationCollectedWithItself &&
										primarySpecificationItem->isHiddenSpecification() )
											primarySpecificationItem = alternativeJustificationSpecificationItem( generalizationWordItem->hasFeminineProperNameEnding(), true, false, false, false, adminItem_->userSpecificationItem(), generalizationWordItem, specificationWordItem );

										if( primarySpecificationItem != NULL )
											{
											// Ask a specification substitution question
											// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, primarySpecificationItem->assumptionLevel(), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem_, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result == RESULT_OK )
												{
												if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
													{
													if( hasAdjustedQuestion_ )
														{
														// Replace obsolete justifications from adjusted compound question
														if( generalizationWordItem->replaceObsoleteJustificationsOfAdjustedQuestion( specificationResult.createdJustificationItem, specificationResult.foundSpecificationItem ) != RESULT_OK )
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace obsolete justifications from an adjusted compound question" );
														}
													}
												else
													{
													// Has created question part
													if( foundSpecificationItem != NULL &&
													secondarySpecificationWordItem != specificationWordItem )
														{
														if( generalizationWordItem->replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								}
							else
								return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );
							}
						else
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item has no specification collection" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNameEnding, bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkippingAdditionalConclusionOrAskQuestion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool hasFoundSelfGeneratedAssumption;
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

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
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
					if( adminItem_->isConclusion( conclusionJustificationTypeNr ) )
						{
						isAssumption = false;
						justificationTypeNr = conclusionJustificationTypeNr;
						}
					else
						adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given conclusion justification type isn't a conclusion" );
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( ( justificationResult = generalizationWordItem->addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, justificationTypeNr, NO_ORDER_NR, commonVariables_->currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, NULL ) ).result == RESULT_OK )
						{
						createdJustificationItem = justificationResult.createdJustificationItem;
						foundJustificationItem = justificationResult.foundJustificationItem;

						if( ( createdOrFoundJustificationItem = ( createdJustificationItem == NULL ? foundJustificationItem : createdJustificationItem ) ) != NULL )
							{
							if( ( specificationResult = adminItem_->addSpecification( isAssignment, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION || justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION ), prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result == RESULT_OK )
								{
								if( !commonVariables_->hasShownWarning )
									{
									createdSpecificationItem = specificationResult.createdSpecificationItem;

									if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
										hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

									if( createdSpecificationItem == NULL )
										{
										if( foundSpecificationItem != NULL )
											{
											if( foundSpecificationItem->isSelfGenerated() )
												{
												// A justification has been created, but the self-generated specification already exists
												// So, the justification needs to be added or replaced separately
												hasFoundSelfGeneratedAssumption = foundSpecificationItem->isSelfGeneratedAssumption();

												if( ( createdJustificationItem == NULL &&
												foundSpecificationItem->hasCurrentCreationSentenceNr() &&
												!foundSpecificationItem->hasFoundJustification( false, foundJustificationItem ) ) ||

												( createdJustificationItem != NULL &&

												( !isAssumption ||
												hasFoundSelfGeneratedAssumption ||
												foundSpecificationItem->isConcludedAssumption() ||

												( secondarySpecificationItem != NULL &&
												secondarySpecificationItem->isAssignment() &&

												( foundSpecificationItem->hasCurrentCreationSentenceNr() ||
												adminItem_->nContextWordsInAllWords( foundSpecificationItem->relationContextNr(), specificationWordItem ) == 1 ) ) ) ) )
													{
													if( processJustification( hasFoundRelationContext, hasFoundSelfGeneratedAssumption, hasRelationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) == RESULT_OK )
														{
														if( hasRelationWord &&
														isAssumption &&
														!isPossessive &&
														justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
														generalizationWordItem->isUserRelationWord &&
														!specificationWordItem->isNounWordCollectedWithItself() )
															{
															if( generalizationWordItem->recalculateAssumptionsInWord() != RESULT_OK )
																adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to process the created justification" );
													}
												}
											else
												adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I found a user specification, whilst trying to create a self-generated specification" );
											}
										else
											adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find or create a specification" );
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
												if( adminItem_->collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, createdRelationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
													adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );
												}

											if( !createdSpecificationItem->isHiddenSpecification() )
												{
												// Check self-generated specification for integrity
												if( generalizationWordItem->writeSelectedSpecification( true, true, false, false, NO_ANSWER_PARAMETER, createdSpecificationItem ) == RESULT_OK )
													{
													if( strlen( commonVariables_->writeSentenceString ) == 0 )
														adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Integrity error! I couldn't write the self-generated specification with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
													}
												else
													adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the self-generated specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to check the writing integrity" );
												}
											}
										else
											{
											if( foundSpecificationItem != NULL &&
											foundSpecificationItem->relationContextNr() == createdRelationContextNr )
												adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Integrity error! A specification was created in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" while this specification was found as well" );
											}
										}

									if( commonVariables_->result == RESULT_OK &&
									!isSkippingAdditionalConclusionOrAskQuestion &&
									adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) &&

									( createdSpecificationItem != NULL ||
									createdJustificationItem != NULL ) )
										{
										if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, false, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) == RESULT_OK )
											{
											if( isUniqueUserRelation &&
											createdSpecificationItem != NULL )
												{
												// Draw a unique relation conclusion
												// Not yet explained in the design
												if( addSelfGeneratedSpecification( false, false, false, isEveryGeneralization, isExclusiveSpecification, false, !isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, createdSpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
													adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							else
								adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a specification" );
							}
						else
							adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find or create a justification" );
						}
					else
						adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a justification" );
					}
				}
			else
				adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.createdJustificationItem = createdJustificationItem;
		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationItem *alternativeAnotherPrimarySpecificationItem()
		{
		return alternativeAnotherPrimarySpecificationItem_;
		}

	SpecificationItem *alternativeJustificationSpecificationItem( bool hasGeneralizationWordFeminineProperNameEnding, bool isCheckingForPossessiveSpecification, bool isCheckingForQuestion, bool isNegative, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *oppositeSpecificationItem;
		SpecificationItem *possessiveSpecificationItem;
		SpecificationItem *questionSpecificationItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;

		alternativeAnotherPrimarySpecificationItem_ = NULL;

		if( userSpecificationItem != NULL &&
		generalizationWordItem != NULL &&
		specificationWordItem != NULL )
			{
			if( generalizationWordItem->isUserRelationWord &&
			userSpecificationItem->isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNameEnding &&
			( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL )
				{
				// Try to find opposite specification
				oppositeSpecificationItem = userGeneralizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

				if( oppositeSpecificationItem != NULL &&
				!oppositeSpecificationItem->isHiddenSpecification() )
					return oppositeSpecificationItem;

				if( isCheckingForPossessiveSpecification &&
				( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
					{
					possessiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, true, NO_QUESTION_PARAMETER, userSpecificationWordItem );

					// Try to find a possessive specification
					if( possessiveSpecificationItem != NULL &&
					!possessiveSpecificationItem->isHiddenSpecification() )
						{
						alternativeAnotherPrimarySpecificationItem_ = adminItem_->lastFoundOppositePossessiveDefinitionSpecificationItem();
						return possessiveSpecificationItem;
						}
					}

				if( isCheckingForQuestion &&
				// Try to find question
				( questionSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, false, WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) ) != NULL )
					return questionSpecificationItem;
				}
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	
 *	Praise God in his sanctuary;
 *	praise him in his mighty heaven!" (Psalm 150:1)
 *************************************************************************/