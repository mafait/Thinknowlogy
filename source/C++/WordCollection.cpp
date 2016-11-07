/*	Class:			WordCollection
 *	Supports class:	WordItem
 *	Purpose:		To create collection structures
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

#include "CollectionList.cpp"
#include "GeneralizationItem.cpp"
#include "SpecificationItem.cpp"

class WordCollection
	{
	friend class WordItem;

	// Private constructed variables

	bool hasCreatedCollection_;

	WordItem *foundCollectionWordItem_;
	WordItem *foundGeneralizationWordItem_;

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	protected:
	// Constructor

	WordCollection( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasCreatedCollection_ = false;

		foundCollectionWordItem_ = NULL;
		foundGeneralizationWordItem_ = NULL;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordCollection" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	CollectionResultType addCollection( bool isExclusiveSpecification, bool isSpecificationGeneralization, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		CollectionResultType collectionResult;
		bool hasFoundCollection = false;
		bool isDuplicateCollection = false;
		unsigned short collectionOrderNr;
		unsigned short foundCollectionWordTypeNr = NO_WORD_TYPE_NR;
		unsigned int foundCollectionNr = NO_COLLECTION_NR;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollection";

		foundCollectionWordItem_ = NULL;

		if( collectionWordItem == NULL )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given collected word is undefined" );

		if( commonWordItem == NULL )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given common word is undefined" );

		if( collectionWordItem == myWordItem_ )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given collected word is the same as my word" );

		if( collectionWordTypeNr == WORD_TYPE_NOUN_PLURAL )
			collectionWordTypeNr = WORD_TYPE_NOUN_SINGULAR;

		if( !myWordItem_->hasWordType( true, collectionWordTypeNr ) )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "I don't have the requested word type number: ", collectionWordTypeNr );

		if( !collectionWordItem->hasWordType( true, collectionWordTypeNr ) )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The collection word item doesn't have the requested word type number: ", collectionWordTypeNr );

		if( collectionNr == NO_COLLECTION_NR )
			{
			if( ( collectionNr = myWordItem_->highestCollectionNrInAllWords() ) >= MAX_COLLECTION_NR )
				return myWordItem_->startCollectionResultSystemErrorInWord( functionNameString, moduleNameString_, "Collection number overflow" );

			collectionResult.createdCollectionNr = ++collectionNr;
			}

		foundCollectionNr = myWordItem_->collectionNr( collectionWordTypeNr, commonWordItem );

		if( foundCollectionNr > NO_COLLECTION_NR &&
		foundCollectionNr != collectionNr )
			{
			if( isSpecificationGeneralization )
				isDuplicateCollection = true;
			else
				{
				// Skip if Spanish ambiguous
				if( commonWordItem != compoundGeneralizationWordItem &&
				!commonWordItem->isNounWordSpanishAmbiguous() )
					{
					// Detected semantic ambiguity of the specification word
					if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_DUE_TO_SPECIFICATION_START, commonWordItem->wordTypeString( true, commonWordTypeNr ), INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_DUE_TO_SPECIFICATION_WORD, myWordItem_->wordTypeString( true, foundCollectionWordTypeNr ), INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_DUE_TO_SPECIFICATION_END ) != RESULT_OK )
						return myWordItem_->addCollectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'ambiguous due to' interface notification" );

					collectionResult.isAmbiguousCollection = true;
					}
				}
			}

		if( !isDuplicateCollection )
			{
			if( myWordItem_->collectionList == NULL )
				{
				// Create list
				if( ( myWordItem_->collectionList = new CollectionList( commonVariables_, myWordItem_ ) ) == NULL )
					return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a collection list" );

				myWordItem_->wordListArray[WORD_COLLECTION_LIST] = myWordItem_->collectionList;
				}
			else
				{
				if( collectionResult.createdCollectionNr == NO_COLLECTION_NR )
					// Check if collection already exists
					hasFoundCollection = myWordItem_->collectionList->hasCollection( collectionNr, collectionWordItem, commonWordItem );
				}

			if( !hasFoundCollection )
				{
				// A collection comes in pairs
				if( ( collectionOrderNr = myWordItem_->highestCollectionOrderNrInAllWords( collectionNr ) ) >= MAX_ORDER_NR - 1 )
					return myWordItem_->startCollectionResultSystemErrorInWord( functionNameString, moduleNameString_, "Collection order number overflow" );

				collectionResult.result = myWordItem_->collectionList->createCollectionItem( isExclusiveSpecification, ++collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
				}
			}

		return collectionResult;
		}

	CollectionResultType addCollectionByGeneralization( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		CollectionResultType collectionResult;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollectionByGeneralization";

		hasCreatedCollection_ = false;

		if( generalizationWordItem == NULL )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == myWordItem_ &&
		!myWordItem_->isNounWordSpanishAmbiguous() )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given generalization word item is the same as my word" );

		if( collectionWordItem == NULL )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given collected word item is undefined" );

		if( collectionWordItem == myWordItem_ )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given collected word item is the same as my word" );

		if( !myWordItem_->isNounWordType( collectionWordTypeNr ) )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given collected word type isn't a noun" );

		if( !myWordItem_->isNounWordType( commonWordTypeNr ) )
			return myWordItem_->startCollectionResultErrorInWord( functionNameString, moduleNameString_, "The given common word type isn't a noun" );

		if( ( currentSpecificationItem = myWordItem_->firstNonQuestionSpecificationItem() ) != NULL )
			{
			do	{
				if( currentSpecificationItem->isExclusiveSpecification() &&
				( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) == generalizationWordItem )
					{
					if( foundCollectionWordItem_ == NULL )
						{
						foundCollectionWordItem_ = collectionWordItem;
						foundGeneralizationWordItem_ = currentSpecificationWordItem;
						}
					else
						{
						if( foundCollectionWordItem_ != collectionWordItem )
							{
							if( ( collectionResult = foundCollectionWordItem_->findCollection( false, collectionWordItem, myWordItem_ ) ).result != RESULT_OK )
								return myWordItem_->addCollectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to find out if word \"", collectionWordItem->anyWordTypeString(), "\" is collected with word \"", foundCollectionWordItem_->anyWordTypeString(), "\"" );

							if( !collectionResult.isCollected )
								{
								if( ( collectionResult = foundCollectionWordItem_->addCollection( false, false, collectionWordTypeNr, commonWordTypeNr, NO_COLLECTION_NR, collectionWordItem, myWordItem_, NULL ) ).result != RESULT_OK )
									return myWordItem_->addCollectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", collectionWordItem->anyWordTypeString(), "\"" );

								if( collectionWordItem->addCollection( false, false, collectionWordTypeNr, commonWordTypeNr, collectionResult.createdCollectionNr, foundCollectionWordItem_, myWordItem_, NULL ).result != RESULT_OK )
									return myWordItem_->addCollectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", collectionWordItem->anyWordTypeString(), "\"" );

								hasCreatedCollection_ = true;
								foundCollectionWordItem_ = NULL;
								}
							}
						}
					}
				}
			while( !hasCreatedCollection_ &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		collectionResult.foundGeneralizationWordItem = foundGeneralizationWordItem_;
		return collectionResult;
		}

	ResultType addCollectionByGeneralization( bool isExclusiveGeneralization, bool isQuestion, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		CollectionResultType collectionResult;
		unsigned int collectionNr;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addCollectionByGeneralization";

		foundGeneralizationWordItem_ = NULL;

		if( collectionWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given collection word item is undefined" );

		if( !hasCreatedCollection_ &&
		foundGeneralizationWordItem_ == NULL &&
		( currentGeneralizationItem = myWordItem_->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

				if( currentGeneralizationWordItem != collectionWordItem )
					{
					if( ( collectionResult = currentGeneralizationWordItem->addCollectionByGeneralization( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem ) ).result != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to add a specification collection by exclusive specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

					foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem;

					if( foundGeneralizationWordItem != NULL &&
					( collectionNr = collectionWordItem->collectionNr( collectionWordTypeNr, currentGeneralizationWordItem ) ) > NO_COLLECTION_NR )
						{
						// Collect by generalization
						if( foundGeneralizationWordItem->collectGeneralizationsOrSpecifications( isExclusiveGeneralization, false, isQuestion, collectionNr ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to collect generalizations and specifications in word \"", foundGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( collectionResult.foundGeneralizationWordItem == NULL &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"They do not fear bad news;
 *	they confidently trust the Lord to take care of them.
 *	They are confident and fearless
 *	and can face their foes triumphantly." (Psalm 112:7-8)
 *************************************************************************/
