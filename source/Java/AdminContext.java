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

class AdminContext
	{
	// Private constructed variables

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean isValidRelationContextInAllWords( boolean isPossessive, int existingRelationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		WordItem currentContextWordItem;

		if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&
		specificationWordItem != null &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem.isProperName() &&
				currentContextWordItem != relationWordItem &&
				!currentContextWordItem.isUserRelationWord &&
				currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, existingRelationContextNr, specificationWordItem ) != null )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return true;
		}

	private ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( boolean isPossessive, int nContextRelations, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		int currentRelationContextNr;
		int nContextWords;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr == Constants.NO_CONTEXT_NR )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = generalizationWordItem.firstRelationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startContextResultError( 1, moduleNameString_, "I have found an undefined generalization word" );

				if( ( currentSpecificationItem = currentGeneralizationWordItem.firstNonQuestionSpecificationItem() ) != null )
					{
					do	{
						if( ( currentRelationContextNr = currentSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
						currentSpecificationItem.isRelatedSpecification( false, false, isPossessive, specificationWordItem ) )
							{
							if( !isPossessive ||
							currentRelationContextNr != relationContextNr )
								{
								nContextWords = adminItem_.nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );

								if( currentSpecificationItem.isPossessiveReversibleConclusion() )
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
					while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
					( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextRelationGeneralizationItem() ) != null );
			}

		return contextResult;
		}

	private byte copyContext( int existingContextNr, int newContextNr )
		{
		ContextItem currentContextItem;
		WordItem currentContextWordItem;

		if( ( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( ( currentContextItem = currentContextWordItem.contextItem( existingContextNr ) ) != null )
					{
					if( currentContextWordItem.addContext( currentContextItem.isCompoundCollectionSpanishAmbiguous(), currentContextItem.contextWordTypeNr(), currentContextItem.specificationWordTypeNr(), newContextNr, currentContextItem.specificationWordItem() ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a copied context to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminContext( AdminItem adminItem )
		{
		String errorString = null;

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
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected context methods

	protected ContextResultType getRelationContext( boolean isArchivedAssignment, boolean isPossessive, boolean isQuestion, boolean isUserSentence, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundRelationContext;
		boolean hasFoundRelationWordInThisList;
		boolean hasSameNumberOrMoreRelationWords;
		boolean isSkippingThisContext = false;
		int currentRelationContextNr;
		int foundRelationContextNr;
		ContextItem currentRelationContextItem;
		ReadItem relationWordReadItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem currentContextWordItem;
		WordItem currentRelationWordItem = relationWordItem;
		WordItem firstContextWordItem = CommonVariables.firstContextWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem != null )
			{
			if( ( relationWordReadItem = startRelationReadItem.firstRelationWordReadItem() ) == null )
				return adminItem_.startContextResultError( 1, moduleNameString_, "The read word of the first relation word is undefined" );

			currentRelationWordItem = relationWordReadItem.readWordItem();
			}

		if( currentRelationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "I couldn't find any relation word" );

		if( ( currentRelationContextItem = currentRelationWordItem.firstActiveContextItem() ) != null )
			{
			// Do for all relation context items in the first relation context word
			do	{
				currentRelationContextNr = currentRelationContextItem.contextNr();

				if( currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) )
					{
					hasFoundRelationWordInThisList = false;
					isSkippingThisContext = false;

					if( firstContextWordItem != null )
						{
						currentContextWordItem = firstContextWordItem;

						// Do for all context words, either in the current relation list or outside this list
						do	{
							foundSpecificationItem = ( isUserSentence ? null : generalizationWordItem.firstActiveAssignmentOrSpecificationItem( true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) );

							if( foundSpecificationItem == null ||
							!foundSpecificationItem.isSelfGeneratedConclusion() )
								{
								if( relationWordReadItem != null )
									hasFoundRelationWordInThisList = relationWordReadItem.hasFoundRelationWordInThisList( currentContextWordItem );

								hasFoundRelationContext = currentContextWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem );

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
						( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
						}

					// The relation words in the list contain this context exclusively. (So, no other words)
					if( !isSkippingThisContext )
						contextResult.contextNr = currentRelationContextNr;
					}
				else
					{
					if( isUserSentence &&
					!currentRelationContextItem.isOlderItem() )
						contextResult.contextNr = currentRelationContextNr;
					}
				}
			while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
			( currentRelationContextItem = currentRelationContextItem.nextContextItem() ) != null );
			}

		if( !isQuestion &&
		contextResult.contextNr == Constants.NO_CONTEXT_NR &&
		( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, null ) ) != null )
			{
			if( ( foundRelationContextNr = foundSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR )
				{
				hasSameNumberOrMoreRelationWords = ( adminItem_.nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) >= CommonVariables.nUserRelationWords );

				if( hasSameNumberOrMoreRelationWords ||
				generalizationWordItem.isUserRelationWord )
					{
					if( foundSpecificationItem.isExclusiveSpecification() )
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

								if( !foundSpecificationItem.isActiveAssignment() )
									{
									// Static (exclusive) semantic ambiguity
									if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
										return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

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
								if( ( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isPossessive, nContextRelations, contextResult.contextNr, foundSpecificationItem, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
								}
							}
						}
					}
				}
			}

		if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_.highestContextNrInAllContextWords() ) >= Constants.MAX_CONTEXT_NR )
				return adminItem_.startContextResultSystemError( 1, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		return contextResult;
		}

	protected ContextResultType getSpecificationRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCompoundCollectionSpanishAmbiguous, boolean isPossessive, boolean isSelfGeneratedAssumption, int specificationCollectionNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasRelationWordExistingRelationContext;
		boolean isExistingSpecificationConcludedAssumption;
		boolean isGeneralizationWordUserRelationWord;
		boolean isPossessiveUserSpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		int tempContextNr;
		int existingCopyContextNr = Constants.NO_CONTEXT_NR;
		int existingRelationContextNr = Constants.NO_CONTEXT_NR;
		int foundRelationContextNr = Constants.NO_CONTEXT_NR;
		int nUserRelationWords = CommonVariables.nUserRelationWords;
		ContextItem foundContextItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem existingSpecificationItem = null;

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( isCompoundCollectionSpanishAmbiguous )
			existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem );
		else
			existingSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, isArchivedAssignment, false, isPossessive, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

		// Try to find relation context with same number of relation words as in the user sentence
		if( ( foundContextItem = relationWordItem.contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, specificationWordItem ) ) == null )
			{
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( existingSpecificationItem == null ||

			( isGeneralizationWordUserRelationWord &&
			!isSpecificationWordSpanishAmbiguous &&
			!existingSpecificationItem.hasRelationContext() &&
			!relationWordItem.hasConfirmedSpecification() ) )
				{
				if( nUserRelationWords > 1 &&

				( CommonVariables.nUserGeneralizationWords > 1 ||
				!relationWordItem.isOlderItem() ||
				// Typically for the Spanish language
				generalizationWordItem.isFemale() ) &&

				( tempContextNr = relationWordItem.contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
					contextResult.contextNr = tempContextNr;
				}
			else
				{
				isExistingSpecificationConcludedAssumption = existingSpecificationItem.isConcludedAssumption();
				existingRelationContextNr = existingSpecificationItem.relationContextNr();

				// Not found yet
				if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
					{
					// Find specification with found context word as relation word
					if( isSpecificationWordSpanishAmbiguous )
						{
						if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
							{
							if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&

							( !foundSpecificationItem.hasRelationContext() ||
							existingSpecificationItem.isHiddenSpanishSpecification() ) )
								foundRelationContextNr = existingRelationContextNr;
							else
								{
								if( specificationCollectionNr == Constants.NO_COLLECTION_NR ||
								foundSpecificationItem.isUserSpecification() ||
								foundSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
									foundRelationContextNr = foundSpecificationItem.relationContextNr();
								}
							}
						}
					else
						{
						if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&

						( !isExistingSpecificationConcludedAssumption ||
						isGeneralizationWordUserRelationWord ) &&

						existingSpecificationItem.isArchivedAssignment() == isArchivedAssignment )
							foundRelationContextNr = existingRelationContextNr;
						}

					// Cross-collected afterwards
					if( foundRelationContextNr > Constants.NO_CONTEXT_NR &&
					contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						userSpecificationItem = adminItem_.userSpecificationItem();

						if( generalizationWordItem.hasConfirmedSpecification() )
							{
							if( !isPossessive ||
							specificationCollectionNr == Constants.NO_COLLECTION_NR ||
							!existingSpecificationItem.isOlderItem() ||

							( isSelfGeneratedAssumption &&
							existingSpecificationItem.isSelfGeneratedConclusion() ) ||

							( ( !isSpecificationWordSpanishAmbiguous ||
							// Typically for Spanish
							userSpecificationItem.isSpecificationWordSpanishAmbiguous() ) &&

							( userSpecificationItem != null &&
							userSpecificationItem.hasSpecificationCompoundCollection() ) ) )
								contextResult.contextNr = existingRelationContextNr;
							}
						else
							{
							hasRelationWordExistingRelationContext = relationWordItem.hasContextInWord( existingRelationContextNr, specificationWordItem );
							isPossessiveUserSpecification = ( userSpecificationItem != null &&
															userSpecificationItem.isPossessive() );

							// Check for copy context
							if( !hasRelationWordExistingRelationContext &&
							isGeneralizationWordUserRelationWord &&
							existingSpecificationItem.isOlderItem() &&

							( ( isExistingSpecificationConcludedAssumption &&
							isPossessiveUserSpecification ) ||

							( !isExistingSpecificationConcludedAssumption &&
							!isValidRelationContextInAllWords( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) &&
							adminItem_.nContextWordsInAllWords( existingRelationContextNr, specificationWordItem ) > CommonVariables.nUserGeneralizationWords ) ) )
								// Need to copy context (1)
								existingCopyContextNr = existingRelationContextNr;
							else
								{
								if( isExistingSpecificationConcludedAssumption ||
								isSpecificationWordSpanishAmbiguous ||

								( isPossessive &&
								isPossessiveUserSpecification &&
								existingSpecificationItem.isSelfGeneratedConclusion() ) )
									contextResult.contextNr = foundRelationContextNr;
								else
									{
									if( existingSpecificationItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption )
										{
										if( isPossessive ||
										isArchivedAssignment ||
										existingSpecificationItem.isSelfGenerated() )
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

										( specificationCollectionNr == Constants.NO_COLLECTION_NR ||

										( !isGeneralizationWordUserRelationWord &&
										generalizationWordItem.hasAnsweredSelfGeneratedQuestion() ) ) ) )
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
			existingSpecificationItem != null &&
			existingSpecificationItem.hasRelationContext() &&
			existingSpecificationItem.relationContextNr() != contextResult.contextNr &&
			relationWordItem.hasConfirmedSpecification() )
				contextResult.contextNr = existingSpecificationItem.relationContextNr();
			else
				{
				tempContextNr = foundContextItem.contextNr();

				if( isAssignment &&
				!isInactiveAssignment &&
				!isArchivedAssignment &&
				// Check if assignment already exists
				generalizationWordItem.firstNonQuestionAssignmentItem( ( !isInactiveAssignment && !isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, false, isPossessive, tempContextNr, specificationWordItem ) == null &&
				// Check also recently replaced assignments
				generalizationWordItem.firstRecentlyReplacedAssignmentItem( false, isPossessive, Constants.NO_QUESTION_PARAMETER, tempContextNr, specificationWordItem ) == null )
					{
					// Dynamic semantic ambiguity
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
						return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				else
					contextResult.contextNr = tempContextNr;
				}
			}

		if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_.highestContextNrInAllContextWords() ) >= Constants.MAX_CONTEXT_NR )
				return adminItem_.startContextResultSystemError( 1, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;

			if( existingCopyContextNr > Constants.NO_CONTEXT_NR )
				{
				if( copyContext( existingCopyContextNr, contextResult.contextNr ) != Constants.RESULT_OK )
					return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to copied a context" );

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
