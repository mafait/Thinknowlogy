/*	Class:			AdminReasoningOld
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (old design)
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

class AdminReasoningOld
	{
	// Private constructed variables

	private boolean hasSuggestiveQuestionAssumption_;

	private int postponeNegativeConclusionSpecificationCollectionNr_;

	private SpecificationItem alternativeSpanishAnotherPrimarySpecificationItem_;
	private SpecificationItem oppositePossessiveDefinitionSpecificationItem_;
	private SpecificationItem possessiveDefinitionSpecificationItem_;
	private SpecificationItem postponeNegativeConclusionAnotherPrimarySpecificationItem_;
	private SpecificationItem postponeNegativeConclusionPrimarySpecificationItem_;
	private SpecificationItem spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
	private SpecificationItem spanishAmbiguousCompoundPrimarySpecificationItem_;

	private WordItem feminineSpecificationWordItem_;
	private WordItem generalizationAssumptionWordItem_;
	private WordItem oppositePossessiveSpecificationWordItem_;


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


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
				{
				if( specificationWordItem.isNounWordSpanishAmbiguous() )
					{
					if( ( isPossessive &&
					relationWordItem != null &&
					relationWordItem.hasFeminineProperNounEnding() ) ||

					( !isPossessive &&
					generalizationWordItem.hasFeminineProperNounEnding() ) )
						return true;
					}
				else
					{
					// Possessive (relation)
					if( ( isPossessive &&
					relationWordItem != null &&

					( ( isFemaleSpecificationWord &&
					relationWordItem.hasFeminineProperNounEnding() ) ||

					( isMaleSpecificationWord &&
					relationWordItem.hasMasculineProperNounEnding() ) ) ) ||

					// Non-possessive (generalization)
					( !isPossessive &&

					( ( isFemaleSpecificationWord &&
					generalizationWordItem.hasFeminineProperNounEnding() ) ||

					( isMaleSpecificationWord &&
					generalizationWordItem.hasMasculineProperNounEnding() ) ) ) )
						return true;
					}
				}
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
			!generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() &&

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
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
							if( isPrimaryUserSpecification ||
							primarySpecificationItem.hasRelationContext() ||
							generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == null )
								{
								if( adminItem_.askQuestions( true ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );
								}
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
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

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
						if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

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

	private byte drawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasAnotherSecondarySpecificationWithCompoundCollection = false;
		boolean hasDisplayedMoreSpecificNonExclusiveSpecification;
		boolean hasFoundOldSpanishUserSpecification = false;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasRelatedSpecificationsInSameSentence = false;
		boolean isCompoundUserSpecification = false;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isPostponingNegativeConclusion = false;
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
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
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

		hasDisplayedMoreSpecificNonExclusiveSpecification = ( primarySpecificationItem == existingPositiveSpecificationItem &&
															primarySpecificationItem.isExclusiveSpecification() );
		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != null )
			foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem.primarySpecificationItem();

		if( ( hasDisplayedMoreSpecificNonExclusiveSpecification ||
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
					return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a specification without specification collection in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				secondarySpecificationItem = createdSpecificationItem;

				if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, secondarySpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an updated specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
						anotherSecondarySpecificationItem = ( !userSpecificationItem.isSpecificationWordSpanishAmbiguous() ? userSpecificationItem : primarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem ) );
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

					if( !hasFoundOldSpanishUserSpecification )
						{
						if( ( anotherSecondarySpecificationItem = secondarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) == null )
							{
							if( secondarySpecificationItem.hasCompoundSpecificationCollection() &&
							secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
							( secondarySpecificationCommonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null &&
							secondarySpecificationCommonWordItem != primarySpecificationWordItem )
								anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem.firstSpecificationItem( false, false, false, secondarySpecificationWordItem );
							}
						else
							{
							if( !anotherSecondarySpecificationItem.isExclusiveSpecification() &&
							( collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != null )
								{
								isPostponingNegativeConclusion = true;
								anotherSecondarySpecificationItem = null;
								}
							}
						}
					}
				}

			if( !isPostponingNegativeConclusion )
				{
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

				( isUserSpecificationSelected &&

				( ( !primarySpecificationItem.hasRelationContext() &&

				( existingNegativeSpecificationItem == null ||
				existingNegativeSpecificationItem.isOlderItem() ) ) ||

				( isHiddenSpanishPrimarySpecification &&
				existingNegativeSpecificationItem != null &&

				( existingNegativeSpecificationItem.hasNonCompoundSpecificationCollection() ||

				( existingNegativeSpecificationItem.isOlderItem() &&
				secondarySpecificationItem.isSelfGeneratedAssumption() ) ) ) ) ) )
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
								negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ? anotherSecondarySpecificationItem : null );
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
							if( hasDisplayedMoreSpecificNonExclusiveSpecification )
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
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, primarySpecificationItem.isExclusiveSpecification(), false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem.isSelfGeneratedAssumption() ? Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
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
										negativeAssumptionJustificationItem = foundSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION );

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
						if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() )
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
			}

		return Constants.RESULT_OK;
		}

	private byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, boolean isUniqueUserRelation, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem secondarySpecificationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFeminineProperNounEnding;
		boolean isAssignment;
		boolean isAssumption = ( userAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL );
		boolean isConclusionAssignment;
		boolean isForcingNewJustification;
		boolean isSecondarySpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		int foundRelationContextNr;
		int generalizationContextNr = Constants.NO_CONTEXT_NR;
		int nContextRelations;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem collectedSpecificationItem;
		SpecificationItem confirmedAllRelationWordsOfSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem collectedWithItselfCommonWordItem;
		WordItem masculineSpecificationWordItem;
		ContextResultType contextResult = new ContextResultType();
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;

		if( relationContextNr <= Constants.NO_CONTEXT_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, specificationWordItem, generalizationWordItem ) ) == null ||
		// Allow existing conclusion specification to change its relation context
		existingSpecificationItem.isSelfGenerated() )
			{
			isSecondarySpecificationWordSpanishAmbiguous = secondarySpecificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			if( isPossessive &&
			isUserGeneralizationWord &&
			!isSecondarySpecificationWordSpanishAmbiguous &&

			( existingSpecificationItem == null ||
			GlobalVariables.nUserRelationWords > 1 ) &&

			( foundRelationContextNr = generalizationWordItem.contextNr( specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
				{
				if( ( nContextRelations = adminItem_.nContextWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
					{
					if( ( contextResult = adminItem_.getGeneralizationContext( isArchivedAssignment, false, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get a generalization context" );

					generalizationContextNr = contextResult.contextNr;
					}
				else
					generalizationContextNr = foundRelationContextNr;
				}

			confirmedAllRelationWordsOfSpecificationItem = contextResult.confirmedAllRelationWordsOfSpecificationItem;
			existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Find the non-reversible specification (active assignments)
			if( isSecondarySpecificationWordSpanishAmbiguous )
				secondarySpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );
			else
				secondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Skip if possessive user specification exists
			if( secondarySpecificationItem != null &&

			// User specification doesn't exist
			( ( existingUserSpecificationItem == null &&
			secondarySpecificationItem.isUserSpecification() ) ||

			// User specification exists
			( existingUserSpecificationItem != null &&

			( ( !isSecondarySpecificationWordSpanishAmbiguous &&

			( isPossessive ||
			existingUserSpecificationItem.isAssignment() ) ) ||

			// Typical for Spanish
			( isSecondarySpecificationWordSpanishAmbiguous &&
			!secondarySpecificationItem.isUserAssignment() &&

			( !existingUserSpecificationItem.hasRelationContext() ||
			generalizationWordItem.hasDisplayedMoreSpecificSpecification() ) ) ) ) ) &&

			secondarySpecificationItem.hasNewInformation() )
				{
				isForcingNewJustification = ( ( ( isSecondarySpecificationWordSpanishAmbiguous ||
											secondarySpecificationItem.isUserAssignment() ) &&

											secondarySpecificationItem.hasSpecificationCollection() ) ||

											( !isArchivedAssignment &&
											isPossessive &&
											existingSpecificationItem != null &&
											secondarySpecificationItem.hasNonCompoundSpecificationCollection() ) );

				if( !isForcingNewJustification &&
				// Draw possessive reversible conclusions
				adminItem_.drawPossessiveReversibleConclusions( generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				if( !isExclusiveSpecification &&
				!secondarySpecificationItem.isExclusiveGeneralization() )
					{
					hasFeminineProperNounEnding = ( isPossessive &&
													isSecondarySpecificationWordSpanishAmbiguous &&
													relationWordItem.hasFeminineProperNounEnding() );

					isConclusionAssignment = ( ( isArchivedAssignment &&
												isAssumption ) ||

												( !isAssumption &&
												isArchivedAssignment ) );

					isAssignment = ( isConclusionAssignment ||

									( existingUserSpecificationItem != null &&
									existingUserSpecificationItem.isAssignment() ) );

					// Typical for Spanish
					if( isSecondarySpecificationWordSpanishAmbiguous &&
					secondarySpecificationWordItem == specificationWordItem &&

					( !isPossessive ||
					!relationWordItem.isMale() ) )
						spanishAmbiguousSpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

					// Draw a possessive reversible conclusion
					// See paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasFeminineProperNounEnding, isAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, null, null, secondarySpecificationItem, ( hasFeminineProperNounEnding ? secondarySpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) : null ), relationWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

					if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
					foundSpecificationItem.isReplacedItem() )
						foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

					if( confirmedAllRelationWordsOfSpecificationItem == null )
						{
						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null &&
						existingSpecificationItem != null )
							{
							if( existingSpecificationItem.isConcludedAssumption() )
								{
								if( relationWordItem.replaceOrDeleteUnreferencedJustifications() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( !isSecondarySpecificationWordSpanishAmbiguous &&
								foundSpecificationItem != null &&
								!existingSpecificationItem.isReplacedOrDeletedItem() &&
								existingSpecificationItem.isSelfGeneratedAssumption() &&
								foundSpecificationItem.isSelfGeneratedConclusion() &&
								foundSpecificationItem.hasCompoundSpecificationCollection() &&

								// Replace assumption by conclusion
								relationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption specification by a conclusion in word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}
							}

						if( isPossessive &&
						( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) != null )
							{
							if( createdOrFoundSpecificationItem.hasNonCompoundSpecificationCollection() &&
							makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, null, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption during the drawing of a possessive reversible conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

							// Typical for Spanish
							if( isSecondarySpecificationWordSpanishAmbiguous &&
							( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem.collectedWithItselfCommonWordItem() ) != null )
								{
								if( relationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, true, false, false, collectedWithItselfCommonWordItem ) == null &&
								( secondarySpecificationItem = collectedWithItselfCommonWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, createdOrFoundSpecificationItem.specificationCollectionNr(), specificationWordItem ) ) != null &&

								drawNegativeConclusion( isArchivedAssignment, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, null, createdOrFoundSpecificationItem, null, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}
							}

						if( isUserGeneralizationWord )
							{
							if( specificationWordItem.isUserSpecificationWord )
								{
								// Typical for Spanish
								if( isSecondarySpecificationWordSpanishAmbiguous )
									{
									if( ( existingSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, !isPossessive, true, specificationWordItem, null ) ) != null &&
									existingSpecificationItem.specificationCollectionNr() != spanishAmbiguousSpecificationCollectionNr )
										{
										if( foundSpecificationItem == null )
											{
											if( existingSpecificationItem.isHiddenSpanishSpecification() )
												{
												// Add context to hidden specification
												if( generalizationWordItem.addContext( relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem.relationContextNr(), Constants.NO_COLLECTION_NR, specificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												// Not hidden anymore after addition of context
												if( relationWordItem.writeUpdatedSpecification( true, false, false, false, false, existingSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to write the hidden assumption in word \"" + relationWordItem.anyWordTypeString() + "\"" );
												}
											else
												{
												if( createdSpecificationItem != null &&
												!existingSpecificationItem.hasCurrentCreationSentenceNr() &&

												// Replace an assumption without relation word
												relationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing assumption by the created conclusion" );
												}
											}
										else
											{
											if( secondarySpecificationItem != null &&
											existingSpecificationItem.secondarySpecificationJustificationItem( secondarySpecificationItem ) != null &&
											generalizationWordItem.hasDisplayedMoreSpecificSpecification() &&

											// Draw proper noun 'part of' conclusions
											adminItem_.drawProperNounPartOfConclusions( false, isArchivedAssignment, null, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
											}
										}
									}
								else
									{
									if( isPossessive )
										{
										if( createdSpecificationItem != null &&
										foundSpecificationItem == null &&
										( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, null ) ) != null )
											{
											// Additional justification (assumption)
											if( ( justificationResult = relationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + relationWordItem.anyWordTypeString() + "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

											if( assumptionSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the assumption specification" );

											if( generalizationWordItem.addContext( relationWordTypeNr, specificationWordTypeNr, assumptionSpecificationItem.relationContextNr(), Constants.NO_COLLECTION_NR, specificationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( ( collectedSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, assumptionSpecificationItem.specificationCollectionNr(), null ) ) != null &&
											( firstJustificationItem = collectedSpecificationItem.firstJustificationItem() ) != null )
												{
												// Additional justification (another)
												if( ( justificationResult = relationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, firstJustificationItem.primarySpecificationItem(), null, collectedSpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + relationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( assumptionSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification (again) of the assumption specification" );

												// Additional justification (collected)
												if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, firstJustificationItem.primarySpecificationItem(), null, createdSpecificationItem, null, collectedSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( collectedSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the collected specification" );
												}
											}
										}
									else
										{
										if( adminItem_.isSpanishCurrentLanguage() &&
										createdSpecificationItem != null &&
										( masculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem() ) != null &&
										( obsoleteSpecificationItem = relationWordItem.noRelationContextSpecificationItem( true, true, masculineSpecificationWordItem ) ) != null )
											{
											// Write an assumption without relation word
											if( relationWordItem.writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + relationWordItem.anyWordTypeString() + "\"" );

											// Replace an assumption without relation word
											if( relationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
											}
										}
									}

								if( makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( createdSpecificationItem != null &&
								foundSpecificationItem != null &&
								( firstJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null &&
								( secondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
									{
									// Additional justification (Created Spanish copy)
									if( ( justificationResult = relationWordItem.addJustification( false, false, true, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

									if( createdSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
									}
								}
							}
						}
					else
						{
						if( relationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, confirmedAllRelationWordsOfSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a specification in word \"" + relationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte drawSpecificationGeneralizationConclusion( short assumptionLevel, SpecificationItem secondarySpecificationItem )
		{
		boolean isArchivedAssignment;

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( !secondarySpecificationItem.isGeneralizationNoun() ||
		!secondarySpecificationItem.isSpecificationNoun() )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );

		isArchivedAssignment = secondarySpecificationItem.isArchivedAssignment();

		// Draw a specification-generalization conclusion
		// Not yet explained in the design
		if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, secondarySpecificationItem.isExclusiveSpecification(), false, secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPartOf(), secondarySpecificationItem.isPossessive(), false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, secondarySpecificationItem.specificationWordItem(), secondarySpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification" );

		return Constants.RESULT_OK;
		}

	private byte makeGeneralizationAssumption( boolean hasCorrectedAssumptionByKnowledge, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		JustificationItem olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem olderGeneralizationAssumptionBySpecificationItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( secondarySpecificationItem.hasCurrentCreationSentenceNr() &&
		// Skip if user specification already exists
		generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null &&
		// Skip if justification with primary specification already exists
		generalizationWordItem.primarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem ) == null )
			{
			// Make a generalization assumption
			// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
			if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, ( generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NOUN ), false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			if( hasCorrectedAssumptionByKnowledge &&
			( olderGeneralizationAssumptionBySpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
			( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != null &&

			// Replace justification
			generalizationWordItem.replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, createAndAssignResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a noun" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( !currentGeneralizationWordItem.hasCollection() &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null )
					{
					// Get secondary specification for justification
					if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null )
						{
						if( !primarySpecificationItem.isSelfGeneratedAssumption() &&
						secondarySpecificationItem.hasCurrentCreationSentenceNr() &&

						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						// Get secondary question specification for justification
						if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, true, specificationWordItem ) ) != null &&
						drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasAlternativeSecondarySpecificationCompoundCollection;
		boolean hasFeminineOrMasculineProperNounEnding = false;
		boolean hasSameSpecificationCollection;
		boolean isExistingHiddenSpanishSpecification = false;
		boolean isForcingNewJustification = false;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveReversibleAssumption;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isRelationWordUserWordGeneralization;
		short assumptionJustificationTypeNr;
		short assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		int hiddenSpanishSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int relationContextNr;
		JustificationItem createdJustificationItem;
		JustificationItem foundJustificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem selfGeneratedConclusionSpecificationItem = null;
		SpecificationItem userSpecificationItem;
		WordItem feminineSpecificationWordItem = null;
		WordItem masculineSpecificationWordItem;
		WordItem possessiveSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult = new JustificationResultType();

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
				!existingSpecificationItem.hasSpecificationCollection() ||
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
				!alternativeSecondarySpecificationItem.isOlderItem() &&

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
					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == possessiveSpecificationWordItem );

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

					// Make a possessive conditional specification assumption (normal)
					// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, false, false, false, false, false, assumptionLevel, ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : null ), null, ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), null, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );

					createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
					createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? createAndAssignResult.foundSpecificationItem : createdSpecificationItem );
					}

				if( !GlobalVariables.hasDisplayedWarning )
					{
					if( oppositePossessiveSpecificationWordItem_ == null )
						return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );

					if( alternativeSecondarySpecificationItem != null &&
					// Try to find a self-generated conclusion first
					( selfGeneratedConclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveSpecificationWordItem_, null ) ) == null &&
					// If not found, try to find a self-generated assumption
					( assumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != null )
						{
						hasSameSpecificationCollection = ( assumptionSpecificationItem.specificationCollectionNr() == alternativeSecondarySpecificationItem.specificationCollectionNr() );

						if( !assumptionSpecificationItem.hasCurrentCreationSentenceNr() &&
						!assumptionSpecificationItem.hasNewInformation() &&

						( generalizationWordItem.hasCurrentlyConfirmedSpecification() == hasSameSpecificationCollection ||
						alternativeSecondarySpecificationItem.isPossessive() ) )
							selfGeneratedConclusionSpecificationItem = assumptionSpecificationItem;
						}

					isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_.isNounWordSpanishAmbiguous();
					feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem() : oppositePossessiveSpecificationWordItem_.feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem.hasFeminineProperNounEnding() : relationWordItem.hasFeminineProperNounEnding() ) );

					if( feminineSpecificationWordItem != null )
						{
						if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
							anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
						else
							feminineSpecificationWordItem = null;
						}

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

					( alternativeSecondarySpecificationItem == null ||
					alternativeSecondarySpecificationItem.isUserSpecification() ) &&

					isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
						hasFeminineOrMasculineProperNounEnding = true;

					hasAlternativeSecondarySpecificationCompoundCollection = ( alternativeSecondarySpecificationItem != null &&
																				alternativeSecondarySpecificationItem.hasCompoundSpecificationCollection() );

					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION :

													// Typical for Spanish
													( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
													!hasAlternativeSecondarySpecificationCompoundCollection &&
													!isPossessive ) ? Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( ( selfGeneratedConclusionSpecificationItem == null ||
					!generalizationWordItem.isOlderItem() ||

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
					isPossessiveReversibleAssumption ) &&

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
					!existingUserSpecificationItem.isOlderItem() ) ) )
						{
						// Typical for Spanish
						if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
						existingSpecificationItem != null &&

						( existingSpecificationItem.isUserSpecification() ||
						existingSpecificationItem.isHiddenSpanishSpecification() ) )
							hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_.specificationCollectionNr();

						// Make a possessive conditional specification assumption (opposite)
						// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, ( assumptionJustificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ? secondarySpecificationItem.assumptionLevel() : Constants.NO_ASSUMPTION_LEVEL ), assumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? null : ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : possessiveDefinitionSpecificationItem_ ) ), null, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( hasFeminineOrMasculineProperNounEnding && !isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem ), null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
							{
							if( selfGeneratedConclusionSpecificationItem == null &&
							// Created, not found
							( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
								{
								// Typical for Spanish
								if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
								secondarySpecificationItem.isUserSpecification() &&
								!createdSpecificationItem.isHiddenSpanishSpecification() &&
								( obsoleteSpecificationItem = relationWordItem.noRelationContextSpecificationItem( isPossessive, true, oppositePossessiveSpecificationWordItem_ ) ) != null )
									{
									// Write an assumption without relation word
									if( relationWordItem.writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + relationWordItem.anyWordTypeString() + "\"" );

									// Replace an assumption without relation word
									if( relationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
									}
								}
							}
						else	// Found
							{
							if( isPossessive &&
							feminineSpecificationWordItem == null &&
							foundSpecificationItem.hasNonCompoundSpecificationCollection() &&
							foundSpecificationItem.isOlderItem() &&
							!foundSpecificationItem.isHiddenSpanishSpecification() &&

							// Cleanup unreferenced justifications
							relationWordItem.replaceOrDeleteUnreferencedJustifications() != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}

						if( alternativeSecondarySpecificationItem != null &&
						feminineSpecificationWordItem != null )
							{
							if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem ) ) == null ||
							existingSpecificationItem.isSelfGeneratedAssumption() )
								{
								// Make an opposite possessive feminine proper noun ending assumption
								// Not yet explained in the design
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( existingSpecificationItem == null ? Constants.NO_CONTEXT_NR : existingSpecificationItem.relationContextNr() ), alternativeSecondarySpecificationItem, ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != null ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
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
											( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
												{
												// Additional justification (feminine)
												if( ( justificationResult = relationWordItem.addJustification( true, false, true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, userSpecificationItem, possessiveDefinitionSpecificationItem_, anotherPrimarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + relationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the feminine assumption specification" );
												}
											}
										}
									}
								else
									{
									feminineSpecificationWordItem_ = feminineSpecificationWordItem;

									if( !isArchivedAssignment &&
									( createdOrFoundSpecificationItem = ( createAndAssignResult.createdSpecificationItem == null ? createAndAssignResult.foundSpecificationItem : createAndAssignResult.createdSpecificationItem ) ) != null &&
									!createdOrFoundSpecificationItem.isReplacedItem() )
										{
										if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() )
											{
											// Draw possessive reversible conclusion
											if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, createdOrFoundSpecificationItem.specificationContextNr(), createdOrFoundSpecificationItem.relationContextNr(), relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											// Typical for Spanish. Test files: "Complejo (6)" and "Complejo (11)"
											if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
											existingSpecificationItem == null &&
											( conclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, false, feminineSpecificationWordItem, null ) ) != null &&
											conclusionSpecificationItem.hasSpecificationCollection() &&
											( foundJustificationItem = conclusionSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null )
												{
												// Additional Spanish feminine conclusion justification
												if( ( justificationResult = relationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, foundJustificationItem.secondarySpecificationItem(), null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + relationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( createdOrFoundSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the found specification" );
												}
											}
										}
									}
								}
							else
								{
								if( isPossessive &&
								existingSpecificationItem.isUserSpecification() &&
								( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, feminineSpecificationWordItem, relationWordItem ) ) != null &&
								foundSpecificationItem.secondarySpecificationJustificationItem( existingSpecificationItem ) == null )
									{
									// Additional justification (User). Test file "John - Anna (before family definition)"
									if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, ( existingSpecificationItem.hasAssumptionLevel() ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, existingSpecificationItem, null, foundSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

									if( foundSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the found specification" );
									}
								}
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
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
			hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();
			hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();
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

					( ( !hasCorrectedAssumptionByOppositeQuestion &&

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
								existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == null ) )
									secondarySpecificationItem = tempSpecificationItem;
								}
							}

						if( secondarySpecificationItem != null &&
						primarySpecificationItem.isExclusiveSpecification() &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Typical for Spanish
						existingSpecificationItem == null ) )
							{
							// Make generalization assumption
							if( makeGeneralizationAssumption( hasCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

							generalizationAssumptionWordItem_ = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		// Definition specification
		if( generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NOUN &&
		generalizationAssumptionWordItem_ != null &&
		generalizationWordItem != generalizationAssumptionWordItem_ &&
		specificationWordItem != generalizationAssumptionWordItem_ &&
		( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
		( secondarySpecificationItem = specificationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != null &&

		// Make an exclusive specification substitution assumption
		// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
		adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isEveryGeneralization(), false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, generalizationAssumptionWordItem_, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

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
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
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
		// Private constructed variables

		hasSuggestiveQuestionAssumption_ = false;

		postponeNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		alternativeSpanishAnotherPrimarySpecificationItem_ = null;
		oppositePossessiveDefinitionSpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = null;
		postponeNegativeConclusionPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		generalizationAssumptionWordItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

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

		hasSuggestiveQuestionAssumption_ = false;

		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		generalizationAssumptionWordItem_ = null;
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

	protected byte drawNegativeConclusionsFromAnsweredQuestions( boolean hasDisplayedMoreSpecificNonExclusiveSpecification, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
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
				if( hasDisplayedMoreSpecificNonExclusiveSpecification ||
				generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, answeredQuestionSpecificationWordItem ) == null )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem.firstJustificationItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem.hasPossessivePrimarySpecification() &&
						// Draw negative conclusion from answered question
						drawNegativeConclusion( answeredQuestionSpecificationItem.isArchivedAssignment(), answeredQuestionSpecificationItem.generalizationWordTypeNr(), answeredQuestionSpecificationItem.specificationWordTypeNr(), answeredQuestionSpecificationItem.generalizationContextNr(), answeredQuestionSpecificationItem.specificationContextNr(), currentJustificationItem.primarySpecificationItem(), currentJustificationItem.secondarySpecificationItem(), userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem.recentlyAnsweredQuestionSpecificationItem( false ) ) != null );

		return Constants.RESULT_OK;
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, int generalizationContextNr, WordItem generalizationWordItem )
		{
		boolean hasFoundAssignment;
		boolean hasMoreOptionsLeft = false;
		SpecificationItem currentAssignmentItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem firstNegativeOnlyOptionLeftAssignmentItem = null;
		SpecificationItem firstOnlyOptionLeftSpecificationItem = null;
		SpecificationItem foundOnlyOptionLeftSpecificationItem = null;
		WordItem specificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
				( currentAssignmentItem = generalizationWordItem.firstSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != null )
					{
					hasFoundAssignment = false;
					specificationWordItem = currentSpecificationItem.specificationWordItem();

					if( currentAssignmentItem.isNegative() &&
					firstNegativeOnlyOptionLeftAssignmentItem == null )
						firstNegativeOnlyOptionLeftAssignmentItem = currentAssignmentItem;

					do	{
						if( currentAssignmentItem.isNegative() &&
						currentAssignmentItem.specificationCollectionNr() == specificationCollectionNr &&
						currentAssignmentItem.specificationWordItem() == specificationWordItem )
							hasFoundAssignment = true;
						}
					while( !hasFoundAssignment &&
					( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );

					if( !hasFoundAssignment )
						{
						if( foundOnlyOptionLeftSpecificationItem == null )
							// Found the first option left
							foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;
						else
							{
							hasMoreOptionsLeft = true;
							// Clear the found option
							foundOnlyOptionLeftSpecificationItem = null;
							}
						}
					}
				}
			while( !hasMoreOptionsLeft &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );

			if( firstNegativeOnlyOptionLeftAssignmentItem != null &&
			foundOnlyOptionLeftSpecificationItem != null &&

			// Draw an only option left conclusion
			// Not yet explained in the design
			adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, boolean isUniqueUserRelation, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		WordItem feminineSpecificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion" );

		// Typical for Spanish
		if( isPossessive &&
		!isArchivedAssignment &&
		specificationWordItem.isNounWordSpanishAmbiguous() &&
		relationWordItem.hasFeminineProperNounEnding() &&
		( feminineSpecificationWordItem = specificationWordItem.feminineCollectionWordItem() ) != null &&

		// Draw possessive reversible feminine conclusion
		drawPossessiveReversibleConclusion( false, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible feminine conclusion" );

		return Constants.RESULT_OK;
		}

	protected byte drawSimpleNegativeSpanishConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		int nonCompoundSpecificationCollectionNr;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem();
		WordItem collectionWordItem;
		WordItem commonWordItem;
		WordItem userSpecificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( simpleUserSpecificationItem != null &&
		!specificationWordItem.isNounWordSpanishAmbiguous() &&
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

			askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		short assumptionLevel;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem = null;
		WordItem currentGeneralizationWordItem;

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != null )
					{
					if( !currentSpecificationItem.hasSpecificationCollection() )
						{
						assumptionLevel = ( secondarySpecificationItem.assumptionLevel() < currentSpecificationItem.assumptionLevel() ? currentSpecificationItem.assumptionLevel() : secondarySpecificationItem.assumptionLevel() );

						// Collect generalization words of a specification-generalization definition
						if( adminItem_.collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						if( currentSpecificationItem.isReplacedItem() )
							currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

						if( currentSpecificationItem.hasCurrentCreationSentenceNr() &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification-generalization conclusion about an earlier sentence" );

						if( !isSelfGenerated &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification-generalization conclusion about the given sentence" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isMakingPartOfAssumption, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean hasCurrentSpecificationCollection;
		boolean hasCurrentSpecificationCompoundCollection;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean hasPrimarySpecificationNonCompoundCollection;
		boolean hasRelationWord = ( relationWordItem != null );
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
		boolean isUserSpecificationWord;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short currentSpecificationWordTypeNr;
		int currentSpecificationCollectionNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem alternativeSpanishAnotherPrimarySpecificationItem;
		SpecificationItem archivedAssignmentItem;
		SpecificationItem bestMatchingRelationSpecificationItem;
		SpecificationItem collectedWithItselfSpecificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem currentSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a specification reasoning word type" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( isMakingPartOfAssumption &&
		makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
			{
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			isIncludingAdjectives = ( !isExclusiveSpecification &&
									!hasCorrectedAssumptionByKnowledge &&
									!userSpecificationItem.isQuestion() );
			}

		if( ( currentSpecificationItem = specificationWordItem.firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != null )
			{
			hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();

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
				collectedWithItselfSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );

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
						if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, false, false, currentSpecificationCollectionNr, currentSpecificationItem.relationContextNr(), currentSpecificationWordItem ) != Constants.RESULT_OK )
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

								( !hasUserSpecificationRelationContext ||
								foundPrimarySpecificationItem.hasNonCompoundSpecificationCollection() ) ) ||

								( ( hasRelationWord ||
								generalizationWordItem.isOlderItem() ) &&

								oppositePossessiveDefinitionSpecificationItem_ != null &&
								oppositePossessiveDefinitionSpecificationItem_.specificationCollectionNr() == userSpecificationCollectionNr ) )
									{
									// Make a feminine or masculine proper noun ending assumption
									// Not yet explained in the design
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem_ : null ), currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : null ) ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

									if( !isUserSpecificationWordSpanishAmbiguous )
										{
										if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
											{
											if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
											( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null &&
											( secondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
												{
												// Additional justification
												if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
												}

											// Draw missing negative conclusions
											if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, false, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
											}
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
							isPossessiveUserSpecification ) ) ||

							( hasPrimarySpecificationNonCompoundCollection &&
							isGeneralizationWordUserRelationWord &&
							isPossessiveUserSpecification ) ) ) ) ) )
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
								hasCorrectedAssumptionByOppositeQuestion ||
								hasSuggestiveQuestionAssumption_ ) )
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
								currentSpecificationItem.hasAssumptionLevel() ) &&

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
										alternativePrimarySpecificationItem = ( hasRelationWord ||
																				isSpecificationWordSpanishAmbiguous ? alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) : null );

										if( ( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem.isHiddenSpanishSpecification() ) ||

										( existingSpecificationItem != null &&
										alternativePrimarySpecificationItem != null &&
										alternativePrimarySpecificationItem.specificationWordItem() == specificationWordItem &&

										( ( isSpecificationWordSpanishAmbiguous &&
										existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) ) ||

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
											existingSpecificationItem != null &&
											userSpecificationItem != null )
												{
												if( ( primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, userSpecificationItem.specificationWordItem() ) ) != null &&
												// Check if justification already exists
												existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) )
													primarySpecificationItem = alternativePrimarySpecificationItem;
												}
											else
												primarySpecificationItem = alternativePrimarySpecificationItem;
											}
										else
											{
											if( isPossessiveUserSpecification &&
											primarySpecificationItem.hasAssumptionLevel() &&
											( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, currentSpecificationWordItem ) ) != null &&
											existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null )
												// Skip if similar justification already exists, without relation word
												primarySpecificationItem = null;
											}
										}

									if( primarySpecificationItem != null &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||

									( currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&

									( primarySpecificationItem.isOlderItem() ||
									primarySpecificationItem.isPossessive() ) ) ) &&

									// Skip if already exists as user specification
									( existingSpecificationItem == null ||
									existingSpecificationItem.isSelfGenerated() ) )
										{
										archivedAssignmentItem = null;

										if( isArchivedUserAssignment &&
										userSpecificationItem != null &&
										userSpecificationItem.isGeneralizationNoun() )
											{
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										// Make a compound specification substitution assumption, which is actually an uncertain conclusion
										// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
										//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeSpanishAnotherPrimarySpecificationItem, ( archivedAssignmentItem == null ? currentSpecificationItem : archivedAssignmentItem ), null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

										if( createAndAssignResult.createdSpecificationItem == null &&

										( createAndAssignResult.createdJustificationItem != null ||
										foundPrimarySpecificationItem.isOlderItem() ) &&

										// Cleanup unreferenced justifications
										generalizationWordItem.replaceOrDeleteUnreferencedJustifications() != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									{
									if( ( !isExclusiveSpecification &&
									bestMatchingRelationSpecificationItem == null &&

									( !isSpecificationWordSpanishAmbiguous ||

									( !isFoundPrimaryHiddenSpanishSpecification &&
									collectedWithItselfSpecificationItem != null &&
									collectedWithItselfSpecificationItem.hasCurrentCreationSentenceNr() ) ) &&

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
										!currentSpecificationItem.hasCompoundSpecificationCollection() ||

										// Typical for Spanish
										( isSpanishCurrentLanguage &&
										currentSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, false, generalizationWordItem ) != null ) ) ) ) )
											{
											// Draw a compound specification substitution conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, ( currentSpecificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? Constants.WORD_TYPE_NOUN_SINGULAR : currentSpecificationWordTypeNr ), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
												{
												if( hasCorrectedAssumptionByKnowledge )
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
														isUserGeneralizationWord &&
														primarySpecificationItem.hasNonCompoundSpecificationCollection() &&

														// Draw simple negative Spanish conclusions
														drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
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

							makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption by generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
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
		int compoundCollectionNr;
		int nonCompoundSpanishSpecificationCollectionNr;
		int relationContextNr;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem olderJustificationItem;
		JustificationItem previousJustificationItem;
		SpecificationItem additionalSecondarySpecificationItem;
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
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem commonWordItem;
		WordItem currentPossessiveNounWordItem;
		WordItem feminineSpecificationWordItem;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult = new JustificationResultType();
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
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( true, isArchivedAssignment, isExclusiveSpecification, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
									makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + commonWordItem.anyWordTypeString() + "\"" );

									// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), foundPossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

									createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
									foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

									if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) == null )
										return adminItem_.startError( 1, moduleNameString_, "No specification item was created or found" );

									secondarySpecificationItem = createdOrFoundSpecificationItem.updatedSpecificationItem();

									if( createdSpecificationItem != null )
										{
										if( isPossessiveSpecificationWordSpanishAmbiguous )
											{
											isReplaceHiddenSpecification = true;

											if( spanishAmbiguousCompoundPrimarySpecificationItem_ != null )
												primarySpecificationItem = spanishAmbiguousCompoundPrimarySpecificationItem_;

											if( spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ != null )
												anotherPrimarySpecificationItem = spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
											}
										else
											{
											if( hasRelationWord &&
											foundSpecificationItem != null &&
											foundSpecificationItem.isSelfGeneratedConclusion() &&
											( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != null &&
											( firstJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null )
												{
												if( ( additionalSecondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, additionalSecondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

													if( createdSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
													}
												else
													return adminItem_.startError( 1, moduleNameString_, "The additional secondary specification item is undefined" );
												}
											}
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
						( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, primarySpecificationItem.isAssignment(), false, false, false, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );
						}

					// First opposite possessive specification
					if( !GlobalVariables.hasDisplayedWarning &&
					possessiveDefinitionSpecificationWordItem != null &&
					foundOppositePossessiveDefinitionSpecificationItem != null &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( isNegative ||
						createdOrFoundSpecificationItem == null ||

						( primarySpecificationItem != null &&
						primarySpecificationItem.hasSpecificationContext() ) )
							{
							secondarySpecificationItem = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
						primarySpecificationItem != null &&
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
							existingSpecificationItem.isSelfGeneratedAssumption() &&

							( hasRelationWordConfirmedSpecification ||
							!hasRelationWordFeminineOrMasculineProperNounEnding ||
							anotherPrimarySpecificationItem != null ) ) ||

							( secondarySpecificationItem.hasRelationContext() &&
							!secondarySpecificationItem.isSelfGeneratedConclusion() &&

							( !isHiddenSecondarySpecification ||

							( primarySpecificationItem.isUserSpecification() &&
							!generalizationWordItem.hasDisplayedMoreSpecificSpecification() ) ||

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

								if( primarySpecificationItem.isReplacedItem() )
									primarySpecificationItem = primarySpecificationItem.updatedSpecificationItem();

								// Make an exclusive specification substitution assumption (opposite)
								// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), foundOppositePossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( isHiddenSecondarySpecification ? primarySpecificationItem : secondarySpecificationItem ) : foundOppositePossessiveDefinitionSpecificationItem ), ( !isReplaceHiddenSpecification && isOppositePossessiveSpecificationWordSpanishAmbiguous ? null : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem : secondarySpecificationItem ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
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
												( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, anotherPrimarySpecificationItem, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
												}

											if( ( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, false, anotherPrimarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// See paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
												if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

												if( feminineSpecificationWordItem_ != null &&
												( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), null, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
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
											if( createdSpecificationItem == null )
												{
												// Found, but not created
												secondarySpecificationItem = foundSpecificationItem.updatedSpecificationItem();

												if( anotherPrimarySpecificationItem != null &&
												relationWordItem != null &&
												relationWordItem.isOlderItem() &&
												!GlobalVariables.hasDisplayedMessage &&
												primarySpecificationItem.isUserSpecification() &&
												( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != null )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, anotherPrimarySpecificationItem, null, primarySpecificationItem, null, additionalSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

													if( additionalSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the found specification" );
													}
												}
											else	// Found and created
												{
												if( !foundSpecificationItem.isDeletedItem() )
													{
													if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) == null )
														return adminItem_.startError( 1, moduleNameString_, "The found specification item has no first justification item" );

													// Replace the secondary specification of a justification
													if( ( obsoleteJustificationItem = primarySpecificationItem.secondarySpecificationJustificationItem( foundSpecificationItem ) ) != null )
														{
														if( ( justificationResult = generalizationWordItem.copyJustification( false, obsoleteJustificationItem.primarySpecificationItem(), createdSpecificationItem, obsoleteJustificationItem.attachedJustificationItem(), obsoleteJustificationItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to copy an obsolete justification item with a different secondary specification item" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
															return adminItem_.startError( 1, moduleNameString_, "I couldn't copy an obsolete justification" );

														// Replace obsolete justification
														if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, primarySpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}

													if( ( additionalSecondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
														{
														// Additional justification
														if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, additionalSecondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
															return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

														if( createdSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
														}
													else
														return adminItem_.startError( 1, moduleNameString_, "The additional secondary specification item is undefined" );
													}
												}
											}
										}
									}
								else	// Non-possessive
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

													if( ( olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) == null )
														olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

													if( olderJustificationItem != null )
														{
														if( generalizationWordItem.attachJustificationInWord( olderJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to attach an older justification item to the created assumption in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														if( ( obsoleteJustificationItem = createdSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != null &&
														( previousJustificationItem = createdSpecificationItem.previousJustificationItem( obsoleteJustificationItem ) ) != null )
															{
															// Replace obsolete justification from created specification
															if( previousJustificationItem.changeAttachedJustification( obsoleteJustificationItem.attachedJustificationItem() ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to change the attached justification item of the previous justification item" );

															if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												else
													{
													if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( !existingSpecificationItem.isConcludedAssumption() &&
													// Cleanup unreferenced justifications
													generalizationWordItem.replaceOrDeleteUnreferencedJustifications() != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											{
											if( relationWordItem != null &&
											existingSpecificationItem.isReplacedItem() &&
											( conclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, Constants.NO_QUESTION_PARAMETER, createdSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) != null &&
											!conclusionSpecificationItem.isOlderItem() )
												{
												// Additional justification
												if( ( justificationResult = generalizationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, foundPossessiveDefinitionSpecificationItem, null, conclusionSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the created specification" );
												}
											}
										}
									else
										{
										if( primarySpecificationItem.isReplacedItem() &&
										( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
											{
											primarySpecificationItem = primarySpecificationItem.updatedSpecificationItem();

											// Additional Spanish justification (User). Test Spanish file: "Complejo (15)"
											if( ( justificationResult = generalizationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, userSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

											if( primarySpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the primary specification" );
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
				if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
					( compoundCollectionNr = specificationWordItem.compoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, compoundCollectionNr, primarySpecificationItem.relationContextNr(), specificationWordItem ) ) != null )
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

				// Draw possessive reversible conclusions
				if( adminItem_.drawPossessiveReversibleConclusions( relationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, ( isHiddenPrimarySpecification ? adminItem_.userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption during the making an exclusive specification substitution assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		int currentSpecificationCollectionNr;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem indirectPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		WordItem commonWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem negativeQuestionSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( userSpecificationItem.hasSpecificationCollection() &&
		( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
					{
					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

					// Draw an indirectly answered question conclusion
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );

					if( relatedResult.relatedSpecificationItem != null &&
					( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
					( negativeQuestionSpecificationWordItem = relatedResult.relatedSpecificationItem.specificationWordItem() ) != null &&
					( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
						{
						if( commonWordItem.isNounWordSpanishAmbiguous() )
							{
							if( generalizationWordItem.replaceOrDeleteSpecification( currentQuestionSpecificationItem.updatedSpecificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace the answered question by the created specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, commonWordItem ) ) != null &&
							( anotherPrimarySpecificationItem = commonWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentQuestionSpecificationWordItem ) ) != null &&

							// Draw a negative assumption
							// Not yet explained in the design
							( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
							}
						}

					currentQuestionSpecificationItem = adminItem_.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
					}
				else
					currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem();
				}
			while( currentQuestionSpecificationItem != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		short nOptionsLeft = 0;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem onlyOptionLeftSpecificationWordItem = null;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( !userSpecificationItem.hasSpecificationCollection() )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification collection" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, currentQuestionSpecificationWordItem ) == null )
					{
					nOptionsLeft++;
					onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
					}
				}
			while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) != null );

			if( nOptionsLeft == 1 &&
			onlyOptionLeftSpecificationWordItem != null &&
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				// Do for all generalization noun specification words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != null &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != null &&

					// Make an only option left assumption
					// Not yet explained in the design
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				while( !GlobalVariables.hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int secondarySpecificationCollectionNr;
		RelatedResultType relatedResult = new RelatedResultType();

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		// Check if specification doesn't exist yet
		if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == null )
			{
			if( secondarySpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
			// Check if a related specification exists
			( relatedResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );

			if( relatedResult.relatedSpecificationItem == null )
				{
				if( generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() )
					{
					if( adminItem_.correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					// Typical for Spanish:
					// Skip making suggestive assumption if specification word is Spanish ambiguous,
					// and if user question has more than one generalization words
					if( !secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ||
					GlobalVariables.nUserGeneralizationWords == 1 )
						{
						// Make a suggestive question assumption
						// Not yet explained in the design
						hasSuggestiveQuestionAssumption_ = true;

						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem oppositePossessiveDefinitionSpecificationItem()
		{
		return oppositePossessiveDefinitionSpecificationItem_;
		}

	protected WordItem possessiveDefinitionSpecificationWordItem()
		{
		if( possessiveDefinitionSpecificationItem_ != null )
			return possessiveDefinitionSpecificationItem_.specificationWordItem();

		return null;
		}

	protected CompoundResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordItem currentGeneralizationWordItem;
		CompoundResultType compoundResult = new CompoundResultType();
		RelatedResultType relatedResult;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startCompoundResultError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null )
					{
					if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem.hasAssumptionLevel(), foundSpecificationItem.isArchivedAssignment(), foundSpecificationItem.isExclusiveSpecification(), true, Constants.NO_QUESTION_PARAMETER, foundSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
						return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

					if( !GlobalVariables.hasDisplayedWarning &&
					foundSpecificationItem.hasSpecificationCollection() )
						{
						if( ( relatedResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );

						if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null )
							{
							if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.hasAssumptionLevel(), relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), true, relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem.specificationWordItem(), null ) != Constants.RESULT_OK )
								return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return compoundResult;
		}
	};

/*************************************************************************
 *	"Praise him for his mighty works;
 *	praise his unequaled greatness!" (Psalm 150:2)
 *************************************************************************/
