/*	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		Storing justification items of self-generated knowledge
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

class JustificationList extends List
	{
	// Private methods

	private static void clearReplacingInfo( JustificationItem searchJustificationItem )
		{
		while( searchJustificationItem != null )
			{
			searchJustificationItem.clearReplacingInfo();
			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}
		}

	private short highestJustificationOrderNr( short justificationTypeNr )
		{
		short highestOrderNr = Constants.NO_ORDER_NR;
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationOrderNr > highestOrderNr &&
			searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem.justificationOrderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return highestOrderNr;
		}

	private short justificationOrderNr( boolean isExclusivePrimarySpecification, boolean isPossessivePrimarySpecification, short justificationTypeNr, short primarySpecificationWordTypeNr, int primarySpecificationCollectionNr, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem.isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&

				// Secondary specification
				// Non-exclusive primary specification
				( ( !isExclusivePrimarySpecification &&
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem ) ||

				// Primary specification
				// Exclusive definition specification
				( isExclusivePrimarySpecification &&
				// Test files: "reasoning\Basic reasoning",
				//				"reasoning\Scientific challenge",
				//				"reasoning\family\Justification report",
				//				among others
				searchJustificationItem.isExclusivePrimarySpecification() &&
				searchJustificationItem.primarySpecificationWordTypeNr() == primarySpecificationWordTypeNr &&
				searchJustificationItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr ) ) )

				return searchJustificationItem.justificationOrderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.NO_ORDER_NR;
		}

	private short justificationOrderNr( boolean hasAdditionalDefinitionSpecification, boolean hasFeminineOrMasculineProperNounEnding, boolean isPossessivePrimarySpecification, boolean isPossessiveSecondarySpecification, boolean isPrimarySpecificationWordSpanishAmbiguous, short justificationTypeNr, int primarySpecificationCollectionNr )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem.hasFeminineOrMasculineProperNounEnding() == hasFeminineOrMasculineProperNounEnding &&
			searchJustificationItem.hasAdditionalDefinitionSpecification() == hasAdditionalDefinitionSpecification &&
			searchJustificationItem.isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&
			searchJustificationItem.isPossessiveSecondarySpecification() == isPossessiveSecondarySpecification &&

				( searchJustificationItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr ||
				// Typical for Spanish
				isPrimarySpecificationWordSpanishAmbiguous ||
				searchJustificationItem.isPrimarySpecificationWordSpanishAmbiguous() ) )

				return searchJustificationItem.justificationOrderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.NO_ORDER_NR;
		}

	private short negativeJustificationOrderNr( boolean isExclusivePrimarySpecification, boolean isPossessivePrimarySpecification, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isNegativeAssumptionOrConclusion() &&
			searchJustificationItem.isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&

				// Non-exclusive primary specification
				( ( !isExclusivePrimarySpecification &&
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem ) ||

				// Exclusive definition specification
				( isExclusivePrimarySpecification &&
				// Test file: "reasoning\family\Justification report"
				searchJustificationItem.isExclusivePrimarySpecification() ) ) )

				return searchJustificationItem.justificationOrderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.NO_ORDER_NR;
		}

	private short reversibleJustificationOrderNr( boolean isPossessivePrimarySpecification )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isReversibleAssumptionOrConclusion() &&
			searchJustificationItem.isPossessivePrimarySpecification() == isPossessivePrimarySpecification )
				return searchJustificationItem.justificationOrderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.NO_ORDER_NR;
		}

	private short questionJustificationOrderNr( boolean hasAdditionalDefinitionSpecification, int primarySpecificationCollectionNr, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isQuestionJustification() &&
			searchJustificationItem.hasAdditionalDefinitionSpecification() == hasAdditionalDefinitionSpecification &&

				( searchJustificationItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr ||
				// Typical for Spanish
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem ) )
				return searchJustificationItem.justificationOrderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.NO_ORDER_NR;
		}

	private byte checkJustificationForUsage( boolean isActiveItem, JustificationItem unusedJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );

		if( unusedJustificationItem == null )
			return startError( 1, "The given unused justification item is undefined" );

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.attachedJustificationItem() == unusedJustificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an attached justification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tunusedJustificationItem: " + unusedJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte checkSpecificationForUsage( boolean isActiveItem, SpecificationItem unusedSpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );

		if( unusedSpecificationItem == null )
			return startError( 1, "The given unused specification item is undefined" );

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.primarySpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a primary specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tprimarySpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write a primary specification interface warning" );

			if( searchJustificationItem.additionalDefinitionSpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an additional definition specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tadditionalDefinitionSpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write additional definition specification interface warning" );

			if( searchJustificationItem.secondarySpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a secondary specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tsecondarySpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write secondary specification interface warning" );

			if( searchJustificationItem.additionalProperNounSpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an additional proper noun specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tadditionalProperNounSpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write additional proper noun specification interface warning" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private JustificationItem firstActiveJustificationItem()
		{
		return (JustificationItem)firstActiveItem();
		}

	private JustificationItem firstReplacedJustificationItem()
		{
		return (JustificationItem)firstReplacedItem();
		}

	private JustificationItem firstJustificationItem( boolean isActiveItem )
		{
		return ( isActiveItem ? firstActiveJustificationItem() :
								firstReplacedJustificationItem() );
		}

	private JustificationItem justificationItem( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasJustification( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	private JustificationItem nextJustificationListItem()
		{
		return (JustificationItem)nextListItem();
		}


	// Constructor

	protected JustificationList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_JUSTIFICATION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void clearJustificationHasBeenWritten()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			searchJustificationItem.hasJustificationBeenWritten = false;
			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}
		}

	protected void clearReplacingInfo()
		{
		JustificationItem searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != null )
			clearReplacingInfo( searchJustificationItem );

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != null )
			clearReplacingInfo( searchJustificationItem );
		}

	protected boolean hasJustification( SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem.additionalDefinitionSpecificationItem() == additionalDefinitionSpecificationItem &&
			searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return false;
		}

	protected boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isQuestionJustification() &&
			( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null &&
			secondarySpecificationItem.hasNonCompoundSpecificationCollection() )
				return true;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return false;
		}

	protected byte checkForReplacedOrDeletedSpecifications()
		{
		JustificationItem searchJustificationItem;

		if( !GlobalVariables.hasDisplayedIntegrityWarning &&
		( searchJustificationItem = firstActiveJustificationItem() ) != null )
			{
			do	;
			// Check for replaced and deleted specifications
			while( searchJustificationItem.checkSpecifications( true ) == Constants.RESULT_OK &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItem() ) != null &&
			!searchJustificationItem.isReplacedOrDeletedItem() );
			}

		if( !GlobalVariables.hasDisplayedIntegrityWarning &&
		( searchJustificationItem = firstReplacedJustificationItem() ) != null )
			{
			do	;
			// Only check for deleted specifications
			while( searchJustificationItem.checkSpecifications( false ) == Constants.RESULT_OK &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItem() ) != null );
			}

		return GlobalVariables.result;
		}

	protected byte checkForUnreferencedReplacedJustifications()
		{
		JustificationItem searchJustificationItem = firstReplacedJustificationItem();
		WordItem _myWordItem = myWordItem();

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( !_myWordItem.isJustificationInUse( searchJustificationItem ) &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an unreferenced replaced justification item:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkJustificationForUsage( JustificationItem unusedJustificationItem )
		{
		if( checkJustificationForUsage( true, unusedJustificationItem ) == Constants.RESULT_OK )
			return checkJustificationForUsage( false, unusedJustificationItem );

		return GlobalVariables.result;
		}

	protected byte checkSpecificationForUsage( SpecificationItem unusedSpecificationItem )
		{
		if( checkSpecificationForUsage( true, unusedSpecificationItem ) == Constants.RESULT_OK )
			return checkSpecificationForUsage( false, unusedSpecificationItem );

		return GlobalVariables.result;
		}

	protected byte cleanupUnreferencedJustifications()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		WordItem _myWordItem = myWordItem();

		while( searchJustificationItem != null )
			{
			if( _myWordItem.isJustificationInUse( searchJustificationItem ) )
				searchJustificationItem = searchJustificationItem.nextJustificationItem();
			else
				{
				if( replaceOrDeleteJustification( searchJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to replace or delete an unreferenced justification item" );

				searchJustificationItem = nextJustificationListItem();
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceJustification( boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationItem attachedJustificationItem;
		WordItem _myWordItem = myWordItem();

		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem == replacingJustificationItem )
			return startError( 1, "The given obsolete justification item and the given replacing justification item are the same justification item" );

		if( obsoleteJustificationItem.isReplacedItem() )
			return startError( 1, "The given obsolete justification item is already replaced" );

		// Old justification has an attached justification,
		// Test files: "reasoning\family\Complex (2)",
		//				"reasoning\family\Complex (4)",
		//				"reasoning\family\Complex (5)",
		//				"reasoning\family\Complex (6)",
		//				"reasoning\family\Complex (17)",
		//				"reasoning\family\Complex (21)",
		//				among others
		if( ( attachedJustificationItem = obsoleteJustificationItem.attachedJustificationItem() ) != null &&
		// that is active,
		attachedJustificationItem.isActiveItem() &&
		replacingJustificationItem != null &&
		// but the replacing one no attached justifications
		replacingJustificationItem.attachedJustificationItem() == null &&
		// Change attached justification
		replacingJustificationItem.changeAttachedJustification( attachedJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to change an attached justification item" );

		if( attachedJustificationItem == null ||
		// Replacing justification item has the same attached justification item
		replacingJustificationItem != null )
			{
			// Replace or delete obsolete justification
			if( replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to replace the obsolete justification item" );

			if( _myWordItem.updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to update a justification in specifications of my word" );
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem.hasCurrentCreationSentenceNr() )
			{
			// Used for developer statistics
			GlobalVariables.nDeletedJustificationItems++;

			if( deleteItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to delete an obsolete justification" );
			}
		else
			{
			// Used for developer statistics
			GlobalVariables.nReplacedJustificationItems++;

			if( replaceItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to replace an obsolete justification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte updateSpecificationOfJustifications( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		boolean hasCurrentlyCorrectedAssumption;
		boolean isExclusiveGeneralization = false;
		boolean isReplacingPrimarySpecification;
		boolean isReplacingSecondarySpecification;
		JustificationItem createdJustificationItem = null;
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem additionalProperNounSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		WordItem _myWordItem = myWordItem();
		JustificationResultType justificationResult;

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		hasCurrentlyCorrectedAssumption = _myWordItem.hasCurrentlyCorrectedAssumption();

		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.isExclusiveNonPossessiveGeneralizationAssignment() )
			isExclusiveGeneralization = true;

		while( searchJustificationItem != null )
			{
			// In current implementation, this specification is never updated
			if( ( additionalDefinitionSpecificationItem = searchJustificationItem.additionalDefinitionSpecificationItem() ) == obsoleteSpecificationItem )
				return startError( 1, "Replacing an obsolete additional definition specification item is not implemented" );

			// In current implementation, this specification is never updated
			if( ( additionalProperNounSpecificationItem = searchJustificationItem.additionalProperNounSpecificationItem() ) == obsoleteSpecificationItem )
				return startError( 1, "Replacing an obsolete additional proper noun specification item is not implemented" );

			// In current implementation, only primary and secondary specification are updated
			isReplacingPrimarySpecification = ( searchJustificationItem.primarySpecificationItem() == obsoleteSpecificationItem );
			isReplacingSecondarySpecification = ( searchJustificationItem.secondarySpecificationItem() == obsoleteSpecificationItem );

			if( isReplacingPrimarySpecification ||
			isReplacingSecondarySpecification )
				{
				if( replacingSpecificationItem != null &&
				searchJustificationItem.hasCurrentCreationSentenceNr() )
					{
					if( searchJustificationItem.replaceSpecification( isReplacingPrimarySpecification, isReplacingSecondarySpecification, replacingSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace a specification item of the search justification item" );
					}
				else
					{
					createdJustificationItem = null;

					if( replacingSpecificationItem != null &&

						( !searchJustificationItem.isOppositePossessiveSpecificationAssumption() ||
						replacingSpecificationItem.hasRelationWord() ||
						// Test file: "reasoning\family\This information is more specific (non-exclusive)"
						replacingSpecificationItem.isSelfGeneratedSpecification() ||
						// Typical for Spanish
						// Spanish test file: "razonamiento\familia\Esta información es más específico (no exclusiva)"
						obsoleteSpecificationItem.isUserSpecification() ) &&

					// Avoid creating unreferenced justifications
					_myWordItem.isJustificationInUse( searchJustificationItem ) )
						{
						selectedPrimarySpecificationItem = ( isReplacingPrimarySpecification ? replacingSpecificationItem :
																								searchJustificationItem.updatedPrimarySpecificationItem() );
						selectedSecondarySpecificationItem = ( isReplacingSecondarySpecification ? replacingSpecificationItem :
																								searchJustificationItem.updatedSecondarySpecificationItem() );

						// Create justification
						if( ( justificationResult = createJustificationItem( searchJustificationItem.hasFeminineOrMasculineProperNounEnding(), searchJustificationItem.justificationTypeNr(), searchJustificationItem.justificationOrderNr, searchJustificationItem.originalSentenceNr(), selectedPrimarySpecificationItem, additionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, additionalProperNounSpecificationItem, searchJustificationItem.attachedJustificationItem() ) ).result != Constants.RESULT_OK )
							return addError( 1, "I failed to create a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
							return startError( 1, "No justification item is created" );
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace a justification item" );
					}
				}

			searchJustificationItem = ( !hasCurrentlyCorrectedAssumption &&					// First justification
										searchJustificationItem.isReplacedOrDeletedItem() ? firstActiveJustificationItem() :
																							// Next justification
																							searchJustificationItem.nextJustificationItem() );
			}

		return Constants.RESULT_OK;
		}

	protected JustificationItem foundJustificationItem( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		if( primarySpecificationItem != null &&

			// Test files: "reasoning\family\I know (1)",
			//				"reasoning\family\I know (10)"
			( justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ||

			// Test file: "reasoning\family\Laura has a father - Laura is a child - John and Anna"
			justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||

			// Typical for Spanish
			// Spanish test files: "razonamiento\familia\Complejo (1)",
			//						"razonamiento\familia\Complejo (6)",
			//						among others
			justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||

			// Typical for Spanish
			// Spanish test file: "razonamiento\familia\Complejo (15)"
			( justificationTypeNr == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
			!primarySpecificationItem.hasCurrentCreationSentenceNr() &&
			primarySpecificationItem.hasRelationCollection() &&
			primarySpecificationItem.isSelfGeneratedSpecification() ) ) )

			return justificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem );

		return null;
		}

	protected JustificationItem primarySpecificationJustificationItem( short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.primarySpecificationItem() == primarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem primaryAndSecondarySpecificationJustificationItem( short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem.primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationItem secondarySpecificationJustificationItem( short justificationTypeNr, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem suggestiveQuestionAssumptionSpecificationItem()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isSuggestiveQuestionAssumption() )
				return searchJustificationItem.primarySpecificationItem();

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationResultType copyJustificationItem( SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		if( originalJustificationItem == null )
			return startJustificationResultError( 1, "The given original justification item is undefined" );

		if( originalJustificationItem.isReplacedItem() )
			return startJustificationResultError( 1, "The given original justification item is replaced" );

		return createJustificationItem( originalJustificationItem.hasFeminineOrMasculineProperNounEnding(), originalJustificationItem.justificationTypeNr(), originalJustificationItem.justificationOrderNr, originalJustificationItem.originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem.additionalDefinitionSpecificationItem(), newSecondarySpecificationItem, originalJustificationItem.additionalProperNounSpecificationItem(), newAttachedJustificationItem );
		}

	protected JustificationResultType createJustificationItem( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, short _justificationOrderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem additionalProperNounSpecificationItem, JustificationItem attachedJustificationItem )
		{
		boolean isIncrementingOrderNr = false;
		boolean isOppositePossessiveSpecificationAssumption = ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION );
		short foundJustificationOrderNr;
		JustificationResultType justificationResult = new JustificationResultType();

		if( primarySpecificationItem == null )
			return startJustificationResultError( 1, "The given primary specification item is undefined" );

		if( primarySpecificationItem.isReplacedItem() )
			return startJustificationResultError( 1, "The given primary specification item is replaced" );

		if( primarySpecificationItem.isHiddenSpanishSpecification() )
			return startJustificationResultError( 1, "The given primary specification item is hidden" );

		if( isOppositePossessiveSpecificationAssumption )
			{
			if( primarySpecificationItem.isProperNounGeneralization() )
				return startJustificationResultError( 1, "The given primary specification item is a proper noun specification" );

			if( primarySpecificationItem.isExclusiveSpecification() )
				return startJustificationResultError( 1, "The given primary specification item is exclusive, while the justification type is non-exclusive" );
			}

		if( additionalDefinitionSpecificationItem != null )
			{
			if( additionalDefinitionSpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given additional definition specification item is replaced" );

			if( !additionalDefinitionSpecificationItem.isNounGeneralization() )
				return startJustificationResultError( 1, "The given additional definition specification item is not a definition specification" );
			}

		if( secondarySpecificationItem != null )
			{
			if( secondarySpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given secondary specification item is replaced" );

			if( secondarySpecificationItem.isHiddenSpanishSpecification() )
				return startJustificationResultError( 1, "The given secondary specification item is hidden" );

			// Definition
			if( secondarySpecificationItem.isNounGeneralization() )
				{
				if( isOppositePossessiveSpecificationAssumption &&
				secondarySpecificationItem.isExclusiveSpecification() )
					return startJustificationResultError( 1, "The given secondary specification item is exclusive, while the justification type is non-exclusive" );

				if( ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION ) && 

				!secondarySpecificationItem.isPartOf() )
					return startJustificationResultError( 1, "The given secondary specification item is no part-of definition" );
				}
			}

		if( additionalProperNounSpecificationItem != null )
			{
			if( additionalProperNounSpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given additional proper noun specification item is replaced" );

			if( !additionalProperNounSpecificationItem.isProperNounGeneralization() )
				return startJustificationResultError( 1, "The given additional definition specification item is not a proper noun specification" );
			}

		if( secondarySpecificationItem != null )
			{
			if( isOppositePossessiveSpecificationAssumption )
				{
				if( additionalDefinitionSpecificationItem == null )
					{
					if( primarySpecificationItem.specificationWordItem() != secondarySpecificationItem.specificationWordItem() &&

						( !primarySpecificationItem.isSpecificationWordSpanishAmbiguous() ||
						// Typical for Spanish
						secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) )
						return startJustificationResultError( 1, "The specification words of the given primary and secondary specification items are different" );
					}
				else
					{
					if( primarySpecificationItem.specificationWordItem() != additionalDefinitionSpecificationItem.generalizationWordItem() )
						return startJustificationResultError( 1, "The specification word of the given primary and generalization word of the given additional definition specification are different" );

					if( additionalDefinitionSpecificationItem.specificationWordItem() != secondarySpecificationItem.specificationWordItem() )
						return startJustificationResultError( 1, "The specification words of the given additional definition and secondary specification items are different" );
					}
				}
			else
				{
				if( justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
				!primarySpecificationItem.isExclusiveSpecification() &&
				primarySpecificationItem.isProperNounGeneralization() &&
				!secondarySpecificationItem.isExclusiveSpecification() )
					return startJustificationResultError( 1, "Both the given primary and secondary specification item are non-exclusive, while the justification type is exclusive" );
				}
			}

		if( _justificationOrderNr == Constants.NO_ORDER_NR )
			{
			if( ( _justificationOrderNr = highestJustificationOrderNr( justificationTypeNr ) ) > Constants.NO_ORDER_NR )
				{
				// Group justification by selecting the order number
				switch( justificationTypeNr )
					{
					case Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
					case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
					case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
						if( ( foundJustificationOrderNr = justificationOrderNr( primarySpecificationItem.isExclusiveSpecification(), primarySpecificationItem.isPossessive(), justificationTypeNr, primarySpecificationItem.specificationWordTypeNr(), primarySpecificationItem.specificationCollectionNr(), secondarySpecificationItem ) ) == Constants.NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
					case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
						if( secondarySpecificationItem == null )
							return startJustificationResultError( 1, "This justification type should have a secondary specification" );

						// Typical for Spanish
						if( ( foundJustificationOrderNr = justificationOrderNr( ( additionalDefinitionSpecificationItem != null ), hasFeminineOrMasculineProperNounEnding, primarySpecificationItem.isPossessive(), secondarySpecificationItem.isPossessive(), primarySpecificationItem.isSpecificationWordSpanishAmbiguous(), justificationTypeNr, primarySpecificationItem.specificationCollectionNr() ) ) == Constants.NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
						if( ( foundJustificationOrderNr = reversibleJustificationOrderNr( primarySpecificationItem.isPossessive() ) ) == Constants.NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION:
						if( ( foundJustificationOrderNr = negativeJustificationOrderNr( primarySpecificationItem.isExclusiveSpecification(), primarySpecificationItem.isPossessive(), secondarySpecificationItem ) ) == Constants.NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
						if( ( foundJustificationOrderNr = questionJustificationOrderNr( ( additionalDefinitionSpecificationItem != null ), primarySpecificationItem.specificationCollectionNr(), secondarySpecificationItem ) ) == Constants.NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;
					}
				}

			if( isIncrementingOrderNr ||
			_justificationOrderNr == Constants.NO_ORDER_NR )
				{
				if( _justificationOrderNr >= Constants.MAX_ORDER_NR )
					return startJustificationResultError( 1, "Justification order number overflow! I can't accept more justifications" );

				_justificationOrderNr++;
				}
			}

		if( _justificationOrderNr <= Constants.NO_ORDER_NR )
			return startJustificationResultError( 1, "The order number is undefined" );

		// Create justification item
		if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, _justificationOrderNr, originalSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem, attachedJustificationItem, this, myWordItem() ) ) == null )
			return startJustificationResultError( 1, "I failed to create a justification item" );

		// Add justification item to list
		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
			return addJustificationResultError( 1, "I failed to add a justification item" );

		return justificationResult;
		}
	}

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
