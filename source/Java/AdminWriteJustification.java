/*
 *	Class:			AdminWriteJustification
 *	Supports class:	AdminItem
 *	Purpose:		To write justification reports for the
 *					self-generated knowledge
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

class AdminWriteJustification
	{
	// Private constructible variables

	private SpecificationItem previousPrimarySpecificationItem_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte writeSpecificationJustifications( boolean isWritingPrimarySpecification, boolean isWritingGivenPrimarySpecificationWordOnly, boolean isWritingGivenSecondarySpecificationWordOnly, JustificationItem writeJustificationItem )
		{
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;
		WordItem generalizationWordItem;
		WordItem definitionGeneralizationWordItem = null;

		if( writeJustificationItem != null )
			{
			if( isWritingPrimarySpecification )
				{
				if( ( primarySpecificationItem = writeJustificationItem.primarySpecificationItem() ) != null )
					{
					if( ( definitionGeneralizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( definitionGeneralizationWordItem.writeJustificationSpecification( isWritingGivenPrimarySpecificationWordOnly, primarySpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the definition justification specification" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The primary specification item of the given first justification item has no generalization word" );
					}

				if( ( anotherPrimarySpecificationItem = writeJustificationItem.anotherPrimarySpecificationItem() ) != null )
					{
					if( ( generalizationWordItem = anotherPrimarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( generalizationWordItem.writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the another definition justification specification" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The another primary specification item of the given first justification item has no generalization word" );
					}
				}

			if( ( secondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) != null )
					{
					if( writeJustificationItem.isFeminineOrMasculineProperNameEndingAssumption() )
						{
						if( definitionGeneralizationWordItem != null )
							{
							if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WRITE, Constants.INTERFACE_JUSTIFICATION_SENTENCE_START, Constants.EMPTY_STRING, Constants.INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING, definitionGeneralizationWordItem.anyWordTypeString(), ( generalizationWordItem.isFeminineWord() ? Constants.INTERFACE_JUSTIFICATION_FEMININE_PROPER_NAME_ENDING : Constants.INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NAME_ENDING ) ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification sentence start string" );
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The definition generalization word item is undefined" );
						}
					else
						{
						if( generalizationWordItem.writeJustificationSpecification( isWritingGivenSecondarySpecificationWordOnly, secondarySpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the secondary justification specification" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The secondary specification item of the given first justification item has no generalization word" );
				}

			if( ( anotherSecondarySpecificationItem = writeJustificationItem.anotherSecondarySpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = anotherSecondarySpecificationItem.generalizationWordItem() ) != null )
					{
					if( generalizationWordItem.writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the another secondary justification specification" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The secondary specification item of the given first justification item has no generalization word" );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given write justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationSpecifications( boolean isWritingPrimarySpecification, boolean isWritingExtraSeparator, JustificationItem startJustificationItem )
		{
		boolean isQuestionJustification;
		boolean isExclusivePrimarySpecification;
		boolean isExclusiveSecondarySpecification;
		boolean isPossessivePrimarySpecification;
		boolean isFirstTime = true;
		boolean isStop = false;
		JustificationItem currentJustificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		WordItem definitionGeneralizationWordItem;

		if( startJustificationItem != null )
			{
			currentJustificationItem = startJustificationItem;

			do	{
				isExclusivePrimarySpecification = false;
				isExclusiveSecondarySpecification = false;
				isPossessivePrimarySpecification = false;
				isQuestionJustification = currentJustificationItem.isQuestionJustification();

				if( ( primarySpecificationItem = currentJustificationItem.primarySpecificationItem() ) != null )
					{
					isExclusivePrimarySpecification = primarySpecificationItem.isExclusiveSpecification();
					isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();
					}

				if( ( secondarySpecificationItem = currentJustificationItem.secondarySpecificationItem() ) != null )
					isExclusiveSecondarySpecification = secondarySpecificationItem.isExclusiveSpecification();

				if( isWritingPrimarySpecification ||

				( ( isFirstTime &&
				!isQuestionJustification &&
				primarySpecificationItem != previousPrimarySpecificationItem_ ) ||

				( !isQuestionJustification &&
				!isExclusiveSecondarySpecification ) ||

				( !isExclusiveSecondarySpecification &&
				primarySpecificationItem != null &&
				previousPrimarySpecificationItem_ != null &&
				primarySpecificationItem.specificationCollectionNr() == previousPrimarySpecificationItem_.specificationCollectionNr() ) ) )
					{
					if( isWritingExtraSeparator )
						{
						if( Presentation.writeInterfaceText( false, true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_JUSTIFICATION_AND ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a justification string" );
						}

					if( writeSpecificationJustifications( isWritingPrimarySpecification, ( !isExclusivePrimarySpecification && !isPossessivePrimarySpecification ), !isExclusiveSecondarySpecification, currentJustificationItem ) == Constants.RESULT_OK )
						{
						isFirstTime = false;
						isWritingPrimarySpecification = false;
						currentJustificationItem.hasBeenWritten = true;

						previousPrimarySpecificationItem_ = primarySpecificationItem;

						if( secondarySpecificationItem != null &&
						primarySpecificationItem != null &&
						primarySpecificationItem.isExclusiveSpecification() &&
						primarySpecificationItem.isQuestion() &&
						( definitionGeneralizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
							{
							if( definitionGeneralizationWordItem.writeRelatedJustificationSpecifications( currentJustificationItem.justificationTypeNr(), secondarySpecificationItem.specificationCollectionNr() ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the related justification specifications" );
							}
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the current justification specification" );
					}
				else
					isStop = true;
				}
			while( !isStop &&
			( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given start justification item is undefined" );

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
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification sentence" );
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
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the current justification specifications" );
												}
											}
										}
									while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
									}
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification specifications" );
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification start string" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given self-generated specification item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given write justification is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given justification sentence string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminWriteJustification( WordItem myWordItem )
		{
		String errorString = null;

		previousPrimarySpecificationItem_ = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
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
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a justification type of a specification" );
						}
					while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != null );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given self-generated specification item has no generalization word" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given self-generated specification item is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Oh, the joys of those who trust the Lord,
 *	who have no confidence of the proud
 *	or in those who worship idols." (Psalm 40:4)
 *************************************************************************/
