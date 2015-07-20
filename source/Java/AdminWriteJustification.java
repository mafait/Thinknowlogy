/*
 *	Class:			AdminWriteJustification
 *	Supports class:	AdminItem
 *	Purpose:		To write justification reports for the
 *					self-generated knowledge
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

class AdminWriteJustification
	{
	// Private constructible variables

	private SpecificationItem previousPrimarySpecificationItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte writeSpecificationJustifications( boolean isWritingPrimarySpecification, JustificationItem writeJustificationItem )
		{
		boolean isWritingCurrentSpecificationWordOnly;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;
		SpecificationItem primarySpecificationItem = null;
		WordItem generalizationWordItem;
		WordItem feminineOrMasculineProperNameEndingWordItem = null;

		if( writeJustificationItem != null )
			{
			if( isWritingPrimarySpecification )
				{
				if( ( primarySpecificationItem = writeJustificationItem.primarySpecificationItem() ) != null )
					{
					if( ( generalizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( !primarySpecificationItem.isHiddenSpecification() )
							{
							if( generalizationWordItem.writeJustificationSpecification( ( !primarySpecificationItem.isExclusiveSpecification() && !primarySpecificationItem.isPossessive() ), primarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the definition justification specification" );
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The primary specification item is a hidden specification" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given primary specification item is undefined" );
					}

				if( ( anotherPrimarySpecificationItem = writeJustificationItem.anotherPrimarySpecificationItem() ) != null )
					{
					if( ( generalizationWordItem = anotherPrimarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( !anotherPrimarySpecificationItem.isHiddenSpecification() )
							{
							if( generalizationWordItem.writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the another definition justification specification" );
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The another primary specification item is a hidden specification" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given another primary specification item is undefined" );
					}
				}

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
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the secondary justification specification" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The secondary specification item is a hidden specification" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given secondary specification item is undefined" );
				}

			if( ( anotherSecondarySpecificationItem = writeJustificationItem.anotherSecondarySpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = anotherSecondarySpecificationItem.generalizationWordItem() ) != null )
					{
					if( !anotherSecondarySpecificationItem.isHiddenSpecification() )
						{
						if( generalizationWordItem.writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the another secondary justification specification" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The another secondary specification item is a hidden specification" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given another secondary specification item is undefined" );
				}

			if( isWritingPrimarySpecification &&
			writeJustificationItem.hasFeminineOrMasculineProperNameEnding() )
				{
				if( ( feminineOrMasculineProperNameEndingWordItem = ( primarySpecificationItem != null && secondarySpecificationItem != null && secondarySpecificationItem.isPossessive() ? secondarySpecificationItem.myWordItem() : writeJustificationItem.myWordItem() ) ) != null )
					{
					if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WRITE, Constants.INTERFACE_JUSTIFICATION_SENTENCE_START, Constants.EMPTY_STRING, Constants.INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING, feminineOrMasculineProperNameEndingWordItem.anyWordTypeString(), ( feminineOrMasculineProperNameEndingWordItem.hasFeminineProperNameEnding() ? Constants.INTERFACE_JUSTIFICATION_FEMININE_PROPER_NAME_ENDING : Constants.INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NAME_ENDING ) ) != Constants.RESULT_OK )
						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification sentence start string" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the feminine or masculine proper name ending word" );
				}
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given write justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationSpecifications( boolean isWritingPrimarySpecification, boolean isWritingExtraSeparator, JustificationItem startJustificationItem )
		{
		boolean isQuestionJustification;
		boolean isExclusiveSecondarySpecification;
		boolean isFirstTime = true;
		boolean isStop = false;
		JustificationItem currentJustificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		WordItem generalizationWordItem;

		if( startJustificationItem != null )
			{
			currentJustificationItem = startJustificationItem;

			do	{
				isExclusiveSecondarySpecification = false;
				isQuestionJustification = currentJustificationItem.isQuestionJustification();

				primarySpecificationItem = currentJustificationItem.primarySpecificationItem();

				if( ( secondarySpecificationItem = currentJustificationItem.secondarySpecificationItem() ) != null )
					{
					if( secondarySpecificationItem.isExclusiveSpecification() )
						isExclusiveSecondarySpecification = true;
					}

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
				primarySpecificationItem.specificationCollectionNr() == previousPrimarySpecificationItem_.specificationCollectionNr() &&

				// Skip possessive secondary specifications without relation
				( !secondarySpecificationItem.isPossessive() ||
				secondarySpecificationItem.hasRelationContext() ) ) ) )
					{
					if( isWritingExtraSeparator )
						{
						if( Presentation.writeInterfaceText( false, true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_JUSTIFICATION_AND ) != Constants.RESULT_OK )
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a justification string" );
						}

					if( writeSpecificationJustifications( isWritingPrimarySpecification, currentJustificationItem ) == Constants.RESULT_OK )
						{
						isFirstTime = false;
						isWritingPrimarySpecification = false;
						currentJustificationItem.hasBeenWritten = true;

						previousPrimarySpecificationItem_ = primarySpecificationItem;

						if( secondarySpecificationItem != null &&
						primarySpecificationItem != null &&
						primarySpecificationItem.isExclusiveSpecification() &&
						primarySpecificationItem.isQuestion() &&
						( generalizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
							{
							if( generalizationWordItem.writeRelatedJustificationSpecifications( currentJustificationItem.justificationTypeNr(), secondarySpecificationItem.specificationCollectionNr() ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the related justification specifications" );
							}
						}
					else
						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the current justification specification" );
					}
				else
					isStop = true;
				}
			while( !isStop &&
			( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given start justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationType( boolean isFirstJustificationType, String justificationSentenceString, JustificationItem writeJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
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

		if( justificationSentenceString != null )
			{
			if( writeJustificationItem != null )
				{
				if( selfGeneratedSpecificationItem != null )
					{
					if( ( generalizationWordItem = selfGeneratedSpecificationItem.generalizationWordItem() ) != null )
						{
						if( isFirstJustificationType )
							{
							if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, justificationSentenceString ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification sentence" );
							}

						specificationCollectionNr = selfGeneratedSpecificationItem.specificationCollectionNr();

						if( ( firstSecondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null )
							{
							if( ( secondarySpecificationWordItem = firstSecondarySpecificationItem.specificationWordItem() ) != null )
								hasNonExclusiveCollection = secondarySpecificationWordItem.hasNonExclusiveCollection( specificationCollectionNr );
							}

						if( Presentation.writeInterfaceText( false, !isFirstJustificationType, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isFirstJustificationType ? Constants.INTERFACE_JUSTIFICATION_BECAUSE : Constants.INTERFACE_JUSTIFICATION_AND ) ) == Constants.RESULT_OK )
							{
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
											if( !currentJustificationItem.hasBeenWritten )
												{
												currentPrimarySpecificationItem = currentJustificationItem.primarySpecificationItem();
												currentSecondarySpecificationItem = currentJustificationItem.secondarySpecificationItem();

												isWritingPrimarySpecificationOrExtraSeparator = ( currentPrimarySpecificationItem == null ||
																								currentPrimarySpecificationItem.isQuestion() ||

																								( isFirstTime &&
																								firstSecondarySpecificationItem != null &&
																								currentSecondarySpecificationItem != null &&
																								firstSecondarySpecificationItem.specificationCollectionNr() != currentSecondarySpecificationItem.specificationCollectionNr() &&
																								firstSecondarySpecificationItem.nInvolvedSpecificationWords( false, false ) != currentSecondarySpecificationItem.nInvolvedSpecificationWords( false, false ) ) ||

																								( currentSecondarySpecificationItem != null &&
																								currentSecondarySpecificationItem.isGeneralizationProperName() ) );

												if( writeJustificationSpecifications( isWritingPrimarySpecificationOrExtraSeparator, isWritingPrimarySpecificationOrExtraSeparator, currentJustificationItem ) == Constants.RESULT_OK )
													isFirstTime = false;
												else
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the current justification specifications" );
												}
											}
										}
									while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
									}
								}
							else
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification specifications" );
							}
						else
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification start string" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given self-generated specification item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given write justification is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given justification sentence string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminWriteJustification( AdminItem adminItem )
		{
		String errorString = null;

		previousPrimarySpecificationItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte writeJustificationSpecification( String justificationSentenceString, SpecificationItem selfGeneratedSpecificationItem )
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
					generalizationWordItem.clearJustificationHasBeenWritten();
					currentJustificationItem = firstJustificationItem;

					do	{
						if( writeJustificationType( isFirstJustificationType, justificationSentenceString, currentJustificationItem, selfGeneratedSpecificationItem ) == Constants.RESULT_OK )
							isFirstJustificationType = false;
						else
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a justification type of a specification" );
						}
					while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != null );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given self-generated specification item is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Oh, the joys of those who trust the Lord,
 *	who have no confidence of the proud
 *	or in those who worship idols." (Psalm 40:4)
 *************************************************************************/
