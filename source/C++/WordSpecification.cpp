/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
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

#include "CollectionResultType.cpp"
#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "SpecificationList.cpp"
#include "WordTypeItem.cpp"

class WordSpecification
	{
	friend class WordItem;

	// Private constructed variables

	bool hasConfirmedSpecification_;
	bool hasCorrectedAssumptionByKnowledge_;
	bool hasCorrectedAssumptionByOppositeQuestion_;
	bool hasDisplayedMoreSpecificRelatedQuestion_;
	bool isConfirmedExclusive_;
	bool isMyWordTouchedDuringCurrentSentence_;
	bool isNonExclusiveSpecification_;
	bool isOnlyCheckingForConflicts_;
	bool isSameQuestionFromUser_;
	bool isSimilarOrRelatedQuestion_;

	unsigned int spanishCompoundSpecificationCollectionNr_;
	unsigned int userSpecificationCollectionNr_;

	SpecificationItem *confirmedReplacedSpecificationItem_;
	SpecificationItem *confirmedSpecificationButNoRelationReplacedSpecificationItem_;
	SpecificationItem *conflictingSpecificationItem_;
	SpecificationItem *correctedAssumptionReplacedSpecificationItem_;
	SpecificationItem *firstConfirmedReplacedSpanishSpecificationItem_;
	SpecificationItem *replacedAssignmentItem_;

	WordItem *spanishCompoundSpecificationWordItem_;


	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void addToTouchedDuringCurrentSentenceWordList()
		{
		WordItem *tempWordItem;
		WordItem *lastTouchedWordItem = commonVariables_->firstTouchedWordItem;

		if( lastTouchedWordItem == NULL )
			commonVariables_->firstTouchedWordItem = myWordItem_;
		else
			{
			// Word order is important: Add word at end of temporary touched word list
			while( ( tempWordItem = lastTouchedWordItem->nextTouchedDuringCurrentSentenceWordItem ) != NULL )
				lastTouchedWordItem = tempWordItem;

			lastTouchedWordItem->nextTouchedDuringCurrentSentenceWordItem = myWordItem_;
			}
		}

	ResultType addGeneralization( bool isRelation, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *specificationWordItem )
		{
		GeneralizationResultType generalizationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addGeneralization";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( generalizationResult = specificationWordItem->findGeneralization( isRelation, myWordItem_ ) ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a generalization item" );

		if( !generalizationResult.hasFoundGeneralization )
			{
			// Specification word of a generalization word: noun
			if( specificationWordItem->createGeneralizationItem( isRelation, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a generalization item" );
			}

		return RESULT_OK;
		}

	ResultType checkSpecificationWordForConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isSpecificationWordSpanishAmbiguous;
		bool hasFoundCompoundConflict = false;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int compoundSpecificationCollectionNr = NO_COLLECTION_NR;
		SpecificationItem *anotherConflictingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *tempConflictingSpecificationItem;
		SpecificationItem *pastTenseAssignmentItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationWordForConflict";

		conflictingSpecificationItem_ = NULL;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Derive conflict
		if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

		if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) == NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			// Check for looping
			if( isSpecificationWordSpanishAmbiguous ||
			( conflictingSpecificationItem_ = specificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, NO_CONTEXT_NR, relationContextNr, myWordItem_ ) ) == NULL )
				{
				// Check for past tense
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem != NULL &&
				foundSpecificationItem->isArchivedAssignment() != isArchivedAssignment )
					conflictingSpecificationItem_ = foundSpecificationItem;
				else
					{
					if( !isSpecificationWordSpanishAmbiguous &&
					( compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
						{
						// Check for negative
						foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, NO_CONTEXT_NR, relationContextNr, specificationWordItem );

						if( foundSpecificationItem != NULL &&
						!specificationWordItem->isUserSpecificationWord &&
						foundSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
							hasFoundCompoundConflict = true;

						if( !isNegative &&
						foundSpecificationItem != NULL &&

						( hasFoundCompoundConflict ||
						compoundSpecificationCollectionNr != specificationCollectionNr ||
						foundSpecificationItem->isUserSpecification() ) )
							{
							// First check other conflict
							if( ( anotherConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, NO_CONTEXT_NR, relationContextNr, NULL ) ) != NULL )
								{
								// Write conflict
								if( anotherConflictingSpecificationItem->writeSpecificationConflict() != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write another conflicting specification" );
								}

							conflictingSpecificationItem_ = foundSpecificationItem;
							}
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = myWordItem_->nonCompoundCollectionNrInAllWords( compoundSpecificationCollectionNr ) ) > NO_COLLECTION_NR )
								{
								tempConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, relationContextNr, NULL );

								if( tempConflictingSpecificationItem == NULL )
									{
									tempConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, NO_CONTEXT_NR, relationContextNr, NULL );

									if( tempConflictingSpecificationItem != NULL &&
									!tempConflictingSpecificationItem->isOlderItem() &&
									tempConflictingSpecificationItem->specificationWordItem() != specificationWordItem )
										conflictingSpecificationItem_ = tempConflictingSpecificationItem;
									}
								else
									{
									if( tempConflictingSpecificationItem->specificationWordItem() != specificationWordItem &&
									!tempConflictingSpecificationItem->isHiddenSpanishSpecification() )
										conflictingSpecificationItem_ = tempConflictingSpecificationItem;
									}
								}
							}
						}
					}
				}
			}
		else
			{
			// Find for possible past tense assignment of relatedSpecificationItem
			pastTenseAssignmentItem = myWordItem_->firstNonQuestionAssignmentItem( false, false, true, isNegative, isPossessive, relationContextNr, relatedSpecificationItem->specificationWordItem() );
			conflictingSpecificationItem_ = ( pastTenseAssignmentItem == NULL ? relatedSpecificationItem : pastTenseAssignmentItem );
			}

		if( !hasFoundCompoundConflict &&
		conflictingSpecificationItem_ != NULL &&
		!conflictingSpecificationItem_->hasSpecificationBeenWrittenAsConflict &&

		( specificationCollectionNr == NO_COLLECTION_NR ||
		!specificationWordItem->hasNonExclusiveCollection( specificationCollectionNr ) ) )
			{
			if( !isExclusiveSpecification &&
			specificationCollectionNr > NO_COLLECTION_NR &&
			!conflictingSpecificationItem_->isOlderItem() )
				{
				// Sentence in conflict with itself
				if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WARNING, INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_ITSELF ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				// Write conflict
				if( conflictingSpecificationItem_->writeSpecificationConflict() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the conflicting specification" );
				}
			}

		return RESULT_OK;
		}

	ResultType checkUserQuestion( bool hasRelationContext, bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *foundQuestionSpecificationItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *relatedSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserQuestion";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( foundQuestionSpecificationItem != NULL &&
		foundQuestionSpecificationItem->isAnsweredQuestion() &&
		foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem->relationContextNr() == relationContextNr )
			commonVariables_->isQuestionAlreadyAnswered = true;

		if( foundQuestionSpecificationItem == NULL ||
		!foundQuestionSpecificationItem->isQuestion() )
			{
			if( ( specificationResult = findRelatedSpecification( false, false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related question" );

			if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL &&
			!isExclusiveSpecification &&
			!isSpecificationGeneralization &&
			!specificationWordItem->hasNonExclusiveCollection( specificationCollectionNr ) )
				{
				if( relatedSpecificationItem->isOlderItem() )
					{
					if( hasRelationContext )
						{
						if( correctedAssumptionReplacedSpecificationItem_ != NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The corrected assumption replaced specification item already assigned" );

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, true, true, relatedSpecificationItem->isNegative(), relatedSpecificationItem->isPossessive(), true, NO_QUESTION_PARAMETER, relatedSpecificationItem->specificationCollectionNr(), relatedSpecificationItem->relationContextNr(), relatedSpecificationItem->specificationWordItem() ) ) != NULL )
							{
							if( myWordItem_->writeUpdatedSpecification( false, false, false, true, false, false, correctedAssumptionReplacedSpecificationItem_ ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

							hasCorrectedAssumptionByOppositeQuestion_ = true;
							}
						}
					}
				else
					{
					if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WARNING, INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType checkUserSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelection, bool isValueSpecification, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, SpecificationItem *foundSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundRelationContext;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		WordItem *currentLanguageWordItem;
		WordItem *currentSpecificationWordItem;
		WordTypeItem *pluralNounWordTypeItem;
		const char *unknownPluralNounString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserSpecification";

		if( foundSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem->isValueSpecification() != isValueSpecification )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Value specification conflict! Specification word \"", specificationWordItem->anyWordTypeString(), "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		if( !hasRelationContext &&
		!isAssignment &&
		!isExclusiveSpecification &&
		foundSpecificationItem->isExclusiveSpecification() &&
		foundSpecificationItem->isUserSpecification() )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			isNonExclusiveSpecification_ = true;
			}
		else
			{
			if( !isSelection &&
			hasRelationContext &&
			foundSpecificationItem->isOlderItem() )
				{
				if( foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&
				// Check if negative specification exists
				myWordItem_->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) == NULL )
					{
					if( foundSpecificationItem->hasRelationContext() )
						{
						if( isPossessive &&
						relationWordItem != NULL &&
						specificationWordItem->isFemale() != relationWordItem->isFemale() &&
						specificationWordItem->isMale() != relationWordItem->isMale() )
							{
							if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
							}
						}
					else
						{
						if( !isInactiveAssignment )
							{
							if( writeMoreSpecificSpecification( foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific related specification" );
							}
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				if( foundSpecificationItem->hasRelationContext() &&
				foundSpecificationItem->relationContextNr() != relationContextNr &&
				foundSpecificationItem->isInactiveAssignment() == isInactiveAssignment &&
				foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&

				( !hasRelationContext ||
				// Current user specification has less relation words than the found specification
				commonVariables_->nUserRelationWords < myWordItem_->nContextWordsInAllWords( foundSpecificationItem->relationContextNr(), specificationWordItem ) ) )
					{
					// Has new relation context
					if( hasRelationContext ||
					myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == NULL )
						{
						if( !hasRelationContext )
							confirmedSpecificationButNoRelationReplacedSpecificationItem_ = foundSpecificationItem;

						if( !foundSpecificationItem->isHiddenSpanishSpecification() )
							{
							if( myWordItem_->writeSelectedSpecification( false, true, foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed self-generated specification" );

							if( strlen( commonVariables_->writtenSentenceString ) == 0 )
								return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the confirmed self-generated specification" );

							if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( foundSpecificationItem->isSelfGeneratedAssumption() ? ( hasRelationContext ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NO_RELATION ) : ( hasRelationContext ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NO_RELATION ) ) ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a confirmation interface text" );

							if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a confirmed specification" );
							}
						}
					}
				else
					{
					if( hasRelationContext )
						{
						hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

						if( !hasFoundRelationContext ||

						( !foundSpecificationItem->isOlderItem() &&
						foundSpecificationItem->relationContextNr() == relationContextNr ) )
							// Confirmed relation words
							foundSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( hasFoundRelationContext, false, false, isNegative, isPossessive, true, NO_QUESTION_PARAMETER, specificationCollectionNr, foundSpecificationItem->relationContextNr(), specificationWordItem );
						}

					if( foundSpecificationItem != NULL )
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( foundSpecificationItem->isSelfGenerated() &&

						( hasRelationContext ||
						!foundSpecificationItem->isExclusiveSpecification() ) )
							{
							if( foundSpecificationItem->isExclusiveSpecification() )
								isConfirmedExclusive_ = true;

							if( foundSpecificationItem->isSelfGenerated() )
								hasConfirmedSpecification_ = true;

							confirmedReplacedSpecificationItem_ = foundSpecificationItem;

							if( !foundSpecificationItem->isHiddenSpanishSpecification() )
								{
								if( writeConfirmedSpecification( foundSpecificationItem ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );
								}
							}

						if( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem->specificationWordTypeNr() == WORD_TYPE_NOUN_SINGULAR &&
						( pluralNounWordTypeItem = specificationWordItem->activeWordTypeItem( WORD_TYPE_NOUN_PLURAL ) ) != NULL )
							{
							// Skip announcement for new occurrences
							if( !pluralNounWordTypeItem->isOlderItem() )
								{
								// Found unknown plural specification word
								if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a notification text" );

								confirmedReplacedSpecificationItem_ = foundSpecificationItem;
								currentLanguageWordItem = commonVariables_->currentLanguageWordItem;

								if( currentLanguageWordItem != NULL &&
								( currentSpecificationWordItem = commonVariables_->firstSpecificationWordItem ) != NULL &&
								( unknownPluralNounString = currentLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != NULL )
									{
									// Do for all specification words
									do	{
										if( currentSpecificationWordItem->clearLastWrittenSentenceStringWithUnknownPluralNoun( unknownPluralNounString, specificationWordItem ) != RESULT_OK )
											return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the last written sentence string with specification word \"", specificationWordItem->anyWordTypeString(), "\" in generalization word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
										}
									while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
									}
								}
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType checkUserSpecificationOrQuestion( bool hasRelationContext, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isValueSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		GeneralizationResultType generalizationResult;
		SpecificationResultType specificationResult;
		bool hasFeminineSingularNounEnding;
		bool isSingularNounGeneralizationWord = ( generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordTypeItem *foundWordTypeItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserSpecificationOrQuestion";

		hasCorrectedAssumptionByOppositeQuestion_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Skip if definition (singular noun) is Spanish ambiguous
		if( isSingularNounGeneralizationWord &&
		specificationWordItem != myWordItem_ )
			{
			// Check specification in opposite direction
			if( ( generalizationResult = myWordItem_->findGeneralization( false, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find out if word \"", specificationWordItem->anyWordTypeString(), "\" is one of my generalization words" );
			}

		// If user specification is looping
		if( generalizationResult.hasFoundGeneralization )
			{
			if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification looping conflict" );
			}
		else
			{
			// Check current assignment, specification or question (with relation)
			if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( true, true, true, true, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isQuestion ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != NULL )
				{
				if( !isQuestion &&
				isArchivedAssignment != foundSpecificationItem->isArchivedAssignment() )
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( isQuestion &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( foundSpecificationItem == NULL )
				{
				// Check current assignment or specification (without this relation)
				if( isQuestion )
					foundSpecificationItem = myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem == NULL )
					foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem );

				if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME )
					{
					// Exclusive specifications are not conflicting
					if( !isExclusiveSpecification &&
					// Possessive specifications are not conflicting
					!isPossessive &&
					foundSpecificationItem == NULL )
						{
						if( !myWordItem_->isFemaleOrMale() &&
						myWordItem_->hasFeminineProperNameEnding() != specificationWordItem->isFemale() &&
						myWordItem_->hasMasculineProperNameEnding() != specificationWordItem->isMale() )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NAME_TO_BE, myWordItem_->anyWordTypeString(), ( myWordItem_->hasFeminineProperNameEnding() ? INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a proper name expected to be feminine or masculine" );
							}

						if( specificationCollectionNr > NO_COLLECTION_NR )
							{
							if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

							relatedSpecificationItem = specificationResult.relatedSpecificationItem;

							if( hasRelationContext &&
							relatedSpecificationItem != NULL &&
							!relatedSpecificationItem->hasRelationContext() &&
							relatedSpecificationItem->isSelfGeneratedAssumption() )
								{
								// Assumption needs to be corrected
								if( correctedAssumptionReplacedSpecificationItem_ != NULL )
									return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The corrected assumption specification item already assigned" );

								if( myWordItem_->writeUpdatedSpecification( false, false, true, false, false, false, relatedSpecificationItem ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

								hasCorrectedAssumptionByKnowledge_ = true;
								correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
								}
							else
								{
								if( !isPossessive &&
								!isQuestion &&
								myWordItem_->isUserGeneralizationWord &&

								( relatedSpecificationItem == NULL ||

								( ( !relatedSpecificationItem->hasRelationContext() ||
								relatedSpecificationItem->specificationWordItem() != specificationWordItem ) &&

								( !specificationWordItem->isNounWordSpanishAmbiguous() ||
								!relatedSpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) &&

								// Skip checking for conflicts if a question with the current specification word exists
								myWordItem_->firstSpecificationItem( false, false, WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) == NULL )
									{
									if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
										return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
									}
								}
							}
						}

					if( !commonVariables_->hasDisplayedWarning &&
					!hasRelationContext &&
					isAssignment &&
					!isInactiveAssignment &&
					!isArchivedAssignment &&
					!isNegative &&
					!isValueSpecification &&
					specificationContextNr == NO_CONTEXT_NR &&
					!myWordItem_->isLanguageWord() &&
					myWordItem_->isNounWordType( specificationWordTypeNr ) &&
					// Negative specification doesn't exist
					myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == NULL )
						{
						if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning about ambiguity" );
						}
					}
				else
					{
					if( isSingularNounGeneralizationWord &&
					!myWordItem_->isFemaleOrMale() &&
					( foundWordTypeItem = myWordItem_->activeWordTypeItem( WORD_TYPE_NOUN_SINGULAR ) ) != NULL )
						{
						if( foundWordTypeItem->hasFeminineOrMasculineWordEnding() )
							{
							hasFeminineSingularNounEnding = foundWordTypeItem->hasFeminineWordEnding();

							if( !myWordItem_->isCorrectIndefiniteArticle( ( hasFeminineSingularNounEnding ? WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ), generalizationWordTypeNr ) )
								{
								foundWordTypeItem->clearIndefiniteArticleParameter();

								if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_SINGULAR_NOUN_TO_BE, myWordItem_->anyWordTypeString(), ( hasFeminineSingularNounEnding ? INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a singular noun expected to be feminine or masculine" );

								commonVariables_->hasDisplayedArticleNotification = true;
								}
							}
						}
					}
				}
			else
				{
				if( !hasRelationContext &&
				isAssignment &&
				!isNegative &&
				generalizationContextNr == NO_CONTEXT_NR &&
				specificationContextNr == NO_CONTEXT_NR &&
				foundSpecificationItem->hasRelationContext() )
					{
					if( myWordItem_->writeSelectedSpecification( false, true, foundSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					if( strlen( commonVariables_->writtenSentenceString ) == 0 )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );

					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a missing relation" );

					if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					// Force 'I know' notification
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'I know' interface notification" );
					}
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				if( isQuestion )
					{
					if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the user question" );

					// Detect self-generated question
					if( findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );

					// Detect user question
					if( findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );

					isOnlyCheckingForConflicts_ = true;
					}
				else
					{
					if( foundSpecificationItem != NULL )
						{
						if( checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isValueSpecification, specificationWordTypeNr, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check the user specification" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType displayAssumptionsThatAreNotHiddenAnymore()
		{
		ContextItem *currentContextItem;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *hiddenSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayAssumptionsThatAreNotHiddenAnymore";

		if( ( currentContextItem = myWordItem_->firstActiveContextItem() ) != NULL )
			{
			// Do for all context items in my word
			do	{
				if( ( currentGeneralizationItem = myWordItem_->firstGeneralizationItem() ) != NULL )
					{
					// Check all involved words
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

						if( ( hiddenSpecificationItem = currentGeneralizationWordItem->firstUnhiddenSpecificationItem( currentContextItem->contextNr() ) ) != NULL )
							{
							if( currentGeneralizationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, hiddenSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption that is not hidden anymore" );
							}
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
					}
				}
			while( ( currentContextItem = currentContextItem->nextContextItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType findSameSimilarOrRelatedQuestion( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr )
		{
		bool hasFoundQuestion;
		bool isUserSpecificationWord;
		bool hasFoundOlderSpecification = false;
		bool hasFoundSpecificationRelationContext = false;
		bool hasOneSpecificationWord = ( commonVariables_->nUserSpecificationWords == 1 );
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isConflictingQuestion = false;
		bool isRelatedQuestion = false;
		bool isSameAssignment = false;
		bool isSameExclusiveSpecification = false;
		bool isSameRelationContext = false;
		bool isSameQuestion = true;
		bool isSimilarQuestion = false;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *foundQuestionSpecificationItem = NULL;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSameSimilarOrRelatedQuestion";

		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		if( !hasOneSpecificationWord &&
		!isOnlyCheckingForConflicts_ &&
		!isSelfGenerated &&
		( tempSpecificationItem = myWordItem_->sameUserQuestionSpecificationItem( questionParameter ) ) != NULL )
			{
			if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );

			isSameQuestionFromUser_ = true;
			}

		if( ( currentWordItem = commonVariables_->firstWordItem ) == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The first word item is undefined" );

		// Do for all words
		// Only checking specification words is not sufficient
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem->isUserSpecificationWord;

			if( ( tempSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, currentWordItem ) ) != NULL )
				{
				hasFoundQuestion = true;

				if( foundQuestionSpecificationItem == NULL ||
				foundQuestionSpecificationItem->isExclusiveSpecification() != tempSpecificationItem->isExclusiveSpecification() )
					{
					foundQuestionSpecificationItem = tempSpecificationItem;

					hasFoundOlderSpecification = foundQuestionSpecificationItem->isOlderItem();
					hasFoundSpecificationRelationContext = foundQuestionSpecificationItem->hasRelationContext();
					isSameAssignment = ( foundQuestionSpecificationItem->isAssignment() == isAssignment );
					isSameRelationContext = ( foundQuestionSpecificationItem->relationContextNr() == relationContextNr );
					isSameExclusiveSpecification = ( foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification );
					}
				}

			if( hasFoundQuestion ||
			isUserSpecificationWord )
				{
				if( hasFoundQuestion &&
				isUserSpecificationWord )
					{
					isSimilarQuestion = true;

					if( foundQuestionSpecificationItem != NULL )
						{
						if( !hasFoundSpecificationRelationContext ||
						isSameRelationContext )
							{
							if( !isSelfGenerated &&

							( ( isSameRelationContext &&
							!isSameAssignment ) ||

							// Found user question with different exclusive specification
							foundQuestionSpecificationItem->isExclusiveSpecification() != isExclusiveSpecification ||
							// Found user question with different assumption level
							foundQuestionSpecificationItem->assumptionLevel() != assumptionLevel ) )
								isSameQuestion = false;
							else
								{
								if( !hasFoundSpecificationRelationContext &&
								!isExclusiveSpecification &&

								( !hasFoundOlderSpecification ||
								hasRelationContext ) )
									isConflictingQuestion = true;
								}
							}
						else
							{
							if( myWordItem_->isContextSubsetInAllWords( relationContextNr, foundQuestionSpecificationItem->relationContextNr() ) )
								isConflictingQuestion = true;
							else
								{
								isRelatedQuestion = true;
								isSameQuestion = false;
								}
							}
						}
					}
				else
					{
					if( isUserSpecificationWord ||

					// Found question has specification words that aren't in the user question
					( hasSpecificationCollection &&
					isExclusiveSpecification &&
					currentWordItem->isCompoundCollection( specificationCollectionNr ) ) )
						isSameQuestion = false;

					if( isSpecificationGeneralization ||

					// Prefer similar over related if both occur
					( ( !hasFoundSpecificationRelationContext ||
					!isSameRelationContext ) &&

					foundQuestionSpecificationItem != NULL ) )
						{
						if( !isSameAssignment ||
						hasSpecificationCollection )
							isSimilarQuestion = true;
						else
							isConflictingQuestion = true;
						}
					}
				}
			}
		while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

		if( foundQuestionSpecificationItem != NULL )
			{
			if( !isConflictingQuestion &&
			!isOnlyCheckingForConflicts_ &&
			!isSameQuestionFromUser_ &&

			( isExclusiveSpecification ||
			isSameExclusiveSpecification ) )
				{
				if( isSameQuestion )
					{
					if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? INTERFACE_QUESTION_I_HAD_THE_SAME_QUESTION_BEFORE : INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about the same question" );

					if( foundQuestionSpecificationItem->isUserQuestion() )
						isSameQuestionFromUser_ = true;
					}
				else
					{
					if( myWordItem_->writeSelectedSpecification( false, false, foundQuestionSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the found specification" );

					if( strlen( commonVariables_->writtenSentenceString ) == 0 )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the found specification" );

					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a related question" );

					if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

					isSimilarOrRelatedQuestion_ = true;
					}
				}
			else
				{
				if( hasFoundOlderSpecification )
					{
					if( !isOnlyCheckingForConflicts_ &&

					( !isSameExclusiveSpecification ||
					!isSameRelationContext ) )
						{
						hasDisplayedMoreSpecificRelatedQuestion_ = true;

						// Skip displaying announcement if question is in conflict with itself
						if( hasOneSpecificationWord ||
						isExclusiveSpecification )
							{
							if( writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a more specific related question" );
							}
						}
					}
				else
					{
					if( hasDisplayedMoreSpecificRelatedQuestion_ &&
					isConflictingQuestion )
						{
						if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WARNING, INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *createdSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isAssignment() )
			{
			if( replacedAssignmentItem_ == NULL &&
			firstConfirmedReplacedSpanishSpecificationItem_ != NULL &&
			myWordItem_->nContextWordsInAllWords( obsoleteSpecificationItem->relationContextNr(), obsoleteSpecificationItem->specificationWordItem() ) == commonVariables_->nUserRelationWords )
				{
				if( myWordItem_->replaceOrDeleteSpecification( true, firstConfirmedReplacedSpanishSpecificationItem_, obsoleteSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a confirmed specification" );
				}

			replacedAssignmentItem_ = obsoleteSpecificationItem;
			}
		else
			{
			if( myWordItem_->replaceOrDeleteSpecification( false, obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a specification" );

			if( firstConfirmedReplacedSpanishSpecificationItem_ == NULL &&
			obsoleteSpecificationItem->isReplacedItem() )
				firstConfirmedReplacedSpanishSpecificationItem_ = obsoleteSpecificationItem;
			}

		return RESULT_OK;
		}

	ResultType replaceObsoleteSpecification( SpecificationItem *createdSpecificationItem, SpecificationItem *foundUserSpecificationItem, SpecificationItem *obsoleteAssumptionSpecificationItem, SpecificationItem *obsoleteConcludedAssumptionSpecificationItem, SpecificationItem *obsoleteNotCollectedSpecificationItem, SpecificationItem *obsoleteHiddenSpanishSpecificationItem, SpecificationItem *obsoleteUserSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceObsoleteSpecification";

		if( createdSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given created specification is undefined" );

		if( confirmedReplacedSpecificationItem_ != NULL )
			{
			if( replaceOrDeleteSpecification( confirmedReplacedSpecificationItem_, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a confirmed specification" );

			if( confirmedReplacedSpecificationItem_->isReplacedItem() )
				{
				if( myWordItem_->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace obsolete justification items" );
				}

			if( foundUserSpecificationItem != NULL &&
			!foundUserSpecificationItem->isReplacedOrDeletedItem() )
				{
				if( replaceOrDeleteSpecification( foundUserSpecificationItem, createdSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a duplicate user specification" );
				}
			}

		if( confirmedSpecificationButNoRelationReplacedSpecificationItem_ != NULL )
			{
			if( myWordItem_->confirmSpecificationButNotItsRelation( confirmedSpecificationButNoRelationReplacedSpecificationItem_, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to confirm a specification, but no relation(s)" );
			}

		if( correctedAssumptionReplacedSpecificationItem_ != NULL )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a corrected specification" );

			correctedAssumptionReplacedSpecificationItem_ = NULL;
			}

		if( obsoleteAssumptionSpecificationItem != NULL )
			{
			if( myWordItem_->attachJustificationInWord( obsoleteAssumptionSpecificationItem->firstJustificationItem(), createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to attach the first justification of the found assumption specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteAssumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found assumption specification" );
			}

		if( obsoleteConcludedAssumptionSpecificationItem != NULL )
			{
			if( replaceOrDeleteSpecification( obsoleteConcludedAssumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found replacing user specification" );
			}

		if( obsoleteNotCollectedSpecificationItem != NULL )
			{
			if( replaceOrDeleteSpecification( obsoleteNotCollectedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found replacing user specification" );

			if( myWordItem_->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace obsolete justification items" );
			}

		if( obsoleteHiddenSpanishSpecificationItem != NULL )
			{
			if( createdSpecificationItem->attachJustificationToSpecification( obsoleteHiddenSpanishSpecificationItem->firstJustificationItem() ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found replaced hidden specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteHiddenSpanishSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found replaced hidden specification" );
			}

		if( obsoleteUserSpecificationItem != NULL )
			{
			if( replaceOrDeleteSpecification( obsoleteUserSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found replacing user specification" );
			}

		return RESULT_OK;
		}

	ResultType writeConfirmedSpecification( SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeConfirmedSpecification";

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( myWordItem_->writeSelectedSpecification( false, true, writeSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

		if( strlen( commonVariables_->writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given write specification" );

		if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( writeSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ) ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a confirmation" );

		if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

		return RESULT_OK;
		}

	ResultType writeMoreSpecificSpecification( SpecificationItem *olderSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeMoreSpecificSpecification";

		if( olderSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given older specification item is undefined" );

		if( !olderSpecificationItem->isOlderItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given older specification item isn't old" );

		if( myWordItem_->writeSelectedSpecification( false, !olderSpecificationItem->isExclusiveSpecification(), olderSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the given older specification" );

		if( strlen( commonVariables_->writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given older specification" );

		if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( olderSpecificationItem->isQuestion() ? ( olderSpecificationItem->isSelfGenerated() ? INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem->isSelfGenerated() ? ( olderSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface listing text" );

		if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );

		return RESULT_OK;
		}

	SpecificationResultType findRelatedSpecification( bool isCheckingAllQuestions, bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool isFirstRelatedSpecification = true;
		bool isLastRelatedSpecification = false;
		unsigned int currentSpecificationSentenceNr;
		unsigned int currentSpecificationItemNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;
		unsigned int highestItemNr = NO_ITEM_NR;
		SpecificationItem *currentSpecificationItem = ( isCheckingAllQuestions ? myWordItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) : myWordItem_->firstSelectedSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) );
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( specificationWordItem == NULL )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( currentSpecificationItem != NULL &&
		specificationCollectionNr > NO_COLLECTION_NR )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) != NULL )
					{
					currentSpecificationSentenceNr = currentSpecificationItem->creationSentenceNr();
					currentSpecificationItemNr = currentSpecificationItem->itemNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem->relationContextNr() == relationContextNr )
						{
						// Found the given specification item
						isFirstRelatedSpecification = false;

						if( currentSpecificationSentenceNr > highestSentenceNr ||

						( currentSpecificationSentenceNr == highestSentenceNr &&
						currentSpecificationItemNr > highestItemNr ) )
							{
							isLastRelatedSpecification = true;

							highestSentenceNr = currentSpecificationSentenceNr;
							highestItemNr = currentSpecificationItemNr;
							}
						}
					else
						{
						if( currentSpecificationSentenceNr > highestSentenceNr ||

						( currentSpecificationSentenceNr == highestSentenceNr &&
						currentSpecificationItemNr > highestItemNr ) )
							{
							isLastRelatedSpecification = false;

							highestSentenceNr = currentSpecificationSentenceNr;
							highestItemNr = currentSpecificationItemNr;
							}

						// If the specification has no relation, select the oldest one (the first of a series)
						if( !hasRelationContext ||
						specificationResult.relatedSpecificationItem == NULL ||

						( currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
						currentSpecificationWordItem != specificationWordItem ) )
							{
							specificationResult.isFirstRelatedSpecification = isFirstRelatedSpecification;
							specificationResult.relatedSpecificationItem = currentSpecificationItem;
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( isCheckingAllQuestions ? currentSpecificationItem->nextSelectedSpecificationItem() : currentSpecificationItem->nextSelectedSpecificationItem() ) ) != NULL );

			specificationResult.isLastRelatedSpecification = isLastRelatedSpecification;
			}

		return specificationResult;
		}


	protected:
	// Constructor

	WordSpecification( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasConfirmedSpecification_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasCorrectedAssumptionByOppositeQuestion_ = false;
		hasDisplayedMoreSpecificRelatedQuestion_ = false;
		isConfirmedExclusive_ = false;
		isMyWordTouchedDuringCurrentSentence_ = false;
		isNonExclusiveSpecification_ = false;
		isOnlyCheckingForConflicts_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		spanishCompoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		confirmedReplacedSpecificationItem_ = NULL;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = NULL;
		conflictingSpecificationItem_ = NULL;
		correctedAssumptionReplacedSpecificationItem_ = NULL;
		firstConfirmedReplacedSpanishSpecificationItem_ = NULL;
		replacedAssignmentItem_ = NULL;

		spanishCompoundSpecificationWordItem_ = NULL;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordSpecification" );

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

	void initializeWordSpecificationVariables()
		{
		hasConfirmedSpecification_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasDisplayedMoreSpecificRelatedQuestion_ = false;
		isMyWordTouchedDuringCurrentSentence_ = false;
		isOnlyCheckingForConflicts_ = false;

		spanishCompoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		firstConfirmedReplacedSpanishSpecificationItem_ = NULL;
		spanishCompoundSpecificationWordItem_ = NULL;
		}

	bool hasConfirmedSpecification()
		{
		return hasConfirmedSpecification_;
		}

	bool hasCorrectedAssumption()
		{
		return ( hasCorrectedAssumptionByKnowledge_ ||
				hasCorrectedAssumptionByOppositeQuestion_ );
		}

	bool hasCorrectedAssumptionByKnowledge()
		{
		return hasCorrectedAssumptionByKnowledge_;
		}

	bool hasCorrectedAssumptionByOppositeQuestion()
		{
		return hasCorrectedAssumptionByOppositeQuestion_;
		}

	bool hasDisplayedMoreSpecificRelatedQuestion()
		{
		return hasDisplayedMoreSpecificRelatedQuestion_;
		}

	ResultType checkForSpecificationConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForSpecificationConflict";

		conflictingSpecificationItem_ = NULL;

		if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );

		if( conflictingSpecificationItem_ != NULL &&
		conflictingSpecificationItem_->isOlderItem() &&

		( isNegative ||
		!conflictingSpecificationItem_->isNegative() ||
		conflictingSpecificationItem_->hasSpecificationCompoundCollection() ) )
			{
			// Opposite negative of conflicting specification word
			if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, !isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, conflictingSpecificationItem_->specificationWordItem() ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for the opposite negative specification conflict" );
			}

		return RESULT_OK;
		}

	SpecificationResultType addSpecificationInWord( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		bool hasCopiedRelationContext = ( copiedRelationContextNr > NO_CONTEXT_NR );
		bool hasFoundSpecificationWithDifferentRelationContext = false;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserRelationWord = myWordItem_->isUserRelationWord;
		unsigned int nonCompoundCollectionNr;
		unsigned int originalSentenceNr;
		unsigned int compoundCollectionNr = NO_COLLECTION_NR;
		unsigned int generalizationCollectionNr = NO_COLLECTION_NR;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *replacingSpecificationItem;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *foundUserSpecificationItem = NULL;
		SpecificationItem *obsoleteAssumptionSpecificationItem = NULL;
		SpecificationItem *obsoleteConcludedAssumptionSpecificationItem = NULL;
		SpecificationItem *obsoleteNotCollectedSpecificationItem = NULL;
		SpecificationItem *obsoleteHiddenSpanishSpecificationItem = NULL;
		SpecificationItem *obsoleteUserSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationInWord";

		isConfirmedExclusive_ = false;
		isNonExclusiveSpecification_ = false;
		isSameQuestionFromUser_ = false;

		if( !isMyWordTouchedDuringCurrentSentence_ )
			{
			isMyWordTouchedDuringCurrentSentence_ = true;
			addToTouchedDuringCurrentSentenceWordList();
			}

		confirmedReplacedSpecificationItem_ = NULL;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = NULL;
		correctedAssumptionReplacedSpecificationItem_ = NULL;
		replacedAssignmentItem_ = NULL;

		// Check generalization word type
		if( !myWordItem_->hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables
		if( ( hasRelationContext ||
		relationWordTypeNr != NO_WORD_TYPE_NR ||
		relationWordItem != NULL ) &&

		( ( !hasRelationContext &&
		// Exception: Conditions have no relation context
		!isConditional ) ||

		relationWordItem == NULL ||
		!relationWordItem->hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The relation variables aren't matching" );

		if( specificationWordItem != NULL )
			{
			// Check specification word type
			if( !specificationWordItem->hasWordType( true, specificationWordTypeNr ) &&
			// New created language
			!myWordItem_->isLanguageWord() )
				return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification word type" );

			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( !isSpecificationWordSpanishAmbiguous )
				generalizationCollectionNr = myWordItem_->collectionNr( generalizationWordTypeNr, specificationWordItem );

			// Collect definition specification without specification collection
			if( !isExclusiveSpecification &&
			!isQuestion &&
			!specificationWordItem->hasCollection() &&
			myWordItem_->isNounWordType( generalizationWordTypeNr ) &&
			myWordItem_->isNounWordType( specificationWordTypeNr ) )
				{
				if( myWordItem_->addCollectionByGeneralization( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to add a collection by generalization of specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}

			if( hasSpecificationCollection )
				{
				if( !specificationWordItem->hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}
			else
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr );
					nonCompoundCollectionNr = specificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );

					if( nonCompoundCollectionNr > NO_COLLECTION_NR &&

					( ( compoundCollectionNr == NO_COLLECTION_NR &&
					// Not a definition sentence with the same generalization and specification word
					!specificationWordItem->isUserGeneralizationWord ) ||

					// Non-exclusive specification collection
					( !isExclusiveSpecification &&
					!isQuestion &&

					( !isSpecificationWordSpanishAmbiguous ||

					( hasRelationContext &&

					( nContextRelations > 1 ||
					!specificationWordItem->isUserSpecificationWord ) ) ) ) ) )
						specificationCollectionNr = nonCompoundCollectionNr;
					else
						specificationCollectionNr = compoundCollectionNr;
					}
				}

			if( !isSelfGenerated &&
			myWordItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				if( checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check a user specification or question with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

				if( isGeneralizationProperName ||

				( !isAssignment &&
				!isEveryGeneralization ) )
					userSpecificationCollectionNr_ = specificationCollectionNr;
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				// Try to find specification
				// Start with finding user specification
				if( ( foundUserSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, true, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == NULL )
					{
					if( isSelfGenerated )
						{
						if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isAssignment ? NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == NULL )
							{
							// Not found
							if( hasRelationContext &&
							!isQuestion &&
							( replacingSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) != NULL )
								{
								if( ( isSpecificationWordSpanishAmbiguous ||
								compoundCollectionNr > NO_COLLECTION_NR ) &&

								!replacingSpecificationItem->hasSpecificationCollection() &&
								replacingSpecificationItem->isSelfGenerated() )
									obsoleteNotCollectedSpecificationItem = replacingSpecificationItem;
								else
									{
									if( isSpecificationWordSpanishAmbiguous &&
									replacingSpecificationItem->hasRelationContext() )
										{
										if( replacingSpecificationItem->isOlderItem() &&
										replacingSpecificationItem->isHiddenSpanishSpecification() )
											obsoleteHiddenSpanishSpecificationItem = replacingSpecificationItem;
										}
									else
										{
										if( ( !isSpecificationWordSpanishAmbiguous ||
										hasSpecificationCollection ) &&

										( isUserRelationWord &&
										replacingSpecificationItem->isSelfGenerated() &&
										replacingSpecificationItem->hasSpecificationNonCompoundCollection() &&
										// Exclude archived assignment
										!replacingSpecificationItem->hasCurrentCreationSentenceNr() ) )
											{
											if( confirmedReplacedSpecificationItem_ != NULL )
												return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The confirmed replaced specification item is already defined" );

											if( myWordItem_->writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, isSpecificationWordSpanishAmbiguous, false, false, false, !isSpecificationWordSpanishAmbiguous, replacingSpecificationItem ) != RESULT_OK )
												return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption, which will have an added relation" );

											confirmedReplacedSpecificationItem_ = replacingSpecificationItem;
											}
										}
									}
								}
							}
						else
							{
							if( foundSpecificationItem->relationContextNr() == relationContextNr )
								{
								if( isGeneralizationProperName &&
								firstJustificationItem != NULL &&
								foundSpecificationItem->isOlderItem() )
									{
									if( firstJustificationItem->isConclusionJustification() )
										{
										if( foundSpecificationItem->isSelfGeneratedAssumption() &&

										( !hasRelationContext ||
										foundSpecificationItem->hasSpecificationCompoundCollection() ||

										( isUserRelationWord &&
										foundSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL &&

										( isSpecificationWordSpanishAmbiguous ||
										foundSpecificationItem->hasOnlyOneRelationWord() ) ) ) &&

										( ( !isSpecificationWordSpanishAmbiguous &&
										// Conclusion doesn't exist
										myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, NULL ) == NULL ) ||

										// Typically for the Spanish language
										( isSpecificationWordSpanishAmbiguous &&
										!foundSpecificationItem->isHiddenSpanishSpecification() ) ) )
											{
											if( foundSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
												return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

											if( isUserRelationWord ||
											foundSpecificationItem->hasCurrentActiveSentenceNr() ||
											!foundSpecificationItem->hasSpecificationCompoundCollection() )
												{
												if( myWordItem_->writeUpdatedSpecification( true, false, false, false, true, false, foundSpecificationItem ) != RESULT_OK )
													return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a concluded assumption" );
												}
											}
										}
									else
										{
										if( hasRelationContext &&
										!isUserRelationWord &&
										!isSpecificationWordSpanishAmbiguous &&
										foundSpecificationItem->isConcludedAssumption() )
											obsoleteConcludedAssumptionSpecificationItem = foundSpecificationItem;
										}
									}
								}
							else
								{
								if( hasRelationContext &&
								foundSpecificationItem->hasRelationContext() )
									{
									hasFoundSpecificationWithDifferentRelationContext = true;

									if( !isSpecificationWordSpanishAmbiguous &&
									firstJustificationItem != NULL &&

									( ( foundSpecificationItem->isSelfGeneratedAssumption() &&
									firstJustificationItem->isOppositePossessiveConditionalSpecificationAssumption() ) ||

									( foundSpecificationItem->isSelfGeneratedConclusion() &&
									firstJustificationItem->isPossessiveReversibleConclusion() ) ) )
										obsoleteAssumptionSpecificationItem = foundSpecificationItem;
									}
								}
							}
						}
					}
				else
					{
					if( foundUserSpecificationItem->isSpecificationGeneralization() == isSpecificationGeneralization )
						{
						if( !isSelfGenerated ||
						foundUserSpecificationItem->hasRelationContext() ||
						!foundUserSpecificationItem->isExclusiveSpecification() ||
						foundUserSpecificationItem->isOlderItem() )
							{
							foundSpecificationItem = foundUserSpecificationItem;

							if( isGeneralizationProperName &&
							specificationCollectionNr > NO_COLLECTION_NR &&

							( ( isAssignment &&
							confirmedReplacedSpecificationItem_ != NULL &&
							foundUserSpecificationItem->isUserAssignment() ) ||

							!foundUserSpecificationItem->hasSpecificationCollection() ) )
								// Prepare foundUserSpecificationItem for confirmation
								foundUserSpecificationItem = myWordItem_->firstSpecificationItem( isPossessive, false, NO_QUESTION_PARAMETER, specificationWordItem );
							else
								foundUserSpecificationItem = NULL;
							}
						else	// Typically for compound collections in Spanish
							{
							if( spanishCompoundSpecificationCollectionNr_ == NO_COLLECTION_NR ||
							spanishCompoundSpecificationWordItem_ == NULL )
								{
								if( ( specificationCollectionNr = myWordItem_->highestCollectionNrInAllWords() ) >= MAX_COLLECTION_NR )
									return myWordItem_->startSpecificationResultSystemErrorInWord( functionNameString, moduleNameString_, "Collection number overflow" );

								spanishCompoundSpecificationCollectionNr_ = ++specificationCollectionNr;
								spanishCompoundSpecificationWordItem_ = specificationWordItem;
								}
							else
								{
								specificationCollectionNr = spanishCompoundSpecificationCollectionNr_;

								if( spanishCompoundSpecificationWordItem_->addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", spanishCompoundSpecificationWordItem_->anyWordTypeString(), "\" with word \"", specificationWordItem->anyWordTypeString(), "\"" );

								if( specificationWordItem->addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishCompoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", specificationWordItem->anyWordTypeString(), "\" with word \"", spanishCompoundSpecificationWordItem_->anyWordTypeString(), "\"" );
								}
							}
						}
					else
						{
						if( foundUserSpecificationItem->hasGeneralizationCollection() &&
						foundUserSpecificationItem->isSpecificationWordSpanishAmbiguous() )
							specificationCollectionNr = foundUserSpecificationItem->generalizationCollectionNr();
						}
					}
				}

			// Add generalization items
			if( hasRelationContext ||
			foundSpecificationItem == NULL )
				{
				// Specification word
				if( addGeneralization( false, generalizationWordTypeNr, specificationWordTypeNr, specificationWordItem ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to add a specification word to the generalizations" );

				// Relation word
				if( relationWordItem != NULL )
					{
					if( addGeneralization( true, generalizationWordTypeNr, relationWordTypeNr, relationWordItem ) != RESULT_OK )
						return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to add a relation word to the generalizations" );
					}
				}
			}

		if( !commonVariables_->hasDisplayedWarning )
			{
			if( foundSpecificationItem == NULL ||

			// Exceptions
			isNonExclusiveSpecification_ ||
			confirmedReplacedSpecificationItem_ != NULL ||
			correctedAssumptionReplacedSpecificationItem_ != NULL ||
			obsoleteConcludedAssumptionSpecificationItem != NULL ||

			// Overwrite non-conditional specification by conditional one
			( isConditional &&
			!foundSpecificationItem->isConditional() ) ||

			// Overwrite non-exclusive specification by exclusive one
			( isExclusiveSpecification &&
			!foundSpecificationItem->isExclusiveSpecification() ) ||

			// Overwrite user questions by non-exclusive one
			( !isAssignment &&
			isQuestion &&
			!isSameQuestionFromUser_ &&
			!isSelfGenerated &&
			foundSpecificationItem->isExclusiveSpecification() ) ||

			// Overwrite self-generated assignment by user assignment
			( isAssignment &&
			!isSelfGenerated &&
			foundSpecificationItem->isSelfGenerated() ) ||

			// User specification has copied relation context
			( hasCopiedRelationContext &&
			isSelfGenerated &&
			foundSpecificationItem->isUserSpecification() ) ||

			// Accept different relation context (e.g. ambiguous specification)
			( hasFoundSpecificationWithDifferentRelationContext &&
			!isArchivedAssignment &&
			isSelfGenerated ) )
				{
				if( foundSpecificationItem != NULL )
					{
					if( !isConditional &&
					foundSpecificationItem->isConditional() )
						isConditional = true;

					if( !isExclusiveSpecification &&
					!isQuestion &&
					foundSpecificationItem->isExclusiveSpecification() )
						isExclusiveSpecification = true;

					if( !isQuestion &&
					!isSelfGenerated &&
					foundSpecificationItem->isUserSpecification() &&
					// Check with other replacements if this one is unique
					foundSpecificationItem != confirmedReplacedSpecificationItem_ &&
					foundSpecificationItem != confirmedSpecificationButNoRelationReplacedSpecificationItem_ &&
					foundSpecificationItem != correctedAssumptionReplacedSpecificationItem_ &&
					foundSpecificationItem != obsoleteAssumptionSpecificationItem &&
					foundSpecificationItem != obsoleteConcludedAssumptionSpecificationItem &&
					foundSpecificationItem != obsoleteNotCollectedSpecificationItem &&
					foundSpecificationItem != obsoleteHiddenSpanishSpecificationItem )
						obsoleteUserSpecificationItem = foundSpecificationItem;
					}

				if( isExclusiveSpecification )
					{
					if( isNonExclusiveSpecification_ )
						isExclusiveSpecification = false;
					}
				else
					{
					if( isConfirmedExclusive_ )
						isExclusiveSpecification = true;
					}

				originalSentenceNr = ( hasRelationContext ||
									isQuestion ||
									foundSpecificationItem == NULL ? commonVariables_->currentSentenceNr : foundSpecificationItem->originalSentenceNr() );

				// Create the actual specification
				if( ( specificationResult = myWordItem_->createSpecificationItem( false, false, false, false, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveSpecification, false, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, NO_ASSIGNMENT_LEVEL, assumptionLevel, commonVariables_->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? NO_WORD_TYPE_NR : relationWordTypeNr ), generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, NULL ) ).result != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a specification item" );

				if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
					return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't create a specification item" );

				// Now replaced obsolete specifications by the created specification
				if( replaceObsoleteSpecification( createdSpecificationItem, foundUserSpecificationItem, obsoleteAssumptionSpecificationItem, obsoleteConcludedAssumptionSpecificationItem, obsoleteNotCollectedSpecificationItem, obsoleteHiddenSpanishSpecificationItem, obsoleteUserSpecificationItem ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete specification" );

				if( hasCopiedRelationContext &&
				firstJustificationItem != NULL &&
				foundSpecificationItem != NULL )
					{
					if( !firstJustificationItem->isOlderItem() &&

					( isSpecificationWordSpanishAmbiguous ||
					foundSpecificationItem->hasCurrentCreationSentenceNr() ) )
						{
						if( firstJustificationItem->changeAttachedJustification( foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
							return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
						}

					if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) == RESULT_OK )
						{
						if( myWordItem_->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
							return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace obsolete justification items" );
						}
					else
						return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
					}

				if( !isNegative &&
				!isPossessive &&
				!isQuestion &&
				!isSimilarOrRelatedQuestion_ &&
				specificationWordItem != NULL &&

				( !isSpecificationWordSpanishAmbiguous ||
				!createdSpecificationItem->isHiddenSpanishSpecification() ) )
					{
					if( myWordItem_->findAnswersToQuestions( specificationWordItem->compoundCollectionNr( specificationWordTypeNr ), createdSpecificationItem ) != RESULT_OK )
						return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find answers to questions" );

					if( !isSelfGenerated &&
					!myWordItem_->isFemaleOrMale() )
						{
						if( specificationWordItem->isFemale() )
							{
							if( myWordItem_->markWordAsFemale() != RESULT_OK )
								return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as female" );
							}
						else
							{
							if( specificationWordItem->isMale() )
								{
								if( myWordItem_->markWordAsMale() != RESULT_OK )
									return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as male" );

								if( !isAssignment &&
								isGeneralizationProperName &&
								myWordItem_->isSpanishCurrentLanguage() )
									{
									if( displayAssumptionsThatAreNotHiddenAnymore() != RESULT_OK )
										return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to display assumptions that are not hidden anymore" );
									}
								}
							}
						}
					}
				}
			}

		specificationResult.foundSpecificationItem = foundSpecificationItem;
		specificationResult.replacedAssignmentItem = replacedAssignmentItem_;

		return specificationResult;
		}

	SpecificationResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( searchSpecificationItem == NULL )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( false, isCheckingRelationContext, false, false, searchSpecificationItem->isAssignment(), searchSpecificationItem->isInactiveAssignment(), searchSpecificationItem->isArchivedAssignment(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPossessive(), searchSpecificationItem->questionParameter(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->specificationWordItem() );
		}

	SpecificationResultType findRelatedSpecification( bool isCheckingAllQuestions, bool isIgnoringExclusive, bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( isIncludingAssignments )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			specificationResult.relatedSpecificationItem == NULL )
				{
				// Archived assignments
				if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification as well" );
				}
			}

		// Specifications
		if( specificationResult.relatedSpecificationItem == NULL )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );
			}

		return specificationResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
