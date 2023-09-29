/*	Class:		Item
 *	Purpose:	Base class for the knowledge structure
 *	Version:	Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

#include "CompoundResultType.cpp"
#include "ConditionResultType.cpp"
#include "ContextResultType.cpp"
#include "CreateReadWordResultType.cpp"
#include "FileResultType.cpp"
#include "FindSpecificationResultType.cpp"
#include "ReadWordResultType.cpp"
#include "SelectionResultType.cpp"
#include "UserSpecificationResultType.cpp"
#include "WordEndingResultType.cpp"
//Java
//Java
//Java
//Java
//Java
//Java
//Java
//Java
//Java
//Java

	// Private functions

	unsigned int Item::justificationAssumptionGrade( bool hasFeminineOrMasculineProperNounEnding, bool hasAdditionalDefinitionSpecification, bool isExclusivePrimarySpecification, bool isNegativePrimarySpecification, bool isNegativeSecondarySpecification, unsigned short justificationTypeNr )
		{
		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
			case JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION:
			case JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION:
			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				return 1;

			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				return ( hasAdditionalDefinitionSpecification ?
						( hasFeminineOrMasculineProperNounEnding ? 3 : 2 ) : 1 );

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				return ( hasAdditionalDefinitionSpecification &&
						hasFeminineOrMasculineProperNounEnding ? 3 :

							( hasFeminineOrMasculineProperNounEnding ||
							isExclusivePrimarySpecification ? 2 : 1 ) );

			case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION:
				return ( hasAdditionalDefinitionSpecification ||
						isNegativePrimarySpecification ||

						( isExclusivePrimarySpecification &&
						!isNegativeSecondarySpecification ) ? 1 : 0 );

			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION:
				return ( hasAdditionalDefinitionSpecification ? 1 : 0 );

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				return ( hasAdditionalDefinitionSpecification ?
						( hasFeminineOrMasculineProperNounEnding ? 2 : 1 ) : 0 );

			default:
				return NO_ASSUMPTION_LEVEL;
			}
		}

	char *Item::myWordTypeString( unsigned short queryWordTypeNr )
		{
		char *wordTypeString = NULL;

		if( myList_ != NULL &&
		// Don't display my word string if the item is in an Admin list
		!myList_->isAdminList() &&
		myWordItem_ != NULL &&
		( wordTypeString = myWordItem_->wordTypeString( true, queryWordTypeNr ) ) == NULL )
			wordTypeString = myWordItem_->anyWordTypeString();

		return wordTypeString;
		}


	// Constructor

	Item::Item()
		{
		}


	// Protected error functions

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addError( functionNameString, moduleNameString, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString )
		{
		if( inputOutput_ == NULL )
			fprintf( stderr, "\nClass:\t%s\nParent class:\t%s\nFunction:\t%s\nWordItem:%s\nError:\t\t%s.\n", classNameString_, parentClassNameString_, functionNameString, ( wordItemString == NULL ? ADMIN_USER_NAME_STRING : wordItemString ), errorString );
		else
			inputOutput_->displayError( SYMBOL_QUESTION_MARK, ( moduleNameString == NULL ? classNameString_ : moduleNameString ), ( moduleNameString == NULL ? parentClassNameString_ : NULL ), wordItemString, functionNameString, errorString );

		return ( globalVariables_ == NULL ? RESULT_ERROR : globalVariables_->result );
		}

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		sprintf( tempString, "%s%u", errorString, number );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		sprintf( tempString, "%s%u%s%u", errorString1, number1, errorString2, number2 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number )
		{
		sprintf( tempString, "%s%s%s%u", errorString1, errorString2, errorString3, number );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	signed char Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	signed char Item::addError( char listChar, const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		if( inputOutput_ == NULL )
			fprintf( stderr, "\nClass:\t%s\nParent class:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString_, parentClassNameString_, functionNameString, errorString );
		else
			inputOutput_->displayError( listChar, ( moduleNameString == NULL ? classNameString_ : moduleNameString ), ( moduleNameString == NULL ? parentClassNameString_ : NULL ), wordNameString, functionNameString, errorString );

		return ( globalVariables_ == NULL ? RESULT_ERROR : globalVariables_->result );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, NULL, errorString );

		if( globalVariables_ != NULL )
		globalVariables_->result = RESULT_ERROR;
		return RESULT_ERROR;
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, wordNameString, errorString );

		if( globalVariables_ != NULL )
		globalVariables_->result = RESULT_ERROR;
		return RESULT_ERROR;
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		sprintf( tempString, "%s%u", errorString, number );
		return startError( functionNameString, moduleNameString, NULL, tempString );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		sprintf( tempString, "%s%u%s%u", errorString1, number1, errorString2, number2 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2, const char *errorString3, unsigned int number3 )
		{
		sprintf( tempString, "%s%u%s%u%s%u", errorString1, number1, errorString2, number2, errorString3, number3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2, char char2, const char *errorString3 )
		{
		sprintf( tempString, "%s%c%s%c%s", errorString1, char1, errorString2, char2, errorString3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	signed char Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number1, const char *errorString4, unsigned int number2, const char *errorString5, unsigned int number3 )
		{
		sprintf( tempString, "%s%s%s%u%s%u%s%u", errorString1, errorString2, errorString3, number1, errorString4, number2, errorString5, number3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	signed char Item::startSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSystemError( functionNameString, moduleNameString, NULL, errorString );
		}

	signed char Item::startSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		char textChar;
		size_t errorStringPosition = 0;
		size_t tempStringPosition = 0;

		strcpy( tempString, EMPTY_STRING );

		while( errorStringPosition < strlen( errorString ) )
			{
			if( errorString[errorStringPosition] == SYMBOL_BACK_SLASH )
				{
				errorStringPosition++;

				if( errorStringPosition < strlen( errorString ) &&
				( textChar = inputOutput_->convertDiacriticalChar( errorString[errorStringPosition] ) ) != NEW_LINE_CHAR )
					tempString[tempStringPosition++] = textChar;
				}
			else
				tempString[tempStringPosition++] = errorString[errorStringPosition];

			errorStringPosition++;
			}

		addError( functionNameString, moduleNameString, wordNameString, tempString );

		if( globalVariables_ != NULL )
		globalVariables_->result = RESULT_SYSTEM_ERROR;
		return RESULT_SYSTEM_ERROR;
		}

	BoolResultType Item::addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		BoolResultType boolResult;

		boolResult.result = addError( functionNameString, moduleNameString, NULL, errorString );
		return boolResult;
		}

	BoolResultType Item::addBoolResultError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString )
		{
		BoolResultType boolResult;

		boolResult.result = addError( functionNameString, moduleNameString, wordItemString, errorString );
		return boolResult;
		}

	BoolResultType Item::addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		BoolResultType boolResult;

		boolResult.result = addError( functionNameString, moduleNameString, errorString, number );
		return boolResult;
		}

	BoolResultType Item::addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		BoolResultType boolResult;

		boolResult.result = addError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return boolResult;
		}

	BoolResultType Item::addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		BoolResultType boolResult;

		boolResult.result = addError( functionNameString, moduleNameString, errorString1, errorString2, errorString3, errorString4, errorString5 );
		return boolResult;
		}

	BoolResultType Item::startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		BoolResultType boolResult;

		boolResult.result = startError( functionNameString, moduleNameString, errorString );
		return boolResult;
		}

	BoolResultType Item::startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		BoolResultType boolResult;

		boolResult.result = startError( functionNameString, moduleNameString, errorString, number );
		return boolResult;
		}

	BoolResultType Item::startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		BoolResultType boolResult;

		boolResult.result = startError( functionNameString, moduleNameString, errorString1, number1, errorString2, number2 );
		return boolResult;
		}

	BoolResultType Item::startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		BoolResultType boolResult;

		boolResult.result = startError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return boolResult;
		}

	BoolResultType Item::startBoolResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		BoolResultType boolResult;

		boolResult.result = startSystemError( functionNameString, moduleNameString, errorString );
		return boolResult;
		}

	CollectionResultType Item::addCollectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = addError( functionNameString, moduleNameString, wordNameString, errorString );
		return collectionResult;
		}

	CollectionResultType Item::startCollectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return collectionResult;
		}

	CompoundResultType Item::addCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CompoundResultType compoundResult;

		compoundResult.result = addError( functionNameString, moduleNameString, errorString );
		return compoundResult;
		}

	CompoundResultType Item::addCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addCompoundResultError( functionNameString, moduleNameString, tempString );
		}

	CompoundResultType Item::addCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addCompoundResultError( functionNameString, moduleNameString, tempString );
		}

	ConditionResultType Item::addConditionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ConditionResultType conditionResult;

		conditionResult.result = addError( functionNameString, moduleNameString, errorString );
		return conditionResult;
		}

	ConditionResultType Item::startConditionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ConditionResultType conditionResult;

		conditionResult.result = startError( functionNameString, moduleNameString, errorString );
		return conditionResult;
		}

	CompoundResultType Item::startCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CompoundResultType compoundResult;

		compoundResult.result = startError( functionNameString, moduleNameString, errorString );
		return compoundResult;
		}

	ContextResultType Item::addContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ContextResultType contextResult;

		contextResult.result = addError( functionNameString, moduleNameString, errorString );
		return contextResult;
		}

	ContextResultType Item::addContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		ContextResultType contextResult;

		contextResult.result = addError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return contextResult;
		}

	ContextResultType Item::startContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ContextResultType contextResult;

		contextResult.result = startError( functionNameString, moduleNameString, errorString );
		return contextResult;
		}

	ContextResultType Item::startContextResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ContextResultType contextResult;

		contextResult.result = startSystemError( functionNameString, moduleNameString, errorString );
		return contextResult;
		}

	CreateAndAssignResultType Item::addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = addError( functionNameString, moduleNameString, errorString );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = addError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = addError( functionNameString, moduleNameString, errorString1, errorString2, errorString3, errorString4, errorString5 );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = addError( functionNameString, moduleNameString, wordNameString, errorString );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = startError( functionNameString, moduleNameString, errorString );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = startError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = startError( functionNameString, moduleNameString, errorString1, errorString2, errorString3, errorString4, errorString5 );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return createAndAssignResult;
		}

	CreateAndAssignResultType Item::startCreateAndAssignResultSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = startSystemError( functionNameString, moduleNameString, wordNameString, errorString );
		return createAndAssignResult;
		}

	CreateReadWordResultType Item::addCreateReadWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CreateReadWordResultType createReadWordResult;

		createReadWordResult.result = addError( functionNameString, moduleNameString, errorString );
		return createReadWordResult;
		}

	CreateReadWordResultType Item::startCreateReadWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CreateReadWordResultType createReadWordResult;

		createReadWordResult.result = startError( functionNameString, moduleNameString, errorString );
		return createReadWordResult;
		}

	DuplicateResultType Item::startDuplicateResultError( const char *functionNameString, const char *errorString )
		{
		DuplicateResultType duplicateResult;

		duplicateResult.result = startError( functionNameString, NULL, errorString );
		return duplicateResult;
		}

	FileResultType Item::addFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = addError( functionNameString, moduleNameString, errorString );
		return fileResult;
		}

	FileResultType Item::startFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = startError( functionNameString, moduleNameString, errorString );
		return fileResult;
		}

	FindSpecificationResultType Item::startFindSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		FindSpecificationResultType findSpecificationResult;

		findSpecificationResult.result = startError( functionNameString, moduleNameString, errorString );
		return findSpecificationResult;
		}

	GrammarResultType Item::startGrammarResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		GrammarResultType grammarResult;

		grammarResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return grammarResult;
		}

	JustificationResultType Item::startJustificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		JustificationResultType justificationResult;

		justificationResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return justificationResult;
		}

	QueryResultType Item::addQueryResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		QueryResultType queryResult;

		queryResult.result = addError( functionNameString, moduleNameString, errorString );
		return queryResult;
		}

	QueryResultType Item::startQueryResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		QueryResultType queryResult;

		queryResult.result = startError( functionNameString, moduleNameString, errorString );
		return queryResult;
		}

	QueryResultType Item::startQueryResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2 )
		{
		sprintf( tempString, "%s%c%s", errorString1, char1, errorString2 );
		return startQueryResultError( functionNameString, moduleNameString, tempString );
		}

	ReadWordResultType Item::startReadWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ReadWordResultType readWordResult;

		readWordResult.result = startError( functionNameString, moduleNameString, errorString );
		return readWordResult;
		}

	RelatedResultType Item::addRelatedResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		RelatedResultType relatedResult;

		relatedResult.result = addError( functionNameString, moduleNameString, wordNameString, errorString );
		return relatedResult;
		}

	RelatedResultType Item::startRelatedResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		RelatedResultType relatedResult;

		relatedResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return relatedResult;
		}

	SelectionResultType Item::startSelectionResultError( const char *functionNameString, const char *errorString )
		{
		SelectionResultType selectionResult;

		selectionResult.result = startError( functionNameString, NULL, errorString );
		return selectionResult;
		}

	ShortResultType Item::startShortResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ShortResultType shortResult;

		shortResult.result = startError( functionNameString, moduleNameString, errorString );
		return shortResult;
		}

	ShortResultType Item::startShortResultError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		ShortResultType shortResult;

		shortResult.result = startError( functionNameString, moduleNameString, errorString, number );
		return shortResult;
		}

	SpecificationResultType Item::addSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		SpecificationResultType specificationResult;

		specificationResult.result = addError( functionNameString, moduleNameString, wordNameString, errorString );
		return specificationResult;
		}

	SpecificationResultType Item::startSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		SpecificationResultType specificationResult;

		specificationResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return specificationResult;
		}

	UserSpecificationResultType Item::addUserSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		UserSpecificationResultType userSpecificationResult;

		userSpecificationResult.result = addError( functionNameString, moduleNameString, wordNameString, errorString );
		return userSpecificationResult;
		}

	UserSpecificationResultType Item::startUserSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		UserSpecificationResultType userSpecificationResult;

		userSpecificationResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return userSpecificationResult;
		}

	WordEndingResultType Item::startWordEndingResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		WordEndingResultType wordEndingResult;

		wordEndingResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return wordEndingResult;
		}

	WordResultType Item::addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		WordResultType wordResult;

		wordResult.result = addError( functionNameString, moduleNameString, errorString );
		return wordResult;
		}

	WordResultType Item::addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		WordResultType wordResult;

		wordResult.result = addError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return wordResult;
		}

	WordResultType Item::startWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		WordResultType wordResult;

		wordResult.result = startError( functionNameString, moduleNameString, errorString );
		return wordResult;
		}

	WordTypeResultType Item::startWordTypeResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		WordTypeResultType wordTypeResult;

		wordTypeResult.result = startError( functionNameString, moduleNameString, wordNameString, errorString );
		return wordTypeResult;
		}


	// Protected virtual functions

	void Item::checkForUsage()
		{
		// This is a virtual function. Therefore, it has no body.
		}

	void Item::displayString( bool _isReturnQueryToPosition )
		{
		// This is a virtual function. Therefore, it has no body, and the given variables are unreferenced.
		}

	void Item::displayWordReferences( bool _isReturnQueryToPosition )
		{
		// This is a virtual function. Therefore, it has no body, and the given variables are unreferenced.
		}

	void Item::selectingAttachedJustifications( bool _isSelectingJustificationSpecifications )
		{
		// This is a virtual function. Therefore, it has no body, and the given variables are unreferenced.
		}

	void Item::selectingJustificationSpecifications()
		{
		// This is a virtual function. Therefore, it has no body.
		}

	bool Item::hasParameter( unsigned int _queryParameter )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return false;
		}

	bool Item::hasWordType( unsigned short _queryWordTypeNr )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return false;
		}

	bool Item::hasReferenceItemById( unsigned int _querySentenceNr, unsigned int _queryItemNr )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return false;
		}

	bool Item::isSorted( Item *nextSortItem )
		{
		return ( nextSortItem != NULL &&
				// Default: Descending creationSentenceNr_
				creationSentenceNr_ > nextSortItem->creationSentenceNr_ );
		}

	char *Item::itemString()
		{
		// This is a virtual function
		return NULL;
		}

	char *Item::itemToString( unsigned short _queryWordTypeNr )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return NULL;
		}

	BoolResultType Item::findMatchingWordReferenceString( char *_queryString )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		BoolResultType boolResult;
		return boolResult;
		}


	// Protected common functions

	void Item::clearArchivedSentenceNr()
		{
		archivedSentenceNr_ = NO_SENTENCE_NR;
		}

	void Item::clearReplacedSentenceNr()
		{
		replacedSentenceNr_ = NO_SENTENCE_NR;
		}

	void Item::setActiveStatus()
		{
		statusChar_ = QUERY_ACTIVE_CHAR;
		}

	void Item::setArchivedStatus()
		{
		statusChar_ = QUERY_ARCHIVED_CHAR;
		}

	void Item::setInactiveStatus()
		{
		statusChar_ = QUERY_INACTIVE_CHAR;
		}

	void Item::setReplacedStatus()
		{
		statusChar_ = QUERY_REPLACED_CHAR;
		}

	void Item::setDeletedStatus()
		{
		statusChar_ = QUERY_DELETED_CHAR;
		}

	void Item::setActiveSentenceNr()
		{
		if( activeSentenceNr_ == NO_SENTENCE_NR )
			activeSentenceNr_ = globalVariables_->currentSentenceNr;
		}

	void Item::setArchivedSentenceNr()
		{
		if( archivedSentenceNr_ == NO_SENTENCE_NR )
			archivedSentenceNr_ = globalVariables_->currentSentenceNr;
		}

	void Item::setInactiveSentenceNr()
		{
		if( inactiveSentenceNr_ == NO_SENTENCE_NR )
			inactiveSentenceNr_ = globalVariables_->currentSentenceNr;
		}

	void Item::setReplacedSentenceNr()
		{
		if( replacedSentenceNr_ == NO_SENTENCE_NR )
			replacedSentenceNr_ = globalVariables_->currentSentenceNr;
		}

	void Item::displayWords( bool isReturnQueryToPosition, unsigned short queryWordTypeNr )
		{
		char *myWordString;

		statusString[0] = statusChar_;

		if( ( myWordString = myWordTypeString( queryWordTypeNr ) ) != NULL )
			{
			if( globalVariables_->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables_->queryString, statusString );

			globalVariables_->hasFoundQuery = true;
			strcat( globalVariables_->queryString, myWordString );
			}
		}

	// Strictly for initialization of AdminItem
	void Item::initializeItemVariables( const char *classNameString, GlobalVariables *globalVariables, WordItem *myWordItem )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( globalVariables_ = globalVariables ) == NULL )
			strcpy( errorString, "The given global variables is undefined" );

		if( ( myWordItem_ = myWordItem ) == NULL )
			strcpy( errorString, "The given my word item is undefined" );

		if( classNameString == NULL )
			strcpy( errorString, "The given class name string is undefined" );
		else
			strcpy( classNameString_, classNameString );

		if( strlen( errorString ) > 0 )
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	void Item::initializeItemVariables( unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, const char *classNameString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;
//Java
//Java

		// Checking private initialized variables

		if( ( globalVariables_ = globalVariables ) == NULL )
			strcpy( errorString, "The given global variables is undefined" );
		else
			{
			userNr_ = globalVariables_->currentUserNr;
			originalSentenceNr_ = ( originalSentenceNr == NO_SENTENCE_NR ? globalVariables_->currentSentenceNr : originalSentenceNr );
			creationSentenceNr_ = globalVariables_->currentSentenceNr;
			activeSentenceNr_ = ( originalSentenceNr == NO_SENTENCE_NR ? globalVariables_->currentSentenceNr : activeSentenceNr );
			inactiveSentenceNr_ = inactiveSentenceNr;
			archivedSentenceNr_ = archivedSentenceNr;

			if( globalVariables_->currentSentenceItemNr >= NO_ITEM_NR )
				{
				if( globalVariables_->currentSentenceItemNr < MAX_ITEM_NR )
					itemNr_ = ++globalVariables_->currentSentenceItemNr;
				else
					strcpy( errorString, "Current item number overflow" );
				}
			else
				strcpy( errorString, "The given current item number is less than zero" );
			}

		if( ( inputOutput_ = inputOutput ) == NULL )
			strcpy( errorString, "The given input-output is undefined" );

		if( ( myList_ = myList ) == NULL )
			strcpy( errorString, "The given my list is undefined" );

		if( ( myWordItem_ = myWordItem ) == NULL )
			strcpy( errorString, "The given my word item is undefined" );

		if( classNameString == NULL )
			strcpy( errorString, "The given class name string is undefined" );
		else
			strcpy( classNameString_, classNameString );

		if( strlen( errorString ) > 0 )
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	bool Item::hasActiveSentenceNr()
		{
		return ( activeSentenceNr_ > NO_SENTENCE_NR );
		}

	bool Item::hasInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ > NO_SENTENCE_NR );
		}

	bool Item::hasCurrentCreationSentenceNr()
		{
		return ( creationSentenceNr_ == globalVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentOrNewerCreationSentenceNr()
		{
		return ( creationSentenceNr_ >= globalVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentActiveSentenceNr()
		{
		return ( activeSentenceNr_ == globalVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ == globalVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentArchivedSentenceNr()
		{
		return ( archivedSentenceNr_ == globalVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentReplacedSentenceNr()
		{
		return ( replacedSentenceNr_ == globalVariables_->currentSentenceNr );
		}

	bool Item::hasSentenceNr( unsigned int sentenceNr )
		{
		return ( originalSentenceNr_ == sentenceNr ||
				creationSentenceNr_ == sentenceNr ||
				activeSentenceNr_ == sentenceNr ||
				inactiveSentenceNr_ == sentenceNr ||
				archivedSentenceNr_ == sentenceNr ||
				replacedSentenceNr_ == sentenceNr );
		}

	bool Item::hasUserNr()
		{
		return ( userNr_ > NO_USER_NR );
		}

	bool Item::isOlderItem()
		{
		return ( originalSentenceNr_ < globalVariables_->currentSentenceNr );
		}

	bool Item::isActiveItem()
		{
		return ( statusChar_ == QUERY_ACTIVE_CHAR );
		}

	bool Item::isInactiveItem()
		{
		return ( statusChar_ == QUERY_INACTIVE_CHAR );
		}

	bool Item::isArchivedItem()
		{
		return ( statusChar_ == QUERY_ARCHIVED_CHAR );
		}

	bool Item::isReplacedItem()
		{
		return ( statusChar_ == QUERY_REPLACED_CHAR );
		}

	bool Item::isDeletedItem()
		{
		return ( statusChar_ == QUERY_DELETED_CHAR );
		}

	bool Item::isReplacedOrDeletedItem()
		{
		return ( statusChar_ == QUERY_REPLACED_CHAR ||
				statusChar_ == QUERY_DELETED_CHAR );
		}

	bool Item::wasActiveBefore()
		{
		return ( previousStatusChar == QUERY_ACTIVE_CHAR );
		}

	bool Item::wasInactiveBefore()
		{
		return ( previousStatusChar == QUERY_INACTIVE_CHAR );
		}

	bool Item::wasArchivedBefore()
		{
		return ( previousStatusChar == QUERY_ARCHIVED_CHAR );
		}

	unsigned short Item::userNr()
		{
		return userNr_;
		}

	unsigned int Item::activeSentenceNr()
		{
		return activeSentenceNr_;
		}

	unsigned int Item::inactiveSentenceNr()
		{
		return inactiveSentenceNr_;
		}

	unsigned int Item::originalSentenceNr()
		{
		return originalSentenceNr_;
		}

	unsigned int Item::creationSentenceNr()
		{
		return creationSentenceNr_;
		}

	unsigned int Item::archivedSentenceNr()
		{
		return archivedSentenceNr_;
		}

	unsigned int Item::replacedSentenceNr()
		{
		return replacedSentenceNr_;
		}

	unsigned int Item::itemNr()
		{
		return itemNr_;
		}

	signed char Item::decrementActiveSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementActiveSentenceNr";

		if( activeSentenceNr_ <= NO_SENTENCE_NR )
			return startError( functionNameString, parentClassNameString_, "The active sentence number is undefined" );

		activeSentenceNr_--;

		return RESULT_OK;
		}

	signed char Item::decrementInactiveSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementInactiveSentenceNr";

		if( inactiveSentenceNr_ <= NO_SENTENCE_NR )
			return startError( functionNameString, parentClassNameString_, "The inactive sentence number is undefined" );

		inactiveSentenceNr_--;

		return RESULT_OK;
		}

	signed char Item::decrementOriginalSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementOriginalSentenceNr";

		if( originalSentenceNr_ <= NO_SENTENCE_NR )
			return startError( functionNameString, parentClassNameString_, "The original sentence number is undefined" );

		originalSentenceNr_--;

		return RESULT_OK;
		}

	signed char Item::decrementCreationSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementCreationSentenceNr";

		if( creationSentenceNr_ <= NO_SENTENCE_NR )
			return startError( functionNameString, parentClassNameString_, "The creation sentence number is undefined" );

		creationSentenceNr_--;

		return RESULT_OK;
		}

	signed char Item::decrementArchivedSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementArchivedSentenceNr";

		if( archivedSentenceNr_ <= NO_SENTENCE_NR )
			return startError( functionNameString, parentClassNameString_, "The archived sentence number is undefined" );

		archivedSentenceNr_--;

		return RESULT_OK;
		}

	signed char Item::decrementReplacedSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementReplacedSentenceNr";

		if( replacedSentenceNr_ <= NO_SENTENCE_NR )
			return startError( functionNameString, parentClassNameString_, "The replaced sentence number is undefined" );

		replacedSentenceNr_--;

		return RESULT_OK;
		}

	signed char Item::decrementItemNr( unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementItemNr";

		if( decrementOffset >= itemNr_ )
			return startError( functionNameString, parentClassNameString_, "The given decrement offset is higher than the item number itself" );

		itemNr_ -= decrementOffset;

		return RESULT_OK;
		}

	char Item::statusChar()
		{
		return statusChar_;
		}

	char *Item::classNameString()
		{
		return classNameString_;
		}

	char *Item::itemBaseToString( unsigned short queryWordTypeNr )
		{
		char *myWordString = myWordTypeString( queryWordTypeNr );
		char *queryString;
		char *userNameString = ( myWordItem_ != NULL ?
								myWordItem_->userNameString( userNr_ ) : NULL );

		statusString[0] = statusChar_;
		strcpy( globalVariables_->queryString, EMPTY_STRING );

		queryString = globalVariables_->queryString;

		// Display status if not active
		if( !isActiveItem() )
			strcat( queryString, statusString );

		if( myWordString != NULL )
			{
			sprintf( tempString, "%c%s%c", QUERY_WORD_START_CHAR, myWordString, QUERY_WORD_END_CHAR );
			strcat( queryString, tempString );
			}

		sprintf( tempString, "%c%c%c", QUERY_LIST_START_CHAR, ( myList_ == NULL ? QUERY_NO_LIST_CHAR : myList_->listChar() ), QUERY_LIST_END_CHAR );
		strcat( queryString, tempString );

		sprintf( tempString, "%c%u%c%u%c", QUERY_ITEM_START_CHAR, creationSentenceNr_, QUERY_SEPARATOR_CHAR, itemNr_, QUERY_ITEM_END_CHAR );
		strcat( queryString, tempString );
/*
		// Don't display. Is always true during a query
		if( isSelectedByQuery )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSelectedByQuery" );
			}
*/
		if( isArchivedItem() ||
		isReplacedItem() )
			{
			sprintf( tempString, "%cpreviousStatusChar:%c", QUERY_SEPARATOR_CHAR, previousStatusChar );
			strcat( queryString, tempString );
			}

		if( userNr_ > NO_USER_NR )
			{
			if( userNameString != NULL )
				sprintf( tempString, "%cuser:%s", QUERY_SEPARATOR_CHAR, userNameString );
			else
				sprintf( tempString, "%cuser:%u", QUERY_SEPARATOR_CHAR, userNr_ );

			strcat( queryString, tempString );
			}

		if( originalSentenceNr_ > NO_SENTENCE_NR &&
		originalSentenceNr_ != creationSentenceNr_ )
			{
			sprintf( tempString, "%coriginalSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, originalSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( activeSentenceNr_ > NO_SENTENCE_NR &&
		activeSentenceNr_ != creationSentenceNr_ )
			{
			sprintf( tempString, "%cactiveSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, activeSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( inactiveSentenceNr_ > NO_SENTENCE_NR &&
		inactiveSentenceNr_ != creationSentenceNr_ )
			{
			sprintf( tempString, "%cinactiveSentence:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, inactiveSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( archivedSentenceNr_ > NO_SENTENCE_NR )
			{
			sprintf( tempString, "%carchivedSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, archivedSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( replacedSentenceNr_ > NO_SENTENCE_NR )
			{
			sprintf( tempString, "%creplacedSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, replacedSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}

	char *Item::parentClassNameString()
		{
		return parentClassNameString_;
		}

	GlobalVariables *Item::globalVariables()
		{
		return globalVariables_;
		}

	InputOutput *Item::inputOutput()
		{
		return inputOutput_;
		}

	List *Item::myList()
		{
		return myList_;
		}

	Item *Item::tailOfList()
		{
		Item *previousSearchItem = this;
		Item *searchItem = nextItem;

		while( searchItem != NULL )
			{
			previousSearchItem = searchItem;
			searchItem = searchItem->nextItem;
			}

		return previousSearchItem;
		}

	WordItem *Item::myWordItem()
		{
		return myWordItem_;
		}


	// Protected database connection functions
/*
	signed char Item::storeChangesInFutureDatabase()
		{
		// Save changes of this item to the database

		return RESULT_OK;
		}
*/

	// Protected definition functions

	bool Item::doesStringStartWithPhoneticVowel( char *textString )
		{
		unsigned short index = 0;

		if( textString != NULL )
			{
			while( index < NUMBER_OF_PHONETIC_VOWELS )
				{
				if( textString[0] == PHONETIC_VOWEL[index] )
					return true;

				index++;
				}
			}

		return false;
		}

	bool Item::isAdjectiveParameter( unsigned short checkParameter )
		{
		return ( checkParameter == WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE );
//				checkParameter == WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL ||
//				checkParameter == WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE );
		}

	bool Item::isDefiniteArticleParameter( unsigned short checkParameter )
		{
		return ( checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	bool Item::isIndefiniteArticleParameter( unsigned short checkParameter )
		{
		return ( checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||

				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE );
		}

	bool Item::isFeminineArticleParameter( unsigned short articleParameter )
		{
		return ( articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||

				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE );
		}

	bool Item::isMasculineArticleParameter( unsigned short articleParameter )
		{
		return ( articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ||

				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	bool Item::isMatchingWordType( unsigned short firstWordTypeNr, unsigned short secondWordTypeNr )
		{
		return	( firstWordTypeNr == secondWordTypeNr ||

				( isNounWordType( firstWordTypeNr ) &&
				isNounWordType( secondWordTypeNr ) ) );
		}

	bool Item::isNounWordType( unsigned short wordTypeNr )
		{
		return ( wordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == WORD_TYPE_NOUN_PLURAL );
		}

	bool Item::isGeneralizationReasoningWordType( unsigned short wordTypeNr )
		{
		return ( wordTypeNr == WORD_TYPE_PROPER_NOUN ||
				wordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == WORD_TYPE_NOUN_PLURAL );
		}

	bool Item::isSpecificationReasoningWordType( unsigned short wordTypeNr )
		{
		return ( wordTypeNr == WORD_TYPE_ADJECTIVE ||
				wordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == WORD_TYPE_NOUN_PLURAL );
		}

	unsigned int Item::justificationAssumptionLevel( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem )
		{
		unsigned int assumptionLevel = justificationAssumptionGrade( hasFeminineOrMasculineProperNounEnding, ( additionalDefinitionSpecificationItem != NULL ), ( primarySpecificationItem != NULL && primarySpecificationItem->isExclusiveSpecification() ), ( primarySpecificationItem != NULL && primarySpecificationItem->isNegative() ), ( secondarySpecificationItem != NULL && secondarySpecificationItem->isNegative() ), justificationTypeNr );
		unsigned int tempAssumptionLevel;

		if( primarySpecificationItem != NULL &&
		( tempAssumptionLevel = primarySpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
			assumptionLevel += tempAssumptionLevel;

		if( additionalDefinitionSpecificationItem != NULL &&
		( tempAssumptionLevel = additionalDefinitionSpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
			assumptionLevel += tempAssumptionLevel;

		if( secondarySpecificationItem != NULL &&
		( tempAssumptionLevel = secondarySpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
			assumptionLevel += tempAssumptionLevel;

		if( additionalProperNounSpecificationItem != NULL &&
		( tempAssumptionLevel = additionalProperNounSpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
			assumptionLevel += tempAssumptionLevel;

		return assumptionLevel;
		}

/*************************************************************************
 *	"Give thanks to him who made the heavenly lights-
 *		His faithful love endures forever.
 *	the sun to rule the day
 *		His faithful love endures forever.
 *	and the moon and stars to rule the night.
 *		His faithful love endures forever." (Psalm 136:7-9)
 *************************************************************************/
