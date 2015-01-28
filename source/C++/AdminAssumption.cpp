/*
 *	Class:			AdminAssumption
 *	Supports class:	AdminItem
 *	Purpose:		To create assumptions autonomously
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
	bool isAssumption_;

	unsigned int lastCreatedRelationContextNr_;

	SpecificationItem *foundPossessiveSpecificationItem_;
	SpecificationItem *foundOppositePossessiveSpecificationItem_;
	SpecificationItem *possessiveSpecificationItem_;
	SpecificationItem *secondarySpecificationItem_;

	WordItem *generalizationAssumptionWordItem_;
	WordItem *oppositePossessiveSpecificationWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isUniqueRelation( unsigned short justificationTypeNr )
		{
		return ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );
		}

	ResultType addMissingJustification( unsigned short justificationTypeNr, SpecificationItem *assumptionSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *createdJustificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMissingJustification";

		if( assumptionSpecificationItem != NULL )
			{
			if( secondarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( ( justificationResult = generalizationWordItem->addJustification( false, justificationTypeNr, NO_ORDER_NR, commonVariables_->currentSentenceNr, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, NULL ) ).result == RESULT_OK )
						{
						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL )
							{
							if( generalizationWordItem->attachJustification( createdJustificationItem, assumptionSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the created justification to the given assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a justification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given assumption specification item is undefined" );

		return RESULT_OK;
		}

	ResultType correctSuggestiveAssumptionsByOppositeQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem *oldCorrectedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem *createdSpecificationItem;
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
						if( oldSuggestiveQuestionAssumptionAssignmentItem->isAssignment() )
							{
							if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem->specificationWordItem() ) != NULL )
								{
								if( secondarySpecificationItem->isAssignment() )
									{
									// Correct old question
									if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, oldSuggestiveQuestionAssumptionAssignmentItem->assumptionLevel(), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result == RESULT_OK )
										{
										if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
											{
											// Correct new question
											if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, secondarySpecificationItem->assumptionLevel(), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result == RESULT_OK )
												{
												if( ( oldCorrectedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem->oldCorrectedAssumptionByOppositeQuestionJustificationItem() ) != NULL )
													{
													if( ( secondarySpecificationWordItem = oldCorrectedAssumptionByOppositeQuestionJustificationItem->secondarySpecificationWordItem() ) != NULL )
														{
														// Correct generalization assumption
														if( ( primarySpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != NULL )
															{
															if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result == RESULT_OK )
																{
																if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
																	{
																	if( foundSpecificationItem->attachJustificationToSpecification( oldCorrectedAssumptionByOppositeQuestionJustificationItem ) != RESULT_OK )
																		return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );
																	}
																else
																	return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the corrected generalization assumption" );
																}
															else
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification substitution assumption" );
															}
														else
															return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );
														}
													else
														return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );
													}
												else
													return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I could find the old corrected assumption by opposite question justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any specification for the old suggestive question assumption" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is no assignment" );
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );
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

	ResultType findPossessiveSpecifications( unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *definitionWordItem, WordItem *specificationWordItem )
		{
		bool hasFoundPossessiveSpecificationWord = true;
		unsigned int possessiveSpecificationCollectionNr;
		SpecificationItem *possessiveSpecificationItem;
		SpecificationItem *previousPossessiveSpecificationItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		WordItem *previousPossessiveSpecificationWordItem;
		WordItem *higherLevelPossessiveSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossessiveSpecifications";

		foundPossessiveSpecificationItem_ = NULL;
		foundOppositePossessiveSpecificationItem_ = NULL;

		if( generalizationWordItem != NULL )
			{
			if( definitionWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					// First try to find a possessive specification on a higher level
					if( ( possessiveSpecificationItem = definitionWordItem->firstNonQuestionSpecificationItem() ) != NULL )
						{
						do	{
							if( !possessiveSpecificationItem->isExclusiveSpecification() &&
							possessiveSpecificationItem->isPossessive() &&
							possessiveSpecificationItem->isSpecificationNoun() &&
							( possessiveSpecificationWordItem = possessiveSpecificationItem->specificationWordItem() ) != NULL )
								{
								if( possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
								possessiveSpecificationWordItem->firstActiveAssignmentOrSpecificationItem( false, true, false, false, NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, specificationWordItem ) != NULL )
									{
									if( higherLevelPossessiveSpecificationWordItem == NULL )
										higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
									else
										hasFoundPossessiveSpecificationWord = false;
									}
								}
							}
						while( ( possessiveSpecificationItem = possessiveSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

						if( hasFoundPossessiveSpecificationWord &&
						( possessiveSpecificationItem = definitionWordItem->firstNonQuestionSpecificationItem() ) != NULL )
							{
							if( possessiveSpecificationItem->hasSpecificationCollection() )
								{
								if( higherLevelPossessiveSpecificationWordItem == NULL )
									higherLevelPossessiveSpecificationWordItem = specificationWordItem;

								// Try to find the opposite of the possessive specification
								do	{
									if( !possessiveSpecificationItem->isExclusiveSpecification() &&
									!possessiveSpecificationItem->isPartOf() &&
									possessiveSpecificationItem->isPossessive() &&
									( possessiveSpecificationWordItem = possessiveSpecificationItem->specificationWordItem() ) != NULL )
										{
										if( possessiveSpecificationWordItem != generalizationWordItem )
											{
											if( possessiveSpecificationWordItem->isSingularNoun() &&
											previousPossessiveSpecificationItem != NULL &&
											( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem->specificationWordItem() ) != NULL )
												{
												possessiveSpecificationCollectionNr = possessiveSpecificationItem->specificationCollectionNr();

												if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
												higherLevelPossessiveSpecificationWordItem->hasCollectionNr( possessiveSpecificationCollectionNr ) &&
												previousPossessiveSpecificationItem->specificationCollectionNr() == possessiveSpecificationCollectionNr )
													foundOppositePossessiveSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveSpecificationItem );

												foundPossessiveSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveSpecificationItem : previousPossessiveSpecificationItem );
												}

											previousPossessiveSpecificationItem = possessiveSpecificationItem;
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );
										}
									}
								while( ( possessiveSpecificationItem = possessiveSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
								}
							}
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given definition word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

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
				if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result == RESULT_OK )
					{
					if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() &&
					specificationResult.createdJustificationItem != NULL &&
					( olderGeneralizationAssumptionBySpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
						{
						if( ( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem->olderGeneralizationAssumptionBySpecificationJustificationItem() ) != NULL )
							{
							if( generalizationWordItem->replaceJustification( false, olderGeneralizationAssumptionBySpecificationJustificationItem, specificationResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item" );
							}
						}
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminAssumption( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundSuggestiveQuestionAssumption_ = false;
		isAssumption_ = false;

		lastCreatedRelationContextNr_ = NO_CONTEXT_NR;

		foundPossessiveSpecificationItem_ = NULL;
		foundOppositePossessiveSpecificationItem_ = NULL;
		possessiveSpecificationItem_ = NULL;
		secondarySpecificationItem_ = NULL;

		generalizationAssumptionWordItem_ = NULL;
		oppositePossessiveSpecificationWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminAssumption" );

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

	void initializeAdminAssumptionVariables()
		{
		hasFoundSuggestiveQuestionAssumption_ = false;
		isAssumption_ = false;

		secondarySpecificationItem_ = NULL;
		generalizationAssumptionWordItem_ = NULL;
		}

	bool hasFoundSuggestiveQuestionAssumption()
		{
		return hasFoundSuggestiveQuestionAssumption_;
		}

	ResultType makeExclusiveSpecificationSubstitutionAssumption( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isOppositePossessiveSpecificationCollectedWithItself;
		bool isPossessiveSpecificationCollectedWithItself;
		bool hasRelationContext = ( relationWordItem != NULL );
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		WordItem *currentWordItem;
		WordItem *possessiveSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		possessiveSpecificationItem_ = NULL;
		secondarySpecificationItem_ = NULL;

		oppositePossessiveSpecificationWordItem_ = NULL;

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				userSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

				if( userSpecificationItem != NULL &&

				( hasRelationContext ||

				( !userSpecificationItem->isOlderItem() &&

				( !isPossessive ||
				!userSpecificationItem->isExclusiveSpecification() ) ) ) )
					{
					// Initially not all words are linked. So, search all words.
					if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
						{
						do	{
							if( currentWordItem->isNoun() &&
							currentWordItem != specificationWordItem )
								{
								if( findPossessiveSpecifications( generalizationContextNr, specificationContextNr, generalizationWordItem, currentWordItem, specificationWordItem ) == RESULT_OK )
									{
									if( foundPossessiveSpecificationItem_ != NULL )
										{
										possessiveSpecificationItem_ = foundPossessiveSpecificationItem_;

										if( ( possessiveSpecificationWordItem = possessiveSpecificationItem_->specificationWordItem() ) != NULL )
											{
											if( ( primarySpecificationItem = possessiveSpecificationWordItem->firstActiveAssignmentOrSpecificationItem( false, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, specificationWordItem ) ) != NULL )
												{
												isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem->isCollectedWithItself();

												if( ( !userSpecificationItem->hasRelationContext() &&
												generalizationWordItem->hasAnsweredQuestion( primarySpecificationItem->specificationCollectionNr() ) ) ||

												( isPossessiveSpecificationCollectedWithItself &&
												!userSpecificationItem->isActiveAssignment() &&
												userSpecificationItem->specificationWordItem() == possessiveSpecificationWordItem ) )
													{
													// Skip making exclusive specification substitution assumption
													if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveSpecificationWordItem );

													if( existingUserSpecificationItem == NULL ||

													// Confirmed specification without relation word
													( relationWordItem != NULL &&

													( isPossessiveSpecificationCollectedWithItself ||
													!existingUserSpecificationItem->hasRelationContext() ) ) )
														{
														existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, possessiveSpecificationWordItem );

														// Make an exclusive specification substitution assumption - normal
														// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
														if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationContext ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, NULL, userSpecificationItem, NULL, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) ).result == RESULT_OK )
															{
															if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
																createdOrFoundSpecificationItem = specificationResult.foundSpecificationItem;
															else
																createdOrFoundSpecificationItem = createdSpecificationItem;

															if( createdOrFoundSpecificationItem != NULL )
																{
																if( isArchivedAssignment )
																	secondarySpecificationItem_ = generalizationWordItem->firstAssignmentItem( false, false, true, isPossessive, createdOrFoundSpecificationItem->questionParameter(), createdOrFoundSpecificationItem->relationContextNr(), createdOrFoundSpecificationItem->specificationWordItem() );
																else
																	secondarySpecificationItem_ = createdOrFoundSpecificationItem->updatedSpecificationItem();

																if( hasRelationContext &&
																createdSpecificationItem != NULL &&
																existingSpecificationItem != NULL &&
																existingSpecificationItem->hasRelationContext() &&
																existingSpecificationItem->isSelfGeneratedAssumption() &&
																// If cross-collected afterwards
																existingSpecificationItem->relationContextNr() != createdSpecificationItem->relationContextNr() &&
																( firstJustificationItem = existingSpecificationItem->firstJustificationItem() ) != NULL )
																	{
																	if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) == RESULT_OK )
																		{
																		if( generalizationWordItem->attachJustification( firstJustificationItem, createdSpecificationItem ) != RESULT_OK )
																			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the justifications of the existing specification to the created assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																		}
																	else
																		return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the existing specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "No specification item was created or found" );
															}
														else
															return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}

									// First opposite possessive specification
									if( !commonVariables_->hasShownWarning &&
									possessiveSpecificationWordItem != NULL &&
									foundOppositePossessiveSpecificationItem_ != NULL &&
									( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveSpecificationItem_->specificationWordItem() ) != NULL )
										{
										if( secondarySpecificationItem_ == NULL )
											{
											secondarySpecificationItem_ = userSpecificationItem;

											// Find another primary definition
											if( !hasRelationContext ||
											isPossessive )
												anotherPrimarySpecificationItem = possessiveSpecificationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );
											}

										existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

										if( secondarySpecificationItem_ != NULL &&

										( !secondarySpecificationItem_->isOlderItem() ||
										secondarySpecificationItem_->isSelfGenerated() ) &&

										( existingSpecificationItem == NULL ||
										existingSpecificationItem->hasRelationContext() ||
										existingSpecificationItem->isOlderItem() ) &&

										// Skip if user specification exists
										generalizationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == NULL )
											{
											isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_->isCollectedWithItself();
											existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, isNegative, !isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

											// Make an exclusive specification substitution assumption - opposite
											// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
											if( ( specificationResult = addSelfGeneratedSpecification( false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationContext ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, foundOppositePossessiveSpecificationItem_, ( isOppositePossessiveSpecificationCollectedWithItself ? NULL : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationCollectedWithItself ? userSpecificationItem : secondarySpecificationItem_ ), NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result == RESULT_OK )
												{
												if( !isPossessive )
													{
													if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
														{
														createdJustificationItem = specificationResult.createdJustificationItem;

														if( createdJustificationItem != NULL &&
														createdJustificationItem->isActiveItem() &&
														!generalizationWordItem->isJustificationInUse( createdJustificationItem ) )
															{
															if( generalizationWordItem->replaceOrDeleteJustification( createdJustificationItem ) != RESULT_OK )
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														{
														if( hasRelationContext &&
														existingSpecificationItem != NULL &&
														existingSpecificationItem->hasRelationContext() &&
														!existingSpecificationItem->isReplacedOrDeletedItem() &&
														// If cross-collected afterwards
														existingSpecificationItem->relationContextNr() != createdSpecificationItem->relationContextNr() &&
														( firstJustificationItem = existingSpecificationItem->firstJustificationItem() ) != NULL )
															{
															if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) == RESULT_OK )
																{
																if( generalizationWordItem->attachJustification( firstJustificationItem, createdSpecificationItem ) != RESULT_OK )
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach the created possessive reversible assumption justification to the created assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															else
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace the existing specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );
											}
										}
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find possessive specifications in word \"", currentWordItem->anyWordTypeString(), "\"" );
								}
							}
						while( !commonVariables_->hasShownWarning &&
						( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

						if( !commonVariables_->hasShownWarning &&
						hasRelationContext &&
						secondarySpecificationItem_ != NULL )
							{
							if( makePossessiveConditionalSpecificationAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem_, userSpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );
					}
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short specificQuestionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionByGeneralization";

		if( adminItem_->isReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
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
									currentGeneralizationWordItem->isNoun() &&
									!currentGeneralizationWordItem->hasCollection() &&
									!currentGeneralizationWordItem->hasPossessiveButNonQuestionSpecification() &&
									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
										{
										// Get secondary specification for justification
										if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
											{
											if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME )
												{
												if( makeGeneralizationAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											{
											// Get secondary question specification for justification
											if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, specificQuestionParameter, specificationWordItem ) ) != NULL )
												{
												if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, isNegative, false, isPossessive, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, NULL ) != RESULT_OK )
													myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
								}
							while( !commonVariables_->hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
							}
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word type isn't a noun" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		return RESULT_OK;
		}

	ResultType makeGeneralizationAssumptionBySpecification( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasGeneralizationWordConfirmedSpecification;
		bool isGeneralizationCollectedWithItself;
		bool isSpecificationCollectedWithItself;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *userSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionBySpecification";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( generalizationWordItem->isUserGeneralizationWord )
						userSpecificationItem = adminItem_->userSpecificationItem();

					if( generalizationWordItem != specificationWordItem &&
					( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem() ) != NULL )
						{
						hasGeneralizationWordConfirmedSpecification = generalizationWordItem->hasConfirmedSpecification();
						isGeneralizationCollectedWithItself = generalizationWordItem->isCollectedWithItself();
						isSpecificationCollectedWithItself = specificationWordItem->isCollectedWithItself();

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&
								currentGeneralizationWordItem->isNoun() &&

								( ( !isPossessive &&
								currentGeneralizationWordItem->hasNonPossessiveAndNonQuestionPartOfSpecification() ) ||

								( !currentGeneralizationWordItem->hasCollection() &&
								!currentGeneralizationWordItem->hasPossessiveButNonQuestionSpecification() ) ) )
									{
									existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

									// Skip if concluded conclusion exists
									if( ( existingSpecificationItem == NULL ||
									existingSpecificationItem != userSpecificationItem ) &&

									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
										{
										// Get secondary specification for justification
										if( userSpecificationItem != NULL &&
										!userSpecificationItem->isPossessive() &&

										( isSpecificationCollectedWithItself ||
										userSpecificationItem->specificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() ) )
											secondarySpecificationItem = userSpecificationItem;
										else
											secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, ( isSpecificationCollectedWithItself ? NO_CONTEXT_NR : lastCreatedRelationContextNr_ ), specificationWordItem );

										if( secondarySpecificationItem != NULL &&

										( !isSpecificationCollectedWithItself ||
										existingSpecificationItem == NULL ||
										generalizationWordItem->isUserRelationWord ||
										!secondarySpecificationItem->isHiddenSpecification() ) )
											{
											if( primarySpecificationItem->isSpecificationGeneralization() )
												{
												if( primarySpecificationItem->isPartOf() &&
												// Skip creating an assumption, if a conclusion exists
												generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isNegative, false, false, currentGeneralizationWordItem ) == NULL )
													{
													// Draw a "part of" generalization conclusion
													// Not yet explained in the design
													if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem->generalizationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ) ).result == RESULT_OK )
														{
														if( ( hasGeneralizationWordConfirmedSpecification ||
														!secondarySpecificationItem->hasAssumptionLevel() ) &&

														( createdJustificationItem = specificationResult.createdJustificationItem ) != NULL &&
														( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem->specificationSubstitutionPartOfAssumptionJustificationItem( primarySpecificationItem ) ) != NULL )
															{
															if( generalizationWordItem->updateJustificationInSpecifications( false, false, false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) == RESULT_OK )
																{
																if( isSpecificationCollectedWithItself &&
																specificationSubstitutionPartOfAssumptionJustificationItem->isActiveItem() &&
																!generalizationWordItem->isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) )
																	{
																	if( generalizationWordItem->replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != RESULT_OK )
																		return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the specification substitution part-of assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										}
									}
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
							}
						while( !commonVariables_->hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
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
						if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, generalizationAssumptionWordItem_, NULL ).result != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a generalization assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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

		return RESULT_OK;
		}

	ResultType makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		SpecificationResultType specificationResult;
		unsigned int currentSpecificationCollectionNr;
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
									if( ( specificationResult = generalizationWordItem->findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, specificationWordItem, NULL ) ).result == RESULT_OK )
										{
										if( specificationResult.relatedSpecificationItem != NULL )
											negativeQuestionSpecificationWordItem = specificationResult.relatedSpecificationItem->specificationWordItem();

										// Draw an indirectly answered question conclusion
										// Not yet explained in the design
										if( ( specificationResult = addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, indirectPrimarySpecificationItem, NULL, userSpecificationItem, NULL, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result == RESULT_OK )
											{
											if( negativeQuestionSpecificationWordItem != NULL &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL &&
											( commonWordItem = currentQuestionSpecificationWordItem->commonWordItem( currentSpecificationCollectionNr ) ) != NULL )
												{
												if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, commonWordItem ) ) != NULL &&
												( anotherPrimarySpecificationItem = commonWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != NULL )
													{
													// Draw a negative assumption
													// Not yet explained in the design
													if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, NULL, generalizationWordItem, negativeQuestionSpecificationWordItem, NULL ).result != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeQuestionSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}

											currentQuestionSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is collected with itself" );
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
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

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
						( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem->firstSpecificationGeneralizationItem() ) != NULL )
							{
							do	{
								if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
									{
									if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != NULL &&
									( anotherPrimarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != NULL )
										{
										// Make an only option left assumption
										// Not yet explained in the design
										if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
								}
							while( !commonVariables_->hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
							}
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item has no specification collection" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		return RESULT_OK;
		}

	ResultType makePossessiveConditionalSpecificationAssumption( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *collectedWithItselfSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isOppositePossessiveSpecificationCollectedWithItself;
		bool isPossessiveReversibleAssumption;
		bool isPossessiveSpecificationCollectedWithItself;
		unsigned short assumptionJustificationTypeNr;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *feminineSpecificationWordItem;
		WordItem *possessiveSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makePossessiveConditionalSpecificationAssumption";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( secondarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( relationWordItem != NULL )
							{
							// Skip if global variables are not assigned
							if( possessiveSpecificationItem_ != NULL &&
							secondarySpecificationItem_ != NULL &&
							( possessiveSpecificationWordItem = possessiveSpecificationItem_->specificationWordItem() ) != NULL )
								{
								isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem->isCollectedWithItself();

								if( isPossessiveSpecificationCollectedWithItself ||
								possessiveSpecificationItem_->specificationCollectionNr() == secondarySpecificationItem->specificationCollectionNr() )
									{
									isPossessiveReversibleAssumption = ( secondarySpecificationItem->specificationWordItem() == possessiveSpecificationWordItem );

									if( ( ( !isPossessiveSpecificationCollectedWithItself &&
									!possessiveSpecificationWordItem->isUserSpecificationWord &&
									// Skip if possessive specification exists
									relationWordItem->secondarySpecificationJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL ) ||

									( isPossessiveSpecificationCollectedWithItself &&
									!secondarySpecificationItem->isOlderItem() &&
									collectedWithItselfSpecificationItem != NULL &&
									collectedWithItselfSpecificationItem->isAssignment() ) ) &&

									// Skip if user specification exists
									relationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, specificationContextNr, possessiveSpecificationWordItem ) == NULL )
										{
										// Make a possessive conditional specification assumption - normal
										// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
										if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, isNegative, isPartOf, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, ( isPossessive ? possessiveSpecificationItem_ : NULL ), NULL, ( isPossessiveSpecificationCollectedWithItself ? collectedWithItselfSpecificationItem : secondarySpecificationItem ), NULL, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result == RESULT_OK )
											{
											if( isPossessive &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
												{
												if( adminItem_->drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, !isPossessive, false, createdSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdSpecificationItem->relationContextNr(), relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );
										}

									if( !commonVariables_->hasShownWarning )
										{
										if( oppositePossessiveSpecificationWordItem_ != NULL )
											{
											isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_->isCollectedWithItself();

											isPossessiveReversibleAssumption = ( secondarySpecificationItem->specificationWordItem() == oppositePossessiveSpecificationWordItem_ );
											assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

											if( ( existingSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isNegative, isPossessive, false, oppositePossessiveSpecificationWordItem_ ) ) == NULL &&
											( tempSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != NULL )
												{
												if( collectedWithItselfSpecificationItem != NULL &&
												!tempSpecificationItem->hasCurrentCreationSentenceNr() &&
												!tempSpecificationItem->hasNewInformation() &&
												tempSpecificationItem->specificationCollectionNr() == collectedWithItselfSpecificationItem->specificationCollectionNr() )
													existingSpecificationItem = tempSpecificationItem;
												}

											// Skip if assumption already exists as user specification
											if( ( existingSpecificationItem == NULL ||
											!existingSpecificationItem->hasRelationContext() ) &&

											( ( !isOppositePossessiveSpecificationCollectedWithItself &&
											// Skip if already exists as self-generated assumption
											relationWordItem->secondarySpecificationJustificationItem( assumptionJustificationTypeNr, secondarySpecificationItem ) == NULL ) ||

											( isOppositePossessiveSpecificationCollectedWithItself &&
											collectedWithItselfSpecificationItem != NULL ) ) )
												{
												// Make a possessive conditional specification assumption - opposite
												// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
												if( ( specificationResult = addSelfGeneratedSpecification( false, false, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, assumptionJustificationTypeNr, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, ( isPossessiveReversibleAssumption ? NULL : possessiveSpecificationItem_ ), NULL, ( isOppositePossessiveSpecificationCollectedWithItself && collectedWithItselfSpecificationItem != NULL ? collectedWithItselfSpecificationItem : secondarySpecificationItem ), NULL, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result == RESULT_OK )
													{
													if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
														{
														foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

														if( !isPossessiveReversibleAssumption &&
														foundSpecificationItem->isOlderItem() )
															{
															if( secondarySpecificationItem->isSelfGeneratedAssumption() &&
															!secondarySpecificationItem->isHiddenSpecification() &&
															secondarySpecificationItem->oppositePossessiveConditionalSpecificationAssumptionJustificationItem( foundSpecificationItem ) == NULL )
																{
																if( addMissingJustification( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem, possessiveSpecificationItem_, foundSpecificationItem, generalizationWordItem ) != RESULT_OK )
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add missing an opposite possessive conditional specification assumption justification to an assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}

															assumptionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isNegative, !isPossessive, true, oppositePossessiveSpecificationWordItem_ );

															if( assumptionSpecificationItem != NULL &&
															!assumptionSpecificationItem->isHiddenSpecification() &&
															assumptionSpecificationItem->possessiveReversibleAssumptionJustificationItem( foundSpecificationItem ) == NULL )
																{
																if( addMissingJustification( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, assumptionSpecificationItem, NULL, foundSpecificationItem, generalizationWordItem ) != RESULT_OK )
																	return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a missing possessive reversible assumption justification to an assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														}

													// Check for feminine specification word
													if( isOppositePossessiveSpecificationCollectedWithItself &&
													oppositePossessiveSpecificationWordItem_->isMasculineWord() &&
													// Select feminine specification word
													( feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem() ) != NULL )
														{
														if( ( isPossessive &&
														generalizationWordItem->hasFeminineProperNameEnding() ) ||

														( !isPossessive &&
														relationWordItem->hasFeminineProperNameEnding() ) )
															{
															if( addSelfGeneratedSpecification( false, false, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, possessiveSpecificationItem_, NULL, collectedWithItselfSpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												else
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );
												}
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );
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
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		return RESULT_OK;
		}

	ResultType makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		unsigned int secondarySpecificationCollectionNr;
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( secondarySpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

					// Check if specification doesn't exist yet
					if( specificationWordItem->isCollectedWithItself() ||
					generalizationWordItem->firstNonQuestionAssignmentOrSpecificationItem( true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) == NULL )
						{
						// Check if a related specification exists
						if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, NULL ) ).result == RESULT_OK )
							{
							if( specificationResult.relatedSpecificationItem == NULL )
								{
								if( generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() )
									{
									if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									// Make a suggestive question assumption
									// Not yet explained in the design
									hasFoundSuggestiveQuestionAssumption_ = true;

									if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the secondary specification" );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType addSelfGeneratedSpecification( bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipAdditionalConclusionOrAskQuestion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool hasFoundSelfGeneratedAssumption;
		bool hasFoundRelationContext = false;
		bool hasRelationContext = ( relationWordItem != NULL );
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		unsigned short justificationTypeNr = assumptionJustificationTypeNr;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *foundJustificationItem = NULL;
		JustificationItem *createdOrFoundJustificationItem = NULL;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSelfGeneratedSpecification";

		isAssumption_ = adminItem_->isAssumption( assumptionJustificationTypeNr );
		lastCreatedRelationContextNr_ = NO_CONTEXT_NR;

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				// Check assumption
				if( isAssumption_ &&
				adminItem_->assumptionGrade( assumptionJustificationTypeNr ) == 0 &&

				( primarySpecificationItem == NULL ||
				!primarySpecificationItem->hasAssumptionLevel() ) &&

				( anotherPrimarySpecificationItem == NULL ||
				!anotherPrimarySpecificationItem->hasAssumptionLevel() ) &&

				( secondarySpecificationItem == NULL ||
				!secondarySpecificationItem->hasAssumptionLevel() ) )
					{
					// Is conclusion
					if( adminItem_->isConclusion( conclusionJustificationTypeNr ) )
						{
						isAssumption_ = false;
						justificationTypeNr = conclusionJustificationTypeNr;
						}
					else
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given conclusion justification type isn't a conclusion" );
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( isAssumption_ ||
					questionParameter > NO_QUESTION_PARAMETER ||

					// Check conclusion justifications
					( ( primarySpecificationItem == NULL ||
					!primarySpecificationItem->isQuestion() ) &&

					( secondarySpecificationItem == NULL ||
					!secondarySpecificationItem->isQuestion() ) ) )
						{
						if( ( justificationResult = generalizationWordItem->addJustification( isForcingNewJustification, justificationTypeNr, NO_ORDER_NR, commonVariables_->currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, NULL ) ).result == RESULT_OK )
							{
							createdJustificationItem = justificationResult.createdJustificationItem;
							foundJustificationItem = justificationResult.foundJustificationItem;

							if( ( createdOrFoundJustificationItem = ( createdJustificationItem == NULL ? foundJustificationItem : createdJustificationItem ) ) != NULL )
								{
								if( ( specificationResult = adminItem_->addSpecification( isAssignment, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation( justificationTypeNr ), prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result == RESULT_OK )
									{
									if( !commonVariables_->hasShownWarning )
										{
										createdSpecificationItem = specificationResult.createdSpecificationItem;

										if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
											hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

										if( !isPossessive &&
										justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
										!specificationWordItem->isCollectedWithItself() )
											{
											secondarySpecificationItem_ = ( foundSpecificationItem != NULL &&

																			( !foundSpecificationItem->isOlderItem() ||
																			myWordItem_->nContextWordsInAllWords( foundSpecificationItem->relationContextNr(), specificationWordItem ) > commonVariables_->nUserRelationWords ) ? foundSpecificationItem : createdSpecificationItem );
											}

										if( createdSpecificationItem == NULL )
											{
											if( foundSpecificationItem != NULL )
												{
												hasFoundSelfGeneratedAssumption = foundSpecificationItem->isSelfGeneratedAssumption();

												if( hasFoundRelationContext &&
												hasFoundSelfGeneratedAssumption &&
												generalizationWordItem->isUserGeneralizationWord &&
												justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION )
													{
													// Check for confirmation of at least one relation word
													if( generalizationWordItem->checkForConfirmedJustifications( secondarySpecificationItem, foundSpecificationItem ) != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for confirmed justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( commonVariables_->result == RESULT_OK )
													{
													if( foundSpecificationItem->isSelfGenerated() )
														{
														if( createdJustificationItem == NULL )
															{
															if( foundSpecificationItem->hasCurrentCreationSentenceNr() &&
															!foundSpecificationItem->hasFoundJustification( false, foundJustificationItem ) )
																{
																// Change the justification of the found specification by the found justification
																if( foundSpecificationItem->changeFirstJustification( foundJustificationItem ) != RESULT_OK )
																	myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to change the first justification of the found justification by a conclusion justification" );
																}
															}
														else
															{
															// A justification has been created, but the self-generated specification already exists
															// So, the justification needs to be added or replaced separately
															if( hasFoundSelfGeneratedAssumption ||

															( !isAssumption_ &&
															foundSpecificationItem->isSelfGeneratedConclusion() ) )
																{
																// Attach created justification to the found specification
																if( generalizationWordItem->attachJustification( createdJustificationItem, foundSpecificationItem ) == RESULT_OK )
																	{
																	if( isAssumption_ &&
																	isGeneralizationProperName &&
																	!isPossessive &&

																	( isNegative ||

																	( hasRelationContext &&
																	generalizationWordItem->isUserRelationWord ) ||

																	// Assumption will be concluded
																	( !hasRelationContext &&
																	!generalizationWordItem->hasFoundAnswerToQuestion() ) ) )
																		{
																		if( foundSpecificationItem->isHiddenSpecification() )
																			// Clear sentence write buffer of hidden specification
																			strcpy( foundSpecificationItem->lastWrittenSentenceString, EMPTY_STRING );
																		else
																			{
																			if( generalizationWordItem->recalculateAssumptions() != RESULT_OK )
																				myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to recalculate assumptions of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																			}
																		}
																	}
																else
																	myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														}
													else
														{
														if( createdJustificationItem != NULL &&
														!generalizationWordItem->isJustificationInUse( createdJustificationItem ) )
															myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "A justification has been created, but the specification already exists" );
														}
													}
												}
											else
												myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find or create a specification" );
											}
										else
											{
											lastCreatedRelationContextNr_ = createdSpecificationItem->relationContextNr();

											if( !hasRelationContext ||
											!hasFoundRelationContext ||
											generalizationWordItem->hasConfirmedSpecification() )
												{
												// Check self-generated specification for integrity
												if( generalizationWordItem->writeSelectedSpecification( true, true, false, false, NO_ANSWER_PARAMETER, createdSpecificationItem ) == RESULT_OK )
													{
													if( strlen( commonVariables_->writeSentenceString ) > 0 )
														{
														if( hasRelationContext &&
														isGeneralizationProperName )
															{
															if( adminItem_->collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, lastCreatedRelationContextNr_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );
															}
														}
													else
														myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "Integrity error! I couldn't write the self-generated specification with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
													}
												else
													myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the self-generated specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to check the writing integrity" );
												}
											else
												{
												if( foundSpecificationItem != NULL &&
												foundSpecificationItem->relationContextNr() == lastCreatedRelationContextNr_ )
													myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "Integrity error! A specification was created in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" while this specification was found as well" );
												}
											}

										if( commonVariables_->result == RESULT_OK &&
										!isSkipAdditionalConclusionOrAskQuestion &&
										adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) &&

										( createdSpecificationItem != NULL ||

										( createdJustificationItem != NULL &&
										createdJustificationItem->isActiveItem() ) ) )
											{
											if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption_, false, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) == RESULT_OK )
												{
												if( isUniqueUserRelation &&
												createdSpecificationItem != NULL )
													{
													// Draw a unique relation conclusion
													// Not yet explained in the design
													if( addSelfGeneratedSpecification( false, false, isEveryGeneralization, isExclusiveSpecification, false, !isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, createdSpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								else
									myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a specification" );
								}
							else
								myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find or create a justification" );
							}
						else
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a justification" );
						}
					else
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The primary specification item or secondary specification item of this conclusion, is a question or an assumption, which can't be a justification for a conclusion" );
					}
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.createdJustificationItem = createdJustificationItem;
		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"He is my loving ally and my fortress,
 *	my tower of safety, my rescuer.
 *	He is my shield, and I take refuge in him.
 *	He makes the nations submit to me." (Psalm 144:2)
 *************************************************************************/
