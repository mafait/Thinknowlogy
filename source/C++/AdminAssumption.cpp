/*
 *	Class:			AdminAssumption
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously make grammar-based assumptions
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "SpecificationItem.cpp"

class AdminAssumption
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundSuggestiveQuestionAssumption_;

	SpecificationItem *foundPossessiveDefinitionSpecificationItem_;
	SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem_;
	SpecificationItem *lastFoundOppositePossessiveDefinitionSpecificationItem_;
	SpecificationItem *possessiveDefinitionSpecificationItem_;
	SpecificationItem *secondarySpecificationItem_;

	WordItem *generalizationAssumptionWordItem_;
	WordItem *oppositePossessiveSpecificationWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isFeminineOrMasculineProperNameEnding( bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isFemaleSpecificationWord;
		bool isMaleSpecificationWord;

		if( generalizationWordItem != NULL &&
		specificationWordItem != NULL )
			{
			isFemaleSpecificationWord = specificationWordItem->isFemale();
			isMaleSpecificationWord = specificationWordItem->isMale();

			if( isFemaleSpecificationWord ||
			isMaleSpecificationWord )
				{
				if( specificationWordItem->isNounWordCollectedWithItself() )
					{
					if( ( isPossessive &&
					relationWordItem != NULL &&
					relationWordItem->hasFeminineProperNameEnding() ) ||

					( !isPossessive &&
					generalizationWordItem->hasFeminineProperNameEnding() ) )
						return true;
					}
				else
					{
					// Possessive (relation)
					if( ( isPossessive &&
					relationWordItem != NULL &&

					( ( isFemaleSpecificationWord &&
					relationWordItem->hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					relationWordItem->hasMasculineProperNameEnding() ) ) ) ||

					// Non-possessive (generalization)
					( !isPossessive &&

					( ( isFemaleSpecificationWord &&
					generalizationWordItem->hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					generalizationWordItem->hasMasculineProperNameEnding() ) ) ) )
						return true;
					}
				}
			}

		return false;
		}

	ResultType correctSuggestiveAssumptionsByOppositeQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem *createdNewSpecificationItem;
		SpecificationItem *createdOldSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem *oldSuggestiveSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "correctSuggestiveAssumptionsByOppositeQuestion";

		if( secondarySpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					// Previously, a suggestive question assumption was made.
					// However, now a question with the opposite specification is asked.
					// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
					if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem->oldSuggestiveQuestionAssumptionSecondarySpecificationItem() ) != NULL )
						{
						if( oldSuggestiveQuestionAssumptionAssignmentItem->isActiveAssignment() )
							{
							if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem->specificationWordItem() ) != NULL )
								{
								if( secondarySpecificationItem->isActiveAssignment() )
									{
									// Correct old question
									if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, oldSuggestiveQuestionAssumptionAssignmentItem->assumptionLevel(), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result == RESULT_OK )
										{
										if( ( createdOldSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
											{
											// Correct new question
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, secondarySpecificationItem->assumptionLevel(), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result == RESULT_OK )
												{
												if( ( createdNewSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
													{
													if( ( correctedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem->correctedAssumptionByOppositeQuestionJustificationItem() ) != NULL )
														{
														// Change primary user specification by primary corrected question specification
														if( correctedAssumptionByOppositeQuestionJustificationItem->changePrimarySpecification( createdNewSpecificationItem ) == RESULT_OK )
															{
															if( ( secondarySpecificationWordItem = correctedAssumptionByOppositeQuestionJustificationItem->secondarySpecificationWordItem() ) != NULL )
																{
																// Correct generalization assumption
																if( ( primarySpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != NULL )
																	{
																	if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result == RESULT_OK )
																		{
																		if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
																			{
																			if( foundSpecificationItem->attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != RESULT_OK )
																				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );
																			}
																		else
																			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the corrected generalization assumption" );
																		}
																	else
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification substitution assumption" );
																	}
																else
																	return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );
																}
															else
																return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );
															}
														else
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to change the primary specification of the corrected assumption by opposite question justification item" );
														}
													else
														return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the old corrected assumption by opposite question justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't create the new suggestive question assumption" );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't create the old suggestive question assumption" );
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is no assignment" );
								}
							else
								return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );
							}
						else
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType findPossessiveSpecifications( WordItem *generalizationWordItem, WordItem *definitionWordItem, WordItem *specificationWordItem )
		{
		bool hasFoundPossessiveSpecificationWord = true;
		unsigned int possessiveSpecificationCollectionNr;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *previousPossessiveSpecificationItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		WordItem *previousPossessiveSpecificationWordItem;
		WordItem *higherLevelPossessiveSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossessiveSpecifications";

		foundPossessiveDefinitionSpecificationItem_ = NULL;
		foundOppositePossessiveDefinitionSpecificationItem_ = NULL;

		if( generalizationWordItem != NULL )
			{
			if( definitionWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					// First try to find a possessive specification on a higher level
					if( ( possessiveDefinitionSpecificationItem = definitionWordItem->firstNonQuestionSpecificationItem() ) != NULL )
						{
						do	{
							if( !possessiveDefinitionSpecificationItem->isExclusiveSpecification() &&
							possessiveDefinitionSpecificationItem->isPossessive() &&
							possessiveDefinitionSpecificationItem->isSpecificationNoun() &&
							( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
								{
								if( possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
								possessiveSpecificationWordItem->firstSpecificationItem( false, NO_QUESTION_PARAMETER, specificationWordItem ) != NULL )
									{
									if( higherLevelPossessiveSpecificationWordItem == NULL )
										higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
									else
										hasFoundPossessiveSpecificationWord = false;
									}
								}
							}
						while( ( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

						if( hasFoundPossessiveSpecificationWord &&
						( possessiveDefinitionSpecificationItem = definitionWordItem->firstNonQuestionSpecificationItem() ) != NULL )
							{
							if( possessiveDefinitionSpecificationItem->hasSpecificationCollection() )
								{
								if( higherLevelPossessiveSpecificationWordItem == NULL )
									higherLevelPossessiveSpecificationWordItem = specificationWordItem;

								// Try to find the opposite of the possessive specification
								do	{
									if( !possessiveDefinitionSpecificationItem->isExclusiveSpecification() &&
									!possessiveDefinitionSpecificationItem->isPartOf() &&
									possessiveDefinitionSpecificationItem->isPossessive() &&
									( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
										{
										if( possessiveSpecificationWordItem != generalizationWordItem )
											{
											if( possessiveSpecificationWordItem->isSingularNoun() &&
											previousPossessiveSpecificationItem != NULL &&
											( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem->specificationWordItem() ) != NULL )
												{
												possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem->specificationCollectionNr();

												if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
												higherLevelPossessiveSpecificationWordItem->hasCollectionNr( possessiveSpecificationCollectionNr ) &&
												previousPossessiveSpecificationItem->specificationCollectionNr() == possessiveSpecificationCollectionNr )
													{
													foundOppositePossessiveDefinitionSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );
													lastFoundOppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem_;
													}

												foundPossessiveDefinitionSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
												}

											previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );
										}
									}
								while( ( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
								}
							}
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given definition word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType makeGeneralizationAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem *olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem *olderGeneralizationAssumptionBySpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumption";

		if( generalizationWordItem != NULL )
			{
			// Skip if already exists
			if( generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) == NULL )
				{
				// Make a generalization assumption
				// Not yet explained in the design
				if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result == RESULT_OK )
					{
					if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() &&
					specificationResult.createdJustificationItem != NULL &&
					( olderGeneralizationAssumptionBySpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
						{
						if( ( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != NULL )
							{
							if( generalizationWordItem->replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, specificationResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != RESULT_OK )
								adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item" );
							}
						}
					}
				else
					return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminAssumption( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundSuggestiveQuestionAssumption_ = false;

		foundPossessiveDefinitionSpecificationItem_ = NULL;
		foundOppositePossessiveDefinitionSpecificationItem_ = NULL;
		lastFoundOppositePossessiveDefinitionSpecificationItem_ = NULL;
		possessiveDefinitionSpecificationItem_ = NULL;
		secondarySpecificationItem_ = NULL;

		generalizationAssumptionWordItem_ = NULL;
		oppositePossessiveSpecificationWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminAssumption" );

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

	void initializeAdminAssumptionVariables()
		{
		hasFoundSuggestiveQuestionAssumption_ = false;

		lastFoundOppositePossessiveDefinitionSpecificationItem_ = NULL;
		secondarySpecificationItem_ = NULL;
		generalizationAssumptionWordItem_ = NULL;
		}

	bool hasFoundSuggestiveQuestionAssumption()
		{
		return hasFoundSuggestiveQuestionAssumption_;
		}

	ResultType makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasFeminineOrMasculineProperNameEnding;
		bool hasSecondarySpecificationNonCompoundCollection;
		bool isHiddenSecondarySpecification;
		bool isOppositePossessiveSpecificationCollectedWithItself;
		bool isPossessiveSpecificationCollectedWithItself;
		bool isSpecificationCollectedWithItself;
		bool hasRelationWord = ( relationWordItem != NULL );
		unsigned int compoundCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *olderPossessiveReversibleConclusionJustificationItem;
		JustificationItem *replacingJustificationItem;
		SpecificationItem *compoundSecondarySpecificationItem;
		SpecificationItem *compoundSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *primarySpecificationItem = NULL;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		WordItem *currentWordItem;
		WordItem *possessiveSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		possessiveDefinitionSpecificationItem_ = NULL;
		secondarySpecificationItem_ = NULL;

		oppositePossessiveSpecificationWordItem_ = NULL;

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

				if( secondarySpecificationItem != NULL &&

				( hasRelationWord ||

				( !secondarySpecificationItem->isOlderItem() &&

				( !isPossessive ||
				!secondarySpecificationItem->isExclusiveSpecification() ) ) ) )
					{
					// Initially not all words are linked. So, search all words.
					if( ( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
						{
						isHiddenSecondarySpecification = secondarySpecificationItem->isHiddenSpecification();
						isSpecificationCollectedWithItself = specificationWordItem->isNounWordCollectedWithItself();

						do	{
							if( currentWordItem->isNoun() &&
							currentWordItem != specificationWordItem )
								{
								if( findPossessiveSpecifications( generalizationWordItem, currentWordItem, specificationWordItem ) == RESULT_OK )
									{
									if( foundPossessiveDefinitionSpecificationItem_ != NULL )
										{
										possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem_;

										if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) != NULL )
											{
											if( ( primarySpecificationItem = possessiveSpecificationWordItem->firstSpecificationItem( isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
												{
												isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem->isNounWordCollectedWithItself();

												if( ( !secondarySpecificationItem->hasRelationContext() &&
												generalizationWordItem->hasAnsweredQuestion() ) ||

												( isPossessiveSpecificationCollectedWithItself &&
												secondarySpecificationItem->specificationWordItem() == possessiveSpecificationWordItem ) )
													{
													if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) != RESULT_OK )
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveSpecificationWordItem );

													if( existingUserSpecificationItem == NULL ||

													// Confirmed specification without relation word
													( hasRelationWord &&

													( !existingUserSpecificationItem->hasRelationContext() ||
													!existingUserSpecificationItem->isOlderItem() ) ) )
														{
														hasFeminineOrMasculineProperNameEnding = false;

														if( isPossessiveSpecificationCollectedWithItself &&
														isFeminineOrMasculineProperNameEnding( isPossessive, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) )
															hasFeminineOrMasculineProperNameEnding = true;

														// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
														if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), foundPossessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) ).result == RESULT_OK )
															{
															createdSpecificationItem = specificationResult.createdSpecificationItem;
															foundSpecificationItem = specificationResult.foundSpecificationItem;

															if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ) ) != NULL )
																{
																if( isArchivedAssignment )
																	secondarySpecificationItem_ = generalizationWordItem->firstAssignmentItem( false, false, true, isPossessive, createdOrFoundSpecificationItem->questionParameter(), createdOrFoundSpecificationItem->relationContextNr(), createdOrFoundSpecificationItem->specificationWordItem() );
																else
																	secondarySpecificationItem_ = createdOrFoundSpecificationItem->updatedSpecificationItem();

																if( hasRelationWord &&
																createdSpecificationItem != NULL &&
																foundSpecificationItem != NULL &&
																foundSpecificationItem->hasRelationContext() &&
																// Cross-collected afterwards
																foundSpecificationItem->relationContextNr() != createdSpecificationItem->relationContextNr() &&
																( createdJustificationItem = specificationResult.createdJustificationItem ) != NULL &&
																( obsoleteJustificationItem = generalizationWordItem->olderPrimarySpecificationJustificationItem( false, true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_COLLECTION_NR, NULL ) ) != NULL )
																	{
																	if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, createdSpecificationItem ) == RESULT_OK )
																		{
																		if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL )
																			{
																			// Move justifications from the found specification to the created specification
																			if( createdSpecificationItem->changeFirstJustification( firstJustificationItem ) == RESULT_OK )
																				{
																				if( generalizationWordItem->replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																					return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																				}
																			else
																				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to change the first justification item of the created specification item" );
																			}
																		else
																			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The found specification item has no first justification item" );
																		}
																	else
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the obsolete justification item by the created justification item" );
																	}
																}
															else
																return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "No specification item was created or found" );
															}
														else
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}

									// First opposite possessive specification
									if( !commonVariables_->hasShownWarning &&
									possessiveSpecificationWordItem != NULL &&
									foundOppositePossessiveDefinitionSpecificationItem_ != NULL &&
									( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem_->specificationWordItem() ) != NULL )
										{
										if( isNegative ||
										specificationContextNr > NO_CONTEXT_NR ||
										secondarySpecificationItem_ == NULL )
											{
											secondarySpecificationItem_ = secondarySpecificationItem;

											// Find another primary definition
											if( !hasRelationWord ||
											isPossessive )
												anotherPrimarySpecificationItem = possessiveSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );
											}

										if( secondarySpecificationItem_ != NULL &&

										( !secondarySpecificationItem_->isOlderItem() ||
										secondarySpecificationItem_->isSelfGenerated() ) )
											{
											existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

											if( ( ( hasRelationWord &&
											isPossessive ) ||

											existingSpecificationItem == NULL ||
											!existingSpecificationItem->hasRelationContext() ||

											( !isHiddenSecondarySpecification &&
											secondarySpecificationItem_->hasRelationContext() &&
											!secondarySpecificationItem_->isSelfGeneratedConclusion() ) ) &&

											// Skip if user specification exists
											generalizationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == NULL )
												{
												hasFeminineOrMasculineProperNameEnding = false;
												hasSecondarySpecificationNonCompoundCollection = false;
												isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_->isNounWordCollectedWithItself();

												if( isOppositePossessiveSpecificationCollectedWithItself &&
												isPossessive &&
												isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
													{
													hasFeminineOrMasculineProperNameEnding = true;

													if( secondarySpecificationItem->hasSpecificationNonCompoundCollection() )
														{
														hasSecondarySpecificationNonCompoundCollection = true;
														anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_->firstAssignmentOrSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ );
														}
													}

												// Make an exclusive specification substitution assumption (opposite)
												// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
												if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), foundOppositePossessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundOppositePossessiveDefinitionSpecificationItem_, ( isOppositePossessiveSpecificationCollectedWithItself ? NULL : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationCollectedWithItself ? secondarySpecificationItem : secondarySpecificationItem_ ), ( hasSecondarySpecificationNonCompoundCollection ? anotherPrimarySpecificationItem : NULL ), generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result == RESULT_OK )
													{
													createdSpecificationItem = specificationResult.createdSpecificationItem;
													foundSpecificationItem = specificationResult.foundSpecificationItem;

													if( isPossessive )
														{
														if( isOppositePossessiveSpecificationCollectedWithItself )
															{
															if( anotherPrimarySpecificationItem != NULL &&
															( compoundSecondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, NO_QUESTION_PARAMETER, anotherPrimarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
																{
																if( !hasRelationWord &&
																adminItem_->isPossessiveUserSpecification() )
																	{
																	if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( hasFeminineOrMasculineProperNameEnding, false, false, false, isNegative, false, !isPossessive, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) != RESULT_OK )
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw an opposite specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																else
																	{
																	// Make a compound specification exclusive specification substitution assumption (opposite)
																	// Not yet explained in the design
																	if( adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundOppositePossessiveDefinitionSpecificationItem_, NULL, compoundSecondarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ).result != RESULT_OK )
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is collected with itself in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															}
														else
															{
															if( foundSpecificationItem == NULL )
																secondarySpecificationItem_ = createdSpecificationItem;
															else
																{
																if( createdSpecificationItem == NULL )
																	secondarySpecificationItem_ = foundSpecificationItem->updatedSpecificationItem();
																else
																	{
																	if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL )
																		{
																		// Move justifications from the found specification to the created specification
																		if( createdSpecificationItem->changeFirstJustification( firstJustificationItem ) == RESULT_OK )
																			{
																			if( generalizationWordItem->replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																			}
																		else
																			return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to change the first justification item of the created specification item" );
																		}
																	else
																		return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The found specification item has no first justification item" );
																	}
																}
															}
														}
													else
														{
														if( createdSpecificationItem == NULL )
															{
															if( ( obsoleteJustificationItem = generalizationWordItem->obsoleteJustificationItem() ) == NULL )
																{
																if( foundSpecificationItem != NULL &&
																specificationResult.createdJustificationItem == NULL &&
																( replacingJustificationItem = generalizationWordItem->olderSecondarySpecificationJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem_ ) ) != NULL &&
																( obsoleteJustificationItem = generalizationWordItem->olderSecondarySpecificationJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, secondarySpecificationItem_ ) ) != NULL )
																	{
																	if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, replacingJustificationItem, foundSpecificationItem ) != RESULT_OK )
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																{
																if( generalizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
																	return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															{
															if( hasRelationWord &&
															existingSpecificationItem != NULL &&
															existingSpecificationItem->hasRelationContext() &&

															( !createdSpecificationItem->isHiddenSpecification() ||
															existingSpecificationItem->isHiddenSpecification() ||

															( isSpecificationCollectedWithItself &&
															existingSpecificationItem->specificationCollectionNr() != createdSpecificationItem->specificationCollectionNr() ) ) &&

															( existingSpecificationItem->isSelfGeneratedAssumption() ||

															( isOppositePossessiveSpecificationCollectedWithItself &&
															// Cross-collected afterwards
															existingSpecificationItem->relationContextNr() != createdSpecificationItem->relationContextNr() ) ) )
																{
																if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) == RESULT_OK )
																	{
																	if( isOppositePossessiveSpecificationCollectedWithItself )
																		{
																		if( ( olderPossessiveReversibleConclusionJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL )
																			{
																			if( generalizationWordItem->attachJustification( olderPossessiveReversibleConclusionJustificationItem, createdSpecificationItem ) != RESULT_OK )
																				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the created possessive reversible assumption justification to the created assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																			}
																		}
																	else
																		{
																		if( ( firstJustificationItem = existingSpecificationItem->firstJustificationItem() ) != NULL )
																			{
																			// Attach all justification items of the existing specification to the created specification
																			if( createdSpecificationItem->changeFirstJustification( firstJustificationItem ) != RESULT_OK )
																				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to change the first justification item of the created specification" );
																			}
																		else
																			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The existing specification item has no first justification item" );
																		}
																	}
																else
																	return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the existing specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														}
													}
												else
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								else
									return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find possessive specifications in word \"", currentWordItem->anyWordTypeString(), "\"" );
								}
							}
						while( !commonVariables_->hasShownWarning &&
						( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

						if( !commonVariables_->hasShownWarning &&
						hasRelationWord &&
						secondarySpecificationItem_ != NULL )
							{
							if( secondarySpecificationItem->isSpecificationWordCollectedWithItself() &&
							secondarySpecificationItem->hasSpecificationNonCompoundCollection() &&
							( compoundCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
								{
								if( ( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, compoundCollectionNr, secondarySpecificationItem->relationContextNr(), specificationWordItem ) ) != NULL )
									secondarySpecificationItem = compoundSpecificationItem;
								}

							if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem_, ( isHiddenSecondarySpecification ? adminItem_->userSpecificationItem() : secondarySpecificationItem ), generalizationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );
					}
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		unsigned short questionParameter = ( isGeneralizationProperName ? WORD_PARAMETER_SINGULAR_VERB_IS : NO_QUESTION_PARAMETER );
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionByGeneralization";

		if( adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
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
									!currentGeneralizationWordItem->hasCollection() &&
									!currentGeneralizationWordItem->hasPossessiveButNonQuestionSpecification() &&
									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
										{
										// Get secondary specification for justification
										if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
											{
											if( isGeneralizationProperName )
												{
												if( makeGeneralizationAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											{
											// Get secondary question specification for justification
											if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != NULL )
												{
												if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, isNegative, false, isPossessive, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, NULL ) != RESULT_OK )
													adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an undefined generalization word" );
								}
							while( !commonVariables_->hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
							}
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word is undefined" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word type isn't a noun" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		return RESULT_OK;
		}

	ResultType makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		SpecificationResultType specificationResult;
		unsigned int currentSpecificationCollectionNr;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *indirectPrimarySpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *negativeQuestionSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( userSpecificationItem != NULL )
					{
					if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != NULL )
						{
						do	{
							if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
							( currentSpecificationCollectionNr = currentQuestionSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
								{
								if( ( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem->firstActiveAssignmentOrSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
									{
									if( ( specificationResult = generalizationWordItem->findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, specificationWordItem ) ).result == RESULT_OK )
										{
										if( specificationResult.relatedSpecificationItem != NULL )
											negativeQuestionSpecificationWordItem = specificationResult.relatedSpecificationItem->specificationWordItem();

										// Draw an indirectly answered question conclusion
										// Not yet explained in the design
										if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, indirectPrimarySpecificationItem, NULL, userSpecificationItem, NULL, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result == RESULT_OK )
											{
											if( negativeQuestionSpecificationWordItem != NULL &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL &&
											( commonWordItem = currentQuestionSpecificationWordItem->commonWordItem( currentSpecificationCollectionNr ) ) != NULL )
												{
												if( commonWordItem->isNounWordCollectedWithItself() )
													{
													if( generalizationWordItem->replaceOrDeleteSpecification( currentQuestionSpecificationItem->updatedSpecificationItem(), createdSpecificationItem ) == RESULT_OK )
														{
														while( ( obsoleteJustificationItem = generalizationWordItem->obsoleteJustificationItem() ) != NULL )
															{
															if( generalizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
																return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the answered question by the created specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, commonWordItem ) ) != NULL &&
													( anotherPrimarySpecificationItem = commonWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != NULL )
														{
														// Draw a negative assumption
														// Not yet explained in the design
														if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, NULL, generalizationWordItem, negativeQuestionSpecificationWordItem, NULL ).result != RESULT_OK )
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeQuestionSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}

											currentQuestionSpecificationItem = adminItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
											}
										else
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is collected with itself" );
									}
								else
									currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem();
								}
							else
								currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem();
							}
						while( currentQuestionSpecificationItem != NULL );
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned short nOptionsLeft = 0;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *onlyOptionLeftSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeOnlyOptionLeftAssumption";

		if( userSpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( userSpecificationItem->hasSpecificationCollection() )
					{
					if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSelectedSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != NULL )
						{
						do	{
							if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL )
								{
								if( generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, NO_QUESTION_PARAMETER, currentQuestionSpecificationWordItem ) == NULL )
									{
									nOptionsLeft++;
									onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
									}
								}
							}
						while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

						if( nOptionsLeft == 1 &&
						onlyOptionLeftSpecificationWordItem != NULL &&
						( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
							{
							do	{
								if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
									{
									if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != NULL &&
									( anotherPrimarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != NULL )
										{
										// Make an only option left assumption
										// Not yet explained in the design
										if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an undefined generalization word" );
								}
							while( !commonVariables_->hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
							}
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item has no specification collection" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		return RESULT_OK;
		}

	ResultType makePossessiveConditionalSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *alternativeSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool hasGeneralizationWordConfirmedSpecification;
		bool hasSameSpecificationCollection;
		bool isOppositePossessiveSpecificationCollectedWithItself;
		bool isPossessiveReversibleAssumption;
		bool isPossessiveSpecificationCollectedWithItself;
		bool hasFeminineOrMasculineProperNameEnding = false;
		bool isArchivedAssignmentAssumption = false;
		bool isForcingNewJustification = false;
		unsigned short assumptionJustificationTypeNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *possessiveSpecificationWordItem;
		WordItem *feminineSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makePossessiveConditionalSpecificationAssumption";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( secondarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( relationWordItem != NULL )
						{
						// Skip if global variables are not assigned
						if( possessiveDefinitionSpecificationItem_ != NULL &&
						secondarySpecificationItem_ != NULL &&
						( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) != NULL )
							{
							isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem->isNounWordCollectedWithItself();

							if( isPossessiveSpecificationCollectedWithItself ||
							possessiveDefinitionSpecificationItem_->specificationCollectionNr() == secondarySpecificationItem->specificationCollectionNr() )
								{
								existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, !isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, possessiveSpecificationWordItem );
								existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveSpecificationWordItem );

								// Skip if possessive user specification exists
								if( ( existingUserSpecificationItem == NULL &&
								!possessiveSpecificationWordItem->isUserSpecificationWord &&

								( !isPossessiveSpecificationCollectedWithItself ||

								( existingSpecificationItem == NULL &&
								relationWordItem->isUserGeneralizationWord ) ) &&

								// Skip if possessive specification exists
								relationWordItem->olderSecondarySpecificationJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL ) ||

								( isPossessiveSpecificationCollectedWithItself &&
								alternativeSecondarySpecificationItem != NULL &&
								!alternativeSecondarySpecificationItem->isOlderItem() &&
								alternativeSecondarySpecificationItem->isActiveAssignment() &&

								// Skip if possessive user specification exists
								( existingUserSpecificationItem == NULL ||

								( existingUserSpecificationItem->isOlderItem() &&
								existingUserSpecificationItem->hasSpecificationCompoundCollection() ) ) ) )
									{
									isPossessiveReversibleAssumption = ( secondarySpecificationItem->specificationWordItem() == possessiveSpecificationWordItem );

									// Make a possessive conditional specification assumption (normal)
									// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
									if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, isNegative, isPartOf, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : NULL ), NULL, ( isPossessiveSpecificationCollectedWithItself ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), NULL, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result == RESULT_OK )
										{
										if( !isPossessive &&
										existingSpecificationItem != NULL &&
										( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
											{
											existingSpecificationItem = existingSpecificationItem->updatedSpecificationItem();

											if( createdSpecificationItem->assumptionLevel() < existingSpecificationItem->assumptionLevel() )
												{
												if( relationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the existing specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );
									}

								if( !commonVariables_->hasShownWarning )
									{
									existingSpecificationItem = NULL;

									if( oppositePossessiveSpecificationWordItem_ != NULL )
										{
										isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_->isNounWordCollectedWithItself();
										isPossessiveReversibleAssumption = ( secondarySpecificationItem->specificationWordItem() == oppositePossessiveSpecificationWordItem_ );
										assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

										if( alternativeSecondarySpecificationItem != NULL &&
										// Try to find a self-generated conclusion first
										( existingSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, isNegative, isPossessive, false, oppositePossessiveSpecificationWordItem_, NULL ) ) == NULL &&
										// If not found, try to find a self-generated assumption
										( assumptionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != NULL )
											{
											hasGeneralizationWordConfirmedSpecification = generalizationWordItem->hasConfirmedSpecification();
											hasSameSpecificationCollection = ( assumptionSpecificationItem->specificationCollectionNr() == alternativeSecondarySpecificationItem->specificationCollectionNr() );

											if( !assumptionSpecificationItem->hasCurrentCreationSentenceNr() &&
											!assumptionSpecificationItem->hasNewInformation() &&

											( ( hasGeneralizationWordConfirmedSpecification &&
											hasSameSpecificationCollection ) ||

											( !hasGeneralizationWordConfirmedSpecification &&
											!hasSameSpecificationCollection ) ||

											alternativeSecondarySpecificationItem->isPossessive() ) )
												existingSpecificationItem = assumptionSpecificationItem;
											}

										// Skip if assumption already exists as user specification
										if( ( existingSpecificationItem == NULL ||
										!existingSpecificationItem->hasRelationContext() ||
										!generalizationWordItem->isOlderItem() ) &&

										( ( !isOppositePossessiveSpecificationCollectedWithItself &&
										// Skip if already exists as self-generated assumption
										relationWordItem->olderSecondarySpecificationJustificationItem( assumptionJustificationTypeNr, secondarySpecificationItem ) == NULL ) ||

										( isOppositePossessiveSpecificationCollectedWithItself &&
										alternativeSecondarySpecificationItem != NULL &&
										// Skip if already exists as self-generated assumption
										relationWordItem->olderSecondarySpecificationJustificationItem( assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == NULL ) ) )
											{
											if( isOppositePossessiveSpecificationCollectedWithItself &&

											( alternativeSecondarySpecificationItem == NULL ||
											alternativeSecondarySpecificationItem->isUserSpecification() ) &&

											isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
												hasFeminineOrMasculineProperNameEnding = true;

											// Make a possessive conditional specification assumption (opposite)
											// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, false, false, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, assumptionJustificationTypeNr, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? NULL : possessiveDefinitionSpecificationItem_ ), NULL, ( isOppositePossessiveSpecificationCollectedWithItself && alternativeSecondarySpecificationItem != NULL ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), NULL, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result == RESULT_OK )
												{
												if( !isOppositePossessiveSpecificationCollectedWithItself &&
												!isPossessiveReversibleAssumption &&
												( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
													{
													if( foundSpecificationItem->isOlderItem() &&
													( assumptionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, !isPossessive, true, oppositePossessiveSpecificationWordItem_, relationWordItem ) ) != NULL )
														{
														if( assumptionSpecificationItem->secondarySpecificationJustificationItem( foundSpecificationItem ) == NULL )
															{
															// Add missing justification
															if( ( justificationResult = generalizationWordItem->addJustification( false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, foundSpecificationItem, NULL, NULL ) ).result == RESULT_OK )
																{
																if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL )
																	{
																	if( generalizationWordItem->attachJustification( createdJustificationItem, assumptionSpecificationItem ) != RESULT_OK )
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the created justification to the given assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a justification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}

												if( alternativeSecondarySpecificationItem != NULL )
													{
													if( isOppositePossessiveSpecificationCollectedWithItself )
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem();
													else
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem->hasFeminineProperNameEnding() : relationWordItem->hasFeminineProperNameEnding() );

													if( feminineSpecificationWordItem != NULL &&
													isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
														{
														existingSpecificationItem = NULL;

														if( isOppositePossessiveSpecificationCollectedWithItself )
															existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, feminineSpecificationWordItem, generalizationWordItem );

														if( existingSpecificationItem == NULL ||

														// Skip if assumption under certain conditions already exists
														( !existingSpecificationItem->isOlderItem() &&
														!secondarySpecificationItem->isHiddenSpecification() ) )
															{
															if( isArchivedAssignment &&
															relationWordItem->isUserGeneralizationWord )
																isArchivedAssignmentAssumption = true;

															if( isOppositePossessiveSpecificationCollectedWithItself &&
															isPossessive &&
															( tempSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != NULL )
																{
																if( tempSpecificationItem->isOlderItem() )
																	isForcingNewJustification = true;
																}

															// Make an opposite possessive feminine proper name ending assumption
															// Not yet explained in the design
															if( adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignmentAssumption, isArchivedAssignmentAssumption, false, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationItem_, NULL, alternativeSecondarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ).result == RESULT_OK )
																{
																if( ( obsoleteJustificationItem = generalizationWordItem->obsoleteJustificationItem() ) != NULL )
																	{
																	if( generalizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );
											}
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );
									}
								}
							}
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation word item is undefined" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		return RESULT_OK;
		}

	ResultType makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasGeneralizationWordConfirmedSpecification;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool isGeneralizationCollectedWithItself;
		bool isGeneralizationWordUserGeneralizationWord;
		bool isSpecificationCollectedWithItself;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem *alternativeAnotherPrimarySpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *primaryPartOfSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *feminineCollectionWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSpecificationSubstitutionPartOfAssumption";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( generalizationWordItem != specificationWordItem &&
					( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
						{
						hasGeneralizationWordConfirmedSpecification = generalizationWordItem->hasConfirmedSpecification();
						hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
						isGeneralizationCollectedWithItself = generalizationWordItem->isNounWordCollectedWithItself();
						isGeneralizationWordUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
						isSpecificationCollectedWithItself = specificationWordItem->isNounWordCollectedWithItself();

						simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem();

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&

								( ( !isPossessive &&
								currentGeneralizationWordItem->hasNonPossessiveAndNonQuestionPartOfSpecification() ) ||

								( !currentGeneralizationWordItem->hasCollection() &&
								!currentGeneralizationWordItem->hasPossessiveButNonQuestionSpecification() ) ) )
									{
									existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

									// Skip if concluded conclusion exists
									if( ( existingSpecificationItem == NULL ||
									existingSpecificationItem != simpleUserSpecificationItem ) &&

									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
										{
										alternativeAnotherPrimarySpecificationItem = NULL;
										secondarySpecificationItem = NULL;

										// Get secondary specification for justification
										if( isGeneralizationWordUserGeneralizationWord &&
										simpleUserSpecificationItem != NULL &&
										!simpleUserSpecificationItem->isPossessive() &&

										( ( isSpecificationCollectedWithItself &&
										simpleUserSpecificationItem->specificationWordItem() == primarySpecificationItem->specificationWordItem() ) ||

										simpleUserSpecificationItem->specificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() ) )
											secondarySpecificationItem = simpleUserSpecificationItem;
										else
											{
											if( isSpecificationCollectedWithItself &&
											hasGeneralizationWordFeminineProperNameEnding &&
											( feminineCollectionWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
												secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, feminineCollectionWordItem );

											if( secondarySpecificationItem == NULL )
												{
												// Try to find self-generated conclusion first
												if( ( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, NULL ) ) == NULL )
													// Otherwise, try to find self-generated assumption
													tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );

												if( tempSpecificationItem != NULL &&

												( existingSpecificationItem == NULL ||
												tempSpecificationItem->isSelfGeneratedConclusion() ||
												// Skip assumption with relation word
												existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == NULL ) )
													secondarySpecificationItem = tempSpecificationItem;
												}

											if( secondarySpecificationItem == NULL )
												{
												if( hasGeneralizationWordFeminineProperNameEnding &&
												!isGeneralizationWordUserGeneralizationWord &&
												simpleUserSpecificationItem != NULL &&
												simpleUserSpecificationItem->isSpecificationWordCollectedWithItself() &&
												simpleUserSpecificationItem->isPossessive() &&
												( userSpecificationWordItem = simpleUserSpecificationItem->specificationWordItem() ) != NULL )
													{
													if( ( tempSpecificationItem = userSpecificationWordItem->firstSpecificationItem( isPossessive, NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != NULL )
														{
														primarySpecificationItem = simpleUserSpecificationItem;
														secondarySpecificationItem = tempSpecificationItem;
														}
													}
												}
											else
												{
												if( isSpecificationCollectedWithItself &&
												secondarySpecificationItem->isHiddenSpecification() )
													{
													secondarySpecificationItem = adminItem_->alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, false, true, isNegative, !isPossessive, simpleUserSpecificationItem, generalizationWordItem, specificationWordItem );
													alternativeAnotherPrimarySpecificationItem = adminItem_->alternativeAnotherPrimarySpecificationItem();
													}
												}
											}

										if( secondarySpecificationItem != NULL &&

										( !isSpecificationCollectedWithItself ||
										existingSpecificationItem == NULL ||
										!secondarySpecificationItem->isHiddenSpecification() ) )
											{
											if( primarySpecificationItem->isSpecificationGeneralization() )
												{
												if( primarySpecificationItem->isPartOf() &&
												// If a conclusion exists, skip creating an assumption
												generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, true, isNegative, false, false, currentGeneralizationWordItem, NULL ) == NULL )
													{
													// Make a substitution 'part of' assumption
													// Not yet explained in the design
													if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem->generalizationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, alternativeAnotherPrimarySpecificationItem, primarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ) ).result == RESULT_OK )
														{
														if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != NULL &&
														( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem->specificationSubstitutionPartOfAssumptionJustificationItem( primarySpecificationItem ) ) != NULL )
															{
															primaryPartOfSpecificationItem = specificationSubstitutionPartOfAssumptionJustificationItem->primarySpecificationItem();

															if( hasGeneralizationWordConfirmedSpecification ||
															specificationSubstitutionPartOfAssumptionJustificationItem->hasPrimaryAnsweredQuestion() ||

															( primaryPartOfSpecificationItem != NULL &&
															secondarySpecificationItem->assumptionLevel() < primaryPartOfSpecificationItem->assumptionLevel() ) )
																{
																if( generalizationWordItem->updateJustificationInSpecifications( false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) == RESULT_OK )
																	{
																	if( !specificationSubstitutionPartOfAssumptionJustificationItem->isReplacedOrDeletedItem() &&
																	!generalizationWordItem->isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) )
																		{
																		if( generalizationWordItem->replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != RESULT_OK )
																			return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the specification substitution 'part of' assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																		}
																	}
																else
																	return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														}
													else
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												{
												if( !isGeneralizationCollectedWithItself ||

												( primarySpecificationItem->isExclusiveSpecification() &&
												secondarySpecificationItem->isExclusiveSpecification() &&
												!secondarySpecificationItem->isOlderItem() ) )
													{
													if( makeGeneralizationAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) == RESULT_OK )
														generalizationAssumptionWordItem_ = currentGeneralizationWordItem;
													else
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										}
									}
								}
							else
								return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an undefined generalization word" );
							}
						while( !commonVariables_->hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
						}

					// Definition specification
					if( generalizationWordTypeNr != WORD_TYPE_PROPER_NAME &&
					generalizationAssumptionWordItem_ != NULL &&
					generalizationWordItem != generalizationAssumptionWordItem_ &&
					( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
					( secondarySpecificationItem = specificationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != NULL )
						{
						// Make an exclusive specification substitution assumption
						// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, generalizationAssumptionWordItem_, NULL ).result != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		return RESULT_OK;
		}

	ResultType makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		unsigned int secondarySpecificationCollectionNr;
		SpecificationItem *existingSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( secondarySpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();
					existingSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

					// Check if specification doesn't exist yet
					if( existingSpecificationItem == NULL ||
					existingSpecificationItem->isHiddenSpecification() )
						{
						// Check if a related specification exists
						if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result == RESULT_OK )
							{
							if( specificationResult.relatedSpecificationItem == NULL )
								{
								if( generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() )
									{
									if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									// Make a suggestive question assumption
									// Not yet explained in the design
									hasFoundSuggestiveQuestionAssumption_ = true;

									if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						else
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the secondary specification" );
						}
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	SpecificationItem *lastFoundOppositePossessiveDefinitionSpecificationItem()
		{
		return lastFoundOppositePossessiveDefinitionSpecificationItem_;
		}
	};

/*************************************************************************
 *	"He is my loving ally and my fortress,
 *	my tower of safety, my rescuer.
 *	He is my shield, and I take refuge in him.
 *	He makes the nations submit to me." (Psalm 144:2)
 *************************************************************************/
