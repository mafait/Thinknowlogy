/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

// Justification Item header

// Class declarations
class SpecificationItem;

class JustificationItem : private Item
	{
	friend class AdminReasoningNew;
	friend class AdminReasoningOld;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class JustificationList;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordSpecification;

	// Private initialized variables

	bool hasFeminineOrMasculineProperNameEnding_;

	unsigned short justificationTypeNr_;

	SpecificationItem *primarySpecificationItem_;
	SpecificationItem *anotherPrimarySpecificationItem_;
	SpecificationItem *secondarySpecificationItem_;
	SpecificationItem *anotherSecondarySpecificationItem_;

	JustificationItem *attachedJustificationItem_;


	// Private functions

	bool hasJustification( JustificationItem *checkJustificationItem );
	bool isContextSimilarInContextWords( unsigned int firstContextNr, unsigned int secondContextNr );
	bool isExclusiveSpecificationSubstitutionAssumption();
	bool isSameJustificationType( JustificationItem *referenceJustificationItem );


	protected:
	// Protected constructed variables

	bool hasJustificationBeenWritten;

	unsigned short orderNr;


	// Constructor

	JustificationItem( bool hasFeminineOrMasculineProperNameEnding, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem, CommonVariables *commonVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem );


	// Protected virtual functions

	virtual void checkForUsage();
	virtual void selectingJustificationSpecifications();

	virtual bool hasWordType( unsigned short queryWordTypeNr );
	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );

	virtual char *itemToString( unsigned short queryWordTypeNr );


	// Protected functions

	void clearReplacingInfo();

	bool hasAttachedJustification();
	bool hasFeminineOrMasculineProperNameEnding();
	bool hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem );
	bool hasJustification( bool hasFeminineOrMasculineProperNameEnding, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem );
	bool hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition();

	bool hasNonPossessivePrimaryUserSpecification();
	bool hasPrimaryAnsweredQuestion();
	bool hasPrimaryQuestion();
	bool hasPossessivePrimarySpecification();
	bool hasPossessiveSecondarySpecification();
	bool hasReplacedPrimarySpecification();
	bool isPrimarySpecificationWordSpanishAmbiguous();
	bool isUpdatedPrimarySpecificationWordSpanishAmbiguous();

	bool hasAnotherPrimarySpecification();

	bool isAssumptionJustification();
	bool isConclusionJustification();

	bool isGeneralizationAssumption();
	bool isNegativeAssumptionOrConclusion();
	bool isOppositePossessiveConditionalSpecificationAssumption();
	bool isPossessiveReversibleAssumption();
	bool isPossessiveReversibleConclusion();
	bool isPossessiveReversibleConclusionWithoutRelationContext();
	bool isPossessiveReversibleAssumptionOrConclusion();
	bool isQuestionJustification();
	bool isSpecificationSubstitutionAssumption();
	bool isSuggestiveQuestionAssumption();

	unsigned short justificationAssumptionGrade();
	unsigned short justificationTypeNr();
	unsigned short updatedJustificationTypeNr( SpecificationItem *secondarySpecificationItem );
	unsigned short primarySpecificationAssumptionLevel();

	unsigned int nJustificationContextRelations( unsigned int relationContextNr, unsigned int nRelationWords );
	unsigned int primarySpecificationCollectionNr();

	signed char attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *mySpecificationItem );

	signed char changeAttachedJustification( JustificationItem *newAttachedJustificationItem );
	signed char changePrimarySpecification( SpecificationItem *replacingSpecificationItem );
	signed char changeAnotherPrimarySpecification( SpecificationItem *replacingSpecificationItem );
	signed char changeSecondarySpecification( SpecificationItem *replacingSpecificationItem );
	signed char changeAnotherSecondarySpecification( SpecificationItem *replacingSpecificationItem );

	signed char checkForDeletedSpecifications();
	signed char checkForReplacedOrDeletedSpecification();

	JustificationItem *attachedJustificationItem();
	JustificationItem *attachedPredecessorOfOldJustificationItem( JustificationItem *obsoleteJustificationItem );

	JustificationItem *nextJustificationItem();
	JustificationItem *nextJustificationItemWithSameTypeAndOrderNr();
	JustificationItem *nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem *firstJustificationItem );

	JustificationItem *obsoleteSpanishJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *primarySpecificationWithoutRelationContextJustificationItem( WordItem *primarySpecificationWordItem );
	JustificationItem *secondarySpecificationQuestion();

	SpecificationItem *primarySpecificationItem();
	SpecificationItem *anotherPrimarySpecificationItem();
	SpecificationItem *secondarySpecificationItem();
	SpecificationItem *anotherSecondarySpecificationItem();

	SpecificationItem *updatedPrimarySpecificationItem();
	SpecificationItem *updatedSecondarySpecificationItem();

	WordItem *generalizationWordItem();
	WordItem *primarySpecificationWordItem();
	WordItem *secondaryGeneralizationWordItem();
	WordItem *secondarySpecificationWordItem();

	ShortResultType getCombinedAssumptionLevel();

	char *debugString();
	};

/*************************************************************************
 *	"Come, let us tell of the Lord's greatness;
 *	let us exalt his name together." (Psalm 34:3)
 *************************************************************************/
