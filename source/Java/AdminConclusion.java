/*	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously draw grammar-based conclusions
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

class AdminConclusion
	{
	// Private constructed variables

	private boolean hasFoundConflictWithUserSpecification_;
	private boolean isSpanishAmbiguousCompoundSpecification_;

	private int postponeNegativeConclusionSpecificationCollectionNr_;

	private SpecificationItem alternativeAnotherPrimarySpecificationItem_;
	private SpecificationItem adjustedQuestionSecondarySpecificationItem_;
	private SpecificationItem adjustedQuestionSpecificationItem_;
	private SpecificationItem postponeNegativeConclusionPrimarySpecificationItem_;
	private SpecificationItem postponeNegativeConclusionAnotherPrimarySpecificationItem_;
	private SpecificationItem spanishAmbiguousCompoundPrimarySpecificationItem_;
	private SpecificationItem spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;

	private WordItem adjustedQuestionWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private void clearPostponedNegativeConclusionVariables()
		{
		postponeNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		postponeNegativeConclusionPrimarySpecificationItem_ = null;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = null;
		}

	private byte askSpecificationSubstitutionQuestion( boolean isArchivedAssignment, boolean isExclusiveSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem existingQuestionSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isSecondarySpecificationCompoundCollection;
		boolean isUserGeneralizationWord;
		boolean isBlockingCommonWordOfCompoundCollection = false;
		int nonCompoundSpecificationCollectionNr;
		int secondaryRelationContextNr;
		int secondarySpecificationCollectionNr;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem adjustedQuestionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr() ) == Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification collection" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();
		isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		nonCompoundSpecificationCollectionNr = secondarySpecificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );
		secondaryRelationContextNr = secondarySpecificationItem.relationContextNr();
		isSecondarySpecificationCompoundCollection = secondarySpecificationWordItem.isCompoundCollection( secondarySpecificationCollectionNr, specificationWordItem );

		foundSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

		// Skip if already exists as specification
		if( hasCorrectedAssumptionByOppositeQuestion ||
		isPrimarySpecificationWordSpanishAmbiguous ||
		foundSpecificationItem == null ||

		( isSecondarySpecificationCompoundCollection &&
		foundSpecificationItem.isSelfGeneratedAssumption() ) )
			{
			if( isUserGeneralizationWord )
				{
				if( isPrimarySpecificationWordSpanishAmbiguous &&
				adjustedQuestionSpecificationItem_ == null &&
				existingQuestionSpecificationItem != null &&
				existingQuestionSpecificationItem.hasSpecificationCompoundCollection() &&
				existingQuestionSpecificationItem.specificationCollectionNr() != secondarySpecificationCollectionNr )
					adjustedQuestionSpecificationItem_ = generalizationWordItem.firstSpecificationItem( false, false, existingQuestionSpecificationItem.questionParameter(), secondarySpecificationWordItem );

				if( isSecondarySpecificationCompoundCollection &&

				( adjustedQuestionSpecificationItem_ != null ||

				( !isPrimarySpecificationWordSpanishAmbiguous &&
				nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR ) ) )
					{
					// Check for adjusted compound question
					if( ( specificationResult = generalizationWordItem.findQuestionToBeAdjustedByCompoundCollection( false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, ( isPrimarySpecificationWordSpanishAmbiguous && existingQuestionSpecificationItem != null ? existingQuestionSpecificationItem.specificationCollectionNr() : secondarySpecificationCollectionNr ), secondaryRelationContextNr, adjustedQuestionSpecificationItem_, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );

					if( ( adjustedQuestionSpecificationItem = specificationResult.adjustedQuestionSpecificationItem ) != null )
						{
						if( adjustedQuestionWordItem_ != null )
							return adminItem_.startError( 1, moduleNameString_, "The adjusted question word item is already defined" );

						// Write adjusted compound question
						if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, adjustedQuestionSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						adjustedQuestionWordItem_ = generalizationWordItem;
						}
					}
				else
					{
					if( checkForConflictWithUserSpecification( generalizationWordItem, specificationWordItem, secondarySpecificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to check for a conflict with the user specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( hasFoundConflictWithUserSpecification_ )
						isBlockingCommonWordOfCompoundCollection = true;
					else
						{
						if( nonCompoundSpecificationCollectionNr == Constants.NO_COLLECTION_NR )
							nonCompoundSpecificationCollectionNr = adminItem_.nonCompoundCollectionNrInAllWords( secondarySpecificationCollectionNr );

						if( generalizationWordItem.firstUserSpecificationItem( false, false, secondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) != null ||

						( nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
						generalizationWordItem.firstUserSpecificationItem( false, false, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) != null ) )
							isBlockingCommonWordOfCompoundCollection = true;
						}
					}
				}

			if( !CommonVariables.hasDisplayedWarning &&
			!isBlockingCommonWordOfCompoundCollection &&

			( isExclusiveSpecification ||
			hasCorrectedAssumptionByOppositeQuestion ||
			secondarySpecificationItem.isExclusiveSpecification() ) )
				{
				alternativeAnotherPrimarySpecificationItem_ = null;

				if( isPrimarySpecificationWordSpanishAmbiguous &&
				primarySpecificationItem.isHiddenSpanishSpecification() )
					primarySpecificationItem = alternativeJustificationSpecificationItem( generalizationWordItem.hasFeminineProperNameEnding(), true, false, false, false, adminItem_.userSpecificationItem(), generalizationWordItem, specificationWordItem );

				if( primarySpecificationItem != null &&

				( hasCorrectedAssumptionByOppositeQuestion ||
				!primarySpecificationItem.isOlderItem() ||
				primarySpecificationItem.isSelfGenerated() ) )
					{
					// Ask a specification substitution question
					// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, primarySpecificationItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem_, ( isPrimarySpecificationWordSpanishAmbiguous && isUserGeneralizationWord && adjustedQuestionSecondarySpecificationItem_ != null ? adjustedQuestionSecondarySpecificationItem_ : secondarySpecificationItem ), null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a self-generated specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
						{
						if( isPrimarySpecificationWordSpanishAmbiguous &&
						isUserGeneralizationWord &&
						adjustedQuestionSecondarySpecificationItem_ == null &&
						specificationResult.foundSpecificationItem != null &&
						!primarySpecificationItem.isPossessive() &&
						secondarySpecificationItem.hasSpecificationCompoundCollection() )
							{
							adjustedQuestionSecondarySpecificationItem_ = secondarySpecificationItem;
							adjustedQuestionSpecificationItem_ = specificationResult.foundSpecificationItem.updatedSpecificationItem();
							}
						}
					else
						{
						// Has created question part
						// Change specification to question part
						if( foundSpecificationItem != null &&
						secondarySpecificationWordItem != specificationWordItem )
							{
							if( generalizationWordItem.replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

							if( createdSpecificationItem.isCorrectedAssumption() &&
							( obsoleteJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, createdSpecificationItem ) ) != null )
								{
								// Remove justification from corected assumption
								if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to delete an obsolete justification item" );
								}
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegativeConclusion, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasGeneralizationWordMasculineProperNameEnding;
		boolean isCommonWordWordSpanishAmbiguous;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isFemaleUserSpecificationWord = false;
		boolean isMaleUserSpecificationWord = false;
		int anotherPrimarySpecificationCollectionNr;
		int nonCompoundCollectionNr;
		int specificationCollectionNr;
		SpecificationItem firstNonCompoundCollectionSpecificationItem;
		SpecificationItem questionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem = null;
		SpecificationItem existingSpecificationItem = null;
		WordItem anotherPrimarySpecificationWordItem;
		WordItem commonWordItem;
		WordItem userSpecificationWordItem;
		WordItem collectionWordItem = null;

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

		if( ( specificationCollectionNr = commonWordItem.collectionNr( specificationWordTypeNr, specificationWordItem ) ) == Constants.NO_COLLECTION_NR )
			specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

		if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		isCommonWordWordSpanishAmbiguous = commonWordItem.isNounWordSpanishAmbiguous();
		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		relatedSpecificationItem = specificationResult.relatedSpecificationItem;

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		relatedSpecificationItem == null &&
		!userSpecificationItem.isPossessive() &&
		( nonCompoundCollectionNr = commonWordItem.nonCompoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
			{
			if( nonCompoundCollectionNr != specificationCollectionNr )
				{
				if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, nonCompoundCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

				relatedSpecificationItem = specificationResult.relatedSpecificationItem;
				}
			}

		if( relatedSpecificationItem == null )
			{
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
			hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem.hasMasculineProperNameEnding();

			if( userSpecificationItem != primarySpecificationItem &&
			!userSpecificationItem.hasRelationContext() &&
			!userSpecificationItem.isExclusiveSpecification() &&
			CommonVariables.nUserSpecificationWords == 1 &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
				{
				if( userSpecificationWordItem.isFemale() )
					isFemaleUserSpecificationWord = true;

				if( userSpecificationWordItem.isMale() )
					isMaleUserSpecificationWord = true;
				}

			// Negative feminine conclusion
			if( isFemaleUserSpecificationWord ||
			isMaleUserSpecificationWord ||

			( hasGeneralizationWordMasculineProperNameEnding &&
			anotherPrimarySpecificationWordItem.isFemale() ) ||

			// Negative masculine conclusion
			( hasGeneralizationWordFeminineProperNameEnding &&
			anotherPrimarySpecificationWordItem.isMale() ) ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() &&

			( hasGeneralizationWordFeminineProperNameEnding ||
			adminItem_.isUserQuestion() ) ) )
				{
				if( isFemaleUserSpecificationWord ||
				isMaleUserSpecificationWord )
					{
					if( anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() &&

					( anotherPrimarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ||

					( isFemaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem.isMale() ) ||

					( isMaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem.isFemale() ) ) )
						{
						if( drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					if( !isHiddenSpanishPrimarySpecification ||
					!isSpecificationWordSpanishAmbiguous )
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
				if( ( ( !hasGeneralizationWordFeminineProperNameEnding &&
				!hasGeneralizationWordMasculineProperNameEnding ) ||

				primarySpecificationItem.hasSpecificationCompoundCollection() ) &&

				( !isSpecificationWordSpanishAmbiguous ||
				anotherPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ||
				anotherPrimarySpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
					else
						{
						if( !isNegativeConclusion )
							collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr, null );
						}

					// Skip invalid questions (step 2)
					if( ( !isNegativeConclusion &&
					collectionWordItem != null &&
					generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, collectionWordItem ) == null ) ||

					( isSpecificationWordSpanishAmbiguous &&

					( generalizationWordItem.isUserGeneralizationWord ||
					existingSpecificationItem == null ||
					existingSpecificationItem.isHiddenSpanishSpecification() ) ) )
						{
						if( isSpecificationWordSpanishAmbiguous &&
						!primarySpecificationItem.isExclusiveSpecification() &&
						primarySpecificationItem.isUserSpecification() )
							{
							postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
							postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
							postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
							}
						else
							{
							if( primarySpecificationItem.hasRelationContext() ||
							primarySpecificationItem.isUserSpecification() ||
							generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == null )
								{
								if( askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, null, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								if( !primarySpecificationItem.hasSpecificationCollection() )
									{
									if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to delete an obsolete justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
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

				if( postponeNegativeConclusionSpecificationCollectionNr_ == anotherPrimarySpecificationCollectionNr )
					{
					if( drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}

				clearPostponedNegativeConclusionVariables();

				if( isCommonWordWordSpanishAmbiguous &&
				isSpecificationWordSpanishAmbiguous &&
				// specificationWordItem == commonWordItem &&
				CommonVariables.nUserSpecificationWords == 1 &&
				!primarySpecificationItem.hasSpecificationNonCompoundCollection() &&
				primarySpecificationItem.specificationCollectionNr() != anotherPrimarySpecificationCollectionNr &&

				( primarySpecificationItem.isUserSpecification() ||
				generalizationWordItem.isUserRelationWord ) )
					{
					questionSpecificationItem = generalizationWordItem.firstActiveAssignmentOrSpecificationItem( isArchivedAssignment, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem );

					if( ( questionSpecificationItem == null ||
					questionSpecificationItem.isOlderItem() ) &&

					generalizationWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem ) == null )
						{
						if( askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, questionSpecificationItem, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );
						}
					}
				else
					{
					if( anotherPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ||
					anotherPrimarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ||

					( !relatedSpecificationItem.isHiddenSpanishSpecification() &&
					anotherPrimarySpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
						{
						if( drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( isSpanishAmbiguousCompoundSpecification_ )
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

	private byte checkForConflictWithUserSpecification( WordItem generalizationWordItem, WordItem primarySpecificationWordItem, WordItem questionSpecificationWordItem )
		{
		SpecificationItem currentSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem currentSpecificationWordItem;

		hasFoundConflictWithUserSpecification_ = false;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification word item is undefined" );

		if( questionSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given question specification word item is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			do	{
				if( !currentSpecificationItem.isExclusiveSpecification() &&
				!currentSpecificationItem.isNegative() &&
				!currentSpecificationItem.isPossessive() &&
				// Current sentence
				!currentSpecificationItem.isOlderItem() &&
				// User sentence
				!currentSpecificationItem.isSelfGenerated() &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
					{
					foundSpecificationItem = currentSpecificationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, questionSpecificationWordItem );

					if( ( foundSpecificationItem == null ||

					( !foundSpecificationItem.isExclusiveSpecification() &&
					!foundSpecificationItem.isNegative() ) ) )
						hasFoundConflictWithUserSpecification_ = true;
					}
				}
			while( !hasFoundConflictWithUserSpecification_ &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeConclusion( boolean isArchivedAssignment, boolean isSpecificationWordSpanishAmbiguous, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isGeneralizationWordUserRelationWord;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isPrimarySameAsSecondarySpecification;
		boolean isUserGeneralizationWord;
		boolean hasAnotherSecondarySpecificationWithCompoundCollection = false;
		boolean hasFoundOldSpanishUserSpecification = false;
		boolean hasRelatedSpecificationsInSameSentence = false;
		boolean isCompoundUserSpecification = false;
		boolean isUserSpecificationSelected = false;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		int nonCompoundCollectionNr;
		int secondarySpecificationCollectionNr;
		int anotherPrimarySpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem createdJustificationItem;
		JustificationItem negativeAssumptionJustificationItem;
		JustificationItem negativeAssumptionOrConclusionJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingNegativeSpecificationItem;
		SpecificationItem existingPositiveSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem negativeArchivedAssignmentItem;
		SpecificationItem negativeConclusionPrimarySpecificationItem;
		SpecificationItem negativeConclusionAnotherPrimarySpecificationItem;
		SpecificationItem negativeConclusionSecondarySpecificationItem;
		SpecificationItem negativeConclusionAnotherSecondarySpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem foundNegativeAssumptionOrConclusionPrimarySpecificationItem = null;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem = null;
		WordItem anotherPrimarySpecificationWordItem;
		WordItem collectionWordItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		WordItem compoundGeneralizationWordItem = null;
		WordItem secondarySpecificationCommonWordItem = null;

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

		if( anotherPrimarySpecificationWordItem == null ||
		( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		// If exists, replace self-generated assumption for conclusion
		if( primarySpecificationItem.isSelfGeneratedAssumption() &&
		( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem.isArchivedAssignment(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), false, primarySpecificationWordItem, null ) ) != null )
			primarySpecificationItem = tempSpecificationItem;

		isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		existingPositiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

		if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != null )
			foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem.primarySpecificationItem();

		// No positive specification exists with this specification word
		if( ( existingPositiveSpecificationItem == null ||
		existingPositiveSpecificationItem.isHiddenSpanishSpecification() ) &&

		// Prefer primary specification with the least uncertainty
		( foundNegativeAssumptionOrConclusionPrimarySpecificationItem == null ||
		!primarySpecificationItem.hasSpecificationCollection() ||
		primarySpecificationItem.assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem.assumptionLevel() ) &&

		// Select correct compound specification collection when Spanish ambiguous
		( !isSpecificationWordSpanishAmbiguous ||
		isGeneralizationWordUserRelationWord ||
		anotherPrimarySpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ||

		( anotherPrimarySpecificationItem != null &&
		anotherPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ) ) &&

		// No question exists with this specification word
		generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == null )
			{
			if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

			isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			if( isPrimarySameAsSecondarySpecification )
				{
				if( primarySpecificationItem.hasSpecificationCompoundCollection() &&
				primarySpecificationItem.specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
					{
					collectionWordItem = collectionWordItemInAllWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem );

					if( collectionWordItem != null &&
					( anotherSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem ) ) == null )
						{
						if( ( anotherSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == null )
							{
							if( userSpecificationItem.isSpecificationWordSpanishAmbiguous() )
								anotherSecondarySpecificationItem = primarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem );
							else
								anotherSecondarySpecificationItem = userSpecificationItem;
							}
						}
					}
				else
					{
					if( CommonVariables.nUserSpecificationWords > 1 &&
					CommonVariables.nUserRelationWords == 0 &&
					userSpecificationItem.isSpecificationWordSpanishAmbiguous() )
						hasRelatedSpecificationsInSameSentence = true;
					}
				}
			else
				{
				if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
					{
					if( isHiddenSpanishPrimarySpecification &&
					isGeneralizationWordUserRelationWord &&
					anotherPrimarySpecificationItem != null &&
					anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() &&
					anotherPrimarySpecificationItem.isSelfGenerated() &&
					( collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr, null ) ) != null )
						{
						if( ( nonCompoundCollectionNr = collectionWordItem.nonCompoundCollectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
							nonCompoundCollectionNr = anotherPrimarySpecificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );

						if( nonCompoundCollectionNr > Constants.NO_COLLECTION_NR &&
						( tempSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, nonCompoundCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) != null )
							{
							hasFoundOldSpanishUserSpecification = true;
							secondarySpecificationItem = tempSpecificationItem;
							}
						}

					if( !hasFoundOldSpanishUserSpecification &&
					( anotherSecondarySpecificationItem = secondarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) == null )
						{
						if( secondarySpecificationItem.hasSpecificationCompoundCollection() &&
						secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
						( secondarySpecificationCommonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null )
							{
							if( secondarySpecificationCommonWordItem != primarySpecificationWordItem )
								anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem );
							}
						}
					}
				}

			if( isUserGeneralizationWord )
				{
				if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
					{
					if( userSpecificationWordItem.isNounWordSpanishAmbiguous() )
						isUserSpecificationWordSpanishAmbiguous = true;

					if( !isHiddenSpanishPrimarySpecification ||

					( !userSpecificationItem.isHiddenSpanishSpecification() &&

					( isUserSpecificationWordSpanishAmbiguous ||
					userSpecificationItem.isPossessive() ) ) )		
						{
						isUserSpecificationSelected = true;

						if( !isPrimarySameAsSecondarySpecification &&
						anotherSecondarySpecificationItem != null &&
						userSpecificationItem.hasSpecificationCompoundCollection() &&
						userSpecificationWordItem.hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
							isCompoundUserSpecification = true;
						}
					else
						{
						if( isSpecificationWordSpanishAmbiguous &&
						!anotherPrimarySpecificationWordItem.isNounWordSpanishAmbiguous() &&
						( simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null )
							{
							isUserSpecificationSelected = true;
							userSpecificationItem = simpleUserSpecificationItem;

							if( isHiddenSpanishPrimarySpecification &&
							anotherSecondarySpecificationItem != null &&
							!anotherSecondarySpecificationItem.isGeneralizationProperName() &&
							anotherPrimarySpecificationCollectionNr == anotherSecondarySpecificationItem.specificationCollectionNr() )
								hasAnotherSecondarySpecificationWithCompoundCollection = true;
							}
						}

					if( isUserSpecificationSelected &&
					userSpecificationItem.isPossessive() )
						oppositePossessiveDefinitionSpecificationItem = adminItem_.oppositePossessiveDefinitionSpecificationItem();
					}
				}

			existingNegativeSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

			if( hasFoundOldSpanishUserSpecification ||
			isCompoundUserSpecification ||

			( ( existingNegativeSpecificationItem == null ||

			( !isHiddenSpanishPrimarySpecification &&

			( existingNegativeSpecificationItem.isSelfGeneratedAssumption() ||
			primarySpecificationItem.hasSpecificationCompoundCollection() ) &&

			( existingNegativeSpecificationItem.isOlderItem() ||
			existingNegativeSpecificationItem.hasSpecificationNonCompoundCollection() ) ) ) &&

			generalizationWordItem.primarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, primarySpecificationItem ) == null ) ||

			( isUserGeneralizationWord &&
			isUserSpecificationSelected &&

			( ( !primarySpecificationItem.hasRelationContext() &&

			( existingNegativeSpecificationItem == null ||
			existingNegativeSpecificationItem.isOlderItem() ) ) ||

			( isHiddenSpanishPrimarySpecification &&
			existingNegativeSpecificationItem != null &&

			( existingNegativeSpecificationItem.isOlderItem() ||
			existingNegativeSpecificationItem.hasSpecificationNonCompoundCollection() ) ) ) ) )
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
							userSpecificationItem.hasSpecificationNonCompoundCollection() ) ||

							( anotherPrimarySpecificationItem != null &&
							anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() ) )
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
								negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationItem.isHiddenSpanishSpecification() ? userSpecificationItem : secondarySpecificationItem );
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
					if( !isPrimarySameAsSecondarySpecification &&
					isUserSpecificationSelected &&
					primarySpecificationItem.isSelfGenerated() &&
					primarySpecificationItem.hasSpecificationCompoundCollection() &&
					!userSpecificationItem.isQuestion() &&
					userSpecificationItem.hasSpecificationCompoundCollection() )
						compoundGeneralizationWordItem = anotherPrimarySpecificationWordItem.compoundGeneralizationWordItem( anotherPrimarySpecificationCollectionNr );

					if( compoundGeneralizationWordItem != null &&
					compoundGeneralizationWordItem.isNounWordSpanishAmbiguous() )
						{
						isSpanishAmbiguousCompoundSpecification_ = true;

						negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
						negativeConclusionAnotherPrimarySpecificationItem = null;
						negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
						negativeConclusionAnotherSecondarySpecificationItem = null;
						}
					else
						{
						negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
						negativeConclusionAnotherPrimarySpecificationItem = ( hasRelatedSpecificationsInSameSentence ? null : anotherPrimarySpecificationItem );
						negativeConclusionSecondarySpecificationItem = ( isCompoundUserSpecification ? userSpecificationItem : ( isPrimarySameAsSecondarySpecification ? ( hasRelatedSpecificationsInSameSentence ? anotherPrimarySpecificationItem : ( primarySpecificationItem != userSpecificationItem && !userSpecificationItem.hasRelationContext() && userSpecificationItem.specificationCollectionNr() == anotherPrimarySpecificationCollectionNr ? userSpecificationItem : anotherSecondarySpecificationItem ) ) : secondarySpecificationItem ) );
						negativeConclusionAnotherSecondarySpecificationItem = ( isCompoundUserSpecification || isPrimarySameAsSecondarySpecification ? null : anotherSecondarySpecificationItem );
						}
					}

				// Draw a negative negativeConclusion
				// Not yet explained in the design
				if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isExclusiveSpecification(), false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem.isSelfGeneratedAssumption() ? Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + anotherPrimarySpecificationWordItem.anyWordTypeString() + "\"" );

				createdJustificationItem = specificationResult.createdJustificationItem;

				// Found specification
				if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
					{
					if( createdJustificationItem != null )
						{
						// Cleanup after found specification
						if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "The found specification item is undefined" );

						if( negativeAssumptionOrConclusionJustificationItem == null )
							{
							if( foundSpecificationItem.isSelfGeneratedAssumption() )
								{
								if( createdJustificationItem.isConclusionJustification() )
									{
									if( foundSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

									if( isArchivedAssignment )
										{
										if( ( negativeArchivedAssignmentItem = generalizationWordItem.firstAssignmentItem( false, false, true, true, false, true, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't find a past tense negative assumption assignment" );

										// Also mark the past tense negative assumption assignment as concluded
										if( negativeArchivedAssignmentItem.markAsConcludedAssumption() != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to mark a past tense negative assumption assignment as concluded" );
										}
									}
								}
							else
								{
								if( generalizationWordItem.isJustificationInUse( createdJustificationItem ) )
									{
									// Check for negative assumption justification
									negativeAssumptionJustificationItem = foundSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION );

									if( negativeAssumptionJustificationItem != null &&
									negativeAssumptionJustificationItem.primarySpecificationItem() == negativeConclusionPrimarySpecificationItem )
										{
										if( generalizationWordItem.replaceJustification( negativeAssumptionJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
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
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				else
					{
					// Created specification
					// Replace corrected negative conclusion
					if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() )
						{
						if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created negative conclusion" );

						relatedSpecificationItem = specificationResult.relatedSpecificationItem;

						if( relatedSpecificationItem != null &&
						relatedSpecificationItem.isOlderItem() )
							{
							if( generalizationWordItem.replaceOrDeleteSpecification( false, relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						{
						if( isCompoundUserSpecification &&
						!isPrimarySameAsSecondarySpecification &&
						anotherSecondarySpecificationItem != null &&
						// Check for answered question justification
						createdSpecificationItem.answeredQuestionJustificationItem() == null )
							{
							if( generalizationWordItem.changeJustificationOfNegativeAssumptions( userSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace a question justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, boolean isUniqueUserRelation, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem secondarySpecificationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		SpecificationResultType specificationResult;
		boolean hasFeminineProperNameEnding;
		boolean isAssignment;
		boolean isConclusionAssignment;
		boolean isForcingNewJustification;
		boolean isUserGeneralizationWord;
		boolean isSecondarySpecificationWordSpanishAmbiguous;
		boolean isAssumption = ( userAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL );
		boolean isReplacingObsoleteJustifications = false;
		int foundRelationContextNr;
		int nContextRelations;
		int generalizationContextNr = Constants.NO_CONTEXT_NR;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem collectedWithItselfCommonWordItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr == Constants.NO_CONTEXT_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
		existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, !isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

		if( existingSpecificationItem == null ||
		// Allow existing conclusion specification to change its relation context
		existingSpecificationItem.isSelfGenerated() )
			{
			isSecondarySpecificationWordSpanishAmbiguous = secondarySpecificationWordItem.isNounWordSpanishAmbiguous();

			if( isUserGeneralizationWord &&
			!isSecondarySpecificationWordSpanishAmbiguous &&

			( existingSpecificationItem == null ||
			CommonVariables.nUserRelationWords > 1 ) &&

			( foundRelationContextNr = generalizationWordItem.contextNr( specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
				{
				if( ( nContextRelations = adminItem_.nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
					{
					if( ( contextResult = adminItem_.getRelationContext( isArchivedAssignment, !isPossessive, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the relation context number" );

					generalizationContextNr = contextResult.contextNr;
					}
				else
					generalizationContextNr = foundRelationContextNr;
				}

			conclusionSpecificationItem = contextResult.conclusionSpecificationItem;
			existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Find the non-reversible specification (active assignments)
			if( isSecondarySpecificationWordSpanishAmbiguous )
				secondarySpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );
			else
				secondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Skip if possessive user specification exists
			if( secondarySpecificationItem != null &&

			( !isSecondarySpecificationWordSpanishAmbiguous ||
			secondarySpecificationItem.hasSpecificationNonCompoundCollection() ||
			existingUserSpecificationItem == null ||
			!existingUserSpecificationItem.hasRelationContext() ) )
				{
				if( ( existingSpecificationItem == null ||
				secondarySpecificationItem.isUserSpecification() ||
				relationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, secondarySpecificationItem ) == null ) &&

				secondarySpecificationItem.hasNewInformation() )
					{
					hasFeminineProperNameEnding = ( isPossessive &&
													isSecondarySpecificationWordSpanishAmbiguous &&
													relationWordItem.hasFeminineProperNameEnding() );

					isConclusionAssignment = ( ( isArchivedAssignment &&
												isAssumption ) ||

												( !isAssumption &&

												( isArchivedAssignment ||
												secondarySpecificationItem.hasExclusiveGeneralizationCollection() ) ) );

					isAssignment = ( isConclusionAssignment ||

									( existingUserSpecificationItem != null &&
									existingUserSpecificationItem.isAssignment() ) );

					// Typically for the Spanish language
					isForcingNewJustification = ( ( isSecondarySpecificationWordSpanishAmbiguous ||
												secondarySpecificationItem.isUserAssignment() ||

												( !isArchivedAssignment &&
												isPossessive &&
												isUserGeneralizationWord &&
												existingSpecificationItem != null &&
												secondarySpecificationItem.isUserSpecification() ) ) &&

												secondarySpecificationItem.hasSpecificationCollection() );

					// Typically for the Spanish language
					if( isSecondarySpecificationWordSpanishAmbiguous &&
					secondarySpecificationWordItem == specificationWordItem &&

					( !isPossessive ||
					!relationWordItem.isMale() ) )
						spanishAmbiguousSpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

					// Draw a possessive reversible conclusion
					// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
					if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( hasFeminineProperNameEnding, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, null, null, secondarySpecificationItem, null, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

					if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
						foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

					if( conclusionSpecificationItem == null )
						{
						if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
							{
							if( existingSpecificationItem != null &&
							foundSpecificationItem != null )
								{
								if( existingSpecificationItem.isConcludedAssumption() )
									isReplacingObsoleteJustifications = true;
								else
									{
									if( !isSecondarySpecificationWordSpanishAmbiguous &&
									existingSpecificationItem.isSelfGeneratedAssumption() &&
									foundSpecificationItem.isSelfGeneratedConclusion() &&
									foundSpecificationItem.hasSpecificationCompoundCollection() )
										{
										if( relationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption specification by a conclusion in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

										isReplacingObsoleteJustifications = true;
										}
									}

								if( isReplacingObsoleteJustifications )
									{
									if( relationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace obsolete justification items in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						else
							{
							if( !isPossessive &&
							secondarySpecificationItem.isUserSpecification() &&
							createdSpecificationItem.isOlderItem() &&
							createdSpecificationItem.isSelfGeneratedAssumption() &&
							createdSpecificationItem.hasOnlyOneRelationWord() )
								{
								if( createdSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

								if( relationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write the concluded assumption in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}
							}

						if( isPossessive &&
						( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) != null )
							{
							if( !createdOrFoundSpecificationItem.hasSpecificationCompoundCollection() )
								{
								if( adminItem_.makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, null, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}

							if( isSecondarySpecificationWordSpanishAmbiguous &&
							( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem.collectedWithItselfCommonWordItem() ) != null )
								{
								// If the reversed specification doesn't already exist
								if( !isArchivedAssignment &&
								generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, true, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == null )
									{
									// Generate the reversed specification
									if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem.relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}

								if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem.firstNounSpecificationGeneralizationItem() ) != null )
									{
									do	{
										if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
											return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

										partOfSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem );

										if( partOfSpecificationItem != null &&
										partOfSpecificationItem.isPartOf() &&

										( !createdOrFoundSpecificationItem.hasAssumptionLevel() ||
										// Skip if 'part of' conclusion exists
										relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentGeneralizationWordItem, null ) == null ) )
											{
											// Draw a 'part of' generalization conclusion
											// Not yet explained in the design
											if( adminItem_.addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, partOfSpecificationItem, null, relationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
											}
										}
									while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
									}

								if( relationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) == null &&
								( secondarySpecificationItem = collectedWithItselfCommonWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, createdOrFoundSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
									{
									if( drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, null, createdOrFoundSpecificationItem, null, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}

						if( isUserGeneralizationWord &&
						specificationWordItem.isUserSpecificationWord )
							{
							if( isSecondarySpecificationWordSpanishAmbiguous )
								{
								existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );

								if( existingSpecificationItem != null &&
								existingSpecificationItem.isHiddenSpanishSpecification() &&
								existingSpecificationItem.specificationCollectionNr() != spanishAmbiguousSpecificationCollectionNr )
									{
									// Add context to hidden specification
									if( generalizationWordItem.addContext( false, relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem.relationContextNr(), specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									// If not hidden anymore
									if( !existingSpecificationItem.isHiddenSpanishSpecification() )
										{
										if( relationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, existingSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to write the hidden assumption in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}

							if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						{
						if( foundSpecificationItem != null &&
						foundSpecificationItem.isSelfGeneratedAssumption() )
							{
							if( foundSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to mark the replacing specification as concluded assumption in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

							// Write adjusted specification
							if( relationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, foundSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

							if( ( firstJustificationItem = conclusionSpecificationItem.firstJustificationItem() ) == null )
								return adminItem_.startError( 1, moduleNameString_, "The conclusion specification item has no justifications" );

							if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != null )
								{
								// Attach older conclusion justifications to the created conclusion justification item
								if( createdJustificationItem.changeAttachedJustification( firstJustificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );

								if( relationWordItem.replaceOrDeleteSpecification( false, conclusionSpecificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

								firstJustificationItem = foundSpecificationItem.firstJustificationItem();

								if( firstJustificationItem != null &&
								!firstJustificationItem.isPossessiveReversibleConclusion() )
									{
									if( firstJustificationItem.changeAttachedJustification( createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to delete obsolete assumption justification items in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
									}

								if( relationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
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

		// Draw a specification generalization conclusion
		// Not yet explained in the design
		if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem.isExclusiveSpecification(), false, secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPartOf(), secondarySpecificationItem.isPossessive(), false, false, true, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, secondarySpecificationItem.specificationWordItem(), secondarySpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );

		return Constants.RESULT_OK;
		}

	private static WordItem collectionWordItemInAllWords( int compoundCollectionNr, WordItem commonWordItem )
		{
		WordItem currentCollectionWordItem;

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null &&
		( currentCollectionWordItem = CommonVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem != commonWordItem &&
				currentCollectionWordItem.hasCollectionNr( compoundCollectionNr, commonWordItem ) )
					return currentCollectionWordItem;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return null;
		}


	// Constructor

	protected AdminConclusion( AdminItem adminItem )
		{
		String errorString = null;

		hasFoundConflictWithUserSpecification_ = false;
		isSpanishAmbiguousCompoundSpecification_ = false;

		postponeNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		alternativeAnotherPrimarySpecificationItem_ = null;
		adjustedQuestionSecondarySpecificationItem_ = null;
		adjustedQuestionSpecificationItem_ = null;
		postponeNegativeConclusionPrimarySpecificationItem_ = null;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;

		adjustedQuestionWordItem_ = null;

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

	protected void initializeAdminConclusionVariables()
		{
		clearPostponedNegativeConclusionVariables();

		hasFoundConflictWithUserSpecification_ = false;
		isSpanishAmbiguousCompoundSpecification_ = false;

		adjustedQuestionSecondarySpecificationItem_ = null;
		adjustedQuestionSpecificationItem_ = null;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;

		adjustedQuestionWordItem_ = null;
		}

	protected boolean hasUnprocessedNegativeConclusion()
		{
		return ( postponeNegativeConclusionSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
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

		if( ( answeredQuestionSpecificationItem = generalizationWordItem.firstRecentlyAnsweredQuestionSpecificationItem( Constants.WORD_PARAMETER_SINGULAR_VERB_IS ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any answered question specification" );

		do	{
			if( userSpecificationWordItem.hasCollectionNr( answeredQuestionSpecificationItem.specificationCollectionNr() ) )
				{
				if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no specification word" );

				// Check if already exists as positive specification
				if( generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == null )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem.firstJustificationItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem.hasPrimaryQuestion() &&
						!currentJustificationItem.hasPossessivePrimarySpecification() )
							{
							if( drawNegativeConclusion( answeredQuestionSpecificationItem.isArchivedAssignment(), false, answeredQuestionSpecificationItem.generalizationWordTypeNr(), answeredQuestionSpecificationItem.specificationWordTypeNr(), answeredQuestionSpecificationItem.generalizationContextNr(), answeredQuestionSpecificationItem.specificationContextNr(), currentJustificationItem.primarySpecificationItem(), currentJustificationItem.secondarySpecificationItem(), userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem.recentlyAnsweredQuestionSpecificationItem( false ) ) != null );

		return Constants.RESULT_OK;
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, WordItem generalizationWordItem )
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

		if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
				( currentAssignmentItem = generalizationWordItem.firstSelectedSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != null )
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
			foundOnlyOptionLeftSpecificationItem != null )
				{
				// Draw an only option left conclusion
				// Not yet explained in the design
				if( adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
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

		if( isPossessive &&
		!isArchivedAssignment &&
		specificationWordItem.isNounWordSpanishAmbiguous() &&
		relationWordItem.hasFeminineProperNameEnding() &&
		( feminineSpecificationWordItem = specificationWordItem.feminineCollectionWordItem() ) != null )
			{
			if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSimpleNegativeSpanishConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		int nonCompoundCollectionNr;
		int userSpecificationCollectionNr;
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
		( nonCompoundCollectionNr = specificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
			{
			if( !userSpecificationWordItem.isNounWordSpanishAmbiguous() &&
			( collectionWordItem = specificationWordItem.collectionWordItem( nonCompoundCollectionNr, null ) ) != null )
				{
				if( collectionWordItem.isNounWordSpanishAmbiguous() )
					anotherPrimarySpecificationItem = collectionWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, collectionWordItem );
				else
					{
					userSpecificationCollectionNr = simpleUserSpecificationItem.specificationCollectionNr();

					if( userSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
					( commonWordItem = userSpecificationWordItem.commonWordItem( userSpecificationCollectionNr ) ) != null )
						{
						if( commonWordItem.isNounWordSpanishAmbiguous() )
							anotherPrimarySpecificationItem = commonWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, false, false, true, Constants.NO_QUESTION_PARAMETER, nonCompoundCollectionNr, Constants.NO_CONTEXT_NR, collectionWordItem );
						}
					}

				if( anotherPrimarySpecificationItem != null &&
				anotherPrimarySpecificationItem.hasSpecificationCollection() )
					{
					if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isPossessive, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
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
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
					{
					if( !currentSpecificationItem.hasSpecificationCollection() &&
					!currentSpecificationItem.isExclusiveSpecification() )
						{
						assumptionLevel = ( secondarySpecificationItem.assumptionLevel() < currentSpecificationItem.assumptionLevel() ? currentSpecificationItem.assumptionLevel() : secondarySpecificationItem.assumptionLevel() );

						if( adminItem_.collectGeneralizationWordWithPreviousOne( false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

						if( currentSpecificationItem.hasCurrentCreationSentenceNr() )
							{
							if( drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
							}

						if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isMakingPartOfAssumption, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean hasCurrentSpecificationCollection;
		boolean hasCurrentSpecificationCompoundCollection;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasGeneralizationWordMasculineProperNameEnding;
		boolean hasPrimarySpecificationNonCompoundCollection;
		boolean isDefinitionSpanishAmbiguous;
		boolean isExclusiveCurrentSpecification;
		boolean isFemaleOrMaleGeneralizationWord;
		boolean isFoundPrimaryHiddenSpanishSpecification;
		boolean isGeneralizationWordUserRelationWord;
		boolean isIncludingRelationWord;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationWord;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		boolean isExclusiveUserSpecification = false;
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		boolean isNegativeConclusion = false;
		boolean isPossessiveUserSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		int currentSpecificationCollectionNr;
		int relationContextNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		SpecificationItem alternativeAnotherPrimarySpecificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem archivedAssignmentItem;
		SpecificationItem bestMatchingRelationSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem collectedWithItselfSpecificationItem = null;
		WordItem currentSpecificationWordItem;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a reasoning type" );

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( isMakingPartOfAssumption )
			{
			if( adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();

			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem.hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
			hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem.hasMasculineProperNameEnding();

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			isFemaleOrMaleGeneralizationWord = generalizationWordItem.isFemaleOrMale();
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isUserSpecificationWord = specificationWordItem.isUserSpecificationWord;

			oppositePossessiveDefinitionSpecificationItem = adminItem_.oppositePossessiveDefinitionSpecificationItem();

			if( isSpecificationWordSpanishAmbiguous )
				collectedWithItselfSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

			if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
				{
				isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
				isPossessiveUserSpecification = userSpecificationItem.isPossessive();
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
				userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
				}

			do	{
				// Select non-possessive definition
				if( !currentSpecificationItem.isPossessive() &&
				currentSpecificationItem.isSpecificationNoun() &&
				!currentSpecificationItem.isSpecificationGeneralization() &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
					{
					if( currentSpecificationWordItem != generalizationWordItem )
						{
						isExclusiveCurrentSpecification = currentSpecificationItem.isExclusiveSpecification();
						currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
						hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR );

						if( ( bestMatchingRelationSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, false, false, questionParameter, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) != null )
							{
							if( !isQuestion &&
							!bestMatchingRelationSpecificationItem.isExclusiveSpecification() )
								isNegativeConclusion = true;
							}

						if( !hasRelationWord ||
						!isPossessiveUserSpecification ||
						( foundPrimarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, questionParameter, specificationWordItem, relationWordItem ) ) == null )
							foundPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isArchivedAssignment, false, false, questionParameter, specificationWordItem );

						if( foundPrimarySpecificationItem == null )
							{
							// Check for specification conflict
							if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.relationContextNr(), currentSpecificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem.isHiddenSpanishSpecification();

							if( !CommonVariables.hasDisplayedWarning &&
							isExclusiveCurrentSpecification &&
							( feminineOrMasculineSpecificationItem = currentSpecificationWordItem.firstFeminineOrMasculineSpecificationItem() ) != null )
								{
								isIncludingRelationWord = ( hasRelationWord &&
															feminineOrMasculineSpecificationItem.hasSpecificationCollection() &&

															( isFoundPrimaryHiddenSpanishSpecification ||
															foundPrimarySpecificationItem.hasNewInformation() ) );

								if( ( !hasRelationWord ||
								isIncludingRelationWord ||
								isPossessiveUserSpecification ) &&

								// Matching feminine word
								( ( ( hasGeneralizationWordFeminineProperNameEnding &&
								currentSpecificationWordItem.isFemale() ) ||

								// Matching masculine word
								( hasGeneralizationWordMasculineProperNameEnding &&
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
									if( !isFemaleOrMaleGeneralizationWord )
										{
										if( isIncludingRelationWord ||

										( isUserGeneralizationWord &&
										oppositePossessiveDefinitionSpecificationItem == null ) ||

										( oppositePossessiveDefinitionSpecificationItem != null &&
										oppositePossessiveDefinitionSpecificationItem.specificationCollectionNr() == userSpecificationCollectionNr ) )
											{
											// Make a feminine or masculine proper name ending assumption
											// Not yet explained in the design
											if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem : null ), currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : null ) ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( specificationResult.createdSpecificationItem == null )
												{
												if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justification items in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												{
												if( !isUserSpecificationWordSpanishAmbiguous &&

												// Has missed drawing some negative conclusions
												( postponeNegativeConclusionSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
												postponeNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr ) )
													{
													// Draw missing negative conclusions
													if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, false, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											clearPostponedNegativeConclusionVariables();
										}

									if( !isArchivedAssignment &&
									isIncludingRelationWord &&
									bestMatchingRelationSpecificationItem != null &&
									!foundPrimarySpecificationItem.isHiddenSpanishSpecification() &&
									( relationContextNr = bestMatchingRelationSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR )
										{
										if( drawPossessiveReversibleConclusion( false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}

							if( ( bestMatchingRelationSpecificationItem == null &&

							( hasCurrentSpecificationCollection ||
							!foundPrimarySpecificationItem.hasSpecificationCollection() ) ) ||

							( !isExclusiveCurrentSpecification &&

							( isGeneralizationProperName ||

							( hasCurrentSpecificationCollection &&
							isDefinitionSpanishAmbiguous ) ) ) ||

							( isDefinitionSpanishAmbiguous &&
							hasCurrentSpecificationCollection &&
							isExclusiveCurrentSpecification &&
							!isUserSpecificationWord &&
							!currentSpecificationItem.isOlderItem() ) )
								{
								hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem.hasSpecificationNonCompoundCollection();

								if( ( !isExclusiveSpecification &&
								isExclusiveCurrentSpecification ) ||

								( isGeneralizationProperName &&

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
									hasCurrentSpecificationCompoundCollection = ( currentSpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) > Constants.NO_COLLECTION_NR );

									// Ask a question or draw a negative conclusion
									if( isGeneralizationProperName &&
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
									currentSpecificationItem.hasSpecificationNonCompoundCollection() ) ) ) ||

									// Question
									hasCorrectedAssumptionByOppositeQuestion ||
									adminItem_.hasSuggestiveQuestionAssumption() ) )
										{
										if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									{
									// Make an assumption or draw a conclusion
									if( isAssumption ||
									currentSpecificationItem.hasAssumptionLevel() )
										{
										alternativeAnotherPrimarySpecificationItem = null;
										primarySpecificationItem = null;

										if( isSpecificationWordSpanishAmbiguous )
											primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
										else
											{
											primarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null );

											if( primarySpecificationItem == null ||
											primarySpecificationItem .isUserAssignment() )
												primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );
											}

										existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationWordItem );

										if( primarySpecificationItem != null )
											{
											alternativePrimarySpecificationItem = ( hasRelationWord || isSpecificationWordSpanishAmbiguous ? alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) : null );

											if( ( isSpecificationWordSpanishAmbiguous &&
											primarySpecificationItem.isHiddenSpanishSpecification() ) ||

											( existingSpecificationItem != null &&
											alternativePrimarySpecificationItem != null &&
											alternativePrimarySpecificationItem.specificationWordItem() == specificationWordItem &&

											( ( isSpecificationWordSpanishAmbiguous &&
											existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) ) ||

											( !isSpecificationWordSpanishAmbiguous &&
											!existingSpecificationItem.hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&

											( isUserGeneralizationWord ||
											!specificationWordItem.isCollectionSpanishAmbiguous( alternativePrimarySpecificationItem.specificationCollectionNr() ) ) ) ) ) )
												{
												if( alternativePrimarySpecificationItem != null )
													alternativeAnotherPrimarySpecificationItem = alternativeAnotherPrimarySpecificationItem_;

												if( isSpecificationWordSpanishAmbiguous &&
												isGeneralizationWordUserRelationWord &&
												alternativeAnotherPrimarySpecificationItem != null &&
												existingSpecificationItem != null &&
												userSpecificationItem != null &&
												primarySpecificationItem.isHiddenSpanishSpecification() &&
												!userSpecificationItem.isArchivedAssignment() )
													{
													primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, userSpecificationItem.specificationWordItem() );

													if( primarySpecificationItem != null &&
													// Check if justification already exists
													existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) )
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												else
													primarySpecificationItem = alternativePrimarySpecificationItem;
												}
											else
												{
												if( userSpecificationItem != null &&
												userSpecificationItem.isPossessive() &&
												primarySpecificationItem.hasAssumptionLevel() )
													{
													existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationWordItem );

													if( existingSpecificationItem != null &&
													existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null )
														// Skip if similar justification already exists, without relation word
														primarySpecificationItem = null;
													}
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

											if( userSpecificationItem != null &&
											userSpecificationItem.isArchivedAssignment() &&
											userSpecificationItem.isGeneralizationNoun() )
												{
												isAssignment = true;
												isArchivedAssignment = true;
												archivedAssignmentItem = userSpecificationItem;
												}

											// Make a compound specification substitution assumption, which is actually an uncertain conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, ( archivedAssignmentItem == null ? currentSpecificationItem : archivedAssignmentItem ), null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( specificationResult.createdSpecificationItem == null &&

											( specificationResult.createdJustificationItem != null ||
											!primarySpecificationItem.hasRelationContext() ||
											foundPrimarySpecificationItem.isOlderItem() ) )
												{
												if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete compound justification items in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
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
										generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isArchivedAssignment, false, false, true, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) )
											{
											primarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

											if( primarySpecificationItem != null &&
											primarySpecificationItem.isHiddenSpanishSpecification() &&
											( primarySpecificationItem = alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != null )
												alternativeAnotherPrimarySpecificationItem = alternativeAnotherPrimarySpecificationItem_;

											if( primarySpecificationItem == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );

											if( !primarySpecificationItem.isPartOf() )
												{
												// Draw a compound specification substitution conclusion
												// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
												//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
												if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem.isEveryGeneralization(), isExclusiveSpecification, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

												if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
													{
													if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() )
														{
														if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created specification" );

														if( specificationResult.relatedSpecificationItem != null )
															{
															// Replace incorrect assumption
															if( generalizationWordItem.replaceOrDeleteSpecification( false, specificationResult.relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														{
														if( isGeneralizationProperName )
															{
															if( !hasRelationWord &&
															hasCurrentSpecificationCollection &&
															!isExclusiveSpecification &&
															!isSpecificationWordSpanishAmbiguous &&
															isUserGeneralizationWord &&
															primarySpecificationItem.hasSpecificationNonCompoundCollection() &&
															adminItem_.isSpanishCurrentLanguage() )
																{
																if( drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationItem.specificationWordTypeNr(), currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																	return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															{
															if( adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												}
											}
										}
									}

								if( !isExclusiveCurrentSpecification &&
								isGeneralizationProperName &&
								hasCurrentSpecificationCollection )
									{
									if( adminItem_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption by generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType findSpecificationResult;
		SpecificationResultType returnSpecificationResult = new SpecificationResultType();
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem previousSpecificationItem = null;
		SpecificationItem lastCreatedSpecificationItem = null;
		WordItem currentGeneralizationWordItem;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		do	{
			if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
				{
				previousSpecificationItem = lastCreatedSpecificationItem;

				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startSpecificationResultError( 1, moduleNameString_, "I have found an undefined generalization word" );

					if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null )
						{
						if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem.hasAssumptionLevel(), foundSpecificationItem.isInactiveAssignment(), foundSpecificationItem.isArchivedAssignment(), foundSpecificationItem.isExclusiveSpecification(), true, Constants.NO_QUESTION_PARAMETER, foundSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( !CommonVariables.hasDisplayedWarning )
							{
							if( ( findSpecificationResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );

							if( ( relatedSpecificationItem = findSpecificationResult.relatedSpecificationItem ) != null )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.hasAssumptionLevel(), relatedSpecificationItem.isInactiveAssignment(), relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), true, relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem.specificationWordItem(), null ) != Constants.RESULT_OK )
									return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								returnSpecificationResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
								}
							}
						}
					}
				while( !CommonVariables.hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
				}
			}
		while( !CommonVariables.hasDisplayedWarning &&
		// Do until no more conclusions or questions are created
		previousSpecificationItem != null );

		return returnSpecificationResult;
		}

	protected SpecificationItem alternativeAnotherPrimarySpecificationItem()
		{
		return alternativeAnotherPrimarySpecificationItem_;
		}

	protected SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNameEnding, boolean isCheckingForPossessiveSpecification, boolean isCheckingForQuestion, boolean isNegative, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem oppositeSpecificationItem;
		SpecificationItem possessiveSpecificationItem;
		SpecificationItem questionSpecificationItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;

		alternativeAnotherPrimarySpecificationItem_ = null;

		if( userSpecificationItem == null ||
		generalizationWordItem == null ||
		specificationWordItem == null )
			return null;

		if( generalizationWordItem.isUserRelationWord &&
		userSpecificationItem.isPossessive() )
			return userSpecificationItem;

		if( !hasGeneralizationWordFeminineProperNameEnding &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null )
			{
			// Try to find opposite specification
			oppositeSpecificationItem = userGeneralizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

			if( oppositeSpecificationItem != null &&
			!oppositeSpecificationItem.isHiddenSpanishSpecification() )
				return oppositeSpecificationItem;

			if( isCheckingForPossessiveSpecification &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
				{
				possessiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, true, Constants.NO_QUESTION_PARAMETER, userSpecificationWordItem );

				// Try to find a possessive specification
				if( possessiveSpecificationItem != null &&
				!possessiveSpecificationItem.isHiddenSpanishSpecification() )
					{
					alternativeAnotherPrimarySpecificationItem_ = adminItem_.oppositePossessiveDefinitionSpecificationItem();
					return possessiveSpecificationItem;
					}
				}

			if( isCheckingForQuestion &&
			// Try to find question
			( questionSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) ) != null )
				return questionSpecificationItem;
			}

		return null;
		}

	protected SpecificationItem spanishAmbiguousCompoundPrimarySpecificationItem()
		{
		return spanishAmbiguousCompoundPrimarySpecificationItem_;
		}

	protected SpecificationItem spanishAmbiguousCompoundAnotherPrimarySpecificationItem()
		{
		return spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
		}

	protected WordItem adjustedQuestionWordItem()
		{
		return adjustedQuestionWordItem_;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
