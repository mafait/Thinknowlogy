/*
 *	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
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

#ifndef SPECIFICATIONITEM
#define SPECIFICATIONITEM 1
#include "List.h"
#include "WordItem.h"

class SpecificationItem : private Item
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminCollection;
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminLanguage;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class AdminWriteJustification;
	friend class AdminWriteSpecification;
	friend class JustificationItem;
	friend class JustificationList;
	friend class SpecificationList;
	friend class WordAssignment;
	friend class WordCollection;
	friend class WordItem;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordWrite;
	friend class WordWriteSentence;
	friend class WordWriteWords;

	// Private constructible variables

	unsigned short nAssumptionLevelRecalculations_;

	// Private loadable variables

	bool isAnsweredQuestion_;
	bool isConcludedAssumption_;
	bool isConditional_;
	bool isCorrectedAssumption_;
	bool isEveryGeneralization_;
	bool isExclusiveSpecification_;
	bool isGeneralizationAssignment_;
	bool isLanguageWord_;
	bool isNegative_;
	bool isPartOf_;
	bool isPossessive_;
	bool isSpecificationGeneralization_;
	bool isUniqueRelation_;
	bool isValueSpecification_;

	unsigned short assignmentLevel_;
	unsigned short assumptionLevel_;
	unsigned short grammarLanguageNr_;
	unsigned short prepositionParameter_;
	unsigned short questionParameter_;

	unsigned short generalizationWordTypeNr_;
	unsigned short specificationWordTypeNr_;
	unsigned short relationWordTypeNr_;

	unsigned int generalizationCollectionNr_;
	unsigned int specificationCollectionNr_;

	unsigned int generalizationContextNr_;
	unsigned int specificationContextNr_;
	unsigned int relationContextNr_;

	unsigned int nContextRelations_;

	JustificationItem *firstJustificationItem_;

	WordItem *specificationWordItem_;

	char *specificationString_;


	// Private constructible variables

	unsigned short specificationStringWriteLevel_;

	unsigned int lastCheckedAssumptionLevelItemNr_;


	// Private question functions

	SpecificationItem *newUserQuestion( bool isIncludingThisItem )
		{
		SpecificationItem *searchItem = ( isIncludingThisItem ? this : nextSelectedSpecificationItem() );

		while( searchItem != NULL )
			{
			if( !searchItem->isAnsweredQuestion_ &&
			!searchItem->isOlderItem() &&
			searchItem->isUserQuestion() )
				return searchItem;

			searchItem = searchItem->nextSelectedSpecificationItem();
			}

		return searchItem;
		}


	// Private specification functions

	SpecificationResultType calculateAssumptionLevel( bool isNeedingRecalculation )
		{
		SpecificationResultType specificationResult;
		unsigned short highestAssumptionLevel;
		unsigned short lowestAssumptionLevel = MAX_LEVEL;
		unsigned int tempAssumptionLevel;
		unsigned int nJustificationRelationWords;
		unsigned int nSpecificationRelationWords;
		JustificationItem *currentJustificationItem;
		JustificationItem *nextJustificationItem = firstJustificationItem_;
		char functionNameString[FUNCTION_NAME_LENGTH] = "calculateAssumptionLevel";

		if( !isSpecificationGeneralization_ &&
		isSelfGeneratedAssumption() )
			{
			if( ++nAssumptionLevelRecalculations_ < MAX_ASSUMPTION_LEVEL_RECALCULATIONS )
				{
				if( isNeedingRecalculation ||
				assumptionLevel_ == NO_ASSUMPTION_LEVEL )
					{
					specificationResult.hasPerformedRecalculation = true;
					nSpecificationRelationWords = nRelationContextWords();

					do	{
						nJustificationRelationWords = 0;
						highestAssumptionLevel = NO_ASSUMPTION_LEVEL;
						currentJustificationItem = nextJustificationItem;

						if( currentJustificationItem != NULL )
							{
							do	{
								nJustificationRelationWords += currentJustificationItem->nJustificationContextRelations( relationContextNr_, nSpecificationRelationWords );

								if( currentJustificationItem->isAssumptionJustification() )
									{
									if( ( specificationResult = currentJustificationItem->getCombinedAssumptionLevel() ).result == RESULT_OK )
										{
										if( ( tempAssumptionLevel = ( specificationResult.combinedAssumptionLevel + currentJustificationItem->justificationAssumptionGrade() ) ) > highestAssumptionLevel )
											{
											if( tempAssumptionLevel < MAX_LEVEL )
												highestAssumptionLevel = (unsigned short)tempAssumptionLevel;
											else
												startSystemErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "Assumption level overflow" );
											}
										}
									else
										addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to get the combined assumption level" );
									}
								}
							while( commonVariables()->result == RESULT_OK &&
							( currentJustificationItem = currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );
							}

						if( commonVariables()->result == RESULT_OK &&
						highestAssumptionLevel < lowestAssumptionLevel &&

						( nJustificationRelationWords == nSpecificationRelationWords ||

						( assumptionLevel_ == NO_ASSUMPTION_LEVEL &&
						lastCheckedAssumptionLevelItemNr_ == commonVariables_->currentItemNr ) ) )	// To avoid looping: calculation A, calculation B, calculation A, etc.
							lowestAssumptionLevel = highestAssumptionLevel;
						}
					while( commonVariables()->result == RESULT_OK &&
					( nextJustificationItem = nextJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != NULL );

					if( commonVariables()->result == RESULT_OK )
						{
						if( lowestAssumptionLevel < MAX_LEVEL )
							{
							nAssumptionLevelRecalculations_ = 0;

							if( assumptionLevel_ != lowestAssumptionLevel )
								{
								assumptionLevel_ = lowestAssumptionLevel;

								if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
									strcpy( lastWrittenSentenceString, EMPTY_STRING );
								}
							}

						lastCheckedAssumptionLevelItemNr_ = commonVariables_->currentItemNr;
						}
					}
				}
			else
				startErrorInItem( functionNameString, NULL, "There is probably an endless loop in the assumption level calculation of my specification, because the number of iterations is: ", nAssumptionLevelRecalculations_ );
			}

		specificationResult.assumptionLevel = assumptionLevel_;
		specificationResult.result = commonVariables()->result;
		return specificationResult;
		}


	protected:
	// Protected constructible variables

	bool hasAlreadyBeenWrittenAsAnswer;
	bool hasAlreadyBeenWrittenAsConflict;

	SpecificationItem *replacingSpecificationItem;

	char lastWrittenSentenceString[MAX_SENTENCE_STRING_LENGTH];
	char lastWrittenSentenceWithOneSpecificationOnlyString[MAX_SENTENCE_STRING_LENGTH];


	// Constructor / deconstructor

	SpecificationItem( bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isLanguageWord, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short grammarLanguageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		size_t specificationStringLength;

		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, "SpecificationItem", commonVariables, myList, myWordItem );

		// Private constructible variables

		nAssumptionLevelRecalculations_ = 0;

		// Private loadable variables

		isAnsweredQuestion_ = isAnsweredQuestion;
		isConcludedAssumption_ = isConcludedAssumption;
		isConditional_ = isConditional;
		isCorrectedAssumption_ = isCorrectedAssumption;
		isEveryGeneralization_ = isEveryGeneralization;
		isExclusiveSpecification_ = isExclusiveSpecification;
		isGeneralizationAssignment_ = isGeneralizationAssignment;
		isLanguageWord_ = isLanguageWord;
		isNegative_ = isNegative;
		isPartOf_ = isPartOf;
		isPossessive_ = isPossessive;
		isSpecificationGeneralization_ = isSpecificationGeneralization;
		isUniqueRelation_ = isUniqueRelation;
		isValueSpecification_ = isValueSpecification;

		assignmentLevel_ = assignmentLevel;
		assumptionLevel_ = assumptionLevel;
		grammarLanguageNr_ = grammarLanguageNr;
		prepositionParameter_ = prepositionParameter;
		questionParameter_ = questionParameter;

		generalizationWordTypeNr_ = generalizationWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;
		relationWordTypeNr_ = relationWordTypeNr;

		generalizationCollectionNr_ = generalizationCollectionNr;
		specificationCollectionNr_ = specificationCollectionNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nContextRelations_ = nContextRelations;

		firstJustificationItem_ = firstJustificationItem;

		specificationWordItem_ = specificationWordItem;

		specificationString_ = NULL;

		if( specificationString != NULL )
			{
			if( ( specificationStringLength = strlen( specificationString ) ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( specificationString_ = new char[specificationStringLength + 1] ) != NULL )
					strcpy( specificationString_, specificationString );
				else
					startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create a specification string" );
				}
			else
				startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given specification string is too long" );
			}

		// Private constructible variables

		specificationStringWriteLevel_ = NO_WRITE_LEVEL;
		lastCheckedAssumptionLevelItemNr_ = NO_ITEM_NR;

		// Protected constructible variables

		hasAlreadyBeenWrittenAsAnswer = false;
		hasAlreadyBeenWrittenAsConflict = false;

		replacingSpecificationItem = NULL;
		strcpy( lastWrittenSentenceString, EMPTY_STRING );
		strcpy( lastWrittenSentenceWithOneSpecificationOnlyString, EMPTY_STRING );
		}

	~SpecificationItem()
		{
		if( specificationString_ != NULL )
			delete specificationString_;
		}


	// Protected virtual functions

	virtual void showString( bool isReturnQueryToPosition )
		{
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( specificationString_ != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status when not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, specificationString_ );
			}
		}

	virtual void showWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status when not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( prepositionParameter_ == queryParameter ||
				questionParameter_ == queryParameter ||
				generalizationCollectionNr_ == queryParameter ||
				specificationCollectionNr_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nContextRelations_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( prepositionParameter_ > NO_PREPOSITION_PARAMETER ||
				questionParameter_ > NO_QUESTION_PARAMETER ||
				generalizationCollectionNr_ > NO_COLLECTION_NR ||
				specificationCollectionNr_ > NO_COLLECTION_NR ||
				generalizationContextNr_ > NO_CONTEXT_NR ||
				specificationContextNr_ > NO_CONTEXT_NR ||
				relationContextNr_ > NO_CONTEXT_NR ||
				nContextRelations_ > 0 ) ) );
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( firstJustificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : firstJustificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : firstJustificationItem_->itemNr() == queryItemNr ) ) ||

				( specificationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : specificationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : specificationWordItem_->itemNr() == queryItemNr ) ) ||

				( replacingSpecificationItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : replacingSpecificationItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : replacingSpecificationItem->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( generalizationWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr	||
				relationWordTypeNr_ == queryWordTypeNr	);
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		SpecificationItem *nextSortSpecificationItem = (SpecificationItem *)nextSortItem;

		return ( nextSortItem != NULL &&

				// 1) Assignment needs descending assignmentLevel_
				( assignmentLevel_ > nextSortSpecificationItem->assignmentLevel_ ||

				// 2) Question and specification needs descending creationSentenceNr()
				( assignmentLevel_ == nextSortSpecificationItem->assignmentLevel_ &&
				creationSentenceNr() > nextSortItem->creationSentenceNr() ) ) );
		}

	virtual ResultType checkForUsage()
		{
		return myWordItem()->checkSpecificationForUsageOfInvolvedWords( this );
		}

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( specificationWordItem_ != NULL )
			{
			if( ( referenceResult = specificationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find a matching word reference string for the specification word" );
			}

		return referenceResult;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *languageNameString = myWordItem()->grammarLanguageNameString( grammarLanguageNr_ );
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		char *relationWordTypeString = ( relationWordTypeNr_ == WORD_TYPE_UNDEFINED ? NULL : myWordItem()->wordTypeNameString( relationWordTypeNr_ ) );

		Item::toString( queryWordTypeNr );

		if( grammarLanguageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%cgrammarLanguageNr:%u", QUERY_SEPARATOR_CHAR, grammarLanguageNr_ );
			else
				sprintf( tempString, "%cgrammarLanguage:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( commonVariables()->queryString, tempString );
			}

		if( isAnsweredQuestion_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isAnsweredQuestion" );
			}

		if( isConcludedAssumption_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isConcludedAssumption" );
			}

		if( isConditional_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isConditional" );
			}

		if( isCorrectedAssumption_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isCorrectedAssumption" );
			}

		if( isEveryGeneralization_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isEveryGeneralization" );
			}

		if( isExclusiveSpecification_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isExclusiveSpecification" );
			}

		if( isGeneralizationAssignment_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isGeneralizationAssignment" );
			}

		if( isLanguageWord_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isLanguageWord" );
			}

		if( isNegative_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isNegative" );
			}

		if( isPartOf_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isPartOf" );
			}

		if( isPossessive_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isPossessive" );
			}

		if( isSpecificationGeneralization_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isSpecificationGeneralization" );
			}

		if( isUniqueRelation_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isUniqueRelation" );
			}

		if( isValueSpecification_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isValueSpecification" );
			}

		if( assignmentLevel_ > NO_ASSIGNMENT_LEVEL )
			{
			sprintf( tempString, "%cassignmentLevel:%u", QUERY_SEPARATOR_CHAR, assignmentLevel_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
			{
			sprintf( tempString, "%cassumptionLevel:%u", QUERY_SEPARATOR_CHAR, assumptionLevel_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationStringWriteLevel_ > NO_WRITE_LEVEL )
			{
			sprintf( tempString, "%cspecificationStringWriteLevel:%u", QUERY_SEPARATOR_CHAR, specificationStringWriteLevel_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( prepositionParameter_ > NO_PREPOSITION_PARAMETER )
			{
			sprintf( tempString, "%cprepositionParameter:%u", QUERY_SEPARATOR_CHAR, prepositionParameter_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( questionParameter_ > NO_QUESTION_PARAMETER )
			{
			sprintf( tempString, "%cquestionParameter:%u", QUERY_SEPARATOR_CHAR, questionParameter_ );
			strcat( commonVariables()->queryString, tempString );
			}
/*
		if( lastCheckedAssumptionLevelItemNr_ > NO_ITEM_NR )
			{
			sprintf( tempString, "%clastCheckedAssumptionLevelItemNr:%u", QUERY_SEPARATOR_CHAR, lastCheckedAssumptionLevelItemNr_ );
			strcat( commonVariables()->queryString, tempString );
			}
*/
		if( generalizationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cgeneralizationCollectionNr:%u", QUERY_SEPARATOR_CHAR, generalizationCollectionNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( generalizationWordTypeString == NULL )
			sprintf( tempString, "%cgeneralizationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );
		else
			sprintf( tempString, "%cgeneralizationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, generalizationWordTypeString, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( generalizationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cgeneralizationContextNr:%u", QUERY_SEPARATOR_CHAR, generalizationContextNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cspecificationCollectionNr:%u", QUERY_SEPARATOR_CHAR, specificationCollectionNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationWordTypeString == NULL )
			sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
		else
			sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( specificationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cspecificationContextNr:%u", QUERY_SEPARATOR_CHAR, specificationContextNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationWordItem_ != NULL )
			{
			sprintf( tempString, "%cspecificationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, specificationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, specificationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );

			if( ( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( commonVariables()->queryString, tempString );
				}
			}

		if( relationWordTypeString != NULL )
			{
			sprintf( tempString, "%crelationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, relationWordTypeString, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( relationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%crelationContextNr:%u", QUERY_SEPARATOR_CHAR, relationContextNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( nContextRelations_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cnContextRelations:%u", QUERY_SEPARATOR_CHAR, nContextRelations_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( firstJustificationItem_ != NULL )
			{
			sprintf( tempString, "%cfirstJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, firstJustificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, firstJustificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( replacingSpecificationItem != NULL )
			{
			sprintf( tempString, "%creplacingSpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, replacingSpecificationItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, replacingSpecificationItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationString_ != NULL )
			{
			sprintf( tempString, "%cspecificationString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, specificationString_, QUERY_STRING_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		return commonVariables()->queryString;
		}


	// Protected assignment functions

	bool isAssignment()
		{
		return ( myList()->isAssignmentList() );
		}

	bool isActiveAssignment()
		{
		return ( isAssignment() &&
				isActiveItem() );
		}

	bool isInactiveAssignment()
		{
		return ( isAssignment() &&
				isInactiveItem() );
		}

	bool isArchivedAssignment()
		{
		return ( isAssignment() &&
				isArchivedItem() );
		}

	bool isGeneralizationAssignment()
		{
		return isGeneralizationAssignment_;
		}

	unsigned short assignmentLevel()
		{
		return assignmentLevel_;
		}

	unsigned short assumptionLevel()
		{
		return assumptionLevel_;
		}

	unsigned short grammarLanguageNr()
		{
		return grammarLanguageNr_;
		}

	unsigned int nRelationContextWords()
		{
		return myWordItem()->nContextWordsInAllWords( relationContextNr_, specificationWordItem_ );
		}

	SpecificationItem *getAssignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem )
		{
		SpecificationItem *searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == commonVariables()->currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != NULL )
			{
			// Skip replaced assignments
			if( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem->isAnsweredQuestion_ )
				return searchItem;

			searchItem = searchItem->nextAssignmentItemWithCurrentLevel();
			}

		return NULL;
		}

	SpecificationItem *getAssignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		SpecificationItem *searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == commonVariables()->currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != NULL )
			{
			if( searchItem->isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem->grammarLanguageNr_ == currentGrammarLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem->isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem->nextAssignmentItemWithCurrentLevel();
			}

		return NULL;
		}

	SpecificationItem *getAssignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, unsigned short questionParameter )
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		SpecificationItem *searchItem = ( isIncludingThisItem ? ( assignmentLevel_ == commonVariables()->currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithCurrentLevel() );

		while( searchItem != NULL )
			{
			if( searchItem->questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem->grammarLanguageNr_ == currentGrammarLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem->isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem->nextAssignmentItemWithCurrentLevel();
			}

		return NULL;
		}

	SpecificationItem *nextAssignmentItemWithCurrentLevel()
		{
		SpecificationItem *nextAssignmentItem = (SpecificationItem *)nextItem;

		if( nextAssignmentItem != NULL &&
		nextAssignmentItem->assignmentLevel() == commonVariables()->currentAssignmentLevel )
			return nextAssignmentItem;

		return NULL;
		}

	SpecificationItem *nextNonQuestionAssignmentItem()
		{
		return getAssignmentItem( false, false, false );
		}


	// Protected question functions

	bool isAnsweredQuestion()
		{
		return isAnsweredQuestion_;
		}

	bool isQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER );
		}

	bool isUserQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == NULL );
		}

	unsigned short questionParameter()
		{
		return questionParameter_;
		}

	SpecificationItem *firstNewUserQuestion()
		{
		return newUserQuestion( true );
		}

	SpecificationItem *nextNewUserQuestion()
		{
		return newUserQuestion( false );
		}


	// Protected specification functions

	void clearLastCheckedAssumptionLevelItemNr()
		{
		lastCheckedAssumptionLevelItemNr_ = NO_ITEM_NR;
		}

	void clearSpecificationStringWriteLevel( unsigned short currentWriteLevel )
		{
		if( specificationStringWriteLevel_ > currentWriteLevel )
			specificationStringWriteLevel_ = NO_WRITE_LEVEL;
		}

	void markAsGeneralizationAssignment()
		{
		isGeneralizationAssignment_ = true;
		strcpy( lastWrittenSentenceString, EMPTY_STRING );
		}

	bool hasAssumptionLevel()
		{
		return ( assumptionLevel_ > NO_ASSUMPTION_LEVEL );
		}

	bool hasNewInformation()
		{
		return ( !isOlderItem() ||
				hasCurrentInactiveSentenceNr() ||	// Has been inactivated during this sentence
				hasCurrentArchivedSentenceNr() ||	// Has been archived during this sentence

				( relationContextNr_ > NO_CONTEXT_NR &&
				specificationWordItem_ != NULL &&
				myWordItem()->hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr_, specificationWordItem_ ) ) );
		}

	bool hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasExclusiveGeneralizationCollection()
		{
		return ( isExclusiveSpecification_ &&
				generalizationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasExclusiveSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isExclusiveCollection( specificationCollectionNr_ ) );
		}

	bool hasFeminineOrMasculineSpecificationWord()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->isFeminineOrMasculineWord() );
		}

	bool hasFoundReplacedOrDeletedJustification()
		{
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem->isReplacedOrDeletedItem() )
				return true;

			searchItem = searchItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasFoundDeletedJustification()
		{
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem->isDeletedItem() )
				return true;

			searchItem = searchItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasFoundJustificationWithReplacedPrimarySpecification()
		{
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem->hasReplacedPrimarySpecification() )
				return true;

			searchItem = searchItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasFoundJustification( JustificationItem *searchJustificationItem )
		{
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem == searchJustificationItem )
				return true;

			searchItem = searchItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasFoundJustificationOfSameType( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		unsigned int searchPrimarySpecificationCollectionNr;
		unsigned int searchSecondarySpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr = ( primarySpecificationItem == NULL ? NO_COLLECTION_NR : primarySpecificationItem->specificationCollectionNr() );
		unsigned int secondarySpecificationCollectionNr = ( secondarySpecificationItem == NULL ? NO_COLLECTION_NR : secondarySpecificationItem->specificationCollectionNr() );
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem->justificationTypeNr() == justificationTypeNr )
				{
				searchPrimarySpecificationCollectionNr = ( searchItem->primarySpecificationItem() == NULL ? NO_COLLECTION_NR : searchItem->primarySpecificationItem()->specificationCollectionNr() );
				searchSecondarySpecificationCollectionNr = ( searchItem->secondarySpecificationItem() == NULL ? NO_COLLECTION_NR : searchItem->secondarySpecificationItem()->specificationCollectionNr() );

				if( searchPrimarySpecificationCollectionNr == primarySpecificationCollectionNr &&
				searchSecondarySpecificationCollectionNr == secondarySpecificationCollectionNr )
					return true;
				}

			searchItem = searchItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasGeneralizationContext()
		{
		return ( generalizationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasHiddenSpecificationWord()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->activeWordTypeString( specificationWordTypeNr_ ) == NULL );
		}

	bool hasPrepositionParameter()
		{
		return ( prepositionParameter_ > NO_PREPOSITION_PARAMETER );
		}

	bool hasRelationContext()
		{
		return ( relationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasSpecificationCompoundCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isCompoundCollection( specificationCollectionNr_ ) );
		}

	bool hasSpecificationContext()
		{
		return ( specificationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasUserSpecificationWordItem()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->isUserSpecificationWord );
		}

	bool isConcludedAssumption()
		{
		return isConcludedAssumption_;
		}

	bool isConditional()
		{
		return isConditional_;
		}

	bool isCorrectedAssumption()
		{
		return isCorrectedAssumption_;
		}

	bool isCorrectSpecificationArticle( bool isFeminineWord, bool isMasculineWord, unsigned short articleParameter )
		{
		bool hasFoundUnwrittenWord = false;
		bool isIncludingAnsweredQuestions = isAnsweredQuestion();
		bool _isSelfGenerated = isSelfGenerated();
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *foundWordItem = NULL;

		if( isSpecificationNoun() &&
		( currentSpecificationItem = myWordItem()->firstSelectedSpecificationItem( isIncludingAnsweredQuestions, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != NULL )
			{
			do	{
				if( currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, _isSelfGenerated, assumptionLevel_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) )
					{
					if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( !currentSpecificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
							{
							hasFoundUnwrittenWord = true;
							foundWordItem = currentSpecificationWordItem;
							}
						}
					}
				else	// No more specifications
					foundWordItem = specificationWordItem_;
				}
			while( !hasFoundUnwrittenWord &&
			( currentSpecificationItem = currentSpecificationItem->nextSpecificationItemWithSameQuestionParameter( isIncludingAnsweredQuestions ) ) != NULL );

			return ( foundWordItem == NULL ? true : ( isDefiniteArticleParameter( articleParameter ) ? foundWordItem->isCorrectDefiniteArticle( isFeminineWord, isMasculineWord, articleParameter, specificationWordTypeNr_ ) : foundWordItem->isCorrectIndefiniteArticle( isFeminineWord, isMasculineWord, articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	bool isCorrectRelationArticle( unsigned short definiteArticleParameter )
		{
		bool hasFoundUnwrittenWord = false;
		WordItem *currentRelationWordItem = NULL;	// Start to search first word in function contextWordInAllWords

		if( isRelationProperName() )
			{
			do	{
				if( ( currentRelationWordItem = relationWordItem( currentRelationWordItem ) ) != NULL )
					{
					if( !currentRelationWordItem->isRelationWordTypeAlreadyWritten( WORD_TYPE_PROPER_NAME ) )
						hasFoundUnwrittenWord = true;
					}
				}
			while( !hasFoundUnwrittenWord &&
			currentRelationWordItem != NULL );

			return ( currentRelationWordItem != NULL &&
					currentRelationWordItem->isProperNamePrecededByDefiniteArticle( definiteArticleParameter ) );
			}

		return false;
		}

	bool isEveryGeneralization()
		{
		return isEveryGeneralization_;
		}

	bool isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	bool isHiddenSpecification()
		{
		return ( isSelfGeneratedAssumption() &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isCollectedWithItself() );
		}

	bool isNegative()
		{
		return isNegative_;
		}

	bool isPartOf()
		{
		return isPartOf_;
		}

	bool isPossessive()
		{
		return isPossessive_;
		}

	bool isMatchingGeneralizationContextNr( bool isAllowingEmptyContextResult, unsigned int generalizationContextNr )
		{
		if( generalizationContextNr == NO_CONTEXT_NR ||			// Empty subset
		generalizationContextNr_ == generalizationContextNr ||	// Same set

		( isAllowingEmptyContextResult &&
		generalizationContextNr_ == NO_CONTEXT_NR ) )
			return true;

		return myWordItem()->isContextSubsetInAllWords( generalizationContextNr_, generalizationContextNr );
		}

	bool isMatchingSpecificationContextNr( bool isAllowingEmptyContextResult, unsigned int specificationContextNr )
		{
		if( specificationContextNr == NO_CONTEXT_NR ||			// Empty subset
		specificationContextNr_ == specificationContextNr ||	// Same set

		( isAllowingEmptyContextResult &&
		specificationContextNr_ == NO_CONTEXT_NR ) )
			return true;

		return myWordItem()->isContextSubsetInAllWords( specificationContextNr_, specificationContextNr );
		}

	bool isMatchingRelationContextNr( bool isAllowingEmptyContextResult, unsigned int relationContextNr )
		{
		if( relationContextNr == NO_CONTEXT_NR ||		// Empty subset
		relationContextNr_ == relationContextNr ||		// Same set

		( isAllowingEmptyContextResult &&
		relationContextNr_ == NO_CONTEXT_NR ) )
			return true;

		return myWordItem()->isContextSubsetInAllWords( relationContextNr_, relationContextNr );
		}

	bool isSelfGeneratedAssumption()
		{
		JustificationItem *searchItem = firstJustificationItem_;

		if( !isConcludedAssumption_ &&
		firstJustificationItem_ != NULL )
			{
			if( assumptionLevel_ > NO_ASSUMPTION_LEVEL &&
			questionParameter_ == NO_QUESTION_PARAMETER )
				return true;

			while( searchItem != NULL )
				{
				// At least one justification is an assumption
				if( searchItem->isAssumptionJustification() )
					return true;

				searchItem = searchItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool isSelfGeneratedConclusion()
		{
		JustificationItem *searchItem = firstJustificationItem_;

		if( isConcludedAssumption_ )
			return true;

		if( firstJustificationItem_ != NULL )
			{
			while( searchItem != NULL )
				{
				// At least one justification is an assumption. So, it is not a conclusion
				if( searchItem->isAssumptionJustification() )
					return false;

				searchItem = searchItem->attachedJustificationItem();
				}

			return true;
			}

		return false;
		}

	bool isSelfGeneratedQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER &&
				firstJustificationItem_ != NULL );
		}

	bool isSelfGenerated()
		{
		return ( firstJustificationItem_ != NULL );
		}

	bool isPossessiveReversibleConclusion()
		{
		return ( firstJustificationItem_ != NULL &&
				firstJustificationItem_->isPossessiveReversibleConclusion() );
		}

	bool isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	bool isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > NO_WRITE_LEVEL );
		}

	bool isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				!isAssignment() &&
				myWordItem()->nContextWordsInAllWords( relationContextNr_, specificationWordItem_ ) > 1 );
		}

	bool isUniqueRelation()
		{
		return isUniqueRelation_;
		}

	bool isUserSpecification()
		{
		return ( questionParameter_ == NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == NULL );
		}

	bool isValueSpecification()
		{
		return isValueSpecification_;
		}

	bool isGeneralizationNoun()
		{
		return isSingularOrPluralNoun( generalizationWordTypeNr_ );
		}

	bool isGeneralizationProperName()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_PROPER_NAME );
		}

	bool isGeneralizationPluralNoun()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isSpecificationNoun()
		{
		return isSingularOrPluralNoun( specificationWordTypeNr_ );
		}

	bool isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isRelationNoun()
		{
		return isSingularOrPluralNoun( relationWordTypeNr_ );
		}

	bool isRelationProperName()
		{
		return ( relationWordTypeNr_ == WORD_TYPE_PROPER_NAME );
		}

	bool isRelatedQuestion( bool isExclusiveSpecification, unsigned int specificationCollectionNr, unsigned int subsetRelationContextNr )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				specificationCollectionNr_ == specificationCollectionNr &&
				isMatchingRelationContextNr( true, subsetRelationContextNr ) );
		}

	bool isRelatedSpecification( bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				generalizationWordTypeNr_ == generalizationWordTypeNr );
		}

	bool isRelatedSpecification( bool isExclusiveSpecification, bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				relationContextNr_ > NO_CONTEXT_NR &&
				specificationWordItem != NULL &&
				specificationWordItem_ == specificationWordItem );
		}

	bool isRelatedSpecification( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				specificationWordItem != NULL &&
				specificationWordItem_ == specificationWordItem &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	bool isRelatedSpecification( bool isNegative, bool isPossessive, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int relationContextNr )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				hasSpecificationCollection() &&
				generalizationCollectionNr_ == generalizationCollectionNr &&
				specificationCollectionNr_ == specificationCollectionNr &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	bool isRelatedSpecification( bool isNegative, bool isPossessive, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int compoundSpecificationCollectionNr, WordItem *specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				generalizationCollectionNr_ == generalizationCollectionNr &&

				( specificationCollectionNr_ == specificationCollectionNr ||

				( compoundSpecificationCollectionNr > NO_COLLECTION_NR &&
				specificationCollectionNr_ == compoundSpecificationCollectionNr ) ) &&

				( hasSpecificationCollection() ||
				specificationWordItem_ == specificationWordItem ) );
		}

	bool isRelatedSpecification( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool _isSelfGenerated, unsigned short assumptionLevel, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr )
		{
		return ( hasSpecificationCollection() &&
				isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGenerated() == _isSelfGenerated &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				specificationContextNr_ == specificationContextNr &&
				relationContextNr_ == relationContextNr &&

				( assumptionLevel_ == assumptionLevel ||
				isQuestion() ) );
		}

	bool isRelatedSpecification( bool isCheckingRelationContext, bool isCheckingSelfGeneratedAssumption, bool isIgnoringExclusive, bool isIgnoringNegative, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool _isSelfGeneratedAssumption, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr )
		{
		return ( ( isIgnoringExclusive ||
				isExclusiveSpecification_ == isExclusiveSpecification ) &&

				( !isCheckingSelfGeneratedAssumption ||
				isSelfGeneratedAssumption() == _isSelfGeneratedAssumption ) &&

				( isIgnoringNegative ||
				isNegative_ == isNegative ) &&

				isPossessive_ == isPossessive &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				specificationContextNr_ == specificationContextNr &&

				( !isCheckingRelationContext ||
				relationContextNr_ == relationContextNr ) );
		}

	unsigned short prepositionParameter()
		{
		return prepositionParameter_;
		}

	unsigned short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	unsigned short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	unsigned short relationWordTypeNr()
		{
		return relationWordTypeNr_;
		}

	unsigned short highestAttachedJustificationOrderNr( unsigned short justificationTypeNr )
		{
		unsigned short highestOrderNr = NO_ORDER_NR;
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem->orderNr > highestOrderNr &&
			searchItem->justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchItem->orderNr;

			searchItem = searchItem->attachedJustificationItem();
			}

		return highestOrderNr;
		}

	unsigned int generalizationCollectionNr()
		{
		return generalizationCollectionNr_;
		}

	unsigned int specificationCollectionNr()
		{
		return specificationCollectionNr_;
		}

	unsigned int generalizationContextNr()
		{
		return generalizationContextNr_;
		}

	unsigned int specificationContextNr()
		{
		return specificationContextNr_;
		}

	unsigned int relationContextNr()
		{
		return relationContextNr_;
		}

	unsigned int nContextRelations()
		{
		return nContextRelations_;
		}

	unsigned int nInvolvedSpecificationWords( bool isCheckingForSameCreationSentenceNr, bool isCheckingForUserSpecificationWords )
		{
		bool _isSelfGenerated = isSelfGenerated();
		unsigned int _creationSentenceNr = creationSentenceNr();
		unsigned int nInvolvedSpecificationWords = 0;
		SpecificationItem *searchItem = myWordItem()->firstSelectedSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchItem != NULL )
			{
			if( searchItem == this ||

			( ( !isCheckingForSameCreationSentenceNr ||
			searchItem->creationSentenceNr() == _creationSentenceNr ) &&

			searchItem->isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, _isSelfGenerated, assumptionLevel_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) ) )
				{
				if( isCheckingForUserSpecificationWords &&
				!searchItem->hasUserSpecificationWordItem() )
					return 0;

				nInvolvedSpecificationWords++;
				}

			searchItem = searchItem->nextSpecificationItemWithSameQuestionParameter();
			}

		return nInvolvedSpecificationWords;
		}

	ResultType attachJustificationToSpecification( JustificationItem *attachJustificationItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustificationToSpecification";

		if( attachJustificationItem != NULL )
			{
			if( attachJustificationItem->isActiveItem() )
				{
				if( !isReplacedOrDeletedItem() )
					{
					if( ( firstJustificationItem = firstJustificationItem_ ) != NULL )
						{
						if( hasCurrentCreationSentenceNr() )
							{
							if( changeFirstJustification( attachJustificationItem ) == RESULT_OK )
								{
								if( !firstJustificationItem->isDeletedItem() )
									{
									if( attachJustificationItem->attachJustification( firstJustificationItem, this ) != RESULT_OK )
										return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach the first justification item to the given attached justification item" );
									}
								}
							else
								return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change my first justification item" );
							}
						else
							{
							if( ( specificationResult = myWordItem()->createSpecification( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isConcludedAssumption_, isConditional_, isCorrectedAssumption_, isEveryGeneralization_, isExclusiveSpecification_, isGeneralizationAssignment_, isNegative_, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, grammarLanguageNr_, prepositionParameter_, questionParameter_, generalizationWordTypeNr_, specificationWordTypeNr_, relationWordTypeNr_, generalizationCollectionNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, lastWrittenSentenceString ) ).result == RESULT_OK )
								{
								if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
									{
									if( attachJustificationItem->attachJustification( firstJustificationItem, createdSpecificationItem ) == RESULT_OK )
										{
										if( myWordItem()->replaceOrDeleteSpecification( this, createdSpecificationItem ) != RESULT_OK )
											return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace or delete a specification" );
										}
									else
										return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach the first justification item to the given attached justification item of the created specification item" );
									}
								else
									return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't create a specification" );
								}
							else
								return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to create a copy of myself with a different first justification item" );
							}
						}
					else
						return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I have no justification items" );
					}
				else
					return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I am replaced or deleted" );
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item isn't active" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item is undefined" );

		return RESULT_OK;
		}

	ResultType changeFirstJustification( JustificationItem *replacingJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeFirstJustification";

		if( replacingJustificationItem != NULL )
			{
			if( replacingJustificationItem->isActiveItem() )
				{
				if( hasCurrentCreationSentenceNr() )
					firstJustificationItem_ = replacingJustificationItem;
				else
					return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item isn't active" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item is undefined" );

		return RESULT_OK;
		}

	ResultType collectSpecification( bool isCollectGeneralization, bool isExclusiveSpecification, unsigned int collectionNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecification";

		if( collectionNr > NO_COLLECTION_NR )
			{
			if( hasCurrentCreationSentenceNr() )
				{
				if( isExclusiveSpecification )
					isExclusiveSpecification_ = true;

				if( isCollectGeneralization )
					{
					if( generalizationCollectionNr_ == NO_COLLECTION_NR )
						generalizationCollectionNr_ = collectionNr;
					else
						return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The generalization is already collected" );
					}
				else
					{
					if( specificationCollectionNr_ == NO_COLLECTION_NR )
						specificationCollectionNr_ = collectionNr;
					else
						return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The specification is already collected" );
					}
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given collection number is undefined" );

		return RESULT_OK;
		}

	ResultType markAsConcludedAssumption( bool isSkippingSelfGeneratedAssumptionCheck )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markAsConcludedAssumption";

		if( isSkippingSelfGeneratedAssumptionCheck ||
		isSelfGeneratedAssumption() )
			{
			isConcludedAssumption_ = true;
			assumptionLevel_ = NO_ASSUMPTION_LEVEL;
			strcpy( lastWrittenSentenceString, EMPTY_STRING );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I am not a self-generated assumption" );

		return RESULT_OK;
		}

	ResultType markSpecificationStringAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationStringAsWritten";

		if( commonVariables()->currentWriteLevel < MAX_LEVEL )
			{
			if( specificationStringWriteLevel_ == NO_WRITE_LEVEL )
				specificationStringWriteLevel_ = ++commonVariables()->currentWriteLevel;
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My write level is already assigned" );
			}
		else
			return startSystemErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "Current write word level overflow" );

		return RESULT_OK;
		}

	ResultType recalculateAssumption()
		{
		SpecificationResultType specificationResult;
		bool isAdjustedSpecification = false;
		unsigned short previousAssumptionLevel;
		char functionNameString[FUNCTION_NAME_LENGTH] = "recalculateAssumption";

		if( ( specificationResult = getAssumptionLevel() ).result == RESULT_OK )
			{
			previousAssumptionLevel = specificationResult.assumptionLevel;

			if( firstJustificationItem_ != NULL &&
			firstJustificationItem_->hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition() )
				{
				if( markAsConcludedAssumption( true ) == RESULT_OK )
					isAdjustedSpecification = true;
				else
					return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark myself as concluded assumption" );
				}
			else
				{
				if( !specificationResult.hasPerformedRecalculation &&

				// Avoid corrected assumption to be concluded when collected with itself
				( isCorrectedAssumption_ ||
				specificationCollectionNr_ == NO_COLLECTION_NR ||
				!myWordItem()->hasCorrectedAssumptionByKnowledge() ) )
					{
					if( ( specificationResult = recalculateAssumptionLevel() ).result == RESULT_OK )
						{
						if( isOlderItem() &&
						specificationResult.assumptionLevel != previousAssumptionLevel &&

						// Avoid updates of high uncertainty mapping,
						// from "may be" of one level to "may be" of another level
						( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
						specificationResult.assumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ) )
							{
							if( specificationResult.assumptionLevel == NO_ASSUMPTION_LEVEL )
								{
								if( markAsConcludedAssumption( true ) == RESULT_OK )
									{
									if( hasSpecificationCompoundCollection() )
										{
										if( ( specificationResult = myWordItem()->findRelatedSpecification( false, false, this ) ).result == RESULT_OK )
											{
											if( specificationResult.relatedSpecificationItem == NULL ||
											specificationResult.relatedSpecificationItem->isConcludedAssumption() )
												isAdjustedSpecification = true;
											}
										else
											return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find out if a search specification is related" );
										}
									else
										isAdjustedSpecification = true;
									}
								else
									return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "After recalculation, I failed to mark myself as concluded assumption" );
								}
							else
								{
								// Skip early anouncement of adjusted assumption
								if( specificationResult.assumptionLevel < previousAssumptionLevel ||
								relationContextNr_ == NO_CONTEXT_NR ||
								myWordItem()->nContextWordsInAllWords( relationContextNr_, specificationWordItem_ ) == commonVariables_->nUserRelationWords )
									isAdjustedSpecification = true;
								}
							}
						}
					else
						return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to recalculate the assumption level" );
					}
				}

			if( isAdjustedSpecification )
				{
				// Write adjusted specification
				if( myWordItem()->writeUpdatedSpecification( true, false, false, this ) != RESULT_OK )
					return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an adjusted specification" );
				}
			}
		else
			return addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to get the assumption level" );

		return RESULT_OK;
		}

	SpecificationResultType getAssumptionLevel()
		{
		return calculateAssumptionLevel( false );
		}

	SpecificationResultType recalculateAssumptionLevel()
		{
		nAssumptionLevelRecalculations_ = 0;
		return calculateAssumptionLevel( true );
		}

	JustificationItem *firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	JustificationItem *oppositePossessiveConditionalSpecificationAssumptionJustificationItem( SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchItem = firstJustificationItem_;

		if( secondarySpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isOppositePossessiveConditionalSpecificationAssumption() &&
				searchItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *olderGeneralizationAssumptionBySpecificationJustificationItem()
		{
		JustificationItem *searchItem = firstJustificationItem_;

		while( searchItem != NULL )
			{
			if( searchItem->isOlderItem() &&
			searchItem->isGeneralizationAssumption() )
				return searchItem;

			searchItem = searchItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *primarySpecificationJustificationItem( SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchItem = firstJustificationItem_;

		if( primarySpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->primarySpecificationItem() == primarySpecificationItem )
					return searchItem;

				searchItem = searchItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *nextSpecificationItem()
		{
		return (SpecificationItem *)nextItem ;
		}

	SpecificationItem *nextAssignmentOrSpecificationItem()
		{
		return ( isAssignment() ? nextAssignmentItemWithCurrentLevel() : nextSpecificationItem() );
		}

	SpecificationItem *updatedSpecificationItem()
		{
		SpecificationItem *updatedSpecificationItem;
		SpecificationItem *searchItem = this;

		while( ( updatedSpecificationItem = searchItem->replacingSpecificationItem ) != NULL )
			searchItem = updatedSpecificationItem;

		return searchItem;
		}

	SpecificationItem *getSpecificationItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		SpecificationItem *searchItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchItem != NULL )
			{
			if( searchItem->isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem->grammarLanguageNr_ == currentGrammarLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem->isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *getSpecificationItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, unsigned short questionParameter )
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		SpecificationItem *searchItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchItem != NULL )
			{
			if( searchItem->questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchItem->grammarLanguageNr_ == currentGrammarLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchItem->isAnsweredQuestion_ ) )
				return searchItem;

			searchItem = searchItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, isQuestion() ) : getSpecificationItem( false, false, isQuestion() ) );
		}

	SpecificationItem *nextNonQuestionSpecificationItem()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, false ) : getSpecificationItem( false, false, false ) );
		}

	SpecificationItem *nextSpecificationItemWithSameQuestionParameter()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, questionParameter_ ) : getSpecificationItem( false, false, questionParameter_ ) );
		}

	SpecificationItem *nextSpecificationItemWithSameQuestionParameter( bool isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? getAssignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) : getSpecificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	SpecificationItem *recentlyAnsweredQuestionSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchItem = ( isAssignment() ? getAssignmentItem( true, isIncludingThisItem, questionParameter_ ) : getSpecificationItem( true, isIncludingThisItem, questionParameter_ ) );

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			searchItem->isAnsweredQuestion_ &&
			searchItem->isSelfGenerated() )
				return searchItem;

			searchItem = searchItem->nextSpecificationItemWithSameQuestionParameter( true );
			}

		return NULL;
		}

	WordItem *collectedWithItselfCommonWordItem()
		{
		WordItem *commonWordItem;

		if( specificationWordItem_ != NULL &&
		( commonWordItem = specificationWordItem_->commonWordItem( specificationCollectionNr_ ) ) != NULL )
			{
			if( commonWordItem->isCollectedWithItself() )
				return commonWordItem;
			}

		return NULL;
		}

	WordItem *generalizationWordItem()
		{
		return myWordItem();
		}

	WordItem *specificationWordItem()
		{
		return specificationWordItem_;
		}

	WordItem *relationWordItem()
		{
		return relationWordItem( NULL );
		}

	WordItem *relationWordItem( WordItem *previousWordItem )
		{
		return myWordItem()->contextWordItemInAllWords( relationContextNr_, specificationWordItem_, previousWordItem );
		}

	char *specificationString()
		{
		return specificationString_;
		}

	char *activeGeneralizationWordTypeString()
		{
		return myWordItem()->activeWordTypeString( generalizationWordTypeNr_ );
		}

	char *activeSpecificationWordTypeString()
		{
		return ( specificationWordItem_ == NULL ? NULL : specificationWordItem_->activeWordTypeString( specificationWordTypeNr_ ) );
		}
	};
#endif

/*************************************************************************
 *
 *	"All he does is just and good,
 *	and all his commandments are trustworthy." (Psalm 111:7)
 *
 *************************************************************************/
