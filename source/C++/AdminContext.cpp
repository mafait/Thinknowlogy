/*	Class:			AdminContext
 *	Supports class:	AdminItem
 *	Purpose:		To create context structures
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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
#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"

class AdminContext
	{
	friend class AdminItem;

	// Private constructible variables

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isValidContextInAllWords( bool isPossessive, unsigned int existingRelationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *currentWordItem;

		if( existingRelationContextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
			{
			// Do for all active words
			do	{
				if( currentWordItem->isProperName() &&
				currentWordItem != relationWordItem &&
				!currentWordItem->isUserRelationWord &&
				currentWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, NO_QUESTION_PARAMETER, existingRelationContextNr, specificationWordItem ) != NULL )
					return false;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return true;
		}

	unsigned int collectionNrInAllWords( unsigned int contextNr )
		{
		ContextItem *foundContextItem;
		WordItem *currentWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
			{
			// Do for all active words
			do	{
				if( ( foundContextItem = currentWordItem->contextItem( contextNr ) ) != NULL )
					return currentWordItem->collectionNr( foundContextItem->contextWordTypeNr(), foundContextItem->specificationWordItem() );
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( bool isPossessive, unsigned int nContextRelations, unsigned int relationContextNr, SpecificationItem *foundSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		ContextResultType contextResult;
		bool hasFoundAllRelationWords;
		bool isFoundRelationContext;
		unsigned int currentRelationContextNr;
		unsigned int nContextWords;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossessiveReversibleConclusionRelationContextOfInvolvedWords";

		if( relationContextNr > NO_CONTEXT_NR )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( ( currentGeneralizationItem = generalizationWordItem->firstRelationGeneralizationItem() ) != NULL )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( ( currentSpecificationItem = currentGeneralizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
									{
									do	{
										if( currentSpecificationItem->hasRelationContext() &&
										currentSpecificationItem->isRelatedSpecification( false, false, isPossessive, specificationWordItem ) )
											{
											currentRelationContextNr = currentSpecificationItem->relationContextNr();
											nContextWords = adminItem_->nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );

											// This relation word will be the last one
											hasFoundAllRelationWords = ( nContextWords + 1 == nContextRelations );
											isFoundRelationContext = ( currentRelationContextNr == relationContextNr );

											if( currentSpecificationItem->isPossessiveReversibleConclusion() )
												{
												if( ( isPossessive &&
												!isFoundRelationContext ) ||

												( !isPossessive &&
												isFoundRelationContext &&
												!hasFoundAllRelationWords ) )
													{
													if( hasFoundAllRelationWords )
														{
														contextResult.contextNr = relationContextNr;
														contextResult.conclusionSpecificationItem = currentSpecificationItem;
														}
													else
														contextResult.contextNr = currentRelationContextNr;
													}
												}
											else
												{
												if( !isPossessive &&
												!isFoundRelationContext &&
												nContextWords == nContextRelations )
													{
													contextResult.contextNr = currentRelationContextNr;
													contextResult.conclusionSpecificationItem = foundSpecificationItem;
													}
												}
											}
										}
									while( contextResult.contextNr == NO_CONTEXT_NR &&
									( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
									}
								}
							else
								adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
							}
						while( commonVariables_->result == RESULT_OK &&
						( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
						}
					}
				else
					adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}

	ResultType copyContext( unsigned int existingContextNr, unsigned int newContextNr )
		{
		ContextItem *currentContextItem;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyContext";

		if( ( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
			{
			// Do for all active words
			do	{
				if( ( currentContextItem = currentWordItem->contextItem( existingContextNr ) ) != NULL )
					{
					if( currentWordItem->addContext( currentContextItem->isCompoundCollectionSpanishAmbiguous(), currentContextItem->contextWordTypeNr(), currentContextItem->specificationWordTypeNr(), newContextNr, currentContextItem->specificationWordItem() ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a copied context to word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ContextItem *firstContextItemInAllWords( bool isArchivedAssignment, bool isNegative, bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isCheckingOlderContextWords;
		ContextItem *currentContextItem;
		ContextItem *foundContextItem = NULL;
		SpecificationItem *userSpecificationItem = adminItem_->userSpecificationItem();
		WordItem *currentWordItem;

		if( specificationWordItem != NULL &&
		( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
			{
			isCheckingOlderContextWords = ( isPossessive &&

											( userSpecificationItem == NULL ||
											!userSpecificationItem->isPossessive() ) );

			// Do for all active words
			do	{
				if( currentWordItem != generalizationWordItem &&
				( currentContextItem = currentWordItem->contextItem( specificationWordItem ) ) != NULL )
					{
					if( isCheckingOlderContextWords &&
					currentContextItem->isOlderItem() )
						{
						if( foundContextItem == NULL )
							foundContextItem = currentContextItem;
						}
					else
						return currentContextItem;
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

			if( foundContextItem != NULL &&
			( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
				{
				// Do for all active words
				do	{
					if( !currentWordItem->isUserGeneralizationWord &&
					!currentWordItem->isUserRelationWord &&
					currentWordItem->isProperName() &&
					currentWordItem->bestMatchingRelationContextNrSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, foundContextItem->contextNr(), specificationWordItem ) != NULL )
						// Found context is already in use by other words
						return NULL;
					}
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
				}
			}

		return foundContextItem;
		}


	protected:
	// Constructor / deconstructor

	AdminContext( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminContext" );

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


	// Protected context functions

	ContextResultType getRelationContext( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned short specificationWordTypeNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		ContextResultType contextResult;
		bool hasFoundRelationContext;
		bool hasFoundRelationWordInThisList;
		bool hasSameNumberOrMoreRelationWords;
		bool isSkippingThisContext = false;
		unsigned int currentRelationContextNr;
		unsigned int foundRelationContextNr;
		ContextItem *currentRelationContextItem;
		ReadItem *relationWordReadItem = NULL;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		WordItem *currentWordItem;
		WordItem *currentRelationWordItem = relationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( startRelationReadItem != NULL )
					{
					if( ( relationWordReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL )
						currentRelationWordItem = relationWordReadItem->readWordItem();
					else
						adminItem_->startError( functionNameString, moduleNameString_, "The read word of the first relation word is undefined" );
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( currentRelationWordItem != NULL )
						{
						if( ( currentRelationContextItem = currentRelationWordItem->firstActiveContextItem() ) != NULL )
							{
							// Do for all relation context items in the first relation context word
							do	{
								currentRelationContextNr = currentRelationContextItem->contextNr();

								if( currentRelationWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem ) )
									{
									if( ( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
										{
										hasFoundRelationWordInThisList = false;
										isSkippingThisContext = false;

										// Do for all active words, either in the current relation list or outside this list
										do	{
											foundSpecificationItem = ( isUserSentence ? NULL : generalizationWordItem->firstActiveAssignmentOrSpecificationItem( true, false, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) );

											if( foundSpecificationItem == NULL ||
											!foundSpecificationItem->isSelfGeneratedConclusion() )
												{
												if( relationWordReadItem != NULL )
													hasFoundRelationWordInThisList = relationWordReadItem->hasFoundRelationWordInThisList( currentWordItem );

												hasFoundRelationContext = currentWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem );

												// Word is one of the relation words in this list, but doesn't have current context
												if( ( !hasFoundRelationContext &&
												hasFoundRelationWordInThisList ) ||

												// Word is in not current list of relation words, but has current context
												( hasFoundRelationContext &&
												!hasFoundRelationWordInThisList ) )
													isSkippingThisContext = true;
												}
											}
										while( !isSkippingThisContext &&
										( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

										// The relation words in the list contain this context exclusively. (So, no other words)
										if( !isSkippingThisContext )
											{
											existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );

											if( existingSpecificationItem != NULL &&
											existingSpecificationItem->hasRelationContext() &&
											existingSpecificationItem->isUserSpecification() &&
											existingSpecificationItem->relationContextNr() != currentRelationContextNr &&

											// Skip mix-up singular/plural noun
											( specificationWordTypeNr != WORD_TYPE_NOUN_SINGULAR ||
											!existingSpecificationItem->isSpecificationPluralNoun() ) )
												contextResult.contextNr = existingSpecificationItem->relationContextNr();
											else
												contextResult.contextNr = currentRelationContextNr;
											}
										}
									else
										adminItem_->startError( functionNameString, moduleNameString_, "The last predefined word item is undefined" );
									}
								else
									{
									if( isUserSentence &&
									!currentRelationContextItem->isOlderItem() )
										contextResult.contextNr = currentRelationContextNr;
									}
								}
							while( commonVariables_->result == RESULT_OK &&
							contextResult.contextNr == NO_CONTEXT_NR &&
							( currentRelationContextItem = currentRelationContextItem->nextContextItem() ) != NULL );
							}

						if( commonVariables_->result == RESULT_OK &&
						!isQuestion &&
						contextResult.contextNr == NO_CONTEXT_NR &&
						( foundSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, NULL ) ) != NULL )
							{
							if( ( foundRelationContextNr = foundSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR )
								{
								hasSameNumberOrMoreRelationWords = ( adminItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) >= commonVariables_->nUserRelationWords );

								if( hasSameNumberOrMoreRelationWords ||
								generalizationWordItem->isUserRelationWord )
									{
									contextResult.contextNr = foundRelationContextNr;

									if( foundSpecificationItem->isExclusiveSpecification() )
										// Already existing static (exclusive) semantic ambiguity
										contextResult.isExclusiveContext = true;
									else
										{
										if( !foundSpecificationItem->isActiveAssignment() )
											{
											if( relationWordItem == NULL )
												{
												if( isSkippingThisContext )
													// Didn't confirm all relation words
													// Create new context number
													contextResult.contextNr = NO_CONTEXT_NR;
												else
													{
													// Static (exclusive) semantic ambiguity
													if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) == RESULT_OK )
														contextResult.isExclusiveContext = true;
													else
														adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );
													}
												}
											else
												{
												if( hasSameNumberOrMoreRelationWords )
													{
													// Try to find the relation context of a possessive reversible conclusion
													if( ( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isPossessive, nContextRelations, contextResult.contextNr, foundSpecificationItem, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
														adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
													}
												}
											}
										}
									}
								}
							}

						if( commonVariables_->result == RESULT_OK &&
						contextResult.contextNr == NO_CONTEXT_NR )
							{
							if( ( contextResult.contextNr = adminItem_->highestContextNrInAllWords() ) < MAX_CONTEXT_NR )
								// Create new context number
								contextResult.contextNr++;
							else
								adminItem_->startSystemError( functionNameString, moduleNameString_, "Context number overflow" );
							}
						}
					else
						adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any relation word" );
					}
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}

	ContextResultType getSpecificationRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCompoundCollectionSpanishAmbiguous, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int existingRelationContextNr;
		unsigned int existingCopyContextNr = NO_CONTEXT_NR;
		unsigned int foundRelationContextNr = NO_CONTEXT_NR;
		unsigned int nUserRelationWords = commonVariables_->nUserRelationWords;
		ContextItem *foundContextItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getSpecificationRelationContext";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					// Try to find relation context with same number of relation words as in the user sentence
					if( ( foundContextItem = relationWordItem->contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, specificationWordItem ) ) == NULL )
						{
						if( isCompoundCollectionSpanishAmbiguous )
							existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, specificationWordItem );
						else
							existingSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

						isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

						if( existingSpecificationItem == NULL &&
						contextResult.contextNr == NO_CONTEXT_NR &&
						nUserRelationWords > 1 &&

						( !isSpecificationWordSpanishAmbiguous ||
						commonVariables_->nUserGeneralizationWords > 1 ||
						generalizationWordItem->isFemale() ||
						!relationWordItem->isOlderItem() ) )
							contextResult.contextNr = relationWordItem->contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem );

						// Not found yet
						if( contextResult.contextNr == NO_CONTEXT_NR &&
						( foundContextItem = firstContextItemInAllWords( isArchivedAssignment, isNegative, isPossessive, generalizationWordItem, specificationWordItem ) ) != NULL )
							{
							existingRelationContextNr = ( existingSpecificationItem == NULL ? NO_CONTEXT_NR : existingSpecificationItem->relationContextNr() );

							// Find specification with found context word as relation word
							if( ( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
								{
								if( isSpecificationWordSpanishAmbiguous &&
								generalizationWordItem->isUserRelationWord &&
								foundContextItem->myWordItem() == relationWordItem )
									contextResult.contextNr = foundContextItem->contextNr();
								}
							else
								{
								if( existingSpecificationItem != NULL &&

								( !foundSpecificationItem->hasRelationContext() ||

								( isSpecificationWordSpanishAmbiguous &&
								existingSpecificationItem->isHiddenSpanishSpecification() ) ) )
									foundRelationContextNr = existingRelationContextNr;
								else
									{
									if( ( !isSpecificationWordSpanishAmbiguous ||
									specificationCollectionNr == NO_COLLECTION_NR ||
									foundSpecificationItem->isUserSpecification() ||
									foundSpecificationItem->specificationCollectionNr() == specificationCollectionNr ) &&

									foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment )
										foundRelationContextNr = foundSpecificationItem->relationContextNr();
									}
								}

							// Cross-collected afterwards
							if( foundRelationContextNr > NO_CONTEXT_NR &&
							contextResult.contextNr == NO_CONTEXT_NR &&
							existingSpecificationItem != NULL )
								{
								if( generalizationWordItem->hasConfirmedSpecification() )
									{
									if( existingSpecificationItem->isSelfGeneratedAssumption() )
										// Found reversed
										contextResult.contextNr = existingRelationContextNr;
									}
								else
									{
									if( generalizationWordItem->isUserRelationWord &&
									existingSpecificationItem->isOlderItem() &&
									!relationWordItem->hasContextInWord( existingRelationContextNr, specificationWordItem ) &&
									!isValidContextInAllWords( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) &&

									( existingSpecificationItem->isConcludedAssumption() ||
									adminItem_->nContextWordsInAllWords( existingRelationContextNr, specificationWordItem ) > commonVariables_->nUserGeneralizationWords ) )
										existingCopyContextNr = existingRelationContextNr;
									else
										{
										if( isSpecificationWordSpanishAmbiguous ||
										existingSpecificationItem->isConcludedAssumption() )
											contextResult.contextNr = foundRelationContextNr;
										else
											{
											// Skip on difference in assumption / conclusion
											if( existingSpecificationItem->isSelfGeneratedAssumption() == isSelfGeneratedAssumption )
												contextResult.contextNr = existingRelationContextNr;
											}
										}
									}
								}
							}
						}
					else
						{
						contextResult.contextNr = foundContextItem->contextNr();

						if( isAssignment &&
						// Has no relation context collection
						collectionNrInAllWords( relationContextNr ) == NO_COLLECTION_NR &&
						// Check if assignment already exists
						generalizationWordItem->firstNonQuestionAssignmentItem( ( !isInactiveAssignment && !isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, contextResult.contextNr, specificationWordItem ) == NULL &&
						// Check also recently replaced assignments
						generalizationWordItem->firstRecentlyReplacedAssignmentItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, contextResult.contextNr, specificationWordItem ) == NULL )
							{
							// Dynamic semantic ambiguity
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) == RESULT_OK )
								{
								contextResult.isAmbiguousRelationContext = true;
								// Create new context number
								contextResult.contextNr = NO_CONTEXT_NR;
								}
							else
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );
							}
						}

					if( commonVariables_->result == RESULT_OK &&
					contextResult.contextNr == NO_CONTEXT_NR )
						{
						if( ( contextResult.contextNr = adminItem_->highestContextNrInAllWords() ) < MAX_CONTEXT_NR )
							{
							// Create new context number
							contextResult.contextNr++;

							if( existingCopyContextNr > NO_CONTEXT_NR )
								{
								if( copyContext( existingCopyContextNr, contextResult.contextNr ) == RESULT_OK )
									contextResult.copiedRelationContextNr = existingCopyContextNr;
								else
									adminItem_->addError( functionNameString, moduleNameString_, "I failed to copied a context" );
								}
							}
						else
							adminItem_->startSystemError( functionNameString, moduleNameString_, "Context number overflow" );
						}
					}
				else
					adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	How joyful are those who fear the Lord
 *	and delight in obeying his commands." (Psalm 112:1)
 *************************************************************************/
