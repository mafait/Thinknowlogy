/*	Class:			AdminReasoningOld
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (old design)
 *	Version:		Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

class AdminReasoningOld
	{
	// Private constructed variables

	private int postponeNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

	private SpecificationItem alternativeSpanishAnotherPrimarySpecificationItem_ = null;
	private SpecificationItem oppositePossessiveDefinitionSpecificationItem_ = null;
	private SpecificationItem possessiveDefinitionSpecificationItem_ = null;
	private SpecificationItem postponeNegativeConclusionAnotherPrimarySpecificationItem_ = null;
	private SpecificationItem postponeNegativeConclusionPrimarySpecificationItem_ = null;
	private SpecificationItem spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;
	private SpecificationItem spanishAmbiguousCompoundPrimarySpecificationItem_ = null;

	private WordItem feminineSpecificationWordItem_ = null;
	private WordItem oppositePossessiveSpecificationWordItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private void clearPostponedNegativeConclusionVariables()
		{
		postponeNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		postponeNegativeConclusionPrimarySpecificationItem_ = null;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = null;
		}

	private static boolean isFeminineOrMasculineProperNounEnding( boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isFemaleSpecificationWord;
		boolean isMaleSpecificationWord;

		if( generalizationWordItem != null &&
		specificationWordItem != null )
			{
			isFemaleSpecificationWord = specificationWordItem.isFemale();
			isMaleSpecificationWord = specificationWordItem.isMale();

			if( isFemaleSpecificationWord ||
			isMaleSpecificationWord )
						// Possessive (relation)
				return ( ( isPossessive &&
						relationWordItem != null &&

						( ( isFemaleSpecificationWord &&
						relationWordItem.hasFeminineProperNounEnding() ) ||

						( isMaleSpecificationWord &&
						relationWordItem.hasMasculineProperNounEnding() ) ) ) ||

						// Not possessive (generalization)
						( !isPossessive &&

						( ( isFemaleSpecificationWord &&
						generalizationWordItem.hasFeminineProperNounEnding() ) ||

						( isMaleSpecificationWord &&

						( generalizationWordItem.hasMasculineProperNounEnding() ||

						// Typical for Spanish
						( specificationWordItem.isNounWordSpanishAmbiguous() &&
						generalizationWordItem.hasFeminineProperNounEnding() ) ) ) ) ) );
			}

		return false;
		}

	private byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isArchivedAssignment, boolean isNegativeConclusion, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean isCommonWordWordSpanishAmbiguous;
		boolean isFemaleUserSpecificationWord = false;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isMaleUserSpecificationWord = false;
		boolean isPrimaryUserSpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		int anotherPrimarySpecificationCollectionNr;
		int nonCompoundSpecificationCollectionNr;
		int specificationCollectionNr;
		SpecificationItem anotherSecondarySpecificationItem = null;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem firstNonCompoundCollectionSpecificationItem;
		SpecificationItem questionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem anotherPrimarySpecificationWordItem;
		WordItem collectionWordItem = null;
		WordItem commonWordItem;
		WordItem userSpecificationWordItem;
		RelatedResultType relatedResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given another primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem.specificationCollectionNr() ) == Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given another primary specification item has no specification collection" );

		if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given another primary specification item has no specification word item" );

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		if( ( commonWordItem = anotherPrimarySpecificationWordItem.commonWordItem( anotherPrimarySpecificationCollectionNr ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the common word of the given specification collection number" );

		if( ( specificationCollectionNr = commonWordItem.collectionNr( specificationWordItem ) ) == Constants.NO_COLLECTION_NR )
			specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
		isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();
		isCommonWordWordSpanishAmbiguous = commonWordItem.isNounWordSpanishAmbiguous();
		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		relatedSpecificationItem == null &&
		!userSpecificationItem.isPossessive() &&
		( nonCompoundSpecificationCollectionNr = commonWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR )
			{
			if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

		if( relatedSpecificationItem == null )
			{
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem.hasMasculineProperNounEnding();

			if( userSpecificationItem != primarySpecificationItem &&
			!userSpecificationItem.hasRelationContext() &&
			!userSpecificationItem.isExclusiveSpecification() &&
			GlobalVariables.nUserSpecificationWords == 1 &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
				{
				if( userSpecificationWordItem.isFemale() )
					isFemaleUserSpecificationWord = true;

				if( userSpecificationWordItem.isMale() )
					isMaleUserSpecificationWord = true;
				}

			if( isFemaleUserSpecificationWord ||
			isMaleUserSpecificationWord ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&

			( hasGeneralizationWordFeminineProperNounEnding ||
			userSpecificationItem.isQuestion() ) ) )
				{
				if( isFemaleUserSpecificationWord ||
				isMaleUserSpecificationWord )
					{
					if( anotherPrimarySpecificationItem.hasCompoundSpecificationCollection() &&

					( anotherPrimarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ||

					( isFemaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem.isMale() ) ||

					( isMaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem.isFemale() ) ) &&

					// Draw negative conclusion
					drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					if( !isHiddenSpanishPrimarySpecification )
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
				if( ( ( !hasGeneralizationWordFeminineProperNounEnding &&
				!hasGeneralizationWordMasculineProperNounEnding ) ||

				( primarySpecificationItem == userSpecificationItem &&
				!anotherPrimarySpecificationWordItem.isFemaleOrMale() ) ||

				primarySpecificationItem.hasCompoundSpecificationCollection() ) &&

				( !isSpecificationWordSpanishAmbiguous ||
				anotherPrimarySpecificationItem.hasNonCompoundSpecificationCollection() ||
				anotherPrimarySpecificationWordItem.compoundCollectionNr() == anotherPrimarySpecificationCollectionNr ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem );
					else
						{
						if( !isNegativeConclusion )
							collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr );
						}

					// Skip invalid questions (step 2)
					if( ( !isNegativeConclusion &&
					collectionWordItem != null &&
					generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, collectionWordItem ) == null ) ||

					( isSpecificationWordSpanishAmbiguous &&

					( existingSpecificationItem == null ||
					existingSpecificationItem.isHiddenSpanishSpecification() ||
					generalizationWordItem.isUserGeneralizationWord ) ) )
						{
						if( isSpecificationWordSpanishAmbiguous &&
						isPrimaryUserSpecification &&
						!primarySpecificationItem.isExclusiveSpecification() )
							{
							postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;

							postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
							postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
							}
						else
							{
							if( ( isPrimaryUserSpecification ||
							primarySpecificationItem.hasRelationContext() ||
							generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == null ) &&

							adminItem_.askQuestions( true ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );
							}
						}
					}
				}
			}
		else
			{
			if( anotherPrimarySpecificationItem.isExclusiveSpecification() )
				{
				if( !isCommonWordWordSpanishAmbiguous &&
				( firstNonCompoundCollectionSpecificationItem = generalizationWordItem.firstNonCompoundCollectionSpecificationItem( anotherPrimarySpecificationCollectionNr ) ) != null )
					relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

				if( postponeNegativeConclusionSpecificationCollectionNr_ == anotherPrimarySpecificationCollectionNr &&
				// Draw postponed negative conclusion
				drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				clearPostponedNegativeConclusionVariables();

				// Typical for Spanish
				if( isCommonWordWordSpanishAmbiguous &&
				isSpecificationWordSpanishAmbiguous &&
				// specificationWordItem == commonWordItem &&
				GlobalVariables.nUserSpecificationWords == 1 &&
				!primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
				primarySpecificationItem.specificationCollectionNr() != anotherPrimarySpecificationCollectionNr &&

				( isPrimaryUserSpecification ||
				generalizationWordItem.isUserRelationWord ) )
					{
					questionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, anotherPrimarySpecificationWordItem );

					if( ( questionSpecificationItem == null ||
					questionSpecificationItem.isOlderItem() ) &&

					generalizationWordItem.firstSpecificationItem( false, false, false, anotherPrimarySpecificationWordItem ) == null &&

					// Ask Spanish ambiguous questions
					adminItem_.askQuestions( false ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );
					}
				else
					{
					if( anotherPrimarySpecificationItem.hasNonCompoundSpecificationCollection() ||
					anotherPrimarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ||
					anotherPrimarySpecificationWordItem.compoundCollectionNr() == anotherPrimarySpecificationCollectionNr )
						{
						if( relatedSpecificationItem.isOlderItem() &&
						relatedSpecificationItem.isSelfGeneratedAssumption() &&
						( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, relatedSpecificationItem.specificationWordItem(), userSpecificationItem.generalizationWordItem() ) ) != null )
							relatedSpecificationItem = conclusionSpecificationItem;

						if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( isHiddenSpanishPrimarySpecification &&
						specificationWordItem == commonWordItem &&
						anotherPrimarySpecificationWordItem == commonWordItem &&
						userSpecificationItem != relatedSpecificationItem &&
						!relatedSpecificationItem.hasRelationContext() &&
						relatedSpecificationItem.isSelfGenerated() )
							{
							spanishAmbiguousCompoundPrimarySpecificationItem_ = relatedSpecificationItem;
							spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNounEnding, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationItem possessiveSpecificationItem;
		WordItem userSpecificationWordItem;

		alternativeSpanishAnotherPrimarySpecificationItem_ = null;

		if( userSpecificationItem != null )
			{
			if( generalizationWordItem.isUserRelationWord &&
			userSpecificationItem.isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNounEnding &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&
			// Try to find a possessive specification
			( possessiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, userSpecificationWordItem ) ) != null )
				{
				alternativeSpanishAnotherPrimarySpecificationItem_ = oppositePossessiveDefinitionSpecificationItem_;
				return possessiveSpecificationItem;
				}
			}

		return null;
		}

	private static WordItem collectionWordItemInCollectionWords( int compoundCollectionNr, WordItem commonWordItem )
		{
		WordItem currentCollectionWordItem;

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null &&
		( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem.hasCollectionNr( compoundCollectionNr, commonWordItem ) )
					return currentCollectionWordItem;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return null;
		}

	private FindSpecificationResultType findPossessiveSpecifications( WordItem generalizationWordItem, WordItem possessiveNounWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem foundPossessiveDefinitionSpecificationItem = null;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem higherLevelPossessiveSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		FindSpecificationResultType findSpecificationResult = new FindSpecificationResultType();

		if( generalizationWordItem == null )
			return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( possessiveNounWordItem == null )
			return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The given possessive noun word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		// First try to find a possessive specification on a higher level
		if( ( possessiveDefinitionSpecificationItem = possessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
			{
			do	{
				if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
				possessiveSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) != null )
					{
					if( higherLevelPossessiveSpecificationWordItem == null )
						higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
					else
						hasFoundPossessiveSpecificationWord = false;
					}
				}
			while( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
				{
				if( higherLevelPossessiveSpecificationWordItem == null )
					higherLevelPossessiveSpecificationWordItem = specificationWordItem;

				// Try to find the opposite of the possessive specification
				do	{
					if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( possessiveSpecificationWordItem == generalizationWordItem )
							return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );

						if( previousPossessiveSpecificationItem != null )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem.specificationCollectionNr();

							if( higherLevelPossessiveSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ?
																							previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ?
																				possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
							}

						previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
						}
					}
				while( foundPossessiveDefinitionSpecificationItem == null &&
				( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );
				}
			}

		findSpecificationResult.foundPossessiveDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
		findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;
		return findSpecificationResult;
		}


	// Constructor

	protected AdminReasoningOld( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReasoningOldVariables()
		{
		clearPostponedNegativeConclusionVariables();

		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		}

	protected byte checkForUnprocessedNegativeConclusion()
		{
		if( postponeNegativeConclusionSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR )
			{
			if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an unprocessed negative conclusion: postponeNegativeConclusionSpecificationCollectionNr_: " + postponeNegativeConclusionSpecificationCollectionNr_ + ";\n\tpostponeNegativeConclusionPrimarySpecificationItem_: " + ( postponeNegativeConclusionPrimarySpecificationItem_ == null ? Constants.EMPTY_STRING : postponeNegativeConclusionPrimarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) ) + ";\n\tpostponeNegativeConclusionAnotherPrimarySpecificationItem_: " + ( postponeNegativeConclusionAnotherPrimarySpecificationItem_ == null ? Constants.EMPTY_STRING : postponeNegativeConclusionAnotherPrimarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) ) + ".\n" ) != Constants.RESULT_OK )
				return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasAnotherSecondarySpecificationWithCompoundCollection = false;
		boolean hasCurrentlyMoreSpecificNonExclusiveSpecification;
		boolean hasFoundOldSpanishUserSpecification = false;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasRelatedSpecificationsInSameSentence = false;
		boolean isCompoundUserSpecification = false;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isPrimarySameAsSecondarySpecification;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationSelected = false;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		int anotherPrimarySpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int nonCompoundSpecificationCollectionNr;
		int secondarySpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem negativeAssumptionJustificationItem;
		JustificationItem negativeAssumptionOrConclusionJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingNegativeSpecificationItem;
		SpecificationItem existingPositiveSpecificationItem;
		SpecificationItem foundNegativeAssumptionOrConclusionPrimarySpecificationItem = null;
		SpecificationItem foundSpecificationItem;
		SpecificationItem negativeConclusionAnotherPrimarySpecificationItem;
		SpecificationItem negativeConclusionPrimarySpecificationItem;
		SpecificationItem negativeConclusionAnotherSecondarySpecificationItem;
		SpecificationItem negativeConclusionSecondarySpecificationItem;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem anotherPrimarySpecificationWordItem;
		WordItem collectionWordItem;
		WordItem compoundGeneralizationWordItem = null;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationCommonWordItem = null;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationItem.isPossessive() ||
		primarySpecificationItem.isQuestion() )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item shouldn't be possessive, nor a question" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( anotherPrimarySpecificationItem == null )
			anotherPrimarySpecificationWordItem = primarySpecificationWordItem;
		else
			{
			anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem.specificationCollectionNr();
			anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem.specificationWordItem();
			}

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		// If exists, replace self-generated assumption for conclusion
		if( primarySpecificationItem.isSelfGeneratedAssumption() &&
		( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem.isArchivedAssignment(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), false, primarySpecificationWordItem, null ) ) != null )
			primarySpecificationItem = tempSpecificationItem;

		existingPositiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, anotherPrimarySpecificationWordItem );

		hasCurrentlyMoreSpecificNonExclusiveSpecification = ( primarySpecificationItem == existingPositiveSpecificationItem &&
															primarySpecificationItem.isExclusiveSpecification() );
		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != null )
			foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem.primarySpecificationItem();

		if( ( hasCurrentlyMoreSpecificNonExclusiveSpecification ||
		// No positive specification exists with this specification word
		existingPositiveSpecificationItem == null ||
		existingPositiveSpecificationItem.isHiddenSpanishSpecification() ) &&

		// Prefer primary specification with the least uncertainty
		( foundNegativeAssumptionOrConclusionPrimarySpecificationItem == null ||
		!primarySpecificationItem.hasSpecificationCollection() ||
		primarySpecificationItem.assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem.assumptionLevel() ) &&

		// No question exists
		generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, true, anotherPrimarySpecificationWordItem ) == null )
			{
			if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

			if( !secondarySpecificationItem.isOlderItem() &&
			secondarySpecificationItem.isSelfGeneratedAssumption() &&
			( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem, null ) ) != null )
				{
				// Add specification collection to specification
				if( ( createAndAssignResult = generalizationWordItem.copySpecificationItem( secondarySpecificationItem.specificationCollectionNr(), tempSpecificationItem.firstJustificationItem(), tempSpecificationItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to copy a specification without specification collection" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I couldn't create a specification" );

				if( generalizationWordItem.replaceOrDeleteSpecification( tempSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a specification without specification collection in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				secondarySpecificationItem = createdSpecificationItem;

				if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, secondarySpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an updated specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
			isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

			if( isPrimarySameAsSecondarySpecification )
				{
				if( hasPrimarySpecificationCompoundCollection &&
				primarySpecificationItem.specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
					{
					if( ( collectionWordItem = collectionWordItemInCollectionWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem ) ) != null &&
					( anotherSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem ) ) == null )
						anotherSecondarySpecificationItem = ( !userSpecificationItem.isSpecificationWordSpanishAmbiguous() ?
																userSpecificationItem : primarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem ) );
					}
				else
					{
					if( GlobalVariables.nUserSpecificationWords > 1 &&
					GlobalVariables.nUserRelationWords == 0 &&
					userSpecificationItem.isSpecificationWordSpanishAmbiguous() )
						hasRelatedSpecificationsInSameSentence = true;
					}
				}
			else
				{
				if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
					{
					if( isHiddenSpanishPrimarySpecification &&
					anotherPrimarySpecificationItem != null &&
					anotherPrimarySpecificationItem.hasCompoundSpecificationCollection() &&
					anotherPrimarySpecificationItem.isSelfGenerated() &&
					generalizationWordItem.isUserRelationWord &&
					( collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != null )
						{
						if( ( nonCompoundSpecificationCollectionNr = collectionWordItem.nonCompoundCollectionNr() ) == Constants.NO_COLLECTION_NR )
							nonCompoundSpecificationCollectionNr = anotherPrimarySpecificationWordItem.nonCompoundCollectionNr();

						if( ( tempSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, isArchivedAssignment, false, false, nonCompoundSpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) != null )
							{
							hasFoundOldSpanishUserSpecification = true;
							secondarySpecificationItem = tempSpecificationItem;
							}
						}

					if( !hasFoundOldSpanishUserSpecification &&
					secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
					secondarySpecificationItem.hasCompoundSpecificationCollection() &&
					( anotherSecondarySpecificationItem = secondarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) == null &&
					( secondarySpecificationCommonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null &&
					secondarySpecificationCommonWordItem != primarySpecificationWordItem )
						anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem.firstSpecificationItem( false, false, false, secondarySpecificationWordItem );
					}
				}

			if( isUserGeneralizationWord &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
				{
				if( userSpecificationWordItem.isNounWordSpanishAmbiguous() )
					isUserSpecificationWordSpanishAmbiguous = true;

				if( !isHiddenSpanishPrimarySpecification ||

				( isUserSpecificationWordSpanishAmbiguous ||
				userSpecificationItem.isPossessive() ) )
					{
					isUserSpecificationSelected = true;

					if( !isPrimarySameAsSecondarySpecification &&
					anotherPrimarySpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
					anotherSecondarySpecificationItem != null &&
					userSpecificationItem.hasCompoundSpecificationCollection() &&
					userSpecificationWordItem.hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
						isCompoundUserSpecification = true;
					}
				else
					{
					if( !anotherPrimarySpecificationWordItem.isNounWordSpanishAmbiguous() &&
					( simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null )
						{
						isUserSpecificationSelected = true;
						userSpecificationItem = simpleUserSpecificationItem;

						// Hidden
						if( anotherSecondarySpecificationItem != null )
							hasAnotherSecondarySpecificationWithCompoundCollection = true;
						}
					}

				if( isUserSpecificationSelected &&
				userSpecificationItem.isPossessive() )
					oppositePossessiveDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
				}

			existingNegativeSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, true, false, false, anotherPrimarySpecificationWordItem );

			if( hasFoundOldSpanishUserSpecification ||
			isCompoundUserSpecification ||

			( ( existingNegativeSpecificationItem == null ||

			( !isHiddenSpanishPrimarySpecification &&

			( hasPrimarySpecificationCompoundCollection ||
			existingNegativeSpecificationItem.isSelfGeneratedAssumption() ) &&

			( existingNegativeSpecificationItem.isOlderItem() ||
			existingNegativeSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ) &&

			generalizationWordItem.primarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, primarySpecificationItem ) == null ) ||

			// User specification is selected
			( isUserSpecificationSelected &&

			( ( !primarySpecificationItem.hasRelationContext() &&

			( existingNegativeSpecificationItem == null ||
			existingNegativeSpecificationItem.isOlderItem() ) ) ||

			( isHiddenSpanishPrimarySpecification &&
			existingNegativeSpecificationItem != null &&

			( existingNegativeSpecificationItem.hasNonCompoundSpecificationCollection() ||

			( existingNegativeSpecificationItem.isOlderItem() &&
			secondarySpecificationItem.hasRelationContext() ) ) ) ) ) )
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
							negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ?
																						anotherSecondarySpecificationItem : null );
							}
						else
							{
							if( ( isUserSpecificationWordSpanishAmbiguous &&
							userSpecificationItem.hasNonCompoundSpecificationCollection() ) ||

							( anotherPrimarySpecificationItem != null &&
							anotherPrimarySpecificationItem.hasCompoundSpecificationCollection() ) )
								{
								negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
								negativeConclusionAnotherPrimarySpecificationItem = null;
								negativeConclusionSecondarySpecificationItem = secondarySpecificationItem;
								negativeConclusionAnotherSecondarySpecificationItem = anotherSecondarySpecificationItem;
								}
							else
								{
								negativeConclusionPrimarySpecificationItem = userSpecificationItem;
								negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
								negativeConclusionSecondarySpecificationItem = anotherPrimarySpecificationItem;
								negativeConclusionAnotherSecondarySpecificationItem = secondarySpecificationItem;
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
					if( hasPrimarySpecificationCompoundCollection &&
					!isPrimarySameAsSecondarySpecification &&
					isUserSpecificationSelected &&
					primarySpecificationItem.isSelfGenerated() &&
					!userSpecificationItem.isQuestion() &&
					userSpecificationItem.hasCompoundSpecificationCollection() )
						compoundGeneralizationWordItem = anotherPrimarySpecificationWordItem.compoundGeneralizationWordItem( anotherPrimarySpecificationCollectionNr );

					if( compoundGeneralizationWordItem != null &&
					compoundGeneralizationWordItem.isNounWordSpanishAmbiguous() )
						{
						negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, false, anotherPrimarySpecificationWordItem );
						negativeConclusionAnotherPrimarySpecificationItem = null;
						negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
						negativeConclusionAnotherSecondarySpecificationItem = null;
						}
					else
						{
						if( hasCurrentlyMoreSpecificNonExclusiveSpecification )
							{
							negativeConclusionPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? anotherSecondarySpecificationItem : generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, secondarySpecificationCommonWordItem ) );
							negativeConclusionAnotherPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? null : anotherSecondarySpecificationItem );
							negativeConclusionSecondarySpecificationItem = userSpecificationItem;
							negativeConclusionAnotherSecondarySpecificationItem = null;
							}
						else
							{
							negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
							negativeConclusionAnotherPrimarySpecificationItem = ( hasRelatedSpecificationsInSameSentence ? null : anotherPrimarySpecificationItem );

							negativeConclusionSecondarySpecificationItem = ( isCompoundUserSpecification ? userSpecificationItem :

																			( isPrimarySameAsSecondarySpecification ? ( hasRelatedSpecificationsInSameSentence ? anotherPrimarySpecificationItem :

																			( primarySpecificationItem != userSpecificationItem &&
																			!userSpecificationItem.hasRelationContext() &&
																			userSpecificationItem.specificationCollectionNr() == anotherPrimarySpecificationCollectionNr ? userSpecificationItem : anotherSecondarySpecificationItem ) ) : secondarySpecificationItem ) );

							negativeConclusionAnotherSecondarySpecificationItem = ( isCompoundUserSpecification ||
																					isPrimarySameAsSecondarySpecification ? null : anotherSecondarySpecificationItem );
							}
						}
					}

				// Draw negative conclusion
				if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, primarySpecificationItem.isExclusiveSpecification(), false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + anotherPrimarySpecificationWordItem.anyWordTypeString() + "\"" );

				createdJustificationItem = createAndAssignResult.createdJustificationItem;

				// Found specification
				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
					{
					if( createdJustificationItem != null )
						{
						// Cleanup after found specification
						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "The found specification item is undefined" );

						if( negativeAssumptionOrConclusionJustificationItem == null )
							{
							if( foundSpecificationItem.isSelfGeneratedAssumption() )
								{
								if( createdJustificationItem.isConclusionJustification() &&
								foundSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
								}
							else
								{
								if( generalizationWordItem.isJustificationInUse( createdJustificationItem ) )
									{
									// Check for negative assumption justification
									negativeAssumptionJustificationItem = foundSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION );

									if( negativeAssumptionJustificationItem != null &&
									negativeAssumptionJustificationItem.primarySpecificationItem() == negativeConclusionPrimarySpecificationItem &&
									generalizationWordItem.replaceJustification( negativeAssumptionJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption justification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( generalizationWordItem.replaceOrDeleteJustification( createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to delete the created justification" );
									}
								}
							}
						else
							{
							// Replace justification of negative conclusion / assumption
							if( generalizationWordItem.replaceJustification( negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				else
					{
					// Created specification
					// Replace corrected negative conclusion
					if( generalizationWordItem.hasCurrentlyCorrectedAssumptionByKnowledge() )
						{
						if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a negative conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created negative conclusion" );

						relatedSpecificationItem = relatedResult.relatedSpecificationItem;

						if( relatedSpecificationItem != null &&
						// Replace a corrected negative conclusion
						generalizationWordItem.replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected negative conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						if( isCompoundUserSpecification &&
						// Change justification of negative assumptions
						generalizationWordItem.changeJustificationOfNegativeAssumptions( userSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace a question justification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( boolean hasCurrentlyMoreSpecificNonExclusiveSpecification, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		JustificationItem currentJustificationItem;
		SpecificationItem answeredQuestionSpecificationItem;
		WordItem answeredQuestionSpecificationWordItem;
		WordItem userSpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( answeredQuestionSpecificationItem = generalizationWordItem.firstRecentlyAnsweredQuestionSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any answered question specification" );

		do	{
			if( userSpecificationWordItem.hasCollectionNr( answeredQuestionSpecificationItem.specificationCollectionNr() ) )
				{
				if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no specification word" );

				// Check if already exists as positive specification
				if( hasCurrentlyMoreSpecificNonExclusiveSpecification ||
				generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, answeredQuestionSpecificationWordItem ) == null )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem.firstJustificationItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem.hasPossessivePrimarySpecification() &&
						// Draw negative conclusion from answered question
						drawNegativeConclusion( answeredQuestionSpecificationItem.isArchivedAssignment(), answeredQuestionSpecificationItem.generalizationWordTypeNr(), answeredQuestionSpecificationItem.specificationWordTypeNr(), answeredQuestionSpecificationItem.generalizationContextNr(), answeredQuestionSpecificationItem.specificationContextNr(), currentJustificationItem.primarySpecificationItem(), currentJustificationItem.secondarySpecificationItem(), userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem.recentlyAnsweredQuestionSpecificationItem( false ) ) != null );

		return Constants.RESULT_OK;
		}

	protected byte drawSimpleNegativeSpanishConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		int nonCompoundSpecificationCollectionNr;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem simpleUserSpecificationItem;
		WordItem collectionWordItem;
		WordItem commonWordItem = null;
		WordItem userSpecificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( !specificationWordItem.isNounWordSpanishAmbiguous() &&
		( simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null &&
		( userSpecificationWordItem = simpleUserSpecificationItem.specificationWordItem() ) != null &&
		( nonCompoundSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
		( collectionWordItem = specificationWordItem.collectionWordItem( nonCompoundSpecificationCollectionNr ) ) != null )
			{
			if( collectionWordItem.isNounWordSpanishAmbiguous() )
				anotherPrimarySpecificationItem = collectionWordItem.firstSpecificationItem( false, false, false, collectionWordItem );
			else
				{
				if( ( commonWordItem = userSpecificationWordItem.commonWordItem( simpleUserSpecificationItem.specificationCollectionNr() ) ) != null &&
				commonWordItem.isNounWordSpanishAmbiguous() )
					anotherPrimarySpecificationItem = commonWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, false, true, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, collectionWordItem );
				}

			if( anotherPrimarySpecificationItem != null &&
			anotherPrimarySpecificationItem.hasSpecificationCollection() &&
			// Ask specification substitution question, or draw negative conclusion
			askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isArchivedAssignment, boolean isExclusiveSpecification, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCurrentlyCorrectedAssumptionByKnowledge;
		boolean hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasCurrentSpecificationCollection;
		boolean hasCurrentSpecificationCompoundCollection;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean hasPrimarySpecificationNonCompoundCollection;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasUserRelationContext = false;
		boolean hasUserSpecificationRelationContext = false;
		boolean isArchivedUserAssignment = false;
		boolean isDefinitionSpanishAmbiguous;
		boolean isExclusiveCurrentSpecification;
		boolean isExclusiveUserSpecification = false;
		boolean isFemaleOrMaleGeneralizationWord;
		boolean isFoundPrimaryHiddenSpanishSpecification;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isGeneralizationWordUserRelationWord;
		boolean isIncludingAdjectives = false;
		boolean isIncludingRelationWord;
		boolean isNegativeConclusion = false;
		boolean isPossessiveUserSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		boolean isUserQuestion = false;
		boolean isUserSpecificationWord;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short currentSpecificationWordTypeNr;
		short selectedSpecificationWordTypeNr;
		short selectedRelationWordTypeNr;
		int currentSpecificationCollectionNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem tempJustification;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem alternativeSpanishAnotherPrimarySpecificationItem;
		SpecificationItem archivedAssignmentItem;
		SpecificationItem bestMatchingRelationSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAnotherPrimarySpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem spanishAmbiguousSpecificationItem = null;
		SpecificationItem userSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem selectedRelationWordItem;
		WordItem userSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a specification reasoning word type" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem.hasCurrentlyCorrectedAssumptionByKnowledge();

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
			{
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			hasUserRelationContext = userSpecificationItem.hasRelationContext();
			isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserQuestion = userSpecificationItem.isQuestion();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
			userSpecificationWordItem = userSpecificationItem.specificationWordItem();

			isIncludingAdjectives = ( !isExclusiveSpecification &&
									!hasCurrentlyCorrectedAssumptionByKnowledge &&
									!userSpecificationItem.isQuestion() );
			}

		if( ( currentSpecificationItem = specificationWordItem.firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != null )
			{
			hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();

			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem.hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem.hasMasculineProperNounEnding();

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			isFemaleOrMaleGeneralizationWord = generalizationWordItem.isFemaleOrMale();
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isUserSpecificationWord = specificationWordItem.isUserSpecificationWord;

			if( isSpecificationWordSpanishAmbiguous )
				spanishAmbiguousSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					isExclusiveCurrentSpecification = currentSpecificationItem.isExclusiveSpecification();
					currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR );

					if( ( bestMatchingRelationSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) != null &&
					!isQuestion &&
					!bestMatchingRelationSpecificationItem.isExclusiveSpecification() )
						isNegativeConclusion = true;

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, specificationWordItem, relationWordItem ) ) == null )
						foundPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, isQuestion, specificationWordItem );

					if( foundPrimarySpecificationItem == null )
						{
						// Check for specification conflict
						if( currentSpecificationItem.isUserSpecification() &&
						generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, false, false, currentSpecificationCollectionNr, currentSpecificationItem.relationContextNr(), currentSpecificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to check for a specification conflict in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem.isHiddenSpanishSpecification();

						if( !GlobalVariables.hasDisplayedWarning &&
						isExclusiveCurrentSpecification &&
						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem.firstFeminineOrMasculineSpecificationItem() ) != null )
							{
							isIncludingRelationWord = ( hasRelationWord &&
														feminineOrMasculineSpecificationItem.hasSpecificationCollection() &&

														( isFoundPrimaryHiddenSpanishSpecification ||
														foundPrimarySpecificationItem.hasNewInformation() ) );

							if( !isFemaleOrMaleGeneralizationWord &&

							( isIncludingRelationWord ||

							( !hasRelationWord &&
							!isPossessiveUserSpecification ) ||

							( isPossessiveUserSpecification &&

							( !isUserGeneralizationWord ||
							foundPrimarySpecificationItem.isOlderItem() ) ) ) &&

							// Matching feminine word
							( ( ( hasGeneralizationWordFeminineProperNounEnding &&
							currentSpecificationWordItem.isFemale() ) ||

							// Matching masculine word
							( hasGeneralizationWordMasculineProperNounEnding &&
							currentSpecificationWordItem.isMale() ) ) &&

							( ( isFoundPrimaryHiddenSpanishSpecification &&
							isUserGeneralizationWord ) ||

							( !isFoundPrimaryHiddenSpanishSpecification &&

							( ( !isUserSpecificationWordSpanishAmbiguous &&
							!hasGeneralizationWordActiveQuestionWithCompoundCollection ) ||

							( !isPossessiveUserSpecification &&
							foundPrimarySpecificationItem.hasRelationContext() ) ) ) ) &&

							// Select correct compound word to avoid assumption to be made if a question should be asked
							currentSpecificationWordItem.compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem ) )
								{
								if( isIncludingRelationWord ||

								( isUserGeneralizationWord &&
								foundPrimarySpecificationItem.hasNewInformation() &&

								( !hasUserSpecificationRelationContext ||
								foundPrimarySpecificationItem.hasNonCompoundSpecificationCollection() ) ) ||

								( !isUserGeneralizationWord &&

								( hasRelationWord ||
								generalizationWordItem.isOlderItem() ) &&

								oppositePossessiveDefinitionSpecificationItem_ != null &&
								oppositePossessiveDefinitionSpecificationItem_.specificationCollectionNr() == userSpecificationCollectionNr ) )
									{
									selectedRelationWordTypeNr = ( isIncludingRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR );
									selectedRelationWordItem = ( isIncludingRelationWord ? relationWordItem : null );
									selectedPrimarySpecificationItem = ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem );
									selectedAnotherPrimarySpecificationItem = ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem_ : null );

									// Make a feminine or masculine proper noun ending assumption
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAnotherPrimarySpecificationItem, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

									if( !isUserSpecificationWordSpanishAmbiguous &&
									( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
										{
										// Draw missing negative conclusions
										if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									clearPostponedNegativeConclusionVariables();
								}
							}

						if( ( bestMatchingRelationSpecificationItem == null &&

						( hasCurrentSpecificationCollection ||
						!foundPrimarySpecificationItem.hasSpecificationCollection() ) ) ||

						( !isExclusiveCurrentSpecification &&

						( isGeneralizationProperNoun ||

						( hasCurrentSpecificationCollection &&
						isDefinitionSpanishAmbiguous ) ) ) ||

						( isDefinitionSpanishAmbiguous &&
						hasCurrentSpecificationCollection &&
						!isUserSpecificationWord &&
						!currentSpecificationItem.isOlderItem() ) )
							{
							hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem.hasNonCompoundSpecificationCollection();

							if( ( !isExclusiveSpecification &&
							isExclusiveCurrentSpecification ) ||

							( isGeneralizationProperNoun &&

							( ( isExclusiveSpecification &&
							hasCurrentSpecificationCollection ) ||

							( isSpecificationWordSpanishAmbiguous &&
							!isFoundPrimaryHiddenSpanishSpecification &&

							( ( isExclusiveSpecification &&

							( isGeneralizationWordUserRelationWord ||
							isPossessiveUserSpecification ) ) ) ) ) ) )
								{
								hasCurrentSpecificationCompoundCollection = ( currentSpecificationWordItem.compoundCollectionNr() > Constants.NO_COLLECTION_NR );

								// Ask a question or draw a negative conclusion
								if( isGeneralizationProperNoun &&
								hasCurrentSpecificationCollection &&
								userSpecificationItem != null &&

								// Specification isn't Spanish ambiguous
								( ( !isSpecificationWordSpanishAmbiguous &&

								( !hasCurrentSpecificationCompoundCollection ||
								!hasGeneralizationWordActiveQuestionWithCompoundCollection ||
								isExclusiveUserSpecification ||
								isUserSpecificationWordSpanishAmbiguous ||
								foundPrimarySpecificationItem.hasSpecificationCollection() ) ) ||

								// Specification is Spanish ambiguous
								( isSpecificationWordSpanishAmbiguous &&

								( hasPrimarySpecificationNonCompoundCollection ||
								!isExclusiveCurrentSpecification ||
								isNegativeConclusion ||

								( isUserGeneralizationWord &&
								!isExclusiveUserSpecification &&
								currentSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ) ||

								// Question
								hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ||
								adminItem_.hasSuggestiveQuestionAssumption() ) )
									{
									if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							else
								{
								currentSpecificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();

								// Make an assumption or draw a conclusion
								if( ( isAssumption ||
								currentSpecificationItem.hasAssumptionLevel() ||

								( relationWordItem != null &&
								!isSpanishCurrentLanguage &&
								foundPrimarySpecificationItem.hasRelationContext() &&
								relationWordItem.hasCurrentlyConfirmedSpecification() ) ) &&

								currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE )
									{
									alternativeSpanishAnotherPrimarySpecificationItem = null;
									primarySpecificationItem = null;

									if( isSpecificationWordSpanishAmbiguous )
										primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );
									else
										{
										primarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null );

										if( primarySpecificationItem == null ||
										primarySpecificationItem .isUserAssignment() )
											primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );
										}

									existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, currentSpecificationWordItem );

									if( primarySpecificationItem != null )
										{
										alternativePrimarySpecificationItem = ( !isSpecificationWordSpanishAmbiguous ? ( hasRelationWord ? userSpecificationItem : foundPrimarySpecificationItem ) :
																					alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) );

										if( ( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem.isHiddenSpanishSpecification() ) ||

										( existingSpecificationItem != null &&
										alternativePrimarySpecificationItem != null &&
										alternativePrimarySpecificationItem.specificationWordItem() == specificationWordItem &&

										( ( isSpecificationWordSpanishAmbiguous &&
										( existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) ||

										( primarySpecificationItem.hasRelationContext() &&
										generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) != null ) ) ) ||

										( !isSpecificationWordSpanishAmbiguous &&
										!existingSpecificationItem.hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&
										!specificationWordItem.isCollectionSpanishAmbiguous( alternativePrimarySpecificationItem.specificationCollectionNr() ) ) ) ) )
											{
											if( alternativePrimarySpecificationItem != null )
												alternativeSpanishAnotherPrimarySpecificationItem = alternativeSpanishAnotherPrimarySpecificationItem_;

											if( !isArchivedUserAssignment &&
											isSpecificationWordSpanishAmbiguous &&
											isGeneralizationWordUserRelationWord &&
											alternativeSpanishAnotherPrimarySpecificationItem != null &&
											existingSpecificationItem != null )
												{
												if( ( primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) ) != null &&
												// Check if justification already exists
												existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) )
													{
													if( primarySpecificationItem == alternativePrimarySpecificationItem )
														{
														if( ( tempJustification = foundPrimarySpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != null &&
														primarySpecificationItem != tempJustification.primarySpecificationItem() )
															primarySpecificationItem = tempJustification.primarySpecificationItem();
														}
													else
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												}
											else
												primarySpecificationItem = alternativePrimarySpecificationItem;
											}
										else
											{
											if( isPossessiveUserSpecification &&
											primarySpecificationItem.hasAssumptionLevel() &&
											( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, currentSpecificationWordItem ) ) != null &&
											existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null )
												// Skip if similar justification already exists, without relation word
												primarySpecificationItem = null;
											}
										}

									if( primarySpecificationItem != null &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||

									( ( currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR ||
									existingSpecificationItem != null ) &&

									primarySpecificationItem.isPossessive() ) ) &&

									// No existing specification
									( ( existingSpecificationItem == null &&

									( !hasUserRelationContext ||
									isUserQuestion ||
									currentSpecificationItem.isUserSpecification() ||
									!currentSpecificationItem.hasSpecificationCollection() ) ) ||

									// Existing specification
									( existingSpecificationItem != null &&
									!existingSpecificationItem.hasRelationContext() &&
									existingSpecificationItem.isSelfGenerated() &&
									// Skip uncertain justifications
									( primarySpecificationItem.assumptionLevel() + currentSpecificationItem.assumptionLevel() ) <= existingSpecificationItem.assumptionLevel() ) ) )
										{
										archivedAssignmentItem = null;

										if( isArchivedUserAssignment &&
										userSpecificationItem != null &&
										userSpecificationItem.isGeneralizationNoun() )
											{
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										selectedSecondarySpecificationItem = ( archivedAssignmentItem == null ? currentSpecificationItem : archivedAssignmentItem );

										// Make a compound specification substitution assumption, which is actually an uncertain conclusion
										// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
										//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeSpanishAnotherPrimarySpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

										if( createAndAssignResult.createdJustificationItem != null &&
										( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
										!foundSpecificationItem.isReplacedItem() &&
										!foundSpecificationItem.hasCurrentCreationSentenceNr() &&
										// Cleanup unreferenced justifications
										generalizationWordItem.cleanupUnreferencedJustifications() != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to clean up unreferenced justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									{
									if( ( !isExclusiveSpecification &&
									bestMatchingRelationSpecificationItem == null &&

									( !isSpecificationWordSpanishAmbiguous ||

									( !isFoundPrimaryHiddenSpanishSpecification &&
									spanishAmbiguousSpecificationItem != null &&
									spanishAmbiguousSpecificationItem.hasCurrentCreationSentenceNr() ) ) &&

									// Skip if already exists as user specification
									generalizationWordItem.firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) ||

									// Check if exclusive conclusion already exists
									( isExclusiveSpecification &&
									generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, true, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) )
										{
										if( ( primarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, specificationWordItem ) ) != null &&

										( isGeneralizationProperNoun ||

										( primarySpecificationItem.isUserSpecification() &&

										( !isExclusiveSpecification ||
										currentSpecificationWordItem.isFemaleOrMale() ||
										!currentSpecificationItem.hasCompoundSpecificationCollection() ) ) ) )
											{
											selectedSpecificationWordTypeNr = ( currentSpecificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? Constants.WORD_TYPE_NOUN_SINGULAR : currentSpecificationWordTypeNr );

											// Draw a compound specification substitution conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, selectedSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
												{
												if( hasCurrentlyCorrectedAssumptionByKnowledge )
													{
													if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created specification" );

													if( relatedResult.relatedSpecificationItem != null &&
													// Replace incorrect assumption
													generalizationWordItem.replaceOrDeleteSpecification( relatedResult.relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( isGeneralizationProperNoun )
														{
														if( isSpanishCurrentLanguage &&
														!hasRelationWord &&
														hasCurrentSpecificationCollection &&
														!isSpecificationWordSpanishAmbiguous &&
														primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
														// Draw simple negative Spanish conclusions
														drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( !createdSpecificationItem.hasCompoundSpecificationCollection() &&
														// Make a specification substitution 'part of' assumption
														makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}
									}
								}

							if( !isExclusiveCurrentSpecification &&
							isGeneralizationProperNoun &&
							hasCurrentSpecificationCollection &&

							( hasCurrentlyCorrectedAssumptionByKnowledge ||
							isUserSpecificationWordSpanishAmbiguous ) &&

							// Make generalization assumption by generalization
							adminItem_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption by generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUncountableGeneralizationNoun, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFeminineOrMasculineProperNounEnding;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification = false;
		boolean hasRelationWordFeminineOrMasculineProperNounEnding = false;
		boolean isGeneralizationWordUserRelationWord;
		boolean isHiddenPrimarySpecification;
		boolean isHiddenSecondarySpecification;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isReplaceHiddenSpecification;
		short selectedRelationWordTypeNr;
		int nonCompoundSpanishSpecificationCollectionNr;
		int relationContextNr;
		JustificationItem olderJustificationItem;
		SpecificationItem additionalSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem compoundSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem;
		SpecificationItem foundPossessiveDefinitionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem possessiveSpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem = null;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAnotherPrimarySpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem commonWordItem;
		WordItem currentPossessiveNounWordItem;
		WordItem feminineSpecificationWordItem;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;

		oppositePossessiveDefinitionSpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;

		oppositePossessiveSpecificationWordItem_ = null;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, relationWordItem );

		// Typical for Spanish
		if( specificationWordItem.isNounWordSpanishAmbiguous() &&
		relationWordItem != null &&
		primarySpecificationItem != null &&
		primarySpecificationItem.hasCompoundSpecificationCollection() &&
		primarySpecificationItem.isSelfGenerated() &&
		( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
		( tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, nonCompoundSpanishSpecificationCollectionNr, specificationWordItem ) ) != null &&
		( relationContextNr = tempSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
		relationWordItem.hasContextInWord( relationContextNr, specificationWordItem ) )
			// Spanish non-compound specification
			primarySpecificationItem = tempSpecificationItem;

		if( primarySpecificationItem != null &&

		( !primarySpecificationItem.isOlderItem() ||

		( relationWordItem != null &&

		( primarySpecificationItem.isSelfGenerated() ||
		!relationWordItem.isOlderItem() ) ) ) &&

		( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
			isHiddenPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();

			if( relationWordItem != null )
				{
				hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();
				hasRelationWordFeminineOrMasculineProperNounEnding = relationWordItem.hasFeminineOrMasculineProperNounEnding();
				}

			// Do for all possessive noun words
			do	{
				if( currentPossessiveNounWordItem != specificationWordItem &&
				currentPossessiveNounWordItem != generalizationWordItem )
					{
					if( ( findSpecificationResult = findPossessiveSpecifications( generalizationWordItem, currentPossessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );

					isReplaceHiddenSpecification = false;
					anotherPrimarySpecificationItem = null;
					createdOrFoundSpecificationItem = null;
					foundOppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem;
					foundPossessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem;

					if( foundPossessiveDefinitionSpecificationItem != null &&
					( possessiveDefinitionSpecificationWordItem = foundPossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						oppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem;
						possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem;

						if( ( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem.firstSpecificationItem( isPossessive, false, false, specificationWordItem ) ) != null )
							{
							isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

							if( primarySpecificationItem != null &&

							( ( !primarySpecificationItem.hasRelationContext() &&
							generalizationWordItem.hadOnceAnsweredSelfGeneratedQuestion() ) ||

							( isPossessiveSpecificationWordSpanishAmbiguous &&
							primarySpecificationItem.specificationWordItem() == possessiveDefinitionSpecificationWordItem ) ) )
								{
								// Draw a specification substitution conclusion or ask a question
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( true, isArchivedAssignment, isExclusiveSpecification, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem );

								if( ( existingUserSpecificationItem == null &&

								( !isPossessiveSpecificationWordSpanishAmbiguous ||
								!isNegative ) ) ||

								( hasRelationWord &&
								existingUserSpecificationItem != null &&

								// Confirmed specification without relation word
								( !existingUserSpecificationItem.hasRelationContext() ||
								!existingUserSpecificationItem.hasSpecificationCollection() ) ) )
									{
									hasFeminineOrMasculineProperNounEnding = false;

									if( isPossessiveSpecificationWordSpanishAmbiguous &&
									isFeminineOrMasculineProperNounEnding( isPossessive, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) )
										hasFeminineOrMasculineProperNounEnding = true;

									if( !hasRelationWord &&
									!isNegative &&
									( possessiveSpecificationWordItem = possessiveSpecificationItem.specificationWordItem() ) != null &&
									( commonWordItem = possessiveSpecificationWordItem.commonWordItem( possessiveSpecificationItem.specificationCollectionNr() ) ) != null &&
									commonWordItem != generalizationWordItem &&
									commonWordItem.isFemaleOrMale() &&
									!commonWordItem.isNounWordSpanishAmbiguous() &&
									( definitionSpecificationItem = commonWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, possessiveSpecificationWordItem ) ) != null &&
									// Make generalization assumption
									adminItem_.makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + commonWordItem.anyWordTypeString() + "\"" );

									selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR );

									// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, foundPossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

									createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
									foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

									if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) == null )
										return adminItem_.startError( 1, moduleNameString_, "No specification item was created or found" );

									secondarySpecificationItem = createdOrFoundSpecificationItem.updatedSpecificationItem();

									if( createdSpecificationItem != null &&
									isPossessiveSpecificationWordSpanishAmbiguous )
										{
										isReplaceHiddenSpecification = true;

										if( spanishAmbiguousCompoundPrimarySpecificationItem_ != null )
											primarySpecificationItem = spanishAmbiguousCompoundPrimarySpecificationItem_;

										if( spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ != null )
											anotherPrimarySpecificationItem = spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
										}
									}
								}
							}
						}
					else
						{
						if( hasRelationWord &&
						primarySpecificationItem != null &&
						( foundSpecificationItem = currentPossessiveNounWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
						// Uncountable generalization noun. Example: "Water ...".
						adminItem_.addSelfGeneratedSpecification( false, primarySpecificationItem.isAssignment(), false, false, false, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );
						}

					// First opposite possessive specification
					if( !GlobalVariables.hasDisplayedWarning &&
					primarySpecificationItem != null &&
					possessiveDefinitionSpecificationWordItem != null &&
					foundOppositePossessiveDefinitionSpecificationItem != null &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( isNegative ||
						createdOrFoundSpecificationItem == null ||
						primarySpecificationItem.hasSpecificationContext() )
							{
							secondarySpecificationItem = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
						secondarySpecificationItem != null &&

						( !primarySpecificationItem.isOlderItem() ||
						primarySpecificationItem.isSelfGenerated() ) )
							{
							isHiddenSecondarySpecification = secondarySpecificationItem.isHiddenSpanishSpecification();
							existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, false, oppositePossessiveSpecificationWordItem_ );

							if( !isHiddenSecondarySpecification &&
							hasGeneralizationWordCurrentlyConfirmedSpecification &&
							existingSpecificationItem != null &&
							existingSpecificationItem.hasCompoundSpecificationCollection() &&
							existingSpecificationItem.hasOnlyOneRelationWord() &&
							( tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, !isPossessive, false, foundOppositePossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) ) != null )
								existingSpecificationItem = tempSpecificationItem;

							if( ( existingSpecificationItem == null ||
							!existingSpecificationItem.hasRelationContext() ||

							( ( hasRelationWord &&
							isGeneralizationWordUserRelationWord &&
							isPossessive &&

							( existingSpecificationItem.isSelfGeneratedAssumption() ||

							( existingSpecificationItem.isHiddenSpanishSpecification() &&
							relationWordItem != null &&
							relationWordItem.hasCurrentlyAnsweredSelfGeneratedQuestion() ) ) &&

							( hasRelationWordConfirmedSpecification ||
							!hasRelationWordFeminineOrMasculineProperNounEnding ||
							anotherPrimarySpecificationItem != null ) ) ||

							( secondarySpecificationItem.hasRelationContext() &&

							( !secondarySpecificationItem.isSelfGeneratedConclusion() ||
							secondarySpecificationItem.isHiddenSpanishSpecification() ) &&

							( !isHiddenSecondarySpecification ||

							( primarySpecificationItem.isUserSpecification() &&
							!generalizationWordItem.hasCurrentlyMoreSpecificSpecification() ) ||

							// Typical for Spanish
							// Self-generated assignment
							primarySpecificationItem.isAssignment() ) &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							!existingSpecificationItem.isOlderItem() ||

							( anotherPrimarySpecificationItem == null &&

							( primarySpecificationItem == secondarySpecificationItem ||
							existingSpecificationItem.isSelfGeneratedAssumption() ) ) ||

							( relationWordItem != null &&
							!relationWordItem.isOlderItem() ) ) ) ) ) &&

							// User specification doesn't exist
							generalizationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == null &&
							// No justification exist as an opposite possessive conditional specification assumption
							generalizationWordItem.secondarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null )
								{
								isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_.isNounWordSpanishAmbiguous();
								selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR );
								primarySpecificationItem = primarySpecificationItem.updatedSpecificationItem();
								selectedPrimarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ?
																		( isHiddenSecondarySpecification ? primarySpecificationItem : secondarySpecificationItem ) :
																											foundOppositePossessiveDefinitionSpecificationItem );
								selectedAnotherPrimarySpecificationItem = ( !isReplaceHiddenSpecification &&
																			isOppositePossessiveSpecificationWordSpanishAmbiguous ? null : anotherPrimarySpecificationItem );
								selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem : secondarySpecificationItem );

								// Make an exclusive specification substitution assumption (opposite)
								// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, foundOppositePossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAnotherPrimarySpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

								if( isPossessive )
									{
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										{
										if( hasRelationWord &&
										primarySpecificationItem.isSelfGenerated() &&
										( anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) ) != null )
											{
											hasFeminineOrMasculineProperNounEnding = false;
											feminineSpecificationWordItem = null;

											if( isFeminineOrMasculineProperNounEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
												{
												hasFeminineOrMasculineProperNounEnding = true;

												if( feminineSpecificationWordItem_ == null )
													{
													if( existingSpecificationItem == null )
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem();
													}
												else
													feminineSpecificationWordItem = feminineSpecificationWordItem_;

												if( feminineSpecificationWordItem != null &&
												adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, anotherPrimarySpecificationItem, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
												}

											if( ( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, false, anotherPrimarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// See paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
												if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

												if( feminineSpecificationWordItem_ != null &&
												adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), null, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem_.anyWordTypeString() + "\"" );
												}
											}
										}
									else
										{
										if( foundSpecificationItem == null )
											secondarySpecificationItem = createdSpecificationItem;
										else
											{
											// Found, but not created
											if( createdSpecificationItem == null )
												{
												secondarySpecificationItem = foundSpecificationItem.updatedSpecificationItem();

												if( hasRelationWord &&
												anotherPrimarySpecificationItem != null &&
												primarySpecificationItem.isUserSpecification() &&
												( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != null &&
												additionalSpecificationItem.hasOnlyOneRelationWord() &&

												// Add exclusive specification substitution justification
												adminItem_.addJustificationToSpecification( false, false, false, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, additionalSpecificationItem, anotherPrimarySpecificationItem, null, primarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add an exclusive specification substitution justification to the additional specification" );
												}
											}
										}
									}
								else	// Not possessive
									{
									if( hasRelationWord &&
									createdSpecificationItem != null &&
									existingSpecificationItem != null )
										{
										if( existingSpecificationItem.hasRelationContext() )
											{
											if( !existingSpecificationItem.isReplacedOrDeletedItem() )
												{
												if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
													{
													// Created specification was hidden
													if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( olderJustificationItem = existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) == null )
														olderJustificationItem = existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

													if( olderJustificationItem != null &&
													createdSpecificationItem.attachJustificationToSpecificationAdvanced( olderJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to attach an older justification item to the created assumption" );
													}
												else
													{
													if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											{
											if( relationWordItem != null &&
											existingSpecificationItem.isReplacedItem() &&
											( conclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, Constants.NO_QUESTION_PARAMETER, createdSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) != null &&
											!conclusionSpecificationItem.isOlderItem() &&

											// Add opposite possessive justification
											adminItem_.addJustificationToSpecification( false, true, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, createdSpecificationItem, foundPossessiveDefinitionSpecificationItem, null, conclusionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive justification to the created specification" );
											}
										}
									}
								}
							}
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );

			if( !GlobalVariables.hasDisplayedWarning &&
			primarySpecificationItem != null &&
			secondarySpecificationItem != null &&
			relationWordItem != null )
				{
				// Draw reversible conclusions
				if( adminItem_.drawReversibleConclusions( true, relationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
					( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, specificationWordItem.compoundCollectionNr(), primarySpecificationItem.relationContextNr(), specificationWordItem ) ) != null )
						primarySpecificationItem = compoundSpecificationItem;
					}
				else
					{
					if( existingSpecificationItem != null &&
					primarySpecificationItem == secondarySpecificationItem &&
					primarySpecificationItem.hasCompoundSpecificationCollection() &&
					!existingSpecificationItem.isSpecificationWordSpanishAmbiguous() &&
					relationWordItem.isUserGeneralizationWord )
						secondarySpecificationItem = existingSpecificationItem;
					}

				// Make a possessive conditional specification assumption
				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, ( isHiddenPrimarySpecification ? adminItem_.userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption during the making an exclusive specification substitution assumption in word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasAlternativeSecondarySpecificationCompoundCollection;
		boolean hasFeminineOrMasculineProperNounEnding = false;
		boolean hasSameSpecificationCollection;
		boolean isExistingHiddenSpanishSpecification = false;
		boolean isForcingNewJustification = false;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isReversibleAssumption;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isRelationWordUserWordGeneralization;
		short assumptionJustificationTypeNr;
		short assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		short selectedAssumptionJustificationTypeNr;
		int hiddenSpanishSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int relationContextNr;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem selfGeneratedConclusionSpecificationItem = null;
		SpecificationItem userSpecificationItem;
		WordItem alternativeSecondarySpecificationWordItem;
		WordItem feminineSpecificationWordItem = null;
		WordItem masculineSpecificationWordItem;
		WordItem possessiveSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( possessiveDefinitionSpecificationItem_ != null &&
		( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
			{
			isPossessiveSpecificationWordSpanishAmbiguous = possessiveSpecificationWordItem.isNounWordSpanishAmbiguous();

			if( isPossessiveSpecificationWordSpanishAmbiguous ||
			!secondarySpecificationItem.hasCompoundSpecificationCollection() )
				{
				isRelationWordUserWordGeneralization = relationWordItem.isUserGeneralizationWord;

				if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, !isPossessive, possessiveSpecificationWordItem ) ) != null &&
				existingSpecificationItem.isHiddenSpanishSpecification() )
					isExistingHiddenSpanishSpecification = true;

				existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );

				if( ( !isPossessiveSpecificationWordSpanishAmbiguous ||
				existingUserSpecificationItem != null ||
				existingSpecificationItem == null ||
				existingSpecificationItem.isHiddenSpanishSpecification() ||
				!existingSpecificationItem.hasOnlyOneRelationWord() ||
				secondarySpecificationItem.isSelfGenerated() ||
				!generalizationWordItem.hasCurrentlyConfirmedSpecification() ) &&

				// Skip if possessive user specification exists
				( ( existingUserSpecificationItem == null &&
				!possessiveSpecificationWordItem.isUserSpecificationWord &&

				( !isPossessiveSpecificationWordSpanishAmbiguous ||

				// Typical for Spanish
				( isRelationWordUserWordGeneralization &&
				existingSpecificationItem == null ) ) &&

				// Skip if possessive specification exists
				relationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null ) ||

				// Typical for Spanish
				( isPossessiveSpecificationWordSpanishAmbiguous &&
				alternativeSecondarySpecificationItem != null &&

				( existingUserSpecificationItem != null ||
				alternativeSecondarySpecificationItem.isUserSpecification() ) &&

				( ( !isPossessive &&
				existingUserSpecificationItem != null ) ||

				( ( existingSpecificationItem == null ||
				!existingSpecificationItem.isConcludedAssumption() ) &&

				alternativeSecondarySpecificationItem.isActiveAssignment() ) ||

				( existingSpecificationItem != null &&
				existingUserSpecificationItem == null &&
				!generalizationWordItem.hasActiveQuestionWithCompoundCollection() &&

				// Not hidden
				( ( !isExistingHiddenSpanishSpecification &&
				!existingSpecificationItem.isOlderItem() ) ||

				// Hidden
				( isExistingHiddenSpanishSpecification &&
				existingSpecificationItem.hasCurrentCreationSentenceNr() &&
				existingSpecificationItem.isOlderItem() ) ) ) ) &&

				// Skip if possessive user specification exists
				( existingUserSpecificationItem == null ||

				( existingUserSpecificationItem.isOlderItem() &&
				existingUserSpecificationItem.hasCompoundSpecificationCollection() ) ) ) ) )
					{
					isReversibleAssumption = ( secondarySpecificationWordItem == possessiveSpecificationWordItem );

					if( isPossessiveSpecificationWordSpanishAmbiguous &&
					!isPossessive &&
					existingSpecificationItem != null )
						{
						if( isExistingHiddenSpanishSpecification )
							{
							if( existingSpecificationItem.isSelfGeneratedAssumption() &&
							secondarySpecificationItem.isUserSpecification() )
								{
								// Force assumption
								assumptionLevel++;
								isForcingNewJustification = true;
								}
							}
						else
							isForcingNewJustification = true;
						}

					selectedAssumptionJustificationTypeNr = ( isReversibleAssumption ? Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );
					selectedPrimarySpecificationItem = ( isPossessive ? possessiveDefinitionSpecificationItem_ : null );
					selectedSecondarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem );

					// Make a possessive conditional specification assumption (normal)
					// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, false, false, false, false, false, assumptionLevel, selectedAssumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );
					}

				if( !GlobalVariables.hasDisplayedWarning )
					{
					if( oppositePossessiveSpecificationWordItem_ == null )
						return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );

					if( alternativeSecondarySpecificationItem != null )
						{
						// Try to find a self-generated conclusion first
						if( ( selfGeneratedConclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveSpecificationWordItem_, null ) ) != null &&
						selfGeneratedConclusionSpecificationItem.isHiddenSpanishSpecification() &&
						alternativeSecondarySpecificationItem.isUserSpecification() )
							selfGeneratedConclusionSpecificationItem = null;

						// If not found, try to find a self-generated assumption
						if( selfGeneratedConclusionSpecificationItem == null &&
						( assumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != null )
							{
							hasSameSpecificationCollection = ( assumptionSpecificationItem.specificationCollectionNr() == alternativeSecondarySpecificationItem.specificationCollectionNr() );

							if( !assumptionSpecificationItem.hasCurrentCreationSentenceNr() &&
							!assumptionSpecificationItem.hasNewInformation() &&

							( generalizationWordItem.hasCurrentlyConfirmedSpecification() == hasSameSpecificationCollection ||
							alternativeSecondarySpecificationItem.isPossessive() ) )
								selfGeneratedConclusionSpecificationItem = assumptionSpecificationItem;
							}
						}

					isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_.isNounWordSpanishAmbiguous();
					feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ?
														oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem() : oppositePossessiveSpecificationWordItem_.feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem.hasFeminineProperNounEnding() : relationWordItem.hasFeminineProperNounEnding() ) );

					if( feminineSpecificationWordItem != null )
						{
						if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
							anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
						else
							feminineSpecificationWordItem = null;
						}

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != null &&
					alternativeSecondarySpecificationItem.isUserSpecification() &&

					isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
						hasFeminineOrMasculineProperNounEnding = true;

					hasAlternativeSecondarySpecificationCompoundCollection = ( alternativeSecondarySpecificationItem != null &&
																				alternativeSecondarySpecificationItem.hasCompoundSpecificationCollection() );

					isReversibleAssumption = ( secondarySpecificationWordItem == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isReversibleAssumption ? Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION :

													// Typical for Spanish
													( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
													!hasAlternativeSecondarySpecificationCompoundCollection &&
													!isPossessive ) ? Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
					// Typical for Spanish
					isPossessive ||
					existingSpecificationItem == null ||
					!existingSpecificationItem.isActiveItem() ||
					!existingSpecificationItem.isHiddenSpanishSpecification() ||
					secondarySpecificationItem.isSelfGenerated() ) &&

					( ( ( !generalizationWordItem.isOlderItem() ||
					selfGeneratedConclusionSpecificationItem == null ||

					( isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem.isSelfGenerated() &&
					relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) != null ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

					( ( relationContextNr = selfGeneratedConclusionSpecificationItem.relationContextNr() ) == Constants.NO_CONTEXT_NR ||

					( !isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem.isUserSpecification() &&
					// Relation context doesn't exist
					!generalizationWordItem.hasContextInWord( relationContextNr, oppositePossessiveSpecificationWordItem_ ) ) ) ) ) &&

					( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!secondarySpecificationItem.isConcludedAssumption() &&

					( relationWordItem.secondarySpecificationJustificationItem( true, assumptionJustificationTypeNr, secondarySpecificationItem ) == null ||

					( feminineSpecificationWordItem != null &&

					( hasAlternativeSecondarySpecificationCompoundCollection ||
					isReversibleAssumption ) &&

					// Skip if feminine specification exists as user specification
					relationWordItem.firstUserSpecificationItem( false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, feminineSpecificationWordItem ) == null ) ) ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != null &&
					// Skip if already exists as self-generated assumption
					relationWordItem.primarySpecificationJustificationItem( false, assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == null ) ) ) &&

					( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
					!isPossessive ||
					anotherPrimarySpecificationItem == null ||

					( isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem == null ) ||

					( selfGeneratedConclusionSpecificationItem != null &&
					( relationContextNr = selfGeneratedConclusionSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
					!generalizationWordItem.hasContextInWord( relationContextNr, oppositePossessiveSpecificationWordItem_ ) ) ||

					( ( existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, isPossessive, anotherPrimarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, null ) ) != null &&
					!existingUserSpecificationItem.isOlderItem() ) ) ) )
						{
						// Typical for Spanish
						if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
						existingSpecificationItem != null &&

						( existingSpecificationItem.isUserSpecification() ||
						existingSpecificationItem.isHiddenSpanishSpecification() ) )
							hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_.specificationCollectionNr();

						selectedAssumptionJustificationTypeNr = ( assumptionJustificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ?
																	secondarySpecificationItem.assumptionLevel() : Constants.NO_ASSUMPTION_LEVEL );
						selectedPrimarySpecificationItem = ( isReversibleAssumption ? null :
															( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
															alternativeSecondarySpecificationItem != null ?
																alternativeSecondarySpecificationItem.updatedSpecificationItem() : possessiveDefinitionSpecificationItem_ ) );
						selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ?
																( hasFeminineOrMasculineProperNounEnding &&
																!isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem );

						// Make a possessive conditional specification assumption (opposite)
						// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, selectedAssumptionJustificationTypeNr, assumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

						// Test file: "I know (11)"
						// Add reversible user justification
						if( isPossessive &&
						isReversibleAssumption &&
						feminineSpecificationWordItem != null &&
						secondarySpecificationItem.isOlderItem() &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
						foundSpecificationItem.secondarySpecificationJustificationItem( selectedSecondarySpecificationItem ) == null &&
						adminItem_.addJustificationToSpecification( false, true, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, foundSpecificationItem, null, null, selectedSecondarySpecificationItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the found specification" );

						if( alternativeSecondarySpecificationItem != null &&
						feminineSpecificationWordItem != null &&
						( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem.specificationWordItem() ) != null &&

						( ( isPossessive ||
						!secondarySpecificationItem.isSelfGeneratedAssumption() ||
						generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, secondarySpecificationWordItem, relationWordItem ) == null ) &&

						( possessiveSpecificationWordItem == secondarySpecificationWordItem ||

						( !isPossessiveSpecificationWordSpanishAmbiguous &&
						secondarySpecificationItem.isUserSpecification() ) ) ) &&

						( ( existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem ) ) == null ||
						existingSpecificationItem.isSelfGeneratedAssumption() ) )
							{
							// Make an opposite possessive feminine proper noun ending assumption
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( existingSpecificationItem == null ? Constants.NO_CONTEXT_NR : existingSpecificationItem.relationContextNr() ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous || possessiveSpecificationWordItem == alternativeSecondarySpecificationWordItem ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != null ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

							if( isPossessive )
								{
								if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
									{
									// Typical for Spanish
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										{
										if( ( masculineSpecificationWordItem = feminineSpecificationWordItem.masculineCollectionWordItem() ) != null &&
										( obsoleteSpecificationItem = relationWordItem.noRelationContextSpecificationItem( true, true, masculineSpecificationWordItem ) ) != null )
											{
											// Test Spanish test file: "Complejo (19 - mixed)"
											// Write an assumption without relation word
											if( relationWordItem.writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + relationWordItem.anyWordTypeString() + "\"" );

											// Replace an assumption without relation word
											if( relationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
											}
										}
									else
										{
										if( alternativeSecondarySpecificationItem.isOlderItem() &&
										alternativeSecondarySpecificationItem.isSelfGeneratedAssumption() &&
										( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&

										// Add opposite possessive justification
										adminItem_.addJustificationToSpecification( true, false, true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, createdSpecificationItem, userSpecificationItem, possessiveDefinitionSpecificationItem_, anotherPrimarySpecificationItem, relationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive justification to the created specification" );
										}
									}
								}
							else
								feminineSpecificationWordItem_ = feminineSpecificationWordItem;
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasCurrentlyCorrectedAssumptionByKnowledge;
		boolean hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasSimpleUserSpecificationCompoundCollection = false;
		boolean isChineseCurrentLanguage;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem feminineCollectionWordItem;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem != specificationWordItem &&
		( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem.hasCurrentlyCorrectedAssumptionByKnowledge();
			hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
			isChineseCurrentLanguage = adminItem_.isChineseCurrentLanguage();
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			if( ( simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null )
				hasSimpleUserSpecificationCompoundCollection = simpleUserSpecificationItem.hasCompoundSpecificationCollection();

			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null )
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();

					if( ( currentGeneralizationWordItem.hasPartOfSpecification() ||

					( ( !hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&

					( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||
					!currentGeneralizationWordItem.hasCollection() ||

					// Typical for Chinese
					( isChineseCurrentLanguage &&
					hasPrimarySpecificationCompoundCollection &&
					hasSimpleUserSpecificationCompoundCollection &&
					generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
					!generalizationWordItem.isOlderItem() ) ) ) ) ) &&

					( ( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem ) ) == null ||
					// Skip if concluded conclusion already exists
					existingSpecificationItem != simpleUserSpecificationItem ) )
						{
						alternativeSpanishAnotherPrimarySpecificationItem_ = null;
						secondarySpecificationItem = null;

						// Get secondary specification for justification
						if( isUserGeneralizationWord &&
						simpleUserSpecificationItem != null &&
						!simpleUserSpecificationItem.isPossessive() &&

						( ( isSpecificationWordSpanishAmbiguous &&
						simpleUserSpecificationItem.specificationWordItem() == primarySpecificationItem.specificationWordItem() ) ||

						simpleUserSpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() ) )
							secondarySpecificationItem = simpleUserSpecificationItem;
						else
							{
							if( isSpecificationWordSpanishAmbiguous &&
							hasGeneralizationWordFeminineProperNounEnding &&
							( feminineCollectionWordItem = specificationWordItem.feminineCollectionWordItem() ) != null )
								secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, feminineCollectionWordItem );

							if( secondarySpecificationItem == null )
								{
								// Try to find self-generated conclusion first
								if( ( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null )
									// Otherwise, try to find self-generated assumption
									tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );

								if( tempSpecificationItem != null &&

								( existingSpecificationItem == null ||
								tempSpecificationItem.isSelfGeneratedConclusion() ||
								// Skip assumption with relation word
								existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == null ) )
									secondarySpecificationItem = tempSpecificationItem;
								}
							}

						if( secondarySpecificationItem != null &&
						primarySpecificationItem.isExclusiveSpecification() &&
						// Make generalization assumption
						adminItem_.makeGeneralizationAssumption( hasCurrentlyCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem definitionSpecificationItem( WordItem specificationWordItem )
		{
		return ( possessiveDefinitionSpecificationItem_ != null &&
				possessiveDefinitionSpecificationItem_.specificationWordItem() == specificationWordItem ? possessiveDefinitionSpecificationItem_ : oppositePossessiveDefinitionSpecificationItem_ );
		}

	protected WordItem oppositePossessiveDefinitionSpecificationWordItem( WordItem specificationWordItem )
		{
		if( possessiveDefinitionSpecificationItem_ != null &&
		oppositePossessiveDefinitionSpecificationItem_ != null )
			{
			if( possessiveDefinitionSpecificationItem_.specificationWordItem() == specificationWordItem )
				return oppositePossessiveDefinitionSpecificationItem_.specificationWordItem();

			if( oppositePossessiveDefinitionSpecificationItem_.specificationWordItem() == specificationWordItem )
				return possessiveDefinitionSpecificationItem_.specificationWordItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"Praise him for his mighty works;
 *	praise his unequaled greatness!" (Psalm 150:2)
 *************************************************************************/
