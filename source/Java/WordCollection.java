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

class WordCollection
	{
	// Private constructed variables

	private boolean hasCreatedCollection_;

	private WordItem foundCollectionWordItem_;
	private WordItem foundGeneralizationWordItem_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Constructor

	protected WordCollection( WordItem myWordItem )
		{
		String errorString = null;

		hasCreatedCollection_ = false;

		foundCollectionWordItem_ = null;
		foundGeneralizationWordItem_ = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected CollectionResultType addCollection( boolean isExclusiveSpecification, boolean isSpecificationGeneralization, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		boolean hasFoundCollection = false;
		boolean isDuplicateCollection = false;
		short collectionOrderNr;
		short foundCollectionWordTypeNr = Constants.NO_WORD_TYPE_NR;
		int foundCollectionNr = Constants.NO_COLLECTION_NR;

		foundCollectionWordItem_ = null;

		if( collectionWordItem == null )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given collected word is undefined" );

		if( commonWordItem == null )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given common word is undefined" );

		if( collectionWordItem == myWordItem_ )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given collected word is the same as my word" );

		if( collectionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL )
			collectionWordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;

		if( !myWordItem_.hasWordType( true, collectionWordTypeNr ) )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "I don't have the requested word type number: " + collectionWordTypeNr );

		if( !collectionWordItem.hasWordType( true, collectionWordTypeNr ) )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The collection word item doesn't have the requested word type number: " + collectionWordTypeNr );

		if( collectionNr == Constants.NO_COLLECTION_NR )
			{
			if( ( collectionNr = myWordItem_.highestCollectionNrInAllWords() ) >= Constants.MAX_COLLECTION_NR )
				return myWordItem_.startCollectionResultSystemErrorInWord( 1, moduleNameString_, "Collection number overflow" );

			collectionResult.createdCollectionNr = ++collectionNr;
			}

		foundCollectionNr = myWordItem_.collectionNr( collectionWordTypeNr, commonWordItem );

		if( foundCollectionNr > Constants.NO_COLLECTION_NR &&
		foundCollectionNr != collectionNr )
			{
			if( isSpecificationGeneralization )
				isDuplicateCollection = true;
			else
				{
				// Skip if Spanish ambiguous
				if( commonWordItem != compoundGeneralizationWordItem &&
				!commonWordItem.isNounWordSpanishAmbiguous() )
					{
					// Detected semantic ambiguity of the specification word
					if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_DUE_TO_SPECIFICATION_START, commonWordItem.wordTypeString( true, commonWordTypeNr ), Constants.INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_DUE_TO_SPECIFICATION_WORD, myWordItem_.wordTypeString( true, foundCollectionWordTypeNr ), Constants.INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_DUE_TO_SPECIFICATION_END ) != Constants.RESULT_OK )
						return myWordItem_.addCollectionResultErrorInWord( 1, moduleNameString_, "I failed to write the 'ambiguous due to' interface notification" );

					collectionResult.isAmbiguousCollection = true;
					}
				}
			}

		if( !isDuplicateCollection )
			{
			if( myWordItem_.collectionList == null )
				{
				// Create list
				if( ( myWordItem_.collectionList = new CollectionList( myWordItem_ ) ) == null )
					return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "I failed to create a collection list" );

				myWordItem_.wordListArray[Constants.WORD_COLLECTION_LIST] = myWordItem_.collectionList;
				}
			else
				{
				if( collectionResult.createdCollectionNr == Constants.NO_COLLECTION_NR )
					// Check if collection already exists
					hasFoundCollection = myWordItem_.collectionList.hasCollection( collectionNr, collectionWordItem, commonWordItem );
				}

			if( !hasFoundCollection )
				{
				// A collection comes in pairs
				if( ( collectionOrderNr = myWordItem_.highestCollectionOrderNrInAllWords( collectionNr ) ) >= Constants.MAX_ORDER_NR - 1 )
					return myWordItem_.startCollectionResultSystemErrorInWord( 1, moduleNameString_, "Collection order number overflow" );

				collectionResult.result = myWordItem_.collectionList.createCollectionItem( isExclusiveSpecification, ++collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem );
				}
			}

		return collectionResult;
		}

	protected CollectionResultType addCollectionByGeneralization( short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;

		hasCreatedCollection_ = false;

		if( generalizationWordItem == null )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == myWordItem_ &&
		!myWordItem_.isNounWordSpanishAmbiguous() )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given generalization word item is the same as my word" );

		if( collectionWordItem == null )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given collected word item is undefined" );

		if( collectionWordItem == myWordItem_ )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given collected word item is the same as my word" );

		if( !myWordItem_.isNounWordType( collectionWordTypeNr ) )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given collected word type isn't a noun" );

		if( !myWordItem_.isNounWordType( commonWordTypeNr ) )
			return myWordItem_.startCollectionResultErrorInWord( 1, moduleNameString_, "The given common word type isn't a noun" );

		if( ( currentSpecificationItem = myWordItem_.firstNonQuestionSpecificationItem() ) != null )
			{
			do	{
				if( currentSpecificationItem.isExclusiveSpecification() &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) == generalizationWordItem )
					{
					if( foundCollectionWordItem_ == null )
						{
						foundCollectionWordItem_ = collectionWordItem;
						foundGeneralizationWordItem_ = currentSpecificationWordItem;
						}
					else
						{
						if( foundCollectionWordItem_ != collectionWordItem )
							{
							if( ( collectionResult = foundCollectionWordItem_.findCollection( false, collectionWordItem, myWordItem_ ) ).result != Constants.RESULT_OK )
								return myWordItem_.addCollectionResultErrorInWord( 1, moduleNameString_, "I failed to find out if word \"" + collectionWordItem.anyWordTypeString() + "\" is collected with word \"" + foundCollectionWordItem_.anyWordTypeString() + "\"" );

							if( !collectionResult.isCollected )
								{
								if( ( collectionResult = foundCollectionWordItem_.addCollection( false, false, collectionWordTypeNr, commonWordTypeNr, Constants.NO_COLLECTION_NR, collectionWordItem, myWordItem_, null ) ).result != Constants.RESULT_OK )
									return myWordItem_.addCollectionResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + collectionWordItem.anyWordTypeString() + "\"" );

								if( collectionWordItem.addCollection( false, false, collectionWordTypeNr, commonWordTypeNr, collectionResult.createdCollectionNr, foundCollectionWordItem_, myWordItem_, null ).result != Constants.RESULT_OK )
									return myWordItem_.addCollectionResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + collectionWordItem.anyWordTypeString() + "\"" );

								hasCreatedCollection_ = true;
								foundCollectionWordItem_ = null;
								}
							}
						}
					}
				}
			while( !hasCreatedCollection_ &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		collectionResult.foundGeneralizationWordItem = foundGeneralizationWordItem_;
		return collectionResult;
		}

	protected byte addCollectionByGeneralization( boolean isExclusiveGeneralization, boolean isQuestion, short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		int collectionNr;
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem foundGeneralizationWordItem;

		foundGeneralizationWordItem_ = null;

		if( collectionWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given collection word item is undefined" );

		if( !hasCreatedCollection_ &&
		foundGeneralizationWordItem_ == null &&
		( currentGeneralizationItem = myWordItem_.firstNounSpecificationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "I have found an undefined generalization word" );

				if( currentGeneralizationWordItem != collectionWordItem )
					{
					if( ( collectionResult = currentGeneralizationWordItem.addCollectionByGeneralization( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem ) ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to add a specification collection by exclusive specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

					foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem;

					if( foundGeneralizationWordItem != null &&
					( collectionNr = collectionWordItem.collectionNr( collectionWordTypeNr, currentGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
						{
						// Collect by generalization
						if( foundGeneralizationWordItem.collectGeneralizationsOrSpecifications( isExclusiveGeneralization, false, isQuestion, collectionNr ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to collect generalizations and specifications in word \"" + foundGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( collectionResult.foundGeneralizationWordItem == null &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"They do not fear bad news;
 *	they confidently trust the Lord to take care of them.
 *	They are confident and fearless
 *	and can face their foes triumphantly." (Psalm 112:7-8)
 *************************************************************************/
