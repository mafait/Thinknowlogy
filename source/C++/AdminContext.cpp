/*	Class:			AdminContext
 *	Supports class:	AdminItem
 *	Purpose:		To create context structures
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
#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"

class AdminContext
	{
	friend class AdminItem;

	// Private constructed variables

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isValidRelationContextInAllWords( bool isPossessive, unsigned int existingRelationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *currentContextWordItem;

		if( existingRelationContextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->isProperName() &&
				currentContextWordItem != relationWordItem &&
				!currentContextWordItem->isUserRelationWord &&
				currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, NO_QUESTION_PARAMETER, existingRelationContextNr, specificationWordItem ) != NULL )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return true;
		}

	ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( bool isPossessive, unsigned int nContextRelations, unsigned int relationContextNr, SpecificationItem *foundSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		ContextResultType contextResult;
		unsigned int currentRelationContextNr;
		unsigned int nContextWords;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossessiveReversibleConclusionRelationContextOfInvolvedWords";

		if( relationContextNr == NO_CONTEXT_NR )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = generalizationWordItem->firstRelationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startContextResultError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

				if( ( currentSpecificationItem = currentGeneralizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
					{
					do	{
						if( ( currentRelationContextNr = currentSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
						currentSpecificationItem->isRelatedSpecification( false, false, isPossessive, specificationWordItem ) )
							{
							if( !isPossessive ||
							currentRelationContextNr != relationContextNr )
								{
								nContextWords = adminItem_->nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );

								if( currentSpecificationItem->isPossessiveReversibleConclusion() )
									{
									// Has found all relation words
									// (This relation word will be the last one)
									if( nContextWords + 1 == nContextRelations )
										{
										contextResult.contextNr = relationContextNr;
										contextResult.conclusionSpecificationItem = currentSpecificationItem;
										}
									else
										contextResult.contextNr = currentRelationContextNr;
									}
								else
									{
									// Found equal number of relation words
									if( nContextWords == nContextRelations )
										{
										contextResult.contextNr = currentRelationContextNr;
										contextResult.conclusionSpecificationItem = foundSpecificationItem;
										}
									}
								}
							}
						}
					while( contextResult.contextNr == NO_CONTEXT_NR &&
					( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
			}

		return contextResult;
		}

	ResultType copyContext( unsigned int existingContextNr, unsigned int newContextNr )
		{
		ContextItem *currentContextItem;
		WordItem *currentContextWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyContext";

		if( ( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( ( currentContextItem = currentContextWordItem->contextItem( existingContextNr ) ) != NULL )
					{
					if( currentContextWordItem->addContext( currentContextItem->isCompoundCollectionSpanishAmbiguous(), currentContextItem->contextWordTypeNr(), currentContextItem->specificationWordTypeNr(), newContextNr, currentContextItem->specificationWordItem() ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a copied context to word \"", currentContextWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

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

	ContextResultType getRelationContext( bool isArchivedAssignment, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
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
		WordItem *currentContextWordItem;
		WordItem *currentRelationWordItem = relationWordItem;
		WordItem *firstContextWordItem = commonVariables_->firstContextWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem != NULL )
			{
			if( ( relationWordReadItem = startRelationReadItem->firstRelationWordReadItem() ) == NULL )
				return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The read word of the first relation word is undefined" );

			currentRelationWordItem = relationWordReadItem->readWordItem();
			}

		if( currentRelationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "I couldn't find any relation word" );

		if( ( currentRelationContextItem = currentRelationWordItem->firstActiveContextItem() ) != NULL )
			{
			// Do for all relation context items in the first relation context word
			do	{
				currentRelationContextNr = currentRelationContextItem->contextNr();

				if( currentRelationWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem ) )
					{
					hasFoundRelationWordInThisList = false;
					isSkippingThisContext = false;

					if( firstContextWordItem != NULL )
						{
						currentContextWordItem = firstContextWordItem;

						// Do for all context words, either in the current relation list or outside this list
						do	{
							foundSpecificationItem = ( isUserSentence ? NULL : generalizationWordItem->firstActiveAssignmentOrSpecificationItem( true, false, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) );

							if( foundSpecificationItem == NULL ||
							!foundSpecificationItem->isSelfGeneratedConclusion() )
								{
								if( relationWordReadItem != NULL )
									hasFoundRelationWordInThisList = relationWordReadItem->hasFoundRelationWordInThisList( currentContextWordItem );

								hasFoundRelationContext = currentContextWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem );

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
						( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
						}

					// The relation words in the list contain this context exclusively. (So, no other words)
					if( !isSkippingThisContext )
						contextResult.contextNr = currentRelationContextNr;
					}
				else
					{
					if( isUserSentence &&
					!currentRelationContextItem->isOlderItem() )
						contextResult.contextNr = currentRelationContextNr;
					}
				}
			while( contextResult.contextNr == NO_CONTEXT_NR &&
			( currentRelationContextItem = currentRelationContextItem->nextContextItem() ) != NULL );
			}

		if( !isQuestion &&
		contextResult.contextNr == NO_CONTEXT_NR &&
		( foundSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, NULL ) ) != NULL )
			{
			if( ( foundRelationContextNr = foundSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR )
				{
				hasSameNumberOrMoreRelationWords = ( adminItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) >= commonVariables_->nUserRelationWords );

				if( hasSameNumberOrMoreRelationWords ||
				generalizationWordItem->isUserRelationWord )
					{
					if( foundSpecificationItem->isExclusiveSpecification() )
						{
						// Already existing static (exclusive) semantic ambiguity
						contextResult.isAmbiguousRelationContext = true;
						contextResult.contextNr = foundRelationContextNr;
						}
					else
						{
						if( isUserSentence )
							{
							if( !isSkippingThisContext )
								{
								contextResult.contextNr = foundRelationContextNr;

								if( !foundSpecificationItem->isActiveAssignment() )
									{
									// Static (exclusive) semantic ambiguity
									if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
										return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

									contextResult.isAmbiguousRelationContext = true;
									}
								}
							}
						else
							{
							contextResult.contextNr = foundRelationContextNr;

							if( hasSameNumberOrMoreRelationWords )
								{
								// Try to find the relation context of a possessive reversible conclusion
								if( ( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isPossessive, nContextRelations, contextResult.contextNr, foundSpecificationItem, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
									return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
								}
							}
						}
					}
				}
			}

		if( contextResult.contextNr == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		return contextResult;
		}

	ContextResultType getSpecificationRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCompoundCollectionSpanishAmbiguous, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int specificationCollectionNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		bool hasRelationWordExistingRelationContext;
		bool isExistingSpecificationConcludedAssumption;
		bool isGeneralizationWordUserRelationWord;
		bool isPossessiveUserSpecification;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int tempContextNr;
		unsigned int existingCopyContextNr = NO_CONTEXT_NR;
		unsigned int existingRelationContextNr = NO_CONTEXT_NR;
		unsigned int foundRelationContextNr = NO_CONTEXT_NR;
		unsigned int nUserRelationWords = commonVariables_->nUserRelationWords;
		ContextItem *foundContextItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getSpecificationRelationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( isCompoundCollectionSpanishAmbiguous )
			existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, false, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, specificationWordItem );
		else
			existingSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, isArchivedAssignment, false, isPossessive, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

		// Try to find relation context with same number of relation words as in the user sentence
		if( ( foundContextItem = relationWordItem->contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, specificationWordItem ) ) == NULL )
			{
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( existingSpecificationItem == NULL ||

			( isGeneralizationWordUserRelationWord &&
			!isSpecificationWordSpanishAmbiguous &&
			!existingSpecificationItem->hasRelationContext() &&
			!relationWordItem->hasConfirmedSpecification() ) )
				{
				if( nUserRelationWords > 1 &&

				( commonVariables_->nUserGeneralizationWords > 1 ||
				!relationWordItem->isOlderItem() ||
				// Typically for the Spanish language
				generalizationWordItem->isFemale() ) &&

				( tempContextNr = relationWordItem->contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem ) ) > NO_CONTEXT_NR )
					contextResult.contextNr = tempContextNr;
				}
			else
				{
				isExistingSpecificationConcludedAssumption = existingSpecificationItem->isConcludedAssumption();
				existingRelationContextNr = existingSpecificationItem->relationContextNr();

				// Not found yet
				if( contextResult.contextNr == NO_CONTEXT_NR )
					{
					// Find specification with found context word as relation word
					if( isSpecificationWordSpanishAmbiguous )
						{
						if( ( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
							{
							if( existingRelationContextNr > NO_CONTEXT_NR &&

							( !foundSpecificationItem->hasRelationContext() ||
							existingSpecificationItem->isHiddenSpanishSpecification() ) )
								foundRelationContextNr = existingRelationContextNr;
							else
								{
								if( specificationCollectionNr == NO_COLLECTION_NR ||
								foundSpecificationItem->isUserSpecification() ||
								foundSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
									foundRelationContextNr = foundSpecificationItem->relationContextNr();
								}
							}
						}
					else
						{
						if( existingRelationContextNr > NO_CONTEXT_NR &&

						( !isExistingSpecificationConcludedAssumption ||
						isGeneralizationWordUserRelationWord ) &&

						existingSpecificationItem->isArchivedAssignment() == isArchivedAssignment )
							foundRelationContextNr = existingRelationContextNr;
						}

					// Cross-collected afterwards
					if( foundRelationContextNr > NO_CONTEXT_NR &&
					contextResult.contextNr == NO_CONTEXT_NR )
						{
						userSpecificationItem = adminItem_->userSpecificationItem();

						if( generalizationWordItem->hasConfirmedSpecification() )
							{
							if( !isPossessive ||
							specificationCollectionNr == NO_COLLECTION_NR ||
							!existingSpecificationItem->isOlderItem() ||

							( isSelfGeneratedAssumption &&
							existingSpecificationItem->isSelfGeneratedConclusion() ) ||

							( ( !isSpecificationWordSpanishAmbiguous ||
							// Typically for Spanish
							userSpecificationItem->isSpecificationWordSpanishAmbiguous() ) &&

							( userSpecificationItem != NULL &&
							userSpecificationItem->hasSpecificationCompoundCollection() ) ) )
								contextResult.contextNr = existingRelationContextNr;
							}
						else
							{
							hasRelationWordExistingRelationContext = relationWordItem->hasContextInWord( existingRelationContextNr, specificationWordItem );
							isPossessiveUserSpecification = ( userSpecificationItem != NULL &&
															userSpecificationItem->isPossessive() );

							// Check for copy context
							if( !hasRelationWordExistingRelationContext &&
							isGeneralizationWordUserRelationWord &&
							existingSpecificationItem->isOlderItem() &&

							( ( isExistingSpecificationConcludedAssumption &&
							isPossessiveUserSpecification ) ||

							( !isExistingSpecificationConcludedAssumption &&
							!isValidRelationContextInAllWords( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) &&
							adminItem_->nContextWordsInAllWords( existingRelationContextNr, specificationWordItem ) > commonVariables_->nUserGeneralizationWords ) ) )
								// Need to copy context (1)
								existingCopyContextNr = existingRelationContextNr;
							else
								{
								if( isExistingSpecificationConcludedAssumption ||
								isSpecificationWordSpanishAmbiguous ||

								( isPossessive &&
								isPossessiveUserSpecification &&
								existingSpecificationItem->isSelfGeneratedConclusion() ) )
									contextResult.contextNr = foundRelationContextNr;
								else
									{
									if( existingSpecificationItem->isSelfGeneratedAssumption() == isSelfGeneratedAssumption )
										{
										if( isPossessive ||
										isArchivedAssignment ||
										existingSpecificationItem->isSelfGenerated() )
											contextResult.contextNr = existingRelationContextNr;
										else
											// Need to copy context (1)
											existingCopyContextNr = existingRelationContextNr;
										}
									else
										{
										if( ( isGeneralizationWordUserRelationWord &&
										isSelfGeneratedAssumption &&

										( hasRelationWordExistingRelationContext ||

										( isPossessive &&
										!isSpecificationWordSpanishAmbiguous &&
										isValidRelationContextInAllWords( true, existingRelationContextNr, specificationWordItem, relationWordItem ) ) ) ) ||

										( isPossessive &&

										( specificationCollectionNr == NO_COLLECTION_NR ||

										( !isGeneralizationWordUserRelationWord &&
										generalizationWordItem->hasAnsweredSelfGeneratedQuestion() ) ) ) )
											contextResult.contextNr = existingRelationContextNr;
										}
									}
								}
							}
						}
					}
				}
			}
		else
			{
			if( isSelfGeneratedAssumption &&
			existingSpecificationItem != NULL &&
			existingSpecificationItem->hasRelationContext() &&
			existingSpecificationItem->relationContextNr() != contextResult.contextNr &&
			relationWordItem->hasConfirmedSpecification() )
				contextResult.contextNr = existingSpecificationItem->relationContextNr();
			else
				{
				tempContextNr = foundContextItem->contextNr();

				if( isAssignment &&
				!isInactiveAssignment &&
				!isArchivedAssignment &&
				// Check if assignment already exists
				generalizationWordItem->firstNonQuestionAssignmentItem( ( !isInactiveAssignment && !isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, false, isPossessive, tempContextNr, specificationWordItem ) == NULL &&
				// Check also recently replaced assignments
				generalizationWordItem->firstRecentlyReplacedAssignmentItem( false, isPossessive, NO_QUESTION_PARAMETER, tempContextNr, specificationWordItem ) == NULL )
					{
					// Dynamic semantic ambiguity
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
						return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				else
					contextResult.contextNr = tempContextNr;
				}
			}

		if( contextResult.contextNr == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;

			if( existingCopyContextNr > NO_CONTEXT_NR )
				{
				if( copyContext( existingCopyContextNr, contextResult.contextNr ) != RESULT_OK )
					return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to copied a context" );

				contextResult.copiedRelationContextNr = existingCopyContextNr;
				}
			}

		return contextResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	How joyful are those who fear the Lord
 *	and delight in obeying his commands." (Psalm 112:1)
 *************************************************************************/
