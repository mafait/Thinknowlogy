/*	Class:			AdminReadSentence
 *	Supports class:	AdminItem
 *	Purpose:		To read and analyze sentences
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

class AdminReadSentence
	{
	// Private constructed variables

	private boolean hasAnyChangeBeenMadeByThisSentence_;
	private boolean hasFemaleUserSpecificationWord_;
	private boolean isAssignment_;
	private boolean isInactiveAssignment_;
	private boolean isArchivedAssignment_;
	private boolean isCharacteristicFor_;
	private boolean isChineseCurrentLanguage_;
	private boolean isConditional_;
	private boolean isEveryGeneralization_;
	private boolean isExclusiveSpecification_;
	private boolean isNegative_;
	private boolean isPartOf_;
	private boolean isPossessive_;
	private boolean isSpecific_;
	private boolean isSpecificationGeneralization_;
	private boolean isUncountableGeneralizationNoun_;
	private boolean isUniqueUserRelation_;
	private boolean isUserImperativeSentence_;
	private boolean wasPreviousCommandUndoOrRedo_;

//	short answerParameter_;
	private short currentParseWordOrderNr_;
	private short prepositionParameter_;
	private short previousGeneralizationWordTypeNr_;
	private short questionParameter_;
	private short selectionListNr_;
	private short userAssumptionLevel_;

	private int generalizationContextNr_;
	private int specificationContextNr_;

	private ReadItem currentReadItem_;
	private ReadItem linkedGeneralizationReadItem_;
	private ReadItem startGeneralizationReadItem_;
	private ReadItem endGeneralizationReadItem_;
	private ReadItem startSpecificationReadItem_;
	private ReadItem endSpecificationReadItem_;
	private ReadItem startRelationReadItem_;
	private ReadItem endRelationReadItem_;

	private WordItem previousGeneralizationWordItem_;


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private void checkForChangesMadeByThisSentence()
		{
		int currentSentenceNr = GlobalVariables.currentSentenceNr;
		int highestFoundSentenceNr = adminItem_.highestFoundSentenceNr( false, currentSentenceNr );

		hasAnyChangeBeenMadeByThisSentence_ = ( highestFoundSentenceNr >= currentSentenceNr );
		}

	private static void clearReplacingInfoInSpecificationWords()
		{
		WordItem currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all specification words
			do	currentSpecificationWordItem.clearReplacingInfoInWord();
			while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}
		}

	private void setVariables( short wordParameter )
		{
		switch( wordParameter )
			{
			case Constants.WORD_PARAMETER_ADJECTIVE_NO:
			case Constants.WORD_PARAMETER_ADVERB_NOT:
			case Constants.WORD_PARAMETER_ADVERB_CHINESE_NEVER_AGAIN:
			case Constants.WORD_PARAMETER_ADVERB_FRENCH_PAS:
				isNegative_ = true;
				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_CHARACTERISTIC:
				isCharacteristicFor_ = true;
				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_SPECIFIC:
				isSpecific_ = true;
				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
				userAssumptionLevel_ = 3;
				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
				userAssumptionLevel_ = 2;
				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				userAssumptionLevel_ = 1;
				break;

			case Constants.WORD_PARAMETER_ADVERB_ANYMORE:
				isUniqueUserRelation_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_WAS:
			case Constants.WORD_PARAMETER_PLURAL_VERB_WERE:
				isAssignment_ = true;
				isArchivedAssignment_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_BE:
			case Constants.WORD_PARAMETER_PLURAL_VERB_CAN_BE:
				isConditional_ = true;
				break;

			case Constants.WORD_PARAMETER_PREPOSITION_OF:
				if( !isAssignment_ &&
				startSpecificationReadItem_ == null )
					// Typical for Chinese
					isPossessive_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_HAS:
			case Constants.WORD_PARAMETER_PLURAL_VERB_HAVE:
				isPossessive_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_HAD:
			case Constants.WORD_PARAMETER_PLURAL_VERB_HAD:
				isAssignment_ = true;
				isArchivedAssignment_ = true;
				isPossessive_ = true;
			}
		}

	private byte assignLanguage( short newLanguageNr )
		{
		WordItem languageWordItem;
		WordItem predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = GlobalVariables.predefinedNounLanguageWordItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The predefined noun language word item is undefined" );

		if( ( languageWordItem = predefinedNounLanguageWordItem.languageWordItem( newLanguageNr ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the requested language" );

		GlobalVariables.currentLanguageNr = newLanguageNr;
		GlobalVariables.currentLanguageWordItem = languageWordItem;

		if( adminItem_.assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, languageWordItem, GlobalVariables.predefinedNounLanguageWordItem, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language with authorization" );

		return Constants.RESULT_OK;
		}

	private byte addMultipleWord( short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, ReadItem firstReadItem, ReadItem secondReadItem )
		{
		boolean hasCurrentWordWithSameWordType = true;
		boolean hasFoundFrenchPreposition = false;
		boolean hasFoundWordWithDifferentWordType = false;
		boolean isFirstWordTypeNumeral = false;
		boolean isFrenchPreposition;
		short currentWordTypeNr;
		short firstWordTypeNr;
		short nWordParts = 1;
		ReadItem deletedReadItem;
		ReadItem currentReadItem = secondReadItem;
		ReadItem lastReadItem = null;
		WordItem createdMultipleWordItem;
		WordItem createdWordItem;
		WordItem currentWordItem;
		WordItem firstWordItem;
		WordItem multipleWordItem;
		WordTypeItem currentWordTypeItem;
		WordTypeItem differentWordTypeItem;
		WordTypeItem firstWordTypeItem;
		WordTypeItem foundWordTypeItem;
		StringBuffer existingMultipleWordStringBuffer;
		StringBuffer multipleWordStringBuffer;
		WordResultType wordResult = new WordResultType();

		if( firstReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first read item is undefined" );

		if( secondReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given second read item is undefined" );

		if( ( firstWordItem = firstReadItem.readWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first read item has no read word" );

		firstWordTypeNr = firstReadItem.wordTypeNr();

		if( firstWordTypeNr == Constants.WORD_TYPE_NUMERAL )
			isFirstWordTypeNumeral = true;

		if( ( firstWordTypeItem = firstWordItem.activeWordTypeItem( firstWordTypeNr ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type item of the given first read item" );

		if( adjectiveParameter == Constants.NO_ADJECTIVE_PARAMETER )
			adjectiveParameter = firstWordTypeItem.adjectiveParameter();

		if( definiteArticleParameter == Constants.NO_DEFINITE_ARTICLE_PARAMETER )
			definiteArticleParameter = firstWordTypeItem.definiteArticleParameter();

		if( indefiniteArticleParameter == Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
			indefiniteArticleParameter = firstWordTypeItem.indefiniteArticleParameter();

		multipleWordStringBuffer = new StringBuffer( firstWordTypeItem.itemString() );
		existingMultipleWordStringBuffer = new StringBuffer( multipleWordStringBuffer );

		do	{
			isFrenchPreposition = currentReadItem.isFrenchPreposition();

			if( isFrenchPreposition )
				hasFoundFrenchPreposition = true;

			currentWordTypeNr = currentReadItem.wordTypeNr();

			// The current word has the same word type as the first word
			if( isFirstWordTypeNumeral ||
			isFrenchPreposition ||
			firstWordTypeNr == currentWordTypeNr ||

			// Allow mix of singular and plural nouns
			( adminItem_.isNounWordType( firstWordTypeNr ) &&
			adminItem_.isNounWordType( currentWordTypeNr ) ) )
				{
				if( ( currentWordItem = currentReadItem.readWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I found a read item without read word" );

				if( ( currentWordTypeItem = currentWordItem.activeWordTypeItem( currentWordTypeNr ) ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type item of the current read word" );

				nWordParts++;
				lastReadItem = currentReadItem;

				if( !isChineseCurrentLanguage_ ||
				// Typical for Chinese: Only spaces in multiple proper noun words
				currentWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN )
					{
					multipleWordStringBuffer.append( Constants.SPACE_STRING );
					existingMultipleWordStringBuffer.append( Constants.SPACE_STRING );
					}

				multipleWordStringBuffer.append( currentWordTypeItem.itemString() );

				if( firstWordTypeNr != currentWordTypeNr &&
				( differentWordTypeItem = currentWordItem.activeWordTypeItem( firstWordTypeNr ) ) != null )
					{
					hasFoundWordWithDifferentWordType = true;
					existingMultipleWordStringBuffer.append( differentWordTypeItem.itemString() );
					}
				else
					existingMultipleWordStringBuffer.append( currentWordTypeItem.itemString() );
				}
			else
				hasCurrentWordWithSameWordType = false;
			}
		while( hasCurrentWordWithSameWordType &&
		( currentReadItem = currentReadItem.nextReadItem() ) != null &&
		!currentReadItem.isSymbol() );

		currentReadItem = secondReadItem;

		do	{
			currentWordTypeNr = ( hasFoundFrenchPreposition ? firstWordTypeNr : currentReadItem.wordTypeNr() );

			if( ( currentWordItem = currentReadItem.readWordItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "I found a read item without read word" );

			createdMultipleWordItem = null;
			createdWordItem = null;
			multipleWordItem = null;

			if( hasFoundWordWithDifferentWordType )
				{
				if( ( wordResult = adminItem_.findWordTypeInAllWords( false, firstWordTypeNr, existingMultipleWordStringBuffer.toString(), null ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find an exact word type in all words" );

				if( ( createdWordItem = wordResult.foundWordItem ) != null )
					{
					if( createdWordItem.addWordType( false, firstWordTypeItem.isProperNounPrecededByDefiniteArticle( firstWordTypeItem.definiteArticleParameter() ), adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, currentWordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add the word type to multiple word \"" + createdWordItem.anyWordTypeString() + "\"" );

					createdMultipleWordItem = createdWordItem;
					multipleWordItem = createdWordItem;
					}
				}

			if( createdWordItem == null )
				{
				if( ( wordResult = adminItem_.addWord( false, true, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, firstReadItem.wordParameter(), ( isFirstWordTypeNumeral ? firstWordTypeNr : currentWordTypeNr ), multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the multiple word" );

				createdMultipleWordItem = wordResult.createdWordItem;
				multipleWordItem = ( createdMultipleWordItem == null ? wordResult.foundWordItem : createdMultipleWordItem );
				foundWordTypeItem = wordResult.foundWordTypeItem;

				if( foundWordTypeItem != null )
					{
					if( adjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER &&
					!foundWordTypeItem.hasAdjectiveParameter() &&
					// Set adjective parameter
					foundWordTypeItem.setAdjectiveParameter( adjectiveParameter ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to set the adjective parameter of a singular noun" );

					if( definiteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER &&
					!foundWordTypeItem.hasDefiniteArticleParameter() &&
					// Set definite article parameter
					foundWordTypeItem.setDefiniteArticleParameter( definiteArticleParameter ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to set the definite article parameter of a singular noun" );

					if( indefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
					!foundWordTypeItem.hasIndefiniteArticleParameter() &&
					// Set indefinite article parameter
					foundWordTypeItem.setIndefiniteArticleParameter( indefiniteArticleParameter ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to set the indefinite article parameter of a singular noun" );
					}
				}

			if( createdMultipleWordItem != null )
				{
				// Link both words to the created multiple word
				if( firstWordItem.addMultipleWord( nWordParts, firstWordTypeNr, createdMultipleWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the first multiple word" );

				if( ( currentWordItem = currentReadItem.readWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The current word item is undefined" );

				if( currentWordItem.addMultipleWord( nWordParts, firstWordTypeNr, createdMultipleWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the second multiple word" );
				}

			if( multipleWordItem == null )
				return adminItem_.startError( 1, moduleNameString_, "The multiple word is undefined" );

			if( firstReadItem.changeReadWord( ( isFirstWordTypeNumeral ? firstWordTypeNr : currentWordTypeNr ), multipleWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete an active read item" );

			deletedReadItem = currentReadItem;
			currentReadItem = ( deletedReadItem == lastReadItem ? null : currentReadItem.nextReadItem() );

			if( adminItem_.deleteReadItem( deletedReadItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the second read item" );
			}
		while( currentReadItem != null );

		return Constants.RESULT_OK;
		}

	private byte addReadSpecification( boolean isAction, boolean isNewStart, short selectionLevel, String readUserSentenceString )
		{
		boolean isConditional;
		boolean isSelection = ( selectionListNr_ != Constants.NO_LIST_NR );
		short tempVerbParameter;
		short imperativeVerbParameter = Constants.NO_IMPERATIVE_PARAMETER;
		ReadItem currentGeneralizationReadItem = startGeneralizationReadItem_;
		SpecificationItem userSpecificationItem;

		if( currentGeneralizationReadItem == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The start generalization read item is undefined" );

		if( endGeneralizationReadItem_ == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The end generalization read item is undefined" );

		if( !isSelection &&
		GlobalVariables.nUserGeneralizationWords == 1 &&
		GlobalVariables.nUserSpecificationWords > 1 &&
		GlobalVariables.nUserRelationWords > 1 )
			{
			// Ambiguity: Missing relation context
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MULTIPLE_SPECIFICATIONS_RELATIONS ) != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to write an interface warning about ambiguity" );
			}
		else
			{
			if( wasPreviousCommandUndoOrRedo_ )
				{
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				wasPreviousCommandUndoOrRedo_ = false;
				}

			isConditional = ( isSelection ||
							isConditional_ );

			adminItem_.initializeAdminVariables();

			do	{
				switch( currentGeneralizationReadItem.grammarParameter )
					{
					case Constants.GRAMMAR_IMPERATIVE:
						// Typical for Chinese
						if( isChineseCurrentLanguage_ &&
						( tempVerbParameter = currentGeneralizationReadItem.readAheadChineseImperativeVerbParameter() ) > Constants.NO_WORD_PARAMETER )
							imperativeVerbParameter = tempVerbParameter;
						else
							imperativeVerbParameter = currentGeneralizationReadItem.wordParameter();
						// Don't insert a break statement

					case Constants.GRAMMAR_GENERALIZATION_WORD:
						if( adminItem_.addUserSpecification( isAction, isAssignment_, isCharacteristicFor_, isConditional, isInactiveAssignment_, isArchivedAssignment_, isEveryGeneralization_, isExclusiveSpecification_, isNegative_, isNewStart, isPartOf_, isPossessive_, isSpecific_, isSpecificationGeneralization_, isUncountableGeneralizationNoun_, isUniqueUserRelation_, imperativeVerbParameter, prepositionParameter_, questionParameter_, selectionLevel, selectionListNr_, userAssumptionLevel_, generalizationContextNr_, specificationContextNr_, currentGeneralizationReadItem, startSpecificationReadItem_, endSpecificationReadItem_, startRelationReadItem_, endRelationReadItem_ ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to add the read user specifications" );

						if( !isSelection &&
						( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
						adminItem_.prepareIntegrityCheckOfStoredUserSentence( isChineseCurrentLanguage_, userSpecificationItem, readUserSentenceString ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to prepare the integrity check of the stored user sentence" );
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			currentGeneralizationReadItem != endGeneralizationReadItem_ &&
			( currentGeneralizationReadItem = currentGeneralizationReadItem.nextReadItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte checkAllWordsForStructuralIntegrity()
		{
		WordItem currentSpecificationWordItem;

		if( adminItem_.checkForUnprocessedNegativeConclusion() == Constants.RESULT_OK &&
		( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem.checkStructuralIntegrityInWord() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to check the structural integrity in word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( !GlobalVariables.hasDisplayedIntegrityWarning &&
			( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte checkForDuplicateSelection()
		{
		boolean hasFoundDuplicateSelection = false;
		int duplicateConditionSentenceNr;
		DuplicateResultType duplicateResult;

		if( ( duplicateResult = adminItem_.checkForDuplicateCondition() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to check if the condition selection part is duplicate" );

		if( ( duplicateConditionSentenceNr = duplicateResult.duplicateConditionSentenceNr ) > Constants.NO_SENTENCE_NR )
			{
			if( ( duplicateResult = adminItem_.checkForDuplicateSelectionPart( true, duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check if the action selection part is duplicate" );

			if( duplicateResult.hasFoundDuplicateSelection )
				{
				if( GlobalVariables.adminAlternativeList == null )
					hasFoundDuplicateSelection = true;
				else
					{
					if( ( duplicateResult = adminItem_.checkForDuplicateSelectionPart( false, duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );

					if( duplicateResult.hasFoundDuplicateSelection )
						hasFoundDuplicateSelection = true;
					}
				}

			if( hasFoundDuplicateSelection )
				adminItem_.deleteSentences( GlobalVariables.currentSentenceNr );
			}

		return Constants.RESULT_OK;
		}

	private byte deleteAllWordTypesOfCurrentSentence()
		{
		if( deleteAllWordTypesOfCurrentSentence( true ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete all word types of the active read list" );

		if( deleteAllWordTypesOfCurrentSentence( false ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete all word types of the inactive read list" );

		return Constants.RESULT_OK;
		}

	private byte deleteAllWordTypesOfCurrentSentence( boolean isActiveItems )
		{
		ReadItem currentReadItem = ( isActiveItems ? adminItem_.firstActiveReadItem() : adminItem_.firstInactiveReadItem() );
		WordItem currentWordItem;

		while( currentReadItem != null )
			{
			currentWordItem = currentReadItem.readWordItem();

			// Skip text
			if( currentWordItem != null &&
			!currentWordItem.isDeletedItem() )
				{
				if( currentWordItem.deleteAllWordTypesOfCurrentSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete all word types of the current sentence in word \"" + currentWordItem.anyWordTypeString() + "\"" );

				if( !currentWordItem.hasAnyWordType() &&
				adminItem_.deleteWordItem( currentWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete word \"" + currentWordItem.anyWordTypeString() + "\"" );
				}

			currentReadItem = currentReadItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordsAndWordTypes()
		{
		// Active read items
		if( deleteUnusedWordsAndWordTypes( true ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete unused word types in the active read list" );

		// Inactive read items
		if( deleteUnusedWordsAndWordTypes( false ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete unused word types in the inactive read list" );

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordsAndWordTypes( boolean isActiveItems )
		{
		boolean isSameWordOrderNr;
		short nReadWordReferences;
		short previousWordOrderNr = Constants.NO_ORDER_NR;
		WordTypeItem currentWordTypeItem;
		ReadItem currentReadItem = ( isActiveItems ? adminItem_.firstActiveReadItem() : adminItem_.firstInactiveReadItem() );
		WordItem currentReadWordItem;
		ShortResultType shortResult;

		while( currentReadItem != null )
			{
			isSameWordOrderNr = ( currentReadItem.wordOrderNr() == previousWordOrderNr );
			previousWordOrderNr = currentReadItem.wordOrderNr();

			// Skip text
			if( ( currentReadWordItem = currentReadItem.readWordItem() ) != null )
				{
				if( !isActiveItems ||

				// More word types for this word number
				( isSameWordOrderNr &&
				!currentReadItem.isSingularNoun() ) )
					{
					if( ( currentWordTypeItem = currentReadItem.activeReadWordTypeItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type of an active read word" );

					if( currentWordTypeItem.hasCurrentCreationSentenceNr() )
						{
						if( ( shortResult = adminItem_.getNumberOfReadWordReferences( currentReadItem.wordTypeNr(), currentReadWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to get the number of read word references" );

						if( ( nReadWordReferences = shortResult.shortValue ) < 1 )
							return adminItem_.startError( 1, moduleNameString_, "I found an invalid number of read word references" );

						if( nReadWordReferences == 1 )
							{
							if( currentReadWordItem.deleteWordType( currentWordTypeItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to delete an unused word type item" );

							if( !currentReadWordItem.hasAnyWordType() &&
							adminItem_.deleteWordItem( currentReadWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to delete an unused word item" );
							}
						}
					}

				if( !isActiveItems ||
				isSameWordOrderNr )
					{
					if( adminItem_.deleteReadItem( currentReadItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to delete an active read item" );

					currentReadItem = adminItem_.nextReadListItem();
					}
				else
					currentReadItem = currentReadItem.nextReadItem();
				}
			else
				currentReadItem = currentReadItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}

	private byte findGrammarPath( short grammarLevel, short lastCreatedWordOrderNr, GrammarItem parseGrammarItem )
		{
		boolean isChoice;
		boolean isOption;
		boolean isWaitingForChoiceEnd;
		boolean isWaitingForNewStart;
		short choiceStartWordOrderNr = Constants.NO_ORDER_NR;
		short previousWordOrderNr;
		short startWordOrderNr;
		GrammarItem definitionParseGrammarItem;
		BoolResultType boolResult;

		if( grammarLevel >= Constants.MAX_GRAMMAR_LEVEL )
			return adminItem_.startError( 1, moduleNameString_, "The given grammar level is too high: #" + grammarLevel );

		if( parseGrammarItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given parse grammar item is undefined" );

		if( grammarLevel == Constants.NO_GRAMMAR_LEVEL )
			currentParseWordOrderNr_ = Constants.NO_ORDER_NR;

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
				parseGrammarItem.isNewStart() )
					isWaitingForNewStart = false;

				if( !isWaitingForNewStart &&
				!isWaitingForChoiceEnd )
					{
					previousWordOrderNr = currentParseWordOrderNr_;

					if( parseGrammarItem.isOptionStart() )
						isOption = true;

					if( parseGrammarItem.isChoiceStart() )
						{
						isChoice = true;
						choiceStartWordOrderNr = currentParseWordOrderNr_;
						}

					if( parseGrammarItem.isDefinitionStart() )
						{
						if( ( boolResult = adminItem_.selectMatchingWordType( currentParseWordOrderNr_, definitionParseGrammarItem.grammarParameter(), definitionParseGrammarItem.grammarWordTypeNr() ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to select a matching word type" );

						// Has found matching word type
						if( boolResult.booleanValue )
							currentParseWordOrderNr_++;
						}
					else
						{
						if( grammarLevel + 1 >= Constants.MAX_GRAMMAR_LEVEL )
							return adminItem_.startError( 1, moduleNameString_, "There is probably an endless loop in the grammar definitions, because I reached the grammar level: #" + grammarLevel );

						if( parseGrammarItem.definitionGrammarItem == null )
							return adminItem_.startError( 1, moduleNameString_, "Grammar word \"" + parseGrammarItem.grammarString() + "\" isn't defined in the grammar file" );

						if( currentParseWordOrderNr_ < lastCreatedWordOrderNr &&
						findGrammarPath( (short)( grammarLevel + 1 ), lastCreatedWordOrderNr, parseGrammarItem.definitionGrammarItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find the grammar path at grammar word \"" + parseGrammarItem.grammarString() + "\"" );

						// Unsuccessful
						if( currentParseWordOrderNr_ == previousWordOrderNr )
							{
							if( isOption )
								isWaitingForNewStart = true;

							if( isChoice )
								{
								if( !parseGrammarItem.isChoiceEnd )
									isWaitingForNewStart = true;

								currentParseWordOrderNr_ = choiceStartWordOrderNr;
								}

							if( startWordOrderNr < currentParseWordOrderNr_ &&
							adminItem_.activateInactiveReadWords( currentParseWordOrderNr_ ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to activate inactive read words from position: " + currentParseWordOrderNr_ );
							}
						}
					}

				if( parseGrammarItem.isOptionEnd )
					isOption = false;

				if( parseGrammarItem.isChoiceEnd )
					{
					isChoice = false;
					isWaitingForChoiceEnd = false;
					}

				parseGrammarItem = parseGrammarItem.nextGrammarItem();

				if( isChoice &&
				!isWaitingForChoiceEnd &&
				currentParseWordOrderNr_ > previousWordOrderNr &&
				parseGrammarItem != null &&
				parseGrammarItem.isNewStart() )
					isWaitingForChoiceEnd = true;
				}
			while( ( isWaitingForNewStart ||
			isWaitingForChoiceEnd ||
			currentParseWordOrderNr_ > previousWordOrderNr ) &&

			parseGrammarItem != null &&
			definitionParseGrammarItem.activeSentenceNr() == parseGrammarItem.activeSentenceNr() );

			if( !isWaitingForNewStart &&
			currentParseWordOrderNr_ > startWordOrderNr &&
			currentParseWordOrderNr_ == previousWordOrderNr )
				currentParseWordOrderNr_ = startWordOrderNr;
			}
		while( currentParseWordOrderNr_ == startWordOrderNr &&
		definitionParseGrammarItem != null &&
		( parseGrammarItem = definitionParseGrammarItem.nextDefinitionGrammarItem ) != null );

		if( ( previousWordOrderNr > startWordOrderNr ||
		currentParseWordOrderNr_ > startWordOrderNr ) &&

		definitionParseGrammarItem != null &&
		definitionParseGrammarItem.isGrammarDefinition() &&

		// Set grammar parameter
		adminItem_.setGrammarParameter( ( currentParseWordOrderNr_ > startWordOrderNr ), startWordOrderNr, ( currentParseWordOrderNr_ > startWordOrderNr ? currentParseWordOrderNr_ : previousWordOrderNr ), definitionParseGrammarItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to set the grammar parameter of the read words between the positions " + startWordOrderNr + " and " + currentParseWordOrderNr_ );

		return Constants.RESULT_OK;
		}

	private static byte initializeVariablesInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = GlobalVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.initializeVariablesInWord();
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte parseSentence( StringBuffer readUserSentenceStringBuffer )
		{
		boolean isAction = false;
		boolean isChineseCurrentLanguage = adminItem_.isChineseCurrentLanguage();
		boolean isNewStart = true;
		short selectionLevel = Constants.NO_SELECTION_LEVEL;
		short wordOrderNr;
		short wordParameter;

		isChineseCurrentLanguage_ = isChineseCurrentLanguage;
		isUserImperativeSentence_ = false;

		selectionListNr_ = Constants.NO_LIST_NR;

		linkedGeneralizationReadItem_ = null;
		startGeneralizationReadItem_ = null;
		endGeneralizationReadItem_ = null;

		adminItem_.initializeLinkedWord();

		if( readUserSentenceStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read user sentence string buffer is undefined" );

		if( ( currentReadItem_ = adminItem_.firstActiveReadItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I failed to get the first read item" );

		do	{
			if( !currentReadItem_.isSeparator() )
				{
				wordOrderNr = currentReadItem_.wordOrderNr();
				wordParameter = currentReadItem_.wordParameter();

				switch( currentReadItem_.grammarParameter )
					{
					case Constants.GRAMMAR_GENERALIZATION_SPECIFICATION:
					case Constants.GRAMMAR_GENERALIZATION_PART:
					case Constants.GRAMMAR_GENERALIZATION_WORD:
					case Constants.GRAMMAR_SPECIFICATION_PART:
					case Constants.GRAMMAR_ASSIGNMENT_PART:
					case Constants.GRAMMAR_SPECIFICATION_WORD:
					case Constants.GRAMMAR_RELATION_PART:
					case Constants.GRAMMAR_RELATION_WORD:
					case Constants.GRAMMAR_GENERALIZATION_ASSIGNMENT:
					case Constants.GRAMMAR_SPECIFICATION_ASSIGNMENT:
					case Constants.GRAMMAR_RELATION_ASSIGNMENT:
					case Constants.GRAMMAR_VERB:
					case Constants.GRAMMAR_QUESTION_VERB:
					case Constants.GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
					case Constants.GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
						if( scanSpecification() != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to scan the generalization-specification" );

						if( addReadSpecification( isAction, isNewStart, selectionLevel, readUserSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to add the read specification" );

						isNewStart = false;

						break;

					case Constants.GRAMMAR_IMPERATIVE:
						if( readImperative( isAction, isNewStart, selectionLevel ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to read an imperative" );

						isNewStart = false;
						isUserImperativeSentence_ = true;

						break;

					case Constants.GRAMMAR_STATEMENT_ANSWER:
						if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to write an interface warning" );

						break;

					case Constants.GRAMMAR_SENTENCE_CONJUNCTION:
						switch( wordParameter )
							{
							case Constants.WORD_PARAMETER_SYMBOL_COMMA:
							case Constants.WORD_PARAMETER_CONJUNCTION_AND:
							// Typical for Dutch: "in zowel ... als ..."
							case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:
								break;

							case Constants.WORD_PARAMETER_CONJUNCTION_OR:
								isNewStart = true;

								break;

							default:
								return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I found an illegal conjunction word parameter: " + wordParameter );
							}

						break;

					case Constants.GRAMMAR_SELECTION:
					case Constants.WORD_PARAMETER_SELECTION_IF:	// In case "then" is missing
					case Constants.WORD_PARAMETER_SELECTION_THEN:
					case Constants.WORD_PARAMETER_SELECTION_ELSE:
						switch( wordParameter )
							{
							case Constants.WORD_PARAMETER_SELECTION_IF:
								if( selectionListNr_ != Constants.NO_LIST_NR &&
								++selectionLevel == Constants.MAX_LEVEL )
									return adminItem_.startSystemError( 1, moduleNameString_, "Selection overflow in list <" + adminItem_.adminListChar( selectionListNr_ ) + "> at word position " + wordOrderNr );

								isNewStart = true;
								selectionListNr_ = Constants.ADMIN_CONDITION_LIST;

								break;

							case Constants.WORD_PARAMETER_SELECTION_THEN:
								isNewStart = true;
								isAction = true;
								selectionListNr_ = Constants.ADMIN_ACTION_LIST;

								break;

							case Constants.WORD_PARAMETER_SELECTION_ELSE:
								isNewStart = true;
								isAction = false;
								selectionListNr_ = Constants.ADMIN_ALTERNATIVE_LIST;

								break;

							default:
								return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I found an illegal selection word" );
							}

						break;

					default:
						if( adminItem_.isCurrentlyTesting() ||
						adminItem_.isSystemStartingUp() )
							return adminItem_.startError( 1, moduleNameString_, "I found an unknown word in sentence \"" + readUserSentenceStringBuffer + "\" at position " + wordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + wordParameter );

						return adminItem_.startError( 1, moduleNameString_, "I found an unknown word at position " + wordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + wordParameter );
					}
				}
			}
		while( currentReadItem_ != null &&
		!GlobalVariables.hasDisplayedWarning &&
		!adminItem_.hasRequestedRestart() &&
		( currentReadItem_ = currentReadItem_.nextReadItem() ) != null );

		if( selectionListNr_ != Constants.NO_LIST_NR &&
		checkForDuplicateSelection() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to check on a duplicate selection" );

		return Constants.RESULT_OK;
		}

	private byte processSentence( StringBuffer readUserSentenceStringBuffer )
		{
		boolean isInterpretationSuccessful;
		short currentLanguageNr;
		short lastCreatedWordOrderNr;
		short nLanguages;
		short originalLanguageNr = GlobalVariables.currentLanguageNr;
		GrammarItem readingGrammarItem;
		BoolResultType boolResult;

		if( readUserSentenceStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read user sentence string buffer is undefined" );

		if( ( nLanguages = adminItem_.nLanguages() ) <= Constants.NO_LANGUAGE_NR )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any language" );

		currentLanguageNr = originalLanguageNr;

		if( nLanguages < currentLanguageNr )
			return adminItem_.startError( 1, moduleNameString_, "The current language number exceeds the number of languages" );

		do	{
			adminItem_.deleteTemporaryReadList();

			// Need to switch language
			if( currentLanguageNr != GlobalVariables.currentLanguageNr &&
			assignLanguage( currentLanguageNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );

			if( adminItem_.checkGrammarOfCurrentLanguage() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check the grammar" );

			// Typical for French: Merged words
			if( ( boolResult = adminItem_.expandMergedWordsInReadSentenceOfCurrentLanguage( readUserSentenceStringBuffer ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to expand the merged words in the read user sentence string" );

			// Current language has merged words
			if( boolResult.booleanValue &&
			GlobalVariables.readUserSentenceStringBuffer != null )
				readUserSentenceStringBuffer = GlobalVariables.readUserSentenceStringBuffer;

			// Create read words from a given sentence
			if( ( boolResult = adminItem_.createReadWords( isChineseCurrentLanguage_, readUserSentenceStringBuffer.toString() ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to create the read words" );

			isInterpretationSuccessful = false;
			adminItem_.clearLastActivatedWordOrderNr();

			// Has created all read words
			if( boolResult.booleanValue )
				{
				lastCreatedWordOrderNr = adminItem_.lastCreatedWordOrderNr();
				readingGrammarItem = adminItem_.firstCurrentLanguageReadingGrammarItem();

				do	{
					if( findGrammarPath( Constants.NO_GRAMMAR_LEVEL, lastCreatedWordOrderNr, readingGrammarItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find the grammar path for a sentence with language \"" + adminItem_.languageNameString( currentLanguageNr ) + "\"" );

					isInterpretationSuccessful = ( currentParseWordOrderNr_ == lastCreatedWordOrderNr );

					if( !isInterpretationSuccessful &&
					( boolResult = adminItem_.findMoreInterpretations() ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find more interpretations" );
					}
				while( !isInterpretationSuccessful &&
				// Has found another interpretation
				boolResult.booleanValue );
				}

			if( isInterpretationSuccessful )
				{
				if( deleteUnusedWordsAndWordTypes() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete the unused words and word types of the read words" );

				if( parseSentence( readUserSentenceStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to parse the sentence" );
				}
			else
				{
				if( deleteAllWordTypesOfCurrentSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete words and word types of the read words" );

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
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				wasPreviousCommandUndoOrRedo_ = false;
				}

			if( GlobalVariables.currentLanguageNr != originalLanguageNr &&
			// Restore the original language
			assignLanguage( originalLanguageNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language after an interpretation failure" );

			if( currentParseWordOrderNr_ == Constants.NO_ORDER_NR )
				{
				if( adminItem_.isSystemStartingUp() )
					return adminItem_.startSystemError( 1, moduleNameString_, "I don't understand this sentence. Please make sure you enter a sentence within my limited grammar definition" );

				if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_NOT_CONFORM_GRAMMAR ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( adminItem_.isSystemStartingUp() )
					return adminItem_.startSystemError( 1, moduleNameString_, "I don't understand the sentence from the word at position: " + currentParseWordOrderNr_ );

				if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START, currentParseWordOrderNr_, Constants.INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_END ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte readImperative( boolean isAction, boolean isNewStart, short selectionLevel )
		{
		boolean hasFoundChineseImperativeVerb = false;
		short executionNounWordParameter = Constants.NO_WORD_PARAMETER;
		short imperativeVerbParameter = Constants.NO_IMPERATIVE_PARAMETER;
		short specificationWordTypeNr;
		short tempNounWordParameter;
		short tempVerbParameter;
		ReadItem currentReadItem;
		ReadItem specificationReadItem;
		WordItem imperativeVerbWordItem = null;

		startGeneralizationReadItem_ = null;
		endGeneralizationReadItem_ = null;

		if( scanSpecification() != Constants.RESULT_OK )
			return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to scan the generalization-specification" );

		if( endGeneralizationReadItem_ == null ||
		( currentReadItem = startGeneralizationReadItem_ ) == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I couldn't find the imperative" );

		do	{
			if( currentReadItem.isVerb() )
				{
				// Typical for Chinese
				if( isChineseCurrentLanguage_ &&
				( tempVerbParameter = currentReadItem.readAheadChineseImperativeVerbParameter() ) > Constants.NO_WORD_PARAMETER )
					{
					hasFoundChineseImperativeVerb = true;
					imperativeVerbParameter = tempVerbParameter;
					imperativeVerbWordItem = adminItem_.predefinedWordItem( tempVerbParameter );
					}
				else
					{
					imperativeVerbParameter = currentReadItem.wordParameter();
					imperativeVerbWordItem = currentReadItem.readWordItem();
					}
				}
			else
				{
				if( currentReadItem.isSpecificationWord() &&
				!currentReadItem.isNounHeadOrTail() &&
				( tempNounWordParameter = currentReadItem.wordParameter() ) > Constants.NO_WORD_PARAMETER )
					executionNounWordParameter = tempNounWordParameter;
				}
			}
		while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );

		// Imperative word has no specifications
		if( ( specificationReadItem = startSpecificationReadItem_ ) == null )
			{
			if( imperativeVerbWordItem != null &&
			imperativeVerbWordItem.isImperativeVerbUndoOrRedo() )
				wasPreviousCommandUndoOrRedo_ = true;
			else
				{
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				wasPreviousCommandUndoOrRedo_ = false;
				}

			if( adminItem_.executeImperative( true, Constants.NO_LIST_NR, Constants.NO_WORD_PARAMETER, startGeneralizationReadItem_.wordParameter(), Constants.NO_WORD_TYPE_NR, Constants.MAX_PROGRESS, startGeneralizationReadItem_.readString, startGeneralizationReadItem_.readWordItem(), null, startRelationReadItem_, endRelationReadItem_, null, null ) != Constants.RESULT_OK )
				{
				if( startGeneralizationReadItem_ == null )
					return adminItem_.addError( 1, moduleNameString_, "I failed to execute the single imperative" );

				return adminItem_.addError( 1, moduleNameString_, "I failed to execute the single imperative at position " + startGeneralizationReadItem_.wordOrderNr() );
				}
			}
		else
			{
			// Imperative word has specifications
			if( endSpecificationReadItem_ == null )
				return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The end specification read item is undefined" );

			if( addReadSpecification( isAction, isNewStart, selectionLevel, null ) != Constants.RESULT_OK )
				{
				if( startGeneralizationReadItem_ == null ||
				endGeneralizationReadItem_ == null )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the generalization part of the read specification" );

				return adminItem_.addError( 1, moduleNameString_, "I failed to add the generalization part of the read specification between the positions " + startGeneralizationReadItem_.wordOrderNr() + " and " + endGeneralizationReadItem_.wordOrderNr() );
				}

			if( !GlobalVariables.hasDisplayedWarning &&
			selectionListNr_ == Constants.NO_LIST_NR )
				{
				do	{
					if( specificationReadItem.isImperativeNoun() )
						{
						specificationWordTypeNr = specificationReadItem.wordTypeNr();

						if( ( !hasFoundChineseImperativeVerb ||
						// Typical for Chinese
						specificationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ) &&

						// Execute imperative
						adminItem_.executeImperative( true, Constants.NO_LIST_NR, executionNounWordParameter, imperativeVerbParameter, specificationWordTypeNr, Constants.MAX_PROGRESS, specificationReadItem.readString, imperativeVerbWordItem, specificationReadItem.readWordItem(), startRelationReadItem_, endRelationReadItem_, null, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to execute an imperative with specifications" );
						}
					}
				while( startGeneralizationReadItem_ == endGeneralizationReadItem_ &&
				// Loop shouldn't end if virtual list prepositions are used to for example display justification reports
				specificationReadItem != endSpecificationReadItem_ &&
				!specificationReadItem.isVirtualListPreposition() &&
				// Still active read items available
				adminItem_.firstActiveReadItem() != null &&
				!GlobalVariables.hasDisplayedWarning &&
				( specificationReadItem = specificationReadItem.nextReadItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte scanSpecification()
		{
		boolean hasGeneralizationArticle = false;
		boolean isFrenchPreposition;
		boolean isSameWordTypeAsPreviousWord;
		boolean isStop = false;
		short currentWordParameter;
		short currentWordTypeNr;
		short generalizationAdjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
		short generalizationDefiniteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		short generalizationIndefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		short previousWordTypeNr = Constants.NO_WORD_TYPE_NR;
		ReadItem nextReadItem;
		ReadItem previousReadItem = null;
		WordItem currentReadWordItem;
		WordTypeItem currentReadWordTypeItem;
		WordTypeItem generalizationWordTypeItem;
		ContextResultType contextResult;
		WordEndingResultType wordEndingResult;

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

//		answerParameter_ = Constants.NO_ANSWER_PARAMETER;
		currentParseWordOrderNr_ = Constants.NO_ORDER_NR;
		prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
		questionParameter_ = Constants.NO_QUESTION_PARAMETER;
		userAssumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

		generalizationContextNr_ = Constants.NO_CONTEXT_NR;
		specificationContextNr_ = Constants.NO_CONTEXT_NR;

		startSpecificationReadItem_ = null;
		endSpecificationReadItem_ = null;
		startRelationReadItem_ = null;
		endRelationReadItem_ = null;

		GlobalVariables.nUserGeneralizationWords = 0;
		GlobalVariables.nUserSpecificationWords = 0;
		GlobalVariables.nUserRelationWords = 0;

		if( linkedGeneralizationReadItem_ == null )
			{
			startGeneralizationReadItem_ = null;
			endGeneralizationReadItem_ = null;
			}

		if( currentReadItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The read item is undefined" );

		do	{
			isFrenchPreposition = currentReadItem_.isFrenchPreposition();
			currentWordParameter = currentReadItem_.wordParameter();
			currentWordTypeNr = currentReadItem_.wordTypeNr();
			currentReadWordItem = currentReadItem_.readWordItem();

			setVariables( currentWordParameter );

			isSameWordTypeAsPreviousWord = ( isFrenchPreposition ||
											previousWordTypeNr == currentWordTypeNr ||

											// Allow mix of singular and plural nouns
											( adminItem_.isNounWordType( previousWordTypeNr ) &&
											adminItem_.isNounWordType( currentWordTypeNr ) ) );

			switch( currentReadItem_.grammarParameter )
				{
				case Constants.GRAMMAR_STATEMENT:
					// Typical for Chinese
					if( isChineseCurrentLanguage_ &&
					currentReadItem_.isQuestionMark() )
						questionParameter_ = Constants.WORD_PARAMETER_SINGULAR_VERB_IS;

					break;

/*				case Constants.GRAMMAR_STATEMENT_ANSWER:
					answerParameter_ = currentReadItem_.grammarParameter;
					break;
*/
				// Assignment generalization-specification
				case Constants.GRAMMAR_GENERALIZATION_ASSIGNMENT:
					isAssignment_ = true;

					// Don't insert a break statement here

				case Constants.GRAMMAR_IMPERATIVE:
				case Constants.GRAMMAR_GENERALIZATION_SPECIFICATION:
				case Constants.GRAMMAR_GENERALIZATION_PART:
				case Constants.GRAMMAR_GENERALIZATION_WORD:
					switch( currentWordTypeNr )
						{
						case Constants.WORD_TYPE_ADJECTIVE:
							if( adminItem_.isAdjectiveParameter( currentReadItem_.wordParameter() ) )
								generalizationAdjectiveParameter = currentReadItem_.wordParameter();

							if( currentReadItem_.isAdjectivePrevious() )
								{
								isAssignment_ = true;
								isInactiveAssignment_ = true;
								}

							if( currentReadItem_.isAdjectiveEvery() )
								isEveryGeneralization_ = true;

							break;

						case Constants.WORD_TYPE_ARTICLE:
							hasGeneralizationArticle = true;

							if( currentReadItem_.isDefiniteArticleParameter( currentReadItem_.wordParameter() ) )
								generalizationDefiniteArticleParameter = currentReadItem_.wordParameter();
							else
								generalizationIndefiniteArticleParameter = currentReadItem_.wordParameter();

							break;

						case Constants.WORD_TYPE_PROPER_NOUN:
						case Constants.WORD_TYPE_NOUN_SINGULAR:
						case Constants.WORD_TYPE_NOUN_PLURAL:
							if( currentReadItem_.isNounPartOf() )
								isPartOf_ = true;

							if( currentReadItem_.isGeneralizationWord() &&
							currentReadWordItem != null )
								{
								if( isSameWordTypeAsPreviousWord )
									{
									if( addMultipleWord( generalizationAdjectiveParameter, generalizationDefiniteArticleParameter, generalizationIndefiniteArticleParameter, previousReadItem, currentReadItem_ ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple generalization word" );

									// Current read item is deleted
									currentReadItem_ = previousReadItem;
									}
								else
									{
									previousGeneralizationWordTypeNr_ = currentWordTypeNr;
									previousGeneralizationWordItem_ = currentReadWordItem;

									GlobalVariables.nUserGeneralizationWords++;
									}

								if( currentReadItem_.isUncountableGeneralizationNoun() )
									isUncountableGeneralizationNoun_ = true;

								currentReadWordItem.isUserGeneralizationWord = true;
								}

							break;

						case Constants.WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_SUBJECTIVE:
						case Constants.WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_OBJECTIVE:
						case Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR:
						case Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR:
						case Constants.WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE:
						case Constants.WORD_TYPE_PERSONAL_PRONOUN_PLURAL_OBJECTIVE:
						case Constants.WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL:
						case Constants.WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL:
							if( generalizationContextNr_ > Constants.NO_CONTEXT_NR )
								return adminItem_.startError( 1, moduleNameString_, "The generalization context number is already assigned" );

							if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a generalization pronoun context" );

							generalizationContextNr_ = contextResult.contextNr;
						}

					if( startGeneralizationReadItem_ == null )
						startGeneralizationReadItem_ = currentReadItem_;

					endGeneralizationReadItem_ = currentReadItem_;

					break;

				case Constants.GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION:
					isStop = true;
					nextReadItem = currentReadItem_.nextReadItem();
					linkedGeneralizationReadItem_ = nextReadItem;
					break;

				case Constants.GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION:
					isExclusiveSpecification_ = true;
					break;

				case Constants.GRAMMAR_RELATION_PART:
				case Constants.GRAMMAR_RELATION_WORD:
					if( currentReadItem_.isPreposition() )
						prepositionParameter_ = currentWordParameter;

					if( currentReadItem_.isRelationWord() &&
					currentReadWordItem != null )
						{
						if( previousWordTypeNr != currentWordTypeNr )
							{
							if( previousWordTypeNr == Constants.WORD_TYPE_NUMERAL )
								// Multiple word type relation
								isSameWordTypeAsPreviousWord = true;
							else
								{
								// Typical for Chinese
								if( currentWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
								previousWordTypeNr == Constants.WORD_TYPE_VERB_SINGULAR )
									isUncountableGeneralizationNoun_ = true;
								}
							}

						if( isSameWordTypeAsPreviousWord )
							{
							if( addMultipleWord( Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, previousReadItem, currentReadItem_ ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple relation word" );

							// Current read item is deleted
							currentReadItem_ = previousReadItem;
							}
						else
							GlobalVariables.nUserRelationWords++;

						currentReadWordItem.isUserRelationWord = true;
						}

					if( startSpecificationReadItem_ == null )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					if( startRelationReadItem_ == null )
						startRelationReadItem_ = currentReadItem_;

					endRelationReadItem_ = currentReadItem_;

					// Typical for English: "... in both ... and ..."
					if( currentWordParameter == Constants.WORD_PARAMETER_NUMERAL_BOTH ||
					// Typical for Dutch: "... in zowel ... als ..."
					currentWordParameter == Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL )
						isExclusiveSpecification_ = true;

					break;

				case Constants.GRAMMAR_ASSIGNMENT_PART:
				case Constants.GRAMMAR_SPECIFICATION_ASSIGNMENT:
					isAssignment_ = true;

					if( currentReadItem_.isDeterminerOrPronoun() )
						{
						if( specificationContextNr_ > Constants.NO_CONTEXT_NR )
							return adminItem_.startError( 1, moduleNameString_, "The specification context number is already assigned" );

						if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification pronoun context" );

						specificationContextNr_ = contextResult.contextNr;
						}

					// Don't insert a break statement here

				case Constants.GRAMMAR_SPECIFICATION_PART:
				case Constants.GRAMMAR_SPECIFICATION_WORD:
				case Constants.GRAMMAR_TEXT:
					if( currentReadItem_.isSpecificationWord() &&
					currentReadWordItem != null )
						{
						if( isSameWordTypeAsPreviousWord )
							{
							if( addMultipleWord( Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, previousReadItem, currentReadItem_ ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple specification word" );

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
									if( startRelationReadItem_ != null )
										{
										if( ( wordEndingResult = adminItem_.analyzeWordEndingWithCurrentLanguage( Constants.WORD_CHINESE_EXCLUSIVE_NOUN, 0, currentReadWordItem.activeWordTypeString( Constants.WORD_TYPE_NOUN_SINGULAR ) ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to check for a Chinese assignment noun" );

										if( wordEndingResult.hasFoundWordEnding )
											isUniqueUserRelation_ = true;
										}
									}
								else
									isUniqueUserRelation_ = true;
								}

							if( currentReadWordItem.isFemale() )
								hasFemaleUserSpecificationWord_ = true;

							GlobalVariables.nUserSpecificationWords++;

							if( currentWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR )
								{
								if( isChineseCurrentLanguage_ )
									{
									if( !isAssignment_ &&
									!isPossessive_ &&
									currentWordParameter == Constants.NO_WORD_PARAMETER &&
									startRelationReadItem_ != null )
										{
										if( ( wordEndingResult = adminItem_.analyzeWordEndingWithCurrentLanguage( Constants.WORD_CHINESE_EXCLUSIVE_NOUN, 0, currentReadWordItem.activeWordTypeString( Constants.WORD_TYPE_NOUN_SINGULAR ) ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to check for a Chinese assignment noun" );

										if( wordEndingResult.hasFoundWordEnding )
											isAssignment_ = true;
										}
									}
								else
									{
									// Typical for Spanish
									if( previousGeneralizationWordItem_ != null &&
									previousGeneralizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NOUN &&
									( currentReadWordTypeItem = currentReadItem_.activeReadWordTypeItem() ) != null &&
									!currentReadWordTypeItem.hasDefiniteArticleParameter() &&
									!currentReadWordTypeItem.hasIndefiniteArticleParameter() &&
									( generalizationWordTypeItem = previousGeneralizationWordItem_.activeWordTypeItem( Constants.WORD_TYPE_PROPER_NOUN ) ) != null &&
									generalizationWordTypeItem.hasFeminineOrMasculineWordEnding() &&

									// Set indefinite article parameter
									currentReadWordTypeItem.setIndefiniteArticleParameter( generalizationWordTypeItem.hasFeminineWordEnding() ? Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to set the indefinite article parameter of a singular noun" );
									}
								}
							}

						currentReadWordItem.isUserSpecificationWord = true;
						}

					if( startSpecificationReadItem_ == null )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					break;

				case Constants.GRAMMAR_RELATION_ASSIGNMENT:
					break;

				case Constants.GRAMMAR_VERB:
					if( startSpecificationReadItem_ == null )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					break;

				case Constants.GRAMMAR_QUESTION_VERB:
					questionParameter_ = currentWordParameter;
					break;

				case Constants.GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
					questionParameter_ = currentWordParameter;

					// Don't insert a break statement here

				case Constants.GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
					isSpecificationGeneralization_ = true;

					if( startSpecificationReadItem_ == null )
						startSpecificationReadItem_ = currentReadItem_;

					endSpecificationReadItem_ = currentReadItem_;

					break;

				default:
					if( previousReadItem == null )
						return adminItem_.startError( 1, moduleNameString_, "I found a word that doesn't belong to an assignment or a specification" );

					isStop = true;
					currentReadItem_ = previousReadItem;
				}

			if( !isFrenchPreposition )
				previousWordTypeNr = currentWordTypeNr;

			previousReadItem = currentReadItem_;
			}
		while( !isStop &&
		( currentReadItem_ = currentReadItem_.nextReadItem() ) != null );

		return Constants.RESULT_OK;
		}

	private ContextResultType addPronounContext( short contextWordTypeNr, WordItem contextWordItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( contextWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The read word of the read ahead item is undefined" );

		if( ( contextResult.contextNr = contextWordItem.contextNr( null ) ) == Constants.NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_.highestContextNrInAllContextWords() ) >= Constants.MAX_CONTEXT_NR )
				return adminItem_.startContextResultSystemError( 1, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		if( contextWordItem.addContext( contextWordTypeNr, Constants.NO_WORD_TYPE_NR, contextResult.contextNr, Constants.NO_COLLECTION_NR, null ) != Constants.RESULT_OK )
			return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to add a pronoun context to word \"" + contextWordItem.anyWordTypeString() + "\"" );

		return contextResult;
		}


	// Constructor

	protected AdminReadSentence( AdminItem adminItem )
		{
		// Private constructed variables

		hasAnyChangeBeenMadeByThisSentence_ = false;
		hasFemaleUserSpecificationWord_ = false;
		isAssignment_ = false;
		isInactiveAssignment_ = false;
		isArchivedAssignment_ = false;
		isCharacteristicFor_ = false;
		isChineseCurrentLanguage_ = false;
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
		isUserImperativeSentence_ = false;
		wasPreviousCommandUndoOrRedo_ = false;

//		answerParameter_ = Constants.NO_ANSWER_PARAMETER;
		currentParseWordOrderNr_ = Constants.NO_ORDER_NR;
		prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
		previousGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
		questionParameter_ = Constants.NO_QUESTION_PARAMETER;
		selectionListNr_ = Constants.NO_LIST_NR;
		userAssumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;

		generalizationContextNr_ = Constants.NO_CONTEXT_NR;
		specificationContextNr_ = Constants.NO_CONTEXT_NR;

		currentReadItem_ = null;
		linkedGeneralizationReadItem_ = null;
		startGeneralizationReadItem_ = null;
		endGeneralizationReadItem_ = null;
		startSpecificationReadItem_ = null;
		endSpecificationReadItem_ = null;
		startRelationReadItem_ = null;
		endRelationReadItem_ = null;

		previousGeneralizationWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected boolean hasAnyChangeBeenMadeByThisSentence()
		{
		return hasAnyChangeBeenMadeByThisSentence_;
		}

	protected boolean hasFemaleUserSpecificationWord()
		{
		return hasFemaleUserSpecificationWord_;
		}

	protected boolean isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	protected boolean isUserQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER );
		}

	protected boolean wasPreviousCommandUndoOrRedo()
		{
		return wasPreviousCommandUndoOrRedo_;
		}

	protected byte processReadSentence( StringBuffer readUserSentenceStringBuffer )
		{
		int startSentenceNr = GlobalVariables.currentSentenceNr;

		previousGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
		previousGeneralizationWordItem_ = null;

		GlobalVariables.hasFoundAnswerToQuestion = false;
		GlobalVariables.isFirstAnswerToQuestion = true;
		GlobalVariables.isQuestionAlreadyAnswered = false;
		GlobalVariables.firstTouchedWordItem = null;

		GlobalVariables.writtenUserSentenceStringBuffer = new StringBuffer();

		if( readUserSentenceStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read user sentence string buffer is undefined" );

		if( initializeVariablesInAllWords() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to initialize variables in all words" );

		if( processSentence( readUserSentenceStringBuffer ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to process sentence: \"" + readUserSentenceStringBuffer + "\"" );

		if( !GlobalVariables.hasDisplayedWarning &&
		!adminItem_.hasRequestedRestart() &&
		startSentenceNr == GlobalVariables.currentSentenceNr )
			{
			checkForChangesMadeByThisSentence();

			// Skip integrity check if no changes are made
			if( hasAnyChangeBeenMadeByThisSentence_ &&
			!isUserImperativeSentence_ &&
			selectionListNr_ == Constants.NO_LIST_NR &&
			adminItem_.checkIntegrityOfStoredUserSentence( readUserSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check the integrity of the stored user sentence \"" + readUserSentenceStringBuffer + "\"" );

			// Has passed integrity check
			if( !GlobalVariables.hasDisplayedWarning )
				{
				// Skip when no change has been made
				if( hasAnyChangeBeenMadeByThisSentence_ )
					{
					// Display self-generated conclusions of the current sentence
					if( adminItem_.writeSelfGeneratedInfo( true, false, false ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated conclusions" );

					// Display self-generated assumptions of the current sentence
					if( adminItem_.writeSelfGeneratedInfo( false, true, false ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated assumptions" );

					// Display self-generated questions of the current sentence
					if( adminItem_.writeSelfGeneratedInfo( false, false, true ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated questions" );

					if( checkAllWordsForStructuralIntegrity() != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "The system has problem with the structural integrity" );
/*
					// In case you are planning to save the data in a database
					if( adminItem_.storeChangesInFutureDatabase() != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to store the changes in a future database" );
*/					}

				if( questionParameter_ > Constants.NO_QUESTION_PARAMETER &&
				adminItem_.answerQuestions() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to answer questions" );
				}
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"You have turned my mourning into joyful dancing.
 *	You have taken away my clothes of mourning and
 *	clothed me with joy,
 *	that I might sing praises to you and not be silent.
 *	O Lord my God, I will give you thanks forever!" (Psalm 30:11-12)
 *************************************************************************/
