/*
 *	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To create conclusions and questions autonomously
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *************************************************************************/
/*  This program is free software: you can redistribute it and/or modify
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
 *************************************************************************/

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "SpecificationItem.cpp"

class AdminConclusion
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasCreatedPossessiveReversibleConclusion_;
	bool hasFoundAdjustedQuestion_;
	bool hasFoundConflictWithUserSpecification_;
	bool hasFoundSpecificationWordCollectedWithItself_;

	unsigned short questionParameter_;

	unsigned int generalizationSingleContextNr_;
	unsigned int missingNegativeConclusionSpecificationCollectionNr_;

	SpecificationItem *createdFeminineOrMasculineSpecificationItem_;
	SpecificationItem *foundFeminineOrMasculineSpecificationItem_;
	SpecificationItem *lastCreatedSpecificationSubstitutionConclusionSpecificationItem_;
	SpecificationItem *missingNegativeConclusionPrimarySpecificationItem_;
	SpecificationItem *missingNegativeConclusionAnotherPrimarySpecificationItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType askSpecificationSubstitutionQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isSecondarySpecificationCompoundCollection;
		bool isBlockingCommonWordOfCompoundCollection = false;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondaryRelationContextNr;
		unsigned int secondarySpecificationCollectionNr;
		SpecificationItem *adjustedQuestionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
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
								questionParameter_ = WORD_PARAMETER_SINGULAR_VERB_IS;

								nonCompoundSpecificationCollectionNr = secondarySpecificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );
								secondaryRelationContextNr = secondarySpecificationItem->relationContextNr();
								isSecondarySpecificationCompoundCollection = secondarySpecificationWordItem->isCompoundCollection( secondarySpecificationCollectionNr, specificationWordItem );

								foundSpecificationItem = generalizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if already exists as specification
								if( foundSpecificationItem == NULL ||
								generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() ||

								( isSecondarySpecificationCompoundCollection &&
								foundSpecificationItem->isSelfGeneratedAssumption() ) )
									{
									if( isSecondarySpecificationCompoundCollection &&
									nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR )
										{
										// Check for conflicts
										if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, secondaryRelationContextNr, secondarySpecificationWordItem, secondarySpecificationItem->specificationString() ) == RESULT_OK )
											{
											// Check for adjusted compound question
											if( ( specificationResult = generalizationWordItem->findQuestionToBeAdjustedByCompoundCollection( false, false, questionParameter_, secondarySpecificationCollectionNr, secondaryRelationContextNr, specificationWordItem ) ).result == RESULT_OK )
												{
												if( ( adjustedQuestionSpecificationItem = specificationResult.adjustedQuestionSpecificationItem ) != NULL )
													{
													// Write adjusted compound question
													if( generalizationWordItem->writeUpdatedSpecification( true, false, false, adjustedQuestionSpecificationItem ) == RESULT_OK )
														hasFoundAdjustedQuestion_ = true;
													else
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an adjusted compound question" );
													}
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
													nonCompoundSpecificationCollectionNr = myWordItem_->nonCompoundCollectionNrInAllWords( secondarySpecificationCollectionNr );

												if( generalizationWordItem->firstUserSpecificationItem( false, false, secondarySpecificationCollectionNr, NO_CONTEXT_NR, NULL ) != NULL ||

												( nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR &&
												generalizationWordItem->firstUserSpecificationItem( false, false, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) != NULL ) )
													isBlockingCommonWordOfCompoundCollection = true;
												}
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a conflict with the user specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}

									if( !commonVariables_->hasShownWarning &&
									!isBlockingCommonWordOfCompoundCollection )
										{
										// Ask a specification substitution question
										// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
										if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, ( primarySpecificationItem->assumptionLevel() + secondarySpecificationItem->assumptionLevel() ), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter_, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result == RESULT_OK )
											{
											if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
												{
												if( hasFoundAdjustedQuestion_ )
													{
													// Replace obsolete justifications from adjusted compound question
													if( generalizationWordItem->replaceObsoleteJustificationsOfAdjustedQuestion( specificationResult.createdJustificationItem, specificationResult.foundSpecificationItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to archive obsolete justifications from an adjusted compound question" );
													}
												}
											else	// Has created question part
												{
												if( secondarySpecificationWordItem != specificationWordItem &&
												( obsoleteSpecificationItem = generalizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != NULL )
													{
													if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item has no specification collection" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegativeConclusion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool isCommonWordCollectedWithItself;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondarySpecificationCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *firstNonCompoundCollectionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *commonWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

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
								if( ( commonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL )
									{
									if( ( specificationCollectionNr = commonWordItem->collectionNr( specificationWordTypeNr, specificationWordItem ) ) == NO_COLLECTION_NR )
										specificationCollectionNr = secondarySpecificationCollectionNr;

									if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationWordItem, NULL ) ).result == RESULT_OK )
										{
										isCommonWordCollectedWithItself = commonWordItem->isCollectedWithItself();
										relatedSpecificationItem = specificationResult.relatedSpecificationItem;

										// Current question part isn't related to an answer or a similar question
										if( isCommonWordCollectedWithItself &&
										relatedSpecificationItem == NULL )
											{
											nonCompoundSpecificationCollectionNr = commonWordItem->nonCompoundCollectionNr( specificationWordTypeNr );

											if( nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR &&
											nonCompoundSpecificationCollectionNr != specificationCollectionNr )
												{
												if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationWordItem, NULL ) ).result == RESULT_OK )
													relatedSpecificationItem = specificationResult.relatedSpecificationItem;
												else
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is collected with itself" );
												}
											}

										if( relatedSpecificationItem == NULL )
											{
											hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
											hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

											// Negative feminine conclusion
											if( ( hasGeneralizationWordMasculineProperNameEnding &&
											secondarySpecificationWordItem->isFeminineWord() ) ||

											// Negative masculine conclusion
											( hasGeneralizationWordFeminineProperNameEnding &&
											secondarySpecificationWordItem->isMasculineWord() ) ||

											// Specification is collected with itself
											( specificationWordItem->isCollectedWithItself() &&
											generalizationWordItem->isUserGeneralizationWord &&
											!generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&
											!adminItem_->isPossessiveUserSpecification() ) )
												{
												missingNegativeConclusionSpecificationCollectionNr_ = secondarySpecificationCollectionNr;
												missingNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
												missingNegativeConclusionAnotherPrimarySpecificationItem_ = secondarySpecificationItem;
												}
											else
												{
												// Skip invalid questions
												if( !isNegativeConclusion &&

												( primarySpecificationItem->hasSpecificationCompoundCollection() ||

												( !hasGeneralizationWordFeminineProperNameEnding &&
												!hasGeneralizationWordMasculineProperNameEnding ) ) )
													{
													if( askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										else
											{
											if( !isCommonWordCollectedWithItself &&
											secondarySpecificationItem->hasSpecificationCompoundCollection() &&
											( firstNonCompoundCollectionSpecificationItem = generalizationWordItem->firstNonCompoundCollectionSpecificationItem( secondarySpecificationCollectionNr ) ) != NULL )
												relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

											if( missingNegativeConclusionAnotherPrimarySpecificationItem_ != NULL &&
											missingNegativeConclusionAnotherPrimarySpecificationItem_->specificationCollectionNr() == secondarySpecificationCollectionNr )
												{
												if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, missingNegativeConclusionPrimarySpecificationItem_, missingNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, generalizationWordItem ) == RESULT_OK )
													{
													missingNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;
													missingNegativeConclusionPrimarySpecificationItem_ = NULL;
													missingNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
													}
												else
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
												}

											if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the common word of the given specification collection number" );
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item has no specification collection" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

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
							!currentSpecificationItem->isOlderItem() &&			// Current sentence
							!currentSpecificationItem->isSelfGenerated() &&		// User sentence
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
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given question specification word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType drawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isPrimaryAndUserSameSpecification;
		bool isPrimarySpecificationHidden;
		bool isSelectingUserSpecification = false;
		bool isUserSpecificationCollectedWithItself = false;
		unsigned int anotherPrimarySpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *negativeAssumptionOrConclusionJustificationItem;
		JustificationItem *questionJustificationItem;
		SpecificationItem *conclusionPrimarySpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingPositiveSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *foundNegativeAssumptionOrConclusionPrimarySpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem = NULL;
		SpecificationItem *userSpecificationItem = NULL;
		WordItem *primarySpecificationWordItem;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusion";

		if( primarySpecificationItem != NULL )
			{
			if( anotherPrimarySpecificationItem != NULL )
				{
				if( secondarySpecificationItem != NULL )
					{
					if( generalizationWordItem != NULL )
						{
						if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
							{
							if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
								{
								if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) != NULL )
									{
									// If exists, replace self-generated assumption for conclusion
									if( primarySpecificationItem->isSelfGeneratedAssumption() &&
									( conclusionPrimarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( primarySpecificationItem->isArchivedAssignment(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), false, primarySpecificationWordItem ) ) != NULL )
										primarySpecificationItem = conclusionPrimarySpecificationItem;

									existingPositiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
									foundNegativeAssumptionOrConclusionPrimarySpecificationItem = NULL;

									if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != NULL )
										foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem->primarySpecificationItem();

									// Skip if negative conclusion already exists
									if( ( !primarySpecificationItem->isUserSpecification() ||
									!secondarySpecificationItem->isOlderItem() ) &&

									// No positive specification exists with this specification word
									( existingPositiveSpecificationItem == NULL ||
									existingPositiveSpecificationItem->isHiddenSpecification() ) &&

									// Prefer primary specification with the least uncertainty
									( !primarySpecificationItem->hasSpecificationCollection() ||
									foundNegativeAssumptionOrConclusionPrimarySpecificationItem == NULL ||
									primarySpecificationItem->assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem->assumptionLevel() ) &&

									// No question exists with this specification word
									generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == NULL )
										{
										if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
											{
											isPrimaryAndUserSameSpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );
											isPrimarySpecificationHidden = primarySpecificationItem->isHiddenSpecification();

											if( !isPrimaryAndUserSameSpecification )
												anotherSecondarySpecificationItem = secondarySpecificationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL );

											if( generalizationWordItem->isUserGeneralizationWord &&
											( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
												{
												if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
													{
													// Force conclusion if user specification that is collected with itself
													if( userSpecificationWordItem->isCollectedWithItself() )
														isUserSpecificationCollectedWithItself = true;

													if( anotherSecondarySpecificationItem != NULL &&
													userSpecificationItem->hasSpecificationCompoundCollection() &&
													userSpecificationWordItem->hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
														isSelectingUserSpecification = true;
													}
												}

											// Draw a negative conclusion
											// Not yet explained in the design
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, ( isUserSpecificationCollectedWithItself ? JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isPrimarySpecificationHidden ? anotherPrimarySpecificationItem : ( isPrimaryAndUserSameSpecification ? NULL : primarySpecificationItem ) ), ( isPrimarySpecificationHidden ? NULL : anotherPrimarySpecificationItem ), ( isSelectingUserSpecification ? userSpecificationItem : secondarySpecificationItem ), ( isSelectingUserSpecification ? NULL : anotherSecondarySpecificationItem ), generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result == RESULT_OK )
												{
												createdJustificationItem = specificationResult.createdJustificationItem;

												// Found specification
												if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
													{
													if( createdJustificationItem != NULL )
														{
														if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
															{
															foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

															if( negativeAssumptionOrConclusionJustificationItem == NULL )
																{
																if( foundSpecificationItem->isSelfGeneratedAssumption() )
																	{
																	if( createdJustificationItem->isConclusionJustification() )
																		{
																		// Replace justification of negative conclusion / assumption
																		if( foundSpecificationItem->markAsConcludedAssumption( false ) != RESULT_OK )
																			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
																		}
																	}
																else
																	{
																	if( !isUserSpecificationCollectedWithItself )
																		{
																		if( generalizationWordItem->replaceOrDeleteJustification( createdJustificationItem ) != RESULT_OK )
																			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the created justification" );
																		}
																	}
																}
															else
																{
																if( !foundSpecificationItem->isConcludedAssumption() )
																	{
																	// Replace justification of negative conclusion / assumption
																	if( generalizationWordItem->replaceJustification( false, negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
																		return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															}
														else
															return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The found specification item is undefined" );
														}
													}
												else	// Created specification
													{
													// Replace corrected negative conclusion
													if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
														{
														if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result == RESULT_OK )
															{
															if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
																{
																if( generalizationWordItem->replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created negative conclusion" );
														}
													else
														{
														// Replace question justification
														if( anotherSecondarySpecificationItem != NULL &&
														anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() )
															{
															if( ( questionJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) ) == NULL )
																{
																if( isSelectingUserSpecification )
																	{
																	if( generalizationWordItem->changeJustificationOfNegativeAssumptions( userSpecificationItem ) != RESULT_OK )
																		return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																{
																if( generalizationWordItem->replaceJustification( false, questionJustificationItem, createdJustificationItem, createdSpecificationItem ) != RESULT_OK )
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														}
													}
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", anotherPrimarySpecificationWordItem->anyWordTypeString(), "\"" );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given related specification item has no specification word item" );
										}
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given another primary specification item has no specification word item" );
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given another primary specification item has no specification collection" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( unsigned short assumptionLevel, SpecificationItem *secondarySpecificationItem )
		{
		bool isArchivedAssignment;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem != NULL )
			{
			if( secondarySpecificationItem->isGeneralizationNoun() &&
			secondarySpecificationItem->isSpecificationNoun() )
				{
				secondarySpecificationItem = secondarySpecificationItem->updatedSpecificationItem();
				isArchivedAssignment = secondarySpecificationItem->isArchivedAssignment();

				// Draw a specification generalization conclusion
				// Not yet explained in the design
				if( adminItem_->addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, true, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminConclusion( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasCreatedPossessiveReversibleConclusion_ = false;
		hasFoundAdjustedQuestion_ = false;
		hasFoundConflictWithUserSpecification_ = false;
		hasFoundSpecificationWordCollectedWithItself_ = false;

		questionParameter_ = NO_QUESTION_PARAMETER;

		generalizationSingleContextNr_ = NO_CONTEXT_NR;
		missingNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		createdFeminineOrMasculineSpecificationItem_ = NULL;
		foundFeminineOrMasculineSpecificationItem_ = NULL;
		lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = NULL;
		missingNegativeConclusionPrimarySpecificationItem_ = NULL;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminConclusion" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeAdminConclusionVariables()
		{
		hasCreatedPossessiveReversibleConclusion_ = false;
		hasFoundAdjustedQuestion_ = false;
		hasFoundSpecificationWordCollectedWithItself_ = false;

		generalizationSingleContextNr_ = NO_CONTEXT_NR;
		missingNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		createdFeminineOrMasculineSpecificationItem_ = NULL;
		foundFeminineOrMasculineSpecificationItem_ = NULL;
		lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = NULL;
		missingNegativeConclusionPrimarySpecificationItem_ = NULL;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		}

	ResultType drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		JustificationItem *currentJustificationItem;
		SpecificationItem *answeredQuestionSpecificationItem;
		WordItem *answeredQuestionSpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusionsFromAnsweredQuestions";

		if( userSpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( ( answeredQuestionSpecificationItem = generalizationWordItem->firstRecentlyAnsweredQuestionSpecificationItem( WORD_PARAMETER_SINGULAR_VERB_IS ) ) != NULL )
						{
						do	{
							if( userSpecificationWordItem->hasCollectionNr( answeredQuestionSpecificationItem->specificationCollectionNr() ) )
								{
								if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem->specificationWordItem() ) != NULL )
									{
									// Check if already exists as positive specification
									if( generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == NULL )
										{
										if( ( currentJustificationItem = answeredQuestionSpecificationItem->firstJustificationItem() ) != NULL )
											{
											// Do for all justifications
											do	{
												if( !currentJustificationItem->hasPrimaryQuestion() )
													{
													if( drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, generalizationWordItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The answered question specification item has no justification" );
										}
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The answered question specification item has no specification word" );
								}
							}
						while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any answered question specification" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item has no specification word" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
		{
		bool hasFoundAssignment;
		bool hasFoundMoreOptionsLeft = false;
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstNegativeOnlyOptionLeftAssignmentItem = NULL;
		SpecificationItem *firstOnlyOptionLeftSpecificationItem = NULL;
		SpecificationItem *foundOnlyOptionLeftSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( generalizationWordItem != NULL )
			{
			if( specificationCollectionNr > NO_COLLECTION_NR )
				{
				if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
					{
					firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

					do	{
						if( currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
						( currentAssignmentItem = generalizationWordItem->firstSelectedSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != NULL )
							{
							hasFoundAssignment = false;
							specificationWordItem = currentSpecificationItem->specificationWordItem();

							if( currentAssignmentItem->isNegative() &&
							firstNegativeOnlyOptionLeftAssignmentItem == NULL )
								firstNegativeOnlyOptionLeftAssignmentItem = currentAssignmentItem;

							do	{
								if( currentAssignmentItem->isNegative() &&
								currentAssignmentItem->specificationCollectionNr() == specificationCollectionNr &&
								currentAssignmentItem->specificationWordItem() == specificationWordItem )
									hasFoundAssignment = true;
								}
							while( !hasFoundAssignment &&
							( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );

							if( !hasFoundAssignment )
								{
								if( foundOnlyOptionLeftSpecificationItem == NULL )
									foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;	// Found the first option left
								else
									{
									hasFoundMoreOptionsLeft = true;					// There are more options left
									foundOnlyOptionLeftSpecificationItem = NULL;	// So, clear the found option
									}
								}
							}
						}
					while( !hasFoundMoreOptionsLeft &&
					( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

					if( firstNegativeOnlyOptionLeftAssignmentItem != NULL &&
					foundOnlyOptionLeftSpecificationItem != NULL )
						{
						// Draw an only option left conclusion
						// Not yet explained in the design
						if( adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification collection number is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		bool isConclusionAssignment;
		bool isSpecificationCollectedWithItself;
		bool hasFoundSecondaryArchivedAssignment = false;
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		bool isAssumption = ( userAssumptionLevel > NO_ASSUMPTION_LEVEL );
		unsigned int nContextRelations;
		unsigned int foundRelationContextNr;
		unsigned int multipleRelationContextNr;
		unsigned int generalizationContextNr = NO_CONTEXT_NR;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *obsoleteAssumptionJustificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *existingConclusionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *collectedWithItselfCommonWordItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( relationContextNr > NO_CONTEXT_NR )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( relationWordItem != NULL )
						{
						if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME &&
						adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
							{
							isSpecificationCollectedWithItself = specificationWordItem->isCollectedWithItself();

							if( ( foundRelationContextNr = generalizationWordItem->contextNrInWord( specificationWordItem ) ) > NO_CONTEXT_NR )
								{
								// Check on single entry context
								if( ( nContextRelations = myWordItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) == 1 )
									{
									if( isSpecificationCollectedWithItself )
										{
										if( generalizationSingleContextNr_ == NO_CONTEXT_NR )
											{
											multipleRelationContextNr = generalizationWordItem->contextNrInWord( specificationWordItem );

											if( !hasCreatedPossessiveReversibleConclusion_ &&

											( multipleRelationContextNr == NO_CONTEXT_NR ||
											multipleRelationContextNr == foundRelationContextNr ) )
												{
												if( ( generalizationContextNr = adminItem_->highestContextNr() ) < MAX_CONTEXT_NR )
													generalizationSingleContextNr_ = ++generalizationContextNr;
												else
													return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Context number overflow" );
												}
											else
												// Found context is single context
												generalizationContextNr = foundRelationContextNr;
											}
										else
											generalizationContextNr = generalizationSingleContextNr_;
										}
									}
								else
									{
									// Multiple entry context
									if( nContextRelations > 1 )
										{
										// Find correct generalization context, which will become the relation context
										if( ( contextResult = adminItem_->getRelationContext( false, isExclusiveSpecification, isNegative, !isPossessive, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, NULL ) ).result == RESULT_OK )
											generalizationContextNr = contextResult.contextNr;
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get the relation context number" );
										}
									}
								}

							existingConclusionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, !isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

							// Conclusion doesn't exist yet as user specification
							if( ( existingConclusionSpecificationItem == NULL ||
							// Allow replacing exclusive generalization conclusion
							existingConclusionSpecificationItem->isInactiveAssignment() ||
							// Allow existing conclusion to change its relation context
							existingConclusionSpecificationItem->isSelfGenerated() ) &&

							// Find the non-reversible specification (active assignments)
							( secondarySpecificationItem = generalizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, true, false, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
								{
								if( !secondarySpecificationItem->isOlderItem() ||
								secondarySpecificationItem->assumptionLevel() != userAssumptionLevel ||
								myWordItem_->hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) )
									{
									// Archived assignments
									if( ( tempSpecificationItem = generalizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, false, true, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
										{
										if( secondarySpecificationItem->isAssignment() )
											hasFoundSecondaryArchivedAssignment = true;
										else
											secondarySpecificationItem = tempSpecificationItem;
										}

									// Check if user specification exists
									userSpecificationItem = relationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationWordItem );

									if( userSpecificationItem == NULL ||
									// Ambiguous specification: different tense (archived means: past tense)
									userSpecificationItem->isArchivedAssignment() != hasFoundSecondaryArchivedAssignment )
										{
										isConclusionAssignment = ( ( isAssignment &&
																	isAssumption ) ||

																	( !isAssumption &&

																	( ( isAssignment &&
																	userSpecificationItem == NULL ) ||

																	secondarySpecificationItem->hasExclusiveGeneralizationCollection() ) ) );

										// Draw a possessive reversible conclusion
										// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
										if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( isConclusionAssignment, isArchivedAssignment, false, isExclusiveSpecification, isSpecificationCollectedWithItself, isNegative, false, !isPossessive, isUniqueUserRelation, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, secondarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, NULL, NULL, secondarySpecificationItem, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result == RESULT_OK )
											{
											hasCreatedPossessiveReversibleConclusion_ = true;

											if( contextResult.replaceContextNr == NO_CONTEXT_NR )
												{
												if( ( createdOrFoundSpecificationItem = ( specificationResult.createdSpecificationItem == NULL ? specificationResult.foundSpecificationItem : specificationResult.createdSpecificationItem ) ) != NULL )
													{
													if( isPossessive )
														{
														if( adminItem_->makePossessiveConditionalSpecificationAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, false, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, createdOrFoundSpecificationItem, NULL, generalizationWordItem, specificationWordItem, relationWordItem ) == RESULT_OK )
															{
															if( ( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem->collectedWithItselfCommonWordItem() ) != NULL )
																{
																if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem->firstSpecificationGeneralizationItem() ) != NULL )
																	{
																	do	{
																		if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
																			{
																			if( ( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != NULL )
																				{
																				// Draw a "part of" generalization conclusion
																				// Not yet explained in the design
																				if( foundSpecificationItem->isPartOf() )
																					{
																					if( adminItem_->addSelfGeneratedSpecification( isConclusionAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, foundSpecificationItem, NULL, relationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
																						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated reversible part-of specification to generalization word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			}
																		else
																			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
																		}
																	while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
																	}
																}
															}
														else
															return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														if( generalizationWordItem->isUserGeneralizationWord &&
														specificationWordItem->isUserSpecificationWord )
															{
															if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, true, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, generalizationContextNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with generalization word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												else
													return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "No specification item was found or created" );
												}
											else
												{
												if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
													{
													foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

													if( foundSpecificationItem->isSelfGeneratedAssumption() )
														{
														if( relationWordItem->replaceOlderSpecificationByConcludedAssumption( contextResult.replaceContextNr, foundSpecificationItem ) == RESULT_OK )
															{
															if( ( obsoleteAssumptionJustificationItem = relationWordItem->obsoleteAssumptionJustificationItem() ) != NULL )
																{
																if( relationWordItem->replaceOrDeleteJustification( obsoleteAssumptionJustificationItem ) != RESULT_OK )
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace older conclusion specifications in relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find an older specification to replace" );
												}
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem() ) != NULL )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&
								currentGeneralizationItem->generalizationWordTypeNr() == generalizationWordTypeNr &&
								( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
									{
									if( !foundSpecificationItem->hasSpecificationCollection() &&
									!foundSpecificationItem->isExclusiveSpecification() )
										{
										assumptionLevel = ( foundSpecificationItem->assumptionLevel() > secondarySpecificationItem->assumptionLevel() ? foundSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

										if( adminItem_->collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) == RESULT_OK )
											{
											if( drawSpecificationGeneralizationConclusion( assumptionLevel, foundSpecificationItem ) == RESULT_OK )
												{
												if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != RESULT_OK )
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );
										}
									}
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
							}
						while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool hasRelationContext;
		bool isExclusiveCurrentSpecification;
		bool isGeneralizationFeminineOrMasculineWord;
		bool isSpecificationCollectedWithItself;
		bool isUserSpecificationWord;
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		bool isDefinitionSpecification = adminItem_->isSingularOrPluralNoun( generalizationWordTypeNr );
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isNegativeConclusion = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		unsigned int currentRelationContextNr;
		unsigned int currentSpecificationCollectionNr;
		JustificationItem *obsoleteAssumptionJustificationItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstFeminineOrMasculineSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *collectedWithItselfSpecificationItem = NULL;
		WordItem *currentSpecificationWordItem;
		char *currentSpecificationString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( adminItem_->isReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
						isSpecificationCollectedWithItself = specificationWordItem->isCollectedWithItself();

						// Skip definition sentence with the same generalization and specification word
						if( !hasFoundSpecificationWordCollectedWithItself_ ||
						!hasCorrectedAssumptionByKnowledge )
							{
							if( adminItem_->makeGeneralizationAssumptionBySpecification( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) == RESULT_OK )
								{
								if( isSpecificationCollectedWithItself )
									hasFoundSpecificationWordCollectedWithItself_ = true;
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption by specification word \"", specificationWordItem->anyWordTypeString(), "\" in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}

						if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
							{
							hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

							hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
							hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
							hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

							isGeneralizationFeminineOrMasculineWord = generalizationWordItem->isFeminineOrMasculineWord();
							isUserSpecificationWord = specificationWordItem->isUserSpecificationWord;

							if( isSpecificationCollectedWithItself )
								collectedWithItselfSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

							do	{
								if( currentSpecificationItem->isSpecificationNoun() &&
								!currentSpecificationItem->isSpecificationGeneralization() &&
								currentSpecificationItem->isNegative() == isNegative &&
								currentSpecificationItem->isPossessive() == isPossessive &&
								( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
									{
									questionParameter_ = NO_QUESTION_PARAMETER;

									isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
									currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
									currentRelationContextNr = currentSpecificationItem->relationContextNr();

									currentSpecificationString = currentSpecificationItem->specificationString();

									if( ( bestMatchingRelationSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL )
										{
										if( !isQuestion )
											isNegativeConclusion = true;
										}

									if( ( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == NULL )
										{
										// Check for more conflicts
										if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), currentRelationContextNr, currentSpecificationWordItem, currentSpecificationString ) != RESULT_OK )
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									else
										{
										if( !hasGeneralizationWordActiveQuestionWithCompoundCollection &&
										isExclusiveCurrentSpecification &&
										!isGeneralizationFeminineOrMasculineWord &&

										// Matching feminine word
										( ( hasGeneralizationWordFeminineProperNameEnding &&
										currentSpecificationWordItem->isFeminineWord() ) ||

										// Matching masculine word
										( hasGeneralizationWordMasculineProperNameEnding &&
										currentSpecificationWordItem->isMasculineWord() ) ) &&

										// Select correct compound word to avoid assumption to be made if a question should be asked
										currentSpecificationWordItem->compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem &&
										// Skip creating feminine or masculine proper name ending assumption if conclusion already exists
										generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( isArchivedAssignment, false, false, false, currentSpecificationWordItem ) == NULL )
											{
											if( ( firstFeminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL )
												{
												hasRelationContext = ( foundSpecificationItem->hasRelationContext() &&
																		firstFeminineOrMasculineSpecificationItem->isExclusiveSpecification() );

												// Make a feminine or masculine proper name ending assumption
												// Not yet explained in the design
												if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION, JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION, foundSpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationContext ? foundSpecificationItem->relationWordTypeNr() : WORD_TYPE_UNDEFINED ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, foundSpecificationItem, currentSpecificationItem, firstFeminineOrMasculineSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, ( hasRelationContext ? relationWordItem : NULL ) ) ).result == RESULT_OK )
													{
													if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
														{
														if( hasRelationContext &&
														generalizationWordItem->isUserGeneralizationWord )
															{
															createdFeminineOrMasculineSpecificationItem_ = createdSpecificationItem->updatedSpecificationItem();
															foundFeminineOrMasculineSpecificationItem_ = foundSpecificationItem->updatedSpecificationItem();
															}

														// Has missed drawing some negative conclusions
														if( missingNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr )
															{
															// Draw missing negative conclusions
															if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												else
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
												}
											else
												return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find a feminine or masculine specification item in current specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
											}

										if( !isExclusiveCurrentSpecification ||
										bestMatchingRelationSpecificationItem == NULL )
											{
											if( isGeneralizationProperName ||
											currentSpecificationCollectionNr > NO_COLLECTION_NR )
												{
												if( ( isExclusiveSpecification &&
												isGeneralizationProperName ) ||

												( !isExclusiveSpecification &&
												isExclusiveCurrentSpecification ) )
													{
													if( isGeneralizationProperName &&
													currentSpecificationCollectionNr > NO_COLLECTION_NR )
														{
														// Ask a question or draw a negative conclusion
														if( !isNegative &&
														!isPossessive &&
														!specificationWordItem->isUserGeneralizationWord &&

														( isNegativeConclusion ||

														// Skip invalid compound feminine/masculine questions,
														// but allow those questions to be adjusted
														( ( !isExclusiveCurrentSpecification ||

														( !foundSpecificationItem->isQuestion() &&
														!currentSpecificationWordItem->isUserSpecificationWord ) ) &&

														( adminItem_->hasFoundSuggestiveQuestionAssumption() ||
														generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() ||

														( ( !isSpecificationCollectedWithItself ||
														!isUserSpecificationWord ) &&

														( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||
														foundSpecificationItem->hasSpecificationCollection() ||
														adminItem_->isExclusiveUserSpecification() ||
														// If specification is collected with itself
														currentSpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == NO_COLLECTION_NR ) ) ) ) ) )
															{
															if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundSpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												else	// Make an assumption or draw a conclusion
													{
													if( isAssumption ||
													currentSpecificationItem->hasAssumptionLevel() )
														{
														if( isArchivedAssignment )
															primarySpecificationItem = foundSpecificationItem;
														else
															{
															if( isSpecificationCollectedWithItself ||
															( primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, isNegative, isPossessive, questionParameter, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
																primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, isNegative, isPossessive, questionParameter, specificationWordItem );
															}

														if( primarySpecificationItem != NULL )
															{
															if( ( !isSpecificationCollectedWithItself ||

															( !foundSpecificationItem->isOlderItem() &&

															( commonVariables_->isUserQuestion ||
															foundSpecificationItem->isUserSpecification() ||

															( currentSpecificationCollectionNr == NO_COLLECTION_NR &&
															generalizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, false, false, isNegative, isPossessive, specificationWordItem->compoundCollectionNr( specificationWordTypeNr ), generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL ) == NULL ) ) ) ) &&

															// Skip if already exists as user specification
															generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL )
																{
																// Make a compound specification substitution assumption, which is actually an uncertain conclusion
																// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, isSpecificationCollectedWithItself, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result == RESULT_OK )
																	{
																	if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
																		{
																		if( relationWordItem != NULL &&
																		relationWordItem->isUserGeneralizationWord )
																			{
																			if( foundSpecificationItem->addFeminineOrMasculineContext( relationWordItem ) != RESULT_OK )
																				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a context to generalization word \"", relationWordItem->anyWordTypeString(), "\"" );
																			}

																		if( ( obsoleteAssumptionJustificationItem = generalizationWordItem->obsoleteAssumptionJustificationItem() ) != NULL )
																			{
																			if( generalizationWordItem->replaceOrDeleteJustification( obsoleteAssumptionJustificationItem ) != RESULT_OK )
																				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																			}
																		}
																	}
																else
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The primary specification item is undefined" );
														}
													else
														{
														if( ( !isExclusiveSpecification &&

														( !isSpecificationCollectedWithItself ||

														( collectedWithItselfSpecificationItem != NULL &&
														!collectedWithItselfSpecificationItem->isOlderItem() &&
														!generalizationWordItem->hasFeminineProperNameEnding() ) ) &&

														// Skip if already exists as user specification
														generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) ||

														// Check if exclusive conclusion already exists
														( isExclusiveSpecification &&
														generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isArchivedAssignment, isNegative, isPossessive, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) )
															{
															if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
																{
																if( !primarySpecificationItem->isPartOf() )
																	{
																	if( lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ != NULL &&
																	lastCreatedSpecificationSubstitutionConclusionSpecificationItem_->generalizationWordItem() == generalizationWordItem )
																		currentSpecificationCollectionNr = lastCreatedSpecificationSubstitutionConclusionSpecificationItem_->specificationCollectionNr();

																	// Draw a compound specification substitution conclusion
																	// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																	//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																	if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, primarySpecificationItem->isEveryGeneralization(), isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result == RESULT_OK )
																		{
																		if( ( lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = specificationResult.createdSpecificationItem ) != NULL )
																			{
																			if( hasCorrectedAssumptionByKnowledge )
																				{
																				if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ ) ).result == RESULT_OK )
																					{
																					if( specificationResult.relatedSpecificationItem != NULL )
																						{
																						// Replace incorrect assumption
																						if( generalizationWordItem->replaceOrDeleteSpecification( specificationResult.relatedSpecificationItem, lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ ) != RESULT_OK )
																							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace a corrected specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																						}
																					}
																				else
																					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created specification" );
																				}
																			else
																				{
																				if( !isGeneralizationProperName )
																					{
																					if( adminItem_->makeGeneralizationAssumptionBySpecification( isArchivedAssignment, isNegative, isPossessive, currentSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption in generalization word \"", specificationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			}
																		}
																	else
																		return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );
															}
														}
													}

												if( !commonVariables_->hasShownWarning &&
												currentSpecificationCollectionNr > NO_COLLECTION_NR &&

												( !isExclusiveCurrentSpecification ||

												( isDefinitionSpecification &&
												isExclusiveSpecification ) ) )
													{
													if( adminItem_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, isNegative, isPossessive, questionParameter_, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption by generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												{
												if( !isExclusiveSpecification &&
												isGeneralizationProperName &&
												!isPossessive &&
												currentSpecificationItem->isUserSpecification() )
													{
													if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), currentRelationContextNr, currentSpecificationWordItem, currentSpecificationString ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
							}
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning type" );

		return RESULT_OK;
		}

	SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( bool isNegative, bool isPartOf, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *previousSpecificationItem = NULL;
		SpecificationItem *lastCreatedSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( specificationWordItem != NULL )
				{
				do	{
					if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem() ) != NULL )
						{
						previousSpecificationItem = lastCreatedSpecificationItem;

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( ( foundSpecificationItem = currentGeneralizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
									{
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), foundSpecificationItem->isInactiveAssignment(), foundSpecificationItem->isArchivedAssignment(), foundSpecificationItem->isExclusiveSpecification(), isNegative, isPartOf, isPossessive, NO_QUESTION_PARAMETER, foundSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) == RESULT_OK )
										{
										if( !commonVariables_->hasShownWarning )
											{
											if( ( specificationResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result == RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
													{
													if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isInactiveAssignment(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), isNegative, isPartOf, isPossessive, relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) == RESULT_OK )
														specificationResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );
											}
										}
									else
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
							}
						while( commonVariables_->result == RESULT_OK &&
						!commonVariables_->hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
						}
					}
				while( commonVariables_->result == RESULT_OK &&
				!commonVariables_->hasShownWarning &&
				previousSpecificationItem != NULL );	// Do until no more conclusions or questions are created
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	SpecificationItem *createdFeminineOrMasculineSpecificationItem()
		{
		return createdFeminineOrMasculineSpecificationItem_;
		}

	SpecificationItem *foundFeminineOrMasculineSpecificationItem()
		{
		return foundFeminineOrMasculineSpecificationItem_;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
