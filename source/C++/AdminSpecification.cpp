/*
 *	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
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
#include "ContextItem.cpp"
#include "Presentation.cpp"
#include "ReadItem.cpp"
#include "SelectionItem.cpp"
#include "SpecificationItem.cpp"

class AdminSpecification
	{
	friend class AdminItem;

	// Private constructible variables

	bool isArchivedAssignment_;
	bool isFirstPartOfUserSentence_;

	unsigned short doneSpecificationWordOrderNr_;
	unsigned short linkedGeneralizationWordTypeNr_;

	unsigned int missingRelationContextNr_;
	unsigned int mostRecentRelationContextNr_;
	unsigned int userRelationContextNr_;

	SpecificationItem *userSpecificationItem_;

	WordItem *linkedGeneralizationWordItem_;

	char *previousSpecificationString_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType addUserSpecificationWithRelation( bool isAction, bool isAssignedOrClear, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		bool isFirstComparisonPart = ( selectionListNr == ADMIN_CONDITION_LIST );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isStop = false;
		unsigned short relationWordTypeNr;
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		ReadItem *currentRelationReadItem = startRelationReadItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *mostRecentContextWord;
		WordItem *relationWordItem;
		WordItem *previousRelationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecificationWithRelation";

		if( startRelationReadItem != NULL )
			{
			if( endRelationReadItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						do	{
							if( currentRelationReadItem->isPreposition() )
								prepositionParameter = currentRelationReadItem->wordParameter();
							else
								{
								if( currentRelationReadItem->isRelationWord() )
									{
									if( ( relationWordItem = currentRelationReadItem->readWordItem() ) != NULL )
										{
										if( relationWordItem != generalizationWordItem )
											{
											if( relationWordItem != specificationWordItem )
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
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );
													}

												if( selectionListNr == NO_LIST_NR )
													{
													if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NULL, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ).result != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( ( relationContextNr = relationWordItem->contextNrInWord( specificationWordItem ) ) == NO_CONTEXT_NR )
														{
														if( ( relationContextNr = adminItem_->highestContextNr() ) < MAX_CONTEXT_NR )
															{
															if( relationWordItem->addContext( isQuestion, relationWordTypeNr, specificationWordTypeNr, ++relationContextNr, specificationWordItem ) != RESULT_OK )
																return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );
															}
														else
															return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Context number overflow" );
														}

													if( adminItem_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem->isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) == RESULT_OK )
														isFirstComparisonPart = false;
													else
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a selection part having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
													}

												previousRelationWordItem = relationWordItem;
												}
											else
												return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The relation word is the same as the specification word" );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The relation word is the same as the generalization word" );
										}
									else
										return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined read word" );
									}
								}
							}
						while( !isStop &&
						currentRelationReadItem != endRelationReadItem &&
						( currentRelationReadItem = currentRelationReadItem->nextReadItem() ) != NULL );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given end relation read item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given start relation read item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType addSpecification( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, bool isValueSpecification, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( generalizationWordItem != NULL )
			{
			if( adminItem_->isSystemStartingUp() &&

			( generalizationWordItem->isNeedingAuthorizationForChanges() ||

			( specificationWordItem != NULL &&
			specificationWordItem->isNeedingAuthorizationForChanges() ) ) )
				return specificationResult = adminItem_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, mostRecentRelationContextNr_, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			if( !generalizationWordItem->isVerbImperativeLogin() )	// Already created by during startup
				return generalizationWordItem->addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, adminItem_->isUserSentenceMixOfFeminineAndMasculineSpecificationWords(), isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, mostRecentRelationContextNr_, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}

	WordItem *mostRecentContextWordInAllWords( unsigned int contextNr )
		{
		ContextItem *currentContextItem;
		ContextItem *mostRecentContextItem = NULL;
		WordItem *currentWordItem;
		WordItem *mostRecentWordItem = NULL;

		if( contextNr > NO_CONTEXT_NR &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( currentContextItem = currentWordItem->contextItemInWord( contextNr ) ) != NULL )
					{
					if( mostRecentContextItem == NULL ||
					currentContextItem->isMoreRecent( mostRecentContextItem ) )
						{
						mostRecentWordItem = currentWordItem;
						mostRecentContextItem = currentContextItem;
						}
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return mostRecentWordItem;
		}


	protected:
	// Constructor / deconstructor

	AdminSpecification( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		isArchivedAssignment_ = false;
		isFirstPartOfUserSentence_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = WORD_TYPE_UNDEFINED;

		missingRelationContextNr_ = NO_CONTEXT_NR;
		mostRecentRelationContextNr_ = NO_CONTEXT_NR;
		userRelationContextNr_ = NO_CONTEXT_NR;

		userSpecificationItem_ = NULL;

		linkedGeneralizationWordItem_ = NULL;

		previousSpecificationString_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminSpecification" );

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


	// Protected assignment functions

	ResultType assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSelectionSpecification";

		if( assignmentSelectionItem != NULL )
			{
			if( assignSpecification( false, assignmentSelectionItem->isAssignedOrClear(), assignmentSelectionItem->isInactiveAssignment(), assignmentSelectionItem->isArchivedAssignment(), assignmentSelectionItem->isNegative(), false, assignmentSelectionItem->isPossessive(), assignmentSelectionItem->isSpecificationGeneralization(), assignmentSelectionItem->isUniqueRelation(), assignmentSelectionItem->assumptionLevel(), assignmentSelectionItem->prepositionParameter(), NO_QUESTION_PARAMETER, assignmentSelectionItem->relationWordTypeNr(), assignmentSelectionItem->generalizationContextNr(), assignmentSelectionItem->specificationContextNr(), assignmentSelectionItem->relationContextNr(), NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, assignmentSelectionItem->nContextRelations(), NULL, assignmentSelectionItem->generalizationWordItem(), assignmentSelectionItem->specificationWordItem(), assignmentSelectionItem->specificationString() ).result != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign a specification" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given assignment selection item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( generalizationWordItem != NULL )
			{
			if( adminItem_->isSystemStartingUp() &&

			( generalizationWordItem->isNeedingAuthorizationForChanges() ||

			( specificationWordItem != NULL &&
			specificationWordItem->isNeedingAuthorizationForChanges() ) ) )
				{
				if( ( specificationResult = adminItem_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
					myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", ( specificationWordItem == NULL ? specificationString : specificationWordItem->anyWordTypeString() ), "\" with authorization" );
				}
			else
				{
				if( ( specificationResult = generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL ) ).result != RESULT_OK )
					myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", ( specificationWordItem == NULL ? specificationString : specificationWordItem->anyWordTypeString() ), "\"" );
				}
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}


	// Protected specification functions

	void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = WORD_TYPE_UNDEFINED;
		linkedGeneralizationWordItem_ = NULL;
		}

	void initializeAdminSpecificationVariables()
		{
		isArchivedAssignment_ = false;
		isFirstPartOfUserSentence_ = true;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = WORD_TYPE_UNDEFINED;

		missingRelationContextNr_ = NO_CONTEXT_NR;
		mostRecentRelationContextNr_ = NO_CONTEXT_NR;
		userRelationContextNr_ = NO_CONTEXT_NR;

		userSpecificationItem_ = NULL;

		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordItem_ = NULL;
		}

	ResultType addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		CollectionResultType collectionResult;
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		bool isGeneralizationProperName;
		bool isRelationWord;
		bool isSpecificationWord;
		bool hasFoundAction = false;
		bool hasRelationContext = ( startRelationReadItem != NULL );
		bool hasLinkedPossessiveDeterminer = false;
		bool isExclusiveContext = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelection = ( selectionListNr != NO_LIST_NR );
		bool isSkippingThisGeneralizationPart = false;
		bool isSpecificationWordAlreadyAssignedByComparison = false;
		bool isValueSpecificationWord = false;
		bool isWaitingForRelation = false;
		bool isWaitingForText = false;
		unsigned short firstSpecificationWordOrderNr;
		unsigned short generalizationWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned short valueGeneralizationWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned short linkedSpecificationWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned short currentSpecificationWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned short previousSpecificationWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned int nContextRelations = 0;
		ReadItem *currentReadItem;
		ReadItem *firstRelationReadItem;
		ReadItem *lastGeneralizationReadItem;
		WordItem *currentWordItem;
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

		if( generalizationReadItem != NULL )
			{
			if( ( generalizationWordItem = generalizationReadItem->readWordItem() ) != NULL )
				{
				isGeneralizationProperName = generalizationReadItem->isProperName();
				generalizationWordTypeNr = generalizationReadItem->wordTypeNr();

				if( ( currentReadItem = startSpecificationReadItem ) != NULL )
					{
					if( endSpecificationReadItem != NULL )
						{
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
									if( generalizationReadItem->wordOrderNr() < currentReadItem->wordOrderNr() )
										isSkippingThisGeneralizationPart = false;
									}
								}
							else
								{
								if( doneSpecificationWordOrderNr_ > currentReadItem->wordOrderNr() )
									isSkippingThisGeneralizationPart = true;
								else
									{
									if( currentReadItem->isGeneralizationWord() )
										{
										isSkippingThisGeneralizationPart = true;
										isWaitingForRelation = true;
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
							!currentReadItem->isNumeralBoth() )		// Skip numeral 'both'. Typically for English: in both ... and ...
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
									if( isFirstPartOfUserSentence_ &&
									isSpecificationWord &&
									startRelationReadItem != NULL &&
									!generalizationWordItem->isVerbImperativeReadOrShow() )
										{
										if( ( contextResult = adminItem_->getRelationContext( isAssignment, isExclusiveSpecification, isNegative, isPossessive, isQuestion, true, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, startRelationReadItem ) ).result == RESULT_OK )
											{
											hasFoundAction = true;
											isExclusiveContext = contextResult.isExclusiveContext;
											userRelationContextNr_ = contextResult.contextNr;
											}
										else
											return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to get the relation context" );
										}

									if( currentReadItem->isNumeral() ||
									currentReadItem->isText() ||

									( isSpecificationWord &&
									!currentReadItem->isPreposition() ) )
										{
										if( isValueSpecificationWord )
											{
											if( isSelection )
												{
												if( adminItem_->createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
													return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to create a value selection item" );
												}

											if( !hasRelationContext )
												{
												// Value, but no relation
												if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
													return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to add a specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											else
												return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "Adding a value specification with relation isn't implemented" );
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
												if( selectionListNr == NO_LIST_NR )
													{
													if( previousSpecificationString_ == NULL )
														{
														if( previousSpecificationWordItem != NULL &&
														currentSpecificationWordItem != NULL &&
														currentReadItem->isMatchingWordTypeNr( previousSpecificationWordTypeNr ) )
															{
															if( ( collectionResult = adminItem_->collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) ).result == RESULT_OK )
																{
																if( collectionResult.isAmbiguousCollection )
																	{
																	if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, NULL, generalizationWordItem, previousSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
																		return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to add a specification with an ambiguous specification word to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to collect specification words" );
															}
														}
													else
														{
														if( adminItem_->collectSpecificationStrings( ( isExclusiveSpecification || isPossessive ), generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationWordItem, previousSpecificationString_, specificationString ) != RESULT_OK )
															return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to collect specification strings" );
														}

													if( currentSpecificationWordTypeNr != WORD_TYPE_PREPOSITION )
														{
														previousSpecificationWordItem = currentSpecificationWordItem;
														previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
														previousSpecificationString_ = specificationString;
														}
													}
												else	// Create selection
													{
													if( hasRelationContext )
														{
														// Selection, no value, with relation
														if( addUserSpecificationWithRelation( isAction, currentReadItem->isAdjectiveAssignedOrClear(), isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isExclusiveSpecification || isExclusiveContext ), isNegative, isNewStart, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, userAssumptionLevel, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem, specificationString ) != RESULT_OK )
															return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to create a selection part with relation" );
														}
													else
														{
														// Selection, no value, no relation
														if( adminItem_->createSelectionPart( isAction, currentReadItem->isAdjectiveAssignedOrClear(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
															return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to create a selection part" );
														}
													}
												}

											if( !currentReadItem->isAdjectiveAssignedOrClear() )
												{
												doneSpecificationWordOrderNr_ = currentReadItem->wordOrderNr();
												linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
												linkedSpecificationWordItem = currentSpecificationWordItem;

												if( !currentReadItem->isNounValue() &&
												currentSpecificationWordTypeNr != WORD_TYPE_ADVERB &&
												currentReadItem->wordOrderNr() <= firstSpecificationWordOrderNr )
													{
													if( generalizationWordItem->isVerbImperativeReadOrShow() )
														hasFoundAction = true;
													else
														{
														if( hasRelationContext )
															{
															// No value, with relation
															if( addUserSpecificationWithRelation( false, false, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isExclusiveSpecification || isExclusiveContext ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, NO_SELECTION_LEVEL, NO_LIST_NR, imperativeParameter, specificationWordParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem, specificationString ) == RESULT_OK )
																hasFoundAction = true;
															else
																return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to add a user specification with a relation to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														else
															{
															// No value, no relation
															if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, NULL, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result == RESULT_OK )
																hasFoundAction = true;
															else
																return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to add a specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}

														if( !isQuestion &&
														currentSpecificationWordItem != NULL &&
														adminItem_->isReasoningWordType( generalizationWordTypeNr ) &&
														adminItem_->isSingularOrPluralNoun( currentSpecificationWordTypeNr ) )
															{
															// Generalizations with noun specifications - without relations
															if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, NULL ) == RESULT_OK )
																{
																if( !commonVariables_->hasShownWarning )
																	{
																	if( adminItem_->isSingularOrPluralNoun( generalizationWordTypeNr ) )	// Definition specification
																		{
																		// Definition specification
																		if( ( specificationResult = adminItem_->drawCompoundSpecificationSubstitutionConclusion( isNegative, isPartOf, isPossessive, generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result == RESULT_OK )
																			compoundGeneralizationWordItem = specificationResult.compoundGeneralizationWordItem;
																		else
																			return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																		}
																	else
																		{
																		if( isGeneralizationProperName &&
																		commonVariables_->nUserRelationWords == 1 &&
																		startRelationReadItem != NULL &&
																		( firstRelationReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL )
																			{
																			if( ( relationWordItem = firstRelationReadItem->readWordItem() ) != NULL )
																				{
																				if( adminItem_->drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueRelation, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, firstRelationReadItem->wordTypeNr(), specificationContextNr, userRelationContextNr_, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
																					return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with one relation word, from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
																				}
																			}
																		}
																	}
																}
															else
																return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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

									if( !commonVariables_->hasShownWarning &&
									currentSpecificationWordItem != NULL )
										{
										if( currentReadItem->isVerb() )
											hasFoundAction = true;

										if( !isQuestion &&
										linkedSpecificationWordItem != NULL &&

										( isRelationWord ||
										isSpecificationWord ) &&

										( !hasRelationContext ||
										linkedSpecificationWordItem != currentSpecificationWordItem ) )
											{
											if( isGeneralizationProperName &&
											!isValueSpecificationWord &&
											// Skip adjectives
											adminItem_->isSingularOrPluralNoun( linkedSpecificationWordTypeNr ) )
												{
												if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationContext ? currentSpecificationWordItem : NULL ) ) != RESULT_OK )
													return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );
												}

											if( isRelationWord ||

											( isSpecificationWord &&
											hasLinkedPossessiveDeterminer ) )
												{
												if( !commonVariables_->hasShownWarning &&
												// Linked specification
												currentSpecificationWordItem == linkedSpecificationWordItem )
													{
													if( linkedGeneralizationWordItem_ == NULL )
														{
														linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
														linkedGeneralizationWordItem_ = generalizationWordItem;
														}
													else
														return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "Linked word \"", linkedGeneralizationWordItem_->anyWordTypeString(), "\" is already assigned" );
													}

												if( !commonVariables_->hasShownWarning &&
												!isQuestion &&
												userRelationContextNr_ > NO_CONTEXT_NR &&
												commonVariables_->nUserRelationWords > 1 )
													{
													if( adminItem_->drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												{
												if( selectionListNr == NO_LIST_NR &&
												currentReadItem->isAdjectiveAssigned() )
													{
													if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem->readWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, currentReadItem->wordOrderNr(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
													}
												}
											}
										}
									}
								}
							}
						while( !commonVariables_->hasShownWarning &&
						currentReadItem != endSpecificationReadItem &&
						( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

						if( hasFoundAction )
							{
							isFirstPartOfUserSentence_ = false;

							if( isGeneralizationProperName &&
							( currentWordItem = commonVariables_->firstWordItem ) != NULL )
								{
								// Do for all words
								do	{
									if( currentWordItem->isWordTouchedDuringCurrentSentence )
										{
										// Recalculate the generalization word(s)
										if( currentWordItem->recalculateAssumptions() != RESULT_OK )
											return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"", currentWordItem->anyWordTypeString(), "\"" );
										}
									}
								while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
								}
							}
						else
							return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I couldn't find anything to do" );
						}
					else
						return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The given end specification read item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The given start specification read item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The given generalization read item has no read word" );
			}
		else
			return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The given generalization read item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType addSpecification( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, bool isValueSpecification, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		ContextResultType contextResult;
		SpecificationResultType assignmentResult;
		SpecificationResultType specificationResult;
		bool isDefinitionSpecification = ( relationWordItem == NULL &&
											adminItem_->isSingularOrPluralNoun( generalizationWordTypeNr ) &&
											adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationContextAlreadyDefined = false;
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		SpecificationItem *createdFeminineOrMasculineSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *foundFeminineOrMasculineSpecificationItem;
		SpecificationItem *replacedAssignmentItem;
		SpecificationItem *createdAssignmentItem = NULL;
		SpecificationItem *createdSpecificationItem = NULL;
		WordItem *foundWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( generalizationWordItem != NULL )
			{
			if( generalizationWordItem != myWordItem_ )
				{
				if( specificationWordItem != myWordItem_ )
					{
					if( specificationWordItem != NULL &&
					relationWordItem != NULL &&
					!relationWordItem->isNumeralWordType() )
						{
						if( relationContextNr == NO_CONTEXT_NR ||
						// If context number is already taken by the opposite word
						generalizationWordItem->contextItemInWord( relationContextNr ) != NULL )
							{
							if( ( contextResult = adminItem_->getRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, ( firstJustificationItem != NULL && firstJustificationItem->isAssumptionJustification() ), userRelationContextNr_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result == RESULT_OK )
								relationContextNr = contextResult.contextNr;
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a relation context" );
							}
						else
							{
							// Add missing relation context, if specification word is collected with itself
							if( specificationWordItem->isCollectedWithItself() &&
							specificationCollectionNr > NO_COLLECTION_NR &&
							specificationWordItem->isCompoundCollection( specificationCollectionNr ) &&
							( foundWordItem = myWordItem_->contextWordItemInAllWords( relationContextNr, specificationWordItem, NULL ) ) != NULL )
								{
								if( foundWordItem != relationWordItem )
									{
									if( missingRelationContextNr_ > NO_CONTEXT_NR &&
									relationWordItem->hasContextInWord( missingRelationContextNr_, specificationWordItem ) )
										relationContextNr = missingRelationContextNr_;
									else
										{
										if( ( relationContextNr = adminItem_->highestContextNr() ) < MAX_CONTEXT_NR )
											// Create new context number
											missingRelationContextNr_ = ++relationContextNr;
										else
											myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Context number overflow" );
										}
									}
								}
							else
								isRelationContextAlreadyDefined = true;
							}

						if( commonVariables_->result == RESULT_OK &&
						!isSelection )
							{
							if( relationWordItem->addContext( isQuestion, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) == RESULT_OK )
								{
								if( relationWordItem->isUserRelationWord &&
								( foundFeminineOrMasculineSpecificationItem = adminItem_->foundFeminineOrMasculineSpecificationItem() ) != NULL )
									{
									if( foundFeminineOrMasculineSpecificationItem->relationContextNr() == relationContextNr &&
									( createdFeminineOrMasculineSpecificationItem = adminItem_->createdFeminineOrMasculineSpecificationItem() ) != NULL )
										{
										if( relationWordItem->addContext( false, relationWordTypeNr, specificationWordTypeNr, createdFeminineOrMasculineSpecificationItem->relationContextNr(), createdFeminineOrMasculineSpecificationItem->specificationWordItem() ) != RESULT_OK )
											myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a missing feminine or masculine relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );
							}
						}

					if( commonVariables_->result == RESULT_OK )
						{
						mostRecentRelationContextNr_ = relationContextNr;
						existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

						// Skip adding specification if already exists
						if( isAssignment ||
						!isRelationContextAlreadyDefined ||
						existingSpecificationItem == NULL ||
						!existingSpecificationItem->isAssignment() )
							{
							// Check for an assignment or a specification with opposite negative indicator
							if( !isAssignment &&
							isArchivedAssignment_ &&
							generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, !isNegative, isPossessive, questionParameter, specificationWordItem ) != NULL )
								isAssignment = true;

							if( ( specificationResult = addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == RESULT_OK )
								{
								if( !commonVariables_->hasShownWarning )
									{
									isArchivedAssignment_ = isArchivedAssignment;
									replacedAssignmentItem = specificationResult.replacedAssignmentItem;

									if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
										{
										if( !isQuestion &&
										!isSelfGenerated )
											userSpecificationItem_ = createdSpecificationItem;
										}

									if( !isExclusiveSpecification &&
									!isQuestion &&
									!isSelfGenerated &&
									!isSpecificationGeneralization &&
									specificationWordItem != NULL )
										{
										if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME )
											{
											if( createdSpecificationItem != NULL )
												{
												if( isNegative )
													{
													if( createdSpecificationItem->hasSpecificationCollection() )
														{
														if( adminItem_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
															myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an only-option-left assumption" );
														}
													}
												else
													{
													if( adminItem_->collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) == RESULT_OK )
														{
														if( !isPossessive )
															{
															if( adminItem_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) == RESULT_OK )
																{
																if( generalizationWordItem->hasFoundAnswerToQuestion() &&
																!specificationWordItem->isCompoundCollection( createdSpecificationItem->specificationCollectionNr() ) )
																	{
																	if( adminItem_->drawNegativeConclusionsFromAnsweredQuestions( createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
																		myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw negative conclusions from answered questions in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );
													}
												}
											}
										else
											{
											if( isDefinitionSpecification )
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
															if( generalizationWordItem->writeSelectedSpecification( true, ( foundAssignmentItem == NULL ? specificationResult.foundSpecificationItem : foundAssignmentItem ) ) == RESULT_OK )
																{
																if( strlen( commonVariables_->writeSentenceString ) > 0 )
																	{
																	if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( foundAssignmentItem == NULL ? INTERFACE_LISTING_SENTENCE_ASSIGNMENT_CONFLICTS_WITH_DEFINITION_SPECIFICATION : INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) == RESULT_OK )
																		{
																		if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString ) != RESULT_OK )
																			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to write a sentence" );
																		}
																	else
																		myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to write a header" );
																	}
																else
																	myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
																}
															else
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a non-proper-name specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												else
													{
													if( !isAssignment &&
													isPossessive )
														{
														// Draw a reversed "part of" conclusion
														// Not yet explained in the design
														if( adminItem_->addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, true, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
															myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a self-generated reversible part-of specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}

									if( commonVariables_->result == RESULT_OK &&
									!commonVariables_->hasShownWarning &&
									// Skip for example imperative verbs
									adminItem_->isReasoningWordType( generalizationWordTypeNr ) &&

									( ( isAssignment &&
									// At this stage, selections must be stored - rather than executed. So, don't assign them
									!isSelection ) ||

									replacedAssignmentItem != NULL ) )
										{
										if( ( assignmentResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != NULL && specificationWordItem->isAdjectiveAssignedOrClear() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result == RESULT_OK )
											{
											if( ( createdAssignmentItem = assignmentResult.createdSpecificationItem ) != NULL )
												{
												specificationResult.createdSpecificationItem = createdAssignmentItem;

												if( isArchivedAssignment_ &&
												!isQuestion &&
												!isSelfGenerated )
													userSpecificationItem_ = createdAssignmentItem;

												if( replacedAssignmentItem != NULL )
													{
													if( generalizationWordItem->replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( commonVariables_->result == RESULT_OK &&
												!isExclusiveSpecification &&
												isQuestion &&
												relationWordItem != NULL )
													{
													if( adminItem_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, createdAssignmentItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, createdAssignmentItem, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( commonVariables_->result == RESULT_OK &&
												isNegative &&
												!isQuestion &&
												createdAssignmentItem->hasSpecificationCollection() )
													{
													if( adminItem_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem->specificationCollectionNr(), generalizationWordItem ) != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
													}
												}
											}
										else
											myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the specification" );
										}

									if( commonVariables_->result == RESULT_OK &&
									!commonVariables_->hasShownWarning &&
									isDefinitionSpecification &&
									!isExclusiveSpecification &&
									!isQuestion &&
									!isSelfGenerated &&
									!isSpecificationGeneralization &&
									nContextRelations == 0 &&
									generalizationContextNr == NO_CONTEXT_NR &&
									specificationContextNr == NO_CONTEXT_NR &&
									specificationWordItem != NULL &&
									userSpecificationItem_ != NULL )
										{
										if( adminItem_->drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
											myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion" );
										}
									}
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word is the administrator" );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word is the administrator" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
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
