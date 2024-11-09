/*	Class:			SpecificationItem
 *	Purpose:		Storing info about the specification structure of a word
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

class SpecificationItem extends Item
	{
	// Private initialized variables

	private boolean isAnsweredQuestion_ = false;
	private boolean isCharacteristicFor_ = false;
	private boolean isConditional_ = false;
	private boolean isCorrectedSpecification_ = false;
	private boolean isEveryGeneralization_ = false;
	private boolean isExclusiveGeneralization_ = false;
	private boolean isExclusiveSpecification_ = false;
	private boolean isLanguageWord_ = false;
	private boolean isNegative_ = false;
	private boolean isPartOf_ = false;
	private boolean isPossessive_ = false;
	private boolean isSpecific_ = false;
	private boolean isSpecificationGeneralization_ = false;
	private boolean isUncountableGeneralizationNoun_ = false;
	private boolean isUniqueUserRelation_ = false;
	private boolean isValueSpecification_ = false;

	private short assignmentLevel_ = Constants.NO_ASSIGNMENT_LEVEL;
	private short assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
	private short languageNr_ = Constants.NO_LANGUAGE_NR;
	private short prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
	private short questionParameter_ = Constants.NO_QUESTION_PARAMETER;

	private short generalizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
	private short specificationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
	private short relationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private int generalizationCollectionNr_ = Constants.NO_COLLECTION_NR;
	private int specificationCollectionNr_ = Constants.NO_COLLECTION_NR;
	private int relationCollectionNr_ = Constants.NO_COLLECTION_NR;

	private int generalizationContextNr_ = Constants.NO_CONTEXT_NR;
	private int specificationContextNr_ = Constants.NO_CONTEXT_NR;
	private int relationContextNr_ = Constants.NO_CONTEXT_NR;

	private int nEnteredRelationWords_ = 0;

	private JustificationItem firstJustificationItem_ = null;

	private WordItem specificationWordItem_ = null;
	private WordItem relationWordItem_ = null;

	private String specificationString_ = null;

	private StringBuffer storedSentenceStringBuffer_ = new StringBuffer();
	private StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer_ = new StringBuffer();


	// Private constructed variables

	private short specificationStringWriteLevel_ = Constants.NO_WRITE_LEVEL;


	// Private assignment methods

	private SpecificationItem nextAssignmentItemWithAssignmentLevel( short currentAssignmentLevel )
		{
		SpecificationItem nextAssignmentItem;

		return ( ( nextAssignmentItem = nextSpecificationItem() ) != null &&
				nextAssignmentItem.assignmentLevel() == currentAssignmentLevel ? nextAssignmentItem : null );
		}


	// Private justification methods

	private boolean hasAtLeastOneAssumptionJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasAssumptionLevel() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	private boolean hasFoundCircularReasoning()
		{
		boolean hasFoundOnlyUserSpecifications = true;
		boolean hasFoundOtherJustificationTypes = false;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( !hasFoundOtherJustificationTypes &&
		searchJustificationItem != null )
			{
			if( searchJustificationItem.isReversibleAssumptionOrConclusion() )
				{
				if( searchJustificationItem.primarySelfGeneratedSpecification() != null )
					hasFoundOnlyUserSpecifications = false;
				}
			else
				hasFoundOtherJustificationTypes = true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		if( !hasFoundOtherJustificationTypes &&
		!hasFoundOnlyUserSpecifications )
			return true;

		return false;
		}

	private byte calculateAssumptionLevel()
		{
		boolean _hasRelationWord = ( relationWordItem_ != null );
		boolean isBlockingHighestAssumptionLevel = false;
		boolean isCheckingRelationWords;
		boolean isForcingHighestAssumptionLevel;
		boolean isOlderSpecification = isOlderItem();
		boolean isReversibleAssumptionOrConclusion;
		boolean _isSpecificationWordSpanishAmbiguous = isSpecificationWordSpanishAmbiguous();
		short highestAssumptionLevel;
		short lowestAssumptionLevel = Constants.MAX_LEVEL;
		int nJustificationRelationWords;
		int _nRelationWords = nRelationWords();
		int justificationAssumptionLevel;
		JustificationItem currentJustificationItem = firstJustificationItem_;
		JustificationItem searchJustificationItem;

		if( isSpecificationGeneralization_ )
			return startError( 1, null, "I am a specification-generalization" );

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			return startError( 1, null, "I am a question" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification" );

		do	{
			isCheckingRelationWords = true;
			isForcingHighestAssumptionLevel = false;
			highestAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
			nJustificationRelationWords = 0;
			searchJustificationItem = currentJustificationItem;

			do	{
				if( _hasRelationWord )
					nJustificationRelationWords += searchJustificationItem.nJustificationRelationWords( relationCollectionNr_, _nRelationWords, relationWordItem_ );

				// Calculate assumption level
				if( ( justificationAssumptionLevel = searchJustificationItem.justificationAssumptionLevel() ) > Constants.MAX_LEVEL )
					return startSystemError( 1, null, "Assumption level overflow" );

				if( isCheckingRelationWords )
					{
					if( justificationAssumptionLevel > highestAssumptionLevel )
						{
						if( justificationAssumptionLevel > assumptionLevel_ )
							{
							isReversibleAssumptionOrConclusion = searchJustificationItem.isReversibleAssumptionOrConclusion();

							// Opposite possessive specification assumption, or Exclusive specification substitution assumption
							if( ( !isReversibleAssumptionOrConclusion &&
							isOlderSpecification &&

								// Test files: "reasoning\family\Complex (13)",
								//				"reasoning\family\Complex (18)",
								//				"reasoning\family\Complex (19 - mixed)",
								//				"reasoning\family\Complex (19 - strange)"
								( searchJustificationItem.isExclusiveSpecificationSubstitutionAssumption() ||

								// Typical for Spanish
								// Spanish test files: "conflicto\familia\José es una madre",
								//						"razonamiento\familia\Complejo (12)",
								//						"razonamiento\familia\Complejo (13)",
								//						"razonamiento\familia\Complejo (14)",
								//						"razonamiento\familia\Complejo (15)",
								//						among others
								assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ) ) ||

							// Reversible assumption
							// Test files: "reasoning\family\Complex (13)",
							//				"reasoning\family\Complex (18)",
							//				"reasoning\family\Complex (19 - mixed)",
							//				"reasoning\family\Complex (19 - strange)"
							( isReversibleAssumptionOrConclusion &&
							assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL &&
							searchJustificationItem == currentJustificationItem &&
							hasCompoundSpecificationCollection() ) )
								isForcingHighestAssumptionLevel = true;
							}
						else
							{
							// Typical for Spanish
							// Spanish test files: "razonamiento\familia\Complejo (19 - extraño)",
							//						"razonamiento\familia\Paz - José - Juan y Ana"
							if( isOlderSpecification &&
							justificationAssumptionLevel < assumptionLevel_ &&
							relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
							searchJustificationItem.isExclusiveSpecificationSubstitutionAssumption() &&
							searchJustificationItem.hasCurrentCreationSentenceNr() &&
							searchJustificationItem.isPossessivePrimarySpecification() )
								isBlockingHighestAssumptionLevel = true;
							}

						highestAssumptionLevel = (short)justificationAssumptionLevel;
						}
					}
				else
					{
					// Test files: "reasoning\family\Complex (1)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				"reasoning\family\Complex (9)",
					//				"reasoning\family\Complex (15)",
					//				"reasoning\family\I know (11)",
					//				among others
					if( justificationAssumptionLevel < highestAssumptionLevel )
						highestAssumptionLevel = (short)justificationAssumptionLevel;
					}

				// Test files: "reasoning\family\Avoid duplicate context",
				//				"reasoning\family\Complex (1)",
				//				"reasoning\family\Complex (4)",
				//				"reasoning\family\Complex (5)",
				//				"reasoning\family\Complex (6)",
				//				"reasoning\family\Complex (9)",
				//				among others
				if( !_hasRelationWord )
					isCheckingRelationWords = false;
				}
			while( !isBlockingHighestAssumptionLevel &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );

			if( !isBlockingHighestAssumptionLevel &&
			highestAssumptionLevel < lowestAssumptionLevel &&

				// Reversible assumption
				// Test files: "reasoning\family\Complex (13)",
				//				"reasoning\family\Complex (18)",
				//				"reasoning\family\Complex (19 - mixed)",
				//				"reasoning\family\Complex (19 - strange)"
				( isForcingHighestAssumptionLevel ||

				// Test files: "conflict\family\Joe is a mother",
				//				"reasoning\family\Complex (14)",
				//				"reasoning\family\Joe is a son of John and Anna",
				//				"reasoning\family\My assumptions that are confirmed (Paul and John)",
				//				"reasoning\family\Paul - John and Anna - Joe - Laura",
				//				"reasoning\family\You have confirmed at least one relation word"
				( nJustificationRelationWords > _nRelationWords &&
				highestAssumptionLevel <= assumptionLevel_ ) ||

				( nJustificationRelationWords == _nRelationWords &&

					// No assumption level
					// Test files: "reasoning\family\I know (11)",
					//				"reasoning\family\Joe has 2 parents - Joe is a child",
					//				"reasoning\family\Joe is a child - Joe has 2 parents",
					//				"reasoning\family\Joe is a child - John and Anna",
					//				among others
					( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ||

					// Assumption level
					// Test files: "conflict\family\Joe is a mother",
					//				"reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				"reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				among others
						( !_isSpecificationWordSpanishAmbiguous &&
					assumptionLevel_ + 2 > highestAssumptionLevel ) ||

					// Typical for Spanish
					// Spanish test file: "razonamiento\familia\Complejo (13)"
					isNegative_ ||

					// Typical for Spanish
					// First set of justifications
					// Spanish test file: "conflicto\familia\José es una madre",
					//						"razonamiento\familia\Complejo (1)",
					//						"razonamiento\familia\Complejo (6)",
					//						"razonamiento\familia\Complejo (9)",
					//						"razonamiento\familia\Complejo (11)",
					//						among others
					highestAssumptionLevel < assumptionLevel_ ) ) ) )
				lowestAssumptionLevel = highestAssumptionLevel;
			}
		while( !isBlockingHighestAssumptionLevel &&
		( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != null );

		if( !isBlockingHighestAssumptionLevel &&
		lowestAssumptionLevel < Constants.MAX_LEVEL &&
		// Assumption level has changed
		lowestAssumptionLevel != assumptionLevel_ )
			{
			assumptionLevel_ = lowestAssumptionLevel;

			// Clear stored sentence buffer to lose the previous assumption level word
			clearStoredSentenceStringBuffer();
			}

		return Constants.RESULT_OK;
		}

	private short highestJustificationAssumptionLevel()
		{
		short highestAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		short searchAssumptionLevel;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( ( searchAssumptionLevel = searchJustificationItem.justificationAssumptionLevel() ) > highestAssumptionLevel )
				highestAssumptionLevel = searchAssumptionLevel;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return highestAssumptionLevel;
		}

	private byte addJustificationsToSpecificationsOfRelationWords( WordItem currentRelationWordItem, WordItem partOfSpecificationWordItem )
		{
		SpecificationItem additionalSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem reversibleSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem oppositeSpecificationWordItem;
		WordItem _myWordItem = myWordItem();

		if( currentRelationWordItem == null )
			return startError( 1, null, "The given current relation word item is undefined" );

		if( specificationWordItem_ == null )
			return startError( 1, null, "I have no specification word" );

		if( ( reversibleSpecificationItem = currentRelationWordItem.bestMatchingRelationSpecificationItem( true, true, false, !isPossessive_, specificationWordItem_, _myWordItem ) ) != null &&
		!reversibleSpecificationItem.isHiddenSpanishSpecification() )
			{
			if( !hasOnlyOneRelationWord() &&
			reversibleSpecificationItem.isSelfGeneratedSpecification() )
				{
				if( ( reversibleSpecificationItem.isOlderItem() ||
				_myWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() ) &&

				// Justification doesn't exist yet in reversible specification
				!reversibleSpecificationItem.hasPrimarySpecificationJustification( this ) &&
				// Justification has at least the same assumption level
				reversibleSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, this, null, null, null ) &&
				// Add non-possessive reversible justification to myself
				reversibleSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, this, null, null ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to add a non-possessive reversible justification to myself" );

				reversibleSpecificationItem = reversibleSpecificationItem.updatedSpecificationItem();

				if( partOfSpecificationWordItem != null )
					{
					if( isPossessive_ )
						{
						if( ( partOfSpecificationItem = currentRelationWordItem.firstPartOfSpecificationItem( partOfSpecificationWordItem ) ) != null &&
						// Justification doesn't exist yet in part-of specification
						!partOfSpecificationItem.hasPrimarySpecificationJustification( reversibleSpecificationItem ) &&
						( definitionSpecificationItem = partOfSpecificationWordItem.firstPartOfSpecificationItem( specificationWordItem_ ) ) != null &&
						// Justification has at least the same assumption level
						partOfSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, definitionSpecificationItem, null ) &&
						// Add non-possessive part-of justification to part-of specification
						partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, definitionSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to add a non-possessive part-of justification to a part-of specification" );
						}
					else
						{
						if( ( partOfSpecificationItem = _myWordItem.firstPartOfSpecificationItem( partOfSpecificationWordItem ) ) != null &&
						// Justification doesn't exist yet in part-of specification
						!partOfSpecificationItem.hasPrimarySpecificationJustification( this ) &&
						( definitionSpecificationItem = partOfSpecificationWordItem.firstPartOfSpecificationItem( specificationWordItem_ ) ) != null &&
						// Justification has at least the same assumption level
						partOfSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem, null ) &&
						// Skip if specification exists without relation word
						_myWordItem.bestMatchingRelationSpecificationItem( true, false, false, false, false, false, false, false, specificationCollectionNr_, Constants.NO_COLLECTION_NR, specificationWordItem_, null ) == this &&
						// Add non-possessive part-of justification to part-of specification
						partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to add a non-possessive part-of justification to a part-of specification" );
						}
					}
				}

			if( !isPossessive_ &&
			!reversibleSpecificationItem.hasOnlyOneRelationWord() &&
			// I don't have this reversible justification
			!hasPrimarySpecificationJustification( reversibleSpecificationItem ) &&
			// I have no better assumption level
			assumptionLevel_ >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, null, null ) &&
			// Add possessive reversible justification to myself
			addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, null ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to add a possessive reversible justification to myself" );

			if( reversibleSpecificationItem.assumptionLevel() < assumptionLevel_ &&

				// Test files: "reasoning\family\Complex (13)",
				//				"reasoning\family\Complex (18)",
				//				"reasoning\family\Complex (19 - mixed)",
				//				"reasoning\family\Complex (19 - strange)"
				( hasCompoundSpecificationCollection() ||

				// Test files: "reasoning\family\Complex (14)",
				//				"reasoning\family\John - Anna (before family definition)",
				//				"reasoning\family\John is the father - Paul - Laura"
				!reversibleSpecificationItem.hasOnlyOneRelationWord() ) &&

			// Recalculate assumption levels of specifications in context word
			currentRelationWordItem.recalculateAssumptionLevelsInWord( false ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to recalculate the assumption levels of specifications in word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );
			}

		if( isPossessive_ &&
		partOfSpecificationWordItem != null )
			{
			// Part-of
			if( ( partOfSpecificationItem = currentRelationWordItem.firstPartOfSpecificationItem( partOfSpecificationWordItem ) ) != null &&
			// Part-of specification has no similar justification
			partOfSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, _myWordItem ) == null &&
			// Justification doesn't exist yet in part-of specification
			!partOfSpecificationItem.hasPrimarySpecificationJustification( this ) &&
			( definitionSpecificationItem = partOfSpecificationWordItem.firstPartOfSpecificationItem( specificationWordItem_ ) ) != null &&
			// Justification has at least the same assumption level
			partOfSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem, null ) &&
			// Add possessive part-of justification to part-of specification
			partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to add a possessive part-of justification to a part-of specification" );

			if( !specificationWordItem_.isNounWordSpanishAmbiguous() &&
			( oppositeSpecificationWordItem = specificationWordItem_.collectionWordItem( specificationCollectionNr_ ) ) != null )
				{
				// Add justification to additional specification
				if( ( definitionSpecificationItem = partOfSpecificationWordItem.firstSpecificationItem( false, true, false, specificationWordItem_ ) ) != null )
					{
					// My word
					if( !_myWordItem.hasCurrentlyConfirmedSpecification() &&
					( additionalSpecificationItem = _myWordItem.firstSpecificationItem( false, false, false, oppositeSpecificationWordItem ) ) != null &&
					// Justification doesn't exist yet in part-of specification
					!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, this ) &&
					// Justification has at least the same assumption level
					additionalSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, this, null ) &&
					// Add opposite possessive assumption justification to additional specification of my word
					additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, this ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add an opposite possessive assumption justification to an additional specification of my word" );

					// Current relation word
					if( ( additionalSpecificationItem = currentRelationWordItem.firstSpecificationItem( false, true, false, oppositeSpecificationWordItem ) ) != null &&
					// Justification doesn't exist yet in part-of specification
					!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, this ) &&
					// Justification has at least the same assumption level
					additionalSpecificationItem.assumptionLevel() >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, this, null ) &&
					// Add opposite possessive assumption justification to additional specification of current relation word
					additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, this ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add an opposite possessive assumption justification to an additional specification of the current relation word" );
					}

				// Add justification to me
				if( ( definitionSpecificationItem = partOfSpecificationWordItem.firstSpecificationItem( false, true, false, oppositeSpecificationWordItem ) ) != null )
					{
					// My word
					if( ( secondarySpecificationItem = _myWordItem.bestMatchingRelationSpecificationItem( false, false, false, false, oppositeSpecificationWordItem, currentRelationWordItem ) ) != null &&
					// Justification doesn't exist yet in part-of specification
					!hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, secondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					assumptionLevel_ >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, secondarySpecificationItem, null ) &&
					// Add opposite possessive assumption justification of my word to me
					addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add an opposite possessive assumption justification of my word to me" );

					// Current relation word
					if( !currentRelationWordItem.hasCurrentlyConfirmedSpecification() &&
					( secondarySpecificationItem = currentRelationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, oppositeSpecificationWordItem, _myWordItem ) ) != null &&
					// Justification doesn't exist yet in part-of specification
					!hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, secondarySpecificationItem ) &&
					assumptionLevel_ >= Item.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, secondarySpecificationItem, null ) &&
					// Add opposite possessive assumption justification of current relation word to me
					addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add an opposite possessive assumption justification of current relation word to me" );
					}
				}
			}

		return Constants.RESULT_OK;
		}


	// Private specification methods

	private boolean hasContextMasculineWordInRelationWords()
		{
		WordItem currentCollectionWordItem;

		if( relationWordItem_ != null &&
		relationWordItem_.isMasculineWord() )
			return true;

		if( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr_ ) ) != null )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem.isMasculineWord() )
					return true;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr_ ) ) != null );
			}

		return false;
		}

	private boolean hasSecondaryActiveAssignmentSpecification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isSecondaryAssignmentSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	private boolean hasUserSpecificationWordItem()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isUserSpecificationWord );
		}

	private boolean isCandidateForQuestion( boolean isAllowingSpanishPossessiveSpecification )
		{
		return ( !isExclusiveSpecification_ &&
				!isNegative_ &&
				!isPartOf_ &&

				( !isPossessive_ ||
				isAllowingSpanishPossessiveSpecification ) &&

				questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
				!isHiddenSpanishSpecification() );
		}

	private boolean isRelationWordCoveredByJustifications( WordItem givenRelationWordItem )
		{
		boolean hasFoundRelationWordInJustification = false;
		int relationCollectionNr;
		JustificationItem searchJustificationItem = firstJustificationItem_;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;

		if( givenRelationWordItem != null )
			{
			while( !hasFoundRelationWordInJustification &&
			searchJustificationItem != null )
				{
				// Check primary specification
				if( ( ( primarySpecificationItem = searchJustificationItem.primarySpecificationItem() ) != null &&
				primarySpecificationItem.hasRelationWord() &&

					( primarySpecificationItem.generalizationWordItem() == givenRelationWordItem ||
					primarySpecificationItem.relationWordItem() == givenRelationWordItem ||

					( ( relationCollectionNr = primarySpecificationItem.relationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					givenRelationWordItem.hasCollectionNr( relationCollectionNr ) ) ) ) ||

				// Check secondary specification
				( ( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null &&
				secondarySpecificationItem.hasRelationWord() &&

					( secondarySpecificationItem.generalizationWordItem() == givenRelationWordItem ||
					secondarySpecificationItem.relationWordItem() == givenRelationWordItem ||

					( ( relationCollectionNr = secondarySpecificationItem.relationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					givenRelationWordItem.hasCollectionNr( relationCollectionNr ) ) ) ) )
					hasFoundRelationWordInJustification = true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}

			if( !hasFoundRelationWordInJustification )
				// At least one relation word is not covered by justifications
				return false;
			}

		// Relation words are covered by justifications
		return true;
		}

	private WordItem relationWordNotCoveredByJustifications()
		{
		WordItem currentCollectionWordItem;

		if( relationWordItem_ != null &&
		firstJustificationItem_ != null )
			{
			if( !isRelationWordCoveredByJustifications( relationWordItem_ ) )
				// Relation word is not covered by justifications
				return relationWordItem_;

			if( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
			( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr_ ) ) != null )
				{
				// Do for all collection words
				do	{
					if( !isRelationWordCoveredByJustifications( currentCollectionWordItem ) )
						// Relation word is not covered by justifications
						return currentCollectionWordItem;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr_ ) ) != null );
				}
			}

		// Relation words are covered by justifications
		return null;
		}

	// Protected constructed variables

	protected boolean hasSpecificationBeenWrittenAsAnswer = false;

	protected SpecificationItem replacingSpecificationItem = null;


	// Constructor

	protected SpecificationItem( boolean isAnsweredQuestion, boolean isCharacteristicFor, boolean isConditional, boolean isCorrectedSpecification, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isLanguageWord, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nEnteredRelationWords, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString, StringBuffer storedSentenceStringBuffer, StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer, List myList, WordItem myWordItem )
		{

		// Used for developer statistics
		GlobalVariables.nCreatedSpecificationItems++;

		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, myList, myWordItem );

		// Private initialized variables

		isAnsweredQuestion_ = isAnsweredQuestion;
		isCharacteristicFor_ = isCharacteristicFor;
		isConditional_ = isConditional;
		isCorrectedSpecification_ = isCorrectedSpecification;
		isEveryGeneralization_ = isEveryGeneralization;
		isExclusiveGeneralization_ = isExclusiveGeneralization;
		isExclusiveSpecification_ = isExclusiveSpecification;
		isLanguageWord_ = isLanguageWord;
		isNegative_ = isNegative;
		isPartOf_ = isPartOf;
		isPossessive_ = isPossessive;
		isSpecific_ = isSpecific;
		isSpecificationGeneralization_ = isSpecificationGeneralization;
		isUncountableGeneralizationNoun_ = isUncountableGeneralizationNoun;
		isUniqueUserRelation_ = isUniqueUserRelation;
		isValueSpecification_ = isValueSpecification;

		assignmentLevel_ = assignmentLevel;
		assumptionLevel_ = assumptionLevel;
		languageNr_ = languageNr;
		prepositionParameter_ = prepositionParameter;
		questionParameter_ = questionParameter;

		generalizationWordTypeNr_ = generalizationWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;
		relationWordTypeNr_ = relationWordTypeNr;

		generalizationCollectionNr_ = generalizationCollectionNr;
		specificationCollectionNr_ = specificationCollectionNr;
		relationCollectionNr_ = relationCollectionNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nEnteredRelationWords_ = nEnteredRelationWords;

		firstJustificationItem_ = firstJustificationItem;

		specificationWordItem_ = specificationWordItem;
		relationWordItem_ = relationWordItem;

		specificationString_ = specificationString;

		storedSentenceStringBuffer_ = storedSentenceStringBuffer;
		storedSentenceWithOnlyOneSpecificationStringBuffer_ = storedSentenceWithOnlyOneSpecificationStringBuffer;
		}


	// Protected virtual methods

	@Override
	protected void checkForUsage()
		{
		myWordItem().checkSpecificationForUsageOfInvolvedWords( this );
		}

	@Override
	protected void selectingAttachedJustifications( boolean isSelectingJustificationSpecifications )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			searchJustificationItem.isSelectedByQuery = true;

			if( isSelectingJustificationSpecifications )
				searchJustificationItem.selectingJustificationSpecifications();

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}
		}

	@Override
	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( specificationString_ != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( specificationString_ );
			}
		}

	@Override
	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( specificationWordItem_ != null &&
		( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( wordString );
			}
		}

	@Override
	protected boolean hasParameter( int queryParameter )
		{
		return ( prepositionParameter_ == queryParameter ||
				questionParameter_ == queryParameter ||
				generalizationCollectionNr_ == queryParameter ||
				specificationCollectionNr_ == queryParameter ||
				relationCollectionNr_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nEnteredRelationWords_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

					( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER ||
					questionParameter_ > Constants.NO_QUESTION_PARAMETER ||
					generalizationCollectionNr_ > Constants.NO_COLLECTION_NR ||
					specificationCollectionNr_ > Constants.NO_COLLECTION_NR ||
					relationCollectionNr_ > Constants.NO_COLLECTION_NR ||
					generalizationContextNr_ > Constants.NO_CONTEXT_NR ||
					specificationContextNr_ > Constants.NO_CONTEXT_NR ||
					relationContextNr_ > Constants.NO_CONTEXT_NR ||
					nEnteredRelationWords_ > 0 ) ) );
		}

	@Override
	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( firstJustificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : firstJustificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : firstJustificationItem_.itemNr() == queryItemNr ) ) ||

				( specificationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : specificationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : specificationWordItem_.itemNr() == queryItemNr ) ) ||

				( replacingSpecificationItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : replacingSpecificationItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : replacingSpecificationItem.itemNr() == queryItemNr ) ) );
		}

	@Override
	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( generalizationWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr	||
				relationWordTypeNr_ == queryWordTypeNr	);
		}

	@Override
	protected boolean isSorted( Item nextSortItem )
		{
		SpecificationItem nextSortSpecificationItem = (SpecificationItem)nextSortItem;

		return ( nextSortItem != null &&

				// 1) Assignment needs descending assignmentLevel_
				( assignmentLevel_ > nextSortSpecificationItem.assignmentLevel_ ||

				// 2) Question and specification needs descending creationSentenceNr()
				( assignmentLevel_ == nextSortSpecificationItem.assignmentLevel_ &&
				creationSentenceNr() > nextSortSpecificationItem.creationSentenceNr() ) ) );
		}

	@Override
	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		String languageNameString = WordItem.languageNameString( languageNr_ );
		String generalizationWordTypeString = WordItem.wordTypeNameString( generalizationWordTypeNr_ );
		StringBuffer queryStringBuffer;
		String relationWordTypeString = ( relationWordTypeNr_ == Constants.NO_WORD_TYPE_NR ? null : WordItem.wordTypeNameString( relationWordTypeNr_ ) );
		String specificationWordTypeString = WordItem.wordTypeNameString( specificationWordTypeNr_ );
		String wordString;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( languageNr_ > Constants.NO_LANGUAGE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + ( languageNameString == null ? ( "languageNr:" + languageNr_ ) : ( "language:" + languageNameString ) ) );

		if( isAnsweredQuestion_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAnsweredQuestion" );

		if( isCharacteristicFor_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isCharacteristicFor" );

		if( isConditional_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isConditional" );

		if( isCorrectedSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isCorrectedSpecification" );

		if( isEveryGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isEveryGeneralization" );

		if( isExclusiveGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveGeneralization" );

		if( isExclusiveSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecification" );

		if( isLanguageWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isNegative_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegative" );

		if( isPartOf_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPartOf" );

		if( isPossessive_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessive" );

		if( isSpecific_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecific" );

		if( isSpecificationGeneralization_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralization" );

		if( isUncountableGeneralizationNoun_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUncountableGeneralizationNoun" );

		if( isUniqueUserRelation_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueUserRelation" );

		if( isValueSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isValueSpecification" );

		if( assignmentLevel_ > Constants.NO_ASSIGNMENT_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assignmentLevel:" + assignmentLevel_ );

		if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "assumptionLevel:" + assumptionLevel_ );

		if( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationStringWriteLevel:" + specificationStringWriteLevel_ );

		if( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "prepositionParameter:" + prepositionParameter_ );

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "questionParameter:" + questionParameter_ );

		if( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationCollectionNr:" + generalizationCollectionNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordType:" + ( generalizationWordTypeString == null ? Constants.EMPTY_STRING : generalizationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + generalizationWordTypeNr_ );

		if( generalizationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationContextNr:" + generalizationContextNr_ );

		if( specificationCollectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationCollectionNr:" + specificationCollectionNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordType:" + ( specificationWordTypeString == null ? Constants.EMPTY_STRING : specificationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + specificationWordTypeNr_ );

		if( specificationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationContextNr:" + specificationContextNr_ );

		if( specificationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + specificationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + specificationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( relationCollectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationCollectionNr:" + relationCollectionNr_ );

		if( relationWordTypeNr_ > Constants.NO_WORD_TYPE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordType:" + ( relationWordTypeString == null ? Constants.EMPTY_STRING : relationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + relationWordTypeNr_ );

		if( relationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationContextNr:" + relationContextNr_ );

		if( relationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + relationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + relationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = relationWordItem_.wordTypeString( true, relationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( nEnteredRelationWords_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nEnteredRelationWords:" + nEnteredRelationWords_ );

		if( firstJustificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "firstJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + firstJustificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + firstJustificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( replacingSpecificationItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacingSpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + replacingSpecificationItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + replacingSpecificationItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( specificationString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationString:" + Constants.QUERY_STRING_START_CHAR + specificationString_ + Constants.QUERY_STRING_END_CHAR );

		if( storedSentenceStringBuffer_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "storedSentenceStringBuffer:" + Constants.QUERY_STRING_START_CHAR + storedSentenceStringBuffer_ + Constants.QUERY_STRING_END_CHAR );

		if( storedSentenceWithOnlyOneSpecificationStringBuffer_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "storedSentenceWithOnlyOneSpecificationStringBuffer:" + Constants.QUERY_STRING_START_CHAR + storedSentenceWithOnlyOneSpecificationStringBuffer_ + Constants.QUERY_STRING_END_CHAR );

		return queryStringBuffer;
		}

	@Override
	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		if( specificationWordItem_ != null )
			return specificationWordItem_.findMatchingWordReferenceString( queryString );

		return new BoolResultType();
		}


	// Protected common methods

	protected boolean hasAssumptionLevel()
		{
		return ( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL );
		}

	protected short assumptionLevel()
		{
		return assumptionLevel_;
		}

	protected short languageNr()
		{
		return languageNr_;
		}

	protected int nRelationWords()
		{
		return WordItem.nRelationWords( relationCollectionNr_, relationWordItem_ );
		}


	// Protected assignment methods

	protected boolean isAssignment()
		{
		return myList().isAssignmentList();
		}

	protected boolean isUserAssignment()
		{
		return ( isAssignment() &&
				isUserSpecification() );
		}

	protected boolean isActiveAssignment()
		{
		return ( isAssignment() &&
				isActiveItem() );
		}

	protected boolean isInactiveAssignment()
		{
		return ( isAssignment() &&
				isInactiveItem() );
		}

	protected boolean isArchivedAssignment()
		{
		return ( isAssignment() &&
				isArchivedItem() );
		}

	protected short assignmentLevel()
		{
		return assignmentLevel_;
		}

	protected SpecificationItem assignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ?
														( assignmentLevel_ == currentAssignmentLevel ? this : null ) :
														nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isQuestion() == isQuestion &&

				( isIncludingAnsweredQuestions ||
				// Skip answered questions
				!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel );
			}

		return null;
		}

	protected SpecificationItem assignmentItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ?
														( assignmentLevel_ == currentAssignmentLevel ? this : null ) :
														nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.questionParameter_ == questionParameter &&

				( isIncludingAnsweredQuestions ||
				// Skip answered questions
				!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel );
			}

		return null;
		}


	// Protected justification methods

	protected boolean hasJustification( JustificationItem referenceJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( referenceJustificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem == referenceJustificationItem )
					return true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean hasJustificationWithAdditionalDefinitionSpecification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasAdditionalDefinitionSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasOnlyOneJustificationType( short justificationTypeNr )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() != justificationTypeNr )
				return false;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return true;
		}

	protected boolean hasPrimaryNounSpecificationJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isPrimaryNounSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasPrimarySpecificationJustification( SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem )
					return true;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return false;
		}

	protected boolean hasPrimaryAndSecondarySpecificationJustification( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasSecondarySpecificationJustification( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasSecondarySpecificationCompoundCollectionJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasSecondaryCompoundCollectionSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasSecondaryUserSpecificationJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isOppositePossessiveSpecificationAssumption() &&
			searchJustificationItem.hasSecondaryUserSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean isFirstJustificationReversibleAssumptionOrConclusion()
		{
		return ( firstJustificationItem_ != null &&
				firstJustificationItem_.isReversibleAssumptionOrConclusion() );
		}

	protected boolean isFirstJustificationReversibleAssumption()
		{
		return ( firstJustificationItem_ != null &&
				firstJustificationItem_.isReversibleAssumptionOrConclusion() &&
				firstJustificationItem_.hasAssumptionLevel() );
		}

	protected int nJustificationItems()
		{
		int nItems = 0;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			nItems++;
			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return nItems;
		}

	protected byte changeFirstJustification( boolean isCheckingForDuplicates, JustificationItem replacingJustificationItem )
		{
		if( replacingJustificationItem == null )
			return startError( 1, null, "The given replacing justification item is undefined" );

		if( !replacingJustificationItem.isActiveItem() )
			return startError( 1, null, "The given replacing justification item isn't active" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isCheckingForDuplicates &&
		checkJustification( replacingJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to check the given replacing justification item" );

		firstJustificationItem_ = replacingJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte checkForCircularReasoning()
		{
		boolean hasFoundCircularReasoning = false;
		boolean hasFoundOnlyUserSpecifications = true;
		boolean hasFoundOtherJustificationTypes = false;
		JustificationItem searchJustificationItem = firstJustificationItem_;
		SpecificationItem primarySelfGeneratedSpecification;

		while( !hasFoundOtherJustificationTypes &&
		searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.isReversibleAssumptionOrConclusion() )
				{
				if( ( primarySelfGeneratedSpecification = searchJustificationItem.primarySelfGeneratedSpecification() ) != null )
					{
					hasFoundOnlyUserSpecifications = false;

					if( primarySelfGeneratedSpecification.hasFoundCircularReasoning() )
						hasFoundCircularReasoning = true;
					}
				}
			else
				hasFoundOtherJustificationTypes = true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		if( hasFoundCircularReasoning &&
		!hasFoundOtherJustificationTypes &&
		!hasFoundOnlyUserSpecifications &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a case of circular reasoning:\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.isDeletedItem() &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted justification item in a replaced specification item:\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustification: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, null, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForReplacedOrDeletedJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.isReplacedOrDeletedItem() &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted justification item:\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, null, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForJustificationsCoveringRelationWords()
		{
		WordItem notCoveredRelationWord;

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification. So, I have no justifications" );

		if( GlobalVariables.firstCollectionWordItem == null )
			return startError( 1, null, "I could not find any collection word, while checking myself for justifications covering relation words" );

		if( ( notCoveredRelationWord = relationWordNotCoveredByJustifications() ) != null &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a specification missing justification covering relation word: \"" + notCoveredRelationWord.anyWordTypeString() + "\";\n\tRelation collection number: " + relationCollectionNr_ + ";\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	protected byte checkJustification( JustificationItem referenceJustificationItem )
		{
		boolean hasFeminineOrMasculineProperNounEnding;
		short justificationTypeNr;
		JustificationItem searchJustificationItem = firstJustificationItem_;
		SpecificationItem referencePrimarySpecificationItem;
		SpecificationItem referenceAdditionalDefinitionSpecificationItem;
		SpecificationItem referenceSecondarySpecificationItem;
//		SpecificationItem referenceAdditionalProperNounSpecificationItem;

		if( referenceJustificationItem == null )
			return startError( 1, null, "The given reference justification item is undefined" );

		if( searchJustificationItem == null )
			return startError( 1, null, "I have no justification items" );

		hasFeminineOrMasculineProperNounEnding = referenceJustificationItem.hasFeminineOrMasculineProperNounEnding();
		justificationTypeNr = referenceJustificationItem.justificationTypeNr();
		referencePrimarySpecificationItem = referenceJustificationItem.primarySpecificationItem();
		referenceAdditionalDefinitionSpecificationItem = referenceJustificationItem.additionalDefinitionSpecificationItem();
		referenceSecondarySpecificationItem = referenceJustificationItem.secondarySpecificationItem();
//		referenceAdditionalProperNounSpecificationItem = referenceJustificationItem.additionalProperNounSpecificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem == referenceJustificationItem )
				return startError( 1, null, "The given reference justification item is already one of my attached justification items" );

			if( searchJustificationItem.hasJustification( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, referencePrimarySpecificationItem, referenceAdditionalDefinitionSpecificationItem, referenceSecondarySpecificationItem ) )
				return startError( 1, null, "I already have a justification item like the given attached justification item" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte removeRemainingObsoleteAssumptionJustifications()
		{
		JustificationItem previousJustificationItem = null;
		JustificationItem searchJustificationItem = firstJustificationItem_;
		WordItem _myWordItem = myWordItem();

		if( questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
		assumptionLevel_ < Constants.ASSUMPTION_LEVEL_MAYBE )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isOlderItem() &&
				assumptionLevel_ < searchJustificationItem.justificationAssumptionLevel() )
					{
					if( previousJustificationItem == null )
						{
						if( hasCurrentCreationSentenceNr() )
							{
							firstJustificationItem_ = searchJustificationItem.attachedJustificationItem();

							// Change attached justification
							if( searchJustificationItem.changeAttachedJustification( null ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to disconnect the attached justification item of the search justification item" );

							if( _myWordItem.replaceOrDeleteJustification( searchJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to replace an obsolete justification" );

							searchJustificationItem = firstJustificationItem_;
							}
						else
							{
							previousJustificationItem = searchJustificationItem;
							searchJustificationItem = searchJustificationItem.attachedJustificationItem();
							}
						}
					else	// Specification is not current
						{
						if( previousJustificationItem.hasCurrentCreationSentenceNr() )
							{
							// Change attached justification
							if( previousJustificationItem.changeAttachedJustification( searchJustificationItem.attachedJustificationItem() ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to disconnect the attached justification item of the search justification item" );

							if( searchJustificationItem.hasCurrentCreationSentenceNr() &&
							// Change attached justification
							searchJustificationItem.changeAttachedJustification( null ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to disconnect the attached justification item of the search justification item" );

							if( _myWordItem.replaceOrDeleteJustification( searchJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to replace an obsolete justification" );

							previousJustificationItem = null;
							searchJustificationItem = firstJustificationItem_;
							}
						else	// Previous justification is not current
							{
							previousJustificationItem = searchJustificationItem;
							searchJustificationItem = searchJustificationItem.attachedJustificationItem();
							}
						}
					}
				else
					{
					previousJustificationItem = searchJustificationItem;
					searchJustificationItem = searchJustificationItem.attachedJustificationItem();
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected JustificationItem firstJustificationItem( short justificationTypeNr )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstJustificationItem( short justificationTypeNr, short justificationOrderNr )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem.justificationOrderNr == justificationOrderNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstJustificationItem( short justificationTypeNr, WordItem primaryGeneralizationWordItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem.primaryGeneralizationWordItem() == primaryGeneralizationWordItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstReplacedPrimarySpecificationJustificationItem()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isReplacedPrimarySpecification() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstReplacedSecondarySpecificationJustificationItem()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isReplacedSecondarySpecification() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem firstSecondaryJustificationItem( boolean isPossessiveSecondarySpecification )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isPossessiveSecondarySpecification() == isPossessiveSecondarySpecification )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return null;
		}

	protected JustificationItem primarySpecificationJustificationItem( SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.attachedJustificationItem();
				}
			}

		return null;
		}


	// Protected question methods

	protected boolean isAnsweredQuestion()
		{
		return isAnsweredQuestion_;
		}

	protected boolean isQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER );
		}

	protected boolean isUserQuestion()
		{
		return ( firstJustificationItem_ == null &&
				questionParameter_ > Constants.NO_QUESTION_PARAMETER );
		}

	protected short questionParameter()
		{
		return questionParameter_;
		}


	// Protected specification methods

	protected void clearReplacingInfo()
		{
		if( hasCurrentReplacedSentenceNr() ||

		( replacingSpecificationItem != null &&
		replacingSpecificationItem.hasCurrentOrNewerCreationSentenceNr() ) )
			{
			replacingSpecificationItem = null;
			clearReplacedSentenceNr();
			}
		}

	protected void clearStoredSentenceStringBuffer()
		{
		storedSentenceStringBuffer_ = null;
		}

	protected void initializeSpecificationVariables()
		{
		// Protected constructed variables
		hasSpecificationBeenWrittenAsAnswer = false;
		}

	protected void storeWrittenSentenceStringBuffer()
		{
		storedSentenceStringBuffer_ = GlobalVariables.writtenSentenceStringBuffer;
		}

	protected void storeWrittenSentenceStringBufferWithOneSpecificationOnly()
		{
		storedSentenceWithOnlyOneSpecificationStringBuffer_ = GlobalVariables.writtenSentenceStringBuffer;
		}

	protected boolean hasCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isCompoundCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasNonCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.isNonCompoundCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasNonExclusiveSpecificationCollection()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isNonExclusiveCollection( specificationCollectionNr_ ) );
		}

	protected boolean hasOnlyOneRelationWord()
		{
		return ( relationCollectionNr_ == Constants.NO_COLLECTION_NR && 
				relationWordItem_ != null );
		}

	protected boolean hasPrimaryActiveAssignmentJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isPrimaryActiveAssignmentSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	protected boolean hasRelationWord()
		{
		return ( relationWordItem_ != null );
		}

	protected boolean hasRelationCollection()
		{
		return ( relationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasRelationCollectionCurrentlyBeenUpdated()
		{
		WordItem currentCollectionWordItem;

		if( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr_ ) ) != null )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem.hasCollectionCurrentlyBeenUpdated( relationCollectionNr_ ) )
					return true;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr_ ) ) != null );
			}

		return false;
		}

	protected boolean hasSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected boolean hasSpecificationContext()
		{
		return ( specificationContextNr_ > Constants.NO_CONTEXT_NR );
		}

	protected boolean hasUpdatedInformation()
		{
		return ( !isOlderItem() ||

				( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				hasRelationCollectionCurrentlyBeenUpdated() ) );
		}

	protected boolean hasUpdatedWriteInformation()
		{
		JustificationItem attachedJustificationItem;

		return ( !isOlderItem() ||

				( relationWordItem_ != null &&

					// Has been inactivated during this sentence
					// Test file: "ambiguity\US presidents"
					( hasCurrentInactiveSentenceNr() ||

					// Has been archived during this sentence
					// Test file: "ambiguity\US presidents"
					hasCurrentArchivedSentenceNr() ||

					( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
					hasCurrentCreationSentenceNr() &&

						// Test files: "reasoning\family\Complex (12)",
						//				"reasoning\family\Complex (14)",
						//				"reasoning\family\Complex (16)",
						//				"reasoning\family\Complex (20)",
						//				"reasoning\family\I know (11)",
						//				among others
						( ( storedSentenceStringBuffer_ == null &&
						!hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) ||

						( hasRelationCollectionCurrentlyBeenUpdated() &&
						firstJustificationItem_ != null &&

							( ( !isSpecificationWordSpanishAmbiguous() &&

								// Test files: "question answering\family\Indirectly answered questions (with confirmation)",
								//				"reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
								//				"reasoning\family\Complex (11)",
								//				"reasoning\family\Complex (18)",
								//				"reasoning\family\Correcting invalidated assumption (by knowledge)",
								//				"reasoning\family\I know (3)",
								//				"reasoning\family\I know (7)",
								//				among others
								( isPossessive_ ||

								// Test files: "question answering\family\Indirectly answered questions (with confirmation)",
								//				"reasoning\family\Anna - John - Paul, Joe and Laura",
								//				"reasoning\family\Anna was the mother of Paul, Joe and Laura",
								//				"reasoning\family\Anna was the mother of Paul, Joe and Laura",
								//				"reasoning\family\Complex (4)",
								//				"reasoning\family\Complex (6)",
								//				"reasoning\family\Complex (15)",
								//				among others
								firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == null ||

								( !relationWordItem_.isUserGeneralizationWord &&

									// Test files: "reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
									//				"reasoning\family\Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura",
									//				"reasoning\family\Complex (3)",
									//				"reasoning\family\Complex (4)",
									//				"reasoning\family\Complex (5)",
									//				"reasoning\family\Complex (6)",
									//				among others
									( ( !firstJustificationItem_.isOlderItem() &&
									!firstJustificationItem_.isOppositePossessiveSpecificationAssumption() ) ||

									// Test files: "reasoning\family\Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura",
									//				"reasoning\family\Complex (18)",
									//				"reasoning\family\Complex (19 - mixed)",
									//				"reasoning\family\Complex (19 - strange)"
									//				"reasoning\family\Complex (21)",
									//				"reasoning\family\I know (11)",
									//				among others
									( ( attachedJustificationItem = firstJustificationItem_.attachedJustificationItem() ) != null &&
									attachedJustificationItem.isReversibleAssumptionOrConclusion() ) ) ) ) ) ||

							// Typical for Spanish
							( isSpecificationWordSpanishAmbiguous() &&

								// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
								//						"razonamiento\familia\Complejo (19 - extraño)"
								( !isPossessive_ ||

								( !firstJustificationItem_.isOlderItem() &&

									// Spanish test files: "razonamiento\familia\Complejo (3)",
									//						"razonamiento\familia\Complejo (6)",
									//						"razonamiento\familia\Complejo (7 - Becky)",
									//						"razonamiento\familia\Complejo (7 - Claudia)",
									//						among others
									( !firstJustificationItem_.isReversibleAssumptionOrConclusion() ||

									// Spanish test files: "razonamiento\familia\Complejo (9)",
									//						"razonamiento\familia\Complejo (13)",
									//						"razonamiento\familia\Complejo (15)",
									//						"razonamiento\familia\Sé (3)",
									//						among others
									( !hasSecondaryActiveAssignmentSpecification() &&
									!firstJustificationItem_.isPrimaryActiveAssignmentSpecification() ) ) ) ) ) ) ) ) ) ) ) );
		}

	protected boolean isAdjectiveSpecification()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_ADJECTIVE );
		}

	protected boolean isAnsweredSelfGeneratedQuestion()
		{
		return ( isAnsweredQuestion_ &&
				firstJustificationItem_ != null );
		}

	protected boolean isCharacteristicFor()
		{
		return isCharacteristicFor_;
		}

	protected boolean isConditional()
		{
		return isConditional_;
		}

	protected boolean isCorrectedSpecification()
		{
		return isCorrectedSpecification_;
		}

	protected boolean isCorrectSpecificationArticle( boolean isDefiniteArticle, boolean isWritingCurrentSpecificationWordOnly, short articleParameter )
		{
		boolean isSelfGenerated;
		SpecificationItem searchSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem foundWordItem = null;

		if( isWritingCurrentSpecificationWordOnly )
			foundWordItem = specificationWordItem_;
		else
			{
			if( ( searchSpecificationItem = myWordItem().firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != null )
				{
				isSelfGenerated = isSelfGeneratedSpecification();

				do	{
					if( ( currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ :
														searchSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGenerated, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, relationCollectionNr_, generalizationContextNr_, relationWordItem_ ) ) ) != null &&
					!currentSpecificationWordItem.isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
						foundWordItem = currentSpecificationWordItem;
					}
				while( foundWordItem == null &&
				( searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion_ ) ) != null );
				}
			}

		return ( foundWordItem == null ? true : ( isDefiniteArticle ? foundWordItem.isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem.isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
		}

	protected boolean isCorrectArticleOfFirstUnwrittenProperNounRelationWord( short definiteArticleParameter )
		{
		WordItem firstProperNounUnwrittenRelationWordItem;

		return ( ( firstProperNounUnwrittenRelationWordItem = firstUnwrittenRelationWordItem( Constants.WORD_TYPE_PROPER_NOUN, null ) ) != null &&
				firstProperNounUnwrittenRelationWordItem.isProperNounPrecededByDefiniteArticle( definiteArticleParameter ) );
		}

	protected boolean isExclusiveNonPossessiveGeneralizationAssignment()
		{
		return ( isAssignment() &&
				isExclusiveNonPossessiveGeneralization() );
		}

	protected boolean isExclusiveNonPossessiveGeneralization()
		{
		return ( isExclusiveGeneralization_ &&
				!isPossessive_ );
		}

	protected boolean isEveryGeneralization()
		{
		return isEveryGeneralization_;
		}

	protected boolean isExclusiveGeneralization()
		{
		return isExclusiveGeneralization_;
		}

	protected boolean isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	protected boolean isHiddenSpanishSpecification()
		{
		WordItem _myWordItem;

		return ( isSpecificationWordSpanishAmbiguous() &&
				generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NOUN &&
				!isNegative_ &&
				// Self-generated specification
				firstJustificationItem_ != null &&
				hasNonCompoundSpecificationCollection() &&

					// Non-possessive specification not in a male word
					( ( !isPossessive_ &&
					( _myWordItem = myWordItem() ) != null &&
					!_myWordItem.isMasculineWord() &&
					!_myWordItem.hasMasculineProperNounEnding() ) ||

					// Possessive specification without male relation context
					( isPossessive_ &&
					relationWordItem_ != null &&
					specificationWordItem_ != null &&
					specificationWordItem_.compoundCollectionNr() > Constants.NO_COLLECTION_NR &&
					!hasContextMasculineWordInRelationWords() ) ) );
		}

	protected boolean isMatchingRelation( boolean isAllowingEmptyRelation, int relationCollectionNr, WordItem relationWordItem )
		{		// Empty subset
		return ( relationWordItem == null ||

				// Same set
				( relationCollectionNr_ == relationCollectionNr &&
				relationWordItem_ == relationWordItem ) ||

				// No relation word
				( relationWordItem_ == null &&
				isAllowingEmptyRelation ) ||

				// Relation word
				( relationWordItem_ != null &&

					( ( relationCollectionNr > Constants.NO_COLLECTION_NR &&
					relationWordItem.hasCollectionNr( relationCollectionNr ) &&
					relationWordItem_.hasCollectionNr( relationCollectionNr ) ) ||

					( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
					relationWordItem.hasCollectionNr( relationCollectionNr_ ) &&
					relationWordItem_.hasCollectionNr( relationCollectionNr_ ) ) ) ) );
		}

	protected boolean isNegative()
		{
		return isNegative_;
		}

	protected boolean isNounGeneralization()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isNounSpecification()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isNumeralSpecification()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isPartOf()
		{
		return isPartOf_;
		}

	protected boolean isPossessive()
		{
		return isPossessive_;
		}

	protected boolean isPluralNounSpecification()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isProperNounGeneralization()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NOUN );
		}

	protected boolean isRelatedSpecification( int answerSpecificationCollectionNr, int questionSpecificationCollectionNr, WordItem specificationWordItem )
		{
		return ( ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == answerSpecificationCollectionNr ) ||

				( questionSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

					( specificationCollectionNr_ == questionSpecificationCollectionNr ||
					specificationWordItem_ == specificationWordItem ) ) );
		}

	protected boolean isSelfGeneratedAssumption()
		{
		if( !isSpecificationGeneralization_ &&
		// Not a question
		questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
		// Self-generated
		firstJustificationItem_ != null )
			{
			if( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL &&

			//Avoid idle calls
				( relationWordItem_ != null ||
				!isOlderItem() ) &&

			hasAtLeastOneAssumptionJustification() )
				// Calculate assumption level
				calculateAssumptionLevel();

			return ( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL );
			}

		return false;
		}

	protected boolean isSelfGeneratedConclusion()
		{
		return ( !isSpecificationGeneralization_ &&
				// No assumption level
				assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL &&
				// Not a question
				questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
				// Self-generated
				firstJustificationItem_ != null );
		}

	protected boolean isSelfGeneratedConclusionOrAssumption()
		{
		return ( !isSpecificationGeneralization_ &&
				// Not a question
				questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
				// Self-generated
				firstJustificationItem_ != null );
		}

	protected boolean isSelfGeneratedQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER &&
				firstJustificationItem_ != null );
		}

	protected boolean isSelfGeneratedSpecification()
		{
		return ( firstJustificationItem_ != null );
		}

	protected boolean isSingularNounSpecification()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isSpecific()
		{
		return isSpecific_;
		}

	protected boolean isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	protected boolean isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				nRelationWords() > 1 );
		}

	protected boolean isSpecificationWordSpanishAmbiguous()
		{
		return ( specificationWordItem_ != null &&
				specificationWordItem_.isNounWordSpanishAmbiguous() );
		}

	protected boolean isTriggeringExecutionOfSelections()
		{
		return ( isAssignment() ||
				generalizationWordTypeNr_ == Constants.WORD_TYPE_VERB_SINGULAR );
		}

	protected boolean isUncountableGeneralizationNoun()
		{
		return isUncountableGeneralizationNoun_;
		}

	protected boolean isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	protected boolean isUserSpecification()
		{
		return ( questionParameter_ == Constants.NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == null );
		}

	protected boolean isValueSpecification()
		{
		return isValueSpecification_;
		}

	protected boolean wasHiddenSpanishSpecification()
		{
				// Self-generated specification
		return ( firstJustificationItem_ != null &&
				relationWordItem_ != null &&
				// Not written yet (as unhidden Spanish specification)
				storedSentenceStringBuffer_ == null &&
				isSpecificationWordSpanishAmbiguous() &&
				isOlderItem() &&
				!isHiddenSpanishSpecification() &&

					// Spanish test files: "conflicto\familia\José es una madre",
					//						"razonamiento\familia\Complejo (9)",
					//						"razonamiento\familia\Complejo (13)",
					//						"razonamiento\familia\Complejo (14)",
					//						"razonamiento\familia\Complejo (16)",
					//						"razonamiento\familia\Complejo (18)",
					//						among others
					( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL ||
					// Spanish test file: "razonamiento\familia\Mis conclusiones que se confirman (José tiene 2 padres)"
					firstJustificationItem_.isPossessivePrimarySpecification() ) );
		}

	protected short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	protected short prepositionParameter()
		{
		return prepositionParameter_;
		}

	protected short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	protected short relationWordTypeNr()
		{
		return relationWordTypeNr_;
		}

	protected int generalizationCollectionNr()
		{
		return generalizationCollectionNr_;
		}

	protected int specificationCollectionNr()
		{
		return specificationCollectionNr_;
		}

	protected int relationCollectionNr()
		{
		return relationCollectionNr_;
		}

	protected int generalizationContextNr()
		{
		return generalizationContextNr_;
		}

	protected int specificationContextNr()
		{
		return specificationContextNr_;
		}

	protected int relationContextNr()
		{
		return relationContextNr_;
		}

	protected int nEnteredRelationWords()
		{
		return nEnteredRelationWords_;
		}

	protected int nInvolvedSpecificationWords()
		{
		int myCreationSentenceNr = creationSentenceNr();
		int nInvolvedSpecificationWords = 0;
		SpecificationItem searchSpecificationItem = myWordItem().firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.creationSentenceNr() == myCreationSentenceNr )
				{
				if( !searchSpecificationItem.hasUserSpecificationWordItem() )
					return 0;

				nInvolvedSpecificationWords++;
				}

			searchSpecificationItem = searchSpecificationItem.nextSelectedQuestionParameterSpecificationItem();
			}

		return nInvolvedSpecificationWords;
		}

	protected byte addJustificationToSpecification( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem createdJustificationItem;
		JustificationResultType justificationResult;

		if( isReplacedOrDeletedItem() )
			return startError( 1, null, "I am replaced or deleted" );

		if( isUserSpecification() )
			return startError( 1, null, "I am a user specification" );

		if( primarySpecificationItem == null )
			return startError( 1, null, "The given primary specification item is undefined" );

		if( primarySpecificationItem == this )
			return startError( 1, null, "The given primary specification item is the same as me" );

		if( additionalDefinitionSpecificationItem == this )
			return startError( 1, null, "The given additional definition specification item is the same as me" );

		if( secondarySpecificationItem == this )
			return startError( 1, null, "The given secondary specification item is the same as me" );

		// Create justification
		if( ( justificationResult = myWordItem().createJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
			return addError( 1, null, "I failed to create a justification item to my word" );

		if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
			return startError( 1, null, "I couldn't create an additional justification" );

		if( attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to attach the created justification to myself" );

		return Constants.RESULT_OK;
		}

	protected byte addUserSpecificationToSpanishJustifications( SpecificationItem userSpecificationItem )
		{
		JustificationItem currentJustificationItem = firstJustificationItem_;
		SpecificationItem selfGeneratedSpecificationItem;
		WordItem secondaryGeneralizationWordItem;

		if( userSpecificationItem == null )
			return startError( 1, null, "The given user specification item is undefined" );

		if( specificationWordItem_ == null )
			return startError( 1, null, "I have no specification word item" );

		do	{
			if( ( secondaryGeneralizationWordItem = currentJustificationItem.secondaryGeneralizationWordItem() ) != null &&
			( selfGeneratedSpecificationItem = secondaryGeneralizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, !isPossessive_, true, specificationWordItem_, null ) ) != null &&
			!selfGeneratedSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
			// Add user specification reversible justification to Spanish primary specification
			selfGeneratedSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to add a user specification reversible justification to the primary specification" );

			currentJustificationItem = currentJustificationItem.attachedJustificationItem();
			}
		while( currentJustificationItem != null );

		return Constants.RESULT_OK;
		}

	protected byte attachJustificationToSpecification( JustificationItem attachJustificationItem )
		{
		boolean _hasRelationWord = ( relationWordItem_ != null );
		JustificationItem lastAttachedJustificationItem;
		JustificationItem obsoleteJustificationItem = null;
		JustificationItem originalFirstJustificationItem;
		JustificationItem tempJustificationItem;
		SpecificationItem attachedPrimarySpecificationItem;
		SpecificationItem attachedSecondarySpecificationItem;
		SpecificationItem createdSpecificationItem;
		WordItem _myWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult;

		if( attachJustificationItem == null )
			return startError( 1, null, "The given attached justification item is undefined" );

		if( !attachJustificationItem.isActiveItem() )
			return startError( 1, null, "The given attached justification item isn't active" );

		if( isReplacedOrDeletedItem() )
			return startError( 1, null, "I am replaced or deleted" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I have no justification items" );

		if( ( attachedPrimarySpecificationItem = attachJustificationItem.primarySpecificationItem() ) != null )
			{
			if( attachedPrimarySpecificationItem.hasRelationWord() &&
			!attachedPrimarySpecificationItem.isPossessive() )
				obsoleteJustificationItem = firstJustificationItem_.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem.specificationWordItem() );
			else
				{
				// Typical for Spanish
				if( isSpecificationWordSpanishAmbiguous() &&
				_hasRelationWord &&
				( attachedSecondarySpecificationItem = attachJustificationItem.secondarySpecificationItem() ) != null &&
				attachedSecondarySpecificationItem.hasCompoundSpecificationCollection() &&
				!attachedSecondarySpecificationItem.isPossessive() &&
				attachedSecondarySpecificationItem.isUserSpecification() )
					obsoleteJustificationItem = firstJustificationItem_.obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
				}
			}

		if( obsoleteJustificationItem == null )
			{
			// Find a justification item with current sentence number, but without attached justification item
			lastAttachedJustificationItem = firstJustificationItem_;

			while( ( tempJustificationItem = lastAttachedJustificationItem.attachedJustificationItem() ) != null )
				lastAttachedJustificationItem = tempJustificationItem;

			if( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() )
				{
				if( lastAttachedJustificationItem.attachJustification( attachJustificationItem, this ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to attach a justification item" );

				// Test files: "reasoning\family\Complex (3)",
				//				"reasoning\family\Laura - Joe - John and Anna - Paul",
				//				"reasoning\family\Laura - Joe - Paul - John and Anna"
				if( !_hasRelationWord &&
				specificationCollectionNr_ == Constants.NO_COLLECTION_NR &&
				!isOlderItem() &&
				attachJustificationItem.justificationAssumptionLevel() < assumptionLevel_ &&
				attachJustificationItem.isSpecificationSubstitutionAssumptionOrConclusion() &&
				// Recalculate my assumption level
				recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to recalculate my assumption level" );
				}
			else
				{
				if( attachJustificationItem.attachedJustificationItem() != null )
					return startError( 1, null, "The given attached justification has a justification attached" );

				if( hasCurrentCreationSentenceNr() )
					{
					originalFirstJustificationItem = firstJustificationItem_;

					if( changeFirstJustification( true, attachJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to change my first justification item" );

					if( attachJustificationItem.attachJustification( originalFirstJustificationItem, this ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to attach the first justification item of myself to the given attached justification item" );
					}
				else
					{
					// Create copy of myself with different first justification item
					if( ( createAndAssignResult = _myWordItem.createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isCharacteristicFor_, isConditional(), isCorrectedSpecification_, isEveryGeneralization_, isExclusiveGeneralization_, isExclusiveSpecification_, isNegative_, isPartOf_, isPossessive_, isSpecific_, isSpecificationGeneralization_, isUncountableGeneralizationNoun_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr(), prepositionParameter(), questionParameter(), generalizationWordTypeNr(), specificationWordTypeNr(), relationWordTypeNr(), specificationCollectionNr_, relationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nEnteredRelationWords_, attachJustificationItem, specificationWordItem_, relationWordItem_, specificationString_, storedSentenceStringBuffer_, storedSentenceWithOnlyOneSpecificationStringBuffer_ ) ).result != Constants.RESULT_OK )
						return addError( 1, null, "I failed to create a copy of myself with a different first justification item" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						return startError( 1, null, "I couldn't create a specification" );

					if( attachJustificationItem.attachJustification( firstJustificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );

					if( _myWordItem.replaceOrDeleteSpecification( this, createdSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace or delete a specification" );
					}
				}
			}
		else
			{
			if( _hasRelationWord &&
			// Replace obsolete justification item
			_myWordItem.replaceJustification( obsoleteJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to replace the obsolete justification item" );
			}

		return Constants.RESULT_OK;
		}

	protected byte changeSpecification( boolean isNewAnsweredQuestion, boolean isNewExclusiveGeneralization, int newGeneralizationCollectionNr, int newSpecificationCollectionNr, int newRelationCollectionNr, JustificationItem newFirstJustificationItem )
		{
		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		isAnsweredQuestion_ = isNewAnsweredQuestion;
		isExclusiveGeneralization_ = isNewExclusiveGeneralization;
		generalizationCollectionNr_ = newGeneralizationCollectionNr;
		specificationCollectionNr_ = newSpecificationCollectionNr;
		relationCollectionNr_ = newRelationCollectionNr;
		firstJustificationItem_ = newFirstJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralization( boolean isExclusiveGeneralization, int generalizationCollectionNr )
		{
		if( generalizationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The given generalization collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveGeneralization )
			isExclusiveGeneralization_ = true;

		if( generalizationCollectionNr_ > Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The generalization is already collected" );

		generalizationCollectionNr_ = generalizationCollectionNr;

		return Constants.RESULT_OK;
		}

	protected byte collectSpecification( boolean isExclusiveSpecification, int specificationCollectionNr )
		{
		if( specificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The given specification collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveSpecification )
			isExclusiveSpecification_ = true;

		if( specificationCollectionNr_ > Constants.NO_COLLECTION_NR )
			return startError( 1, null, "The specification is already collected" );

		specificationCollectionNr_ = specificationCollectionNr;

		return Constants.RESULT_OK;
		}

	protected byte markAsAnsweredQuestion()
		{
		if( hasCurrentCreationSentenceNr() )
			isAnsweredQuestion_ = true;
		else
			{
			if( myWordItem().changeSpecification( true, isExclusiveGeneralization_, generalizationCollectionNr_, specificationCollectionNr_, relationCollectionNr_, firstJustificationItem_, this ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to change myself into an answered question" );
			}

		return Constants.RESULT_OK;
		}

	protected byte markSpecificationStringAsWritten()
		{
		if( GlobalVariables.currentWriteLevel >= Constants.MAX_LEVEL )
			return startSystemError( 1, null, "Current write word level overflow" );

		if( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL )
			return startError( 1, null, "My write level is already assigned" );

		specificationStringWriteLevel_ = ++GlobalVariables.currentWriteLevel;

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptionLevel( boolean isSituationStable )
		{
		boolean hasChangedAssumptionLevel;
		boolean _hasRelationWord = ( relationWordItem_ != null );
		boolean isOlderSpecification = isOlderItem();
		short previousAssumptionLevel = assumptionLevel_;
		JustificationItem attachedJustificationItem;
		SpecificationItem partOfSpecificationItem;
		WordItem currentCollectionWordItem;
		WordItem _myWordItem = myWordItem();
		WordItem partOfSpecificationWordItem;

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification" );

		if( specificationWordItem_ == null )
			return startError( 1, null, "I have no specification word" );

		if( isReplacedOrDeletedItem() )
			return startError( 1, null, "I am a replaced or deleted specification" );

		// Calculate assumption level
		if( calculateAssumptionLevel() != Constants.RESULT_OK )
			return addError( 1, null, "I failed to calculate my assumption level" );

		if( hasCurrentCreationSentenceNr() )
			{
			hasChangedAssumptionLevel = ( previousAssumptionLevel != assumptionLevel_ );

			if( ( ( isOlderSpecification &&
			!isNegative_ ) ||

			( isSituationStable &&

				// No relation word
				// Test file: "reasoning\family\John - Anna (before family definition)"
				( ( !_hasRelationWord &&
				!hasChangedAssumptionLevel &&
				isPartOf_ &&
				highestJustificationAssumptionLevel() > assumptionLevel_ ) ||

				// Relation word
				( _hasRelationWord &&
				hasNonCompoundSpecificationCollection() &&

					// Non-possessive
					// Test files: "reasoning\family\Complex (3)",
					//				"reasoning\family\Complex (12)",
					//				"reasoning\family\John - Anna (before family definition)"
					( ( !isPossessive_ &&
					_myWordItem.isUserRelationWord ) ||

					// Possessive
					// Test files: "reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (19 - strange)",
					//				"reasoning\family\John - Laura - John and Anna"
					( !hasChangedAssumptionLevel &&
					isPossessive_ ) ) ) ) ) ) &&

			// Remove obsolete assumption justifications
			removeObsoleteAssumptionJustifications( isSituationStable ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to remove obsolete assumption justifications from this specification" );

			if( hasChangedAssumptionLevel &&
			isOlderSpecification &&

				// Only one relation word
				( ( relationCollectionNr_ == Constants.NO_COLLECTION_NR &&

					// Adjusted specifications
					( ( ( assumptionLevel_ < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||

					// Test files: "reasoning\family\Complex (13)",
					//				"reasoning\family\Complex (18)",
					//				"reasoning\family\Complex (19 - mixed)",
					//				"reasoning\family\Complex (19 - strange)"
					previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) &&

					// Skip writing update of specification part of assignment
					_myWordItem.firstAssignmentItem( isPossessive_, false, specificationWordItem_ ) == null ) ||

					// Past-tense specifications
					// Test files: "conflict\family\John was the father of Paul (before family definition)",
					//				"question answering\family\Just a few questions (1)",
					//				"reasoning\family\James was the father of Peter (before family definition)"
					isAssignment() ) ) ||

				// Relation word collection
				( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
				assumptionLevel_ < previousAssumptionLevel &&
				assumptionLevel_ < Constants.NUMBER_OF_ASSUMPTION_LEVELS &&

					( ( !hasRelationCollectionCurrentlyBeenUpdated() &&

						// Test files: "question answering\family\Intelligent answering of questions",
						//				"reasoning\family\Avoid duplicate context",
						//				"reasoning\family\Complex (1)",
						//				"reasoning\family\Complex (3)",
						//				among others
						( !firstJustificationItem_.isPrimaryActiveAssignmentSpecification() ||

						// Test file: "reasoning\family\I know (3)"
						( ( attachedJustificationItem = firstJustificationItem_.attachedJustificationItem() ) != null &&
						attachedJustificationItem.isOppositePossessiveSpecificationAssumption() ) ) ) ||

					// Typical for Spanish: previously hidden Spanish ambiguous specifications
					// Spanish test files: "conflicto\familia\José es una madre",
					//						"razonamiento\familia\Complejo (6)",
					//						"razonamiento\familia\Complejo (9)",
					//						"razonamiento\familia\Complejo (11)",
					//						"razonamiento\familia\Complejo (13)",
					//						among others
					assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ) ) ) &&

				// Write adjusted specification
				_myWordItem.writeUpdatedSpecification( true, false, false, false, false, false, false, this ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to write an adjusted specification" );

			if( relationWordItem_ != null &&

				// Changed assumption level
				( hasChangedAssumptionLevel ||
				_myWordItem.isUserGeneralizationWord ) &&

			( partOfSpecificationWordItem = specificationWordItem_.commonWordItem( specificationCollectionNr_ ) ) != null )
				{
				// Only one relation word
				if( relationCollectionNr_ == Constants.NO_COLLECTION_NR )
					{
					// Test files: "question answering\family\Indirectly answered questions (with confirmation)",
					//				"question answering\family\Negative and alternative answers",
					//				"reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
					//				"reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				among others
					if( isPossessive_ &&
					// Add justifications to specifications of my relation word
					addJustificationsToSpecificationsOfRelationWords( relationWordItem_, partOfSpecificationWordItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add justifications to the specifications of my relation word" );
					}
				else	// Relation collection
					{
					if( ( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr_ ) ) != null )
						{
						// Do for all collection words
						do	{
							// Add justifications to specifications of current relation word
							if( addJustificationsToSpecificationsOfRelationWords( currentCollectionWordItem, partOfSpecificationWordItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to add justifications to the specifications of current relation word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Luisa - Juan y Ana - José"
							if( _myWordItem.hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore &&
							!currentCollectionWordItem.isUserGeneralizationWord &&
							!specificationWordItem_.isUserSpecificationWord &&
							( partOfSpecificationItem = currentCollectionWordItem.firstSpecificationItem( false, false, false, partOfSpecificationWordItem ) ) != null &&
							// Recalculate assumption level of part-of specification in current collection word
							partOfSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to recalculate assumption level of part-of specification in current collection word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );
							}
						while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr_ ) ) != null );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte removeObsoleteAssumptionJustifications( boolean isSituationStable )
		{
		boolean hasConfirmedAnySpecification = GlobalVariables.hasConfirmedAnySpecification;
		boolean isIncludingExclusiveSpecificationSubstitutionAssumption;
		boolean isIncludingOppositePossessiveSpecificationAssumption;
		boolean isIncludingReversibleAssumptionOrConclusion;
		boolean isIncludingSpecificationSubstitutionAssumptionOrConclusion;
		boolean isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion;
		boolean isOlderSpecification = isOlderItem();
		JustificationItem attachedJustificationItem;
		JustificationItem copiedJustificationItem;
		JustificationItem currentJustificationItem = firstJustificationItem_;
		JustificationItem mostRecentCurrentJustificationItem = null;
		JustificationItem oldJustificationItem;
		JustificationItem previousJustificationItem = null;
		JustificationItem previousPreviousJustificationItem = null;
		WordItem _myWordItem = myWordItem();
		JustificationResultType justificationResult;

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			return startError( 1, null, "I am a question" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

																// Test files: "reasoning\family\Complex (18)",
																//				"reasoning\family\Complex (19 - mixed)",
																//				"reasoning\family\Complex (19 - strange)"
		isIncludingExclusiveSpecificationSubstitutionAssumption = ( isSituationStable ||

																	// Spanish test files: "razonamiento\familia\Mis suposiciones que se confirman (Juan y Ana)",
																	//						"razonamiento\familia\Que haya confirmado al menos una palabra de relación"
																	relationCollectionNr_ == Constants.NO_COLLECTION_NR );

																// Test files: "reasoning\family\Complex (1)",
																//				"reasoning\family\John - Laura - John and Anna",
																//				"reasoning\family\My assumptions that are confirmed (Paul, Joe and Laura)"
		isIncludingOppositePossessiveSpecificationAssumption = ( ( isSituationStable &&
																relationCollectionNr_ > Constants.NO_COLLECTION_NR ) ||

																// Typical for Spanish
																// Spanish test files: "razonamiento\familia\Complejo (1)",
																//						"razonamiento\familia\Complejo (14)",
																//						"razonamiento\familia\Mis suposiciones que se confirman (hijos)",
																//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa"
																isSpecificationWordSpanishAmbiguous() );

		isIncludingReversibleAssumptionOrConclusion = ( isSituationStable &&
														// Test file: "reasoning\family\Complex (20)"
														!firstJustificationItem_.isOlderItem() );

																		// Test files: "reasoning\family\Complex (3)",
																		//				"reasoning\family\Correcting invalidated assumption (by knowledge)",
																		//				"reasoning\family\Laura - Joe - John and Anna - Paul",
																		//				"reasoning\family\Laura - Joe - Paul - John and Anna"
		isIncludingSpecificationSubstitutionAssumptionOrConclusion = ( !hasConfirmedAnySpecification ||

																		// Test files: "reasoning\family\Complex (20)"
																		//				"reasoning\family\My assumptions that are confirmed (Paul, Joe and Laura)"
																		( isSituationStable &&
																		relationCollectionNr_ > Constants.NO_COLLECTION_NR ) );

		isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion = ( !hasConfirmedAnySpecification &&
																			!firstJustificationItem_.isOlderItem() );

		if( isIncludingExclusiveSpecificationSubstitutionAssumption ||
		isIncludingOppositePossessiveSpecificationAssumption ||
//		isIncludingReversibleAssumptionOrConclusion ||
		isIncludingSpecificationSubstitutionAssumptionOrConclusion /*||
		isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion*/ )
			{
			do	{
				if( currentJustificationItem.isObsoleteAssumptionJustification( isIncludingOppositePossessiveSpecificationAssumption, isIncludingReversibleAssumptionOrConclusion, isIncludingSpecificationSubstitutionAssumptionOrConclusion, isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion, isOlderSpecification, assumptionLevel_ ) )
					{
					attachedJustificationItem = currentJustificationItem.attachedJustificationItem();

					if( previousJustificationItem == null ||
					previousJustificationItem.hasCurrentCreationSentenceNr() )
						{
						if( currentJustificationItem.hasCurrentCreationSentenceNr() )
							{
							// Disconnect current justification item
							if( currentJustificationItem.changeAttachedJustification( null ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the attached justification item of the current justification item" );
							}
						else
							{
							// Replace current justification
							if( _myWordItem.replaceOrDeleteJustification( currentJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to replace the current justification item" );
							}

						if( previousJustificationItem == null )
							{
							if( attachedJustificationItem != null &&
							// Change first justification item
							changeFirstJustification( !currentJustificationItem.isReplacedItem(), attachedJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change my first justification item" );
							}
						else
							{
							// Remove current justification item
							if( previousJustificationItem.changeAttachedJustification( attachedJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the attached justification item of the previous justification item" );
							}

						currentJustificationItem = attachedJustificationItem;
						}
					else
						{
						if( previousPreviousJustificationItem == null ||
						previousPreviousJustificationItem.hasCurrentCreationSentenceNr() )
							{
							// Copy previous justification
							if( ( justificationResult = _myWordItem.copyJustificationItem( previousJustificationItem.primarySpecificationItem(), previousJustificationItem.secondarySpecificationItem(), attachedJustificationItem, previousJustificationItem ) ).result != Constants.RESULT_OK )
								return addError( 1, null, "I failed to copy the previous justification item" );

							if( ( copiedJustificationItem = justificationResult.createdJustificationItem ) == null )
								return startError( 1, null, "The copied justification item is undefined" );

							if( previousPreviousJustificationItem == null )
								{
								// Change first justification item to created justification
								if( changeFirstJustification( true, copiedJustificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, "I failed to change my first justification item to the created justification" );
								}
							else
								{
								// Change attached justification item of previous previous justification to created justification
								if( previousPreviousJustificationItem.changeAttachedJustification( copiedJustificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, "I failed to change the attached justification item of the previous previous justification item to the created justification" );
								}

							// Replace previous justification
							if( _myWordItem.replaceOrDeleteJustification( previousJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to replace the previous justification item" );

							// Replace current justification
							if( _myWordItem.replaceOrDeleteJustification( currentJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to replace the current justification item" );

							// Reset variables
							mostRecentCurrentJustificationItem = null;
							previousPreviousJustificationItem = null;
							previousJustificationItem = null;
							currentJustificationItem = firstJustificationItem_;
							}
						else
							{
							while( mostRecentCurrentJustificationItem != null &&
							( oldJustificationItem = mostRecentCurrentJustificationItem.attachedJustificationItem() ) != null &&
							oldJustificationItem != currentJustificationItem )
								{
								// Copy old justification
								if( ( justificationResult = _myWordItem.copyJustificationItem( oldJustificationItem.primarySpecificationItem(), oldJustificationItem.secondarySpecificationItem(), oldJustificationItem.attachedJustificationItem(), oldJustificationItem ) ).result != Constants.RESULT_OK )
									return addError( 1, null, "I failed to copy the previous justification item" );

								if( ( copiedJustificationItem = justificationResult.createdJustificationItem ) == null )
									return startError( 1, null, "The copied justification item is undefined" );

								// Connect copied justification to most recent current justification
								if( mostRecentCurrentJustificationItem.changeAttachedJustification( copiedJustificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, "I failed to disconnect the attached justification item of the search justification item" );

								// Remove obsolete justification
								if( _myWordItem.replaceOrDeleteJustification( oldJustificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, "I failed to replace an obsolete justification" );

								// Copy and replace next old justification
								mostRecentCurrentJustificationItem = copiedJustificationItem;
								}

							// Reset variables
							mostRecentCurrentJustificationItem = null;
							previousPreviousJustificationItem = null;
							previousJustificationItem = null;
							currentJustificationItem = firstJustificationItem_;
							}
						}
					}
				else
					{
					previousPreviousJustificationItem = previousJustificationItem;
					previousJustificationItem = currentJustificationItem;

					if( currentJustificationItem.hasCurrentCreationSentenceNr() )
						mostRecentCurrentJustificationItem = currentJustificationItem;

					currentJustificationItem = currentJustificationItem.attachedJustificationItem();
					}
				}
			while( currentJustificationItem != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSpecificationConflict( boolean isQuestion, boolean isWritingCurrentSpecificationWordOnly )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( myWordItem().writeSelectedSpecification( false, isWritingCurrentSpecificationWordOnly, this ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write a conflicting specification" );

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() == 0 )
			return startError( 1, null, "The written sentence string buffer is empty" );

		if( InputOutput.writeInterfaceText( true, ( isQuestion ? Constants.INPUT_OUTPUT_PROMPT_WRITE : Constants.INPUT_OUTPUT_PROMPT_WARNING ), ( isOlderItem() ? ( isQuestion ? Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_DEFINITION : Constants.INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_DEFINITION ) : Constants.INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write an interface warning" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write the conflict sentence" );

		return Constants.RESULT_OK;
		}

	protected String specificationString()
		{
		return specificationString_;
		}

	protected String activeGeneralizationWordTypeString()
		{
		return myWordItem().activeWordTypeString( generalizationWordTypeNr_ );
		}

	protected String activeSpecificationWordTypeString()
		{
		return ( specificationWordItem_ != null ?
				specificationWordItem_.activeWordTypeString( specificationWordTypeNr_ ) : null );
		}

	protected StringBuffer storedSentenceStringBuffer()
		{
		return storedSentenceStringBuffer_;
		}

	protected StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer()
		{
		return storedSentenceWithOnlyOneSpecificationStringBuffer_;
		}

	protected JustificationItem firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	protected SpecificationItem candidateForQuestionSpecificationItem( boolean isAllowingSpanishPossessiveSpecification, boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isCandidateForQuestion( isAllowingSpanishPossessiveSpecification ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem exclusiveSpecificationItem( boolean isIncludingAdjectives, boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isExclusiveSpecification() &&
			!searchSpecificationItem.isAnsweredQuestion() &&

				( isIncludingAdjectives ||
				!searchSpecificationItem.isAdjectiveSpecification() ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem olderNonPossessiveNonQuestionSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isOlderItem() &&
			!searchSpecificationItem.isPossessive() &&
			!searchSpecificationItem.isQuestion() &&
			searchSpecificationItem.hasSpecificationCollection() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nonNegativeNonPosessiveDefinitionSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.isNegative() &&
			!searchSpecificationItem.isSpecificationGeneralization() &&
			// Typical for Chinese
			// Chinese test files: "Boiling point",
			//						"Condensation point"
			!searchSpecificationItem.isPossessive() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nonQuestionSpecificationItem( boolean isAllowingEmptyRelation, boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( ( isAllowingEmptyRelation ||
			searchSpecificationItem.hasRelationWord() ) &&

			!searchSpecificationItem.isQuestion() &&
			!searchSpecificationItem.isHiddenSpanishSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem possessiveSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isPossessive() &&
			searchSpecificationItem.hasSpecificationCollection() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem specificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, boolean isQuestion )
		{
		short currentLanguageNr = GlobalVariables.currentLanguageNr;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isQuestion() == isQuestion &&

				( isLanguageWord_ ||
				// Skip other languages
				searchSpecificationItem.languageNr_ == currentLanguageNr ) &&

				( isIncludingAnsweredQuestions ||
				// Skip answered questions
				!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem specificationItem( boolean isIncludingAnsweredQuestions, boolean isIncludingThisItem, short questionParameter )
		{
		short currentLanguageNr = GlobalVariables.currentLanguageNr;
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.questionParameter_ == questionParameter &&

				( isLanguageWord_ ||
				// Skip other languages
				searchSpecificationItem.languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem.isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem negativeSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem.isNegative() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nextExclusiveSpecificationItem( boolean isIncludingAdjectives )
		{
		return exclusiveSpecificationItem( isIncludingAdjectives, false );
		}

	protected SpecificationItem nextOlderNonPossessiveNonQuestionSpecificationItem()
		{
		return olderNonPossessiveNonQuestionSpecificationItem( false );
		}

	protected SpecificationItem nextNegativeSpecificationItem()
		{
		return negativeSpecificationItem( false );
		}

	protected SpecificationItem nextNonNegativeNonPosessiveDefinitionSpecificationItem()
		{
		return nonNegativeNonPosessiveDefinitionSpecificationItem( false );
		}

	protected SpecificationItem nextPossessiveSpecificationItem()
		{
		return possessiveSpecificationItem( false );
		}

	protected SpecificationItem nextCandidateForQuestionSpecificationItem( boolean isAllowingSpanishPossessiveSpecification )
		{
		return candidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, false );
		}

	protected SpecificationItem nextNonQuestionNonRelationSpecificationItem()
		{
		return nonQuestionSpecificationItem( false, false );
		}

	protected SpecificationItem nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, isQuestion() ) :
									specificationItem( false, false, isQuestion() ) );
		}

	protected SpecificationItem nextSelectedQuestionParameterSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, questionParameter_ ) :
									specificationItem( false, false, questionParameter_ ) );
		}

	protected SpecificationItem nextSelectedQuestionParameterSpecificationItem( boolean isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? assignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) :
									specificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	protected SpecificationItem nextSpecificationItem()
		{
		return (SpecificationItem)nextItem;
		}

	protected SpecificationItem updatedSpecificationItem()
		{
		SpecificationItem updatedSpecificationItem;
		SpecificationItem searchSpecificationItem = this;

		while( ( updatedSpecificationItem = searchSpecificationItem.replacingSpecificationItem ) != null )
			searchSpecificationItem = updatedSpecificationItem;

		return searchSpecificationItem;
		}

	protected WordItem firstUnwrittenRelationWordItem( short wordTypeNr, WordItem previousRelationWordItem )
		{
		WordItem currentCollectionWordItem;

		if( relationWordItem_ != null )
			{
			if( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
			( currentCollectionWordItem = ( previousRelationWordItem == null ?
												WordItem.firstCollectionWordItem( relationCollectionNr_ ) :
												previousRelationWordItem ) ) != null )
				{
				// Do for all collection words
				do	{
					if( !currentCollectionWordItem.isRelationWordTypeAlreadyWritten( wordTypeNr ) )
						return currentCollectionWordItem;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr_ ) ) != null );
				}

			if( relationWordItem_ != previousRelationWordItem &&
			!relationWordItem_.isRelationWordTypeAlreadyWritten( wordTypeNr ) )
				return relationWordItem_;
			}

		return null;
		}

	protected WordItem generalizationWordItem()
		{
		return myWordItem();
		}

	protected WordItem mostRecentRelationWord()
		{
		WordItem currentCollectionWordItem;
		WordItem previousCollectionWordItem;

		if( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = WordItem.firstCollectionWordItem( relationCollectionNr_ ) ) != null )
			{
			// Do for all collection words
			do	previousCollectionWordItem = currentCollectionWordItem;
			while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( relationCollectionNr_ ) ) != null );

			return previousCollectionWordItem;
			}

		return relationWordItem_;
		}

	protected WordItem relationCollectionWordItem()
		{
		if( relationCollectionNr_ > Constants.NO_COLLECTION_NR &&
		relationWordItem_ != null )
			return relationWordItem_.collectionWordItem( relationCollectionNr_ );

		// If relation collection doesn't exist, return relation word
		return relationWordItem_;
		}

	protected WordItem relationWordItem()
		{
		return relationWordItem_;
		}

	protected WordItem relatedSpecificationWordItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean _isSelfGeneratedSpecification, short assumptionLevel, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, WordItem relationWordItem )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGeneratedSpecification() == _isSelfGeneratedSpecification &&
				specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr &&
				relationCollectionNr_ == relationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				relationWordItem_ == relationWordItem &&

					( ( assumptionLevel_ == assumptionLevel &&

						( specificationWordTypeNr == Constants.NO_WORD_TYPE_NR ||
						specificationWordTypeNr_ == specificationWordTypeNr ) &&

						// Typical for Spanish
						// Spanish test file: "Reto científico"
						!isHiddenSpanishSpecification() ) ||

					// Test files: "conflict\family\Question in conflict with itself (with knowledge)",
					//				"reasoning\Scientific challenge",
					//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)",
					//				"reasoning\family\John - Anna (before family definition)",
					//				"reasoning\family\This information is more specific (non-exclusive)"
					questionParameter_ > Constants.NO_QUESTION_PARAMETER ) ? specificationWordItem_ : null );
		}

	protected WordItem relatedSpecificationWordItem( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, WordItem relationWordItem )
		{
		return ( isPossessive_ == isPossessive &&

					( isIgnoringExclusive ||
					isExclusiveSpecification_ == isExclusiveSpecification ) &&

					( isIgnoringNegative ||
					isNegative_ == isNegative ) &&

				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				isMatchingWordType( specificationWordTypeNr_, specificationWordTypeNr ) &&

					( !isCheckingRelationContext ||

					( relationCollectionNr_ == relationCollectionNr &&
					relationWordItem_ == relationWordItem ) ) ? specificationWordItem_ : null );
		}

	protected WordItem singleRelationWordItem()
		{
		return ( relationCollectionNr_ == Constants.NO_COLLECTION_NR ? relationWordItem_ : null );
		}

	protected WordItem specificationWordItem()
		{
		return specificationWordItem_;
		}
	}

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his covenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
