/*	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
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

#ifndef SPECIFICATIONITEM
#define SPECIFICATIONITEM 1
#include "List.h"
#include "Presentation.cpp"
#include "SpecificationResultType.cpp"

class SpecificationItem : private Item
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminCollection;
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminLanguage;
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
	friend class WordSelectionCondition;
	friend class WordSpecification;
	friend class WordWrite;
	friend class WordWriteSentence;
	friend class WordWriteWords;

	// Private constructed variables

	unsigned short nAssumptionLevelRecalculations_;

	// Private initialized variables

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
	bool isUniqueUserRelation_;
	bool isValueSpecification_;

	unsigned short assignmentLevel_;
	unsigned short assumptionLevel_;
	unsigned short languageNr_;
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


	// Private constructed variables

	unsigned short specificationStringWriteLevel_;

	unsigned int lastCheckedAssumptionLevelItemNr_;


	// Private question functions

	SpecificationItem *newUserQuestion( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSelectedSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isAnsweredQuestion_ &&
			!searchSpecificationItem->isOlderItem() &&
			searchSpecificationItem->isUserQuestion() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedSpecificationItem();
			}

		return searchSpecificationItem;
		}


	// Private specification functions

	bool hasContextMaleWordInAllWords( unsigned int contextNr, WordItem *specificationWordItem )
		{
		WordItem *currentContextWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = commonVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->isMale() &&
				currentContextWordItem->hasContextInWord( contextNr, specificationWordItem ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return false;
		}

	SpecificationResultType calculateAssumptionLevel( bool isForcingRecalculation )
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
			if( ++nAssumptionLevelRecalculations_ >= MAX_ASSUMPTION_LEVEL_RECALCULATIONS )
				return startSpecificationResultError( functionNameString, NULL, "There is probably an endless loop in the assumption level calculation of my specification, because the number of iterations is: ", nAssumptionLevelRecalculations_ );

			if( isForcingRecalculation ||
			assumptionLevel_ == NO_ASSUMPTION_LEVEL )
				{
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
								if( ( specificationResult = currentJustificationItem->getCombinedAssumptionLevel() ).result != RESULT_OK )
									return addSpecificationResultError( functionNameString, NULL, "I failed to get the combined assumption level" );

								if( ( tempAssumptionLevel = ( specificationResult.combinedAssumptionLevel + currentJustificationItem->justificationAssumptionGrade() ) ) > highestAssumptionLevel )
									{
									if( tempAssumptionLevel >= MAX_LEVEL )
										return startSpecificationResultSystemError( functionNameString, NULL, "Assumption level overflow" );

									highestAssumptionLevel = (unsigned short)tempAssumptionLevel;
									}
								}
							}
						while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );
						}

					if( highestAssumptionLevel < lowestAssumptionLevel &&

					( nJustificationRelationWords == nSpecificationRelationWords ||

					( assumptionLevel_ == NO_ASSUMPTION_LEVEL &&
					// To avoid looping
					lastCheckedAssumptionLevelItemNr_ == commonVariables()->currentItemNr ) ) )
						lowestAssumptionLevel = highestAssumptionLevel;
					}
				while( ( nextJustificationItem = nextJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != NULL );

				if( lowestAssumptionLevel < MAX_LEVEL &&
				assumptionLevel_ != lowestAssumptionLevel )
					{
					assumptionLevel_ = lowestAssumptionLevel;

					if( assumptionLevel_ > NO_ASSUMPTION_LEVEL &&
					isOlderItem() )
						// Clear sentence write buffer if assumption level is changed
						strcpy( lastWrittenSentenceString, EMPTY_STRING );
					}

				lastCheckedAssumptionLevelItemNr_ = commonVariables()->currentItemNr;
				}
			}

		specificationResult.assumptionLevel = assumptionLevel_;
		return specificationResult;
		}


	protected:
	// Protected constructed variables

	bool hasSpecificationBeenWrittenAsAnswer;
	bool hasSpecificationBeenWrittenAsConflict;

	SpecificationItem *replacingSpecificationItem;

	char lastWrittenSentenceString[MAX_SENTENCE_STRING_LENGTH];
	char lastWrittenSentenceWithOneSpecificationOnlyString[MAX_SENTENCE_STRING_LENGTH];


	// Constructor

	SpecificationItem( bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isLanguageWord, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		size_t specificationStringLength;

		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, "SpecificationItem", commonVariables, myList, myWordItem );

		// Private constructed variables

		nAssumptionLevelRecalculations_ = 0;

		// Private initialized variables

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
		isUniqueUserRelation_ = isUniqueUserRelation;
		isValueSpecification_ = isValueSpecification;

		assignmentLevel_ = assignmentLevel;
		assumptionLevel_ = assumptionLevel;
		languageNr_ = languageNr;
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
					startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create a specification string" );
				}
			else
				startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given specification string is too long" );
			}


		// Private constructed variables

		specificationStringWriteLevel_ = NO_WRITE_LEVEL;
		lastCheckedAssumptionLevelItemNr_ = NO_ITEM_NR;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;
		hasSpecificationBeenWrittenAsConflict = false;

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

	virtual void clearReplacingInfo()
		{
		clearReplacedSentenceNr();
		replacingSpecificationItem = NULL;
		}

	virtual void selectingAttachedJustifications( bool isSelectingJustificationSpecifications )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			searchJustificationItem->isSelectedByQuery = true;

			if( isSelectingJustificationSpecifications )
				searchJustificationItem->selectingJustificationSpecifications();

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}
		}

	virtual void displayString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( specificationString_ != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, specificationString_ );
			}
		}

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;

		statusString[0] = statusChar();

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
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

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
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

	virtual bool hasWordType( unsigned short queryWordTypeNr )
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
				creationSentenceNr() > nextSortSpecificationItem->creationSentenceNr() ) ) );
		}

	virtual ResultType checkForUsage()
		{
		return myWordItem()->checkSpecificationForUsageOfInvolvedWords( this );
		}

	virtual StringResultType findMatchingWordReferenceString( char *queryString )
		{
		StringResultType stringResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( specificationWordItem_ != NULL )
			{
			if( ( stringResult = specificationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				return addStringResultError( functionNameString, NULL, "I failed to find a matching word reference string for the specification word" );
			}

		return stringResult;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *wordString;
		char *languageNameString = myWordItem()->languageNameString( languageNr_ );
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		char *relationWordTypeString = ( relationWordTypeNr_ == NO_WORD_TYPE_NR ? NULL : myWordItem()->wordTypeNameString( relationWordTypeNr_ ) );

		Item::toString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( languageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%clanguageNr:%u", QUERY_SEPARATOR_CHAR, languageNr_ );
			else
				sprintf( tempString, "%clanguage:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( queryString, tempString );
			}

		if( isAnsweredQuestion_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAnsweredQuestion" );
			}

		if( isConcludedAssumption_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isConcludedAssumption" );
			}

		if( isConditional_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isConditional" );
			}

		if( isCorrectedAssumption_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isCorrectedAssumption" );
			}

		if( isEveryGeneralization_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isEveryGeneralization" );
			}

		if( isExclusiveSpecification_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isExclusiveSpecification" );
			}

		if( isGeneralizationAssignment_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isGeneralizationAssignment" );
			}

		if( isLanguageWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isLanguageWord" );
			}

		if( isNegative_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNegative" );
			}

		if( isPartOf_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isPartOf" );
			}

		if( isPossessive_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isPossessive" );
			}

		if( isSpecificationGeneralization_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSpecificationGeneralization" );
			}

		if( isUniqueUserRelation_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isUniqueUserRelation" );
			}

		if( isValueSpecification_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isValueSpecification" );
			}

		if( assignmentLevel_ > NO_ASSIGNMENT_LEVEL )
			{
			sprintf( tempString, "%cassignmentLevel:%u", QUERY_SEPARATOR_CHAR, assignmentLevel_ );
			strcat( queryString, tempString );
			}

		if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
			{
			sprintf( tempString, "%cassumptionLevel:%u", QUERY_SEPARATOR_CHAR, assumptionLevel_ );
			strcat( queryString, tempString );
			}

		if( specificationStringWriteLevel_ > NO_WRITE_LEVEL )
			{
			sprintf( tempString, "%cspecificationStringWriteLevel:%u", QUERY_SEPARATOR_CHAR, specificationStringWriteLevel_ );
			strcat( queryString, tempString );
			}

		if( prepositionParameter_ > NO_PREPOSITION_PARAMETER )
			{
			sprintf( tempString, "%cprepositionParameter:%u", QUERY_SEPARATOR_CHAR, prepositionParameter_ );
			strcat( queryString, tempString );
			}

		if( questionParameter_ > NO_QUESTION_PARAMETER )
			{
			sprintf( tempString, "%cquestionParameter:%u", QUERY_SEPARATOR_CHAR, questionParameter_ );
			strcat( queryString, tempString );
			}
/*
		if( lastCheckedAssumptionLevelItemNr_ > NO_ITEM_NR )
			{
			sprintf( tempString, "%clastCheckedAssumptionLevelItemNr:%u", QUERY_SEPARATOR_CHAR, lastCheckedAssumptionLevelItemNr_ );
			strcat( queryString, tempString );
			}
*/
		if( generalizationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cgeneralizationCollectionNr:%u", QUERY_SEPARATOR_CHAR, generalizationCollectionNr_ );
			strcat( queryString, tempString );
			}

		if( generalizationWordTypeString == NULL )
			sprintf( tempString, "%cgeneralizationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );
		else
			sprintf( tempString, "%cgeneralizationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, generalizationWordTypeString, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );

		strcat( queryString, tempString );

		if( generalizationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cgeneralizationContextNr:%u", QUERY_SEPARATOR_CHAR, generalizationContextNr_ );
			strcat( queryString, tempString );
			}

		if( specificationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cspecificationCollectionNr:%u", QUERY_SEPARATOR_CHAR, specificationCollectionNr_ );
			strcat( queryString, tempString );
			}

		if( specificationWordTypeString == NULL )
			sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
		else
			sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

		strcat( queryString, tempString );

		if( specificationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cspecificationContextNr:%u", QUERY_SEPARATOR_CHAR, specificationContextNr_ );
			strcat( queryString, tempString );
			}

		if( specificationWordItem_ != NULL )
			{
			sprintf( tempString, "%cspecificationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, specificationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, specificationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( relationWordTypeString != NULL )
			{
			sprintf( tempString, "%crelationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, relationWordTypeString, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );
			strcat( queryString, tempString );
			}

		if( relationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%crelationContextNr:%u", QUERY_SEPARATOR_CHAR, relationContextNr_ );
			strcat( queryString, tempString );
			}

		if( nContextRelations_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cnContextRelations:%u", QUERY_SEPARATOR_CHAR, nContextRelations_ );
			strcat( queryString, tempString );
			}

		if( firstJustificationItem_ != NULL )
			{
			sprintf( tempString, "%cfirstJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, firstJustificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, firstJustificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( replacingSpecificationItem != NULL )
			{
			sprintf( tempString, "%creplacingSpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, replacingSpecificationItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, replacingSpecificationItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( specificationString_ != NULL )
			{
			sprintf( tempString, "%cspecificationString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, specificationString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( strlen( lastWrittenSentenceString ) > 0 )
			{
			sprintf( tempString, "%clastWrittenSentenceString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, lastWrittenSentenceString, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( strlen( lastWrittenSentenceWithOneSpecificationOnlyString ) > 0 )
			{
			sprintf( tempString, "%clastWrittenSentenceWithOneSpecificationOnlyString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, lastWrittenSentenceWithOneSpecificationOnlyString, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected assignment functions

	bool isAssignment()
		{
		return ( myList()->isAssignmentList() );
		}

	bool isUserAssignment()
		{
		return ( isAssignment() &&
				isUserSpecification() );
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

	unsigned short languageNr()
		{
		return languageNr_;
		}

	unsigned int nRelationContextWords()
		{
		return myWordItem()->nContextWordsInAllWords( relationContextNr_, specificationWordItem_ );
		}

	SpecificationItem *getAssignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == commonVariables()->currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithCurrentLevel() );

		while( searchSpecificationItem != NULL )
			{
			// Skip replaced assignments
			if( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithCurrentLevel();
			}

		return NULL;
		}

	SpecificationItem *getAssignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == commonVariables()->currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithCurrentLevel() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isQuestion() == isQuestion &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithCurrentLevel();
			}

		return NULL;
		}

	SpecificationItem *getAssignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, unsigned short questionParameter )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == commonVariables()->currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithCurrentLevel() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->questionParameter_ == questionParameter &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithCurrentLevel();
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

	void initializeSpecificationVariables()
		{
		// Private constructed variables

		lastCheckedAssumptionLevelItemNr_ = NO_ITEM_NR;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;
		hasSpecificationBeenWrittenAsConflict = false;
		}

	void markAsGeneralizationAssignment()
		{
		isGeneralizationAssignment_ = true;

		// Clear sentence write buffer
		strcpy( lastWrittenSentenceString, EMPTY_STRING );
		}

	bool hasAnsweredQuestionInJustification()
		{
		SpecificationItem *secondarySpecificationItem;
		WordItem *secondarySpecificationWordItem;

		if( firstJustificationItem_ != NULL &&
		firstJustificationItem_->attachedJustificationItem() == NULL &&
		firstJustificationItem_->isNegativeAssumptionOrConclusion() &&
		( secondarySpecificationItem = firstJustificationItem_->secondarySpecificationItem() ) != NULL )
			{
			if( secondarySpecificationItem->isAnsweredQuestion_ &&
			( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
				{
				if( !secondarySpecificationWordItem->isUserSpecificationWord )
					return true;
				}
			}

		return false;
		}

	bool hasAssumptionLevel()
		{
		return ( assumptionLevel_ > NO_ASSUMPTION_LEVEL );
		}

	bool hasDeletedJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isDeletedItem() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasExclusiveGeneralizationCollection()
		{
		return ( isExclusiveSpecification_ &&
				generalizationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasJustification( bool isIncludingReplacingJustification, JustificationItem *referenceJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( referenceJustificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem == referenceJustificationItem ||

				( isIncludingReplacingJustification &&
				searchJustificationItem->replacingJustificationItem == referenceJustificationItem ) )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool hasJustificationOfSameType( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		unsigned int searchPrimarySpecificationCollectionNr;
		unsigned int searchSecondarySpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr = ( primarySpecificationItem == NULL ? NO_COLLECTION_NR : primarySpecificationItem->specificationCollectionNr() );
		unsigned int secondarySpecificationCollectionNr = ( secondarySpecificationItem == NULL ? NO_COLLECTION_NR : secondarySpecificationItem->specificationCollectionNr() );
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		SpecificationItem *searchPrimarySpecificationItem;
		SpecificationItem *searchSecondarySpecificationItem;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				{
				searchPrimarySpecificationItem = searchJustificationItem->primarySpecificationItem();
				searchSecondarySpecificationItem = searchJustificationItem->secondarySpecificationItem();

				searchPrimarySpecificationCollectionNr = ( searchPrimarySpecificationItem == NULL ? NO_COLLECTION_NR : searchPrimarySpecificationItem->specificationCollectionNr() );
				searchSecondarySpecificationCollectionNr = ( searchSecondarySpecificationItem == NULL ? NO_COLLECTION_NR : searchSecondarySpecificationItem->specificationCollectionNr() );

				if( searchPrimarySpecificationCollectionNr == primarySpecificationCollectionNr &&
				searchSecondarySpecificationCollectionNr == secondarySpecificationCollectionNr )
					return true;
				}

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasNewInformation()
		{
		return ( !isOlderItem() ||
				// Has been inactivated during this sentence
				hasCurrentInactiveSentenceNr() ||
				// Has been archived during this sentence
				hasCurrentArchivedSentenceNr() ||

				( relationContextNr_ > NO_CONTEXT_NR &&
				specificationWordItem_ != NULL &&
				myWordItem()->hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr_, specificationWordItem_ ) ) );
		}

	bool hasOnlyOneRelationWord()
		{
		return ( myWordItem()->nContextWordsInAllWords( relationContextNr_, specificationWordItem_ ) == 1 );
		}

	bool hasPrepositionParameter()
		{
		return ( prepositionParameter_ > NO_PREPOSITION_PARAMETER );
		}

	bool hasPrimarySpecificationJustification( SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool hasRelationContext()
		{
		return ( relationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasReplacedOrDeletedJustification( bool isAllowingNewerReplaceOrDeleteSentenceNr )
		{
		unsigned int currentSentenceNr = commonVariables()->currentSentenceNr;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isReplacedOrDeletedItem() &&

			( !isAllowingNewerReplaceOrDeleteSentenceNr ||

			( searchJustificationItem->replacedSentenceNr() < currentSentenceNr &&
			searchJustificationItem->deletedSentenceNr() < currentSentenceNr ) ) )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
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

	bool hasSpecificationNonCompoundCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isNonCompoundCollection( specificationCollectionNr_ ) );
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

	bool isCorrectSpecificationArticle( unsigned short articleParameter )
		{
		bool isIncludingAnsweredQuestions = isAnsweredQuestion();
		bool isSelfGeneratedSpecification = isSelfGenerated();
		SpecificationItem *searchSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *foundWordItem = NULL;

		if( specificationWordItem_ != NULL &&
		isSpecificationNoun() &&
		( searchSpecificationItem = myWordItem()->firstSelectedSpecificationItem( isIncludingAnsweredQuestions, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != NULL )
			{
			do	{
				currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ : searchSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) );

				if( currentSpecificationWordItem != NULL &&
				!currentSpecificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
					foundWordItem = currentSpecificationWordItem;
				}
			while( foundWordItem == NULL &&
			( searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isIncludingAnsweredQuestions ) ) != NULL );

			return ( foundWordItem == NULL ? true : ( isDefiniteArticleParameter( articleParameter ) ? foundWordItem->isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem->isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	bool isCorrectArticleOfFirstUnwrittenProperNameRelationWord( unsigned short definiteArticleParameter )
		{
		WordItem *firstUnwrittenRelationWordItem = unwrittenProperNameRelationWordItem( NULL );

		return ( firstUnwrittenRelationWordItem != NULL &&
				firstUnwrittenRelationWordItem->isProperNamePrecededByDefiniteArticle( definiteArticleParameter ) );
		}

	bool isEveryGeneralization()
		{
		return isEveryGeneralization_;
		}

	bool isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	bool isGeneralizationNoun()
		{
		return isNounWordType( generalizationWordTypeNr_ );
		}

	bool isGeneralizationProperName()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_PROPER_NAME );
		}

	bool isHiddenSpanishSpecification()
		{
		return ( isSpecificationWordSpanishAmbiguous() &&
				hasSpecificationNonCompoundCollection() &&
				!isNegative_ &&
				isSelfGenerated() &&
				!isQuestion() &&
				isGeneralizationProperName() &&

				// Not possessive specification not in a male word
				( ( !isPossessive_ &&

				( isCorrectedAssumption_ ||
				!myWordItem()->isMale() ) ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				hasRelationContext() &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->compoundCollectionNr( specificationWordTypeNr_ ) > NO_COLLECTION_NR &&
				!hasContextMaleWordInAllWords( relationContextNr_, specificationWordItem_ ) ) ) );
		}

	bool isMatchingGeneralizationContextNr( bool isAllowingEmptyContextResult, unsigned int generalizationContextNr )
		{
		// Empty subset
		if( generalizationContextNr == NO_CONTEXT_NR ||
		// Same set
		generalizationContextNr_ == generalizationContextNr ||

		( isAllowingEmptyContextResult &&
		generalizationContextNr_ == NO_CONTEXT_NR ) )
			return true;

		return myWordItem()->isContextSubsetInAllWords( generalizationContextNr_, generalizationContextNr );
		}

	bool isMatchingRelationContextNr( bool isAllowingEmptyContextResult, unsigned int relationContextNr )
		{
		// Empty subset
		if( relationContextNr == NO_CONTEXT_NR ||
		// Same set
		relationContextNr_ == relationContextNr ||

		( isAllowingEmptyContextResult &&
		relationContextNr_ == NO_CONTEXT_NR ) )
			return true;

		return myWordItem()->isContextSubsetInAllWords( relationContextNr_, relationContextNr );
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

	bool isPossessiveReversibleConclusion()
		{
		return ( firstJustificationItem_ != NULL &&
				firstJustificationItem_->isPossessiveReversibleConclusion() );
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

				( ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr ) ||

				( compoundSpecificationCollectionNr > NO_COLLECTION_NR &&

				( specificationCollectionNr_ == compoundSpecificationCollectionNr ||
				specificationWordItem_ == specificationWordItem ) ) ) );
		}

	bool isRelatedSpecification( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr )
		{
		return ( relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGeneratedSpecification, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) != NULL );
		}

	bool isSelfGeneratedAssumption()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( !isConcludedAssumption_ &&
		questionParameter_ == NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != NULL )
			{
			if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
				return true;

			while( searchJustificationItem != NULL )
				{
				// At least one justification is an assumption
				if( searchJustificationItem->isAssumptionJustification() )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool isSelfGeneratedConclusion()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( questionParameter_ == NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != NULL )
			{
			if( isConcludedAssumption_ )
				return true;

			while( searchJustificationItem != NULL )
				{
				// At least one justification is an assumption. So, it is not a conclusion
				if( searchJustificationItem->isAssumptionJustification() )
					return false;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
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

	bool isAnsweredSelfGeneratedQuestion()
		{
		return ( isAnsweredQuestion_ &&
				firstJustificationItem_ != NULL );
		}

	bool isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	bool isSpecificationNoun()
		{
		return isNounWordType( specificationWordTypeNr_ );
		}

	bool isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isSpecificationPluralNoun()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isSpecificationSingularNoun()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
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

	bool isSpecificationWordSpanishAmbiguous()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->isNounWordSpanishAmbiguous() );
		}

	bool isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
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

	bool wasHiddenSpanishSpecification()
		{
		return ( strlen( lastWrittenSentenceString ) == 0 &&
				isSpecificationWordSpanishAmbiguous() &&
				hasRelationContext() &&
				hasSpecificationNonCompoundCollection() );
		}

	unsigned short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	unsigned short highestAttachedJustificationOrderNr( unsigned short justificationTypeNr )
		{
		unsigned short highestOrderNr = NO_ORDER_NR;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->orderNr > highestOrderNr &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem->orderNr;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return highestOrderNr;
		}

	unsigned short prepositionParameter()
		{
		return prepositionParameter_;
		}

	unsigned short relationWordTypeNr()
		{
		return relationWordTypeNr_;
		}

	unsigned short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
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

	unsigned int nContextRelations()
		{
		return nContextRelations_;
		}

	unsigned int nInvolvedSpecificationWords( bool isCheckingForUserSpecificationWords )
		{
		bool isSelfGeneratedSpecification = isSelfGenerated();
		unsigned int _creationSentenceNr = creationSentenceNr();
		unsigned int nInvolvedSpecificationWords = 0;
		SpecificationItem *searchSpecificationItem = myWordItem()->firstSelectedSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem == this ||

			( searchSpecificationItem->creationSentenceNr() == _creationSentenceNr &&
			searchSpecificationItem->isRelatedSpecification( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_ ) ) )
				{
				if( isCheckingForUserSpecificationWords &&
				!searchSpecificationItem->hasUserSpecificationWordItem() )
					return 0;

				nInvolvedSpecificationWords++;
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return nInvolvedSpecificationWords;
		}

	unsigned int specificationContextNr()
		{
		return specificationContextNr_;
		}

	unsigned int relationContextNr()
		{
		return relationContextNr_;
		}

	ResultType attachJustificationToSpecification( JustificationItem *attachJustificationItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustificationToSpecification";

		if( attachJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given attached justification item is undefined" );

		if( !attachJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given attached justification item isn't active" );

		if( isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "I am replaced or deleted" );

		if( ( firstJustificationItem = firstJustificationItem_ ) == NULL )
			return startError( functionNameString, NULL, "I have no justification items" );

		if( hasCurrentCreationSentenceNr() )
			{
			if( changeFirstJustification( attachJustificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to change my first justification item" );

			if( !myWordItem()->hasCorrectedAssumptionByKnowledge() )
				{
				if( attachJustificationItem->attachJustification( firstJustificationItem, this ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to attach the first justification item of myself to the given attached justification item" );
				}
			}
		else
			{
			if( ( specificationResult = myWordItem()->createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isConcludedAssumption_, isConditional_, isCorrectedAssumption_, isEveryGeneralization_, isExclusiveSpecification_, isGeneralizationAssignment_, isNegative_, isPartOf_, isPossessive_, isSpecificationGeneralization_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr_, prepositionParameter_, questionParameter_, generalizationWordTypeNr_, specificationWordTypeNr_, relationWordTypeNr_, generalizationCollectionNr_, specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, lastWrittenSentenceString ) ).result != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to create a copy of myself with a different first justification item" );

			if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
				return startError( functionNameString, NULL, "I couldn't create a specification" );

			if( attachJustificationItem->attachJustification( firstJustificationItem, createdSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );

			if( myWordItem()->replaceOrDeleteSpecification( false, this, createdSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to replace or delete a specification" );
			}

		return RESULT_OK;
		}

	ResultType changeFirstJustification( JustificationItem *replacingJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeFirstJustification";

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing justification item is undefined" );

		if( !replacingJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given replacing justification item isn't active" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		firstJustificationItem_ = replacingJustificationItem;

		return RESULT_OK;
		}

	ResultType collectSpecification( bool isCollectGeneralization, bool isExclusiveSpecification, unsigned int collectionNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecification";

		if( collectionNr == NO_COLLECTION_NR )
			return startError( functionNameString, NULL, "The given collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveSpecification )
			isExclusiveSpecification_ = true;

		if( isCollectGeneralization )
			{
			if( generalizationCollectionNr_ > NO_COLLECTION_NR )
				return startError( functionNameString, NULL, "The generalization is already collected" );

			generalizationCollectionNr_ = collectionNr;
			}
		else
			{
			if( specificationCollectionNr_ > NO_COLLECTION_NR )
				return startError( functionNameString, NULL, "The specification is already collected" );

			specificationCollectionNr_ = collectionNr;
			}

		return RESULT_OK;
		}

	ResultType markAsConcludedAssumption()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markAsConcludedAssumption";

		if( !isSelfGeneratedAssumption() )
			return startError( functionNameString, NULL, "I am not a self-generated assumption" );

		isConcludedAssumption_ = true;
		assumptionLevel_ = NO_ASSUMPTION_LEVEL;

		// Clear sentence write buffer to lose the uncertainty word written in the sentence
		strcpy( lastWrittenSentenceString, EMPTY_STRING );

		return RESULT_OK;
		}

	ResultType markSpecificationStringAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationStringAsWritten";

		if( commonVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, "Current write word level overflow" );

		if( specificationStringWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, "My write level is already assigned" );

		specificationStringWriteLevel_ = ++commonVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	ResultType recalculateAssumption()
		{
		bool isAdjustedSpecification = false;
		unsigned short previousAssumptionLevel;
		char functionNameString[FUNCTION_NAME_LENGTH] = "recalculateAssumption";

		if( getAssumptionLevel().result != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to get the assumption level" );

		previousAssumptionLevel = assumptionLevel_;

		if( firstJustificationItem_ != NULL &&
		firstJustificationItem_->hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition() )
			{
			if( markAsConcludedAssumption() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to mark myself as a concluded assumption" );

			isAdjustedSpecification = true;
			}
		else
			{
			// Avoid corrected assumption to be concluded if Spanish ambiguous
			if( isCorrectedAssumption_ ||
			specificationCollectionNr_ == NO_COLLECTION_NR ||
			!myWordItem()->hasCorrectedAssumptionByKnowledge() )
				{
				if( recalculateAssumptionLevel().result != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to recalculate the assumption level" );

				if( isOlderItem() &&
				assumptionLevel_ != previousAssumptionLevel &&

				// Avoid updates of high uncertainty mapping,
				// from 'maybe' of one level to 'maybe' of another level
				( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
				assumptionLevel_ < NUMBER_OF_ASSUMPTION_LEVELS ) )
					{
					if( assumptionLevel_ == NO_ASSUMPTION_LEVEL )
						{
						if( markAsConcludedAssumption() != RESULT_OK )
							return addError( functionNameString, NULL, "After recalculation, I failed to mark myself as concluded assumption" );

						if( isPossessive_ ||
						relationContextNr_ == NO_CONTEXT_NR )
							isAdjustedSpecification = true;
						}
					else
						{
						if( assumptionLevel_ < previousAssumptionLevel &&

						( !isPossessive_ ||
						hasSpecificationCompoundCollection() ) )
							isAdjustedSpecification = true;
						}
					}
				}
			}

		if( isAdjustedSpecification &&
		!isHiddenSpanishSpecification() )
			{
			// Write adjusted specification
			if( myWordItem()->writeUpdatedSpecification( true, false, false, false, true, false, this ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to write an adjusted specification" );
			}

		return RESULT_OK;
		}

	ResultType writeSpecificationConflict()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationConflict";

		if( myWordItem()->writeSelectedSpecification( false, false, this ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write the conflicting specification" );

		if( strlen( commonVariables()->writtenSentenceString ) == 0 )
			return startError( functionNameString, NULL, "The written sentence string is empty" );

		if( commonVariables()->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WARNING, ( isOlderItem() ? INTERFACE_LISTING_CONFLICT : INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write an interface warning" );

		if( commonVariables()->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables()->writtenSentenceString, commonVariables()->learnedFromUserString ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write the conflict sentence" );

		hasSpecificationBeenWrittenAsConflict = true;

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

	JustificationItem *answeredQuestionJustificationItem()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasPrimaryAnsweredQuestion() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *differentAssumptionLevelSpecificationSubstitutionJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		unsigned short primaryAssumptionLevel;
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		SpecificationItem *searchPrimarySpecificationItem;

		if( primarySpecificationItem != NULL &&
		secondarySpecificationItem != NULL )
			{
			primaryAssumptionLevel = primarySpecificationItem->assumptionLevel();

			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isSpecificationSubstitutionAssumption() &&
				searchJustificationItem->isOlderItem() &&
				searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem &&
				( searchPrimarySpecificationItem = searchJustificationItem->primarySpecificationItem() ) != NULL )
					{
					if( searchPrimarySpecificationItem->assumptionLevel() != primaryAssumptionLevel )
						return searchJustificationItem;
					}

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	JustificationItem *firstJustificationItem( unsigned short justificationTypeNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *olderJustificationItem( unsigned short justificationTypeNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isOlderItem() &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *primaryOrSecondarySpecificationJustificationItem( SpecificationItem *referenceSpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( referenceSpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->primarySpecificationItem() == referenceSpecificationItem ||
				searchJustificationItem->secondarySpecificationItem() == referenceSpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *getSpecificationItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchSpecificationItem->languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *getSpecificationItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, unsigned short questionParameter )
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchSpecificationItem->languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nextSpecificationItem()
		{
		return (SpecificationItem *)nextItem;
		}

	SpecificationItem *nextAssignmentOrSpecificationItem()
		{
		return ( isAssignment() ? nextAssignmentItemWithCurrentLevel() : nextSpecificationItem() );
		}

	SpecificationItem *nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, isQuestion() ) : getSpecificationItem( false, false, isQuestion() ) );
		}

	SpecificationItem *nextSelectedQuestionParameterSpecificationItem()
		{
		return ( isAssignment() ? getAssignmentItem( false, false, questionParameter_ ) : getSpecificationItem( false, false, questionParameter_ ) );
		}

	SpecificationItem *nextSelectedQuestionParameterSpecificationItem( bool isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? getAssignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) : getSpecificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	SpecificationItem *recentlyAnsweredQuestionSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isAssignment() ? getAssignmentItem( true, isIncludingThisItem, questionParameter_ ) : getSpecificationItem( true, isIncludingThisItem, questionParameter_ ) );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasCurrentCreationSentenceNr() &&
			searchSpecificationItem->isAnsweredQuestion_ &&
			searchSpecificationItem->isSelfGenerated() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( true );
			}

		return NULL;
		}

	SpecificationItem *updatedSpecificationItem()
		{
		SpecificationItem *updatedSpecificationItem;
		SpecificationItem *searchSpecificationItem = this;

		while( ( updatedSpecificationItem = searchSpecificationItem->replacingSpecificationItem ) != NULL )
			searchSpecificationItem = updatedSpecificationItem;

		return searchSpecificationItem;
		}

	WordItem *collectedWithItselfCommonWordItem()
		{
		WordItem *commonWordItem;

		if( specificationWordItem_ != NULL &&
		( commonWordItem = specificationWordItem_->commonWordItem( specificationCollectionNr_ ) ) != NULL )
			{
			if( commonWordItem->isNounWordSpanishAmbiguous() )
				return commonWordItem;
			}

		return NULL;
		}

	WordItem *generalizationWordItem()
		{
		return myWordItem();
		}

	WordItem *singleRelationWordItem()
		{
		WordItem *firstRelationWordItem;

		// Check all relation words
		if( ( firstRelationWordItem = myWordItem()->contextWordItemInAllWords( relationContextNr_, specificationWordItem_, NULL ) ) != NULL )
			{
			// Found another relation word
			if( myWordItem()->contextWordItemInAllWords( relationContextNr_, specificationWordItem_, firstRelationWordItem ) != NULL )
				return NULL;
			}

		return firstRelationWordItem;
		}

	WordItem *relatedSpecificationWordItem( bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr )
		{
		if( ( isIgnoringExclusive ||
		isExclusiveSpecification_ == isExclusiveSpecification ) &&

		( isIgnoringNegative ||
		isNegative_ == isNegative ) &&

		isPossessive_ == isPossessive &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&
		specificationContextNr_ == specificationContextNr &&

		( ( !isCheckingRelationContext &&
		!isHiddenSpanishSpecification() ) ||

		relationContextNr_ == relationContextNr ) )
			return specificationWordItem_;

		return NULL;
		}

	WordItem *relatedSpecificationWordItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr )
		{
		if( hasSpecificationCollection() &&
		isExclusiveSpecification_ == isExclusiveSpecification &&
		isNegative_ == isNegative &&
		isPossessive_ == isPossessive &&
		isSelfGenerated() == isSelfGeneratedSpecification &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&
		specificationContextNr_ == specificationContextNr &&
		relationContextNr_ == relationContextNr &&

		( assumptionLevel_ == assumptionLevel ||
		isQuestion() ) &&

		( specificationWordTypeNr == NO_WORD_TYPE_NR ||
		specificationWordTypeNr_ == specificationWordTypeNr ) )
			return specificationWordItem_;

		return NULL;
		}

	WordItem *specificationWordItem()
		{
		return specificationWordItem_;
		}

	WordItem *unwrittenProperNameRelationWordItem( WordItem *previousWordItem )
		{
		// Start with previous word
		WordItem *currentRelationWordItem = previousWordItem;

		// Check all relation words
		while( ( currentRelationWordItem = myWordItem()->contextWordItemInAllWords( relationContextNr_, specificationWordItem_, currentRelationWordItem ) ) != NULL )
			{
			if( !currentRelationWordItem->isRelationWordTypeAlreadyWritten( WORD_TYPE_PROPER_NAME ) )
				return currentRelationWordItem;
			}

		return NULL;
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
 *	"All he does is just and good,
 *	and all his commandments are trustworthy." (Psalm 111:7)
 *************************************************************************/
