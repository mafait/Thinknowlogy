/*	Class:			AdminReadSentence
 *	Supports class:	AdminItem
 *	Purpose:		To read and analyze sentences
 *	Version:		Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

#include "AdminItem.h"
#include "ReadList.cpp"
#include "SelectionList.cpp"
#include "WordEndingResultType.cpp"
#include "WordTypeItem.cpp"

class AdminReadSentence
	{
	friend class AdminItem;

	// Private constructed variables

		// Private constructed variables

	bool hasAnyChangeBeenMadeByThisSentence_ = false;
	bool hasFemaleUserSpecificationWord_ = false;
	bool isAssignment_ = false;
	bool isInactiveAssignment_ = false;
	bool isArchivedAssignment_ = false;
	bool isCharacteristicFor_ = false;
	bool isChineseCurrentLanguage_ = false;
	bool isConditional_ = false;
	bool isEveryGeneralization_ = false;
	bool isExclusiveSpecification_ = false;
	bool isNegative_ = false;
	bool isPartOf_ = false;
	bool isPossessive_ = false;
	bool isSpecific_ = false;
	bool isSpecificationGeneralization_ = false;
	bool isUncountableGeneralizationNoun_ = false;
	bool isUniqueUserRelation_ = false;
	bool isUserImperativeSentence_ = false;
	bool wasPreviousCommandUndoOrRedo_ = false;

	unsigned short currentParseWordOrderNr_ = NO_ORDER_NR;
	unsigned short prepositionParameter_ = NO_PREPOSITION_PARAMETER;
	unsigned short previousGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
	unsigned short questionParameter_ = NO_QUESTION_PARAMETER;
	unsigned short selectionListNr_ = NO_LIST_NR;
	unsigned short userAssumptionLevel_ = NO_ASSUMPTION_LEVEL;

	unsigned int generalizationContextNr_ = NO_CONTEXT_NR;
	unsigned int specificationContextNr_ = NO_CONTEXT_NR;

	ReadItem *currentReadItem_ = NULL;
	ReadItem *linkedGeneralizationReadItem_ = NULL;
	ReadItem *startGeneralizationReadItem_ = NULL;
	ReadItem *endGeneralizationReadItem_ = NULL;
	ReadItem *startSpecificationReadItem_ = NULL;
	ReadItem *endSpecificationReadItem_ = NULL;
	ReadItem *startRelationReadItem_ = NULL;
	ReadItem *endRelationReadItem_ = NULL;

	WordItem *previousGeneralizationWordItem_ = NULL;

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminReadSentence";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


	// Private functions

	void checkForChangesMadeByThisSentence()
		{
		unsigned int currentSentenceNr = globalVariables_->currentSentenceNr;
		unsigned int highestFoundSentenceNr = adminItem_->highestFoundSentenceNr( false, false, currentSentenceNr );

		hasAnyChangeBeenMadeByThisSentence_ = ( highestFoundSentenceNr >= currentSentenceNr );
		}

	void clearReplacingInfoInSpecificationWords()
		{
		WordItem *currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL )
			{
			// Do for all specification words
			do	currentSpecificationWordItem->clearReplacingInfoInWord();
			while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
			}
		}

	void setVariables( unsigned short wordParameter )
		{
		switch( wordParameter )
			{
			case WORD_PARAMETER_ADJECTIVE_NO:
			case WORD_PARAMETER_ADVERB_NOT:
			case WORD_PARAMETER_ADVERB_CHINESE_NEVER_AGAIN:
			case WORD_PARAMETER_ADVERB_FRENCH_PAS:
				isNegative_ = true;
				break;

			case WORD_PARAMETER_ADJECTIVE_CHARACTERISTIC:
				isCharacteristicFor_ = true;
				break;

			case WORD_PARAMETER_ADJECTIVE_SPECIFIC:
				isSpecific_ = true;
				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
				userAssumptionLevel_ = 3;
				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
				userAssumptionLevel_ = 2;
				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				userAssumptionLevel_ = 1;
				break;

			case WORD_PARAMETER_ADVERB_ANYMORE:
				isUniqueUserRelation_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_WAS:
			case WORD_PARAMETER_PLURAL_VERB_WERE:
				isAssignment_ = true;
				isArchivedAssignment_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_CAN_BE:
			case WORD_PARAMETER_PLURAL_VERB_CAN_BE:
				isConditional_ = true;
				break;

			case WORD_PARAMETER_PREPOSITION_OF:
				if( !isAssignment_ &&
				startSpecificationReadItem_ == NULL )
					// Typical for Chinese
					isPossessive_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAS:
			case WORD_PARAMETER_PLURAL_VERB_HAVE:
				isPossessive_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAD:
			case WORD_PARAMETER_PLURAL_VERB_HAD:
				isAssignment_ = true;
				isArchivedAssignment_ = true;
				isPossessive_ = true;
			}
		}

	signed char assignLanguage( unsigned short newLanguageNr )
		{
		WordItem *languageWordItem;
		WordItem *predefinedNounLanguageWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignLanguage";

		if( ( predefinedNounLanguageWordItem = globalVariables_->predefinedNounLanguageWordItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The predefined noun language word item is undefined" );

		if( ( languageWordItem = predefinedNounLanguageWordItem->languageWordItem( newLanguageNr ) ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the requested language" );

		globalVariables_->currentLanguageNr = newLanguageNr;
		globalVariables_->currentLanguageWordItem = languageWordItem;

		if( adminItem_->assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, languageWordItem, globalVariables_->predefinedNounLanguageWordItem, NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language with authorization" );

		return RESULT_OK;
		}

	signed char addMultipleWord( unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, ReadItem *firstReadItem, ReadItem *secondReadItem )
		{
		bool hasCurrentWordWithSameWordType = true;
		bool hasFoundFrenchPreposition = false;
		bool hasFoundWordWithDifferentWordType = false;
		bool isFirstWordTypeNumeral = false;
		bool isFrenchPreposition;
		unsigned short currentWordTypeNr;
		unsigned short firstWordTypeNr;
		unsigned short nWordParts = 1;
		ReadItem *deletedReadItem;
		ReadItem *currentReadItem = secondReadItem;
		ReadItem *lastReadItem = NULL;
		WordItem *createdMultipleWordItem;
		WordItem *createdWordItem;
		WordItem *currentWordItem;
		WordItem *firstWordItem;
		WordItem *multipleWordItem;
		WordTypeItem *currentWordTypeItem;
		WordTypeItem *differentWordTypeItem;
		WordTypeItem *firstWordTypeItem;
		WordTypeItem *foundWordTypeItem;
		char existingMultipleWordString[SENTENCE_STRING_LENGTH];
		char multipleWordString[SENTENCE_STRING_LENGTH];
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addMultipleWord";

		if( firstReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first read item is undefined" );

		if( secondReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given second read item is undefined" );

		if( ( firstWordItem = firstReadItem->readWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first read item has no read word" );

		firstWordTypeNr = firstReadItem->wordTypeNr();

		if( firstWordTypeNr == WORD_TYPE_NUMERAL )
			isFirstWordTypeNumeral = true;

		if( ( firstWordTypeItem = firstWordItem->activeWordTypeItem( firstWordTypeNr ) ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the word type item of the given first read item" );

		if( adjectiveParameter == NO_ADJECTIVE_PARAMETER )
			adjectiveParameter = firstWordTypeItem->adjectiveParameter();

		if( definiteArticleParameter == NO_DEFINITE_ARTICLE_PARAMETER )
			definiteArticleParameter = firstWordTypeItem->definiteArticleParameter();

		if( indefiniteArticleParameter == NO_INDEFINITE_ARTICLE_PARAMETER )
			indefiniteArticleParameter = firstWordTypeItem->indefiniteArticleParameter();

		strcpy( multipleWordString, firstWordTypeItem->itemString() );
		strcpy( existingMultipleWordString, multipleWordString );

		do	{
			isFrenchPreposition = currentReadItem->isFrenchPreposition();

			if( isFrenchPreposition )
				hasFoundFrenchPreposition = true;

			currentWordTypeNr = currentReadItem->wordTypeNr();

			// The current word has the same word type as the first word
			if( isFirstWordTypeNumeral ||
			isFrenchPreposition ||
			firstWordTypeNr == currentWordTypeNr ||

			// Allow mix of singular and plural nouns
			( adminItem_->isNounWordType( firstWordTypeNr ) &&
			adminItem_->isNounWordType( currentWordTypeNr ) ) )
				{
				if( ( currentWordItem = currentReadItem->readWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I found a read item without read word" );

				if( ( currentWordTypeItem = currentWordItem->activeWordTypeItem( currentWordTypeNr ) ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the word type item of the current read word" );

				nWordParts++;
				lastReadItem = currentReadItem;

				if( !isChineseCurrentLanguage_ ||
				// Typical for Chinese: Only spaces in multiple proper noun words
				currentWordTypeNr == WORD_TYPE_PROPER_NOUN )
					{
					strcat( multipleWordString, SPACE_STRING );
					strcat( existingMultipleWordString, SPACE_STRING );
					}

				strcat( multipleWordString, currentWordTypeItem->itemString() );

				if( firstWordTypeNr != currentWordTypeNr &&
				( differentWordTypeItem = currentWordItem->activeWordTypeItem( firstWordTypeNr ) ) != NULL )
					{
					hasFoundWordWithDifferentWordType = true;
					strcat( existingMultipleWordString, differentWordTypeItem->itemString() );
					}
				else
					strcat( existingMultipleWordString, currentWordTypeItem->itemString() );
				}
			else
				hasCurrentWordWithSameWordType = false;
			}
		while( hasCurrentWordWithSameWordType &&
		( currentReadItem = currentReadItem->nextReadItem() ) != NULL &&
		!currentReadItem->isSymbol() );

		currentReadItem = secondReadItem;

		do	{
			currentWordTypeNr = ( hasFoundFrenchPreposition ? firstWordTypeNr : currentReadItem->wordTypeNr() );

			if( ( currentWordItem = currentReadItem->readWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "I found a read item without read word" );

			createdMultipleWordItem = NULL;
			createdWordItem = NULL;
			multipleWordItem = NULL;

			if( hasFoundWordWithDifferentWordType )
				{
				if( ( wordResult = adminItem_->findWordTypeInAllWords( false, firstWordTypeNr, existingMultipleWordString, NULL ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find an exact word type in all words" );

				if( ( createdWordItem = wordResult.foundWordItem ) != NULL )
					{
					if( createdWordItem->addWordType( false, firstWordTypeItem->isProperNounPrecededByDefiniteArticle( firstWordTypeItem->definiteArticleParameter() ), adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, currentWordTypeNr, strlen( multipleWordString ), multipleWordString ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the word type to multiple word \"", createdWordItem->anyWordTypeString(), "\"" );

					createdMultipleWordItem = createdWordItem;
					multipleWordItem = createdWordItem;
					}
				}

			if( createdWordItem == NULL )
				{
				if( ( wordResult = adminItem_->addWord( false, true, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, firstReadItem->wordParameter(), ( isFirstWordTypeNumeral ? firstWordTypeNr : currentWordTypeNr ), strlen( multipleWordString ), multipleWordString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the multiple word" );

				createdMultipleWordItem = wordResult.createdWordItem;
				multipleWordItem = ( createdMultipleWordItem == NULL ? wordResult.foundWordItem : createdMultipleWordItem );
				foundWordTypeItem = wordResult.foundWordTypeItem;

				if( foundWordTypeItem != NULL )
					{
					if( adjectiveParameter > NO_ADJECTIVE_PARAMETER &&
					!foundWordTypeItem->hasAdjectiveParameter() &&
					// Set adjective parameter
					foundWordTypeItem->setAdjectiveParameter( adjectiveParameter ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the adjective parameter of a singular noun" );

					if( definiteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER &&
					!foundWordTypeItem->hasDefiniteArticleParameter() &&
					// Set definite article parameter
					foundWordTypeItem->setDefiniteArticleParameter( definiteArticleParameter ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the definite article parameter of a singular noun" );

					if( indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
					!foundWordTypeItem->hasIndefiniteArticleParameter() &&
					// Set indefinite article parameter
					foundWordTypeItem->setIndefiniteArticleParameter( indefiniteArticleParameter ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the indefinite article parameter of a singular noun" );
					}
				}

			if( createdMultipleWordItem != NULL )
				{
				// Link both words to the created multiple word
				if( firstWordItem->addMultipleWord( nWordParts, firstWordTypeNr, createdMultipleWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the first multiple word" );

				if( ( currentWordItem = currentReadItem->readWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The current word item is undefined" );

				if( currentWordItem->addMultipleWord( nWordParts, firstWordTypeNr, createdMultipleWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the second multiple word" );
				}

			if( multipleWordItem == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The multiple word is undefined" );

			if( firstReadItem->changeReadWord( ( isFirstWordTypeNumeral ? firstWordTypeNr : currentWordTypeNr ), multipleWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an active read item" );

			deletedReadItem = currentReadItem;
			currentReadItem = ( deletedReadItem == lastReadItem ? NULL : currentReadItem->nextReadItem() );

			if( adminItem_->deleteReadItem( deletedReadItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the second read item" );
			}
		while( currentReadItem != NULL );

		return RESULT_OK;
		}

	signed char addReadSpecification( bool isAction, bool isNewStart, unsigned short selectionLevel, char *readUserSentenceString )
		{
		bool isConditional;
		bool isSelection = ( selectionListNr_ != NO_LIST_NR );
		unsigned short tempVerbParameter;
		unsigned short imperativeVerbParameter = NO_IMPERATIVE_PARAMETER;
		ReadItem *currentGeneralizationReadItem = startGeneralizationReadItem_;
		SpecificationItem *userSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addReadSpecification";

		if( currentGeneralizationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "The start generalization read item is undefined" );

		if( endGeneralizationReadItem_ == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "The end generalization read item is undefined" );

		if( !isSelection &&
		globalVariables_->nUserGeneralizationWords == 1 &&
		globalVariables_->nUserSpecificationWords > 1 &&
		globalVariables_->nUserRelationWords > 1 )
			{
			// Ambiguity: Missing relation context
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MULTIPLE_SPECIFICATIONS_RELATIONS ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to write an interface warning about ambiguity" );
			}
		else
			{
			if( wasPreviousCommandUndoOrRedo_ )
				{
				wasPreviousCommandUndoOrRedo_ = false;
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				}

			isConditional = ( isSelection ||
							isConditional_ );

			adminItem_->initializeAdminVariables();

			do	{
				switch( currentGeneralizationReadItem->grammarParameter )
					{
					case GRAMMAR_IMPERATIVE:
						// Typical for Chinese
						if( isChineseCurrentLanguage_ &&
						( tempVerbParameter = currentGeneralizationReadItem->readAheadChineseImperativeVerbParameter() ) > NO_WORD_PARAMETER )
							imperativeVerbParameter = tempVerbParameter;
						else
							imperativeVerbParameter = currentGeneralizationReadItem->wordParameter();
						// Don't insert a break statement

					case GRAMMAR_GENERALIZATION_WORD:
						if( adminItem_->addUserSpecification( isAction, isAssignment_, isCharacteristicFor_, isConditional, isInactiveAssignment_, isArchivedAssignment_, isEveryGeneralization_, isExclusiveSpecification_, isNegative_, isNewStart, isPartOf_, isPossessive_, isSpecific_, isSpecificationGeneralization_, isUncountableGeneralizationNoun_, isUniqueUserRelation_, imperativeVerbParameter, prepositionParameter_, questionParameter_, selectionLevel, selectionListNr_, userAssumptionLevel_, generalizationContextNr_, specificationContextNr_, currentGeneralizationReadItem, startSpecificationReadItem_, endSpecificationReadItem_, startRelationReadItem_, endRelationReadItem_ ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to add the read user specifications" );

						if( !isSelection &&
						( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
						adminItem_->prepareIntegrityCheckOfStoredUserSentence( isChineseCurrentLanguage_, userSpecificationItem, readUserSentenceString ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to prepare the integrity check of the stored user sentence" );
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			currentGeneralizationReadItem != endGeneralizationReadItem_ &&
			( currentGeneralizationReadItem = currentGeneralizationReadItem->nextReadItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char checkAllWordsForStructuralIntegrity()
		{
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForStructuralIntegrity";

		if( adminItem_->checkForUnprocessedNegativeConclusion() == RESULT_OK &&
		( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem->checkStructuralIntegrityInWord() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the structural integrity in word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( !globalVariables_->hasDisplayedIntegrityWarning &&
			( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char checkForDuplicateSelection()
		{
		bool hasFoundDuplicateSelection = false;
		unsigned int duplicateConditionSentenceNr;
		DuplicateResultType duplicateResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateSelection";

		if( ( duplicateResult = adminItem_->checkForDuplicateCondition() ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check if the condition selection part is duplicate" );

		if( ( duplicateConditionSentenceNr = duplicateResult.duplicateConditionSentenceNr ) > NO_SENTENCE_NR )
			{
			if( ( duplicateResult = adminItem_->checkForDuplicateSelectionPart( true, duplicateConditionSentenceNr ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check if the action selection part is duplicate" );

			if( duplicateResult.hasFoundDuplicateSelection )
				{
				if( globalVariables_->adminAlternativeList == NULL )
					hasFoundDuplicateSelection = true;
				else
					{
					if( ( duplicateResult = adminItem_->checkForDuplicateSelectionPart( false, duplicateConditionSentenceNr ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );

					if( duplicateResult.hasFoundDuplicateSelection )
						hasFoundDuplicateSelection = true;
					}
				}

			if( hasFoundDuplicateSelection )
				adminItem_->deleteSentences( globalVariables_->currentSentenceNr );
			}

		return RESULT_OK;
		}

	signed char deleteAllWordTypesOfCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAllWordTypesOfCurrentSentence";

		if( deleteAllWordTypesOfCurrentSentence( true ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all word types of the active read list" );

		if( deleteAllWordTypesOfCurrentSentence( false ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all word types of the inactive read list" );

		return RESULT_OK;
		}

	signed char deleteAllWordTypesOfCurrentSentence( bool isActiveItems )
		{
		ReadItem *currentReadItem = ( isActiveItems ? adminItem_->firstActiveReadItem() : adminItem_->firstInactiveReadItem() );
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAllWordTypesOfCurrentSentence";

		while( currentReadItem != NULL )
			{
			currentWordItem = currentReadItem->readWordItem();

			// Skip text
			if( currentWordItem != NULL &&
			!currentWordItem->isDeletedItem() )
				{
				if( currentWordItem->deleteAllWordTypesOfCurrentSentence() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all word types of the current sentence in word \"", currentWordItem->anyWordTypeString(), "\"" );

				if( !currentWordItem->hasAnyWordType() &&
				adminItem_->deleteWordItem( currentWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete word \"", currentWordItem->anyWordTypeString(), "\"" );
				}

			currentReadItem = currentReadItem->nextReadItem();
			}

		return RESULT_OK;
		}

	signed char deleteUnusedWordsAndWordTypes()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteUnusedWordsAndWordTypes";

		// Active read items
		if( deleteUnusedWordsAndWordTypes( true ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete unused word types in the active read list" );

		// Inactive read items
		if( deleteUnusedWordsAndWordTypes( false ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete unused word types in the inactive read list" );

		return RESULT_OK;
		}

	signed char deleteUnusedWordsAndWordTypes( bool isActiveItems )
		{
		bool isSameWordOrderNr;
		unsigned short nReadWordReferences;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		WordTypeItem *currentWordTypeItem;
		ReadItem *currentReadItem = ( isActiveItems ? adminItem_->firstActiveReadItem() : adminItem_->firstInactiveReadItem() );
		WordItem *currentReadWordItem;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteUnusedWordsAndWordTypes";

		while( currentReadItem != NULL )
			{
			isSameWordOrderNr = ( currentReadItem->wordOrderNr() == previousWordOrderNr );
			previousWordOrderNr = currentReadItem->wordOrderNr();

			// Skip text
			if( ( currentReadWordItem = currentReadItem->readWordItem() ) != NULL )
				{
				if( !isActiveItems ||

				// More word types for this word number
				( isSameWordOrderNr &&
				!currentReadItem->isSingularNoun() ) )
					{
					if( ( currentWordTypeItem = currentReadItem->activeReadWordTypeItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the word type of an active read word" );

					if( currentWordTypeItem->hasCurrentCreationSentenceNr() )
						{
						if( ( shortResult = adminItem_->getNumberOfReadWordReferences( currentReadItem->wordTypeNr(), currentReadWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the number of read word references" );

						if( ( nReadWordReferences = shortResult.shortValue ) < 1 )
							return adminItem_->startError( functionNameString, moduleNameString_, "I found an invalid number of read word references" );

						if( nReadWordReferences == 1 )
							{
							if( currentReadWordItem->deleteWordType( currentWordTypeItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an unused word type item" );

							if( !currentReadWordItem->hasAnyWordType() &&
							adminItem_->deleteWordItem( currentReadWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an unused word item" );
							}
						}
					}

				if( !isActiveItems ||
				isSameWordOrderNr )
					{
					if( adminItem_->deleteReadItem( currentReadItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an active read item" );

					currentReadItem = adminItem_->nextReadListItem();
					}
				else
					currentReadItem = currentReadItem->nextReadItem();
				}
			else
				currentReadItem = currentReadItem->nextReadItem();
			}

		return RESULT_OK;
		}

	signed char findGrammarPath( unsigned short grammarLevel, unsigned short lastCreatedWordOrderNr, GrammarItem *parseGrammarItem )
		{
		bool isChoice;
		bool isOption;
		bool isWaitingForChoiceEnd;
		bool isWaitingForNewStart;
		unsigned short choiceStartWordOrderNr = NO_ORDER_NR;
		unsigned short previousWordOrderNr;
		unsigned short startWordOrderNr;
		GrammarItem *definitionParseGrammarItem;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findGrammarPath";

		if( grammarLevel >= MAX_GRAMMAR_LEVEL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given grammar level is too high: #", grammarLevel );

		if( parseGrammarItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given parse grammar item is undefined" );

		if( grammarLevel == NO_GRAMMAR_LEVEL )
			currentParseWordOrderNr_ = NO_ORDER_NR;

		startWordOrderNr = currentParseWordOrderNr_;

		do	{
			isChoice = false;
			isOption = false;
			isWaitingForChoiceEnd = false;
			isWaitingForNewStart = true;

			currentParseWordOrderNr_ = startWordOrderNr;
			previousWordOrderNr = startWordOrderNr;

			definitionParseGrammarItem = parseGrammarItem;

			do	{
				if( isWaitingForNewStart &&
				// Skip first grammar definition item, if not is a data item
				parseGrammarItem->isNewStart() )
					isWaitingForNewStart = false;

				if( !isWaitingForNewStart &&
				!isWaitingForChoiceEnd )
					{
					previousWordOrderNr = currentParseWordOrderNr_;

					if( parseGrammarItem->isOptionStart() )
						isOption = true;

					if( parseGrammarItem->isChoiceStart() )
						{
						isChoice = true;
						choiceStartWordOrderNr = currentParseWordOrderNr_;
						}

					if( parseGrammarItem->isDefinitionStart() )
						{
						if( ( boolResult = adminItem_->selectMatchingWordType( currentParseWordOrderNr_, definitionParseGrammarItem->grammarParameter(), definitionParseGrammarItem->grammarWordTypeNr() ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to select a matching word type" );

						// Has found matching word type
						if( boolResult.booleanValue )
							currentParseWordOrderNr_++;
						}
					else
						{
						if( grammarLevel + 1 >= MAX_GRAMMAR_LEVEL )
							return adminItem_->startError( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because I reached the grammar level: #", grammarLevel );

						if( parseGrammarItem->definitionGrammarItem == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "Grammar word \"", parseGrammarItem->grammarString(), "\" isn't defined in the grammar file" );

						if( currentParseWordOrderNr_ < lastCreatedWordOrderNr &&
						findGrammarPath( ( grammarLevel + 1 ), lastCreatedWordOrderNr, parseGrammarItem->definitionGrammarItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the grammar path at grammar word \"", parseGrammarItem->grammarString(), "\"" );

						// Unsuccessful
						if( currentParseWordOrderNr_ == previousWordOrderNr )
							{
							if( isOption )
								isWaitingForNewStart = true;

							if( isChoice )
								{
								if( !parseGrammarItem->isChoiceEnd )
									isWaitingForNewStart = true;

								currentParseWordOrderNr_ = choiceStartWordOrderNr;
								}

							if( startWordOrderNr < currentParseWordOrderNr_ &&
							adminItem_->activateInactiveReadWords( currentParseWordOrderNr_ ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to activate inactive read words from position: ", currentParseWordOrderNr_ );
							}
						}
					}

				if( parseGrammarItem->isOptionEnd )
					isOption = false;

				if( parseGrammarItem->isChoiceEnd )
					{
					isChoice = false;
					isWaitingForChoiceEnd = false;
					}

				parseGrammarItem = parseGrammarItem->nextGrammarItem();

				if( isChoice &&
				!isWaitingForChoiceEnd &&
				currentParseWordOrderNr_ > previousWordOrderNr &&
				parseGrammarItem != NULL &&
				parseGrammarItem->isNewStart() )
					isWaitingForChoiceEnd = true;
				}
			while( ( isWaitingForNewStart ||
			isWaitingForChoiceEnd ||
			currentParseWordOrderNr_ > previousWordOrderNr ) &&

			parseGrammarItem != NULL &&
			definitionParseGrammarItem->activeSentenceNr() == parseGrammarItem->activeSentenceNr() );

			if( !isWaitingForNewStart &&
			currentParseWordOrderNr_ > startWordOrderNr &&
			currentParseWordOrderNr_ == previousWordOrderNr )
				currentParseWordOrderNr_ = startWordOrderNr;
			}
		while( currentParseWordOrderNr_ == startWordOrderNr &&
		definitionParseGrammarItem != NULL &&
		( parseGrammarItem = definitionParseGrammarItem->nextDefinitionGrammarItem ) != NULL );

		if( ( previousWordOrderNr > startWordOrderNr ||
		currentParseWordOrderNr_ > startWordOrderNr ) &&

		definitionParseGrammarItem != NULL &&
		definitionParseGrammarItem->isGrammarDefinition() &&

		// Set grammar parameter
		adminItem_->setGrammarParameter( ( currentParseWordOrderNr_ > startWordOrderNr ), startWordOrderNr, ( currentParseWordOrderNr_ > startWordOrderNr ? currentParseWordOrderNr_ : previousWordOrderNr ), definitionParseGrammarItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the grammar parameter of the read words between the positions ", startWordOrderNr, " and ", currentParseWordOrderNr_ );

		return RESULT_OK;
		}

	signed char initializeVariablesInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = globalVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->initializeVariablesInWord();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char parseSentence( char *readUserSentenceString )
		{
		bool isAction = false;
		bool isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
		bool isNewStart = true;
		unsigned short selectionLevel = NO_SELECTION_LEVEL;
		unsigned short wordOrderNr;
		unsigned short wordParameter;
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "parseSentence";

		isChineseCurrentLanguage_ = isChineseCurrentLanguage;
		isUserImperativeSentence_ = false;

		selectionListNr_ = NO_LIST_NR;

		linkedGeneralizationReadItem_ = NULL;
		startGeneralizationReadItem_ = NULL;
		endGeneralizationReadItem_ = NULL;

		adminItem_->initializeLinkedWord();

		if( readUserSentenceString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( currentReadItem_ = adminItem_->firstActiveReadItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I failed to get the first read item" );

		do	{
			if( !currentReadItem_->isSeparator() )
				{
				wordOrderNr = currentReadItem_->wordOrderNr();
				wordParameter = currentReadItem_->wordParameter();

				switch( currentReadItem_->grammarParameter )
					{
					case GRAMMAR_GENERALIZATION_SPECIFICATION:
					case GRAMMAR_GENERALIZATION_PART:
					case GRAMMAR_GENERALIZATION_WORD:
					case GRAMMAR_SPECIFICATION_PART:
					case GRAMMAR_ASSIGNMENT_PART:
					case GRAMMAR_SPECIFICATION_WORD:
					case GRAMMAR_RELATION_PART:
					case GRAMMAR_RELATION_WORD:
					case GRAMMAR_GENERALIZATION_ASSIGNMENT:
					case GRAMMAR_SPECIFICATION_ASSIGNMENT:
					case GRAMMAR_RELATION_ASSIGNMENT:
					case GRAMMAR_VERB:
					case GRAMMAR_QUESTION_VERB:
					case GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
					case GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
						if( scanSpecification() != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to scan the generalization-specification" );

						if( addReadSpecification( isAction, isNewStart, selectionLevel, readUserSentenceString ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to add the read specification" );

						isNewStart = false;

						break;

					case GRAMMAR_IMPERATIVE:
						if( readImperative( isAction, isNewStart, selectionLevel ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read an imperative" );

						isNewStart = false;
						isUserImperativeSentence_ = true;

						break;

					case GRAMMAR_STATEMENT_ANSWER:
						if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to write an interface warning" );

						break;

					case GRAMMAR_SENTENCE_CONJUNCTION:
						switch( wordParameter )
							{
							case WORD_PARAMETER_SYMBOL_COMMA:
							case WORD_PARAMETER_CONJUNCTION_AND:
							// Typical for Dutch: "in zowel ... als ..."
							case WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
								break;

							case WORD_PARAMETER_CONJUNCTION_OR:
								isNewStart = true;

								break;

							default:
								return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I found an illegal conjunction word parameter: ", wordParameter );
							}

						break;

					case GRAMMAR_SELECTION:
					case WORD_PARAMETER_SELECTION_IF:	// In case "then" is missing
					case WORD_PARAMETER_SELECTION_THEN:
					case WORD_PARAMETER_SELECTION_ELSE:
						switch( wordParameter )
							{
							case WORD_PARAMETER_SELECTION_IF:
								if( selectionListNr_ != NO_LIST_NR &&
								++selectionLevel == MAX_LEVEL )
									{
									sprintf( errorString, "Selection overflow in list <%c> at position %u", adminItem_->adminListChar( selectionListNr_ ), wordOrderNr );
									return adminItem_->startSystemError( functionNameString, moduleNameString_, errorString );
									}

								isNewStart = true;
								selectionListNr_ = ADMIN_CONDITION_LIST;

								break;

							case WORD_PARAMETER_SELECTION_THEN:
								isNewStart = true;
								isAction = true;
								selectionListNr_ = ADMIN_ACTION_LIST;

								break;

							case WORD_PARAMETER_SELECTION_ELSE:
								isNewStart = true;
								isAction = false;
								selectionListNr_ = ADMIN_ALTERNATIVE_LIST;

								break;

							default:
								return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I found an illegal selection word" );
							}

						break;

					default:
						if( adminItem_->isCurrentlyTesting() ||
						adminItem_->isSystemStartingUp() )
							return adminItem_->startError( functionNameString, moduleNameString_, "I found an unknown word in sentence \"", readUserSentenceString, "\" at position ", wordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", wordParameter );

						return adminItem_->startError( functionNameString, moduleNameString_, "I found an unknown word at position ", wordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", wordParameter );
					}
				}
			}
		while( currentReadItem_ != NULL &&
		!globalVariables_->hasDisplayedWarning &&
		!adminItem_->hasRequestedRestart() &&
		( currentReadItem_ = currentReadItem_->nextReadItem() ) != NULL );

		if( selectionListNr_ != NO_LIST_NR &&
		checkForDuplicateSelection() != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check on a duplicate selection" );

		return RESULT_OK;
		}

	signed char processSentence( char *readUserSentenceString )
		{
		bool isInterpretationSuccessful;
		unsigned short currentLanguageNr;
		unsigned short lastCreatedWordOrderNr;
		unsigned short nLanguages;
		unsigned short originalLanguageNr = globalVariables_->currentLanguageNr;
		GrammarItem *readingGrammarItem;
		BoolResultType boolResult;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "processSentence";

		if( readUserSentenceString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( nLanguages = adminItem_->nLanguages() ) <= NO_LANGUAGE_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any language" );

		currentLanguageNr = originalLanguageNr;

		if( nLanguages < currentLanguageNr )
			return adminItem_->startError( functionNameString, moduleNameString_, "The current language number exceeds the number of languages" );

		do	{
			adminItem_->deleteTemporaryReadList();

			// Need to switch language
			if( currentLanguageNr != globalVariables_->currentLanguageNr &&
			assignLanguage( currentLanguageNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language" );

			// Typical for French: Merged words
			if( ( boolResult = adminItem_->expandMergedWordsInReadSentenceOfCurrentLanguage( readUserSentenceString ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to expand the merged words in the read user sentence string" );

			// Create read words from a given sentence
			if( ( boolResult = adminItem_->createReadWords( isChineseCurrentLanguage_, readUserSentenceString ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create the read words" );

			isInterpretationSuccessful = false;
			adminItem_->clearLastActivatedWordOrderNr();

			// Has created all read words
			if( boolResult.booleanValue )
				{
				lastCreatedWordOrderNr = adminItem_->lastCreatedWordOrderNr();
				readingGrammarItem = adminItem_->firstCurrentLanguageReadingGrammarItem();

				do	{
					if( findGrammarPath( NO_GRAMMAR_LEVEL, lastCreatedWordOrderNr, readingGrammarItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the grammar path for a sentence with language \"", adminItem_->languageNameString( currentLanguageNr ), "\"" );

					isInterpretationSuccessful = ( currentParseWordOrderNr_ == lastCreatedWordOrderNr );

					if( !isInterpretationSuccessful &&
					( boolResult = adminItem_->findMoreInterpretations() ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find more interpretations" );
					}
				while( !isInterpretationSuccessful &&
				// Has found another interpretation
				boolResult.booleanValue );
				}

			if( isInterpretationSuccessful )
				{
				if( deleteUnusedWordsAndWordTypes() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the unused words and word types of the read words" );

				if( parseSentence( readUserSentenceString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to parse the sentence" );
				}
			else
				{
				if( deleteAllWordTypesOfCurrentSentence() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete words and word types of the read words" );

				// Interpretation not successful. Try other languages
				if( nLanguages <= 1 )
					// The only language
					currentLanguageNr++;
				else
					{
					// Original language not successful
					if( currentLanguageNr == originalLanguageNr )
						// Start from first language
						currentLanguageNr = 1;
					else
						currentLanguageNr++;

					// Skip original language (already tested)
					if( currentLanguageNr == originalLanguageNr )
						currentLanguageNr++;
					}
				}
			}
		while( !isInterpretationSuccessful &&
		currentLanguageNr <= nLanguages );

		// Failed to interpret sentence

		if( !isInterpretationSuccessful )
			{
			if( wasPreviousCommandUndoOrRedo_ )
				{
				wasPreviousCommandUndoOrRedo_ = false;
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				}

			if( globalVariables_->currentLanguageNr != originalLanguageNr &&
			// Restore the original language
			assignLanguage( originalLanguageNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language after an interpretation failure" );

			if( currentParseWordOrderNr_ == NO_ORDER_NR )
				{
				if( adminItem_->isSystemStartingUp() )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I don't understand this sentence. Please make sure you enter a sentence within my limited grammar definition" );

				if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_NOT_CONFORM_GRAMMAR ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( adminItem_->isSystemStartingUp() )
					{
					sprintf( errorString, "I don't understand the sentence from the word at position %u", currentParseWordOrderNr_ );
					return adminItem_->startSystemError( functionNameString, moduleNameString_, errorString );
					}

				if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START, (unsigned int)currentParseWordOrderNr_, INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			}

		return RESULT_OK;
		}

	signed char readImperative( bool isAction, bool isNewStart, unsigned short selectionLevel )
		{
		bool hasFoundChineseImperativeVerb = false;
		unsigned short executionNounWordParameter = NO_WORD_PARAMETER;
		unsigned short imperativeVerbParameter = NO_IMPERATIVE_PARAMETER;
		unsigned short specificationWordTypeNr;
		unsigned short tempNounWordParameter;
		unsigned short tempVerbParameter;
		ReadItem *currentReadItem;
		ReadItem *specificationReadItem;
		WordItem *imperativeVerbWordItem = NULL;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readImperative";

		startGeneralizationReadItem_ = NULL;
		endGeneralizationReadItem_ = NULL;

		if( scanSpecification() != RESULT_OK )
			return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to scan the generalization-specification" );

		if( endGeneralizationReadItem_ == NULL ||
		( currentReadItem = startGeneralizationReadItem_ ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I couldn't find the imperative" );

		do	{
			if( currentReadItem->isVerb() )
				{
				// Typical for Chinese
				if( isChineseCurrentLanguage_ &&
				( tempVerbParameter = currentReadItem->readAheadChineseImperativeVerbParameter() ) > NO_WORD_PARAMETER )
					{
					hasFoundChineseImperativeVerb = true;
					imperativeVerbParameter = tempVerbParameter;
					imperativeVerbWordItem = adminItem_->predefinedWordItem( tempVerbParameter );
					}
				else
					{
					imperativeVerbParameter = currentReadItem->wordParameter();
					imperativeVerbWordItem = currentReadItem->readWordItem();
					}
				}
			else
				{
				if( currentReadItem->isSpecificationWord() &&
				!currentReadItem->isNounHeadOrTail() &&
				( tempNounWordParameter = currentReadItem->wordParameter() ) > NO_WORD_PARAMETER )
					executionNounWordParameter = tempNounWordParameter;
				}
			}
		while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		// Imperative word has no specifications
		if( ( specificationReadItem = startSpecificationReadItem_ ) == NULL )
			{
			if( imperativeVerbWordItem != NULL &&
			imperativeVerbWordItem->isImperativeVerbUndoOrRedo() )
				wasPreviousCommandUndoOrRedo_ = true;
			else
				{
				wasPreviousCommandUndoOrRedo_ = false;
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				}

			if( adminItem_->executeImperative( true, NO_LIST_NR, NO_WORD_PARAMETER, startGeneralizationReadItem_->wordParameter(), NO_WORD_TYPE_NR, MAX_PROGRESS, startGeneralizationReadItem_->readString, startGeneralizationReadItem_->readWordItem(), NULL, startRelationReadItem_, endRelationReadItem_, NULL, NULL ) != RESULT_OK )
				{
				if( startGeneralizationReadItem_ == NULL )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute the single imperative" );

				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute the single imperative at position ", startGeneralizationReadItem_->wordOrderNr() );
				}
			}
		else
			{
			// Imperative word has specifications
			if( endSpecificationReadItem_ == NULL )
				return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "The end specification read item is undefined" );

			if( addReadSpecification( isAction, isNewStart, selectionLevel, NULL ) != RESULT_OK )
				{
				if( startGeneralizationReadItem_ == NULL ||
				endGeneralizationReadItem_ == NULL )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the generalization part of the read specification" );

				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the generalization part of the read specification between the positions ", startGeneralizationReadItem_->wordOrderNr(), " and ", endGeneralizationReadItem_->wordOrderNr() );
				}

			if( !globalVariables_->hasDisplayedWarning &&
			selectionListNr_ == NO_LIST_NR )
				{
				do	{
					if( specificationReadItem->isImperativeNoun() )
						{
						specificationWordTypeNr = specificationReadItem->wordTypeNr();

						if( ( !hasFoundChineseImperativeVerb ||
						// Typical for Chinese
						specificationWordTypeNr != WORD_TYPE_NOUN_SINGULAR ) &&

						// Execute imperative
						adminItem_->executeImperative( true, NO_LIST_NR, executionNounWordParameter, imperativeVerbParameter, specificationWordTypeNr, MAX_PROGRESS, specificationReadItem->readString, imperativeVerbWordItem, specificationReadItem->readWordItem(), startRelationReadItem_, endRelationReadItem_, NULL, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute an imperative with specifications" );
						}
					}
				while( startGeneralizationReadItem_ == endGeneralizationReadItem_ &&
				// Loop shouldn't end if virtual list prepositions are used to for example display justification reports
				specificationReadItem != endSpecificationReadItem_ &&
				!specificationReadItem->isVirtualListPreposition() &&
				// Still active read items available
				adminItem_->firstActiveReadItem() != NULL &&
				!globalVariables_->hasDisplayedWarning &&
				( specificationReadItem = specificationReadItem->nextReadItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char scanSpecification()
		{
		bool hasGeneralizationArticle = false;
		bool isFrenchPreposition;
		bool isSameWordTypeAsPreviousWord;
		bool isStop = false;
		unsigned short currentWordParameter;
		unsigned short currentWordTypeNr;
		unsigned short generalizationAdjectiveParameter = NO_ADJECTIVE_PARAMETER;
		unsigned short generalizationDefiniteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		unsigned short generalizationIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		unsigned short previousWordTypeNr = NO_WORD_TYPE_NR;
		ReadItem *nextReadItem;
		ReadItem *previousReadItem = NULL;
		WordItem *currentReadWordItem;
		WordTypeItem *currentReadWordTypeItem;
		WordTypeItem *generalizationWordTypeItem;
		ContextResultType contextResult;
		WordEndingResultType wordEndingResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "scanSpecification";

		hasFemaleUserSpecificationWord_ = false;
		isAssignment_ = false;
		isInactiveAssignment_ = false;
		isArchivedAssignment_ = false;
		isCharacteristicFor_ = false;
		isConditional_ = false;
		isEveryGeneralization_ = false;
		isExclusiveSpecification_ = false;
		isNegative_ = false;
		isPartOf_ = false;
		isPossessive_ = false;
		isSpecific_ = false;
		isSpecificationGeneralization_ = false;
		isUncountableGeneralizationNoun_ = false;
		isUniqueUserRelation_ = false;

		currentParseWordOrderNr_ = NO_ORDER_NR;
		prepositionParameter_ = NO_PREPOSITION_PARAMETER;
		questionParameter_ = NO_QUESTION_PARAMETER;
		userAssumptionLevel_ = NO_ASSUMPTION_LEVEL;

		generalizationContextNr_ = NO_CONTEXT_NR;
		specificationContextNr_ = NO_CONTEXT_NR;

		startSpecificationReadItem_ = NULL;
		endSpecificationReadItem_ = NULL;
		startRelationReadItem_ = NULL;
		endRelationReadItem_ = NULL;

		globalVariables_->nUserGeneralizationWords = 0;
		globalVariables_->nUserSpecificationWords = 0;
		globalVariables_->nUserRelationWords = 0;

		if( linkedGeneralizationReadItem_ == NULL )
			{
			startGeneralizationReadItem_ = NULL;
			endGeneralizationReadItem_ = NULL;
			}

		if( currentReadItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The read item is undefined" );

		do	{
			isFrenchPreposition = currentReadItem_->isFrenchPreposition();
			currentWordParameter = currentReadItem_->wordParameter();
			currentWordTypeNr = currentReadItem_->wordTypeNr();
			currentReadWordItem = currentReadItem_->readWordItem();

			setVariables( currentWordParameter );

			isSameWordTypeAsPreviousWord = ( isFrenchPreposition ||
											previousWordTypeNr == currentWordTypeNr ||

											// Allow mix of singular and plural nouns
											( adminItem_->isNounWordType( previousWordTypeNr ) &&
											adminItem_->isNounWordType( currentWordTypeNr ) ) );

			switch( currentReadItem_->grammarParameter )
				{
				case GRAMMAR_STATEMENT:
					// Typical for Chinese
					if( isChineseCurrentLanguage_ &&
					currentReadItem_->isQuestionMark() )
						questionParameter_ = WORD_PARAMETER_SINGULAR_VERB_IS;

					break;

/*				case GRAMMAR_STATEMENT_ANSWER:
					answerParameter_ = currentReadItem_->grammarParameter;
					break;
*/
				// Assignment generalization-specification
				case GRAMMAR_GENERALIZATION_ASSIGNMENT:
					isAssignment_ = true;

					// Don't insert a break statement here

				case GRAMMAR_IMPERATIVE:
				case GRAMMAR_GENERALIZATION_SPECIFICATION:
				case GRAMMAR_GENERALIZATION_PART:
				case GRAMMAR_GENERALIZATION_WORD:
					switch( currentWordTypeNr )
						{
						case WORD_TYPE_ADJECTIVE:
							if( adminItem_->isAdjectiveParameter( currentReadItem_->wordParameter() ) )
								generalizationAdjectiveParameter = currentReadItem_->wordParameter();

							if( currentReadItem_->isAdjectivePrevious() )
								{
								isAssignment_ = true;
								isInactiveAssignment_ = true;
								}

							if( currentReadItem_->isAdjectiveEvery() )
								isEveryGeneralization_ = true;

							break;

						case WORD_TYPE_ARTICLE:
							hasGeneralizationArticle = true;

							if( currentReadItem_->isDefiniteArticleParameter( currentReadItem_->wordParameter() ) )
								generalizationDefiniteArticleParameter = currentReadItem_->wordParameter();
							else
								generalizationIndefiniteArticleParameter = currentReadItem_->wordParameter();

							break;

						case WORD_TYPE_PROPER_NOUN:
						case WORD_TYPE_NOUN_SINGULAR:
						case WORD_TYPE_NOUN_PLURAL:
							if( currentReadItem_->isNounPartOf() )
								isPartOf_ = true;

							if( currentReadItem_->isGeneralizationWord() &&
							currentReadWordItem != NULL )
								{
								if( isSameWordTypeAsPreviousWord )
									{
									if( addMultipleWord( generalizationAdjectiveParameter, generalizationDefiniteArticleParameter, generalizationIndefiniteArticleParameter, previousReadItem, currentReadItem_ ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a multiple generalization word" );

									// Current read item is deleted
									currentReadItem_ = previousReadItem;
									}
								else
									{
									previousGeneralizationWordTypeNr_ = currentWordTypeNr;
									previousGeneralizationWordItem_ = currentReadWordItem;

									globalVariables_->nUserGeneralizationWords++;
									}

								if( currentReadItem_->isUncountableGeneralizationNoun() )
									isUncountableGeneralizationNoun_ = true;

								currentReadWordItem->isUserGeneralizationWord = true;
								}

							break;

						case WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_SUBJECTIVE:
						case WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_OBJECTIVE:
						case WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR:
						case WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR:
						case WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE:
						case WORD_TYPE_PERSONAL_PRONOUN_PLURAL_OBJECTIVE:
						case WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL:
						case WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL:
							if( generalizationContextNr_ > NO_CONTEXT_NR )
								return adminItem_->startError( functionNameString, moduleNameString_, "The generalization context number is already assigned" );

							if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a generalization pronoun context" );

							generalizationContextNr_ = contextResult.contextNr;
						}

					if( startGeneralizationReadItem_ == NULL )
						startGeneralizationReadItem_ = currentReadItem_;

					endGeneralizationReadItem_ = currentReadItem_;

					break;

				case GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION:
					isStop = true;
					nextReadItem = currentReadItem_->nextReadItem();
					linkedGeneralizationReadItem_ = nextReadItem;
					break;

				case GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION:
					isExclusiveSpecification_ = true;
					break;

				case GRAMMAR_RELATION_PART:
				case GRAMMAR_RELATION_WORD:
					if( currentReadItem_->isPreposition() )
						prepositionParameter_ = currentWordParameter;

					if( currentReadItem_->isRelationWord() &&
					currentReadWordItem != NULL )
						{
						if( previousWordTypeNr != currentWordTypeNr )
							{
							if( previousWordTypeNr == WORD_TYPE_NUMERAL )
								// Multiple word type relation
								isSameWordTypeAsPreviousWord = true;
							else
								{
								// Typical for Chinese
								if( currentWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
								previousWordTypeNr == WORD_TYPE_VERB_SINGULAR )
									isUncountableGeneralizationNoun_ = true;
								}
							}

						if( isSameWordTypeAsPreviousWord )
							{
							if( addMultipleWord( NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, previousReadItem, currentReadItem_ ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a multiple relation word" );

							// Current read item is deleted
							currentReadItem_ = previousReadItem;
							}
						else
							globalVariables_->nUserRelationWords++;

						currentReadWordItem->isUserRelationWord = true;
						}

					if( startSpecificationReadItem_ == NULL )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					if( startRelationReadItem_ == NULL )
						startRelationReadItem_ = currentReadItem_;

					endRelationReadItem_ = currentReadItem_;

					// Typical for English: "... in both ... and ..."
					if( currentWordParameter == WORD_PARAMETER_NUMERAL_BOTH ||
					// Typical for Dutch: "... in zowel ... als ..."
					currentWordParameter == WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL )
						isExclusiveSpecification_ = true;

					break;

				case GRAMMAR_ASSIGNMENT_PART:
				case GRAMMAR_SPECIFICATION_ASSIGNMENT:
					isAssignment_ = true;

					if( currentReadItem_->isDeterminerOrPronoun() )
						{
						if( specificationContextNr_ > NO_CONTEXT_NR )
							return adminItem_->startError( functionNameString, moduleNameString_, "The specification context number is already assigned" );

						if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification pronoun context" );

						specificationContextNr_ = contextResult.contextNr;
						}

					// Don't insert a break statement here

				case GRAMMAR_SPECIFICATION_PART:
				case GRAMMAR_SPECIFICATION_WORD:
				case GRAMMAR_TEXT:
					if( currentReadItem_->isSpecificationWord() &&
					currentReadWordItem != NULL )
						{
						if( isSameWordTypeAsPreviousWord )
							{
							if( addMultipleWord( NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, previousReadItem, currentReadItem_ ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a multiple specification word" );

							// Current read item is deleted
							currentReadItem_ = previousReadItem;
							}
						else
							{
							if( !hasGeneralizationArticle &&
							isArchivedAssignment_ )
								{
								// Typical for Chinese
								if( isChineseCurrentLanguage_ )
									{
									if( startRelationReadItem_ != NULL )
										{
										if( ( wordEndingResult = adminItem_->analyzeWordEndingWithCurrentLanguage( WORD_CHINESE_EXCLUSIVE_NOUN, 0, currentReadWordItem->activeWordTypeString( WORD_TYPE_NOUN_SINGULAR ) ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a Chinese assignment noun" );

										if( wordEndingResult.hasFoundWordEnding )
											isUniqueUserRelation_ = true;
										}
									}
								else
									isUniqueUserRelation_ = true;
								}

							if( currentReadWordItem->isFemale() )
								hasFemaleUserSpecificationWord_ = true;

							globalVariables_->nUserSpecificationWords++;

							if( currentWordTypeNr == WORD_TYPE_NOUN_SINGULAR )
								{
								if( isChineseCurrentLanguage_ )
									{
									if( !isAssignment_ &&
									!isPossessive_ &&
									currentWordParameter == NO_WORD_PARAMETER &&
									startRelationReadItem_ != NULL )
										{
										if( ( wordEndingResult = adminItem_->analyzeWordEndingWithCurrentLanguage( WORD_CHINESE_EXCLUSIVE_NOUN, 0, currentReadWordItem->activeWordTypeString( WORD_TYPE_NOUN_SINGULAR ) ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a Chinese assignment noun" );

										if( wordEndingResult.hasFoundWordEnding )
											isAssignment_ = true;
										}
									}
								else
									{
									// Typical for Spanish
									if( previousGeneralizationWordItem_ != NULL &&
									previousGeneralizationWordTypeNr_ == WORD_TYPE_PROPER_NOUN &&
									( currentReadWordTypeItem = currentReadItem_->activeReadWordTypeItem() ) != NULL &&
									!currentReadWordTypeItem->hasDefiniteArticleParameter() &&
									!currentReadWordTypeItem->hasIndefiniteArticleParameter() &&
									( generalizationWordTypeItem = previousGeneralizationWordItem_->activeWordTypeItem( WORD_TYPE_PROPER_NOUN ) ) != NULL &&
									generalizationWordTypeItem->hasFeminineOrMasculineWordEnding() &&

									// Set indefinite article parameter
									currentReadWordTypeItem->setIndefiniteArticleParameter( generalizationWordTypeItem->hasFeminineWordEnding() ? WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the indefinite article parameter of a singular noun" );
									}
								}
							}

						currentReadWordItem->isUserSpecificationWord = true;
						}

					if( startSpecificationReadItem_ == NULL )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					break;

				case GRAMMAR_RELATION_ASSIGNMENT:
					break;

				case GRAMMAR_VERB:
					if( startSpecificationReadItem_ == NULL )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					break;

				case GRAMMAR_QUESTION_VERB:
					questionParameter_ = currentWordParameter;
					break;

				case GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
					questionParameter_ = currentWordParameter;

					// Don't insert a break statement here

				case GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
					isSpecificationGeneralization_ = true;

					if( startSpecificationReadItem_ == NULL )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					break;

				default:
					if( previousReadItem == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a word that doesn't belong to an assignment or a specification" );

					isStop = true;
					currentReadItem_ = previousReadItem;
				}

			if( !isFrenchPreposition )
				previousWordTypeNr = currentWordTypeNr;

			previousReadItem = currentReadItem_;
			}
		while( !isStop &&
		( currentReadItem_ = currentReadItem_->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	ContextResultType addPronounContext( unsigned short contextWordTypeNr, WordItem *contextWordItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addPronounContext";

		if( contextWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The read word of the read ahead item is undefined" );

		if( ( contextResult.contextNr = contextWordItem->contextNr( NULL ) ) == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		if( contextWordItem->addContext( contextWordTypeNr, NO_WORD_TYPE_NR, contextResult.contextNr, NO_COLLECTION_NR, NULL ) != RESULT_OK )
			return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to add a pronoun context to word \"", contextWordItem->anyWordTypeString(), "\"" );

		return contextResult;
		}


	protected:
	// Constructor

	AdminReadSentence( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	bool hasAnyChangeBeenMadeByThisSentence()
		{
		return hasAnyChangeBeenMadeByThisSentence_;
		}

	bool hasFemaleUserSpecificationWord()
		{
		return hasFemaleUserSpecificationWord_;
		}

	bool isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	bool isUserQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER );
		}

	bool wasPreviousCommandUndoOrRedo()
		{
		return wasPreviousCommandUndoOrRedo_;
		}

	signed char processReadSentence( char *readUserSentenceString )
		{
		unsigned int startSentenceNr = globalVariables_->currentSentenceNr;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "processReadSentence";

		previousGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
		previousGeneralizationWordItem_ = NULL;

		globalVariables_->hasFoundAnswerToQuestion = false;
		globalVariables_->isFirstAnswerToQuestion = true;
		globalVariables_->isQuestionAlreadyAnswered = false;
		globalVariables_->firstTouchedWordItem = NULL;

		strcpy( globalVariables_->writtenUserSentenceString, EMPTY_STRING );

		if( readUserSentenceString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( initializeVariablesInAllWords() != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to initialize variables in all words" );

		if( processSentence( readUserSentenceString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to process sentence: \"", readUserSentenceString, "\"" );

		if( !globalVariables_->hasDisplayedWarning &&
		!adminItem_->hasRequestedRestart() &&
		startSentenceNr == globalVariables_->currentSentenceNr )
			{
			checkForChangesMadeByThisSentence();

			// Skip integrity check if no changes are made
			if( hasAnyChangeBeenMadeByThisSentence_ &&
			!isUserImperativeSentence_ &&
			selectionListNr_ == NO_LIST_NR &&
			adminItem_->checkIntegrityOfStoredUserSentence( readUserSentenceString ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the integrity of the stored user sentence \"", readUserSentenceString, "\"" );

			// Has passed integrity check
			if( !globalVariables_->hasDisplayedWarning )
				{
				// Skip when no change has been made
				if( hasAnyChangeBeenMadeByThisSentence_ )
					{
					// Display self-generated conclusions of the current sentence
					if( adminItem_->writeSelfGeneratedInfo( true, false, false ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the self-generated conclusions" );

					// Display self-generated assumptions of the current sentence
					if( adminItem_->writeSelfGeneratedInfo( false, true, false ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the self-generated assumptions" );

					// Display self-generated questions of the current sentence
					if( adminItem_->writeSelfGeneratedInfo( false, false, true ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the self-generated questions" );

					if( checkAllWordsForStructuralIntegrity() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "The system has problem with the structural integrity" );
/*
					// In case you are planning to save the data in a database
					if( adminItem_->storeChangesInFutureDatabase() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to store the changes in a future database" );
*/					}

				if( questionParameter_ > NO_QUESTION_PARAMETER &&
				adminItem_->answerQuestions() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to answer questions" );
				}
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"You have turned my mourning into joyful dancing.
 *	You have taken away my clothes of mourning and
 *	clothed me with joy,
 *	that I might sing praises to you and not be silent.
 *	O Lord my God, I will give you thanks forever!" (Psalm 30:11-12)
 *************************************************************************/
