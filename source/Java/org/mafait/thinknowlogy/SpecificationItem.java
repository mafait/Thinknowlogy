/*	Class:			SpecificationItem
 *	Purpose:		Storing info about the specification structure of a word
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

	private int generalizationContextNr_ = Constants.NO_CONTEXT_NR;
	private int specificationContextNr_ = Constants.NO_CONTEXT_NR;
	private int relationContextNr_ = Constants.NO_CONTEXT_NR;

	private int nContextRelations_ = 0;

	private JustificationItem firstJustificationItem_ = null;

	private WordItem specificationWordItem_ = null;

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

	private boolean hasPrimaryAssignmentJustification()
		{
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isPrimaryAssignmentSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
		}

	private byte addJustificationsToContextSpecifications()
		{
		SpecificationItem definitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem oppositeSpecificationItem;
		SpecificationItem reversibleSpecificationItem;
		WordItem currentContextWordItem;
		WordItem partOfSpecificationWordItem;
		WordItem oppositeSpecificationWordItem;
		WordItem _myWordItem = myWordItem();

		if( specificationWordItem_ == null )
			return startError( 1, null, "I have no specification word" );

		if( ( currentContextWordItem = _myWordItem.firstContextWordItem( relationContextNr_ ) ) != null )
			{
			partOfSpecificationWordItem = specificationWordItem_.commonWordItem( specificationCollectionNr_ );

			// Do for all context words with my relation context number
			do	{
				if( ( reversibleSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive_, specificationWordItem_, _myWordItem ) ) != null &&
				!reversibleSpecificationItem.isHiddenSpanishSpecification() )
					{
					if( !hasOnlyOneRelationWord() &&
					reversibleSpecificationItem.isSelfGeneratedSpecification() )
						{
						// Justification doesn't exist yet in reversible specification
						if( !reversibleSpecificationItem.hasPrimarySpecificationJustification( this ) &&
						// Justification has at least the same assumption level
						reversibleSpecificationItem.assumptionLevel() >= this.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, this, null, null, null ) &&
						// Add non-possessive reversible justification to myself
						reversibleSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, this, null, null ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to add a non-possessive reversible justification to myself" );

						reversibleSpecificationItem = reversibleSpecificationItem.updatedSpecificationItem();

						if( partOfSpecificationWordItem != null )
							{
							if( isPossessive_ )
								{
								if( ( partOfSpecificationItem = currentContextWordItem.partOfSpecificationItem( partOfSpecificationWordItem ) ) != null &&
								// Justification doesn't exist yet in part-of specification
								!partOfSpecificationItem.hasPrimarySpecificationJustification( reversibleSpecificationItem ) &&
								( definitionSpecificationItem = partOfSpecificationWordItem.partOfSpecificationItem( specificationWordItem_ ) ) != null &&
								// Justification has at least the same assumption level
								partOfSpecificationItem.assumptionLevel() >= this.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, definitionSpecificationItem, null ) &&
								// Add non-possessive part-of justification to part-of specification
								partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, definitionSpecificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, "I failed to add a non-possessive part-of justification to a part-of specification" );
								}
							else
								{
								if( ( partOfSpecificationItem = _myWordItem.partOfSpecificationItem( partOfSpecificationWordItem ) ) != null &&
								// Justification doesn't exist yet in part-of specification
								!partOfSpecificationItem.hasPrimarySpecificationJustification( this ) &&
								( definitionSpecificationItem = partOfSpecificationWordItem.partOfSpecificationItem( specificationWordItem_ ) ) != null &&
								// Justification has at least the same assumption level
								partOfSpecificationItem.assumptionLevel() >= this.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem, null ) &&
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
					assumptionLevel_ >= this.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, null, null ) &&
					// Add possessive reversible justification to myself
					addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, null, null ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add a possessive reversible justification to myself" );

					if( currentContextWordItem.hasContextInWord( relationContextNr_ ) &&
					// Recalculate assumption levels of specifications in context word
					currentContextWordItem.recalculateAssumptionLevelsInWord() != Constants.RESULT_OK )
						return addError( 1, null, "I failed to recalculate the assumption levels of specifications in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
					}

				if( isPossessive_ &&
				partOfSpecificationWordItem != null )
					{
					if( ( partOfSpecificationItem = currentContextWordItem.partOfSpecificationItem( partOfSpecificationWordItem ) ) != null &&
					// Part-of specification has no similar justification
					partOfSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, _myWordItem ) == null &&
					// Justification doesn't exist yet in part-of specification
					!partOfSpecificationItem.hasPrimarySpecificationJustification( this ) &&
					( definitionSpecificationItem = partOfSpecificationWordItem.partOfSpecificationItem( specificationWordItem_ ) ) != null &&
					// Justification has at least the same assumption level
					partOfSpecificationItem.assumptionLevel() >= this.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem, null ) &&
					// Add possessive part-of justification to part-of specification
					partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, null, definitionSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add a possessive part-of justification to a part-of specification" );

					if( !specificationWordItem_.isNounWordSpanishAmbiguous() &&
					( oppositeSpecificationWordItem = specificationWordItem_.collectionWordItem( specificationCollectionNr_ ) ) != null &&
					( oppositeSpecificationItem = _myWordItem.firstSpecificationItem( false, false, false, oppositeSpecificationWordItem ) ) != null &&
					( definitionSpecificationItem = partOfSpecificationWordItem.firstSpecificationItem( false, true, false, specificationWordItem_ ) ) != null &&
					// Justification doesn't exist yet in part-of specification
					!oppositeSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, this ) &&
					// Justification has at least the same assumption level
					oppositeSpecificationItem.assumptionLevel() >= this.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, this, null ) &&
					// Add opposite possessive assumption justification to opposite specification
					oppositeSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, this ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to add an opposite possessive assumption justification to the opposite specification" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( relationContextNr_ ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processChangedAssumption( short previousAssumptionLevel )
		{
		boolean _hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem _myWordItem = myWordItem();

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification" );

		if( specificationWordItem_ == null )
			return startError( 1, null, "I have no specification word" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isReplacedOrDeletedItem() )
			return startError( 1, null, "I am a replaced or deleted specification" );

		// Quick condition check
		if( ( !_hasRelationContext ||
		assumptionLevel_ <= previousAssumptionLevel ||
		assumptionLevel_ >= Constants.NUMBER_OF_ASSUMPTION_LEVELS ) &&

		// Within defined number of assumption levels
		( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
		assumptionLevel_ < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) &&

		( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ||
		!hasNewInformation() ) &&

		( isPossessive_ ||
		assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ||

		( !_hasRelationContext &&
		!isSpecificationWordSpanishAmbiguous() ) ||

		!_myWordItem.isUserGeneralizationWord ||
		// Relation won't be expanded
		// Test file: "Complex (7 - Claudia)"
		GlobalVariables.nUserRelationWords == 1 ||

		( ( !hasPrimaryAssignmentJustification() ||
		firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == null ) &&

		!hasOnlyOneRelationWord() ) ) &&

		( isAssignment() ||
		// Skip writing update of specification part of assignment
		_myWordItem.firstAssignmentItem( isPossessive_, false, Constants.NO_CONTEXT_NR, specificationWordItem_ ) == null ) &&

		// Write adjusted specification
		_myWordItem.writeUpdatedSpecification( true, false, false, false, false, false, false, this ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to write an adjusted specification" );

		if( _hasRelationContext )
			{
			// Add justifications to context specifications
			if( addJustificationsToContextSpecifications() != Constants.RESULT_OK )
				return addError( 1, null, "I failed to add justifications to context specifications" );
			}
		else
			{
			// Definition specification
			if( generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR &&
			// Get first generalization specification proper noun word
			( currentGeneralizationItem = specificationWordItem_.firstProperNounSpecificationGeneralizationItem() ) != null )
				{
				// Do for all generalization specification proper noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return addError( 1, null, "I found an undefined generalization word" );

					// Recalculate assumption levels of specifications in proper noun word
					if( currentGeneralizationWordItem.recalculateAssumptionLevelsInWord() != Constants.RESULT_OK )
						return addError( 1, null, "I failed to recalculate the assumption levels of specifications in proper noun word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextProperNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}


	// Private specification methods

	private boolean hasContextMaleWordInContextWords()
		{
		WordItem currentContextWordItem;
		WordItem _myWordItem = myWordItem();

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		specificationWordItem_ != null &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != _myWordItem &&
				currentContextWordItem.isMasculineWord() &&
				currentContextWordItem.hasContextInWord( relationContextNr_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
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

	private WordItem relationContextWordItemInContextWords( WordItem previousContextWordItem )
		{
		WordItem currentContextWordItem;

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		// Do for all (remaining) context words
		( currentContextWordItem = ( previousContextWordItem == null ? GlobalVariables.firstContextWordItem : previousContextWordItem.nextContextWordItem ) ) != null )
			{
			do	{
				if( currentContextWordItem.hasContextInWord( relationContextNr_ ) )
					return currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return null;
		}

	private WordItem relationContextWordNotCoveredByJustifications()
		{
		boolean hasFoundContextInJustification = false;
		JustificationItem searchJustificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentContextWordItem;

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		firstJustificationItem_ != null &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem.hasContextInWord( relationContextNr_ ) )
					{
					hasFoundContextInJustification = false;
					searchJustificationItem = firstJustificationItem_;

					while( !hasFoundContextInJustification &&
					searchJustificationItem != null )
						{
						// Check primary specification
						if( ( ( primarySpecificationItem = searchJustificationItem.primarySpecificationItem() ) != null &&
						primarySpecificationItem.hasRelationContext() &&

						( primarySpecificationItem.generalizationWordItem() == currentContextWordItem ||
						currentContextWordItem.hasContextInWord( primarySpecificationItem.relationContextNr() ) ) ) ||

						// Check secondary specification
						( ( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null &&
						secondarySpecificationItem.hasRelationContext() &&

						( secondarySpecificationItem.generalizationWordItem() == currentContextWordItem ||
						currentContextWordItem.hasContextInWord( secondarySpecificationItem.relationContextNr() ) ) ) )
							hasFoundContextInJustification = true;

						searchJustificationItem = searchJustificationItem.attachedJustificationItem();
						}

					if( !hasFoundContextInJustification )
						// This relation context word is not covered by justifications
						return currentContextWordItem;
					}
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		// Justifications are covering given relation context
		return null;
		}

	// Protected constructed variables

	protected boolean hasAdjustedSpecificationBeenWritten = false;
	protected boolean hasSpecificationBeenWrittenAsAnswer = false;

	protected SpecificationItem replacingSpecificationItem = null;


	// Constructor

	protected SpecificationItem( boolean isAnsweredQuestion, boolean isCharacteristicFor, boolean isConditional, boolean isCorrectedSpecification, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isLanguageWord, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assignmentLevel, short assumptionLevel, short languageNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationCollectionNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, String specificationString, StringBuffer storedSentenceStringBuffer, StringBuffer storedSentenceWithOnlyOneSpecificationStringBuffer, List myList, WordItem myWordItem )
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

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nContextRelations_ = nContextRelations;

		firstJustificationItem_ = firstJustificationItem;

		specificationWordItem_ = specificationWordItem;

		specificationString_ = specificationString;


		// Private constructed variables

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
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nContextRelations_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( prepositionParameter_ > Constants.NO_PREPOSITION_PARAMETER ||
				questionParameter_ > Constants.NO_QUESTION_PARAMETER ||
				generalizationCollectionNr_ > Constants.NO_COLLECTION_NR ||
				specificationCollectionNr_ > Constants.NO_COLLECTION_NR ||
				generalizationContextNr_ > Constants.NO_CONTEXT_NR ||
				specificationContextNr_ > Constants.NO_CONTEXT_NR ||
				relationContextNr_ > Constants.NO_CONTEXT_NR ||
				nContextRelations_ > 0 ) ) );
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
		WordItem _myWordItem = myWordItem();
		String languageNameString = _myWordItem.languageNameString( languageNr_ );
		String generalizationWordTypeString = _myWordItem.wordTypeNameString( generalizationWordTypeNr_ );
		StringBuffer queryStringBuffer;
		String relationWordTypeString = ( relationWordTypeNr_ == Constants.NO_WORD_TYPE_NR ? null : _myWordItem.wordTypeNameString( relationWordTypeNr_ ) );
		String specificationWordTypeString = _myWordItem.wordTypeNameString( specificationWordTypeNr_ );
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

		if( relationWordTypeString != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationWordType:" + relationWordTypeString + Constants.QUERY_WORD_TYPE_STRING + relationWordTypeNr_ );

		if( relationContextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "relationContextNr:" + relationContextNr_ );

		if( nContextRelations_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nContextRelations:" + nContextRelations_ );

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

	protected int nRelationContextWords()
		{
		return myWordItem().nContextWords( relationContextNr_ );
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

	protected boolean hasAdditionalDefinitionSpecificationJustification()
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
			if( searchJustificationItem.additionalDefinitionSpecificationItem() != null )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return false;
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
			if( searchJustificationItem.hasCompoundCollectionSecondarySpecification() )
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

	protected int highestReversibleJustificationAssumptionLevel()
		{
		int currentAssumptionLevel;
		int highestAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		JustificationItem searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isReversibleAssumptionOrConclusion() &&
			( currentAssumptionLevel = searchJustificationItem.justificationAssumptionLevel() ) > highestAssumptionLevel )
				highestAssumptionLevel = currentAssumptionLevel;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem();
			}

		return highestAssumptionLevel;
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

	protected byte checkForJustificationsCoveringRelationContext()
		{
		WordItem currentContextWordItem;

		if( relationContextNr_ == Constants.NO_CONTEXT_NR )
			return startError( 1, null, "I have no relation context" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification. So, I have no justifications" );

		if( GlobalVariables.firstContextWordItem == null )
			return startError( 1, null, "I could not find any context word, while checking myself for justifications covering a relation context" );

		if( ( currentContextWordItem = relationContextWordNotCoveredByJustifications() ) != null &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a specification missing justifications covering context word: \"" + currentContextWordItem.anyWordTypeString() + "\"; relationContextNr_: " + relationContextNr_ + ";\n\tSpecificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
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
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == null );
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

		hasAdjustedSpecificationBeenWritten = false;
		hasSpecificationBeenWrittenAsAnswer = false;
		}

	protected void storeWrittenSentenceStringBuffer()
		{
		storedSentenceStringBuffer_ = GlobalVariables.writtenSentenceStringBuffer;
		}

	protected void storedWrittenSentenceStringBufferWithOneSpecificationOnly()
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

	protected boolean hasNewInformation()
		{
		return ( !isOlderItem() ||
				// Has been inactivated during this sentence
				hasCurrentInactiveSentenceNr() ||
				// Has been archived during this sentence
				hasCurrentArchivedSentenceNr() ||

				( relationContextNr_ > Constants.NO_CONTEXT_NR &&
				hasRelationContextCurrentlyBeenUpdated() ) );
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
		return ( myWordItem().nContextWords( relationContextNr_ ) == 1 );
		}

	protected boolean hasSpecificationContext()
		{
		return ( specificationContextNr_ > Constants.NO_CONTEXT_NR );
		}

	protected boolean hasRelationContext()
		{
		return ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		}

	protected boolean hasRelationContextCurrentlyBeenUpdated()
		{
		WordItem currentContextWordItem;
		WordItem _myWordItem = myWordItem();

		if( relationContextNr_ > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != _myWordItem &&
				currentContextWordItem.hasContextCurrentlyBeenUpdatedInWord( relationContextNr_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return false;
		}

	protected boolean hasSpecificationCollection()
		{
		return ( specificationCollectionNr_ > Constants.NO_COLLECTION_NR );
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
					if( ( currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ : searchSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGenerated, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, relationContextNr_ ) ) ) != null &&
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

	protected boolean isGeneralizationNoun()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isGeneralizationProperNoun()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NOUN );
		}

	protected boolean isHiddenSpanishSpecification()
		{
		return ( isSpecificationWordSpanishAmbiguous() &&
				hasNonCompoundSpecificationCollection() &&
				!isNegative_ &&
				isGeneralizationProperNoun() &&
				isSelfGeneratedSpecification() &&

				// Non-possessive specification not in a male word
				( ( !isPossessive_ &&
		!myWordItem().hasMasculineProperNounEnding() &&
				!myWordItem().isMasculineWord() ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				relationContextNr_ > Constants.NO_CONTEXT_NR &&
				specificationWordItem_ != null &&
				specificationWordItem_.compoundCollectionNr() > Constants.NO_COLLECTION_NR &&
				!hasContextMaleWordInContextWords() ) ) );
		}

	protected boolean isMatchingGeneralizationContextNr( boolean isAllowingEmptyRelationContext, int generalizationContextNr )
		{
		return ( isAllowingEmptyRelationContext ||
				// Empty subset
				generalizationContextNr == Constants.NO_CONTEXT_NR ||
				// Same set
				generalizationContextNr_ == generalizationContextNr ? true : myWordItem().isContextSubsetInContextWords( generalizationContextNr_, generalizationContextNr ) );
		}

	protected boolean isMatchingRelationContextNr( boolean isAllowingEmptyRelationContext, int relationContextNr )
		{		// Empty subset
		return ( relationContextNr == Constants.NO_CONTEXT_NR ||
				// Same set
				relationContextNr_ == relationContextNr ||

				( isAllowingEmptyRelationContext &&
				relationContextNr_ == Constants.NO_CONTEXT_NR ) ? true : myWordItem().isContextSubsetInContextWords( relationContextNr_, relationContextNr ) );
		}

	protected boolean isNegative()
		{
		return isNegative_;
		}

	protected boolean isPartOf()
		{
		return isPartOf_;
		}

	protected boolean isPossessive()
		{
		return isPossessive_;
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
			if( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL )
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

	protected boolean isSpecific()
		{
		return isSpecific_;
		}

	protected boolean isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	protected boolean isSpecificationAdjective()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_ADJECTIVE );
		}

	protected boolean isSpecificationNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isSpecificationPluralNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isSpecificationSingularNoun()
		{
		return ( specificationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > Constants.NO_WRITE_LEVEL );
		}

	protected boolean isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				myWordItem().nContextWords( relationContextNr_ ) > 1 );
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
		return ( relationContextNr_ > Constants.NO_CONTEXT_NR &&
				// Self-generated specification
				firstJustificationItem_ != null &&
				isSpecificationWordSpanishAmbiguous() &&
				isOlderItem() &&
				!isHiddenSpanishSpecification() &&
				storedSentenceStringBuffer_ == null &&

				( isSelfGeneratedAssumption() ||
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

	protected int nContextRelations()
		{
		return nContextRelations_;
		}

	protected int nInvolvedSpecificationWords()
		{
		int myCreationSentenceNr = creationSentenceNr();
		int nInvolvedSpecificationWords = 0;
		SpecificationItem searchSpecificationItem = myWordItem().firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != null )
			{
			if( searchSpecificationItem == this ||
			searchSpecificationItem.creationSentenceNr() == myCreationSentenceNr )
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
		WordItem _myWordItem = myWordItem();

		if( userSpecificationItem == null )
			return startError( 1, null, "The given user specification item is undefined" );

		if( specificationWordItem_ == null )
			return startError( 1, null, "I have no specification word item" );

		do	{
			if( ( secondaryGeneralizationWordItem = currentJustificationItem.secondaryGeneralizationWordItem() ) != null &&
			( selfGeneratedSpecificationItem = secondaryGeneralizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, !isPossessive_, true, specificationWordItem_, _myWordItem ) ) != null &&
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
		boolean _hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean _isQuestion = isQuestion();
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

		if( ( attachedPrimarySpecificationItem = attachJustificationItem.primarySpecificationItem() ) != null &&
		!attachedPrimarySpecificationItem.isPossessive() &&
		attachedPrimarySpecificationItem.hasRelationContext() )
			obsoleteJustificationItem = firstJustificationItem_.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem.specificationWordItem() );
		else
			{
			// Typical for Spanish
			if( ( _isQuestion ||
			isSpecificationWordSpanishAmbiguous() ) &&

			( attachedSecondarySpecificationItem = attachJustificationItem.secondarySpecificationItem() ) != null &&
			attachedSecondarySpecificationItem.hasCompoundSpecificationCollection() &&
			!attachedSecondarySpecificationItem.isPossessive() &&
			attachedSecondarySpecificationItem.isUserSpecification() )
				obsoleteJustificationItem = firstJustificationItem_.obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
			}

		if( _hasRelationContext &&
		obsoleteJustificationItem != null &&
		// Replace obsolete justification item
		_myWordItem.replaceJustification( obsoleteJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to replace the obsolete justification item" );

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

				if( !_hasRelationContext &&
				!isNegative_ &&
				!_isQuestion &&
				assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL &&
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
					if( ( createAndAssignResult = _myWordItem.createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isCharacteristicFor_, isConditional(), isCorrectedSpecification_, isEveryGeneralization_, isExclusiveGeneralization_, isExclusiveSpecification_, isNegative_, isPartOf_, isPossessive_, isSpecific_, isSpecificationGeneralization_, isUncountableGeneralizationNoun_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr(), prepositionParameter(), questionParameter(), generalizationWordTypeNr(), specificationWordTypeNr(), relationWordTypeNr(), specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, storedSentenceStringBuffer_, storedSentenceWithOnlyOneSpecificationStringBuffer_ ) ).result != Constants.RESULT_OK )
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

		return Constants.RESULT_OK;
		}

	protected byte calculateAssumptionLevel()
		{
		boolean hasFoundReversibleAssumptionOrConclusion;
		boolean _hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean isCheckingRelationWords;
		boolean isBlockingHighestAssumptionLevel;
		boolean isForcingHighestAssumptionLevel;
		boolean isOlderSpecification = isOlderItem();
		boolean isReversibleAssumptionOrConclusion;
		boolean _isSpecificationWordSpanishAmbiguous = false;
		short highestAssumptionLevel;
		short lowestAssumptionLevel = Constants.MAX_LEVEL;
		int nJustificationRelationWords;
		int nRelationWords;
		int justificationAssumptionLevel;
		JustificationItem currentJustificationItem = firstJustificationItem_;
		JustificationItem searchJustificationItem;

		if( isSpecificationGeneralization_ )
			return startError( 1, null, "I am a specification-generalization" );

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			return startError( 1, null, "I am a question" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification" );

		if( assumptionLevel_ > Constants.NO_ASSUMPTION_LEVEL ||

		( ( _hasRelationContext ||
		!isOlderSpecification ||
		!hasCurrentCreationSentenceNr() ) &&

		hasAtLeastOneAssumptionJustification() ) )
			{
			hasFoundReversibleAssumptionOrConclusion = ( firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != null );
			_isSpecificationWordSpanishAmbiguous = isSpecificationWordSpanishAmbiguous();
			nRelationWords = nRelationContextWords();

			do	{
				isCheckingRelationWords = true;
				isBlockingHighestAssumptionLevel = false;
				isForcingHighestAssumptionLevel = false;
				highestAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
				nJustificationRelationWords = 0;
				searchJustificationItem = currentJustificationItem;

				do	{
					if( _hasRelationContext )
						nJustificationRelationWords += searchJustificationItem.nJustificationContextRelations( relationContextNr_, nRelationWords );

					// Calculate assumption level
					if( ( justificationAssumptionLevel = searchJustificationItem.justificationAssumptionLevel() ) > Constants.MAX_LEVEL )
						return startSystemError( 1, null, "Assumption level overflow" );

					if( isCheckingRelationWords )
						{
						if( justificationAssumptionLevel > highestAssumptionLevel )
							{
							if( hasFoundReversibleAssumptionOrConclusion &&
							justificationAssumptionLevel > assumptionLevel_ )
								{
								isReversibleAssumptionOrConclusion = searchJustificationItem.isReversibleAssumptionOrConclusion();

								// Opposite possessive specification assumption, or Exclusive specification substitution assumption
								if( ( !isReversibleAssumptionOrConclusion &&
								isOlderSpecification &&

								( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ||
								highestAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL ||
								searchJustificationItem.isExclusiveSpecificationSubstitutionAssumption() ) ) ||

								// Reversible assumption
								( isReversibleAssumptionOrConclusion &&
								highestAssumptionLevel == Constants.NO_ASSUMPTION_LEVEL &&
								// Typical for Spanish
								!_isSpecificationWordSpanishAmbiguous &&
								hasCompoundSpecificationCollection() ) )
									isForcingHighestAssumptionLevel = true;
								else
									{
									if( searchJustificationItem.isExclusiveSpecificationSubstitutionAssumption() )
										isBlockingHighestAssumptionLevel = true;
									}
								}
							else
								{
								// Typical for Spanish
								// Spanish test file: "Complejo (19 - extraño)"
								if( isOlderSpecification &&
								justificationAssumptionLevel < assumptionLevel_ &&
								searchJustificationItem.hasCurrentCreationSentenceNr() &&
								searchJustificationItem.isExclusiveSpecificationSubstitutionAssumption() &&
								searchJustificationItem.isPossessivePrimarySpecification() &&
								searchJustificationItem.isOlderItem() )
									isBlockingHighestAssumptionLevel = true;
								}

							highestAssumptionLevel = (short)justificationAssumptionLevel;
							}
						}
					else
						{
						if( justificationAssumptionLevel < highestAssumptionLevel )
							highestAssumptionLevel = (short)justificationAssumptionLevel;
						}

					if( !_hasRelationContext )
						isCheckingRelationWords = false;
					}
				while( ( searchJustificationItem = searchJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );

				if( highestAssumptionLevel < lowestAssumptionLevel &&

				( isForcingHighestAssumptionLevel ||

				( nJustificationRelationWords > nRelationWords &&

				// Test files: "Complex (18)", "Complex (19 - mixed)" and "Complex (19 - strange)"
				( !hasFoundReversibleAssumptionOrConclusion ||
				highestAssumptionLevel <= assumptionLevel_ ) ) ||

				( !isBlockingHighestAssumptionLevel &&
				nJustificationRelationWords == nRelationWords &&

				( isNegative_ ||
				// No assumption level
				assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL ||

				// First set of justifications
				( ( lowestAssumptionLevel == Constants.MAX_LEVEL &&
				highestAssumptionLevel < assumptionLevel_ ) ||

				// Assumption level
				( !_isSpecificationWordSpanishAmbiguous &&
				assumptionLevel_ + 2 > highestAssumptionLevel ) ) ) ) ) )
					lowestAssumptionLevel = highestAssumptionLevel;
				}
			while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != null );

			if( lowestAssumptionLevel < Constants.MAX_LEVEL &&
			// Assumption level has changed
			lowestAssumptionLevel != assumptionLevel_ )
				{
				assumptionLevel_ = lowestAssumptionLevel;

				// Clear stored sentence buffer to lose the previous assumption level word
				clearStoredSentenceStringBuffer();
				}
			}

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
			if( myWordItem().copyAndReplaceSpecification( true, isExclusiveGeneralization_, generalizationCollectionNr_, specificationCollectionNr_, firstJustificationItem_, this ).result != Constants.RESULT_OK )
				return addError( 1, null, "I failed to copy and replace myself as an answered question" );
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
		boolean _hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean isOlderSpecification = isOlderItem();
		short previousAssumptionLevel = assumptionLevel_;

		// Calculate assumption level
		if( calculateAssumptionLevel() != Constants.RESULT_OK )
			return addError( 1, null, "I failed to calculate my assumption level" );

		if( hasCurrentCreationSentenceNr() )
			{
			hasChangedAssumptionLevel = ( previousAssumptionLevel != assumptionLevel_ );

			if( ( ( isOlderSpecification &&
			!isNegative_ ) ||

			( isSituationStable &&

			// No relation context
			( ( !_hasRelationContext &&

			( isNegative_ ||

			// Test file: "John - Anna (before family definition)"
			( !hasChangedAssumptionLevel &&
			isPartOf_ ) ) ) ||

			// Relation context
			( _hasRelationContext &&
			hasNonCompoundSpecificationCollection() &&

			( isPossessive_ ||
			myWordItem().isUserRelationWord ||
			firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) == null ) ) ) ) ) &&

			// Remove obsolete assumption justifications
			removeObsoleteAssumptionJustifications( isOlderSpecification, isSituationStable ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to remove obsolete assumption justifications from this assumption" );

			if( hasChangedAssumptionLevel &&
			isOlderSpecification &&
			// Process changed assumption level
			processChangedAssumption( previousAssumptionLevel ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to process the changed assumption level" );
			}

		return Constants.RESULT_OK;
		}

	protected byte removeObsoleteAssumptionJustifications( boolean isOnlySelectingOlderJustifications, boolean isSituationStable )
		{
		boolean hasConfirmedAnySpecification = GlobalVariables.hasConfirmedAnySpecification;
		boolean _hasOnlyOneRelationWord;
		boolean _hasRelationContext = ( relationContextNr_ > Constants.NO_CONTEXT_NR );
		boolean isIncludingNegativeAssumptionOrConclusion;
		boolean isIncludingReversibleAssumptionOrConclusion;
		boolean isIncludingSpecificationSubstitutionAssumptionOrConclusion;
		JustificationItem attachedJustificationItem;
		JustificationItem createdJustificationItem;
		JustificationItem currentJustificationItem = firstJustificationItem_;
		JustificationItem previousJustificationItem = null;
		JustificationItem previousPreviousJustificationItem = null;
		SpecificationItem userAssignmentItem;
		WordItem _myWordItem = myWordItem();
		JustificationResultType justificationResult;

		if( questionParameter_ > Constants.NO_QUESTION_PARAMETER )
			return startError( 1, null, "I am a question" );

		if( firstJustificationItem_ == null )
			return startError( 1, null, "I am a user specification" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		isIncludingNegativeAssumptionOrConclusion = ( ( assumptionLevel_ == Constants.NO_ASSUMPTION_LEVEL &&

													( !hasConfirmedAnySpecification ||
													!isOnlySelectingOlderJustifications ||
													// Test file: "My assumptions that are confirmed (John)"
													!firstJustificationItem_.hasPrimarySpecificationCurrentCreationSentenceNr() ) ) ||

													( !hasConfirmedAnySpecification &&
													// Typical for Spanish
													// Spanish test file: "Complejo (13)"
													firstJustificationItem_.justificationAssumptionLevel() == assumptionLevel_ ) );

		isIncludingReversibleAssumptionOrConclusion = ( isSituationStable ||
														!hasNewInformation() ||
														// Typical for Spanish
														isHiddenSpanishSpecification() );

		isIncludingSpecificationSubstitutionAssumptionOrConclusion = ( !hasConfirmedAnySpecification ||

																		( isSituationStable &&
																		specificationCollectionNr_ == Constants.NO_COLLECTION_NR ) );

		_hasOnlyOneRelationWord = ( _hasRelationContext &&
									isSituationStable &&
									hasOnlyOneRelationWord() );

		do	{
			if( currentJustificationItem.isObsoleteAssumptionJustification(_hasOnlyOneRelationWord, isIncludingNegativeAssumptionOrConclusion, isIncludingReversibleAssumptionOrConclusion, isIncludingSpecificationSubstitutionAssumptionOrConclusion, isOnlySelectingOlderJustifications, assumptionLevel_) )
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

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
							return startError( 1, null, "The created justification item is undefined" );

						if( previousPreviousJustificationItem == null )
							{
							// Change first justification item to created justification
							if( changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change my first justification item to the created justification" );
							}
						else
							{
							// Change attached justification item of previous previous justification to created justification
							if( previousPreviousJustificationItem.changeAttachedJustification( createdJustificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the attached justification item of the previous previous justification item to the created justification" );
							}

						// Replace previous justification
						if( _myWordItem.replaceOrDeleteJustification( previousJustificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to replace the previous justification item" );

						// Replace current justification
						if( _myWordItem.replaceOrDeleteJustification( currentJustificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to replace the current justification item" );

						// Reset variables
						previousPreviousJustificationItem = null;
						previousJustificationItem = null;
						currentJustificationItem = firstJustificationItem_;
						}
					else
						{
						// Used for developer statistics
						GlobalVariables.nSkippedRemovingObsoleteJustifications++;

						previousPreviousJustificationItem = previousJustificationItem;
						previousJustificationItem = currentJustificationItem;
						currentJustificationItem = currentJustificationItem.attachedJustificationItem();
						}
					}
				}
			else
				{
				if( _hasRelationContext &&
				!hasAdjustedSpecificationBeenWritten &&
				!isIncludingNegativeAssumptionOrConclusion &&
				!isIncludingReversibleAssumptionOrConclusion &&
				currentJustificationItem.isSpecificationSubstitutionAssumptionOrConclusion() &&
				!currentJustificationItem.hasPrimarySpecificationRelationContext() &&
				( userAssignmentItem = _myWordItem.firstAssignmentItem( false, false, Constants.NO_CONTEXT_NR, currentJustificationItem.primarySpecificationWordItem() ) ) != null )
					{
					// Copy current justification
					if( ( justificationResult = _myWordItem.copyJustificationItem( userAssignmentItem, currentJustificationItem.secondarySpecificationItem(), currentJustificationItem.attachedJustificationItem(), currentJustificationItem ) ).result != Constants.RESULT_OK )
						return addError( 1, null, "I failed to copy the previous justification item" );

					if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
						return startError( 1, null, "The created justification item is undefined" );

					// Replace specification without relation context, by assignment specification
					if( _myWordItem.replaceJustification( currentJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace a specification without relation context, by an assignment specification" );

					// Start from scratch
					previousPreviousJustificationItem = null;
					previousJustificationItem = null;
					currentJustificationItem = firstJustificationItem_;
					}
				else
					{
					previousPreviousJustificationItem = previousJustificationItem;
					previousJustificationItem = currentJustificationItem;
					currentJustificationItem = currentJustificationItem.attachedJustificationItem();
					}
				}
			}
		while( currentJustificationItem != null );

		// Typical for Spanish
		// Spanish test file: "Complejo (12)"
		if( _hasRelationContext &&
		isOnlySelectingOlderJustifications &&
		isPossessive_ &&
		!isSituationStable &&
		isHiddenSpanishSpecification() &&
		// (Re)calculate assumption level
		calculateAssumptionLevel() != Constants.RESULT_OK )
			return addError( 1, null, "I failed to (re)calculate my assumption level" );

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
			!searchSpecificationItem.isSpecificationAdjective() ) )
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

	protected SpecificationItem nonNegativeNonPossessiveDefinitionSpecificationItem( boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( !searchSpecificationItem.isNegative() &&
			!searchSpecificationItem.isPossessive() &&
			!searchSpecificationItem.isSpecificationGeneralization() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem.nextSpecificationItem();
			}

		return null;
		}

	protected SpecificationItem nonQuestionSpecificationItem( boolean isAllowingEmptyRelationContext, boolean isIncludingThisItem )
		{
		SpecificationItem searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != null )
			{
			if( ( isAllowingEmptyRelationContext ||
			searchSpecificationItem.hasRelationContext() ) &&

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

	protected SpecificationItem nextNonNegativeNonPossessiveDefinitionSpecificationItem()
		{
		return nonNegativeNonPossessiveDefinitionSpecificationItem( false );
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

	protected WordItem firstUnwrittenRelationWordItem( short wordTypeNr, WordItem previousWordItem )
		{
		// Start with previous word
		WordItem currentRelationWordItem = previousWordItem;

		// Check all relation words
		while( ( currentRelationWordItem = relationContextWordItemInContextWords( currentRelationWordItem ) ) != null )
			{
			if( !currentRelationWordItem.isRelationWordTypeAlreadyWritten( wordTypeNr ) )
				return currentRelationWordItem;
			}

		return null;
		}

	protected WordItem generalizationWordItem()
		{
		return myWordItem();
		}

	protected WordItem relatedSpecificationWordItem( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean _isSelfGeneratedSpecification, short assumptionLevel, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGeneratedSpecification() == _isSelfGeneratedSpecification &&
				specificationCollectionNr > Constants.NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				relationContextNr_ == relationContextNr &&

				( assumptionLevel_ == assumptionLevel ||
				questionParameter_ > Constants.NO_QUESTION_PARAMETER ) &&

				( specificationWordTypeNr == Constants.NO_WORD_TYPE_NR ||
				specificationWordTypeNr_ == specificationWordTypeNr ) ? specificationWordItem_ : null );
		}

	protected WordItem relatedSpecificationWordItem( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int relationContextNr )
		{
		return ( isPossessive_ == isPossessive &&

				( isIgnoringExclusive ||
				isExclusiveSpecification_ == isExclusiveSpecification ) &&

				( isIgnoringNegative ||
				isNegative_ == isNegative ) &&

				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				isMatchingWordType( specificationWordTypeNr_, specificationWordTypeNr ) &&

				( relationContextNr_ == relationContextNr ||

				( !isCheckingRelationContext &&
				!isHiddenSpanishSpecification() ) ) ? specificationWordItem_ : null );
		}

	protected WordItem singleRelationWordItem()
		{
		WordItem firstRelationWordItem;

				// First relation word
		return ( ( firstRelationWordItem = relationContextWordItemInContextWords( null ) ) != null &&
				// No another relation word
				relationContextWordItemInContextWords( firstRelationWordItem ) == null ? firstRelationWordItem : null );
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
