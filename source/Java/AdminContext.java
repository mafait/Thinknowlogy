/*
 *	Class:			AdminContext
 *	Supports class:	AdminItem
 *	Purpose:		To create context structures
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

class AdminContext
	{
	// Private constructible variables

	private boolean hasFoundUserSpecification_;

	private int relationContextNr_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private static int contextNrInUserGeneralizationWord( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		ContextItem foundContextItem;
		WordItem currentWordItem;

		if( generalizationWordItem != null &&
		specificationWordItem != null &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.isUserGeneralizationWord &&
				currentWordItem != generalizationWordItem &&
				( foundContextItem = currentWordItem.contextItemInWord( specificationWordItem ) ) != null )
					return foundContextItem.contextNr();
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.NO_CONTEXT_NR;
		}

	private static int contextNrInUserRelationWord( WordItem relationWordItem, WordItem specificationWordItem )
		{
		ContextItem foundContextItem;
		WordItem currentWordItem;

		if( relationWordItem != null &&
		specificationWordItem != null &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.isUserRelationWord &&
				currentWordItem != relationWordItem &&
				( foundContextItem = currentWordItem.contextItemInWord( specificationWordItem ) ) != null )
					return foundContextItem.contextNr();
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return Constants.NO_CONTEXT_NR;
		}

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
								if( ( currentSpecificationItem = currentGeneralizationWordItem.firstSelectedSpecificationItem( false, false, false, false ) ) != null )
									{
									do	{
										if( currentSpecificationItem.hasRelationContext() &&
										currentSpecificationItem.isPossessive() == isPossessive &&
										currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, specificationWordItem ) )
											{
											currentRelationContextNr = currentSpecificationItem.relationContextNr();
											nContextWords = myWordItem_.nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );
											hasFoundAllRelationWords = ( nContextWords + 1 == nContextRelations );		// This relation word will be the last one

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

	private byte getRelationContext( boolean isNegative, boolean isPossessive, short questionParameter, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		ContextItem foundContextItem;
		SpecificationItem foundSpecificationItem;
		WordItem foundContextWordItem;

		relationContextNr_ = Constants.NO_CONTEXT_NR;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( ( foundContextItem = contextItemInAllWords( specificationWordItem ) ) != null )
					{
					if( ( foundContextWordItem = foundContextItem.myWordItem() ) != null )
						{
						// Find specification with found context word as relation word
						if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, questionParameter, specificationWordItem, foundContextWordItem ) ) == null )
							{
							if( ( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != null )
								// No such specification found. Get the relation context of a similar specification.
								relationContextNr_ = foundSpecificationItem.relationContextNr();
							}
						else
							// Specification found. Take the found relation context.
							relationContextNr_ = foundContextItem.contextNr();
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The word of the found context item is undefined" );
					}
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private static ContextItem contextItemInAllWords( WordItem specificationWordItem )
		{
		ContextItem foundContextItem;
		WordItem currentWordItem;

		if( specificationWordItem != null &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( foundContextItem = currentWordItem.contextItemInWord( specificationWordItem ) ) != null )
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

		relationContextNr_ = Constants.NO_CONTEXT_NR;

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
					contextResult.contextNr++;
				else
					myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
				}

			if( contextWordItem.addContext( false, contextWordTypeNr, Constants.WORD_TYPE_UNDEFINED, contextResult.contextNr, null ) != Constants.RESULT_OK )
				myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a context to word \"" + contextWordItem.anyWordTypeString() + "\"" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The read word of the read ahead item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	protected ContextResultType getRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short specificationWordTypeNr, short relationWordTypeNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasRelationContextCollection;
		boolean isSpecificationCollectedWithItself;
		boolean hasFoundContext = false;
		int oldRelationContextNr;
		SpecificationItem existingSpecificationItem;
		SpecificationItem reversedExistingSpecificationItem = null;
		WordItem foundRelationWordItem = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( relationWordItem != null )
					{
					hasRelationContextCollection = ( myWordItem_.collectionNrInAllWords( relationContextNr ) > Constants.NO_COLLECTION_NR );
					isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

					if( ( contextResult.contextNr = relationWordItem.contextNrInWord( CommonVariables.nUserRelationWords, specificationWordItem ) ) == Constants.NO_CONTEXT_NR )
						{
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem );

						if( isSpecificationCollectedWithItself ||
						existingSpecificationItem == null ||
						existingSpecificationItem.isOlderItem() ||
						relationWordItem.hasContextInWord( contextResult.contextNr, specificationWordItem ) )
							contextResult.contextNr = relationWordItem.contextNrInWord( specificationWordItem );
						}
					else
						hasFoundContext = true;

					if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						if( getRelationContext( isNegative, isPossessive, questionParameter, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
							{
							if( relationContextNr_ == Constants.NO_CONTEXT_NR )
								{
								if( relationWordItem.isUserGeneralizationWord )
									contextResult.contextNr = contextNrInUserGeneralizationWord( relationWordItem, specificationWordItem );
								else
									{
									if( relationWordItem.isUserRelationWord )
										contextResult.contextNr = contextNrInUserRelationWord( relationWordItem, specificationWordItem );
									}
								}
							else
								{
								if( ( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != null )
									foundRelationWordItem = existingSpecificationItem.relationWordItem();

								if( existingSpecificationItem != null )
									{
									if( !hasFoundUserSpecification_ &&
									!isSpecificationCollectedWithItself &&

									generalizationWordItem.isUserGeneralizationWord &&

									( foundRelationWordItem == null ||
									foundRelationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) != null ) )
										hasFoundUserSpecification_ = true;
									}

								// Cross-collected afterwards
								if( existingSpecificationItem != null &&

								( hasFoundUserSpecification_ ||
								generalizationWordItem.hasConfirmedSpecification() ) )
									{
									if( foundRelationWordItem != null )
										reversedExistingSpecificationItem = foundRelationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, !isPossessive, questionParameter, specificationWordItem );

									if( existingSpecificationItem.hasCurrentCreationSentenceNr() ||

									( reversedExistingSpecificationItem != null &&
									reversedExistingSpecificationItem.hasCurrentCreationSentenceNr() ) )
										contextResult.contextNr = existingSpecificationItem.relationContextNr();
									}
								else
									contextResult.contextNr = relationContextNr_;
								}

							if( !hasRelationContextCollection &&
							contextResult.contextNr > Constants.NO_CONTEXT_NR &&
							!relationWordItem.hasContextInWord( contextResult.contextNr, specificationWordItem ) )
								// Create new context number
								contextResult.contextNr = Constants.NO_CONTEXT_NR;
							}
						else
							myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the relation context" );
						}
					else
						{
						if( isAssignment &&
						!hasRelationContextCollection &&
						// Check if assignment already exists
						generalizationWordItem.firstAssignmentItem( ( !isInactiveAssignment && !isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, contextResult.contextNr, specificationWordItem ) == null &&
						// Check also recently replaced assignments
						generalizationWordItem.firstRecentlyReplacedAssignmentItem( isNegative, isPossessive, questionParameter, contextResult.contextNr, specificationWordItem ) == null )
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
						else
							{
							if( generalizationWordItem.isUserGeneralizationWord )
								{
								if( !hasFoundContext )
									{
									if( getRelationContext( isNegative, isPossessive, questionParameter, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
										{
										if( relationContextNr_ > Constants.NO_CONTEXT_NR )
											contextResult.contextNr = relationContextNr_;
										}
									else
										myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the relation context" );
									}
								}
							else
								{
								if( contextResult.contextNr > Constants.NO_CONTEXT_NR &&
								( oldRelationContextNr = contextNrInUserGeneralizationWord( relationWordItem, specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
									{
									if( isSpecificationCollectedWithItself )
										{
										if( contextResult.contextNr == contextNrInUserGeneralizationWord( generalizationWordItem, specificationWordItem ) )
											contextResult.contextNr = oldRelationContextNr;
										}
									else
										{
										// In case the context number doesn't exists for current sentence
										if( !myWordItem_.isCurrentContextInAllWords( contextResult.contextNr, specificationWordItem ) )
											{
											if( ( contextResult.contextNr = highestContextNr() ) < Constants.MAX_CONTEXT_NR )
												{
												if( myWordItem_.copyContextInAllWords( relationWordTypeNr, specificationWordTypeNr, oldRelationContextNr, ++contextResult.contextNr, specificationWordItem ) != Constants.RESULT_OK )
													myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to copy the old context in all words" );
												}
											else
												myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
											}
										}
									}
								}
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK &&
					contextResult.contextNr == Constants.NO_CONTEXT_NR )		// No context found yet
						{
						if( ( contextResult.contextNr = highestContextNr() ) < Constants.MAX_CONTEXT_NR )
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

	protected ContextResultType getRelationContext( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUserSentence, short questionParameter, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundMatchingSpecificationCollection;
		boolean hasFoundRelationContext;
		boolean hasFoundRelationWordInThisList;
		boolean hasSameNumberOrMoreRelationWords;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
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
										hasFoundMatchingSpecificationCollection = ( !isQuestion &&
																					!currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) );

										// Do for all words, either in the current relation list or outside this list
										do	{
											foundSpecificationItem = ( isUserSentence ? null : generalizationWordItem.firstActiveAssignmentOrSpecificationItem( true, isNegative, isPossessive, questionParameter, specificationWordItem ) );

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
						questionParameter == Constants.NO_QUESTION_PARAMETER &&
						contextResult.contextNr == Constants.NO_CONTEXT_NR &&
						( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, isNegative, isPossessive, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
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
											else	// Try to find the relation context of a possessive reversible conclusion
												{
												if( hasSameNumberOrMoreRelationWords ||
												specificationWordItem.isCollectedWithItself() )
													{
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
						contextResult.contextNr == Constants.NO_CONTEXT_NR )		// No context found yet
							{
							if( ( contextResult.contextNr = highestContextNr() ) < Constants.MAX_CONTEXT_NR )
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
 *
 *	"Praise the Lord!
 *	How joyful are those who fear the Lord
 *	and delight in obeying his commands." (Psalm 112:1)
 *
 *************************************************************************/
