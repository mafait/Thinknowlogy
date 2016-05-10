/*	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously draw grammar-based conclusions
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"

class AdminConclusion
	{
	friend class AdminItem;

	// Private constructible variables

	bool isSpanishAmbiguousCompoundSpecification_;

	unsigned int postponeNegativeConclusionSpecificationCollectionNr_;

	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
	SpecificationItem *postponeNegativeConclusionPrimarySpecificationItem_;
	SpecificationItem *postponeNegativeConclusionAnotherPrimarySpecificationItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearPostponedNegativeConclusionVariables()
		{
		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		}

	ResultType askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegativeConclusion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool isCommonWordWordSpanishAmbiguous;
		bool isHiddenSpanishPrimarySpecification;
		bool isSpecificationWordSpanishAmbiguous;
		bool isFemaleUserSpecificationWord = false;
		bool isMaleUserSpecificationWord = false;
		unsigned int anotherPrimarySpecificationCollectionNr;
		unsigned int nonCompoundCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *firstNonCompoundCollectionSpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *commonWordItem;
		WordItem *userSpecificationWordItem;
		WordItem *collectionWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

		if( primarySpecificationItem != NULL )
			{
			if( anotherPrimarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
							{
							if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) != NULL )
								{
								if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
									{
									if( ( commonWordItem = anotherPrimarySpecificationWordItem->commonWordItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
										{
										if( ( specificationCollectionNr = commonWordItem->collectionNr( specificationWordTypeNr, specificationWordItem ) ) == NO_COLLECTION_NR )
											specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

										if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result == RESULT_OK )
											{
											isCommonWordWordSpanishAmbiguous = commonWordItem->isNounWordSpanishAmbiguous();
											isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
											isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

											relatedSpecificationItem = specificationResult.relatedSpecificationItem;

											// Current question part isn't related to an answer or a similar question
											if( isCommonWordWordSpanishAmbiguous &&
											relatedSpecificationItem == NULL &&
											!userSpecificationItem->isPossessive() &&
											( nonCompoundCollectionNr = commonWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
												{
												if( nonCompoundCollectionNr != specificationCollectionNr )
													{
													if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, nonCompoundCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result == RESULT_OK )
														relatedSpecificationItem = specificationResult.relatedSpecificationItem;
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );
													}
												}

											if( relatedSpecificationItem == NULL )
												{
												hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
												hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

												if( userSpecificationItem != primarySpecificationItem &&
												!userSpecificationItem->hasRelationContext() &&
												!userSpecificationItem->isExclusiveSpecification() &&
												commonVariables_->nUserSpecificationWords == 1 &&
												( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
													{
													if( userSpecificationWordItem->isFemale() )
														isFemaleUserSpecificationWord = true;

													if( userSpecificationWordItem->isMale() )
														isMaleUserSpecificationWord = true;
													}

												// Negative feminine conclusion
												if( isFemaleUserSpecificationWord ||
												isMaleUserSpecificationWord ||

												( hasGeneralizationWordMasculineProperNameEnding &&
												anotherPrimarySpecificationWordItem->isFemale() ) ||

												// Negative masculine conclusion
												( hasGeneralizationWordFeminineProperNameEnding &&
												anotherPrimarySpecificationWordItem->isMale() ) ||

												// Specification is Spanish ambiguous
												( isSpecificationWordSpanishAmbiguous &&
												!generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&

												( hasGeneralizationWordFeminineProperNameEnding ||
												adminItem_->isUserQuestion() ) ) )
													{
													if( isFemaleUserSpecificationWord ||
													isMaleUserSpecificationWord )
														{
														if( anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&

														( anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||

														( isFemaleUserSpecificationWord &&
														anotherPrimarySpecificationWordItem->isMale() ) ||

														( isMaleUserSpecificationWord &&
														anotherPrimarySpecificationWordItem->isFemale() ) ) )
															{
															if( drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														{
														if( !isHiddenSpanishPrimarySpecification ||
														!isSpecificationWordSpanishAmbiguous )
															{
															postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
															postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
															postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
															}
														}
													}
												else
													{
													// Skip invalid questions (step 1)
													if( ( ( !hasGeneralizationWordFeminineProperNameEnding &&
													!hasGeneralizationWordMasculineProperNameEnding ) ||

													primarySpecificationItem->hasSpecificationCompoundCollection() ) &&

													( !isSpecificationWordSpanishAmbiguous ||
													anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
													anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
														{
														if( isSpecificationWordSpanishAmbiguous )
															existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, specificationWordItem );
														else
															{
															if( !isNegativeConclusion )
																collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr, NULL );
															}

														// Skip invalid questions (step 2)
														if( ( !isNegativeConclusion &&
														collectionWordItem != NULL &&
														generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, collectionWordItem ) == NULL ) ||

														( isSpecificationWordSpanishAmbiguous &&

														( generalizationWordItem->isUserGeneralizationWord ||
														existingSpecificationItem == NULL ||
														existingSpecificationItem->isHiddenSpanishSpecification() ) ) )
															{
															if( isSpecificationWordSpanishAmbiguous &&
															!primarySpecificationItem->isExclusiveSpecification() &&
															primarySpecificationItem->isUserSpecification() )
																{
																postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
																postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
																postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
																}
															else
																{
																if( primarySpecificationItem->hasRelationContext() ||
																primarySpecificationItem->isUserSpecification() ||
																generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == NULL )
																	{
																	if( adminItem_->askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, NULL, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															}
														}
													}
												}
											else
												{
												if( anotherPrimarySpecificationItem->isExclusiveSpecification() )
													{
													if( !isCommonWordWordSpanishAmbiguous &&
													( firstNonCompoundCollectionSpecificationItem = generalizationWordItem->firstNonCompoundCollectionSpecificationItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
														relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

													if( postponeNegativeConclusionSpecificationCollectionNr_ == anotherPrimarySpecificationCollectionNr )
														{
														if( drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, NULL, generalizationWordItem ) == RESULT_OK )
															clearPostponedNegativeConclusionVariables();
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}

													clearPostponedNegativeConclusionVariables();

													if( isCommonWordWordSpanishAmbiguous &&
													isSpecificationWordSpanishAmbiguous &&
													// specificationWordItem == commonWordItem &&
													commonVariables_->nUserSpecificationWords == 1 &&
													!primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
													primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr &&

													( primarySpecificationItem->isUserSpecification() ||
													generalizationWordItem->isUserRelationWord ) )
														{
														questionSpecificationItem = generalizationWordItem->firstActiveAssignmentOrSpecificationItem( isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem );

														if( ( questionSpecificationItem == NULL ||
														questionSpecificationItem->isOlderItem() ) &&

														generalizationWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem ) == NULL )
															{
															if( adminItem_->askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, questionSpecificationItem, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );
															}
														}
													else
														{
														if( anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
														anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||

														( !relatedSpecificationItem->isHiddenSpanishSpecification() &&
														anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
															{
															if( drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) == RESULT_OK )
																{
																if( isSpanishAmbiguousCompoundSpecification_ )
																	{
																	spanishAmbiguousCompoundPrimarySpecificationItem_ = relatedSpecificationItem;
																	spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
																	}
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									else
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the common word of the given specification collection number" );
									}
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification word item" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification collection" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawNegativeConclusion( bool isArchivedAssignment, bool isSpecificationWordSpanishAmbiguous, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isPrimarySameAsSecondarySpecification;
		bool isHiddenSpanishPrimarySpecification;
		bool isUserGeneralizationWord;
		bool hasAnotherSecondarySpecificationWithCompoundCollection = false;
		bool hasFoundOldSpanishUserSpecification = false;
		bool hasRelatedSpecificationsInSameSentence = false;
		bool isCompoundUserSpecification = false;
		bool isUserSpecificationSelected = false;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned int nonCompoundCollectionNr;
		unsigned int secondarySpecificationCollectionNr;
		unsigned int anotherPrimarySpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *createdJustificationItem;
		JustificationItem *negativeAssumptionJustificationItem;
		JustificationItem *negativeAssumptionOrConclusionJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingNegativeSpecificationItem;
		SpecificationItem *existingPositiveSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *negativeArchivedAssignmentItem;
		SpecificationItem *negativeConclusionPrimarySpecificationItem;
		SpecificationItem *negativeConclusionAnotherPrimarySpecificationItem;
		SpecificationItem *negativeConclusionSecondarySpecificationItem;
		SpecificationItem *negativeConclusionAnotherSecondarySpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *foundNegativeAssumptionOrConclusionPrimarySpecificationItem = NULL;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *collectionWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *secondarySpecificationCommonWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusion";

		if( primarySpecificationItem != NULL )
			{
			if( secondarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( !primarySpecificationItem->isPossessive() &&
					!primarySpecificationItem->isQuestion() )
						{
						if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
							{
							if( anotherPrimarySpecificationItem == NULL )
								anotherPrimarySpecificationWordItem = primarySpecificationWordItem;
							else
								{
								anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr();
								anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem();
								}

							if( anotherPrimarySpecificationWordItem != NULL &&
							( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
								{
								// If exists, replace self-generated assumption for conclusion
								if( primarySpecificationItem->isSelfGeneratedAssumption() &&
								( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem->isArchivedAssignment(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), false, primarySpecificationWordItem, NULL ) ) != NULL )
									primarySpecificationItem = tempSpecificationItem;

								isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();

								secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

								existingPositiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

								if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != NULL )
									foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem->primarySpecificationItem();

								// No positive specification exists with this specification word
								if( ( existingPositiveSpecificationItem == NULL ||
								existingPositiveSpecificationItem->isHiddenSpanishSpecification() ) &&

								// Prefer primary specification with the least uncertainty
								( foundNegativeAssumptionOrConclusionPrimarySpecificationItem == NULL ||
								!primarySpecificationItem->hasSpecificationCollection() ||
								primarySpecificationItem->assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem->assumptionLevel() ) &&

								// Select correct compound specification collection when Spanish ambiguous
								( !isSpecificationWordSpanishAmbiguous ||
								generalizationWordItem->isUserRelationWord ||
								anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ||

								( anotherPrimarySpecificationItem != NULL &&
								anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ) ) &&

								// No question exists with this specification word
								generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == NULL )
									{
									if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
										{
										isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );
										isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

										if( isPrimarySameAsSecondarySpecification )
											{
											if( primarySpecificationItem->hasSpecificationCompoundCollection() &&
											primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
												{
												collectionWordItem = collectionWordItemInAllWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem );

												if( collectionWordItem != NULL &&
												( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, collectionWordItem ) ) == NULL )
													{
													if( ( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == NULL )
														{
														if( userSpecificationItem->isSpecificationWordSpanishAmbiguous() )
															anotherSecondarySpecificationItem = primarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, collectionWordItem );
														else
															anotherSecondarySpecificationItem = userSpecificationItem;
														}
													}
												}
											else
												{
												if( commonVariables_->nUserSpecificationWords > 1 &&
												commonVariables_->nUserRelationWords == 0 &&
												userSpecificationItem->isSpecificationWordSpanishAmbiguous() )
													hasRelatedSpecificationsInSameSentence = true;
												}
											}
										else
											{
											if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
												{
												if( isHiddenSpanishPrimarySpecification &&
												generalizationWordItem->isUserRelationWord &&
												anotherPrimarySpecificationItem != NULL &&
												anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&
												anotherPrimarySpecificationItem->isSelfGenerated() &&
												( collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr, NULL ) ) != NULL )
													{
													if( ( nonCompoundCollectionNr = collectionWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
														nonCompoundCollectionNr = anotherPrimarySpecificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );

													if( nonCompoundCollectionNr > NO_COLLECTION_NR &&
													( tempSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, nonCompoundCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL ) ) != NULL )
														{
														hasFoundOldSpanishUserSpecification = true;
														secondarySpecificationItem = tempSpecificationItem;
														}
													}

												if( !hasFoundOldSpanishUserSpecification &&
												( anotherSecondarySpecificationItem = secondarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL ) ) == NULL )
													{
													if( secondarySpecificationItem->hasSpecificationCompoundCollection() &&
													secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
													( secondarySpecificationCommonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL )
														{
														if( secondarySpecificationCommonWordItem != primarySpecificationWordItem )
															anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem );
														}
													}
												}
											}

										if( isUserGeneralizationWord )
											{
											if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
												{
												if( userSpecificationWordItem->isNounWordSpanishAmbiguous() )
													isUserSpecificationWordSpanishAmbiguous = true;

												if( !isHiddenSpanishPrimarySpecification ||

												( !userSpecificationItem->isHiddenSpanishSpecification() &&

												( isUserSpecificationWordSpanishAmbiguous ||
												userSpecificationItem->isPossessive() ) ) )		
													{
													isUserSpecificationSelected = true;

													if( !isPrimarySameAsSecondarySpecification &&
													anotherSecondarySpecificationItem != NULL &&
													userSpecificationItem->hasSpecificationCompoundCollection() &&
													userSpecificationWordItem->hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
														isCompoundUserSpecification = true;
													}
												else
													{
													if( isSpecificationWordSpanishAmbiguous &&
													!anotherPrimarySpecificationWordItem->isNounWordSpanishAmbiguous() &&
													( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL )
														{
														isUserSpecificationSelected = true;
														userSpecificationItem = simpleUserSpecificationItem;

														if( isHiddenSpanishPrimarySpecification &&
														anotherSecondarySpecificationItem != NULL &&
														!anotherSecondarySpecificationItem->isGeneralizationProperName() &&
														anotherPrimarySpecificationCollectionNr == anotherSecondarySpecificationItem->specificationCollectionNr() )
															hasAnotherSecondarySpecificationWithCompoundCollection = true;
														}
													}

												if( isUserSpecificationSelected &&
												userSpecificationItem->isPossessive() )
													oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem();
												}
											}

										existingNegativeSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

										if( hasFoundOldSpanishUserSpecification ||
										isCompoundUserSpecification ||

										( ( existingNegativeSpecificationItem == NULL ||

										( !isHiddenSpanishPrimarySpecification &&

										( existingNegativeSpecificationItem->isSelfGeneratedAssumption() ||
										primarySpecificationItem->hasSpecificationCompoundCollection() ) &&

										( existingNegativeSpecificationItem->isOlderItem() ||
										existingNegativeSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) &&

										generalizationWordItem->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, primarySpecificationItem ) == NULL ) ||

										( isUserGeneralizationWord &&
										isUserSpecificationSelected &&

										( ( !primarySpecificationItem->hasRelationContext() &&

										( existingNegativeSpecificationItem == NULL ||
										existingNegativeSpecificationItem->isOlderItem() ) ) ||

										( isHiddenSpanishPrimarySpecification &&
										existingNegativeSpecificationItem != NULL &&

										( existingNegativeSpecificationItem->isOlderItem() ||
										existingNegativeSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) ) )
											{
											if( isHiddenSpanishPrimarySpecification )
												{
												if( isUserSpecificationSelected )
													{
													if( hasAnotherSecondarySpecificationWithCompoundCollection )
														{
														negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
														negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
														negativeConclusionSecondarySpecificationItem = userSpecificationItem;
														negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ? anotherSecondarySpecificationItem : NULL );
														}
													else
														{
														if( ( isUserSpecificationWordSpanishAmbiguous &&
														userSpecificationItem->hasSpecificationNonCompoundCollection() ) ||

														( anotherPrimarySpecificationItem != NULL &&
														anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() ) )
															{
															negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
															negativeConclusionAnotherPrimarySpecificationItem = NULL;
															negativeConclusionSecondarySpecificationItem = secondarySpecificationItem;
															negativeConclusionAnotherSecondarySpecificationItem = anotherSecondarySpecificationItem;
															}
														else
															{
															negativeConclusionPrimarySpecificationItem = userSpecificationItem;
															negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
															negativeConclusionSecondarySpecificationItem = anotherPrimarySpecificationItem;
															negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationItem->isHiddenSpanishSpecification() ? userSpecificationItem : secondarySpecificationItem );
															}
														}
													}
												else
													{
													negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
													negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
													negativeConclusionSecondarySpecificationItem = secondarySpecificationItem;
													negativeConclusionAnotherSecondarySpecificationItem = anotherSecondarySpecificationItem;
													}
												}
											else	// Not isHiddenSpanishPrimarySpecification
												{
												if( !isPrimarySameAsSecondarySpecification &&
												isUserSpecificationSelected &&
												primarySpecificationItem->isSelfGenerated() &&
												primarySpecificationItem->hasSpecificationCompoundCollection() &&
												!userSpecificationItem->isQuestion() &&
												userSpecificationItem->hasSpecificationCompoundCollection() )
													compoundGeneralizationWordItem = anotherPrimarySpecificationWordItem->compoundGeneralizationWordItem( anotherPrimarySpecificationCollectionNr );

												if( compoundGeneralizationWordItem != NULL &&
												compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() )
													{
													isSpanishAmbiguousCompoundSpecification_ = true;

													negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
													negativeConclusionAnotherPrimarySpecificationItem = NULL;
													negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
													negativeConclusionAnotherSecondarySpecificationItem = NULL;
													}
												else
													{
													negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
													negativeConclusionAnotherPrimarySpecificationItem = ( hasRelatedSpecificationsInSameSentence ? NULL : anotherPrimarySpecificationItem );
													negativeConclusionSecondarySpecificationItem = ( isCompoundUserSpecification ? userSpecificationItem : ( isPrimarySameAsSecondarySpecification ? ( hasRelatedSpecificationsInSameSentence ? anotherPrimarySpecificationItem : ( primarySpecificationItem != userSpecificationItem && !userSpecificationItem->hasRelationContext() && userSpecificationItem->specificationCollectionNr() == anotherPrimarySpecificationCollectionNr ? userSpecificationItem : anotherSecondarySpecificationItem ) ) : secondarySpecificationItem ) );
													negativeConclusionAnotherSecondarySpecificationItem = ( isCompoundUserSpecification || isPrimarySameAsSecondarySpecification ? NULL : anotherSecondarySpecificationItem );
													}
												}

											// Draw a negative negativeConclusion
											// Not yet explained in the design
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem->isSelfGeneratedAssumption() ? JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result == RESULT_OK )
												{
												createdJustificationItem = specificationResult.createdJustificationItem;

												// Found specification
												if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
													{
													if( createdJustificationItem != NULL )
														{
														// Cleanup after found specification
														if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
															{
															if( negativeAssumptionOrConclusionJustificationItem == NULL )
																{
																if( foundSpecificationItem->isSelfGeneratedAssumption() )
																	{
																	if( createdJustificationItem->isConclusionJustification() )
																		{
																		if( foundSpecificationItem->markAsConcludedAssumption() == RESULT_OK )
																			{
																			if( isArchivedAssignment )
																				{
																				if( ( negativeArchivedAssignmentItem = generalizationWordItem->firstAssignmentItem( false, false, true, true, false, true, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) != NULL )
																					{
																					// Also mark the past tense negative assumption assignment as concluded
																					if( negativeArchivedAssignmentItem->markAsConcludedAssumption() != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark a past tense negative assumption assignment as concluded" );
																					}
																				else
																					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a past tense negative assumption assignment" );
																				}
																			}
																		else
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
																		}
																	}
																else
																	{
																	if( generalizationWordItem->isJustificationInUse( createdJustificationItem ) )
																		{
																		// Check for negative assumption justification
																		negativeAssumptionJustificationItem = foundSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION );

																		if( negativeAssumptionJustificationItem != NULL &&
																		negativeAssumptionJustificationItem->primarySpecificationItem() == negativeConclusionPrimarySpecificationItem )
																			{
																			if( generalizationWordItem->replaceJustification( negativeAssumptionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																			}
																		}
																	else
																		{
																		if( generalizationWordItem->replaceOrDeleteJustification( createdJustificationItem ) != RESULT_OK )
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the created justification" );
																		}
																	}
																}
															else
																{
																// Replace justification of negative conclusion / assumption
																if( generalizationWordItem->replaceJustification( negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item is undefined" );
														}
													}
												else
													{
													// Created specification
													// Replace corrected negative conclusion
													if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
														{
														if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result == RESULT_OK )
															{
															relatedSpecificationItem = specificationResult.relatedSpecificationItem;

															if( relatedSpecificationItem != NULL &&
															relatedSpecificationItem->isOlderItem() )
																{
																if( generalizationWordItem->replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created negative conclusion" );
														}
													else
														{
														if( isCompoundUserSpecification &&
														!isPrimarySameAsSecondarySpecification &&
														anotherSecondarySpecificationItem != NULL &&
														// Check for answered question justification
														createdSpecificationItem->answeredQuestionJustificationItem() == NULL )
															{
															if( generalizationWordItem->changeJustificationOfNegativeAssumptions( userSpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", anotherPrimarySpecificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									else
										return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );
									}
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item shouldn't be possessive, nor a question, nor a hidden specification" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		bool hasFeminineProperNameEnding;
		bool isAssignment;
		bool isConclusionAssignment;
		bool isForcingNewJustification;
		bool isSecondarySpecificationWordSpanishAmbiguous;
		bool isAssumption = ( userAssumptionLevel > NO_ASSUMPTION_LEVEL );
		unsigned int foundRelationContextNr;
		unsigned int nContextRelations;
		unsigned int generalizationContextNr = NO_CONTEXT_NR;
		unsigned int specificationCollectionNr = NO_COLLECTION_NR;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *collectedWithItselfCommonWordItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( relationContextNr > NO_CONTEXT_NR )
			{
			if( generalizationWordItem != NULL )
				{
				if( secondarySpecificationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( relationWordItem != NULL )
							{
							existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, !isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

							if( existingSpecificationItem == NULL ||
							// Allow existing conclusion specification to change its relation context
							existingSpecificationItem->isSelfGenerated() )
								{
								if( ( foundRelationContextNr = generalizationWordItem->contextNr( specificationWordItem ) ) > NO_CONTEXT_NR )
									{
									if( ( nContextRelations = adminItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
										{
										if( ( contextResult = adminItem_->getRelationContext( isArchivedAssignment, false, !isPossessive, false, false, specificationWordTypeNr, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, NULL ) ).result == RESULT_OK )
											generalizationContextNr = contextResult.contextNr;
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the relation context number" );
										}
									else
										{
										if( existingSpecificationItem != NULL )
											generalizationContextNr = foundRelationContextNr;
										}
									}

								isSecondarySpecificationWordSpanishAmbiguous = secondarySpecificationWordItem->isNounWordSpanishAmbiguous();

								conclusionSpecificationItem = contextResult.conclusionSpecificationItem;
								existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Find the non-reversible specification (active assignments)
								if( isSecondarySpecificationWordSpanishAmbiguous )
									secondarySpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );
								else
									secondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if possessive user specification exists
								if( secondarySpecificationItem != NULL &&

								( !isSecondarySpecificationWordSpanishAmbiguous ||
								secondarySpecificationItem->hasSpecificationNonCompoundCollection() ||
								existingUserSpecificationItem == NULL ||
								!existingUserSpecificationItem->hasRelationContext() ) )
									{
									if( ( existingSpecificationItem == NULL ||
									secondarySpecificationItem->isUserSpecification() ||
									relationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, secondarySpecificationItem ) == NULL ) &&

									secondarySpecificationItem->hasNewInformation() )
										{
										hasFeminineProperNameEnding = ( isPossessive &&
																		isSecondarySpecificationWordSpanishAmbiguous &&
																		relationWordItem->hasFeminineProperNameEnding() );

										isConclusionAssignment = ( ( isArchivedAssignment &&
																	isAssumption ) ||

																	( !isAssumption &&

																	( isArchivedAssignment ||
																	secondarySpecificationItem->hasExclusiveGeneralizationCollection() ) ) );

										isAssignment =				( isConclusionAssignment ||

																	( existingUserSpecificationItem != NULL &&
																	existingUserSpecificationItem->isAssignment() ) );

										isForcingNewJustification = ( isSecondarySpecificationWordSpanishAmbiguous ||

																	( !isPossessive &&
																	existingSpecificationItem != NULL &&

																	( conclusionSpecificationItem != NULL ||
																	secondarySpecificationItem->isActiveAssignment() ||
																	existingSpecificationItem->relationContextNr() != generalizationContextNr ) ) );

										if( isSecondarySpecificationWordSpanishAmbiguous &&
										secondarySpecificationWordItem == specificationWordItem &&

										( !isPossessive ||
										!relationWordItem->isMale() ) )
											specificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

										// Draw a possessive reversible conclusion
										// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
										if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( hasFeminineProperNameEnding, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, specificationCollectionNr, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, NULL, NULL, secondarySpecificationItem, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result == RESULT_OK )
											{
											if( conclusionSpecificationItem == NULL )
												{
												if( specificationResult.createdSpecificationItem == NULL )
													{
													if( relationWordItem->replaceOrDeleteObsoleteJustifications() == RESULT_OK )
														createdOrFoundSpecificationItem = specificationResult.foundSpecificationItem;
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace obsolete justification items in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
													}
												else
													createdOrFoundSpecificationItem = specificationResult.createdSpecificationItem;

												if( createdOrFoundSpecificationItem != NULL )
													{
													if( isPossessive )
														{
														if( adminItem_->makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, NULL, generalizationWordItem, relationWordItem ) == RESULT_OK )
															{
															if( isSecondarySpecificationWordSpanishAmbiguous &&
															( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem->collectedWithItselfCommonWordItem() ) != NULL )
																{
																// If the reversed specification doesn't already exist
																if( !isArchivedAssignment &&
																generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == NULL )
																	{
																	// Generate the reversed specification
																	if( adminItem_->drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem->relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																	}

																if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
																	{
																	do	{
																		if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
																			{
																			if( ( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != NULL )
																				{
																				if( foundSpecificationItem->isPartOf() &&

																				( !createdOrFoundSpecificationItem->hasAssumptionLevel() ||
																				// Skip if 'part of' conclusion exists
																				relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentGeneralizationWordItem, NULL ) == NULL ) )
																					{
																					// Draw a 'part of' generalization conclusion
																					// Not yet explained in the design
																					if( adminItem_->addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, foundSpecificationItem, NULL, relationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			}
																		else
																			return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
																		}
																	while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
																	}

																if( relationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) == NULL &&
																( secondarySpecificationItem = collectedWithItselfCommonWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, createdOrFoundSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
																	{
																	if( drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, NULL, createdOrFoundSpecificationItem, NULL, relationWordItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														// Not possessive
														if( secondarySpecificationItem->isUserSpecification() &&
														createdOrFoundSpecificationItem->isOlderItem() &&
														createdOrFoundSpecificationItem->isSelfGeneratedAssumption() &&
														adminItem_->nContextWordsInAllWords( createdOrFoundSpecificationItem->relationContextNr(), specificationWordItem ) == 1 )
															{
															if( createdOrFoundSpecificationItem->markAsConcludedAssumption() == RESULT_OK )
																{
																if( relationWordItem->writeUpdatedSpecification( true, true, false, false, createdOrFoundSpecificationItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the concluded assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
															}
														}

													if( generalizationWordItem->isUserGeneralizationWord &&
													specificationWordItem->isUserSpecificationWord )
														{
														if( isSecondarySpecificationWordSpanishAmbiguous &&
														( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
															{
															if( existingSpecificationItem->isHiddenSpanishSpecification() &&
															existingSpecificationItem->specificationCollectionNr() != createdOrFoundSpecificationItem->specificationCollectionNr() )
																{
																// Add context to hidden specification
																if( generalizationWordItem->addContext( false, relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem->relationContextNr(), specificationWordItem ) == RESULT_OK )
																	{
																	if( relationWordItem->writeUpdatedSpecification( true, false, false, false, existingSpecificationItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the hidden assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																	}
																else
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}

														if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													return adminItem_->startError( functionNameString, moduleNameString_, "No specification item was found or created" );
												}
											else
												{
												if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
													{
													if( foundSpecificationItem->isSelfGeneratedAssumption() )
														{
														if( foundSpecificationItem->markAsConcludedAssumption() == RESULT_OK )
															{
															// Write adjusted specification
															if( relationWordItem->writeUpdatedSpecification( true, true, false, false, foundSpecificationItem ) == RESULT_OK )
																{
																if( ( firstJustificationItem = conclusionSpecificationItem->firstJustificationItem() ) != NULL )
																	{
																	if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != NULL )
																		{
																		// Attach older conclusion justifications to the created conclusion justification item
																		if( createdJustificationItem->changeAttachedJustification( firstJustificationItem ) == RESULT_OK )
																			{
																			if( relationWordItem->replaceOrDeleteSpecification( conclusionSpecificationItem, foundSpecificationItem ) == RESULT_OK )
																				{
																				firstJustificationItem = foundSpecificationItem->firstJustificationItem();

																				if( firstJustificationItem != NULL &&
																				!firstJustificationItem->isPossessiveReversibleConclusion() )
																					{
																					if( firstJustificationItem->changeAttachedJustification( createdJustificationItem ) != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete obsolete assumption justification items in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																					}

																				if( relationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
																					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																				}
																			else
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																			}
																		else
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );
																		}
																	else
																		return adminItem_->startError( functionNameString, moduleNameString_, "The created conclusion justification item is undefined" );
																	}
																else
																	return adminItem_->startError( functionNameString, moduleNameString_, "The conclusion specification item has no justifications" );
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the replacing specification as concluded assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find an older specification to replace" );
												}
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation context number is undefined" );

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
				if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, true, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	WordItem *collectionWordItemInAllWords( unsigned int compoundCollectionNr, WordItem *commonWordItem )
		{
		WordItem *currentWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all active words
			do	{
				if( currentWordItem != commonWordItem &&
				currentWordItem->hasCollectionNr( compoundCollectionNr, commonWordItem ) )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	AdminConclusion( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		isSpanishAmbiguousCompoundSpecification_ = false;

		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminConclusion" );

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


	// Protected functions

	void initializeAdminConclusionVariables()
		{
		clearPostponedNegativeConclusionVariables();

		isSpanishAmbiguousCompoundSpecification_ = false;

		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		}

	bool hasFoundUnprocessedNegativeConclusion()
		{
		return ( postponeNegativeConclusionSpecificationCollectionNr_ > NO_COLLECTION_NR );
		}

	SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *previousSpecificationItem = NULL;
		SpecificationItem *lastCreatedSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( specificationWordItem != NULL )
				{
				do	{
					if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
						{
						previousSpecificationItem = lastCreatedSpecificationItem;

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
									{
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), foundSpecificationItem->isInactiveAssignment(), foundSpecificationItem->isArchivedAssignment(), foundSpecificationItem->isExclusiveSpecification(), true, NO_QUESTION_PARAMETER, foundSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) == RESULT_OK )
										{
										if( !commonVariables_->hasShownWarning )
											{
											if( ( specificationResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result == RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
													{
													if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isInactiveAssignment(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), true, relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) == RESULT_OK )
														compoundGeneralizationWordItem = currentGeneralizationWordItem;
													else
														adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );
											}
										}
									else
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
							}
						while( commonVariables_->result == RESULT_OK &&
						!commonVariables_->hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
						}
					}
				while( commonVariables_->result == RESULT_OK &&
				!commonVariables_->hasShownWarning &&
				// Do until no more conclusions or questions are created
				previousSpecificationItem != NULL );
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		specificationResult.compoundGeneralizationWordItem = compoundGeneralizationWordItem;
		specificationResult.result = commonVariables_->result;
		return specificationResult;
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
												if( !currentJustificationItem->hasPrimaryQuestion() &&
												!currentJustificationItem->hasPossessivePrimarySpecification() )
													{
													if( drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), false, answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
											}
										else
											return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no justification" );
										}
									}
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no specification word" );
								}
							}
						while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any answered question specification" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned int nonCompoundCollectionNr;
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem();
		WordItem *collectionWordItem;
		WordItem *commonWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSimpleNegativeSpanishConclusion";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( simpleUserSpecificationItem != NULL &&
				!specificationWordItem->isNounWordSpanishAmbiguous() &&
				( userSpecificationWordItem = simpleUserSpecificationItem->specificationWordItem() ) != NULL &&
				( nonCompoundCollectionNr = specificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
					{
					if( !userSpecificationWordItem->isNounWordSpanishAmbiguous() &&
					( collectionWordItem = specificationWordItem->collectionWordItem( nonCompoundCollectionNr, NULL ) ) != NULL )
						{
						if( collectionWordItem->isNounWordSpanishAmbiguous() )
							anotherPrimarySpecificationItem = collectionWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, collectionWordItem );
						else
							{
							userSpecificationCollectionNr = simpleUserSpecificationItem->specificationCollectionNr();

							if( userSpecificationCollectionNr > NO_COLLECTION_NR &&
							( commonWordItem = userSpecificationWordItem->commonWordItem( userSpecificationCollectionNr ) ) != NULL )
								{
								if( commonWordItem->isNounWordSpanishAmbiguous() )
									anotherPrimarySpecificationItem = commonWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, false, false, true, NO_QUESTION_PARAMETER, nonCompoundCollectionNr, NO_CONTEXT_NR, collectionWordItem );
								}
							}

						if( anotherPrimarySpecificationItem != NULL &&
						anotherPrimarySpecificationItem->hasSpecificationCollection() )
							{
							if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

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
									// Found the first option left
									foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;
								else
									{
									// There are more options left
									hasFoundMoreOptionsLeft = true;
									// So, clear the found option
									foundOnlyOptionLeftSpecificationItem = NULL;
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
						if( adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification collection number is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *feminineSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, specificationWordItem, relationWordItem ) == RESULT_OK )
						{
						if( isPossessive &&
						!isArchivedAssignment &&
						specificationWordItem->isNounWordSpanishAmbiguous() &&
						relationWordItem->hasFeminineProperNameEnding() &&
						( feminineSpecificationWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
							{
							if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion" );
							}
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&
								( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
									{
									if( !currentSpecificationItem->hasSpecificationCollection() &&
									!currentSpecificationItem->isExclusiveSpecification() )
										{
										assumptionLevel = ( secondarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

										if( adminItem_->collectGeneralizationWordWithPreviousOne( false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) == RESULT_OK )
											{
											if( drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) == RESULT_OK )
												{
												if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );
										}
									}
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
							}
						while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
						}
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool hasCurrentSpecificationCompoundCollection;
		bool hasFoundPrimarySpecificationNonCompoundCollection;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool isExclusiveCurrentSpecification;
		bool isFemaleOrMaleGeneralizationWord;
		bool isFoundPrimaryHiddenSpanishSpecification;
		bool isGeneralizationWordUserRelationWord;
		bool isIncludingRelationWord;
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		bool isExclusiveUserSpecification = false;
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isNegativeConclusion = false;
		bool isPossessiveUserSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned int currentSpecificationCollectionNr;
		unsigned int relationContextNr;
		SpecificationItem *alternativeAnotherPrimarySpecificationItem;
		SpecificationItem *alternativeJustificationSpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *collectedWithItselfSpecificationItem = NULL;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( isMakingPartOfAssumption )
							{
							if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}

						if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
							{
							hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

							hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
							hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
							hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

							isFemaleOrMaleGeneralizationWord = generalizationWordItem->isFemaleOrMale();
							isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
							isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
							isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

							oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem();

							if( isSpecificationWordSpanishAmbiguous )
								collectedWithItselfSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );

							if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
								{
								isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
								isPossessiveUserSpecification = userSpecificationItem->isPossessive();
								isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
								}

							do	{
								// Select non-possessive definition
								if( !currentSpecificationItem->isPossessive() &&
								currentSpecificationItem->isSpecificationNoun() &&
								!currentSpecificationItem->isSpecificationGeneralization() &&
								( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
									{
									isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
									currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();

									if( ( bestMatchingRelationSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, false, false, questionParameter, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL )
										{
										if( !isQuestion &&
										!bestMatchingRelationSpecificationItem->isExclusiveSpecification() )
											isNegativeConclusion = true;
										}

									if( ( foundPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isArchivedAssignment, false, false, questionParameter, specificationWordItem ) ) == NULL )
										{
										// Check for specification conflict
										if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									else
										{
										hasFoundPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem->hasSpecificationNonCompoundCollection();
										isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem->isHiddenSpanishSpecification();

										if( !commonVariables_->hasShownWarning &&
										isExclusiveCurrentSpecification &&
										!isFemaleOrMaleGeneralizationWord &&
										( feminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL )
											{
											// Matching feminine word
											if( ( ( hasGeneralizationWordFeminineProperNameEnding &&
											currentSpecificationWordItem->isFemale() ) ||

											// Matching masculine word
											( hasGeneralizationWordMasculineProperNameEnding &&
											currentSpecificationWordItem->isMale() ) ) &&

											( ( isFoundPrimaryHiddenSpanishSpecification &&
											isUserGeneralizationWord ) ||

											( !isFoundPrimaryHiddenSpanishSpecification &&

											( ( !isUserSpecificationWordSpanishAmbiguous &&
											!hasGeneralizationWordActiveQuestionWithCompoundCollection ) ||

											( !isPossessiveUserSpecification &&
											foundPrimarySpecificationItem->hasRelationContext() ) ) ) ) &&

											// Select correct compound word to avoid assumption to be made if a question should be asked
											currentSpecificationWordItem->compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem )
												{
												isIncludingRelationWord = ( hasRelationWord &&
																			feminineOrMasculineSpecificationItem->hasSpecificationCollection() );

												existingSpecificationItem = ( hasRelationWord ? generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem ) : NULL );

												if( !hasRelationWord ||
												isPossessiveUserSpecification ||

												( isIncludingRelationWord &&

												( existingSpecificationItem == NULL ||
												existingSpecificationItem->assumptionLevel() >= foundPrimarySpecificationItem->assumptionLevel() ) &&

												generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, currentSpecificationItem ) == NULL ) )
													{
													// Make a feminine or masculine proper name ending assumption
													// Not yet explained in the design
													if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem : NULL ), currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : NULL ) ) ).result == RESULT_OK )
														{
														if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
															{
															if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() == RESULT_OK )
																relationContextNr = ( specificationResult.foundSpecificationItem == NULL ? NO_CONTEXT_NR : specificationResult.foundSpecificationItem->relationContextNr() );
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justification items in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														else
															{
															relationContextNr = createdSpecificationItem->relationContextNr();

															if( !isUserSpecificationWordSpanishAmbiguous &&
															// Has missed drawing some negative conclusions
															( postponeNegativeConclusionSpecificationCollectionNr_ == NO_COLLECTION_NR ||
															postponeNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr ) )
																{
																// Draw missing negative conclusions
																if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, false, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																}
															}

														if( !isArchivedAssignment &&
														isIncludingRelationWord &&
														relationContextNr > NO_CONTEXT_NR )
															{
															if( drawPossessiveReversibleConclusion( false, false, false, false, NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}

										if( ( isGeneralizationProperName ||
										currentSpecificationCollectionNr > NO_COLLECTION_NR ) &&

										( !isExclusiveCurrentSpecification ||
										bestMatchingRelationSpecificationItem == NULL ||

										// Definition collected with itself
										( generalizationWordItem == specificationWordItem &&
										!specificationWordItem->isUserSpecificationWord &&
										!currentSpecificationItem->isOlderItem() &&
										currentSpecificationItem->isUserSpecification() ) ) )
											{
											if( ( !isExclusiveSpecification &&
											isExclusiveCurrentSpecification ) ||

											( isGeneralizationProperName &&

											( ( isExclusiveSpecification &&
											currentSpecificationCollectionNr > NO_COLLECTION_NR ) ||

											( isSpecificationWordSpanishAmbiguous &&
											!isFoundPrimaryHiddenSpanishSpecification &&

											( ( isExclusiveSpecification &&

											( isGeneralizationWordUserRelationWord ||
											isPossessiveUserSpecification ) ) ||

											( hasFoundPrimarySpecificationNonCompoundCollection &&
											isGeneralizationWordUserRelationWord &&
											isPossessiveUserSpecification ) ) ) ) ) )
												{
												hasCurrentSpecificationCompoundCollection = ( currentSpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) > NO_COLLECTION_NR );

												// Ask a question or draw a negative conclusion
												if( isGeneralizationProperName &&
												currentSpecificationCollectionNr > NO_COLLECTION_NR &&
												userSpecificationItem != NULL &&

												// Specification isn't Spanish ambiguous
												( ( !isSpecificationWordSpanishAmbiguous &&

												( !hasCurrentSpecificationCompoundCollection ||
												!hasGeneralizationWordActiveQuestionWithCompoundCollection ||
												isExclusiveUserSpecification ||
												isUserSpecificationWordSpanishAmbiguous ||
												foundPrimarySpecificationItem->hasSpecificationCollection() ) ) ||

												// Specification is Spanish ambiguous
												( isSpecificationWordSpanishAmbiguous &&

												( hasFoundPrimarySpecificationNonCompoundCollection ||
												!isExclusiveCurrentSpecification ||
												isNegativeConclusion ||

												( isUserGeneralizationWord &&
												!isExclusiveUserSpecification &&
												currentSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) ||

												// Question
												hasCorrectedAssumptionByOppositeQuestion ||
												adminItem_->hasFoundSuggestiveQuestionAssumption() ) )
													{
													if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												{
												// Make an assumption or draw a conclusion
												if( isAssumption ||
												currentSpecificationItem->hasAssumptionLevel() )
													{
													alternativeAnotherPrimarySpecificationItem = NULL;
													primarySpecificationItem = NULL;

													if( isSpecificationWordSpanishAmbiguous )
														primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );
													else
														{
														primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL );

														if( primarySpecificationItem == NULL ||
														primarySpecificationItem ->isUserAssignment() )
															primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );
														}

													if( primarySpecificationItem != NULL )
														{
														alternativeJustificationSpecificationItem = ( hasRelationWord || isSpecificationWordSpanishAmbiguous ? adminItem_->alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) : NULL );
														existingSpecificationItem = ( hasRelationWord ? generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem ) : NULL );

														if( ( isSpecificationWordSpanishAmbiguous &&
														primarySpecificationItem->isHiddenSpanishSpecification() ) ||

														( existingSpecificationItem != NULL &&
														alternativeJustificationSpecificationItem != NULL &&
														alternativeJustificationSpecificationItem->specificationWordItem() == specificationWordItem &&

														( ( isSpecificationWordSpanishAmbiguous &&
														existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ) ||

														( !isSpecificationWordSpanishAmbiguous &&
														!existingSpecificationItem->hasPrimarySpecificationJustification( alternativeJustificationSpecificationItem ) &&

														( isUserGeneralizationWord ||
														!specificationWordItem->isCollectionSpanishAmbiguous( alternativeJustificationSpecificationItem->specificationCollectionNr() ) ) ) ) ) )
															{
															if( alternativeJustificationSpecificationItem != NULL )
																alternativeAnotherPrimarySpecificationItem = adminItem_->alternativeAnotherPrimarySpecificationItem();

															if( isSpecificationWordSpanishAmbiguous &&
															generalizationWordItem->isUserRelationWord &&
															alternativeAnotherPrimarySpecificationItem != NULL &&
															existingSpecificationItem != NULL &&
															userSpecificationItem != NULL &&
															primarySpecificationItem->isHiddenSpanishSpecification() &&
															!userSpecificationItem->isArchivedAssignment() )
																{
																primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, NO_QUESTION_PARAMETER, userSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, userSpecificationItem->specificationWordItem() );

																if( primarySpecificationItem != NULL &&
																// Check if justification already exists
																existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
																	primarySpecificationItem = alternativeJustificationSpecificationItem;
																}
															else
																primarySpecificationItem = alternativeJustificationSpecificationItem;
															}
														else
															{
															if( userSpecificationItem != NULL &&
															userSpecificationItem->isPossessive() &&
															primarySpecificationItem->hasAssumptionLevel() )
																{
																existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem );

																if( existingSpecificationItem != NULL &&
																existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL )
																	// Skip if similar justification already exists, without relation word
																	primarySpecificationItem = NULL;
																}
															}
														}

													if( primarySpecificationItem != NULL &&

													( currentSpecificationCollectionNr == NO_COLLECTION_NR ||
													primarySpecificationItem == foundPrimarySpecificationItem ) &&

													// Skip if already exists as user specification
													generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL )
														{
														archivedAssignmentItem = NULL;

														if( userSpecificationItem != NULL &&
														userSpecificationItem->isArchivedAssignment() &&
														userSpecificationItem->isGeneralizationNoun() )
															{
															isAssignment = true;
															isArchivedAssignment = true;
															archivedAssignmentItem = userSpecificationItem;
															}

														// Make a compound specification substitution assumption, which is actually an uncertain conclusion
														// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
														//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
														if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, ( archivedAssignmentItem == NULL ? currentSpecificationItem : archivedAssignmentItem ), NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result == RESULT_OK )
															{
															if( specificationResult.createdSpecificationItem == NULL )
																{
																if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justification items in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													{
													if( ( !isExclusiveSpecification &&
													bestMatchingRelationSpecificationItem == NULL &&

													( !isSpecificationWordSpanishAmbiguous ||

													( !isFoundPrimaryHiddenSpanishSpecification &&
													collectedWithItselfSpecificationItem != NULL &&
													collectedWithItselfSpecificationItem->hasCurrentCreationSentenceNr() ) ) &&

													// Skip if already exists as user specification
													generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) ||

													// Check if exclusive conclusion already exists
													( isExclusiveSpecification &&
													generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isArchivedAssignment, false, false, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) )
														{
														primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

														if( primarySpecificationItem != NULL &&
														primarySpecificationItem->isHiddenSpanishSpecification() &&
														( primarySpecificationItem = adminItem_->alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != NULL )
															alternativeAnotherPrimarySpecificationItem = adminItem_->alternativeAnotherPrimarySpecificationItem();

														if( primarySpecificationItem != NULL )
															{
															if( !primarySpecificationItem->isPartOf() )
																{
																// Draw a compound specification substitution conclusion
																// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem->isEveryGeneralization(), isExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result == RESULT_OK )
																	{
																	if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
																		{
																		if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
																			{
																			if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result == RESULT_OK )
																				{
																				if( specificationResult.relatedSpecificationItem != NULL )
																					{
																					// Replace incorrect assumption
																					if( generalizationWordItem->replaceOrDeleteSpecification( specificationResult.relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			else
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created specification" );
																			}
																		else
																			{
																			if( isGeneralizationProperName )
																				{
																				if( !hasRelationWord &&
																				!isExclusiveSpecification &&
																				!isSpecificationWordSpanishAmbiguous &&
																				isUserGeneralizationWord &&
																				currentSpecificationCollectionNr > NO_COLLECTION_NR &&
																				primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
																				adminItem_->isSpanishCurrentLanguage() )
																					{
																					if( drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			else
																				{
																				if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																				}
																			}
																		}
																	}
																else
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );
														}
													}
												}

											if( !isExclusiveCurrentSpecification &&
											isGeneralizationProperName &&
											currentSpecificationCollectionNr > NO_COLLECTION_NR )
												{
												if( adminItem_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption by generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
							}
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning type" );

		return RESULT_OK;
		}

	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem()
		{
		return spanishAmbiguousCompoundPrimarySpecificationItem_;
		}

	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem()
		{
		return spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
