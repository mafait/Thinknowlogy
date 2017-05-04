/*	Class:			AdminReadSentence
 *	Supports class:	AdminItem
 *	Purpose:		To read and analyze sentences
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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
	private boolean isConditional_;
	private boolean isEveryGeneralization_;
	private boolean isExclusiveSpecification_;
	private boolean isNegative_;
	private boolean isPartOf_;
	private boolean isPossessive_;
	private boolean isSpecificationGeneralization_;
	private boolean isUniqueUserRelation_;
	private boolean isUserImperativeSentence_;
	private boolean wasPreviousCommandUndoOrRedo_;

	private short assumptionLevel_;
	private short currentParseWordOrderNr_;
	private short prepositionParameter_;
	private short previousGeneralizationWordTypeNr_;
	private short questionParameter_;
	private short selectionListNr_;

	private int generalizationContextNr_;
	private int specificationContextNr_;

	private ReadItem currentReadItem_;
	private ReadItem linkedGeneralizationReadItem_;
	private ReadItem startGeneralizationWordReadItem_;
	private ReadItem endGeneralizationWordReadItem_;
	private ReadItem startSpecificationWordReadItem_;
	private ReadItem endSpecificationWordReadItem_;
	private ReadItem startRelationWordReadItem_;
	private ReadItem endRelationReadItem_;

	private WordItem previousGeneralizationWordItem_;

	private StringBuffer originalReadStringBuffer;


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private void checkForChangesMadeByThisSentence()
		{
		int currentSentenceNr = CommonVariables.currentSentenceNr;
		int highestInUseSentenceNr = adminItem_.highestInUseSentenceNr( false, false, currentSentenceNr );

		hasAnyChangeBeenMadeByThisSentence_ = ( highestInUseSentenceNr >= currentSentenceNr );
		}

	private static void clearReplacingInfoInSpecificationWords()
		{
		WordItem currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = CommonVariables.firstSpecificationWordItem ) != null )
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
			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE:
				assumptionLevel_ = 3;
				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY:
				assumptionLevel_ = 2;
				break;

			case Constants.WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY:
				assumptionLevel_ = 1;
				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_NO:
			case Constants.WORD_PARAMETER_ADVERB_NOT:
			case Constants.WORD_PARAMETER_ADVERB_NOT_FRENCH:
				isNegative_ = true;
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

			case Constants.WORD_PARAMETER_SINGULAR_VERB_HAS:
			case Constants.WORD_PARAMETER_PLURAL_VERB_HAVE:
				isPossessive_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_HAD:
			case Constants.WORD_PARAMETER_PLURAL_VERB_HAD:
				isAssignment_ = true;
				isArchivedAssignment_ = true;
				isPossessive_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_CAN_HAVE:
			case Constants.WORD_PARAMETER_PLURAL_VERB_CAN_HAVE:
				isConditional_ = true;
				isPossessive_ = true;
				break;
			}
		}

	private byte assignLanguage( short newLanguageNr )
		{
		WordItem languageWordItem;
		WordItem predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The predefined noun language word item is undefined" );

		if( ( languageWordItem = predefinedNounLanguageWordItem.languageWordItemInWord( newLanguageNr ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the requested language" );

		CommonVariables.currentLanguageNr = newLanguageNr;
		CommonVariables.currentLanguageWordItem = languageWordItem;

		if( adminItem_.assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, CommonVariables.currentLanguageWordItem, CommonVariables.predefinedNounLanguageWordItem, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language with authorization" );

		return Constants.RESULT_OK;
		}

	private byte addMultipleWord( short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, ReadItem firstReadItem, ReadItem secondReadItem )
		{
		WordResultType wordResult;
		boolean isFrenchPreposition;
		boolean hasFoundFrenchPreposition = false;
		boolean hasCurrentWordWithSameWordType = true;
		boolean hasFoundWordWithDifferentWordType = false;
		short currentWordTypeNr;
		short firstWordTypeNr;
		short nWordParts = 1;
		ReadItem deletedReadItem;
		ReadItem currentReadItem = secondReadItem;
		ReadItem lastReadItem = null;
		WordItem currentWordItem;
		WordItem firstWordItem;
		WordItem multipleWordItem;
		WordItem createdMultipleWordItem;
		WordItem createdWordItem;
		WordTypeItem currentWordTypeItem;
		WordTypeItem differentWordTypeItem;
		WordTypeItem firstWordTypeItem;
		WordTypeItem foundWordTypeItem;
		StringBuffer existingMultipleWordStringBuffer;
		StringBuffer multipleWordStringBuffer;

		if( firstReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first read item is undefined" );

		if( secondReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given second read item is undefined" );

		if( ( firstWordItem = firstReadItem.readWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first read item has no read word" );

		firstWordTypeNr = firstReadItem.wordTypeNr();

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
			if( isFrenchPreposition ||
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
				multipleWordStringBuffer.append( Constants.SPACE_STRING + currentWordTypeItem.itemString() );

				if( firstWordTypeNr != currentWordTypeNr &&
				( differentWordTypeItem = currentWordItem.activeWordTypeItem( firstWordTypeNr ) ) != null )
					{
					hasFoundWordWithDifferentWordType = true;
					existingMultipleWordStringBuffer.append( Constants.SPACE_STRING + differentWordTypeItem.itemString() );
					}
				else
					existingMultipleWordStringBuffer.append( Constants.SPACE_STRING + currentWordTypeItem.itemString() );
				}
			else
				hasCurrentWordWithSameWordType = false;
			}
		while( hasCurrentWordWithSameWordType &&
		( currentReadItem = currentReadItem.nextReadItem() ) != null );

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
					if( createdWordItem.addWordType( false, firstWordTypeItem.isProperNamePrecededByDefiniteArticle( firstWordTypeItem.definiteArticleParameter() ), adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, currentWordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add the word type to multiple word \"" + createdWordItem.anyWordTypeString() + "\"" );

					createdMultipleWordItem = createdWordItem;
					multipleWordItem = createdWordItem;
					}
				}

			if( createdWordItem == null )
				{
				if( ( wordResult = adminItem_.addWord( false, true, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, firstReadItem.wordParameter(), currentWordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the multiple word" );

				createdMultipleWordItem = wordResult.createdWordItem;
				multipleWordItem = ( createdMultipleWordItem == null ? wordResult.foundWordItem : createdMultipleWordItem );
				foundWordTypeItem = wordResult.foundWordTypeItem;

				if( foundWordTypeItem != null &&

				( ( adjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER &&
				!foundWordTypeItem.hasAdjectiveParameter() ) ||

				( definiteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER &&
				!foundWordTypeItem.hasDefiniteArticleParameter() ) ||

				( indefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
				!foundWordTypeItem.hasIndefiniteArticleParameter() ) ) )
					{
					if( foundWordTypeItem.setParametersOfSingularNoun( adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to set a parameter of a multiple word" );
					}
				}

			if( createdMultipleWordItem != null )
				{
				// Link both words to the created multiple word
				if( firstWordItem.addMultipleWord( nWordParts, currentWordTypeNr, createdMultipleWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the first multiple word" );

				if( ( currentWordItem = currentReadItem.readWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The current word item is undefined" );

				if( currentWordItem.addMultipleWord( nWordParts, currentWordTypeNr, createdMultipleWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the second multiple word" );
				}

			if( multipleWordItem == null )
				return adminItem_.startError( 1, moduleNameString_, "The multiple word is undefined" );

			if( firstReadItem.changeReadWord( currentWordTypeNr, multipleWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete an active read item" );

			deletedReadItem = currentReadItem;
			currentReadItem = ( deletedReadItem == lastReadItem ? null : currentReadItem.nextReadItem() );

			if( adminItem_.deleteReadItem( deletedReadItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the second read item" );
			}
		while( currentReadItem != null );

		return Constants.RESULT_OK;
		}

	private byte addReadSpecification( boolean isAction, boolean isNewStart, short selectionLevel )
		{
		boolean isConditional = ( isConditional_ || selectionListNr_ != Constants.NO_LIST_NR );
		short imperativeVerbParameter = Constants.NO_IMPERATIVE_PARAMETER;
		ReadItem currentGeneralizationReadItem = startGeneralizationWordReadItem_;
		SpecificationItem userSpecificationItem;

		if( currentGeneralizationReadItem == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The start generalization read item is undefined" );

		if( endGeneralizationWordReadItem_ == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The end generalization read item is undefined" );

		if( selectionListNr_ == Constants.NO_LIST_NR &&
		CommonVariables.nUserGeneralizationWords == 1 &&
		CommonVariables.nUserSpecificationWords > 1 &&
		CommonVariables.nUserRelationWords > 1 )
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

			adminItem_.initializeAdminVariables();

			do	{
				switch( currentGeneralizationReadItem.grammarParameter )
					{
					case Constants.GRAMMAR_IMPERATIVE:
						imperativeVerbParameter = currentGeneralizationReadItem.wordParameter();
						// Don't insert a break statement

					case Constants.GRAMMAR_GENERALIZATION_WORD:
						if( adminItem_.addUserSpecifications( isAction, isAssignment_, isConditional, isInactiveAssignment_, isArchivedAssignment_, isEveryGeneralization_, isExclusiveSpecification_, isNegative_, isNewStart, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueUserRelation_, assumptionLevel_, prepositionParameter_, questionParameter_, selectionLevel, selectionListNr_, imperativeVerbParameter, generalizationContextNr_, specificationContextNr_, currentGeneralizationReadItem, startSpecificationWordReadItem_, endSpecificationWordReadItem_, startRelationWordReadItem_, endRelationReadItem_ ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to add the read user specifications" );

						if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
						adminItem_.markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to mark the words passing the integrity check of the stored user sentence" );
					}
				}
			while( !CommonVariables.hasDisplayedWarning &&
			currentGeneralizationReadItem != endGeneralizationWordReadItem_ &&
			( currentGeneralizationReadItem = currentGeneralizationReadItem.nextReadItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte checkAllWordsForStructuralIntegrity()
		{
		WordItem currentSpecificationWordItem;

		if( adminItem_.checkForUnprocessedNegativeConclusion() == Constants.RESULT_OK &&
		( currentSpecificationWordItem = CommonVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem.checkStructuralIntegrityInWord() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to check the structural integrity in word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( !CommonVariables.hasDisplayedIntegrityWarning &&
			( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte checkForDuplicateSelection()
		{
		boolean hasFoundDuplicateSelection = false;
		int duplicateConditionSentenceNr;
		DuplicateResultType duplicateResult;

		if( ( duplicateResult = adminItem_.checkDuplicateCondition() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to check if the condition selection part is duplicate" );

		if( ( duplicateConditionSentenceNr = duplicateResult.duplicateConditionSentenceNr ) > Constants.NO_SENTENCE_NR )
			{
			if( ( duplicateResult = adminItem_.checkDuplicateSelectionPart( true, duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check if the action selection part is duplicate" );

			if( duplicateResult.hasFoundDuplicateSelection )
				{
				if( CommonVariables.adminAlternativeList == null )
					hasFoundDuplicateSelection = true;
				else
					{
					if( ( duplicateResult = adminItem_.checkDuplicateSelectionPart( false, duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );

					if( duplicateResult.hasFoundDuplicateSelection )
						hasFoundDuplicateSelection = true;
					}
				}

			if( hasFoundDuplicateSelection &&
			adminItem_.removeDuplicateSelection() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to remove a duplicate selection" );
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
							!currentReadWordItem.isDeletedItem() &&
							currentReadWordItem.hasCurrentCreationSentenceNr() &&
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

	private byte findGrammarPath( short grammarLevel, GrammarItem parseGrammarItem )
		{
		boolean isOption;
		boolean isChoice;
		boolean isWaitingForNewStart;
		boolean isWaitingForChoiceEnd;
		short previousWordOrderNr;
		short startWordOrderNr;
		short choiceStartWordOrderNr = Constants.NO_ORDER_NR;
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
							return adminItem_.startError( 1, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #" + grammarLevel );

						if( parseGrammarItem.definitionGrammarItem == null )
							return adminItem_.startError( 1, moduleNameString_, "Grammar word \"" + parseGrammarItem.grammarString() + "\" isn't defined in the grammar file" );

						if( findGrammarPath( (short)( grammarLevel + 1 ), parseGrammarItem.definitionGrammarItem ) != Constants.RESULT_OK )
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
				parseGrammarItem != null &&
				parseGrammarItem.isNewStart() &&
				currentParseWordOrderNr_ > previousWordOrderNr )
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

		if( definitionParseGrammarItem != null &&
		definitionParseGrammarItem.isGrammarDefinition() &&

		( previousWordOrderNr > startWordOrderNr ||
		currentParseWordOrderNr_ > startWordOrderNr ) )
			{
			if( adminItem_.setGrammarParameter( ( currentParseWordOrderNr_ > startWordOrderNr ), startWordOrderNr, ( currentParseWordOrderNr_ > startWordOrderNr ? currentParseWordOrderNr_ : previousWordOrderNr ), definitionParseGrammarItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to set the grammar parameter of the read words between the positions " + startWordOrderNr + " and " + currentParseWordOrderNr_ );
			}

		return Constants.RESULT_OK;
		}

	private static byte initializeVariablesInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.initializeVariablesInWord();
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte parseSentence( String readUserSentenceString )
		{
		boolean isAction = false;
		boolean isNewStart = true;
		short wordOrderNr;
		short wordParameter;
		short selectionLevel = Constants.NO_SELECTION_LEVEL;

		isUserImperativeSentence_ = false;

		selectionListNr_ = Constants.NO_LIST_NR;

		linkedGeneralizationReadItem_ = null;
		startGeneralizationWordReadItem_ = null;
		endGeneralizationWordReadItem_ = null;

		adminItem_.initializeLinkedWord();

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
						if( scanSpecification( readUserSentenceString ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to scan the generalization-specification" );

						if( addReadSpecification( isAction, isNewStart, selectionLevel ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to add the read specification" );

						isNewStart = false;

						break;

					case Constants.GRAMMAR_IMPERATIVE:
						if( readImperative( isAction, isNewStart, selectionLevel, readUserSentenceString ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to read an imperative" );

						isNewStart = false;
						isUserImperativeSentence_ = true;

						break;

					case Constants.GRAMMAR_ANSWER:
						if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to write an interface warning" );

						break;

					case Constants.GRAMMAR_SENTENCE_CONJUNCTION:
						switch( wordParameter )
							{
							case Constants.WORD_PARAMETER_SYMBOL_COMMA:
							case Constants.WORD_PARAMETER_CONJUNCTION_AND:
							// Typically for Dutch: in zowel ... als ...
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
						if( readUserSentenceString != null &&

						( adminItem_.isCurrentlyTesting() ||
						adminItem_.isSystemStartingUp() ) )
							return adminItem_.startError( 1, moduleNameString_, "I found an unknown word in sentence \"" + readUserSentenceString + "\" at position " + wordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + wordParameter );

						return adminItem_.startError( 1, moduleNameString_, "I found an unknown word at position " + wordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + wordParameter );
					}
				}
			}
		while( currentReadItem_ != null &&
		!CommonVariables.hasDisplayedWarning &&
		!adminItem_.hasRequestedRestart() &&
		( currentReadItem_ = currentReadItem_.nextReadItem() ) != null );

		if( selectionListNr_ != Constants.NO_LIST_NR &&
		checkForDuplicateSelection() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to check on a duplicate selection" );

		return Constants.RESULT_OK;
		}

	private byte processSentence( String readUserSentenceString )
		{
		boolean isInterpretationSuccessful;
		short currentLanguageNr;
		short lastCreatedWordOrderNr;
		short nLanguages;
		short originalLanguageNr = CommonVariables.currentLanguageNr;
		long startNanoTime = System.nanoTime();
		GrammarItem firstGrammarItem;
		WordItem currentLanguageWordItem;
		BoolResultType boolResult = new BoolResultType();

		if( readUserSentenceString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( nLanguages = adminItem_.nLanguages() ) <= Constants.NO_LANGUAGE_NR )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any language" );

		currentLanguageNr = originalLanguageNr;

		if( nLanguages < currentLanguageNr )
			return adminItem_.startError( 1, moduleNameString_, "The current language number exceeds the number of languages" );

		originalReadStringBuffer = new StringBuffer( readUserSentenceString );

		do	{
			adminItem_.deleteTemporaryReadList();

			// Need to switch language
			if( currentLanguageNr != CommonVariables.currentLanguageNr )
				{
				if( assignLanguage( currentLanguageNr ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );

				// Avoid using the expanded read string with other languages
				readUserSentenceString = originalReadStringBuffer.toString();
				}

			if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );

			if( currentLanguageWordItem.isCheckingGrammarNeeded() &&
			currentLanguageWordItem.checkGrammar() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check the grammar" );

			if( readUserSentenceString != null &&
			currentLanguageWordItem.isLanguageWithMergedWords() )
				{
				// Typically for French: Compound words
				if( currentLanguageWordItem.expandMergedWordsInReadSentence( new StringBuffer( readUserSentenceString ) ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to expand the compound wordds in the read user sentence string" );

				if( CommonVariables.readUserSentenceStringBuffer != null )
					readUserSentenceString = CommonVariables.readUserSentenceStringBuffer.toString();
				}

			// Create read words from a given sentence
			if( ( boolResult = adminItem_.createReadWords( readUserSentenceString ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to create the read words" );

			isInterpretationSuccessful = false;
			adminItem_.clearLastActivatedWordOrderNr();

			// Has created all read words
			if( boolResult.booleanValue )
				{
				lastCreatedWordOrderNr = adminItem_.lastCreatedWordOrderNr();
				firstGrammarItem = currentLanguageWordItem.firstGrammarItem();

				do	{
					if( findGrammarPath( Constants.NO_GRAMMAR_LEVEL, firstGrammarItem ) != Constants.RESULT_OK )
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

			CommonVariables.parsingTime += ( System.nanoTime() - startNanoTime );

			if( isInterpretationSuccessful )
				{
				if( deleteUnusedWordsAndWordTypes() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete the unused words and word types of the read words" );

				if( parseSentence( readUserSentenceString ) != Constants.RESULT_OK )
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

			if( CommonVariables.currentLanguageNr != originalLanguageNr &&
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

				if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START, currentParseWordOrderNr_, Constants.INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_END ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte readImperative( boolean isAction, boolean isNewStart, short selectionLevel, String readUserSentenceString )
		{
		short imperativeVerbParameter = Constants.NO_IMPERATIVE_PARAMETER;
		short executionNounWordParameter = Constants.NO_WORD_PARAMETER;
		ReadItem imperativeReadItem;
		ReadItem specificationReadItem;
		WordItem imperativeWordItem = null;

		startGeneralizationWordReadItem_ = null;
		endGeneralizationWordReadItem_ = null;

		if( scanSpecification( readUserSentenceString ) != Constants.RESULT_OK )
			return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to scan the generalization-specification" );

		if( endGeneralizationWordReadItem_ == null ||
		( imperativeReadItem = startGeneralizationWordReadItem_ ) == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I couldn't find the imperative" );

		do	{
			if( imperativeReadItem.isVerb() )
				{
				imperativeVerbParameter = imperativeReadItem.wordParameter();
				imperativeWordItem = imperativeReadItem.readWordItem();
				}
			}
		while( ( imperativeReadItem = imperativeReadItem.nextReadItem() ) != null );

		// Only imperative word
		if( ( specificationReadItem = startSpecificationWordReadItem_ ) == null )
			{
			if( imperativeWordItem != null &&
			imperativeWordItem.isVerbUndoOrRedo() )
				wasPreviousCommandUndoOrRedo_ = true;
			else
				{
				// From this point, redo is not possible anymore
				clearReplacingInfoInSpecificationWords();
				wasPreviousCommandUndoOrRedo_ = false;
				}

			if( adminItem_.executeImperative( true, Constants.NO_LIST_NR, startGeneralizationWordReadItem_.wordParameter(), Constants.NO_WORD_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.MAX_PROGRESS, startGeneralizationWordReadItem_.readString, startGeneralizationWordReadItem_.readWordItem(), null, startRelationWordReadItem_, endRelationReadItem_, null, null ) != Constants.RESULT_OK )
				{
				if( startGeneralizationWordReadItem_ == null )
					return adminItem_.addError( 1, moduleNameString_, "I failed to execute the single imperative" );

				return adminItem_.addError( 1, moduleNameString_, "I failed to execute the single imperative at position " + startGeneralizationWordReadItem_.wordOrderNr() );
				}
			}
		else
			{
			// Imperative word has specifications
			if( endSpecificationWordReadItem_ == null )
				return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The end specification read item is undefined" );

			if( addReadSpecification( isAction, isNewStart, selectionLevel ) != Constants.RESULT_OK )
				{
				if( startGeneralizationWordReadItem_ == null ||
				endGeneralizationWordReadItem_ == null )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add the generalization part of the read specification" );

				return adminItem_.addError( 1, moduleNameString_, "I failed to add the generalization part of the read specification between the positions " + startGeneralizationWordReadItem_.wordOrderNr() + " and " + endGeneralizationWordReadItem_.wordOrderNr() );
				}

			if( !CommonVariables.hasDisplayedWarning &&
			selectionListNr_ == Constants.NO_LIST_NR )
				{
				do	{
					if( specificationReadItem.isUserDefined() ||

					( specificationReadItem.isNoun() &&
					!specificationReadItem.isNounInformation() ) )
						{
						// Make distinction between reading a normal file or a test file
						if( specificationReadItem.isNounFile() ||
						// Make distinction between displaying a word or justification report
						specificationReadItem.isNounJustificationReport() )
							executionNounWordParameter = specificationReadItem.wordParameter();
						else
							{
							if( adminItem_.executeImperative( true, Constants.NO_LIST_NR, imperativeVerbParameter, ( executionNounWordParameter == Constants.NO_WORD_PARAMETER ? specificationReadItem.wordParameter() : executionNounWordParameter ), specificationReadItem.wordTypeNr(), Constants.MAX_PROGRESS, specificationReadItem.readString, imperativeWordItem, specificationReadItem.readWordItem(), startRelationWordReadItem_, endRelationReadItem_, null, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to execute an imperative with specifications" );
							}
						}
					}
				while( !adminItem_.hasRequestedRestart() &&
				!CommonVariables.hasDisplayedWarning &&
				// Still read items available
				adminItem_.firstActiveReadItem() != null &&

				( !specificationReadItem.isVirtualListPreposition() ||
				// Loop shouldn't end if virtual list prepositions are used to e.g. display justification reports
				executionNounWordParameter > Constants.NO_WORD_PARAMETER ) &&

				specificationReadItem != endSpecificationWordReadItem_ &&
				( specificationReadItem = specificationReadItem.nextReadItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte scanSpecification( String readUserSentenceString )
		{
		ContextResultType contextResult;
		boolean isFrenchPreposition;
		boolean isSameWordTypeAsPreviousWord;
		boolean hasGeneralizationArticle = false;
		boolean isStop = false;
		short currentWordOrderNr;
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

		hasFemaleUserSpecificationWord_ = false;
		isAssignment_ = false;
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

		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
		currentParseWordOrderNr_ = Constants.NO_ORDER_NR;
		prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
		questionParameter_ = Constants.NO_QUESTION_PARAMETER;

		generalizationContextNr_ = Constants.NO_CONTEXT_NR;
		specificationContextNr_ = Constants.NO_CONTEXT_NR;

		startSpecificationWordReadItem_ = null;
		endSpecificationWordReadItem_ = null;
		startRelationWordReadItem_ = null;
		endRelationReadItem_ = null;

		CommonVariables.nUserGeneralizationWords = 0;
		CommonVariables.nUserSpecificationWords = 0;
		CommonVariables.nUserRelationWords = 0;

		if( linkedGeneralizationReadItem_ == null )
			{
			startGeneralizationWordReadItem_ = null;
			endGeneralizationWordReadItem_ = null;
			}
		else
			{
			if( linkedGeneralizationReadItem_.isProperName() )
				startGeneralizationWordReadItem_ = linkedGeneralizationReadItem_;

			linkedGeneralizationReadItem_ = null;
			}

		if( currentReadItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The read item is undefined" );

		do	{
			isFrenchPreposition = currentReadItem_.isFrenchPreposition();

			currentWordOrderNr = currentReadItem_.wordOrderNr();
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
				case Constants.GRAMMAR_SENTENCE:
					if( !currentReadItem_.isSeparator() )
						{
						if( readUserSentenceString != null &&

						( adminItem_.isCurrentlyTesting() ||
						adminItem_.isSystemStartingUp() ) )
							return adminItem_.startError( 1, moduleNameString_, "I found an unknown word in sentence \"" + readUserSentenceString + "\" at position " + currentWordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + currentWordParameter );

						return adminItem_.startError( 1, moduleNameString_, "I found an unknown word at position " + currentWordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + currentWordParameter );
						}

					break;

				case Constants.GRAMMAR_ANSWER:
					// Needs to be implemented
					break;

				// Assignment generalization-specification
				case Constants.GRAMMAR_GENERALIZATION_ASSIGNMENT:
					isAssignment_ = true;

					// Don't insert a break statement here

				case Constants.GRAMMAR_IMPERATIVE:
				case Constants.GRAMMAR_GENERALIZATION_SPECIFICATION:
				case Constants.GRAMMAR_GENERALIZATION_PART:
				case Constants.GRAMMAR_GENERALIZATION_WORD:
					if( currentReadItem_.isAdjective() )
						{
						if( adminItem_.isAdjectiveParameter( currentReadItem_.wordParameter() ) )
							generalizationAdjectiveParameter = currentReadItem_.wordParameter();

						if( currentReadItem_.isAdjectivePrevious() )
							{
							isAssignment_ = true;
							isInactiveAssignment_ = true;
							}

						if( currentReadItem_.isAdjectiveEvery() )
							isEveryGeneralization_ = true;
						}

					if( currentReadItem_.isArticle() )
						{
						hasGeneralizationArticle = true;

						if( currentReadItem_.isDefiniteArticleParameter( currentReadItem_.wordParameter() ) )
							generalizationDefiniteArticleParameter = currentReadItem_.wordParameter();
						else
							generalizationIndefiniteArticleParameter = currentReadItem_.wordParameter();
						}

					if( currentReadItem_.isNounPartOf() )
						isPartOf_ = true;

					if( currentReadItem_.isGeneralizationWord() )
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

							CommonVariables.nUserGeneralizationWords++;
							}

						if( currentReadWordItem != null )
							currentReadWordItem.isUserGeneralizationWord = true;
						}

					if( currentReadItem_.isDeterminerOrPronoun() )
						{
						if( generalizationContextNr_ > Constants.NO_CONTEXT_NR )
							return adminItem_.startError( 1, moduleNameString_, "The generalization context number is already assigned" );

						if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a generalization pronoun context" );

						generalizationContextNr_ = contextResult.contextNr;
						}

					if( startGeneralizationWordReadItem_ == null )
						startGeneralizationWordReadItem_ = currentReadItem_;

					endGeneralizationWordReadItem_ = currentReadItem_;

					break;

				case Constants.GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION:
					nextReadItem = currentReadItem_.nextReadItem();

					if( nextReadItem != null &&
					nextReadItem.readWordItem() != previousGeneralizationWordItem_ )
						{
						isStop = true;
						linkedGeneralizationReadItem_ = nextReadItem;
						}

					break;

				case Constants.GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION:
					isExclusiveSpecification_ = true;
					break;

				case Constants.GRAMMAR_RELATION_PART:
				case Constants.GRAMMAR_RELATION_WORD:
					if( currentReadItem_.isPreposition() )
						prepositionParameter_ = currentWordParameter;

					if( currentReadItem_.isRelationWord() )
						{
						if( isSameWordTypeAsPreviousWord )
							{
							if( addMultipleWord( Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, previousReadItem, currentReadItem_ ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple relation word" );

							// Current read item is deleted
							currentReadItem_ = previousReadItem;
							}
						else
							CommonVariables.nUserRelationWords++;

						if( currentReadWordItem != null )
							currentReadWordItem.isUserRelationWord = true;
						}

					if( startSpecificationWordReadItem_ == null )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					if( startRelationWordReadItem_ == null )
						startRelationWordReadItem_ = currentReadItem_;

					endRelationReadItem_ = currentReadItem_;

					switch( currentWordParameter )
						{
						// Typically for English: "... in both ... and ..."
						case Constants.WORD_PARAMETER_NUMERAL_BOTH:
						// Typically for Dutch: "... in zowel ... als ..."
						case Constants.WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:
							isExclusiveSpecification_ = true;
							break;
						}

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
					if( currentReadItem_.isSpecificationWord() )
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
							if( isArchivedAssignment_ &&
							!hasGeneralizationArticle )
								isUniqueUserRelation_ = true;

							if( currentReadWordItem != null &&
							currentReadWordItem.isFemale() )
								hasFemaleUserSpecificationWord_ = true;

							CommonVariables.nUserSpecificationWords++;

							if( previousGeneralizationWordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME &&
							currentWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
							previousGeneralizationWordItem_ != null &&
							( currentReadWordTypeItem = currentReadItem_.activeReadWordTypeItem() ) != null &&
							!currentReadWordTypeItem.hasDefiniteArticleParameter() &&
							!currentReadWordTypeItem.hasIndefiniteArticleParameter() &&
							( generalizationWordTypeItem = previousGeneralizationWordItem_.activeWordTypeItem( Constants.WORD_TYPE_PROPER_NAME ) ) != null &&
							generalizationWordTypeItem.hasFeminineOrMasculineWordEnding() &&

							// Set parameters
							currentReadWordTypeItem.setParametersOfSingularNoun( Constants.NO_ADJECTIVE_PARAMETER, ( generalizationWordTypeItem.hasFeminineWordEnding() ? Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE : Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE ), ( generalizationWordTypeItem.hasFeminineWordEnding() ? Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to set the definite and indefinite article parameters of a singular noun" );
							}

						if( currentReadWordItem != null )
							currentReadWordItem.isUserSpecificationWord = true;
						}

					if( startSpecificationWordReadItem_ == null )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					break;

				case Constants.GRAMMAR_RELATION_ASSIGNMENT:
					break;

				case Constants.GRAMMAR_VERB:
					if( startSpecificationWordReadItem_ == null )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

					break;

				case Constants.GRAMMAR_QUESTION_VERB:
					questionParameter_ = currentWordParameter;
					break;

				case Constants.GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
					questionParameter_ = currentWordParameter;

					// Don't insert a break statement here

				case Constants.GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
					isSpecificationGeneralization_ = true;

					if( startSpecificationWordReadItem_ == null )
						startSpecificationWordReadItem_ = currentReadItem_;

					endSpecificationWordReadItem_ = currentReadItem_;

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

		if( contextWordItem.addContext( false, contextWordTypeNr, Constants.NO_WORD_TYPE_NR, contextResult.contextNr, null ) != Constants.RESULT_OK )
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

		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
		currentParseWordOrderNr_ = Constants.NO_ORDER_NR;
		prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
		previousGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
		questionParameter_ = Constants.NO_QUESTION_PARAMETER;
		selectionListNr_ = Constants.NO_LIST_NR;

		generalizationContextNr_ = Constants.NO_CONTEXT_NR;
		specificationContextNr_ = Constants.NO_CONTEXT_NR;

		currentReadItem_ = null;
		linkedGeneralizationReadItem_ = null;
		startGeneralizationWordReadItem_ = null;
		endGeneralizationWordReadItem_ = null;
		startSpecificationWordReadItem_ = null;
		endSpecificationWordReadItem_ = null;
		startRelationWordReadItem_ = null;
		endRelationReadItem_ = null;

		previousGeneralizationWordItem_ = null;

		originalReadStringBuffer = new StringBuffer();

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
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

	protected boolean wasPreviousCommandUndoOrRedo()
		{
		return wasPreviousCommandUndoOrRedo_;
		}

	protected byte processReadSentence( String readUserSentenceString )
		{
		int startSentenceNr = CommonVariables.currentSentenceNr;

		previousGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
		previousGeneralizationWordItem_ = null;

		CommonVariables.hasFoundAnswerToQuestion = false;
		CommonVariables.hasDisplayedArticleNotification = false;
		CommonVariables.isFirstAnswerToQuestion = true;
		CommonVariables.isQuestionAlreadyAnswered = false;
		CommonVariables.firstTouchedProperNameWordItem = null;

		CommonVariables.writtenUserSentenceStringBuffer = new StringBuffer();

		if( readUserSentenceString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read user sentence string is undefined" );

		if( initializeVariablesInAllWords() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to initialize variables in all words" );

		if( processSentence( readUserSentenceString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to process sentence: \"" + readUserSentenceString + "\"" );

		if( !CommonVariables.hasDisplayedWarning &&
		!adminItem_.hasRequestedRestart() &&
		startSentenceNr == CommonVariables.currentSentenceNr )
			{
			checkForChangesMadeByThisSentence();

			// Skip integrity check if no changes are made
			if( hasAnyChangeBeenMadeByThisSentence_ &&
			!isUserImperativeSentence_ &&
			selectionListNr_ == Constants.NO_LIST_NR &&
			// User specification is already known (notification: I know)
			adminItem_.userSpecificationItem() != null &&
			adminItem_.checkIntegrityOfStoredUserSentence( readUserSentenceString ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check the integrity of the stored user sentence \"" + readUserSentenceString + "\"" );

			// Has passed integrity check
			if( !CommonVariables.hasDisplayedWarning )
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
