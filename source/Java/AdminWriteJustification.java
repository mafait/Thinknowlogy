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

class AdminWriteJustification
	{
	// Private constructible variables

	private boolean isWritingSeparator_;

	private SpecificationItem previousPrimarySpecificationItem_;

	private WordItem lastWrittenFeminineOrMasculineProperNameEndingWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte writeFeminineOrMasculineProperNameEnding( JustificationItem writeJustificationItem, SpecificationItem primarySpecificationItem )
		{
		WordItem feminineOrMasculineProperNameEndingWordItem;

		if( writeJustificationItem != null )
			{
			if( ( feminineOrMasculineProperNameEndingWordItem = ( primarySpecificationItem != null && primarySpecificationItem.isGeneralizationProperName() && primarySpecificationItem.isPossessive() ? primarySpecificationItem.generalizationWordItem() : writeJustificationItem.myWordItem() ) ) != null )
				{
				if( feminineOrMasculineProperNameEndingWordItem != lastWrittenFeminineOrMasculineProperNameEndingWordItem_ )
					{
					if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WRITE_INDENTED, Constants.INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING, feminineOrMasculineProperNameEndingWordItem.anyWordTypeString(), ( feminineOrMasculineProperNameEndingWordItem.hasFeminineProperNameEnding() ? Constants.INTERFACE_JUSTIFICATION_FEMININE_PROPER_NAME_ENDING : Constants.INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NAME_ENDING ) ) == Constants.RESULT_OK )
						lastWrittenFeminineOrMasculineProperNameEndingWordItem_ = feminineOrMasculineProperNameEndingWordItem;
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification sentence start string" );
					}
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "I couldn't find the feminine or masculine proper name ending word" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given write justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationJustifications( boolean isOnlyWritingPrimarySpecification, boolean isWritingPrimarySpecification, JustificationItem writeJustificationItem )
		{
		boolean isWritingCurrentSpecificationWordOnly;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;
		SpecificationItem primarySpecificationItem = null;
		WordItem generalizationWordItem;
		WordItem primaryGeneralizationWordItem = null;

		if( writeJustificationItem != null )
			{
			if( isWritingPrimarySpecification )
				{
				if( ( primarySpecificationItem = writeJustificationItem.primarySpecificationItem() ) != null )
					{
					if( ( primaryGeneralizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( !primarySpecificationItem.isHiddenSpecification() )
							{
							if( primaryGeneralizationWordItem.writeJustificationSpecification( ( !primarySpecificationItem.isExclusiveSpecification() && !primarySpecificationItem.isPossessive() ), primarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write the definition justification specification" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The primary specification item is a hidden specification" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given primary specification item is undefined" );
					}

				if( !isOnlyWritingPrimarySpecification &&
				( anotherPrimarySpecificationItem = writeJustificationItem.anotherPrimarySpecificationItem() ) != null )
					{
					if( ( generalizationWordItem = anotherPrimarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( !anotherPrimarySpecificationItem.isHiddenSpecification() )
							{
							if( generalizationWordItem.writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write the another definition justification specification" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The another primary specification item is a hidden specification" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given another primary specification item is undefined" );
					}
				}

			if( !isOnlyWritingPrimarySpecification )
				{
				if( ( secondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null )
					{
					if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( !secondarySpecificationItem.isHiddenSpecification() )
							{
							isWritingCurrentSpecificationWordOnly = ( secondarySpecificationItem.isPartOf() ||

																	( !secondarySpecificationItem.isExclusiveSpecification() &&
																	secondarySpecificationItem.isGeneralizationProperName() &&
																	!secondarySpecificationItem.isPossessive() ) );

							if( generalizationWordItem.writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write the secondary justification specification" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The secondary specification item is a hidden specification" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given secondary specification item is undefined" );
					}

				if( ( anotherSecondarySpecificationItem = writeJustificationItem.anotherSecondarySpecificationItem() ) != null )
					{
					if( ( generalizationWordItem = anotherSecondarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( !anotherSecondarySpecificationItem.isHiddenSpecification() )
							{
							if( generalizationWordItem.writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write the another secondary justification specification" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The another secondary specification item is a hidden specification" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given another secondary specification item is undefined" );
					}

				if( isWritingPrimarySpecification &&
				writeJustificationItem.hasFeminineOrMasculineProperNameEnding() )
					{
					if( writeFeminineOrMasculineProperNameEnding( writeJustificationItem, primarySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the feminine or masculine proper name ending" );
					}
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given write justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationSpecifications( boolean isWritingPrimarySpecification, boolean isWritingExtraSeparator, JustificationItem startJustificationItem )
		{
		boolean isExclusiveSecondarySpecification;
		boolean isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion;
		boolean isOnlyWritingPrimarySpecification;
		boolean isQuestionJustification;
		boolean isFirstTime = true;
		boolean isStop = false;
		JustificationItem currentJustificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem previousSecondarySpecificationItem = null;
		WordItem generalizationWordItem;

		if( startJustificationItem != null )
			{
			currentJustificationItem = startJustificationItem;

			do	{
				isExclusiveSecondarySpecification = false;
				isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = false;
				isOnlyWritingPrimarySpecification = false;
				isQuestionJustification = currentJustificationItem.isQuestionJustification();

				primarySpecificationItem = currentJustificationItem.primarySpecificationItem();

				if( ( secondarySpecificationItem = currentJustificationItem.secondarySpecificationItem() ) != null )
					{
					if( secondarySpecificationItem.isExclusiveSpecification() )
						isExclusiveSecondarySpecification = true;
					}

				if( !isQuestionJustification &&
				primarySpecificationItem != null &&
				primarySpecificationItem.hasRelationContext() )
					{
					isWritingPrimarySpecification = true;
					isOnlyWritingPrimarySpecification = ( currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() != null );

					if( primarySpecificationItem.isSpecificationWordCollectedWithItself() &&
					primarySpecificationItem.isPossessiveReversibleConclusion() &&
					primarySpecificationItem.hasSpecificationNonCompoundCollection() &&
					primarySpecificationItem.isPossessive() &&

					( currentJustificationItem.isSpecificationSubstitutionAssumption() ||
					currentJustificationItem.isExclusiveSpecificationSubstitutionAssumption() ) &&

					adminItem_.nContextWordsInAllWords( primarySpecificationItem.relationContextNr(), primarySpecificationItem.specificationWordItem() ) == 1 )
						{
						isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = true;
						isWritingSeparator_ = false;
						}
					}

				if( !isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion )
					{
					if( isWritingPrimarySpecification ||

					( ( !isQuestionJustification &&

					( primarySpecificationItem == null ||

					( isFirstTime &&
					primarySpecificationItem != previousPrimarySpecificationItem_ ) ) ) ||

					( !isExclusiveSecondarySpecification &&
					primarySpecificationItem != null &&
					secondarySpecificationItem != null &&
					previousPrimarySpecificationItem_ != null &&
					!primarySpecificationItem.isCorrectedAssumption() &&
					secondarySpecificationItem != previousSecondarySpecificationItem &&
					primarySpecificationItem.specificationCollectionNr() == previousPrimarySpecificationItem_.specificationCollectionNr() &&

					// Skip possessive secondary specifications without relation
					( !secondarySpecificationItem.isPossessive() ||
					secondarySpecificationItem.hasRelationContext() ) ) ) )
						{
						if( isWritingExtraSeparator )
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_JUSTIFICATION_AND ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an extra justification string" );
							}

						if( writeSpecificationJustifications( isOnlyWritingPrimarySpecification, isWritingPrimarySpecification, currentJustificationItem ) == Constants.RESULT_OK )
							{
							isWritingSeparator_ = true;
							isFirstTime = false;
							isWritingPrimarySpecification = false;
							currentJustificationItem.hasJustificationBeenWritten = true;

							previousPrimarySpecificationItem_ = primarySpecificationItem;
							previousSecondarySpecificationItem = secondarySpecificationItem;

							if( secondarySpecificationItem != null &&
							primarySpecificationItem != null &&
							primarySpecificationItem.isExclusiveSpecification() &&
							primarySpecificationItem.isQuestion() &&
							( generalizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
								{
								if( generalizationWordItem.writeRelatedJustificationSpecifications( currentJustificationItem.justificationTypeNr(), secondarySpecificationItem.specificationCollectionNr() ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write the related justification specifications" );
								}
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the current justification specification" );
						}
					else
						isStop = true;
					}
				}
			while( !isStop &&
			( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given start justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationType( boolean isFirstJustificationType, JustificationItem writeJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isSelfGenerated;
		boolean isWritingPrimarySpecificationOrExtraSeparator;
		boolean isFirstTime = true;
		boolean hasNonExclusiveCollection = false;
		short assumptionLevel;
		int specificationCollectionNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		JustificationItem currentJustificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem currentPrimarySpecificationItem;
		SpecificationItem currentSecondarySpecificationItem;
		SpecificationItem firstSecondarySpecificationItem;
		WordItem generalizationWordItem;
		WordItem secondarySpecificationWordItem;

		previousPrimarySpecificationItem_ = null;
		lastWrittenFeminineOrMasculineProperNameEndingWordItem_ = null;

		if( writeJustificationItem != null )
			{
			if( selfGeneratedSpecificationItem != null )
				{
				if( ( generalizationWordItem = selfGeneratedSpecificationItem.generalizationWordItem() ) != null )
					{
					if( isFirstJustificationType )
						{
						if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification sentence" );
						}

					specificationCollectionNr = selfGeneratedSpecificationItem.specificationCollectionNr();

					if( ( firstSecondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null )
						{
						if( ( secondarySpecificationWordItem = firstSecondarySpecificationItem.specificationWordItem() ) != null )
							hasNonExclusiveCollection = secondarySpecificationWordItem.hasNonExclusiveCollection( specificationCollectionNr );
						}

					if( isWritingSeparator_ )
						{
						if( Presentation.writeInterfaceText( false, !isFirstJustificationType, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isFirstJustificationType ? Constants.INTERFACE_JUSTIFICATION_BECAUSE : Constants.INTERFACE_JUSTIFICATION_AND ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification start string" );
						}

					if( writeJustificationSpecifications( true, false, writeJustificationItem ) == Constants.RESULT_OK )
						{
						if( ( hasNonExclusiveCollection ||
						!writeJustificationItem.isNegativeAssumptionOrConclusion() ) &&

						( currentSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( false, selfGeneratedSpecificationItem.isAssignment(), selfGeneratedSpecificationItem.isInactiveAssignment(), selfGeneratedSpecificationItem.isArchivedAssignment(), selfGeneratedSpecificationItem.questionParameter() ) ) != null )
							{
							isExclusiveSpecification = selfGeneratedSpecificationItem.isExclusiveSpecification();
							isNegative = selfGeneratedSpecificationItem.isNegative();
							isPossessive = selfGeneratedSpecificationItem.isPossessive();
							isSelfGenerated = selfGeneratedSpecificationItem.isSelfGenerated();

							assumptionLevel = selfGeneratedSpecificationItem.assumptionLevel();

							generalizationContextNr = selfGeneratedSpecificationItem.generalizationContextNr();
							specificationContextNr = selfGeneratedSpecificationItem.specificationContextNr();
							relationContextNr = selfGeneratedSpecificationItem.relationContextNr();

							do	{
								// Self-generated
								if( ( currentJustificationItem = currentSpecificationItem.firstJustificationItem() ) != null &&
								currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
									{
									currentPrimarySpecificationItem = currentJustificationItem.primarySpecificationItem();

									if( currentJustificationItem.hasJustificationBeenWritten )
										{
										if( lastWrittenFeminineOrMasculineProperNameEndingWordItem_ != null &&
										currentPrimarySpecificationItem != null )
											{
											if( writeFeminineOrMasculineProperNameEnding( writeJustificationItem, currentPrimarySpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to write the feminine or masculine proper name ending" );
											}
										}
									else
										{
										currentSecondarySpecificationItem = currentJustificationItem.secondarySpecificationItem();

										isWritingPrimarySpecificationOrExtraSeparator = ( currentPrimarySpecificationItem == null ||
																						currentPrimarySpecificationItem.isQuestion() ||

																						( currentSecondarySpecificationItem != null &&

																						( currentSecondarySpecificationItem.isGeneralizationProperName() ||

																						( !currentPrimarySpecificationItem.hasRelationContext() &&
																						currentPrimarySpecificationItem.isGeneralizationProperName() &&
																						!selfGeneratedSpecificationItem.isQuestion() ) ||

																						( isFirstTime &&
																						firstSecondarySpecificationItem != null &&
																						firstSecondarySpecificationItem.specificationCollectionNr() != currentSecondarySpecificationItem.specificationCollectionNr() ) ) ) );

										if( writeJustificationSpecifications( isWritingPrimarySpecificationOrExtraSeparator, isWritingPrimarySpecificationOrExtraSeparator, currentJustificationItem ) == Constants.RESULT_OK )
											isFirstTime = false;
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to write the current justification specifications" );
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
							}
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification specifications" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given self-generated specification item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given write justification is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminWriteJustification( AdminItem adminItem )
		{
		String errorString = null;

		isWritingSeparator_ = false;
		previousPrimarySpecificationItem_ = null;
		lastWrittenFeminineOrMasculineProperNameEndingWordItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte writeJustificationSpecification( SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isFirstJustificationType = true;
		JustificationItem firstJustificationItem;
		JustificationItem currentJustificationItem;
		WordItem generalizationWordItem;

		if( selfGeneratedSpecificationItem != null )
			{
			if( ( firstJustificationItem = selfGeneratedSpecificationItem.firstJustificationItem() ) != null )
				{
				if( ( generalizationWordItem = selfGeneratedSpecificationItem.generalizationWordItem() ) != null )
					{
					isWritingSeparator_ = true;

					generalizationWordItem.clearJustificationHasBeenWritten();
					currentJustificationItem = firstJustificationItem;

					do	{
						if( writeJustificationType( isFirstJustificationType, currentJustificationItem, selfGeneratedSpecificationItem ) == Constants.RESULT_OK )
							isFirstJustificationType = false;
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification type of a specification" );
						}
					while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != null );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given self-generated specification item is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Oh, the joys of those who trust the Lord,
 *	who have no confidence of the proud
 *	or in those who worship idols." (Psalm 40:4)
 *************************************************************************/
