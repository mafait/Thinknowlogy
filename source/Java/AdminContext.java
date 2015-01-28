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

class AdminContext
	{
	// Private constructible variables

	private boolean hasFoundUserSpecification_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, int nContextRelations, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundAllRelationWords;
		int currentRelationContextNr;
		int nContextWords;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( generalizationWordItem != null )
				{
				if( specificationWordItem != null )
					{
					if( ( currentGeneralizationItem = generalizationWordItem.firstRelationGeneralizationItem() ) != null )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
								{
								if( ( currentSpecificationItem = currentGeneralizationWordItem.firstNonQuestionSpecificationItem() ) != null )
									{
									do	{
										if( currentSpecificationItem.hasRelationContext() &&
										currentSpecificationItem.isPossessive() == isPossessive &&
										currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, specificationWordItem ) )
											{
											currentRelationContextNr = currentSpecificationItem.relationContextNr();
											nContextWords = myWordItem_.nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );
											// This relation word will be the last one
											hasFoundAllRelationWords = ( nContextWords + 1 == nContextRelations );

											if( currentSpecificationItem.isPossessiveReversibleConclusion() )
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
									while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
									( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
									}
								}
							else
								myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
							}
						while( CommonVariables.result == Constants.RESULT_OK &&
						( currentGeneralizationItem = currentGeneralizationItem.nextRelationGeneralizationItem() ) != null );
						}
					}
				else
					myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation context number is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	private static ContextItem firstContextItemInAllWords( boolean isUserGeneralizationWord, WordItem specificationWordItem )
		{
		ContextItem foundContextItem;
		WordItem currentWordItem;

		if( specificationWordItem != null &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( ( isUserGeneralizationWord &&
				currentWordItem.isUserGeneralizationWord ) ||

				( !isUserGeneralizationWord &&
				currentWordItem.isUserRelationWord ) ) &&

				( foundContextItem = currentWordItem.contextItemInWord( specificationWordItem ) ) != null )
					return foundContextItem;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return null;
		}


	// Constructor / deconstructor

	protected AdminContext( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		hasFoundUserSpecification_ = false;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected context methods

	protected void initializeAdminContextVariables()
		{
		hasFoundUserSpecification_ = false;
		}

	protected int highestContextNr()
		{
		int tempContextNr;
		int highestContextNr = Constants.NO_CONTEXT_NR;
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( tempContextNr = currentWordItem.highestContextNrInWord() ) > highestContextNr )
					highestContextNr = tempContextNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return highestContextNr;
		}

	protected ContextResultType addPronounContext( short contextWordTypeNr, WordItem contextWordItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( contextWordItem != null )
			{
			if( ( contextResult.contextNr = contextWordItem.contextNrInWord( null ) ) == Constants.NO_CONTEXT_NR )
				{
				if( ( contextResult.contextNr = highestContextNr() ) < Constants.MAX_CONTEXT_NR )
					// Create new context number
					contextResult.contextNr++;
				else
					myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
				}

			if( contextWordItem.addContext( false, contextWordTypeNr, Constants.WORD_TYPE_UNDEFINED, contextResult.contextNr, null ) != Constants.RESULT_OK )
				myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a pronoun context to word \"" + contextWordItem.anyWordTypeString() + "\"" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The read word of the read ahead item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	protected ContextResultType getRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean isSpecificationCollectedWithItself;
		int foundRelationContextNr;
		ContextItem foundContextItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem selfGeneratedSpecificationItem = null;
		WordItem foundRelationWordItem = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( relationWordItem != null )
					{
					isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

					// Try to find relation context with same number of relation words as in the user sentence
					if( ( contextResult.contextNr = relationWordItem.contextNrInWord( CommonVariables.nUserRelationWords, specificationWordItem ) ) == Constants.NO_CONTEXT_NR )
						{
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

						if( ( foundRelationContextNr = relationWordItem.contextNrInWord( specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
							{
							if( !isSelfGeneratedAssumption ||
							existingSpecificationItem == null ||
							!existingSpecificationItem.hasRelationContext() )
								contextResult.contextNr = foundRelationContextNr;
							else
								{
								if( generalizationWordItem.hasConfirmedSpecification() )
									{
									selfGeneratedSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isNegative, isPossessive, !isSelfGeneratedAssumption, specificationWordItem );

									contextResult.contextNr = ( selfGeneratedSpecificationItem == null ||
																selfGeneratedSpecificationItem.relationContextNr() == foundRelationContextNr ? existingSpecificationItem.relationContextNr() : foundRelationContextNr );
									}
								}
							}
						}

					if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						foundRelationContextNr = Constants.NO_CONTEXT_NR;

						if( ( foundContextItem = firstContextItemInAllWords( generalizationWordItem.isUserGeneralizationWord, specificationWordItem ) ) != null )
							{
							// Find specification with found context word as relation word
							foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );

							if( foundSpecificationItem != null &&
							!foundSpecificationItem.hasRelationContext() )
								foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

							if( foundSpecificationItem != null )
								// No such specification found. Get the relation context of a similar specification
								foundRelationContextNr = foundSpecificationItem.relationContextNr();
							else
								// Take the found relation context
								foundRelationContextNr = foundContextItem.contextNr();
							}

						if( foundRelationContextNr > Constants.NO_CONTEXT_NR )
							{
							// If cross-collected afterwards
							if( existingSpecificationItem != null )
								{
								foundRelationWordItem = existingSpecificationItem.relationWordItem();

								if( !hasFoundUserSpecification_ &&
								!isSpecificationCollectedWithItself &&
								generalizationWordItem.isUserGeneralizationWord &&

								( foundRelationWordItem == null ||
								foundRelationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) != null ) )
									hasFoundUserSpecification_ = true;

								if( hasFoundUserSpecification_ ||
								generalizationWordItem.hasConfirmedSpecification() )
									{
									if( existingSpecificationItem.isSelfGeneratedAssumption() )
										contextResult.contextNr = existingSpecificationItem.relationContextNr();
									}
								else
									{
									// Skip exclusive generalization
									if( !isAssignment &&

									( isSpecificationCollectedWithItself ||
									// Skip on difference in assumption / conclusion
									existingSpecificationItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption ) )
										contextResult.contextNr = foundRelationContextNr;
									}
								}
							}
						}
					else
						{
						if( isAssignment &&
						// Has no relation context collection
						myWordItem_.collectionNrInAllWords( relationContextNr ) == Constants.NO_COLLECTION_NR &&
						// Check if assignment already exists
						generalizationWordItem.firstNonQuestionAssignmentItem( ( !isInactiveAssignment && !isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, contextResult.contextNr, specificationWordItem ) == null &&
						// Check also recently replaced assignments
						generalizationWordItem.firstRecentlyReplacedAssignmentItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, contextResult.contextNr, specificationWordItem ) == null )
							{
							// Dynamic semantic ambiguity
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) == Constants.RESULT_OK )
								{
								contextResult.isAmbiguousRelationContext = true;
								// Create new context number
								contextResult.contextNr = Constants.NO_CONTEXT_NR;
								}
							else
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK &&
					contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						if( ( contextResult.contextNr = highestContextNr() ) < Constants.MAX_CONTEXT_NR )
							// Create new context number
							contextResult.contextNr++;
						else
							myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
						}
					}
				else
					myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	protected ContextResultType getRelationContext( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isQuestion, boolean isUserSentence, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundMatchingSpecificationCollection;
		boolean hasFoundRelationContext;
		boolean hasFoundRelationWordInThisList;
		boolean hasSameNumberOrMoreRelationWords;
		boolean isSkippingThisContext = false;
		int currentRelationContextNr;
		int foundRelationContextNr;
		ContextItem currentRelationContextItem;
		ReadItem relationWordReadItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem currentWordItem;
		WordItem currentRelationWordItem = relationWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( startRelationReadItem != null )
					{
					if( ( relationWordReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null )
						currentRelationWordItem = relationWordReadItem.readWordItem();
					else
						myWordItem_.startErrorInItem( 1, moduleNameString_, "The read word of the first relation word is undefined" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( currentRelationWordItem != null )
						{
						if( ( currentRelationContextItem = currentRelationWordItem.firstActiveContextItem() ) != null )
							{
							do	{	// Do for all relation context items in the first relation context word
								currentRelationContextNr = currentRelationContextItem.contextNr();

								if( currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) )
									{
									if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
										{
										hasFoundRelationWordInThisList = false;
										isSkippingThisContext = false;

										// The context that was found, is may be found by specification collection instead of specification word.
										// So, do the same check with context without specification collection.
										hasFoundMatchingSpecificationCollection = !currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem );

										// Do for all words, either in the current relation list or outside this list
										do	{
											foundSpecificationItem = ( isUserSentence ? null : generalizationWordItem.firstActiveAssignmentOrSpecificationItem( true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) );

											if( foundSpecificationItem == null ||
											!foundSpecificationItem.isSelfGeneratedConclusion() ||
											foundSpecificationItem.relationContextNr() != currentRelationContextNr )
												{
												if( relationWordReadItem != null )
													hasFoundRelationWordInThisList = relationWordReadItem.hasFoundRelationWordInThisList( currentWordItem );

												hasFoundRelationContext = currentWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem );

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
										( currentWordItem = currentWordItem.nextWordItem() ) != null );

										// The relation words in the list contain this context exclusively. (So, no other words)
										if( !isSkippingThisContext )
											contextResult.contextNr = currentRelationContextNr;
										}
									else
										myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );
									}
								}
							while( CommonVariables.result == Constants.RESULT_OK &&
							contextResult.contextNr == Constants.NO_CONTEXT_NR &&
							( currentRelationContextItem = currentRelationContextItem.nextContextItem() ) != null );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isQuestion &&
						contextResult.contextNr == Constants.NO_CONTEXT_NR &&
						( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isNegative, isPossessive, isPossessive, specificationWordItem ) ) != null )
							{
							if( ( foundRelationContextNr = foundSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR )
								{
								hasSameNumberOrMoreRelationWords = ( myWordItem_.nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) >= CommonVariables.nUserRelationWords );

								if( hasSameNumberOrMoreRelationWords ||
								generalizationWordItem.isUserRelationWord )
									{
									contextResult.contextNr = foundRelationContextNr;

									if( foundSpecificationItem.isExclusiveSpecification() )
										// Static (exclusive) semantic ambiguity
										contextResult.isExclusiveContext = true;
									else
										{
										if( !foundSpecificationItem.isAssignment() )
											{
											if( relationWordItem == null )
												{
												if( isAssignment ||
												isSkippingThisContext )
													// Different number of relation words
													// Create new context number
													contextResult.contextNr = Constants.NO_CONTEXT_NR;
												else
													{
													// Static (exclusive) semantic ambiguity
													if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) == Constants.RESULT_OK )
														contextResult.isExclusiveContext = true;
													else
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
													}
												}
											else
												{
												if( hasSameNumberOrMoreRelationWords ||
												specificationWordItem.isCollectedWithItself() )
													{
													// Try to find the relation context of a possessive reversible conclusion
													if( ( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isExclusiveSpecification, isNegative, isPossessive, nContextRelations, contextResult.contextNr, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
													}
												}
											}
										}
									}
								}
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						contextResult.contextNr == Constants.NO_CONTEXT_NR )
							{
							if( ( contextResult.contextNr = highestContextNr() ) < Constants.MAX_CONTEXT_NR )
								// Create new context number
								contextResult.contextNr++;
							else
								myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
							}
						}
					else
						myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any relation word" );
					}
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	How joyful are those who fear the Lord
 *	and delight in obeying his commands." (Psalm 112:1)
 *************************************************************************/
