/*
 *	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
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

// Justification Item header

// Class declarations needed by some compilers
class SpecificationItem;
class SpecificationResultType;

class JustificationItem : private Item
	{
	friend class AdminAssumption;
	friend class AdminConclusion;
	friend class AdminWriteJustification;
	friend class AdminWriteSpecification;
	friend class JustificationList;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordSpecification;

	// Private loadable variables

	unsigned short justificationTypeNr_;

	SpecificationItem *primarySpecificationItem_;
	SpecificationItem *anotherPrimarySpecificationItem_;
	SpecificationItem *secondarySpecificationItem_;
	SpecificationItem *anotherSecondarySpecificationItem_;

	JustificationItem *attachedJustificationItem_;


	// Private functions

	bool isSameJustificationType( JustificationItem *referenceJustificationItem );


	protected:
	// Protected constructible variables

	bool hasBeenWritten;

	unsigned short orderNr;

	JustificationItem *replacingJustificationItem;


	// Constructor / deconstructor

	JustificationItem( unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem );


	// Protected virtual functions

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );

	virtual ResultType checkForUsage();

	virtual char *toString( unsigned short queryWordTypeNr );


	// Protected functions

	bool hasAttachedJustification();
	bool hasNewJustification();
	bool hasFoundJustification( JustificationItem *checkJustificationItem );
	bool hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition();

	bool hasPrimarySpecification();
	bool hasPrimaryQuestion();
	bool hasPrimarySpecificationWithRelationContext();
	bool hasPrimaryUserSpecification();
	bool hasHiddenPrimarySpecification();
	bool hasReplacedPrimarySpecification();
	bool hasUpdatedPrimarySpecificationWordCollectedWithItself();

	bool hasSecondaryAssignment();
	bool hasSecondarySpecification();
	bool hasSecondarySpecificationWithMultipleRelationContext();
	bool hasSecondaryUserSpecification();

	bool isAssumptionJustification();
	bool isConclusionJustification();

	bool isBackFiredPossessiveConditionalSpecificationAssumption();
	bool isExclusiveSpecificationSubstitutionAssumption();
	bool isFeminineOrMasculineProperNameEndingAssumption();
	bool isGeneralizationAssumption();
	bool isNegativeAssumptionOrConclusion();
	bool isOppositePossessiveConditionalSpecificationAssumption();
	bool isSpecificationSubstitutionAssumption();
	bool isSpecificationSubstitutionPartOfAssumption();
	bool isSuggestiveQuestionAssumption();

	bool isPossessiveReversibleConclusion();

	bool isQuestionJustification();

	unsigned short justificationAssumptionGrade();
	unsigned short justificationTypeNr();

	unsigned int nJustificationContextRelations( unsigned int relationContextNr, unsigned int nSpecificationRelationWords );

	ResultType attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *mySpecificationItem );

	ResultType changeAttachedJustification( JustificationItem *newAttachedJustificationItem );
	ResultType changePrimarySpecification( SpecificationItem *replacingSpecificationItem );
	ResultType changeSecondarySpecification( SpecificationItem *replacingSpecificationItem );

	ResultType checkForDeletedSpecifications();
	ResultType checkForReplacedOrDeletedSpecifications();

	SpecificationResultType getCombinedAssumptionLevel();

	JustificationItem *attachedJustificationItem();
	JustificationItem *attachedPredecessorOfOldJustificationItem( JustificationItem *oldJustificationItem );

	JustificationItem *nextJustificationItem();
	JustificationItem *nextJustificationItemWithSameTypeAndOrderNr();
	JustificationItem *nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem *firstJustificationItem );

	JustificationItem *primarySpecificationWithoutRelationContextJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *foundSecondarySpecificationQuestion();
	JustificationItem *selfGeneratedSecondarySpecificationJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *updatedJustificationItem();

	SpecificationItem *primarySpecificationItem();
	SpecificationItem *anotherPrimarySpecificationItem();
	SpecificationItem *secondarySpecificationItem();
	SpecificationItem *anotherSecondarySpecificationItem();

	WordItem *primarySpecificationWordItem();
	WordItem *secondarySpecificationWordItem();
	};

/*************************************************************************
 *
 *	"Come, let us tell of the Lord's greatness;
 *	let us exalt his name together." (Psalm 34:3)
 *
 *************************************************************************/
