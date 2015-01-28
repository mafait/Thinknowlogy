/*
 *	Class:			AdminContext
 *	Supports class:	AdminItem
 *	Purpose:		To create context structures
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
#include "Presentation.cpp"
#include "ReadItem.cpp"
#include "SpecificationItem.cpp"

class AdminContext
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundUserSpecification_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned int nContextRelations, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		ContextResultType contextResult;
		bool hasFoundAllRelationWords;
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
										currentSpecificationItem->isPossessive() == isPossessive &&
										currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, specificationWordItem ) )
											{
											currentRelationContextNr = currentSpecificationItem->relationContextNr();
											nContextWords = myWordItem_->nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );
											// This relation word will be the last one
											hasFoundAllRelationWords = ( nContextWords + 1 == nContextRelations );

											if( currentSpecificationItem->isPossessiveReversibleConclusion() )
												{
												if( ( isPossessive &&
												currentRelationContextNr != relationContextNr ) ||

												( !isPossessive &&
												!hasFoundAllRelationWords &&
												currentRelationContextNr == relationContextNr ) )
													{
													if( hasFoundAllRelationWords )
														{
														contextResult.contextNr = relationContextNr;
														contextResult.replaceContextNr = currentRelationContextNr;
														}
													else
														contextResult.contextNr = currentRelationContextNr;
													}
												}
											else
												{
												if( !isPossessive &&
												nContextWords == nContextRelations &&
												currentRelationContextNr != relationContextNr )
													{
													contextResult.contextNr = currentRelationContextNr;
													contextResult.replaceContextNr = relationContextNr;
													}
												}
											}
										}
									while( contextResult.contextNr == NO_CONTEXT_NR &&
									( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
									}
								}
							else
								myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
							}
						while( commonVariables_->result == RESULT_OK &&
						( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
						}
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}

	ContextItem *firstContextItemInAllWords( bool isUserGeneralizationWord, WordItem *specificationWordItem )
		{
		ContextItem *foundContextItem;
		WordItem *currentWordItem;

		if( specificationWordItem != NULL &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( ( isUserGeneralizationWord &&
				currentWordItem->isUserGeneralizationWord ) ||

				( !isUserGeneralizationWord &&
				currentWordItem->isUserRelationWord ) ) &&

				( foundContextItem = currentWordItem->contextItemInWord( specificationWordItem ) ) != NULL )
					return foundContextItem;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	AdminContext( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundUserSpecification_ = false;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminContext" );

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


	// Protected context functions

	void initializeAdminContextVariables()
		{
		hasFoundUserSpecification_ = false;
		}

	unsigned int highestContextNr()
		{
		unsigned int tempContextNr;
		unsigned int highestContextNr = NO_CONTEXT_NR;
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( ( tempContextNr = currentWordItem->highestContextNrInWord() ) > highestContextNr )
					highestContextNr = tempContextNr;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return highestContextNr;
		}

	ContextResultType addPronounContext( unsigned short contextWordTypeNr, WordItem *contextWordItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addPronounContext";

		if( contextWordItem != NULL )
			{
			if( ( contextResult.contextNr = contextWordItem->contextNrInWord( NULL ) ) == NO_CONTEXT_NR )
				{
				if( ( contextResult.contextNr = highestContextNr() ) < MAX_CONTEXT_NR )
					// Create new context number
					contextResult.contextNr++;
				else
					myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Context number overflow" );
				}

			if( contextWordItem->addContext( false, contextWordTypeNr, WORD_TYPE_UNDEFINED, contextResult.contextNr, NULL ) != RESULT_OK )
				myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a pronoun context to word \"", contextWordItem->anyWordTypeString(), "\"" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read word of the read ahead item is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}

	ContextResultType getRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		bool isSpecificationCollectedWithItself;
		unsigned int foundRelationContextNr;
		ContextItem *foundContextItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *selfGeneratedSpecificationItem = NULL;
		WordItem *foundRelationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					isSpecificationCollectedWithItself = specificationWordItem->isCollectedWithItself();

					// Try to find relation context with same number of relation words as in the user sentence
					if( ( contextResult.contextNr = relationWordItem->contextNrInWord( commonVariables_->nUserRelationWords, specificationWordItem ) ) == NO_CONTEXT_NR )
						{
						existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

						if( ( foundRelationContextNr = relationWordItem->contextNrInWord( specificationWordItem ) ) > NO_CONTEXT_NR )
							{
							if( !isSelfGeneratedAssumption ||
							existingSpecificationItem == NULL ||
							!existingSpecificationItem->hasRelationContext() )
								contextResult.contextNr = foundRelationContextNr;
							else
								{
								if( generalizationWordItem->hasConfirmedSpecification() )
									{
									selfGeneratedSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isNegative, isPossessive, !isSelfGeneratedAssumption, specificationWordItem );

									contextResult.contextNr = ( selfGeneratedSpecificationItem == NULL ||
																selfGeneratedSpecificationItem->relationContextNr() == foundRelationContextNr ? existingSpecificationItem->relationContextNr() : foundRelationContextNr );
									}
								}
							}
						}

					if( contextResult.contextNr == NO_CONTEXT_NR )
						{
						foundRelationContextNr = NO_CONTEXT_NR;

						if( ( foundContextItem = firstContextItemInAllWords( generalizationWordItem->isUserGeneralizationWord, specificationWordItem ) ) != NULL )
							{
							// Find specification with found context word as relation word
							foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );

							if( foundSpecificationItem != NULL &&
							!foundSpecificationItem->hasRelationContext() )
								foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

							if( foundSpecificationItem != NULL )
								// No such specification found. Get the relation context of a similar specification
								foundRelationContextNr = foundSpecificationItem->relationContextNr();
							else
								// Take the found relation context
								foundRelationContextNr = foundContextItem->contextNr();
							}

						if( foundRelationContextNr > NO_CONTEXT_NR )
							{
							// If cross-collected afterwards
							if( existingSpecificationItem != NULL )
								{
								foundRelationWordItem = existingSpecificationItem->relationWordItem();

								if( !hasFoundUserSpecification_ &&
								!isSpecificationCollectedWithItself &&
								generalizationWordItem->isUserGeneralizationWord &&

								( foundRelationWordItem == NULL ||
								foundRelationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) != NULL ) )
									hasFoundUserSpecification_ = true;

								if( hasFoundUserSpecification_ ||
								generalizationWordItem->hasConfirmedSpecification() )
									{
									if( existingSpecificationItem->isSelfGeneratedAssumption() )
										contextResult.contextNr = existingSpecificationItem->relationContextNr();
									}
								else
									{
									// Skip exclusive generalization
									if( !isAssignment &&

									( isSpecificationCollectedWithItself ||
									// Skip on difference in assumption / conclusion
									existingSpecificationItem->isSelfGeneratedAssumption() == isSelfGeneratedAssumption ) )
										contextResult.contextNr = foundRelationContextNr;
									}
								}
							}
						}
					else
						{
						if( isAssignment &&
						// Has no relation context collection
						myWordItem_->collectionNrInAllWords( relationContextNr ) == NO_COLLECTION_NR &&
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
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
							}
						}

					if( commonVariables_->result == RESULT_OK &&
					contextResult.contextNr == NO_CONTEXT_NR )
						{
						if( ( contextResult.contextNr = highestContextNr() ) < MAX_CONTEXT_NR )
							// Create new context number
							contextResult.contextNr++;
						else
							myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Context number overflow" );
						}
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}

	ContextResultType getRelationContext( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		ContextResultType contextResult;
		bool hasFoundMatchingSpecificationCollection;
		bool hasFoundRelationContext;
		bool hasFoundRelationWordInThisList;
		bool hasSameNumberOrMoreRelationWords;
		bool isSkippingThisContext = false;
		unsigned int currentRelationContextNr;
		unsigned int foundRelationContextNr;
		ContextItem *currentRelationContextItem;
		ReadItem *relationWordReadItem = NULL;
		SpecificationItem *foundSpecificationItem;
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
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read word of the first relation word is undefined" );
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( currentRelationWordItem != NULL )
						{
						if( ( currentRelationContextItem = currentRelationWordItem->firstActiveContextItem() ) != NULL )
							{
							do	{	// Do for all relation context items in the first relation context word
								currentRelationContextNr = currentRelationContextItem->contextNr();

								if( currentRelationWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem ) )
									{
									if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
										{
										hasFoundRelationWordInThisList = false;
										isSkippingThisContext = false;

										// The context that was found, is may be found by specification collection instead of specification word.
										// So, do the same check with context without specification collection.
										hasFoundMatchingSpecificationCollection = !currentRelationWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem );

										// Do for all words, either in the current relation list or outside this list
										do	{
											foundSpecificationItem = ( isUserSentence ? NULL : generalizationWordItem->firstActiveAssignmentOrSpecificationItem( true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) );

											if( foundSpecificationItem == NULL ||
											!foundSpecificationItem->isSelfGeneratedConclusion() ||
											foundSpecificationItem->relationContextNr() != currentRelationContextNr )
												{
												if( relationWordReadItem != NULL )
													hasFoundRelationWordInThisList = relationWordReadItem->hasFoundRelationWordInThisList( currentWordItem );

												hasFoundRelationContext = currentWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem );

												// Word is one of the relation words in this list, but doesn't have current context
												if( ( !hasFoundRelationContext &&
												hasFoundRelationWordInThisList &&
												!hasFoundMatchingSpecificationCollection ) ||

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
											contextResult.contextNr = currentRelationContextNr;
										}
									else
										myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );
									}
								}
							while( commonVariables_->result == RESULT_OK &&
							contextResult.contextNr == NO_CONTEXT_NR &&
							( currentRelationContextItem = currentRelationContextItem->nextContextItem() ) != NULL );
							}

						if( commonVariables_->result == RESULT_OK &&
						!isQuestion &&
						contextResult.contextNr == NO_CONTEXT_NR &&
						( foundSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isNegative, isPossessive, isPossessive, specificationWordItem ) ) != NULL )
							{
							if( ( foundRelationContextNr = foundSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR )
								{
								hasSameNumberOrMoreRelationWords = ( myWordItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) >= commonVariables_->nUserRelationWords );

								if( hasSameNumberOrMoreRelationWords ||
								generalizationWordItem->isUserRelationWord )
									{
									contextResult.contextNr = foundRelationContextNr;

									if( foundSpecificationItem->isExclusiveSpecification() )
										// Static (exclusive) semantic ambiguity
										contextResult.isExclusiveContext = true;
									else
										{
										if( !foundSpecificationItem->isAssignment() )
											{
											if( relationWordItem == NULL )
												{
												if( isAssignment ||
												isSkippingThisContext )
													// Different number of relation words
													// Create new context number
													contextResult.contextNr = NO_CONTEXT_NR;
												else
													{
													// Static (exclusive) semantic ambiguity
													if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) == RESULT_OK )
														contextResult.isExclusiveContext = true;
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
													}
												}
											else
												{
												if( hasSameNumberOrMoreRelationWords ||
												specificationWordItem->isCollectedWithItself() )
													{
													// Try to find the relation context of a possessive reversible conclusion
													if( ( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isExclusiveSpecification, isNegative, isPossessive, nContextRelations, contextResult.contextNr, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
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
							if( ( contextResult.contextNr = highestContextNr() ) < MAX_CONTEXT_NR )
								// Create new context number
								contextResult.contextNr++;
							else
								myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Context number overflow" );
							}
						}
					else
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any relation word" );
					}
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = commonVariables_->result;
		return contextResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	How joyful are those who fear the Lord
 *	and delight in obeying his commands." (Psalm 112:1)
 *************************************************************************/
