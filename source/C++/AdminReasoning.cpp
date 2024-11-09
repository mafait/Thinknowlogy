/*	Class:			AdminReasoning
 *	Supports class:	AdminItem
 *	Purpose:		Providing autonomous reasoning
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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
#include "FindSpecificationResultType.cpp"
#include "GeneralizationItem.cpp"

class AdminReasoning
	{
	friend class AdminItem;

	// Private constructed variables

	SpecificationItem *oppositePossessiveDefinitionSpecificationItem_ = NULL;
	SpecificationItem *possessiveDefinitionSpecificationItem_ = NULL;

	WordItem *firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;
	WordItem *secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;

//Java_private_final
	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminReasoning";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


	// Private functions

	bool isFeminineOrMasculineProperNounEnding( bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isFeminineSpecificationWord;
		bool isMasculineSpecificationWord;

		if( generalizationWordItem != NULL &&
		specificationWordItem != NULL )
			{
			isFeminineSpecificationWord = specificationWordItem->isFeminineWord();
			isMasculineSpecificationWord = specificationWordItem->isMasculineWord();

			if( isFeminineSpecificationWord ||
			isMasculineSpecificationWord )
						// Possessive (relation)
				return ( ( isPossessive &&
						relationWordItem != NULL &&

							( ( isFeminineSpecificationWord &&
							relationWordItem->hasFeminineProperNounEnding() ) ||

							( isMasculineSpecificationWord &&
							relationWordItem->hasMasculineProperNounEnding() ) ) ) ||

						// Non-possessive (generalization)
						( !isPossessive &&

							( ( isFeminineSpecificationWord &&
							generalizationWordItem->hasFeminineProperNounEnding() ) ||

							( isMasculineSpecificationWord &&

								( generalizationWordItem->hasMasculineProperNounEnding() ||

								// Typical for Spanish
								( specificationWordItem->isNounWordSpanishAmbiguous() &&
								generalizationWordItem->hasFeminineProperNounEnding() ) ) ) ) ) );
			}

		return false;
		}

	signed char addPartOfJustificationToRelationCollectionWords( unsigned int relationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		SpecificationItem *partOfSpecificationItem;
		WordItem *currentCollectionWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addPartOfJustificationToRelationCollectionWords";

		if( relationCollectionNr == NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The relation collection number is undefined" );

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( ( currentCollectionWordItem = adminItem_->firstCollectionWordItem( relationCollectionNr ) ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( partOfSpecificationItem = currentCollectionWordItem->firstPartOfSpecificationItem() ) != NULL &&
				// Justification doesn't exist yet in specification
				!partOfSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) &&
				// Justification has at least the same assumption level
				partOfSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
				// Add part-of justification to part-of specification
				partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a part-of justification to a part-of specification in word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestions( bool isSpanishCurrentLanguage, SpecificationItem *simpleUserSpecificationItem, SpecificationItem *userSpecificationItem )
		{
		bool hasUserSpecificationCurrentlyMoreSpecificSpecification;
		bool hasUserSpecificationNonCompoundCollection;
		bool isAllowingSpanishPossessiveSpecification;
		bool isArchivedUserAssignment;
		bool isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
		bool isCurrentTouchedRelationWord;
		bool isPossessiveUserSpecification;
		unsigned int candidateForQuestionSpecificationCollectionNr;
		unsigned int nRelationWords;
		unsigned int nUserRelationWords;
		SpecificationItem *additionalDefinitionSpecificationItem = NULL;
		SpecificationItem *candidateForQuestionSpecificationItem;
		SpecificationItem *firstExclusiveSpecificationItem;
		SpecificationItem *foundQuestionSpecificationItem;
		WordItem *currentTouchedWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( ( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		isPossessiveUserSpecification = userSpecificationItem->isPossessive();

		if( userSpecificationItem->isProperNounGeneralization() )
			{
			additionalDefinitionSpecificationItem = getAdditionalDefinitionSpecificationItem( userSpecificationWordItem );

			if( userSpecificationItem->isExclusiveSpecification() )
				{
				// Question part doesn't exist yet
				if( !userGeneralizationWordItem->hasJustification( userSpecificationItem, NULL, NULL ) )
					{
					isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();

					// Ask question after more specific non-exclusive user specification
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedUserAssignment, isArchivedUserAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, userGeneralizationWordItem, userSpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about the current user sentence" );
					}
				}
			else
				{
				if( isPossessiveUserSpecification )
					{
					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					!userGeneralizationWordItem->hasFeminineOrMasculineProperNounEnding() &&

						( userSpecificationItem->isSpecificationWordSpanishAmbiguous() ||
						!userGeneralizationWordItem->hasActiveQuestionWithCompoundCollection() ) &&

					// Ask questions about generalization word with possessive primary specification
					askSpanishQuestionAboutGeneralizationWord( true, userSpecificationItem->specificationCollectionNr(), userSpecificationItem, additionalDefinitionSpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with possessive user specification as primary specification" );
					}
				else
					{
					if( userGeneralizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
					// Avoid idle call
					userGeneralizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( false ) != NULL &&
					// Correct suggestive assumptions by opposite suggestive question
					correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( userSpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question in word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			hasUserSpecificationCurrentlyMoreSpecificSpecification = userGeneralizationWordItem->hasCurrentlyMoreSpecificSpecification();
			hasUserSpecificationNonCompoundCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			nRelationWords = userSpecificationItem->nRelationWords();
			nUserRelationWords = globalVariables_->nUserRelationWords;

			// Do for all words touched during current sentence
			do	{
				if( currentTouchedWordItem->isProperNounWord() )
					{
					isCurrentTouchedRelationWord = currentTouchedWordItem->isUserRelationWord;
					// Typical for Spanish
					isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
																isCurrentTouchedRelationWord );

					if( ( candidateForQuestionSpecificationItem = currentTouchedWordItem->firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL )
						{
						if( !candidateForQuestionSpecificationItem->hasRelationWord() &&
						candidateForQuestionSpecificationItem->hasSpecificationCollection() &&
						candidateForQuestionSpecificationItem->isSelfGeneratedSpecification() )
							candidateForQuestionSpecificationItem = userSpecificationItem;

						candidateForQuestionSpecificationCollectionNr = candidateForQuestionSpecificationItem->specificationCollectionNr();
						foundQuestionSpecificationItem = currentTouchedWordItem->firstSpecificationItem( false, false, false, true );

						if( ( ( simpleUserSpecificationItem != NULL &&

							// Non-possessive user specification
							( ( !isPossessiveUserSpecification &&

								// Not touched relation word
								( ( !isCurrentTouchedRelationWord &&
								candidateForQuestionSpecificationItem == simpleUserSpecificationItem &&

									// Not Spanish
									( ( !isSpanishCurrentLanguage &&

										( userSpecificationItem == simpleUserSpecificationItem ||

										( foundQuestionSpecificationItem == NULL &&

										// Test file: "Your question is more specific (about John)"
										( nUserRelationWords == 1 ||
										// Test file: "Paul is the son of John and Anna"
										nRelationWords < nUserRelationWords ) ) ) ) ||

									// Typical for Spanish
									// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
									( isSpanishCurrentLanguage &&
									userSpecificationItem == simpleUserSpecificationItem &&
									foundQuestionSpecificationItem != NULL &&
									foundQuestionSpecificationItem->isOlderItem() ) ) ) ||

								// Touched relation word
								( isCurrentTouchedRelationWord &&
								// Typical for Spanish
								isSpanishCurrentLanguage &&
								// Positive specification doesn't exist
								currentTouchedWordItem->bestMatchingRelationSpecificationItem( false, false, candidateForQuestionSpecificationCollectionNr, NULL ) == NULL ) ) ) ||

							// Possessive user specification
							// Test file: "Sentence with possessive verb"
							( isPossessiveUserSpecification &&
							isCurrentTouchedRelationWord &&
							// No question exists
							foundQuestionSpecificationItem == NULL ) ) ) ||

						// Negative specification doesn't exist
						currentTouchedWordItem->bestMatchingRelationSpecificationItem( true, false, candidateForQuestionSpecificationCollectionNr, NULL ) == NULL ) &&

						// Ask questions
						askQuestions( isAllowingSpanishPossessiveSpecification, isSpanishCurrentLanguage, additionalDefinitionSpecificationItem, userSpecificationItem, currentTouchedWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );

						if( additionalDefinitionSpecificationItem != NULL &&
						( firstExclusiveSpecificationItem = currentTouchedWordItem->firstExclusiveSpecificationItem( false ) ) != NULL &&

							// Not touched relation word
							( ( !isCurrentTouchedRelationWord &&

								( nUserRelationWords <= 1 ||

								( hasUserSpecificationNonCompoundCollection &&
								!candidateForQuestionSpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() ) ) ) ||

							// Touched relation word
							( isCurrentTouchedRelationWord &&

								( hasUserSpecificationCurrentlyMoreSpecificSpecification ||

								( candidateForQuestionSpecificationItem->isOlderItem() &&

									( candidateForQuestionSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == NULL ||

									// Test file: "reasoning\family\Complex (5)"
									( hasUserSpecificationNonCompoundCollection &&
									// Positive specification doesn't exist
									currentTouchedWordItem->bestMatchingRelationSpecificationItem( false, false, candidateForQuestionSpecificationCollectionNr, NULL ) == NULL ) ||

									// Typical for Chinese
									isChineseCurrentLanguage ) ) ||

								( !firstExclusiveSpecificationItem->isOlderItem() &&
								!candidateForQuestionSpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() &&
								!currentTouchedWordItem->hasFeminineOrMasculineProperNounEnding() &&
								// Adjective questions don't exist yet
								currentTouchedWordItem->firstAdjectiveSpecificationItem( false, true ) == NULL ) ||

								// Typical for Spanish
								isAllowingSpanishPossessiveSpecification ) ) ) &&

						// Ask questions with question as primary specification
						askQuestionsWithQuestionAsPrimarySpecification( userSpecificationItem, currentTouchedWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestions( bool isAllowingSpanishPossessiveSpecification, bool isSpanishCurrentLanguage, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool isExclusiveUserSpecification;
		bool isPossessiveUserSpecification;
		bool isPrimarySameAsUserSpecification;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		bool isSkippingSimpleQuestions;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *archivedPrimarySpecificationItem;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *existingQuestionSpecificationItem;
		SpecificationItem *positiveSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem->firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL )
			{
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			do	{
				if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
					{
					if( primarySpecificationItem->isPossessive() )
						{
						if( !hasGeneralizationWordFeminineOrMasculineProperNounEnding )
							{
							selectedPrimarySpecificationItem = ( ( archivedPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem ) ) != NULL &&
																archivedPrimarySpecificationItem->isArchivedAssignment() ? archivedPrimarySpecificationItem :
																															primarySpecificationItem );

							// Typical for Spanish
							// Specification with user specification collection doesn't exist yet
							if( generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NULL ) == NULL &&
							// Ask questions about generalization word with possessive primary specification
							askSpanishQuestionAboutGeneralizationWord( isPossessiveUserSpecification, userSpecificationCollectionNr, selectedPrimarySpecificationItem, additionalDefinitionSpecificationItem, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask Spanish questions about word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
							}
						}
					else
						{
						isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
						isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();

						if( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
						isPrimarySpecificationWordSpanishAmbiguous ||
						primarySpecificationItem->hasCompoundSpecificationCollection() ||

						// Test file: "Additional test"
						( isPrimarySameAsUserSpecification &&
						!primarySpecificationItem->hasRelationWord() ) ) &&

						( exclusiveSecondarySpecificationItem = primarySpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL &&
						( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL )
							{
							isSkippingSimpleQuestions = false;

							if( !isUserGeneralizationWord )
								{
								if( isPossessiveUserSpecification )
									{
									// Typical for Spanish
									if( isUserSpecificationWordSpanishAmbiguous &&
									// Number of relation words of user specification item
									userSpecificationItem->nEnteredRelationWords() > 1 &&

										// Skip if current questions exist
										( ( questionSpecificationItem = generalizationWordItem->firstActiveQuestionSpecificationItem() ) == NULL ||
										questionSpecificationItem->isOlderItem() ) &&

										( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
										// Skip invalid questions / similar specification doesn't exist yet
										generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL )
										{
										isSkippingSimpleQuestions = true;

										// Ask Spanish question about word relation word with possessive primary specification
										if( askSpanishQuestionAboutRelationWord( primarySpecificationItem->isArchivedAssignment(), primarySpecificationItem->generalizationWordTypeNr(), userSpecificationItem, generalizationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a Spanish question about relation word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
										}
									}
								else
									{
									if( generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem->isArchivedAssignment(), false, false, false, primarySpecificationWordItem, NULL ) != NULL )
										isSkippingSimpleQuestions = true;
									}
								}

							if( !isSkippingSimpleQuestions &&
							// Positive specification doesn't exist
							generalizationWordItem->firstSpecificationItem( false, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&

								// Question specification doesn't exist
								( ( ( ( existingQuestionSpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem( exclusiveSecondarySpecificationWordItem ) ) == NULL ||
								existingQuestionSpecificationItem->isOlderItem() ||

								( !isPrimarySameAsUserSpecification &&
								!primarySpecificationItem->isOlderItem() ) ) &&

									( ( isPrimarySameAsUserSpecification &&
									// Negative specification doesn't exist
									generalizationWordItem->firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == NULL ) ||

									// Question specification doesn't exist
									( existingQuestionSpecificationItem == NULL &&
									generalizationWordItem->firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == NULL ) ||

									// Test file: "This information is more specific (non-exclusive)"
									( isExclusiveUserSpecification &&
									!primarySpecificationItem->hasSpecificationCollection() ) ||

									// Question specification exists
									( existingQuestionSpecificationItem != NULL &&
									// Justification doesn't exist yet in question
									!existingQuestionSpecificationItem->hasSecondarySpecificationJustification( exclusiveSecondarySpecificationItem ) &&
									// Negative specification doesn't exist
									generalizationWordItem->bestMatchingRelationSpecificationItem( true, false, userSpecificationCollectionNr, NULL ) == NULL ) ) ) ||

								// Positive specification doesn't exist
								( ( positiveSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, userSpecificationCollectionNr, NULL ) ) == NULL &&
								// Question specification doesn't exist
								existingQuestionSpecificationItem == NULL ) ||

								// Typical for Spanish
								( isPrimarySpecificationWordSpanishAmbiguous &&
								positiveSpecificationItem != NULL &&
								!positiveSpecificationItem->isOlderItem() ) ) &&

							// Ask simple questions
							askSimpleQuestions( isSpanishCurrentLanguage, exclusiveSecondarySpecificationItem, primarySpecificationItem, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask simple questions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem->nextCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestionsWithQuestionAsPrimarySpecification( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasUserSpecificationCompoundCollection;
		bool isArchivedAssignment;
		bool isFeminineOrMasculineUserSpecificationWord;
		bool isPossessiveUserSpecification;
		bool isUserRelationWord;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestionsWithQuestionAsPrimarySpecification";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem( false ) ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			isFeminineOrMasculineUserSpecificationWord = userSpecificationWordItem->isFeminineOrMasculineWord();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();

			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( ( hasUserSpecificationCompoundCollection ||
				primarySpecificationItem->hasCompoundSpecificationCollection() ) &&

				( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
				( secondarySpecificationItem = primarySpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Question part doesn't exist yet as specification
				generalizationWordItem->firstSpecificationItem( false, false, false, secondarySpecificationWordItem ) == NULL &&

					( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					isUserRelationWord ) ||

					// Skip adjective specification words of user generalization word
					!secondarySpecificationItem->isAdjectiveSpecification() ) &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == NULL ||
				// Justification doesn't exist yet in question
				!existingSpecificationItem->hasSecondarySpecificationJustification( secondarySpecificationItem ) ) &&

					( !isFeminineOrMasculineUserSpecificationWord ||
					!isPossessiveUserSpecification ||
					// Typical for Spanish
					existingSpecificationItem != NULL ) )
					{
					isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

					// Ask a specification substitution question
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem->nextExclusiveSpecificationItem( false ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askSimpleQuestions( bool isSpanishCurrentLanguage, SpecificationItem *exclusiveSecondarySpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasExclusiveSecondarySpecificationCompoundCollection;
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasPrimarySpecificationRelationWord;
		bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection;
		bool hasSkippedAskingQuestion = false;
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationRelationWord;
		bool isArchivedAssignment;
		bool isCompoundGeneralizationWordSpanishAmbiguous;
		bool isOlderPrimarySpecification;
		bool isOlderUserSpecification;
		bool isPossessiveUserSpecification;
		bool isPrimarySameAsUserSpecification;
		bool isPrimaryUserSpecification;
		bool isUserGeneralizationWord;
		bool isUserQuestion;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *exclusiveSpecificationItem;
		SpecificationItem *existingQuestionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *exclusiveSpecificationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSimpleQuestions";

		if( exclusiveSecondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given exclusive secondary specification item is undefined" );

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();
		hasPrimarySpecificationRelationWord = primarySpecificationItem->hasRelationWord();
		isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();
		isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
		isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
		isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();
		generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();
		primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

		hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
		hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();
		isOlderUserSpecification = userSpecificationItem->isOlderItem();
		isPossessiveUserSpecification = userSpecificationItem->isPossessive();
		isUserQuestion = userSpecificationItem->isQuestion();
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
		userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

		hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
		hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection = generalizationWordItem->hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		do	{
			hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem->hasCompoundSpecificationCollection();

			if( ( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL )
				{
				exclusiveSpecificationItem = NULL;
				foundSpecificationItem = NULL;

				if( ( exclusiveSpecificationItem = exclusiveSecondarySpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
					{
					// Do for all exclusive specifications of this word (including adjectives)
					do	{
						if( ( exclusiveSpecificationWordItem = exclusiveSpecificationItem->specificationWordItem() ) != NULL )
							foundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, exclusiveSpecificationWordItem );
						}
					while( foundSpecificationItem == NULL &&
					( exclusiveSpecificationItem = exclusiveSpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
					}

				if( foundSpecificationItem == NULL )
					{
					if( ( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
						{
						isCompoundGeneralizationWordSpanishAmbiguous = ( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem->compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL &&
																		compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() );

						existingQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem );

						// Useful user specification without relation
						if( ( ( hasPrimarySpecificationCompoundCollection &&
						isPrimaryUserSpecification &&
						!exclusiveSecondarySpecificationItem->isAdjectiveSpecification() &&

							( !isUserSpecificationWordSpanishAmbiguous ||

							// Typical for Spanish
							( isPrimarySameAsUserSpecification &&
							!hasPrimarySpecificationRelationWord &&
							!exclusiveSecondarySpecificationWordItem->isFeminineWord() ) ) ) ||

						// No useful user specification without relation
						( !hasUserSpecificationCompoundCollection &&
						!hasPrimarySpecificationRelationWord &&
						!isOlderPrimarySpecification &&

							// Test file: "reasoning\family\Complex (16)"
							( ( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
							hasPrimarySpecificationCompoundCollection ) ||

							( isPrimaryUserSpecification &&
							exclusiveSecondarySpecificationItem->isUserSpecification() ) ) ) ||

						// Compound collection
						( hasExclusiveSecondarySpecificationCompoundCollection &&
						hasUserSpecificationRelationWord &&

							( ( hasPrimarySpecificationCompoundCollection &&

								( hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
								!exclusiveSecondarySpecificationWordItem->isFeminineWord() ) ) ||

							( !isUserGeneralizationWord &&

								( !isPossessiveUserSpecification ||

								( !isUserSpecificationWordSpanishAmbiguous &&
								primarySpecificationWordItem->isUserSpecificationWord ) ) ) ) ) ||

						// No compound collection
						// Added additional justification
						( !hasExclusiveSecondarySpecificationCompoundCollection &&
						!hasPrimarySpecificationRelationWord &&
						hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection &&

							// Test file: "reasoning\Scientific challenge"
							( ( existingQuestionSpecificationItem == NULL &&
							!hasUserSpecificationCompoundCollection ) ||

							( existingQuestionSpecificationItem != NULL &&
							!existingQuestionSpecificationItem->hasSecondarySpecificationCompoundCollectionJustification() ) ) ) ||

						// Avoid adding non-compound justification to compound question
						( primarySpecificationCollectionNr > NO_COLLECTION_NR &&

							( hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection ||

							( isUserGeneralizationWord &&
							!hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
							!isSpanishCurrentLanguage ) ) ) ) &&

						// Typical for Spanish
						// Spanish test file: "Especificación-generalización - Especificación individual"
						( ( isCompoundGeneralizationWordSpanishAmbiguous &&
						!isUserSpecificationWordSpanishAmbiguous ) ||

						// Question part doesn't exist yet
						!generalizationWordItem->hasJustification( primarySpecificationItem, NULL, exclusiveSecondarySpecificationItem ) ) )
							{
							// Has possibly a gap in knowledge
							if( ( ( !isOlderPrimarySpecification ||
							isUserQuestion ) &&

							// Negative specification doesn't exist yet
							( generalizationWordItem->firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&
							generalizationWordItem->hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationCollectionNr, primarySpecificationItem ) ) ) ||

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (17)"
							( isUserSpecificationWordSpanishAmbiguous &&
							!hasPrimarySpecificationRelationWord &&
							compoundGeneralizationWordItem != primarySpecificationWordItem ) )
								{
								// Avoid asking invalid questions:
								// 1) Check for existing knowledge with same specification collection
								if( generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, (unsigned short)NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, exclusiveSecondarySpecificationWordItem ) == NULL &&
								// 2) Check for existing negative specification
								generalizationWordItem->firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&

								( existingQuestionSpecificationItem == NULL ||

								( ( !isSpanishCurrentLanguage ||
								// Typical for Spanish
								// Avoid almost duplicate justifications
								!hasPrimarySpecificationRelationWord ||
								!isPossessiveUserSpecification ) &&

								// Justification doesn't exist yet in specification
								!existingQuestionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, exclusiveSecondarySpecificationItem ) ) ) )
									{
									// Ask a specification substitution question
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );

									if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
									createdOrFoundSpecificationItem->isCorrectedSpecification() )
										{
										if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ == NULL )
											firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = primarySpecificationWordItem;
										else
											{
											if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != primarySpecificationWordItem )
												secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = primarySpecificationWordItem;
											}
										}
									}
								else
									hasSkippedAskingQuestion = true;
								}
							else
								{
								if( hasExclusiveSecondarySpecificationCompoundCollection &&
								hasUserSpecificationCompoundCollection &&
								!isOlderUserSpecification &&
								isUserGeneralizationWord &&
								primarySpecificationWordItem->hasCollectionNr( userSpecificationCollectionNr ) &&
								exclusiveSecondarySpecificationWordItem->nonCompoundCollectionNr() > NO_COLLECTION_NR &&
								// Question exists
								generalizationWordItem->firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem ) != NULL &&
								// Find question to be adjusted by compound collection
								findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationItem, userSpecificationItem, generalizationWordItem, primarySpecificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection about word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );
								}
							}
						}
					}
				else
					{
					// Test file: "Skip invalid questions (1)"
					// Specification doesn't exist yet
					if( generalizationWordItem->firstSpecificationItem( false, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&
					// Make exclusive specification substitution assumption
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, exclusiveSecondarySpecificationItem, exclusiveSpecificationItem, foundSpecificationItem, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}
		while( !hasSkippedAskingQuestion &&
		( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );

		return RESULT_OK;
		}

	signed char askSpanishQuestionAboutGeneralizationWord( bool isPossessiveUserSpecification, unsigned int userSpecificationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasMultipleUserGeneralizationWords = ( globalVariables_->nUserGeneralizationWords > 1 );
		bool hasPrimarySpecificationCompoundCollection;
		bool hasPrimarySpecificationRelationWord;
		bool isOlderPrimarySpecification;
		bool isUserGeneralizationWord;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *existingQuestionSpecificationItem;
		WordItem *additionalDefinitionSpecificationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *tempWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpanishQuestionAboutGeneralizationWord";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given additional definition specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
		( exclusiveSecondarySpecificationItem = additionalDefinitionSpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();
			hasPrimarySpecificationRelationWord = primarySpecificationItem->hasRelationWord();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();

			do	{
				// Current question part doesn't exist yet
				if( !generalizationWordItem->hasJustification( primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL &&

					// No answered questions
					( ( ( existingQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, false, WORD_PARAMETER_SINGULAR_VERB_IS ) ) == NULL &&
					isUserGeneralizationWord &&
					hasPrimarySpecificationRelationWord &&
					// User specification doesn't exist yet
					generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NULL ) == NULL ) ||

					( ( exclusiveSecondarySpecificationItem->isSelfGeneratedSpecification() ||

					( ( tempWordItem = exclusiveSecondarySpecificationWordItem->compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL &&
					!tempWordItem->isNounWordSpanishAmbiguous() ) ) &&

 						( hasMultipleUserGeneralizationWords ||
						isUserGeneralizationWord || 

						( isOlderPrimarySpecification &&
						!isPossessiveUserSpecification ) ||

						( hasPrimarySpecificationCompoundCollection &&
						!isPossessiveUserSpecification ) ) &&

					( existingQuestionSpecificationItem == NULL ||

					// Justification doesn't exist yet in specification
					( !existingQuestionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, exclusiveSecondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingQuestionSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, NULL ) ) ) &&

					// Positive specification doesn't exist yet
					generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, (unsigned short)NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL &&
					// Negative specification doesn't exist yet
					generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, (unsigned short)NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL ) ) &&

				// Ask a specification substitution question
				adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askSpanishQuestionAboutRelationWord( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationItem *exclusiveSecondarySpecificationItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpanishQuestionAboutRelationWord";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( !generalizationWordItem->hasJustification( userSpecificationItem, NULL, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Ask a specification substitution question
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByKnowledge( SpecificationItem *correctedSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned int correctedSpecificationCollectionNr;
		JustificationItem *attachedJustificationItem;
		JustificationItem *previousJustificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		WordItem *correctedSpecificationWordItem;
		WordItem *oppositeSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByKnowledge";

		if( correctedSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given corrected specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( correctedSpecificationWordItem = correctedSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The corrected specification item has no specification word" );

		correctedSpecificationCollectionNr = correctedSpecificationItem->specificationCollectionNr();

		// Negative specification
		if( ( obsoleteSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, correctedSpecificationWordItem ) ) != NULL )
			{
			// Write invalid negative suggestive assumption by knowledge
			if( generalizationWordItem->writeUpdatedSpecification( false, true, false, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid negative suggestive assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Replace invalid negative suggestive assumption by knowledge
			if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, correctedSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid negative suggestive assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		if( ( oppositeSpecificationWordItem = correctedSpecificationWordItem->collectionWordItem( correctedSpecificationCollectionNr ) ) != NULL &&
		( obsoleteSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, oppositeSpecificationWordItem ) ) != NULL )
			{
			// Write invalid opposite suggestive assumption by knowledge
			if( generalizationWordItem->writeUpdatedSpecification( false, true, false, false, false, false, false, obsoleteSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid opposite assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Replace invalid opposite suggestive assumption by knowledge
			if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, correctedSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid opposite assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		if( correctedSpecificationItem->hasNonCompoundSpecificationCollection() &&
		( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			// Do for all non-question specifications
			do	{
				if( currentSpecificationItem->isOlderItem() &&
				( previousJustificationItem = currentSpecificationItem->firstJustificationItem() ) != NULL &&
				currentSpecificationItem->hasCurrentCreationSentenceNr() )
					{
					while( ( attachedJustificationItem = previousJustificationItem->attachedJustificationItem() ) != NULL &&
					// Remove justification with replaced primary specification
					!attachedJustificationItem->isReplacedPrimarySpecification() &&
					// Remove corrupted justification
					attachedJustificationItem->primarySpecificationWordItem() == attachedJustificationItem->secondaryGeneralizationWordItem() )
						previousJustificationItem = attachedJustificationItem;

					// Found justification with replaced primary specification
					if( attachedJustificationItem != NULL )
						{
						if( previousJustificationItem->isOlderPrimarySpecification() )
							{
							// Write invalid negative suggestive assumption by knowledge
							if( generalizationWordItem->writeUpdatedSpecification( false, true, false, false, false, true, false, currentSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid negative suggestive assumption with older primary specification about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

							// Replace invalid negative suggestive assumption by knowledge
							if( generalizationWordItem->replaceOrDeleteSpecification( currentSpecificationItem, correctedSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid negative suggestive assumption with older primary specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							// Remove justification with replaced primary specification
							if( previousJustificationItem->changeAttachedJustification( attachedJustificationItem->isOlderItem() ? NULL : attachedJustificationItem->attachedJustificationItem() ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove a justification with replaced primary specification" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned int obsoleteSpecificationCollectionNr;
		SpecificationItem *obsoleteSpecificationItem;
		WordItem *obsoleteSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByOppositeSuggestiveQuestion";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Replace all invalid suggestive assumptions by opposite suggestive question
		if( ( obsoleteSpecificationItem = generalizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( false ) ) != NULL )
			{
			do	{
				if( ( obsoleteSpecificationCollectionNr = obsoleteSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( obsoleteSpecificationWordItem = obsoleteSpecificationItem->specificationWordItem() ) != NULL &&
				obsoleteSpecificationWordItem->isExclusiveCollection( obsoleteSpecificationCollectionNr ) )
					{
					// Write invalid suggestive assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem->writeUpdatedSpecification( false, false, true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid suggestive assumption by opposite suggestive question (obsolete) in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					// Replace invalid suggestive assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, userSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid suggestive assumption by opposite suggestive question (obsolete) in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					obsoleteSpecificationItem = generalizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( false );
					}
				else
					obsoleteSpecificationItem = obsoleteSpecificationItem->nextOlderNonPossessiveNonQuestionSpecificationItem();
				}
			while( obsoleteSpecificationItem != NULL );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *createdNewSpecificationItem;
		SpecificationItem *createdOldSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem *oldSuggestiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByOppositeSuggestiveQuestion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is no assignment" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem->suggestiveQuestionAssumptionSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, NULL, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != NULL )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem->firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add first old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the first old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );

			if( ( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add first new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdNewSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the first new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );
			}

		if( secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != NULL )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem->firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add second old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the second old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );

			if( ( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add second new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdNewSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the second new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeAdjectiveConclusions( bool isArchivedAssignment, bool isSpanishCurrentLanguage, unsigned short generalizationWordTypeNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned int specificationCollectionNr;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *collectionWordItem;
		WordItem *commonWordItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeAdjectiveConclusions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem->isAdjectiveSpecification() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no adjective specification" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = primarySpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( secondarySpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, primarySpecificationWordItem ) ) != NULL &&

					// Specification doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == NULL ||

					// User specification doesn't exist yet
					( existingSpecificationItem->isSelfGeneratedSpecification() &&
					// Justification doesn't exist yet in specification
					!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) ) ) )
					{
					// Draw negative adjective conclusion
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

					if( !isSpanishCurrentLanguage &&
					( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
					( specificationCollectionNr = createdOrFoundSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
					( collectionWordItem = currentGeneralizationWordItem->collectionWordItem( specificationCollectionNr ) ) != NULL &&
					( commonWordItem = currentGeneralizationWordItem->commonWordItem( specificationCollectionNr ) ) != NULL &&
					( foundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, commonWordItem ) ) != NULL &&
					foundSpecificationItem->hasRelationWord() &&
					foundSpecificationItem->isOlderItem() &&
					foundSpecificationItem->hasSpecificationCollection() &&
					// Positive specification doesn't exist
					generalizationWordItem->firstSpecificationItem( false, false, false, collectionWordItem ) == NULL &&
					( additionalDefinitionSpecificationItem = commonWordItem->firstExclusiveSpecificationItem( currentGeneralizationWordItem ) ) != NULL &&
					// Make indirectly answered question assumption
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, additionalDefinitionSpecificationItem, createdOrFoundSpecificationItem, NULL, generalizationWordItem, collectionWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", collectionWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool isPrimarySameAsSecondarySpecification;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		unsigned int additionalDefinitionSpecificationCollectionNr;
		SpecificationItem *additionalProperNounSpecificationItem = NULL;
		SpecificationItem *negativeConclusionPrimarySpecificationItem = NULL;
		SpecificationItem *negativeConclusionAdditionalDefinitionSpecificationItem = NULL;
		SpecificationItem *negativeConclusionSecondarySpecificationItem = NULL;
		SpecificationItem *negativeDefinitionSpecificationItem = NULL;
		WordItem *additionalDefinitionSpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given additional definition specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given additional definition specification item has no specification word item" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
		isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

		additionalDefinitionSpecificationCollectionNr = additionalDefinitionSpecificationItem->specificationCollectionNr();

		if( ( negativeDefinitionSpecificationItem = secondarySpecificationWordItem->firstSpecificationItem( true, false, false, additionalDefinitionSpecificationWordItem ) ) != NULL &&
		!isPrimarySpecificationWordSpanishAmbiguous )
			{
			// Test file: "Simple family definition - John and Anna - Paul, Joe and Laura"
			negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
			negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
			}
		else
			{
			if( isPrimarySpecificationWordSpanishAmbiguous &&
			negativeDefinitionSpecificationItem != NULL )
				{
				// Typical for Spanish
				// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
				negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
				negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
				}
			else
				{
				negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
				negativeConclusionAdditionalDefinitionSpecificationItem = additionalDefinitionSpecificationItem;
				negativeConclusionSecondarySpecificationItem = ( isPrimarySameAsSecondarySpecification ? additionalProperNounSpecificationItem :
																										secondarySpecificationItem );

				if( negativeConclusionSecondarySpecificationItem == NULL )
					// Typical for Spanish
					negativeConclusionSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, (unsigned short)NO_QUESTION_PARAMETER, additionalDefinitionSpecificationCollectionNr, NULL );
				}
			}

		if( negativeConclusionSecondarySpecificationItem == NULL )
			{
			// Typical for Spanish
			// Spanish test file: "razonamiento\Reto científico"
			if( generalizationWordItem->firstSpecificationItem( false, false, true, additionalDefinitionSpecificationWordItem ) == NULL &&
			// Ask a specification substitution question
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, NULL, negativeConclusionAdditionalDefinitionSpecificationItem, NULL, generalizationWordItem, additionalDefinitionSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", additionalDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
			}
		else
			{
			// Draw negative conclusion
			if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAdditionalDefinitionSpecificationItem, negativeConclusionSecondarySpecificationItem, NULL, generalizationWordItem, additionalDefinitionSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", additionalDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeDefinitionConclusions( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short negativeSpecificationWordTypeNr, SpecificationItem *negativeDefinitionSpecificationItem, WordItem *generalizationWordItem, WordItem *negativeSpecificationWordItem, WordItem *specificationWordItem )
		{
		bool isProperNounArchivedAssignment;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *positiveAdjectiveDefinitionSpecificationItem;
		SpecificationItem *positiveSpecificationItem;
		SpecificationItem *negativeSpecificationItem;
		SpecificationItem *negativeAdjectiveDefinitionSpecificationItem;
		SpecificationItem *secondaryAdjectiveDefinitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *reversedNegativeSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeDefinitionConclusions";

		if( negativeDefinitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given negative definition specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( negativeSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given negative specification word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( negativeSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE )
			{
			// Negative specification is adjective
			if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL )
				{
				// Do for all specification words
				do	{
					if( currentSpecificationWordItem != generalizationWordItem &&
					currentSpecificationWordItem != specificationWordItem &&
					currentSpecificationWordItem->isSingularNounWord() &&
					( positiveAdjectiveDefinitionSpecificationItem = currentSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != NULL &&
					( negativeAdjectiveDefinitionSpecificationItem = generalizationWordItem->firstNonExclusiveSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) != NULL )
						{
						// Negative specification doesn't exist yet
						if( ( ( existingSpecificationItem = currentSpecificationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) ) == NULL ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveAdjectiveDefinitionSpecificationItem, negativeAdjectiveDefinitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL ) ) ) &&

						// Draw reverse negative adjective conclusion (on generalization word)
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL, currentSpecificationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reverse negative adjective conclusion (on generalization word) about generalization word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						// Refresh negative adjective definition specification
						if( ( negativeAdjectiveDefinitionSpecificationItem = specificationWordItem->firstNonExclusiveSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) != NULL &&
						// Negative specification doesn't exist yet
						currentSpecificationWordItem->firstSpecificationItem( true, false, false, specificationWordItem ) == NULL &&
						// Draw reverse negative adjective conclusion (on specification word)
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL, currentSpecificationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reverse negative adjective conclusion (on specification word) about generalization word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
				}
			}
		else	// Negative specification is non-adjective
			{
			// Test file: "reasoning\Scientific challenge"
			if( !negativeDefinitionSpecificationItem->hasSpecificationCollection() &&
			( positiveAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&
			( secondaryAdjectiveDefinitionSpecificationItem = generalizationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&
			// Draw simple negative adjective conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, NULL, secondaryAdjectiveDefinitionSpecificationItem, NULL, negativeSpecificationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative adjective conclusion about generalization word \"", negativeSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			if( ( positiveAdjectiveDefinitionSpecificationItem = generalizationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&
			( negativeAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem->firstAdjectiveSpecificationItem( true, false ) ) != NULL &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == NULL ||

				// Justification doesn't exist yet in specification
				( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveAdjectiveDefinitionSpecificationItem, negativeAdjectiveDefinitionSpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL ) ) ) &&

			// Draw another simple negative adjective conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another simple negative adjective conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		if( ( definitionSpecificationItem = negativeSpecificationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) ) == NULL )
			definitionSpecificationItem = negativeDefinitionSpecificationItem;

		// Get first generalization specification proper noun word
		if( ( currentGeneralizationItem = generalizationWordItem->firstProperNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification proper noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->addError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( positiveSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, (unsigned short)NO_QUESTION_PARAMETER, NO_COLLECTION_NR, negativeSpecificationWordItem ) ) != NULL &&

					( ( existingSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, (unsigned short)NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationWordItem ) ) == NULL ||

					// Justification doesn't exist yet in specification
					( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL ) ) ) )
					{
					isProperNounArchivedAssignment = positiveSpecificationItem->isArchivedAssignment();

					// Draw negative conclusion
					if( adminItem_->addSelfGeneratedSpecification( false, isProperNounArchivedAssignment, isProperNounArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, generalizationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL, currentGeneralizationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextProperNounSpecificationGeneralizationItem() ) != NULL );
			}

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = generalizationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				currentGeneralizationWordItem != negativeSpecificationWordItem )
					{
					if( ( positiveSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != NULL &&
					!positiveSpecificationItem->isExclusiveSpecification() &&

						// Specification doesn't exist yet
						( ( existingSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) ) == NULL ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL ) ) ) &&

					// Draw negative adjective conclusion
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL, currentGeneralizationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( ( reversedNegativeSpecificationWordItem = negativeSpecificationWordItem->collectionWordItem( definitionSpecificationItem->specificationCollectionNr() ) ) != NULL &&
					( negativeSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, reversedNegativeSpecificationWordItem ) ) != NULL &&
					( positiveSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, reversedNegativeSpecificationWordItem ) ) != NULL &&

						// Specification doesn't exist yet
						( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == NULL ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, negativeSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, negativeSpecificationItem, NULL ) ) ) &&

					// Draw negative adjective conclusion
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, negativeSpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawNegativeExclusiveConclusions( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *negativePrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *exclusiveSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeExclusiveConclusions";

		if( negativePrimarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given negative primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( exclusiveSpecificationItem = specificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				// Test file: "reasoning\Knowledge of organisms (1)"
				if( ( secondarySpecificationWordItem = exclusiveSpecificationItem->specificationWordItem() ) != NULL &&
				// Specification doesn't exist yet
				generalizationWordItem->firstSpecificationItem( true, false, false, secondarySpecificationWordItem ) == NULL &&
				// Draw exclusive negative compound conclusion
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, exclusiveSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, negativePrimarySpecificationItem, NULL, exclusiveSpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an exclusive negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSpecificationItem = exclusiveSpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawOnlyOptionLeftProperNounConclusion( SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCreatedSpecification = false;
		bool isArchivedAssignment;
		bool hasFoundMoreMatches = false;
		unsigned int currentSpecificationCollectionNr;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *foundDefinitionSpecificationItem = NULL;
		SpecificationItem *negativeSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *foundDefinitionSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawOnlyOptionLeftProperNounConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			do	{
				if( currentSpecificationItem->isExclusiveSpecification() &&
				( currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				( negativeSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != NULL &&
				( definitionSpecificationItem = specificationWordItem->firstExclusiveSpecificationItem( false ) ) != NULL )
					{
					do	{
						if( definitionSpecificationItem->specificationWordItem() != currentSpecificationWordItem &&
						// Positive specification doesn't exist yet
						generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, (unsigned short)NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NULL ) == NULL &&
						definitionSpecificationItem->specificationCollectionNr() == currentSpecificationCollectionNr )
							{
							if( foundDefinitionSpecificationItem == NULL )
								foundDefinitionSpecificationItem = definitionSpecificationItem;
							else
								hasFoundMoreMatches = true;
							}
						}
					while( !hasFoundMoreMatches &&
					( definitionSpecificationItem = definitionSpecificationItem->nextExclusiveSpecificationItem( false ) ) != NULL );

					if( !hasFoundMoreMatches &&
					foundDefinitionSpecificationItem != NULL &&
					( foundDefinitionSpecificationWordItem = foundDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

						// Draw an only option left conclusion
						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, primarySpecificationItem->generalizationWordTypeNr(), primarySpecificationItem->specificationWordTypeNr(), primarySpecificationItem->relationWordTypeNr(), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, foundDefinitionSpecificationItem, negativeSpecificationItem, NULL, generalizationWordItem, foundDefinitionSpecificationWordItem, relationWordItem ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						hasCreatedSpecification = true;
						}
					}
				}
			while( !hasCreatedSpecification &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusions( bool isSpanishCurrentLanguage, SpecificationItem *primarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasExistingSpecificationCurrentCreationSentenceNr;
		bool hasPrimarySpecificationRelationWord;
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationRelationWord;
		bool isOlderPrimarySpecification;
		bool isOlderUserSpecification;
		bool isPossessivePrimarySpecification;
		bool isPossessiveUserSpecification;
		bool isPrimarySpecificationArchivedAssignment;
		bool isUserGeneralizationWord;
		bool isUserRelationWord;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *spanishMasculineSpecificationWordItem = NULL;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given user specification item is undefined" );

		if( ( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();
			isOlderUserSpecification = userSpecificationItem->isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();

			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			hasPrimarySpecificationRelationWord = primarySpecificationItem->hasRelationWord();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();
			isPrimarySpecificationArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

			if( hasPrimarySpecificationRelationWord &&
			isOlderPrimarySpecification &&
			!isPossessiveUserSpecification &&
			primarySpecificationItem->hasCurrentCreationSentenceNr() &&
			primarySpecificationItem->isSelfGeneratedAssumption() &&
			// Recalculate assumption level of primary specification
			primarySpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the primary specification" );

			// Typical for Spanish
			if( isSpanishCurrentLanguage &&
			specificationWordItem->isFeminineWord() &&
			!userSpecificationItem->isQuestion() )
				spanishMasculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem();

			// Do for all possessive noun words
			do	{
				if( ( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPartOfSpecificationItem( specificationWordItem ) ) == NULL &&
				spanishMasculineSpecificationWordItem != NULL )
					partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPartOfSpecificationItem( spanishMasculineSpecificationWordItem );

				if( partOfDefinitionSpecificationItem != NULL )
					{
					if( ( createdOrFoundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentPossessiveNounWordItem ) ) == NULL ||

					// Justification doesn't exist yet in word
					( !generalizationWordItem->hasJustification( primarySpecificationItem, NULL, partOfDefinitionSpecificationItem ) &&
					// Justification has at least the same assumption level
					createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) ) )
						{
						hasExistingSpecificationCurrentCreationSentenceNr = ( createdOrFoundSpecificationItem == NULL ? false : createdOrFoundSpecificationItem->hasCurrentCreationSentenceNr() );

						if( ( ( !isOlderPrimarySpecification &&

							// Typical for Spanish
							( isPossessivePrimarySpecification ||
							createdOrFoundSpecificationItem == NULL ||

							( !isSpanishCurrentLanguage &&
							hasUserSpecificationRelationWord ) ) ) ||

						( !isOlderUserSpecification &&

							( ( !isUserRelationWord &&
							hasUserSpecificationRelationWord ) ||

							// Non-possessive user specification
							( !isPossessiveUserSpecification &&

								( hasUserSpecificationRelationWord ||
								!isUserGeneralizationWord ||

								( ( hasPrimarySpecificationRelationWord &&
								!hasExistingSpecificationCurrentCreationSentenceNr ) ||

								// Not Spanish
								( !isSpanishCurrentLanguage &&
								// Test file: "No conflict on multiple related specifications"
								hasExistingSpecificationCurrentCreationSentenceNr ) ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&
								!hasPrimarySpecificationRelationWord ) ) ) ) ||

							// Possessive user specification
							// Typical for Spanish
							// Spanish test files: "razonamiento\familia\Complejo (2)",
							//						"razonamiento\familia\Complejo (17)"
							( isPossessiveUserSpecification &&
							hasUserSpecificationCompoundCollection &&
							primarySpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == NULL ) ) ) ) )
							{
							// Draw 'part of' generalization conclusion
							// See Block 1 and Block 2 of the included challenge document,
							// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

							createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
							}
						}

					if( createdOrFoundSpecificationItem != NULL &&
					!isPossessivePrimarySpecification &&

						// Non-possessive user specification
						// Test file: "reasoning\family\Laura - John and Anna - Joe"
						( ( !isPossessiveUserSpecification &&
						!hasPrimarySpecificationRelationWord &&
						!isSpanishCurrentLanguage ) ||

						// Possessive user specification
						( isPossessiveUserSpecification &&
						userSpecificationWordItem == specificationWordItem ) ) )
						{
						selectedPrimarySpecificationItem = ( isPossessiveUserSpecification ? userSpecificationItem :
																							userGeneralizationWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) );

						if( selectedPrimarySpecificationItem != NULL &&
						// Justification doesn't exist yet in specification
						!createdOrFoundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, partOfDefinitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) &&
						// Add part-of justification to part-of specification
						createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, NULL, partOfDefinitionSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a part-of justification to a part-of specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusions( bool isActiveUserAssignment, bool isSpanishCurrentLanguage, SpecificationItem *givenSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGivenNonCompoundSpecificationCollection;
		bool hasUserNonCompoundSpecificationCollection;
		bool isAssignment;
		bool isExistingUserSpecification = false;
		bool isGivenArchivedAssignment;
		bool isGivenExclusiveGeneralizationAssignment;
		bool isGivenExclusiveSpecification;
		bool isGivenOlderSpecification;
		bool isGivenSelfGeneratedAssumption;
		bool isGivenSelfGeneratedSpecification;
		bool isGivenUserAssignment;
		bool isOlderAlternativeSpecification;
		bool isOlderExistingSpecification = false;
		bool isOlderUserSpecification;
		bool isSpanishSpecificationFemaleWord = false;
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUniqueUserRelation;
		bool isUserAssignment;
		bool isUserGeneralizationWord;
		bool isGivenUserSpecification;
		unsigned short givenRelationWordTypeNr;
		unsigned short givenSpecificationWordTypeNr;
		unsigned int createdRelationCollectionNr;
		unsigned int givenSpecificationCollectionNr;
		unsigned int spanishAmbiguousSpecificationCollectionNr;
		JustificationItem *firstJustificationItem;
		JustificationItem *foundJustificationItem;
		SpecificationItem *collectedSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *additionalUserSpecificationItem;
		SpecificationItem *alternativeSpecificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentExistingSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *firstUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *possessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *spanishMasculineSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *additionalGeneralizationWordItem;
		WordItem *commonWordItem;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *partOfSpecificationWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveNounWordItem;
		WordItem *spanishMasculineSpecificationWordItem = NULL;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		CollectionResultType collectionResult;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusions";

		if( givenSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			hasUserNonCompoundSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			isUserAssignment = userSpecificationItem->isAssignment();

			hasGivenNonCompoundSpecificationCollection = givenSpecificationItem->hasNonCompoundSpecificationCollection();
			isGivenArchivedAssignment = givenSpecificationItem->isArchivedAssignment();
			isGivenExclusiveSpecification = givenSpecificationItem->isExclusiveSpecification();
			isGivenOlderSpecification = givenSpecificationItem->isOlderItem();
			isGivenSelfGeneratedAssumption = givenSpecificationItem->isSelfGeneratedAssumption();
			isGivenSelfGeneratedSpecification = givenSpecificationItem->isSelfGeneratedSpecification();
			isGivenUserAssignment = givenSpecificationItem->isUserAssignment();
			isGivenUserSpecification = givenSpecificationItem->isUserSpecification();
			givenRelationWordTypeNr = givenSpecificationItem->relationWordTypeNr();
			givenSpecificationWordTypeNr = givenSpecificationItem->specificationWordTypeNr();
			givenSpecificationCollectionNr = givenSpecificationItem->specificationCollectionNr();

			isGivenExclusiveGeneralizationAssignment = ( isGivenUserAssignment &&
														givenSpecificationItem->isExclusiveGeneralization() );

			// Compound generalization word
			if( ( possessiveNounWordItem = specificationWordItem->compoundGeneralizationWordItem( givenSpecificationCollectionNr ) ) != NULL )
				{
				if( specificationWordItem != userSpecificationWordItem &&
				possessiveDefinitionSpecificationItem_ != NULL &&
				oppositePossessiveDefinitionSpecificationItem_ != NULL )
					{
					possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem_;
					possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem();

					oppositePossessiveDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
					oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_->specificationWordItem();
					}
				else
					{
					// Find possessive specifications
					if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"", possessiveNounWordItem->anyWordTypeString(), "\"" );

					if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != NULL )
						possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem();

					if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != NULL )
						oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem->specificationWordItem();
					}
				}

			// Typical for Spanish
			if( isSpanishCurrentLanguage )
				{
				isSpecificationWordSpanishAmbiguous = givenSpecificationItem->isSpecificationWordSpanishAmbiguous();
				isSpanishSpecificationFemaleWord = specificationWordItem->isFeminineWord();

				if( isSpanishSpecificationFemaleWord )
					spanishMasculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem();
				}

			if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( true, true, false, true, specificationWordItem, generalizationWordItem ) ) != NULL )
				{
				isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
				isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
				}

			if( isGivenExclusiveSpecification )
				{
				// Specification doesn't exist yet
				if( existingSpecificationItem == NULL &&
				// Draw possessive reversible conclusion
				adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, givenRelationWordTypeNr, givenSpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, relationWordItem, specificationWordItem, generalizationWordItem ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with current context word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
			else
				{
				firstUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, specificationWordItem );

				isOlderUserSpecification = ( existingSpecificationItem == NULL &&
											firstUserSpecificationItem != NULL );

				isOlderAlternativeSpecification = ( ( alternativeSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, false, specificationWordItem, relationWordItem ) ) != NULL &&
													alternativeSpecificationItem->isOlderItem() );

				if( isOlderUserSpecification ||
				firstUserSpecificationItem == NULL ||

				// Typical for Spanish
				// Spanish test file: "razonamiento\familia\Luisa - José - Ana"
				( isSpanishCurrentLanguage &&
				!isOlderAlternativeSpecification &&
				givenSpecificationItem != alternativeSpecificationItem ) )
					{
					currentExistingSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, true, true, NO_QUESTION_PARAMETER, specificationWordItem->nonCompoundCollectionNr(), specificationWordItem );

					if( existingSpecificationItem == NULL ||

					( givenSpecificationItem == alternativeSpecificationItem &&
					// Justification doesn't exist yet in specification
					!existingSpecificationItem->hasPrimarySpecificationJustification( givenSpecificationItem ) &&

						( !isSpanishCurrentLanguage ||
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL, NULL ) ) &&

						( currentExistingSpecificationItem == NULL ||
						// Justification has at least the same assumption level
						currentExistingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL, NULL ) ||

						( isOlderExistingSpecification &&

							( relationWordItem->hasCurrentlyConfirmedSpecification() ||
							existingSpecificationItem->hasRelationCollectionCurrentlyBeenUpdated() ) ) ) ) )
						{
						if( !isSpecificationWordSpanishAmbiguous ||

						// Typical for Spanish
						( !isGivenUserAssignment &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							isGivenUserSpecification ||
							currentExistingSpecificationItem != NULL ) ) ||

						( isGivenUserSpecification &&
						!hasGeneralizationWordCurrentlyConfirmedSpecification ) )
							{
							isAssignment = ( isGivenArchivedAssignment ||
											isGivenExclusiveGeneralizationAssignment );

							isUniqueUserRelation = ( isGivenArchivedAssignment &&
													isGivenUserAssignment &&
													adminItem_->isUniqueUserRelation() );

							createdSpecificationItem = NULL;
							foundSpecificationItem = NULL;
							createdOrFoundSpecificationItem = NULL;

							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							existingSpecificationItem == NULL &&
							hasUserNonCompoundSpecificationCollection &&
							isGivenOlderSpecification )
								{
								if( ( secondarySpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
								// Draw Spanish feminine exclusive specification substitution assumption
								adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, givenRelationWordTypeNr, givenSpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, secondarySpecificationItem, NULL, relationWordItem, specificationWordItem, generalizationWordItem ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to a Spanish feminine exclusive specification substitution assumption about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;

								// Typical for Spanish
								if( isSpecificationWordSpanishAmbiguous &&
								existingSpecificationItem != NULL )
									{
									spanishAmbiguousSpecificationCollectionNr = existingSpecificationItem->specificationCollectionNr();

									if( !existingSpecificationItem->hasCurrentCreationSentenceNr() &&
									existingSpecificationItem->wasHiddenSpanishSpecification() )
										{
										// Write Spanish assumption that is not hidden anymore
										if( relationWordItem->writeUpdatedSpecification( false, false, false, false, false, false, true, existingSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"", relationWordItem->anyWordTypeString(), "\"" );

										// Add reversible justifications after Spanish specification not hidden anymore
										if( addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore( existingSpecificationItem, specificationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to reversible justifications after Spanish specification that is not hidden anymore in word \"", relationWordItem->anyWordTypeString(), "\"" );
										}
									}

								// Draw reversible conclusion
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isGivenArchivedAssignment, false, false, false, false, false, true, isUniqueUserRelation, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, givenRelationWordTypeNr, givenSpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
								createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
								}

							if( !isOlderUserSpecification &&
							!isSpanishCurrentLanguage &&
							isGivenSelfGeneratedSpecification &&
							createdOrFoundSpecificationItem != NULL &&
							createdOrFoundSpecificationItem->hasNonCompoundSpecificationCollection() &&

								( hasUserNonCompoundSpecificationCollection ||
								isActiveUserAssignment ||
								isGivenOlderSpecification ||

								( foundSpecificationItem != NULL &&
								!foundSpecificationItem->isOlderItem() ) ||

								givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL ) &&

							// Justification doesn't exist yet in specification
							!givenSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
							// Justification has at least the same assumption level
							givenSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL, NULL ) )
								{
								if( isGivenArchivedAssignment ||
								createdSpecificationItem == NULL ||

								// Not user generalization word
								( !isUserGeneralizationWord &&
								!isActiveUserAssignment ) ||

								// User generalization word
								( isUserGeneralizationWord &&

									( foundSpecificationItem == NULL ||

									( isActiveUserAssignment &&
									givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != NULL ) ) ) )
									{
									// Test file: "reasoning\family\Laura - Paul - John and Anna"
									if( isActiveUserAssignment &&
									createdSpecificationItem == NULL &&
									givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != NULL &&
									givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL &&
									( firstJustificationItem = givenSpecificationItem->firstJustificationItem() ) != NULL &&
									firstJustificationItem->isOlderItem() &&
									firstJustificationItem->isReversibleAssumptionOrConclusion() )
										givenSpecificationItem->clearStoredSentenceString();

									// Draw reversible conclusion
									if( adminItem_->addSelfGeneratedSpecification( false, isGivenArchivedAssignment, isGivenArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, givenSpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, relationWordItem ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}

								if( ( commonWordItem = specificationWordItem->commonWordItem( createdOrFoundSpecificationItem->specificationCollectionNr() ) ) != NULL &&
								( primarySpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
								// Justification doesn't exist yet in specification
								!createdOrFoundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, userSpecificationItem ) &&
								// Justification has at least the same assumption level
								createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, userSpecificationItem, NULL ) &&
								// Add user opposite possessive conclusion justification to created or found specification
								createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, userSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a user opposite possessive conclusion justification to the created or found specification" );

								if( createdSpecificationItem == NULL &&
								!createdOrFoundSpecificationItem->isOlderItem() )
									{
									if( ( createdRelationCollectionNr = createdOrFoundSpecificationItem->relationCollectionNr() ) > NO_COLLECTION_NR &&
									( partOfSpecificationItem = generalizationWordItem->firstPartOfSpecificationItem() ) != NULL &&
									( partOfSpecificationWordItem = partOfSpecificationItem->specificationWordItem() ) != NULL &&
									( partOfDefinitionSpecificationItem = partOfSpecificationWordItem->firstPartOfSpecificationItem( specificationWordItem ) ) != NULL &&
									// Add part-of justification to part-of specification in relation collection words
									addPartOfJustificationToRelationCollectionWords( createdRelationCollectionNr, createdOrFoundSpecificationItem, partOfDefinitionSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a part-of justification to a part-of specification in relation collection words" );

									// Test file: "reasoning\family\John - Anna (before family definition)"
									if( isActiveUserAssignment &&
									!isGivenOlderSpecification &&
									isUserGeneralizationWord &&
									( additionalUserSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, NO_COLLECTION_NR, specificationWordItem, NULL ) ) != NULL )
										{
										// Test file: "reasoning\family\John - Anna (before family definition)"
										// Justification doesn't exist yet in specification
										if( !givenSpecificationItem->hasPrimarySpecificationJustification( additionalUserSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, NULL, NULL, NULL ) &&
										// Add user reversible justification to current secondary specification
										givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a user reversible justification to the current secondary specification" );

										// Test file: "reasoning\family\John - Anna (before family definition)"
										if( ( additionalGeneralizationWordItem = additionalUserSpecificationItem->relationWordItem() ) != NULL &&
										( additionalSpecificationItem = additionalGeneralizationWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&
										// Justification doesn't exist yet in specification
										!givenSpecificationItem->hasPrimarySpecificationJustification( additionalSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, NULL, NULL, NULL ) &&
										// Add assumption reversible justification to current secondary specification
										givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an assumption reversible justification to the current secondary specification" );
										}
									}
								}

							// Not created
							if( createdSpecificationItem == NULL )
								{
								// Test file: "reasoning\family\John - Anna (before family definition)"
								if( isGivenOlderSpecification &&
								isUserAssignment &&
								!isUserGeneralizationWord &&
								existingSpecificationItem == NULL &&
								foundSpecificationItem != NULL &&
								!foundSpecificationItem->isOlderItem() &&
								// Recalculate assumption level of found specification
								foundSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );
								}
							else
								{
								// Created, not found
								if( foundSpecificationItem == NULL )
									{
									if( isUserGeneralizationWord )
										{
										if( specificationWordItem == userSpecificationWordItem )
											{
											if( possessiveDefinitionSpecificationWordItem != NULL &&
											( assumptionSpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
											assumptionSpecificationItem->hasRelationWord() )
												{
												if( ( secondarySpecificationItem = specificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
												// Justification doesn't exist yet in specification
												!assumptionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( createdSpecificationItem, secondarySpecificationItem ) &&
												// Justification has at least the same assumption level
												assumptionSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
												// Add specification substitution justification to additional specification
												assumptionSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );

												if( possessiveDefinitionSpecificationItem != NULL &&
												oppositePossessiveDefinitionSpecificationWordItem != NULL &&
												( additionalSpecificationItem = relationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
												!additionalSpecificationItem->isHiddenSpanishSpecification() )
													{
													assumptionSpecificationItem = assumptionSpecificationItem->updatedSpecificationItem();

													// Justification doesn't exist yet in specification
													if( !additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, assumptionSpecificationItem ) &&
													// Justification has at least the same assumption level
													additionalSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, assumptionSpecificationItem, NULL ) &&
													// Add opposite possessive conclusion justification to additional specification
													additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, assumptionSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to an additional specification" );
													}
												}
											}
										else
											{
											if( ( primarySpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
											// Skip archived user assignment
											primarySpecificationItem->hasRelationWord() &&
											( secondarySpecificationItem = userSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
											// Justification doesn't exist yet in specification
											!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
											// Add specification substitution justification to created specification
											createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the created specification" );
											}
										}

									if( isSpanishCurrentLanguage )
										{
										// Typical for Spanish
										if( ( obsoleteSpecificationItem = relationWordItem->noRelationWordSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : specificationWordItem ) ) ) == NULL )
											{
											if( !isUserAssignment &&
											( secondarySpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
											// Justification doesn't exist yet in specification
											!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
											// Add exclusive user specification substitution assumption justification to created specification
											createdSpecificationItem->addJustificationToSpecification( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an exclusive user specification substitution assumption justification to the created specification" );
											}
										else
											{
											// Write an assumption without relation word
											if( isGivenUserSpecification &&
											relationWordItem->writeUpdatedSpecification( true, false, false, false, true, false, false, obsoleteSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", relationWordItem->anyWordTypeString(), "\"" );

											// Replace an assumption without relation word
											if( relationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
											}
										}
									}
								else	// Created and found
									{
									if( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
									isUserGeneralizationWord &&
									!isOlderUserSpecification &&
									( foundJustificationItem = givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) != NULL &&
									( primarySpecificationItem = foundJustificationItem->primarySpecificationItem() ) != NULL &&
									( secondarySpecificationItem = foundJustificationItem->secondarySpecificationItem() ) != NULL &&
									// Justification doesn't exist yet in specification
									!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
									// Justification has at least the same assumption level
									createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
									// Add opposite possessive conclusion justification to created specification
									createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the created specification" );
									}

								if( !isGivenExclusiveGeneralizationAssignment &&
								isOlderUserSpecification &&
								foundSpecificationItem != NULL )
									{
									// Test file: "reasoning\family\Complex (19 - strange)"
									// Add reversible justification
									if( createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, " failed to add a reversible justification to the created specification" );

									// Update specification of justifications of involved words, except for current context word
									if( relationWordItem->updateSpecificationOfJustificationsOfInvolvedWords( false, foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( possessiveDefinitionSpecificationItem == NULL &&
									( commonWordItem = specificationWordItem->commonWordItem( givenSpecificationCollectionNr ) ) != NULL )
										possessiveDefinitionSpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, specificationWordItem );

									if( oppositePossessiveDefinitionSpecificationWordItem == NULL &&
									possessiveDefinitionSpecificationItem != NULL )
										oppositePossessiveDefinitionSpecificationWordItem = specificationWordItem->collectionWordItem( givenSpecificationCollectionNr );

									if( possessiveDefinitionSpecificationItem != NULL &&
									oppositePossessiveDefinitionSpecificationWordItem != NULL )
										{
										// Test file: "reasoning\family\Complex (19 - strange)"
										// Make an opposite possessive specification assumption
										if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationItem, NULL, createdSpecificationItem, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"", relationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

										// Test file: "reasoning\family\Complex (19 - strange)"
										if( ( additionalSpecificationItem = generalizationWordItem->firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
										// Justification doesn't exist yet in specification
										!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, createdSpecificationItem ) &&
										// Justification has at least the same assumption level
										additionalSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, createdSpecificationItem, NULL ) &&
										// Add opposite possessive conclusion justification to additional specification
										additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, createdSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the additional specification" );
										}
									}
								}

							givenSpecificationItem = givenSpecificationItem->updatedSpecificationItem();

							if( createdOrFoundSpecificationItem != NULL &&
							createdOrFoundSpecificationItem->assumptionLevel() == givenSpecificationItem->assumptionLevel() &&

							( !isSpanishCurrentLanguage ||
							// Typical for Spanish
							isGivenUserSpecification ||
							givenSpecificationItem->hasOnlyOneRelationWord() ) )
								{
								commonWordItem = ( hasGivenNonCompoundSpecificationCollection ? specificationWordItem->commonWordItem( givenSpecificationCollectionNr ) :
																								specificationWordItem->compoundGeneralizationWordItem( givenSpecificationCollectionNr ) );

								if( commonWordItem != NULL &&
								( partOfDefinitionSpecificationItem = commonWordItem->firstPartOfSpecificationItem( specificationWordItem ) ) != NULL &&

									( ( partOfSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, commonWordItem ) ) == NULL ||

									// Justification doesn't exist yet in part-of specification
									( !partOfSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( createdOrFoundSpecificationItem, partOfDefinitionSpecificationItem ) &&
									// Justification has at least the same assumption level
									partOfSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) ) ) &&

								// Draw 'part of' generalization conclusion
								// See Block 1 and Block 2 of the included challenge document,
								// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
								adminItem_->addSelfGeneratedSpecification( false, isGivenArchivedAssignment, isGivenArchivedAssignment, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL, generalizationWordItem, commonWordItem, NULL ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					else
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						!hasGivenNonCompoundSpecificationCollection &&
						isGivenOlderSpecification &&
						alternativeSpecificationItem != NULL &&
						givenSpecificationItem != alternativeSpecificationItem &&
						// Justification doesn't exist yet in specification
						!alternativeSpecificationItem->hasPrimarySpecificationJustification( givenSpecificationItem ) &&
						// Justification has at least the same assumption level
						alternativeSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL, NULL ) &&
						// Add Spanish reversible justification to alternative specification
						alternativeSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible justification to the alternative specification" );
						}
					}

				if( hasGivenNonCompoundSpecificationCollection &&
				isGivenSelfGeneratedSpecification &&
				!isSpanishCurrentLanguage &&
				alternativeSpecificationItem != NULL &&
				existingSpecificationItem == userSpecificationItem &&

					( isGivenSelfGeneratedAssumption ||
					!givenSpecificationItem->isOlderItem() ) &&

				// Make an opposite possessive specification assumption (based on reversible conclusion)
				makeOppositePossessiveSpecificationAssumption( isGivenArchivedAssignment, false, true, false, WORD_TYPE_PROPER_NOUN, givenSpecificationWordTypeNr, givenRelationWordTypeNr, ( isGivenSelfGeneratedAssumption ? alternativeSpecificationItem : givenSpecificationItem ), NULL, userSpecificationItem, relationWordItem, generalizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption during drawing of a reversible conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				if( existingSpecificationItem != NULL &&

				( isSpecificationWordSpanishAmbiguous ||
				// Test file: "reasoning\family\John - Anna (before family definition)"
				givenSpecificationCollectionNr == NO_COLLECTION_NR ||

				( hasGivenNonCompoundSpecificationCollection &&
				!isExistingUserSpecification &&
				isOlderExistingSpecification &&
				givenSpecificationItem == alternativeSpecificationItem &&
				!givenSpecificationItem->hasCurrentCreationSentenceNr() ) ||

				( isExistingUserSpecification &&
				givenSpecificationItem != alternativeSpecificationItem &&
				relationWordItem->hasCurrentlyConfirmedSpecification() ) ) )
					{
					// Alternative specification is same as current secondary specification
					if( ( alternativeSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, false, specificationWordItem, relationWordItem ) ) == givenSpecificationItem )
						{
						if( isGivenSelfGeneratedSpecification &&
						isOlderExistingSpecification &&
						existingSpecificationItem->hasNonCompoundSpecificationCollection() )
							{
							if( isGivenOlderSpecification &&
							// Secondary specification word is same as user specification word
							specificationWordItem == userSpecificationWordItem &&
							// Justification doesn't exist yet in specification
							!givenSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Possessive user specification doesn't exist
							relationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, specificationWordItem ) == NULL &&
							( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, specificationWordItem, userGeneralizationWordItem ) ) != NULL )
								{
								// Add reversible user justification
								if( givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

								givenSpecificationItem = givenSpecificationItem->updatedSpecificationItem();

								// Collect relation words
								if( ( collectionResult = adminItem_->collectRelationWords( isGivenExclusiveSpecification, true, givenRelationWordTypeNr, givenSpecificationWordTypeNr, relationWordItem, userGeneralizationWordItem, specificationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", relationWordItem->anyWordTypeString(), "\" to relation word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );

								if( ( createdRelationCollectionNr = collectionResult.createdCollectionNr ) > NO_COLLECTION_NR &&
								// Add relation collection number to found user specification
								generalizationWordItem->changeSpecification( givenSpecificationItem->isAnsweredQuestion(), givenSpecificationItem->isEveryGeneralization(), givenSpecificationItem->generalizationCollectionNr(), givenSpecificationItem->specificationCollectionNr(), createdRelationCollectionNr, givenSpecificationItem->firstJustificationItem(), givenSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation collection number to the given specification" );

								if( ( commonWordItem = specificationWordItem->commonWordItem( givenSpecificationCollectionNr ) ) != NULL &&
								( possessiveDefinitionSpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&
								( oppositePossessiveDefinitionSpecificationWordItem = specificationWordItem->collectionWordItem( givenSpecificationCollectionNr ) ) != NULL &&
								( oppositePossessiveDefinitionSpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
								( collectedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, (unsigned short)NO_QUESTION_PARAMETER, givenSpecificationItem->specificationCollectionNr(), NULL ) ) != NULL )
									{
									if( !collectedSpecificationItem->isOlderItem() &&
									// Justification doesn't exist yet in specification
									!givenSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( oppositePossessiveDefinitionSpecificationItem, collectedSpecificationItem ) &&
									// Add collected opposite possessive justification
									givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, NULL, collectedSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a collected opposite possessive justification to the current secondary specification" );

									// Justification doesn't exist yet in specification
									if( !collectedSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, conclusionSpecificationItem ) &&
									// Add opposite possessive conclusion justification to collected specification
									collectedSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, conclusionSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
									}
								}
							}
						}
					else	// Alternative is different from current secondary
						{
						if( alternativeSpecificationItem != NULL )
							{
							if( givenSpecificationItem->hasNonCompoundSpecificationCollection() )
								{
								if( isSpanishCurrentLanguage )
									{
									// Typical for Spanish
									// Spanish test file: "razonamiento\familia\Complejo (17)"
									if( !givenSpecificationItem->isReplacedOrDeletedItem() &&
									// Replace assumption by conclusion
									generalizationWordItem->replaceOrDeleteSpecification( givenSpecificationItem, alternativeSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a Spanish assumption specification by a conclusion specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( alternativeSpecificationItem->isSelfGeneratedAssumption() &&
									( oppositePossessiveDefinitionSpecificationWordItem = specificationWordItem->collectionWordItem( givenSpecificationCollectionNr ) ) != NULL &&
									( foundSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( false, false, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&
									foundSpecificationItem->hasSecondarySpecificationJustification( alternativeSpecificationItem ) &&
									// Replace assumption justification by conclusion justification of involved word
									relationWordItem->updateSpecificationOfJustificationsInWord( alternativeSpecificationItem, givenSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification items in word \"", relationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								{
								if( isGivenOlderSpecification &&
								!isOlderExistingSpecification &&
								alternativeSpecificationItem->isOlderItem() &&
								// Replace assumption by conclusion
								generalizationWordItem->replaceOrDeleteSpecification( alternativeSpecificationItem, givenSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption specification by a conclusion specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					}

				// Typical for Spanish
				if( isSpanishCurrentLanguage &&

					( isSpanishSpecificationFemaleWord ||

					( isSpecificationWordSpanishAmbiguous &&
					existingSpecificationItem != NULL ) ) )
					{
					if( specificationWordItem->isFeminineWord() &&
					( spanishMasculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem() ) != NULL )
						{
						spanishMasculineSpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem );

						// No existing specification
						if( ( existingSpecificationItem == NULL &&

							( spanishMasculineSpecificationItem == NULL ||

							( isUserGeneralizationWord &&
							!userSpecificationItem->isPossessive() ) ) ) ||

						// Existing specification
						( existingSpecificationItem != NULL &&
						isGivenSelfGeneratedSpecification &&
						!givenSpecificationItem->hasCurrentCreationSentenceNr() &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||

							( userSpecificationItem->hasRelationWord() &&

								( spanishMasculineSpecificationItem == NULL ||
								existingSpecificationItem == userSpecificationItem ||

								( isUserGeneralizationWord &&
								!userSpecificationItem->isPossessive() &&
								spanishMasculineSpecificationItem->isSelfGeneratedSpecification() ) ) ) ) ) )
							{
							// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
							givenSpecificationItem = givenSpecificationItem->updatedSpecificationItem();

							// Draw Spanish reversible conclusion
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, spanishMasculineSpecificationWordItem->nonCompoundCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, relationWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" and with masculine specification word \"", spanishMasculineSpecificationWordItem->anyWordTypeString(), "\"" );

							if( !isUserGeneralizationWord &&
							existingSpecificationItem == userSpecificationItem &&
							( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
							// Justification doesn't exist yet in question
							!createdOrFoundSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Justification has at least the same assumption level
							createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL, NULL ) &&
							// Add opposite possessive justification
							createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the created or found specification" );
							}
						}
					else
						{
						if( existingSpecificationItem != NULL &&
						relationWordItem->isUserGeneralizationWord &&
						givenSpecificationItem->isSelfGeneratedConclusion() )
							{
							if( existingSpecificationItem->hasCompoundSpecificationCollection() )
								{
								// Add Spanish non-compound reversible conclusion
								if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, givenSpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, specificationWordItem->nonCompoundCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, relationWordItem, specificationWordItem, generalizationWordItem ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish non-compound reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								// Spanish test file: "razonamiento\familia\Complejo (17)"
								if( existingSpecificationItem->isSelfGeneratedSpecification() &&
								// Justification doesn't exist yet in specification
								!existingSpecificationItem->hasPrimarySpecificationJustification( givenSpecificationItem ) &&
								// Add reversible justification to existing non-compound specification
								existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the existing non-compound specification" );
								}
							}
						}
					}

				if( isExistingUserSpecification &&
				isOlderExistingSpecification &&
				!isGivenOlderSpecification &&
				isGivenSelfGeneratedAssumption &&
				// Justification doesn't exist yet in specification
				!givenSpecificationItem->hasPrimarySpecificationJustification( existingSpecificationItem ) &&
				// Add reversible user justification
				givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, NULL, NULL ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );
				}
			}

		return RESULT_OK;
		}

	signed char drawSelectedReversibleConclusions( bool isAssignment, bool isArchivedAssignment, bool isDrawingPossessiveReversibleConclusions, bool isSpanishCurrentLanguage, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasCurrentSpecificationCreationSentenceNr;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		bool hasNonCompoundCurrentSpecificationCollection;
		bool hasUserGeneralizationWordCurrentlyConfirmedSpecification = false;
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationRelationWord = false;
		bool isArchivedUserAssignment = false;
		bool isCurrentSpecificationSelfGeneratedAssumption;
		bool isCurrentSpecificationSelfGeneratedSpecification;
		bool isCurrentSpecificationWordSpanishAmbiguous;
		bool isExclusiveUserSpecification;
		bool isOlderCurrentSpecification;
		bool isPossessiveUserSpecification = false;
		bool isSameSpecification;
		bool isUserAssignment = false;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned short generalizationWordTypeNr;
		unsigned short specificationWordTypeNr;
		unsigned short relationWordTypeNr;
		unsigned int currentRelationCollectionNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int tempRelationCollectionNr;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		WordItem *currentCollectionWordItem;
		WordItem *currentRelationWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSelectedReversibleConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) ) != NULL &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			specificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();

			hasUserGeneralizationWordCurrentlyConfirmedSpecification = userGeneralizationWordItem->hasCurrentlyConfirmedSpecification();
			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();
			isUserAssignment = userSpecificationItem->isAssignment();
			isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			if( isPossessiveUserSpecification )
				{
				if( isUserGeneralizationWord &&
				( currentRelationWordItem = currentSpecificationItem->relationWordItem() ) != NULL &&
				currentRelationWordItem->isUserRelationWord )
					{
					// Draw user reversible conclusion about relation word
					if( drawUserReversibleConclusions( isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem, userSpecificationWordItem, currentRelationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a user reversible conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and relation word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

					if( ( currentRelationCollectionNr = currentSpecificationItem->relationCollectionNr() ) > NO_COLLECTION_NR &&
					( currentCollectionWordItem = adminItem_->firstCollectionWordItem( currentRelationCollectionNr ) ) != NULL )
						{
						// Do for all collection words
						do	{
							if( currentCollectionWordItem != currentRelationWordItem &&
							currentCollectionWordItem->isUserRelationWord &&
							// Draw user reversible conclusion about collection relation word
							drawUserReversibleConclusions( isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem, userSpecificationWordItem, currentCollectionWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a user reversible conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and collection relation word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );
							}
						while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( currentRelationCollectionNr ) ) != NULL );
						}
					}
				}
			else	// Non-possessive user specification
				{
				if( !isUserGeneralizationWord &&
				hasUserSpecificationRelationWord &&
				( existingSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, userSpecificationWordItem, userGeneralizationWordItem ) ) != NULL &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&

				( existingSpecificationItem->isFirstJustificationReversibleAssumption() ||
				existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL ||
				// Typical for Spanish
				// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
				!existingSpecificationItem->hasSpecificationCollection() ) &&

				// Draw reversible user conclusion
				adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );
				}

			// Do for all selected specifications
			do	{
				hasCurrentSpecificationCreationSentenceNr = currentSpecificationItem->hasCurrentCreationSentenceNr();
				hasNonCompoundCurrentSpecificationCollection = currentSpecificationItem->hasNonCompoundSpecificationCollection();
				isCurrentSpecificationSelfGeneratedAssumption = currentSpecificationItem->isSelfGeneratedAssumption();
				isCurrentSpecificationSelfGeneratedSpecification = currentSpecificationItem->isSelfGeneratedSpecification();
				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem->isSpecificationWordSpanishAmbiguous();
				isOlderCurrentSpecification = currentSpecificationItem->isOlderItem();
				generalizationWordTypeNr = currentSpecificationItem->generalizationWordTypeNr();
				specificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();
				relationWordTypeNr = currentSpecificationItem->relationWordTypeNr();
				currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
				currentRelationCollectionNr = currentSpecificationItem->relationCollectionNr();
				currentRelationWordItem = currentSpecificationItem->relationWordItem();

				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The current specification item has no specification word" );

				if( currentSpecificationItem->isPossessive() )
					{
					if( isCurrentSpecificationSelfGeneratedSpecification )
						{
						if( isUserGeneralizationWord ||
						currentSpecificationCollectionNr == NO_COLLECTION_NR )
							{
							if( !isSpanishCurrentLanguage &&
							currentRelationCollectionNr > NO_COLLECTION_NR &&

							( !isOlderCurrentSpecification ||
							hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation ) &&

							( currentCollectionWordItem = adminItem_->firstCollectionWordItem( currentRelationCollectionNr ) ) != NULL )
								{
								// Do for all collection words
								do	{
									if( ( existingSpecificationItem = currentCollectionWordItem->bestMatchingRelationSpecificationItem( false, false, false, false, currentSpecificationWordItem, generalizationWordItem ) ) != NULL &&
									existingSpecificationItem->hasCurrentCreationSentenceNr() &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimarySpecificationJustification( currentSpecificationItem ) )
									// Forcing assumption to be less certain. So, no checking on assumption level
									// Draw reversible conclusions in current relation collection word
										{
										if( isOlderCurrentSpecification )
											{
											// Test file: "reasoning\family\You have confirmed at least one relation word"
											if( currentCollectionWordItem->isUserRelationWord &&
											( obsoleteJustificationItem = currentSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentCollectionWordItem ) ) != NULL )
												{
												// Create justification
												if( ( justificationResult = generalizationWordItem->createJustificationItem( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, userSpecificationItem, NULL, NULL, NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a reversible assumption justification item to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find or create a reversible assumption justification" );

												if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete reversible assumption justification of the current secondary specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											{
											// Test files: "reasoning\family\Complex (18)",
											//				"reasoning\family\Complex (19 - mixed)",
											//				"reasoning\family\Complex (19 - strange)"
											// Add reversible conclusion in current relation collection word
											if( existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentSpecificationItem, NULL, NULL ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
											}
										}
									}
								while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( currentRelationCollectionNr ) ) != NULL );
								}
							}
						else
							{
							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							hasNonCompoundCurrentSpecificationCollection )
								{
								if( isOlderCurrentSpecification &&
								isUserAssignment &&
								// Justification doesn't exist yet in specification
								!currentSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
								// Draw Spanish non-compound reversible conclusion
								adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, relationWordTypeNr, currentSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible specification from word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

								if( !isPossessiveUserSpecification &&
								isUserSpecificationWordSpanishAmbiguous &&
								// Specification doesn't exist yet with user specification collection
								generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, (unsigned int)userSpecificationCollectionNr, userSpecificationWordItem ) == NULL )
									{
									// Draw Spanish reversible conclusion
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, isArchivedUserAssignment, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, userSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );

									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
									// Specification doesn't exist yet with without specification collection
									( obsoleteSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, (unsigned int)NO_COLLECTION_NR, userSpecificationWordItem ) ) != NULL &&
									// Replace specification without specification collection with specification with specification
									// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
									generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a specification without specification collection with a specification with specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					else	// User specification
						{
						if( !hasNonCompoundCurrentSpecificationCollection &&
						!isOlderCurrentSpecification &&
						currentRelationCollectionNr == NO_COLLECTION_NR &&
						currentRelationWordItem != NULL &&
						currentRelationWordItem->hasNegativeSpecification() &&

							// Possessive specification
							( ( existingSpecificationItem = currentRelationWordItem->firstPossessiveSpecificationItem() ) == NULL ||
							existingSpecificationItem->isOlderItem() ) &&

							// Exclusive specification, including adjectives
							( ( existingSpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem( true ) ) == NULL ||
							existingSpecificationItem->isOlderItem() ) &&

						// Draw specification substitution conclusion or ask a question
						drawSpecificationSubstitutionConclusionOrAskQuestion( false, false, isSpanishCurrentLanguage, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, currentRelationWordItem, currentSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else	// Non-possessive
					{
					if( hasUserSpecificationRelationWord &&
					isCurrentSpecificationSelfGeneratedSpecification &&
					// Secondary specification word is same as user specification word
					currentSpecificationWordItem == userSpecificationWordItem )
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage )
							{
							if( hasUserSpecificationCompoundCollection &&
							// Spanish test file: "razonamiento\familia\Juan - Luz - Ana"
							!isUserAssignment &&

								( isUserGeneralizationWord ||
								hasNonCompoundCurrentSpecificationCollection ) &&

							// Justification doesn't exist yet in specification
							!currentSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Add Spanish reversible user justification
							currentSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible user justification to the current secondary specification" );
							}
						else
							{
							if( isPossessiveUserSpecification &&
							isCurrentSpecificationSelfGeneratedAssumption )
								{
								if( ( hasUserGeneralizationWordCurrentlyConfirmedSpecification ||
								!currentSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) &&

								// Conclusion doesn't exist yet
								generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, userGeneralizationWordItem ) == NULL )
									{
									conclusionSpecificationItem = ( !hasNonCompoundCurrentSpecificationCollection ||
																	!hasUserGeneralizationWordCurrentlyConfirmedSpecification ? NULL :
																		generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, NULL ) );

									// Draw reversible user conclusion
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

									if( conclusionSpecificationItem == NULL )
										{
										if( createAndAssignResult.foundSpecificationItem != NULL &&
										( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
										// Add reversible justification to current secondary specification
										currentSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the current secondary specification" );
										}
									else
										{
										conclusionSpecificationItem = conclusionSpecificationItem->updatedSpecificationItem();

										// Test files: "reasoning\family\My assumptions that are confirmed (parents)",
										//				"reasoning\family\Paul - Joe - Laura - John and Anna (son - daughter)"
										// Confirmed all relation words
										if( conclusionSpecificationItem->nRelationWords() == currentSpecificationItem->nRelationWords() &&
										// Replace assumption by conclusion
										generalizationWordItem->replaceOrDeleteSpecification( currentSpecificationItem, conclusionSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
										}
									}
								}
							else
								{
								// Test file: "reasoning\family\Complex (17)"
								if( isOlderCurrentSpecification &&
								( obsoleteSpecificationItem = generalizationWordItem->noRelationWordSpecificationItem( false, true, currentSpecificationWordItem ) ) != NULL &&
								// Replace obsolete assumption specification by conclusion
								generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, currentSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete assumption specification by a conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}

					// Typical for Spanish
					if( isCurrentSpecificationWordSpanishAmbiguous )
						currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

					if( isDrawingPossessiveReversibleConclusions &&
					currentRelationWordItem != NULL &&

						// No current specification creation sentence number
						( ( !hasCurrentSpecificationCreationSentenceNr &&
						!generalizationWordItem->hasJustification( currentSpecificationItem, NULL, NULL ) ) ||

						// Current specification creation sentence number
						( hasCurrentSpecificationCreationSentenceNr &&

							( currentSpecificationCollectionNr > NO_COLLECTION_NR ||
							// Test file: "Unique relation"
							userSpecificationCollectionNr == currentSpecificationCollectionNr ||
							// Test file: "reasoning\family\John - Anna (before family definition)"
							currentSpecificationItem->hasOnlyOneRelationWord() ) ) ) )
						{
						// Not older
						if( ( ( !isOlderCurrentSpecification &&

							( ( existingSpecificationItem = currentRelationWordItem->bestMatchingRelationSpecificationItem( true, true, false, true, currentSpecificationWordItem, generalizationWordItem ) ) == NULL ||

							// User specification
							( hasNonCompoundCurrentSpecificationCollection &&
							!existingSpecificationItem->isOlderItem() ) ||

							// Self-generated specification
							( existingSpecificationItem->isSelfGeneratedSpecification() &&

								( isUserGeneralizationWord ||
								existingSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ) ) ||

						// Older
						( isOlderCurrentSpecification &&
						isCurrentSpecificationSelfGeneratedSpecification &&
						!isUserGeneralizationWord &&
						currentSpecificationWordItem == userSpecificationWordItem ) ||

						// Typical for Spanish
						( isSpanishCurrentLanguage &&

							( isCurrentSpecificationSelfGeneratedSpecification ||

							// Spanish test file: "razonamiento\Reto científico"
							( isOlderCurrentSpecification &&
							currentSpecificationCollectionNr == NO_COLLECTION_NR ) ) ) ) &&

						// Draw possessive reversible conclusions about relation word
						drawPossessiveReversibleConclusions( isUserAssignment, isSpanishCurrentLanguage, currentSpecificationItem, generalizationWordItem, currentSpecificationWordItem, currentRelationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\" in relation word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

						isSameSpecification = ( currentSpecificationItem == userSpecificationItem );

						if( isSameSpecification &&
						currentRelationCollectionNr == NO_COLLECTION_NR &&
						!hasUserGeneralizationWordCurrentlyConfirmedSpecification &&
						!hasUserSpecificationCompoundCollection &&
						( tempRelationCollectionNr = currentRelationWordItem->collectionNr( currentSpecificationWordItem ) ) > NO_COLLECTION_NR )
							// Test file: "reasoning\family\Complex (13)"
							currentRelationCollectionNr = tempRelationCollectionNr;

						if( currentRelationCollectionNr > NO_COLLECTION_NR &&
						( currentCollectionWordItem = adminItem_->firstCollectionWordItem( currentRelationCollectionNr ) ) != NULL )
							{
							// Do for all collection words
							do	{
								if( currentCollectionWordItem != currentRelationWordItem &&

									// Different specifications
									( ( !isSameSpecification &&

										( ( isCurrentSpecificationSelfGeneratedSpecification &&
										( firstJustificationItem = currentSpecificationItem->firstJustificationItem() ) != NULL &&

											( !isOlderCurrentSpecification ||

											( firstJustificationItem->isOppositePossessiveSpecificationAssumption() &&
											currentSpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() ) ||

											currentSpecificationItem->hasRelationCollectionCurrentlyBeenUpdated() ) ) ||

										// Typical for Spanish
										( isSpanishCurrentLanguage &&

											// User specification word not Spanish ambiguous
											// Spanish test file: "razonamiento\familia\Complejo (14)",
											//						"razonamiento\familia\Mis suposiciones que se confirman (Juan y Ana)"
											( ( !isUserSpecificationWordSpanishAmbiguous &&
											isCurrentSpecificationSelfGeneratedSpecification ) ||

											// User specification word Spanish ambiguous
											// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
											( isUserSpecificationWordSpanishAmbiguous &&
											isCurrentSpecificationWordSpanishAmbiguous ) ) ) ) ) ||

									// Same specification
									( isSameSpecification &&

										( currentCollectionWordItem->isUserRelationWord ||

										// Test file: "Acceptance - Boston - United States - United Kingdom"
										( isExclusiveUserSpecification &&
										currentCollectionWordItem->firstSpecificationItem( false, true, false, currentSpecificationWordItem ) == NULL ) ) ) ) &&

								// Draw possessive reversible conclusions about collection relation word
								drawPossessiveReversibleConclusions( isUserAssignment, isSpanishCurrentLanguage, currentSpecificationItem, generalizationWordItem, currentSpecificationWordItem, currentCollectionWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\" in collection relation word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );
								}
							while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( currentRelationCollectionNr ) ) != NULL );
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem->isReplacedOrDeletedItem() ?
												// First specification
												generalizationWordItem->firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) :
												// Next specification
												currentSpecificationItem->nextNonQuestionNonRelationSpecificationItem() ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawUserReversibleConclusions( bool isSpanishCurrentLanguage, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasNonCompoundUserSpecificationCollection = false;
		bool isArchivedAssignment;
		bool isAssignment;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *assignmentSpecificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *assumptionSpecificationWordItem;
		WordItem *commonWordItem;
		WordItem *spanishFeminineSpecificationWordItem;
		WordItem *spanishMasculineSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawUserReversibleConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		// Specification doesn't exist yet
		if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( true, true, false, false, specificationWordItem, generalizationWordItem ) ) == NULL ||

		( existingSpecificationItem->isSelfGeneratedSpecification() &&
		// Justification doesn't exist yet in specification
		!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) )
			{
			hasNonCompoundUserSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			conclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, specificationWordItem, NULL );

			if( ( !hasNonCompoundUserSpecificationCollection &&
			existingSpecificationItem == NULL ) ||

			( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
			// Self-generated conclusion with different relation word
			conclusionSpecificationItem != NULL ) ||

			( existingSpecificationItem != NULL &&
			// Conclusion doesn't exist yet
			conclusionSpecificationItem == NULL ) ||

			( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
			generalizationWordItem->firstActiveQuestionSpecificationItem() != NULL ) &&

			// Assumption doesn't exist yet
			relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, NULL ) == NULL ) ||

			// Typical for Spanish
			isSpanishCurrentLanguage )
				{
				isAssignment = false;
				isArchivedAssignment = false;
				assignmentSpecificationItem = NULL;
				spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
				userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

				// Typical for Spanish
				if( isSpanishCurrentLanguage &&
				hasNonCompoundUserSpecificationCollection &&

					( !relationWordItem->hasNegativeSpecification() ||
					relationWordItem->hasFeminineProperNounEnding() ) )
					spanishAmbiguousSpecificationCollectionNr = userSpecificationCollectionNr;

				// Similar specification already exists as assignment
				// So, make this conclusion an assignment too
				if( existingSpecificationItem == NULL &&
				( assignmentSpecificationItem = relationWordItem->firstAssignmentItem( false, false, specificationWordItem ) ) != NULL )
					{
					isAssignment = true;
					isArchivedAssignment = assignmentSpecificationItem->isArchivedAssignment();
					}

				// Draw reversible user conclusion
				if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
					{
					// No existing specification
					if( existingSpecificationItem == NULL )
						{							// Assignment specification
						foundSpecificationItem = ( assignmentSpecificationItem != NULL ? assignmentSpecificationItem :
													// Found specification
													( createAndAssignResult.foundSpecificationItem != NULL ? createAndAssignResult.foundSpecificationItem :
													// User specification
													relationWordItem->firstUserSpecificationItem( false, false, createdSpecificationItem->specificationCollectionNr(), specificationWordItem ) ) );

						// No found specification
						if( foundSpecificationItem == NULL )
							{
							if( hasNonCompoundUserSpecificationCollection &&
							!isSpanishCurrentLanguage )
								{
								// Test file: "reasoning\family\Complex (3)"
								// Draw only option left conclusion
								if( drawOnlyOptionLeftProperNounConclusion( createdSpecificationItem, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only-option-left proper noun conclusion" );


								if( !createdSpecificationItem->isHiddenSpanishSpecification() &&
								( commonWordItem = specificationWordItem->commonWordItem( userSpecificationCollectionNr ) ) != NULL &&
								( assumptionSpecificationWordItem = specificationWordItem->collectionWordItem( userSpecificationCollectionNr ) ) != NULL &&
								( definitionSpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&
								( assumptionSpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, assumptionSpecificationWordItem ) ) != NULL &&
								// Justification doesn't exist yet in specification
								!assumptionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, createdSpecificationItem ) &&
								// Add opposite possessive assumption justification to assumption specification
								assumptionSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, createdSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive assumption justification to an assumption specification" );
								}
							}
						else	// Found specification
							{
							if( ( isAssignment ||
							!userSpecificationItem->hasSpecificationCollection() ) &&

							// Add reversible user justification to created specification
							createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the created specification" );
							}
						}
					else	// Existing specification
						{
						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
							{
							if( foundSpecificationItem->hasCompoundSpecificationCollection() )
								{
								// Replace an assumption specification by a conclusion specification in justifications of involved word
								if( relationWordItem->updateSpecificationOfJustificationsInWord( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in a justification in word \"", relationWordItem->anyWordTypeString(), "\"" );

								// Add reversible user justification to created specification
								if( foundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the created specification" );

								// Typical for Spanish
								// Spanish test file: "razonamiento\familia\Complejo (15)"
								if( isSpanishCurrentLanguage &&
								( spanishMasculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem() ) != NULL &&
								( assumptionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem ) ) != NULL &&
								!assumptionSpecificationItem->isHiddenSpanishSpecification() &&
								// Add Spanish reversible justification to assumption specification
								assumptionSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible justification to the assumption specification" );
								}
							else
								{
								if( ( obsoleteJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, generalizationWordItem ) ) != NULL )
									{
									// Test file: "reasoning\family\You have confirmed at least one relation word"
									// Create justification
									if( ( justificationResult = relationWordItem->createJustificationItem( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, userSpecificationItem, NULL, NULL, NULL, NULL ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a justification item to word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

									// Replace negative assumption justification
									if( relationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption justification in word \"", relationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					}

				// Typical for Spanish
				if( userSpecificationItem->isSpecificationWordSpanishAmbiguous() )
					{
					if( relationWordItem->hasFeminineProperNounEnding() &&
					( spanishFeminineSpecificationWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL &&
					( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, spanishFeminineSpecificationWordItem ) ) != NULL &&
					relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, NO_QUESTION_PARAMETER, definitionSpecificationItem->specificationCollectionNr(), NO_COLLECTION_NR, spanishFeminineSpecificationWordItem, NULL ) == NULL &&
					// Draw Spanish feminine exclusive specification substitution assumption
					adminItem_->addSelfGeneratedSpecification( true, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, relationWordItem, spanishFeminineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish exclusive specification substitution assumption about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", spanishFeminineSpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return RESULT_OK;
		}

	FindSpecificationResultType findPossessiveSpecifications( WordItem *possessiveNounWordItem, WordItem *specificationWordItem )
		{
		bool hasFoundPossessiveSpecificationWord = true;
		unsigned int possessiveSpecificationCollectionNr;
		SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *foundPossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *previousPossessiveSpecificationItem = NULL;
		WordItem *higherLevelPossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findPossessiveSpecifications";

		if( possessiveNounWordItem == NULL )
			return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The given possessive noun word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// First try to find a possessive specification on a higher level
		if( ( possessiveDefinitionSpecificationItem = possessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
			{
			do	{
				if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
				possessiveSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) != NULL )
					{
					if( higherLevelPossessiveDefinitionSpecificationWordItem == NULL )
						higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
					else
						hasFoundPossessiveSpecificationWord = false;
					}
				}
			while( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
				{
				if( higherLevelPossessiveDefinitionSpecificationWordItem == NULL )
					higherLevelPossessiveDefinitionSpecificationWordItem = specificationWordItem;

				// Try to find the opposite of the possessive specification
				do	{
					if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( previousPossessiveSpecificationItem != NULL )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem->specificationCollectionNr();

							if( higherLevelPossessiveDefinitionSpecificationWordItem->hasCollectionNr( possessiveSpecificationCollectionNr ) )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem :
																																												possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem :
																																									previousPossessiveSpecificationItem );
							}

						previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
						}
					}
				while( foundPossessiveDefinitionSpecificationItem == NULL &&
				( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );
				}
			}

		findSpecificationResult.foundPossessiveDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
		findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;

		return findSpecificationResult;
		}

	signed char findQuestionToBeAdjustedByCompoundCollection( SpecificationItem *definitionSpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		unsigned int definitionSpecificationCollectionNr;
		SpecificationItem *adjustedQuestionCompoundSpecificationItem;
		SpecificationItem *lastAdjustedQuestionCompoundSpecificationItem = NULL;
		SpecificationItem *replacingSpecificationItem = NULL;
		WordItem *commonWordItem;
		WordItem *currentCollectionWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( definitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given definition specification item is undefined" );

		if( ( definitionSpecificationCollectionNr = definitionSpecificationItem->specificationCollectionNr() ) <= NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given definition specification item has no specification collection" );

		if( ( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
			{
			hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem->hasCurrentlyMoreSpecificSpecification();

			// Do for all collection words
			do	{
				if( ( commonWordItem = currentCollectionWordItem->commonWordItem( definitionSpecificationCollectionNr ) ) != NULL &&
				( adjustedQuestionCompoundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, currentCollectionWordItem ) ) != NULL )
					{
					if( commonWordItem == specificationWordItem )
						{
						// Skip updating question if the old justification has no relation word,
						// while the current user specification has a relation word.
						// Test file: "reasoning\family\This information is more specific"
						if( !hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
						// Justification doesn't exist yet in specification
						!adjustedQuestionCompoundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) )
							{
							// Create adjusted question
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, adjustedQuestionCompoundSpecificationItem->isInactiveAssignment(), adjustedQuestionCompoundSpecificationItem->isArchivedAssignment(), false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, adjustedQuestionCompoundSpecificationItem->generalizationWordTypeNr(), adjustedQuestionCompoundSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, currentCollectionWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create an adjusted question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );

							replacingSpecificationItem = createAndAssignResult.foundSpecificationItem;
							}
						}
					else
						{
						if( generalizationWordItem->replaceOrDeleteSpecification( adjustedQuestionCompoundSpecificationItem, ( replacingSpecificationItem == NULL || replacingSpecificationItem->isReplacedOrDeletedItem() ? NULL : replacingSpecificationItem ) ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a question part" );

						lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem->updatedSpecificationItem();
						}
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != NULL &&
		// Write last instance of adjusted compound question
		generalizationWordItem->writeUpdatedSpecification( true, false, false, true, false, false, false, lastAdjustedQuestionCompoundSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted compound question" );

		return RESULT_OK;
		}

	signed char makeOppositePossessiveSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isSpanishCurrentLanguage, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *alternativeSecondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasExistingSpecificationCurrentCreationSentenceNr = false;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		bool hasRelationWordConfirmedSpecification;
		bool hasRelationWordMasculineProperNounEnding = false;
		bool hasSameSpecificationCollection;
		bool isAlternativeSecondaryUserSpecification = false;
		bool isExistingUserSpecification = false;
		bool isOlderCurrentExistingSpecification = false;
		bool isOlderExistingSpecification = false;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isRelationWordUserGeneralizationWord;
		unsigned short createAssumptionLevel;
		unsigned short currentExistingAssumptionLevel = NO_ASSUMPTION_LEVEL;
		unsigned short selectedJustificationTypeNr;
		unsigned int hiddenSpanishSpecificationCollectionNr;
		unsigned int relationCollectionNr;
		SpecificationItem *additionalDefinitionSpecificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentExistingSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *feminineSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selectedSpecificationItem = NULL;
		WordItem *alternativeSecondarySpecificationWordItem;
		WordItem *currentCollectionWordItem;
		WordItem *feminineSpecificationWordItem = NULL;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeOppositePossessiveSpecificationAssumption";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( oppositePossessiveDefinitionSpecificationItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item is undefined" );

		if( possessiveDefinitionSpecificationItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The possessive definition specification item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item has no specification word" );

		if( ( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The The possessive definition specification item has no specification word item" );

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
		hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem->hasCurrentlyMoreSpecificSpecification();
		hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
		isRelationWordUserGeneralizationWord = relationWordItem->isUserGeneralizationWord;

		if( oppositePossessiveDefinitionSpecificationItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item is undefined" );

		if( possessiveDefinitionSpecificationItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The possessive definition specification item is undefined" );

		if( ( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The The possessive definition specification item has no specification word item" );

		if( ( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item has no specification word" );

		isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

		if( !isPossessiveSpecificationWordSpanishAmbiguous &&
		secondarySpecificationItem->hasCompoundSpecificationCollection() )
			return adminItem_->startError( functionNameString, moduleNameString_, "Some preconditions are not met" );

		if( alternativeSecondarySpecificationItem != NULL )
			isAlternativeSecondaryUserSpecification = alternativeSecondarySpecificationItem->isUserSpecification();

		if( isPossessive )
			{
			selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;

			selectedPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
			selectedSecondarySpecificationItem = secondarySpecificationItem;
			}
		else
			{
			selectedJustificationTypeNr = JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION;

			selectedPrimarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																								secondarySpecificationItem );
			selectedSecondarySpecificationItem = NULL;
			}

		if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( true, false, !isPossessive, possessiveDefinitionSpecificationWordItem ) ) != NULL )
			{
			hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem->hasCurrentCreationSentenceNr();
			isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
			isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
			}

		// User specification doesn't exist
		if( ( ( existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, possessiveDefinitionSpecificationWordItem ) ) == NULL &&
		alternativeSecondarySpecificationItem != NULL &&
		!possessiveDefinitionSpecificationWordItem->isUserSpecificationWord &&

			( ( isPossessive &&
			!isOlderExistingSpecification &&
			!isPossessiveSpecificationWordSpanishAmbiguous &&
			isRelationWordUserGeneralizationWord &&
			existingSpecificationItem != NULL &&
			// Justification doesn't exist yet in specification
			!existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
			// Justification has at least the same assumption level
			existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) ) ||

		// Typical for Spanish
		( isPossessiveSpecificationWordSpanishAmbiguous &&
		alternativeSecondarySpecificationItem != NULL &&

			// User specification doesn't exist yet
			( ( existingUserSpecificationItem == NULL &&

				// No alternative
				( ( !isAlternativeSecondaryUserSpecification &&
				existingSpecificationItem == NULL ) ||

				// Alternative
				( isAlternativeSecondaryUserSpecification &&

				( !generalizationWordItem->hasActiveQuestionWithCompoundCollection() ||
				// Spanish test file: "Saltarse las preguntas no válidos (2)"
				alternativeSecondarySpecificationItem->isActiveAssignment() ) &&

					( existingSpecificationItem == NULL ||

					( ( !existingSpecificationItem->hasNonCompoundSpecificationCollection() ||

					// Justification doesn't exist yet in specification
					( !existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) &&

					// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
					( ( hasExistingSpecificationCurrentCreationSentenceNr ||
					!alternativeSecondarySpecificationItem->isOlderItem() ) &&

						// Don't include following condition line
						// Justification doesn't exist yet in specification
//						!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) ) ) ) ) ||

				// User specification exists
				// Typical for Spanish
				// Spanish test file: "razonamiento\familia\Complejo (4)"
				( existingUserSpecificationItem != NULL &&
				isRelationWordUserGeneralizationWord &&
				existingSpecificationItem != NULL &&
				existingUserSpecificationItem->isOlderItem() &&
				existingUserSpecificationItem->hasCompoundSpecificationCollection() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) ) )
			{
			// Make an opposite possessive specification assumption (normal)
			if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, false, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
			}
		else
			{
			if( isPossessiveSpecificationWordSpanishAmbiguous &&
			// Draw Spanish proper noun part-of conclusions in proper noun words
			drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, false, true, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
			}

		if( existingSpecificationItem != NULL &&
		alternativeSecondarySpecificationItem != NULL &&
		alternativeSecondarySpecificationItem->isPossessive() &&
		!existingSpecificationItem->hasCurrentCreationSentenceNr() &&
		existingSpecificationItem->hasNonCompoundSpecificationCollection() &&

			( userSpecificationItem->isPossessive() ||
			userSpecificationItem->hasCompoundSpecificationCollection() ) &&

		// Avoid idle call
		possessiveDefinitionSpecificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() != NULL &&
		// Draw specification substitution conclusion or ask a question
		drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", relationWordItem->anyWordTypeString(), "\"" );

		if( alternativeSecondarySpecificationItem != NULL )
			{
			if( ( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem->specificationWordItem() ) != NULL &&
			// Avoid idle call
			alternativeSecondarySpecificationWordItem->firstNegativeSpecificationItem() != NULL &&
			( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, alternativeSecondarySpecificationWordItem ) ) != NULL &&
			!foundSpecificationItem->isHiddenSpanishSpecification() &&
			// Draw simple negative conclusions
			drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, foundSpecificationItem, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Try to find user specification or self-generated conclusion first
			if( ( selectedSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem, NULL ) ) != NULL &&
			// Typical for Spanish
			isSpanishCurrentLanguage &&
			isAlternativeSecondaryUserSpecification &&
			selectedSpecificationItem->isHiddenSpanishSpecification() )
				selectedSpecificationItem = NULL;

			// If not found, try to find a self-generated assumption
			if( selectedSpecificationItem == NULL &&
			( assumptionSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL )
				{
				hasSameSpecificationCollection = ( assumptionSpecificationItem->specificationCollectionNr() == alternativeSecondarySpecificationItem->specificationCollectionNr() );

				if( !assumptionSpecificationItem->hasCurrentCreationSentenceNr() &&

				( hasGeneralizationWordCurrentlyConfirmedSpecification == hasSameSpecificationCollection ||
				alternativeSecondarySpecificationItem->isPossessive() ) )
					selectedSpecificationItem = assumptionSpecificationItem;
				}
			}

		isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();
		feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationWordItem->feminineCollectionWordItem() :
																									oppositePossessiveDefinitionSpecificationWordItem->feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem->hasFeminineProperNounEnding() : relationWordItem->hasFeminineProperNounEnding() ) );

		if( feminineSpecificationWordItem != NULL )
			{
			if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
				additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
			else
				feminineSpecificationWordItem = NULL;
			}

		if( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
		// Typical for Spanish
		isRelationWordUserGeneralizationWord ||
		additionalDefinitionSpecificationItem == NULL ||
		generalizationWordItem->firstUserSpecificationItem( false, isPossessive, additionalDefinitionSpecificationItem->specificationCollectionNr(), NULL ) != NULL ) &&

		( selectedSpecificationItem == NULL ||

		// Relation word is not user generalization word
		( !isRelationWordUserGeneralizationWord &&
		!hasGeneralizationWordCurrentlyConfirmedSpecification &&
		!hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
		existingSpecificationItem != NULL &&
		existingSpecificationItem->isSelfGeneratedSpecification() &&

			// Test files: "reasoning\family\Complex (16)",
			//				"reasoning\family\Complex (17)",
			//				"reasoning\family\I know (1)"
			( !selectedSpecificationItem->hasRelationWord() ||

			( selectedSpecificationItem->isSelfGeneratedSpecification() &&
			relationWordItem->bestMatchingRelationSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) == NULL ) ) ) ||

		// Relation word is user generalization word
		( isRelationWordUserGeneralizationWord &&
		// Typical for Spanish
		isOppositePossessiveSpecificationWordSpanishAmbiguous &&

			( !selectedSpecificationItem->hasRelationWord() ||

			( selectedSpecificationItem->hasOnlyOneRelationWord() &&
			!selectedSpecificationItem->isHiddenSpanishSpecification() ) ||

			( selectedSpecificationItem->isSelfGeneratedSpecification() &&
			relationWordItem->bestMatchingRelationSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) != NULL ) ) ) ||

		( ( !hasGeneralizationWordCurrentlyMoreSpecificSpecification ||
		additionalDefinitionSpecificationItem == NULL ||
		selectedSpecificationItem->isSelfGeneratedConclusionOrAssumption() ) &&

			// Typical for Spanish
			// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
			( ( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
			hasGeneralizationWordCurrentlyMoreSpecificSpecification ) ||

			// Try to find user specification or self-generated conclusion first
			relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, true, oppositePossessiveDefinitionSpecificationWordItem, NULL ) != NULL ) ) ) )
			{
			if( ( currentExistingSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( true, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL )
				{
				isOlderCurrentExistingSpecification = currentExistingSpecificationItem->isOlderItem();
				currentExistingAssumptionLevel = currentExistingSpecificationItem->assumptionLevel();
				}

			selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
			selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem_;
			selectedAdditionalDefinitionSpecificationItem = NULL;
			selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																											secondarySpecificationItem );

			// Typical for Spanish
			// Spanish test file: "razonamiento\Reto científico"
			if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
			!isPossessive &&
			additionalDefinitionSpecificationItem == NULL &&
			relationWordItem->hasMasculineProperNounEnding() &&
			( foundSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
				{
				hasRelationWordMasculineProperNounEnding = true;
				selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
				selectedPrimarySpecificationItem = selectedSecondarySpecificationItem;
				selectedAdditionalDefinitionSpecificationItem = possessiveDefinitionSpecificationItem_;
				selectedSecondarySpecificationItem = foundSpecificationItem;
				}

			if( selectedSecondarySpecificationItem != NULL )
				{
				createAssumptionLevel = adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL );

				// Non-possessive
				if( ( !isPossessive &&

					( ( !hasRelationWordConfirmedSpecification &&
					!isOlderCurrentExistingSpecification ) ||

					// Typical for Spanish
					// Spanish test file: "razonamiento\familia\Complejo (3)"
					( currentExistingSpecificationItem == NULL ||

					// Justification doesn't exist yet in specification
					( !currentExistingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&

						// No confirmed relation word
						( ( !hasRelationWordConfirmedSpecification &&

							( ( !isExistingUserSpecification &&
							currentExistingAssumptionLevel >= createAssumptionLevel ) ||

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (18)"
							( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							currentExistingAssumptionLevel == createAssumptionLevel ) ) ) ||

						// Confirmed relation word
						( hasRelationWordConfirmedSpecification &&
						!selectedSecondarySpecificationItem->hasRelationCollection() &&

							// Test file: "reasoning\family\Complex (16)"
							( !selectedSecondarySpecificationItem->isOlderItem() ||
							// Test file: "reasoning\family\Joe is a son - Joe has 2 parents"
							currentExistingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL ) ) ) ) ||

					// Typical for Spanish
					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!hasRelationWordConfirmedSpecification &&
					currentExistingAssumptionLevel == createAssumptionLevel &&
					currentExistingSpecificationItem->isHiddenSpanishSpecification() ) ) ) ) ||

				// Possessive
				( isPossessive &&
				existingSpecificationItem != NULL &&

					( alternativeSecondarySpecificationItem == NULL ||

					// Typical for Spanish
					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

						( currentExistingSpecificationItem == NULL ||
						currentExistingAssumptionLevel >= createAssumptionLevel ||

						( currentExistingSpecificationItem->hasCurrentCreationSentenceNr() &&

							( existingSpecificationItem->isSelfGeneratedSpecification() ||

							( isOlderCurrentExistingSpecification &&
							!alternativeSecondarySpecificationItem->isOlderItem() ) ) ) ) ) ) ) )
					{
					// Make an assumption
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an assumption from generalization word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

					createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
					foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

					if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					existingSpecificationItem != NULL &&
					( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
					// Justification doesn't exist yet in specification
					!createdOrFoundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem_, existingSpecificationItem ) &&
					// Justification has at least the same assumption level
					createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem_, NULL, existingSpecificationItem, NULL ) &&
					// Add opposite possessive conclusion justification to found specification
					createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem_, NULL, existingSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish opposite possessive conclusion justification to the found specification" );

					if( isPossessive )
						{
						// Typical for Spanish
						if( hasRelationWordConfirmedSpecification &&
						foundSpecificationItem != NULL &&
						( relationCollectionNr = foundSpecificationItem->relationCollectionNr() ) > NO_COLLECTION_NR &&
						( currentCollectionWordItem = adminItem_->firstCollectionWordItem( relationCollectionNr ) ) != NULL )
							{
							// Do for all collection words
							do	{
								if( ( selectedSecondarySpecificationItem = currentCollectionWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, secondarySpecificationWordItem ) ) != NULL &&
								// Justification doesn't exist yet in specification
								!foundSpecificationItem->hasSecondarySpecificationJustification( selectedSecondarySpecificationItem ) &&
								// Justification has at least the same assumption level
								foundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
								// Add Spanish opposite possessive conclusion justification to found specification
								foundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish opposite possessive conclusion justification to the found specification" );
								}
							while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr ) ) != NULL );
							}
						}
					else
						{
						if( createdSpecificationItem != NULL )
							{
							// Created, but not found
							if( foundSpecificationItem == NULL )
								{
								// Typical for Spanish
								// Spanish test file: "razonamiento\Reto científico"
								if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
								isExistingUserSpecification &&
								additionalDefinitionSpecificationItem == NULL &&
								( hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_->specificationCollectionNr() ) > NO_COLLECTION_NR &&
								relationWordItem->hasFeminineProperNounEnding() &&
								( feminineSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstNegativeSpecificationItem() ) != NULL &&
								( feminineSpecificationWordItem = feminineSpecificationItem->specificationWordItem() ) != NULL )
									{
									// Make feminine assumption
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine assumption from generalization word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );

									if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
									// Specification doesn't exist yet
									( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL, NULL ) &&
									// Add reversible justification to existing specification
									existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
									}
								}
							else	// Created and found
								{
								// Justification doesn't exist yet in specification
								if( !createdSpecificationItem->hasPrimarySpecificationJustification( foundSpecificationItem ) &&
								// Justification has at least the same assumption level
								createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL, NULL ) &&
								// Add reversible justification
								createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the found specification" );
								}
							}
						}
					}
				}

			if( alternativeSecondarySpecificationItem != NULL &&
			feminineSpecificationWordItem != NULL &&
			possessiveDefinitionSpecificationWordItem == secondarySpecificationWordItem &&

				// Test files: "reasoning\family\Complex (18)",
				//				"reasoning\family\Complex (19 - mixed)",
				//				"reasoning\family\Complex (19 - strange)"
				( isPossessive ||
				!secondarySpecificationItem->isSelfGeneratedAssumption() ||
				secondarySpecificationItem != alternativeSecondarySpecificationItem ) &&

			( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem->specificationWordItem() ) != NULL )
				{
				existingSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem );

				selectedPrimarySpecificationItem = ( isPossessive &&
													existingSpecificationItem == NULL &&
													userSpecificationItem->assumptionLevel() < secondarySpecificationItem->assumptionLevel() ? userSpecificationItem :

													( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
													possessiveDefinitionSpecificationWordItem == alternativeSecondarySpecificationWordItem ? alternativeSecondarySpecificationItem :
																																			secondarySpecificationItem ) );
				selectedAdditionalDefinitionSpecificationItem = ( !isPossessive &&
																isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationItem_ :
																														possessiveDefinitionSpecificationItem_ );
				selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;

				if( existingSpecificationItem == NULL ||

				( !selectedPrimarySpecificationItem->isPossessive() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) )
					{
					// Make feminine possessive exclusive specification substitution assumption
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine possessive exclusive specification substitution assumption with generalization word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );

					if( isPossessive )
						{
						// Test files: "reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)",
						//				among others
						if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&

							// Specification doesn't exist yet
							( ( existingSpecificationItem = generalizationWordItem->firstNonExclusiveSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) == NULL ||

							// Justification doesn't exist yet in specification
							( !existingSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL, NULL ) ) ) &&

						// Draw possessive reversible conclusion
						adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, NULL, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						// Typical for Spanish
						// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
						if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
						!userSpecificationItem->hasSpecificationCollection() &&
						existingSpecificationItem != NULL &&
						( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL &&
						( selectedPrimarySpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
						// Justification doesn't exist yet in specification
						!existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) &&
						// Add Spanish possessive exclusive specification substitution justification to existing specification
						existingSpecificationItem->addJustificationToSpecification( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish possessive exclusive specification substitution assumption justification to the existing specification" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	SpecificationItem *alternativeJustificationSpecificationItem( bool hasGeneralizationWordFeminineProperNounEnding, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationItem *possessiveSpecificationItem;
		WordItem *userSpecificationWordItem;

		if( generalizationWordItem != NULL &&
		userSpecificationItem != NULL )
			{
			if( generalizationWordItem->isUserRelationWord &&
			userSpecificationItem->isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNounEnding &&
			( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL &&
			// Try to find a possessive specification
			( possessiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, true, false, userSpecificationWordItem ) ) != NULL )
				return possessiveSpecificationItem;
			}

		return NULL;
		}

	SpecificationItem *getAdditionalDefinitionSpecificationItem( WordItem *userSpecificationWordItem )
		{
		SpecificationItem *additionalDefinitionSpecificationItem = NULL;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *higherLevelPossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;

		if( userSpecificationWordItem != NULL &&
		( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			// Do for all possessive noun words
			do	{
				// First, try to find a possessive specification on a higher level
				if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
					{
					do	{
						if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
						possessiveSpecificationWordItem->firstSpecificationItem( false, false, false, userSpecificationWordItem ) != NULL )
							higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveDefinitionSpecificationWordItem == NULL &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
						additionalDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == NULL ||
																higherLevelPossessiveDefinitionSpecificationWordItem == possessiveDefinitionSpecificationItem->specificationWordItem() ? possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() :
																																														possessiveDefinitionSpecificationItem );
					}
				}
			while( additionalDefinitionSpecificationItem == NULL &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return additionalDefinitionSpecificationItem;
		}


	protected:
	// Constructor

	AdminReasoning( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeAdminReasoningVariables()
		{
		firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;
		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;
		}

	signed char addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore( SpecificationItem *hiddenSpanishSpecificationItem, WordItem *specificationWordItem )
		{
		unsigned int relationCollectionNr;
		SpecificationItem *reverseSpecificationItem;
		WordItem *currentCollectionWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore";

		if( hiddenSpanishSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given hidden Spanish specification item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( relationCollectionNr = hiddenSpanishSpecificationItem->relationCollectionNr() ) > NO_COLLECTION_NR &&
		( currentCollectionWordItem = adminItem_->firstCollectionWordItem( relationCollectionNr ) ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( reverseSpecificationItem = currentCollectionWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
				reverseSpecificationItem->isSelfGeneratedSpecification() &&
				// Justification doesn't already exist in specification
				!reverseSpecificationItem->hasPrimarySpecificationJustification( hiddenSpanishSpecificationItem ) &&
				// Justification has at least the same assumption level
				reverseSpecificationItem->assumptionLevel() >= hiddenSpanishSpecificationItem->assumptionLevel() &&
				// Add reversible justification to unhidden specification
				reverseSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, hiddenSpanishSpecificationItem, NULL, NULL ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible specification justification to the reverse specification" );
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isSpanishCurrentLanguage, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *simpleUserSpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasNonCompoundSecondarySpecificationCollection;
		bool hasPrimarySpecificationRelationWord;
		bool hasUserSpecificationRelationWord;
		bool isCommonWordWordSpanishAmbiguous = false;
		bool isPrimaryExclusiveSpecification;
		bool isPrimarySameAsUserSpecification;
		bool isPrimaryUserSpecification;
		bool isSecondaryAdjectiveSpecification;
		bool isSecondaryExclusiveSpecification;
		bool isSpecificationWordSpanishAmbiguous = false;
		unsigned int compoundSpecificationCollectionNr;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondarySpecificationCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *relatedSpecificationItem;
		WordItem *collectionWordItem = NULL;
		WordItem *commonWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr() ) == NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification collection" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( commonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL )
			isCommonWordWordSpanishAmbiguous = commonWordItem->isNounWordSpanishAmbiguous();

		if( ( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( commonWordItem == NULL ||
		( specificationCollectionNr = commonWordItem->collectionNr( specificationWordItem ) ) == NO_COLLECTION_NR )
			specificationCollectionNr = secondarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		hasPrimarySpecificationRelationWord = primarySpecificationItem->hasRelationWord();
		isPrimaryExclusiveSpecification = primarySpecificationItem->isExclusiveSpecification();
		isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
		isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();

		hasNonCompoundSecondarySpecificationCollection = secondarySpecificationItem->hasNonCompoundSpecificationCollection();
		isSecondaryAdjectiveSpecification = secondarySpecificationItem->isAdjectiveSpecification();
		isSecondaryExclusiveSpecification = secondarySpecificationItem->isExclusiveSpecification();

		hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
		hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
		hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();

		if( isSpanishCurrentLanguage )
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		commonWordItem != NULL &&
		relatedSpecificationItem == NULL &&
		!userSpecificationItem->isPossessive() &&
		( nonCompoundSpecificationCollectionNr = commonWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR )
			{
			if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

			// No related specification
		if( relatedSpecificationItem == NULL )
			{
			if( ( !isSpecificationWordSpanishAmbiguous ||
			// Typical for Spanish
			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ||
			!hasGeneralizationWordFeminineProperNounEnding ) &&

				( hasUserSpecificationRelationWord ||
				isPrimarySameAsUserSpecification ||
				!userSpecificationWordItem->isMasculineWord() ||

				( !isSecondaryAdjectiveSpecification &&

					( ( compoundSpecificationCollectionNr = secondarySpecificationWordItem->compoundCollectionNr() ) == NO_COLLECTION_NR ||
					secondarySpecificationWordItem->compoundGeneralizationWordItem( compoundSpecificationCollectionNr ) == userSpecificationWordItem ) ) ||

				( globalVariables_->nUserSpecificationWords > 1 &&

					( !userSpecificationWordItem->isNounWordSpanishAmbiguous() ||
					// Typical for Spanish
					isPrimaryExclusiveSpecification ) ) ) )
				{
				// Skip invalid questions (step 1)
				if( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
				primarySpecificationItem->hasCompoundSpecificationCollection() ||

				// Test file: "Additional test"
				( isPrimarySameAsUserSpecification &&
				!secondarySpecificationWordItem->isFeminineOrMasculineWord() ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						// Typical for Spanish
						existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem );
					else
						collectionWordItem = secondarySpecificationWordItem->collectionWordItem( secondarySpecificationCollectionNr );

					// Skip invalid questions (step 2)
					if( ( collectionWordItem != NULL &&

						( hasPrimarySpecificationRelationWord ||
						generalizationWordItem->primaryAndSecondarySpecificationJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, secondarySpecificationItem ) == NULL ) ) ||

					// Typical for Spanish
					( isSpecificationWordSpanishAmbiguous &&

						// Not user specification
						( ( !isPrimaryUserSpecification &&

							( existingSpecificationItem == NULL ||
							existingSpecificationItem->isHiddenSpanishSpecification() ) ) ||

						// User specification
						( isPrimaryExclusiveSpecification &&
						isPrimaryUserSpecification ) ) ) )
						{
						if( ( isPrimarySameAsUserSpecification ||
						isSecondaryExclusiveSpecification ||
						hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ) &&

						// Typical for Spanish
						( isSpanishCurrentLanguage ||
						!primarySpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() ||
						primarySpecificationItem->isOlderItem() ||
						userGeneralizationWordItem->hasCurrentlyMoreSpecificSpecification() ) &&

						// Ask questions
						askQuestions( isSpanishCurrentLanguage, simpleUserSpecificationItem, userSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );

						if( !hasUserSpecificationRelationWord &&
						isSecondaryAdjectiveSpecification &&

							( !isSpecificationWordSpanishAmbiguous ||
							// Typical for Spanish
							// Spanish test file: "Esta información es más específico (no exclusiva)"
							!isSecondaryExclusiveSpecification ) &&

						// Justification doesn't exist yet in word
						generalizationWordItem->secondarySpecificationJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, secondarySpecificationItem ) == NULL &&
						// Ask adjective question
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, WORD_TYPE_ADJECTIVE, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask an adjective question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					{
					// Definition specification
					// Test files: "reasoning\Scientific challenge",
					//				"reasoning\family\John - Anna (before family definition)"
					if( hasPrimarySpecificationRelationWord &&
					userSpecificationItem->isNounGeneralization() &&
					primarySpecificationItem->isSelfGeneratedSpecification() &&

					// Question part doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == NULL ||
					// Justification doesn't exist yet in question
					!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) ) &&

					// Ask a specification substitution question
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}
		else	// Related specification
			{
			// Typical for Spanish
			if( isCommonWordWordSpanishAmbiguous &&
			isSpecificationWordSpanishAmbiguous &&
			isPrimaryUserSpecification &&
			globalVariables_->nUserSpecificationWords == 1 &&
			!primarySpecificationItem->hasNonCompoundSpecificationCollection() )
				{
				// Spanish test file: "Español ambigua - Ana es un padre - Luisa es un hijo"
				if( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
				// Ask Spanish ambiguous questions
				askQuestions( isSpanishCurrentLanguage, simpleUserSpecificationItem, userSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask Spanish ambiguous questions" );
				}
			else
				{
				if( !hasPrimarySpecificationRelationWord &&

					( hasNonCompoundSecondarySpecificationCollection ||

					// Test file: "reasoning\Knowledge of organisms (1)"
					( secondarySpecificationItem->isUserSpecification() &&
					secondarySpecificationWordItem->compoundCollectionNr() == secondarySpecificationCollectionNr ) ) &&

				// Draw negative conclusion
				drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char drawCompoundSpecificationSubstitutionConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isSpanishCurrentLanguage, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasCurrentCompoundSpecificationCollection;
		bool hasCurrentNonCompoundSpecificationCollection;
		bool hasCurrentSpecificationCreationSentenceNr;
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool isCurrentAdjectiveSpecification;
		bool isCurrentExclusiveSpecification;
		bool isPrimaryExclusiveSpecification;
		bool isExclusiveUserSpecification = false;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isOlderPrimarySpecification;
		bool isSelectedExclusiveSpecification;
		unsigned short currentSpecificationWordTypeNr;
		unsigned short selectedJustificationTypeNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr;
		SpecificationItem *adjectiveSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *exclusiveSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *previousUserSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *previousUserSpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() ) != NULL )
			{
			isPrimaryExclusiveSpecification = primarySpecificationItem->isExclusiveSpecification();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();

			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();

			compoundGeneralizationWordItem = specificationWordItem->compoundGeneralizationWordItem(primarySpecificationCollectionNr);

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem &&
				currentSpecificationWordItem != compoundGeneralizationWordItem )
					{
					hasCurrentCompoundSpecificationCollection = currentSpecificationItem->hasCompoundSpecificationCollection();
					hasCurrentNonCompoundSpecificationCollection = currentSpecificationItem->hasNonCompoundSpecificationCollection();
					hasCurrentSpecificationCreationSentenceNr = currentSpecificationItem->hasCurrentCreationSentenceNr();
					isCurrentAdjectiveSpecification = currentSpecificationItem->isAdjectiveSpecification();
					isCurrentExclusiveSpecification = currentSpecificationItem->isExclusiveSpecification();
					currentSpecificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();

					isSelectedExclusiveSpecification = ( isExclusiveSpecification &&
														currentSpecificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
														currentSpecificationCollectionNr > NO_COLLECTION_NR );

					// Definition specification
					if( ( !isGeneralizationProperNoun &&

						( ( !isCurrentExclusiveSpecification &&

							( hasCurrentNonCompoundSpecificationCollection ||

							// Older definition specification (without specification collection)
							( currentSpecificationCollectionNr == NO_COLLECTION_NR &&
							currentSpecificationItem->isOlderItem() ) ) ) ||

						// Exclusive compound definition specification
						( isCurrentExclusiveSpecification &&
						hasCurrentCompoundSpecificationCollection &&
						compoundGeneralizationWordItem != NULL &&
						currentSpecificationWordItem->isFeminineOrMasculineWord() ) ) &&

					// User specification doesn't exist yet
					generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, currentSpecificationWordItem ) == NULL ) ||

					// Proper noun specification
					// Test file: "reasoning\Scientific challenge"
					( isGeneralizationProperNoun &&
					// Adjective specification word
					isCurrentAdjectiveSpecification &&
					isCurrentExclusiveSpecification &&
					isOlderPrimarySpecification &&
					hasCurrentSpecificationCreationSentenceNr &&
					!hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					!isSpanishCurrentLanguage ) )
						{
						selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;

						if( isCurrentAdjectiveSpecification )
							{
							if( compoundGeneralizationWordItem == NULL )
								{
								if( isCurrentExclusiveSpecification )
									{
									// Create adjective question
									isSelectedExclusiveSpecification = true;
									questionParameter = WORD_PARAMETER_SINGULAR_VERB_IS;
									selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION;
									}
								else
									{
									if( isPrimaryExclusiveSpecification )
										{
										if( isOlderPrimarySpecification )
											// Test file: "reasoning\family\John has 2 sons and a daughter (before family definition)"
											isSelectedExclusiveSpecification = true;

										// Definition
										if( !isOlderPrimarySpecification &&
										!isSpanishCurrentLanguage &&
										primarySpecificationCollectionNr > NO_COLLECTION_NR )
											// Test file: "reasoning\Scientific challenge"
											isSelectedExclusiveSpecification = true;
										}
									}
								}
							else
								{
								// Specification doesn't exist yet
								if( generalizationWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem ) == NULL )
									// Create exclusive adjective specification
									isSelectedExclusiveSpecification = true;
								}
							}

						if( !hasCurrentSpecificationCreationSentenceNr ||
						isSelectedExclusiveSpecification ||

						( !isExclusiveUserSpecification &&
						!isSpanishCurrentLanguage ) )
							{
							if( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, questionParameter, currentSpecificationWordItem ) ) != NULL &&
							// Typical for Spanish
							isSpanishCurrentLanguage &&
							isExclusiveUserSpecification &&
							isSelectedExclusiveSpecification &&
							!existingSpecificationItem->isExclusiveSpecification() &&
							!existingSpecificationItem->isOlderItem() &&
							( tempSpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem( currentSpecificationWordItem ) ) != NULL )
								// Spanish test file: "razonamiento\Reto científico"
								existingSpecificationItem = tempSpecificationItem;

							// Specification doesn't exist yet
							if( existingSpecificationItem == NULL ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, currentSpecificationItem ) )
								{
								// Draw specification substitution conclusion
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isSelectedExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

								if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
									{
									if( !createdOrFoundSpecificationItem->isExclusiveSpecification() &&
									// Test file: "reasoning\Scientific challenge"
									specificationWordItem->bestMatchingRelationSpecificationItem( true, false, currentSpecificationCollectionNr, NULL ) != NULL &&
									// Draw simple negative conclusions
									drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( !isGeneralizationProperNoun )
										{
										if( isSelectedExclusiveSpecification )
											{
											if( hasCurrentNonCompoundSpecificationCollection &&
											hasCurrentSpecificationCreationSentenceNr &&
											!isSpanishCurrentLanguage &&
											// Make generalization assumption definition
											adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, specificationWordTypeNr, generalizationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, currentSpecificationItem, NULL, specificationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption definition about generalization word \"", specificationWordItem->anyWordTypeString(), "\" and specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( !hasCurrentNonCompoundSpecificationCollection &&
											primarySpecificationItem->hasSpecificationCollection() &&
											( adjectiveSpecificationItem = userSpecificationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&
											( exclusiveSpecificationItem = currentSpecificationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL )
												{
												// Find previous user specification
												if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, userSpecificationItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

												if( ( previousUserSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
												( previousUserSpecificationWordItem = previousUserSpecificationItem->specificationWordItem() ) != NULL &&
												( adjectiveSpecificationItem = previousUserSpecificationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&
												( exclusiveSpecificationItem = currentSpecificationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&

												// Specification doesn't exist yet
												( ( existingSpecificationItem = previousUserSpecificationWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) == NULL ||
												// Justification doesn't exist yet in specification
												!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( exclusiveSpecificationItem, adjectiveSpecificationItem ) ) )
													{
													// Make previous user generalization assumption definition
													if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, previousUserSpecificationItem->generalizationWordTypeNr(), exclusiveSpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, exclusiveSpecificationItem, NULL, adjectiveSpecificationItem, NULL, previousUserSpecificationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a previous user generalization assumption definition about generalization word \"", previousUserSpecificationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

													if( ( secondarySpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
													// Add specification substitution justification to created or found specification
													createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, previousUserSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the created or found specification" );
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
						// Corrected assumption by opposite suggestive question
						if( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&
						isCurrentAdjectiveSpecification &&
						// Get exclusive question part
						( exclusiveSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, true, questionParameter, ( compoundGeneralizationWordItem == NULL ? specificationWordItem->compoundCollectionNr() : primarySpecificationItem->specificationCollectionNr() ), specificationWordItem ) ) != NULL &&
						// Ask adjective question
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, exclusiveSpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask an adjective question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem->nextNonNegativeNonPosessiveDefinitionSpecificationItem() ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawExclusiveNegativeUserConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		SpecificationItem *previousUserSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawExclusiveNegativeUserConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem != NULL &&
		( previousUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, previousSpecificationWordItem ) ) != NULL )
			{
			// Negative specification doesn't exist yet
			if( previousSpecificationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) == NULL &&
			// Draw negative conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, previousUserSpecificationItem, NULL, NULL, NULL, previousSpecificationWordItem, currentSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative user conclusion about generalization word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

			// Negative specification doesn't exist yet
			if( currentSpecificationWordItem->firstSpecificationItem( true, false, false, previousSpecificationWordItem ) == NULL &&
			// Draw another negative conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, currentSpecificationWordItem, previousSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeUserDefinitionConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *adjectiveSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *adjectiveNegativeSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeUserDefinitionConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( adjectiveSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given adjective specification word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		// Negative specification doesn't exist yet
		if( generalizationWordItem->firstSpecificationItem( true, false, false, adjectiveSpecificationWordItem ) == NULL )
			{
			// Draw adjective negative conclusion
			if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_ADJECTIVE, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, adjectiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", adjectiveSpecificationWordItem->anyWordTypeString(), "\"" );

			if( ( adjectiveNegativeSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
			// Get first generalization specification noun word
			( currentGeneralizationItem = adjectiveSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
					currentGeneralizationWordItem != generalizationWordItem )
						{
						// Negative definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != NULL &&
						// Draw negative generalization conclusion (negative definition)
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative generalization conclusion with negative definition about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

						// Positive definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem->firstNonExclusiveSpecificationItem( false, false, false, adjectiveSpecificationWordItem ) ) != NULL )
							{
							// Negative conclusion doesn't exist yet
							if( generalizationWordItem->firstNonExclusiveSpecificationItem( true, false, false, currentGeneralizationWordItem ) == NULL &&
							// Draw negative generalization conclusion (positive definition)
							adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, definitionSpecificationItem, NULL, adjectiveNegativeSpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative generalization conclusion with positive definition about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

							// Draw negative generalization conclusion (reversed, positive definition)
							if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, definitionSpecificationItem, NULL, adjectiveNegativeSpecificationItem, NULL, currentGeneralizationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversed negative generalization conclusion with positive definition about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char drawOnlyOptionLeftNounConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem )
		{
		bool hasFoundAssignment;
		bool hasMoreOptionsLeft = false;
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstNegativeOnlyOptionLeftAssignmentItem = NULL;
		SpecificationItem *firstOnlyOptionLeftSpecificationItem = NULL;
		SpecificationItem *foundOnlyOptionLeftSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawOnlyOptionLeftNounConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationCollectionNr <= NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem->isExclusiveSpecification() &&
				currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
				( currentAssignmentItem = generalizationWordItem->firstSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != NULL )
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
							hasMoreOptionsLeft = true;
							// Clear the found option
							foundOnlyOptionLeftSpecificationItem = NULL;
							}
						}
					}
				}
			while( !hasMoreOptionsLeft &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

			if( firstNegativeOnlyOptionLeftAssignmentItem != NULL &&
			foundOnlyOptionLeftSpecificationItem != NULL &&
			// Draw an only option left conclusion
			adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, true, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusionsInProperNounWords( bool isArchivedAssignment, bool isChineseCurrentLanguage, bool isSpanishCurrentLanguage, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
		{
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationCurrentlyMoreSpecificSpecification;
		bool hasUserSpecificationRelationWord;
		bool isPossessiveUserSpecification;
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserAssignment;
		bool isUserQuestion;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *firstJustificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *currentProperNounWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userRelationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusionsInProperNounWords";

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		if( ( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the user specification item is undefined" );

		if( ( currentProperNounWordItem = globalVariables_->firstUserDefinedProperNounWordItem ) != NULL )
			{
			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			hasUserSpecificationCurrentlyMoreSpecificSpecification = userGeneralizationWordItem->hasCurrentlyMoreSpecificSpecification();

			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserAssignment = userSpecificationItem->isAssignment();
			isUserQuestion = userSpecificationItem->isQuestion();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
			userRelationWordItem = userSpecificationItem->relationWordItem();

			// Do for all user-defined proper noun words
			do	{
				if( currentProperNounWordItem != spanishRelationWordItem )
					{
					if( ( primarySpecificationItem = currentProperNounWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL ||
					isSpecificationWordSpanishAmbiguous )
						{
						primarySpecificationItem = currentProperNounWordItem->firstAssignmentOrSpecificationItem( false, true, false, false, false, specificationWordItem );

						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&

						( primarySpecificationItem == NULL ||
						primarySpecificationItem->isHiddenSpanishSpecification() ) )
							{
							if( spanishRelationWordItem == NULL )
								{
								// Try to find self-generated question, as alternative to hidden specification
								primarySpecificationItem = ( ( tempSpecificationItem = currentProperNounWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, true, specificationWordItem ) ) == NULL ||
															!tempSpecificationItem->isExclusiveSpecification() ? NULL :
																												tempSpecificationItem );
								}
							else
								{
								// Typical for Spanish
								if( hasUserSpecificationRelationWord &&
								isPossessiveUserSpecification &&
								!isUserSpecificationWordSpanishAmbiguous &&
								primarySpecificationItem != NULL &&
								primarySpecificationItem->hasRelationCollection() &&
								!currentProperNounWordItem->isUserGeneralizationWord &&
								currentProperNounWordItem->isUserRelationWord )
									primarySpecificationItem = userSpecificationItem;
								else
									{
									tempSpecificationItem = ( userSpecificationCollectionNr == NO_COLLECTION_NR &&	// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
																primarySpecificationItem != NULL ) ? spanishRelationWordItem->bestMatchingRelationSpecificationItem( true, true, false, true, primarySpecificationItem->specificationCollectionNr(), specificationWordItem ) : 
																									spanishRelationWordItem->bestMatchingRelationSpecificationItem( true, true, false, true, specificationWordItem, currentProperNounWordItem );

									primarySpecificationItem = ( tempSpecificationItem == NULL ||
																tempSpecificationItem->isHiddenSpanishSpecification() ? NULL : tempSpecificationItem );
									}
								}
							}
						}
					else
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage &&
						primarySpecificationItem->isUserSpecification() &&
						( tempSpecificationItem = currentProperNounWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
							// Convert user assignment to simple user specification
							primarySpecificationItem = tempSpecificationItem;
						}

					if( primarySpecificationItem != NULL &&

						// No user relation word
						( ( !hasUserSpecificationRelationWord &&

							( !primarySpecificationItem->isOlderItem() ||
							// Test file: "reasoning\family\John - Anna (before family definition)"
							!primarySpecificationItem->hasSpecificationCollection() ) ) ||

						// User relation word
						( hasUserSpecificationRelationWord &&
						primarySpecificationItem->hasCurrentCreationSentenceNr() &&

							// No user compound collection
							( ( !hasUserSpecificationCompoundCollection &&

								// Test file: "reasoning\family\Complex (13)"
								( ( primarySpecificationItem->isOlderItem() &&
								primarySpecificationItem->hasCompoundSpecificationCollection() ) ||

								( primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
								primarySpecificationItem->specificationCollectionNr() == userSpecificationCollectionNr ) ) ) ||

							// User compound collection
							( hasUserSpecificationCompoundCollection &&
							primarySpecificationItem->hasNonCompoundSpecificationCollection() &&

								( isUserQuestion ||

								( primarySpecificationItem->hasRelationWord() &&
								( firstJustificationItem = primarySpecificationItem->firstJustificationItem() ) != NULL &&

									// Not older specification
									( ( !primarySpecificationItem->isOlderItem() &&

										// Specification substitution assumption or conclusion
										( firstJustificationItem->isSpecificationSubstitutionAssumptionOrConclusion() ||

										// Opposite possessive specification assumption
										( firstJustificationItem->isOppositePossessiveSpecificationAssumption() &&
										primarySpecificationItem->hasOnlyOneRelationWord() &&

											( !primarySpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() ||
											// Test files: "reasoning\family\Complex (12)",
											//				"reasoning\family\Complex (15)",
											//				"reasoning\family\This information is more specific"
											primarySpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) == NULL ||

											// Typical for Chinese
											// Chinese test file: "This information is more specific"
											( isChineseCurrentLanguage &&
											hasUserSpecificationCurrentlyMoreSpecificSpecification ) ) ) ) ) ||

									// Older specification
									( primarySpecificationItem->isOlderItem() &&
									primarySpecificationItem->hasRelationCollection() &&

										// Specification substitution assumption or conclusion
										// Test files: "reasoning\family\Complex (4)",
										//				"reasoning\family\Complex (5)",
										//				"reasoning\family\Complex (6)",
										//				"reasoning\family\Complex (9)",
										//				"reasoning\family\Complex (14)",
										//				among others
										( ( userRelationWordItem != NULL &&
										( tempSpecificationItem = userRelationWordItem->firstActiveQuestionSpecificationItem() ) != NULL &&
										tempSpecificationItem->isOlderItem() ) ||

										( !firstJustificationItem->isOlderItem() &&

											// Test files: "conflict\family\Joe is a mother",
											//				"reasoning\family\Complex (14)",
											//				among others
											( firstJustificationItem->isSpecificationSubstitutionAssumptionOrConclusion() ||

											// Test file: "reasoning\family\John is the father - Paul - Laura"
											( !isUserAssignment &&
											firstJustificationItem->isReversibleAssumptionOrConclusion() &&
											currentProperNounWordItem->firstActiveQuestionSpecificationItem() != NULL &&
											( tempSpecificationItem = currentProperNounWordItem->firstNegativeSpecificationItem() ) != NULL &&
											tempSpecificationItem->isOlderItem() ) ) ) ) ) ||

									// Typical for Spanish
									isSpanishCurrentLanguage ) ) ) ) ) ) ||

							// Typical for Spanish
							( isSpanishCurrentLanguage &&

								// No user relation word
								( !hasUserSpecificationRelationWord ||
								isUserSpecificationWordSpanishAmbiguous ||
								// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
								userSpecificationCollectionNr == NO_COLLECTION_NR ||

								( primarySpecificationItem->hasCurrentCreationSentenceNr() &&
								primarySpecificationItem->specificationCollectionNr() == userSpecificationCollectionNr ) ) ) ) &&

					// Avoid idle call
					globalVariables_->firstPossessiveNounWordItem != NULL &&
					// Draw proper noun part-of conclusions
					drawProperNounPartOfConclusions( isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, currentProperNounWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions in proper noun word \"", currentProperNounWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentProperNounWordItem = currentProperNounWordItem->nextUserDefinedProperNounWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawReversibleConclusions( bool isCheckingAssignment, bool isChineseCurrentLanguage, bool isDrawingPossessiveReversibleConclusions, bool isSpanishCurrentLanguage, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool isArchivedAssignment;
		unsigned int nStartCreatedISpanishItems = 0;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawReversibleConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem->isProperNounWord() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item isn't a proper noun" );

		if( isCheckingAssignment )
			{
			if( isSpanishCurrentLanguage )
				nStartCreatedISpanishItems = globalVariables_->nTotalCreatedItems;

			isArchivedAssignment = ( generalizationWordItem->firstNonQuestionSpecificationItem( true, true, false ) == NULL );

			// Draw selected reversible conclusions based on assignments
			if( drawSelectedReversibleConclusions( true, isArchivedAssignment, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with active assignments" );
			}

		if( ( !isCheckingAssignment ||
		userSpecificationItem->isOlderItem() ||
		// Typical for Chinese
		isChineseCurrentLanguage ||

		// Typical for Spanish
		( isSpanishCurrentLanguage &&
		// No progress made in assignment calls above
		globalVariables_->nTotalCreatedItems == nStartCreatedISpanishItems ) ) &&

		// Draw selected reversible conclusions based on specifications
		drawSelectedReversibleConclusions( false, false, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with specifications" );

		return RESULT_OK;
		}

	signed char drawSimpleNegativeConclusions( bool isArchivedAssignment, bool isSpanishCurrentLanguage, SpecificationItem *primarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge;
		bool hasUserSpecificationCurrentCreationSentenceNr;
		bool hasUserSpecificationNonCompoundCollection;
		bool hasUserSpecificationRelationWord;
		bool isDrawingCompoundNegativeConclusions;
		bool isDrawingNegativeUserConclusion;
		bool isPrimaryExclusiveSpecification;
		bool isPossessiveUserSpecification;
		bool isPrimarySelfGeneratedSpecification;
		bool isUserRelationWord;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned short generalizationWordTypeNr;
		unsigned short negativeSpecificationWordTypeNr;
		unsigned short primaryAssumptionLevel;
		unsigned short userAssumptionLevel;
		unsigned short userSpecificationWordTypeNr;
		unsigned int compoundCollectionNr = NO_COLLECTION_NR;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *negativeSpecificationItem;
		SpecificationItem *negativeDefinitionSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		WordItem *negativeSpecificationWordItem;
		WordItem *specificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSimpleNegativeConclusions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( specificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( negativeDefinitionSpecificationItem = specificationWordItem->firstNegativeSpecificationItem() ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			isPrimaryExclusiveSpecification = primarySpecificationItem->isExclusiveSpecification();
			isPrimarySelfGeneratedSpecification = primarySpecificationItem->isSelfGeneratedSpecification();
			generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();
			primaryAssumptionLevel = primarySpecificationItem->assumptionLevel();

			// Don't move, because of redefining user specification
			if( ( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL )
				userSpecificationItem = simpleUserSpecificationItem;

			hasUserSpecificationCurrentCreationSentenceNr = userSpecificationItem->hasCurrentCreationSentenceNr();
			hasUserSpecificationNonCompoundCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationWordTypeNr = userSpecificationItem->specificationWordTypeNr();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
			userAssumptionLevel = userSpecificationItem->assumptionLevel();

												// If the user specification is possessive, the user generalization word must be opposite, and otherwise
			isDrawingNegativeUserConclusion = ( isPossessiveUserSpecification == isUserRelationWord &&
												userSpecificationItem->isProperNounGeneralization() &&
												!userSpecificationItem->isOlderItem() &&
												!userSpecificationItem->isQuestion() );

			isDrawingCompoundNegativeConclusions = ( isDrawingNegativeUserConclusion &&
													( compoundCollectionNr = userSpecificationWordItem->compoundCollectionNr() ) > NO_COLLECTION_NR &&
													userSpecificationWordItem->collectionWordItem( compoundCollectionNr ) != NULL );

			do	{
				if( ( negativeSpecificationWordItem = negativeDefinitionSpecificationItem->specificationWordItem() ) != NULL &&

					( negativeDefinitionSpecificationItem->isAdjectiveSpecification() ||

					( !isPrimaryExclusiveSpecification &&

						// Positive specification doesn't exist
						( generalizationWordItem->firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) == NULL ||
						hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge ||
						// Typical for Spanish
						negativeSpecificationWordItem->isNounWordSpanishAmbiguous() ) ) ) )
					{
					negativeSpecificationWordTypeNr = negativeDefinitionSpecificationItem->specificationWordTypeNr();

					if( isDrawingNegativeUserConclusion &&

						// Test files: "reasoning\family\Complex (8)",
						//				"reasoning\family\Complex (15)",
						//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
						( isPossessiveUserSpecification ||
						// No answered question with this specification word
						generalizationWordItem->bestMatchingRelationSpecificationItem( true, true, false, false, true, false, false, true, userSpecificationCollectionNr, NO_COLLECTION_NR, negativeSpecificationWordItem, NULL ) == NULL ) )
						{
						negativeSpecificationItem = ( userSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE ? negativeSpecificationWordItem->firstSpecificationItem( true, false, false, userSpecificationWordItem ) :
																														userSpecificationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) );

						if( negativeSpecificationItem != NULL &&
							
							// Negative specification doesn't exist
							( ( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == NULL &&

								( negativeSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE ||

								( hasUserSpecificationNonCompoundCollection &&

									( negativeSpecificationItem->specificationCollectionNr() == userSpecificationCollectionNr ||
									// Typical for Spanish
									isSpanishCurrentLanguage ) ) ) ) ||

							// Negative specification exists
							( existingSpecificationItem != NULL &&
							!hasGeneralizationWordCurrentlyConfirmedSpecification &&
							!hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation &&
							existingSpecificationItem->hasPrimaryNounSpecificationJustification() &&
							// Justification doesn't exist yet in specification
							!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, negativeSpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, negativeSpecificationItem, NULL ) ) ) &&
						// Draw negative user conclusion
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, negativeSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );
						}

					if( hasUserSpecificationCurrentCreationSentenceNr ||
					isUserRelationWord )
						{
						obsoleteSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, (unsigned short)NO_QUESTION_PARAMETER, negativeDefinitionSpecificationItem->specificationCollectionNr(), NULL );

						// Negative specification doesn't exist
						if( ( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == NULL &&

							( !isDrawingNegativeUserConclusion ||

							( obsoleteSpecificationItem != NULL &&
							primaryAssumptionLevel <= userAssumptionLevel ) ||

							( isPrimarySelfGeneratedSpecification &&

								( !isDrawingCompoundNegativeConclusions ||

								// Test file: "reasoning\Knowledge of organisms (1)"
								( hasUserSpecificationNonCompoundCollection &&
								primaryAssumptionLevel <= userAssumptionLevel ) ||

								// Typical for Spanish
								( isUserSpecificationWordSpanishAmbiguous &&

									( negativeSpecificationWordItem->isMasculineWord() ||

									// Spanish test files: "razonamiento\familia\Complejo (12)",
									//						"razonamiento\familia\Complejo (15)"
									( isPossessiveUserSpecification &&
									( firstJustificationItem = primarySpecificationItem->firstJustificationItem() ) != NULL &&
									firstJustificationItem->hasPrimaryUserSpecification() ) ) ) ) ) ) ) ||

						// Negative specification exists
						( existingSpecificationItem != NULL &&
						// Justification doesn't exist yet in specification
						!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, negativeDefinitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, negativeDefinitionSpecificationItem, NULL ) ) )
							{
							// Draw simple negative conclusion
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, negativeDefinitionSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );

							if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
								{
								// Definition specification
								if( generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR )
									{
									// Draw negative definition conclusions
									if( drawNegativeDefinitionConclusions( isArchivedAssignment, generalizationWordTypeNr, negativeSpecificationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, negativeSpecificationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative definition conclusions" );
									}
								else	// Proper noun specification
									{
									if( obsoleteSpecificationItem != NULL &&
									// Test file: "reasoning\family\Correcting invalidated assumption (by knowledge)"
									createdOrFoundSpecificationItem->isCorrectedSpecification() &&
									negativeDefinitionSpecificationItem->specificationCollectionNr() == userSpecificationCollectionNr &&
									// Correct invalid suggestive assumptions by knowledge
									correctSuggestiveAssumptionsByKnowledge( userSpecificationItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct invalid suggestive assumptions by knowledge about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}

								// Test file: "reasoning\Knowledge of organisms (1)"
								if( !hasUserSpecificationRelationWord &&
								isDrawingCompoundNegativeConclusions &&
								userSpecificationCollectionNr != compoundCollectionNr &&
								// Draw negative exclusive conclusions
								drawNegativeExclusiveConclusions( isArchivedAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, negativeSpecificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative exclusive conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with negative specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					}
				}
			while( ( negativeDefinitionSpecificationItem = negativeDefinitionSpecificationItem->nextNegativeSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *primaryGeneralizationWordItem;
		WordItem *primarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != NULL )
					{
					assumptionLevel = ( primarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() :
																																		primarySpecificationItem->assumptionLevel() );

					// Collect generalization words of a specification-generalization definition
					if( adminItem_->collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, generalizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

					// Specification-generalization
					currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

					if( currentSpecificationItem->hasCurrentCreationSentenceNr() &&
					// Specification-generalization doesn't exist yet

						( ( foundSpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, currentGeneralizationWordItem ) ) == NULL ||
						foundSpecificationItem->isUserSpecification() ) &&

					// Draw specification-generalization conclusion (using current specification)
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, currentSpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentSpecificationItem, NULL, NULL, NULL, specificationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the current specification" );

					if( !isSelfGenerated &&
					currentSpecificationItem->hasGeneralizationCollection() &&
					( primaryGeneralizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					// Specification-generalization doesn't exist yet
					primarySpecificationWordItem->firstSpecificationItem( false, false, false, primaryGeneralizationWordItem ) == NULL &&
					// Draw specification-generalization conclusion (using primary specification)
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, primarySpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, NULL, NULL, primarySpecificationWordItem, primaryGeneralizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the primary specification" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationSubstitutionConclusionOrAskQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isSpanishCurrentLanguage, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCurrentSpecificationCollection;
		bool hasCurrentSpecificationCompoundCollection;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool hasPrimarySpecificationNonCompoundCollection;
		bool hasPrimarySpecificationRelationWord;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasUserSpecificationCompoundCollection = false;
		bool hasUserSpecificationNonCompoundCollection = false;
		bool hasUserSpecificationRelationWord = false;
		bool isCurrentAdjectiveSpecification;
		bool isArchivedUserAssignment = false;
		bool isCurrentUserSpecification;
		bool isDefinitionSpanishAmbiguous;
		bool isCurrentExclusiveSpecification;
		bool isExclusiveUserSpecification = false;
		bool isFeminineOrMasculineGeneralizationWord;
		bool isFoundPrimaryHiddenSpanishSpecification;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isOlderCurrentSpecification;
		bool isPossessiveUserSpecification = false;
		bool isPossessivePrimarySpecification;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserAssignment = false;
		bool isUserAdjectiveSpecification = false;
		bool isUserDefinitionSpecification = false;
		bool isUserGeneralizationWord;
		bool isUserQuestion = false;
		bool isUserRelationWord;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short currentSpecificationWordTypeNr;
		unsigned short selectedRelationWordTypeNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *tempJustificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *alternativeSpanishAdditionalDefinitionSpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *negativeConclusionSpecificationItem;
		SpecificationItem *negativeDefinitionSpecificationItem;
		SpecificationItem *negativeProperNounSpecificationItem;
		SpecificationItem *positiveDefinitionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selfGeneratedSpecificationItem;
		SpecificationItem *simpleUserSpecificationItem = NULL;
		SpecificationItem *spanishNonCompoundSpecificationItem;
		WordItem *collectionWordItem;
		WordItem *commonWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *oppositeSpecificationWordItem;
		WordItem *selectedRelationWordItem;
		WordItem *userSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_->isSpecificationReasoningWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a specification reasoning type" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() ) != NULL )
			{
			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem->hasMasculineProperNounEnding();
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem->isFeminineOrMasculineWord();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( userSpecificationItem != NULL )
				{
				hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
				hasUserSpecificationNonCompoundCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
				hasUserSpecificationRelationWord = userSpecificationItem->hasRelationWord();
				isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
				isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
				isPossessiveUserSpecification = userSpecificationItem->isPossessive();
				isUserAssignment = userSpecificationItem->isAssignment();
				isUserAdjectiveSpecification = userSpecificationItem->isAdjectiveSpecification();
				isUserDefinitionSpecification = userSpecificationItem->isNounGeneralization();
				isUserQuestion = userSpecificationItem->isQuestion();
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
				userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
				userSpecificationWordItem = userSpecificationItem->specificationWordItem();

				simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem();
				}

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					hasCurrentSpecificationCompoundCollection = currentSpecificationItem->hasCompoundSpecificationCollection();
					isCurrentUserSpecification = currentSpecificationItem->isUserSpecification();
					isCurrentAdjectiveSpecification = currentSpecificationItem->isAdjectiveSpecification();
					isCurrentExclusiveSpecification = currentSpecificationItem->isExclusiveSpecification();
					isOlderCurrentSpecification = currentSpecificationItem->isOlderItem();
					currentSpecificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > NO_COLLECTION_NR );

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, specificationWordItem, relationWordItem ) ) == NULL )
						foundPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, isQuestion, specificationWordItem );

					if( foundPrimarySpecificationItem == NULL )
						{
						if( isCurrentUserSpecification &&
						currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
						!currentSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
						// Check for specification conflict
						generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, false, false, isSpecificationWordSpanishAmbiguous, currentSpecificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->relationCollectionNr(), currentSpecificationWordItem, currentSpecificationItem->relationWordItem() ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem->hasNonCompoundSpecificationCollection();
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem->isHiddenSpanishSpecification();

						// Specification doesn't exist yet
						if( ( existingSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, currentSpecificationCollectionNr, NO_COLLECTION_NR, currentSpecificationWordItem, NULL ) ) == NULL )
							// Test file: "reasoning\family\John - Anna (before family definition)"
							// Typical for Spanish
							// Spanish test file: "razonamiento\Reto científico"
							existingSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, NO_COLLECTION_NR, NO_COLLECTION_NR, currentSpecificationWordItem, NULL );

						if( isCurrentExclusiveSpecification &&
						!isFeminineOrMasculineGeneralizationWord &&
						userSpecificationItem != NULL &&
						userSpecificationWordItem != NULL &&

							( existingSpecificationItem == NULL ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( foundPrimarySpecificationItem, currentSpecificationItem ) ||

							( hasCurrentSpecificationCompoundCollection &&
							relationWordItem != NULL &&
							!relationWordItem->hasCollectionNr( existingSpecificationItem->relationCollectionNr() ) &&
							generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, specificationWordItem, NULL ) != NULL ) ) &&

						// Matching feminine word
						( ( hasGeneralizationWordFeminineProperNounEnding &&
						currentSpecificationWordItem->isFeminineWord() ) ||

						// Matching masculine word
						( hasGeneralizationWordMasculineProperNounEnding &&
						currentSpecificationWordItem->isMasculineWord() ) ) &&

						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL &&
						// Select correct compound word to avoid assumption to be made if a question should be asked
						currentSpecificationWordItem->compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem )
							{
							selectedRelationWordItem = ( feminineOrMasculineSpecificationItem->isAdjectiveSpecification() ? NULL : relationWordItem );

							if( isFoundPrimaryHiddenSpanishSpecification )
								{
								selectedPrimarySpecificationItem = userSpecificationItem;
								selectedAdditionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
								}
							else
								{
								selectedPrimarySpecificationItem = foundPrimarySpecificationItem;
								selectedAdditionalDefinitionSpecificationItem = NULL;
								}

							// User generalization word
							if( ( isUserGeneralizationWord &&
							foundPrimarySpecificationItem->hasUpdatedInformation() &&

								( ( !isFoundPrimaryHiddenSpanishSpecification &&

								// Non-compound specification collection
								( ( hasPrimarySpecificationNonCompoundCollection &&

								( hasCurrentSpecificationCompoundCollection &&

								( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||
								foundPrimarySpecificationItem->hasRelationWord() ) ) ) ||

								// No non-compound specification collection
								( !hasPrimarySpecificationNonCompoundCollection &&
								!hasUserSpecificationRelationWord &&
								currentSpecificationWordItem->collectionNrByCompoundGeneralizationWordInWord( WORD_TYPE_NOUN_SINGULAR, userSpecificationWordItem ) == NO_COLLECTION_NR ) ) ) ||

								// Typical for Spanish
								( isFoundPrimaryHiddenSpanishSpecification &&
								// Justification doesn't exist yet in specification
								generalizationWordItem->foundJustificationItem( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem ) == NULL ) ) ) ||

							// Not user generalization word
							( !isUserGeneralizationWord &&
							hasPrimarySpecificationNonCompoundCollection &&
							!isSpecificationWordSpanishAmbiguous &&

								// No existing specification
								( ( existingSpecificationItem == NULL &&

									( !isPossessiveUserSpecification ||
									selectedRelationWordItem != NULL ) ) ||

								// Existing specification
								( existingSpecificationItem != NULL &&
								existingSpecificationItem->hasSpecificationCollection() &&

									( ( isUserAssignment &&
									!existingSpecificationItem->hasCurrentCreationSentenceNr() ) ||

									( ( hasRelationWord &&
									selectedRelationWordItem != NULL &&
									existingSpecificationItem->hasRelationWord() &&
									// Relation word is not covered by justification
									existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem ) == NULL &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, currentSpecificationItem ) &&

										( !isPossessiveUserSpecification ||
										!selectedRelationWordItem->hasCurrentlyConfirmedSpecification() ||
										// Justification doesn't exist yet
										generalizationWordItem->secondarySpecificationJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, currentSpecificationItem ) == NULL ) ) ||

									( isPossessiveUserSpecification &&
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, NULL ) ) ) ) ) ) ) )
								{
								selectedRelationWordTypeNr = ( selectedRelationWordItem == NULL ? NO_WORD_TYPE_NR : WORD_TYPE_PROPER_NOUN );

								// Make feminine or masculine proper noun ending assumption
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

								// Test files: "reasoning\family\Complex (2)",
								//				"reasoning\family\Complex (3)",
								//				"reasoning\family\Complex (4)",
								//				"reasoning\family\Complex (5)",
								//				"reasoning\family\Complex (8)",
								//				"reasoning\family\Complex (13)",
								//				among others
								if( createAndAssignResult.createdSpecificationItem != NULL &&
								hasUserSpecificationNonCompoundCollection &&
								relationWordItem != NULL &&
								!foundPrimarySpecificationItem->isOlderItem() &&
								foundPrimarySpecificationItem->isSelfGeneratedSpecification() &&
								foundPrimarySpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == NULL &&
								// Draw specification substitution conclusion or ask a question
								drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							}

						if( isCurrentExclusiveSpecification &&
						isGeneralizationProperNoun &&
						!isPossessiveUserSpecification &&
						currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
						existingSpecificationItem != NULL &&
						!existingSpecificationItem->isExclusiveSpecification() &&
						!existingSpecificationItem->isReplacedOrDeletedItem() &&
						existingSpecificationItem->isSelfGeneratedSpecification() &&
						!foundPrimarySpecificationItem->hasRelationWord() &&
						foundPrimarySpecificationItem->isSelfGeneratedSpecification() &&
						( definitionSpecificationItem = currentSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
						// Justification doesn't exist yet in specification
						!foundPrimarySpecificationItem->hasPrimaryAndSecondarySpecificationJustification( existingSpecificationItem, definitionSpecificationItem ) )
							{
							// Add specification substitution justification to found primary specification
							if( foundPrimarySpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, NULL, definitionSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the found primary specification" );
							}
						else
							{
							if( userSpecificationItem != NULL &&

								( ( existingSpecificationItem == NULL &&

									( hasCurrentSpecificationCollection ||
									// Test files: "reasoning\Scientific challenge",
									//				"reasoning\Knowledge of organisms (1)"
									!foundPrimarySpecificationItem->hasSpecificationCollection() ) ) ||

								( !isCurrentExclusiveSpecification &&
								isGeneralizationProperNoun ) ||

								// Typical for Spanish
								( isDefinitionSpanishAmbiguous &&
								hasCurrentSpecificationCollection &&

									( !isOlderCurrentSpecification ||
									// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Paz, José y Luisa"
									!isCurrentExclusiveSpecification ) ) ) )
								{
								// Non-exclusive
								if( ( !isExclusiveSpecification &&
								isCurrentExclusiveSpecification ) ||

								// Exclusive
								( isExclusiveSpecification &&
								isGeneralizationProperNoun &&
								hasCurrentSpecificationCollection &&
								isOlderCurrentSpecification ) )
									{
									// Ask a question or draw a negative conclusion
									if( isGeneralizationProperNoun &&
									hasCurrentSpecificationCollection &&
									userSpecificationWordItem != NULL &&

										// Adjective
										( ( isCurrentAdjectiveSpecification &&

											( ( !hasGeneralizationWordFeminineProperNounEnding &&
											!hasGeneralizationWordMasculineProperNounEnding &&
											isUserGeneralizationWord ) ||

											( !foundPrimarySpecificationItem->hasCompoundSpecificationCollection() &&

												( foundPrimarySpecificationItem->hasRelationWord() ||
												// Typical for Spanish
												isUserSpecificationWordSpanishAmbiguous ) ) ) ) ||

										// Non-adjective
										( !isCurrentAdjectiveSpecification &&

											( hasCurrentSpecificationCompoundCollection ||
											!foundPrimarySpecificationItem->hasCompoundSpecificationCollection() ||

											// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
											( isUserGeneralizationWord &&
											foundPrimarySpecificationItem->isCorrectedSpecification() ) ) ) ) )
										{
										positiveDefinitionSpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem );
										negativeDefinitionSpecificationItem = userSpecificationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem );

										// Question part doesn't exist yet
										if( ( ( questionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, currentSpecificationWordItem ) ) == NULL &&

											// No negative proper noun specification
											( ( ( negativeProperNounSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) == NULL &&

												( ( !isSpecificationWordSpanishAmbiguous &&

													// Non-adjective
													( ( !isCurrentAdjectiveSpecification &&
													!hasUserSpecificationRelationWord &&
													isUserGeneralizationWord &&
													negativeDefinitionSpecificationItem == NULL ) ||

													// Adjective
													( isCurrentAdjectiveSpecification &&

														( ( isUserRelationWord &&

															// Typical for Spanish
															// Spanish test file: "razonamiento\familia\Complejo (4)"
															( isUserSpecificationWordSpanishAmbiguous ||
															negativeDefinitionSpecificationItem == NULL ) ) ||

														( isUserGeneralizationWord &&

															( !hasUserSpecificationRelationWord ||
															// Typical for Spanish
															// Spanish test files: "respuestas a las preguntas\familia\Su pregunta es más específica (sobre Juan)",
															//						"razonamiento\familia\Paz es el hijo Juan y Ana"
															isUserSpecificationWordSpanishAmbiguous ||
															foundPrimarySpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) ) ) ) ||

													// Ask questions
													( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
													positiveDefinitionSpecificationItem == NULL &&
													foundPrimarySpecificationItem->isSelfGeneratedSpecification() &&
													!foundPrimarySpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) ||

													// Typical for Spanish
													( isSpanishCurrentLanguage &&

														// Spanish test file: "razonamiento\familia\Juan es el padre de Paz - José - Luisa - Bonita"
														( !isUserGeneralizationWord ||

														( !isUserAssignment &&

														( negativeDefinitionSpecificationItem == NULL ||
														foundPrimarySpecificationItem->hasPrimarySpecificationJustification( simpleUserSpecificationItem ) ) ) ) ) ) ) ||

												// Typical for Spanish
												( isSpecificationWordSpanishAmbiguous &&
												!foundPrimarySpecificationItem->isHiddenSpanishSpecification() &&

													( isUserGeneralizationWord ||
													// Spanish test file: "razonamiento\Reto científico"
													currentSpecificationItem == userSpecificationItem ) ) ) ) ||

											// Negative proper noun specification
											( negativeProperNounSpecificationItem != NULL &&
											hasUserSpecificationRelationWord &&
											isCurrentAdjectiveSpecification &&

												( ( isUserGeneralizationWord &&
												!hasGeneralizationWordCurrentlyConfirmedSpecification &&
												!isSpecificationWordSpanishAmbiguous &&

													// Test file: "reasoning\family\Paul - Joe - Laura - John and Anna (parents)"
													( ( positiveDefinitionSpecificationItem == NULL &&
													foundPrimarySpecificationItem->hasRelationWord() &&
													foundPrimarySpecificationItem->isOlderItem() ) ||

													foundPrimarySpecificationItem->hasPrimarySpecificationJustification( simpleUserSpecificationItem ) ) ) ||

												( negativeProperNounSpecificationItem->isOlderItem() &&

													( ( !isSpecificationWordSpanishAmbiguous &&

														( ( isUserRelationWord &&

															// Typical for Spanish
															// Spanish test file: "razonamiento\familia\Complejo (4)"
															( isUserSpecificationWordSpanishAmbiguous ||
															negativeDefinitionSpecificationItem == NULL ) ) ||

														( positiveDefinitionSpecificationItem == NULL &&
														!hasGeneralizationWordCurrentlyConfirmedSpecification &&
														foundPrimarySpecificationItem->isSelfGeneratedSpecification() &&
														!foundPrimarySpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) ) ) ||

													// Typical for Spanish
													( isSpecificationWordSpanishAmbiguous &&

														( ( isUserGeneralizationWord &&
														!foundPrimarySpecificationItem->isHiddenSpanishSpecification() ) ||

														// Spanish test files: "razonamiento\familia\Complejo (13)",
														//						"razonamiento\familia\Complejo (16)",
														//						among others
														( !isUserGeneralizationWord &&
														negativeDefinitionSpecificationItem != NULL &&
														foundPrimarySpecificationItem->hasRelationCollectionCurrentlyBeenUpdated() &&
														foundPrimarySpecificationItem->isOlderItem() ) ) ) ) ) ) ) ) ) ||

										// Question part exists
										( questionSpecificationItem != NULL &&

											( ( !isSpecificationWordSpanishAmbiguous &&

												// No positive definition
												( ( positiveDefinitionSpecificationItem == NULL &&
												!hasGeneralizationWordCurrentlyConfirmedSpecification &&

													( ( isCurrentAdjectiveSpecification ||

													// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
													( negativeDefinitionSpecificationItem == NULL &&
													questionSpecificationItem->isCorrectedSpecification() ) ) &&

													!questionSpecificationItem->hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ||

												// Positive definition
												( positiveDefinitionSpecificationItem != NULL &&

													// Non-adjective
													( ( !isCurrentAdjectiveSpecification &&
													negativeDefinitionSpecificationItem == NULL &&

														( ( isUserGeneralizationWord &&

															( ( questionSpecificationItem->isOlderItem() &&

																// Question to be adjusted
																( hasGeneralizationWordCurrentlyConfirmedSpecification ||

																( !hasUserSpecificationRelationWord &&
																// Justification doesn't exist yet in question
																!questionSpecificationItem->hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ) ||

															// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
															( questionSpecificationItem->isCorrectedSpecification() &&
															!questionSpecificationItem->hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ) ||

														// Typical for Spanish
														( isSpanishCurrentLanguage &&
														questionSpecificationItem->isOlderItem() &&

															// Spanish test file: "Juan es el padre de Paz - José - Luisa - Bonita"
															( !isUserGeneralizationWord ||
															!isUserAssignment ) ) ) ) ||

													// Adjective
													( isCurrentAdjectiveSpecification &&
													!hasGeneralizationWordCurrentlyConfirmedSpecification &&

														( ( ( isUserRelationWord ||

														( !hasUserSpecificationRelationWord &&
														isUserGeneralizationWord ) ) &&

															( questionSpecificationItem->isOlderItem() ||
															!questionSpecificationItem->hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ||

														( isUserGeneralizationWord &&

															( ( foundPrimarySpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
															!questionSpecificationItem->hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ||

															// Typical for Spanish
															// Spanish test files: "respuestas a las preguntas\familia\Su pregunta es más específica (sobre Juan)",
															//						"razonamiento\familia\Paz es el hijo Juan y Ana"
															( isUserSpecificationWordSpanishAmbiguous &&
															questionSpecificationItem->isOlderItem() ) ) ) ) ) ) ) ) ) ||

											// Typical for Spanish
											( isSpecificationWordSpanishAmbiguous &&

												// Not user generalization word
												( ( !isUserGeneralizationWord &&
												negativeDefinitionSpecificationItem != NULL &&
												negativeDefinitionSpecificationItem->hasNonCompoundSpecificationCollection() &&

													// Spanish test file: "José es hijo - José tiene 2 padres"
													// Non-adjective
													( ( !isCurrentAdjectiveSpecification &&
													positiveDefinitionSpecificationItem != NULL &&
													questionSpecificationItem->isOlderItem() ) ||

													// Adjective
													( isCurrentAdjectiveSpecification &&
													foundPrimarySpecificationItem->hasRelationCollectionCurrentlyBeenUpdated() ) ) ) ||

												// User generalization word
												( isUserGeneralizationWord &&
												isCurrentAdjectiveSpecification &&
												!foundPrimarySpecificationItem->isHiddenSpanishSpecification() ) ) ) ) ) )
											{
											selectedPrimarySpecificationItem = ( currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE &&
																				specificationWordItem->isUserSpecificationWord ? userSpecificationItem :
																																foundPrimarySpecificationItem );

											// Ask specification substitution question, or draw negative conclusion
											if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, selectedPrimarySpecificationItem, currentSpecificationItem, simpleUserSpecificationItem, userSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								else
									{
									// Make an assumption or draw a conclusion
									if( isGeneralizationProperNoun &&
									currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&

									( existingSpecificationItem != NULL ||
									!foundPrimarySpecificationItem->isCorrectedSpecification() ) )
										{
										alternativeSpanishAdditionalDefinitionSpecificationItem = NULL;
										primarySpecificationItem = NULL;

										if( isSpecificationWordSpanishAmbiguous )
											{
											if( currentSpecificationWordItem != specificationWordItem )
												primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );
											}
										else
											{
											if( ( primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL ||
											primarySpecificationItem->isUserAssignment() )
												primarySpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );
											}

										if( primarySpecificationItem != NULL )
											{
											alternativePrimarySpecificationItem = ( !isSpecificationWordSpanishAmbiguous ?
																					( hasRelationWord ? userSpecificationItem :
																										foundPrimarySpecificationItem ) :
																										alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) );

											if( ( existingSpecificationItem != NULL &&
											alternativePrimarySpecificationItem != NULL &&
											alternativePrimarySpecificationItem->specificationWordItem() == specificationWordItem &&

												( ( !isSpecificationWordSpanishAmbiguous &&
												!existingSpecificationItem->hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) ) ||

												// Typical for Spanish
												( isSpecificationWordSpanishAmbiguous &&

													( existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ||

													( isPossessiveUserSpecification &&
													primarySpecificationItem->hasRelationWord() &&
													generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, specificationWordItem ) != NULL ) ) ) ) ) ||

											// Typical for Spanish
											( isSpecificationWordSpanishAmbiguous &&
											primarySpecificationItem->isHiddenSpanishSpecification() ) )
												{
												// Typical for Spanish
												if( isSpecificationWordSpanishAmbiguous &&
												specificationWordItem != userSpecificationWordItem &&
												( commonWordItem = specificationWordItem->commonWordItem( primarySpecificationItem->specificationCollectionNr() ) ) != NULL &&
												( oppositeSpecificationWordItem = specificationWordItem->collectionWordItem( primarySpecificationItem->specificationCollectionNr() ) ) != NULL &&
												( alternativeSpanishAdditionalDefinitionSpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, oppositeSpecificationWordItem ) ) != NULL &&
												!isArchivedUserAssignment &&
												isUserRelationWord &&
												existingSpecificationItem != NULL )
													{
													if( ( primarySpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, userSpecificationCollectionNr, userSpecificationWordItem ) ) != NULL &&
													// Check if justification already exists
													existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
														{
														if( primarySpecificationItem == alternativePrimarySpecificationItem &&
														( tempJustificationItem = foundPrimarySpecificationItem->firstSecondaryJustificationItem( true ) ) != NULL )
															{
															primarySpecificationItem = tempJustificationItem->secondarySpecificationItem();

															if( existingSpecificationItem->isOlderItem() &&
															// Justification already exists yet in specification. Try to find a non-compound specification
															existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) &&
															( nonCompoundSpecificationCollectionNr = alternativeSpanishAdditionalDefinitionSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
															( oppositeSpecificationWordItem = alternativeSpanishAdditionalDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
															( spanishNonCompoundSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, nonCompoundSpecificationCollectionNr, oppositeSpecificationWordItem ) ) != NULL &&
															!spanishNonCompoundSpecificationItem->isHiddenSpanishSpecification() )
																primarySpecificationItem = spanishNonCompoundSpecificationItem;
															}
														}
													else
														{
														if( !existingSpecificationItem->hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&
														// Justification has at least the same assumption level
														existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, alternativePrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, currentSpecificationItem, NULL ) )
															primarySpecificationItem = alternativePrimarySpecificationItem;
														}
													}
												else
													primarySpecificationItem = alternativePrimarySpecificationItem;
												}
											}

										hasPrimarySpecificationRelationWord = ( primarySpecificationItem == NULL ? false : primarySpecificationItem->hasRelationWord() );
										isPossessivePrimarySpecification = ( primarySpecificationItem == NULL ? false : primarySpecificationItem->isPossessive() );
										archivedAssignmentItem = NULL;

										if( isArchivedUserAssignment &&
										isUserDefinitionSpecification )
											{
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										selectedPrimarySpecificationItem = ( ( !isPossessiveUserSpecification ||

																			( primarySpecificationItem != NULL &&
																			!isPossessivePrimarySpecification &&

																				( !primarySpecificationItem->isSelfGeneratedAssumption() ||
																				!primarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) ? primarySpecificationItem :
																																						// Typical for Spanish
																																						userSpecificationItem );
										selectedSecondarySpecificationItem = ( archivedAssignmentItem == NULL ? currentSpecificationItem :
																												archivedAssignmentItem );

										if( primarySpecificationItem != NULL &&
										selectedPrimarySpecificationItem != NULL &&
										// Justification doesn't exist yet
										generalizationWordItem->foundJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) == NULL &&

										// No existing specification
										( ( existingSpecificationItem == NULL &&

											( ( primarySpecificationItem == foundPrimarySpecificationItem &&

												( isCurrentUserSpecification ||
												isUserQuestion ||

												( ( !hasUserSpecificationRelationWord ||
												currentSpecificationCollectionNr == NO_COLLECTION_NR ) &&

													( isUserGeneralizationWord ||
													isUserRelationWord ||
													// Test files with archived assignments, and "reasoning\family\John - Anna (before family definition)"
													primarySpecificationItem->isOlderItem() ) ) ) ) ||

											// Typical for Spanish
											( isSpanishCurrentLanguage &&
											foundPrimarySpecificationItem->isHiddenSpanishSpecification() &&
											!isCurrentUserSpecification ) ) ) ||

										// Existing specification
										( existingSpecificationItem != NULL &&
										existingSpecificationItem->isSelfGeneratedSpecification() &&
										!selectedPrimarySpecificationItem->isActiveAssignment() &&
										// Justification doesn't exist yet in specification
										!existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) &&

											( ( ( ( hasPrimarySpecificationRelationWord &&
											isPossessivePrimarySpecification ) ||

											( isUserRelationWord &&

												// Test files: "reasoning\family\Complex (7 - Becky)",
												//				"reasoning\family\Complex (7 - Claudia)",
												//				"reasoning\family\Complex (16)",
												//				"reasoning\family\Complex (17)",
												//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
												( ( hasPrimarySpecificationRelationWord &&
												!existingSpecificationItem->hasSpecificationCollection() ) ||

												// Test files: "reasoning\family\Complex (17)",
												//				"reasoning\family\My assumptions that are confirmed (John and Anna)",
												//				"reasoning\family\My assumptions that are confirmed (John and Anna - reversed)"
												( primarySpecificationItem == foundPrimarySpecificationItem &&
												primarySpecificationItem->hasCurrentCreationSentenceNr() ) ) ) ||

											( !existingSpecificationItem->hasRelationWord() &&

												// No archived assignment
												( ( !isArchivedAssignment &&

													( ( hasPrimarySpecificationRelationWord &&
													!primarySpecificationItem->hasSpecificationCollection() ) ||

													( primarySpecificationItem == foundPrimarySpecificationItem &&

														( primarySpecificationItem->hasCurrentCreationSentenceNr() ||

														// Typical for Spanish
														// Spanish test file: "razonamiento\Reto científico"
														( isSpanishCurrentLanguage &&
														existingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ) ) ) ||

												// Archived assignment
												( isArchivedAssignment &&
												hasPrimarySpecificationRelationWord &&

													// Test file: "reasoning\Scientific challenge"
													( isUserDefinitionSpecification ||
													// Other test files with archived assignments
													!existingSpecificationItem->isOlderItem() ) ) ) ) ) &&

											// Justification has at least the same assumption level
											existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ||

											// Typical for Spanish
											( isSpanishCurrentLanguage &&
											!isPossessivePrimarySpecification &&
											primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
											// Justification doesn't exist yet in specification
											!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&

												// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es masculino"
												( isUserAdjectiveSpecification ||

												// Spanish test files: "razonamiento\familia\Complejo (12)",
												//						"razonamiento\familia\Complejo (15)"
												( isUserRelationWord &&
												primarySpecificationItem != selectedPrimarySpecificationItem ) ) ) ) ) ) )
											{
											selectedRelationWordItem = ( existingSpecificationItem != NULL &&
																		existingSpecificationItem->hasRelationWord() &&
																		existingSpecificationItem->hasCurrentCreationSentenceNr() ? relationWordItem :

																		// Test file: "reasoning\family\John - Anna (before family definition)"
																		( isExclusiveUserSpecification &&
																		!isSpanishCurrentLanguage &&
																		currentSpecificationCollectionNr > NO_COLLECTION_NR ? selectedPrimarySpecificationItem->relationWordItem() : NULL ) );

											selectedRelationWordTypeNr = ( selectedRelationWordItem == NULL ? NO_WORD_TYPE_NR : WORD_TYPE_PROPER_NOUN );

											// Make compound specification substitution assumption
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
												{
												// Test files: "reasoning\family\Complex (3)",
												//				"reasoning\family\Laura - Joe - John and Anna - Paul",
												//				"reasoning\family\Laura - Joe - Paul - John and Anna"
												if( !hasRelationWord &&
												existingSpecificationItem != NULL &&
												primarySpecificationItem->isUserSpecification() &&
												!createdOrFoundSpecificationItem->isOlderItem() &&
												( tempJustificationItem = createdOrFoundSpecificationItem->firstJustificationItem() ) != NULL &&
												tempJustificationItem->justificationAssumptionLevel() > createdOrFoundSpecificationItem->assumptionLevel() &&
												// Remove obsolete assumption justifications from currently created or found specification
												createdOrFoundSpecificationItem->removeObsoleteAssumptionJustifications( false ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove obsolete assumption justifications from the currently created or found specification" );

												// Typical for Spanish
												// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
												if( isSpanishCurrentLanguage &&
												hasUserSpecificationNonCompoundCollection &&
												hasUserSpecificationRelationWord &&
												!isPossessiveUserSpecification &&
												!isUserSpecificationWordSpanishAmbiguous &&
												simpleUserSpecificationItem != NULL &&
												( oppositeSpecificationWordItem = currentSpecificationWordItem->collectionWordItem( createdOrFoundSpecificationItem->specificationCollectionNr() ) ) != NULL &&
												( collectionWordItem = specificationWordItem->collectionWordItem( userSpecificationCollectionNr ) ) != NULL &&
												( selectedAdditionalDefinitionSpecificationItem = collectionWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) != NULL &&
												// Draw negative Spanish assumption
												adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, simpleUserSpecificationItem, selectedAdditionalDefinitionSpecificationItem, createdOrFoundSpecificationItem, NULL, generalizationWordItem, oppositeSpecificationWordItem, NULL ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative Spanish assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oppositeSpecificationWordItem->anyWordTypeString(), "\"" );
												}
											}

										// Typical for Spanish
										// Spanish test files: "respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)",
										//						"razonamiento\familia\Juan - Ana (antes de la definición de la familia)",
										//						"razonamiento\familia\Juan era el padre de Paz, José y Luisa",
										//						"razonamiento\familia\Tiempo pasado"
										if( isSpecificationWordSpanishAmbiguous &&
										isUserAssignment &&
										isUserGeneralizationWord &&
										hasUserSpecificationCompoundCollection &&
										primarySpecificationItem != NULL &&
										existingSpecificationItem != NULL &&
										existingSpecificationItem->hasCurrentCreationSentenceNr() &&

											( isArchivedUserAssignment ||
											currentSpecificationCollectionNr > NO_COLLECTION_NR ) &&

										// Draw specification substitution conclusion or ask a question
										drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
										}
									else
										{
										if( ( primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, (unsigned short)NO_QUESTION_PARAMETER, NO_COLLECTION_NR, specificationWordItem ) ) != NULL &&

										// Non-exclusive
										( ( !isExclusiveSpecification &&

											// Skip if new user specification has relation word, while the old one did not
											( !primarySpecificationItem->hasRelationWord() ||

											( ( existingSpecificationItem == NULL ||
											// Justification doesn't exist yet in specification
											!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, currentSpecificationItem ) ) &&

											generalizationWordItem->noRelationWordSpecificationItem( false, false, specificationWordItem ) == NULL ) ) &&

										!isFoundPrimaryHiddenSpanishSpecification &&
										// Skip if already exists as user specification
										generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, currentSpecificationWordItem ) == NULL ) ||

										// Exclusive
										( isExclusiveSpecification &&

											( !hasCurrentSpecificationCompoundCollection ||
											currentSpecificationWordItem->isFeminineOrMasculineWord() ) &&

										// Exclusive conclusion doesn't exist
										generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentSpecificationWordItem ) == NULL ) ) )
											{
											createdSpecificationItem = NULL;
											createdOrFoundSpecificationItem = existingSpecificationItem;

											if( existingSpecificationItem == NULL ||

											// Test files: "reasoning\family\Complex (15)",
											//				"reasoning\family\Complex (20)",
											//				"reasoning\family\I know (3)",
											//				among others
											( ( !primarySpecificationItem->isSelfGeneratedConclusion() ||
											generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, NULL ) == NULL ) &&

											// Justification doesn't exist yet in specification
											!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, currentSpecificationItem ) &&
											// Justification has at least the same assumption level
											existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, currentSpecificationItem, NULL ) ) )
												{
												// Draw compound specification substitution conclusion
												if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

												if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
													{
													if( createdOrFoundSpecificationItem->isCorrectedSpecification() )
														{
														// Test file: "reasoning\family\Correcting invalidated assumption (by knowledge)"
														// Correct invalid suggestive assumptions by knowledge
														if( correctSuggestiveAssumptionsByKnowledge( createdOrFoundSpecificationItem, generalizationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct invalid suggestive assumptions by knowledge about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL &&
														// Remove obsolete assumption justifications from created or found specification
														createdOrFoundSpecificationItem->removeObsoleteAssumptionJustifications( false ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove obsolete assumption justifications from the older created or found specification" );
														}
													}
												}

											if( isGeneralizationProperNoun &&
											createdOrFoundSpecificationItem != NULL )
												{
												// Adjective
												if( currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE )
													{
													if( ( ( negativeConclusionSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, (unsigned short)NO_QUESTION_PARAMETER, primarySpecificationItem->specificationCollectionNr(), NULL ) ) == NULL ||

													( !negativeConclusionSpecificationItem->hasCompoundSpecificationCollection() &&

														// Justification doesn't exist yet in specification
														( !negativeConclusionSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) ||

														// Typical for Spanish
														// Spanish test files: "respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)",
														//						"razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
														( isUserSpecificationWordSpanishAmbiguous &&
														isUserAssignment &&
														createdOrFoundSpecificationItem->isOlderItem() ) ) ) ||

													( isUserAssignment &&
													simpleUserSpecificationItem == primarySpecificationItem &&
													createdOrFoundSpecificationItem->isOlderItem() &&
													userSpecificationItem->hasRelationCollection() &&

														// Test file: "question answering\family\Just a few questions (1)"
														( createdOrFoundSpecificationItem->nJustificationItems() == 1 ||
														// Typical for Spanish
														// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
														userSpecificationCollectionNr == NO_COLLECTION_NR ) ) ) &&

													// Draw negative adjective conclusions
													drawNegativeAdjectiveConclusions( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative adjective conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( !isExclusiveUserSpecification &&
													!isUserAssignment &&
													userSpecificationCollectionNr > NO_COLLECTION_NR &&
													createdSpecificationItem != NULL &&

														// Test files: "reasoning\family\Complex (14)",
														//				"reasoning\family\I know (9)",
														//				"reasoning\family\I know (10)"
														( !hasUserSpecificationRelationWord ||
														// Typical for Spanish
														// Spanish test files: "razonamiento\familia\Complejo (2)",
														//						"razonamiento\familia\Complejo (12)",
														//						"razonamiento\familia\Complejo (15)"
														isUserSpecificationWordSpanishAmbiguous ) &&

													// Avoid idle call
													generalizationWordItem->firstSpecificationItem( false, false, isArchivedUserAssignment, true ) != NULL &&
													// Make indirectly answered question assumption
													makeIndirectlyAnsweredQuestionAssumption( isArchivedUserAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( ( !isPossessiveUserSpecification ||
												primarySpecificationItem->isSelfGeneratedAssumption() ) &&

												primarySpecificationItem->assumptionLevel() <= createdOrFoundSpecificationItem->assumptionLevel() )
													{
													// Avoid creating duplicate negative justifications
													if( ( ( selfGeneratedSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, NO_QUESTION_PARAMETER, userSpecificationCollectionNr, specificationWordItem ) ) == NULL ||
													!selfGeneratedSpecificationItem->isOlderItem() ) &&

													// Avoid idle call
													specificationWordItem->firstNegativeSpecificationItem() != NULL &&
													// Draw simple negative conclusions
													drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( hasUserSpecificationCompoundCollection &&
													isPossessiveUserSpecification &&
													existingSpecificationItem != NULL &&
													userSpecificationWordItem != NULL )
														{								// Efficiency: Select currentSpecificationItem is when specification words are the same
														definitionSpecificationItem = ( specificationWordItem == userSpecificationWordItem ? currentSpecificationItem :
																																			userSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, currentSpecificationWordItem ) );

														existingSpecificationItem = existingSpecificationItem->updatedSpecificationItem();

														// Justification doesn't exist yet in specification
														if( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) &&
														// Justification has at least the same assumption level
														existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, definitionSpecificationItem, NULL ) &&
														// Add specification substitution justification to existing specification
														existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, definitionSpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the existing specification" );
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextNonNegativeNonPosessiveDefinitionSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isChineseCurrentLanguage, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpanishCurrentLanguage, bool isUncountableGeneralizationNoun, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasExistingSpecificationCurrentCreationSentenceNr;
		bool hasExistingSpecificationRelationWord;
		bool hasFeminineOrMasculineProperNounEnding;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		bool hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification = false;
		bool hasRelationWordFeminineOrMasculineProperNounEnding = false;
		bool hasRelationWordMasculineProperNounEnding;
		bool hasSecondarySpecificationRelationWord;
		bool isDrawingPossessiveReversibleConclusions;
		bool isExistingHiddenSpanishSpecification;
		bool isExistingSelfGeneratedAssumption;
		bool isHiddenPrimarySpecification;
		bool isHiddenSecondarySpecification;
		bool isOlderExistingSpecification;
		bool isOlderPrimarySpecification;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isPrimarySpecificationAssignment;
		bool isPrimaryUserSpecification;
		bool isPrimarySelfGeneratedSpecification;
		bool isUserRelationWord;
		unsigned short prepositionParameter;
		unsigned short selectedJustificationTypeNr;
		unsigned short selectedRelationWordTypeNr;
		unsigned int specificationCollectionNr;
		JustificationItem *firstJustificationItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *compoundSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentExistingSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *possessiveSpecificationItem = NULL;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem = NULL;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selectedAdditionalProperNounSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *feminineSpecificationWordItem = NULL;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		oppositePossessiveDefinitionSpecificationItem_ = NULL;
		possessiveDefinitionSpecificationItem_ = NULL;

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, relationWordItem );

		// Typical for Spanish
		if( isSpanishCurrentLanguage &&
		hasRelationWord &&
		isPossessive &&
		primarySpecificationItem != NULL &&
		primarySpecificationItem->hasCompoundSpecificationCollection() &&
		primarySpecificationItem->isSelfGeneratedSpecification() &&
		specificationWordItem->isNounWordSpanishAmbiguous() &&
		( specificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
		( tempSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( isNegative, true, specificationCollectionNr, specificationWordItem ) ) != NULL &&
		tempSpecificationItem != primarySpecificationItem &&
		tempSpecificationItem->hasRelationWord() )
			// Spanish non-compound specification
			primarySpecificationItem = tempSpecificationItem;

		if( primarySpecificationItem != NULL )
			{
			hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isPrimarySelfGeneratedSpecification = primarySpecificationItem->isSelfGeneratedSpecification();
			isPrimarySpecificationAssignment = primarySpecificationItem->isAssignment();
			isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();

			if( ( !isOlderPrimarySpecification ||
			isPrimarySpecificationAssignment ||

			( isPrimarySelfGeneratedSpecification &&
			relationWordItem != NULL ) ) &&

			( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
				{
				hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
				hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
				hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem->hasCurrentlyMoreSpecificSpecification();
				hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem->hasMasculineProperNounEnding();
				isUserRelationWord = generalizationWordItem->isUserRelationWord;

				isHiddenPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();

				if( relationWordItem != NULL )
					{
					hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
					hasRelationWordFeminineOrMasculineProperNounEnding = relationWordItem->hasFeminineOrMasculineProperNounEnding();
					}

				// Do for all possessive noun words
				do	{
					if( currentPossessiveNounWordItem != specificationWordItem &&
					currentPossessiveNounWordItem != generalizationWordItem )
						{
						// Find possessive specifications
						if( ( findSpecificationResult = findPossessiveSpecifications( currentPossessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );

						additionalDefinitionSpecificationItem = NULL;
						createdOrFoundSpecificationItem = NULL;
						possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem;
						oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem;

						if( possessiveDefinitionSpecificationItem != NULL &&
						( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
							{
							possessiveDefinitionSpecificationItem_ = possessiveDefinitionSpecificationItem;
							oppositePossessiveDefinitionSpecificationItem_ = oppositePossessiveDefinitionSpecificationItem;

							if( !isPossessive &&
							( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
								{
								isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

								if( ( !primarySpecificationItem->hasRelationWord() &&
								generalizationWordItem->hadOnceAnsweredSelfGeneratedQuestion() ) ||

								( isPossessiveSpecificationWordSpanishAmbiguous &&
								primarySpecificationItem->specificationWordItem() == possessiveDefinitionSpecificationWordItem ) )
									{
									if( ( isArchivedAssignment ||
									primarySpecificationItem->isOlderItem() ||
									primarySpecificationItem->isSelfGeneratedConclusionOrAssumption() ) &&

									// Draw Spanish specification substitution conclusion
									drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( ( existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, possessiveDefinitionSpecificationWordItem ) ) == NULL ||

									( hasRelationWord &&

										// Confirmed specification without relation word
										( !existingUserSpecificationItem->hasRelationWord() ||
										// Test file: "reasoning\family\John - Anna (before family definition)"
										!existingUserSpecificationItem->hasSpecificationCollection() ) ) )
										{
										// Test files: "reasoning\family\I know (11)",
										//				"reasoning\family\Paul is a boy - Laura is a girl"
										if( !hasRelationWord &&
										( possessiveSpecificationWordItem = possessiveSpecificationItem->specificationWordItem() ) != NULL &&
										( commonWordItem = possessiveSpecificationWordItem->commonWordItem( possessiveSpecificationItem->specificationCollectionNr() ) ) != NULL &&
										commonWordItem != generalizationWordItem &&
										commonWordItem->isFeminineOrMasculineWord() &&
										!commonWordItem->isNounWordSpanishAmbiguous() &&
										( definitionSpecificationItem = commonWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, possessiveSpecificationWordItem ) ) != NULL &&
										// Make generalization assumption
										adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, definitionSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, commonWordItem, NULL ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", commonWordItem->anyWordTypeString(), "\"" );

										// User specification doesn't exist yet
										if( generalizationWordItem->firstUserSpecificationItem( !isNegative, isPossessive, NO_COLLECTION_NR, possessiveDefinitionSpecificationWordItem ) == NULL )
											{
											// Specification doesn't exist yet
											if( ( currentExistingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, isPossessive, false, possessiveDefinitionSpecificationWordItem ) ) == NULL ||

											( relationWordItem != NULL &&

												// No user relation word
												( ( !isUserRelationWord &&

													( ( isPrimarySpecificationAssignment &&
													!isOlderPrimarySpecification &&

														( currentExistingSpecificationItem->isSelfGeneratedAssumption() ||

														// Test file: "reasoning\family\Complex (8)"
														( !currentExistingSpecificationItem->hasRelationCollection() &&
														currentExistingSpecificationItem->isSelfGeneratedSpecification() &&
														currentExistingSpecificationItem->relationWordItem() == relationWordItem ) ) ) ||

													// Different relation word
													( currentExistingSpecificationItem->relationWordItem() != relationWordItem &&

														( !isPrimarySpecificationAssignment ||
														!currentExistingSpecificationItem->hasRelationCollection() ||
														relationWordItem->firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) == NULL ) ) ||

													// Typical for Spanish
													( isPossessiveSpecificationWordSpanishAmbiguous &&

														// Spanish test files: "razonamiento\familia\Complejo (7 - Bonita)",
														//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa"
														( hasGeneralizationWordCurrentlyConfirmedSpecification ||
														// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
														currentExistingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ) ) ||

												// User relation word
												( isUserRelationWord &&

													( ( tempSpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) ) == NULL ||

													// Test files: "reasoning\family\Complex (3)",
													//				"reasoning\family\Complex (7 - Becky)",
													//				"reasoning\family\Complex (7 - Claudia)",
													//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
													( isPrimarySpecificationAssignment &&
													tempSpecificationItem->isSelfGeneratedSpecification() ) ||

													// Test files: "reasoning\family\Complex (8)",
													//				"reasoning\family\I know (10)",
													//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
													( !tempSpecificationItem->hasCurrentCreationSentenceNr() &&
													!currentExistingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ) ) ) )
												{
												prepositionParameter = NO_PREPOSITION_PARAMETER;
												selectedRelationWordTypeNr = NO_WORD_TYPE_NR;
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

												if( hasRelationWord )
													{
													prepositionParameter = WORD_PARAMETER_PREPOSITION_OF;
													selectedRelationWordTypeNr = relationWordTypeNr;
													}

												if( ( selectedSecondarySpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) == NULL )
													{
													// Typical for Spanish
													// Make Spanish exclusive specification substitution assumption
													if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, prepositionParameter, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, possessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a Spanish exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

													createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
													}
												else
													{
													// Make specification substitution assumption
													if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, prepositionParameter, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

													if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
													// Justification doesn't exist yet in specification
													!createdOrFoundSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
													// Justification has at least the same assumption level
													createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
													// Add user specification substitution justification to created or found specification
													createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a user specification substitution justification to the created or found specification" );
													}

												// Typical for Spanish
												// Spanish test files: "Contestador inteligente de preguntas",
												//						"razonamiento\familia\Mis suposiciones que se confirman (hijos)",
												//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa",
												//						among others
												if( hasRelationWord &&
												isPrimaryUserSpecification &&
												existingUserSpecificationItem == NULL &&
												createdOrFoundSpecificationItem != NULL &&
												createdOrFoundSpecificationItem->hasCurrentCreationSentenceNr() &&
												createdOrFoundSpecificationItem->isOlderItem() &&
												createdOrFoundSpecificationItem->isHiddenSpanishSpecification() &&
												!primarySpecificationItem->isActiveAssignment() &&
												// Add user specification to justifications of hidden Spanish specification
												createdOrFoundSpecificationItem->addUserSpecificationToSpanishJustifications( primarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the user specification to justifications of the hidden Spanish specification" );
												}
											else
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

											if( createdOrFoundSpecificationItem != NULL )
												{
												secondarySpecificationItem = createdOrFoundSpecificationItem->updatedSpecificationItem();

												// Typical for Spanish
												// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
												if( isPossessiveSpecificationWordSpanishAmbiguous &&
												hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation &&
												// Draw simple negative conclusions
												drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, adminItem_->simpleUserSpecificationItem(), userSpecificationItem, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								}
							}
						else
							{
							// Test files: "reasoning\Boiling point",
							//				"reasoning\Condensation point"
							if( hasRelationWord &&
							( foundSpecificationItem = currentPossessiveNounWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&
							// Uncountable generalization noun. Example: "Water ..."
							adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationAssignment, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );
							}

						// First opposite possessive specification
						if( possessiveDefinitionSpecificationWordItem != NULL &&
						possessiveDefinitionSpecificationItem != NULL &&
						oppositePossessiveDefinitionSpecificationItem != NULL &&
						( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
							{
							if( createdOrFoundSpecificationItem == NULL ||
							primarySpecificationItem->hasSpecificationContext() )
								{
								secondarySpecificationItem = primarySpecificationItem;

								// Find additional definition definition
								if( !hasRelationWord ||
								isPossessive )
									additionalDefinitionSpecificationItem = possessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem );
								}

							if( !isHiddenPrimarySpecification &&
							secondarySpecificationItem != NULL )
								{
								hasSecondarySpecificationRelationWord = secondarySpecificationItem->hasRelationWord();

								// Test file: "reasoning\family\Context - Specification pronoun"
								if( !hasSecondarySpecificationRelationWord ||
								// Test files: "reasoning\family\Complex (19 - strange)",
								//				"reasoning\family\Complex (7 - Becky)"
								isPrimarySelfGeneratedSpecification ||

								( !isOlderPrimarySpecification &&

									( !secondarySpecificationItem->isUserAssignment() ||
									// Typical for Spanish
									// Spanish test files: "conflicto\familia\José es una madre",
									//						"respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)"
									secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) )
									{
									hasExistingSpecificationCurrentCreationSentenceNr = false;
									hasExistingSpecificationRelationWord = false;
									isExistingHiddenSpanishSpecification = false;
									isExistingSelfGeneratedAssumption = false;
									isOlderExistingSpecification = false;
									isHiddenSecondarySpecification = secondarySpecificationItem->isHiddenSpanishSpecification();
									existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									existingSpecificationItem != NULL &&
									!isHiddenSecondarySpecification &&
									hasGeneralizationWordCurrentlyConfirmedSpecification &&
									existingSpecificationItem->hasCompoundSpecificationCollection() &&
									( tempSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, oppositePossessiveDefinitionSpecificationItem->specificationCollectionNr(), oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
									tempSpecificationItem->hasOnlyOneRelationWord() )
										existingSpecificationItem = tempSpecificationItem;

									if( existingSpecificationItem != NULL )
										{
										// Assign some variables
										hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem->hasCurrentCreationSentenceNr();
										hasExistingSpecificationRelationWord = existingSpecificationItem->hasRelationWord();
										isExistingHiddenSpanishSpecification = existingSpecificationItem->isHiddenSpanishSpecification();
										isExistingSelfGeneratedAssumption = existingSpecificationItem->isSelfGeneratedAssumption();
										isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
										}

									hasRelationWordMasculineProperNounEnding = false;
									isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

									// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										primarySpecificationItem = primarySpecificationItem->updatedSpecificationItem();

									if( hasRelationWord &&
									isPossessive &&
									generalizationWordItem->hasMasculineProperNounEnding() &&
									( tempSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
										{
										// Masculine proper name ending
										// Spanish test files: "razonamiento\Reto científico",
										//						"razonamiento\familia\Juan - Ana - Pablo, José y Luisa"
										//						"razonamiento\familia\Sé (1)"
										hasRelationWordMasculineProperNounEnding = true;
										selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
										selectedPrimarySpecificationItem = primarySpecificationItem;
										selectedAdditionalDefinitionSpecificationItem = possessiveDefinitionSpecificationItem;
										selectedSecondarySpecificationItem = tempSpecificationItem;
										}
									else
										{
										selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
										selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem;
										selectedAdditionalDefinitionSpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? NULL : additionalDefinitionSpecificationItem );

										selectedSecondarySpecificationItem = ( isHiddenSecondarySpecification ? primarySpecificationItem :
																												secondarySpecificationItem );
										}

									if( existingSpecificationItem == NULL ||

									( hasSecondarySpecificationRelationWord &&

										// Non-possessive
										( ( !isPossessive &&

											// Not user relation
											( ( !isUserRelationWord &&

												( ( isExistingSelfGeneratedAssumption &&

													( ( hasExistingSpecificationRelationWord &&

														// Test file: "reasoning\family\Complex (6)"
														( ( !isOlderExistingSpecification &&
														existingSpecificationItem->hasOnlyOneRelationWord() ) ||

														( isPrimarySpecificationAssignment &&

															// Justification doesn't exist yet in specification
															// Test files: "reasoning\family\Joe has 2 parents - Joe is a child",
															//				"reasoning\family\Joe is a child - Joe has 2 parents",
															//				"reasoning\family\John - Anna (before family definition)"
															( ( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&

															existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ||

															// Typical for Spanish
															// Spanish test files: "razonamiento\familia\Luisa - José - Ana",
															//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
															isOppositePossessiveSpecificationWordSpanishAmbiguous ) ) ) ) ||

													// Test files: "reasoning\family\Complex (4)",
													//				"reasoning\family\Complex (12)",
													//				"reasoning\family\Complex (16)",
													//				"reasoning\family\Complex (18)",
													//				"reasoning\family\Complex (19 - mixed)",
													//				among others
													( isOlderExistingSpecification &&
													!isPrimarySpecificationAssignment &&
													existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ) ) ||

												( hasGeneralizationWordCurrentlyConfirmedSpecification &&

													// Not hidden existing
													( ( !isExistingHiddenSpanishSpecification &&

														// Test files: "reasoning\family\Complex (17)",
														//				"reasoning\family\Laura has a father - Laura is a child - John and Anna",
														//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
														( ( !hasExistingSpecificationCurrentCreationSentenceNr &&
														existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
														existingSpecificationItem->hasOnlyOneRelationWord() &&
														existingSpecificationItem->isSelfGeneratedConclusion() &&
														selectedSecondarySpecificationItem->hasRelationCollection() &&
														( firstJustificationItem = selectedSecondarySpecificationItem->firstJustificationItem() ) != NULL &&
														firstJustificationItem->isReversibleAssumptionOrConclusion() ) ||

														// Typical for Spanish
														( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

															// Not primary assignment
															( ( !isPrimarySpecificationAssignment &&

																// Spanish test files: "razonamiento\familia\Juan - Luisa - Juan y Ana",
																//						"razonamiento\familia\Juan - Paz - Luisa"
																( secondarySpecificationItem->hasOnlyOneRelationWord() ||

																// Spanish test files: "razonamiento\familia\Complejo (6)",
																//						"razonamiento\familia\Complejo (11)",
																//						"razonamiento\familia\Complejo (20)",
																//						among others
																( existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
																existingSpecificationItem->hasOnlyOneRelationWord() ) ) ) ||

															// Primary assignment
															( isPrimarySpecificationAssignment &&

																// Spanish test files: "razonamiento\familia\Complejo (9)",
																//						"razonamiento\familia\Complejo (16)",
																//						among others
																( !isHiddenSecondarySpecification ||
																// Spanish test files: "razonamiento\familia\Complejo (9)",
																//						"razonamiento\familia\Complejo (20)",
																//						among others
																secondarySpecificationItem->nRelationWords() < primarySpecificationItem->nRelationWords() ) ) ) ) ) ) ||

													// Hidden existing
													// Spanish test files: "razonamiento\familia\Complejo (14)",
													//						"razonamiento\familia\Sé (11)",
													//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz",
													//						"razonamiento\familia\Paz - Ana - José - Luisa - Juan"
													( isExistingHiddenSpanishSpecification &&
													!hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
													existingSpecificationItem->hasOnlyOneRelationWord() ) ) ) ) ) ||

												// User relation
												( isUserRelationWord &&
												// Typical for Spanish
												isOppositePossessiveSpecificationWordSpanishAmbiguous &&
												isExistingSelfGeneratedAssumption &&

													// Typical for Spanish
													// Spanish test files: "razonamiento\familia\Complejo (3)",
													//						"razonamiento\familia\Complejo (12)"
													( isExistingHiddenSpanishSpecification ||

													// Spanish test files: "razonamiento\familia\Complejo (7 - Becky)",
													//						"razonamiento\familia\Complejo (7 - Bonita)",
													//						"razonamiento\familia\Definición sencilla de la familia - Complejo (7 - Bonita)"
													isPrimarySpecificationAssignment ) ) ) ) ||

										// Possessive
										( isPossessive &&
										isExistingSelfGeneratedAssumption &&

											// No additional definition
											// Test file: "reasoning\family\John is a parent - Paul is a child of John and Anna"
											( ( additionalDefinitionSpecificationItem == NULL &&

												// Not older existing specification
												( ( !isOlderExistingSpecification &&

													// Test files: "reasoning\family\John is a parent - Paul is a child of John and Anna",
													//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
													( !hasRelationWordFeminineOrMasculineProperNounEnding ||
													// Test files: "reasoning\family\Complex (11)",
													//				"reasoning\family\Complex (17)",
													//				"reasoning\family\Complex (18)",
													//				"reasoning\family\Complex (19 - mixed)",
													//				"reasoning\family\Complex (19 - strange)",
													//				"reasoning\family\I know (12)",
													//				among others
													generalizationWordItem->firstActiveQuestionSpecificationItem() != NULL ||

													// Test file: "reasoning\family\Complex (17)"
													( ( tempSpecificationItem = generalizationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL &&
													tempSpecificationItem->isOlderItem() ) ) ) ||

												// Older existing specification
												( isOlderExistingSpecification &&
												// Test files: "reasoning\family\Complex (4)",
												//				"reasoning\family\Complex (5)",
												//				"reasoning\family\Complex (16)",
												//				"reasoning\family\Complex (19 - strange)",
												//				"reasoning\family\I know (3)",
												//				"reasoning\family\You have confirmed at least one relation word",
												//				among others
												!hasRelationWordConfirmedSpecification &&
												!secondarySpecificationItem->isSelfGeneratedConclusion() ) ) ) ||

											// Additional definition
											( additionalDefinitionSpecificationItem != NULL &&
											// Justification doesn't exist yet in specification
											!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) ) ) &&

										existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ) ) )
										{
										selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR );
										createdSpecificationItem = existingSpecificationItem;
										foundSpecificationItem = existingSpecificationItem;

										if( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&

											// Test files: "reasoning\family\Context - Specification pronoun",
											//				"reasoning\family\Laura - John and Anna - Joe"
											( !hasRelationWord ||

											// Non-possessive
											( !isPossessive &&
											selectedPrimarySpecificationItem->specificationWordItem() == selectedSecondarySpecificationItem->specificationWordItem() &&

												( !hasGeneralizationWordCurrentlyConfirmedSpecification ||

												// Test files: "reasoning\family\Complex (12)",
												//				"reasoning\family\Complex (16)",
												//				"reasoning\family\Complex (17)",
												//				among others
												( existingSpecificationItem != NULL &&
												( firstJustificationItem = existingSpecificationItem->firstJustificationItem() ) != NULL &&
												firstJustificationItem->hasPrimarySpecificationRelationCollection() ) ) ) ||

											// Possessive
											( isPossessive &&

												( hasExistingSpecificationCurrentCreationSentenceNr ||
												!hasGeneralizationWordCurrentlyConfirmedSpecification ) ) ) ) ||

										// Typical for Spanish
										( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

											// Spanish test file: "razonamiento\familia\Complejo (3)"
											( !isPossessive ||
											// Spanish test files: "razonamiento\Reto científico",
											//						"razonamiento\familia\Sé (1)",
											//						"respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es ..."
											existingSpecificationItem == NULL ||

											( hasExistingSpecificationCurrentCreationSentenceNr &&

												// Spanish test file: "razonamiento\familia\Complejo (3)"
												( isPrimaryUserSpecification ||
												// Spanish test file: "Ana - Juan - Paz, José y Luisa"
												!selectedSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
												// Spanish test file: "Ana - Juan - Paz, José y Luisa"
												!existingSpecificationItem->hasOnlyOneRelationWord() ||
												globalVariables_->nUserGeneralizationWords > 1 ) ) ) ) )
											{
											// Make opposite possessive specification assumption or specification substitution assumption
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, possessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption or a specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
											foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
											}

										if( relationWordItem != NULL )
											{
											// Non-possessive
											if( !isPossessive )
												{
												if( existingSpecificationItem != NULL )
													{
													// Not created specification
													if( createdSpecificationItem == NULL )
														{
														if( existingSpecificationItem->isReplacedItem() )
															{
															// Replaced. So, update required
															existingSpecificationItem = existingSpecificationItem->updatedSpecificationItem();

															if( !isSpanishCurrentLanguage &&
															( tempSpecificationItem = relationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
															tempSpecificationItem->isSelfGeneratedConclusion() )
																{
																// Test files: "reasoning\family\Complex (12)",
																//				"reasoning\family\Complex (16)"
																// Draw reversible conclusion
																if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, existingSpecificationItem, NULL, NULL, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															{
															// Test files: "reasoning\family\Joe has 2 parents - Joe is a child",
															//				"reasoning\family\Joe is a child - Joe has 2 parents"
															if( hasExistingSpecificationCurrentCreationSentenceNr &&
															isPrimarySpecificationAssignment &&
															!selectedSecondarySpecificationItem->isOlderItem() &&
															!selectedSecondarySpecificationItem->isUserSpecification() &&
															( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != NULL &&
															// Justification doesn't exist yet in specification
															!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, additionalSpecificationItem ) &&
															// Add additional opposite possessive conclusion justification to existing specification
															existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, additionalSpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an additional opposite possessive conclusion justification to the existing specification" );
															}
														}
													else	// Created specification
														{
														// Test file: "reasoning\family\Complex (19 - mixed)"
														if( !hasExistingSpecificationRelationWord &&
														!isOppositePossessiveSpecificationWordSpanishAmbiguous &&
														( foundSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( false, false, (unsigned int)NO_COLLECTION_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
														// Draw reversible conclusion
														adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													}

												if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
												( selectedPrimarySpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL )
													{
													// Test file: "reasoning\family\John - Anna (before family definition)"
													if( ( existingSpecificationItem == NULL ||
													selectedPrimarySpecificationItem->assumptionLevel() < existingSpecificationItem->assumptionLevel() ) &&

													( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPartOfSpecificationItem( possessiveDefinitionSpecificationWordItem ) ) != NULL &&
													// Justification doesn't exist yet in part-of specification
													( ( partOfSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentPossessiveNounWordItem ) ) != NULL &&
													!partOfSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, partOfDefinitionSpecificationItem ) &&
													// Justification has at least the same assumption level
													partOfSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) ) &&
													// Add part-of justification to part-of specification
													partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, NULL, partOfDefinitionSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a part-of justification to a part-of specification" );

													// Test file: "reasoning\family\Complex (4)"
													// Existing specification
													if( hasGeneralizationWordCurrentlyConfirmedSpecification &&
													!isPrimarySpecificationAssignment &&
													primarySpecificationItem->hasRelationCollectionCurrentlyBeenUpdated() &&
													existingSpecificationItem != NULL &&
													existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != NULL &&
													// Justification doesn't exist yet in specification
													!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, selectedPrimarySpecificationItem ) &&
													// Justification has at least the same assumption level
													existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, selectedPrimarySpecificationItem, NULL ) &&
													// Make opposite possessive specification assumption
													adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationItem, NULL, selectedPrimarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else	// Possessive
												{
												if( !isOppositePossessiveSpecificationWordSpanishAmbiguous )
													{
													if( foundSpecificationItem != NULL )
														{
														if( additionalDefinitionSpecificationItem == NULL )
															{
															// No additional definition
															if( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
															hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation ) &&

															( additionalSpecificationItem = relationWordItem->bestMatchingRelationSpecificationItem( false, isArchivedAssignment, false, true, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
															// Add opposite possessive conclusion justification to additional specification
															additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the additional specification" );
															}
														else	// Additional definition
															{
															if( ( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, NULL ) ) != NULL &&
															( selectedSecondarySpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
															// Add specification substitution justification to additional specification
															additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );
															}
														}
													}
												else	// Typical for Spanish
													{
													if( isPrimarySelfGeneratedSpecification &&
													( additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
													( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, true, false, true, additionalDefinitionSpecificationItem->specificationCollectionNr(), specificationWordItem ) ) != NULL )
														{
														hasFeminineOrMasculineProperNounEnding = false;

														if( isFeminineOrMasculineProperNounEnding( false, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) )
															{
															hasFeminineOrMasculineProperNounEnding = true;
															
															if( !hasGeneralizationWordMasculineProperNounEnding &&
															existingSpecificationItem != NULL &&
															existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) == NULL &&
															( feminineSpecificationWordItem = oppositePossessiveDefinitionSpecificationWordItem->collectionWordItem( additionalDefinitionSpecificationItem->specificationCollectionNr() ) ) != NULL &&
															// Specification doesn't exist yet
															( foundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) != NULL &&
															// Justification doesn't exist yet in specification
															!foundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, additionalDefinitionSpecificationItem ) &&
															// Justification has at least the same assumption level
															foundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem, oppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, NULL ) &&
															// Make exclusive Spanish feminine specification substitution assumption
															adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, oppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive Spanish feminine specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
															}

														if( compoundSpecificationItem != primarySpecificationItem )
															{
															if( hasFeminineOrMasculineProperNounEnding )
																{
																selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
																selectedPrimarySpecificationItem = compoundSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem;
																selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;
																}
															else
																{
																selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
																selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = NULL;
																selectedSecondarySpecificationItem = compoundSpecificationItem;
																}

															// Make compound specification exclusive specification substitution assumption (opposite)
															if( adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, false, false, false, false, false, isNegative, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

															if( feminineSpecificationWordItem != NULL &&
															adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, isNegative, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : oppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? oppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNounEnding ? additionalDefinitionSpecificationItem : compoundSpecificationItem ), NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a Spanish feminine compound exclusive specification substitution assumption that is Spanish ambiguous in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );

				// No conflict found
				if( !globalVariables_->hasDisplayedWarning &&
				primarySpecificationItem != NULL &&
				secondarySpecificationItem != NULL &&
				relationWordItem != NULL )
					{
					// Not user relation word
					if( ( !isUserRelationWord &&
					!userSpecificationItem->isOlderItem() &&

						( ( primarySpecificationItem == userSpecificationItem &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							isPossessive ) ) ||

						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)",
						//						"razonamiento\familia\Que haya confirmado al menos una palabra de relación"
						( isSpanishCurrentLanguage &&
						!isPossessive ) ) ) ||

					// User relation word
					( isUserRelationWord &&

						// Non-possessive
						( ( !isPossessive &&
						primarySpecificationItem != secondarySpecificationItem ) ||

						// Possessive
						// Test files: "reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)"
						( isPossessive &&
						hasPrimarySpecificationCompoundCollection &&
						!hasRelationWordConfirmedSpecification &&
						!isOlderPrimarySpecification &&
						!userSpecificationItem->isAssignment() ) ) ) )
						{											// Non-possessive
						isDrawingPossessiveReversibleConclusions = ( ( !isPossessive &&
																	hasPrimarySpecificationCompoundCollection &&
																	!hasRelationWordConfirmedSpecification &&
																	isUserRelationWord ) ||

																	// Possessive
																	( isPossessive &&
																	!isUserRelationWord &&

																		// Test files: "reasoning\family\Complex (2)",
																		//				"reasoning\family\Complex (3)",
																		//				"reasoning\family\Complex (16)",
																		//				among others
																		( hasGeneralizationWordCurrentlyConfirmedSpecification ||
																		// Test file: "reasoning\family\You have confirmed at least one relation word"
																		primarySpecificationItem->hasOnlyOneRelationWord() ||
																		// Typical for Spanish
																		// Spanish test file: "razonamiento\familia\Paz - José - Luisa - Juan y Ana (padres)"
																		isSpanishCurrentLanguage ) ) );

						// Draw reversible conclusions
						if( drawReversibleConclusions( isArchivedAssignment, isChineseCurrentLanguage, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", relationWordItem->anyWordTypeString(), "\"" );
						}

					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					primarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&
					primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
					( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, specificationWordItem->compoundCollectionNr(), primarySpecificationItem->relationCollectionNr(), specificationWordItem, primarySpecificationItem->relationWordItem() ) ) != NULL )
						primarySpecificationItem = compoundSpecificationItem;

					selectedAdditionalProperNounSpecificationItem = ( isHiddenPrimarySpecification ? userSpecificationItem :
																									primarySpecificationItem );

					if( ( ( ( primarySpecificationItem == userSpecificationItem ||

					( isUserRelationWord &&

						( !isPossessive ||

						// Test files: "reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)"
						( primarySpecificationItem->isOlderItem() &&
						primarySpecificationItem->isSelfGeneratedAssumption() ) ) ) ) &&

					!secondarySpecificationItem->hasCompoundSpecificationCollection() ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&

						( isUserRelationWord ||
						primarySpecificationItem == userSpecificationItem ) ) ) &&

					// Make an opposite possessive specification assumption (based on secondary specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, isSpanishCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, selectedAdditionalProperNounSpecificationItem, userSpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on secondary specification) during the making an exclusive specification substitution assumption about word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

					if( hasPrimarySpecificationCompoundCollection &&
					existingSpecificationItem != NULL &&
					primarySpecificationItem == secondarySpecificationItem &&

						( ( !isSpanishCurrentLanguage &&
						isPossessive &&
						existingSpecificationItem->isOlderItem() &&

							// No user relation word
							// Test file: "reasoning\family\I know (10)"
							( ( !isUserRelationWord &&
							existingSpecificationItem->isSelfGeneratedAssumption() ) ||

							// User relation word
							// Test files: "reasoning\family\Complex (7 - Becky)",
							//				"reasoning\family\Joe has 2 parents - Joe is a child",
							//				"reasoning\family\Joe is a child - Joe has 2 parents",
							//				"reasoning\family\Paul - Joe - Laura - John and Anna (parents)"
							( isUserRelationWord &&
							!existingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ) ||

						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Complejo (5)",
						//						"razonamiento\familia\Complejo (6)",
						//						"razonamiento\familia\Complejo (11)",
						//						"razonamiento\familia\Complejo (19 - mezclado)"
						( isSpanishCurrentLanguage &&
						!isPossessive &&
						!existingSpecificationItem->isOlderItem() &&
						existingSpecificationItem->isHiddenSpanishSpecification() ) ) &&

					// Make an opposite possessive specification assumption (based on existing specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, isSpanishCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, existingSpecificationItem, ( isHiddenPrimarySpecification ? userSpecificationItem : primarySpecificationItem ), userSpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on existing specification) during the making an exclusive specification substitution assumption about word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char makeGeneralizationAssumption( bool isArchivedAssignment, bool isChineseCurrentLanguage, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isFeminineOrMasculineGeneralizationWord;
		bool isUserGeneralizationWord;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeGeneralizationAssumption";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem == specificationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item and specification word item are the same" );

		// Get first noun word specification generalization
		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem->isFeminineOrMasculineWord();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				// Specification doesn't exist yet
				generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem ) == NULL &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != NULL &&
				primarySpecificationItem->isExclusiveSpecification() )
					{
					// Test file: "reasoning\Knowledge of organisms (1)"
					if( ( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||

					( !currentGeneralizationWordItem->hasCollection() &&

						( !isFeminineOrMasculineGeneralizationWord ||
						primarySpecificationItem->isSelfGeneratedSpecification() ) ) ||

					// Typical for Chinese: No plural form
					// Test file: "reasoning\Knowledge of organisms (1)"
					( isChineseCurrentLanguage &&
					primarySpecificationItem->hasCompoundSpecificationCollection() &&

						( ( foundSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) == NULL ||
						foundSpecificationItem->hasSpecificationCollection() ) ) ) )
						{
						secondarySpecificationItem = NULL;

						if( !isFeminineOrMasculineGeneralizationWord &&
						isUserGeneralizationWord &&
						userSpecificationItem->specificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() )
							// Test files: "reasoning\Knowledge of organisms (1)",
							//				"reasoning\family\Simple family definition - John is the mother of Paul",
							//				"reasoning\family\Simple family definition - John is a father"
							secondarySpecificationItem = userSpecificationItem;
						else
							{
							if( ( foundSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( isArchivedAssignment, false, false, specificationWordItem ) ) != NULL )
								secondarySpecificationItem = foundSpecificationItem;
							}

						if( secondarySpecificationItem != NULL &&
						// Make generalization assumption
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeIndirectlyAnsweredQuestionAssumption( bool isArchivedAssignment, bool isSpanishCurrentLanguage, unsigned short generalizationWordTypeNr, SpecificationItem *adjectiveSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned short specificationWordTypeNr;
		unsigned int currentSpecificationCollectionNr;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *relatedSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( adjectiveSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given adjective specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( !adjectiveSpecificationItem->isAdjectiveSpecification() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given adjective specification has no adjective specification" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, isArchivedAssignment, true ) ) != NULL )
			{
			do	{
				if( !currentQuestionSpecificationItem->hasCurrentCreationSentenceNr() &&
				( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
				currentQuestionSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) != NULL )
					{
					specificationWordTypeNr = currentQuestionSpecificationItem->specificationWordTypeNr();
					currentSpecificationCollectionNr = currentQuestionSpecificationItem->specificationCollectionNr();
					createdOrFoundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem );

					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem->findRelatedSpecification( true, isArchivedAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), specificationWordTypeNr, currentSpecificationCollectionNr, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
					relatedSpecificationItem != currentQuestionSpecificationItem &&
					( relatedSpecificationWordItem = relatedSpecificationItem->specificationWordItem() ) != NULL &&
					( primarySpecificationItem = generalizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( isArchivedAssignment ) ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( additionalDefinitionSpecificationItem = primarySpecificationWordItem->firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem ) ) != NULL &&
					( secondarySpecificationItem = relatedSpecificationWordItem->firstAdjectiveSpecificationItem( false, false ) ) != NULL )
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage )
							{
							// Draw Spanish specification substitution conclusion or ask a question
							if( drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isSpanishCurrentLanguage, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, adminItem_->userSpecificationItem(), generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							// Test file: "Indirectly answered questions - John is masculine"
							// Make indirectly answered question assumption
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, adjectiveSpecificationItem, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );

							createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
							}
						}

					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					createdOrFoundSpecificationItem != NULL &&
					currentQuestionSpecificationItem->isExclusiveSpecification() )
						{
						// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es masculino"
						// Draw simple Spanish negative conclusions
						if( drawSimpleNegativeConclusions( isArchivedAssignment, true, createdOrFoundSpecificationItem, adjectiveSpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative Spanish conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						// Draw Spanish proper noun part-of conclusions in proper noun words
						if( drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, false, true, currentQuestionSpecificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );

						// Find answers to questions
						// Missed a question question to answer
						if( generalizationWordItem->findAnswersToQuestions( currentSpecificationCollectionNr, createdOrFoundSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find answers to questions in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentQuestionSpecificationItem = ( currentQuestionSpecificationItem->isReplacedOrDeletedItem() ?
														// First question specification
														adminItem_->firstSpecificationItem( false, false, isArchivedAssignment, true ) :
														// Next question specification
														currentQuestionSpecificationItem->nextSelectedSpecificationItem() ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeOnlyOptionLeftAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned short nOptionsLeft = 0;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *onlyOptionLeftSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeOnlyOptionLeftAssumption";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !userSpecificationItem->hasSpecificationCollection() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification collection" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != NULL )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
				generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, currentQuestionSpecificationWordItem ) == NULL )
					{
					nOptionsLeft++;
					onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
					}
				}
			while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

			if( nOptionsLeft == 1 &&
			onlyOptionLeftSpecificationWordItem != NULL &&
			// Get first generalization specification noun word
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != NULL &&
					( additionalDefinitionSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != NULL &&
					// Make an only option left assumption
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( specificationCollectionNr > NO_COLLECTION_NR &&
		// Check if a related specification exists
		( relatedResult = generalizationWordItem->findRelatedSpecification( false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the secondary specification" );

		if( relatedResult.relatedSpecificationItem == NULL )
			{
			if( generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() )
				{
				// Correct suggestive assumptions by opposite suggestive question
				if( correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( isArchivedAssignment, isNegative, isPossessive, primarySpecificationItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			else
				{
				// Make suggestive question assumption
				if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	CompoundResultType drawDefinitionSpecificationSubstitutionConclusion( bool isChineseCurrentLanguage, bool isSpanishCurrentLanguage, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *specificationWordItem )
		{
		bool hasNonCompoundUserSpecificationCollection;
		bool hasUserGeneralizationCollection;
		bool hasUserSpecificationCollection;
		bool isArchivedUserAssignment;
		bool isExclusiveFoundSpecification;
		bool isExclusiveUserSpecification;
		bool isFoundArchivedAssignment;
		unsigned short generalizationWordTypeNr;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *secondaryDefinitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *relatedSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CompoundResultType compoundResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawDefinitionSpecificationSubstitutionConclusion";

		if( userSpecificationItem == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
			{
			hasNonCompoundUserSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			hasUserGeneralizationCollection = userSpecificationItem->hasGeneralizationCollection();
			hasUserSpecificationCollection = userSpecificationItem->hasSpecificationCollection();
			isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_COLLECTION_NR, NO_COLLECTION_NR, specificationWordItem, NULL ) ) != NULL )
					{
					if( foundSpecificationItem->isPartOf() )
						{
						if( compoundResult.compoundGeneralizationWordItem == NULL )
							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
						}
					else
						{
						isFoundArchivedAssignment = foundSpecificationItem->isArchivedAssignment();
						isExclusiveFoundSpecification = foundSpecificationItem->isExclusiveSpecification();
						generalizationWordTypeNr = foundSpecificationItem->generalizationWordTypeNr();

						// Definition
						if( isExclusiveFoundSpecification &&
						generalizationWordTypeNr != WORD_TYPE_PROPER_NOUN &&

							( !isSpanishCurrentLanguage ||
							// Typical for Spanish
							foundSpecificationItem->hasCompoundSpecificationCollection() ) &&

						( secondaryDefinitionSpecificationItem = specificationWordItem->firstNonCollectedSpecificationItem( true, currentGeneralizationWordItem ) ) != NULL &&
						( secondarySpecificationWordItem = secondaryDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
						// Make specification substitution definition assumption
						adminItem_->addSelfGeneratedSpecification( false, isFoundArchivedAssignment, isFoundArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, secondaryDefinitionSpecificationItem, NULL, currentGeneralizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make a specification substitution definition assumption about word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

						if( ( hasUserSpecificationCollection ||

						( isExclusiveUserSpecification &&
						isExclusiveFoundSpecification ) ) &&

						// Avoid idle call
						specificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() != NULL &&
						// Draw compound specification substitution conclusion
						drawCompoundSpecificationSubstitutionConclusion( isFoundArchivedAssignment, isExclusiveFoundSpecification, isSpanishCurrentLanguage, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, foundSpecificationItem, userSpecificationItem, currentGeneralizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( foundSpecificationItem->isOlderItem() &&

							// Non-exclusive (found specification)
							( ( !isExclusiveFoundSpecification &&

								// Test file: "Knowledge of organisms (2)"
								( hasUserGeneralizationCollection ||
								// Test file: "reasoning\Scientific challenge"
								hasUserSpecificationCollection ||
								// Test file: "Uncertainty and past tense"
								isArchivedUserAssignment ||
								// Test files: "reasoning\James was a father - James was the father of Peter",
								//				"reasoning\Scientific challenge"
								isFoundArchivedAssignment ||
								// Typical for Spanish
								// Spanish test file: "razonamiento\Reto científico"
								isSpanishCurrentLanguage ) ) ||

							// Exclusive (found specification)
							( isExclusiveFoundSpecification &&

								// Non-exclusive (user specification)
								// Test file: "reasoning\Scientific challenge"
								( ( !isExclusiveUserSpecification &&
								hasUserSpecificationCollection &&
								generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN ) ||

								// Exclusive (user specification)
								// Test file: "reasoning\family\John has 2 sons and a daughter (before family definition)"
								( isExclusiveUserSpecification &&
								foundSpecificationItem->hasNonCompoundSpecificationCollection() &&
								userSpecificationItem->hasCompoundSpecificationCollection() ) ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&

									( hasNonCompoundUserSpecificationCollection ||

									//  User specification collection
									( hasUserSpecificationCollection &&
									currentGeneralizationWordItem == specificationWordItem ) ) ) ) ) ) &&

						// Draw specification substitution conclusion or ask a question
						drawSpecificationSubstitutionConclusionOrAskQuestion( isFoundArchivedAssignment, isExclusiveFoundSpecification, isSpanishCurrentLanguage, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, currentGeneralizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( isExclusiveFoundSpecification &&
						foundSpecificationItem->isOlderItem() )
							{
							if( hasNonCompoundUserSpecificationCollection &&
							foundSpecificationItem->hasNonCompoundSpecificationCollection() )
								{
								if( ( relatedResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
									return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );

								if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
								( relatedSpecificationWordItem = relatedSpecificationItem->specificationWordItem() ) != NULL )
									{
									// Avoid idle call
									if( relatedSpecificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() != NULL &&
									// Draw specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), isSpanishCurrentLanguage, relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, currentGeneralizationWordItem, relatedSpecificationWordItem, NULL ) != RESULT_OK )
										return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to draw a related specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

									// Make related generalization assumption
									if( makeGeneralizationAssumption( isFoundArchivedAssignment, isChineseCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, relatedSpecificationWordItem, userSpecificationWordItem->collectionWordItem( userSpecificationItem->specificationCollectionNr() ) ) != RESULT_OK )
										return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make a generalization assumption in specification word \"", relatedSpecificationWordItem->anyWordTypeString(), "\"" );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									// Make Spanish generalization assumption
									makeGeneralizationAssumption( isFoundArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, specificationWordItem, userSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make a Spanish generalization assumption in specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return compoundResult;
		}
	};

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
