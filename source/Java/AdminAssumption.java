/*	Class:			AdminAssumption
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously make grammar-based assumptions
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

class AdminAssumption
	{
	// Private constructed variables

	private boolean hasSuggestiveQuestionAssumption_;

	private SpecificationItem foundPossessiveDefinitionSpecificationItem_;
	private SpecificationItem foundOppositePossessiveDefinitionSpecificationItem_;
	private SpecificationItem possessiveDefinitionSpecificationItem_;
	private SpecificationItem oppositePossessiveDefinitionSpecificationItem_;
	private SpecificationItem secondarySpecificationItem_;

	private WordItem feminineSpecificationWordItem_;
	private WordItem generalizationAssumptionWordItem_;
	private WordItem oppositePossessiveSpecificationWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

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

	private byte correctSuggestiveAssumptionsByOppositeQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem correctedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem oldSuggestiveSpecificationWordItem;
		WordItem secondarySpecificationWordItem;

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
		if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, oldSuggestiveQuestionAssumptionAssignmentItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = specificationResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, secondarySpecificationItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdNewSpecificationItem = specificationResult.createdSpecificationItem ) == null )
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

		if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution assumption" );

		if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the corrected generalization assumption" );

		if( foundSpecificationItem.attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );

		return Constants.RESULT_OK;
		}

	private byte findPossessiveSpecifications( WordItem generalizationWordItem, WordItem definitionWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem possessiveSpecificationWordItem;
		WordItem previousPossessiveSpecificationWordItem;
		WordItem higherLevelPossessiveSpecificationWordItem = null;

		foundPossessiveDefinitionSpecificationItem_ = null;
		foundOppositePossessiveDefinitionSpecificationItem_ = null;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( definitionWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given definition word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// First try to find a possessive specification on a higher level
		if( ( possessiveDefinitionSpecificationItem = definitionWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			do	{
				if( !possessiveDefinitionSpecificationItem.isExclusiveSpecification() &&
				possessiveDefinitionSpecificationItem.isPossessive() &&
				possessiveDefinitionSpecificationItem.isSpecificationNoun() &&
				( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
					{
					if( possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
					possessiveSpecificationWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) != null )
						{
						if( higherLevelPossessiveSpecificationWordItem == null )
							higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
						else
							hasFoundPossessiveSpecificationWord = false;
						}
					}
				}
			while( ( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextSelectedSpecificationItem() ) != null );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = definitionWordItem.firstNonQuestionSpecificationItem() ) != null )
				{
				if( possessiveDefinitionSpecificationItem.hasSpecificationCollection() )
					{
					if( higherLevelPossessiveSpecificationWordItem == null )
						higherLevelPossessiveSpecificationWordItem = specificationWordItem;

					// Try to find the opposite of the possessive specification
					do	{
						if( !possessiveDefinitionSpecificationItem.isExclusiveSpecification() &&
						!possessiveDefinitionSpecificationItem.isPartOf() &&
						possessiveDefinitionSpecificationItem.isPossessive() &&
						( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
							{
							if( possessiveSpecificationWordItem == generalizationWordItem )
								return adminItem_.startError( 1, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );

							if( possessiveSpecificationWordItem.isSingularNoun() &&
							previousPossessiveSpecificationItem != null &&
							( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem.specificationWordItem() ) != null )
								{
								possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem.specificationCollectionNr();

								if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
								higherLevelPossessiveSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) &&
								previousPossessiveSpecificationItem.specificationCollectionNr() == possessiveSpecificationCollectionNr )
									foundOppositePossessiveDefinitionSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

								foundPossessiveDefinitionSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
								}

							previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
							}
						}
					while( ( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextSelectedSpecificationItem() ) != null );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte makeGeneralizationAssumption( boolean hasCorrectedAssumptionByKnowledge, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		JustificationItem olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem olderGeneralizationAssumptionBySpecificationItem;

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
			if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME ? false : primarySpecificationItem.isEveryGeneralization() ), false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			if( hasCorrectedAssumptionByKnowledge &&
			specificationResult.createdJustificationItem != null &&
			( olderGeneralizationAssumptionBySpecificationItem = specificationResult.foundSpecificationItem ) != null )
				{
				if( ( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != null )
					{
					if( generalizationWordItem.replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, specificationResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminAssumption( AdminItem adminItem )
		{
		String errorString = null;

		hasSuggestiveQuestionAssumption_ = false;

		foundPossessiveDefinitionSpecificationItem_ = null;
		foundOppositePossessiveDefinitionSpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;
		oppositePossessiveDefinitionSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		generalizationAssumptionWordItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;

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

	protected void initializeAdminAssumptionVariables()
		{
		hasSuggestiveQuestionAssumption_ = false;

		secondarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		generalizationAssumptionWordItem_ = null;
		}

	protected boolean hasSuggestiveQuestionAssumption()
		{
		return hasSuggestiveQuestionAssumption_;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasFeminineOrMasculineProperNameEnding;
		boolean isHiddenPrimarySpecification;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isReplaceHiddenSpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean hasRelationWord = ( relationWordItem != null );
		int compoundCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem olderJustificationItem;
		JustificationItem replacingJustificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem compoundSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem secondarySpecificationItem = null;
		WordItem commonWordItem;
		WordItem currentSpecificationWordItem;
		WordItem feminineSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem possessiveSpecificationWordItem = null;

		possessiveDefinitionSpecificationItem_ = null;
		oppositePossessiveDefinitionSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

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
		!primarySpecificationItem.isExclusiveSpecification() ) ) ) )
			{
			if( ( currentSpecificationWordItem = CommonVariables.firstSpecificationWordItem ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The first specification word is undefined" );

			isHiddenPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			// Do for all specification words
			do	{
				if( currentSpecificationWordItem.isSingularNoun() &&
				currentSpecificationWordItem != specificationWordItem )
					{
					if( findPossessiveSpecifications( generalizationWordItem, currentSpecificationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

					isReplaceHiddenSpecification = false;
					anotherPrimarySpecificationItem = null;

					if( foundPossessiveDefinitionSpecificationItem_ != null &&
					( possessiveSpecificationWordItem = foundPossessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
						{
						possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem_;
						oppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem_;

						if( ( secondarySpecificationItem = possessiveSpecificationWordItem.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
							{
							isPossessiveSpecificationWordSpanishAmbiguous = possessiveSpecificationWordItem.isNounWordSpanishAmbiguous();

							if( ( !primarySpecificationItem.hasRelationContext() &&
							generalizationWordItem.hasAnsweredSelfGeneratedQuestion() ) ||

							( isPossessiveSpecificationWordSpanishAmbiguous &&
							primarySpecificationItem.specificationWordItem() == possessiveSpecificationWordItem ) )
								{
								if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );

								if( ( existingUserSpecificationItem == null &&

								( !isPossessiveSpecificationWordSpanishAmbiguous ||
								!isNegative ) ) ||

								( hasRelationWord &&
								existingUserSpecificationItem != null &&

								// Confirmed specification without relation word
								( !existingUserSpecificationItem.hasRelationContext() ||
								!existingUserSpecificationItem.isOlderItem() ||
								// Confirmed only a few of the entered relation words
								adminItem_.nContextWordsInAllWords( existingUserSpecificationItem.relationContextNr(), possessiveSpecificationWordItem ) < CommonVariables.nUserRelationWords ) ) )
									{
									hasFeminineOrMasculineProperNameEnding = false;

									if( isPossessiveSpecificationWordSpanishAmbiguous &&
									isFeminineOrMasculineProperNameEnding( isPossessive, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) )
										hasFeminineOrMasculineProperNameEnding = true;

									if( !hasRelationWord &&
									!isNegative &&
									( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
										{
										commonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationItem.specificationCollectionNr() );

										if( commonWordItem != null &&
										commonWordItem.isFemaleOrMale() &&
										!commonWordItem.isNounWordSpanishAmbiguous() &&
										!commonWordItem.isProperName() &&
										// Specification doesn't exist yet
										generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) == null &&
										( definitionSpecificationItem = commonWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
											{
											if( makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + commonWordItem.anyWordTypeString() + "\"" );
											}
										}

									// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), foundPossessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );

									createdSpecificationItem = specificationResult.createdSpecificationItem;
									foundSpecificationItem = specificationResult.foundSpecificationItem;

									if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) == null )
										return adminItem_.startError( 1, moduleNameString_, "No specification item was created or found" );

									if( isArchivedAssignment )
										secondarySpecificationItem_ = generalizationWordItem.firstAssignmentItem( false, false, true, isPossessive, createdOrFoundSpecificationItem.questionParameter(), createdOrFoundSpecificationItem.relationContextNr(), createdOrFoundSpecificationItem.specificationWordItem() );
									else
										secondarySpecificationItem_ = createdOrFoundSpecificationItem.updatedSpecificationItem();

									if( createdSpecificationItem != null )
										{
										if( isPossessiveSpecificationWordSpanishAmbiguous )
											{
											isReplaceHiddenSpecification = true;

											if( ( tempSpecificationItem = adminItem_.spanishAmbiguousCompoundPrimarySpecificationItem() ) != null )
												primarySpecificationItem = tempSpecificationItem;

											if( ( tempSpecificationItem = adminItem_.spanishAmbiguousCompoundAnotherPrimarySpecificationItem() ) != null )
												anotherPrimarySpecificationItem = tempSpecificationItem;
											}
										else
											{
											if( hasRelationWord &&
											foundSpecificationItem != null &&
											foundSpecificationItem.isSelfGeneratedAssumption() &&
											( createdJustificationItem = specificationResult.createdJustificationItem ) != null &&
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
											}
										}
									}
								}
							}
						}

					// First opposite possessive specification
					if( !CommonVariables.hasDisplayedWarning &&
					possessiveSpecificationWordItem != null &&
					foundOppositePossessiveDefinitionSpecificationItem_ != null &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
						{
						if( isNegative ||
						specificationContextNr > Constants.NO_CONTEXT_NR ||
						secondarySpecificationItem_ == null )
							{
							secondarySpecificationItem_ = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
						secondarySpecificationItem_ != null &&

						( !secondarySpecificationItem_.isOlderItem() ||
						secondarySpecificationItem_.isSelfGenerated() ) )
							{
							existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

							if( ( existingSpecificationItem == null ||
							!existingSpecificationItem.hasRelationContext() ||

							( hasRelationWord &&
							isPossessive &&
							existingSpecificationItem.isSelfGeneratedAssumption() ) ||

							( !isHiddenPrimarySpecification &&
							secondarySpecificationItem_.hasRelationContext() &&
							!secondarySpecificationItem_.isSelfGeneratedConclusion() &&

							( !isSpecificationWordSpanishAmbiguous ||
							existingSpecificationItem.wasHiddenSpanishSpecification() ||
							!existingSpecificationItem.isHiddenSpanishSpecification() ) ) ) &&

							// Skip if user specification exists
							generalizationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == null )
								{
								isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_.isNounWordSpanishAmbiguous();

								// Make an exclusive specification substitution assumption (opposite)
								// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), foundOppositePossessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( secondarySpecificationItem_.isHiddenSpanishSpecification() ? primarySpecificationItem : secondarySpecificationItem_ ) : foundOppositePossessiveDefinitionSpecificationItem_ ), ( !isReplaceHiddenSpecification && isOppositePossessiveSpecificationWordSpanishAmbiguous ? null : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem_ : secondarySpecificationItem_ ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

								createdSpecificationItem = specificationResult.createdSpecificationItem;
								foundSpecificationItem = specificationResult.foundSpecificationItem;

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

												if( feminineSpecificationWordItem != null )
													{
													if( adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem_, anotherPrimarySpecificationItem, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}

											compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationWordItem );

											if( compoundSpecificationItem != null &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// Not yet explained in the design
												if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, isNegative, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNameEnding ? foundOppositePossessiveDefinitionSpecificationItem_ : null ), ( hasFeminineOrMasculineProperNameEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem_ ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

												if( feminineSpecificationWordItem_ != null &&
												isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
													{
													if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isNegative, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNameEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem_ ), ( hasFeminineOrMasculineProperNameEnding ? foundOppositePossessiveDefinitionSpecificationItem_ : null ), ( hasFeminineOrMasculineProperNameEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), null, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine compound exclusive specification substitution assumption that is Spanish ambiguous in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem_.anyWordTypeString() + "\"" );
													}
												}
											}
										}
									else
										{
										if( foundSpecificationItem == null )
											secondarySpecificationItem_ = createdSpecificationItem;
										else
											{
											if( createdSpecificationItem == null )
												secondarySpecificationItem_ = foundSpecificationItem.updatedSpecificationItem();
											else
												{
												if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) == null )
													return adminItem_.startError( 1, moduleNameString_, "The found specification item has no first justification item" );

												// Attach all justification items of the found specification to the created specification
												if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the found specification" );

												if( generalizationWordItem.replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								else
									{
									if( createdSpecificationItem == null )
										{
										if( foundSpecificationItem != null )
											{
											if( specificationResult.createdJustificationItem == null )
												{
												if( ( replacingJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem_ ) ) != null &&
												( obsoleteJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, secondarySpecificationItem_ ) ) != null )
													{
													if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, replacingJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( !foundSpecificationItem.hasCurrentCreationSentenceNr() &&
												!foundSpecificationItem.isReplacedOrDeletedItem() )
													{
													if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to delete an obsolete justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
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
											if( generalizationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
												{
												if( ( olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) == null )
													olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

												if( olderJustificationItem == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't find an older justification item to attach" );

												if( generalizationWordItem.attachJustificationInWord( olderJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to attach an older justification item to the created assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												{
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
			while( !CommonVariables.hasDisplayedWarning &&
			( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );

			if( !CommonVariables.hasDisplayedWarning &&
			secondarySpecificationItem_ != null &&
			relationWordItem != null )
				{
				if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem.hasSpecificationNonCompoundCollection() &&
					( compoundCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
						{
						if( ( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundCollectionNr, primarySpecificationItem.relationContextNr(), specificationWordItem ) ) != null )
							primarySpecificationItem = compoundSpecificationItem;
						}
					}
				else
					{
					if( relationWordItem.isUserGeneralizationWord &&
					primarySpecificationItem == secondarySpecificationItem_ &&
					secondarySpecificationItem_.hasSpecificationCompoundCollection() &&
					( tempSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, null ) ) != null )
						{
						if( tempSpecificationItem.hasRelationContext() )
							secondarySpecificationItem_ = tempSpecificationItem;
						}
					}

				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem_, ( isHiddenPrimarySpecification ? adminItem_.userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
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
					return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

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
						!secondarySpecificationItem.isOlderItem() )
							{
							if( makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						{
						// Get secondary question specification for justification
						if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, questionParameter, specificationWordItem ) ) != null )
							{
							if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				}
			while( !CommonVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		SpecificationResultType specificationResult;
		int currentSpecificationCollectionNr;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem indirectPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		WordItem commonWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem negativeQuestionSpecificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( userSpecificationItem.hasSpecificationCollection() &&
		( currentQuestionSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
					{
					if( ( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem.firstActiveAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
						{
						// Search first for related specification, before add a new one
						if( ( specificationResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

						negativeQuestionSpecificationWordItem = ( specificationResult.relatedSpecificationItem == null ? null : specificationResult.relatedSpecificationItem.specificationWordItem() );

						// Draw an indirectly answered question conclusion
						// Not yet explained in the design
						if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );

						if( negativeQuestionSpecificationWordItem != null &&
						( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null &&
						( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
							{
							if( commonWordItem.isNounWordSpanishAmbiguous() )
								{
								if( generalizationWordItem.replaceOrDeleteSpecification( false, currentQuestionSpecificationItem.updatedSpecificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace the answered question by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( generalizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to delete obsolete assumption justification items in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) ) != null &&
								( anotherPrimarySpecificationItem = commonWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != null )
									{
									// Draw a negative assumption
									// Not yet explained in the design
									if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}

						currentQuestionSpecificationItem = adminItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
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

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != null )
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
						return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != null )
						{
						// Make an only option left assumption
						// Not yet explained in the design
						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				while( !CommonVariables.hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasSameSpecificationCollection;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveReversibleAssumption;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isRelationWordUserWordGeneralization;
		boolean hasFeminineOrMasculineProperNameEnding = false;
		short assumptionJustificationTypeNr;
		JustificationItem createdJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem selfGeneratedConclusionSpecificationItem = null;
		WordItem possessiveSpecificationWordItem;
		WordItem feminineSpecificationWordItem = null;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

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
					isPossessiveReversibleAssumption = ( secondarySpecificationItem.specificationWordItem() == possessiveSpecificationWordItem );

					// Make a possessive conditional specification assumption (normal)
					// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, false, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : null ), null, ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), null, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );

					if( !isPossessive &&
					( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
						{
						if( existingSpecificationItem == null )
							{
							userSpecificationItem = adminItem_.userSpecificationItem();

							if( userSpecificationItem != null &&
							!userSpecificationItem.isUserAssignment() )
								{
								// Additional justification
								if( ( justificationResult = generalizationWordItem.addJustification( false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, createdSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
									{
									secondarySpecificationItem = secondarySpecificationItem.updatedSpecificationItem();

									if( secondarySpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the secondary specification" );
									}
								}
							}
						else
							{
							existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

							if( !createdSpecificationItem.isHiddenSpanishSpecification() &&
							createdSpecificationItem.assumptionLevel() < existingSpecificationItem.assumptionLevel() )
								{
								if( relationWordItem.replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}
							}
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

					isPossessiveReversibleAssumption = ( secondarySpecificationItem.specificationWordItem() == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION :
													( isOppositePossessiveSpecificationWordSpanishAmbiguous && !hasFeminineOrMasculineProperNameEnding && isRelationWordUserWordGeneralization && alternativeSecondarySpecificationItem != null && !alternativeSecondarySpecificationItem.hasSpecificationCompoundCollection() ) ? Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( selfGeneratedConclusionSpecificationItem == null ||
					!selfGeneratedConclusionSpecificationItem.hasRelationContext() ||
					!generalizationWordItem.isOlderItem() ||

					( adminItem_.isActiveUserAssignment() &&
					alternativeSecondarySpecificationItem != null &&
					alternativeSecondarySpecificationItem.isSelfGenerated() ) ) &&

					( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!secondarySpecificationItem.isConcludedAssumption() &&

					( ( feminineSpecificationWordItem != null &&
					alternativeSecondarySpecificationItem != null &&
					alternativeSecondarySpecificationItem.isUserAssignment() &&
					// Skip if feminine specification exists as user specification
					relationWordItem.firstUserSpecificationItem( false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, feminineSpecificationWordItem ) == null ) ||

					relationWordItem.secondarySpecificationJustificationItem( true, assumptionJustificationTypeNr, secondarySpecificationItem ) == null ) ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != null &&
					// Skip if already exists as self-generated assumption
					relationWordItem.primarySpecificationJustificationItem( true, assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == null ) ) )
						{
						// Make a possessive conditional specification assumption (opposite)
						// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, assumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? null : ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : possessiveDefinitionSpecificationItem_ ) ), null, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( hasFeminineOrMasculineProperNameEnding && !isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem ), null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );

						if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
							{
							foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

							if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							( assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, !isPossessive, true, oppositePossessiveSpecificationWordItem_, relationWordItem ) ) != null )
								{
								// Additional justifications (Generalization word)
								if( ( justificationResult = generalizationWordItem.addJustification( false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, foundSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
									{
									if( generalizationWordItem.attachJustificationInWord( createdJustificationItem, assumptionSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );
									}

								// Additional justification (Relation word)
								if( ( justificationResult = relationWordItem.addJustification( false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, assumptionSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to generalization word \"" + relationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
									{
									if( foundSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the found specification" );
									}
								}
							else
								{
								if( isPossessiveSpecificationWordSpanishAmbiguous )
									{
									if( relationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace obsolete justification items in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}

						if( alternativeSecondarySpecificationItem != null &&
						feminineSpecificationWordItem != null )
							{
							existingSpecificationItem = null;

							if( isOppositePossessiveSpecificationWordSpanishAmbiguous ||

							( alternativeSecondarySpecificationItem.isSelfGenerated() &&
							!alternativeSecondarySpecificationItem.isUserSpecification() ) )
								existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isPossessive, Constants.NO_QUESTION_PARAMETER, feminineSpecificationWordItem, generalizationWordItem );

							if( existingSpecificationItem == null ||

							( existingSpecificationItem.isSelfGeneratedAssumption() &&

							( !secondarySpecificationItem.isHiddenSpanishSpecification() ||

							( !relationWordItem.hasConfirmedSpecification() &&
							alternativeSecondarySpecificationItem.hasSpecificationCollection() ) ) ) )
								{
								// Make an opposite possessive feminine proper name ending assumption
								// Not yet explained in the design
								if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( existingSpecificationItem == null ? Constants.NO_CONTEXT_NR : existingSpecificationItem.relationContextNr() ), alternativeSecondarySpecificationItem, ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != null ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

								if( !isPossessive )
									{
									feminineSpecificationWordItem_ = feminineSpecificationWordItem;

									if( !isArchivedAssignment &&
									( createdOrFoundSpecificationItem = ( specificationResult.createdSpecificationItem == null ? specificationResult.foundSpecificationItem : specificationResult.createdSpecificationItem ) ) != null )
										{
										if( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
										!createdOrFoundSpecificationItem.isReplacedItem() )
											{
											// Generate the reversed specification
											if( adminItem_.drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, createdOrFoundSpecificationItem.specificationContextNr(), createdOrFoundSpecificationItem.relationContextNr(), relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								}
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean hasGeneralizationWordConfirmedSpecification;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasCorrectedAssumptionByKnowledge;
		boolean isGeneralizationWordSpanishAmbiguous;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem createdJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem alternativeAnotherPrimarySpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primaryPartOfSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem feminineCollectionWordItem;
		WordItem userSpecificationWordItem;

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
			hasGeneralizationWordConfirmedSpecification = generalizationWordItem.hasConfirmedSpecification();
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
			isGeneralizationWordSpanishAmbiguous = generalizationWordItem.isNounWordSpanishAmbiguous();
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem();

			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&

				( currentGeneralizationWordItem.hasNonPossessiveAndNonQuestionPartOfSpecification() ||

				( !currentGeneralizationWordItem.hasCollection() &&
				!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() ) ) )
					{
					existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

					// Skip if concluded conclusion exists
					if( ( existingSpecificationItem == null ||
					existingSpecificationItem != simpleUserSpecificationItem ) &&

					// Get primary specification for justification
					( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
						{
						alternativeAnotherPrimarySpecificationItem = null;
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
								( userSpecificationWordItem = simpleUserSpecificationItem.specificationWordItem() ) != null )
									{
									if( ( tempSpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null )
										{
										primarySpecificationItem = simpleUserSpecificationItem;
										secondarySpecificationItem = tempSpecificationItem;
										}
									}
								}
							else
								{
								if( isSpecificationWordSpanishAmbiguous &&
								secondarySpecificationItem.isHiddenSpanishSpecification() )
									{
									secondarySpecificationItem = adminItem_.alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, false, true, false, true, simpleUserSpecificationItem, generalizationWordItem, specificationWordItem );
									alternativeAnotherPrimarySpecificationItem = adminItem_.alternativeAnotherPrimarySpecificationItem();
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
								if( primarySpecificationItem.isPartOf() &&
								// If a conclusion exists, skip creating an assumption
								generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem, null ) == null )
									{
									// Make a substitution 'part of' assumption
									// Not yet explained in the design
									if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, alternativeAnotherPrimarySpecificationItem, primarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != null &&
									( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem.specificationSubstitutionPartOfAssumptionJustificationItem( primarySpecificationItem ) ) != null )
										{
										primaryPartOfSpecificationItem = specificationSubstitutionPartOfAssumptionJustificationItem.primarySpecificationItem();

										if( hasGeneralizationWordConfirmedSpecification ||
										specificationSubstitutionPartOfAssumptionJustificationItem.hasPrimaryAnsweredQuestion() ||

										( primaryPartOfSpecificationItem != null &&
										secondarySpecificationItem.assumptionLevel() < primaryPartOfSpecificationItem.assumptionLevel() ) )
											{
											if( generalizationWordItem.updateJustificationInSpecifications( false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( !specificationSubstitutionPartOfAssumptionJustificationItem.isReplacedOrDeletedItem() &&
											!generalizationWordItem.isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) )
												{
												if( generalizationWordItem.replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to delete the specification substitution 'part of' assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								}
							else
								{
								if( primarySpecificationItem.isExclusiveSpecification() &&

								( !isGeneralizationWordSpanishAmbiguous ||
								secondarySpecificationItem.isExclusiveSpecification() ) )
									{
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
		( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
		( secondarySpecificationItem = specificationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != null )
			{
			// Make an exclusive specification substitution assumption
			// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
			if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, primarySpecificationItem.isEveryGeneralization(), false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, generalizationAssumptionWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult;
		int secondarySpecificationCollectionNr;
		SpecificationItem existingSpecificationItem;

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();
		existingSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		// Check if specification doesn't exist yet
		if( existingSpecificationItem == null ||
		existingSpecificationItem.isHiddenSpanishSpecification() )
			{
			// Check if a related specification exists
			if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );

			if( specificationResult.relatedSpecificationItem == null )
				{
				if( generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() )
					{
					if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					// Make a suggestive question assumption
					// Not yet explained in the design
					hasSuggestiveQuestionAssumption_ = true;

					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationItem oppositePossessiveDefinitionSpecificationItem()
		{
		return oppositePossessiveDefinitionSpecificationItem_;
		}
	};

/*************************************************************************
 *	"He is my loving ally and my fortress,
 *	my tower of safety, my rescuer.
 *	He is my shield, and I take refuge in him.
 *	He makes the nations submit to me." (Psalm 144:2)
 *************************************************************************/
