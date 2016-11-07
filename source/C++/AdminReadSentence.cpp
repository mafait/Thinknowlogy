/*	Class:			AdminReadSentence
 *	Supports class:	AdminItem
 *	Purpose:		To read and analyze sentences
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

#include "AdminItem.h"
#include "ReadList.cpp"
#include "WordTypeItem.cpp"

class AdminReadSentence
	{
	friend class AdminItem;

	// Private constructed variables

	bool isAssignment_;
	bool isActiveAssignment_;
	bool isInactiveAssignment_;
	bool isArchivedAssignment_;
	bool isConditional_;
	bool isEveryGeneralization_;
	bool isExclusiveSpecification_;
	bool isNegative_;
	bool isPartOf_;
	bool isPossessive_;
	bool isSpecificationGeneralization_;
	bool isUniqueUserRelation_;
	bool isUserImperativeSentence_;
	bool wasPreviousCommandUndoOrRedo_;

	unsigned short assumptionLevel_;
	unsigned short currentParseWordOrderNr_;
	unsigned short prepositionParameter_;
	unsigned short previousGeneralizationWordTypeNr_;
	unsigned short questionParameter_;
	unsigned short selectionListNr_;

	unsigned int generalizationContextNr_;
	unsigned int specificationContextNr_;

	ReadItem *currentReadItem_;
	ReadItem *linkedGeneralizationReadItem_;
	ReadItem *startGeneralizationWordReadItem_;
	ReadItem *endGeneralizationWordReadItem_;
	ReadItem *startSpecificationWordReadItem_;
	ReadItem *endSpecificationWordReadItem_;
	ReadItem *startRelationWordReadItem_;
	ReadItem *endRelationReadItem_;

	WordItem *previousGeneralizationWordItem_;

	char originalReadString[MAX_SENTENCE_STRING_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearReplacingInfoFromItems()
		{
		List *currentAdminList;
		WordItem *currentWordItem;

		wasPreviousCommandUndoOrRedo_ = false;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->clearReplacingInfoInWord();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				currentAdminList->clearReplacingInfoInList();
			}
		}

	void initializeVariablesInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->initializeVariablesInWord();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		}

	void setVariables( unsigned short wordParameter )
		{
		switch( wordParameter )
			{
			case WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
				assumptionLevel_ = 3;
				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
				assumptionLevel_ = 2;
				break;

			case WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				assumptionLevel_ = 1;
				break;

			case WORD_PARAMETER_ADJECTIVE_NO:
			case WORD_PARAMETER_ADVERB_NOT:
			case WORD_PARAMETER_ADVERB_NOT_FRENCH:
				isNegative_ = true;
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

			case WORD_PARAMETER_SINGULAR_VERB_HAS:
			case WORD_PARAMETER_PLURAL_VERB_HAVE:
				isPossessive_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_HAD:
			case WORD_PARAMETER_PLURAL_VERB_HAD:
				isAssignment_ = true;
				isArchivedAssignment_ = true;
				isPossessive_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_CAN_HAVE:
			case WORD_PARAMETER_PLURAL_VERB_CAN_HAVE:
				isConditional_ = true;
				isPossessive_ = true;
				break;
			}
		}

	ContextResultType addPronounContext( unsigned short contextWordTypeNr, WordItem *contextWordItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addPronounContext";

		if( contextWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The read word of the read ahead item is undefined" );

		if( ( contextResult.contextNr = contextWordItem->contextNr( NULL ) ) == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		if( contextWordItem->addContext( false, contextWordTypeNr, NO_WORD_TYPE_NR, contextResult.contextNr, NULL ) != RESULT_OK )
			return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to add a pronoun context to word \"", contextWordItem->anyWordTypeString(), "\"" );

		return contextResult;
		}

	ResultType addMultipleWord( unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, ReadItem *firstReadItem, ReadItem *secondReadItem )
		{
		WordResultType wordResult;
		bool isFrenchPreposition;
		bool hasFoundFrenchPreposition = false;
		bool hasCurrentWordWithSameWordType = true;
		bool hasFoundWordWithDifferentWordType = false;
		unsigned short currentWordTypeNr;
		unsigned short firstWordTypeNr;
		unsigned short nWordParts = 1;
		ReadItem *deletedReadItem;
		ReadItem *currentReadItem = secondReadItem;
		ReadItem *lastReadItem = NULL;
		WordItem *currentWordItem;
		WordItem *firstWordItem;
		WordItem *multipleWordItem;
		WordItem *createdMultipleWordItem;
		WordItem *createdWordItem;
		WordTypeItem *currentWordTypeItem;
		WordTypeItem *differentWordTypeItem;
		WordTypeItem *firstWordTypeItem;
		WordTypeItem *foundWordTypeItem;
		char existingMultipleWordString[MAX_SENTENCE_STRING_LENGTH];
		char multipleWordString[MAX_SENTENCE_STRING_LENGTH];
		ReadList *readList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( firstReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first read item is undefined" );

		if( secondReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given second read item is undefined" );

		if( ( firstWordItem = firstReadItem->readWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first read item has no read word" );

		firstWordTypeNr = firstReadItem->wordTypeNr();

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
			if( isFrenchPreposition ||
			firstWordTypeNr == currentWordTypeNr ||

			// Allow mix of singular and plural nouns
			( adminItem_->isNounWordType( firstWordTypeNr ) &&
			adminItem_->isNounWordType( currentWordTypeNr ) ) )
				{
				if( ( currentWordItem = currentReadItem->readWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I have found a read item without read word" );

				if( ( currentWordTypeItem = currentWordItem->activeWordTypeItem( currentWordTypeNr ) ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the word type item of the current read word" );

				nWordParts++;
				lastReadItem = currentReadItem;
				strcat( multipleWordString, SPACE_STRING );
				strcat( existingMultipleWordString, SPACE_STRING );

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
		( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		currentReadItem = secondReadItem;

		do	{
			currentWordTypeNr = ( hasFoundFrenchPreposition ? firstWordTypeNr : currentReadItem->wordTypeNr() );

			if( ( currentWordItem = currentReadItem->readWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "I have found a read item without read word" );

			createdMultipleWordItem = NULL;
			createdWordItem = NULL;
			multipleWordItem = NULL;

			if( hasFoundWordWithDifferentWordType )
				{
				if( ( wordResult = adminItem_->findWordTypeInAllWords( false, firstWordTypeNr, existingMultipleWordString, NULL ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find an exact word type in all words" );

				if( ( createdWordItem = wordResult.foundWordItem ) != NULL )
					{
					if( createdWordItem->addWordType( false, firstWordTypeItem->isProperNamePrecededByDefiniteArticle( firstWordTypeItem->definiteArticleParameter() ), adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, currentWordTypeNr, strlen( multipleWordString ), multipleWordString ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the word type to multiple word \"", createdWordItem->anyWordTypeString(), "\"" );

					createdMultipleWordItem = createdWordItem;
					multipleWordItem = createdWordItem;
					}
				}

			if( createdWordItem == NULL )
				{
				if( ( wordResult = adminItem_->addWord( false, true, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, firstReadItem->wordParameter(), currentWordTypeNr, strlen( multipleWordString ), multipleWordString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the multiple word" );

				createdMultipleWordItem = wordResult.createdWordItem;
				multipleWordItem = ( createdMultipleWordItem == NULL ? wordResult.foundWordItem : createdMultipleWordItem );
				foundWordTypeItem = wordResult.foundWordTypeItem;

				if( foundWordTypeItem != NULL &&

				( ( adjectiveParameter > NO_ADJECTIVE_PARAMETER &&
				!foundWordTypeItem->hasAdjectiveParameter() ) ||

				( definiteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER &&
				!foundWordTypeItem->hasDefiniteArticleParameter() ) ||

				( indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
				!foundWordTypeItem->hasIndefiniteArticleParameter() ) ) )
					{
					if( foundWordTypeItem->setParametersOfSingularNoun( adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set a parameter of a multiple word" );
					}
				}

			if( createdMultipleWordItem != NULL )
				{
				// Link both words to the created multiple word
				if( firstWordItem->addMultipleWord( nWordParts, currentWordTypeNr, createdMultipleWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the first multiple word" );

				if( ( currentWordItem = currentReadItem->readWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The current word item is undefined" );

				if( currentWordItem->addMultipleWord( nWordParts, currentWordTypeNr, createdMultipleWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the second multiple word" );
				}

			if( multipleWordItem == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The multiple word is undefined" );

			if( firstReadItem->changeReadWord( currentWordTypeNr, multipleWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an active read item" );

			deletedReadItem = currentReadItem;
			currentReadItem = ( deletedReadItem == lastReadItem ? NULL : currentReadItem->nextReadItem() );

			if( ( readList = adminItem_->readList ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The read list isn't created yet" );

			if( readList->deleteItem( deletedReadItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the second read item" );
			}
		while( currentReadItem != NULL );

		return RESULT_OK;
		}

	ResultType addReadSpecification( bool isAction, bool isNewStart, unsigned short selectionLevel )
		{
		bool isConditional = ( isConditional_ || selectionListNr_ != NO_LIST_NR );
		unsigned short imperativeVerbParameter = NO_IMPERATIVE_PARAMETER;
		ReadItem *currentGeneralizationReadItem = startGeneralizationWordReadItem_;
		SpecificationItem *userSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addReadSpecification";

		if( currentGeneralizationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "The start generalization read item is undefined" );

		if( endGeneralizationWordReadItem_ == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "The end generalization read item is undefined" );

		if( selectionListNr_ == NO_LIST_NR &&
		commonVariables_->nUserGeneralizationWords == 1 &&
		commonVariables_->nUserSpecificationWords > 1 &&
		commonVariables_->nUserRelationWords > 1 )
			{
			// Ambiguity: Missing relation context
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MULTIPLE_SPECIFICATIONS_RELATIONS ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to write an interface warning about ambiguity" );
			}
		else
			{
			if( wasPreviousCommandUndoOrRedo_ )
				// From this point, redo is not possible anymore
				clearReplacingInfoFromItems();

			adminItem_->initializeAdminVariables();

			do	{
				switch( currentGeneralizationReadItem->grammarParameter )
					{
					case GRAMMAR_IMPERATIVE:
						imperativeVerbParameter = currentGeneralizationReadItem->wordParameter();
						// Don't insert a break statement

					case GRAMMAR_GENERALIZATION_WORD:
						if( adminItem_->addUserSpecifications( isAction, isAssignment_, isConditional, isInactiveAssignment_, isArchivedAssignment_, isEveryGeneralization_, isExclusiveSpecification_, isNegative_, isNewStart, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueUserRelation_, assumptionLevel_, prepositionParameter_, questionParameter_, selectionLevel, selectionListNr_, imperativeVerbParameter, generalizationContextNr_, specificationContextNr_, currentGeneralizationReadItem, startSpecificationWordReadItem_, endSpecificationWordReadItem_, startRelationWordReadItem_, endRelationReadItem_ ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to add the read user specifications" );

						if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
							{
							if( adminItem_->markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to mark the words passing the integrity check of the stored user sentence" );
							}
					}
				}
			while( !commonVariables_->hasDisplayedWarning &&
			currentGeneralizationReadItem != endGeneralizationWordReadItem_ &&
			( currentGeneralizationReadItem = currentGeneralizationReadItem->nextReadItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType checkAllWordsForStructuralIntegrity()
		{
		bool wasUndoOrRedoCommand = adminItem_->wasUndoOrRedoCommand();
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForStructuralIntegrity";

		if( ( currentSpecificationWordItem = commonVariables_->firstSpecificationWordItem ) != NULL )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem->checkWordForStructuralIntegrity( wasUndoOrRedoCommand ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check word \"", currentSpecificationWordItem->anyWordTypeString(), "\" for its structural integrity" );
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType findGrammarPath( unsigned short grammarLevel, GrammarItem *parseGrammarItem )
		{
		ReadResultType readResult;
		bool isOption;
		bool isChoice;
		bool isWaitingForNewStart;
		bool isWaitingForChoiceEnd;
		unsigned short previousWordOrderNr;
		unsigned short startWordOrderNr;
		unsigned short choiceStartWordOrderNr = NO_ORDER_NR;
		GrammarItem *definitionParseGrammarItem;
		ReadList *readList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findGrammarPath";

		if( grammarLevel >= MAX_GRAMMAR_LEVEL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given grammar level is too high: #", grammarLevel );

		if( parseGrammarItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given parse grammar item is undefined" );

		if( ( readList = adminItem_->readList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The read list isn't created yet" );

		if( grammarLevel == NO_GRAMMAR_LEVEL )
			currentParseWordOrderNr_ = NO_ORDER_NR;

		startWordOrderNr = currentParseWordOrderNr_;

		do	{
			isOption = false;
			isChoice = false;
			isWaitingForNewStart = true;
			isWaitingForChoiceEnd = false;

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
						if( ( readResult = readList->selectMatchingWordType( currentParseWordOrderNr_, definitionParseGrammarItem->grammarParameter(), definitionParseGrammarItem->grammarWordTypeNr() ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to select a matching word type" );

						if( readResult.hasFoundMatchingWordType )
							currentParseWordOrderNr_++;
						}
					else
						{
						if( grammarLevel + 1 >= MAX_GRAMMAR_LEVEL )
							return adminItem_->startError( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #", grammarLevel );

						if( parseGrammarItem->definitionGrammarItem == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "Grammar word \"", parseGrammarItem->grammarString(), "\" isn't defined in the grammar file" );

						if( findGrammarPath( ( grammarLevel + 1 ), parseGrammarItem->definitionGrammarItem ) != RESULT_OK )
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

							if( startWordOrderNr < currentParseWordOrderNr_ )
								{
								if( readList->activateInactiveReadWords( currentParseWordOrderNr_ ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to activate inactive read words from position: ", currentParseWordOrderNr_ );
								}
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
				parseGrammarItem != NULL &&
				parseGrammarItem->isNewStart() &&
				currentParseWordOrderNr_ > previousWordOrderNr )
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

		if( definitionParseGrammarItem != NULL &&
		definitionParseGrammarItem->isGrammarDefinition() &&

		( previousWordOrderNr > startWordOrderNr ||
		currentParseWordOrderNr_ > startWordOrderNr ) )
			{
			if( readList->setGrammarParameter( ( currentParseWordOrderNr_ > startWordOrderNr ), startWordOrderNr, ( currentParseWordOrderNr_ > startWordOrderNr ? currentParseWordOrderNr_ : previousWordOrderNr ), definitionParseGrammarItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the grammar parameter of the read words between the positions ", startWordOrderNr, " and ", currentParseWordOrderNr_ );
			}

		return RESULT_OK;
		}

	ResultType parseSentence( char *readUserSentenceString )
		{
		bool isAction = false;
		bool isNewStart = true;
		unsigned short wordOrderNr;
		unsigned short wordParameter;
		unsigned short selectionLevel = NO_SELECTION_LEVEL;
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "parseSentence";

		isUserImperativeSentence_ = false;

		selectionListNr_ = NO_LIST_NR;

		linkedGeneralizationReadItem_ = NULL;
		startGeneralizationWordReadItem_ = NULL;
		endGeneralizationWordReadItem_ = NULL;

		adminItem_->initializeLinkedWord();

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
						if( scanSpecification( readUserSentenceString ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to scan the generalization-specification" );

						if( addReadSpecification( isAction, isNewStart, selectionLevel ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to add the read specification" );

						isNewStart = false;

						break;

					case GRAMMAR_IMPERATIVE:
						if( readImperative( isAction, isNewStart, selectionLevel, readUserSentenceString ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read an imperative" );

						isNewStart = false;
						isUserImperativeSentence_ = true;

						break;

					case GRAMMAR_ANSWER:
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to write an interface warning" );

						break;

					case GRAMMAR_SENTENCE_CONJUNCTION:
						switch( wordParameter )
							{
							case WORD_PARAMETER_SYMBOL_COMMA:
							case WORD_PARAMETER_CONJUNCTION_AND:
							// Typically for Dutch: in zowel ... als ...
							case WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
								break;

							case WORD_PARAMETER_CONJUNCTION_OR:
								isNewStart = true;

								break;

							default:
								return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I have found an illegal conjunction word parameter: ", wordParameter );
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
								return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I have found an illegal selection word" );
							}

						break;

					default:
						if( readUserSentenceString != NULL &&

						( adminItem_->isCurrentlyTesting() ||
						adminItem_->isSystemStartingUp() ) )
							return adminItem_->startError( functionNameString, moduleNameString_, "I have found an unknown word in sentence \"", readUserSentenceString, "\" at position ", wordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", wordParameter );

						return adminItem_->startError( functionNameString, moduleNameString_, "I have found an unknown word at position ", wordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", wordParameter );
					}
				}
			}
		while( currentReadItem_ != NULL &&
		!commonVariables_->hasDisplayedWarning &&
		!adminItem_->hasRequestedRestart() &&
		( currentReadItem_ = currentReadItem_->nextReadItem() ) != NULL );

		if( selectionListNr_ != NO_LIST_NR )
			{
			if( adminItem_->checkForDuplicateSelection() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check on a duplicate selection" );
			}

		return RESULT_OK;
		}

	ResultType processSentence( char *readUserSentenceString )
		{
		ReadResultType readResult;
		bool isInterpretationSuccessful;
		bool hasCreatedAllReadWords = false;
		unsigned short currentLanguageNr;
		unsigned short lastCreatedWordOrderNr;
		unsigned short nLanguages;
		unsigned short originalLanguageNr = commonVariables_->currentLanguageNr;
		GrammarItem *firstGrammarItem;
		WordItem *currentLanguageWordItem;
		ReadList *readList;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "processSentence";

		if( readUserSentenceString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( nLanguages = adminItem_->nLanguages() ) <= NO_LANGUAGE_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any language" );

		currentLanguageNr = originalLanguageNr;

		if( nLanguages < currentLanguageNr )
			return adminItem_->startError( functionNameString, moduleNameString_, "The current language number exceeds the number of languages" );

		strcpy( originalReadString, readUserSentenceString );

		do	{
			adminItem_->deleteTemporaryReadList();

			// Need to switch language
			if( currentLanguageNr != commonVariables_->currentLanguageNr )
				{
				if( adminItem_->assignLanguage( currentLanguageNr ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language" );

				// Avoid using the expanded read string with other languages
				strcpy( readUserSentenceString, originalReadString );
				}

			if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The current language word item is undefined" );

			if( currentLanguageWordItem->isNeededToCheckGrammar() )
				{
				if( currentLanguageWordItem->checkGrammar() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the grammar" );
				}

			if( readUserSentenceString != NULL &&
			currentLanguageWordItem->isLanguageWithMergedWords() )
				{
				// Typically for French: Compound words
				if( currentLanguageWordItem->expandMergedWordsInReadSentence( readUserSentenceString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to expand the compound wordds in the read user sentence string" );
				}

			// Create read words from a given sentence
			if( ( readResult = adminItem_->createReadWords( readUserSentenceString ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create the read words" );

			hasCreatedAllReadWords = readResult.hasCreatedAllReadWords;

			if( ( readList = adminItem_->readList ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The read list isn't created yet" );

			isInterpretationSuccessful = false;
			readList->clearLastActivatedWordOrderNr();

			if( hasCreatedAllReadWords )
				{
				readResult.hasFoundMoreInterpretations = false;
				lastCreatedWordOrderNr = adminItem_->lastCreatedWordOrderNr();
				firstGrammarItem = currentLanguageWordItem->firstGrammarItem();

				do	{
					if( findGrammarPath( NO_GRAMMAR_LEVEL, firstGrammarItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the grammar path for a sentence with language \"", adminItem_->languageNameString( currentLanguageNr ), "\"" );

					isInterpretationSuccessful = ( currentParseWordOrderNr_ == lastCreatedWordOrderNr );

					if( !isInterpretationSuccessful )
						{
						if( ( readResult = readList->findMoreInterpretations() ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find more interpretations" );
						}
					}
				while( !isInterpretationSuccessful &&
				readResult.hasFoundMoreInterpretations );
				}

			if( isInterpretationSuccessful )
				{
				if( adminItem_->deleteUnusedWordsAndWordTypes() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the unused words and word types of the read words" );

				if( parseSentence( readUserSentenceString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to parse the sentence" );
				}
			else
				{
				if( adminItem_->deleteAllWordTypesOfCurrentSentence() != RESULT_OK )
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
				// From this point, redo is not possible anymore
				clearReplacingInfoFromItems();

			if( commonVariables_->currentLanguageNr != originalLanguageNr )
				{
				// Restore the original language
				if( adminItem_->assignLanguage( originalLanguageNr ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language after an interpretation failure" );
				}

			if( currentParseWordOrderNr_ == NO_ORDER_NR )
				{
				if( adminItem_->isSystemStartingUp() )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I don't understand this sentence. Please make sure you enter a sentence within my limited grammar definition" );

				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_NOT_CONFORM_GRAMMAR ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( adminItem_->isSystemStartingUp() )
					{
					sprintf( errorString, "I don't understand the sentence from the word at position %u", currentParseWordOrderNr_ );
					return adminItem_->startSystemError( functionNameString, moduleNameString_, errorString );
					}

				if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START, (unsigned int)currentParseWordOrderNr_, INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			}

		return RESULT_OK;
		}

	ResultType readImperative( bool isAction, bool isNewStart, unsigned short selectionLevel, char *readUserSentenceString )
		{
		unsigned short imperativeVerbParameter = NO_IMPERATIVE_PARAMETER;
		unsigned short executionNounWordParameter = NO_WORD_PARAMETER;
		ReadItem *imperativeReadItem;
		ReadItem *specificationReadItem;
		WordItem *imperativeWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readImperative";

		startGeneralizationWordReadItem_ = NULL;
		endGeneralizationWordReadItem_ = NULL;

		if( scanSpecification( readUserSentenceString ) != RESULT_OK )
			return adminItem_->addErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I failed to scan the generalization-specification" );

		if( endGeneralizationWordReadItem_ == NULL ||
		( imperativeReadItem = startGeneralizationWordReadItem_ ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "I couldn't find the imperative" );

		do	{
			if( imperativeReadItem->isVerb() )
				{
				imperativeVerbParameter = imperativeReadItem->wordParameter();
				imperativeWordItem = imperativeReadItem->readWordItem();
				}
			}
		while( ( imperativeReadItem = imperativeReadItem->nextReadItem() ) != NULL );

		// Only imperative word
		if( ( specificationReadItem = startSpecificationWordReadItem_ ) == NULL )
			{
			if( imperativeWordItem != NULL &&
			imperativeWordItem->isVerbUndoOrRedo() )
				wasPreviousCommandUndoOrRedo_ = true;
			else
				// From this point, redo is not possible anymore
				clearReplacingInfoFromItems();

			if( adminItem_->executeImperative( true, NO_LIST_NR, startGeneralizationWordReadItem_->wordParameter(), NO_WORD_PARAMETER, NO_WORD_TYPE_NR, MAX_PROGRESS, startGeneralizationWordReadItem_->readString, startGeneralizationWordReadItem_->readWordItem(), NULL, startRelationWordReadItem_, endRelationReadItem_, NULL, NULL ) != RESULT_OK )
				{
				if( startGeneralizationWordReadItem_ == NULL )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute the single imperative" );

				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute the single imperative at position ", startGeneralizationWordReadItem_->wordOrderNr() );
				}
			}
		else
			{
			// Imperative word has specifications
			if( endSpecificationWordReadItem_ == NULL )
				return adminItem_->startErrorWithAdminListNr( selectionListNr_, functionNameString, moduleNameString_, "The end specification read item is undefined" );

			if( addReadSpecification( isAction, isNewStart, selectionLevel ) != RESULT_OK )
				{
				if( startGeneralizationWordReadItem_ == NULL ||
				endGeneralizationWordReadItem_ == NULL )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the generalization part of the read specification" );

				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the generalization part of the read specification between the positions ", startGeneralizationWordReadItem_->wordOrderNr(), " and ", endGeneralizationWordReadItem_->wordOrderNr() );
				}

			if( !commonVariables_->hasDisplayedWarning &&
			selectionListNr_ == NO_LIST_NR )
				{
				do	{
					if( specificationReadItem->isUserDefined() ||

					( specificationReadItem->isNoun() &&
					!specificationReadItem->isNounInformation() ) )
						{
						// Make distinction between reading a normal file or a test file
						if( specificationReadItem->isNounFile() ||
						// Make distinction between displaying a word or justification report
						specificationReadItem->isNounJustificationReport() )
							executionNounWordParameter = specificationReadItem->wordParameter();
						else
							{
							if( adminItem_->executeImperative( true, NO_LIST_NR, imperativeVerbParameter, ( executionNounWordParameter == NO_WORD_PARAMETER ? specificationReadItem->wordParameter() : executionNounWordParameter ), specificationReadItem->wordTypeNr(), MAX_PROGRESS, specificationReadItem->readString, imperativeWordItem, specificationReadItem->readWordItem(), startRelationWordReadItem_, endRelationReadItem_, NULL, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute an imperative with specifications" );
							}
						}
					}
				while( !adminItem_->hasRequestedRestart() &&
				!commonVariables_->hasDisplayedWarning &&
				// Still read items available
				adminItem_->firstActiveReadItem() != NULL &&

				( !specificationReadItem->isVirtualListPreposition() ||
				// Loop shouldn't end if virtual list prepositions are used to e.g. display justification reports
				executionNounWordParameter > NO_WORD_PARAMETER ) &&

				specificationReadItem != endSpecificationWordReadItem_ &&
				( specificationReadItem = specificationReadItem->nextReadItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	ResultType scanSpecification( char *readUserSentenceString )
		{
		ContextResultType contextResult;
		bool isFrenchPreposition;
		bool isSameWordTypeAsPreviousWord;
		bool hasGeneralizationArticle = false;
		bool isStop = false;
		unsigned short currentWordOrderNr;
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "scanSpecification";

		isAssignment_ = false;
		isActiveAssignment_ = false;
		isInactiveAssignment_ = false;
		isArchivedAssignment_ = false;
		isConditional_ = false;
		isEveryGeneralization_ = false;
		isExclusiveSpecification_ = false;
		isNegative_ = false;
		isPartOf_ = false;
		isPossessive_ = false;
		isSpecificationGeneralization_ = false;
		isUniqueUserRelation_ = false;

		assumptionLevel_ = NO_ASSUMPTION_LEVEL;
		currentParseWordOrderNr_ = NO_ORDER_NR;
		prepositionParameter_ = NO_PREPOSITION_PARAMETER;
		questionParameter_ = NO_QUESTION_PARAMETER;

		generalizationContextNr_ = NO_CONTEXT_NR;
		specificationContextNr_ = NO_CONTEXT_NR;

		startSpecificationWordReadItem_ = NULL;
		endSpecificationWordReadItem_ = NULL;
		startRelationWordReadItem_ = NULL;
		endRelationReadItem_ = NULL;

		commonVariables_->nUserGeneralizationWords = 0;
		commonVariables_->nUserSpecificationWords = 0;
		commonVariables_->nUserRelationWords = 0;

		if( linkedGeneralizationReadItem_ == NULL )
			{
			startGeneralizationWordReadItem_ = NULL;
			endGeneralizationWordReadItem_ = NULL;
			}
		else
			{
			if( linkedGeneralizationReadItem_->isProperName() )
				startGeneralizationWordReadItem_ = linkedGeneralizationReadItem_;

			linkedGeneralizationReadItem_ = NULL;
			}

		if( currentReadItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The read item is undefined" );

		do	{
			isFrenchPreposition = currentReadItem_->isFrenchPreposition();

			currentWordOrderNr = currentReadItem_->wordOrderNr();
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
				case GRAMMAR_SENTENCE:
					if( !currentReadItem_->isSeparator() )
						{
						if( readUserSentenceString != NULL &&

						( adminItem_->isCurrentlyTesting() ||
						adminItem_->isSystemStartingUp() ) )
							return adminItem_->startError( functionNameString, moduleNameString_, "I have found an unknown word in sentence \"", readUserSentenceString, "\" at position ", currentWordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", currentWordParameter );

						return adminItem_->startError( functionNameString, moduleNameString_, "I have found an unknown word at position ", currentWordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", currentWordParameter );
						}

					break;

				case GRAMMAR_ANSWER:
					// Needs to be implemented
					break;

				// Assignment generalization-specification
				case GRAMMAR_GENERALIZATION_ASSIGNMENT:
					isAssignment_ = true;

					// Don't insert a break statement here

				case GRAMMAR_IMPERATIVE:
				case GRAMMAR_GENERALIZATION_SPECIFICATION:
				case GRAMMAR_GENERALIZATION_PART:
				case GRAMMAR_GENERALIZATION_WORD:
					if( currentReadItem_->isAdjective() )
						{
						if( adminItem_->isAdjectiveParameter( currentReadItem_->wordParameter() ) )
							generalizationAdjectiveParameter = currentReadItem_->wordParameter();

						if( currentReadItem_->isAdjectivePrevious() )
							{
							isAssignment_ = true;
							isInactiveAssignment_ = true;
							}

						if( currentReadItem_->isAdjectiveEvery() )
							isEveryGeneralization_ = true;
						}

					if( currentReadItem_->isArticle() )
						{
						hasGeneralizationArticle = true;

						if( currentReadItem_->isDefiniteArticleParameter( currentReadItem_->wordParameter() ) )
							generalizationDefiniteArticleParameter = currentReadItem_->wordParameter();
						else
							generalizationIndefiniteArticleParameter = currentReadItem_->wordParameter();
						}

					if( currentReadItem_->isNounPartOf() )
						isPartOf_ = true;

					if( currentReadItem_->isGeneralizationWord() )
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

							commonVariables_->nUserGeneralizationWords++;
							}

						if( currentReadWordItem != NULL )
							currentReadWordItem->isUserGeneralizationWord = true;
						}

					if( currentReadItem_->isDeterminerOrPronoun() )
						{
						if( generalizationContextNr_ > NO_CONTEXT_NR )
							return adminItem_->startError( functionNameString, moduleNameString_, "The generalization context number is already assigned" );

						if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a generalization pronoun context" );

						generalizationContextNr_ = contextResult.contextNr;
						}

					if( startGeneralizationWordReadItem_ == NULL )
						startGeneralizationWordReadItem_ = currentReadItem_;

					endGeneralizationWordReadItem_ = currentReadItem_;

					break;

				case GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION:
					nextReadItem = currentReadItem_->nextReadItem();

					if( nextReadItem != NULL &&
					nextReadItem->readWordItem() != previousGeneralizationWordItem_ )
						{
						isStop = true;
						linkedGeneralizationReadItem_ = nextReadItem;
						}

					break;

				case GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION:
					isExclusiveSpecification_ = true;
					break;

				case GRAMMAR_RELATION_PART:
				case GRAMMAR_RELATION_WORD:
					if( currentReadItem_->isPreposition() )
						prepositionParameter_ = currentWordParameter;

					if( currentReadItem_->isRelationWord() )
						{
						if( isSameWordTypeAsPreviousWord )
							{
							if( addMultipleWord( NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, previousReadItem, currentReadItem_ ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a multiple relation word" );

							// Current read item is deleted
							currentReadItem_ = previousReadItem;
							}
						else
							commonVariables_->nUserRelationWords++;

						if( currentReadWordItem != NULL )
							currentReadWordItem->isUserRelationWord = true;
						}

					if( startSpecificationWordReadItem_ == NULL )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					if( startRelationWordReadItem_ == NULL )
						startRelationWordReadItem_ = currentReadItem_;

					endRelationReadItem_ = currentReadItem_;

					switch( currentWordParameter )
						{
						// Typically for English: "... in both ... and ..."
						case WORD_PARAMETER_NUMERAL_BOTH:
						// Typically for Dutch: "... in zowel ... als ..."
						case WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:
							isExclusiveSpecification_ = true;
							break;
						}

					break;

				case GRAMMAR_ASSIGNMENT_PART:
				case GRAMMAR_SPECIFICATION_ASSIGNMENT:
					isAssignment_ = true;
					isActiveAssignment_ = true;

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
					if( currentReadItem_->isSpecificationWord() )
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
							if( isArchivedAssignment_ &&
							!hasGeneralizationArticle )
								isUniqueUserRelation_ = true;

							commonVariables_->nUserSpecificationWords++;

							if( previousGeneralizationWordTypeNr_ == WORD_TYPE_PROPER_NAME &&
							currentWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
							( currentReadWordTypeItem = currentReadItem_->activeReadWordTypeItem() ) != NULL )
								{
								if( previousGeneralizationWordItem_ != NULL &&
								!currentReadWordTypeItem->hasDefiniteArticleParameter() &&
								!currentReadWordTypeItem->hasIndefiniteArticleParameter() &&
								( generalizationWordTypeItem = previousGeneralizationWordItem_->activeWordTypeItem( WORD_TYPE_PROPER_NAME ) ) != NULL )
									{
									if( generalizationWordTypeItem->hasFeminineOrMasculineWordEnding() )
										{
										if( currentReadWordTypeItem->setParametersOfSingularNoun( NO_ADJECTIVE_PARAMETER, ( generalizationWordTypeItem->hasFeminineWordEnding() ? WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE : WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE ), ( generalizationWordTypeItem->hasFeminineWordEnding() ? WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set the definite and indefinite article parameters of a singular noun" );
										}
									}
								}
							}

						if( currentReadWordItem != NULL )
							currentReadWordItem->isUserSpecificationWord = true;
						}

					if( startSpecificationWordReadItem_ == NULL )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					break;

				case GRAMMAR_RELATION_ASSIGNMENT:
					break;

				case GRAMMAR_VERB:
					if( startSpecificationWordReadItem_ == NULL )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					break;

				case GRAMMAR_QUESTION_VERB:
					questionParameter_ = currentWordParameter;
					break;

				case GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
					questionParameter_ = currentWordParameter;

					// Don't insert a break statement here

				case GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
					isSpecificationGeneralization_ = true;

					if( startSpecificationWordReadItem_ == NULL )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					break;

				default:
					if( previousReadItem == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I have found a word that doesn't belong to an assignment or a specification" );

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


	protected:
	// Constructor

	AdminReadSentence( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		isAssignment_ = false;
		isActiveAssignment_ = false;
		isInactiveAssignment_ = false;
		isArchivedAssignment_ = false;
		isConditional_ = false;
		isEveryGeneralization_ = false;
		isExclusiveSpecification_ = false;
		isNegative_ = false;
		isPartOf_ = false;
		isPossessive_ = false;
		isSpecificationGeneralization_ = false;
		isUniqueUserRelation_ = false;
		isUserImperativeSentence_ = false;
		wasPreviousCommandUndoOrRedo_ = false;

		assumptionLevel_ = NO_ASSUMPTION_LEVEL;
		currentParseWordOrderNr_ = NO_ORDER_NR;
		prepositionParameter_ = NO_PREPOSITION_PARAMETER;
		previousGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
		questionParameter_ = NO_QUESTION_PARAMETER;
		selectionListNr_ = NO_LIST_NR;

		generalizationContextNr_ = NO_CONTEXT_NR;
		specificationContextNr_ = NO_CONTEXT_NR;

		currentReadItem_ = NULL;
		linkedGeneralizationReadItem_ = NULL;
		startGeneralizationWordReadItem_ = NULL;
		endGeneralizationWordReadItem_ = NULL;
		startSpecificationWordReadItem_ = NULL;
		endSpecificationWordReadItem_ = NULL;
		startRelationWordReadItem_ = NULL;
		endRelationReadItem_ = NULL;

		previousGeneralizationWordItem_ = NULL;

		strcpy( originalReadString, EMPTY_STRING );

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminReadSentence" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	bool isActiveUserAssignment()
		{
		return isActiveAssignment_;
		}

	bool isUserQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER );
		}

	bool isUserSelectionSentence()
		{
		return ( selectionListNr_ != NO_LIST_NR );
		}

	bool wasPreviousCommandUndoOrRedo()
		{
		return wasPreviousCommandUndoOrRedo_;
		}

	ResultType processReadSentence( char *readUserSentenceString )
		{
		bool hasAnyChangeBeenMadeByThisSentence;
		unsigned int startSentenceNr = commonVariables_->currentSentenceNr;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processReadSentence";

		previousGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
		previousGeneralizationWordItem_ = NULL;

		commonVariables_->hasFoundAnswerToQuestion = false;
		commonVariables_->hasDisplayedArticleNotification = false;
		commonVariables_->isFirstAnswerToQuestion = true;
		commonVariables_->isQuestionAlreadyAnswered = false;
		commonVariables_->firstTouchedWordItem = NULL;

		strcpy( commonVariables_->writtenUserSentenceString, EMPTY_STRING );

		initializeVariablesInAllWords();

		if( readUserSentenceString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( processSentence( readUserSentenceString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to process sentence: \"", readUserSentenceString, "\"" );

		if( !commonVariables_->hasDisplayedWarning &&
		!adminItem_->hasRequestedRestart() &&
		startSentenceNr == commonVariables_->currentSentenceNr )
			{
			adminItem_->checkForChangesMadeByThisSentence();
			hasAnyChangeBeenMadeByThisSentence = adminItem_->hasAnyChangeBeenMadeByThisSentence();

			// Skip integrity check if no changes are made
			if( hasAnyChangeBeenMadeByThisSentence &&
			!isUserImperativeSentence_ &&
			selectionListNr_ == NO_LIST_NR &&
			// User specification is already known (notification: I know)
			adminItem_->userSpecificationItem() != NULL )
				{
				if( adminItem_->checkIntegrityOfStoredUserSentence( readUserSentenceString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the integrity of the stored user sentence \"", readUserSentenceString, "\"" );
				}

			// Has passed integrity check
			if( !commonVariables_->hasDisplayedWarning )
				{
				// Skip when no changes are made
				if( hasAnyChangeBeenMadeByThisSentence )
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

					if( adminItem_->hasUnprocessedNegativeConclusion() )
						return adminItem_->startError( functionNameString, moduleNameString_, "I have found an unprocessed negative conclusion" );

					if( checkAllWordsForStructuralIntegrity() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "The system has problem with the structural integrity" );

					// In case you are planning to save the data in a database
/*					if( storeChangesInFutureDatabase() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to store the changes in a future database" );
*/					}

				if( isUserQuestion() )
					{
					if( adminItem_->answerQuestions() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to answer questions" );
					}
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
