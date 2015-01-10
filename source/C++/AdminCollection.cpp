/*
 *	Class:			AdminCollection
 *	Supports class:	AdminItem
 *	Purpose:		To collect (associate, combine) words in the knowledge structure
 *					that belong together (which implies differentiating
 *					words that doesn't belong together)
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

#include "AdminItem.h"
#include "SpecificationItem.cpp"

class AdminCollection
	{
	friend class AdminItem;

	// Private constructible variables

	unsigned short existingPairCollectionOrderNr_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType checkCollectionInAllWords( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		unsigned short foundCollectionOrderNr = NO_ORDER_NR;
		unsigned int nWords = 0;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCollectionInAllWords";

		existingPairCollectionOrderNr_ = NO_ORDER_NR;

		if( collectionNr > NO_COLLECTION_NR )
			{
			if( collectionWordItem != NULL )
				{
				if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
					{
					// Do for all words
					do	{
						if( ( foundCollectionOrderNr = currentWordItem->collectionOrderNr( collectionNr, collectionWordItem, commonWordItem ) ) > NO_ORDER_NR )
							{
							if( ++nWords == 2 )		// Found existing collection pair
								existingPairCollectionOrderNr_ = foundCollectionOrderNr;
							}
						}
					while( existingPairCollectionOrderNr_ == NO_ORDER_NR &&
					( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given collection word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given collection number is undefined" );

		return RESULT_OK;
		}

	ResultType collectGeneralizationWords( bool isExclusiveGeneralization, unsigned short generalizationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousGeneralizationWordItem, WordItem *newGeneralizationWordItem, WordItem *previousCommonWordItem, WordItem *newCommonWordItem )
		{
		CollectionResultType collectionResult;
		bool hasFoundCollection = false;
		unsigned int collectionNr = NO_COLLECTION_NR;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectGeneralizationWords";

		if( previousGeneralizationWordItem != NULL )
			{
			if( newGeneralizationWordItem != NULL )
				{
				if( previousGeneralizationWordItem != newGeneralizationWordItem )
					{
					if( newCommonWordItem != NULL )
						{
						if( ( collectionNr = previousGeneralizationWordItem->collectionNr( generalizationWordTypeNr, newCommonWordItem ) ) == NO_COLLECTION_NR )
							collectionNr = newGeneralizationWordItem->collectionNr( generalizationWordTypeNr, newCommonWordItem );

						if( collectionNr > NO_COLLECTION_NR )
							{
							if( ( collectionResult = newGeneralizationWordItem->findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result == RESULT_OK )
								{
								if( collectionResult.isCollected )
									hasFoundCollection = true;
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" is collected with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );
							}

						if( !hasFoundCollection )
							{
							if( ( collectionResult = previousGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, newGeneralizationWordItem, newCommonWordItem, NULL, NULL ) ).result == RESULT_OK )
								{
								if( collectionNr == NO_COLLECTION_NR )
									collectionNr = collectionResult.createdCollectionNr;

								if( newGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, previousGeneralizationWordItem, newCommonWordItem, NULL, NULL ).result == RESULT_OK )
									{
									if( previousGeneralizationWordItem->collectGeneralizationsOrSpecifications( isExclusiveGeneralization, true, false, collectionNr ) == RESULT_OK )
										{
										if( newCommonWordItem != previousCommonWordItem )
											{
											if( previousGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, newGeneralizationWordItem, previousCommonWordItem, NULL, NULL ).result == RESULT_OK )
												{
												if( newGeneralizationWordItem->addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, collectionNr, previousGeneralizationWordItem, previousCommonWordItem, NULL, NULL ).result != RESULT_OK )
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", newGeneralizationWordItem->anyWordTypeString(), "\" with word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );
											}

										if( newGeneralizationWordItem->collectGeneralizationsOrSpecifications( isExclusiveGeneralization, true, false, collectionNr ) != RESULT_OK )
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect generalizations and specifications in the new generalization word" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect the generalizations and specifications in the previous generalization word" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", newGeneralizationWordItem->anyWordTypeString(), "\" with word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given new common word item is undefined" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous and new generalization word items are the same word" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given new generalization word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous generalization word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Deconstructor / deconstructor

	AdminCollection( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		existingPairCollectionOrderNr_ = NO_ORDER_NR;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminCollection" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	CollectionResultType collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		CollectionResultType collectionResult;
		bool hasFoundCollection = false;
		unsigned short collectionOrderNr = NO_ORDER_NR;
		unsigned int collectionNr = NO_COLLECTION_NR;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( generalizationWordItem != NULL )
			{
			if( previousSpecificationWordItem != NULL )
				{
				if( currentSpecificationWordItem != NULL )
					{
					if( previousSpecificationWordItem != currentSpecificationWordItem )
						{
						if( compoundGeneralizationWordItem == NULL )
							{
							if( ( collectionNr = previousSpecificationWordItem->collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == NO_COLLECTION_NR )
								{
								if( ( collectionNr = currentSpecificationWordItem->collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == NO_COLLECTION_NR )
									{
									if( ( collectionNr = previousSpecificationWordItem->collectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
										collectionNr = currentSpecificationWordItem->collectionNr( specificationWordTypeNr );
									}
								}
							}
						else
							collectionNr = myWordItem_->collectionNrByCompoundGeneralizationWordInAllWords( specificationWordTypeNr, compoundGeneralizationWordItem );

						if( collectionNr > NO_COLLECTION_NR )
							{
							if( checkCollectionInAllWords( collectionNr, currentSpecificationWordItem, generalizationWordItem ) == RESULT_OK )
								{
								if( existingPairCollectionOrderNr_ > NO_ORDER_NR )
									{
									if( ( collectionOrderNr = myWordItem_->highestCollectionOrderNrInAllWords( collectionNr ) ) > 1 )
										{
										if( existingPairCollectionOrderNr_ < collectionOrderNr - 1 )	// "- 1" because collections come in pairs
											collectionNr = NO_COLLECTION_NR;
										}
									}
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check the collection in all words" );
							}

						if( commonVariables_->result == RESULT_OK &&
						!isPossessive &&
						collectionNr > NO_COLLECTION_NR )
							{
							if( ( collectionResult = previousSpecificationWordItem->findCollection( true, currentSpecificationWordItem, generalizationWordItem ) ).result == RESULT_OK )
								{
								if( collectionResult.isCollected )
									hasFoundCollection = true;
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if word \"", currentSpecificationWordItem->anyWordTypeString(), "\" is collected with word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
							}

						if( commonVariables_->result == RESULT_OK &&
						!hasFoundCollection )
							{
							if( collectionOrderNr < MAX_ORDER_NR - 2 )
								{
								if( ( collectionResult = previousSpecificationWordItem->addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem, NULL ) ).result == RESULT_OK )
									{
									if( collectionNr == NO_COLLECTION_NR )
										collectionNr = collectionResult.createdCollectionNr;

									if( currentSpecificationWordItem->addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem, NULL ).result != RESULT_OK )
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								else
									myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Collection order number overflow" );
							}

						if( commonVariables_->result == RESULT_OK )
							{
							if( generalizationWordItem->collectGeneralizationsOrSpecifications( isExclusiveSpecification, false, isQuestion, collectionNr ) != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect generalizations and specifications in the given generalization word" );
							}
						}
					else
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous and current specification words are the same word" );
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given current specification word item is undefined" );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous specification word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		collectionResult.result = commonVariables_->result;
		return collectionResult;
		}

	ResultType collectGeneralizationWordWithPreviousOne( bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		GeneralizationResultType generalizationResult;
		bool isNeedingAuthorizationForChanges;
		bool isSpecificationCollectedWithItself;
		bool hasCollectedGeneralizationWords = false;
		bool isExclusiveGeneralization = false;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *previousSpecificationWordItem = NULL;
		WordItem *previousGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectGeneralizationWordWithPreviousOne";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				isNeedingAuthorizationForChanges = specificationWordItem->isNeedingAuthorizationForChanges();
				isSpecificationCollectedWithItself = specificationWordItem->isCollectedWithItself();

				if( specificationCollectionNr == NO_COLLECTION_NR )
					specificationCollectionNr = specificationWordItem->collectionNr( specificationWordTypeNr );

				if( ( currentGeneralizationWordItem = commonVariables_->firstWordItem ) != NULL )
					{
					// Do for all words
					do	{
						if( currentGeneralizationWordItem != generalizationWordItem &&
						currentGeneralizationWordItem->hasWordType( generalizationWordTypeNr ) )
							{
							// Try to find matching specification word
							if( ( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, false, false, isPossessive, NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
								{
								// Relation word of a generalization word: proper noun
								if( ( generalizationResult = currentGeneralizationWordItem->findGeneralization( true, generalizationWordItem ) ).result == RESULT_OK )
									{
									if( !generalizationResult.hasFoundGeneralization )
										{
										if( !isPossessive &&
										!isSpecificationCollectedWithItself &&

										( isNeedingAuthorizationForChanges ||
										foundSpecificationItem->isAssignment() ) )
											isExclusiveGeneralization = true;

										if( previousGeneralizationWordItem != NULL )
											{
											if( collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) == RESULT_OK )
												hasCollectedGeneralizationWords = true;
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect generalization words \"", previousGeneralizationWordItem->anyWordTypeString(), "\" and \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}

										previousGeneralizationWordItem = currentGeneralizationWordItem;
										previousSpecificationWordItem = foundSpecificationItem->specificationWordItem();
										}
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a generalization item" );
								}
							}
						}
					// Continue search - even if a generalization word is found - to get the most recent generalization word
					while( ( currentGeneralizationWordItem = currentGeneralizationWordItem->nextWordItem() ) != NULL );

					if( !hasCollectedGeneralizationWords &&
					previousGeneralizationWordItem != NULL )
						{
						if( collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect generalization words \"", previousGeneralizationWordItem->anyWordTypeString(), "\" and \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any generalization word" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType collectSpecificationStrings( bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, char *previousSpecificationString, char *currentSpecificationString )
		{
		CollectionResultType collectionResult;
		unsigned int collectionNr;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationStrings";

		if( generalizationWordItem != NULL )
			{
			if( previousSpecificationString != NULL )
				{
				if( currentSpecificationString != NULL )
					{
					if( ( collectionNr = generalizationWordItem->collectionNr( specificationWordTypeNr, NULL ) ) == NO_COLLECTION_NR )
						{
						if( ( collectionResult = generalizationWordItem->addCollection( isExclusiveSpecification, false, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, NULL, NULL, NULL, previousSpecificationString ) ).result == RESULT_OK )
							{
							if( collectionNr == NO_COLLECTION_NR )
								collectionNr = collectionResult.createdCollectionNr;
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect the previous specification string \"", previousSpecificationString, "\" in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}

					if( generalizationWordItem->addCollection( isExclusiveSpecification, false, specificationWordTypeNr, generalizationWordTypeNr, collectionNr, NULL, NULL, NULL, currentSpecificationString ).result != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect the current specification string \"", currentSpecificationString, "\" in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given current specification string item is undefined" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous specification string item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *commonWordItem )
		{
		CollectionResultType collectionResult;
		bool hasFoundCollection = false;
		unsigned int collectionNr = NO_COLLECTION_NR;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectRelationWords";

		if( previousRelationWordItem != NULL )
			{
			if( currentRelationWordItem != NULL )
				{
				if( previousRelationWordItem != currentRelationWordItem )
					{
					if( ( collectionNr = previousRelationWordItem->collectionNr( relationWordTypeNr, commonWordItem ) ) == NO_COLLECTION_NR )
						collectionNr = currentRelationWordItem->collectionNr( relationWordTypeNr, commonWordItem );

					if( collectionNr > NO_COLLECTION_NR )
						{
						if( ( collectionResult = previousRelationWordItem->findCollection( false, currentRelationWordItem, commonWordItem ) ).result == RESULT_OK )
							{
							if( collectionResult.isCollected )
								hasFoundCollection = true;
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find out if word \"", previousRelationWordItem->anyWordTypeString(), "\" is collected with word \"", currentRelationWordItem->anyWordTypeString(), "\"" );
						}

					if( !hasFoundCollection )
						{
						if( ( collectionResult = previousRelationWordItem->addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, NULL, NULL ) ).result == RESULT_OK )
							{
							if( collectionNr == NO_COLLECTION_NR )
								collectionNr = collectionResult.createdCollectionNr;

							if( currentRelationWordItem->addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, NULL, NULL ).result != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", currentRelationWordItem->anyWordTypeString(), "\" with word \"", previousRelationWordItem->anyWordTypeString(), "\"" );
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to collect word \"", previousRelationWordItem->anyWordTypeString(), "\" with word \"", currentRelationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous and current relation words are the same word" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given current relation word item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given previous relation word item is undefined" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"I will sing of your love and justice, O Lord.
 *	I will praise you with songs." (Psalm 101:1)
 *
 *************************************************************************/
