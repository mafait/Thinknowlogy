/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
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

// Justification Item header

// Some compilers need these class declarations
class SpecificationItem;
class SpecificationResultType;

class JustificationItem : private Item
	{
	friend class AdminAssumption;
	friend class AdminConclusion;
	friend class AdminSpecification;
	friend class AdminWriteJustification;
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

	bool isContextSimilarInAllWords( unsigned int firstContextNr, unsigned int secondContextNr );
	bool isSameJustificationType( JustificationItem *referenceJustificationItem );


	protected:
	// Protected constructed variables

	bool hasJustificationBeenWritten;

	unsigned short orderNr;

	JustificationItem *replacingJustificationItem;


	// Constructor

	JustificationItem( bool hasFeminineOrMasculineProperNameEnding, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem );


	// Protected virtual functions

	virtual void clearReplacingInfo();

	virtual void selectingJustificationSpecifications();

	virtual bool hasWordType( unsigned short queryWordTypeNr );
	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );

	virtual ResultType checkForUsage();

	virtual char *toString( unsigned short queryWordTypeNr );


	// Protected functions

	bool hasAttachedJustification();
	bool hasFeminineOrMasculineProperNameEnding();
	bool hasJustification( JustificationItem *checkJustificationItem );
	bool hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition();

	bool hasHiddenPrimarySpecification();
	bool hasPrimarySpecification();
	bool hasPrimaryAnsweredQuestion();
	bool hasPrimaryQuestion();
	bool hasPrimaryUserSpecification();
	bool hasPossessivePrimarySpecification();
	bool hasReplacedPrimarySpecification();
	bool isPrimarySpecificationWordSpanishAmbiguous();
	bool isUpdatedPrimarySpecificationWordSpanishAmbiguous();

	bool hasAnotherPrimarySpecification();

	bool hasReplacedSecondarySpecification();

	bool isAssumptionJustification();
	bool isConclusionJustification();

	bool isExclusiveSpecificationSubstitutionAssumption();
	bool isGeneralizationAssumption();
	bool isNegativeAssumptionOrConclusion();
	bool isOppositePossessiveConditionalSpecificationAssumption();
	bool isSpecificationSubstitutionAssumption();
	bool isSpecificationSubstitutionPartOfAssumption();
	bool isSuggestiveQuestionAssumption();

	bool isPossessiveReversibleAssumption();
	bool isPossessiveReversibleConclusion();
	bool isPossessiveReversibleAssumptionOrConclusion();

	bool isQuestionJustification();

	unsigned short justificationAssumptionGrade();
	unsigned short justificationTypeNr();
	unsigned short primarySpecificationAssumptionLevel();

	unsigned int nJustificationContextRelations( unsigned int relationContextNr, unsigned int nSpecificationRelationWords );
	unsigned int primarySpecificationCollectionNr();

	ResultType attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *mySpecificationItem );

	ResultType changeAttachedJustification( JustificationItem *newAttachedJustificationItem );
	ResultType changePrimarySpecification( SpecificationItem *replacingSpecificationItem );
	ResultType changeAnotherPrimarySpecification( SpecificationItem *replacingSpecificationItem );
	ResultType changeSecondarySpecification( SpecificationItem *replacingSpecificationItem );
	ResultType changeAnotherSecondarySpecification( SpecificationItem *replacingSpecificationItem );

	ResultType checkForDeletedSpecifications();
	ResultType checkForReplacedOrDeletedSpecification();

	SpecificationResultType getCombinedAssumptionLevel();

	JustificationItem *attachedJustificationItem();
	JustificationItem *attachedPredecessorOfOldJustificationItem( JustificationItem *obsoleteJustificationItem );

	JustificationItem *nextJustificationItem();
	JustificationItem *nextJustificationItemWithSameTypeAndOrderNr();
	JustificationItem *nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem *firstJustificationItem );

	JustificationItem *obsoleteSpanishJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *primarySpecificationWithoutRelationContextJustificationItem( SpecificationItem *primarySpecificationItem );
	JustificationItem *secondarySpecificationQuestion();

	SpecificationItem *primarySpecificationItem();
	SpecificationItem *anotherPrimarySpecificationItem();
	SpecificationItem *secondarySpecificationItem();
	SpecificationItem *anotherSecondarySpecificationItem();

	WordItem *primarySpecificationWordItem();
	WordItem *secondarySpecificationWordItem();
	};

/*************************************************************************
 *	"Come, let us tell of the Lord's greatness;
 *	let us exalt his name together." (Psalm 34:3)
 *************************************************************************/
