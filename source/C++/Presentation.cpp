/*
 *	Class:		Presentation
 *	Purpose:	To format the information presented to the user
 *	Version:	Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

#ifndef PRESENTATION
#define PRESENTATION 1

#include <ctype.h>
#ifdef _MSC_VER
// MS Visual Studio
// Defines _getch()
#include <conio.h>
#endif
#include <wchar.h>
#include "WordItem.h"

class Presentation
	{
	friend class AdminAuthorization;
	friend class AdminCleanup;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class AdminWriteJustification;
	friend class AdminWriteSpecification;
	friend class GrammarList;
	friend class Item;
	friend class List;
	friend class ListQuery;
	friend class WordCollection;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordTypeItem;
	friend class WordTypeList;
	friend class WordWrite;

	// Private constructible variables

	bool hasReadLine_;
	bool isShowingExtraPromptLine_;

	unsigned short lastShownInterfaceParameter_;
	unsigned short lastUsedPromptTypeNr_;

	size_t currentPosition_;
	size_t progressLength_;
	size_t statusLength_;

	unsigned int maximumProgress_;
	unsigned int previousProgress_;

	char currentProgressString_[MAX_SENTENCE_STRING_LENGTH];
	char currentStatusString_[MAX_SENTENCE_STRING_LENGTH];
	char errorString_[MAX_SENTENCE_STRING_LENGTH];
	char outputString_[MAX_SENTENCE_STRING_LENGTH];
	char tempErrorString_[MAX_SENTENCE_STRING_LENGTH];

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	FILE *testFile_;

	CommonVariables *commonVariables_;


	// Private functions

	void returnOutputToPosition( bool isSkippingInTestFile, bool isError, bool isWriting )
		{
		currentPosition_ = 0;
		addStringToOutput( isSkippingInTestFile, isError, isWriting, NEW_LINE_STRING );
		}

	void addCharToOutput( bool isSkippingInTestFile, bool isError, bool isWriting, char writeChar )
		{
		char charString[2] = SPACE_STRING;

		charString[0] = writeChar;
		addStringToOutput( isSkippingInTestFile, isError, isWriting, charString );
		}

	void addStringToOutput( bool isSkippingInTestFile, bool isError, bool isWriting, const char *writeString )
		{
		if( !isError )
			strcat( outputString_, writeString );

		if( ( isWriting ||
		// Print output string before printing error
		isError ) &&

		strlen( outputString_ ) > 0 )
			{
			if( isSkippingInTestFile ||
			testFile_ == NULL )
				printf( "%s", outputString_ );
			else
#ifdef _MSC_VER
				// MS Visual Studio
				fwprintf( testFile_, L"%S", &outputString_ );
#else
				// For compilers other than MS Visual Studio
				fprintf( testFile_, "%s", outputString_ );
#endif

			// Clear output string
			strcpy( outputString_, EMPTY_STRING );
			}

		if( isError )
			{
			strcat( errorString_, writeString );

			if( isWriting )
				{
				if( isSkippingInTestFile ||
				testFile_ == NULL )
					fprintf( stderr, "%s", errorString_ );
				else
#ifdef _MSC_VER
					// MS Visual Studio
					fwprintf( testFile_, L"%S", &errorString_ );
#else
					// For compilers other than MS Visual Studio
					fprintf( testFile_, "%s", outputString_ );
#endif

				strcpy( errorString_, EMPTY_STRING );
				}
			}
		}

	void stripStartAndEndSpaces( char *inputString, char *outputString )
		{
		size_t endPosition;
		size_t inputStringLength;
		size_t startPosition = 0;

		if( inputString != NULL &&
		outputString != NULL &&
		( inputStringLength = strlen( inputString ) ) > 0 )
			{
			// Skipped start spaces
			while( startPosition < inputStringLength &&
			isspace( inputString[startPosition] ) )
				startPosition++;

			endPosition = strlen( inputString );

			// Skipped end spaces
			while( startPosition < endPosition &&
			isspace( inputString[endPosition - 1] ) )
				endPosition--;

			strcpy( outputString, EMPTY_STRING );
			strncat( outputString, &inputString[startPosition], ( endPosition - startPosition ) );
			}
		}

	bool isBreakCharacter( char character )
		{
		return ( character == SYMBOL_COMMA ||
				character == SYMBOL_COLON ||
				character == SYMBOL_SEMI_COLON ||
				character == SYMBOL_DOUBLE_COLON ||
				character == SYMBOL_EXCLAMATION_MARK ||
				character == SYMBOL_EXCLAMATION_MARK_INVERTED ||
				character == SYMBOL_QUESTION_MARK ||
				character == SYMBOL_QUESTION_MARK_INVERTED ||
				character == SYMBOL_PIPE ||
				character == SYMBOL_SLASH ||
				character == SYMBOL_BACK_SLASH ||
				character == SYMBOL_CLOSE_ROUNDED_BRACKET ||
				character == SYMBOL_CLOSE_CURVED_BRACKET ||
				character == SYMBOL_CLOSE_HOOKED_BRACKET ||
				character == SYMBOL_CLOSE_SQUARE_BRACKET );
		}

	ResultType writeText( bool isSkippingInTestFile, bool isForcingEmptyString, bool isError, bool isFirstCharacterToUpperCase, bool isReturningToPosition, unsigned short promptTypeNr, size_t leftWidth, size_t rightWidth, size_t printStringLength, const char *promptString, const char *writeString )
		{
		bool isPrintingPrompt = true;
		bool isStartingNewLine = false;
		size_t wordPosition;
		size_t i = 0;
		size_t length = 0;
		size_t firstAlphaPosition = 0;
		char textString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeText";

		if( writeString != NULL )
			{
			if( isForcingEmptyString ||
			printStringLength > 0 ||
			leftWidth > NO_CENTER_WIDTH ||
			rightWidth > NO_CENTER_WIDTH )
				{
				if( isReturningToPosition &&
				currentPosition_ > 0 )
					returnOutputToPosition( isSkippingInTestFile, isError, false );

				strcpy( textString, EMPTY_STRING );
				strncat( textString, writeString, printStringLength );

				// Find first alpha character position
				while( firstAlphaPosition < printStringLength &&
				!isalpha( writeString[firstAlphaPosition] ) &&
				!isspace( writeString[firstAlphaPosition] ) )
					firstAlphaPosition++;

				// Convert first alpha character to upper case
				if( isFirstCharacterToUpperCase &&
				firstAlphaPosition < printStringLength &&

				( !isReturningToPosition ||
				currentPosition_ == 0 ) &&

				( promptTypeNr == PRESENTATION_PROMPT_WRITE ||
				promptTypeNr == PRESENTATION_PROMPT_WRITE_INDENTED ||
				promptTypeNr == PRESENTATION_PROMPT_WARNING ) )
					textString[firstAlphaPosition] = (char)toupper( textString[firstAlphaPosition] );

				if( promptString == NULL )
					{
					promptString = getPromptString( promptTypeNr );

					if( isForcingEmptyString &&
					printStringLength == 0 &&
					promptString != NULL )
						{
						currentPosition_ = ( strlen( promptString ) - 1 );
						addStringToOutput( isSkippingInTestFile, isError, false, promptString );
						}
					}

				while( i < printStringLength ||
				leftWidth > NO_CENTER_WIDTH )
					{
					if( currentPosition_ == 0 )
						{
						isPrintingPrompt = false;
						currentPosition_ = ( strlen( promptString ) - 1 );
						addStringToOutput( isSkippingInTestFile, isError, false, promptString );
						}

					if( i < printStringLength )
						{
						if( textString[i] == TAB_CHAR )
							length = ( TAB_LENGTH - ( currentPosition_ + 1 ) % TAB_LENGTH );
						else
							{
							length = 0;
							wordPosition = i;

							do	{
								i++;
								length++;
								}
							while( i < printStringLength &&
							!isspace( textString[i] ) &&
							!isBreakCharacter( textString[i] ) &&

							( !isSkippingInTestFile ||
							( strlen( promptString ) + length ) < NUMBER_OF_CONSOLE_COLUMNS ) );

							if( i < printStringLength &&
							isBreakCharacter( textString[i] ) &&

							( !isSkippingInTestFile ||
							( strlen( promptString ) + length ) < NUMBER_OF_CONSOLE_COLUMNS ) )
								{
								i++;
								length++;
								}

							if( i < printStringLength &&
							( textString[i] == NEW_LINE_CHAR ||
							textString[i] == CARRIAGE_RETURN_CHAR ) )
								length--;

							i = wordPosition;
							}

						if( i < printStringLength &&
						( textString[i] == NEW_LINE_CHAR ||
						textString[i] == CARRIAGE_RETURN_CHAR ||

						( isSkippingInTestFile &&
						( currentPosition_ + length + 1 ) >= NUMBER_OF_CONSOLE_COLUMNS ) ) )
							{
							if( textString[i] == NEW_LINE_CHAR ||
							textString[i] == CARRIAGE_RETURN_CHAR )
								{
								isPrintingPrompt = true;
								isStartingNewLine = true;

								addCharToOutput( isSkippingInTestFile, isError, ( i + 1 == printStringLength ), textString[i] );

								i++;
								}
							else
								{
								while( isspace( textString[i] ) )
									i++;

								addStringToOutput( isSkippingInTestFile, isError, ( i == printStringLength ), NEW_LINE_STRING );
								}

							currentPosition_ = 0;
							}
						}

					if( isStartingNewLine )
						isStartingNewLine = false;
					else
						{
						if( currentPosition_ == 0 )
							{
							addStringToOutput( isSkippingInTestFile, isError, false, ( isPrintingPrompt ? promptString : PRESENTATION_PROMPT_EMPTY_STRING ) );

							isPrintingPrompt = false;
							currentPosition_ = ( ( isPrintingPrompt && strlen( promptString ) > 0 ? strlen( promptString ) : strlen( PRESENTATION_PROMPT_EMPTY_STRING ) ) - 1 );
							}

						while( leftWidth > NO_CENTER_WIDTH )
							{
							leftWidth--;
							currentPosition_++;
							addStringToOutput( isSkippingInTestFile, isError, false, SPACE_STRING );
							}

						if( i < printStringLength )
							{
							if( textString[i] == TAB_CHAR )
								{
								addCharToOutput( isSkippingInTestFile, isError, ( i + 1 == printStringLength ), textString[i] );

								i++;
								currentPosition_ += length;
								}
							else
								{
								do	{
									addCharToOutput( isSkippingInTestFile, isError, ( i + 1 == printStringLength ), textString[i] );

									if( textString[i] == NEW_LINE_CHAR ||
									textString[i] == CARRIAGE_RETURN_CHAR )
										currentPosition_ = 0;
									else
										currentPosition_++;

									i++;
									}
								while( length-- > 1 );
								}
							}
						}
					}

				while( rightWidth-- > NO_CENTER_WIDTH )
					{
					currentPosition_++;
					addStringToOutput( isSkippingInTestFile, isError, ( ( i + 1 == printStringLength ) && rightWidth == NO_CENTER_WIDTH ), SPACE_STRING );
					}
				}
			else
				{
				sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tThe given text string is empty.\n", moduleNameString_, functionNameString );
				addStringToOutput( true, true, true, tempErrorString_ );
				commonVariables_->result = RESULT_ERROR;
				}
			}
		else
			{
			sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tThe given write string is undefined.\n", moduleNameString_, functionNameString );
			addStringToOutput( true, true, true, tempErrorString_ );
			commonVariables_->result = RESULT_ERROR;
			}

		return commonVariables_->result;
		}

	ResultType writeText( bool isSkippingInTestFile, bool isForcingEmptyString, bool isFirstCharacterToUpperCase, bool isReturningToPosition, unsigned short promptTypeNr, size_t queryWidth, const char *promptString, const char *textString )
		{
		size_t widthDifference;
		size_t textStringLength;
		size_t leftWidth = NO_CENTER_WIDTH;
		size_t rightWidth = NO_CENTER_WIDTH;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeText";

		if( textString != NULL )
			{
			if( isForcingEmptyString ||
			strlen( textString ) > 0 ||
			queryWidth > NO_CENTER_WIDTH )
				{
				clearStatus();
				clearProgress();
				textStringLength = strlen( textString );

				if( promptTypeNr == PRESENTATION_PROMPT_WARNING )
					commonVariables_->hasShownWarning = true;
				else
					commonVariables_->hasShownMessage = true;

				if( queryWidth > NO_CENTER_WIDTH &&
				queryWidth < textStringLength )
					textStringLength = queryWidth;

				if( queryWidth >= textStringLength )
					{
					widthDifference = ( queryWidth - textStringLength );
					leftWidth = ( widthDifference / 2 );
					rightWidth = ( widthDifference - leftWidth );
					}

				if( writeText( isSkippingInTestFile, isForcingEmptyString, false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, leftWidth, rightWidth, textStringLength, promptString, textString ) == RESULT_OK )
					lastUsedPromptTypeNr_ = ( promptTypeNr == PRESENTATION_PROMPT_WARNING_INDENTED ? PRESENTATION_PROMPT_WARNING : ( promptTypeNr == PRESENTATION_PROMPT_WRITE_INDENTED ? PRESENTATION_PROMPT_WRITE : promptTypeNr ) );
				else
					{
					sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the string.\n", moduleNameString_, functionNameString );
					addStringToOutput( true, true, true, tempErrorString_ );
					}
				}
			else
				{
				showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The given write string is empty" );
				commonVariables_->result = RESULT_ERROR;
				}
			}
		else
			{
			showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The given write string is undefined" );
			commonVariables_->result = RESULT_ERROR;
			}

		return commonVariables_->result;
		}

	const char *getPromptString( unsigned short promptTypeNr )
		{
		switch( promptTypeNr )
			{
			case PRESENTATION_PROMPT_READ:
				return PRESENTATION_PROMPT_READ_STRING;

			case PRESENTATION_PROMPT_WRITE:
				return PRESENTATION_PROMPT_WRITE_STRING;

			case PRESENTATION_PROMPT_WRITE_INDENTED:
				return PRESENTATION_PROMPT_WRITE_INDENTED_STRING;

			case PRESENTATION_PROMPT_INFO:
				return PRESENTATION_PROMPT_INFO_STRING;

			case PRESENTATION_PROMPT_NOTIFICATION:
				return PRESENTATION_PROMPT_NOTIFICATION_STRING;

			case PRESENTATION_PROMPT_WARNING:
				return PRESENTATION_PROMPT_WARNING_STRING;

			case PRESENTATION_PROMPT_WARNING_INDENTED:
				return PRESENTATION_PROMPT_WARNING_INDENTED_STRING;
			}

		return PRESENTATION_PROMPT_QUERY_STRING;
		}


	protected:
	// Constructor / deconstructor

	Presentation( CommonVariables *commonVariables )
		{
		commonVariables_ = commonVariables;

		hasReadLine_ = false;
		isShowingExtraPromptLine_ = false;

		lastShownInterfaceParameter_ = NO_INTERFACE_PARAMETER;
		lastUsedPromptTypeNr_ = PRESENTATION_PROMPT_QUERY;

		currentPosition_ = 0;
		progressLength_ = 0;
		statusLength_ = 0;

		maximumProgress_ = 0;
		previousProgress_ = 0;

		strcpy( currentProgressString_, EMPTY_STRING );
		strcpy( currentStatusString_, EMPTY_STRING );
		strcpy( errorString_, EMPTY_STRING );
		strcpy( outputString_, EMPTY_STRING );
		strcpy( tempErrorString_, EMPTY_STRING );

		strcpy( moduleNameString_, "Presentation" );

		testFile_ = NULL;

		if( commonVariables_ == NULL )
			{
			sprintf( tempErrorString_, "\nClass:%s\nFunction:\t%s\nError:\tThe given common variables is undefined.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			addStringToOutput( true, true, true, tempErrorString_ );
			}
		}


	// Protected functions

	void redirectOutputToTestFile( FILE *testFile )
		{
		testFile_ = testFile;
		}

	void clearStatus()
		{
		if( statusLength_ > 0 )
			{
			addStringToOutput( true, false, false, CARRIAGE_RETURN_STRING );

			for( size_t i = 0; i < statusLength_; i++ )
				addStringToOutput( true, false, false, SPACE_STRING );

			addStringToOutput( true, false, true, CARRIAGE_RETURN_STRING );

			statusLength_ = 0;
			currentPosition_ = 0;
			strcpy( currentStatusString_, EMPTY_STRING );
			}
		}

	void clearProgress()
		{
		if( progressLength_ > 0 )
			{
			addStringToOutput( true, false, false, CARRIAGE_RETURN_STRING );

			for( unsigned int i = 0; i + 1 < NUMBER_OF_CONSOLE_COLUMNS; i++ )
				addStringToOutput( true, false, false, SPACE_STRING );

			addStringToOutput( true, false, true, CARRIAGE_RETURN_STRING );

			progressLength_ = 0;
			currentPosition_ = 0;
			previousProgress_ = 0;
			strcpy( currentProgressString_, EMPTY_STRING );
			}
		}

	void showError( char functionListChar, const char *classNameString, const char *superClassNameString, const char *wordNameString, const char *functionString, const char *errorString )
		{
		ResultType originalErrorResult = commonVariables_->result;

		strcpy( errorString_, EMPTY_STRING );

		// First error message
		if( commonVariables_->result == RESULT_OK )
			{
			sprintf( tempErrorString_, "%s%c%u%c%u%c%s", PRESENTATION_ERROR_CURRENT_ID_START_STRING, QUERY_ITEM_START_CHAR, commonVariables_->currentSentenceNr, QUERY_SEPARATOR_CHAR, commonVariables_->currentItemNr, QUERY_ITEM_END_CHAR, PRESENTATION_ERROR_CURRENT_ID_END_STRING );
			strcat( errorString_, tempErrorString_ );
			}

		if( classNameString != NULL )
			{
			strcat( errorString_, PRESENTATION_ERROR_CLASS_STRING );
			strcat( errorString_, classNameString );
			}

		if( superClassNameString != NULL )
			{
			strcat( errorString_, PRESENTATION_ERROR_SUPERCLASS_STRING );
			strcat( errorString_, superClassNameString );
			}

		if( functionString != NULL )
			{
			strcat( errorString_, PRESENTATION_ERROR_FUNCTION_STRING );
			strcat( errorString_, functionString );
			}

		if( functionListChar != SYMBOL_QUESTION_MARK )
			{
			sprintf( tempErrorString_, "%s%c%s", PRESENTATION_ERROR_FUNCTION_LIST_START_STRING, functionListChar, PRESENTATION_ERROR_FUNCTION_LIST_END_STRING );
			strcat( errorString_, tempErrorString_ );
			}

		if( wordNameString != NULL )
			{
			strcat( errorString_, PRESENTATION_ERROR_FUNCTION_WORD_START_STRING );
			strcat( errorString_, wordNameString );
			strcat( errorString_, PRESENTATION_ERROR_FUNCTION_WORD_END_STRING );
			}

		if( errorString != NULL )
			{
			strcat( errorString_, PRESENTATION_ERROR_STRING );
			strcat( errorString_, errorString );
			}

		strcat( errorString_, COLON_STRING );

		originalErrorResult = commonVariables_->result;
		commonVariables_->result = RESULT_OK;

		if( writeText( true, false, true, true, PRESENTATION_PROMPT_WARNING, NO_CENTER_WIDTH, NULL, errorString_ ) != RESULT_OK )
			{
			sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the error.\n", moduleNameString_, functionString );
			addStringToOutput( true, true, true, tempErrorString_ );
			}

		commonVariables_->result = originalErrorResult;
		}

	void showStatus( unsigned short interfaceParameter )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];

		if( commonVariables_->currentLanguageWordItem == NULL )
			showStatus( NO_LANGUAGE_WORD_FOUND );
		else
			{
			sprintf( interfaceString, "%s...", commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter ) );
			showStatus( interfaceString );
			}
		}

	void showStatus( const char *newStatusString )
		{
		size_t oldStatusLength = statusLength_;
		char outputString[MAX_SENTENCE_STRING_LENGTH];
		char statusString[MAX_SENTENCE_STRING_LENGTH];

		if( progressLength_ == 0 )
			{
			if( newStatusString == NULL )
				clearStatus();
			else
				{
				if( ( statusLength_ = strlen( newStatusString ) ) < NUMBER_OF_CONSOLE_COLUMNS )
					strcpy( statusString, newStatusString );
				else
					{
					statusLength_ = ( NUMBER_OF_CONSOLE_COLUMNS - 1 );
					strcpy( statusString, EMPTY_STRING );
					strncat( statusString, newStatusString, statusLength_ );
					}

				// Different string
				if( strcmp( statusString, currentStatusString_ ) != 0 )
					{
					if( currentPosition_ > 0 )
						returnOutputToPosition( false, false, true );

					sprintf( outputString, "\r%s", statusString );
					addStringToOutput( true, false, false, outputString );

					for( size_t i = statusLength_; i < oldStatusLength; i++ )
						addStringToOutput( true, false, false, SPACE_STRING );

					addStringToOutput( true, false, true, CARRIAGE_RETURN_STRING );
					strcpy( currentStatusString_, statusString );
					}
				}
			}
		}

	void showProgress( unsigned int currentProgress )
		{
		char outputString[MAX_SENTENCE_STRING_LENGTH];
		unsigned int progress;

		if( progressLength_ > 0 )
			{
			if( currentProgress > maximumProgress_ )
				currentProgress = maximumProgress_;

			progress = (unsigned int)( (long double)( progressLength_ ) * (long double)( currentProgress ) / maximumProgress_ );

			if( currentProgress == 0 ||
			progress < previousProgress_ )
				{
				previousProgress_ = 0;
				sprintf( outputString, "\r%s: ", currentProgressString_ );
				addStringToOutput( true, false, false, outputString );

				for( size_t i = 0; i < progressLength_; i++ )
					addStringToOutput( true, false, false, COLON_STRING );

				sprintf( outputString, "\r%s: ", currentProgressString_ );
				addStringToOutput( true, false, ( progress <= previousProgress_ ), outputString );
				}

			// Only show changes
			if( progress > previousProgress_ )
				{
				for( unsigned int i = previousProgress_; i < progress; i++ )
					addStringToOutput( true, false, ( i + 1 == progress ), ASTERISK_STRING );
				}

			previousProgress_ = progress;
			}
		}

	void startProgress( unsigned int startProgress, unsigned int maxProgress, unsigned short interfaceParameter1, unsigned short shortNumber, unsigned short interfaceParameter2 )
		{
		char newProgressString[MAX_SENTENCE_STRING_LENGTH];

		if( statusLength_ > 0 )
			clearStatus();

		if( commonVariables_->currentLanguageWordItem == NULL )
			strcpy( newProgressString, NO_LANGUAGE_WORD_FOUND );
		else
			{
			sprintf( newProgressString, "%s%u%s", commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ), shortNumber, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ) );
			showStatus( newProgressString );
			}

		strcpy( currentProgressString_, EMPTY_STRING );
		strncat( currentProgressString_, newProgressString, NUMBER_OF_CONSOLE_COLUMNS - 4 );

		maximumProgress_ = maxProgress;
		progressLength_ = ( NUMBER_OF_CONSOLE_COLUMNS - 3 - strlen( currentProgressString_ ) );

		if( currentPosition_ > 0 )
			returnOutputToPosition( false, false, true );

		showProgress( startProgress );
		}

	bool hasReadLine()
		{
		return hasReadLine_;
		}

	char convertDiacriticalChar( char diacriticalChar )
		{
		switch( diacriticalChar )
			{
			case TEXT_TAB_CHAR:
				return TAB_CHAR;

			case TEXT_BELL_CHAR:
				return BELL_CHAR;

			case SYMBOL_DOUBLE_QUOTE:
				return SYMBOL_DOUBLE_QUOTE;

			case TEXT_BACK_SPACE_CHAR:
				return BACK_SPACE_CHAR;

			case TEXT_NEW_LINE_CHAR:
				return NEW_LINE_CHAR;

			// To avoid triggering of query
			case QUERY_CHAR:
				return QUERY_CHAR;
			}

		return diacriticalChar;
		}

	ResultType readLine( bool isPassword, bool isQuestion, bool isText, bool isShowingLine, char *promptString, char *readString, FILE *readFile )
		{
		size_t passwordLength;
		size_t tempLength;
		char inputString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char passwordString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char outputString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
#ifdef _MSC_VER
		// MS Visual Studio
		wchar_t inputWideString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_WIDE_STRING;
#endif
		char functionNameString[FUNCTION_NAME_LENGTH] = "readLine";

		hasReadLine_ = false;
		lastShownInterfaceParameter_ = NO_INTERFACE_PARAMETER;

		if( readString != NULL )
			{
			if( isText ||
			isQuestion )
				strcat( outputString, PRESENTATION_PROMPT_WRITE_STRING );

			if( promptString != NULL )
				{
				tempLength = strlen( outputString );
				strcat( outputString, promptString );

				if( isQuestion &&
				strlen( promptString ) > 0 &&
				isalpha( promptString[0] ) )
					outputString[tempLength] = (char)toupper( outputString[tempLength] );
				}

			strcat( outputString, ( isQuestion ? PRESENTATION_PROMPT_QUERY_STRING : ( isText ? PRESENTATION_PROMPT_ENTER_STRING : PRESENTATION_PROMPT_READ_STRING ) ) );

			if( currentPosition_ > 0 )
				returnOutputToPosition( false, false, true );

			if( isShowingExtraPromptLine_ )
				{
				if( writeText( false, false, true, true, lastUsedPromptTypeNr_, NO_CENTER_WIDTH, NULL, NEW_LINE_STRING ) == RESULT_OK )
					isShowingExtraPromptLine_ = false;
				else
					{
					sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the text.\n", moduleNameString_, functionNameString );
					addStringToOutput( true, true, true, tempErrorString_ );
					}
				}

			if( commonVariables_->result == RESULT_OK )
				{
				if( isPassword ||
				readFile == NULL )
					{
					currentPosition_ = 0;
					addStringToOutput( false, false, true, outputString );

					if( isPassword )
						{
						passwordLength = 0;
						strcpy( inputString, EMPTY_STRING );

						do	{
#ifdef _MSC_VER
							// MS Visual Studio
							if( ( passwordString[passwordLength] = (char)_getch() ) != CARRIAGE_RETURN_CHAR )
#else
							// For compilers other than MS Visual Studio
							if( ( passwordString[passwordLength] = (char)getchar() ) != NEW_LINE_CHAR )
#endif
								{
								if( passwordString[passwordLength] == BACK_SPACE_CHAR )
									{
									if( passwordLength == 0 )
										addStringToOutput( false, false, true, BELL_STRING );
									else
										{
										tempLength = 0;
										currentPosition_--;
										addStringToOutput( false, false, false, CARRIAGE_RETURN_STRING );
										addStringToOutput( false, false, false, outputString );

										while( tempLength++ < passwordLength )
											addStringToOutput( false, false, false, SPACE_STRING );

										passwordLength--;
										tempLength = 0;
										addStringToOutput( false, false, false, CARRIAGE_RETURN_STRING );
										addStringToOutput( false, false, false, outputString );

										while( tempLength++ < passwordLength )
											addStringToOutput( false, false, ( tempLength == passwordLength ), ASTERISK_STRING );
										}
									}
								else
									{
									currentPosition_++;
									passwordLength++;
									addStringToOutput( false, false, true, ASTERISK_STRING );
									}
								}
							}
						while( passwordString[passwordLength] != CARRIAGE_RETURN_CHAR );

						hasReadLine_ = true;
						strncat( inputString, passwordString, passwordLength );
						}
					else
						{
						// Input from keyboard
						if( fgets( inputString, MAX_SENTENCE_STRING_LENGTH, stdin ) != NULL )
							{
							hasReadLine_ = true;
							stripStartAndEndSpaces( inputString, readString );
							}
						else
							{
							sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to get an input string.\n", moduleNameString_, functionNameString );
							addStringToOutput( true, true, true, tempErrorString_ );
							commonVariables_->result = RESULT_ERROR;
							}
						}
					}
				else
					{
					// Input from file
#ifdef _MSC_VER
					// MS Visual Studio
					if( fgetws( inputWideString, MAX_SENTENCE_STRING_LENGTH, readFile ) != NULL )
#else
					// Compilers other than MS Visual Studio
					if( fgets( inputString, MAX_SENTENCE_STRING_LENGTH, readFile ) != NULL )
#endif
						{
						hasReadLine_ = true;

#ifdef _MSC_VER
						// MS Visual Studio
						sprintf( inputString, "%S", &inputWideString );
#else
						// Compilers other than MS Visual Studio
						// Check for possible BOM marker from first line
						if( strlen( inputString ) > 2 &&
						(unsigned short)inputString[0] == FILE_UTF_8_BOM_CHAR_1 &&
						(unsigned short)inputString[1] == FILE_UTF_8_BOM_CHAR_2 &&
						(unsigned short)inputString[2] == FILE_UTF_8_BOM_CHAR_3 )
							// Remove BOM marker
							strcpy( inputString, &inputString[FILE_UTF_8_BOM_LENGTH] );
#endif

						stripStartAndEndSpaces( inputString, readString );

						// Strip C++/Java comment from line
						if( strncmp( readString, PRESENTATION_STRIP_COMMENT_STRING, strlen( PRESENTATION_STRIP_COMMENT_STRING ) ) == 0 )
							strcpy( readString, &readString[ strlen( PRESENTATION_STRIP_COMMENT_STRING ) ] );

						if( isShowingLine &&
						// Skip Java comment line
						strncmp( readString, PRESENTATION_SKIP_COMMENT_STRING, strlen( PRESENTATION_SKIP_COMMENT_STRING ) ) != 0 )
							{
							if( writeText( false, false, true, true, PRESENTATION_PROMPT_READ, NO_CENTER_WIDTH, outputString, ( strlen( readString ) == 0 ? NEW_LINE_STRING : readString ) ) != RESULT_OK )
								{
								sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the text.\n", moduleNameString_, functionNameString );
								addStringToOutput( true, true, true, tempErrorString_ );
								}
							}
						}
					}
				}
			}
		else
			{
			sprintf( tempErrorString_, "\nClass:\t%s\nFunction:\t%s\nError:\tThe given read string is undefined.\n", moduleNameString_, functionNameString );
			addStringToOutput( true, true, true, tempErrorString_ );
			commonVariables_->result = RESULT_ERROR;
			}

		return commonVariables_->result;
		}

	ResultType writeDiacriticalText( unsigned short promptTypeNr, const char *diacriticalTextString )
		{
		return writeDiacriticalText( true, true, promptTypeNr, diacriticalTextString );
		}

	ResultType writeDiacriticalText( bool isFirstCharacterToUpperCase, bool isReturningToPosition, unsigned short promptTypeNr, const char *diacriticalTextString )
		{
		bool hasFoundNewLine = false;
		size_t position = 0;
		char textChar = SYMBOL_QUESTION_MARK;
		char charString[2] = SPACE_STRING;
		char textString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeDiacriticalText";

		if( diacriticalTextString != NULL )
			{
			isShowingExtraPromptLine_ = true;

			if( diacriticalTextString[0] == SYMBOL_DOUBLE_QUOTE )
				position++;

			while( commonVariables_->result == RESULT_OK &&
			position < strlen( diacriticalTextString ) &&
			diacriticalTextString[position] != SYMBOL_DOUBLE_QUOTE )
				{
				if( diacriticalTextString[position] == TEXT_DIACRITICAL_CHAR )
					{
					if( ++position < strlen( diacriticalTextString ) )
						{
						if( ( textChar = convertDiacriticalChar( diacriticalTextString[position] ) ) == NEW_LINE_CHAR )
							hasFoundNewLine = true;
						}
					else
						{
						showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The text string ended with a diacritical sign" );
						commonVariables_->result = RESULT_ERROR;
						}
					}
				else
					textChar = diacriticalTextString[position];

				position++;
				charString[0] = textChar;
				strcat( textString, charString );

				if( hasFoundNewLine ||
				strlen( textString ) + 1 == MAX_SENTENCE_STRING_LENGTH ||

				( strlen( textString ) > 0 &&
				position < strlen( diacriticalTextString ) &&
				diacriticalTextString[position] != SYMBOL_DOUBLE_QUOTE &&
				diacriticalTextString[position] == QUERY_CHAR ) )
					{
					if( writeText( false, false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, NO_CENTER_WIDTH, NULL, textString ) == RESULT_OK )
						{
						hasFoundNewLine = false;
						strcpy( textString, EMPTY_STRING );
						}
					else
						{
						sprintf( tempErrorString_, "\nClass:%s\nFunction:\t%s\nError:\tI failed to write a text string part.\n", moduleNameString_, functionNameString );
						addStringToOutput( true, true, true, tempErrorString_ );
						}
					}
				}

			if( commonVariables_->result == RESULT_OK &&
			strlen( textString ) > 0 )
				{
				if( writeText( false, false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, NO_CENTER_WIDTH, NULL, textString ) != RESULT_OK )
					{
					sprintf( tempErrorString_, "\nClass:%s\nFunction:\t%s\nError:\tI failed to write the last text string part.\n", moduleNameString_, functionNameString );
					addStringToOutput( true, true, true, tempErrorString_ );
					}
				}
			}
		else
			{
			showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The given diacritical text string is undefined" );
			commonVariables_->result = RESULT_ERROR;
			}

		return commonVariables_->result;
		}

	ResultType writeInterfaceText( bool isCheckingForDuplicateInterfaceParameter, unsigned short promptTypeNr, unsigned short interfaceParameter )
		{
		return writeInterfaceText( isCheckingForDuplicateInterfaceParameter, true, promptTypeNr, interfaceParameter );
		}

	ResultType writeInterfaceText( bool isCheckingForDuplicateInterfaceParameter, bool isReturningToPosition, unsigned short promptTypeNr, unsigned short interfaceParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			if( !isCheckingForDuplicateInterfaceParameter ||
			interfaceParameter != lastShownInterfaceParameter_ )
				{
				lastShownInterfaceParameter_ = interfaceParameter;
				return writeDiacriticalText( true, isReturningToPosition, promptTypeNr, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter ) );
				}
			}
		else
			{
			showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
			commonVariables_->result = RESULT_ERROR;
			}

		return commonVariables_->result;
		}

	ResultType writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, unsigned int intNumber, unsigned short interfaceParameter2 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			sprintf( interfaceString, "%s%u%s", commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ), intNumber, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ) );
			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
		commonVariables_->result = RESULT_ERROR;
		return commonVariables_->result;
		}

	ResultType writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, unsigned int intNumber, unsigned short interfaceParameter2, char *textString, unsigned short interfaceParameter3 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			sprintf( interfaceString, "%s%u%s%s%s", commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ), intNumber, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ), textString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter3 ) );
			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
		commonVariables_->result = RESULT_ERROR;
		return commonVariables_->result;
		}

	ResultType writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, unsigned int intNumber1, unsigned short interfaceParameter2, unsigned int intNumber2, unsigned short interfaceParameter3, char *textString, unsigned short interfaceParameter4 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			sprintf( interfaceString, "%s%u%s%u%s%s%s", commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ), intNumber1, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ), intNumber2, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter3 ), textString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter4 ) );
			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
		commonVariables_->result = RESULT_ERROR;
		return commonVariables_->result;
		}

	ResultType writeInterfaceText( bool isCheckingForDuplicateInterfaceParameter, unsigned short promptTypeNr, unsigned short interfaceParameter1, const char *textString, unsigned short interfaceParameter2 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			if( !isCheckingForDuplicateInterfaceParameter ||
			interfaceParameter1 != lastShownInterfaceParameter_ )
				{
				lastShownInterfaceParameter_ = interfaceParameter1;

				strcpy( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ) );
				strcat( interfaceString, textString );
				strcat( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ) );

				return writeDiacriticalText( promptTypeNr, interfaceString );
				}
			}
		else
			{
			showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
			commonVariables_->result = RESULT_ERROR;
			}

		return commonVariables_->result;
		}

	ResultType writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, char *textString, unsigned short interfaceParameter2, unsigned int longNumber, unsigned short interfaceParameter3 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			sprintf( interfaceString, "%s%s%s%u%s", commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ), textString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ), longNumber, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter3 ) );
			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
		commonVariables_->result = RESULT_ERROR;
		return commonVariables_->result;
		}

	ResultType writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, const char *textString, unsigned short interfaceParameter2 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			strcpy( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ) );
			strcat( interfaceString, textString );
			strcat( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ) );

			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
		commonVariables_->result = RESULT_ERROR;
		return commonVariables_->result;
		}

	ResultType writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, const char *textString1, unsigned short interfaceParameter2, const char *textString2, unsigned short interfaceParameter3 )
		{
		char interfaceString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInterfaceText";

		if( commonVariables_->currentLanguageWordItem != NULL )
			{
			strcpy( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter1 ) );
			strcat( interfaceString, textString1 );
			strcat( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter2 ) );
			strcat( interfaceString, textString2 );
			strcat( interfaceString, commonVariables_->currentLanguageWordItem->interfaceString( interfaceParameter3 ) );

			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		showError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The current language word item is undefined" );
		commonVariables_->result = RESULT_ERROR;
		return commonVariables_->result;
		}

	ResultType writeText( unsigned short promptTypeNr, const char *textString1, const char *textString2 )
		{
		isShowingExtraPromptLine_ = true;

		if( writeText( false, false, true, true, promptTypeNr, NO_CENTER_WIDTH, NULL, textString1 ) == RESULT_OK )
			{
			if( textString2 != NULL &&
			strlen( textString2 ) > 0 )
				return writeText( false, false, true, false, promptTypeNr, NO_CENTER_WIDTH, NULL, textString2 );
			}

		return commonVariables_->result;
		}

	ResultType writeText( bool isReturningToPosition, unsigned short promptTypeNr, size_t queryWidth, const char *textString )
		{
		return writeText( false, false, true, isReturningToPosition, promptTypeNr, queryWidth, NULL, textString );
		}

	ResultType writeText( bool isSkippingInTestFile, bool isReturningToPosition, unsigned short promptTypeNr, size_t queryWidth, const char *textString )
		{
		return writeText( isSkippingInTestFile, false, true, isReturningToPosition, promptTypeNr, queryWidth, NULL, textString );
		}
	};

#endif

/*************************************************************************
 *	"The voice of the Lord twists mighty oaks
 *	and strips the forest bare.
 *	In his temple everyone shouts, "Glory!"." (Psalm 29:9)
 *************************************************************************/
