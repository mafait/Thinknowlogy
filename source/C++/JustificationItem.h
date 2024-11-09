/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		Storing info need to write the justification reports
 *					for the self-generated knowledge
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

// Justification Item header

// Class declarations
class SpecificationItem;

class JustificationItem : private Item
	{
	friend class AdminReasoning;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class JustificationList;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordWrite;
	friend class WordSpecification;

	// Private initialized variables

	bool hasFeminineOrMasculineProperNounEnding_ = false;

	unsigned short justificationTypeNr_ = NO_JUSTIFICATION_TYPE;

	SpecificationItem *primarySpecificationItem_ = NULL;
	SpecificationItem *additionalDefinitionSpecificationItem_ = NULL;
	SpecificationItem *secondarySpecificationItem_ = NULL;
	SpecificationItem *additionalProperNounSpecificationItem_ = NULL;

	JustificationItem *attachedJustificationItem_ = NULL;


	// Private functions

	bool hasJustification( JustificationItem *checkJustificationItem );
	bool hasSimilarRelationCollection( unsigned int firstCollectionNr, unsigned int secondCollectionNr, WordItem *firstRelationWordItem, WordItem *secondRelationWordItem );
	bool isSameJustificationType( JustificationItem *referenceJustificationItem );


	protected:
	// Protected constructed variables

	bool hasJustificationBeenWritten = false;

	unsigned short justificationOrderNr = NO_ORDER_NR;


	// Constructor

	JustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, unsigned short justificationOrderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, JustificationItem *attachedJustificationItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem );


	// Protected virtual functions

	virtual void checkForUsage();
	virtual void selectingJustificationSpecifications();

	virtual bool hasWordType( unsigned short queryWordTypeNr );
	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );

	virtual char *itemToString( unsigned short queryWordTypeNr );


	// Protected functions

	void clearReplacingInfo();

	bool hasAdditionalDefinitionSpecification();
	bool hasAssumptionLevel();
	bool hasFeminineOrMasculineProperNounEnding();
	bool hasJustification( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem );

	bool hasPrimarySpecificationRelationCollection();
	bool hasPrimarySpecificationRelationWord();
	bool hasPrimaryUserSpecification();
	bool hasSecondaryCompoundCollectionSpecification();
	bool hasSecondaryUserSpecification();

	bool isExclusivePrimarySpecification();
	bool isOlderPrimarySpecification();
	bool isPossessivePrimarySpecification();
	bool isPossessiveSecondarySpecification();
	bool isPrimaryActiveAssignmentSpecification();
	bool isPrimaryNounSpecification();
	bool isPrimaryQuestionSpecification();
	bool isPrimarySpecificationWordSpanishAmbiguous();
	bool isReplacedPrimarySpecification();
	bool isReplacedSecondarySpecification();

	bool isExclusiveSpecificationSubstitutionAssumption();
	bool isNegativeAssumptionOrConclusion();
	bool isObsoleteAssumptionJustification( bool isIncludingOppositePossessiveSpecificationAssumption, bool isIncludingReversibleAssumptionOrConclusion, bool isIncludingSpecificationSubstitutionAssumptionOrConclusion, bool isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion, bool isOnlySelectingOlderJustifications, unsigned short assumptionLevel );
	bool isOppositePossessiveSpecificationAssumption();
	bool isQuestionJustification();
	bool isReversibleAssumptionOrConclusion();
	bool isSpecificationSubstitutionAssumptionOrConclusion();
	bool isSpecificationSubstitutionPartOfAssumptionOrConclusion();
	bool isSuggestiveQuestionAssumption();

	bool isSecondaryAssignmentSpecification();

	unsigned short justificationAssumptionLevel();
	unsigned short justificationTypeNr();
	unsigned short primarySpecificationWordTypeNr();

	unsigned int nJustificationRelationWords( unsigned int relationCollectionNr, unsigned int nRelationWords, WordItem *relationWordItem );
	unsigned int primarySpecificationCollectionNr();

	signed char attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *mySpecificationItem );
	signed char changeAttachedJustification( JustificationItem *newAttachedJustificationItem );
	signed char checkSpecifications( bool isIncludingReplacedSpecifications );
	signed char replaceSpecification( bool isReplacingPrimarySpecification, bool isReplacingSecondarySpecification, SpecificationItem *replacingSpecificationItem );

	JustificationItem *attachedJustificationItem();
	JustificationItem *attachedPredecessorOfObsoleteJustificationItem( JustificationItem *obsoleteJustificationItem );

	JustificationItem *nextJustificationItem();
	JustificationItem *nextJustificationItemWithSameTypeAndOrderNr();
	JustificationItem *nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem *firstJustificationItem );

	JustificationItem *obsoleteSpanishJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *primarySpecificationWithoutRelationContextJustificationItem( WordItem *primarySpecificationWordItem );

	SpecificationItem *primarySelfGeneratedSpecification();

	SpecificationItem *primarySpecificationItem();
	SpecificationItem *additionalDefinitionSpecificationItem();
	SpecificationItem *secondarySpecificationItem();
	SpecificationItem *additionalProperNounSpecificationItem();

	SpecificationItem *updatedPrimarySpecificationItem();
	SpecificationItem *updatedSecondarySpecificationItem();

	WordItem *generalizationWordItem();
	WordItem *primaryGeneralizationWordItem();
	WordItem *primarySpecificationWordItem();
	WordItem *secondaryGeneralizationWordItem();
	};

/*************************************************************************
 *	"Come, let us tell of the Lord's greatness;
 *	let us exalt his name together." (Psalm 34:3)
 *************************************************************************/
