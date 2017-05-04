/*	Class:			AdminReasoningOld
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (old design)
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

	private SpecificationItem alternativeAnotherPrimarySpecificationItem_;
	private SpecificationItem possessiveDefinitionSpecificationItem_;
	private SpecificationItem oppositePossessiveDefinitionSpecificationItem_;
	private SpecificationItem postponeNegativeConclusionPrimarySpecificationItem_;
	private SpecificationItem postponeNegativeConclusionAnotherPrimarySpecificationItem_;
	private SpecificationItem spanishAmbiguousCompoundPrimarySpecificationItem_;
	private SpecificationItem spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
	private SpecificationItem spanishQuestionPrimarySpecificationItem_;

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

	private static boolean isFeminineOrMasculineProperNameEnding( boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
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
					relationWordItem.hasFeminineProperNameEnding() ) ||

					( !isPossessive &&
					generalizationWordItem.hasFeminineProperNameEnding() ) )
						return true;
					}
				else
					{
					// Possessive (relation)
					if( ( isPossessive &&
					relationWordItem != null &&

					( ( isFemaleSpecificationWord &&
					relationWordItem.hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					relationWordItem.hasMasculineProperNameEnding() ) ) ) ||

					// Non-possessive (generalization)
					( !isPossessive &&

					( ( isFemaleSpecificationWord &&
					generalizationWordItem.hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					generalizationWordItem.hasMasculineProperNameEnding() ) ) ) )
						return true;
					}
				}
			}

		return false;
		}

	private byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isArchivedAssignment, boolean isNegativeConclusion, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
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

		if( ( specificationCollectionNr = commonWordItem.collectionNr( specificationWordTypeNr, specificationWordItem ) ) == Constants.NO_COLLECTION_NR )
			specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		isCommonWordWordSpanishAmbiguous = commonWordItem.isNounWordSpanishAmbiguous();
		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		relatedSpecificationItem == null &&
		!userSpecificationItem.isPossessive() &&
		( nonCompoundCollectionNr = commonWordItem.nonCompoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
			{
			if( nonCompoundCollectionNr != specificationCollectionNr )
				{
				if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, nonCompoundCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

				relatedSpecificationItem = relatedResult.relatedSpecificationItem;
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

			if( isFemaleUserSpecificationWord ||
			isMaleUserSpecificationWord ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() &&

			( hasGeneralizationWordFeminineProperNameEnding ||
			userSpecificationItem.isQuestion() ) ) )
				{
				if( isFemaleUserSpecificationWord ||
				isMaleUserSpecificationWord )
					{
					if( anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() &&

					( anotherPrimarySpecificationCollectionNr == userSpecificationItem.specificationCollectionNr() ||

					( isFemaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem.isMale() ) ||

					( isMaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem.isFemale() ) ) &&

					// Draw negative conclusion
					drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
							collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr );
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
								if( adminItem_.askQuestions() != Constants.RESULT_OK )
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
				drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				clearPostponedNegativeConclusionVariables();

				// Typically for the Spanish language
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
						if( adminItem_.askQuestions() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );

						// Clean-up obsolete justifications
						if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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

						if( isHiddenSpanishPrimarySpecification &&
						specificationWordItem == commonWordItem &&
						anotherPrimarySpecificationWordItem == commonWordItem &&
						userSpecificationItem != relatedSpecificationItem &&
						primarySpecificationItem != relatedSpecificationItem &&
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

	private byte correctSuggestiveAssumptionsByOppositeQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		JustificationItem correctedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem oldSuggestiveSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( !secondarySpecificationItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is no assignment" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		if( ( correctedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem.correctedAssumptionByOppositeQuestionJustificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old corrected assumption by opposite question justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		// Change primary user specification by primary corrected question specification
		if( correctedAssumptionByOppositeQuestionJustificationItem.changePrimarySpecification( createdNewSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to change the primary specification of the corrected assumption by opposite question justification item" );

		if( ( secondarySpecificationWordItem = correctedAssumptionByOppositeQuestionJustificationItem.secondarySpecificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );

		// Correct generalization assumption
		if( ( primarySpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );

		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution assumption" );

		if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the corrected generalization assumption" );

		if( foundSpecificationItem.attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );

		return Constants.RESULT_OK;
		}

	private byte drawNegativeConclusion( boolean isArchivedAssignment, boolean isSpecificationWordSpanishAmbiguous, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasDisplayedMoreSpecificNonExclusiveSpecification;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isPrimarySameAsSecondarySpecification;
		boolean isUserGeneralizationWord;
		boolean hasAnotherSecondarySpecificationWithCompoundCollection = false;
		boolean hasFoundOldSpanishUserSpecification = false;
		boolean hasRelatedSpecificationsInSameSentence = false;
		boolean isCompoundUserSpecification = false;
		boolean isPostponingNegativeConclusion = false;
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
		SpecificationItem existingQuestionSpecificationItem;
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

		if( anotherPrimarySpecificationWordItem == null ||
		( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		// If exists, replace self-generated assumption for conclusion
		if( primarySpecificationItem.isSelfGeneratedAssumption() &&
		( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem.isArchivedAssignment(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), false, primarySpecificationWordItem, null ) ) != null )
			primarySpecificationItem = tempSpecificationItem;

		existingPositiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

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

		// Select correct compound specification collection when Spanish ambiguous
		( !isSpecificationWordSpanishAmbiguous ||
		generalizationWordItem.isUserRelationWord ||
		anotherPrimarySpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ||

		( anotherPrimarySpecificationItem != null &&
		anotherPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ) ) &&

		( ( existingQuestionSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) ) == null ||

		( isUserGeneralizationWord &&
		generalizationWordItem.isOlderItem() &&
		// Skip adjusted question
		!existingQuestionSpecificationItem.isOlderItem() ) ) )
			{
			if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

			isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

			if( isPrimarySameAsSecondarySpecification )
				{
				if( primarySpecificationItem.hasSpecificationCompoundCollection() &&
				primarySpecificationItem.specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
					{
					if( ( collectionWordItem = collectionWordItemInCollectionWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem ) ) != null &&
					( anotherSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem ) ) == null &&
					( anotherSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == null )
						anotherSecondarySpecificationItem = ( !userSpecificationItem.isSpecificationWordSpanishAmbiguous() ? userSpecificationItem : primarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, collectionWordItem ) );
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
					anotherPrimarySpecificationItem != null &&
					anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() &&
					anotherPrimarySpecificationItem.isSelfGenerated() &&
					generalizationWordItem.isUserRelationWord &&
					( collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != null )
						{
						if( ( nonCompoundCollectionNr = collectionWordItem.nonCompoundCollectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
							nonCompoundCollectionNr = anotherPrimarySpecificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );

						if( nonCompoundCollectionNr > Constants.NO_COLLECTION_NR &&
						( tempSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, nonCompoundCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) != null )
							{
							hasFoundOldSpanishUserSpecification = true;
							secondarySpecificationItem = tempSpecificationItem;
							}
						}

					if( !hasFoundOldSpanishUserSpecification )
						{
						if( ( anotherSecondarySpecificationItem = secondarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, null ) ) == null )
							{
							if( secondarySpecificationItem.hasSpecificationCompoundCollection() &&
							secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
							( secondarySpecificationCommonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null &&
							secondarySpecificationCommonWordItem != primarySpecificationWordItem )
								anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem );
							}
						else
							{
							if( !anotherSecondarySpecificationItem.isExclusiveSpecification() &&
							( collectionWordItem = anotherPrimarySpecificationWordItem.collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != null )
								{
								if( ( tempSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, collectionWordItem ) ) == null )
									isPostponingNegativeConclusion = true;
								else
									secondarySpecificationItem = tempSpecificationItem;

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
						oppositePossessiveDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
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

				( isUserSpecificationSelected &&
				isUserGeneralizationWord &&

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
							negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
							negativeConclusionAnotherPrimarySpecificationItem = null;
							negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
							negativeConclusionAnotherSecondarySpecificationItem = null;
							}
						else
							{
							if( hasDisplayedMoreSpecificNonExclusiveSpecification )
								{
								negativeConclusionPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? anotherSecondarySpecificationItem : generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCommonWordItem ) );
								negativeConclusionAnotherPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? null : anotherSecondarySpecificationItem );
								negativeConclusionSecondarySpecificationItem = userSpecificationItem;
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
						}

					// Draw negative conclusion
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, primarySpecificationItem.isExclusiveSpecification(), false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem.isSelfGeneratedAssumption() ? Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
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
									if( createdJustificationItem.isConclusionJustification() )
										{
										if( foundSpecificationItem.markAsConcludedAssumption( false ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

										if( isArchivedAssignment )
											{
											if( ( negativeArchivedAssignmentItem = generalizationWordItem.firstAssignmentItem( false, false, true, true, false, true, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't find a past tense negative assumption assignment" );

											if( negativeArchivedAssignmentItem.isSelfGeneratedAssumption() &&
											// Also mark the past tense negative assumption assignment as concluded
											negativeArchivedAssignmentItem.markAsConcludedAssumption( false ) != Constants.RESULT_OK )
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
										negativeAssumptionJustificationItem.primarySpecificationItem() == negativeConclusionPrimarySpecificationItem &&
										generalizationWordItem.replaceJustification( negativeAssumptionJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
							if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created negative conclusion" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( relatedSpecificationItem != null &&
							relatedSpecificationItem.isOlderItem() &&
							generalizationWordItem.replaceOrDeleteSpecification( false, relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( isCompoundUserSpecification &&
							!isPrimarySameAsSecondarySpecification &&
							anotherSecondarySpecificationItem != null &&
							// Check for answered question justification
							createdSpecificationItem.answeredQuestionJustificationItem() == null &&
							generalizationWordItem.changeJustificationOfNegativeAssumptions( userSpecificationItem ) != Constants.RESULT_OK )
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
		JustificationItem conclusionJustificationItem;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem collectedSpecificationItem;
		SpecificationItem confirmedAllRelationWordsOfSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem justificationSecondarySpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem collectedWithItselfCommonWordItem;
		WordItem commonWordItem;
		WordItem currentGeneralizationWordItem;
		ContextResultType contextResult = new ContextResultType();
		CreateAndAssignResultType createAndAssignResult;
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

		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
		existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, !isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

		if( existingSpecificationItem == null ||
		// Allow existing conclusion specification to change its relation context
		existingSpecificationItem.isSelfGenerated() )
			{
			isSecondarySpecificationWordSpanishAmbiguous = secondarySpecificationWordItem.isNounWordSpanishAmbiguous();

			if( isPossessive &&
			isUserGeneralizationWord &&
			!isSecondarySpecificationWordSpanishAmbiguous &&

			( existingSpecificationItem == null ||
			CommonVariables.nUserRelationWords > 1 ) &&

			( foundRelationContextNr = generalizationWordItem.contextNr( specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
				{
				if( ( nContextRelations = adminItem_.nContextWordsInContextWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
					{
					if( ( contextResult = adminItem_.getGeneralizationContext( isArchivedAssignment, false, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the generalization context number" );

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
				secondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Skip if possessive user specification exists
			if( secondarySpecificationItem != null &&
			secondarySpecificationItem.hasNewInformation() &&

			// User specification doesn't exist
			( ( existingUserSpecificationItem == null &&

			( secondarySpecificationItem.isUserSpecification() ||
			relationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, secondarySpecificationItem ) == null ) ) ||

			// User specification exists
			( existingUserSpecificationItem != null &&

			( ( !isSecondarySpecificationWordSpanishAmbiguous &&

			( isPossessive ||
			existingUserSpecificationItem.isAssignment() ) ) ||

			// Typically for the Spanish language
			( isSecondarySpecificationWordSpanishAmbiguous &&

			( !existingUserSpecificationItem.hasRelationContext() ||
			generalizationWordItem.hasDisplayedMoreSpecificSpecification() ) ) ) ) ) )
				{
				hasFeminineProperNameEnding = ( isPossessive &&
												isSecondarySpecificationWordSpanishAmbiguous &&
												relationWordItem.hasFeminineProperNameEnding() );

				isConclusionAssignment = ( ( isArchivedAssignment &&
											isAssumption ) ||

											( !isAssumption &&

											( isArchivedAssignment ||
											secondarySpecificationItem.isExclusiveNonPossessiveGeneralizationAssignment() ) ) );

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

				// This condition is typical for the Spanish language
				if( isSecondarySpecificationWordSpanishAmbiguous &&
				secondarySpecificationWordItem == specificationWordItem &&

				( !isPossessive ||
				!relationWordItem.isMale() ) )
					spanishAmbiguousSpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

				// Draw a possessive reversible conclusion
				// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
				if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasFeminineProperNameEnding, isAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, null, null, secondarySpecificationItem, null, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

				if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
					foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

				if( confirmedAllRelationWordsOfSpecificationItem == null )
					{
					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						{
						if( existingSpecificationItem != null &&
						foundSpecificationItem != null )
							{
							if( existingSpecificationItem.isConcludedAssumption() )
								isReplacingObsoleteJustifications = true;
							else
								{
								if( !isSecondarySpecificationWordSpanishAmbiguous &&
								!existingSpecificationItem.isReplacedItem() &&
								existingSpecificationItem.isSelfGeneratedAssumption() &&
								foundSpecificationItem.isSelfGeneratedConclusion() &&
								foundSpecificationItem.hasSpecificationCompoundCollection() )
									{
									if( relationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption specification by a conclusion in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

									isReplacingObsoleteJustifications = true;
									}
								}

							if( isReplacingObsoleteJustifications &&
							// Clean-up obsolete justifications
							relationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						{
						if( !isPossessive &&
						foundSpecificationItem == null &&
						secondarySpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) == null &&
						( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, specificationWordItem, null ) ) != null &&
						( conclusionJustificationItem = conclusionSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null &&
						( justificationSecondarySpecificationItem = conclusionJustificationItem.secondarySpecificationItem() ) != null )
							{
							// Additional justification
							if( ( justificationResult = generalizationWordItem.addJustification( false, false, conclusionSpecificationItem.hasSpecificationCollection(), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, justificationSecondarySpecificationItem, null, secondarySpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
								return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

							if( secondarySpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the assumption specification" );
							}
						}

					if( isPossessive &&
					( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) != null )
						{
						if( !createdOrFoundSpecificationItem.hasSpecificationCompoundCollection() &&
						makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, null, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( isSecondarySpecificationWordSpanishAmbiguous &&
						( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem.collectedWithItselfCommonWordItem() ) != null )
							{
							if( !isArchivedAssignment &&
							// Reversed specification doesn't already exist
							generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, true, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == null &&

							// Generate the reversed specification
							drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem.relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

							if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem.firstNounSpecificationGeneralizationItem() ) != null )
								{
								do	{
									if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

									if( ( partOfSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != null &&
									partOfSpecificationItem.isPartOf() &&

									( !createdOrFoundSpecificationItem.hasAssumptionLevel() ||
									// Skip if 'part of' conclusion exists
									relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentGeneralizationWordItem, null ) == null ) )
										{
										// Draw a 'part of' generalization conclusion
										// Not yet explained in the design
										if( adminItem_.addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, partOfSpecificationItem, null, relationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
										}
									}
								while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
								}

							if( relationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) == null &&
							( secondarySpecificationItem = collectedWithItselfCommonWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, createdOrFoundSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&

							drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, null, createdOrFoundSpecificationItem, null, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}
						}

					if( isUserGeneralizationWord &&
					specificationWordItem.isUserSpecificationWord )
						{
						// Typically for the Spanish language
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
										if( generalizationWordItem.addContext( false, relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem.relationContextNr(), specificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

										// Not hidden anymore after addition of context
										if( !existingSpecificationItem.isHiddenSpanishSpecification() &&
										relationWordItem.writeUpdatedSpecification( true, false, false, false, false, existingSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to write the hidden assumption in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									{
									if( secondarySpecificationItem != null )
										{
										// Additional justification
										if( ( justificationResult = relationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

										if( existingSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the existing specification" );
										}
									}
								}
							}
						else
							{
							if( foundSpecificationItem == null )
								{
								if( isPossessive &&
								createdSpecificationItem != null &&
								( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, null ) ) != null )
									{
									// Additional justification (assumption)
									if( ( justificationResult = relationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

									if( assumptionSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the assumption specification" );

									if( generalizationWordItem.addContext( false, relationWordTypeNr, specificationWordTypeNr, assumptionSpecificationItem.relationContextNr(), specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									collectedSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, assumptionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null );

									if( collectedSpecificationItem != null &&
									( firstJustificationItem = collectedSpecificationItem.firstJustificationItem() ) != null )
										{
										// Additional justification (another)
										if( ( justificationResult = relationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, firstJustificationItem.primarySpecificationItem(), null, collectedSpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

										if( assumptionSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification (again) of the assumption specification" );

										// Additional justification (collected)
										if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, firstJustificationItem.primarySpecificationItem(), null, createdSpecificationItem, null, collectedSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

										if( collectedSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the collected specification" );
										}
									}
								}
							else
								{
								// Typically for the Spanish language
								if( secondarySpecificationItem != null &&
								secondarySpecificationItem.hasSpecificationCompoundCollection() &&
								foundSpecificationItem.isSelfGeneratedAssumption() &&
								( commonWordItem = specificationWordItem.commonWordItem( foundSpecificationItem.specificationCollectionNr() ) ) != null &&
								( assumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, true, Constants.NO_QUESTION_PARAMETER, commonWordItem, generalizationWordItem ) ) != null &&
								assumptionSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) != null )
									{
									// Additional justification (Spanish)
									if( ( justificationResult = relationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

									if( assumptionSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the assumption specification" );
									}
								}
							}

						if( makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					if( existingSpecificationItem != null )
						{
						confirmedAllRelationWordsOfSpecificationItem = confirmedAllRelationWordsOfSpecificationItem.updatedSpecificationItem();

						if( relationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, confirmedAllRelationWordsOfSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

						// Clean-up obsolete justifications
						if( relationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
		if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationItem.isExclusiveSpecification(), false, secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPartOf(), secondarySpecificationItem.isPossessive(), false, false, true, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, secondarySpecificationItem.specificationWordItem(), secondarySpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );

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

		// Skip if user specification already exists
		if( generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null &&

		// Skip if justification with primary specification already exists
		( generalizationWordItem.primarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem ) == null ||
		// Skip if justification with secondary specification already exists
		generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, secondarySpecificationItem ) == null ) )
			{
			// Make a generalization assumption
			// Not yet explained in the design
			if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME ? false : primarySpecificationItem.isEveryGeneralization() ), false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			if( hasCorrectedAssumptionByKnowledge &&
			createAndAssignResult.createdJustificationItem != null &&
			( olderGeneralizationAssumptionBySpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
			( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != null &&

			generalizationWordItem.replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, createAndAssignResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		short questionParameter = ( isGeneralizationProperName ? Constants.WORD_PARAMETER_SINGULAR_VERB_IS : Constants.NO_QUESTION_PARAMETER );
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
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				!currentGeneralizationWordItem.hasCollection() &&
				!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
					{
					// Get secondary specification for justification
					if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
						{
						if( isGeneralizationProperName &&
						!primarySpecificationItem.isSelfGeneratedAssumption() &&
						!secondarySpecificationItem.isOlderItem() &&

						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						// Get secondary question specification for justification
						if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, questionParameter, specificationWordItem ) ) != null &&
						drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( !CommonVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasAnotherPrimarySpecificationCompoundCollection;
		boolean hasSameSpecificationCollection;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveReversibleAssumption;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isRelationWordUserWordGeneralization;
		boolean hasFeminineOrMasculineProperNameEnding = false;
		short assumptionJustificationTypeNr;
		JustificationItem createdJustificationItem;
		JustificationItem foundJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem selfGeneratedConclusionSpecificationItem = null;
		WordItem possessiveSpecificationWordItem;
		WordItem secondaryGeneralizationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem feminineSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;

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
		!possessiveDefinitionSpecificationItem_.isDeletedItem() &&
		( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
			{
			isPossessiveSpecificationWordSpanishAmbiguous = possessiveSpecificationWordItem.isNounWordSpanishAmbiguous();

			if( isPossessiveSpecificationWordSpanishAmbiguous ||
			!secondarySpecificationItem.hasSpecificationCompoundCollection() )
				{
				isRelationWordUserWordGeneralization = relationWordItem.isUserGeneralizationWord;

				existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );
				existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );

				// Skip if possessive user specification exists
				if( ( existingUserSpecificationItem == null &&
				!possessiveSpecificationWordItem.isUserSpecificationWord &&

				( !isPossessiveSpecificationWordSpanishAmbiguous ||

				// Typically for the Spanish language
				( isRelationWordUserWordGeneralization &&
				existingSpecificationItem == null ) ) &&

				// Skip if possessive specification exists
				relationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null ) ||

				// Typically for the Spanish language
				( isPossessiveSpecificationWordSpanishAmbiguous &&
				alternativeSecondarySpecificationItem != null &&
				!alternativeSecondarySpecificationItem.isOlderItem() &&
				alternativeSecondarySpecificationItem.isActiveAssignment() &&

				// Skip if possessive user specification exists
				( existingUserSpecificationItem == null ||

				( existingUserSpecificationItem.isOlderItem() &&
				existingUserSpecificationItem.hasSpecificationCompoundCollection() ) ) ) )
					{
					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == possessiveSpecificationWordItem );

					// Make a possessive conditional specification assumption (normal)
					// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, false, false, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : null ), null, ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), null, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						{
						createdOrFoundSpecificationItem = createAndAssignResult.foundSpecificationItem;

						if( isPossessive &&
						!isRelationWordUserWordGeneralization &&
						existingSpecificationItem != null &&
						existingSpecificationItem.isSelfGeneratedAssumption() &&
						( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != null &&
						( foundJustificationItem = existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null )
							{
							secondaryGeneralizationWordItem = foundJustificationItem.secondaryGeneralizationWordItem();

							if( secondaryGeneralizationWordItem != null &&
							secondaryGeneralizationWordItem != generalizationWordItem &&
							( foundJustificationItem = relationWordItem.possessiveReversibleAssumptionJustificationItem( secondaryGeneralizationWordItem, secondarySpecificationWordItem ) ) != null )
								{
								// Additional justification
								if( ( justificationResult = relationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, possessiveDefinitionSpecificationItem_, null, foundJustificationItem.secondarySpecificationItem(), null, existingSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

								if( existingSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
								}
							}
						}
					else
						{
						createdOrFoundSpecificationItem = createdSpecificationItem;

						if( !isPossessive &&
						existingSpecificationItem != null )
							{
							existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

							if( !createdSpecificationItem.isHiddenSpanishSpecification() &&
							createdSpecificationItem.assumptionLevel() < existingSpecificationItem.assumptionLevel() &&
							relationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}
						}

					if( !isPossessive &&
					createdOrFoundSpecificationItem != null &&

					( createdSpecificationItem != null ||

					( secondarySpecificationItem.secondarySpecificationJustificationItem( createdOrFoundSpecificationItem ) == null &&
					!secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) &&

					!generalizationWordItem.hasCorrectedAssumptionByKnowledge() &&
					// Typically for the Spanish language
					!secondarySpecificationItem.isActiveAssignment() )
						{
						// Additional justification
						if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, createdOrFoundSpecificationItem, null, secondarySpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

						secondarySpecificationItem = secondarySpecificationItem.updatedSpecificationItem();

						if( secondarySpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the secondary specification" );
						}
					}

				if( !CommonVariables.hasDisplayedWarning )
					{
					if( oppositePossessiveSpecificationWordItem_ == null )
						return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );

					if( alternativeSecondarySpecificationItem != null &&
					// Try to find a self-generated conclusion first
					( selfGeneratedConclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveSpecificationWordItem_, null ) ) == null &&
					// If not found, try to find a self-generated assumption
					( assumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != null )
						{
						hasSameSpecificationCollection = ( assumptionSpecificationItem.specificationCollectionNr() == alternativeSecondarySpecificationItem.specificationCollectionNr() );

						if( !assumptionSpecificationItem.hasCurrentCreationSentenceNr() &&
						!assumptionSpecificationItem.hasNewInformation() &&

						( generalizationWordItem.hasConfirmedSpecification() == hasSameSpecificationCollection ||
						alternativeSecondarySpecificationItem.isPossessive() ) )
							selfGeneratedConclusionSpecificationItem = assumptionSpecificationItem;
						}

					isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_.isNounWordSpanishAmbiguous();

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
						feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem();
					else
						feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem.hasFeminineProperNameEnding() : relationWordItem.hasFeminineProperNameEnding() );

					if( feminineSpecificationWordItem != null )
						{
						if( isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
							anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, feminineSpecificationWordItem );
						else
							feminineSpecificationWordItem = null;
						}

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

					( alternativeSecondarySpecificationItem == null ||
					alternativeSecondarySpecificationItem.isUserSpecification() ) &&

					isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
						hasFeminineOrMasculineProperNameEnding = true;

					hasAnotherPrimarySpecificationCompoundCollection = ( alternativeSecondarySpecificationItem != null &&
																		alternativeSecondarySpecificationItem.hasSpecificationCompoundCollection() );
					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION :
													( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
													!hasAnotherPrimarySpecificationCompoundCollection &&
													!hasFeminineOrMasculineProperNameEnding &&
													isRelationWordUserWordGeneralization ) ? Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( selfGeneratedConclusionSpecificationItem == null ||
					!generalizationWordItem.isOlderItem() ||

					( isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem.isSelfGenerated() ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!selfGeneratedConclusionSpecificationItem.hasRelationContext() ) ) &&

					( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!secondarySpecificationItem.isConcludedAssumption() &&

					( relationWordItem.secondarySpecificationJustificationItem( true, assumptionJustificationTypeNr, secondarySpecificationItem ) == null ||

					( feminineSpecificationWordItem != null &&

					( hasAnotherPrimarySpecificationCompoundCollection ||
					isPossessiveReversibleAssumption ) &&

					// Skip if feminine specification exists as user specification
					relationWordItem.firstUserSpecificationItem( false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, feminineSpecificationWordItem ) == null ) ) ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != null &&
					// Skip if already exists as self-generated assumption
					relationWordItem.primarySpecificationJustificationItem( true, assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == null ) ) )
						{
						// Make a possessive conditional specification assumption (opposite)
						// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, secondarySpecificationItem.assumptionLevel(), assumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? null : ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : possessiveDefinitionSpecificationItem_ ) ), null, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( hasFeminineOrMasculineProperNameEnding && !isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem ), null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
							{
							foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

							if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							( assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, !isPossessive, true, oppositePossessiveSpecificationWordItem_, relationWordItem ) ) != null )
								{
								// Additional justifications (Generalization word)
								if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, foundSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null &&
								generalizationWordItem.attachJustificationInWord( createdJustificationItem, assumptionSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );

								// Additional justification (Relation word)
								if( ( justificationResult = relationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, assumptionSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + relationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null &&
								foundSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the found specification" );
								}
							else
								{
								if( isPossessiveSpecificationWordSpanishAmbiguous &&
								// Clean-up obsolete justifications
								relationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}
							}

						if( alternativeSecondarySpecificationItem != null &&
						feminineSpecificationWordItem != null )
							{
							if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isPossessive, Constants.NO_QUESTION_PARAMETER, feminineSpecificationWordItem, generalizationWordItem ) ) == null ||

							( existingSpecificationItem.isSelfGeneratedAssumption() &&

							( !secondarySpecificationItem.isHiddenSpanishSpecification() ||

							( !relationWordItem.hasConfirmedSpecification() &&
							alternativeSecondarySpecificationItem.hasSpecificationCollection() ) ) ) )
								{
								// Make an opposite possessive feminine proper name ending assumption
								// Not yet explained in the design
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( existingSpecificationItem == null ? Constants.NO_CONTEXT_NR : existingSpecificationItem.relationContextNr() ), alternativeSecondarySpecificationItem, ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != null ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

								if( !isPossessive )
									{
									feminineSpecificationWordItem_ = feminineSpecificationWordItem;

									if( !isArchivedAssignment &&
									( createdOrFoundSpecificationItem = ( createAndAssignResult.createdSpecificationItem == null ? createAndAssignResult.foundSpecificationItem : createAndAssignResult.createdSpecificationItem ) ) != null &&

									( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
									!createdOrFoundSpecificationItem.isReplacedItem() ) &&

									// Generate the reversed specification
									drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, createdOrFoundSpecificationItem.specificationContextNr(), createdOrFoundSpecificationItem.relationContextNr(), relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
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
		boolean hasGeneralizationWordConfirmedSpecification;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean isGeneralizationProperName;
		boolean isGeneralizationWordSpanishAmbiguous;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem createdJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primaryPartOfSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem collectionWordItem;
		WordItem currentGeneralizationWordItem;
		WordItem feminineCollectionWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

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
			hasGeneralizationWordConfirmedSpecification = generalizationWordItem.hasConfirmedSpecification();
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
			isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
			isGeneralizationWordSpanishAmbiguous = generalizationWordItem.isNounWordSpanishAmbiguous();
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem();

			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&

				( currentGeneralizationWordItem.hasNonPossessiveAndNonQuestionPartOfSpecification() ||

				( ( !currentGeneralizationWordItem.hasCollection() ||

				( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
				!hasCorrectedAssumptionByOppositeQuestion ) ) &&

				!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() ) ) )
					{
					existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

					// Skip if concluded conclusion exists
					if( ( existingSpecificationItem == null ||
					existingSpecificationItem != simpleUserSpecificationItem ) &&

					// Get primary specification for justification
					( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
						{
						alternativeAnotherPrimarySpecificationItem_ = null;
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
							hasGeneralizationWordFeminineProperNameEnding &&
							( feminineCollectionWordItem = specificationWordItem.feminineCollectionWordItem() ) != null )
								secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, feminineCollectionWordItem );

							if( secondarySpecificationItem == null )
								{
								// Try to find self-generated conclusion first
								if( ( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null )
									// Otherwise, try to find self-generated assumption
									tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );

								if( tempSpecificationItem != null &&

								( existingSpecificationItem == null ||
								tempSpecificationItem.isSelfGeneratedConclusion() ||
								// Skip assumption with relation word
								existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == null ) )
									secondarySpecificationItem = tempSpecificationItem;
								}

							if( secondarySpecificationItem == null )
								{
								if( hasGeneralizationWordFeminineProperNameEnding &&
								!isUserGeneralizationWord &&
								simpleUserSpecificationItem != null &&
								simpleUserSpecificationItem.isSpecificationWordSpanishAmbiguous() &&
								simpleUserSpecificationItem.isPossessive() &&
								( userSpecificationWordItem = simpleUserSpecificationItem.specificationWordItem() ) != null &&
								( tempSpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null )
									{
									primarySpecificationItem = simpleUserSpecificationItem;
									secondarySpecificationItem = tempSpecificationItem;
									}
								}
							else
								{
								// Typically for the Spanish language
								if( isSpecificationWordSpanishAmbiguous &&
								secondarySpecificationItem.isHiddenSpanishSpecification() &&
								( secondarySpecificationItem = alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, false, true, false, true, simpleUserSpecificationItem, generalizationWordItem, specificationWordItem ) ) != null )
									{
									if( secondarySpecificationItem.isQuestion() )
										spanishQuestionPrimarySpecificationItem_ = secondarySpecificationItem;
									else
										{
										if( !secondarySpecificationItem.hasAssumptionLevel() &&
										secondarySpecificationItem.isSelfGeneratedAssumption() )
											secondarySpecificationItem.incrementAssumptionLevel();
										}
									}
								}
							}

						if( secondarySpecificationItem != null &&

						( !isSpecificationWordSpanishAmbiguous ||
						existingSpecificationItem == null ||
						!secondarySpecificationItem.isHiddenSpanishSpecification() ) )
							{
							if( primarySpecificationItem.isSpecificationGeneralization() )
								{
								if( isGeneralizationProperName &&
								primarySpecificationItem.isPartOf() &&
								// Skip creating an assumption, if a conclusion exists
								generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem, null ) == null )
									{
									// Make a substitution 'part of' assumption
									// Not yet explained in the design
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, alternativeAnotherPrimarySpecificationItem_, primarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != null &&
									( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, primarySpecificationItem ) ) != null )
										{
										primaryPartOfSpecificationItem = specificationSubstitutionPartOfAssumptionJustificationItem.primarySpecificationItem();

										if( hasGeneralizationWordConfirmedSpecification ||
										specificationSubstitutionPartOfAssumptionJustificationItem.hasPrimaryAnsweredQuestion() ||

										( primaryPartOfSpecificationItem != null &&
										primaryPartOfSpecificationItem != secondarySpecificationItem &&
										secondarySpecificationItem.assumptionLevel() < primaryPartOfSpecificationItem.assumptionLevel() ) )
											{
											if( generalizationWordItem.updateJustificationInSpecifications( false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( !specificationSubstitutionPartOfAssumptionJustificationItem.isReplacedOrDeletedItem() &&
											!generalizationWordItem.isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) &&

											generalizationWordItem.replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to delete the specification substitution 'part of' assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								}
							else
								{
								if( primarySpecificationItem.isExclusiveSpecification() &&

								( !isGeneralizationWordSpanishAmbiguous ||
								secondarySpecificationItem.isExclusiveSpecification() ) &&

								( !primarySpecificationItem.hasSpecificationCompoundCollection() ||

								// Opposite specification doesn't exist
								( ( collectionWordItem = specificationWordItem.collectionWordItem( primarySpecificationItem.specificationCollectionNr() ) ) != null &&
								generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, collectionWordItem ) == null ) ) )
									{
									// Make generalization assumption
									if( makeGeneralizationAssumption( hasCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

									generalizationAssumptionWordItem_ = currentGeneralizationWordItem;
									}
								}
							}
						}
					}
				}
			while( !CommonVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		// Definition specification
		if( generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NAME &&
		generalizationAssumptionWordItem_ != null &&
		generalizationWordItem != generalizationAssumptionWordItem_ &&
		specificationWordItem != generalizationAssumptionWordItem_ &&
		( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
		( secondarySpecificationItem = specificationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != null &&
		// User specification already exists
		generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) == null &&

		// Make an exclusive specification substitution assumption
		// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
		adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, primarySpecificationItem.isEveryGeneralization(), false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, generalizationAssumptionWordItem_, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		return Constants.RESULT_OK;
		}

	private SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNameEnding, boolean isCheckingForPossessiveSpecification, boolean isCheckingForQuestion, boolean isNegative, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
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
					alternativeAnotherPrimarySpecificationItem_ = oppositePossessiveDefinitionSpecificationItem_;
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

	private static WordItem collectionWordItemInCollectionWords( int compoundCollectionNr, WordItem commonWordItem )
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

	private FindSpecificationResultType findPossessiveSpecifications( WordItem generalizationWordItem, WordItem possessiveNounWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem foundPossessiveDefinitionSpecificationItem = null;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem possessiveSpecificationWordItem;
		WordItem previousPossessiveSpecificationWordItem;
		WordItem higherLevelPossessiveSpecificationWordItem = null;
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
				possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
				possessiveSpecificationWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) != null )
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
					if( !possessiveDefinitionSpecificationItem.isPartOf() &&
					( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( possessiveSpecificationWordItem == generalizationWordItem )
							return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );

						if( previousPossessiveSpecificationItem != null &&
						( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem.specificationWordItem() ) != null )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem.specificationCollectionNr();

							if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
							higherLevelPossessiveSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) &&
							previousPossessiveSpecificationItem.specificationCollectionNr() == possessiveSpecificationCollectionNr )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
							}

						previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
						}
					}
				while( foundPossessiveDefinitionSpecificationItem == null &&
				foundOppositePossessiveDefinitionSpecificationItem == null &&
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

		alternativeAnotherPrimarySpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;
		oppositePossessiveDefinitionSpecificationItem_ = null;
		postponeNegativeConclusionPrimarySpecificationItem_ = null;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;
		spanishQuestionPrimarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		generalizationAssumptionWordItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReasoningOldVariables()
		{
		clearPostponedNegativeConclusionVariables();

		hasSuggestiveQuestionAssumption_ = false;

		spanishAmbiguousCompoundPrimarySpecificationItem_ = null;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = null;
		spanishQuestionPrimarySpecificationItem_ = null;

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

		if( ( answeredQuestionSpecificationItem = generalizationWordItem.firstRecentlyAnsweredQuestionSpecificationItem( Constants.WORD_PARAMETER_SINGULAR_VERB_IS ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any answered question specification" );

		do	{
			if( userSpecificationWordItem.hasCollectionNr( answeredQuestionSpecificationItem.specificationCollectionNr() ) )
				{
				if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no specification word" );

				// Check if already exists as positive specification
				if( hasDisplayedMoreSpecificNonExclusiveSpecification ||
				generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == null )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem.firstJustificationItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem.hasPrimaryQuestion() &&
						!currentJustificationItem.hasPossessivePrimarySpecification() &&

						// Draw negative conclusion from answered question
						drawNegativeConclusion( answeredQuestionSpecificationItem.isArchivedAssignment(), false, answeredQuestionSpecificationItem.generalizationWordTypeNr(), answeredQuestionSpecificationItem.specificationWordTypeNr(), answeredQuestionSpecificationItem.generalizationContextNr(), answeredQuestionSpecificationItem.specificationContextNr(), currentJustificationItem.primarySpecificationItem(), currentJustificationItem.secondarySpecificationItem(), userSpecificationItem, null, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
			foundOnlyOptionLeftSpecificationItem != null )
				{
				// Draw an only option left conclusion
				// Not yet explained in the design
				if( adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
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
			( collectionWordItem = specificationWordItem.collectionWordItem( nonCompoundCollectionNr ) ) != null )
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
				anotherPrimarySpecificationItem.hasSpecificationCollection() &&

				askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
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
					return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
					{
					if( !currentSpecificationItem.hasSpecificationCollection() &&
					!currentSpecificationItem.isExclusiveSpecification() )
						{
						assumptionLevel = ( secondarySpecificationItem.assumptionLevel() < currentSpecificationItem.assumptionLevel() ? currentSpecificationItem.assumptionLevel() : secondarySpecificationItem.assumptionLevel() );

						// Collect generalization words of a specification-generalization definition
						if( adminItem_.collectGeneralizationWordWithPreviousOne( false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

						if( currentSpecificationItem.hasCurrentCreationSentenceNr() &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );

						if( !isSelfGenerated &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != Constants.RESULT_OK )
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
		boolean hasCorrectedAssumptionByKnowledge;
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
		boolean hasUserSpecificationRelationContext = false;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		boolean isExclusiveUserSpecification = false;
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		boolean isIncludingAdjectives = false;
		boolean isNegativeConclusion = false;
		boolean isPossessiveUserSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short currentSpecificationWordTypeNr;
		int currentSpecificationCollectionNr;
		int relationContextNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem alternativeAnotherPrimarySpecificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem archivedAssignmentItem;
		SpecificationItem bestMatchingRelationSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem collectedWithItselfSpecificationItem = null;
		WordItem currentSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( specificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
		!adminItem_.isNounWordType( specificationWordTypeNr ) )
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
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			isIncludingAdjectives = ( !isExclusiveSpecification &&
									!isQuestion &&
									!hasCorrectedAssumptionByKnowledge &&
									!userSpecificationItem.isQuestion() );
			}

		if( ( currentSpecificationItem = specificationWordItem.firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != null )
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

			if( isSpecificationWordSpanishAmbiguous )
				collectedWithItselfSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem )
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
						if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.relationContextNr(), currentSpecificationWordItem ) != Constants.RESULT_OK )
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

							if( ( isIncludingRelationWord ||

							( !hasRelationWord &&
							!isPossessiveUserSpecification ) ||

							( isPossessiveUserSpecification &&

							( !isUserGeneralizationWord ||
							foundPrimarySpecificationItem.isOlderItem() ) ) ) &&

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

									( !hasUserSpecificationRelationContext ||
									foundPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ) ) ||

									( ( hasRelationWord ||
									generalizationWordItem.isOlderItem() ) &&

									oppositePossessiveDefinitionSpecificationItem_ != null &&
									oppositePossessiveDefinitionSpecificationItem_.specificationCollectionNr() == userSpecificationCollectionNr ) )
										{
										// Make a feminine or masculine proper name ending assumption
										// Not yet explained in the design
										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem_ : null ), currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : null ) ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

										if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
											{
											if( !feminineOrMasculineSpecificationItem.hasSpecificationCompoundCollection() &&
											// Clean-up obsolete justifications
											generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( !isUserSpecificationWordSpanishAmbiguous &&

											( postponeNegativeConclusionSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
											postponeNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr ) )
												{
												if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
													{
													firstJustificationItem = foundSpecificationItem.firstJustificationItem();

													if( firstJustificationItem != null &&
													( secondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
														{
														// Additional justification
														if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, secondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
															return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

														if( createdSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
														}
													}

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
								( relationContextNr = bestMatchingRelationSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&

								drawPossessiveReversibleConclusion( false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
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
								hasSuggestiveQuestionAssumption_ ) )
									{
									if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
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
										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, ( archivedAssignmentItem == null ? currentSpecificationItem : archivedAssignmentItem ), null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

										if( createAndAssignResult.createdSpecificationItem == null &&

										( createAndAssignResult.createdJustificationItem != null ||
										!primarySpecificationItem.hasRelationContext() ||
										foundPrimarySpecificationItem.isOlderItem() ) &&

										// Clean-up obsolete justifications
										generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
										primarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

										if( primarySpecificationItem != null &&
										primarySpecificationItem.isHiddenSpanishSpecification() &&
										( primarySpecificationItem = alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != null )
											alternativeAnotherPrimarySpecificationItem = alternativeAnotherPrimarySpecificationItem_;

										if( primarySpecificationItem == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );

										if( !primarySpecificationItem.isPartOf() &&

										( isGeneralizationProperName ||

										( primarySpecificationItem.isUserSpecification() &&

										( !isExclusiveSpecification ||
										currentSpecificationWordItem.isFemaleOrMale() ||
										!currentSpecificationItem.hasSpecificationCompoundCollection() ||

										// Typically for the Spanish language
										( isSpanishCurrentLanguage &&

										( generalizationWordItem.isFemaleOrMale() ||
										currentSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, generalizationWordItem ) != null ) ) ) ) ) )
											{
											// Draw a compound specification substitution conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem.isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, ( currentSpecificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? Constants.WORD_TYPE_NOUN_SINGULAR : currentSpecificationWordTypeNr ), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
												{
												if( hasCorrectedAssumptionByKnowledge )
													{
													if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created specification" );

													if( relatedResult.relatedSpecificationItem != null &&
													// Replace incorrect assumption
													generalizationWordItem.replaceOrDeleteSpecification( false, relatedResult.relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( isGeneralizationProperName )
														{
														if( isSpanishCurrentLanguage &&
														!hasRelationWord &&
														hasCurrentSpecificationCollection &&
														!isExclusiveSpecification &&
														!isSpecificationWordSpanishAmbiguous &&
														isUserGeneralizationWord &&
														primarySpecificationItem.hasSpecificationNonCompoundCollection() &&

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
							isGeneralizationProperName &&
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

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFeminineOrMasculineProperNameEnding;
		boolean hasGeneralizationWordConfirmedSpecification;
		boolean isGeneralizationWordUserRelationWord;
		boolean isHiddenPrimarySpecification;
		boolean isHiddenSecondarySpecification;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isReplaceHiddenSpecification;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification = false;
		boolean hasRelationWordFeminineOrMasculineProperNameEnding = false;
		int compoundCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem olderJustificationItem;
		SpecificationItem additionalSecondarySpecificationItem;
		SpecificationItem additionalSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem compoundSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem;
		SpecificationItem foundPossessiveDefinitionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem possessiveSpecificationItem = null;
		SpecificationItem secondarySpecificationItem = null;
		WordItem commonWordItem;
		WordItem currentPossessiveNounWordItem;
		WordItem feminineSpecificationWordItem;
		WordItem possessiveSpecificationWordItem;
		WordItem spanishQuestionGeneralizationWordItem;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;
		FindSpecificationResultType findSpecificationResult;
		ShortResultType shortResult;

		possessiveDefinitionSpecificationItem_ = null;
		oppositePossessiveDefinitionSpecificationItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

		if( primarySpecificationItem != null &&

		( hasRelationWord ||

		( !primarySpecificationItem.isOlderItem() &&

		( !isPossessive ||
		!primarySpecificationItem.isExclusiveSpecification() ) ) ) &&

		( currentPossessiveNounWordItem = CommonVariables.firstPossessiveNounWordItem ) != null )
			{
			hasGeneralizationWordConfirmedSpecification = generalizationWordItem.hasConfirmedSpecification();
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
			isHiddenPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();

			if( relationWordItem != null )
				{
				hasRelationWordConfirmedSpecification = relationWordItem.hasConfirmedSpecification();
				hasRelationWordFeminineOrMasculineProperNameEnding = relationWordItem.hasFeminineOrMasculineProperNameEnding();
				}

			// Do for all possessive noun words
			do	{
				if( currentPossessiveNounWordItem != specificationWordItem )
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

						if( ( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
							{
							isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

							if( primarySpecificationItem != null &&

							( ( !primarySpecificationItem.hasRelationContext() &&
							generalizationWordItem.hasAnsweredSelfGeneratedQuestion() ) ||

							( isPossessiveSpecificationWordSpanishAmbiguous &&
							primarySpecificationItem.specificationWordItem() == possessiveDefinitionSpecificationWordItem ) ) )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
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
								!existingUserSpecificationItem.isOlderItem() ||
								// Confirmed only a few of the entered relation words
								adminItem_.nContextWordsInContextWords( existingUserSpecificationItem.relationContextNr(), possessiveDefinitionSpecificationWordItem ) < CommonVariables.nUserRelationWords ) ) )
									{
									hasFeminineOrMasculineProperNameEnding = false;

									if( isPossessiveSpecificationWordSpanishAmbiguous &&
									isFeminineOrMasculineProperNameEnding( isPossessive, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) )
										hasFeminineOrMasculineProperNameEnding = true;

									if( !hasRelationWord &&
									!isNegative &&
									( possessiveSpecificationWordItem = possessiveSpecificationItem.specificationWordItem() ) != null )
										{
										commonWordItem = possessiveSpecificationWordItem.commonWordItem( possessiveSpecificationItem.specificationCollectionNr() );

										if( commonWordItem != null &&
										commonWordItem != generalizationWordItem &&
										commonWordItem.isFemaleOrMale() &&
										!commonWordItem.isNounWordSpanishAmbiguous() &&
										// Specification doesn't exist yet
										generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) == null &&
										( definitionSpecificationItem = commonWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, possessiveSpecificationWordItem ) ) != null )
											{
											// Make generalization assumption
											if( makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + commonWordItem.anyWordTypeString() + "\"" );
											}
										}

									// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), foundPossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
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
											( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != null )
												{
												if( foundSpecificationItem.isSelfGeneratedAssumption() &&
												( obsoleteJustificationItem = generalizationWordItem.olderComplexJustificationItem( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_COLLECTION_NR, null ) ) != null )
													{
													if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the obsolete justification item by the created justification item" );

													if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) == null )
														return adminItem_.startError( 1, moduleNameString_, "The found specification item has no first justification item" );

													// Attach all justification items of the found specification to the created specification
													if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the found specification" );

													if( generalizationWordItem.replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( foundSpecificationItem.isSelfGeneratedConclusion() &&
													( firstJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null )
														{
														if( ( additionalSecondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
															{
															// Additional justification
															if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, additionalSecondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

															if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
																return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

															if( createdSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
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
							}
						}

					// First opposite possessive specification
					if( !CommonVariables.hasDisplayedWarning &&
					possessiveDefinitionSpecificationWordItem != null &&
					foundOppositePossessiveDefinitionSpecificationItem != null &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( isNegative ||
						specificationContextNr > Constants.NO_CONTEXT_NR ||
						createdOrFoundSpecificationItem == null )
							{
							secondarySpecificationItem = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
						primarySpecificationItem != null &&
						secondarySpecificationItem != null &&

						( !primarySpecificationItem.isOlderItem() ||
						primarySpecificationItem.isSelfGenerated() ) )
							{
							isHiddenSecondarySpecification = secondarySpecificationItem.isHiddenSpanishSpecification();
							existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

							if( ( existingSpecificationItem == null ||
							!existingSpecificationItem.hasRelationContext() ||

							( ( hasRelationWord &&
							isGeneralizationWordUserRelationWord &&
							isPossessive &&
							existingSpecificationItem.isSelfGeneratedAssumption() &&

							( hasRelationWordConfirmedSpecification ||
							!hasRelationWordFeminineOrMasculineProperNameEnding ||
							anotherPrimarySpecificationItem != null ) ) ||

							( !isHiddenPrimarySpecification &&
							secondarySpecificationItem.hasRelationContext() &&
							!secondarySpecificationItem.isSelfGeneratedConclusion() &&

							( !isHiddenSecondarySpecification ||
							// Typically for the Spanish language
							primarySpecificationItem.isAssignment() ||
							primarySpecificationItem.isUserSpecification() ) &&

							( hasGeneralizationWordConfirmedSpecification ||
							!existingSpecificationItem.isOlderItem() ||

							( anotherPrimarySpecificationItem == null &&

							( hasGeneralizationWordConfirmedSpecification ||
							primarySpecificationItem == secondarySpecificationItem ||
							existingSpecificationItem.isSelfGeneratedAssumption() ) ) ||

							( relationWordItem != null &&
							!relationWordItem.isOlderItem() ) ) ) ) ) &&

							// User specification doesn't exist
							generalizationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == null &&
							// No justification exist as an opposite possessive conditional specification assumption
							generalizationWordItem.secondarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null )
								{
								isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_.isNounWordSpanishAmbiguous();

								// Make an exclusive specification substitution assumption (opposite)
								// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), foundOppositePossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( isHiddenSecondarySpecification ? primarySpecificationItem : secondarySpecificationItem ) : foundOppositePossessiveDefinitionSpecificationItem ), ( !isReplaceHiddenSpecification && isOppositePossessiveSpecificationWordSpanishAmbiguous ? null : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem : secondarySpecificationItem ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

								if( isPossessive )
									{
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										{
										if( hasRelationWord &&
										primarySpecificationItem.isSelfGenerated() &&
										( anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) ) != null )
											{
											hasFeminineOrMasculineProperNameEnding = false;
											feminineSpecificationWordItem = null;

											if( isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
												{
												hasFeminineOrMasculineProperNameEnding = true;

												if( feminineSpecificationWordItem_ == null )
													{
													if( existingSpecificationItem == null )
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem();
													}
												else
													feminineSpecificationWordItem = feminineSpecificationWordItem_;

												if( feminineSpecificationWordItem != null &&
												( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, anotherPrimarySpecificationItem, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
												}

											if( ( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// Not yet explained in the design
												if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, isNegative, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNameEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNameEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

												if( feminineSpecificationWordItem_ != null &&
												isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) &&
												( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNameEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNameEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNameEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), null, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine compound exclusive specification substitution assumption that is Spanish ambiguous in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem_.anyWordTypeString() + "\"" );
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
												secondarySpecificationItem = foundSpecificationItem;

												if( anotherPrimarySpecificationItem != null &&
												primarySpecificationItem.isUserSpecification() &&
												( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != null )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, anotherPrimarySpecificationItem, null, primarySpecificationItem, null, additionalSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

													if( additionalSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the found specification" );

													if( existingSpecificationItem != null &&
													!existingSpecificationItem.isOlderItem() &&

													// Clean-up obsolete justifications
													generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												// Found and created
												if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) == null )
													return adminItem_.startError( 1, moduleNameString_, "The found specification item has no first justification item" );

												if( foundSpecificationItem.isSelfGeneratedAssumption() )
													{
													// Attach all justification items of the found specification to the created specification
													if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the found specification" );

													if( generalizationWordItem.replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													// Test file: "Complex (12)"
													// Adjusted context. Needs to be rewritten
													primarySpecificationItem.lastWrittenSentenceStringBuffer = null;

													// Replace the secondary specification of a justification
													if( ( obsoleteJustificationItem = primarySpecificationItem.secondarySpecificationJustificationItem( foundSpecificationItem ) ) != null )
														{
														if( ( justificationResult = generalizationWordItem.copyJustification( false, obsoleteJustificationItem.primarySpecificationItem(), createdSpecificationItem, obsoleteJustificationItem.attachedJustificationItem(), obsoleteJustificationItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to copy an obsolete justification item with a different secondary specification item" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
															return adminItem_.startError( 1, moduleNameString_, "I couldn't copy an obsolete justification" );

														// Replace obsolete justification
														if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, primarySpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}

													if( ( additionalSecondarySpecificationItem = firstJustificationItem.secondarySpecificationItem() ) != null )
														{
														// Additional justification
														if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, additionalSecondarySpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
															return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

														if( createdSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
														}
													else
														return adminItem_.startError( 1, moduleNameString_, "The additional secondary specification item is undefined" );
													}
												}
											}
										}
									}
								else
									{
									if( createdSpecificationItem == null )
										{
										if( foundSpecificationItem == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't find any specification item" );

										if( foundSpecificationItem.isOlderItem() )
											{
											if( createAndAssignResult.createdJustificationItem != null &&
											!foundSpecificationItem.hasCurrentCreationSentenceNr() &&
											!foundSpecificationItem.isReplacedOrDeletedItem() &&

											// Clean-up obsolete justifications
											generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( spanishQuestionPrimarySpecificationItem_ == null )
												{
												if( relationWordItem != null &&
												( additionalSecondarySpecificationItem = relationWordItem.firstUserSpecificationItem( false, isPossessive, primarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, null ) ) != null &&
												additionalSecondarySpecificationItem.hasRelationContext() &&
												!additionalSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, additionalSecondarySpecificationItem, null, foundSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

													if( foundSpecificationItem.changeFirstJustification( createdJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the found specification" );

													if( ( shortResult = foundSpecificationItem.recalculateAssumptionLevel() ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );

													if( shortResult.shortValue == Constants.NO_ASSUMPTION_LEVEL &&
													// Force conclusion
													foundSpecificationItem.markAsConcludedAssumption( false ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
													}
												}
											else
												{
												if( foundSpecificationItem.wasHiddenSpanishSpecification() &&
												( spanishQuestionGeneralizationWordItem = spanishQuestionPrimarySpecificationItem_.generalizationWordItem() ) != null &&

												// Replace a question in a Spanish justification,
												// by a specification that is not hidden anymore
												spanishQuestionGeneralizationWordItem.updateSpecificationsInJustificationInWord( true, spanishQuestionPrimarySpecificationItem_, foundSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to update the justification items in generalization word \"" + spanishQuestionGeneralizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									else
										{
										if( hasRelationWord &&
										existingSpecificationItem != null &&
										existingSpecificationItem.hasRelationContext() &&
										!existingSpecificationItem.isReplacedOrDeletedItem() )
											{
											if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
												{
												if( ( !createdSpecificationItem.isHiddenSpanishSpecification() ||
												existingSpecificationItem.isHiddenSpanishSpecification() ) &&

												( existingSpecificationItem.hasSpecificationNonCompoundCollection() ||

												( existingSpecificationItem.hasSpecificationCompoundCollection() &&
												!existingSpecificationItem.hasOnlyOneRelationWord() ) ) )
													{
													// Created specification was hidden
													if( generalizationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( ( olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) == null )
													olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

												if( olderJustificationItem != null &&
												generalizationWordItem.attachJustificationInWord( olderJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to attach an older justification item to the created assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												{
												if( !generalizationWordItem.hasDisplayedMoreSpecificSpecification() )
													{
													if( generalizationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( firstJustificationItem = existingSpecificationItem.firstJustificationItem() ) == null )
														return adminItem_.startError( 1, moduleNameString_, "The existing specification item has no first justification item" );

													// Attach all justification items of the existing specification to the created specification
													if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the existing specification" );
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
			while( !CommonVariables.hasDisplayedWarning &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );

			if( !CommonVariables.hasDisplayedWarning &&
			primarySpecificationItem != null &&
			secondarySpecificationItem != null &&
			relationWordItem != null )
				{
				if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem.hasSpecificationNonCompoundCollection() &&
					( compoundCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR &&
					( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundCollectionNr, primarySpecificationItem.relationContextNr(), specificationWordItem ) ) != null )
						primarySpecificationItem = compoundSpecificationItem;
					}
				else
					{
					if( existingSpecificationItem != null &&
					primarySpecificationItem == secondarySpecificationItem &&
					primarySpecificationItem.hasSpecificationCompoundCollection() &&
					!existingSpecificationItem.isSpecificationWordSpanishAmbiguous() &&
					relationWordItem.isUserGeneralizationWord )
						secondarySpecificationItem = existingSpecificationItem;
					}

				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, ( isHiddenPrimarySpecification ? adminItem_.userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		int currentSpecificationCollectionNr;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem indirectPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
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
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
					{
					if( ( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem.firstActiveAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
						{
						// Search first for related specification, before add a new one
						if( ( relatedResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

						negativeQuestionSpecificationWordItem = ( relatedResult.relatedSpecificationItem == null ? null : relatedResult.relatedSpecificationItem.specificationWordItem() );

						// Draw an indirectly answered question conclusion
						// Not yet explained in the design
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );

						if( negativeQuestionSpecificationWordItem != null &&
						( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
						( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
							{
							if( commonWordItem.isNounWordSpanishAmbiguous() )
								{
								if( generalizationWordItem.replaceOrDeleteSpecification( false, currentQuestionSpecificationItem.updatedSpecificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace the answered question by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								// Clean-up obsolete justifications
								if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) ) != null &&
								( anotherPrimarySpecificationItem = commonWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != null &&
								// Draw a negative assumption
								// Not yet explained in the design
								( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
								}
							}

						currentQuestionSpecificationItem = adminItem_.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
						}
					else
						currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem();
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
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
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
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null )
					{
					if( generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, Constants.NO_QUESTION_PARAMETER, currentQuestionSpecificationWordItem ) == null )
						{
						nOptionsLeft++;
						onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
						}
					}
				}
			while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) != null );

			if( nOptionsLeft == 1 &&
			onlyOptionLeftSpecificationWordItem != null &&
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != null )
						{
						// Make an only option left assumption
						// Not yet explained in the design
						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				while( !CommonVariables.hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int secondarySpecificationCollectionNr;
		SpecificationItem existingSpecificationItem;
		RelatedResultType relatedResult = new RelatedResultType();

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();
		existingSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		// Check if specification doesn't exist yet
		if( existingSpecificationItem == null ||
		existingSpecificationItem.isHiddenSpanishSpecification() )
			{
			if( secondarySpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
			// Check if a related specification exists
			( relatedResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );

			if( relatedResult.relatedSpecificationItem == null )
				{
				if( generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() )
					{
					if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					// Typically for the Spanish language:
					// Skip making suggestive assumption if specification word is Spanish ambiguous,
					// and if user question has more than one generalization words
					if( !secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ||
					CommonVariables.nUserGeneralizationWords == 1 )
						{
						// Make a suggestive question assumption
						// Not yet explained in the design
						hasSuggestiveQuestionAssumption_ = true;

						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected CompoundResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		RelatedResultType relatedResult;
		CompoundResultType compoundResult = new CompoundResultType();
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem previousSpecificationItem = null;
		SpecificationItem lastCreatedSpecificationItem = null;
		WordItem currentGeneralizationWordItem;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		do	{
			if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
				{
				previousSpecificationItem = lastCreatedSpecificationItem;

				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startCompoundResultError( 1, moduleNameString_, "I found an undefined generalization word" );

					if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null )
						{
						if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem.hasAssumptionLevel(), foundSpecificationItem.isInactiveAssignment(), foundSpecificationItem.isArchivedAssignment(), foundSpecificationItem.isExclusiveSpecification(), true, Constants.NO_QUESTION_PARAMETER, foundSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( !CommonVariables.hasDisplayedWarning &&
						foundSpecificationItem.hasSpecificationCollection() )
							{
							if( ( relatedResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );

							if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.hasAssumptionLevel(), relatedSpecificationItem.isInactiveAssignment(), relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), true, relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem.specificationWordItem(), null ) != Constants.RESULT_OK )
									return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
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

		return compoundResult;
		}
	};

/*************************************************************************
 *	"Praise him for his mighty works;
 *	praise his unequaled greatness!" (Psalm 150:2)
 *************************************************************************/
