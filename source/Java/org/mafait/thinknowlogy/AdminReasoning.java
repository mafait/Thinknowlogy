/*	Class:			AdminReasoning
 *	Supports class:	AdminItem
 *	Purpose:		Providing autonomous reasoning
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

class AdminReasoning
	{
	// Private constructed variables

	private SpecificationItem oppositePossessiveDefinitionSpecificationItem_ = null;
	private SpecificationItem possessiveDefinitionSpecificationItem_ = null;

	private WordItem firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;
	private WordItem secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;

	private final String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private static boolean isFeminineOrMasculineProperNounEnding( boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isFeminineSpecificationWord;
		boolean isMasculineSpecificationWord;

		if( generalizationWordItem != null &&
		specificationWordItem != null )
			{
			isFeminineSpecificationWord = specificationWordItem.isFeminineWord();
			isMasculineSpecificationWord = specificationWordItem.isMasculineWord();

			if( isFeminineSpecificationWord ||
			isMasculineSpecificationWord )
						// Possessive (relation)
				return ( ( isPossessive &&
						relationWordItem != null &&

							( ( isFeminineSpecificationWord &&
							relationWordItem.hasFeminineProperNounEnding() ) ||

							( isMasculineSpecificationWord &&
							relationWordItem.hasMasculineProperNounEnding() ) ) ) ||

						// Non-possessive (generalization)
						( !isPossessive &&

							( ( isFeminineSpecificationWord &&
							generalizationWordItem.hasFeminineProperNounEnding() ) ||

							( isMasculineSpecificationWord &&

								( generalizationWordItem.hasMasculineProperNounEnding() ||

								// Typical for Spanish
								( specificationWordItem.isNounWordSpanishAmbiguous() &&
								generalizationWordItem.hasFeminineProperNounEnding() ) ) ) ) ) );
			}

		return false;
		}

	private byte addPartOfJustificationToRelationCollectionWords( int relationCollectionNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		SpecificationItem partOfSpecificationItem;
		WordItem currentCollectionWordItem;

		if( relationCollectionNr == Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The relation collection number is undefined" );

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( ( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr ) ) != null )
			{
			// Do for all collection words
			do	{
				if( ( partOfSpecificationItem = currentCollectionWordItem.firstPartOfSpecificationItem() ) != null &&
				// Justification doesn't exist yet in specification
				!partOfSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) &&
				// Justification has at least the same assumption level
				partOfSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem, null ) &&
				// Add part-of justification to part-of specification
				partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a part-of justification to a part-of specification in word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestions( boolean isSpanishCurrentLanguage, SpecificationItem simpleUserSpecificationItem, SpecificationItem userSpecificationItem )
		{
		boolean hasUserSpecificationCurrentlyMoreSpecificSpecification;
		boolean hasUserSpecificationNonCompoundCollection;
		boolean isAllowingSpanishPossessiveSpecification;
		boolean isArchivedUserAssignment;
		boolean isChineseCurrentLanguage = WordItem.isChineseCurrentLanguage();
		boolean isCurrentTouchedRelationWord;
		boolean isPossessiveUserSpecification;
		int candidateForQuestionSpecificationCollectionNr;
		int nRelationWords;
		int nUserRelationWords;
		SpecificationItem additionalDefinitionSpecificationItem = null;
		SpecificationItem candidateForQuestionSpecificationItem;
		SpecificationItem firstExclusiveSpecificationItem;
		SpecificationItem foundQuestionSpecificationItem;
		WordItem currentTouchedWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( ( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		isPossessiveUserSpecification = userSpecificationItem.isPossessive();

		if( userSpecificationItem.isProperNounGeneralization() )
			{
			additionalDefinitionSpecificationItem = getAdditionalDefinitionSpecificationItem( userSpecificationWordItem );

			if( userSpecificationItem.isExclusiveSpecification() )
				{
				// Question part doesn't exist yet
				if( !userGeneralizationWordItem.hasJustification( userSpecificationItem, null, null ) )
					{
					isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();

					// Ask question after more specific non-exclusive user specification
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedUserAssignment, isArchivedUserAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, userGeneralizationWordItem, userSpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about the current user sentence" );
					}
				}
			else
				{
				if( isPossessiveUserSpecification )
					{
					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					!userGeneralizationWordItem.hasFeminineOrMasculineProperNounEnding() &&

						( userSpecificationItem.isSpecificationWordSpanishAmbiguous() ||
						!userGeneralizationWordItem.hasActiveQuestionWithCompoundCollection() ) &&

					// Ask questions about generalization word with possessive primary specification
					askSpanishQuestionAboutGeneralizationWord( true, userSpecificationItem.specificationCollectionNr(), userSpecificationItem, additionalDefinitionSpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with possessive user specification as primary specification" );
					}
				else
					{
					if( userGeneralizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
					// Avoid idle call
					userGeneralizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( false ) != null &&
					// Correct suggestive assumptions by opposite suggestive question
					correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( userSpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question in word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			hasUserSpecificationCurrentlyMoreSpecificSpecification = userGeneralizationWordItem.hasCurrentlyMoreSpecificSpecification();
			hasUserSpecificationNonCompoundCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			nRelationWords = userSpecificationItem.nRelationWords();
			nUserRelationWords = GlobalVariables.nUserRelationWords;

			// Do for all words touched during current sentence
			do	{
				if( currentTouchedWordItem.isProperNounWord() )
					{
					isCurrentTouchedRelationWord = currentTouchedWordItem.isUserRelationWord;
					// Typical for Spanish
					isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
																isCurrentTouchedRelationWord );

					if( ( candidateForQuestionSpecificationItem = currentTouchedWordItem.firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null )
						{
						if( !candidateForQuestionSpecificationItem.hasRelationWord() &&
						candidateForQuestionSpecificationItem.hasSpecificationCollection() &&
						candidateForQuestionSpecificationItem.isSelfGeneratedSpecification() )
							candidateForQuestionSpecificationItem = userSpecificationItem;

						candidateForQuestionSpecificationCollectionNr = candidateForQuestionSpecificationItem.specificationCollectionNr();
						foundQuestionSpecificationItem = currentTouchedWordItem.firstSpecificationItem( false, false, false, true );

						if( ( ( simpleUserSpecificationItem != null &&

							// Non-possessive user specification
							( ( !isPossessiveUserSpecification &&

								// Not touched relation word
								( ( !isCurrentTouchedRelationWord &&
								candidateForQuestionSpecificationItem == simpleUserSpecificationItem &&

									// Not Spanish
									( ( !isSpanishCurrentLanguage &&

										( userSpecificationItem == simpleUserSpecificationItem ||

										( foundQuestionSpecificationItem == null &&

										// Test file: "Your question is more specific (about John)"
										( nUserRelationWords == 1 ||
										// Test file: "Paul is the son of John and Anna"
										nRelationWords < nUserRelationWords ) ) ) ) ||

									// Typical for Spanish
									// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
									( isSpanishCurrentLanguage &&
									userSpecificationItem == simpleUserSpecificationItem &&
									foundQuestionSpecificationItem != null &&
									foundQuestionSpecificationItem.isOlderItem() ) ) ) ||

								// Touched relation word
								( isCurrentTouchedRelationWord &&
								// Typical for Spanish
								isSpanishCurrentLanguage &&
								// Positive specification doesn't exist
								currentTouchedWordItem.bestMatchingRelationSpecificationItem( false, false, candidateForQuestionSpecificationCollectionNr, null ) == null ) ) ) ||

							// Possessive user specification
							// Test file: "Sentence with possessive verb"
							( isPossessiveUserSpecification &&
							isCurrentTouchedRelationWord &&
							// No question exists
							foundQuestionSpecificationItem == null ) ) ) ||

						// Negative specification doesn't exist
						currentTouchedWordItem.bestMatchingRelationSpecificationItem( true, false, candidateForQuestionSpecificationCollectionNr, null ) == null ) &&

						// Ask questions
						askQuestions( isAllowingSpanishPossessiveSpecification, isSpanishCurrentLanguage, additionalDefinitionSpecificationItem, userSpecificationItem, currentTouchedWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper noun word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );

						if( additionalDefinitionSpecificationItem != null &&
						( firstExclusiveSpecificationItem = currentTouchedWordItem.firstExclusiveSpecificationItem( false ) ) != null &&

							// Not touched relation word
							( ( !isCurrentTouchedRelationWord &&

								( nUserRelationWords <= 1 ||

								( hasUserSpecificationNonCompoundCollection &&
								!candidateForQuestionSpecificationItem.hasJustificationWithAdditionalDefinitionSpecification() ) ) ) ||

							// Touched relation word
							( isCurrentTouchedRelationWord &&

								( hasUserSpecificationCurrentlyMoreSpecificSpecification ||

								( candidateForQuestionSpecificationItem.isOlderItem() &&

									( candidateForQuestionSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == null ||

									// Test file: "reasoning\family\Complex (5)"
									( hasUserSpecificationNonCompoundCollection &&
									// Positive specification doesn't exist
									currentTouchedWordItem.bestMatchingRelationSpecificationItem( false, false, candidateForQuestionSpecificationCollectionNr, null ) == null ) ||

									// Typical for Chinese
									isChineseCurrentLanguage ) ) ||

								( !firstExclusiveSpecificationItem.isOlderItem() &&
								!candidateForQuestionSpecificationItem.hasJustificationWithAdditionalDefinitionSpecification() &&
								!currentTouchedWordItem.hasFeminineOrMasculineProperNounEnding() &&
								// Adjective questions don't exist yet
								currentTouchedWordItem.firstAdjectiveSpecificationItem( false, true ) == null ) ||

								// Typical for Spanish
								isAllowingSpanishPossessiveSpecification ) ) ) &&

						// Ask questions with question as primary specification
						askQuestionsWithQuestionAsPrimarySpecification( userSpecificationItem, currentTouchedWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper noun word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askQuestions1++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askQuestions1++;
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestions( boolean isAllowingSpanishPossessiveSpecification, boolean isSpanishCurrentLanguage, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean isExclusiveUserSpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimarySameAsUserSpecification;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isSkippingSimpleQuestions;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationWordSpanishAmbiguous;
		int exclusiveSecondarySpecificationCollectionNr;
		int userSpecificationCollectionNr;
		SpecificationItem archivedPrimarySpecificationItem;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem existingQuestionSpecificationItem;
		SpecificationItem positiveSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem questionSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem primarySpecificationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem.firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null )
			{
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			do	{
				if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null )
					{
					if( primarySpecificationItem.isPossessive() )
						{
						if( !hasGeneralizationWordFeminineOrMasculineProperNounEnding )
							{
							selectedPrimarySpecificationItem = ( ( archivedPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem ) ) != null &&
																archivedPrimarySpecificationItem.isArchivedAssignment() ? archivedPrimarySpecificationItem :
																															primarySpecificationItem );

							// Typical for Spanish
							// Specification with user specification collection doesn't exist yet
							if( generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, null ) == null &&
							// Ask questions about generalization word with possessive primary specification
							askSpanishQuestionAboutGeneralizationWord( isPossessiveUserSpecification, userSpecificationCollectionNr, selectedPrimarySpecificationItem, additionalDefinitionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish questions about word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
							}
						}
					else
						{
						isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
						isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();

						if( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
						isPrimarySpecificationWordSpanishAmbiguous ||
						primarySpecificationItem.hasCompoundSpecificationCollection() ||

						// Test file: "Additional test"
						( isPrimarySameAsUserSpecification &&
						!primarySpecificationItem.hasRelationWord() ) ) &&

						( exclusiveSecondarySpecificationItem = primarySpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null &&
						( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null )
							{
							isSkippingSimpleQuestions = false;

							if( !isUserGeneralizationWord )
								{
								if( isPossessiveUserSpecification )
									{
									// Typical for Spanish
									if( isUserSpecificationWordSpanishAmbiguous &&
									// Number of relation words of user specification item
									userSpecificationItem.nEnteredRelationWords() > 1 &&

										// Skip if current questions exist
										( ( questionSpecificationItem = generalizationWordItem.firstActiveQuestionSpecificationItem() ) == null ||
										questionSpecificationItem.isOlderItem() ) &&

										( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
										// Skip invalid questions / similar specification doesn't exist yet
										generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, exclusiveSecondarySpecificationCollectionNr, null ) == null )
										{
										isSkippingSimpleQuestions = true;

										// Ask Spanish question about word relation word with possessive primary specification
										if( askSpanishQuestionAboutRelationWord( primarySpecificationItem.isArchivedAssignment(), primarySpecificationItem.generalizationWordTypeNr(), userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to ask a Spanish question about relation word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
										}
									}
								else
									{
									if( generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem.isArchivedAssignment(), false, false, false, primarySpecificationWordItem, null ) != null )
										isSkippingSimpleQuestions = true;
									}
								}

							if( !isSkippingSimpleQuestions &&
							// Positive specification doesn't exist
							generalizationWordItem.firstSpecificationItem( false, false, false, exclusiveSecondarySpecificationWordItem ) == null &&

								// Question specification doesn't exist
								( ( ( ( existingQuestionSpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem( exclusiveSecondarySpecificationWordItem ) ) == null ||
								existingQuestionSpecificationItem.isOlderItem() ||

								( !isPrimarySameAsUserSpecification &&
								!primarySpecificationItem.isOlderItem() ) ) &&

									( ( isPrimarySameAsUserSpecification &&
									// Negative specification doesn't exist
									generalizationWordItem.firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == null ) ||

									// Question specification doesn't exist
									( existingQuestionSpecificationItem == null &&
									generalizationWordItem.firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == null ) ||

									// Test file: "This information is more specific (non-exclusive)"
									( isExclusiveUserSpecification &&
									!primarySpecificationItem.hasSpecificationCollection() ) ||

									// Question specification exists
									( existingQuestionSpecificationItem != null &&
									// Justification doesn't exist yet in question
									!existingQuestionSpecificationItem.hasSecondarySpecificationJustification( exclusiveSecondarySpecificationItem ) &&
									// Negative specification doesn't exist
									generalizationWordItem.bestMatchingRelationSpecificationItem( true, false, userSpecificationCollectionNr, null ) == null ) ) ) ||

								// Positive specification doesn't exist
								( ( positiveSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, userSpecificationCollectionNr, null ) ) == null &&
								// Question specification doesn't exist
								existingQuestionSpecificationItem == null ) ||

								// Typical for Spanish
								( isPrimarySpecificationWordSpanishAmbiguous &&
								positiveSpecificationItem != null &&
								!positiveSpecificationItem.isOlderItem() ) ) &&

							// Ask simple questions
							askSimpleQuestions( isSpanishCurrentLanguage, exclusiveSecondarySpecificationItem, primarySpecificationItem, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask simple questions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem.nextCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askQuestions2++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askQuestions2++;
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestionsWithQuestionAsPrimarySpecification( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasUserSpecificationCompoundCollection;
		boolean isArchivedAssignment;
		boolean isFeminineOrMasculineUserSpecificationWord;
		boolean isPossessiveUserSpecification;
		boolean isUserRelationWord;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem( false ) ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			isFeminineOrMasculineUserSpecificationWord = userSpecificationWordItem.isFeminineOrMasculineWord();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();

			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( ( hasUserSpecificationCompoundCollection ||
				primarySpecificationItem.hasCompoundSpecificationCollection() ) &&

				( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
				( secondarySpecificationItem = primarySpecificationWordItem.firstNonCompoundUserSpecificationItem() ) != null &&
				( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
				// Question part doesn't exist yet as specification
				generalizationWordItem.firstSpecificationItem( false, false, false, secondarySpecificationWordItem ) == null &&

					( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					isUserRelationWord ) ||

					// Skip adjective specification words of user generalization word
					!secondarySpecificationItem.isAdjectiveSpecification() ) &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == null ||
				// Justification doesn't exist yet in question
				!existingSpecificationItem.hasSecondarySpecificationJustification( secondarySpecificationItem ) ) &&

					( !isFeminineOrMasculineUserSpecificationWord ||
					!isPossessiveUserSpecification ||
					// Typical for Spanish
					existingSpecificationItem != null ) )
					{
					isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

					// Ask a specification substitution question
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem.nextExclusiveSpecificationItem( false ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askQuestionsWithQuestionAsPrimarySpecification++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askQuestionsWithQuestionAsPrimarySpecification++;
			}

		return Constants.RESULT_OK;
		}

	private byte askSimpleQuestions( boolean isSpanishCurrentLanguage, SpecificationItem exclusiveSecondarySpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasExclusiveSecondarySpecificationCompoundCollection;
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasPrimarySpecificationRelationWord;
		boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection;
		boolean hasSkippedAskingQuestion = false;
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationRelationWord;
		boolean isArchivedAssignment;
		boolean isCompoundGeneralizationWordSpanishAmbiguous;
		boolean isOlderPrimarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimarySameAsUserSpecification;
		boolean isPrimaryUserSpecification;
		boolean isUserGeneralizationWord;
		boolean isUserQuestion;
		boolean isUserSpecificationWordSpanishAmbiguous;
		short generalizationWordTypeNr;
		int exclusiveSecondarySpecificationCollectionNr;
		int primarySpecificationCollectionNr;
		int userSpecificationCollectionNr;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem exclusiveSpecificationItem;
		SpecificationItem existingQuestionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem compoundGeneralizationWordItem;
		WordItem exclusiveSpecificationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( exclusiveSecondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given exclusive secondary specification item is undefined" );

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
		hasPrimarySpecificationRelationWord = primarySpecificationItem.hasRelationWord();
		isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();
		isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
		isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
		isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();
		generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();
		primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

		hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
		hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();
		isOlderUserSpecification = userSpecificationItem.isOlderItem();
		isPossessiveUserSpecification = userSpecificationItem.isPossessive();
		isUserQuestion = userSpecificationItem.isQuestion();
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
		userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

		hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
		hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection = generalizationWordItem.hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		do	{
			hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem.hasCompoundSpecificationCollection();

			if( ( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null )
				{
				exclusiveSpecificationItem = null;
				foundSpecificationItem = null;

				if( ( exclusiveSpecificationItem = exclusiveSecondarySpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
					{
					// Do for all exclusive specifications of this word (including adjectives)
					do	{
						if( ( exclusiveSpecificationWordItem = exclusiveSpecificationItem.specificationWordItem() ) != null )
							foundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, exclusiveSpecificationWordItem );
						}
					while( foundSpecificationItem == null &&
					( exclusiveSpecificationItem = exclusiveSpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
					}

				if( foundSpecificationItem == null )
					{
					if( ( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
						{
						isCompoundGeneralizationWordSpanishAmbiguous = ( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem.compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null &&
																		compoundGeneralizationWordItem.isNounWordSpanishAmbiguous() );

						existingQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem );

						// Useful user specification without relation
						if( ( ( hasPrimarySpecificationCompoundCollection &&
						isPrimaryUserSpecification &&
						!exclusiveSecondarySpecificationItem.isAdjectiveSpecification() &&

							( !isUserSpecificationWordSpanishAmbiguous ||

							// Typical for Spanish
							( isPrimarySameAsUserSpecification &&
							!hasPrimarySpecificationRelationWord &&
							!exclusiveSecondarySpecificationWordItem.isFeminineWord() ) ) ) ||

						// No useful user specification without relation
						( !hasUserSpecificationCompoundCollection &&
						!hasPrimarySpecificationRelationWord &&
						!isOlderPrimarySpecification &&

							// Test file: "reasoning\family\Complex (16)"
							( ( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
							hasPrimarySpecificationCompoundCollection ) ||

							( isPrimaryUserSpecification &&
							exclusiveSecondarySpecificationItem.isUserSpecification() ) ) ) ||

						// Compound collection
						( hasExclusiveSecondarySpecificationCompoundCollection &&
						hasUserSpecificationRelationWord &&

							( ( hasPrimarySpecificationCompoundCollection &&

								( hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
								!exclusiveSecondarySpecificationWordItem.isFeminineWord() ) ) ||

							( !isUserGeneralizationWord &&

								( !isPossessiveUserSpecification ||

								( !isUserSpecificationWordSpanishAmbiguous &&
								primarySpecificationWordItem.isUserSpecificationWord ) ) ) ) ) ||

						// No compound collection
						// Added additional justification
						( !hasExclusiveSecondarySpecificationCompoundCollection &&
						!hasPrimarySpecificationRelationWord &&
						hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection &&

							// Test file: "reasoning\Scientific challenge"
							( ( existingQuestionSpecificationItem == null &&
							!hasUserSpecificationCompoundCollection ) ||

							( existingQuestionSpecificationItem != null &&
							!existingQuestionSpecificationItem.hasSecondarySpecificationCompoundCollectionJustification() ) ) ) ||

						// Avoid adding non-compound justification to compound question
						( primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

							( hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection ||

							( isUserGeneralizationWord &&
							!hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
							!isSpanishCurrentLanguage ) ) ) ) &&

						// Typical for Spanish
						// Spanish test file: "Especificación-generalización - Especificación individual"
						( ( isCompoundGeneralizationWordSpanishAmbiguous &&
						!isUserSpecificationWordSpanishAmbiguous ) ||

						// Question part doesn't exist yet
						!generalizationWordItem.hasJustification( primarySpecificationItem, null, exclusiveSecondarySpecificationItem ) ) )
							{
							// Has possibly a gap in knowledge
							if( ( ( !isOlderPrimarySpecification ||
							isUserQuestion ) &&

							// Negative specification doesn't exist yet
							( generalizationWordItem.firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == null &&
							generalizationWordItem.hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationCollectionNr, primarySpecificationItem ) ) ) ||

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (17)"
							( isUserSpecificationWordSpanishAmbiguous &&
							!hasPrimarySpecificationRelationWord &&
							compoundGeneralizationWordItem != primarySpecificationWordItem ) )
								{
								// Avoid asking invalid questions:
								// 1) Check for existing knowledge with same specification collection
								if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, exclusiveSecondarySpecificationWordItem ) == null &&
								// 2) Check for existing negative specification
								generalizationWordItem.firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == null &&

								( existingQuestionSpecificationItem == null ||

								( ( !isSpanishCurrentLanguage ||
								// Typical for Spanish
								// Avoid almost duplicate justifications
								!hasPrimarySpecificationRelationWord ||
								!isPossessiveUserSpecification ) &&

								// Justification doesn't exist yet in specification
								!existingQuestionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, exclusiveSecondarySpecificationItem ) ) ) )
									{
									// Ask a specification substitution question
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );

									if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
									createdOrFoundSpecificationItem.isCorrectedSpecification() )
										{
										if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ == null )
											firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = primarySpecificationWordItem;
										else
											{
											if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != primarySpecificationWordItem )
												secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = primarySpecificationWordItem;
											}
										}
									}
								else
									hasSkippedAskingQuestion = true;
								}
							else
								{
								if( hasExclusiveSecondarySpecificationCompoundCollection &&
								hasUserSpecificationCompoundCollection &&
								!isOlderUserSpecification &&
								isUserGeneralizationWord &&
								primarySpecificationWordItem.hasCollectionNr( userSpecificationCollectionNr ) &&
								exclusiveSecondarySpecificationWordItem.nonCompoundCollectionNr() > Constants.NO_COLLECTION_NR &&
								// Question exists
								generalizationWordItem.firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem ) != null &&
								// Find question to be adjusted by compound collection
								findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationItem, userSpecificationItem, generalizationWordItem, primarySpecificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection about word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );
								}
							}
						}
					}
				else
					{
					// Test file: "Skip invalid questions (1)"
					// Specification doesn't exist yet
					if( generalizationWordItem.firstSpecificationItem( false, false, false, exclusiveSecondarySpecificationWordItem ) == null &&
					// Make exclusive specification substitution assumption
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, exclusiveSecondarySpecificationItem, exclusiveSpecificationItem, foundSpecificationItem, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}
		while( !hasSkippedAskingQuestion &&
		( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askSimpleQuestions++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askSimpleQuestions++;
			}

		return Constants.RESULT_OK;
		}

	private byte askSpanishQuestionAboutGeneralizationWord( boolean isPossessiveUserSpecification, int userSpecificationCollectionNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasMultipleUserGeneralizationWords = ( GlobalVariables.nUserGeneralizationWords > 1 );
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasPrimarySpecificationRelationWord;
		boolean isOlderPrimarySpecification;
		boolean isUserGeneralizationWord;
		short generalizationWordTypeNr;
		int exclusiveSecondarySpecificationCollectionNr;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem existingQuestionSpecificationItem;
		WordItem additionalDefinitionSpecificationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem tempWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given additional definition specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem.specificationWordItem() ) != null &&
		( exclusiveSecondarySpecificationItem = additionalDefinitionSpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
			{
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
			hasPrimarySpecificationRelationWord = primarySpecificationItem.hasRelationWord();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();

			do	{
				// Current question part doesn't exist yet
				if( !generalizationWordItem.hasJustification( primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&

					// No answered questions
					( ( ( existingQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS ) ) == null &&
					isUserGeneralizationWord &&
					hasPrimarySpecificationRelationWord &&
					// User specification doesn't exist yet
					generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, null ) == null ) ||

					( ( exclusiveSecondarySpecificationItem.isSelfGeneratedSpecification() ||

					( ( tempWordItem = exclusiveSecondarySpecificationWordItem.compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null &&
					!tempWordItem.isNounWordSpanishAmbiguous() ) ) &&

 						( hasMultipleUserGeneralizationWords ||
						isUserGeneralizationWord || 

						( isOlderPrimarySpecification &&
						!isPossessiveUserSpecification ) ||

						( hasPrimarySpecificationCompoundCollection &&
						!isPossessiveUserSpecification ) ) &&

					( existingQuestionSpecificationItem == null ||

					// Justification doesn't exist yet in specification
					( !existingQuestionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, exclusiveSecondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingQuestionSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, null ) ) ) &&

					// Positive specification doesn't exist yet
					generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, null ) == null &&
					// Negative specification doesn't exist yet
					generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, null ) == null ) ) &&

				// Ask a specification substitution question
				adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askSpanishQuestionAboutGeneralizationWord++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askSpanishQuestionAboutGeneralizationWord++;
			}

		return Constants.RESULT_OK;
		}

	private byte askSpanishQuestionAboutRelationWord( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationItem exclusiveSecondarySpecificationItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem userSpecificationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( !generalizationWordItem.hasJustification( userSpecificationItem, null, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&
				// Ask a specification substitution question
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askSpanishQuestionAboutRelationWord++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askSpanishQuestionAboutRelationWord++;
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByKnowledge( SpecificationItem correctedSpecificationItem, WordItem generalizationWordItem )
		{
		int correctedSpecificationCollectionNr;
		JustificationItem attachedJustificationItem;
		JustificationItem previousJustificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		WordItem correctedSpecificationWordItem;
		WordItem oppositeSpecificationWordItem;

		if( correctedSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given corrected specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( correctedSpecificationWordItem = correctedSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The corrected specification item has no specification word" );

		correctedSpecificationCollectionNr = correctedSpecificationItem.specificationCollectionNr();

		// Negative specification
		if( ( obsoleteSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, correctedSpecificationWordItem ) ) != null )
			{
			// Write invalid negative suggestive assumption by knowledge
			if( generalizationWordItem.writeUpdatedSpecification( false, true, false, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid negative suggestive assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Replace invalid negative suggestive assumption by knowledge
			if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, correctedSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid negative suggestive assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		if( ( oppositeSpecificationWordItem = correctedSpecificationWordItem.collectionWordItem( correctedSpecificationCollectionNr ) ) != null &&
		( obsoleteSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, oppositeSpecificationWordItem ) ) != null )
			{
			// Write invalid opposite suggestive assumption by knowledge
			if( generalizationWordItem.writeUpdatedSpecification( false, true, false, false, false, false, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid opposite assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Replace invalid opposite suggestive assumption by knowledge
			if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, correctedSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid opposite assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		if( correctedSpecificationItem.hasNonCompoundSpecificationCollection() &&
		( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			// Do for all non-question specifications
			do	{
				if( currentSpecificationItem.isOlderItem() &&
				( previousJustificationItem = currentSpecificationItem.firstJustificationItem() ) != null &&
				currentSpecificationItem.hasCurrentCreationSentenceNr() )
					{
					while( ( attachedJustificationItem = previousJustificationItem.attachedJustificationItem() ) != null &&
					// Remove justification with replaced primary specification
					!attachedJustificationItem.isReplacedPrimarySpecification() &&
					// Remove corrupted justification
					attachedJustificationItem.primarySpecificationWordItem() == attachedJustificationItem.secondaryGeneralizationWordItem() )
						previousJustificationItem = attachedJustificationItem;

					// Found justification with replaced primary specification
					if( attachedJustificationItem != null )
						{
						if( previousJustificationItem.isOlderPrimarySpecification() )
							{
							// Write invalid negative suggestive assumption by knowledge
							if( generalizationWordItem.writeUpdatedSpecification( false, true, false, false, false, true, false, currentSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid negative suggestive assumption with older primary specification about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

							// Replace invalid negative suggestive assumption by knowledge
							if( generalizationWordItem.replaceOrDeleteSpecification( currentSpecificationItem, correctedSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid negative suggestive assumption with older primary specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							// Remove justification with replaced primary specification
							if( previousJustificationItem.changeAttachedJustification( attachedJustificationItem.isOlderItem() ? null : attachedJustificationItem.attachedJustificationItem() ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to remove a justification with replaced primary specification" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		int obsoleteSpecificationCollectionNr;
		SpecificationItem obsoleteSpecificationItem;
		WordItem obsoleteSpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Replace all invalid suggestive assumptions by opposite suggestive question
		if( ( obsoleteSpecificationItem = generalizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( false ) ) != null )
			{
			do	{
				if( ( obsoleteSpecificationCollectionNr = obsoleteSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( obsoleteSpecificationWordItem = obsoleteSpecificationItem.specificationWordItem() ) != null &&
				obsoleteSpecificationWordItem.isExclusiveCollection( obsoleteSpecificationCollectionNr ) )
					{
					// Write invalid suggestive assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem.writeUpdatedSpecification( false, false, true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid suggestive assumption by opposite suggestive question (obsolete) in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					// Replace invalid suggestive assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid suggestive assumption by opposite suggestive question (obsolete) in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					obsoleteSpecificationItem = generalizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( false );
					}
				else
					obsoleteSpecificationItem = obsoleteSpecificationItem.nextOlderNonPossessiveNonQuestionSpecificationItem();
				}
			while( obsoleteSpecificationItem != null );
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem oldSuggestiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is no assignment" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.suggestiveQuestionAssumptionSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, oldSuggestiveQuestionAssumptionAssignmentItem, null, null, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != null )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem.firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add first old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the first old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );

			if( ( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add first new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdNewSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the first new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );
			}

		if( secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != null )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem.firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add second old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the second old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );

			if( ( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add second new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdNewSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the second new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeAdjectiveConclusions( boolean isArchivedAssignment, boolean isSpanishCurrentLanguage, short generalizationWordTypeNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem )
		{
		int specificationCollectionNr;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem collectionWordItem;
		WordItem commonWordItem;
		WordItem currentGeneralizationWordItem;
		WordItem primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem.isAdjectiveSpecification() )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no adjective specification" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = primarySpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( secondarySpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, primarySpecificationWordItem ) ) != null &&

					// Specification doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == null ||

					// User specification doesn't exist yet
					( existingSpecificationItem.isSelfGeneratedSpecification() &&
					// Justification doesn't exist yet in specification
					!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem, null ) ) ) )
					{
					// Draw negative adjective conclusion
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

					if( !isSpanishCurrentLanguage &&
					( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
					( specificationCollectionNr = createdOrFoundSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					( collectionWordItem = currentGeneralizationWordItem.collectionWordItem( specificationCollectionNr ) ) != null &&
					( commonWordItem = currentGeneralizationWordItem.commonWordItem( specificationCollectionNr ) ) != null &&
					( foundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, commonWordItem ) ) != null &&
					foundSpecificationItem.hasRelationWord() &&
					foundSpecificationItem.isOlderItem() &&
					foundSpecificationItem.hasSpecificationCollection() &&
					// Positive specification doesn't exist
					generalizationWordItem.firstSpecificationItem( false, false, false, collectionWordItem ) == null &&
					( additionalDefinitionSpecificationItem = commonWordItem.firstExclusiveSpecificationItem( currentGeneralizationWordItem ) ) != null &&
					// Make indirectly answered question assumption
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, additionalDefinitionSpecificationItem, createdOrFoundSpecificationItem, null, generalizationWordItem, collectionWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + collectionWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawNegativeAdjectiveConclusions++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawNegativeAdjectiveConclusions++;
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean isPrimarySameAsSecondarySpecification;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		int additionalDefinitionSpecificationCollectionNr;
		SpecificationItem additionalProperNounSpecificationItem = null;
		SpecificationItem negativeConclusionPrimarySpecificationItem = null;
		SpecificationItem negativeConclusionAdditionalDefinitionSpecificationItem = null;
		SpecificationItem negativeConclusionSecondarySpecificationItem = null;
		SpecificationItem negativeDefinitionSpecificationItem = null;
		WordItem additionalDefinitionSpecificationWordItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given additional definition specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given additional definition specification item has no specification word item" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
		isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

		additionalDefinitionSpecificationCollectionNr = additionalDefinitionSpecificationItem.specificationCollectionNr();

		if( ( negativeDefinitionSpecificationItem = secondarySpecificationWordItem.firstSpecificationItem( true, false, false, additionalDefinitionSpecificationWordItem ) ) != null &&
		!isPrimarySpecificationWordSpanishAmbiguous )
			{
			// Test file: "Simple family definition - John and Anna - Paul, Joe and Laura"
			negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
			negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
			}
		else
			{
			if( isPrimarySpecificationWordSpanishAmbiguous &&
			negativeDefinitionSpecificationItem != null )
				{
				// Typical for Spanish
				// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
				negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
				negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
				}
			else
				{
				negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
				negativeConclusionAdditionalDefinitionSpecificationItem = additionalDefinitionSpecificationItem;
				negativeConclusionSecondarySpecificationItem = ( isPrimarySameAsSecondarySpecification ? additionalProperNounSpecificationItem :
																										secondarySpecificationItem );

				if( negativeConclusionSecondarySpecificationItem == null )
					// Typical for Spanish
					negativeConclusionSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, additionalDefinitionSpecificationCollectionNr, null );
				}
			}

		if( negativeConclusionSecondarySpecificationItem == null )
			{
			// Typical for Spanish
			// Spanish test file: "razonamiento\Reto científico"
			if( generalizationWordItem.firstSpecificationItem( false, false, true, additionalDefinitionSpecificationWordItem ) == null &&
			// Ask a specification substitution question
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, null, negativeConclusionAdditionalDefinitionSpecificationItem, null, generalizationWordItem, additionalDefinitionSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + additionalDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
			}
		else
			{
			// Draw negative conclusion
			if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAdditionalDefinitionSpecificationItem, negativeConclusionSecondarySpecificationItem, null, generalizationWordItem, additionalDefinitionSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + additionalDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeDefinitionConclusions( boolean isArchivedAssignment, short generalizationWordTypeNr, short negativeSpecificationWordTypeNr, SpecificationItem negativeDefinitionSpecificationItem, WordItem generalizationWordItem, WordItem negativeSpecificationWordItem, WordItem specificationWordItem )
		{
		boolean isProperNounArchivedAssignment;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem positiveAdjectiveDefinitionSpecificationItem;
		SpecificationItem positiveSpecificationItem;
		SpecificationItem negativeSpecificationItem;
		SpecificationItem negativeAdjectiveDefinitionSpecificationItem;
		SpecificationItem secondaryAdjectiveDefinitionSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem currentSpecificationWordItem;
		WordItem reversedNegativeSpecificationWordItem;

		if( negativeDefinitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given negative definition specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( negativeSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given negative specification word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( negativeSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE )
			{
			// Negative specification is adjective
			if( ( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) != null )
				{
				// Do for all specification words
				do	{
					if( currentSpecificationWordItem != generalizationWordItem &&
					currentSpecificationWordItem != specificationWordItem &&
					currentSpecificationWordItem.isSingularNounWord() &&
					( positiveAdjectiveDefinitionSpecificationItem = currentSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != null &&
					( negativeAdjectiveDefinitionSpecificationItem = generalizationWordItem.firstNonExclusiveSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) != null )
						{
						// Negative specification doesn't exist yet
						if( ( ( existingSpecificationItem = currentSpecificationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) ) == null ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveAdjectiveDefinitionSpecificationItem, negativeAdjectiveDefinitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null ) ) ) &&

						// Draw reverse negative adjective conclusion (on generalization word)
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null, currentSpecificationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reverse negative adjective conclusion (on generalization word) about generalization word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						// Refresh negative adjective definition specification
						if( ( negativeAdjectiveDefinitionSpecificationItem = specificationWordItem.firstNonExclusiveSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) != null &&
						// Negative specification doesn't exist yet
						currentSpecificationWordItem.firstSpecificationItem( true, false, false, specificationWordItem ) == null &&
						// Draw reverse negative adjective conclusion (on specification word)
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null, currentSpecificationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reverse negative adjective conclusion (on specification word) about generalization word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
						}
					}
				while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
				}
			}
		else	// Negative specification is non-adjective
			{
			// Test file: "reasoning\Scientific challenge"
			if( !negativeDefinitionSpecificationItem.hasSpecificationCollection() &&
			( positiveAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&
			( secondaryAdjectiveDefinitionSpecificationItem = generalizationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&
			// Draw simple negative adjective conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, null, secondaryAdjectiveDefinitionSpecificationItem, null, negativeSpecificationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative adjective conclusion about generalization word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			if( ( positiveAdjectiveDefinitionSpecificationItem = generalizationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&
			( negativeAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem.firstAdjectiveSpecificationItem( true, false ) ) != null &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == null ||

				// Justification doesn't exist yet in specification
				( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveAdjectiveDefinitionSpecificationItem, negativeAdjectiveDefinitionSpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null ) ) ) &&

			// Draw another simple negative adjective conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw another simple negative adjective conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		if( ( definitionSpecificationItem = negativeSpecificationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) ) == null )
			definitionSpecificationItem = negativeDefinitionSpecificationItem;

		// Get first generalization specification proper noun word
		if( ( currentGeneralizationItem = generalizationWordItem.firstProperNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification proper noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.addError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( positiveSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, negativeSpecificationWordItem ) ) != null &&

					( ( existingSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationWordItem ) ) == null ||

					// Justification doesn't exist yet in specification
					( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, definitionSpecificationItem, null ) ) ) )
					{
					isProperNounArchivedAssignment = positiveSpecificationItem.isArchivedAssignment();

					// Draw negative conclusion
					if( adminItem_.addSelfGeneratedSpecification( false, isProperNounArchivedAssignment, isProperNounArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, generalizationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, definitionSpecificationItem, null, currentGeneralizationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextProperNounSpecificationGeneralizationItem() ) != null );
			}

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = generalizationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				currentGeneralizationWordItem != negativeSpecificationWordItem )
					{
					if( ( positiveSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != null &&
					!positiveSpecificationItem.isExclusiveSpecification() &&

						// Specification doesn't exist yet
						( ( existingSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) ) == null ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, definitionSpecificationItem, null ) ) ) &&

					// Draw negative adjective conclusion
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, definitionSpecificationItem, null, currentGeneralizationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( ( reversedNegativeSpecificationWordItem = negativeSpecificationWordItem.collectionWordItem( definitionSpecificationItem.specificationCollectionNr() ) ) != null &&
					( negativeSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, reversedNegativeSpecificationWordItem ) ) != null &&
					( positiveSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, reversedNegativeSpecificationWordItem ) ) != null &&

						// Specification doesn't exist yet
						( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == null ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, negativeSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, negativeSpecificationItem, null ) ) ) &&

					// Draw negative adjective conclusion
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, negativeSpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeExclusiveConclusions( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem negativePrimarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem exclusiveSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( negativePrimarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given negative primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( exclusiveSpecificationItem = specificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				// Test file: "reasoning\Knowledge of organisms (1)"
				if( ( secondarySpecificationWordItem = exclusiveSpecificationItem.specificationWordItem() ) != null &&
				// Specification doesn't exist yet
				generalizationWordItem.firstSpecificationItem( true, false, false, secondarySpecificationWordItem ) == null &&
				// Draw exclusive negative compound conclusion
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, exclusiveSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, negativePrimarySpecificationItem, null, exclusiveSpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw an exclusive negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSpecificationItem = exclusiveSpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawOnlyOptionLeftProperNounConclusion( SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCreatedSpecification = false;
		boolean isArchivedAssignment;
		boolean hasFoundMoreMatches = false;
		int currentSpecificationCollectionNr;
		SpecificationItem currentSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem foundDefinitionSpecificationItem = null;
		SpecificationItem negativeSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem foundDefinitionSpecificationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			do	{
				if( currentSpecificationItem.isExclusiveSpecification() &&
				( currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				( negativeSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != null &&
				( definitionSpecificationItem = specificationWordItem.firstExclusiveSpecificationItem( false ) ) != null )
					{
					do	{
						if( definitionSpecificationItem.specificationWordItem() != currentSpecificationWordItem &&
						// Positive specification doesn't exist yet
						generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, null ) == null &&
						definitionSpecificationItem.specificationCollectionNr() == currentSpecificationCollectionNr )
							{
							if( foundDefinitionSpecificationItem == null )
								foundDefinitionSpecificationItem = definitionSpecificationItem;
							else
								hasFoundMoreMatches = true;
							}
						}
					while( !hasFoundMoreMatches &&
					( definitionSpecificationItem = definitionSpecificationItem.nextExclusiveSpecificationItem( false ) ) != null );

					if( !hasFoundMoreMatches &&
					foundDefinitionSpecificationItem != null &&
					( foundDefinitionSpecificationWordItem = foundDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

						// Draw an only option left conclusion
						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, primarySpecificationItem.generalizationWordTypeNr(), primarySpecificationItem.specificationWordTypeNr(), primarySpecificationItem.relationWordTypeNr(), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundDefinitionSpecificationItem, negativeSpecificationItem, null, generalizationWordItem, foundDefinitionSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						hasCreatedSpecification = true;
						}
					}
				}
			while( !hasCreatedSpecification &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawOnlyOptionLeftProperNounConclusion++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawOnlyOptionLeftProperNounConclusion++;
			}

		return Constants.RESULT_OK;
		}

	private byte drawProperNounPartOfConclusions( boolean isSpanishCurrentLanguage, SpecificationItem primarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasExistingSpecificationCurrentCreationSentenceNr;
		boolean hasPrimarySpecificationRelationWord;
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationRelationWord;
		boolean isOlderPrimarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessivePrimarySpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimarySpecificationArchivedAssignment;
		boolean isUserGeneralizationWord;
		boolean isUserRelationWord;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem spanishMasculineSpecificationWordItem = null;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The specification word of the given user specification item is undefined" );

		if( ( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();
			isOlderUserSpecification = userSpecificationItem.isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();

			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			hasPrimarySpecificationRelationWord = primarySpecificationItem.hasRelationWord();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();
			isPrimarySpecificationArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

			if( hasPrimarySpecificationRelationWord &&
			isOlderPrimarySpecification &&
			!isPossessiveUserSpecification &&
			primarySpecificationItem.hasCurrentCreationSentenceNr() &&
			primarySpecificationItem.isSelfGeneratedAssumption() &&
			// Recalculate assumption level of primary specification
			primarySpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the primary specification" );

			// Typical for Spanish
			if( isSpanishCurrentLanguage &&
			specificationWordItem.isFeminineWord() &&
			!userSpecificationItem.isQuestion() )
				spanishMasculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem();

			// Do for all possessive noun words
			do	{
				if( ( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPartOfSpecificationItem( specificationWordItem ) ) == null &&
				spanishMasculineSpecificationWordItem != null )
					partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPartOfSpecificationItem( spanishMasculineSpecificationWordItem );

				if( partOfDefinitionSpecificationItem != null )
					{
					if( ( createdOrFoundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentPossessiveNounWordItem ) ) == null ||

					// Justification doesn't exist yet in word
					( !generalizationWordItem.hasJustification( primarySpecificationItem, null, partOfDefinitionSpecificationItem ) &&
					// Justification has at least the same assumption level
					createdOrFoundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null ) ) )
						{
						hasExistingSpecificationCurrentCreationSentenceNr = ( createdOrFoundSpecificationItem == null ? false : createdOrFoundSpecificationItem.hasCurrentCreationSentenceNr() );

						if( ( ( !isOlderPrimarySpecification &&

							// Typical for Spanish
							( isPossessivePrimarySpecification ||
							createdOrFoundSpecificationItem == null ||

							( !isSpanishCurrentLanguage &&
							hasUserSpecificationRelationWord ) ) ) ||

						( !isOlderUserSpecification &&

							( ( !isUserRelationWord &&
							hasUserSpecificationRelationWord ) ||

							// Non-possessive user specification
							( !isPossessiveUserSpecification &&

								( hasUserSpecificationRelationWord ||
								!isUserGeneralizationWord ||

								( ( hasPrimarySpecificationRelationWord &&
								!hasExistingSpecificationCurrentCreationSentenceNr ) ||

								// Not Spanish
								( !isSpanishCurrentLanguage &&
								// Test file: "No conflict on multiple related specifications"
								hasExistingSpecificationCurrentCreationSentenceNr ) ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&
								!hasPrimarySpecificationRelationWord ) ) ) ) ||

							// Possessive user specification
							// Typical for Spanish
							// Spanish test files: "razonamiento\familia\Complejo (2)",
							//						"razonamiento\familia\Complejo (17)"
							( isPossessiveUserSpecification &&
							hasUserSpecificationCompoundCollection &&
							primarySpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == null ) ) ) ) )
							{
							// Draw 'part of' generalization conclusion
							// See Block 1 and Block 2 of the included challenge document,
							// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

							createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
							}
						}

					if( createdOrFoundSpecificationItem != null &&
					!isPossessivePrimarySpecification &&

						// Non-possessive user specification
						// Test file: "reasoning\family\Laura - John and Anna - Joe"
						( ( !isPossessiveUserSpecification &&
						!hasPrimarySpecificationRelationWord &&
						!isSpanishCurrentLanguage ) ||

						// Possessive user specification
						( isPossessiveUserSpecification &&
						userSpecificationWordItem == specificationWordItem ) ) )
						{
						selectedPrimarySpecificationItem = ( isPossessiveUserSpecification ? userSpecificationItem :
																							userGeneralizationWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) );

						if( selectedPrimarySpecificationItem != null &&
						// Justification doesn't exist yet in specification
						!createdOrFoundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, partOfDefinitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						createdOrFoundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, null, partOfDefinitionSpecificationItem, null ) &&
						// Add part-of justification to part-of specification
						createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, null, partOfDefinitionSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a part-of justification to a part-of specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawProperNounPartOfConclusions++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawProperNounPartOfConclusions++;
			}

		return Constants.RESULT_OK;
		}

	private byte drawPossessiveReversibleConclusions( boolean isActiveUserAssignment, boolean isSpanishCurrentLanguage, SpecificationItem givenSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGivenNonCompoundSpecificationCollection;
		boolean hasUserNonCompoundSpecificationCollection;
		boolean isAssignment;
		boolean isExistingUserSpecification = false;
		boolean isGivenArchivedAssignment;
		boolean isGivenExclusiveGeneralizationAssignment;
		boolean isGivenExclusiveSpecification;
		boolean isGivenOlderSpecification;
		boolean isGivenSelfGeneratedAssumption;
		boolean isGivenSelfGeneratedSpecification;
		boolean isGivenUserAssignment;
		boolean isOlderAlternativeSpecification;
		boolean isOlderExistingSpecification = false;
		boolean isOlderUserSpecification;
		boolean isSpanishSpecificationFemaleWord = false;
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUniqueUserRelation;
		boolean isUserAssignment;
		boolean isUserGeneralizationWord;
		boolean isGivenUserSpecification;
		short givenRelationWordTypeNr;
		short givenSpecificationWordTypeNr;
		int createdRelationCollectionNr;
		int givenSpecificationCollectionNr;
		int spanishAmbiguousSpecificationCollectionNr;
		JustificationItem firstJustificationItem;
		JustificationItem foundJustificationItem;
		SpecificationItem collectedSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem additionalSpecificationItem;
		SpecificationItem additionalUserSpecificationItem;
		SpecificationItem alternativeSpecificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentExistingSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem firstUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem possessiveDefinitionSpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem spanishMasculineSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem additionalGeneralizationWordItem;
		WordItem commonWordItem;
		WordItem oppositePossessiveDefinitionSpecificationWordItem = null;
		WordItem partOfSpecificationWordItem;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveNounWordItem;
		WordItem spanishMasculineSpecificationWordItem = null;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		CollectionResultType collectionResult;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( givenSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			hasUserNonCompoundSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			isUserAssignment = userSpecificationItem.isAssignment();

			hasGivenNonCompoundSpecificationCollection = givenSpecificationItem.hasNonCompoundSpecificationCollection();
			isGivenArchivedAssignment = givenSpecificationItem.isArchivedAssignment();
			isGivenExclusiveSpecification = givenSpecificationItem.isExclusiveSpecification();
			isGivenOlderSpecification = givenSpecificationItem.isOlderItem();
			isGivenSelfGeneratedAssumption = givenSpecificationItem.isSelfGeneratedAssumption();
			isGivenSelfGeneratedSpecification = givenSpecificationItem.isSelfGeneratedSpecification();
			isGivenUserAssignment = givenSpecificationItem.isUserAssignment();
			isGivenUserSpecification = givenSpecificationItem.isUserSpecification();
			givenRelationWordTypeNr = givenSpecificationItem.relationWordTypeNr();
			givenSpecificationWordTypeNr = givenSpecificationItem.specificationWordTypeNr();
			givenSpecificationCollectionNr = givenSpecificationItem.specificationCollectionNr();

			isGivenExclusiveGeneralizationAssignment = ( isGivenUserAssignment &&
														givenSpecificationItem.isExclusiveGeneralization() );

			// Compound generalization word
			if( ( possessiveNounWordItem = specificationWordItem.compoundGeneralizationWordItem( givenSpecificationCollectionNr ) ) != null )
				{
				if( specificationWordItem != userSpecificationWordItem &&
				possessiveDefinitionSpecificationItem_ != null &&
				oppositePossessiveDefinitionSpecificationItem_ != null )
					{
					possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem_;
					possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem();

					oppositePossessiveDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
					oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_.specificationWordItem();
					}
				else
					{
					// Find possessive specifications
					if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"" + possessiveNounWordItem.anyWordTypeString() + "\"" );

					if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != null )
						possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem();

					if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != null )
						oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem.specificationWordItem();
					}
				}

			// Typical for Spanish
			if( isSpanishCurrentLanguage )
				{
				isSpecificationWordSpanishAmbiguous = givenSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isSpanishSpecificationFemaleWord = specificationWordItem.isFeminineWord();

				if( isSpanishSpecificationFemaleWord )
					spanishMasculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem();
				}

			if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( true, true, false, true, specificationWordItem, generalizationWordItem ) ) != null )
				{
				isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
				isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
				}

			if( isGivenExclusiveSpecification )
				{
				// Specification doesn't exist yet
				if( existingSpecificationItem == null &&
				// Draw possessive reversible conclusion
				adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, givenRelationWordTypeNr, givenSpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, relationWordItem, specificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with current context word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}
			else
				{
				firstUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, specificationWordItem );

				isOlderUserSpecification = ( existingSpecificationItem == null &&
											firstUserSpecificationItem != null );

				isOlderAlternativeSpecification = ( ( alternativeSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, false, specificationWordItem, relationWordItem ) ) != null &&
													alternativeSpecificationItem.isOlderItem() );

				if( isOlderUserSpecification ||
				firstUserSpecificationItem == null ||

				// Typical for Spanish
				// Spanish test file: "razonamiento\familia\Luisa - José - Ana"
				( isSpanishCurrentLanguage &&
				!isOlderAlternativeSpecification &&
				givenSpecificationItem != alternativeSpecificationItem ) )
					{
					currentExistingSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, true, true, Constants.NO_QUESTION_PARAMETER, specificationWordItem.nonCompoundCollectionNr(), specificationWordItem );

					if( existingSpecificationItem == null ||

					( givenSpecificationItem == alternativeSpecificationItem &&
					// Justification doesn't exist yet in specification
					!existingSpecificationItem.hasPrimarySpecificationJustification( givenSpecificationItem ) &&

						( !isSpanishCurrentLanguage ||
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null, null ) ) &&

						( currentExistingSpecificationItem == null ||
						// Justification has at least the same assumption level
						currentExistingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null, null ) ||

						( isOlderExistingSpecification &&

							( relationWordItem.hasCurrentlyConfirmedSpecification() ||
							existingSpecificationItem.hasRelationCollectionCurrentlyBeenUpdated() ) ) ) ) )
						{
						if( !isSpecificationWordSpanishAmbiguous ||

						// Typical for Spanish
						( !isGivenUserAssignment &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							isGivenUserSpecification ||
							currentExistingSpecificationItem != null ) ) ||

						( isGivenUserSpecification &&
						!hasGeneralizationWordCurrentlyConfirmedSpecification ) )
							{
							isAssignment = ( isGivenArchivedAssignment ||
											isGivenExclusiveGeneralizationAssignment );

							isUniqueUserRelation = ( isGivenArchivedAssignment &&
													isGivenUserAssignment &&
													adminItem_.isUniqueUserRelation() );

							createdSpecificationItem = null;
							foundSpecificationItem = null;
							createdOrFoundSpecificationItem = null;

							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							existingSpecificationItem == null &&
							hasUserNonCompoundSpecificationCollection &&
							isGivenOlderSpecification )
								{
								if( ( secondarySpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
								// Draw Spanish feminine exclusive specification substitution assumption
								adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, givenRelationWordTypeNr, givenSpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, secondarySpecificationItem, null, relationWordItem, specificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to a Spanish feminine exclusive specification substitution assumption about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;

								// Typical for Spanish
								if( isSpecificationWordSpanishAmbiguous &&
								existingSpecificationItem != null )
									{
									spanishAmbiguousSpecificationCollectionNr = existingSpecificationItem.specificationCollectionNr();

									if( !existingSpecificationItem.hasCurrentCreationSentenceNr() &&
									existingSpecificationItem.wasHiddenSpanishSpecification() )
										{
										// Write Spanish assumption that is not hidden anymore
										if( relationWordItem.writeUpdatedSpecification( false, false, false, false, false, false, true, existingSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"" + relationWordItem.anyWordTypeString() + "\"" );

										// Add reversible justifications after Spanish specification not hidden anymore
										if( addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore( existingSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to reversible justifications after Spanish specification that is not hidden anymore in word \"" + relationWordItem.anyWordTypeString() + "\"" );
										}
									}

								// Draw reversible conclusion
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isGivenArchivedAssignment, false, false, false, false, false, true, isUniqueUserRelation, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, givenRelationWordTypeNr, givenSpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
								createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
								}

							if( !isOlderUserSpecification &&
							!isSpanishCurrentLanguage &&
							isGivenSelfGeneratedSpecification &&
							createdOrFoundSpecificationItem != null &&
							createdOrFoundSpecificationItem.hasNonCompoundSpecificationCollection() &&

								( hasUserNonCompoundSpecificationCollection ||
								isActiveUserAssignment ||
								isGivenOlderSpecification ||

								( foundSpecificationItem != null &&
								!foundSpecificationItem.isOlderItem() ) ||

								givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != null ) &&

							// Justification doesn't exist yet in specification
							!givenSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
							// Justification has at least the same assumption level
							givenSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null, null ) )
								{
								if( isGivenArchivedAssignment ||
								createdSpecificationItem == null ||

								// Not user generalization word
								( !isUserGeneralizationWord &&
								!isActiveUserAssignment ) ||

								// User generalization word
								( isUserGeneralizationWord &&

									( foundSpecificationItem == null ||

									( isActiveUserAssignment &&
									givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != null ) ) ) )
									{
									// Test file: "reasoning\family\Laura - Paul - John and Anna"
									if( isActiveUserAssignment &&
									createdSpecificationItem == null &&
									givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != null &&
									givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != null &&
									( firstJustificationItem = givenSpecificationItem.firstJustificationItem() ) != null &&
									firstJustificationItem.isOlderItem() &&
									firstJustificationItem.isReversibleAssumptionOrConclusion() )
										givenSpecificationItem.clearStoredSentenceStringBuffer();

									// Draw reversible conclusion
									if( adminItem_.addSelfGeneratedSpecification( false, isGivenArchivedAssignment, isGivenArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, givenSpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}

								if( ( commonWordItem = specificationWordItem.commonWordItem( createdOrFoundSpecificationItem.specificationCollectionNr() ) ) != null &&
								( primarySpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
								// Justification doesn't exist yet in specification
								!createdOrFoundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, userSpecificationItem ) &&
								// Justification has at least the same assumption level
								createdOrFoundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, userSpecificationItem, null ) &&
								// Add user opposite possessive conclusion justification to created or found specification
								createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, userSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a user opposite possessive conclusion justification to the created or found specification" );

								if( createdSpecificationItem == null &&
								!createdOrFoundSpecificationItem.isOlderItem() )
									{
									if( ( createdRelationCollectionNr = createdOrFoundSpecificationItem.relationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
									( partOfSpecificationItem = generalizationWordItem.firstPartOfSpecificationItem() ) != null &&
									( partOfSpecificationWordItem = partOfSpecificationItem.specificationWordItem() ) != null &&
									( partOfDefinitionSpecificationItem = partOfSpecificationWordItem.firstPartOfSpecificationItem( specificationWordItem ) ) != null &&
									// Add part-of justification to part-of specification in relation collection words
									addPartOfJustificationToRelationCollectionWords( createdRelationCollectionNr, createdOrFoundSpecificationItem, partOfDefinitionSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a part-of justification to a part-of specification in relation collection words" );

									// Test file: "reasoning\family\John - Anna (before family definition)"
									if( isActiveUserAssignment &&
									!isGivenOlderSpecification &&
									isUserGeneralizationWord &&
									( additionalUserSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, specificationWordItem, null ) ) != null )
										{
										// Test file: "reasoning\family\John - Anna (before family definition)"
										// Justification doesn't exist yet in specification
										if( !givenSpecificationItem.hasPrimarySpecificationJustification( additionalUserSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, null, null, null ) &&
										// Add user reversible justification to current secondary specification
										givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a user reversible justification to the current secondary specification" );

										// Test file: "reasoning\family\John - Anna (before family definition)"
										if( ( additionalGeneralizationWordItem = additionalUserSpecificationItem.relationWordItem() ) != null &&
										( additionalSpecificationItem = additionalGeneralizationWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
										// Justification doesn't exist yet in specification
										!givenSpecificationItem.hasPrimarySpecificationJustification( additionalSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, null, null, null ) &&
										// Add assumption reversible justification to current secondary specification
										givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add an assumption reversible justification to the current secondary specification" );
										}
									}
								}

							// Not created
							if( createdSpecificationItem == null )
								{
								// Test file: "reasoning\family\John - Anna (before family definition)"
								if( isGivenOlderSpecification &&
								isUserAssignment &&
								!isUserGeneralizationWord &&
								existingSpecificationItem == null &&
								foundSpecificationItem != null &&
								!foundSpecificationItem.isOlderItem() &&
								// Recalculate assumption level of found specification
								foundSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );
								}
							else
								{
								// Created, not found
								if( foundSpecificationItem == null )
									{
									if( isUserGeneralizationWord )
										{
										if( specificationWordItem == userSpecificationWordItem )
											{
											if( possessiveDefinitionSpecificationWordItem != null &&
											( assumptionSpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) ) != null &&
											assumptionSpecificationItem.hasRelationWord() )
												{
												if( ( secondarySpecificationItem = specificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != null &&
												// Justification doesn't exist yet in specification
												!assumptionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( createdSpecificationItem, secondarySpecificationItem ) &&
												// Justification has at least the same assumption level
												assumptionSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, secondarySpecificationItem, null ) &&
												// Add specification substitution justification to additional specification
												assumptionSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );

												if( possessiveDefinitionSpecificationItem != null &&
												oppositePossessiveDefinitionSpecificationWordItem != null &&
												( additionalSpecificationItem = relationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
												!additionalSpecificationItem.isHiddenSpanishSpecification() )
													{
													assumptionSpecificationItem = assumptionSpecificationItem.updatedSpecificationItem();

													// Justification doesn't exist yet in specification
													if( !additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, assumptionSpecificationItem ) &&
													// Justification has at least the same assumption level
													additionalSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, assumptionSpecificationItem, null ) &&
													// Add opposite possessive conclusion justification to additional specification
													additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, assumptionSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to an additional specification" );
													}
												}
											}
										else
											{
											if( ( primarySpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
											// Skip archived user assignment
											primarySpecificationItem.hasRelationWord() &&
											( secondarySpecificationItem = userSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
											// Justification doesn't exist yet in specification
											!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											createdSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem, null ) &&
											// Add specification substitution justification to created specification
											createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the created specification" );
											}
										}

									if( isSpanishCurrentLanguage )
										{
										// Typical for Spanish
										if( ( obsoleteSpecificationItem = relationWordItem.noRelationWordSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : specificationWordItem ) ) ) == null )
											{
											if( !isUserAssignment &&
											( secondarySpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
											// Justification doesn't exist yet in specification
											!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											createdSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, null, secondarySpecificationItem, null ) &&
											// Add exclusive user specification substitution assumption justification to created specification
											createdSpecificationItem.addJustificationToSpecification( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add an exclusive user specification substitution assumption justification to the created specification" );
											}
										else
											{
											// Write an assumption without relation word
											if( isGivenUserSpecification &&
											relationWordItem.writeUpdatedSpecification( true, false, false, false, true, false, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + relationWordItem.anyWordTypeString() + "\"" );

											// Replace an assumption without relation word
											if( relationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
											}
										}
									}
								else	// Created and found
									{
									if( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
									isUserGeneralizationWord &&
									!isOlderUserSpecification &&
									( foundJustificationItem = givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) != null &&
									( primarySpecificationItem = foundJustificationItem.primarySpecificationItem() ) != null &&
									( secondarySpecificationItem = foundJustificationItem.secondarySpecificationItem() ) != null &&
									// Justification doesn't exist yet in specification
									!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
									// Justification has at least the same assumption level
									createdSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, secondarySpecificationItem, null ) &&
									// Add opposite possessive conclusion justification to created specification
									createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the created specification" );
									}

								if( !isGivenExclusiveGeneralizationAssignment &&
								isOlderUserSpecification &&
								foundSpecificationItem != null )
									{
									// Test file: "reasoning\family\Complex (19 - strange)"
									// Add reversible justification
									if( createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, " failed to add a reversible justification to the created specification" );

									// Update specification of justifications of involved words, except for current context word
									if( relationWordItem.updateSpecificationOfJustificationsOfInvolvedWords( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( possessiveDefinitionSpecificationItem == null &&
									( commonWordItem = specificationWordItem.commonWordItem( givenSpecificationCollectionNr ) ) != null )
										possessiveDefinitionSpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, specificationWordItem );

									if( oppositePossessiveDefinitionSpecificationWordItem == null &&
									possessiveDefinitionSpecificationItem != null )
										oppositePossessiveDefinitionSpecificationWordItem = specificationWordItem.collectionWordItem( givenSpecificationCollectionNr );

									if( possessiveDefinitionSpecificationItem != null &&
									oppositePossessiveDefinitionSpecificationWordItem != null )
										{
										// Test file: "reasoning\family\Complex (19 - strange)"
										// Make an opposite possessive specification assumption
										if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationItem, null, createdSpecificationItem, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"" + relationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

										// Test file: "reasoning\family\Complex (19 - strange)"
										if( ( additionalSpecificationItem = generalizationWordItem.firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
										// Justification doesn't exist yet in specification
										!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, createdSpecificationItem ) &&
										// Justification has at least the same assumption level
										additionalSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, createdSpecificationItem, null ) &&
										// Add opposite possessive conclusion justification to additional specification
										additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, createdSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the additional specification" );
										}
									}
								}

							givenSpecificationItem = givenSpecificationItem.updatedSpecificationItem();

							if( createdOrFoundSpecificationItem != null &&
							createdOrFoundSpecificationItem.assumptionLevel() == givenSpecificationItem.assumptionLevel() &&

							( !isSpanishCurrentLanguage ||
							// Typical for Spanish
							isGivenUserSpecification ||
							givenSpecificationItem.hasOnlyOneRelationWord() ) )
								{
								commonWordItem = ( hasGivenNonCompoundSpecificationCollection ? specificationWordItem.commonWordItem( givenSpecificationCollectionNr ) :
																								specificationWordItem.compoundGeneralizationWordItem( givenSpecificationCollectionNr ) );

								if( commonWordItem != null &&
								( partOfDefinitionSpecificationItem = commonWordItem.firstPartOfSpecificationItem( specificationWordItem ) ) != null &&

									( ( partOfSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, commonWordItem ) ) == null ||

									// Justification doesn't exist yet in part-of specification
									( !partOfSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( createdOrFoundSpecificationItem, partOfDefinitionSpecificationItem ) &&
									// Justification has at least the same assumption level
									partOfSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, partOfDefinitionSpecificationItem, null ) ) ) &&

								// Draw 'part of' generalization conclusion
								// See Block 1 and Block 2 of the included challenge document,
								// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
								adminItem_.addSelfGeneratedSpecification( false, isGivenArchivedAssignment, isGivenArchivedAssignment, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						}
					else
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						!hasGivenNonCompoundSpecificationCollection &&
						isGivenOlderSpecification &&
						alternativeSpecificationItem != null &&
						givenSpecificationItem != alternativeSpecificationItem &&
						// Justification doesn't exist yet in specification
						!alternativeSpecificationItem.hasPrimarySpecificationJustification( givenSpecificationItem ) &&
						// Justification has at least the same assumption level
						alternativeSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null, null ) &&
						// Add Spanish reversible justification to alternative specification
						alternativeSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible justification to the alternative specification" );
						}
					}

				if( hasGivenNonCompoundSpecificationCollection &&
				isGivenSelfGeneratedSpecification &&
				!isSpanishCurrentLanguage &&
				alternativeSpecificationItem != null &&
				existingSpecificationItem == userSpecificationItem &&

					( isGivenSelfGeneratedAssumption ||
					!givenSpecificationItem.isOlderItem() ) &&

				// Make an opposite possessive specification assumption (based on reversible conclusion)
				makeOppositePossessiveSpecificationAssumption( isGivenArchivedAssignment, false, true, false, Constants.WORD_TYPE_PROPER_NOUN, givenSpecificationWordTypeNr, givenRelationWordTypeNr, ( isGivenSelfGeneratedAssumption ? alternativeSpecificationItem : givenSpecificationItem ), null, userSpecificationItem, relationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption during drawing of a reversible conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				if( existingSpecificationItem != null &&

				( isSpecificationWordSpanishAmbiguous ||
				// Test file: "reasoning\family\John - Anna (before family definition)"
				givenSpecificationCollectionNr == Constants.NO_COLLECTION_NR ||

				( hasGivenNonCompoundSpecificationCollection &&
				!isExistingUserSpecification &&
				isOlderExistingSpecification &&
				givenSpecificationItem == alternativeSpecificationItem &&
				!givenSpecificationItem.hasCurrentCreationSentenceNr() ) ||

				( isExistingUserSpecification &&
				givenSpecificationItem != alternativeSpecificationItem &&
				relationWordItem.hasCurrentlyConfirmedSpecification() ) ) )
					{
					// Alternative specification is same as current secondary specification
					if( ( alternativeSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, false, specificationWordItem, relationWordItem ) ) == givenSpecificationItem )
						{
						if( isGivenSelfGeneratedSpecification &&
						isOlderExistingSpecification &&
						existingSpecificationItem.hasNonCompoundSpecificationCollection() )
							{
							if( isGivenOlderSpecification &&
							// Secondary specification word is same as user specification word
							specificationWordItem == userSpecificationWordItem &&
							// Justification doesn't exist yet in specification
							!givenSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Possessive user specification doesn't exist
							relationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, specificationWordItem ) == null &&
							( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, specificationWordItem, userGeneralizationWordItem ) ) != null )
								{
								// Add reversible user justification
								if( givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

								givenSpecificationItem = givenSpecificationItem.updatedSpecificationItem();

								// Collect relation words
								if( ( collectionResult = adminItem_.collectRelationWords( isGivenExclusiveSpecification, true, givenRelationWordTypeNr, givenSpecificationWordTypeNr, relationWordItem, userGeneralizationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous relation word \"" + relationWordItem.anyWordTypeString() + "\" to relation word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdRelationCollectionNr = collectionResult.createdCollectionNr ) > Constants.NO_COLLECTION_NR &&
								// Add relation collection number to found user specification
								generalizationWordItem.changeSpecification( givenSpecificationItem.isAnsweredQuestion(), givenSpecificationItem.isEveryGeneralization(), givenSpecificationItem.generalizationCollectionNr(), givenSpecificationItem.specificationCollectionNr(), createdRelationCollectionNr, givenSpecificationItem.firstJustificationItem(), givenSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation collection number to the given specification" );

								if( ( commonWordItem = specificationWordItem.commonWordItem( givenSpecificationCollectionNr ) ) != null &&
								( possessiveDefinitionSpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
								( oppositePossessiveDefinitionSpecificationWordItem = specificationWordItem.collectionWordItem( givenSpecificationCollectionNr ) ) != null &&
								( oppositePossessiveDefinitionSpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
								( collectedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, givenSpecificationItem.specificationCollectionNr(), null ) ) != null )
									{
									if( !collectedSpecificationItem.isOlderItem() &&
									// Justification doesn't exist yet in specification
									!givenSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( oppositePossessiveDefinitionSpecificationItem, collectedSpecificationItem ) &&
									// Add collected opposite possessive justification
									givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, null, collectedSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a collected opposite possessive justification to the current secondary specification" );

									// Justification doesn't exist yet in specification
									if( !collectedSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, conclusionSpecificationItem ) &&
									// Add opposite possessive conclusion justification to collected specification
									collectedSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, conclusionSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
									}
								}
							}
						}
					else	// Alternative is different from current secondary
						{
						if( alternativeSpecificationItem != null )
							{
							if( givenSpecificationItem.hasNonCompoundSpecificationCollection() )
								{
								if( isSpanishCurrentLanguage )
									{
									// Typical for Spanish
									// Spanish test file: "razonamiento\familia\Complejo (17)"
									if( !givenSpecificationItem.isReplacedOrDeletedItem() &&
									// Replace assumption by conclusion
									generalizationWordItem.replaceOrDeleteSpecification( givenSpecificationItem, alternativeSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a Spanish assumption specification by a conclusion specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( alternativeSpecificationItem.isSelfGeneratedAssumption() &&
									( oppositePossessiveDefinitionSpecificationWordItem = specificationWordItem.collectionWordItem( givenSpecificationCollectionNr ) ) != null &&
									( foundSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( false, false, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&
									foundSpecificationItem.hasSecondarySpecificationJustification( alternativeSpecificationItem ) &&
									// Replace assumption justification by conclusion justification of involved word
									relationWordItem.updateSpecificationOfJustificationsInWord( alternativeSpecificationItem, givenSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification items in word \"" + relationWordItem.anyWordTypeString() + "\"" );
									}
								}
							else
								{
								if( isGivenOlderSpecification &&
								!isOlderExistingSpecification &&
								alternativeSpecificationItem.isOlderItem() &&
								// Replace assumption by conclusion
								generalizationWordItem.replaceOrDeleteSpecification( alternativeSpecificationItem, givenSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption specification by a conclusion specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						}
					}

				// Typical for Spanish
				if( isSpanishCurrentLanguage &&

					( isSpanishSpecificationFemaleWord ||

					( isSpecificationWordSpanishAmbiguous &&
					existingSpecificationItem != null ) ) )
					{
					if( specificationWordItem.isFeminineWord() &&
					( spanishMasculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem() ) != null )
						{
						spanishMasculineSpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem );

						// No existing specification
						if( ( existingSpecificationItem == null &&

							( spanishMasculineSpecificationItem == null ||

							( isUserGeneralizationWord &&
							!userSpecificationItem.isPossessive() ) ) ) ||

						// Existing specification
						( existingSpecificationItem != null &&
						isGivenSelfGeneratedSpecification &&
						!givenSpecificationItem.hasCurrentCreationSentenceNr() &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||

							( userSpecificationItem.hasRelationWord() &&

								( spanishMasculineSpecificationItem == null ||
								existingSpecificationItem == userSpecificationItem ||

								( isUserGeneralizationWord &&
								!userSpecificationItem.isPossessive() &&
								spanishMasculineSpecificationItem.isSelfGeneratedSpecification() ) ) ) ) ) )
							{
							// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
							givenSpecificationItem = givenSpecificationItem.updatedSpecificationItem();

							// Draw Spanish reversible conclusion
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, spanishMasculineSpecificationWordItem.nonCompoundCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, relationWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" and with masculine specification word \"" + spanishMasculineSpecificationWordItem.anyWordTypeString() + "\"" );

							if( !isUserGeneralizationWord &&
							existingSpecificationItem == userSpecificationItem &&
							( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
							// Justification doesn't exist yet in question
							!createdOrFoundSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Justification has at least the same assumption level
							createdOrFoundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null, null ) &&
							// Add opposite possessive justification
							createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the created or found specification" );
							}
						}
					else
						{
						if( existingSpecificationItem != null &&
						relationWordItem.isUserGeneralizationWord &&
						givenSpecificationItem.isSelfGeneratedConclusion() )
							{
							if( existingSpecificationItem.hasCompoundSpecificationCollection() )
								{
								// Add Spanish non-compound reversible conclusion
								if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, givenSpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, specificationWordItem.nonCompoundCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, relationWordItem, specificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish non-compound reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								// Spanish test file: "razonamiento\familia\Complejo (17)"
								if( existingSpecificationItem.isSelfGeneratedSpecification() &&
								// Justification doesn't exist yet in specification
								!existingSpecificationItem.hasPrimarySpecificationJustification( givenSpecificationItem ) &&
								// Add reversible justification to existing non-compound specification
								existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the existing non-compound specification" );
								}
							}
						}
					}

				if( isExistingUserSpecification &&
				isOlderExistingSpecification &&
				!isGivenOlderSpecification &&
				isGivenSelfGeneratedAssumption &&
				// Justification doesn't exist yet in specification
				!givenSpecificationItem.hasPrimarySpecificationJustification( existingSpecificationItem ) &&
				// Add reversible user justification
				givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, null, null ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );
				}
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawPossessiveReversibleConclusions++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawPossessiveReversibleConclusions++;
			}

		return Constants.RESULT_OK;
		}

	private byte drawSelectedReversibleConclusions( boolean isAssignment, boolean isArchivedAssignment, boolean isDrawingPossessiveReversibleConclusions, boolean isSpanishCurrentLanguage, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasCurrentSpecificationCreationSentenceNr;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		boolean hasNonCompoundCurrentSpecificationCollection;
		boolean hasUserGeneralizationWordCurrentlyConfirmedSpecification = false;
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationRelationWord = false;
		boolean isArchivedUserAssignment = false;
		boolean isCurrentSpecificationSelfGeneratedAssumption;
		boolean isCurrentSpecificationSelfGeneratedSpecification;
		boolean isCurrentSpecificationWordSpanishAmbiguous;
		boolean isExclusiveUserSpecification;
		boolean isOlderCurrentSpecification;
		boolean isPossessiveUserSpecification = false;
		boolean isSameSpecification;
		boolean isUserAssignment = false;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationWordSpanishAmbiguous;
		short generalizationWordTypeNr;
		short specificationWordTypeNr;
		short relationWordTypeNr;
		int currentRelationCollectionNr;
		int currentSpecificationCollectionNr;
		int tempRelationCollectionNr;
		int userSpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		WordItem currentCollectionWordItem;
		WordItem currentRelationWordItem;
		WordItem currentSpecificationWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) ) != null &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			specificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();

			hasUserGeneralizationWordCurrentlyConfirmedSpecification = userGeneralizationWordItem.hasCurrentlyConfirmedSpecification();
			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();
			isUserAssignment = userSpecificationItem.isAssignment();
			isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			if( isPossessiveUserSpecification )
				{
				if( isUserGeneralizationWord &&
				( currentRelationWordItem = currentSpecificationItem.relationWordItem() ) != null &&
				currentRelationWordItem.isUserRelationWord )
					{
					// Draw user reversible conclusion about relation word
					if( drawUserReversibleConclusions( isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem, userSpecificationWordItem, currentRelationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a user reversible conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and relation word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

					if( ( currentRelationCollectionNr = currentSpecificationItem.relationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					( currentCollectionWordItem = WordItem.firstCollectionWordItem( currentRelationCollectionNr ) ) != null )
						{
						// Do for all collection words
						do	{
							if( currentCollectionWordItem != currentRelationWordItem &&
							currentCollectionWordItem.isUserRelationWord &&
							// Draw user reversible conclusion about collection relation word
							drawUserReversibleConclusions( isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem, userSpecificationWordItem, currentCollectionWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a user reversible conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and collection relation word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );
							}
						while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( currentRelationCollectionNr ) ) != null );
						}
					}
				}
			else	// Non-possessive user specification
				{
				if( !isUserGeneralizationWord &&
				hasUserSpecificationRelationWord &&
				( existingSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, userSpecificationWordItem, userGeneralizationWordItem ) ) != null &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&

				( existingSpecificationItem.isFirstJustificationReversibleAssumption() ||
				existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null ||
				// Typical for Spanish
				// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
				!existingSpecificationItem.hasSpecificationCollection() ) &&

				// Draw reversible user conclusion
				adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );
				}

			// Do for all selected specifications
			do	{
				hasCurrentSpecificationCreationSentenceNr = currentSpecificationItem.hasCurrentCreationSentenceNr();
				hasNonCompoundCurrentSpecificationCollection = currentSpecificationItem.hasNonCompoundSpecificationCollection();
				isCurrentSpecificationSelfGeneratedAssumption = currentSpecificationItem.isSelfGeneratedAssumption();
				isCurrentSpecificationSelfGeneratedSpecification = currentSpecificationItem.isSelfGeneratedSpecification();
				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isOlderCurrentSpecification = currentSpecificationItem.isOlderItem();
				generalizationWordTypeNr = currentSpecificationItem.generalizationWordTypeNr();
				specificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();
				relationWordTypeNr = currentSpecificationItem.relationWordTypeNr();
				currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
				currentRelationCollectionNr = currentSpecificationItem.relationCollectionNr();
				currentRelationWordItem = currentSpecificationItem.relationWordItem();

				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The current specification item has no specification word" );

				if( currentSpecificationItem.isPossessive() )
					{
					if( isCurrentSpecificationSelfGeneratedSpecification )
						{
						if( isUserGeneralizationWord ||
						currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR )
							{
							if( !isSpanishCurrentLanguage &&
							currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&

							( !isOlderCurrentSpecification ||
							hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation ) &&

							( currentCollectionWordItem = WordItem.firstCollectionWordItem( currentRelationCollectionNr ) ) != null )
								{
								// Do for all collection words
								do	{
									if( ( existingSpecificationItem = currentCollectionWordItem.bestMatchingRelationSpecificationItem( false, false, false, false, currentSpecificationWordItem, generalizationWordItem ) ) != null &&
									existingSpecificationItem.hasCurrentCreationSentenceNr() &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimarySpecificationJustification( currentSpecificationItem ) )
									// Forcing assumption to be less certain. So, no checking on assumption level
									// Draw reversible conclusions in current relation collection word
										{
										if( isOlderCurrentSpecification )
											{
											// Test file: "reasoning\family\You have confirmed at least one relation word"
											if( currentCollectionWordItem.isUserRelationWord &&
											( obsoleteJustificationItem = currentSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentCollectionWordItem ) ) != null )
												{
												// Create justification
												if( ( justificationResult = generalizationWordItem.createJustificationItem( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, userSpecificationItem, null, null, null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to create a reversible assumption justification item to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a reversible assumption justification" );

												if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete reversible assumption justification of the current secondary specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											// Test files: "reasoning\family\Complex (18)",
											//				"reasoning\family\Complex (19 - mixed)",
											//				"reasoning\family\Complex (19 - strange)"
											// Add reversible conclusion in current relation collection word
											if( existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentSpecificationItem, null, null ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
											}
										}
									}
								while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( currentRelationCollectionNr ) ) != null );
								}
							}
						else
							{
							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							hasNonCompoundCurrentSpecificationCollection )
								{
								if( isOlderCurrentSpecification &&
								isUserAssignment &&
								// Justification doesn't exist yet in specification
								!currentSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
								// Draw Spanish non-compound reversible conclusion
								adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, relationWordTypeNr, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible specification from word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

								if( !isPossessiveUserSpecification &&
								isUserSpecificationWordSpanishAmbiguous &&
								// Specification doesn't exist yet with user specification collection
								generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, userSpecificationCollectionNr, userSpecificationWordItem ) == null )
									{
									// Draw Spanish reversible conclusion
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, isArchivedUserAssignment, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );

									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
									// Specification doesn't exist yet with without specification collection
									( obsoleteSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, Constants.NO_COLLECTION_NR, userSpecificationWordItem ) ) != null &&
									// Replace specification without specification collection with specification with specification
									// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
									generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a specification without specification collection with a specification with specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					else	// User specification
						{
						if( !hasNonCompoundCurrentSpecificationCollection &&
						!isOlderCurrentSpecification &&
						currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&
						currentRelationWordItem != null &&
						currentRelationWordItem.hasNegativeSpecification() &&

							// Possessive specification
							( ( existingSpecificationItem = currentRelationWordItem.firstPossessiveSpecificationItem() ) == null ||
							existingSpecificationItem.isOlderItem() ) &&

							// Exclusive specification, including adjectives
							( ( existingSpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem( true ) ) == null ||
							existingSpecificationItem.isOlderItem() ) &&

						// Draw specification substitution conclusion or ask a question
						drawSpecificationSubstitutionConclusionOrAskQuestion( false, false, isSpanishCurrentLanguage, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, currentRelationWordItem, currentSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else	// Non-possessive
					{
					if( hasUserSpecificationRelationWord &&
					isCurrentSpecificationSelfGeneratedSpecification &&
					// Secondary specification word is same as user specification word
					currentSpecificationWordItem == userSpecificationWordItem )
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage )
							{
							if( hasUserSpecificationCompoundCollection &&
							// Spanish test file: "razonamiento\familia\Juan - Luz - Ana"
							!isUserAssignment &&

								( isUserGeneralizationWord ||
								hasNonCompoundCurrentSpecificationCollection ) &&

							// Justification doesn't exist yet in specification
							!currentSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Add Spanish reversible user justification
							currentSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible user justification to the current secondary specification" );
							}
						else
							{
							if( isPossessiveUserSpecification &&
							isCurrentSpecificationSelfGeneratedAssumption )
								{
								if( ( hasUserGeneralizationWordCurrentlyConfirmedSpecification ||
								!currentSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) &&

								// Conclusion doesn't exist yet
								generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, userGeneralizationWordItem ) == null )
									{
									conclusionSpecificationItem = ( !hasNonCompoundCurrentSpecificationCollection ||
																	!hasUserGeneralizationWordCurrentlyConfirmedSpecification ? null :
																		generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, null ) );

									// Draw reversible user conclusion
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

									if( conclusionSpecificationItem == null )
										{
										if( createAndAssignResult.foundSpecificationItem != null &&
										( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
										// Add reversible justification to current secondary specification
										currentSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the current secondary specification" );
										}
									else
										{
										conclusionSpecificationItem = conclusionSpecificationItem.updatedSpecificationItem();

										// Test files: "reasoning\family\My assumptions that are confirmed (parents)",
										//				"reasoning\family\Paul - Joe - Laura - John and Anna (son - daughter)"
										// Confirmed all relation words
										if( conclusionSpecificationItem.nRelationWords() == currentSpecificationItem.nRelationWords() &&
										// Replace assumption by conclusion
										generalizationWordItem.replaceOrDeleteSpecification( currentSpecificationItem, conclusionSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
										}
									}
								}
							else
								{
								// Test file: "reasoning\family\Complex (17)"
								if( isOlderCurrentSpecification &&
								( obsoleteSpecificationItem = generalizationWordItem.noRelationWordSpecificationItem( false, true, currentSpecificationWordItem ) ) != null &&
								// Replace obsolete assumption specification by conclusion
								generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, currentSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete assumption specification by a conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						}

					// Typical for Spanish
					if( isCurrentSpecificationWordSpanishAmbiguous )
						currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

					if( isDrawingPossessiveReversibleConclusions &&
					currentRelationWordItem != null &&

						// No current specification creation sentence number
						( ( !hasCurrentSpecificationCreationSentenceNr &&
						!generalizationWordItem.hasJustification( currentSpecificationItem, null, null ) ) ||

						// Current specification creation sentence number
						( hasCurrentSpecificationCreationSentenceNr &&

							( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR ||
							// Test file: "Unique relation"
							userSpecificationCollectionNr == currentSpecificationCollectionNr ||
							// Test file: "reasoning\family\John - Anna (before family definition)"
							currentSpecificationItem.hasOnlyOneRelationWord() ) ) ) )
						{
						// Not older
						if( ( ( !isOlderCurrentSpecification &&

							( ( existingSpecificationItem = currentRelationWordItem.bestMatchingRelationSpecificationItem( true, true, false, true, currentSpecificationWordItem, generalizationWordItem ) ) == null ||

							// User specification
							( hasNonCompoundCurrentSpecificationCollection &&
							!existingSpecificationItem.isOlderItem() ) ||

							// Self-generated specification
							( existingSpecificationItem.isSelfGeneratedSpecification() &&

								( isUserGeneralizationWord ||
								existingSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ) ) ||

						// Older
						( isOlderCurrentSpecification &&
						isCurrentSpecificationSelfGeneratedSpecification &&
						!isUserGeneralizationWord &&
						currentSpecificationWordItem == userSpecificationWordItem ) ||

						// Typical for Spanish
						( isSpanishCurrentLanguage &&

							( isCurrentSpecificationSelfGeneratedSpecification ||

							// Spanish test file: "razonamiento\Reto científico"
							( isOlderCurrentSpecification &&
							currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR ) ) ) ) &&

						// Draw possessive reversible conclusions about relation word
						drawPossessiveReversibleConclusions( isUserAssignment, isSpanishCurrentLanguage, currentSpecificationItem, generalizationWordItem, currentSpecificationWordItem, currentRelationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\" in relation word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

						isSameSpecification = ( currentSpecificationItem == userSpecificationItem );

						if( isSameSpecification &&
						currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&
						!hasUserGeneralizationWordCurrentlyConfirmedSpecification &&
						!hasUserSpecificationCompoundCollection &&
						( tempRelationCollectionNr = currentRelationWordItem.collectionNr( currentSpecificationWordItem ) ) > Constants.NO_COLLECTION_NR )
							// Test file: "reasoning\family\Complex (13)"
							currentRelationCollectionNr = tempRelationCollectionNr;

						if( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&
						( currentCollectionWordItem = WordItem.firstCollectionWordItem( currentRelationCollectionNr ) ) != null )
							{
							// Do for all collection words
							do	{
								if( currentCollectionWordItem != currentRelationWordItem &&

									// Different specifications
									( ( !isSameSpecification &&

										( ( isCurrentSpecificationSelfGeneratedSpecification &&
										( firstJustificationItem = currentSpecificationItem.firstJustificationItem() ) != null &&

											( !isOlderCurrentSpecification ||

											( firstJustificationItem.isOppositePossessiveSpecificationAssumption() &&
											currentSpecificationItem.hasJustificationWithAdditionalDefinitionSpecification() ) ||

											currentSpecificationItem.hasRelationCollectionCurrentlyBeenUpdated() ) ) ||

										// Typical for Spanish
										( isSpanishCurrentLanguage &&

											// User specification word not Spanish ambiguous
											// Spanish test file: "razonamiento\familia\Complejo (14)",
											//						"razonamiento\familia\Mis suposiciones que se confirman (Juan y Ana)"
											( ( !isUserSpecificationWordSpanishAmbiguous &&
											isCurrentSpecificationSelfGeneratedSpecification ) ||

											// User specification word Spanish ambiguous
											// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
											( isUserSpecificationWordSpanishAmbiguous &&
											isCurrentSpecificationWordSpanishAmbiguous ) ) ) ) ) ||

									// Same specification
									( isSameSpecification &&

										( currentCollectionWordItem.isUserRelationWord ||

										// Test file: "Acceptance - Boston - United States - United Kingdom"
										( isExclusiveUserSpecification &&
										currentCollectionWordItem.firstSpecificationItem( false, true, false, currentSpecificationWordItem ) == null ) ) ) ) &&

								// Draw possessive reversible conclusions about collection relation word
								drawPossessiveReversibleConclusions( isUserAssignment, isSpanishCurrentLanguage, currentSpecificationItem, generalizationWordItem, currentSpecificationWordItem, currentCollectionWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\" in collection relation word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );
								}
							while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( currentRelationCollectionNr ) ) != null );
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem.isReplacedOrDeletedItem() ?
												// First specification
												generalizationWordItem.firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) :
												// Next specification
												currentSpecificationItem.nextNonQuestionNonRelationSpecificationItem() ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawSelectedReversibleConclusions++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawSelectedReversibleConclusions++;
			}

		return Constants.RESULT_OK;
		}

	private byte drawUserReversibleConclusions( boolean isSpanishCurrentLanguage, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasNonCompoundUserSpecificationCollection = false;
		boolean isArchivedAssignment;
		boolean isAssignment;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int userSpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem assignmentSpecificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem assumptionSpecificationWordItem;
		WordItem commonWordItem;
		WordItem spanishFeminineSpecificationWordItem;
		WordItem spanishMasculineSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		// Specification doesn't exist yet
		if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( true, true, false, false, specificationWordItem, generalizationWordItem ) ) == null ||

		( existingSpecificationItem.isSelfGeneratedSpecification() &&
		// Justification doesn't exist yet in specification
		!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) )
			{
			hasNonCompoundUserSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			conclusionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, specificationWordItem, null );

			if( ( !hasNonCompoundUserSpecificationCollection &&
			existingSpecificationItem == null ) ||

			( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
			// Self-generated conclusion with different relation word
			conclusionSpecificationItem != null ) ||

			( existingSpecificationItem != null &&
			// Conclusion doesn't exist yet
			conclusionSpecificationItem == null ) ||

			( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
			generalizationWordItem.firstActiveQuestionSpecificationItem() != null ) &&

			// Assumption doesn't exist yet
			relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, null ) == null ) ||

			// Typical for Spanish
			isSpanishCurrentLanguage )
				{
				isAssignment = false;
				isArchivedAssignment = false;
				assignmentSpecificationItem = null;
				spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
				userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

				// Typical for Spanish
				if( isSpanishCurrentLanguage &&
				hasNonCompoundUserSpecificationCollection &&

					( !relationWordItem.hasNegativeSpecification() ||
					relationWordItem.hasFeminineProperNounEnding() ) )
					spanishAmbiguousSpecificationCollectionNr = userSpecificationCollectionNr;

				// Similar specification already exists as assignment
				// So, make this conclusion an assignment too
				if( existingSpecificationItem == null &&
				( assignmentSpecificationItem = relationWordItem.firstAssignmentItem( false, false, specificationWordItem ) ) != null )
					{
					isAssignment = true;
					isArchivedAssignment = assignmentSpecificationItem.isArchivedAssignment();
					}

				// Draw reversible user conclusion
				if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
					{
					// No existing specification
					if( existingSpecificationItem == null )
						{							// Assignment specification
						foundSpecificationItem = ( assignmentSpecificationItem != null ? assignmentSpecificationItem :
													// Found specification
													( createAndAssignResult.foundSpecificationItem != null ? createAndAssignResult.foundSpecificationItem :
													// User specification
													relationWordItem.firstUserSpecificationItem( false, false, createdSpecificationItem.specificationCollectionNr(), specificationWordItem ) ) );

						// No found specification
						if( foundSpecificationItem == null )
							{
							if( hasNonCompoundUserSpecificationCollection &&
							!isSpanishCurrentLanguage )
								{
								// Test file: "reasoning\family\Complex (3)"
								// Draw only option left conclusion
								if( drawOnlyOptionLeftProperNounConclusion( createdSpecificationItem, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only-option-left proper noun conclusion" );


								if( !createdSpecificationItem.isHiddenSpanishSpecification() &&
								( commonWordItem = specificationWordItem.commonWordItem( userSpecificationCollectionNr ) ) != null &&
								( assumptionSpecificationWordItem = specificationWordItem.collectionWordItem( userSpecificationCollectionNr ) ) != null &&
								( definitionSpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
								( assumptionSpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, assumptionSpecificationWordItem ) ) != null &&
								// Justification doesn't exist yet in specification
								!assumptionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, createdSpecificationItem ) &&
								// Add opposite possessive assumption justification to assumption specification
								assumptionSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive assumption justification to an assumption specification" );
								}
							}
						else	// Found specification
							{
							if( ( isAssignment ||
							!userSpecificationItem.hasSpecificationCollection() ) &&

							// Add reversible user justification to created specification
							createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the created specification" );
							}
						}
					else	// Existing specification
						{
						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
							{
							if( foundSpecificationItem.hasCompoundSpecificationCollection() )
								{
								// Replace an assumption specification by a conclusion specification in justifications of involved word
								if( relationWordItem.updateSpecificationOfJustificationsInWord( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in a justification in word \"" + relationWordItem.anyWordTypeString() + "\"" );

								// Add reversible user justification to created specification
								if( foundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the created specification" );

								// Typical for Spanish
								// Spanish test file: "razonamiento\familia\Complejo (15)"
								if( isSpanishCurrentLanguage &&
								( spanishMasculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem() ) != null &&
								( assumptionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem ) ) != null &&
								!assumptionSpecificationItem.isHiddenSpanishSpecification() &&
								// Add Spanish reversible justification to assumption specification
								assumptionSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible justification to the assumption specification" );
								}
							else
								{
								if( ( obsoleteJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, generalizationWordItem ) ) != null )
									{
									// Test file: "reasoning\family\You have confirmed at least one relation word"
									// Create justification
									if( ( justificationResult = relationWordItem.createJustificationItem( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, userSpecificationItem, null, null, null, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to create a justification item to word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

									// Replace negative assumption justification
									if( relationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption justification in word \"" + relationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					}

				// Typical for Spanish
				if( userSpecificationItem.isSpecificationWordSpanishAmbiguous() )
					{
					if( relationWordItem.hasFeminineProperNounEnding() &&
					( spanishFeminineSpecificationWordItem = specificationWordItem.feminineCollectionWordItem() ) != null &&
					( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, spanishFeminineSpecificationWordItem ) ) != null &&
					relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, definitionSpecificationItem.specificationCollectionNr(), Constants.NO_COLLECTION_NR, spanishFeminineSpecificationWordItem, null ) == null &&
					// Draw Spanish feminine exclusive specification substitution assumption
					adminItem_.addSelfGeneratedSpecification( true, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, relationWordItem, spanishFeminineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish exclusive specification substitution assumption about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + spanishFeminineSpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private FindSpecificationResultType findPossessiveSpecifications( WordItem possessiveNounWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem foundPossessiveDefinitionSpecificationItem = null;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem higherLevelPossessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		FindSpecificationResultType findSpecificationResult = new FindSpecificationResultType();

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
					if( higherLevelPossessiveDefinitionSpecificationWordItem == null )
						higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
					else
						hasFoundPossessiveSpecificationWord = false;
					}
				}
			while( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
				{
				if( higherLevelPossessiveDefinitionSpecificationWordItem == null )
					higherLevelPossessiveDefinitionSpecificationWordItem = specificationWordItem;

				// Try to find the opposite of the possessive specification
				do	{
					if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( previousPossessiveSpecificationItem != null )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem.specificationCollectionNr();

							if( higherLevelPossessiveDefinitionSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem :
																																												possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem :
																																									previousPossessiveSpecificationItem );
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

	private byte findQuestionToBeAdjustedByCompoundCollection( SpecificationItem definitionSpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		int definitionSpecificationCollectionNr;
		SpecificationItem adjustedQuestionCompoundSpecificationItem;
		SpecificationItem lastAdjustedQuestionCompoundSpecificationItem = null;
		SpecificationItem replacingSpecificationItem = null;
		WordItem commonWordItem;
		WordItem currentCollectionWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( definitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given definition specification item is undefined" );

		if( ( definitionSpecificationCollectionNr = definitionSpecificationItem.specificationCollectionNr() ) <= Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given definition specification item has no specification collection" );

		if( ( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
			{
			hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem.hasCurrentlyMoreSpecificSpecification();

			// Do for all collection words
			do	{
				if( ( commonWordItem = currentCollectionWordItem.commonWordItem( definitionSpecificationCollectionNr ) ) != null &&
				( adjustedQuestionCompoundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, currentCollectionWordItem ) ) != null )
					{
					if( commonWordItem == specificationWordItem )
						{
						// Skip updating question if the old justification has no relation word,
						// while the current user specification has a relation word.
						// Test file: "reasoning\family\This information is more specific"
						if( !hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
						// Justification doesn't exist yet in specification
						!adjustedQuestionCompoundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) )
							{
							// Create adjusted question
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, adjustedQuestionCompoundSpecificationItem.isInactiveAssignment(), adjustedQuestionCompoundSpecificationItem.isArchivedAssignment(), false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, adjustedQuestionCompoundSpecificationItem.generalizationWordTypeNr(), adjustedQuestionCompoundSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, currentCollectionWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create an adjusted question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );

							replacingSpecificationItem = createAndAssignResult.foundSpecificationItem;
							}
						}
					else
						{
						if( generalizationWordItem.replaceOrDeleteSpecification( adjustedQuestionCompoundSpecificationItem, ( replacingSpecificationItem == null || replacingSpecificationItem.isReplacedOrDeletedItem() ? null : replacingSpecificationItem ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a question part" );

						lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem.updatedSpecificationItem();
						}
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != null &&
		// Write last instance of adjusted compound question
		generalizationWordItem.writeUpdatedSpecification( true, false, false, true, false, false, false, lastAdjustedQuestionCompoundSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question" );

		return Constants.RESULT_OK;
		}

	private byte makeOppositePossessiveSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, boolean isSpanishCurrentLanguage, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasExistingSpecificationCurrentCreationSentenceNr = false;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		boolean hasRelationWordConfirmedSpecification;
		boolean hasRelationWordMasculineProperNounEnding = false;
		boolean hasSameSpecificationCollection;
		boolean isAlternativeSecondaryUserSpecification = false;
		boolean isExistingUserSpecification = false;
		boolean isOlderCurrentExistingSpecification = false;
		boolean isOlderExistingSpecification = false;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isRelationWordUserGeneralizationWord;
		short createAssumptionLevel;
		short currentExistingAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		short selectedJustificationTypeNr;
		int hiddenSpanishSpecificationCollectionNr;
		int relationCollectionNr;
		SpecificationItem additionalDefinitionSpecificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentExistingSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem feminineSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem selectedSpecificationItem = null;
		WordItem alternativeSecondarySpecificationWordItem;
		WordItem currentCollectionWordItem;
		WordItem feminineSpecificationWordItem = null;
		WordItem oppositePossessiveDefinitionSpecificationWordItem;
		WordItem possessiveDefinitionSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( !Item.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( oppositePossessiveDefinitionSpecificationItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item is undefined" );

		if( possessiveDefinitionSpecificationItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The possessive definition specification item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item has no specification word" );

		if( ( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The The possessive definition specification item has no specification word item" );

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
		hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem.hasCurrentlyMoreSpecificSpecification();
		hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();
		isRelationWordUserGeneralizationWord = relationWordItem.isUserGeneralizationWord;

		if( oppositePossessiveDefinitionSpecificationItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item is undefined" );

		if( possessiveDefinitionSpecificationItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The possessive definition specification item is undefined" );

		if( ( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The The possessive definition specification item has no specification word item" );

		if( ( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item has no specification word" );

		isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

		if( !isPossessiveSpecificationWordSpanishAmbiguous &&
		secondarySpecificationItem.hasCompoundSpecificationCollection() )
			return adminItem_.startError( 1, moduleNameString_, "Some preconditions are not met" );

		if( alternativeSecondarySpecificationItem != null )
			isAlternativeSecondaryUserSpecification = alternativeSecondarySpecificationItem.isUserSpecification();

		if( isPossessive )
			{
			selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;

			selectedPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
			selectedSecondarySpecificationItem = secondarySpecificationItem;
			}
		else
			{
			selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION;

			selectedPrimarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																								secondarySpecificationItem );
			selectedSecondarySpecificationItem = null;
			}

		if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( true, false, !isPossessive, possessiveDefinitionSpecificationWordItem ) ) != null )
			{
			hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem.hasCurrentCreationSentenceNr();
			isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
			isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
			}

		// User specification doesn't exist
		if( ( ( existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, possessiveDefinitionSpecificationWordItem ) ) == null &&
		alternativeSecondarySpecificationItem != null &&
		!possessiveDefinitionSpecificationWordItem.isUserSpecificationWord &&

			( ( isPossessive &&
			!isOlderExistingSpecification &&
			!isPossessiveSpecificationWordSpanishAmbiguous &&
			isRelationWordUserGeneralizationWord &&
			existingSpecificationItem != null &&
			// Justification doesn't exist yet in specification
			!existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
			// Justification has at least the same assumption level
			existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) ) ||

		// Typical for Spanish
		( isPossessiveSpecificationWordSpanishAmbiguous &&
		alternativeSecondarySpecificationItem != null &&

			// User specification doesn't exist yet
			( ( existingUserSpecificationItem == null &&

				// No alternative
				( ( !isAlternativeSecondaryUserSpecification &&
				existingSpecificationItem == null ) ||

				// Alternative
				( isAlternativeSecondaryUserSpecification &&

				( !generalizationWordItem.hasActiveQuestionWithCompoundCollection() ||
				// Spanish test file: "Saltarse las preguntas no válidos (2)"
				alternativeSecondarySpecificationItem.isActiveAssignment() ) &&

					( existingSpecificationItem == null ||

					( ( !existingSpecificationItem.hasNonCompoundSpecificationCollection() ||

					// Justification doesn't exist yet in specification
					( !existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) &&

					// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
					( ( hasExistingSpecificationCurrentCreationSentenceNr ||
					!alternativeSecondarySpecificationItem.isOlderItem() ) &&

						// Don't include following condition line
						// Justification doesn't exist yet in specification
//						!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) ) ) ) ) ||

				// User specification exists
				// Typical for Spanish
				// Spanish test file: "razonamiento\familia\Complejo (4)"
				( existingUserSpecificationItem != null &&
				isRelationWordUserGeneralizationWord &&
				existingSpecificationItem != null &&
				existingUserSpecificationItem.isOlderItem() &&
				existingUserSpecificationItem.hasCompoundSpecificationCollection() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) ) )
			{
			// Make an opposite possessive specification assumption (normal)
			if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, false, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
			}
		else
			{
			if( isPossessiveSpecificationWordSpanishAmbiguous &&
			// Draw Spanish proper noun part-of conclusions in proper noun words
			drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, false, true, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
			}

		if( existingSpecificationItem != null &&
		alternativeSecondarySpecificationItem != null &&
		alternativeSecondarySpecificationItem.isPossessive() &&
		!existingSpecificationItem.hasCurrentCreationSentenceNr() &&
		existingSpecificationItem.hasNonCompoundSpecificationCollection() &&

			( userSpecificationItem.isPossessive() ||
			userSpecificationItem.hasCompoundSpecificationCollection() ) &&

		// Avoid idle call
		possessiveDefinitionSpecificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() != null &&
		// Draw specification substitution conclusion or ask a question
		drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + relationWordItem.anyWordTypeString() + "\"" );

		if( alternativeSecondarySpecificationItem != null )
			{
			if( ( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem.specificationWordItem() ) != null &&
			// Avoid idle call
			alternativeSecondarySpecificationWordItem.firstNegativeSpecificationItem() != null &&
			( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, alternativeSecondarySpecificationWordItem ) ) != null &&
			!foundSpecificationItem.isHiddenSpanishSpecification() &&
			// Draw simple negative conclusions
			drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, foundSpecificationItem, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Try to find user specification or self-generated conclusion first
			if( ( selectedSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem, null ) ) != null &&
			// Typical for Spanish
			isSpanishCurrentLanguage &&
			isAlternativeSecondaryUserSpecification &&
			selectedSpecificationItem.isHiddenSpanishSpecification() )
				selectedSpecificationItem = null;

			// If not found, try to find a self-generated assumption
			if( selectedSpecificationItem == null &&
			( assumptionSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null )
				{
				hasSameSpecificationCollection = ( assumptionSpecificationItem.specificationCollectionNr() == alternativeSecondarySpecificationItem.specificationCollectionNr() );

				if( !assumptionSpecificationItem.hasCurrentCreationSentenceNr() &&

				( hasGeneralizationWordCurrentlyConfirmedSpecification == hasSameSpecificationCollection ||
				alternativeSecondarySpecificationItem.isPossessive() ) )
					selectedSpecificationItem = assumptionSpecificationItem;
				}
			}

		isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();
		feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationWordItem.feminineCollectionWordItem() :
																									oppositePossessiveDefinitionSpecificationWordItem.feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem.hasFeminineProperNounEnding() : relationWordItem.hasFeminineProperNounEnding() ) );

		if( feminineSpecificationWordItem != null )
			{
			if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
				additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
			else
				feminineSpecificationWordItem = null;
			}

		if( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
		// Typical for Spanish
		isRelationWordUserGeneralizationWord ||
		additionalDefinitionSpecificationItem == null ||
		generalizationWordItem.firstUserSpecificationItem( false, isPossessive, additionalDefinitionSpecificationItem.specificationCollectionNr(), null ) != null ) &&

		( selectedSpecificationItem == null ||

		// Relation word is not user generalization word
		( !isRelationWordUserGeneralizationWord &&
		!hasGeneralizationWordCurrentlyConfirmedSpecification &&
		!hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
		existingSpecificationItem != null &&
		existingSpecificationItem.isSelfGeneratedSpecification() &&

			// Test files: "reasoning\family\Complex (16)",
			//				"reasoning\family\Complex (17)",
			//				"reasoning\family\I know (1)"
			( !selectedSpecificationItem.hasRelationWord() ||

			( selectedSpecificationItem.isSelfGeneratedSpecification() &&
			relationWordItem.bestMatchingRelationSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) == null ) ) ) ||

		// Relation word is user generalization word
		( isRelationWordUserGeneralizationWord &&
		// Typical for Spanish
		isOppositePossessiveSpecificationWordSpanishAmbiguous &&

			( !selectedSpecificationItem.hasRelationWord() ||

			( selectedSpecificationItem.hasOnlyOneRelationWord() &&
			!selectedSpecificationItem.isHiddenSpanishSpecification() ) ||

			( selectedSpecificationItem.isSelfGeneratedSpecification() &&
			relationWordItem.bestMatchingRelationSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) != null ) ) ) ||

		( ( !hasGeneralizationWordCurrentlyMoreSpecificSpecification ||
		additionalDefinitionSpecificationItem == null ||
		selectedSpecificationItem.isSelfGeneratedConclusionOrAssumption() ) &&

			// Typical for Spanish
			// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
			( ( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
			hasGeneralizationWordCurrentlyMoreSpecificSpecification ) ||

			// Try to find user specification or self-generated conclusion first
			relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, true, oppositePossessiveDefinitionSpecificationWordItem, null ) != null ) ) ) )
			{
			if( ( currentExistingSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( true, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null )
				{
				isOlderCurrentExistingSpecification = currentExistingSpecificationItem.isOlderItem();
				currentExistingAssumptionLevel = currentExistingSpecificationItem.assumptionLevel();
				}

			selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
			selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem_;
			selectedAdditionalDefinitionSpecificationItem = null;
			selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																											secondarySpecificationItem );

			// Typical for Spanish
			// Spanish test file: "razonamiento\Reto científico"
			if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
			!isPossessive &&
			additionalDefinitionSpecificationItem == null &&
			relationWordItem.hasMasculineProperNounEnding() &&
			( foundSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
				{
				hasRelationWordMasculineProperNounEnding = true;
				selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
				selectedPrimarySpecificationItem = selectedSecondarySpecificationItem;
				selectedAdditionalDefinitionSpecificationItem = possessiveDefinitionSpecificationItem_;
				selectedSecondarySpecificationItem = foundSpecificationItem;
				}

			if( selectedSecondarySpecificationItem != null )
				{
				createAssumptionLevel = Item.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null );

				// Non-possessive
				if( ( !isPossessive &&

					( ( !hasRelationWordConfirmedSpecification &&
					!isOlderCurrentExistingSpecification ) ||

					// Typical for Spanish
					// Spanish test file: "razonamiento\familia\Complejo (3)"
					( currentExistingSpecificationItem == null ||

					// Justification doesn't exist yet in specification
					( !currentExistingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&

						// No confirmed relation word
						( ( !hasRelationWordConfirmedSpecification &&

							( ( !isExistingUserSpecification &&
							currentExistingAssumptionLevel >= createAssumptionLevel ) ||

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (18)"
							( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							currentExistingAssumptionLevel == createAssumptionLevel ) ) ) ||

						// Confirmed relation word
						( hasRelationWordConfirmedSpecification &&
						!selectedSecondarySpecificationItem.hasRelationCollection() &&

							// Test file: "reasoning\family\Complex (16)"
							( !selectedSecondarySpecificationItem.isOlderItem() ||
							// Test file: "reasoning\family\Joe is a son - Joe has 2 parents"
							currentExistingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != null ) ) ) ) ||

					// Typical for Spanish
					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!hasRelationWordConfirmedSpecification &&
					currentExistingAssumptionLevel == createAssumptionLevel &&
					currentExistingSpecificationItem.isHiddenSpanishSpecification() ) ) ) ) ||

				// Possessive
				( isPossessive &&
				existingSpecificationItem != null &&

					( alternativeSecondarySpecificationItem == null ||

					// Typical for Spanish
					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

						( currentExistingSpecificationItem == null ||
						currentExistingAssumptionLevel >= createAssumptionLevel ||

						( currentExistingSpecificationItem.hasCurrentCreationSentenceNr() &&

							( existingSpecificationItem.isSelfGeneratedSpecification() ||

							( isOlderCurrentExistingSpecification &&
							!alternativeSecondarySpecificationItem.isOlderItem() ) ) ) ) ) ) ) )
					{
					// Make an assumption
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an assumption from generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

					createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
					foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

					if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					existingSpecificationItem != null &&
					( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
					// Justification doesn't exist yet in specification
					!createdOrFoundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem_, existingSpecificationItem ) &&
					// Justification has at least the same assumption level
					createdOrFoundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem_, null, existingSpecificationItem, null ) &&
					// Add opposite possessive conclusion justification to found specification
					createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem_, null, existingSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish opposite possessive conclusion justification to the found specification" );

					if( isPossessive )
						{
						// Typical for Spanish
						if( hasRelationWordConfirmedSpecification &&
						foundSpecificationItem != null &&
						( relationCollectionNr = foundSpecificationItem.relationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
						( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr ) ) != null )
							{
							// Do for all collection words
							do	{
								if( ( selectedSecondarySpecificationItem = currentCollectionWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, secondarySpecificationWordItem ) ) != null &&
								// Justification doesn't exist yet in specification
								!foundSpecificationItem.hasSecondarySpecificationJustification( selectedSecondarySpecificationItem ) &&
								// Justification has at least the same assumption level
								foundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
								// Add Spanish opposite possessive conclusion justification to found specification
								foundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish opposite possessive conclusion justification to the found specification" );
								}
							while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr ) ) != null );
							}
						}
					else
						{
						if( createdSpecificationItem != null )
							{
							// Created, but not found
							if( foundSpecificationItem == null )
								{
								// Typical for Spanish
								// Spanish test file: "razonamiento\Reto científico"
								if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
								isExistingUserSpecification &&
								additionalDefinitionSpecificationItem == null &&
								( hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
								relationWordItem.hasFeminineProperNounEnding() &&
								( feminineSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstNegativeSpecificationItem() ) != null &&
								( feminineSpecificationWordItem = feminineSpecificationItem.specificationWordItem() ) != null )
									{
									// Make feminine assumption
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine assumption from generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

									if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
									// Specification doesn't exist yet
									( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null, null ) &&
									// Add reversible justification to existing specification
									existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
									}
								}
							else	// Created and found
								{
								// Justification doesn't exist yet in specification
								if( !createdSpecificationItem.hasPrimarySpecificationJustification( foundSpecificationItem ) &&
								// Justification has at least the same assumption level
								createdSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null, null ) &&
								// Add reversible justification
								createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the found specification" );
								}
							}
						}
					}
				}

			if( alternativeSecondarySpecificationItem != null &&
			feminineSpecificationWordItem != null &&
			possessiveDefinitionSpecificationWordItem == secondarySpecificationWordItem &&

				// Test files: "reasoning\family\Complex (18)",
				//				"reasoning\family\Complex (19 - mixed)",
				//				"reasoning\family\Complex (19 - strange)"
				( isPossessive ||
				!secondarySpecificationItem.isSelfGeneratedAssumption() ||
				secondarySpecificationItem != alternativeSecondarySpecificationItem ) &&

			( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem.specificationWordItem() ) != null )
				{
				existingSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem );

				selectedPrimarySpecificationItem = ( isPossessive &&
													existingSpecificationItem == null &&
													userSpecificationItem.assumptionLevel() < secondarySpecificationItem.assumptionLevel() ? userSpecificationItem :

													( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
													possessiveDefinitionSpecificationWordItem == alternativeSecondarySpecificationWordItem ? alternativeSecondarySpecificationItem :
																																			secondarySpecificationItem ) );
				selectedAdditionalDefinitionSpecificationItem = ( !isPossessive &&
																isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationItem_ :
																														possessiveDefinitionSpecificationItem_ );
				selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;

				if( existingSpecificationItem == null ||

				( !selectedPrimarySpecificationItem.isPossessive() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) )
					{
					// Make feminine possessive exclusive specification substitution assumption
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine possessive exclusive specification substitution assumption with generalization word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

					if( isPossessive )
						{
						// Test files: "reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)",
						//				among others
						if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&

							// Specification doesn't exist yet
							( ( existingSpecificationItem = generalizationWordItem.firstNonExclusiveSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) == null ||

							// Justification doesn't exist yet in specification
							( !existingSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null, null ) ) ) &&

						// Draw possessive reversible conclusion
						adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, null, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						// Typical for Spanish
						// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
						if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
						!userSpecificationItem.hasSpecificationCollection() &&
						existingSpecificationItem != null &&
						( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&
						( selectedPrimarySpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
						// Justification doesn't exist yet in specification
						!existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) &&
						// Add Spanish possessive exclusive specification substitution justification to existing specification
						existingSpecificationItem.addJustificationToSpecification( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish possessive exclusive specification substitution assumption justification to the existing specification" );
						}
					}
				}
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_makeOppositePossessiveSpecificationAssumption++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_makeOppositePossessiveSpecificationAssumption++;
			}

		return Constants.RESULT_OK;
		}

	private static SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNounEnding, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationItem possessiveSpecificationItem;
		WordItem userSpecificationWordItem;

		if( generalizationWordItem != null &&
		userSpecificationItem != null )
			{
			if( generalizationWordItem.isUserRelationWord &&
			userSpecificationItem.isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNounEnding &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&
			// Try to find a possessive specification
			( possessiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, userSpecificationWordItem ) ) != null )
				return possessiveSpecificationItem;
			}

		return null;
		}

	private static SpecificationItem getAdditionalDefinitionSpecificationItem( WordItem userSpecificationWordItem )
		{
		SpecificationItem additionalDefinitionSpecificationItem = null;
		SpecificationItem possessiveDefinitionSpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem higherLevelPossessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;

		if( userSpecificationWordItem != null &&
		( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
			// Do for all possessive noun words
			do	{
				// First, try to find a possessive specification on a higher level
				if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
					{
					do	{
						if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
						possessiveSpecificationWordItem.firstSpecificationItem( false, false, false, userSpecificationWordItem ) != null )
							higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveDefinitionSpecificationWordItem == null &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
						additionalDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == null ||
																higherLevelPossessiveDefinitionSpecificationWordItem == possessiveDefinitionSpecificationItem.specificationWordItem() ? possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() :
																																														possessiveDefinitionSpecificationItem );
					}
				}
			while( additionalDefinitionSpecificationItem == null &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );
			}

		return additionalDefinitionSpecificationItem;
		}


	// Constructor

	protected AdminReasoning( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReasoningVariables()
		{
		firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;
		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;
		}

	protected byte addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore( SpecificationItem hiddenSpanishSpecificationItem, WordItem specificationWordItem )
		{
		int relationCollectionNr;
		SpecificationItem reverseSpecificationItem;
		WordItem currentCollectionWordItem;

		if( hiddenSpanishSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given hidden Spanish specification item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( relationCollectionNr = hiddenSpanishSpecificationItem.relationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr ) ) != null )
			{
			// Do for all collection words
			do	{
				if( ( reverseSpecificationItem = currentCollectionWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
				reverseSpecificationItem.isSelfGeneratedSpecification() &&
				// Justification doesn't already exist in specification
				!reverseSpecificationItem.hasPrimarySpecificationJustification( hiddenSpanishSpecificationItem ) &&
				// Justification has at least the same assumption level
				reverseSpecificationItem.assumptionLevel() >= hiddenSpanishSpecificationItem.assumptionLevel() &&
				// Add reversible justification to unhidden specification
				reverseSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, hiddenSpanishSpecificationItem, null, null ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible specification justification to the reverse specification" );
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isArchivedAssignment, boolean isSpanishCurrentLanguage, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem simpleUserSpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasNonCompoundSecondarySpecificationCollection;
		boolean hasPrimarySpecificationRelationWord;
		boolean hasUserSpecificationRelationWord;
		boolean isCommonWordWordSpanishAmbiguous = false;
		boolean isPrimaryExclusiveSpecification;
		boolean isPrimarySameAsUserSpecification;
		boolean isPrimaryUserSpecification;
		boolean isSecondaryAdjectiveSpecification;
		boolean isSecondaryExclusiveSpecification;
		boolean isSpecificationWordSpanishAmbiguous = false;
		int compoundSpecificationCollectionNr;
		int nonCompoundSpecificationCollectionNr;
		int secondarySpecificationCollectionNr;
		int specificationCollectionNr;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem relatedSpecificationItem;
		WordItem collectionWordItem = null;
		WordItem commonWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		RelatedResultType relatedResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr() ) == Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification collection" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( commonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null )
			isCommonWordWordSpanishAmbiguous = commonWordItem.isNounWordSpanishAmbiguous();

		if( ( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( commonWordItem == null ||
		( specificationCollectionNr = commonWordItem.collectionNr( specificationWordItem ) ) == Constants.NO_COLLECTION_NR )
			specificationCollectionNr = secondarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		hasPrimarySpecificationRelationWord = primarySpecificationItem.hasRelationWord();
		isPrimaryExclusiveSpecification = primarySpecificationItem.isExclusiveSpecification();
		isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
		isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();

		hasNonCompoundSecondarySpecificationCollection = secondarySpecificationItem.hasNonCompoundSpecificationCollection();
		isSecondaryAdjectiveSpecification = secondarySpecificationItem.isAdjectiveSpecification();
		isSecondaryExclusiveSpecification = secondarySpecificationItem.isExclusiveSpecification();

		hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
		hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
		hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();

		if( isSpanishCurrentLanguage )
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		commonWordItem != null &&
		relatedSpecificationItem == null &&
		!userSpecificationItem.isPossessive() &&
		( nonCompoundSpecificationCollectionNr = commonWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR )
			{
			if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

			// No related specification
		if( relatedSpecificationItem == null )
			{
			if( ( !isSpecificationWordSpanishAmbiguous ||
			// Typical for Spanish
			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ||
			!hasGeneralizationWordFeminineProperNounEnding ) &&

				( hasUserSpecificationRelationWord ||
				isPrimarySameAsUserSpecification ||
				!userSpecificationWordItem.isMasculineWord() ||

				( !isSecondaryAdjectiveSpecification &&

					( ( compoundSpecificationCollectionNr = secondarySpecificationWordItem.compoundCollectionNr() ) == Constants.NO_COLLECTION_NR ||
					secondarySpecificationWordItem.compoundGeneralizationWordItem( compoundSpecificationCollectionNr ) == userSpecificationWordItem ) ) ||

				( GlobalVariables.nUserSpecificationWords > 1 &&

					( !userSpecificationWordItem.isNounWordSpanishAmbiguous() ||
					// Typical for Spanish
					isPrimaryExclusiveSpecification ) ) ) )
				{
				// Skip invalid questions (step 1)
				if( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
				primarySpecificationItem.hasCompoundSpecificationCollection() ||

				// Test file: "Additional test"
				( isPrimarySameAsUserSpecification &&
				!secondarySpecificationWordItem.isFeminineOrMasculineWord() ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						// Typical for Spanish
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem );
					else
						collectionWordItem = secondarySpecificationWordItem.collectionWordItem( secondarySpecificationCollectionNr );

					// Skip invalid questions (step 2)
					if( ( collectionWordItem != null &&

						( hasPrimarySpecificationRelationWord ||
						generalizationWordItem.primaryAndSecondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, secondarySpecificationItem ) == null ) ) ||

					// Typical for Spanish
					( isSpecificationWordSpanishAmbiguous &&

						// Not user specification
						( ( !isPrimaryUserSpecification &&

							( existingSpecificationItem == null ||
							existingSpecificationItem.isHiddenSpanishSpecification() ) ) ||

						// User specification
						( isPrimaryExclusiveSpecification &&
						isPrimaryUserSpecification ) ) ) )
						{
						if( ( isPrimarySameAsUserSpecification ||
						isSecondaryExclusiveSpecification ||
						hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ) &&

						// Typical for Spanish
						( isSpanishCurrentLanguage ||
						!primarySpecificationItem.hasJustificationWithAdditionalDefinitionSpecification() ||
						primarySpecificationItem.isOlderItem() ||
						userGeneralizationWordItem.hasCurrentlyMoreSpecificSpecification() ) &&

						// Ask questions
						askQuestions( isSpanishCurrentLanguage, simpleUserSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );

						if( !hasUserSpecificationRelationWord &&
						isSecondaryAdjectiveSpecification &&

							( !isSpecificationWordSpanishAmbiguous ||
							// Typical for Spanish
							// Spanish test file: "Esta información es más específico (no exclusiva)"
							!isSecondaryExclusiveSpecification ) &&

						// Justification doesn't exist yet in word
						generalizationWordItem.secondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, secondarySpecificationItem ) == null &&
						// Ask adjective question
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, Constants.WORD_TYPE_ADJECTIVE, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask an adjective question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					// Definition specification
					// Test files: "reasoning\Scientific challenge",
					//				"reasoning\family\John - Anna (before family definition)"
					if( hasPrimarySpecificationRelationWord &&
					userSpecificationItem.isNounGeneralization() &&
					primarySpecificationItem.isSelfGeneratedSpecification() &&

					// Question part doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == null ||
					// Justification doesn't exist yet in question
					!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) ) &&

					// Ask a specification substitution question
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}
		else	// Related specification
			{
			// Typical for Spanish
			if( isCommonWordWordSpanishAmbiguous &&
			isSpecificationWordSpanishAmbiguous &&
			isPrimaryUserSpecification &&
			GlobalVariables.nUserSpecificationWords == 1 &&
			!primarySpecificationItem.hasNonCompoundSpecificationCollection() )
				{
				// Spanish test file: "Español ambigua - Ana es un padre - Luisa es un hijo"
				if( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
				// Ask Spanish ambiguous questions
				askQuestions( isSpanishCurrentLanguage, simpleUserSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish ambiguous questions" );
				}
			else
				{
				if( !hasPrimarySpecificationRelationWord &&

					( hasNonCompoundSecondarySpecificationCollection ||

					// Test file: "reasoning\Knowledge of organisms (1)"
					( secondarySpecificationItem.isUserSpecification() &&
					secondarySpecificationWordItem.compoundCollectionNr() == secondarySpecificationCollectionNr ) ) &&

				// Draw negative conclusion
				drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_askSpecificationSubstitutionQuestionOrDrawNegativeConclusion++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_askSpecificationSubstitutionQuestionOrDrawNegativeConclusion++;
			}

		return Constants.RESULT_OK;
		}

	protected byte drawCompoundSpecificationSubstitutionConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isSpanishCurrentLanguage, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasCurrentCompoundSpecificationCollection;
		boolean hasCurrentNonCompoundSpecificationCollection;
		boolean hasCurrentSpecificationCreationSentenceNr;
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean isCurrentAdjectiveSpecification;
		boolean isCurrentExclusiveSpecification;
		boolean isPrimaryExclusiveSpecification;
		boolean isExclusiveUserSpecification = false;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isOlderPrimarySpecification;
		boolean isSelectedExclusiveSpecification;
		short currentSpecificationWordTypeNr;
		short selectedJustificationTypeNr;
		int currentSpecificationCollectionNr;
		int primarySpecificationCollectionNr;
		SpecificationItem adjectiveSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem exclusiveSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem previousUserSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem compoundGeneralizationWordItem;
		WordItem currentSpecificationWordItem;
		WordItem previousUserSpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( !Item.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() ) != null )
			{
			isPrimaryExclusiveSpecification = primarySpecificationItem.isExclusiveSpecification();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();

			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();

			compoundGeneralizationWordItem = specificationWordItem.compoundGeneralizationWordItem(primarySpecificationCollectionNr);

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem &&
				currentSpecificationWordItem != compoundGeneralizationWordItem )
					{
					hasCurrentCompoundSpecificationCollection = currentSpecificationItem.hasCompoundSpecificationCollection();
					hasCurrentNonCompoundSpecificationCollection = currentSpecificationItem.hasNonCompoundSpecificationCollection();
					hasCurrentSpecificationCreationSentenceNr = currentSpecificationItem.hasCurrentCreationSentenceNr();
					isCurrentAdjectiveSpecification = currentSpecificationItem.isAdjectiveSpecification();
					isCurrentExclusiveSpecification = currentSpecificationItem.isExclusiveSpecification();
					currentSpecificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();

					isSelectedExclusiveSpecification = ( isExclusiveSpecification &&
														currentSpecificationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
														currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR );

					// Definition specification
					if( ( !isGeneralizationProperNoun &&

						( ( !isCurrentExclusiveSpecification &&

							( hasCurrentNonCompoundSpecificationCollection ||

							// Older definition specification (without specification collection)
							( currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&
							currentSpecificationItem.isOlderItem() ) ) ) ||

						// Exclusive compound definition specification
						( isCurrentExclusiveSpecification &&
						hasCurrentCompoundSpecificationCollection &&
						compoundGeneralizationWordItem != null &&
						currentSpecificationWordItem.isFeminineOrMasculineWord() ) ) &&

					// User specification doesn't exist yet
					generalizationWordItem.firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, currentSpecificationWordItem ) == null ) ||

					// Proper noun specification
					// Test file: "reasoning\Scientific challenge"
					( isGeneralizationProperNoun &&
					// Adjective specification word
					isCurrentAdjectiveSpecification &&
					isCurrentExclusiveSpecification &&
					isOlderPrimarySpecification &&
					hasCurrentSpecificationCreationSentenceNr &&
					!hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					!isSpanishCurrentLanguage ) )
						{
						selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;

						if( isCurrentAdjectiveSpecification )
							{
							if( compoundGeneralizationWordItem == null )
								{
								if( isCurrentExclusiveSpecification )
									{
									// Create adjective question
									isSelectedExclusiveSpecification = true;
									questionParameter = Constants.WORD_PARAMETER_SINGULAR_VERB_IS;
									selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION;
									}
								else
									{
									if( isPrimaryExclusiveSpecification )
										{
										if( isOlderPrimarySpecification )
											// Test file: "reasoning\family\John has 2 sons and a daughter (before family definition)"
											isSelectedExclusiveSpecification = true;

										// Definition
										if( !isOlderPrimarySpecification &&
										!isSpanishCurrentLanguage &&
										primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR )
											// Test file: "reasoning\Scientific challenge"
											isSelectedExclusiveSpecification = true;
										}
									}
								}
							else
								{
								// Specification doesn't exist yet
								if( generalizationWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem ) == null )
									// Create exclusive adjective specification
									isSelectedExclusiveSpecification = true;
								}
							}

						if( !hasCurrentSpecificationCreationSentenceNr ||
						isSelectedExclusiveSpecification ||

						( !isExclusiveUserSpecification &&
						!isSpanishCurrentLanguage ) )
							{
							if( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, questionParameter, currentSpecificationWordItem ) ) != null &&
							// Typical for Spanish
							isSpanishCurrentLanguage &&
							isExclusiveUserSpecification &&
							isSelectedExclusiveSpecification &&
							!existingSpecificationItem.isExclusiveSpecification() &&
							!existingSpecificationItem.isOlderItem() &&
							( tempSpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem( currentSpecificationWordItem ) ) != null )
								// Spanish test file: "razonamiento\Reto científico"
								existingSpecificationItem = tempSpecificationItem;

							// Specification doesn't exist yet
							if( existingSpecificationItem == null ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, currentSpecificationItem ) )
								{
								// Draw specification substitution conclusion
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isSelectedExclusiveSpecification, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

								if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
									{
									if( !createdOrFoundSpecificationItem.isExclusiveSpecification() &&
									// Test file: "reasoning\Scientific challenge"
									specificationWordItem.bestMatchingRelationSpecificationItem( true, false, currentSpecificationCollectionNr, null ) != null &&
									// Draw simple negative conclusions
									drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( !isGeneralizationProperNoun )
										{
										if( isSelectedExclusiveSpecification )
											{
											if( hasCurrentNonCompoundSpecificationCollection &&
											hasCurrentSpecificationCreationSentenceNr &&
											!isSpanishCurrentLanguage &&
											// Make generalization assumption definition
											adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, currentSpecificationItem, null, specificationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption definition about generalization word \"" + specificationWordItem.anyWordTypeString() + "\" and specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( !hasCurrentNonCompoundSpecificationCollection &&
											primarySpecificationItem.hasSpecificationCollection() &&
											( adjectiveSpecificationItem = userSpecificationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&
											( exclusiveSpecificationItem = currentSpecificationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null )
												{
												// Find previous user specification
												if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, userSpecificationItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

												if( ( previousUserSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
												( previousUserSpecificationWordItem = previousUserSpecificationItem.specificationWordItem() ) != null &&
												( adjectiveSpecificationItem = previousUserSpecificationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&
												( exclusiveSpecificationItem = currentSpecificationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&

												// Specification doesn't exist yet
												( ( existingSpecificationItem = previousUserSpecificationWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) == null ||
												// Justification doesn't exist yet in specification
												!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( exclusiveSpecificationItem, adjectiveSpecificationItem ) ) )
													{
													// Make previous user generalization assumption definition
													if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, previousUserSpecificationItem.generalizationWordTypeNr(), exclusiveSpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, exclusiveSpecificationItem, null, adjectiveSpecificationItem, null, previousUserSpecificationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make a previous user generalization assumption definition about generalization word \"" + previousUserSpecificationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

													if( ( secondarySpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
													// Add specification substitution justification to created or found specification
													createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, previousUserSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the created or found specification" );
													}
												}
											}
										}
									}
								}
							}
						}
					else
						{
						// Corrected assumption by opposite suggestive question
						if( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&
						isCurrentAdjectiveSpecification &&
						// Get exclusive question part
						( exclusiveSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, true, questionParameter, ( compoundGeneralizationWordItem == null ? specificationWordItem.compoundCollectionNr() : primarySpecificationItem.specificationCollectionNr() ), specificationWordItem ) ) != null &&
						// Ask adjective question
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, exclusiveSpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask an adjective question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem.nextNonNegativeNonPosessiveDefinitionSpecificationItem() ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawCompoundSpecificationSubstitutionConclusion++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawCompoundSpecificationSubstitutionConclusion++;
			}

		return Constants.RESULT_OK;
		}

	protected byte drawExclusiveNegativeUserConclusions( boolean isArchivedAssignment, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		SpecificationItem previousUserSpecificationItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( currentSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem != null &&
		( previousUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, previousSpecificationWordItem ) ) != null )
			{
			// Negative specification doesn't exist yet
			if( previousSpecificationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) == null &&
			// Draw negative conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, previousUserSpecificationItem, null, null, null, previousSpecificationWordItem, currentSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative user conclusion about generalization word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

			// Negative specification doesn't exist yet
			if( currentSpecificationWordItem.firstSpecificationItem( true, false, false, previousSpecificationWordItem ) == null &&
			// Draw another negative conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, currentSpecificationWordItem, previousSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawNegativeUserDefinitionConclusions( boolean isArchivedAssignment, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem adjectiveSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem adjectiveNegativeSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		WordItem currentGeneralizationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( adjectiveSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given adjective specification word item is undefined" );

		if( currentSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current specification word item is undefined" );

		// Negative specification doesn't exist yet
		if( generalizationWordItem.firstSpecificationItem( true, false, false, adjectiveSpecificationWordItem ) == null )
			{
			// Draw adjective negative conclusion
			if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_ADJECTIVE, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, adjectiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + adjectiveSpecificationWordItem.anyWordTypeString() + "\"" );

			if( ( adjectiveNegativeSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
			// Get first generalization specification noun word
			( currentGeneralizationItem = adjectiveSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
					currentGeneralizationWordItem != generalizationWordItem )
						{
						// Negative definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != null &&
						// Draw negative generalization conclusion (negative definition)
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative generalization conclusion with negative definition about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

						// Positive definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem.firstNonExclusiveSpecificationItem( false, false, false, adjectiveSpecificationWordItem ) ) != null )
							{
							// Negative conclusion doesn't exist yet
							if( generalizationWordItem.firstNonExclusiveSpecificationItem( true, false, false, currentGeneralizationWordItem ) == null &&
							// Draw negative generalization conclusion (positive definition)
							adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, definitionSpecificationItem, null, adjectiveNegativeSpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative generalization conclusion with positive definition about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

							// Draw negative generalization conclusion (reversed, positive definition)
							if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, definitionSpecificationItem, null, adjectiveNegativeSpecificationItem, null, currentGeneralizationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversed negative generalization conclusion with positive definition about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte drawOnlyOptionLeftNounConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, int generalizationContextNr, WordItem generalizationWordItem )
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
				if( currentSpecificationItem.isExclusiveSpecification() &&
				currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
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
			adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, true, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawProperNounPartOfConclusionsInProperNounWords( boolean isArchivedAssignment, boolean isChineseCurrentLanguage, boolean isSpanishCurrentLanguage, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationCurrentlyMoreSpecificSpecification;
		boolean hasUserSpecificationRelationWord;
		boolean isPossessiveUserSpecification;
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserAssignment;
		boolean isUserQuestion;
		boolean isUserSpecificationWordSpanishAmbiguous;
		int userSpecificationCollectionNr;
		JustificationItem firstJustificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem currentProperNounWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userRelationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		if( ( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the user specification item is undefined" );

		if( ( currentProperNounWordItem = GlobalVariables.firstUserDefinedProperNounWordItem ) != null )
			{
			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			hasUserSpecificationCurrentlyMoreSpecificSpecification = userGeneralizationWordItem.hasCurrentlyMoreSpecificSpecification();

			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserAssignment = userSpecificationItem.isAssignment();
			isUserQuestion = userSpecificationItem.isQuestion();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
			userRelationWordItem = userSpecificationItem.relationWordItem();

			// Do for all user-defined proper noun words
			do	{
				if( currentProperNounWordItem != spanishRelationWordItem )
					{
					if( ( primarySpecificationItem = currentProperNounWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null ||
					isSpecificationWordSpanishAmbiguous )
						{
						primarySpecificationItem = currentProperNounWordItem.firstAssignmentOrSpecificationItem( false, true, false, false, false, specificationWordItem );

						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&

						( primarySpecificationItem == null ||
						primarySpecificationItem.isHiddenSpanishSpecification() ) )
							{
							if( spanishRelationWordItem == null )
								{
								// Try to find self-generated question, as alternative to hidden specification
								primarySpecificationItem = ( ( tempSpecificationItem = currentProperNounWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, true, specificationWordItem ) ) == null ||
															!tempSpecificationItem.isExclusiveSpecification() ? null :
																												tempSpecificationItem );
								}
							else
								{
								// Typical for Spanish
								if( hasUserSpecificationRelationWord &&
								isPossessiveUserSpecification &&
								!isUserSpecificationWordSpanishAmbiguous &&
								primarySpecificationItem != null &&
								primarySpecificationItem.hasRelationCollection() &&
								!currentProperNounWordItem.isUserGeneralizationWord &&
								currentProperNounWordItem.isUserRelationWord )
									primarySpecificationItem = userSpecificationItem;
								else
									{
									tempSpecificationItem = ( userSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&	// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
																primarySpecificationItem != null ) ? spanishRelationWordItem.bestMatchingRelationSpecificationItem( true, true, false, true, primarySpecificationItem.specificationCollectionNr(), specificationWordItem ) : 
																									spanishRelationWordItem.bestMatchingRelationSpecificationItem( true, true, false, true, specificationWordItem, currentProperNounWordItem );

									primarySpecificationItem = ( tempSpecificationItem == null ||
																tempSpecificationItem.isHiddenSpanishSpecification() ? null : tempSpecificationItem );
									}
								}
							}
						}
					else
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage &&
						primarySpecificationItem.isUserSpecification() &&
						( tempSpecificationItem = currentProperNounWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
							// Convert user assignment to simple user specification
							primarySpecificationItem = tempSpecificationItem;
						}

					if( primarySpecificationItem != null &&

						// No user relation word
						( ( !hasUserSpecificationRelationWord &&

							( !primarySpecificationItem.isOlderItem() ||
							// Test file: "reasoning\family\John - Anna (before family definition)"
							!primarySpecificationItem.hasSpecificationCollection() ) ) ||

						// User relation word
						( hasUserSpecificationRelationWord &&
						primarySpecificationItem.hasCurrentCreationSentenceNr() &&

							// No user compound collection
							( ( !hasUserSpecificationCompoundCollection &&

								// Test file: "reasoning\family\Complex (13)"
								( ( primarySpecificationItem.isOlderItem() &&
								primarySpecificationItem.hasCompoundSpecificationCollection() ) ||

								( primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
								primarySpecificationItem.specificationCollectionNr() == userSpecificationCollectionNr ) ) ) ||

							// User compound collection
							( hasUserSpecificationCompoundCollection &&
							primarySpecificationItem.hasNonCompoundSpecificationCollection() &&

								( isUserQuestion ||

								( primarySpecificationItem.hasRelationWord() &&
								( firstJustificationItem = primarySpecificationItem.firstJustificationItem() ) != null &&

									// Not older specification
									( ( !primarySpecificationItem.isOlderItem() &&

										// Specification substitution assumption or conclusion
										( firstJustificationItem.isSpecificationSubstitutionAssumptionOrConclusion() ||

										// Opposite possessive specification assumption
										( firstJustificationItem.isOppositePossessiveSpecificationAssumption() &&
										primarySpecificationItem.hasOnlyOneRelationWord() &&

											( !primarySpecificationItem.hasJustificationWithAdditionalDefinitionSpecification() ||
											// Test files: "reasoning\family\Complex (12)",
											//				"reasoning\family\Complex (15)",
											//				"reasoning\family\This information is more specific"
											primarySpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) == null ||

											// Typical for Chinese
											// Chinese test file: "This information is more specific"
											( isChineseCurrentLanguage &&
											hasUserSpecificationCurrentlyMoreSpecificSpecification ) ) ) ) ) ||

									// Older specification
									( primarySpecificationItem.isOlderItem() &&
									primarySpecificationItem.hasRelationCollection() &&

										// Specification substitution assumption or conclusion
										// Test files: "reasoning\family\Complex (4)",
										//				"reasoning\family\Complex (5)",
										//				"reasoning\family\Complex (6)",
										//				"reasoning\family\Complex (9)",
										//				"reasoning\family\Complex (14)",
										//				among others
										( ( userRelationWordItem != null &&
										( tempSpecificationItem = userRelationWordItem.firstActiveQuestionSpecificationItem() ) != null &&
										tempSpecificationItem.isOlderItem() ) ||

										( !firstJustificationItem.isOlderItem() &&

											// Test files: "conflict\family\Joe is a mother",
											//				"reasoning\family\Complex (14)",
											//				among others
											( firstJustificationItem.isSpecificationSubstitutionAssumptionOrConclusion() ||

											// Test file: "reasoning\family\John is the father - Paul - Laura"
											( !isUserAssignment &&
											firstJustificationItem.isReversibleAssumptionOrConclusion() &&
											currentProperNounWordItem.firstActiveQuestionSpecificationItem() != null &&
											( tempSpecificationItem = currentProperNounWordItem.firstNegativeSpecificationItem() ) != null &&
											tempSpecificationItem.isOlderItem() ) ) ) ) ) ||

									// Typical for Spanish
									isSpanishCurrentLanguage ) ) ) ) ) ) ||

							// Typical for Spanish
							( isSpanishCurrentLanguage &&

								// No user relation word
								( !hasUserSpecificationRelationWord ||
								isUserSpecificationWordSpanishAmbiguous ||
								// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
								userSpecificationCollectionNr == Constants.NO_COLLECTION_NR ||

								( primarySpecificationItem.hasCurrentCreationSentenceNr() &&
								primarySpecificationItem.specificationCollectionNr() == userSpecificationCollectionNr ) ) ) ) &&

					// Avoid idle call
					GlobalVariables.firstPossessiveNounWordItem != null &&
					// Draw proper noun part-of conclusions
					drawProperNounPartOfConclusions( isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, currentProperNounWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions in proper noun word \"" + currentProperNounWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentProperNounWordItem = currentProperNounWordItem.nextUserDefinedProperNounWordItem ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawProperNounPartOfConclusionsInProperNounWords++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawProperNounPartOfConclusionsInProperNounWords++;
			}

		return Constants.RESULT_OK;
		}

	protected byte drawReversibleConclusions( boolean isCheckingAssignment, boolean isChineseCurrentLanguage, boolean isDrawingPossessiveReversibleConclusions, boolean isSpanishCurrentLanguage, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean isArchivedAssignment;
		int nStartCreatedISpanishItems = 0;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem.isProperNounWord() )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item isn't a proper noun" );

		if( isCheckingAssignment )
			{
			if( isSpanishCurrentLanguage )
				nStartCreatedISpanishItems = GlobalVariables.nTotalCreatedItems;

			isArchivedAssignment = ( generalizationWordItem.firstNonQuestionSpecificationItem( true, true, false ) == null );

			// Draw selected reversible conclusions based on assignments
			if( drawSelectedReversibleConclusions( true, isArchivedAssignment, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with active assignments" );
			}

		if( ( !isCheckingAssignment ||
		userSpecificationItem.isOlderItem() ||
		// Typical for Chinese
		isChineseCurrentLanguage ||

		// Typical for Spanish
		( isSpanishCurrentLanguage &&
		// No progress made in assignment calls above
		GlobalVariables.nTotalCreatedItems == nStartCreatedISpanishItems ) ) &&

		// Draw selected reversible conclusions based on specifications
		drawSelectedReversibleConclusions( false, false, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with specifications" );

		return Constants.RESULT_OK;
		}

	protected byte drawSimpleNegativeConclusions( boolean isArchivedAssignment, boolean isSpanishCurrentLanguage, SpecificationItem primarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge;
		boolean hasUserSpecificationCurrentCreationSentenceNr;
		boolean hasUserSpecificationNonCompoundCollection;
		boolean hasUserSpecificationRelationWord;
		boolean isDrawingCompoundNegativeConclusions;
		boolean isDrawingNegativeUserConclusion;
		boolean isPrimaryExclusiveSpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimarySelfGeneratedSpecification;
		boolean isUserRelationWord;
		boolean isUserSpecificationWordSpanishAmbiguous;
		short generalizationWordTypeNr;
		short negativeSpecificationWordTypeNr;
		short primaryAssumptionLevel;
		short userAssumptionLevel;
		short userSpecificationWordTypeNr;
		int compoundCollectionNr = Constants.NO_COLLECTION_NR;
		int userSpecificationCollectionNr;
		JustificationItem firstJustificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem negativeSpecificationItem;
		SpecificationItem negativeDefinitionSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		WordItem negativeSpecificationWordItem;
		WordItem specificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( specificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( negativeDefinitionSpecificationItem = specificationWordItem.firstNegativeSpecificationItem() ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem.hasCurrentlyCorrectedAssumptionByKnowledge();
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			isPrimaryExclusiveSpecification = primarySpecificationItem.isExclusiveSpecification();
			isPrimarySelfGeneratedSpecification = primarySpecificationItem.isSelfGeneratedSpecification();
			generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();
			primaryAssumptionLevel = primarySpecificationItem.assumptionLevel();

			// Don't move, because of redefining user specification
			if( ( simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null )
				userSpecificationItem = simpleUserSpecificationItem;

			hasUserSpecificationCurrentCreationSentenceNr = userSpecificationItem.hasCurrentCreationSentenceNr();
			hasUserSpecificationNonCompoundCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
			userSpecificationWordTypeNr = userSpecificationItem.specificationWordTypeNr();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
			userAssumptionLevel = userSpecificationItem.assumptionLevel();

												// If the user specification is possessive, the user generalization word must be opposite, and otherwise
			isDrawingNegativeUserConclusion = ( isPossessiveUserSpecification == isUserRelationWord &&
												userSpecificationItem.isProperNounGeneralization() &&
												!userSpecificationItem.isOlderItem() &&
												!userSpecificationItem.isQuestion() );

			isDrawingCompoundNegativeConclusions = ( isDrawingNegativeUserConclusion &&
													( compoundCollectionNr = userSpecificationWordItem.compoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
													userSpecificationWordItem.collectionWordItem( compoundCollectionNr ) != null );

			do	{
				if( ( negativeSpecificationWordItem = negativeDefinitionSpecificationItem.specificationWordItem() ) != null &&

					( negativeDefinitionSpecificationItem.isAdjectiveSpecification() ||

					( !isPrimaryExclusiveSpecification &&

						// Positive specification doesn't exist
						( generalizationWordItem.firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) == null ||
						hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge ||
						// Typical for Spanish
						negativeSpecificationWordItem.isNounWordSpanishAmbiguous() ) ) ) )
					{
					negativeSpecificationWordTypeNr = negativeDefinitionSpecificationItem.specificationWordTypeNr();

					if( isDrawingNegativeUserConclusion &&

						// Test files: "reasoning\family\Complex (8)",
						//				"reasoning\family\Complex (15)",
						//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
						( isPossessiveUserSpecification ||
						// No answered question with this specification word
						generalizationWordItem.bestMatchingRelationSpecificationItem( true, true, false, false, true, false, false, true, userSpecificationCollectionNr, Constants.NO_COLLECTION_NR, negativeSpecificationWordItem, null ) == null ) )
						{
						negativeSpecificationItem = ( userSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ? negativeSpecificationWordItem.firstSpecificationItem( true, false, false, userSpecificationWordItem ) :
																														userSpecificationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) );

						if( negativeSpecificationItem != null &&
							
							// Negative specification doesn't exist
							( ( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == null &&

								( negativeSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ||

								( hasUserSpecificationNonCompoundCollection &&

									( negativeSpecificationItem.specificationCollectionNr() == userSpecificationCollectionNr ||
									// Typical for Spanish
									isSpanishCurrentLanguage ) ) ) ) ||

							// Negative specification exists
							( existingSpecificationItem != null &&
							!hasGeneralizationWordCurrentlyConfirmedSpecification &&
							!hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation &&
							existingSpecificationItem.hasPrimaryNounSpecificationJustification() &&
							// Justification doesn't exist yet in specification
							!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, negativeSpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, negativeSpecificationItem, null ) ) ) &&
						// Draw negative user conclusion
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, negativeSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );
						}

					if( hasUserSpecificationCurrentCreationSentenceNr ||
					isUserRelationWord )
						{
						obsoleteSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, negativeDefinitionSpecificationItem.specificationCollectionNr(), null );

						// Negative specification doesn't exist
						if( ( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == null &&

							( !isDrawingNegativeUserConclusion ||

							( obsoleteSpecificationItem != null &&
							primaryAssumptionLevel <= userAssumptionLevel ) ||

							( isPrimarySelfGeneratedSpecification &&

								( !isDrawingCompoundNegativeConclusions ||

								// Test file: "reasoning\Knowledge of organisms (1)"
								( hasUserSpecificationNonCompoundCollection &&
								primaryAssumptionLevel <= userAssumptionLevel ) ||

								// Typical for Spanish
								( isUserSpecificationWordSpanishAmbiguous &&

									( negativeSpecificationWordItem.isMasculineWord() ||

									// Spanish test files: "razonamiento\familia\Complejo (12)",
									//						"razonamiento\familia\Complejo (15)"
									( isPossessiveUserSpecification &&
									( firstJustificationItem = primarySpecificationItem.firstJustificationItem() ) != null &&
									firstJustificationItem.hasPrimaryUserSpecification() ) ) ) ) ) ) ) ||

						// Negative specification exists
						( existingSpecificationItem != null &&
						// Justification doesn't exist yet in specification
						!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, negativeDefinitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, negativeDefinitionSpecificationItem, null ) ) )
							{
							// Draw simple negative conclusion
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, negativeDefinitionSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );

							if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
								{
								// Definition specification
								if( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR )
									{
									// Draw negative definition conclusions
									if( drawNegativeDefinitionConclusions( isArchivedAssignment, generalizationWordTypeNr, negativeSpecificationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, negativeSpecificationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative definition conclusions" );
									}
								else	// Proper noun specification
									{
									if( obsoleteSpecificationItem != null &&
									// Test file: "reasoning\family\Correcting invalidated assumption (by knowledge)"
									createdOrFoundSpecificationItem.isCorrectedSpecification() &&
									negativeDefinitionSpecificationItem.specificationCollectionNr() == userSpecificationCollectionNr &&
									// Correct invalid suggestive assumptions by knowledge
									correctSuggestiveAssumptionsByKnowledge( userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to correct invalid suggestive assumptions by knowledge about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}

								// Test file: "reasoning\Knowledge of organisms (1)"
								if( !hasUserSpecificationRelationWord &&
								isDrawingCompoundNegativeConclusions &&
								userSpecificationCollectionNr != compoundCollectionNr &&
								// Draw negative exclusive conclusions
								drawNegativeExclusiveConclusions( isArchivedAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, negativeSpecificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative exclusive conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with negative specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );
								}
							}
						}
					}
				}
			while( ( negativeDefinitionSpecificationItem = negativeDefinitionSpecificationItem.nextNegativeSpecificationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawSimpleNegativeConclusions++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawSimpleNegativeConclusions++;
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		short assumptionLevel;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem primaryGeneralizationWordItem;
		WordItem primarySpecificationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != null )
					{
					assumptionLevel = ( primarySpecificationItem.assumptionLevel() < currentSpecificationItem.assumptionLevel() ? currentSpecificationItem.assumptionLevel() :
																																		primarySpecificationItem.assumptionLevel() );

					// Collect generalization words of a specification-generalization definition
					if( adminItem_.collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

					// Specification-generalization
					currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

					if( currentSpecificationItem.hasCurrentCreationSentenceNr() &&
					// Specification-generalization doesn't exist yet

						( ( foundSpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, currentGeneralizationWordItem ) ) == null ||
						foundSpecificationItem.isUserSpecification() ) &&

					// Draw specification-generalization conclusion (using current specification)
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, currentSpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentSpecificationItem, null, null, null, specificationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the current specification" );

					if( !isSelfGenerated &&
					currentSpecificationItem.hasGeneralizationCollection() &&
					( primaryGeneralizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					// Specification-generalization doesn't exist yet
					primarySpecificationWordItem.firstSpecificationItem( false, false, false, primaryGeneralizationWordItem ) == null &&
					// Draw specification-generalization conclusion (using primary specification)
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, primarySpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, null, null, primarySpecificationWordItem, primaryGeneralizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the primary specification" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawSpecificationGeneralizationConclusion++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawSpecificationGeneralizationConclusion++;
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isSpanishCurrentLanguage, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCurrentSpecificationCollection;
		boolean hasCurrentSpecificationCompoundCollection;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean hasPrimarySpecificationNonCompoundCollection;
		boolean hasPrimarySpecificationRelationWord;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasUserSpecificationCompoundCollection = false;
		boolean hasUserSpecificationNonCompoundCollection = false;
		boolean hasUserSpecificationRelationWord = false;
		boolean isCurrentAdjectiveSpecification;
		boolean isArchivedUserAssignment = false;
		boolean isCurrentUserSpecification;
		boolean isDefinitionSpanishAmbiguous;
		boolean isCurrentExclusiveSpecification;
		boolean isExclusiveUserSpecification = false;
		boolean isFeminineOrMasculineGeneralizationWord;
		boolean isFoundPrimaryHiddenSpanishSpecification;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isOlderCurrentSpecification;
		boolean isPossessiveUserSpecification = false;
		boolean isPossessivePrimarySpecification;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserAssignment = false;
		boolean isUserAdjectiveSpecification = false;
		boolean isUserDefinitionSpecification = false;
		boolean isUserGeneralizationWord;
		boolean isUserQuestion = false;
		boolean isUserRelationWord;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short currentSpecificationWordTypeNr;
		short selectedRelationWordTypeNr;
		int currentSpecificationCollectionNr;
		int nonCompoundSpecificationCollectionNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem tempJustificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem alternativeSpanishAdditionalDefinitionSpecificationItem;
		SpecificationItem archivedAssignmentItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem negativeConclusionSpecificationItem;
		SpecificationItem negativeDefinitionSpecificationItem;
		SpecificationItem negativeProperNounSpecificationItem;
		SpecificationItem positiveDefinitionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem questionSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem selfGeneratedSpecificationItem;
		SpecificationItem simpleUserSpecificationItem = null;
		SpecificationItem spanishNonCompoundSpecificationItem;
		WordItem collectionWordItem;
		WordItem commonWordItem;
		WordItem currentSpecificationWordItem;
		WordItem oppositeSpecificationWordItem;
		WordItem selectedRelationWordItem;
		WordItem userSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( !Item.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !Item.isSpecificationReasoningWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a specification reasoning type" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() ) != null )
			{
			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem.hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem.hasMasculineProperNounEnding();
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem.isFeminineOrMasculineWord();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( userSpecificationItem != null )
				{
				hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
				hasUserSpecificationNonCompoundCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
				hasUserSpecificationRelationWord = userSpecificationItem.hasRelationWord();
				isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
				isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
				isPossessiveUserSpecification = userSpecificationItem.isPossessive();
				isUserAssignment = userSpecificationItem.isAssignment();
				isUserAdjectiveSpecification = userSpecificationItem.isAdjectiveSpecification();
				isUserDefinitionSpecification = userSpecificationItem.isNounGeneralization();
				isUserQuestion = userSpecificationItem.isQuestion();
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
				userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
				userSpecificationWordItem = userSpecificationItem.specificationWordItem();

				simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem();
				}

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					hasCurrentSpecificationCompoundCollection = currentSpecificationItem.hasCompoundSpecificationCollection();
					isCurrentUserSpecification = currentSpecificationItem.isUserSpecification();
					isCurrentAdjectiveSpecification = currentSpecificationItem.isAdjectiveSpecification();
					isCurrentExclusiveSpecification = currentSpecificationItem.isExclusiveSpecification();
					isOlderCurrentSpecification = currentSpecificationItem.isOlderItem();
					currentSpecificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR );

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, specificationWordItem, relationWordItem ) ) == null )
						foundPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, isQuestion, specificationWordItem );

					if( foundPrimarySpecificationItem == null )
						{
						if( isCurrentUserSpecification &&
						currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
						!currentSpecificationItem.isSpecificationWordSpanishAmbiguous() &&
						// Check for specification conflict
						generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, false, false, isSpecificationWordSpanishAmbiguous, currentSpecificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.relationCollectionNr(), currentSpecificationWordItem, currentSpecificationItem.relationWordItem() ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to check for a specification conflict in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem.hasNonCompoundSpecificationCollection();
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem.isHiddenSpanishSpecification();

						// Specification doesn't exist yet
						if( ( existingSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, currentSpecificationCollectionNr, Constants.NO_COLLECTION_NR, currentSpecificationWordItem, null ) ) == null )
							// Test file: "reasoning\family\John - Anna (before family definition)"
							// Typical for Spanish
							// Spanish test file: "razonamiento\Reto científico"
							existingSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, currentSpecificationWordItem, null );

						if( isCurrentExclusiveSpecification &&
						!isFeminineOrMasculineGeneralizationWord &&
						userSpecificationItem != null &&
						userSpecificationWordItem != null &&

							( existingSpecificationItem == null ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( foundPrimarySpecificationItem, currentSpecificationItem ) ||

							( hasCurrentSpecificationCompoundCollection &&
							relationWordItem != null &&
							!relationWordItem.hasCollectionNr( existingSpecificationItem.relationCollectionNr() ) &&
							generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, specificationWordItem, null ) != null ) ) &&

						// Matching feminine word
						( ( hasGeneralizationWordFeminineProperNounEnding &&
						currentSpecificationWordItem.isFeminineWord() ) ||

						// Matching masculine word
						( hasGeneralizationWordMasculineProperNounEnding &&
						currentSpecificationWordItem.isMasculineWord() ) ) &&

						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem.firstFeminineOrMasculineSpecificationItem() ) != null &&
						// Select correct compound word to avoid assumption to be made if a question should be asked
						currentSpecificationWordItem.compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem )
							{
							selectedRelationWordItem = ( feminineOrMasculineSpecificationItem.isAdjectiveSpecification() ? null : relationWordItem );

							if( isFoundPrimaryHiddenSpanishSpecification )
								{
								selectedPrimarySpecificationItem = userSpecificationItem;
								selectedAdditionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
								}
							else
								{
								selectedPrimarySpecificationItem = foundPrimarySpecificationItem;
								selectedAdditionalDefinitionSpecificationItem = null;
								}

							// User generalization word
							if( ( isUserGeneralizationWord &&
							foundPrimarySpecificationItem.hasUpdatedInformation() &&

								( ( !isFoundPrimaryHiddenSpanishSpecification &&

								// Non-compound specification collection
								( ( hasPrimarySpecificationNonCompoundCollection &&

								( hasCurrentSpecificationCompoundCollection &&

								( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||
								foundPrimarySpecificationItem.hasRelationWord() ) ) ) ||

								// No non-compound specification collection
								( !hasPrimarySpecificationNonCompoundCollection &&
								!hasUserSpecificationRelationWord &&
								currentSpecificationWordItem.collectionNrByCompoundGeneralizationWordInWord( Constants.WORD_TYPE_NOUN_SINGULAR, userSpecificationWordItem ) == Constants.NO_COLLECTION_NR ) ) ) ||

								// Typical for Spanish
								( isFoundPrimaryHiddenSpanishSpecification &&
								// Justification doesn't exist yet in specification
								generalizationWordItem.foundJustificationItem( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem ) == null ) ) ) ||

							// Not user generalization word
							( !isUserGeneralizationWord &&
							hasPrimarySpecificationNonCompoundCollection &&
							!isSpecificationWordSpanishAmbiguous &&

								// No existing specification
								( ( existingSpecificationItem == null &&

									( !isPossessiveUserSpecification ||
									selectedRelationWordItem != null ) ) ||

								// Existing specification
								( existingSpecificationItem != null &&
								existingSpecificationItem.hasSpecificationCollection() &&

									( ( isUserAssignment &&
									!existingSpecificationItem.hasCurrentCreationSentenceNr() ) ||

									( ( hasRelationWord &&
									selectedRelationWordItem != null &&
									existingSpecificationItem.hasRelationWord() &&
									// Relation word is not covered by justification
									existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem ) == null &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, currentSpecificationItem ) &&

										( !isPossessiveUserSpecification ||
										!selectedRelationWordItem.hasCurrentlyConfirmedSpecification() ||
										// Justification doesn't exist yet
										generalizationWordItem.secondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, currentSpecificationItem ) == null ) ) ||

									( isPossessiveUserSpecification &&
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, null ) ) ) ) ) ) ) )
								{
								selectedRelationWordTypeNr = ( selectedRelationWordItem == null ? Constants.NO_WORD_TYPE_NR : Constants.WORD_TYPE_PROPER_NOUN );

								// Make feminine or masculine proper noun ending assumption
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

								// Test files: "reasoning\family\Complex (2)",
								//				"reasoning\family\Complex (3)",
								//				"reasoning\family\Complex (4)",
								//				"reasoning\family\Complex (5)",
								//				"reasoning\family\Complex (8)",
								//				"reasoning\family\Complex (13)",
								//				among others
								if( createAndAssignResult.createdSpecificationItem != null &&
								hasUserSpecificationNonCompoundCollection &&
								relationWordItem != null &&
								!foundPrimarySpecificationItem.isOlderItem() &&
								foundPrimarySpecificationItem.isSelfGeneratedSpecification() &&
								foundPrimarySpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == null &&
								// Draw specification substitution conclusion or ask a question
								drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							}

						if( isCurrentExclusiveSpecification &&
						isGeneralizationProperNoun &&
						!isPossessiveUserSpecification &&
						currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
						existingSpecificationItem != null &&
						!existingSpecificationItem.isExclusiveSpecification() &&
						!existingSpecificationItem.isReplacedOrDeletedItem() &&
						existingSpecificationItem.isSelfGeneratedSpecification() &&
						!foundPrimarySpecificationItem.hasRelationWord() &&
						foundPrimarySpecificationItem.isSelfGeneratedSpecification() &&
						( definitionSpecificationItem = currentSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
						// Justification doesn't exist yet in specification
						!foundPrimarySpecificationItem.hasPrimaryAndSecondarySpecificationJustification( existingSpecificationItem, definitionSpecificationItem ) )
							{
							// Add specification substitution justification to found primary specification
							if( foundPrimarySpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, null, definitionSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the found primary specification" );
							}
						else
							{
							if( userSpecificationItem != null &&

								( ( existingSpecificationItem == null &&

									( hasCurrentSpecificationCollection ||
									// Test files: "reasoning\Scientific challenge",
									//				"reasoning\Knowledge of organisms (1)"
									!foundPrimarySpecificationItem.hasSpecificationCollection() ) ) ||

								( !isCurrentExclusiveSpecification &&
								isGeneralizationProperNoun ) ||

								// Typical for Spanish
								( isDefinitionSpanishAmbiguous &&
								hasCurrentSpecificationCollection &&

									( !isOlderCurrentSpecification ||
									// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Paz, José y Luisa"
									!isCurrentExclusiveSpecification ) ) ) )
								{
								// Non-exclusive
								if( ( !isExclusiveSpecification &&
								isCurrentExclusiveSpecification ) ||

								// Exclusive
								( isExclusiveSpecification &&
								isGeneralizationProperNoun &&
								hasCurrentSpecificationCollection &&
								isOlderCurrentSpecification ) )
									{
									// Ask a question or draw a negative conclusion
									if( isGeneralizationProperNoun &&
									hasCurrentSpecificationCollection &&
									userSpecificationWordItem != null &&

										// Adjective
										( ( isCurrentAdjectiveSpecification &&

											( ( !hasGeneralizationWordFeminineProperNounEnding &&
											!hasGeneralizationWordMasculineProperNounEnding &&
											isUserGeneralizationWord ) ||

											( !foundPrimarySpecificationItem.hasCompoundSpecificationCollection() &&

												( foundPrimarySpecificationItem.hasRelationWord() ||
												// Typical for Spanish
												isUserSpecificationWordSpanishAmbiguous ) ) ) ) ||

										// Non-adjective
										( !isCurrentAdjectiveSpecification &&

											( hasCurrentSpecificationCompoundCollection ||
											!foundPrimarySpecificationItem.hasCompoundSpecificationCollection() ||

											// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
											( isUserGeneralizationWord &&
											foundPrimarySpecificationItem.isCorrectedSpecification() ) ) ) ) )
										{
										positiveDefinitionSpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem );
										negativeDefinitionSpecificationItem = userSpecificationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem );

										// Question part doesn't exist yet
										if( ( ( questionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, currentSpecificationWordItem ) ) == null &&

											// No negative proper noun specification
											( ( ( negativeProperNounSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) == null &&

												( ( !isSpecificationWordSpanishAmbiguous &&

													// Non-adjective
													( ( !isCurrentAdjectiveSpecification &&
													!hasUserSpecificationRelationWord &&
													isUserGeneralizationWord &&
													negativeDefinitionSpecificationItem == null ) ||

													// Adjective
													( isCurrentAdjectiveSpecification &&

														( ( isUserRelationWord &&

															// Typical for Spanish
															// Spanish test file: "razonamiento\familia\Complejo (4)"
															( isUserSpecificationWordSpanishAmbiguous ||
															negativeDefinitionSpecificationItem == null ) ) ||

														( isUserGeneralizationWord &&

															( !hasUserSpecificationRelationWord ||
															// Typical for Spanish
															// Spanish test files: "respuestas a las preguntas\familia\Su pregunta es más específica (sobre Juan)",
															//						"razonamiento\familia\Paz es el hijo Juan y Ana"
															isUserSpecificationWordSpanishAmbiguous ||
															foundPrimarySpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) ) ) ) ||

													// Ask questions
													( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
													positiveDefinitionSpecificationItem == null &&
													foundPrimarySpecificationItem.isSelfGeneratedSpecification() &&
													!foundPrimarySpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) ||

													// Typical for Spanish
													( isSpanishCurrentLanguage &&

														// Spanish test file: "razonamiento\familia\Juan es el padre de Paz - José - Luisa - Bonita"
														( !isUserGeneralizationWord ||

														( !isUserAssignment &&

														( negativeDefinitionSpecificationItem == null ||
														foundPrimarySpecificationItem.hasPrimarySpecificationJustification( simpleUserSpecificationItem ) ) ) ) ) ) ) ||

												// Typical for Spanish
												( isSpecificationWordSpanishAmbiguous &&
												!foundPrimarySpecificationItem.isHiddenSpanishSpecification() &&

													( isUserGeneralizationWord ||
													// Spanish test file: "razonamiento\Reto científico"
													currentSpecificationItem == userSpecificationItem ) ) ) ) ||

											// Negative proper noun specification
											( negativeProperNounSpecificationItem != null &&
											hasUserSpecificationRelationWord &&
											isCurrentAdjectiveSpecification &&

												( ( isUserGeneralizationWord &&
												!hasGeneralizationWordCurrentlyConfirmedSpecification &&
												!isSpecificationWordSpanishAmbiguous &&

													// Test file: "reasoning\family\Paul - Joe - Laura - John and Anna (parents)"
													( ( positiveDefinitionSpecificationItem == null &&
													foundPrimarySpecificationItem.hasRelationWord() &&
													foundPrimarySpecificationItem.isOlderItem() ) ||

													foundPrimarySpecificationItem.hasPrimarySpecificationJustification( simpleUserSpecificationItem ) ) ) ||

												( negativeProperNounSpecificationItem.isOlderItem() &&

													( ( !isSpecificationWordSpanishAmbiguous &&

														( ( isUserRelationWord &&

															// Typical for Spanish
															// Spanish test file: "razonamiento\familia\Complejo (4)"
															( isUserSpecificationWordSpanishAmbiguous ||
															negativeDefinitionSpecificationItem == null ) ) ||

														( positiveDefinitionSpecificationItem == null &&
														!hasGeneralizationWordCurrentlyConfirmedSpecification &&
														foundPrimarySpecificationItem.isSelfGeneratedSpecification() &&
														!foundPrimarySpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) ) ) ||

													// Typical for Spanish
													( isSpecificationWordSpanishAmbiguous &&

														( ( isUserGeneralizationWord &&
														!foundPrimarySpecificationItem.isHiddenSpanishSpecification() ) ||

														// Spanish test files: "razonamiento\familia\Complejo (13)",
														//						"razonamiento\familia\Complejo (16)",
														//						among others
														( !isUserGeneralizationWord &&
														negativeDefinitionSpecificationItem != null &&
														foundPrimarySpecificationItem.hasRelationCollectionCurrentlyBeenUpdated() &&
														foundPrimarySpecificationItem.isOlderItem() ) ) ) ) ) ) ) ) ) ||

										// Question part exists
										( questionSpecificationItem != null &&

											( ( !isSpecificationWordSpanishAmbiguous &&

												// No positive definition
												( ( positiveDefinitionSpecificationItem == null &&
												!hasGeneralizationWordCurrentlyConfirmedSpecification &&

													( ( isCurrentAdjectiveSpecification ||

													// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
													( negativeDefinitionSpecificationItem == null &&
													questionSpecificationItem.isCorrectedSpecification() ) ) &&

													!questionSpecificationItem.hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ||

												// Positive definition
												( positiveDefinitionSpecificationItem != null &&

													// Non-adjective
													( ( !isCurrentAdjectiveSpecification &&
													negativeDefinitionSpecificationItem == null &&

														( ( isUserGeneralizationWord &&

															( ( questionSpecificationItem.isOlderItem() &&

																// Question to be adjusted
																( hasGeneralizationWordCurrentlyConfirmedSpecification ||

																( !hasUserSpecificationRelationWord &&
																// Justification doesn't exist yet in question
																!questionSpecificationItem.hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ) ||

															// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
															( questionSpecificationItem.isCorrectedSpecification() &&
															!questionSpecificationItem.hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ) ||

														// Typical for Spanish
														( isSpanishCurrentLanguage &&
														questionSpecificationItem.isOlderItem() &&

															// Spanish test file: "Juan es el padre de Paz - José - Luisa - Bonita"
															( !isUserGeneralizationWord ||
															!isUserAssignment ) ) ) ) ||

													// Adjective
													( isCurrentAdjectiveSpecification &&
													!hasGeneralizationWordCurrentlyConfirmedSpecification &&

														( ( ( isUserRelationWord ||

														( !hasUserSpecificationRelationWord &&
														isUserGeneralizationWord ) ) &&

															( questionSpecificationItem.isOlderItem() ||
															!questionSpecificationItem.hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ||

														( isUserGeneralizationWord &&

															( ( foundPrimarySpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
															!questionSpecificationItem.hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ||

															// Typical for Spanish
															// Spanish test files: "respuestas a las preguntas\familia\Su pregunta es más específica (sobre Juan)",
															//						"razonamiento\familia\Paz es el hijo Juan y Ana"
															( isUserSpecificationWordSpanishAmbiguous &&
															questionSpecificationItem.isOlderItem() ) ) ) ) ) ) ) ) ) ||

											// Typical for Spanish
											( isSpecificationWordSpanishAmbiguous &&

												// Not user generalization word
												( ( !isUserGeneralizationWord &&
												negativeDefinitionSpecificationItem != null &&
												negativeDefinitionSpecificationItem.hasNonCompoundSpecificationCollection() &&

													// Spanish test file: "José es hijo - José tiene 2 padres"
													// Non-adjective
													( ( !isCurrentAdjectiveSpecification &&
													positiveDefinitionSpecificationItem != null &&
													questionSpecificationItem.isOlderItem() ) ||

													// Adjective
													( isCurrentAdjectiveSpecification &&
													foundPrimarySpecificationItem.hasRelationCollectionCurrentlyBeenUpdated() ) ) ) ||

												// User generalization word
												( isUserGeneralizationWord &&
												isCurrentAdjectiveSpecification &&
												!foundPrimarySpecificationItem.isHiddenSpanishSpecification() ) ) ) ) ) )
											{
											selectedPrimarySpecificationItem = ( currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE &&
																				specificationWordItem.isUserSpecificationWord ? userSpecificationItem :
																																foundPrimarySpecificationItem );

											// Ask specification substitution question, or draw negative conclusion
											if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, selectedPrimarySpecificationItem, currentSpecificationItem, simpleUserSpecificationItem, userSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								else
									{
									// Make an assumption or draw a conclusion
									if( isGeneralizationProperNoun &&
									currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&

									( existingSpecificationItem != null ||
									!foundPrimarySpecificationItem.isCorrectedSpecification() ) )
										{
										alternativeSpanishAdditionalDefinitionSpecificationItem = null;
										primarySpecificationItem = null;

										if( isSpecificationWordSpanishAmbiguous )
											{
											if( currentSpecificationWordItem != specificationWordItem )
												primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );
											}
										else
											{
											if( ( primarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null ||
											primarySpecificationItem.isUserAssignment() )
												primarySpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );
											}

										if( primarySpecificationItem != null )
											{
											alternativePrimarySpecificationItem = ( !isSpecificationWordSpanishAmbiguous ?
																					( hasRelationWord ? userSpecificationItem :
																										foundPrimarySpecificationItem ) :
																										alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) );

											if( ( existingSpecificationItem != null &&
											alternativePrimarySpecificationItem != null &&
											alternativePrimarySpecificationItem.specificationWordItem() == specificationWordItem &&

												( ( !isSpecificationWordSpanishAmbiguous &&
												!existingSpecificationItem.hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) ) ||

												// Typical for Spanish
												( isSpecificationWordSpanishAmbiguous &&

													( existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) ||

													( isPossessiveUserSpecification &&
													primarySpecificationItem.hasRelationWord() &&
													generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, specificationWordItem ) != null ) ) ) ) ) ||

											// Typical for Spanish
											( isSpecificationWordSpanishAmbiguous &&
											primarySpecificationItem.isHiddenSpanishSpecification() ) )
												{
												// Typical for Spanish
												if( isSpecificationWordSpanishAmbiguous &&
												specificationWordItem != userSpecificationWordItem &&
												( commonWordItem = specificationWordItem.commonWordItem( primarySpecificationItem.specificationCollectionNr() ) ) != null &&
												( oppositeSpecificationWordItem = specificationWordItem.collectionWordItem( primarySpecificationItem.specificationCollectionNr() ) ) != null &&
												( alternativeSpanishAdditionalDefinitionSpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, oppositeSpecificationWordItem ) ) != null &&
												!isArchivedUserAssignment &&
												isUserRelationWord &&
												existingSpecificationItem != null )
													{
													if( ( primarySpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, userSpecificationCollectionNr, userSpecificationWordItem ) ) != null &&
													// Check if justification already exists
													existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) )
														{
														if( primarySpecificationItem == alternativePrimarySpecificationItem &&
														( tempJustificationItem = foundPrimarySpecificationItem.firstSecondaryJustificationItem( true ) ) != null )
															{
															primarySpecificationItem = tempJustificationItem.secondarySpecificationItem();

															if( existingSpecificationItem.isOlderItem() &&
															// Justification already exists yet in specification. Try to find a non-compound specification
															existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) &&
															( nonCompoundSpecificationCollectionNr = alternativeSpanishAdditionalDefinitionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
															( oppositeSpecificationWordItem = alternativeSpanishAdditionalDefinitionSpecificationItem.specificationWordItem() ) != null &&
															( spanishNonCompoundSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, nonCompoundSpecificationCollectionNr, oppositeSpecificationWordItem ) ) != null &&
															!spanishNonCompoundSpecificationItem.isHiddenSpanishSpecification() )
																primarySpecificationItem = spanishNonCompoundSpecificationItem;
															}
														}
													else
														{
														if( !existingSpecificationItem.hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&
														// Justification has at least the same assumption level
														existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, alternativePrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, currentSpecificationItem, null ) )
															primarySpecificationItem = alternativePrimarySpecificationItem;
														}
													}
												else
													primarySpecificationItem = alternativePrimarySpecificationItem;
												}
											}

										hasPrimarySpecificationRelationWord = ( primarySpecificationItem == null ? false : primarySpecificationItem.hasRelationWord() );
										isPossessivePrimarySpecification = ( primarySpecificationItem == null ? false : primarySpecificationItem.isPossessive() );
										archivedAssignmentItem = null;

										if( isArchivedUserAssignment &&
										isUserDefinitionSpecification )
											{
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										selectedPrimarySpecificationItem = ( ( !isPossessiveUserSpecification ||

																			( primarySpecificationItem != null &&
																			!isPossessivePrimarySpecification &&

																				( !primarySpecificationItem.isSelfGeneratedAssumption() ||
																				!primarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) ? primarySpecificationItem :
																																						// Typical for Spanish
																																						userSpecificationItem );
										selectedSecondarySpecificationItem = ( archivedAssignmentItem == null ? currentSpecificationItem :
																												archivedAssignmentItem );

										if( primarySpecificationItem != null &&
										selectedPrimarySpecificationItem != null &&
										// Justification doesn't exist yet
										generalizationWordItem.foundJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) == null &&

										// No existing specification
										( ( existingSpecificationItem == null &&

											( ( primarySpecificationItem == foundPrimarySpecificationItem &&

												( isCurrentUserSpecification ||
												isUserQuestion ||

												( ( !hasUserSpecificationRelationWord ||
												currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR ) &&

													( isUserGeneralizationWord ||
													isUserRelationWord ||
													// Test files with archived assignments, and "reasoning\family\John - Anna (before family definition)"
													primarySpecificationItem.isOlderItem() ) ) ) ) ||

											// Typical for Spanish
											( isSpanishCurrentLanguage &&
											foundPrimarySpecificationItem.isHiddenSpanishSpecification() &&
											!isCurrentUserSpecification ) ) ) ||

										// Existing specification
										( existingSpecificationItem != null &&
										existingSpecificationItem.isSelfGeneratedSpecification() &&
										!selectedPrimarySpecificationItem.isActiveAssignment() &&
										// Justification doesn't exist yet in specification
										!existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) &&

											( ( ( ( hasPrimarySpecificationRelationWord &&
											isPossessivePrimarySpecification ) ||

											( isUserRelationWord &&

												// Test files: "reasoning\family\Complex (7 - Becky)",
												//				"reasoning\family\Complex (7 - Claudia)",
												//				"reasoning\family\Complex (16)",
												//				"reasoning\family\Complex (17)",
												//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
												( ( hasPrimarySpecificationRelationWord &&
												!existingSpecificationItem.hasSpecificationCollection() ) ||

												// Test files: "reasoning\family\Complex (17)",
												//				"reasoning\family\My assumptions that are confirmed (John and Anna)",
												//				"reasoning\family\My assumptions that are confirmed (John and Anna - reversed)"
												( primarySpecificationItem == foundPrimarySpecificationItem &&
												primarySpecificationItem.hasCurrentCreationSentenceNr() ) ) ) ||

											( !existingSpecificationItem.hasRelationWord() &&

												// No archived assignment
												( ( !isArchivedAssignment &&

													( ( hasPrimarySpecificationRelationWord &&
													!primarySpecificationItem.hasSpecificationCollection() ) ||

													( primarySpecificationItem == foundPrimarySpecificationItem &&

														( primarySpecificationItem.hasCurrentCreationSentenceNr() ||

														// Typical for Spanish
														// Spanish test file: "razonamiento\Reto científico"
														( isSpanishCurrentLanguage &&
														existingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ) ) ) ||

												// Archived assignment
												( isArchivedAssignment &&
												hasPrimarySpecificationRelationWord &&

													// Test file: "reasoning\Scientific challenge"
													( isUserDefinitionSpecification ||
													// Other test files with archived assignments
													!existingSpecificationItem.isOlderItem() ) ) ) ) ) &&

											// Justification has at least the same assumption level
											existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ||

											// Typical for Spanish
											( isSpanishCurrentLanguage &&
											!isPossessivePrimarySpecification &&
											primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
											// Justification doesn't exist yet in specification
											!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&

												// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es masculino"
												( isUserAdjectiveSpecification ||

												// Spanish test files: "razonamiento\familia\Complejo (12)",
												//						"razonamiento\familia\Complejo (15)"
												( isUserRelationWord &&
												primarySpecificationItem != selectedPrimarySpecificationItem ) ) ) ) ) ) )
											{
											selectedRelationWordItem = ( existingSpecificationItem != null &&
																		existingSpecificationItem.hasRelationWord() &&
																		existingSpecificationItem.hasCurrentCreationSentenceNr() ? relationWordItem :

																		// Test file: "reasoning\family\John - Anna (before family definition)"
																		( isExclusiveUserSpecification &&
																		!isSpanishCurrentLanguage &&
																		currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR ? selectedPrimarySpecificationItem.relationWordItem() : null ) );

											selectedRelationWordTypeNr = ( selectedRelationWordItem == null ? Constants.NO_WORD_TYPE_NR : Constants.WORD_TYPE_PROPER_NOUN );

											// Make compound specification substitution assumption
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
												{
												// Test files: "reasoning\family\Complex (3)",
												//				"reasoning\family\Laura - Joe - John and Anna - Paul",
												//				"reasoning\family\Laura - Joe - Paul - John and Anna"
												if( !hasRelationWord &&
												existingSpecificationItem != null &&
												primarySpecificationItem.isUserSpecification() &&
												!createdOrFoundSpecificationItem.isOlderItem() &&
												( tempJustificationItem = createdOrFoundSpecificationItem.firstJustificationItem() ) != null &&
												tempJustificationItem.justificationAssumptionLevel() > createdOrFoundSpecificationItem.assumptionLevel() &&
												// Remove obsolete assumption justifications from currently created or found specification
												createdOrFoundSpecificationItem.removeObsoleteAssumptionJustifications( false ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to remove obsolete assumption justifications from the currently created or found specification" );

												// Typical for Spanish
												// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
												if( isSpanishCurrentLanguage &&
												hasUserSpecificationNonCompoundCollection &&
												hasUserSpecificationRelationWord &&
												!isPossessiveUserSpecification &&
												!isUserSpecificationWordSpanishAmbiguous &&
												simpleUserSpecificationItem != null &&
												( oppositeSpecificationWordItem = currentSpecificationWordItem.collectionWordItem( createdOrFoundSpecificationItem.specificationCollectionNr() ) ) != null &&
												( collectionWordItem = specificationWordItem.collectionWordItem( userSpecificationCollectionNr ) ) != null &&
												( selectedAdditionalDefinitionSpecificationItem = collectionWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) != null &&
												// Draw negative Spanish assumption
												adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, simpleUserSpecificationItem, selectedAdditionalDefinitionSpecificationItem, createdOrFoundSpecificationItem, null, generalizationWordItem, oppositeSpecificationWordItem, null ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative Spanish assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oppositeSpecificationWordItem.anyWordTypeString() + "\"" );
												}
											}

										// Typical for Spanish
										// Spanish test files: "respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)",
										//						"razonamiento\familia\Juan - Ana (antes de la definición de la familia)",
										//						"razonamiento\familia\Juan era el padre de Paz, José y Luisa",
										//						"razonamiento\familia\Tiempo pasado"
										if( isSpecificationWordSpanishAmbiguous &&
										isUserAssignment &&
										isUserGeneralizationWord &&
										hasUserSpecificationCompoundCollection &&
										primarySpecificationItem != null &&
										existingSpecificationItem != null &&
										existingSpecificationItem.hasCurrentCreationSentenceNr() &&

											( isArchivedUserAssignment ||
											currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR ) &&

										// Draw specification substitution conclusion or ask a question
										drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
										}
									else
										{
										if( ( primarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, specificationWordItem ) ) != null &&

										// Non-exclusive
										( ( !isExclusiveSpecification &&

											// Skip if new user specification has relation word, while the old one did not
											( !primarySpecificationItem.hasRelationWord() ||

											( ( existingSpecificationItem == null ||
											// Justification doesn't exist yet in specification
											!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, currentSpecificationItem ) ) &&

											generalizationWordItem.noRelationWordSpecificationItem( false, false, specificationWordItem ) == null ) ) &&

										!isFoundPrimaryHiddenSpanishSpecification &&
										// Skip if already exists as user specification
										generalizationWordItem.firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, currentSpecificationWordItem ) == null ) ||

										// Exclusive
										( isExclusiveSpecification &&

											( !hasCurrentSpecificationCompoundCollection ||
											currentSpecificationWordItem.isFeminineOrMasculineWord() ) &&

										// Exclusive conclusion doesn't exist
										generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, true, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentSpecificationWordItem ) == null ) ) )
											{
											createdSpecificationItem = null;
											createdOrFoundSpecificationItem = existingSpecificationItem;

											if( existingSpecificationItem == null ||

											// Test files: "reasoning\family\Complex (15)",
											//				"reasoning\family\Complex (20)",
											//				"reasoning\family\I know (3)",
											//				among others
											( ( !primarySpecificationItem.isSelfGeneratedConclusion() ||
											generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, null ) == null ) &&

											// Justification doesn't exist yet in specification
											!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, currentSpecificationItem ) &&
											// Justification has at least the same assumption level
											existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, currentSpecificationItem, null ) ) )
												{
												// Draw compound specification substitution conclusion
												if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

												if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
													{
													if( createdOrFoundSpecificationItem.isCorrectedSpecification() )
														{
														// Test file: "reasoning\family\Correcting invalidated assumption (by knowledge)"
														// Correct invalid suggestive assumptions by knowledge
														if( correctSuggestiveAssumptionsByKnowledge( createdOrFoundSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to correct invalid suggestive assumptions by knowledge about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null &&
														// Remove obsolete assumption justifications from created or found specification
														createdOrFoundSpecificationItem.removeObsoleteAssumptionJustifications( false ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to remove obsolete assumption justifications from the older created or found specification" );
														}
													}
												}

											if( isGeneralizationProperNoun &&
											createdOrFoundSpecificationItem != null )
												{
												// Adjective
												if( currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE )
													{
													if( ( ( negativeConclusionSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, primarySpecificationItem.specificationCollectionNr(), null ) ) == null ||

													( !negativeConclusionSpecificationItem.hasCompoundSpecificationCollection() &&

														// Justification doesn't exist yet in specification
														( !negativeConclusionSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) ||

														// Typical for Spanish
														// Spanish test files: "respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)",
														//						"razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
														( isUserSpecificationWordSpanishAmbiguous &&
														isUserAssignment &&
														createdOrFoundSpecificationItem.isOlderItem() ) ) ) ||

													( isUserAssignment &&
													simpleUserSpecificationItem == primarySpecificationItem &&
													createdOrFoundSpecificationItem.isOlderItem() &&
													userSpecificationItem.hasRelationCollection() &&

														// Test file: "question answering\family\Just a few questions (1)"
														( createdOrFoundSpecificationItem.nJustificationItems() == 1 ||
														// Typical for Spanish
														// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
														userSpecificationCollectionNr == Constants.NO_COLLECTION_NR ) ) ) &&

													// Draw negative adjective conclusions
													drawNegativeAdjectiveConclusions( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative adjective conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( !isExclusiveUserSpecification &&
													!isUserAssignment &&
													userSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
													createdSpecificationItem != null &&

														// Test files: "reasoning\family\Complex (14)",
														//				"reasoning\family\I know (9)",
														//				"reasoning\family\I know (10)"
														( !hasUserSpecificationRelationWord ||
														// Typical for Spanish
														// Spanish test files: "razonamiento\familia\Complejo (2)",
														//						"razonamiento\familia\Complejo (12)",
														//						"razonamiento\familia\Complejo (15)"
														isUserSpecificationWordSpanishAmbiguous ) &&

													// Avoid idle call
													generalizationWordItem.firstSpecificationItem( false, false, isArchivedUserAssignment, true ) != null &&
													// Make indirectly answered question assumption
													makeIndirectlyAnsweredQuestionAssumption( isArchivedUserAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( ( !isPossessiveUserSpecification ||
												primarySpecificationItem.isSelfGeneratedAssumption() ) &&

												primarySpecificationItem.assumptionLevel() <= createdOrFoundSpecificationItem.assumptionLevel() )
													{
													// Avoid creating duplicate negative justifications
													if( ( ( selfGeneratedSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, Constants.NO_QUESTION_PARAMETER, userSpecificationCollectionNr, specificationWordItem ) ) == null ||
													!selfGeneratedSpecificationItem.isOlderItem() ) &&

													// Avoid idle call
													specificationWordItem.firstNegativeSpecificationItem() != null &&
													// Draw simple negative conclusions
													drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( hasUserSpecificationCompoundCollection &&
													isPossessiveUserSpecification &&
													existingSpecificationItem != null &&
													userSpecificationWordItem != null )
														{								// Efficiency: Select currentSpecificationItem is when specification words are the same
														definitionSpecificationItem = ( specificationWordItem == userSpecificationWordItem ? currentSpecificationItem :
																																			userSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, currentSpecificationWordItem ) );

														existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

														// Justification doesn't exist yet in specification
														if( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) &&
														// Justification has at least the same assumption level
														existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, definitionSpecificationItem, null ) &&
														// Add specification substitution justification to existing specification
														existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, definitionSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the existing specification" );
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
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextNonNegativeNonPosessiveDefinitionSpecificationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawSpecificationSubstitutionConclusionOrAskQuestion++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawSpecificationSubstitutionConclusionOrAskQuestion++;
			}

		return Constants.RESULT_OK;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isChineseCurrentLanguage, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpanishCurrentLanguage, boolean isUncountableGeneralizationNoun, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasExistingSpecificationCurrentCreationSentenceNr;
		boolean hasExistingSpecificationRelationWord;
		boolean hasFeminineOrMasculineProperNounEnding;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		boolean hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification = false;
		boolean hasRelationWordFeminineOrMasculineProperNounEnding = false;
		boolean hasRelationWordMasculineProperNounEnding;
		boolean hasSecondarySpecificationRelationWord;
		boolean isDrawingPossessiveReversibleConclusions;
		boolean isExistingHiddenSpanishSpecification;
		boolean isExistingSelfGeneratedAssumption;
		boolean isHiddenPrimarySpecification;
		boolean isHiddenSecondarySpecification;
		boolean isOlderExistingSpecification;
		boolean isOlderPrimarySpecification;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isPrimarySpecificationAssignment;
		boolean isPrimaryUserSpecification;
		boolean isPrimarySelfGeneratedSpecification;
		boolean isUserRelationWord;
		short prepositionParameter;
		short selectedJustificationTypeNr;
		short selectedRelationWordTypeNr;
		int specificationCollectionNr;
		JustificationItem firstJustificationItem;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem additionalSpecificationItem;
		SpecificationItem compoundSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentExistingSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem possessiveSpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem = null;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem selectedAdditionalProperNounSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem commonWordItem;
		WordItem currentPossessiveNounWordItem;
		WordItem feminineSpecificationWordItem = null;
		WordItem oppositePossessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		oppositePossessiveDefinitionSpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, relationWordItem );

		// Typical for Spanish
		if( isSpanishCurrentLanguage &&
		hasRelationWord &&
		isPossessive &&
		primarySpecificationItem != null &&
		primarySpecificationItem.hasCompoundSpecificationCollection() &&
		primarySpecificationItem.isSelfGeneratedSpecification() &&
		specificationWordItem.isNounWordSpanishAmbiguous() &&
		( specificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
		( tempSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( isNegative, true, specificationCollectionNr, specificationWordItem ) ) != null &&
		tempSpecificationItem != primarySpecificationItem &&
		tempSpecificationItem.hasRelationWord() )
			// Spanish non-compound specification
			primarySpecificationItem = tempSpecificationItem;

		if( primarySpecificationItem != null )
			{
			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isPrimarySelfGeneratedSpecification = primarySpecificationItem.isSelfGeneratedSpecification();
			isPrimarySpecificationAssignment = primarySpecificationItem.isAssignment();
			isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();

			if( ( !isOlderPrimarySpecification ||
			isPrimarySpecificationAssignment ||

			( isPrimarySelfGeneratedSpecification &&
			relationWordItem != null ) ) &&

			( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
				{
				hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
				hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
				hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem.hasCurrentlyMoreSpecificSpecification();
				hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem.hasMasculineProperNounEnding();
				isUserRelationWord = generalizationWordItem.isUserRelationWord;

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
						// Find possessive specifications
						if( ( findSpecificationResult = findPossessiveSpecifications( currentPossessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );

						additionalDefinitionSpecificationItem = null;
						createdOrFoundSpecificationItem = null;
						possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem;
						oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem;

						if( possessiveDefinitionSpecificationItem != null &&
						( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
							{
							possessiveDefinitionSpecificationItem_ = possessiveDefinitionSpecificationItem;
							oppositePossessiveDefinitionSpecificationItem_ = oppositePossessiveDefinitionSpecificationItem;

							if( !isPossessive &&
							( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
								{
								isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

								if( ( !primarySpecificationItem.hasRelationWord() &&
								generalizationWordItem.hadOnceAnsweredSelfGeneratedQuestion() ) ||

								( isPossessiveSpecificationWordSpanishAmbiguous &&
								primarySpecificationItem.specificationWordItem() == possessiveDefinitionSpecificationWordItem ) )
									{
									if( ( isArchivedAssignment ||
									primarySpecificationItem.isOlderItem() ||
									primarySpecificationItem.isSelfGeneratedConclusionOrAssumption() ) &&

									// Draw Spanish specification substitution conclusion
									drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( ( existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, possessiveDefinitionSpecificationWordItem ) ) == null ||

									( hasRelationWord &&

										// Confirmed specification without relation word
										( !existingUserSpecificationItem.hasRelationWord() ||
										// Test file: "reasoning\family\John - Anna (before family definition)"
										!existingUserSpecificationItem.hasSpecificationCollection() ) ) )
										{
										// Test files: "reasoning\family\I know (11)",
										//				"reasoning\family\Paul is a boy - Laura is a girl"
										if( !hasRelationWord &&
										( possessiveSpecificationWordItem = possessiveSpecificationItem.specificationWordItem() ) != null &&
										( commonWordItem = possessiveSpecificationWordItem.commonWordItem( possessiveSpecificationItem.specificationCollectionNr() ) ) != null &&
										commonWordItem != generalizationWordItem &&
										commonWordItem.isFeminineOrMasculineWord() &&
										!commonWordItem.isNounWordSpanishAmbiguous() &&
										( definitionSpecificationItem = commonWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, possessiveSpecificationWordItem ) ) != null &&
										// Make generalization assumption
										adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, definitionSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + commonWordItem.anyWordTypeString() + "\"" );

										// User specification doesn't exist yet
										if( generalizationWordItem.firstUserSpecificationItem( !isNegative, isPossessive, Constants.NO_COLLECTION_NR, possessiveDefinitionSpecificationWordItem ) == null )
											{
											// Specification doesn't exist yet
											if( ( currentExistingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, isPossessive, false, possessiveDefinitionSpecificationWordItem ) ) == null ||

											( relationWordItem != null &&

												// No user relation word
												( ( !isUserRelationWord &&

													( ( isPrimarySpecificationAssignment &&
													!isOlderPrimarySpecification &&

														( currentExistingSpecificationItem.isSelfGeneratedAssumption() ||

														// Test file: "reasoning\family\Complex (8)"
														( !currentExistingSpecificationItem.hasRelationCollection() &&
														currentExistingSpecificationItem.isSelfGeneratedSpecification() &&
														currentExistingSpecificationItem.relationWordItem() == relationWordItem ) ) ) ||

													// Different relation word
													( currentExistingSpecificationItem.relationWordItem() != relationWordItem &&

														( !isPrimarySpecificationAssignment ||
														!currentExistingSpecificationItem.hasRelationCollection() ||
														relationWordItem.firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) == null ) ) ||

													// Typical for Spanish
													( isPossessiveSpecificationWordSpanishAmbiguous &&

														// Spanish test files: "razonamiento\familia\Complejo (7 - Bonita)",
														//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa"
														( hasGeneralizationWordCurrentlyConfirmedSpecification ||
														// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
														currentExistingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ) ) ||

												// User relation word
												( isUserRelationWord &&

													( ( tempSpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) ) == null ||

													// Test files: "reasoning\family\Complex (3)",
													//				"reasoning\family\Complex (7 - Becky)",
													//				"reasoning\family\Complex (7 - Claudia)",
													//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
													( isPrimarySpecificationAssignment &&
													tempSpecificationItem.isSelfGeneratedSpecification() ) ||

													// Test files: "reasoning\family\Complex (8)",
													//				"reasoning\family\I know (10)",
													//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
													( !tempSpecificationItem.hasCurrentCreationSentenceNr() &&
													!currentExistingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ) ) ) )
												{
												prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
												selectedRelationWordTypeNr = Constants.NO_WORD_TYPE_NR;
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

												if( hasRelationWord )
													{
													prepositionParameter = Constants.WORD_PARAMETER_PREPOSITION_OF;
													selectedRelationWordTypeNr = relationWordTypeNr;
													}

												if( ( selectedSecondarySpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) == null )
													{
													// Typical for Spanish
													// Make Spanish exclusive specification substitution assumption
													if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, prepositionParameter, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, possessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make a Spanish exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

													createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
													}
												else
													{
													// Make specification substitution assumption
													if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, prepositionParameter, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, selectedSecondarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make a specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

													if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
													// Justification doesn't exist yet in specification
													!createdOrFoundSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
													// Justification has at least the same assumption level
													createdOrFoundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
													// Add user specification substitution justification to created or found specification
													createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a user specification substitution justification to the created or found specification" );
													}

												// Typical for Spanish
												// Spanish test files: "Contestador inteligente de preguntas",
												//						"razonamiento\familia\Mis suposiciones que se confirman (hijos)",
												//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa",
												//						among others
												if( hasRelationWord &&
												isPrimaryUserSpecification &&
												existingUserSpecificationItem == null &&
												createdOrFoundSpecificationItem != null &&
												createdOrFoundSpecificationItem.hasCurrentCreationSentenceNr() &&
												createdOrFoundSpecificationItem.isOlderItem() &&
												createdOrFoundSpecificationItem.isHiddenSpanishSpecification() &&
												!primarySpecificationItem.isActiveAssignment() &&
												// Add user specification to justifications of hidden Spanish specification
												createdOrFoundSpecificationItem.addUserSpecificationToSpanishJustifications( primarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add the user specification to justifications of the hidden Spanish specification" );
												}
											else
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

											if( createdOrFoundSpecificationItem != null )
												{
												secondarySpecificationItem = createdOrFoundSpecificationItem.updatedSpecificationItem();

												// Typical for Spanish
												// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
												if( isPossessiveSpecificationWordSpanishAmbiguous &&
												hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation &&
												// Draw simple negative conclusions
												drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, adminItem_.simpleUserSpecificationItem(), userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								}
							}
						else
							{
							// Test files: "reasoning\Boiling point",
							//				"reasoning\Condensation point"
							if( hasRelationWord &&
							( foundSpecificationItem = currentPossessiveNounWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
							// Uncountable generalization noun. Example: "Water ..."
							adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationAssignment, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );
							}

						// First opposite possessive specification
						if( possessiveDefinitionSpecificationWordItem != null &&
						possessiveDefinitionSpecificationItem != null &&
						oppositePossessiveDefinitionSpecificationItem != null &&
						( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
							{
							if( createdOrFoundSpecificationItem == null ||
							primarySpecificationItem.hasSpecificationContext() )
								{
								secondarySpecificationItem = primarySpecificationItem;

								// Find additional definition definition
								if( !hasRelationWord ||
								isPossessive )
									additionalDefinitionSpecificationItem = possessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem );
								}

							if( !isHiddenPrimarySpecification &&
							secondarySpecificationItem != null )
								{
								hasSecondarySpecificationRelationWord = secondarySpecificationItem.hasRelationWord();

								// Test file: "reasoning\family\Context - Specification pronoun"
								if( !hasSecondarySpecificationRelationWord ||
								// Test files: "reasoning\family\Complex (19 - strange)",
								//				"reasoning\family\Complex (7 - Becky)"
								isPrimarySelfGeneratedSpecification ||

								( !isOlderPrimarySpecification &&

									( !secondarySpecificationItem.isUserAssignment() ||
									// Typical for Spanish
									// Spanish test files: "conflicto\familia\José es una madre",
									//						"respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)"
									secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) )
									{
									hasExistingSpecificationCurrentCreationSentenceNr = false;
									hasExistingSpecificationRelationWord = false;
									isExistingHiddenSpanishSpecification = false;
									isExistingSelfGeneratedAssumption = false;
									isOlderExistingSpecification = false;
									isHiddenSecondarySpecification = secondarySpecificationItem.isHiddenSpanishSpecification();
									existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									existingSpecificationItem != null &&
									!isHiddenSecondarySpecification &&
									hasGeneralizationWordCurrentlyConfirmedSpecification &&
									existingSpecificationItem.hasCompoundSpecificationCollection() &&
									( tempSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, oppositePossessiveDefinitionSpecificationItem.specificationCollectionNr(), oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
									tempSpecificationItem.hasOnlyOneRelationWord() )
										existingSpecificationItem = tempSpecificationItem;

									if( existingSpecificationItem != null )
										{
										// Assign some variables
										hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem.hasCurrentCreationSentenceNr();
										hasExistingSpecificationRelationWord = existingSpecificationItem.hasRelationWord();
										isExistingHiddenSpanishSpecification = existingSpecificationItem.isHiddenSpanishSpecification();
										isExistingSelfGeneratedAssumption = existingSpecificationItem.isSelfGeneratedAssumption();
										isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
										}

									hasRelationWordMasculineProperNounEnding = false;
									isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

									// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										primarySpecificationItem = primarySpecificationItem.updatedSpecificationItem();

									if( hasRelationWord &&
									isPossessive &&
									generalizationWordItem.hasMasculineProperNounEnding() &&
									( tempSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
										{
										// Masculine proper name ending
										// Spanish test files: "razonamiento\Reto científico",
										//						"razonamiento\familia\Juan - Ana - Pablo, José y Luisa"
										//						"razonamiento\familia\Sé (1)"
										hasRelationWordMasculineProperNounEnding = true;
										selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
										selectedPrimarySpecificationItem = primarySpecificationItem;
										selectedAdditionalDefinitionSpecificationItem = possessiveDefinitionSpecificationItem;
										selectedSecondarySpecificationItem = tempSpecificationItem;
										}
									else
										{
										selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
										selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem;
										selectedAdditionalDefinitionSpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? null : additionalDefinitionSpecificationItem );

										selectedSecondarySpecificationItem = ( isHiddenSecondarySpecification ? primarySpecificationItem :
																												secondarySpecificationItem );
										}

									if( existingSpecificationItem == null ||

									( hasSecondarySpecificationRelationWord &&

										// Non-possessive
										( ( !isPossessive &&

											// Not user relation
											( ( !isUserRelationWord &&

												( ( isExistingSelfGeneratedAssumption &&

													( ( hasExistingSpecificationRelationWord &&

														// Test file: "reasoning\family\Complex (6)"
														( ( !isOlderExistingSpecification &&
														existingSpecificationItem.hasOnlyOneRelationWord() ) ||

														( isPrimarySpecificationAssignment &&

															// Justification doesn't exist yet in specification
															// Test files: "reasoning\family\Joe has 2 parents - Joe is a child",
															//				"reasoning\family\Joe is a child - Joe has 2 parents",
															//				"reasoning\family\John - Anna (before family definition)"
															( ( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&

															existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ||

															// Typical for Spanish
															// Spanish test files: "razonamiento\familia\Luisa - José - Ana",
															//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
															isOppositePossessiveSpecificationWordSpanishAmbiguous ) ) ) ) ||

													// Test files: "reasoning\family\Complex (4)",
													//				"reasoning\family\Complex (12)",
													//				"reasoning\family\Complex (16)",
													//				"reasoning\family\Complex (18)",
													//				"reasoning\family\Complex (19 - mixed)",
													//				among others
													( isOlderExistingSpecification &&
													!isPrimarySpecificationAssignment &&
													existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ) ) ||

												( hasGeneralizationWordCurrentlyConfirmedSpecification &&

													// Not hidden existing
													( ( !isExistingHiddenSpanishSpecification &&

														// Test files: "reasoning\family\Complex (17)",
														//				"reasoning\family\Laura has a father - Laura is a child - John and Anna",
														//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
														( ( !hasExistingSpecificationCurrentCreationSentenceNr &&
														existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
														existingSpecificationItem.hasOnlyOneRelationWord() &&
														existingSpecificationItem.isSelfGeneratedConclusion() &&
														selectedSecondarySpecificationItem.hasRelationCollection() &&
														( firstJustificationItem = selectedSecondarySpecificationItem.firstJustificationItem() ) != null &&
														firstJustificationItem.isReversibleAssumptionOrConclusion() ) ||

														// Typical for Spanish
														( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

															// Not primary assignment
															( ( !isPrimarySpecificationAssignment &&

																// Spanish test files: "razonamiento\familia\Juan - Luisa - Juan y Ana",
																//						"razonamiento\familia\Juan - Paz - Luisa"
																( secondarySpecificationItem.hasOnlyOneRelationWord() ||

																// Spanish test files: "razonamiento\familia\Complejo (6)",
																//						"razonamiento\familia\Complejo (11)",
																//						"razonamiento\familia\Complejo (20)",
																//						among others
																( existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
																existingSpecificationItem.hasOnlyOneRelationWord() ) ) ) ||

															// Primary assignment
															( isPrimarySpecificationAssignment &&

																// Spanish test files: "razonamiento\familia\Complejo (9)",
																//						"razonamiento\familia\Complejo (16)",
																//						among others
																( !isHiddenSecondarySpecification ||
																// Spanish test files: "razonamiento\familia\Complejo (9)",
																//						"razonamiento\familia\Complejo (20)",
																//						among others
																secondarySpecificationItem.nRelationWords() < primarySpecificationItem.nRelationWords() ) ) ) ) ) ) ||

													// Hidden existing
													// Spanish test files: "razonamiento\familia\Complejo (14)",
													//						"razonamiento\familia\Sé (11)",
													//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz",
													//						"razonamiento\familia\Paz - Ana - José - Luisa - Juan"
													( isExistingHiddenSpanishSpecification &&
													!hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
													existingSpecificationItem.hasOnlyOneRelationWord() ) ) ) ) ) ||

												// User relation
												( isUserRelationWord &&
												// Typical for Spanish
												isOppositePossessiveSpecificationWordSpanishAmbiguous &&
												isExistingSelfGeneratedAssumption &&

													// Typical for Spanish
													// Spanish test files: "razonamiento\familia\Complejo (3)",
													//						"razonamiento\familia\Complejo (12)"
													( isExistingHiddenSpanishSpecification ||

													// Spanish test files: "razonamiento\familia\Complejo (7 - Becky)",
													//						"razonamiento\familia\Complejo (7 - Bonita)",
													//						"razonamiento\familia\Definición sencilla de la familia - Complejo (7 - Bonita)"
													isPrimarySpecificationAssignment ) ) ) ) ||

										// Possessive
										( isPossessive &&
										isExistingSelfGeneratedAssumption &&

											// No additional definition
											// Test file: "reasoning\family\John is a parent - Paul is a child of John and Anna"
											( ( additionalDefinitionSpecificationItem == null &&

												// Not older existing specification
												( ( !isOlderExistingSpecification &&

													// Test files: "reasoning\family\John is a parent - Paul is a child of John and Anna",
													//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
													( !hasRelationWordFeminineOrMasculineProperNounEnding ||
													// Test files: "reasoning\family\Complex (11)",
													//				"reasoning\family\Complex (17)",
													//				"reasoning\family\Complex (18)",
													//				"reasoning\family\Complex (19 - mixed)",
													//				"reasoning\family\Complex (19 - strange)",
													//				"reasoning\family\I know (12)",
													//				among others
													generalizationWordItem.firstActiveQuestionSpecificationItem() != null ||

													// Test file: "reasoning\family\Complex (17)"
													( ( tempSpecificationItem = generalizationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null &&
													tempSpecificationItem.isOlderItem() ) ) ) ||

												// Older existing specification
												( isOlderExistingSpecification &&
												// Test files: "reasoning\family\Complex (4)",
												//				"reasoning\family\Complex (5)",
												//				"reasoning\family\Complex (16)",
												//				"reasoning\family\Complex (19 - strange)",
												//				"reasoning\family\I know (3)",
												//				"reasoning\family\You have confirmed at least one relation word",
												//				among others
												!hasRelationWordConfirmedSpecification &&
												!secondarySpecificationItem.isSelfGeneratedConclusion() ) ) ) ||

											// Additional definition
											( additionalDefinitionSpecificationItem != null &&
											// Justification doesn't exist yet in specification
											!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) ) ) &&

										existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ) ) )
										{
										selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR );
										createdSpecificationItem = existingSpecificationItem;
										foundSpecificationItem = existingSpecificationItem;

										if( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&

											// Test files: "reasoning\family\Context - Specification pronoun",
											//				"reasoning\family\Laura - John and Anna - Joe"
											( !hasRelationWord ||

											// Non-possessive
											( !isPossessive &&
											selectedPrimarySpecificationItem.specificationWordItem() == selectedSecondarySpecificationItem.specificationWordItem() &&

												( !hasGeneralizationWordCurrentlyConfirmedSpecification ||

												// Test files: "reasoning\family\Complex (12)",
												//				"reasoning\family\Complex (16)",
												//				"reasoning\family\Complex (17)",
												//				among others
												( existingSpecificationItem != null &&
												( firstJustificationItem = existingSpecificationItem.firstJustificationItem() ) != null &&
												firstJustificationItem.hasPrimarySpecificationRelationCollection() ) ) ) ||

											// Possessive
											( isPossessive &&

												( hasExistingSpecificationCurrentCreationSentenceNr ||
												!hasGeneralizationWordCurrentlyConfirmedSpecification ) ) ) ) ||

										// Typical for Spanish
										( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

											// Spanish test file: "razonamiento\familia\Complejo (3)"
											( !isPossessive ||
											// Spanish test files: "razonamiento\Reto científico",
											//						"razonamiento\familia\Sé (1)",
											//						"respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es ..."
											existingSpecificationItem == null ||

											( hasExistingSpecificationCurrentCreationSentenceNr &&

												// Spanish test file: "razonamiento\familia\Complejo (3)"
												( isPrimaryUserSpecification ||
												// Spanish test file: "Ana - Juan - Paz, José y Luisa"
												!selectedSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ||
												// Spanish test file: "Ana - Juan - Paz, José y Luisa"
												!existingSpecificationItem.hasOnlyOneRelationWord() ||
												GlobalVariables.nUserGeneralizationWords > 1 ) ) ) ) )
											{
											// Make opposite possessive specification assumption or specification substitution assumption
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, possessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption or a specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
											foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
											}

										if( relationWordItem != null )
											{
											// Non-possessive
											if( !isPossessive )
												{
												if( existingSpecificationItem != null )
													{
													// Not created specification
													if( createdSpecificationItem == null )
														{
														if( existingSpecificationItem.isReplacedItem() )
															{
															// Replaced. So, update required
															existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

															if( !isSpanishCurrentLanguage &&
															( tempSpecificationItem = relationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
															tempSpecificationItem.isSelfGeneratedConclusion() )
																{
																// Test files: "reasoning\family\Complex (12)",
																//				"reasoning\family\Complex (16)"
																// Draw reversible conclusion
																if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, existingSpecificationItem, null, null, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
																	return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															{
															// Test files: "reasoning\family\Joe has 2 parents - Joe is a child",
															//				"reasoning\family\Joe is a child - Joe has 2 parents"
															if( hasExistingSpecificationCurrentCreationSentenceNr &&
															isPrimarySpecificationAssignment &&
															!selectedSecondarySpecificationItem.isOlderItem() &&
															!selectedSecondarySpecificationItem.isUserSpecification() &&
															( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != null &&
															// Justification doesn't exist yet in specification
															!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, additionalSpecificationItem ) &&
															// Add additional opposite possessive conclusion justification to existing specification
															existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, additionalSpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add an additional opposite possessive conclusion justification to the existing specification" );
															}
														}
													else	// Created specification
														{
														// Test file: "reasoning\family\Complex (19 - mixed)"
														if( !hasExistingSpecificationRelationWord &&
														!isOppositePossessiveSpecificationWordSpanishAmbiguous &&
														( foundSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( false, false, Constants.NO_COLLECTION_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
														// Draw reversible conclusion
														adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													}

												if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
												( selectedPrimarySpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null )
													{
													// Test file: "reasoning\family\John - Anna (before family definition)"
													if( ( existingSpecificationItem == null ||
													selectedPrimarySpecificationItem.assumptionLevel() < existingSpecificationItem.assumptionLevel() ) &&

													( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPartOfSpecificationItem( possessiveDefinitionSpecificationWordItem ) ) != null &&
													// Justification doesn't exist yet in part-of specification
													( ( partOfSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentPossessiveNounWordItem ) ) != null &&
													!partOfSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, partOfDefinitionSpecificationItem ) &&
													// Justification has at least the same assumption level
													partOfSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, null, partOfDefinitionSpecificationItem, null ) ) &&
													// Add part-of justification to part-of specification
													partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, null, partOfDefinitionSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a part-of justification to a part-of specification" );

													// Test file: "reasoning\family\Complex (4)"
													// Existing specification
													if( hasGeneralizationWordCurrentlyConfirmedSpecification &&
													!isPrimarySpecificationAssignment &&
													primarySpecificationItem.hasRelationCollectionCurrentlyBeenUpdated() &&
													existingSpecificationItem != null &&
													existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != null &&
													// Justification doesn't exist yet in specification
													!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, selectedPrimarySpecificationItem ) &&
													// Justification has at least the same assumption level
													existingSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, selectedPrimarySpecificationItem, null ) &&
													// Make opposite possessive specification assumption
													adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationItem, null, selectedPrimarySpecificationItem, null, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else	// Possessive
												{
												if( !isOppositePossessiveSpecificationWordSpanishAmbiguous )
													{
													if( foundSpecificationItem != null )
														{
														if( additionalDefinitionSpecificationItem == null )
															{
															// No additional definition
															if( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
															hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation ) &&

															( additionalSpecificationItem = relationWordItem.bestMatchingRelationSpecificationItem( false, isArchivedAssignment, false, true, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
															// Add opposite possessive conclusion justification to additional specification
															additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the additional specification" );
															}
														else	// Additional definition
															{
															if( ( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, null ) ) != null &&
															( selectedSecondarySpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != null &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
															// Add specification substitution justification to additional specification
															additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );
															}
														}
													}
												else	// Typical for Spanish
													{
													if( isPrimarySelfGeneratedSpecification &&
													( additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
													( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, true, false, true, additionalDefinitionSpecificationItem.specificationCollectionNr(), specificationWordItem ) ) != null )
														{
														hasFeminineOrMasculineProperNounEnding = false;

														if( isFeminineOrMasculineProperNounEnding( false, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) )
															{
															hasFeminineOrMasculineProperNounEnding = true;
															
															if( !hasGeneralizationWordMasculineProperNounEnding &&
															existingSpecificationItem != null &&
															existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) == null &&
															( feminineSpecificationWordItem = oppositePossessiveDefinitionSpecificationWordItem.collectionWordItem( additionalDefinitionSpecificationItem.specificationCollectionNr() ) ) != null &&
															// Specification doesn't exist yet
															( foundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) != null &&
															// Justification doesn't exist yet in specification
															!foundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, additionalDefinitionSpecificationItem ) &&
															// Justification has at least the same assumption level
															foundSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem, oppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, null ) &&
															// Make exclusive Spanish feminine specification substitution assumption
															adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, oppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive Spanish feminine specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
															}

														if( compoundSpecificationItem != primarySpecificationItem )
															{
															if( hasFeminineOrMasculineProperNounEnding )
																{
																selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
																selectedPrimarySpecificationItem = compoundSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem;
																selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;
																}
															else
																{
																selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
																selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = null;
																selectedSecondarySpecificationItem = compoundSpecificationItem;
																}

															// Make compound specification exclusive specification substitution assumption (opposite)
															if( adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, false, false, false, false, false, isNegative, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

															if( feminineSpecificationWordItem != null &&
															adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isNegative, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : oppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? oppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNounEnding ? additionalDefinitionSpecificationItem : compoundSpecificationItem ), null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make a Spanish feminine compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
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
					}
				while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );

				// No conflict found
				if( !GlobalVariables.hasDisplayedWarning &&
				primarySpecificationItem != null &&
				secondarySpecificationItem != null &&
				relationWordItem != null )
					{
					// Not user relation word
					if( ( !isUserRelationWord &&
					!userSpecificationItem.isOlderItem() &&

						( ( primarySpecificationItem == userSpecificationItem &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							isPossessive ) ) ||

						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)",
						//						"razonamiento\familia\Que haya confirmado al menos una palabra de relación"
						( isSpanishCurrentLanguage &&
						!isPossessive ) ) ) ||

					// User relation word
					( isUserRelationWord &&

						// Non-possessive
						( ( !isPossessive &&
						primarySpecificationItem != secondarySpecificationItem ) ||

						// Possessive
						// Test files: "reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)"
						( isPossessive &&
						hasPrimarySpecificationCompoundCollection &&
						!hasRelationWordConfirmedSpecification &&
						!isOlderPrimarySpecification &&
						!userSpecificationItem.isAssignment() ) ) ) )
						{											// Non-possessive
						isDrawingPossessiveReversibleConclusions = ( ( !isPossessive &&
																	hasPrimarySpecificationCompoundCollection &&
																	!hasRelationWordConfirmedSpecification &&
																	isUserRelationWord ) ||

																	// Possessive
																	( isPossessive &&
																	!isUserRelationWord &&

																		// Test files: "reasoning\family\Complex (2)",
																		//				"reasoning\family\Complex (3)",
																		//				"reasoning\family\Complex (16)",
																		//				among others
																		( hasGeneralizationWordCurrentlyConfirmedSpecification ||
																		// Test file: "reasoning\family\You have confirmed at least one relation word"
																		primarySpecificationItem.hasOnlyOneRelationWord() ||
																		// Typical for Spanish
																		// Spanish test file: "razonamiento\familia\Paz - José - Luisa - Juan y Ana (padres)"
																		isSpanishCurrentLanguage ) ) );

						// Draw reversible conclusions
						if( drawReversibleConclusions( isArchivedAssignment, isChineseCurrentLanguage, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + relationWordItem.anyWordTypeString() + "\"" );
						}

					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					primarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&
					primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
					( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, specificationWordItem.compoundCollectionNr(), primarySpecificationItem.relationCollectionNr(), specificationWordItem, primarySpecificationItem.relationWordItem() ) ) != null )
						primarySpecificationItem = compoundSpecificationItem;

					selectedAdditionalProperNounSpecificationItem = ( isHiddenPrimarySpecification ? userSpecificationItem :
																									primarySpecificationItem );

					if( ( ( ( primarySpecificationItem == userSpecificationItem ||

					( isUserRelationWord &&

						( !isPossessive ||

						// Test files: "reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)"
						( primarySpecificationItem.isOlderItem() &&
						primarySpecificationItem.isSelfGeneratedAssumption() ) ) ) ) &&

					!secondarySpecificationItem.hasCompoundSpecificationCollection() ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&

						( isUserRelationWord ||
						primarySpecificationItem == userSpecificationItem ) ) ) &&

					// Make an opposite possessive specification assumption (based on secondary specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, isSpanishCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, selectedAdditionalProperNounSpecificationItem, userSpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on secondary specification) during the making an exclusive specification substitution assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

					if( hasPrimarySpecificationCompoundCollection &&
					existingSpecificationItem != null &&
					primarySpecificationItem == secondarySpecificationItem &&

						( ( !isSpanishCurrentLanguage &&
						isPossessive &&
						existingSpecificationItem.isOlderItem() &&

							// No user relation word
							// Test file: "reasoning\family\I know (10)"
							( ( !isUserRelationWord &&
							existingSpecificationItem.isSelfGeneratedAssumption() ) ||

							// User relation word
							// Test files: "reasoning\family\Complex (7 - Becky)",
							//				"reasoning\family\Joe has 2 parents - Joe is a child",
							//				"reasoning\family\Joe is a child - Joe has 2 parents",
							//				"reasoning\family\Paul - Joe - Laura - John and Anna (parents)"
							( isUserRelationWord &&
							!existingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ) ||

						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Complejo (5)",
						//						"razonamiento\familia\Complejo (6)",
						//						"razonamiento\familia\Complejo (11)",
						//						"razonamiento\familia\Complejo (19 - mezclado)"
						( isSpanishCurrentLanguage &&
						!isPossessive &&
						!existingSpecificationItem.isOlderItem() &&
						existingSpecificationItem.isHiddenSpanishSpecification() ) ) &&

					// Make an opposite possessive specification assumption (based on existing specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, isSpanishCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, existingSpecificationItem, ( isHiddenPrimarySpecification ? userSpecificationItem : primarySpecificationItem ), userSpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on existing specification) during the making an exclusive specification substitution assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_makeExclusiveSpecificationSubstitutionAssumption++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_makeExclusiveSpecificationSubstitutionAssumption++;
			}

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumption( boolean isArchivedAssignment, boolean isChineseCurrentLanguage, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isFeminineOrMasculineGeneralizationWord;
		boolean isUserGeneralizationWord;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( !Item.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem == specificationWordItem )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item and specification word item are the same" );

		// Get first noun word specification generalization
		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem.isFeminineOrMasculineWord();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				// Specification doesn't exist yet
				generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem ) == null &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null &&
				primarySpecificationItem.isExclusiveSpecification() )
					{
					// Test file: "reasoning\Knowledge of organisms (1)"
					if( ( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||

					( !currentGeneralizationWordItem.hasCollection() &&

						( !isFeminineOrMasculineGeneralizationWord ||
						primarySpecificationItem.isSelfGeneratedSpecification() ) ) ||

					// Typical for Chinese: No plural form
					// Test file: "reasoning\Knowledge of organisms (1)"
					( isChineseCurrentLanguage &&
					primarySpecificationItem.hasCompoundSpecificationCollection() &&

						( ( foundSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) == null ||
						foundSpecificationItem.hasSpecificationCollection() ) ) ) )
						{
						secondarySpecificationItem = null;

						if( !isFeminineOrMasculineGeneralizationWord &&
						isUserGeneralizationWord &&
						userSpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() )
							// Test files: "reasoning\Knowledge of organisms (1)",
							//				"reasoning\family\Simple family definition - John is the mother of Paul",
							//				"reasoning\family\Simple family definition - John is a father"
							secondarySpecificationItem = userSpecificationItem;
						else
							{
							if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( isArchivedAssignment, false, false, specificationWordItem ) ) != null )
								secondarySpecificationItem = foundSpecificationItem;
							}

						if( secondarySpecificationItem != null &&
						// Make generalization assumption
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_makeGeneralizationAssumption++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_makeGeneralizationAssumption++;
			}

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isArchivedAssignment, boolean isSpanishCurrentLanguage, short generalizationWordTypeNr, SpecificationItem adjectiveSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		short specificationWordTypeNr;
		int currentSpecificationCollectionNr;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem primarySpecificationWordItem;
		WordItem relatedSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( adjectiveSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given adjective specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( !adjectiveSpecificationItem.isAdjectiveSpecification() )
			return adminItem_.startError( 1, moduleNameString_, "The given adjective specification has no adjective specification" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( !currentQuestionSpecificationItem.hasCurrentCreationSentenceNr() &&
				( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				currentQuestionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) != null )
					{
					specificationWordTypeNr = currentQuestionSpecificationItem.specificationWordTypeNr();
					currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr();
					createdOrFoundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem );

					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem.findRelatedSpecification( true, isArchivedAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), specificationWordTypeNr, currentSpecificationCollectionNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
					relatedSpecificationItem != currentQuestionSpecificationItem &&
					( relatedSpecificationWordItem = relatedSpecificationItem.specificationWordItem() ) != null &&
					( primarySpecificationItem = generalizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( isArchivedAssignment ) ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( additionalDefinitionSpecificationItem = primarySpecificationWordItem.firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem ) ) != null &&
					( secondarySpecificationItem = relatedSpecificationWordItem.firstAdjectiveSpecificationItem( false, false ) ) != null )
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage )
							{
							// Draw Spanish specification substitution conclusion or ask a question
							if( drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isSpanishCurrentLanguage, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, adminItem_.userSpecificationItem(), generalizationWordItem, currentQuestionSpecificationWordItem, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							// Test file: "Indirectly answered questions - John is masculine"
							// Make indirectly answered question assumption
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, adjectiveSpecificationItem, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );

							createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
							}
						}

					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					createdOrFoundSpecificationItem != null &&
					currentQuestionSpecificationItem.isExclusiveSpecification() )
						{
						// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es masculino"
						// Draw simple Spanish negative conclusions
						if( drawSimpleNegativeConclusions( isArchivedAssignment, true, createdOrFoundSpecificationItem, adjectiveSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative Spanish conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						// Draw Spanish proper noun part-of conclusions in proper noun words
						if( drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, false, true, currentQuestionSpecificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );

						// Find answers to questions
						// Missed a question question to answer
						if( generalizationWordItem.findAnswersToQuestions( currentSpecificationCollectionNr, createdOrFoundSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find answers to questions in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentQuestionSpecificationItem = ( currentQuestionSpecificationItem.isReplacedOrDeletedItem() ?
														// First question specification
														adminItem_.firstSpecificationItem( false, false, isArchivedAssignment, true ) :
														// Next question specification
														currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_makeIndirectlyAnsweredQuestionAssumption++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_makeIndirectlyAnsweredQuestionAssumption++;
			}

		return Constants.RESULT_OK;
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		short nOptionsLeft = 0;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem additionalDefinitionSpecificationItem;
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

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != null )
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
			// Get first generalization specification noun word
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != null &&
					( additionalDefinitionSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != null &&
					// Make an only option left assumption
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		// Check if a related specification exists
		( relatedResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );

		if( relatedResult.relatedSpecificationItem == null )
			{
			if( generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() )
				{
				// Correct suggestive assumptions by opposite suggestive question
				if( correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( isArchivedAssignment, isNegative, isPossessive, primarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			else
				{
				// Make suggestive question assumption
				if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question assumption to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected CompoundResultType drawDefinitionSpecificationSubstitutionConclusion( boolean isChineseCurrentLanguage, boolean isSpanishCurrentLanguage, short specificationWordTypeNr, SpecificationItem userSpecificationItem, WordItem specificationWordItem )
		{
		boolean hasNonCompoundUserSpecificationCollection;
		boolean hasUserGeneralizationCollection;
		boolean hasUserSpecificationCollection;
		boolean isArchivedUserAssignment;
		boolean isExclusiveFoundSpecification;
		boolean isExclusiveUserSpecification;
		boolean isFoundArchivedAssignment;
		short generalizationWordTypeNr;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem secondaryDefinitionSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem relatedSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		CompoundResultType compoundResult = new CompoundResultType();
		RelatedResultType relatedResult;

		// Used for developer statistics
		int nStartCreatedItems = GlobalVariables.nTotalCreatedItems;
		GlobalVariables.nTotalReasoningCalls++;

		if( userSpecificationItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
			{
			hasNonCompoundUserSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			hasUserGeneralizationCollection = userSpecificationItem.hasGeneralizationCollection();
			hasUserSpecificationCollection = userSpecificationItem.hasSpecificationCollection();
			isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startCompoundResultError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, specificationWordItem, null ) ) != null )
					{
					if( foundSpecificationItem.isPartOf() )
						{
						if( compoundResult.compoundGeneralizationWordItem == null )
							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
						}
					else
						{
						isFoundArchivedAssignment = foundSpecificationItem.isArchivedAssignment();
						isExclusiveFoundSpecification = foundSpecificationItem.isExclusiveSpecification();
						generalizationWordTypeNr = foundSpecificationItem.generalizationWordTypeNr();

						// Definition
						if( isExclusiveFoundSpecification &&
						generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NOUN &&

							( !isSpanishCurrentLanguage ||
							// Typical for Spanish
							foundSpecificationItem.hasCompoundSpecificationCollection() ) &&

						( secondaryDefinitionSpecificationItem = specificationWordItem.firstNonCollectedSpecificationItem( true, currentGeneralizationWordItem ) ) != null &&
						( secondarySpecificationWordItem = secondaryDefinitionSpecificationItem.specificationWordItem() ) != null &&
						// Make specification substitution definition assumption
						adminItem_.addSelfGeneratedSpecification( false, isFoundArchivedAssignment, isFoundArchivedAssignment, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, secondaryDefinitionSpecificationItem, null, currentGeneralizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make a specification substitution definition assumption about word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

						if( ( hasUserSpecificationCollection ||

						( isExclusiveUserSpecification &&
						isExclusiveFoundSpecification ) ) &&

						// Avoid idle call
						specificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() != null &&
						// Draw compound specification substitution conclusion
						drawCompoundSpecificationSubstitutionConclusion( isFoundArchivedAssignment, isExclusiveFoundSpecification, isSpanishCurrentLanguage, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, foundSpecificationItem, userSpecificationItem, currentGeneralizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( foundSpecificationItem.isOlderItem() &&

							// Non-exclusive (found specification)
							( ( !isExclusiveFoundSpecification &&

								// Test file: "Knowledge of organisms (2)"
								( hasUserGeneralizationCollection ||
								// Test file: "reasoning\Scientific challenge"
								hasUserSpecificationCollection ||
								// Test file: "Uncertainty and past tense"
								isArchivedUserAssignment ||
								// Test files: "reasoning\James was a father - James was the father of Peter",
								//				"reasoning\Scientific challenge"
								isFoundArchivedAssignment ||
								// Typical for Spanish
								// Spanish test file: "razonamiento\Reto científico"
								isSpanishCurrentLanguage ) ) ||

							// Exclusive (found specification)
							( isExclusiveFoundSpecification &&

								// Non-exclusive (user specification)
								// Test file: "reasoning\Scientific challenge"
								( ( !isExclusiveUserSpecification &&
								hasUserSpecificationCollection &&
								generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN ) ||

								// Exclusive (user specification)
								// Test file: "reasoning\family\John has 2 sons and a daughter (before family definition)"
								( isExclusiveUserSpecification &&
								foundSpecificationItem.hasNonCompoundSpecificationCollection() &&
								userSpecificationItem.hasCompoundSpecificationCollection() ) ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&

									( hasNonCompoundUserSpecificationCollection ||

									//  User specification collection
									( hasUserSpecificationCollection &&
									currentGeneralizationWordItem == specificationWordItem ) ) ) ) ) ) &&

						// Draw specification substitution conclusion or ask a question
						drawSpecificationSubstitutionConclusionOrAskQuestion( isFoundArchivedAssignment, isExclusiveFoundSpecification, isSpanishCurrentLanguage, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, currentGeneralizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( isExclusiveFoundSpecification &&
						foundSpecificationItem.isOlderItem() )
							{
							if( hasNonCompoundUserSpecificationCollection &&
							foundSpecificationItem.hasNonCompoundSpecificationCollection() )
								{
								if( ( relatedResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );

								if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
								( relatedSpecificationWordItem = relatedSpecificationItem.specificationWordItem() ) != null )
									{
									// Avoid idle call
									if( relatedSpecificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() != null &&
									// Draw specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), isSpanishCurrentLanguage, relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, currentGeneralizationWordItem, relatedSpecificationWordItem, null ) != Constants.RESULT_OK )
										return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to draw a related specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

									// Make related generalization assumption
									if( makeGeneralizationAssumption( isFoundArchivedAssignment, isChineseCurrentLanguage, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, relatedSpecificationWordItem, userSpecificationWordItem.collectionWordItem( userSpecificationItem.specificationCollectionNr() ) ) != Constants.RESULT_OK )
										return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make a generalization assumption in specification word \"" + relatedSpecificationWordItem.anyWordTypeString() + "\"" );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									// Make Spanish generalization assumption
									makeGeneralizationAssumption( isFoundArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, specificationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make a Spanish generalization assumption in specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		// Used for developer statistics
		if( GlobalVariables.nTotalCreatedItems == nStartCreatedItems )
			{
			GlobalVariables.nIdleReasoningCalls++;
			GlobalVariables.nIdleCalls_drawDefinitionSpecificationSubstitutionConclusion++;
			}
		else
			{
			GlobalVariables.nUsefulReasoningCalls++;
			GlobalVariables.nUsefulCalls_drawDefinitionSpecificationSubstitutionConclusion++;
			}

		return compoundResult;
		}
	}

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
