/*	Class:			AdminCollection
 *	Supports class:	AdminItem
 *	Purpose:		To collect (associate, combine) words in the knowledge structure
 *					that belong together (which implies differentiating
 *					words that doesn't belong together)
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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

class AdminCollection
	{
	// Private constructible variables

	private short existingPairCollectionOrderNr_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static int collectionNrByCompoundGeneralizationWordInAllWords( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		WordItem currentWordItem;

		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		compoundGeneralizationWordItem != null &&
		( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( ( collectionNr = currentWordItem.collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	private byte checkCollectionInAllWords( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		short foundCollectionOrderNr = Constants.NO_ORDER_NR;
		int nWords = 0;
		WordItem currentWordItem;

		existingPairCollectionOrderNr_ = Constants.NO_ORDER_NR;

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			if( collectionWordItem != null )
				{
				if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
					{
					// Do for all active words
					do	{
						if( ( foundCollectionOrderNr = currentWordItem.collectionOrderNr( collectionNr, collectionWordItem, commonWordItem ) ) > Constants.NO_ORDER_NR )
							{
							// Found existing collection pair
							if( ++nWords == 2 )
								existingPairCollectionOrderNr_ = foundCollectionOrderNr;
							}
						}
					while( existingPairCollectionOrderNr_ == Constants.NO_ORDER_NR &&
					( currentWordItem = currentWordItem.nextWordItem() ) != null );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The first word item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given collection word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given collection number is undefined" );

		return Constants.RESULT_OK;
		}

	private byte collectGeneralizationWords( boolean isExclusiveGeneralization, short generalizationWordTypeNr, short commonWordTypeNr, WordItem previousGeneralizationWordItem, WordItem newGeneralizationWordItem, WordItem previousCommonWordItem, WordItem newCommonWordItem )
		{
		CollectionResultType collectionResult;
		boolean hasFoundCollection = false;
		int collectionNr = Constants.NO_COLLECTION_NR;

		if( previousGeneralizationWordItem != null )
			{
			if( newGeneralizationWordItem != null )
				{
				if( previousGeneralizationWordItem != newGeneralizationWordItem )
					{
					if( newCommonWordItem != null )
						{
						if( ( collectionNr = previousGeneralizationWordItem.collectionNr( generalizationWordTypeNr, newCommonWordItem ) ) == Constants.NO_COLLECTION_NR )
							collectionNr = newGeneralizationWordItem.collectionNr( generalizationWordTypeNr, newCommonWordItem );

						if( collectionNr > Constants.NO_COLLECTION_NR )
							{
							if( ( collectionResult = newGeneralizationWordItem.findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result == Constants.RESULT_OK )
								{
								if( collectionResult.isCollected )
									hasFoundCollection = true;
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" is collected with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
							}

						if( !hasFoundCollection )
							{
							if( ( collectionResult = previousGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, newGeneralizationWordItem, newCommonWordItem, null ) ).result == Constants.RESULT_OK )
								{
								if( collectionNr == Constants.NO_COLLECTION_NR )
									collectionNr = collectionResult.createdCollectionNr;

								if( newGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, previousGeneralizationWordItem, newCommonWordItem, null ).result == Constants.RESULT_OK )
									{
									if( previousGeneralizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveGeneralization, true, false, collectionNr ) == Constants.RESULT_OK )
										{
										if( newCommonWordItem != previousCommonWordItem )
											{
											if( previousGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, newGeneralizationWordItem, previousCommonWordItem, null ).result == Constants.RESULT_OK )
												{
												if( newGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, previousGeneralizationWordItem, previousCommonWordItem, null ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
											}

										if( newGeneralizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveGeneralization, true, false, collectionNr ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations and specifications in the new generalization word" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to collect the generalizations and specifications in the previous generalization word" );
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The given new common word item is undefined" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given previous and new generalization word items are the same word" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given new generalization word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given previous generalization word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Deconstructor / deconstructor

	protected AdminCollection( AdminItem adminItem )
		{
		String errorString = null;

		existingPairCollectionOrderNr_ = Constants.NO_ORDER_NR;

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

	protected CollectionResultType collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		boolean hasFoundCollection = false;
		short collectionOrderNr = Constants.NO_ORDER_NR;
		int collectionNr = Constants.NO_COLLECTION_NR;

		if( generalizationWordItem != null )
			{
			if( previousSpecificationWordItem != null )
				{
				if( currentSpecificationWordItem != null )
					{
					if( previousSpecificationWordItem != currentSpecificationWordItem )
						{
						if( compoundGeneralizationWordItem == null )
							{
							if( ( collectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == Constants.NO_COLLECTION_NR )
								{
								if( ( collectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == Constants.NO_COLLECTION_NR )
									{
									if( ( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_PLURAL ||
									generalizationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ) &&

									( collectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
										collectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr );
									}
								}
							}
						else
							{
							if( !isExclusiveSpecification ||
							generalizationWordItem == previousSpecificationWordItem ||
							!generalizationWordItem.isNounWordSpanishAmbiguous() )
								collectionNr = collectionNrByCompoundGeneralizationWordInAllWords( specificationWordTypeNr, compoundGeneralizationWordItem );
							}

						if( collectionNr > Constants.NO_COLLECTION_NR )
							{
							if( checkCollectionInAllWords( collectionNr, currentSpecificationWordItem, generalizationWordItem ) == Constants.RESULT_OK )
								{
								if( existingPairCollectionOrderNr_ > Constants.NO_ORDER_NR &&
								( collectionOrderNr = adminItem_.highestCollectionOrderNrInAllWords( collectionNr ) ) > 1 )
									{
									// "- 1" because collections come in pairs
									if( existingPairCollectionOrderNr_ < collectionOrderNr - 1 )
										collectionNr = Constants.NO_COLLECTION_NR;
									}
								}
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to check the collection in all words" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isPossessive &&
						collectionNr > Constants.NO_COLLECTION_NR )
							{
							if( ( collectionResult = previousSpecificationWordItem.findCollection( ( previousSpecificationWordItem != generalizationWordItem ), currentSpecificationWordItem, generalizationWordItem ) ).result == Constants.RESULT_OK )
								{
								if( collectionResult.isCollected )
									hasFoundCollection = true;
								}
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" is collected with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!hasFoundCollection )
							{
							if( collectionOrderNr < Constants.MAX_ORDER_NR - 2 )
								{
								if( ( collectionResult = previousSpecificationWordItem.addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result == Constants.RESULT_OK )
									{
									if( collectionNr == Constants.NO_COLLECTION_NR )
										collectionNr = collectionResult.createdCollectionNr;

									if( currentSpecificationWordItem.addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != Constants.RESULT_OK )
										adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								adminItem_.startSystemError( 1, moduleNameString_, "Collection order number overflow" );
							}

						if( CommonVariables.result == Constants.RESULT_OK )
							{
							if( generalizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveSpecification, false, isQuestion, collectionNr ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations and specifications in the given generalization word" );
							}
						}
					else
						adminItem_.startError( 1, moduleNameString_, "The given previous and current specification words are the same word" );
					}
				else
					adminItem_.startError( 1, moduleNameString_, "The given current specification word item is undefined" );
				}
			else
				adminItem_.startError( 1, moduleNameString_, "The given previous specification word item is undefined" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		collectionResult.result = CommonVariables.result;
		return collectionResult;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		GeneralizationResultType generalizationResult;
		boolean isNeedingAuthorizationForChanges;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isExclusiveGeneralization = false;
		SpecificationItem foundSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem previousGeneralizationWordItem = null;
		WordItem previousSpecificationWordItem = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				isNeedingAuthorizationForChanges = specificationWordItem.isNeedingAuthorizationForChanges();
				isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

				if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
					specificationCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr );

				if( ( currentGeneralizationWordItem = CommonVariables.firstWordItem ) != null )
					{
					// Do for all active words
					do	{
						if( currentGeneralizationWordItem != generalizationWordItem &&
						currentGeneralizationWordItem.hasWordType( false, generalizationWordTypeNr ) &&
						// Try to find matching specification word
						( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
							{
							// Relation word of a generalization word: proper name
							if( ( generalizationResult = currentGeneralizationWordItem.findGeneralization( true, generalizationWordItem ) ).result == Constants.RESULT_OK )
								{
								if( !generalizationResult.hasFoundGeneralization )
									{
									if( isAssignment &&
									!isPossessive &&
									!isSpecificationWordSpanishAmbiguous &&

									( isNeedingAuthorizationForChanges ||
									foundSpecificationItem.isActiveAssignment() ) )
										isExclusiveGeneralization = true;

									previousGeneralizationWordItem = currentGeneralizationWordItem;
									previousSpecificationWordItem = foundSpecificationItem.specificationWordItem();
									}
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to find a generalization item" );
							}
						}
					// Continue search to get the most recent generalization word
					while( ( currentGeneralizationWordItem = currentGeneralizationWordItem.nextWordItem() ) != null );

					if( previousGeneralizationWordItem != null )
						{
						if( collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalization words \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" and \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "I couldn't find any generalization word" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte collectRelationWords( boolean isExclusiveSpecification, short relationWordTypeNr, short commonWordTypeNr, WordItem previousRelationWordItem, WordItem currentRelationWordItem, WordItem commonWordItem )
		{
		CollectionResultType collectionResult;
		boolean hasFoundCollection = false;
		int collectionNr = Constants.NO_COLLECTION_NR;

		if( previousRelationWordItem != null )
			{
			if( currentRelationWordItem != null )
				{
				if( previousRelationWordItem != currentRelationWordItem )
					{
					if( ( collectionNr = previousRelationWordItem.collectionNr( relationWordTypeNr, commonWordItem ) ) == Constants.NO_COLLECTION_NR )
						collectionNr = currentRelationWordItem.collectionNr( relationWordTypeNr, commonWordItem );

					if( collectionNr > Constants.NO_COLLECTION_NR )
						{
						if( ( collectionResult = previousRelationWordItem.findCollection( false, currentRelationWordItem, commonWordItem ) ).result == Constants.RESULT_OK )
							{
							if( collectionResult.isCollected )
								hasFoundCollection = true;
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousRelationWordItem.anyWordTypeString() + "\" is collected with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );
						}

					if( !hasFoundCollection )
						{
						if( ( collectionResult = previousRelationWordItem.addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, null ) ).result == Constants.RESULT_OK )
							{
							if( collectionNr == Constants.NO_COLLECTION_NR )
								collectionNr = collectionResult.createdCollectionNr;

							if( currentRelationWordItem.addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + currentRelationWordItem.anyWordTypeString() + "\" with word \"" + previousRelationWordItem.anyWordTypeString() + "\"" );
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousRelationWordItem.anyWordTypeString() + "\" with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given previous and current relation words are the same word" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given current relation word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given previous relation word item is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"I will sing of your love and justice, O Lord.
 *	I will praise you with songs." (Psalm 101:1)
 *************************************************************************/
