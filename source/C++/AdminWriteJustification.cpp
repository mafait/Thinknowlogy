/*
 *	Class:			AdminWriteJustification
 *	Supports class:	AdminItem
 *	Purpose:		To write justification reports for the
 *					self-generated knowledge
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
#include "Presentation.cpp"
#include "SpecificationItem.cpp"

class AdminWriteJustification
	{
	friend class AdminItem;

	// Private constructible variables

	bool isWritingSeparator_;

	SpecificationItem *previousPrimarySpecificationItem_;

	WordItem *lastWrittenFeminineOrMasculineProperNameEndingWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType writeFeminineOrMasculineProperNameEnding( JustificationItem *writeJustificationItem, SpecificationItem *primarySpecificationItem )
		{
		WordItem *feminineOrMasculineProperNameEndingWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeFeminineOrMasculineProperNameEnding";

		if( writeJustificationItem != NULL )
			{
			if( ( feminineOrMasculineProperNameEndingWordItem = ( primarySpecificationItem != NULL && primarySpecificationItem->isGeneralizationProperName() && primarySpecificationItem->isPossessive() ? primarySpecificationItem->generalizationWordItem() : writeJustificationItem->myWordItem() ) ) != NULL )
				{
				if( feminineOrMasculineProperNameEndingWordItem != lastWrittenFeminineOrMasculineProperNameEndingWordItem_ )
					{
					if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WRITE_INDENTED, INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING, feminineOrMasculineProperNameEndingWordItem->anyWordTypeString(), ( feminineOrMasculineProperNameEndingWordItem->hasFeminineProperNameEnding() ? INTERFACE_JUSTIFICATION_FEMININE_PROPER_NAME_ENDING : INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NAME_ENDING ) ) == RESULT_OK )
						lastWrittenFeminineOrMasculineProperNameEndingWordItem_ = feminineOrMasculineProperNameEndingWordItem;
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification sentence start string" );
					}
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the feminine or masculine proper name ending word" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSpecificationJustifications( bool isOnlyWritingPrimarySpecification, bool isWritingPrimarySpecification, JustificationItem *writeJustificationItem )
		{
		bool isWritingCurrentSpecificationWordOnly;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		SpecificationItem *primarySpecificationItem = NULL;
		WordItem *generalizationWordItem;
		WordItem *primaryGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationJustifications";

		if( writeJustificationItem != NULL )
			{
			if( isWritingPrimarySpecification )
				{
				if( ( primarySpecificationItem = writeJustificationItem->primarySpecificationItem() ) != NULL )
					{
					if( ( primaryGeneralizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( !primarySpecificationItem->isHiddenSpecification() )
							{
							if( primaryGeneralizationWordItem->writeJustificationSpecification( ( !primarySpecificationItem->isExclusiveSpecification() && !primarySpecificationItem->isPossessive() ), primarySpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the definition justification specification" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The primary specification item is a hidden specification" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given primary specification item is undefined" );
					}

				if( !isOnlyWritingPrimarySpecification &&
				( anotherPrimarySpecificationItem = writeJustificationItem->anotherPrimarySpecificationItem() ) != NULL )
					{
					if( ( generalizationWordItem = anotherPrimarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( !anotherPrimarySpecificationItem->isHiddenSpecification() )
							{
							if( generalizationWordItem->writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the another definition justification specification" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The another primary specification item is a hidden specification" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given another primary specification item is undefined" );
					}
				}

			if( !isOnlyWritingPrimarySpecification )
				{
				if( ( secondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL )
					{
					if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( !secondarySpecificationItem->isHiddenSpecification() )
							{
							isWritingCurrentSpecificationWordOnly = ( secondarySpecificationItem->isPartOf() ||

																	( !secondarySpecificationItem->isExclusiveSpecification() &&
																	secondarySpecificationItem->isGeneralizationProperName() &&
																	!secondarySpecificationItem->isPossessive() ) );

							if( generalizationWordItem->writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the secondary justification specification" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The secondary specification item is a hidden specification" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given secondary specification item is undefined" );
					}

				if( ( anotherSecondarySpecificationItem = writeJustificationItem->anotherSecondarySpecificationItem() ) != NULL )
					{
					if( ( generalizationWordItem = anotherSecondarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( !anotherSecondarySpecificationItem->isHiddenSpecification() )
							{
							if( generalizationWordItem->writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the another secondary justification specification" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The another secondary specification item is a hidden specification" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given another secondary specification item is undefined" );
					}

				if( isWritingPrimarySpecification &&
				writeJustificationItem->hasFeminineOrMasculineProperNameEnding() )
					{
					if( writeFeminineOrMasculineProperNameEnding( writeJustificationItem, primarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the feminine or masculine proper name ending" );
					}
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeJustificationSpecifications( bool isWritingPrimarySpecification, bool isWritingExtraSeparator, JustificationItem *startJustificationItem )
		{
		bool isExclusiveSecondarySpecification;
		bool isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion;
		bool isOnlyWritingPrimarySpecification;
		bool isQuestionJustification;
		bool isFirstTime = true;
		bool isStop = false;
		JustificationItem *currentJustificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *previousSecondarySpecificationItem = NULL;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecifications";

		if( startJustificationItem != NULL )
			{
			currentJustificationItem = startJustificationItem;

			do	{
				isExclusiveSecondarySpecification = false;
				isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = false;
				isOnlyWritingPrimarySpecification = false;
				isQuestionJustification = currentJustificationItem->isQuestionJustification();

				primarySpecificationItem = currentJustificationItem->primarySpecificationItem();

				if( ( secondarySpecificationItem = currentJustificationItem->secondarySpecificationItem() ) != NULL )
					{
					if( secondarySpecificationItem->isExclusiveSpecification() )
						isExclusiveSecondarySpecification = true;
					}

				if( !isQuestionJustification &&
				primarySpecificationItem != NULL &&
				primarySpecificationItem->hasRelationContext() )
					{
					isWritingPrimarySpecification = true;
					isOnlyWritingPrimarySpecification = ( currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() != NULL );

					if( primarySpecificationItem->isSpecificationWordCollectedWithItself() &&
					primarySpecificationItem->isPossessiveReversibleConclusion() &&
					primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
					primarySpecificationItem->isPossessive() &&

					( currentJustificationItem->isSpecificationSubstitutionAssumption() ||
					currentJustificationItem->isExclusiveSpecificationSubstitutionAssumption() ) &&

					adminItem_->nContextWordsInAllWords( primarySpecificationItem->relationContextNr(), primarySpecificationItem->specificationWordItem() ) == 1 )
						{
						isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = true;
						isWritingSeparator_ = false;
						}
					}

				if( !isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion )
					{
					if( isWritingPrimarySpecification ||

					( ( !isQuestionJustification &&

					( primarySpecificationItem == NULL ||

					( isFirstTime &&
					primarySpecificationItem != previousPrimarySpecificationItem_ ) ) ) ||

					( !isExclusiveSecondarySpecification &&
					primarySpecificationItem != NULL &&
					secondarySpecificationItem != NULL &&
					previousPrimarySpecificationItem_ != NULL &&
					!primarySpecificationItem->isCorrectedAssumption() &&
					secondarySpecificationItem != previousSecondarySpecificationItem &&
					primarySpecificationItem->specificationCollectionNr() == previousPrimarySpecificationItem_->specificationCollectionNr() &&

					// Skip possessive secondary specifications without relation
					( !secondarySpecificationItem->isPossessive() ||
					secondarySpecificationItem->hasRelationContext() ) ) ) )
						{
						if( isWritingExtraSeparator )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_JUSTIFICATION_AND ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an extra justification string" );
							}

						if( writeSpecificationJustifications( isOnlyWritingPrimarySpecification, isWritingPrimarySpecification, currentJustificationItem ) == RESULT_OK )
							{
							isWritingSeparator_ = true;
							isFirstTime = false;
							isWritingPrimarySpecification = false;
							currentJustificationItem->hasJustificationBeenWritten = true;

							previousPrimarySpecificationItem_ = primarySpecificationItem;
							previousSecondarySpecificationItem = secondarySpecificationItem;

							if( secondarySpecificationItem != NULL &&
							primarySpecificationItem != NULL &&
							primarySpecificationItem->isExclusiveSpecification() &&
							primarySpecificationItem->isQuestion() &&
							( generalizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL )
								{
								if( generalizationWordItem->writeRelatedJustificationSpecifications( currentJustificationItem->justificationTypeNr(), secondarySpecificationItem->specificationCollectionNr() ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the related justification specifications" );
								}
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the current justification specification" );
						}
					else
						isStop = true;
					}
				}
			while( !isStop &&
			( currentJustificationItem = currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start justification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeJustificationType( bool isFirstJustificationType, JustificationItem *writeJustificationItem, SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isSelfGenerated;
		bool isWritingPrimarySpecificationOrExtraSeparator;
		bool isFirstTime = true;
		bool hasNonExclusiveCollection = false;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		JustificationItem *currentJustificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *currentPrimarySpecificationItem;
		SpecificationItem *currentSecondarySpecificationItem;
		SpecificationItem *firstSecondarySpecificationItem;
		WordItem *generalizationWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationType";

		previousPrimarySpecificationItem_ = NULL;
		lastWrittenFeminineOrMasculineProperNameEndingWordItem_ = NULL;

		if( writeJustificationItem != NULL )
			{
			if( selfGeneratedSpecificationItem != NULL )
				{
				if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) != NULL )
					{
					if( isFirstJustificationType )
						{
						if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification sentence" );
						}

					specificationCollectionNr = selfGeneratedSpecificationItem->specificationCollectionNr();

					if( ( firstSecondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL )
						{
						if( ( secondarySpecificationWordItem = firstSecondarySpecificationItem->specificationWordItem() ) != NULL )
							hasNonExclusiveCollection = secondarySpecificationWordItem->hasNonExclusiveCollection( specificationCollectionNr );
						}

					if( isWritingSeparator_ )
						{
						if( commonVariables_->presentation->writeInterfaceText( false, !isFirstJustificationType, PRESENTATION_PROMPT_NOTIFICATION, ( isFirstJustificationType ? INTERFACE_JUSTIFICATION_BECAUSE : INTERFACE_JUSTIFICATION_AND ) ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification start string" );
						}

					if( writeJustificationSpecifications( true, false, writeJustificationItem ) == RESULT_OK )
						{
						if( ( hasNonExclusiveCollection ||
						!writeJustificationItem->isNegativeAssumptionOrConclusion() ) &&

						( currentSpecificationItem = generalizationWordItem->firstSelectedSpecificationItem( false, selfGeneratedSpecificationItem->isAssignment(), selfGeneratedSpecificationItem->isInactiveAssignment(), selfGeneratedSpecificationItem->isArchivedAssignment(), selfGeneratedSpecificationItem->questionParameter() ) ) != NULL )
							{
							isExclusiveSpecification = selfGeneratedSpecificationItem->isExclusiveSpecification();
							isNegative = selfGeneratedSpecificationItem->isNegative();
							isPossessive = selfGeneratedSpecificationItem->isPossessive();
							isSelfGenerated = selfGeneratedSpecificationItem->isSelfGenerated();

							assumptionLevel = selfGeneratedSpecificationItem->assumptionLevel();

							generalizationContextNr = selfGeneratedSpecificationItem->generalizationContextNr();
							specificationContextNr = selfGeneratedSpecificationItem->specificationContextNr();
							relationContextNr = selfGeneratedSpecificationItem->relationContextNr();

							do	{
								// Self-generated
								if( ( currentJustificationItem = currentSpecificationItem->firstJustificationItem() ) != NULL &&
								currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
									{
									currentPrimarySpecificationItem = currentJustificationItem->primarySpecificationItem();

									if( currentJustificationItem->hasJustificationBeenWritten )
										{
										if( lastWrittenFeminineOrMasculineProperNameEndingWordItem_ != NULL &&
										currentPrimarySpecificationItem != NULL )
											{
											if( writeFeminineOrMasculineProperNameEnding( writeJustificationItem, currentPrimarySpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the feminine or masculine proper name ending" );
											}
										}
									else
										{
										currentSecondarySpecificationItem = currentJustificationItem->secondarySpecificationItem();

										isWritingPrimarySpecificationOrExtraSeparator = ( currentPrimarySpecificationItem == NULL ||
																						currentPrimarySpecificationItem->isQuestion() ||

																						( currentSecondarySpecificationItem != NULL &&

																						( currentSecondarySpecificationItem->isGeneralizationProperName() ||

																						( !currentPrimarySpecificationItem->hasRelationContext() &&
																						currentPrimarySpecificationItem->isGeneralizationProperName() &&
																						!selfGeneratedSpecificationItem->isQuestion() ) ||

																						( isFirstTime &&
																						firstSecondarySpecificationItem != NULL &&
																						firstSecondarySpecificationItem->specificationCollectionNr() != currentSecondarySpecificationItem->specificationCollectionNr() ) ) ) );

										if( writeJustificationSpecifications( isWritingPrimarySpecificationOrExtraSeparator, isWritingPrimarySpecificationOrExtraSeparator, currentJustificationItem ) == RESULT_OK )
											isFirstTime = false;
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the current justification specifications" );
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
							}
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification specifications" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminWriteJustification( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		isWritingSeparator_ = false;
		previousPrimarySpecificationItem_ = NULL;
		lastWrittenFeminineOrMasculineProperNameEndingWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminWriteJustification" );

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

	ResultType writeJustificationSpecification( SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isFirstJustificationType = true;
		JustificationItem *firstJustificationItem;
		JustificationItem *currentJustificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( selfGeneratedSpecificationItem != NULL )
			{
			if( ( firstJustificationItem = selfGeneratedSpecificationItem->firstJustificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) != NULL )
					{
					isWritingSeparator_ = true;

					generalizationWordItem->clearJustificationHasBeenWritten();
					currentJustificationItem = firstJustificationItem;

					do	{
						if( writeJustificationType( isFirstJustificationType, currentJustificationItem, selfGeneratedSpecificationItem ) == RESULT_OK )
							isFirstJustificationType = false;
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification type of a specification" );
						}
					while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != NULL );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Oh, the joys of those who trust the Lord,
 *	who have no confidence of the proud
 *	or in those who worship idols." (Psalm 40:4)
 *************************************************************************/
