/*
 *	Class:			AdminReadSentence
 *	Supports class:	AdminItem
 *	Purpose:		To read and analyze sentences
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
#include "ReadList.cpp"
#include "WordTypeItem.cpp"

class AdminReadSentence
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundFeminineSpecificationWord_;
	bool hasFoundMasculineSpecificationWord_;
	bool hasFoundNeutralSpecificationWord_;
	bool isAssignment_;
	bool isInactiveAssignment_;
	bool isArchivedAssignment_;
	bool isConditional_;
	bool isDeletingRollbackInfo_;
	bool isEveryGeneralization_;
	bool isExclusiveSpecification_;
	bool isNegative_;
	bool isPartOf_;
	bool isPossessive_;
	bool isLinkedGeneralizationConjunction_;
	bool isSpecificationGeneralization_;
	bool isUniqueRelation_;

	unsigned short assumptionLevel_;
	unsigned short currentParseWordOrderNr_;
	unsigned short prepositionParameter_;
	unsigned short questionParameter_;

	unsigned int generalizationContextNr_;
	unsigned int specificationContextNr_;

	ReadItem *currentReadItem_;
	ReadItem *startGeneralizationWordReadItem_;
	ReadItem *endGeneralizationWordReadItem_;
	ReadItem *startSpecificationWordReadItem_;
	ReadItem *endSpecificationWordReadItem_;
	ReadItem *startRelationWordReadItem_;
	ReadItem *endRelationReadItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

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
			case WORD_PARAMETER_ADVERB_ASSUMPTION_MAY_BE:
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
				isNegative_ = true;
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

	ResultType addMultipleWord( ReadItem *firstReadItem, ReadItem *secondReadItem )
		{
		WordResultType wordResult;
		unsigned short wordParameter;
		unsigned short firstWordTypeNr;
		unsigned short secondWordTypeNr;
		WordItem *firstWordItem;
		WordItem *secondWordItem;
		WordItem *multipleWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( firstReadItem != NULL )
			{
			if( secondReadItem != NULL )
				{
				if( ( firstWordItem = firstReadItem->readWordItem() ) != NULL )
					{
					if( ( secondWordItem = secondReadItem->readWordItem() ) != NULL )
						{
						wordParameter = firstReadItem->wordParameter();
						firstWordTypeNr = firstReadItem->wordTypeNr();
						secondWordTypeNr = secondReadItem->wordTypeNr();

						if( firstWordTypeNr == secondWordTypeNr ||

						( adminItem_->isSingularOrPluralNoun( firstWordTypeNr ) &&
						adminItem_->isSingularOrPluralNoun( secondWordTypeNr ) ) )
							{
							if( ( wordResult = addMultipleWord( wordParameter, firstWordTypeNr, secondWordTypeNr, firstWordItem, secondWordItem, NULL ) ).result == RESULT_OK )
								{
								if( firstWordTypeNr != secondWordTypeNr )
									{
									if( addMultipleWord( wordParameter, firstWordTypeNr, firstWordTypeNr, firstWordItem, secondWordItem, wordResult.createdWordItem ).result != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a multiple grammar word" );
									}

								if( ( multipleWordItem = ( wordResult.createdWordItem == NULL ? wordResult.foundWordItem : wordResult.createdWordItem ) ) != NULL )
									{
									if( firstReadItem->changeReadWord( secondWordTypeNr, multipleWordItem ) == RESULT_OK )
										{
										if( adminItem_->readList != NULL )
											{
											if( adminItem_->readList->deleteItem( false, secondReadItem ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the second read item" );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read list isn't created yet" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an active read item" );
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The multiple word is undefined" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a multiple grammar word" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "Both given read items have different word types" );
						}
					else
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given second read item has no read word" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given first read item has no read word" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given second read item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given first read item is undefined" );

		return RESULT_OK;
		}

	ResultType addReadSpecification( bool isAction, bool isNewStart, unsigned short selectionLevel, unsigned short selectionListNr )
		{
		bool isConditional = ( isConditional_ || selectionListNr != NO_LIST_NR );
		bool isUserSpecificationExclusivelyFeminineOrMasculine;
		unsigned short imperativeParameter = NO_IMPERATIVE_PARAMETER;
		unsigned short specificationWordParameter = NO_WORD_PARAMETER;
		ReadItem *currentGeneralizationReadItem = startGeneralizationWordReadItem_;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addReadSpecification";

		if( currentGeneralizationReadItem != NULL )
			{
			if( endGeneralizationWordReadItem_ != NULL )
				{
				if( selectionListNr == NO_LIST_NR &&
				commonVariables_->nUserGeneralizationWords == 1 &&
				commonVariables_->nUserSpecificationWords > 1 &&
				commonVariables_->nUserRelationWords > 1 )
					{
					// Ambiguity: Missing relation context
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MULTIPLE_SPECIFICATIONS_RELATIONS ) != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to write an interface warning about ambiguity" );
					}
				else
					{
					adminItem_->initializeAdminAssumptionVariables();
					adminItem_->initializeAdminConclusionVariables();
					adminItem_->initializeAdminContextVariables();
					adminItem_->initializeAdminSpecificationVariables();

					isUserSpecificationExclusivelyFeminineOrMasculine = ( !hasFoundNeutralSpecificationWord_ &&

																		( ( hasFoundFeminineSpecificationWord_ &&
																		!hasFoundMasculineSpecificationWord_ ) ||

																		( !hasFoundFeminineSpecificationWord_ &&
																		hasFoundMasculineSpecificationWord_ ) ) );

					do	{
						switch( currentGeneralizationReadItem->grammarParameter )
							{
							case GRAMMAR_IMPERATIVE:
								imperativeParameter = currentGeneralizationReadItem->wordParameter();
								// Don't insert a break statement

							case GRAMMAR_GENERALIZATION_WORD:
								if( adminItem_->addUserSpecifications( isAction, isAssignment_, isConditional, isInactiveAssignment_, isArchivedAssignment_, isEveryGeneralization_, isExclusiveSpecification_, isNegative_, isNewStart, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueRelation_, isUserSpecificationExclusivelyFeminineOrMasculine, assumptionLevel_, prepositionParameter_, questionParameter_, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, generalizationContextNr_, specificationContextNr_, currentGeneralizationReadItem, startSpecificationWordReadItem_, endSpecificationWordReadItem_, startRelationWordReadItem_, endRelationReadItem_ ) != RESULT_OK )
									return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to add the read user specifications" );
							}
						}
					while( !commonVariables_->hasShownWarning &&
					currentGeneralizationReadItem != endGeneralizationWordReadItem_ &&
					( currentGeneralizationReadItem = currentGeneralizationReadItem->nextReadItem() ) != NULL );
					}
				}
			else
				return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The end generalization read item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The start generalization read item is undefined" );

		return RESULT_OK;
		}

	ResultType checkForStructuralIntegrity()
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForStructuralIntegrity";

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( currentWordItem->checkSpecificationsForReplacedJustifications() == RESULT_OK )
					{
					if( currentWordItem->checkJustificationsForReplacedSpecifications() == RESULT_OK )
						{
						if( currentWordItem->checkForUnreferencedJustifications() != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for unreferenced justifications in word \"", currentWordItem->anyWordTypeString(), "\"" );
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for the justifications for replaced specifications in word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for the specifications for replaced justifications in word \"", currentWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "findGrammarPath";

		if( grammarLevel < MAX_GRAMMAR_LEVEL )
			{
			if( parseGrammarItem != NULL )
				{
				if( adminItem_->readList != NULL )
					{
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
							parseGrammarItem->isNewStart() )	// Skip first grammar definition item, if not is a data item
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
									if( ( readResult = adminItem_->readList->selectMatchingWordType( currentParseWordOrderNr_, definitionParseGrammarItem->grammarParameter(), definitionParseGrammarItem->grammarWordTypeNr() ) ).result == RESULT_OK )
										{
										if( readResult.hasFoundMatchingWordType )
											currentParseWordOrderNr_++;
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to select a matching word type" );
									}
								else
									{
									if( grammarLevel + 1 < MAX_GRAMMAR_LEVEL )
										{
										if( parseGrammarItem->definitionGrammarItem != NULL )
											{
											if( findGrammarPath( ( grammarLevel + 1 ), parseGrammarItem->definitionGrammarItem ) == RESULT_OK )
												{
												if( currentParseWordOrderNr_ == previousWordOrderNr )	// Unsuccessful
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
														if( adminItem_->readList->activateInactiveReadWords( currentParseWordOrderNr_ ) != RESULT_OK )
															return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to activate inactive read words from position: ", currentParseWordOrderNr_ );
														}
													}
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the grammar path at grammar word \"", parseGrammarItem->grammarString(), "\"" );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "Grammar word \"", parseGrammarItem->grammarString(), "\" isn't defined in the grammar file" );
										}
									else
										return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #", grammarLevel );
									}
								}

							if( commonVariables_->result == RESULT_OK )
								{
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
							}
						while( commonVariables_->result == RESULT_OK &&

						( isWaitingForNewStart ||
						isWaitingForChoiceEnd ||
						currentParseWordOrderNr_ > previousWordOrderNr ) &&

						parseGrammarItem != NULL &&
						definitionParseGrammarItem->activeSentenceNr() == parseGrammarItem->activeSentenceNr() );

						if( !isWaitingForNewStart &&
						currentParseWordOrderNr_ > startWordOrderNr &&
						currentParseWordOrderNr_ == previousWordOrderNr )
							currentParseWordOrderNr_ = startWordOrderNr;
						}
					while( commonVariables_->result == RESULT_OK &&
					currentParseWordOrderNr_ == startWordOrderNr &&
					definitionParseGrammarItem != NULL &&
					( parseGrammarItem = definitionParseGrammarItem->nextDefinitionGrammarItem ) != NULL );

					if( commonVariables_->result == RESULT_OK &&
					definitionParseGrammarItem != NULL &&
					definitionParseGrammarItem->isGrammarDefinition() &&

					( previousWordOrderNr > startWordOrderNr ||
					currentParseWordOrderNr_ > startWordOrderNr ) )
						{
						if( adminItem_->readList->setGrammarParameter( ( currentParseWordOrderNr_ > startWordOrderNr ), startWordOrderNr, ( currentParseWordOrderNr_ > startWordOrderNr ? currentParseWordOrderNr_ : previousWordOrderNr ), definitionParseGrammarItem ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to set the grammar parameter of the read words between the positions ", startWordOrderNr, " and ", currentParseWordOrderNr_ );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read list isn't created yet" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given parse grammar item is undefined" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given grammar level is too high: #", grammarLevel );

		return RESULT_OK;
		}

	ResultType parseSentence( char *readSentenceString )
		{
		bool isAction = false;
		bool isNewStart = true;
		unsigned short wordOrderNr;
		unsigned short wordParameter;
		unsigned short selectionLevel = NO_SELECTION_LEVEL;
		unsigned short selectionListNr = NO_LIST_NR;
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "parseSentence";

		isLinkedGeneralizationConjunction_ = false;

		adminItem_->initializeLinkedWord();
		endGeneralizationWordReadItem_ = NULL;
		startGeneralizationWordReadItem_ = NULL;

		if( ( currentReadItem_ = adminItem_->firstActiveReadItem() ) != NULL )
			{
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
							if( readSpecification( isAction, isNewStart, selectionLevel, selectionListNr, readSentenceString ) == RESULT_OK )
								isNewStart = false;
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a specification" );

							break;

						case GRAMMAR_IMPERATIVE:
							if( readImperative( isAction, isNewStart, selectionLevel, selectionListNr, readSentenceString ) == RESULT_OK )
								isNewStart = false;
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read an imperative" );

							break;

						case GRAMMAR_ANSWER:
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION ) != RESULT_OK )
								return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to write an interface warning" );

							break;

						case GRAMMAR_SENTENCE_CONJUNCTION:
							switch( wordParameter )
								{
								case WORD_PARAMETER_SYMBOL_COMMA:
								case WORD_PARAMETER_CONJUNCTION_AND:
								case WORD_PARAMETER_CONJUNCTION_DUTCH_ALS:		// Typically for Dutch: in zowel ... als ...
									break;

								case WORD_PARAMETER_CONJUNCTION_OR:
									isNewStart = true;

									break;

								default:
									return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I found an illegal conjunction word parameter: ", wordParameter );
								}

							break;

						case GRAMMAR_SELECTION:
						case WORD_PARAMETER_SELECTION_IF:	// In case "then" is missing
						case WORD_PARAMETER_SELECTION_THEN:
						case WORD_PARAMETER_SELECTION_ELSE:
							switch( wordParameter )
								{
								case WORD_PARAMETER_SELECTION_IF:
									if( selectionListNr != NO_LIST_NR )
										{
										if( ++selectionLevel == MAX_LEVEL )
											{
											sprintf( errorString, "Selection overflow in list <%c> at position %u", adminItem_->adminListChar( selectionListNr ), wordOrderNr );
											return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, errorString );
											}
										}

									isNewStart = true;
									selectionListNr = ADMIN_CONDITION_LIST;

									break;

								case WORD_PARAMETER_SELECTION_THEN:
									isNewStart = true;
									isAction = true;
									selectionListNr = ADMIN_ACTION_LIST;

									break;

								case WORD_PARAMETER_SELECTION_ELSE:
									isNewStart = true;
									isAction = false;
									selectionListNr = ADMIN_ALTERNATIVE_LIST;

									break;

								default:
									return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I found an illegal selection word" );
								}

							break;

						default:
							if( readSentenceString != NULL &&
							adminItem_->isSystemStartingUp() )
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an unknown word in sentence \"", readSentenceString, "\" at position ", wordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", wordParameter );

							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an unknown word at position ", wordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", wordParameter );
						}
					}
				}
			while( !commonVariables_->hasShownWarning &&
			!adminItem_->hasRequestedRestart() &&
			currentReadItem_ != NULL &&
			( currentReadItem_ = currentReadItem_->nextReadItem() ) != NULL );

			if( selectionListNr != NO_LIST_NR )
				{
				if( adminItem_->checkForDuplicateSelection() != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check on a duplicate selection" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to get the first read item" );

		return RESULT_OK;
		}

	ResultType processSentence( char *readSentenceString )
		{
		ReadResultType readResult;
		bool hasFoundFullPath;
		bool hasFoundMoreInterpretations;
		bool hasCreatedAllReadWords = false;
		unsigned short currentLanguageNr = commonVariables_->currentGrammarLanguageNr;
		unsigned short originalLanguageNr = commonVariables_->currentGrammarLanguageNr;
		unsigned short nLanguages = myWordItem_->nGrammarLanguages();
		GrammarItem *startOfGrammarItem;
		WordItem *currentGrammarLanguageWordItem;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "processSentence";

		if( nLanguages > 0 )
			{
			if( nLanguages >= commonVariables_->currentGrammarLanguageNr )
				{
				do	{
					adminItem_->deleteTemporaryReadList();

					if( adminItem_->deleteUnusedInterpretations( true ) == RESULT_OK )
						{
						if( currentLanguageNr != commonVariables_->currentGrammarLanguageNr )	// Need to switch language
							{
							if( adminItem_->assignGrammarAndInterfaceLanguage( currentLanguageNr ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the grammar and interface language" );
							}

						if( ( currentGrammarLanguageWordItem = commonVariables_->currentGrammarLanguageWordItem ) != NULL )
							{
							if( currentGrammarLanguageWordItem->isCheckingGrammarNeeded() )
								{
								if( currentGrammarLanguageWordItem->checkGrammar() != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check the grammar" );
								}

							if( adminItem_->cleanupDeletedItems() == RESULT_OK )
								{
								hasFoundMoreInterpretations = false;
								startOfGrammarItem = currentGrammarLanguageWordItem->startOfGrammarItem();

									// Create read words from a given sentence
									if( ( readResult = adminItem_->createReadWords( readSentenceString ) ).result == RESULT_OK )
										hasCreatedAllReadWords = readResult.hasCreatedAllReadWords;
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create the read words" );

								if( adminItem_->readList != NULL )
									{
									hasFoundFullPath = false;
									hasFoundMoreInterpretations = false;
									adminItem_->readList->initForParsingReadWords();

									if( hasCreatedAllReadWords )
										{
										do	{
											if( findGrammarPath( NO_GRAMMAR_LEVEL, startOfGrammarItem ) == RESULT_OK )
												{
												hasFoundFullPath = ( currentParseWordOrderNr_ == adminItem_->readList->lastCreatedWordOrderNr() );

												if( !hasFoundFullPath )
													{
													if( ( readResult = adminItem_->readList->findMoreInterpretations() ).result == RESULT_OK )
														hasFoundMoreInterpretations = readResult.hasFoundMoreInterpretations;
													else
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find more interpretations" );
													}
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find the grammar path for a sentence with grammar language \"", myWordItem_->grammarLanguageNameString( currentLanguageNr ), "\"" );
											}
										while( !hasFoundFullPath &&
										hasFoundMoreInterpretations );
										}

									if( adminItem_->deleteUnusedInterpretations( !hasFoundFullPath ) == RESULT_OK )
										{
										if( hasFoundFullPath )	// Successful interpretation of sentence
											{
											if( parseSentence( readSentenceString ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to parse the sentence" );
											}

										if( !hasFoundFullPath )
											{
											if( nLanguages <= 1 )
												currentLanguageNr++;							// The only language
											else
												{
												if( currentLanguageNr == originalLanguageNr )	// Failed for current language
													currentLanguageNr = 1;						// Try all languages
												else
													currentLanguageNr++;

												if( currentLanguageNr == originalLanguageNr )	// Skip current language (already tested)
													currentLanguageNr++;
												}
											}

										if( isDeletingRollbackInfo_ )
											adminItem_->deleteRollbackInfo();
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the unused interpretations of the read words" );
									}
								else
									return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read list isn't created yet" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The current grammar language word item is undefined" );
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete unused interpretations" );
					}
				while( !hasFoundFullPath &&
				currentLanguageNr <= nLanguages );

				if( !hasFoundFullPath )		// Failed to interpret sentence
					{
					if( commonVariables_->currentGrammarLanguageNr != originalLanguageNr )
						{
						if( adminItem_->assignGrammarAndInterfaceLanguage( originalLanguageNr ) != RESULT_OK )	// Restore the original language
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the grammar and interface language after an interpretation failure" );
						}

					if( currentParseWordOrderNr_ == NO_ORDER_NR )
						{
						if( adminItem_->isSystemStartingUp() )
							return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "I don't understand this sentence. Please make sure you enter a sentence within my limited grammar definition" );

						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_NOT_CONFORM_GRAMMAR ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						if( adminItem_->isSystemStartingUp() )
							{
							sprintf( errorString, "I don't understand the sentence from the word at position %u", currentParseWordOrderNr_ );
							return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, errorString );
							}

						if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START, (unsigned int)currentParseWordOrderNr_, INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_END ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The current grammar language number exceeds the number of languages" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any grammar language" );

		return RESULT_OK;
		}

	ResultType readImperative( bool isAction, bool isNewStart, unsigned short selectionLevel, unsigned short selectionListNr, char *readSentenceString )
		{
		unsigned short imperativeParameter = NO_IMPERATIVE_PARAMETER;
		unsigned short specificationWordParameter = NO_WORD_PARAMETER;
		ReadItem *imperativeReadItem;
		ReadItem *specificationReadItem;
		WordItem *imperativeWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readImperative";

		startGeneralizationWordReadItem_ = NULL;
		endGeneralizationWordReadItem_ = NULL;

		if( scanSpecification( readSentenceString ) == RESULT_OK )
			{
			if( ( imperativeReadItem = startGeneralizationWordReadItem_ ) != NULL &&
			endGeneralizationWordReadItem_ != NULL )
				{
				do	{
					if( imperativeReadItem->isVerb() )
						{
						imperativeParameter = imperativeReadItem->wordParameter();
						imperativeWordItem = imperativeReadItem->readWordItem();
						}
					}
				while( ( imperativeReadItem = imperativeReadItem->nextReadItem() ) != NULL );

				if( ( specificationReadItem = startSpecificationWordReadItem_ ) == NULL )	// Only imperative word
					{
					if( adminItem_->executeImperative( true, NO_LIST_NR, startGeneralizationWordReadItem_->wordParameter(), specificationWordParameter, WORD_TYPE_UNDEFINED, MAX_PROGRESS, startGeneralizationWordReadItem_->readString, startGeneralizationWordReadItem_->readWordItem(), NULL, startRelationWordReadItem_, endRelationReadItem_, NULL, NULL ) != RESULT_OK )
						{
						if( startGeneralizationWordReadItem_ == NULL )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute the single imperative" );

						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute the single imperative at position ", startGeneralizationWordReadItem_->wordOrderNr() );
						}
					}
				else	// Imperative word has specifications
					{
					if( endSpecificationWordReadItem_ != NULL )
						{
						if( addReadSpecification( isAction, isNewStart, selectionLevel, selectionListNr ) == RESULT_OK )
							{
							if( !commonVariables_->hasShownWarning &&
							selectionListNr == NO_LIST_NR )
								{
								do	{
									if( specificationReadItem->isUserDefined() ||

									( specificationReadItem->isNoun() &&
									!specificationReadItem->isInformationNoun() ) )
										{
										if( specificationReadItem->isNounFile() ||				// Make distinction between reading a normal file or a test file
										specificationReadItem->isJustificationReportNoun() )	// Make distinction between showing a word or justification report
											specificationWordParameter = specificationReadItem->wordParameter();
										else
											{
											if( adminItem_->executeImperative( true, NO_LIST_NR, imperativeParameter, ( specificationWordParameter == NO_WORD_PARAMETER ? specificationReadItem->wordParameter() : specificationWordParameter ), specificationReadItem->wordTypeNr(), MAX_PROGRESS, specificationReadItem->readString, imperativeWordItem, specificationReadItem->readWordItem(), startRelationWordReadItem_, endRelationReadItem_, NULL, NULL ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute an imperative with specifications" );
											}
										}
									}
								while( !adminItem_->hasRequestedRestart() &&
								!commonVariables_->hasShownWarning &&
								adminItem_->firstActiveReadItem() != NULL &&				// Still read items available

								( !specificationReadItem->isVirtualListPreposition() ||
								specificationWordParameter > NO_WORD_PARAMETER ) &&		// Loop shouldn't end when virtual list prepositions are used to e.g. show justification reports

								specificationReadItem != endSpecificationWordReadItem_ &&
								( specificationReadItem = specificationReadItem->nextReadItem() ) != NULL );
								}
							}
						else
							{
							if( startGeneralizationWordReadItem_ == NULL ||
							endGeneralizationWordReadItem_ == NULL )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add the generalization part of the read specification" );

							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add the generalization part of the read specification between the positions ", startGeneralizationWordReadItem_->wordOrderNr(), " and ", endGeneralizationWordReadItem_->wordOrderNr() );
							}
						}
					else
						return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "The end specification read item is undefined" );
					}
				}
			else
				return myWordItem_->startErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I couldn't find the imperative" );
			}
		else
			return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to scan the generalization-specification" );

		return RESULT_OK;
		}

	ResultType readSpecification( bool isAction, bool isNewStart, unsigned short selectionLevel, unsigned short selectionListNr, char *readSentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readSpecification";

		if( scanSpecification( readSentenceString ) == RESULT_OK )
			{
			if( addReadSpecification( isAction, isNewStart, selectionLevel, selectionListNr ) != RESULT_OK )
				return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to add the read specification" );
			}
		else
			return myWordItem_->addErrorInItem( adminItem_->adminListChar( selectionListNr ), functionNameString, moduleNameString_, "I failed to scan the generalization-specification" );

		return RESULT_OK;
		}

	ResultType scanSpecification( char *readSentenceString )
		{
		ContextResultType contextResult;
		bool isSameWordTypeAsPreviousWord;
		bool isStop = false;
		unsigned short currentWordOrderNr;
		unsigned short currentWordParameter;
		unsigned short currentWordTypeNr;
		unsigned short previousWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned short previousRelationWordOrderNr = NO_ORDER_NR;
		unsigned short previousGeneralizationWordOrderNr = NO_ORDER_NR;
		unsigned short previousSpecificationWordOrderNr = NO_ORDER_NR;
		ReadItem *previousReadItem = NULL;
		WordItem *currentReadWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "scanSpecification";

		hasFoundFeminineSpecificationWord_ = false;
		hasFoundMasculineSpecificationWord_ = false;
		hasFoundNeutralSpecificationWord_ = false;
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
		isUniqueRelation_ = false;

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

		commonVariables_->isUserQuestion = false;
		commonVariables_->nUserGeneralizationWords = 0;
		commonVariables_->nUserSpecificationWords = 0;
		commonVariables_->nUserRelationWords = 0;

		if( currentReadItem_ != NULL )
			{
			if( isLinkedGeneralizationConjunction_ )
				isLinkedGeneralizationConjunction_ = false;
			else
				{
				startGeneralizationWordReadItem_ = NULL;
				endGeneralizationWordReadItem_ = NULL;
				}

			do	{
				currentWordOrderNr = currentReadItem_->wordOrderNr();
				currentWordParameter = currentReadItem_->wordParameter();
				currentWordTypeNr = currentReadItem_->wordTypeNr();
				currentReadWordItem = currentReadItem_->readWordItem();

				setVariables( currentWordParameter );

				isSameWordTypeAsPreviousWord = ( previousWordTypeNr == currentWordTypeNr ||

												( adminItem_->isSingularOrPluralNoun( previousWordTypeNr ) &&
												adminItem_->isSingularOrPluralNoun( currentWordTypeNr ) ) );

				switch( currentReadItem_->grammarParameter )
					{
					case GRAMMAR_SENTENCE:
						if( !currentReadItem_->isSeparator() )
							{
							if( readSentenceString != NULL &&
							adminItem_->isSystemStartingUp() )
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an unknown word in sentence \"", readSentenceString, "\" at position ", currentWordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", currentWordParameter );

							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an unknown word at position ", currentWordOrderNr, " with grammar parameter ", currentReadItem_->grammarParameter, " and word parameter ", currentWordParameter );
							}

						break;

					case GRAMMAR_ANSWER:
						break;	// Needs to be implemented

					case GRAMMAR_GENERALIZATION_ASSIGNMENT:		// Assignment generalization-specification
						isAssignment_ = true;

						// Don't insert a break statement here

					case GRAMMAR_IMPERATIVE:
					case GRAMMAR_GENERALIZATION_SPECIFICATION:
					case GRAMMAR_GENERALIZATION_PART:
					case GRAMMAR_GENERALIZATION_WORD:
						if( currentReadItem_->isAdjectivePrevious() )
							{
							isAssignment_ = true;
							isInactiveAssignment_ = true;
							}

						if( currentReadItem_->isAdjectiveEvery() )
							isEveryGeneralization_ = true;

						if( currentReadItem_->isNounPartOf() )
							isPartOf_ = true;

						if( currentReadItem_->isGeneralizationWord() )
							{
							if( isSameWordTypeAsPreviousWord &&
							previousGeneralizationWordOrderNr + 1 == currentWordOrderNr )
								{
								if( addMultipleWord( previousReadItem, currentReadItem_ ) == RESULT_OK )
									currentReadItem_ = previousReadItem;	// Current read item is deleted
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a multiple generalization word" );
								}
							else
								commonVariables_->nUserGeneralizationWords++;

							if( currentReadWordItem != NULL )
								currentReadWordItem->isUserGeneralizationWord = true;

							previousGeneralizationWordOrderNr = currentWordOrderNr;
							}

						if( currentReadItem_->isDeterminerOrPronoun() )
							{
							if( generalizationContextNr_ == NO_CONTEXT_NR )
								{
								if( ( contextResult = adminItem_->addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result == RESULT_OK )
									generalizationContextNr_ = contextResult.contextNr;
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a generalization pronoun context" );
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization context number is already assigned" );
							}

						if( startGeneralizationWordReadItem_ == NULL )
							startGeneralizationWordReadItem_ = currentReadItem_;

						endGeneralizationWordReadItem_ = currentReadItem_;

						break;

					case GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION:
						isStop = true;
						isLinkedGeneralizationConjunction_ = true;

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
							if( isSameWordTypeAsPreviousWord &&
							previousRelationWordOrderNr + 1 == currentWordOrderNr )
								{
								if( addMultipleWord( previousReadItem, currentReadItem_ ) == RESULT_OK )
									currentReadItem_ = previousReadItem;	// Current read item is deleted
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a multiple relation word" );
								}
							else
								commonVariables_->nUserRelationWords++;

							if( currentReadWordItem != NULL )
								currentReadWordItem->isUserRelationWord = true;

							previousRelationWordOrderNr = currentWordOrderNr;
							}

						if( startSpecificationWordReadItem_ == NULL )
							startSpecificationWordReadItem_ = currentReadItem_;

						endSpecificationWordReadItem_ = currentReadItem_;

						if( startRelationWordReadItem_ == NULL )
							startRelationWordReadItem_ = currentReadItem_;

						endRelationReadItem_ = currentReadItem_;

						switch( currentWordParameter )
							{
							case WORD_PARAMETER_NUMERAL_BOTH:				// Typically for English: "... in both ... and ..."
							case WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL:	// Typically for Dutch: "... in zowel ... als ..."
								isExclusiveSpecification_ = true;
								break;
							}

						break;

					case GRAMMAR_ASSIGNMENT_PART:
					case GRAMMAR_SPECIFICATION_ASSIGNMENT:
						isAssignment_ = true;

						if( isArchivedAssignment_ )
							isUniqueRelation_ = true;

						if( currentReadItem_->isDeterminerOrPronoun() )
							{
							if( specificationContextNr_ == NO_CONTEXT_NR )
								{
								if( ( contextResult = adminItem_->addPronounContext( currentWordTypeNr, currentReadWordItem ) ).result == RESULT_OK )
									specificationContextNr_ = contextResult.contextNr;
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a specification pronoun context" );
								}
							else
								return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The specification context number is already assigned" );
							}

						// Don't insert a break statement here

					case GRAMMAR_SPECIFICATION_PART:
					case GRAMMAR_SPECIFICATION_WORD:
					case GRAMMAR_TEXT:
						if( currentReadItem_->isSpecificationWord() )
							{
							if( isSameWordTypeAsPreviousWord &&
							previousSpecificationWordOrderNr + 1 == currentWordOrderNr )
								{
								if( addMultipleWord( previousReadItem, currentReadItem_ ) == RESULT_OK )
									currentReadItem_ = previousReadItem;	// Current read item is deleted
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a multiple specification word" );
								}
							else
								commonVariables_->nUserSpecificationWords++;

							if( currentReadWordItem != NULL )
								{
								currentReadWordItem->isUserSpecificationWord = true;

								if( currentReadWordItem->isFeminineOrMasculineWord() )
									{
									if( currentReadWordItem->isFeminineWord() )
										hasFoundFeminineSpecificationWord_ = true;

									if( currentReadWordItem->isMasculineWord() )
										hasFoundMasculineSpecificationWord_ = true;
									}
								else
									hasFoundNeutralSpecificationWord_ = true;
								}

							previousSpecificationWordOrderNr = currentWordOrderNr;
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
						commonVariables_->isUserQuestion = true;
						questionParameter_ = currentWordParameter;
						break;

					case GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB:
						commonVariables_->isUserQuestion = true;
						questionParameter_ = currentWordParameter;

						// Don't insert a break statement here

					case GRAMMAR_SPECIFICATION_GENERALIZATION_VERB:
						isSpecificationGeneralization_ = true;

						if( startSpecificationWordReadItem_ == NULL )
							startSpecificationWordReadItem_ = currentReadItem_;

						endSpecificationWordReadItem_ = currentReadItem_;

						break;

					default:
						if( previousReadItem != NULL )
							{
							isStop = true;
							currentReadItem_ = previousReadItem;
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found a word that doesn't belong to an assignment or a specification" );
					}

				previousWordTypeNr = currentWordTypeNr;
				previousReadItem = currentReadItem_;
				}
			while( !isStop &&
			( currentReadItem_ = currentReadItem_->nextReadItem() ) != NULL );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminReadSentence( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundFeminineSpecificationWord_ = false;
		hasFoundMasculineSpecificationWord_ = false;
		hasFoundNeutralSpecificationWord_ = false;
		isAssignment_ = false;
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
		isUniqueRelation_ = false;

		assumptionLevel_ = NO_ASSUMPTION_LEVEL;
		currentParseWordOrderNr_ = NO_ORDER_NR;
		prepositionParameter_ = NO_PREPOSITION_PARAMETER;
		questionParameter_ = NO_QUESTION_PARAMETER;

		generalizationContextNr_ = NO_CONTEXT_NR;
		specificationContextNr_ = NO_CONTEXT_NR;

		currentReadItem_ = NULL;
		startGeneralizationWordReadItem_ = NULL;
		endGeneralizationWordReadItem_ = NULL;
		startSpecificationWordReadItem_ = NULL;
		endSpecificationWordReadItem_ = NULL;
		startRelationWordReadItem_ = NULL;
		endRelationReadItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminReadSentence" );

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

	void dontDeletedRollbackInfo()
		{
		isDeletingRollbackInfo_ = false;
		}

	bool isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	bool isUserSentencePossessive()
		{
		return isPossessive_;
		}

	ResultType processReadSentence( char *readSentenceString )
		{
		unsigned int startSentenceNr = commonVariables_->currentSentenceNr;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processReadSentence";

		isDeletingRollbackInfo_ = true;

		commonVariables_->hasFoundAnswerToQuestion = false;
		commonVariables_->hasShownWarning = false;
		commonVariables_->isFirstAnswerToQuestion = true;
		commonVariables_->isQuestionAlreadyAnswered = false;

		initializeVariablesInAllWords();

		if( processSentence( readSentenceString ) == RESULT_OK )
			{
			if( isDeletingRollbackInfo_ &&	// Skip processing after Undo or Redo
			readSentenceString != NULL &&
			!commonVariables_->hasShownWarning &&
			!adminItem_->hasRequestedRestart() &&
			startSentenceNr == commonVariables_->currentSentenceNr )
				{
				if( adminItem_->checkForStoreAndRetrieveIntegrity( ( questionParameter_ > NO_QUESTION_PARAMETER ), readSentenceString ) == RESULT_OK )
					{
					// Has passed integrity check
					if( !commonVariables_->hasShownWarning )
						{
						if( adminItem_->hasFoundAnyChangeDuringThisSentence() )
							{
							// Show self-generated conclusions of the current sentence
							if( adminItem_->writeSelfGeneratedInfo( true, false, false ) == RESULT_OK )
								{
								// Show self-generated assumptions of the current sentence
								if( adminItem_->writeSelfGeneratedInfo( false, true, false ) == RESULT_OK )
									{
									// Show self-generated questions of the current sentence
									if( adminItem_->writeSelfGeneratedInfo( false, false, true ) == RESULT_OK )
										{
										if( checkForStructuralIntegrity() == RESULT_OK )
											{
											// Not fully implemented yet
/*											if( storeChangesInFutureDataBase() != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to store the sentence in a future database" );
*/											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for structural integrity" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the self-generated questions" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the self-generated assumptions" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the self-generated conclusions" );
							}

						if( commonVariables_->isUserQuestion )
							{
							if( adminItem_->answerQuestions() != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to answer questions" );
							}
						}
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for store and retrieve integrity of sentence \"", readSentenceString, "\"" );
				}
			}
		else
			{
			if( readSentenceString == NULL )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to process an undefined sentence" );

			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to process sentence: \"", readSentenceString, "\"" );
			}

		return RESULT_OK;
		}

	WordResultType addMultipleWord( unsigned short wordParameter, unsigned short firstWordTypeNr, unsigned short secondWordTypeNr, WordItem *firstWordItem, WordItem *secondWordItem, WordItem *existingMultipleWordItem )
		{
		WordResultType wordResult;
		WordItem *createdMultipleWordItem = NULL;
		WordTypeItem *firstWordTypeItem;
		WordTypeItem *secondWordTypeItem;
		char multipleWordString[MAX_SENTENCE_STRING_LENGTH];
		char existingMultipleWordString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( firstWordItem != NULL )
			{
			if( secondWordItem != NULL )
				{
				if( ( firstWordTypeItem = firstWordItem->activeWordTypeItem( true, firstWordTypeNr ) ) != NULL )
					{
					if( ( secondWordTypeItem = secondWordItem->activeWordTypeItem( true, secondWordTypeNr ) ) != NULL )
						{
						strcpy( multipleWordString, firstWordTypeItem->itemString() );
						strcat( multipleWordString, SPACE_STRING );
						strcat( multipleWordString, secondWordTypeItem->itemString() );

						if( firstWordTypeNr != secondWordTypeNr &&
						existingMultipleWordItem == NULL &&
						( secondWordTypeItem = secondWordItem->activeWordTypeItem( true, firstWordTypeNr ) ) != NULL )
							{
							strcpy( existingMultipleWordString, firstWordTypeItem->itemString() );
							strcat( existingMultipleWordString, SPACE_STRING );
							strcat( existingMultipleWordString, secondWordTypeItem->itemString() );

							if( ( wordResult = myWordItem_->findWordTypeInAllWords( false, firstWordTypeNr, existingMultipleWordString, NULL ) ).result == RESULT_OK )
								existingMultipleWordItem = wordResult.foundWordItem;
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find an exact word type in all words" );
							}

						if( existingMultipleWordItem == NULL )
							{
							if( ( wordResult = adminItem_->addWord( false, true, firstWordTypeItem->adjectiveParameter(), firstWordTypeItem->definiteArticleParameter(), firstWordTypeItem->indefiniteArticleParameter(), wordParameter, secondWordTypeNr, strlen( multipleWordString ), multipleWordString ) ).result == RESULT_OK )
								createdMultipleWordItem = wordResult.createdWordItem;
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add the multiple word" );
							}
						else
							{
							if( existingMultipleWordItem->addWordType( true, firstWordTypeItem->isProperNamePrecededByDefiniteArticle( firstWordTypeItem->definiteArticleParameter() ), firstWordTypeItem->adjectiveParameter(), firstWordTypeItem->definiteArticleParameter(), firstWordTypeItem->indefiniteArticleParameter(), secondWordTypeNr, strlen( multipleWordString ), multipleWordString ).result == RESULT_OK )
								createdMultipleWordItem = existingMultipleWordItem;
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add the word type to multiple word \"", existingMultipleWordItem->anyWordTypeString(), "\"" );
							}

						if( commonVariables_->result == RESULT_OK &&
						createdMultipleWordItem != NULL )
							{
							// Link both words to the created multiple word
							if( firstWordItem->addMultipleWord( secondWordTypeNr, createdMultipleWordItem ) == RESULT_OK )
								{
								if( secondWordItem->addMultipleWord( secondWordTypeNr, createdMultipleWordItem ) != RESULT_OK )
									myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add the second multiple word" );
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add the first multiple word" );
							}
						}
					else
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the word type item of the second word" );
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the word type item of the first word" );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given second word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given first word item is undefined" );

		wordResult.result = commonVariables_->result;
		return wordResult;
		}
	};

/*************************************************************************
 *
 *	"You have turned my mourning into joyful dancing.
 *	You have taken away my clothes of mourning and
 *	clothed me with joy,
 *	that I might sing praises to you and not be silent.
 *	O Lord my God, I will give you thanks forever!" (Psalm 30:11-12)
 *
 *************************************************************************/
