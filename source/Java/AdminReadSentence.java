/*
 *	Class:			AdminReadSentence
 *	Supports class:	AdminItem
 *	Purpose:		To read and analyze sentences
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
	// Private constructible variables

	private boolean isAssignment_;
	private boolean isActiveAssignment_;
	private boolean isInactiveAssignment_;
	private boolean isArchivedAssignment_;
	private boolean isConditional_;
	private boolean isDeletingRollbackInfo_;
	private boolean isEveryGeneralization_;
	private boolean isExclusiveSpecification_;
	private boolean isNegative_;
	private boolean isPartOf_;
	private boolean isPossessive_;
	private boolean isLinkedGeneralizationConjunction_;
	private boolean isSpecificationGeneralization_;
	private boolean isUniqueUserRelation_;
	private boolean isUserImperativeSentence_;

	private short assumptionLevel_;
	private short currentParseWordOrderNr_;
	private short prepositionParameter_;
	private short questionParameter_;
	private short selectionListNr_;

	private int generalizationContextNr_;
	private int specificationContextNr_;

	private ReadItem currentReadItem_;
	private ReadItem startGeneralizationWordReadItem_;
	private ReadItem endGeneralizationWordReadItem_;
	private ReadItem startSpecificationWordReadItem_;
	private ReadItem endSpecificationWordReadItem_;
	private ReadItem startRelationWordReadItem_;
	private ReadItem endRelationReadItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static void initializeVariablesInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.initializeVariablesInWord();
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
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
				isNegative_ = true;
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

	private ContextResultType addPronounContext( short contextWordTypeNr, WordItem contextWordItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( contextWordItem != null )
			{
			if( ( contextResult.contextNr = contextWordItem.contextNr( null ) ) == Constants.NO_CONTEXT_NR )
				{
				if( ( contextResult.contextNr = adminItem_.highestContextNrInAllWords() ) < Constants.MAX_CONTEXT_NR )
					// Create new context number
					contextResult.contextNr++;
				else
					adminItem_.startSystemError( 1, moduleNameString_, "Context number overflow" );
				}

			if( contextWordItem.addContext( false, contextWordTypeNr, Constants.WORD_TYPE_UNDEFINED, contextResult.contextNr, null ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to add a pronoun context to word \"" + contextWordItem.anyWordTypeString() + "\"" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The read word of the read ahead item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	private byte addMultipleWord( ReadItem firstReadItem, ReadItem secondReadItem )
		{
		WordResultType wordResult;
		boolean isFirstWordSingularOrPluralNoun;
		boolean hasNextWordWithSameWordType = true;
		boolean hasFoundWordWithDifferentWordType = false;
		short firstWordTypeNr;
		short nextWordTypeNr;
		short nWordParts = 1;
		ReadItem deletedReadItem;
		ReadItem lastReadItem = null;
		ReadItem nextReadItem = secondReadItem;
		WordItem firstWordItem;
		WordItem nextWordItem;
		WordItem multipleWordItem;
		WordItem createdMultipleWordItem;
		WordItem createdWordItem;
		WordTypeItem differentWordTypeItem;
		WordTypeItem firstWordTypeItem;
		WordTypeItem nextWordTypeItem;
		StringBuffer existingMultipleWordStringBuffer;
		StringBuffer multipleWordStringBuffer;
		ReadList readList;

		if( firstReadItem != null )
			{
			if( secondReadItem != null )
				{
				if( ( firstWordItem = firstReadItem.readWordItem() ) != null )
					{
					firstWordTypeNr = firstReadItem.wordTypeNr();

					if( ( firstWordTypeItem = firstWordItem.activeWordTypeItem( firstWordTypeNr ) ) != null )
						{
						isFirstWordSingularOrPluralNoun = adminItem_.isSingularOrPluralNoun( firstWordTypeNr );
						multipleWordStringBuffer = new StringBuffer( firstWordTypeItem.itemString() );
						existingMultipleWordStringBuffer = new StringBuffer( multipleWordStringBuffer );

						do	{
							nextWordTypeNr = nextReadItem.wordTypeNr();

							// The next word has the same word type as the first word
							if( firstWordTypeNr == nextWordTypeNr ||

							( isFirstWordSingularOrPluralNoun &&
							adminItem_.isSingularOrPluralNoun( nextWordTypeNr ) ) )
								{
								if( ( nextWordItem = nextReadItem.readWordItem() ) != null )
									{
									if( ( nextWordTypeItem = nextWordItem.activeWordTypeItem( nextWordTypeNr ) ) != null )
										{
										nWordParts++;
										lastReadItem = nextReadItem;
										multipleWordStringBuffer.append( Constants.SPACE_STRING + nextWordTypeItem.itemString() );

										if( firstWordTypeNr != nextWordTypeNr &&
										( differentWordTypeItem = nextWordItem.activeWordTypeItem( firstWordTypeNr ) ) != null )
											{
											hasFoundWordWithDifferentWordType = true;
											existingMultipleWordStringBuffer.append( Constants.SPACE_STRING + differentWordTypeItem.itemString() );
											}
										else
											existingMultipleWordStringBuffer.append( Constants.SPACE_STRING + nextWordTypeItem.itemString() );
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type item of the next read word" );
									}
								else
									return adminItem_.startError( 1, moduleNameString_, "I have found a read item without read word" );
								}
							else
								hasNextWordWithSameWordType = false;
							}
						while( hasNextWordWithSameWordType &&
						( nextReadItem = nextReadItem.nextReadItem() ) != null );

						nextReadItem = secondReadItem;

						do	{
							nextWordTypeNr = nextReadItem.wordTypeNr();

							if( ( nextWordItem = nextReadItem.readWordItem() ) != null )
								{
								createdMultipleWordItem = null;
								createdWordItem = null;
								multipleWordItem = null;

								if( hasFoundWordWithDifferentWordType )
									{
									if( ( wordResult = adminItem_.findWordTypeInAllWords( false, firstWordTypeNr, existingMultipleWordStringBuffer.toString(), null ) ).result == Constants.RESULT_OK )
										{
										if( ( createdWordItem = wordResult.foundWordItem ) != null )
											{
											if( createdWordItem.addWordType( false, firstWordTypeItem.isProperNamePrecededByDefiniteArticle( firstWordTypeItem.definiteArticleParameter() ), firstWordTypeItem.adjectiveParameter(), firstWordTypeItem.definiteArticleParameter(), firstWordTypeItem.indefiniteArticleParameter(), nextWordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ).result == Constants.RESULT_OK )
												{
												createdMultipleWordItem = createdWordItem;
												multipleWordItem = createdWordItem;
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to add the word type to multiple word \"" + createdWordItem.anyWordTypeString() + "\"" );
											}
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to find an exact word type in all words" );
									}

								if( createdWordItem == null )
									{
									if( ( wordResult = adminItem_.addWord( false, true, firstWordTypeItem.adjectiveParameter(), firstWordTypeItem.definiteArticleParameter(), firstWordTypeItem.indefiniteArticleParameter(), firstReadItem.wordParameter(), nextWordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ) ).result == Constants.RESULT_OK )
										{
										createdMultipleWordItem = wordResult.createdWordItem;
										multipleWordItem = ( createdMultipleWordItem == null ? wordResult.foundWordItem : createdMultipleWordItem );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to add the multiple word" );
									}

								if( createdMultipleWordItem != null )
									{
									// Link both words to the created multiple word
									if( firstWordItem.addMultipleWord( nWordParts, nextWordTypeNr, createdMultipleWordItem ) == Constants.RESULT_OK )
										{
										if( ( nextWordItem = nextReadItem.readWordItem() ) != null )
											{
											if( nextWordItem.addMultipleWord( nWordParts, nextWordTypeNr, createdMultipleWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add the second multiple word" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "The next word item is undefined" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to add the first multiple word" );
									}

								if( multipleWordItem != null )
									{
									if( firstReadItem.changeReadWord( nextWordTypeNr, multipleWordItem ) == Constants.RESULT_OK )
										{
										if( ( readList = adminItem_.readList ) != null )
											{
											deletedReadItem = nextReadItem;
											nextReadItem = ( deletedReadItem == lastReadItem ? null : nextReadItem.nextReadItem() );

											if( readList.deleteItem( false, deletedReadItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to delete the second read item" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "The read list isn't created yet" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to delete an active read item" );
									}
								else
									return adminItem_.startError( 1, moduleNameString_, "The multiple word is undefined" );
								}
							else
								return adminItem_.startError( 1, moduleNameString_, "I have found a read item without read word" );
							}
						while( nextReadItem != null );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type item of the given first read item" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given first read item has no read word" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given second read item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given first read item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte addReadSpecification( boolean isAction, boolean isNewStart, short selectionLevel )
		{
		boolean isConditional = ( isConditional_ || selectionListNr_ != Constants.NO_LIST_NR );
		short imperativeParameter = Constants.NO_IMPERATIVE_PARAMETER;
		short specificationWordParameter = Constants.NO_WORD_PARAMETER;
		ReadItem currentGeneralizationReadItem = startGeneralizationWordReadItem_;
		SpecificationItem userSpecificationItem;

		if( currentGeneralizationReadItem != null )
			{
			if( endGeneralizationWordReadItem_ != null )
				{
				if( selectionListNr_ == Constants.NO_LIST_NR &&
				CommonVariables.nUserGeneralizationWords == 1 &&
				CommonVariables.nUserSpecificationWords > 1 &&
				CommonVariables.nUserRelationWords > 1 )
					{
					// Ambiguity: Missing relation context
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MULTIPLE_SPECIFICATIONS_RELATIONS ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to write an interface warning about ambiguity" );
					}
				else
					{
					adminItem_.initializeAdminAssumptionVariables();
					adminItem_.initializeAdminConclusionVariables();
					adminItem_.initializeAdminReasoningVariables();
					adminItem_.initializeAdminSpecificationVariables();

					do	{
						switch( currentGeneralizationReadItem.grammarParameter )
							{
							case Constants.GRAMMAR_IMPERATIVE:
								imperativeParameter = currentGeneralizationReadItem.wordParameter();
								// Don't insert a break statement

							case Constants.GRAMMAR_GENERALIZATION_WORD:
								if( adminItem_.addUserSpecifications( isAction, isAssignment_, isConditional, isInactiveAssignment_, isArchivedAssignment_, isEveryGeneralization_, isExclusiveSpecification_, isNegative_, isNewStart, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueUserRelation_, assumptionLevel_, prepositionParameter_, questionParameter_, selectionLevel, selectionListNr_, imperativeParameter, specificationWordParameter, generalizationContextNr_, specificationContextNr_, currentGeneralizationReadItem, startSpecificationWordReadItem_, endSpecificationWordReadItem_, startRelationWordReadItem_, endRelationReadItem_ ) == Constants.RESULT_OK )
									{
									if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
										{
										if( adminItem_.markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to mark the words passing the integrity check of the stored user sentence" );
										}
									}
								else
									return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to add the read user specifications" );
							}
						}
					while( !CommonVariables.hasShownWarning &&
					currentGeneralizationReadItem != endGeneralizationWordReadItem_ &&
					( currentGeneralizationReadItem = currentGeneralizationReadItem.nextReadItem() ) != null );
					}
				}
			else
				return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The end generalization read item is undefined" );
			}
		else
			return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The start generalization read item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkForStructuralIntegrity()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.checkSpecificationsForReplacedOrDeletedJustifications() == Constants.RESULT_OK )
					{
					if( currentWordItem.checkJustificationsForReplacedSpecifications() == Constants.RESULT_OK )
						{
						if( currentWordItem.checkForUnreferencedJustifications() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to check for unreferenced justifications in word \"" + currentWordItem.anyWordTypeString() + "\"" );
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to check for the justifications for replaced specifications in word \"" + currentWordItem.anyWordTypeString() + "\"" );
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to check for the specifications for replaced justifications in word \"" + currentWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte findGrammarPath( short grammarLevel, GrammarItem parseGrammarItem )
		{
		ReadResultType readResult;
		boolean isOption;
		boolean isChoice;
		boolean isWaitingForNewStart;
		boolean isWaitingForChoiceEnd;
		short previousWordOrderNr;
		short startWordOrderNr;
		short choiceStartWordOrderNr = Constants.NO_ORDER_NR;
		GrammarItem definitionParseGrammarItem;
		ReadList readList;

		if( grammarLevel < Constants.MAX_GRAMMAR_LEVEL )
			{
			if( parseGrammarItem != null )
				{
				if( ( readList = adminItem_.readList ) != null )
					{
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
									if( ( readResult = readList.selectMatchingWordType( currentParseWordOrderNr_, definitionParseGrammarItem.grammarParameter(), definitionParseGrammarItem.grammarWordTypeNr() ) ).result == Constants.RESULT_OK )
										{
										if( readResult.hasFoundMatchingWordType )
											currentParseWordOrderNr_++;
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to select a matching word type" );
									}
								else
									{
									if( grammarLevel + 1 < Constants.MAX_GRAMMAR_LEVEL )
										{
										if( parseGrammarItem.definitionGrammarItem != null )
											{
											if( findGrammarPath( (short)( grammarLevel + 1 ), parseGrammarItem.definitionGrammarItem ) == Constants.RESULT_OK )
												{
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

													if( startWordOrderNr < currentParseWordOrderNr_ )
														{
														if( readList.activateInactiveReadWords( currentParseWordOrderNr_ ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to activate inactive read words from position: " + currentParseWordOrderNr_ );
														}
													}
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to find the grammar path at grammar word \"" + parseGrammarItem.grammarString() + "\"" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "Grammar word \"" + parseGrammarItem.grammarString() + "\" isn't defined in the grammar file" );
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #" + grammarLevel );
									}
								}

							if( CommonVariables.result == Constants.RESULT_OK )
								{
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
							}
						while( CommonVariables.result == Constants.RESULT_OK &&

						( isWaitingForNewStart ||
						isWaitingForChoiceEnd ||
						currentParseWordOrderNr_ > previousWordOrderNr ) &&

						parseGrammarItem != null &&
						definitionParseGrammarItem.activeSentenceNr() == parseGrammarItem.activeSentenceNr() );

						if( !isWaitingForNewStart &&
						currentParseWordOrderNr_ > startWordOrderNr &&
						currentParseWordOrderNr_ == previousWordOrderNr )
							currentParseWordOrderNr_ = startWordOrderNr;
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					currentParseWordOrderNr_ == startWordOrderNr &&
					definitionParseGrammarItem != null &&
					( parseGrammarItem = definitionParseGrammarItem.nextDefinitionGrammarItem ) != null );

					if( CommonVariables.result == Constants.RESULT_OK &&
					definitionParseGrammarItem != null &&
					definitionParseGrammarItem.isGrammarDefinition() &&

					( previousWordOrderNr > startWordOrderNr ||
					currentParseWordOrderNr_ > startWordOrderNr ) )
						{
						if( readList.setGrammarParameter( ( currentParseWordOrderNr_ > startWordOrderNr ), startWordOrderNr, ( currentParseWordOrderNr_ > startWordOrderNr ? currentParseWordOrderNr_ : previousWordOrderNr ), definitionParseGrammarItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to set the grammar parameter of the read words between the positions " + startWordOrderNr + " and " + currentParseWordOrderNr_ );
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The read list isn't created yet" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given parse grammar item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given grammar level is too high: #" + grammarLevel );

		return Constants.RESULT_OK;
		}

	private byte parseSentence( String readSentenceString )
		{
		boolean isAction = false;
		boolean isNewStart = true;
		short wordOrderNr;
		short wordParameter;
		short selectionLevel = Constants.NO_SELECTION_LEVEL;

		isLinkedGeneralizationConjunction_ = false;
		isUserImperativeSentence_ = false;

		selectionListNr_ = Constants.NO_LIST_NR;

		adminItem_.initializeLinkedWord();
		endGeneralizationWordReadItem_ = null;
		startGeneralizationWordReadItem_ = null;

		if( ( currentReadItem_ = adminItem_.firstActiveReadItem() ) != null )
			{
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
							if( scanSpecification( readSentenceString ) == Constants.RESULT_OK )
								{
								if( addReadSpecification( isAction, isNewStart, selectionLevel ) == Constants.RESULT_OK )
									isNewStart = false;
								else
									return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to add the read specification" );
								}
							else
								return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to scan the generalization-specification" );

							break;

						case Constants.GRAMMAR_IMPERATIVE:
							if( readImperative( isAction, isNewStart, selectionLevel, readSentenceString ) == Constants.RESULT_OK )
								{
								isNewStart = false;
								isUserImperativeSentence_ = true;
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to read an imperative" );

							break;

						case Constants.GRAMMAR_ANSWER:
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION ) != Constants.RESULT_OK )
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
									return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I have found an illegal conjunction word parameter: " + wordParameter );
								}

							break;

						case Constants.GRAMMAR_SELECTION:
						case Constants.WORD_PARAMETER_SELECTION_IF:	// In case "then" is missing
						case Constants.WORD_PARAMETER_SELECTION_THEN:
						case Constants.WORD_PARAMETER_SELECTION_ELSE:
							switch( wordParameter )
								{
								case Constants.WORD_PARAMETER_SELECTION_IF:
									if( selectionListNr_ != Constants.NO_LIST_NR )
										{
										if( ++selectionLevel == Constants.MAX_LEVEL )
											return adminItem_.startSystemError( 1, moduleNameString_, "Selection overflow in list <" + adminItem_.adminListChar( selectionListNr_ ) + "> at word position " + wordOrderNr );
										}

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
									return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I have found an illegal selection word" );
								}

							break;

						default:
							if( readSentenceString != null &&

							( adminItem_.isTesting() ||
							adminItem_.isSystemStartingUp() ) )
								return adminItem_.startError( 1, moduleNameString_, "I have found an unknown word in sentence \"" + readSentenceString + "\" at position " + wordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + wordParameter );

							return adminItem_.startError( 1, moduleNameString_, "I have found an unknown word at position " + wordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + wordParameter );
						}
					}
				}
			while( !CommonVariables.hasShownWarning &&
			!adminItem_.hasRequestedRestart() &&
			currentReadItem_ != null &&
			( currentReadItem_ = currentReadItem_.nextReadItem() ) != null );

			if( selectionListNr_ != Constants.NO_LIST_NR )
				{
				if( adminItem_.checkForDuplicateSelection() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to check on a duplicate selection" );
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "I failed to get the first read item" );

		return Constants.RESULT_OK;
		}

	private byte processSentence( String readSentenceString )
		{
		ReadResultType readResult;
		boolean hasFoundFullPath;
		boolean hasFoundMoreInterpretations;
		boolean hasCreatedAllReadWords = false;
		short lastCreatedWordOrderNr;
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		short originalLanguageNr = CommonVariables.currentLanguageNr;
		short nLanguages = adminItem_.nLanguages();
		GrammarItem startOfGrammarItem;
		WordItem currentLanguageWordItem;
		ReadList readList;

		if( nLanguages > Constants.NO_LANGUAGE_NR )
			{
			if( nLanguages >= CommonVariables.currentLanguageNr )
				{
				do	{
					adminItem_.deleteTemporaryReadList();

					if( adminItem_.deleteUnusedInterpretations( true ) == Constants.RESULT_OK )
						{
						// Need to switch language
						if( currentLanguageNr != CommonVariables.currentLanguageNr )
							{
							if( adminItem_.assignLanguage( currentLanguageNr ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );
							}

						if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
							{
							if( currentLanguageWordItem.isCheckingGrammarNeeded() )
								{
								if( currentLanguageWordItem.checkGrammar() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to check the grammar" );
								}

							if( adminItem_.cleanupDeletedItems() == Constants.RESULT_OK )
								{
								hasFoundMoreInterpretations = false;
								startOfGrammarItem = currentLanguageWordItem.startOfGrammarItem();

									// Create read words from a given sentence
									if( ( readResult = adminItem_.createReadWords( readSentenceString ) ).result == Constants.RESULT_OK )
										hasCreatedAllReadWords = readResult.hasCreatedAllReadWords;
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to create the read words" );

								if( ( readList = adminItem_.readList ) != null )
									{
									hasFoundFullPath = false;
									hasFoundMoreInterpretations = false;
									readList.clearLastActivatedWordOrderNr();
									lastCreatedWordOrderNr = adminItem_.lastCreatedWordOrderNr();

									if( hasCreatedAllReadWords )
										{
										do	{
											if( findGrammarPath( Constants.NO_GRAMMAR_LEVEL, startOfGrammarItem ) == Constants.RESULT_OK )
												{
												hasFoundFullPath = ( currentParseWordOrderNr_ == lastCreatedWordOrderNr );

												if( !hasFoundFullPath )
													{
													if( ( readResult = readList.findMoreInterpretations() ).result == Constants.RESULT_OK )
														hasFoundMoreInterpretations = readResult.hasFoundMoreInterpretations;
													else
														return adminItem_.addError( 1, moduleNameString_, "I failed to find more interpretations" );
													}
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to find the grammar path for a sentence with language \"" + adminItem_.languageNameString( currentLanguageNr ) + "\"" );
											}
										while( !hasFoundFullPath &&
										hasFoundMoreInterpretations );
										}

									if( adminItem_.deleteUnusedInterpretations( !hasFoundFullPath ) == Constants.RESULT_OK )
										{
										// Successful interpretation of sentence
										if( hasFoundFullPath )
											{
											if( parseSentence( readSentenceString ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to parse the sentence" );
											}

										if( !hasFoundFullPath )
											{
											if( nLanguages <= 1 )
												// The only language
												currentLanguageNr++;
											else
												{
												// Failed for current language
												if( currentLanguageNr == originalLanguageNr )
													// Try all languages
													currentLanguageNr = 1;
												else
													currentLanguageNr++;

												// Skip current language (already tested)
												if( currentLanguageNr == originalLanguageNr )
													currentLanguageNr++;
												}
											}

										if( isDeletingRollbackInfo_ )
											adminItem_.deleteRollbackInfo();
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to delete the unused interpretations of the read words" );
									}
								else
									return adminItem_.startError( 1, moduleNameString_, "The read list isn't created yet" );
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to cleanup the deleted items" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to delete unused interpretations" );
					}
				while( !hasFoundFullPath &&
				currentLanguageNr <= nLanguages );

				// Failed to interpret sentence
				if( !hasFoundFullPath )
					{
					if( CommonVariables.currentLanguageNr != originalLanguageNr )
						{
						// Restore the original language
						if( adminItem_.assignLanguage( originalLanguageNr ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language after an interpretation failure" );
						}

					if( currentParseWordOrderNr_ == Constants.NO_ORDER_NR )
						{
						if( adminItem_.isSystemStartingUp() )
							return adminItem_.startSystemError( 1, moduleNameString_, "I don't understand this sentence. Please make sure you enter a sentence within my limited grammar definition" );

						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_NOT_CONFORM_GRAMMAR ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						if( adminItem_.isSystemStartingUp() )
							return adminItem_.startSystemError( 1, moduleNameString_, "I don't understand the sentence from the word at position: " + currentParseWordOrderNr_ );

						if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START, currentParseWordOrderNr_, Constants.INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_END ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The current language number exceeds the number of languages" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any language" );

		return Constants.RESULT_OK;
		}

	private byte readImperative( boolean isAction, boolean isNewStart, short selectionLevel, String readSentenceString )
		{
		short imperativeParameter = Constants.NO_IMPERATIVE_PARAMETER;
		short specificationWordParameter = Constants.NO_WORD_PARAMETER;
		ReadItem imperativeReadItem;
		ReadItem specificationReadItem;
		WordItem imperativeWordItem = null;

		startGeneralizationWordReadItem_ = null;
		endGeneralizationWordReadItem_ = null;

		if( scanSpecification( readSentenceString ) == Constants.RESULT_OK )
			{
			if( ( imperativeReadItem = startGeneralizationWordReadItem_ ) != null &&
			endGeneralizationWordReadItem_ != null )
				{
				do	{
					if( imperativeReadItem.isVerb() )
						{
						imperativeParameter = imperativeReadItem.wordParameter();
						imperativeWordItem = imperativeReadItem.readWordItem();
						}
					}
				while( ( imperativeReadItem = imperativeReadItem.nextReadItem() ) != null );

				// Only imperative word
				if( ( specificationReadItem = startSpecificationWordReadItem_ ) == null )
					{
					if( adminItem_.executeImperative( true, Constants.NO_LIST_NR, startGeneralizationWordReadItem_.wordParameter(), specificationWordParameter, Constants.WORD_TYPE_UNDEFINED, Constants.MAX_PROGRESS, startGeneralizationWordReadItem_.readString, startGeneralizationWordReadItem_.readWordItem(), null, startRelationWordReadItem_, endRelationReadItem_, null, null ) != Constants.RESULT_OK )
						{
						if( startGeneralizationWordReadItem_ == null )
							return adminItem_.addError( 1, moduleNameString_, "I failed to execute the single imperative" );

						return adminItem_.addError( 1, moduleNameString_, "I failed to execute the single imperative at position " + startGeneralizationWordReadItem_.wordOrderNr() );
						}
					}
				else
					{
					// Imperative word has specifications
					if( endSpecificationWordReadItem_ != null )
						{
						if( addReadSpecification( isAction, isNewStart, selectionLevel ) == Constants.RESULT_OK )
							{
							if( !CommonVariables.hasShownWarning &&
							selectionListNr_ == Constants.NO_LIST_NR )
								{
								do	{
									if( specificationReadItem.isUserDefined() ||

									( specificationReadItem.isSingularOrPluralNounWordType() &&
									!specificationReadItem.isNounInformation() ) )
										{
										// Make distinction between reading a normal file or a test file
										if( specificationReadItem.isNounFile() ||
										// Make distinction between showing a word or justification report
										specificationReadItem.isNounJustificationReport() )
											specificationWordParameter = specificationReadItem.wordParameter();
										else
											{
											if( adminItem_.executeImperative( true, Constants.NO_LIST_NR, imperativeParameter, ( specificationWordParameter == Constants.NO_WORD_PARAMETER ? specificationReadItem.wordParameter() : specificationWordParameter ), specificationReadItem.wordTypeNr(), Constants.MAX_PROGRESS, specificationReadItem.readString, imperativeWordItem, specificationReadItem.readWordItem(), startRelationWordReadItem_, endRelationReadItem_, null, null ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to execute an imperative with specifications" );
											}
										}
									}
								while( !adminItem_.hasRequestedRestart() &&
								!CommonVariables.hasShownWarning &&
								// Still read items available
								adminItem_.firstActiveReadItem() != null &&

								( !specificationReadItem.isVirtualListPreposition() ||
								// Loop shouldn't end if virtual list prepositions are used to e.g. show justification reports
								specificationWordParameter > Constants.NO_WORD_PARAMETER ) &&

								specificationReadItem != endSpecificationWordReadItem_ &&
								( specificationReadItem = specificationReadItem.nextReadItem() ) != null );
								}
							}
						else
							{
							if( startGeneralizationWordReadItem_ == null ||
							endGeneralizationWordReadItem_ == null )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add the generalization part of the read specification" );

							return adminItem_.addError( 1, moduleNameString_, "I failed to add the generalization part of the read specification between the positions " + startGeneralizationWordReadItem_.wordOrderNr() + " and " + endGeneralizationWordReadItem_.wordOrderNr() );
							}
						}
					else
						return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "The end specification read item is undefined" );
					}
				}
			else
				return adminItem_.startErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I couldn't find the imperative" );
			}
		else
			return adminItem_.addErrorWithAdminListNr( selectionListNr_, 1, moduleNameString_, "I failed to scan the generalization-specification" );

		return Constants.RESULT_OK;
		}

	private byte scanSpecification( String readSentenceString )
		{
		ContextResultType contextResult;
		boolean isSameWordTypeAsPreviousWord;
		boolean isStop = false;
		short currentWordOrderNr;
		short currentWordParameter;
		short currentWordTypeNr;
		short previousWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		short previousRelationWordOrderNr = Constants.NO_ORDER_NR;
		short previousGeneralizationWordOrderNr = Constants.NO_ORDER_NR;
		short previousSpecificationWordOrderNr = Constants.NO_ORDER_NR;
		ReadItem previousReadItem = null;
		WordItem currentReadWordItem;

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

		if( currentReadItem_ != null )
			{
			if( isLinkedGeneralizationConjunction_ )
				isLinkedGeneralizationConjunction_ = false;
			else
				{
				startGeneralizationWordReadItem_ = null;
				endGeneralizationWordReadItem_ = null;
				}

			do	{
				currentWordOrderNr = currentReadItem_.wordOrderNr();
				currentWordParameter = currentReadItem_.wordParameter();
				currentWordTypeNr = currentReadItem_.wordTypeNr();
				currentReadWordItem = currentReadItem_.readWordItem();

				setVariables( currentWordParameter );

				isSameWordTypeAsPreviousWord = ( previousWordTypeNr == currentWordTypeNr ||

												( adminItem_.isSingularOrPluralNoun( previousWordTypeNr ) &&
												adminItem_.isSingularOrPluralNoun( currentWordTypeNr ) ) );

				switch( currentReadItem_.grammarParameter )
					{
					case Constants.GRAMMAR_SENTENCE:
						if( !currentReadItem_.isSeparator() )
							{
							if( readSentenceString != null &&

							( adminItem_.isTesting() ||
							adminItem_.isSystemStartingUp() ) )
								return adminItem_.startError( 1, moduleNameString_, "I have found an unknown word in sentence \"" + readSentenceString + "\" at position " + currentWordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + currentWordParameter );

							return adminItem_.startError( 1, moduleNameString_, "I have found an unknown word at position " + currentWordOrderNr + " with grammar parameter " + currentReadItem_.grammarParameter + " and word parameter " + currentWordParameter );
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
						if( currentReadItem_.isAdjectivePrevious() )
							{
							isAssignment_ = true;
							isInactiveAssignment_ = true;
							}

						if( currentReadItem_.isAdjectiveEvery() )
							isEveryGeneralization_ = true;

						if( currentReadItem_.isNounPartOf() )
							isPartOf_ = true;

						if( currentReadItem_.isGeneralizationWord() )
							{
							if( isSameWordTypeAsPreviousWord &&
							previousGeneralizationWordOrderNr + 1 == currentWordOrderNr )
								{
								if( addMultipleWord( previousReadItem, currentReadItem_ ) == Constants.RESULT_OK )
									// Current read item has been deleted
									currentReadItem_ = previousReadItem;
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple generalization word" );
								}
							else
								CommonVariables.nUserGeneralizationWords++;

							if( currentReadWordItem != null )
								currentReadWordItem.isUserGeneralizationWord = true;

							previousGeneralizationWordOrderNr = currentWordOrderNr;
							}

						if( currentReadItem_.isDeterminerOrPronoun() )
							{
							if( generalizationContextNr_ == Constants.NO_CONTEXT_NR )
								{
								if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result == Constants.RESULT_OK )
									generalizationContextNr_ = contextResult.contextNr;
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a generalization pronoun context" );
								}
							else
								return adminItem_.startError( 1, moduleNameString_, "The generalization context number is already assigned" );
							}

						if( startGeneralizationWordReadItem_ == null )
							startGeneralizationWordReadItem_ = currentReadItem_;

						endGeneralizationWordReadItem_ = currentReadItem_;

						break;

					case Constants.GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION:
						isStop = true;
						isLinkedGeneralizationConjunction_ = true;

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
							if( isSameWordTypeAsPreviousWord &&
							previousRelationWordOrderNr + 1 == currentWordOrderNr )
								{
								if( addMultipleWord( previousReadItem, currentReadItem_ ) == Constants.RESULT_OK )
									// Current read item has been deleted
									currentReadItem_ = previousReadItem;
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple relation word" );
								}
							else
								CommonVariables.nUserRelationWords++;

							if( currentReadWordItem != null )
								currentReadWordItem.isUserRelationWord = true;

							previousRelationWordOrderNr = currentWordOrderNr;
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
						isActiveAssignment_ = true;

						if( isArchivedAssignment_ )
							isUniqueUserRelation_ = true;

						if( currentReadItem_.isDeterminerOrPronoun() )
							{
							if( specificationContextNr_ == Constants.NO_CONTEXT_NR )
								{
								if( ( contextResult = addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result == Constants.RESULT_OK )
									specificationContextNr_ = contextResult.contextNr;
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification pronoun context" );
								}
							else
								return adminItem_.startError( 1, moduleNameString_, "The specification context number is already assigned" );
							}

						// Don't insert a break statement here

					case Constants.GRAMMAR_SPECIFICATION_PART:
					case Constants.GRAMMAR_SPECIFICATION_WORD:
					case Constants.GRAMMAR_TEXT:
						if( currentReadItem_.isSpecificationWord() )
							{
							if( isSameWordTypeAsPreviousWord &&
							previousSpecificationWordOrderNr + 1 == currentWordOrderNr )
								{
								if( addMultipleWord( previousReadItem, currentReadItem_ ) == Constants.RESULT_OK )
									// Current read item has been deleted
									currentReadItem_ = previousReadItem;
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple specification word" );
								}
							else
								CommonVariables.nUserSpecificationWords++;

							if( currentReadWordItem != null )
								currentReadWordItem.isUserSpecificationWord = true;

							previousSpecificationWordOrderNr = currentWordOrderNr;
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
						if( previousReadItem != null )
							{
							isStop = true;
							currentReadItem_ = previousReadItem;
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "I have found a word that doesn't belong to an assignment or a specification" );
					}

				previousWordTypeNr = currentWordTypeNr;
				previousReadItem = currentReadItem_;
				}
			while( !isStop &&
			( currentReadItem_ = currentReadItem_.nextReadItem() ) != null );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The read item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminReadSentence( AdminItem adminItem )
		{
		String errorString = null;

		isAssignment_ = false;
		isActiveAssignment_ = false;
		isInactiveAssignment_ = false;
		isArchivedAssignment_ = false;
		isConditional_ = false;
		isDeletingRollbackInfo_ = true;
		isEveryGeneralization_ = false;
		isExclusiveSpecification_ = false;
		isNegative_ = false;
		isPartOf_ = false;
		isPossessive_ = false;
		isLinkedGeneralizationConjunction_ = false;
		isSpecificationGeneralization_ = false;
		isUniqueUserRelation_ = false;
		isUserImperativeSentence_ = false;

		assumptionLevel_ = Constants.NO_ASSUMPTION_LEVEL;
		currentParseWordOrderNr_ = Constants.NO_ORDER_NR;
		prepositionParameter_ = Constants.NO_PREPOSITION_PARAMETER;
		questionParameter_ = Constants.NO_QUESTION_PARAMETER;
		selectionListNr_ = Constants.NO_LIST_NR;

		generalizationContextNr_ = Constants.NO_CONTEXT_NR;
		specificationContextNr_ = Constants.NO_CONTEXT_NR;

		currentReadItem_ = null;
		startGeneralizationWordReadItem_ = null;
		endGeneralizationWordReadItem_ = null;
		startSpecificationWordReadItem_ = null;
		endSpecificationWordReadItem_ = null;
		startRelationWordReadItem_ = null;
		endRelationReadItem_ = null;

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
				Console.addError( "\nClass:" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nMethod:\t" + errorString + "\nError:\t\t%s.\n" );
				}
			}
		}


	// Protected methods

	protected void dontDeletedRollbackInfo()
		{
		isDeletingRollbackInfo_ = false;
		}

	protected boolean isActiveUserAssignment()
		{
		return isActiveAssignment_;
		}

	protected boolean isPossessiveUserSpecification()
		{
		return isPossessive_;
		}

	protected boolean isUserImperativeSentence()
		{
		return isUserImperativeSentence_;
		}

	protected boolean isUserQuestion()
		{
		return ( questionParameter_ > Constants.NO_QUESTION_PARAMETER );
		}

	protected boolean isUserSelectionSentence()
		{
		return ( selectionListNr_ != Constants.NO_LIST_NR );
		}

	protected byte processReadSentence( String readSentenceString )
		{
		int startSentenceNr = CommonVariables.currentSentenceNr;

		isDeletingRollbackInfo_ = true;

		CommonVariables.hasFoundAnswerToQuestion = false;
		CommonVariables.hasShownArticleNotification = false;
		CommonVariables.hasShownWarning = false;
		CommonVariables.isFirstAnswerToQuestion = true;
		CommonVariables.isQuestionAlreadyAnswered = false;

		initializeVariablesInAllWords();

		if( processSentence( readSentenceString ) == Constants.RESULT_OK )
			{
			// Skip processing after Undo or Redo
			if( isDeletingRollbackInfo_ &&
			readSentenceString != null &&
			!CommonVariables.hasShownWarning &&
			!adminItem_.hasRequestedRestart() &&
			startSentenceNr == CommonVariables.currentSentenceNr )
				{
				if( adminItem_.checkIntegrityOfStoredUserSentence( readSentenceString ) == Constants.RESULT_OK )
					{
					// Has passed integrity check
					if( !CommonVariables.hasShownWarning )
						{
						// Skip when no changes are made
						if( adminItem_.hasFoundAnyChangeMadeByThisSentence() )
							{
							// Show self-generated conclusions of the current sentence
							if( adminItem_.writeSelfGeneratedInfo( true, false, false ) == Constants.RESULT_OK )
								{
								// Show self-generated assumptions of the current sentence
								if( adminItem_.writeSelfGeneratedInfo( false, true, false ) == Constants.RESULT_OK )
									{
									// Show self-generated questions of the current sentence
									if( adminItem_.writeSelfGeneratedInfo( false, false, true ) == Constants.RESULT_OK )
										{
										if( !adminItem_.hasFoundUnprocessedNegativeConclusion() )
											{
											if( checkForStructuralIntegrity() == Constants.RESULT_OK )
												{
												// In case you are planning to save the data in a database
/*												if( storeChangesInFutureDatabase() != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to store the changes in a future database" );
*/												}
											else
												return adminItem_.addError( 1, moduleNameString_, "The system has problem with the structural integrity" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "I have found an unprocessed negative conclusion" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated questions" );
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated assumptions" );
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated conclusions" );
							}

						if( isUserQuestion() )
							{
							if( adminItem_.answerQuestions() != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to answer questions" );
							}
						}
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to check the integrity of the stored user sentence \"" + readSentenceString + "\"" );
				}
			}
		else
			{
			if( readSentenceString == null )
				return adminItem_.addError( 1, moduleNameString_, "I failed to process an undefined sentence" );

			return adminItem_.addError( 1, moduleNameString_, "I failed to process sentence: \"" + readSentenceString + "\"" );
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
