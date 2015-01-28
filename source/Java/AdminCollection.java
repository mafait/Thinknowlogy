/*
 *	Class:			AdminCollection
 *	Supports class:	AdminItem
 *	Purpose:		To collect (associate, combine) words in the knowledge structure
 *					that belong together (which implies differentiating
 *					words that doesn't belong together)
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

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
					// Do for all words
					do	{
						if( ( foundCollectionOrderNr = currentWordItem.collectionOrderNr( collectionNr, collectionWordItem, commonWordItem ) ) > Constants.NO_ORDER_NR )
							{
							if( ++nWords == 2 )		// Found existing collection pair
								existingPairCollectionOrderNr_ = foundCollectionOrderNr;
							}
						}
					while( existingPairCollectionOrderNr_ == Constants.NO_ORDER_NR &&
					( currentWordItem = currentWordItem.nextWordItem() ) != null );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given collection word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given collection number is undefined" );

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
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" is collected with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
							}

						if( !hasFoundCollection )
							{
							if( ( collectionResult = previousGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, newGeneralizationWordItem, newCommonWordItem, null, null ) ).result == Constants.RESULT_OK )
								{
								if( collectionNr == Constants.NO_COLLECTION_NR )
									collectionNr = collectionResult.createdCollectionNr;

								if( newGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, previousGeneralizationWordItem, newCommonWordItem, null, null ).result == Constants.RESULT_OK )
									{
									if( previousGeneralizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveGeneralization, true, false, collectionNr ) == Constants.RESULT_OK )
										{
										if( newCommonWordItem != previousCommonWordItem )
											{
											if( previousGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, newGeneralizationWordItem, previousCommonWordItem, null, null ).result == Constants.RESULT_OK )
												{
												if( newGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, previousGeneralizationWordItem, previousCommonWordItem, null, null ).result != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
											}

										if( newGeneralizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveGeneralization, true, false, collectionNr ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect generalizations and specifications in the new generalization word" );
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect the generalizations and specifications in the previous generalization word" );
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given new common word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous and new generalization word items are the same word" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given new generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous generalization word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Deconstructor / deconstructor

	protected AdminCollection( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		existingPairCollectionOrderNr_ = Constants.NO_ORDER_NR;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
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
									if( ( collectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
										collectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr );
									}
								}
							}
						else
							collectionNr = myWordItem_.collectionNrByCompoundGeneralizationWordInAllWords( specificationWordTypeNr, compoundGeneralizationWordItem );

						if( collectionNr > Constants.NO_COLLECTION_NR )
							{
							if( checkCollectionInAllWords( collectionNr, currentSpecificationWordItem, generalizationWordItem ) == Constants.RESULT_OK )
								{
								if( existingPairCollectionOrderNr_ > Constants.NO_ORDER_NR )
									{
									if( ( collectionOrderNr = myWordItem_.highestCollectionOrderNrInAllWords( collectionNr ) ) > 1 )
										{
										if( existingPairCollectionOrderNr_ < collectionOrderNr - 1 )	// "- 1" because collections come in pairs
											collectionNr = Constants.NO_COLLECTION_NR;
										}
									}
								}
							else
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check the collection in all words" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isPossessive &&
						collectionNr > Constants.NO_COLLECTION_NR )
							{
							if( ( collectionResult = previousSpecificationWordItem.findCollection( true, currentSpecificationWordItem, generalizationWordItem ) ).result == Constants.RESULT_OK )
								{
								if( collectionResult.isCollected )
									hasFoundCollection = true;
								}
							else
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" is collected with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!hasFoundCollection )
							{
							if( collectionOrderNr < Constants.MAX_ORDER_NR - 2 )
								{
								if( ( collectionResult = previousSpecificationWordItem.addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem, null ) ).result == Constants.RESULT_OK )
									{
									if( collectionNr == Constants.NO_COLLECTION_NR )
										collectionNr = collectionResult.createdCollectionNr;

									if( currentSpecificationWordItem.addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem, null ).result != Constants.RESULT_OK )
										myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Collection order number overflow" );
							}

						if( CommonVariables.result == Constants.RESULT_OK )
							{
							if( generalizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveSpecification, false, isQuestion, collectionNr ) != Constants.RESULT_OK )
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect generalizations and specifications in the given generalization word" );
							}
						}
					else
						myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous and current specification words are the same word" );
					}
				else
					myWordItem_.startErrorInItem( 1, moduleNameString_, "The given current specification word item is undefined" );
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous specification word item is undefined" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		collectionResult.result = CommonVariables.result;
		return collectionResult;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		GeneralizationResultType generalizationResult;
		boolean isNeedingAuthorizationForChanges;
		boolean isSpecificationCollectedWithItself;
		boolean hasCollectedGeneralizationWords = false;
		boolean isExclusiveGeneralization = false;
		SpecificationItem foundSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem previousSpecificationWordItem = null;
		WordItem previousGeneralizationWordItem = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				isNeedingAuthorizationForChanges = specificationWordItem.isNeedingAuthorizationForChanges();
				isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

				if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
					specificationCollectionNr = specificationWordItem.collectionNr( specificationWordTypeNr );

				if( ( currentGeneralizationWordItem = CommonVariables.firstWordItem ) != null )
					{
					// Do for all words
					do	{
						if( currentGeneralizationWordItem != generalizationWordItem &&
						currentGeneralizationWordItem.hasWordType( generalizationWordTypeNr ) )
							{
							// Try to find matching specification word
							if( ( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
								{
								// Relation word of a generalization word: proper noun
								if( ( generalizationResult = currentGeneralizationWordItem.findGeneralization( true, generalizationWordItem ) ).result == Constants.RESULT_OK )
									{
									if( !generalizationResult.hasFoundGeneralization )
										{
										if( !isPossessive &&
										!isSpecificationCollectedWithItself &&

										( isNeedingAuthorizationForChanges ||
										foundSpecificationItem.isAssignment() ) )
											isExclusiveGeneralization = true;

										if( previousGeneralizationWordItem != null )
											{
											if( collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) == Constants.RESULT_OK )
												hasCollectedGeneralizationWords = true;
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect generalization words \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" and \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}

										previousGeneralizationWordItem = currentGeneralizationWordItem;
										previousSpecificationWordItem = foundSpecificationItem.specificationWordItem();
										}
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a generalization item" );
								}
							}
						}
					// Continue search - even if a generalization word is found - to get the most recent generalization word
					while( ( currentGeneralizationWordItem = currentGeneralizationWordItem.nextWordItem() ) != null );

					if( !hasCollectedGeneralizationWords &&
					previousGeneralizationWordItem != null )
						{
						if( collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect generalization words \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" and \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any generalization word" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte collectSpecificationStrings( boolean isExclusiveSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem generalizationWordItem, String previousSpecificationString, String currentSpecificationString )
		{
		CollectionResultType collectionResult;
		int collectionNr;

		if( generalizationWordItem != null )
			{
			if( previousSpecificationString != null )
				{
				if( currentSpecificationString != null )
					{
					if( ( collectionNr = generalizationWordItem.collectionNr( specificationWordTypeNr, null ) ) == Constants.NO_COLLECTION_NR )
						{
						if( ( collectionResult = generalizationWordItem.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, null, null, null, previousSpecificationString ) ).result == Constants.RESULT_OK )
							{
							if( collectionNr == Constants.NO_COLLECTION_NR )
								collectionNr = collectionResult.createdCollectionNr;
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect the previous specification string \"" + previousSpecificationString + "\" in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}

					if( generalizationWordItem.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, null, null, null, currentSpecificationString ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect the current specification string \"" + currentSpecificationString + "\" in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given current specification string item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous specification string item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

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
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if word \"" + previousRelationWordItem.anyWordTypeString() + "\" is collected with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );
						}

					if( !hasFoundCollection )
						{
						if( ( collectionResult = previousRelationWordItem.addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, null, null ) ).result == Constants.RESULT_OK )
							{
							if( collectionNr == Constants.NO_COLLECTION_NR )
								collectionNr = collectionResult.createdCollectionNr;

							if( currentRelationWordItem.addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, null, null ).result != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + currentRelationWordItem.anyWordTypeString() + "\" with word \"" + previousRelationWordItem.anyWordTypeString() + "\"" );
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect word \"" + previousRelationWordItem.anyWordTypeString() + "\" with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous and current relation words are the same word" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given current relation word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given previous relation word item is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"I will sing of your love and justice, O Lord.
 *	I will praise you with songs." (Psalm 101:1)
 *************************************************************************/
