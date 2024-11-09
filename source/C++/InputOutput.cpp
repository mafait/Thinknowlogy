/*	Class:		InputOutput
 *	Purpose:	To read and write user information
 *	Version:	Thinknowlogy 2024 (Intelligent Origin)
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

#ifndef INPUT_OUTPUT
#define INPUT_OUTPUT 1

#include <ctype.h>
#ifdef _MSC_VER
// MS Visual Studio
// Defines _getch()
#include <conio.h>
#endif
#include "WordItem.h"

class InputOutput
	{
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class AdminReasoning;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class ContextList;
	friend class GrammarList;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordTypeItem;
	friend class WordTypeList;
	friend class WordWrite;

	// Private constructed variables

	bool hasReadLine_ = false;
	bool isDisplayingExtraPromptLine_ = false;

	unsigned short lastDisplayedInterfaceParameter_ = NO_INTERFACE_PARAMETER;
	unsigned short lastUsedPromptTypeNr_ = INPUT_OUTPUT_PROMPT_QUERY;

	size_t currentPosition_ = 0;
	size_t progressBarLength_ = 0;
	size_t progressTextStringLength_ = 0;

	unsigned int maximumProgress_ = 0;
	unsigned int previousProgress_ = 0;

	char currentProgressString_[NUMBER_OF_CONSOLE_COLUMNS] = EMPTY_STRING;
	char currentStatusString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char outputString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char tempString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;

	char charString_[2] = SPACE_STRING;
	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "InputOutput";

	FILE *testFile_ = NULL;

	// Private initialized variables

	GlobalVariables *globalVariables_ = NULL;
	WordItem *adminWordItem_ = NULL;


	// Private functions

	void clearProgressText()
		{
		char outputString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;

		if( progressTextStringLength_ > 0 )
			{
			strcat( outputString, CARRIAGE_RETURN_STRING );

			for( size_t i = 0; i < progressTextStringLength_; i++ )
				strcat( outputString, SPACE_STRING );

			strcat( outputString, CARRIAGE_RETURN_STRING );

			progressTextStringLength_ = 0;
			currentPosition_ = 0;
			strcpy( currentStatusString_, EMPTY_STRING );

			addStringToOutput( true, false, true, outputString );
			}
		}

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
			{
			if( ( strlen( outputString_ ) + strlen( writeString ) ) < SENTENCE_STRING_LENGTH )
				strcat( outputString_, writeString );
			else
				{
				// Flush current output string
				addStringToOutput( isSkippingInTestFile, isError, true, EMPTY_STRING );
				strcpy( outputString_, writeString );
				}
			}

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

		if( isError &&
		isWriting )
			{
			if( isSkippingInTestFile ||
			testFile_ == NULL )
				fprintf( stderr, "%s", writeString );
			else
#ifdef _MSC_VER
				// MS Visual Studio
				fwprintf( testFile_, L"%S", writeString );
#else
				// For compilers other than MS Visual Studio
				fprintf( testFile_, "%s", writeString );
#endif
			}
		}

	void stripStartAndEndSpaces( char *inputString, char *outputString )
		{
		size_t endPosition;
		size_t startPosition = 0;

		if( inputString != NULL &&
		outputString != NULL &&
		( endPosition = strlen( inputString ) ) > 0 )
			{
			// Skip start spaces of the input string
			while( startPosition < endPosition &&
			isspace( inputString[startPosition] ) )
				startPosition++;

			endPosition = strlen( inputString );

			// Skip 'new line' and 'carriage return' character
			// at the end of the input string
			while( startPosition < endPosition &&

			( inputString[endPosition - 1] == NEW_LINE_CHAR ||
			inputString[endPosition - 1] == CARRIAGE_RETURN_CHAR ) )
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
				character == SYMBOL_QUESTION_MARK ||
#ifdef _MSC_VER
				character == SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK ||
				character == SYMBOL_SPANISH_INVERTED_QUESTION_MARK ||
#endif
				character == SYMBOL_SLASH ||
				character == SYMBOL_BACK_SLASH ||
				character == SYMBOL_CLOSE_ROUNDED_BRACKET ||
				character == SYMBOL_CLOSE_CURVED_BRACKET ||
				character == SYMBOL_CLOSE_HOOKED_BRACKET ||
				character == SYMBOL_CLOSE_SQUARE_BRACKET );
		}

	signed char writeText( bool isSkippingInTestFile, bool isError, bool isFirstCharacterToUpperCase, bool isReturningToPosition, unsigned short promptTypeNr, size_t leftWidth, size_t rightWidth, size_t printStringLength, const char *promptString, const char *writeString )
		{
		bool isPrintingPrompt = true;
		bool isStartingNewLine = false;
		size_t firstAlphaPosition = 0;
		size_t i = 0;
		size_t length = 0;
		size_t promptStringLength;
		size_t wordPosition;
		char outputString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char textString[SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeText";

		if( writeString != NULL )
			{
			if( printStringLength > 0 ||
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

				( promptTypeNr == INPUT_OUTPUT_PROMPT_WRITE ||
				promptTypeNr == INPUT_OUTPUT_PROMPT_WRITE_INDENTED ||
				promptTypeNr == INPUT_OUTPUT_PROMPT_WARNING ) )
					textString[firstAlphaPosition] = (char)toupper( textString[firstAlphaPosition] );

				if( promptString == NULL )
					promptString = getPromptString( promptTypeNr );

				while( i < printStringLength ||
				leftWidth > NO_CENTER_WIDTH )
					{
					if( currentPosition_ == 0 &&
					promptString != NULL &&
					( promptStringLength = strlen( promptString ) ) > 0 )
						{
						isPrintingPrompt = false;
						currentPosition_ = ( promptStringLength - 1 );
						strcat( outputString, promptString );
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
							!isBreakCharacter( textString[i] ) );

							if( i < printStringLength &&

							( textString[i] == NEW_LINE_CHAR ||
							textString[i] == CARRIAGE_RETURN_CHAR ) )
								length--;

							i = wordPosition;
							}

						if( i < printStringLength &&

						( textString[i] == NEW_LINE_CHAR ||
						textString[i] == CARRIAGE_RETURN_CHAR ) )
							{
							if( textString[i] == NEW_LINE_CHAR ||
							textString[i] == CARRIAGE_RETURN_CHAR )
								{
								isPrintingPrompt = true;
								isStartingNewLine = true;
								charString_[0] = textString[i];
								strcat( outputString, charString_ );
								i++;
								}
							else
								{
								while( isspace( textString[i] ) )
									i++;

								strcat( outputString, NEW_LINE_STRING );
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
							if( !isPrintingPrompt ||
							promptString != NULL )
								strcat( outputString, ( isPrintingPrompt ? promptString : INPUT_OUTPUT_PROMPT_EMPTY_STRING ) );

							isPrintingPrompt = false;
							currentPosition_ = ( ( isPrintingPrompt &&
												( promptStringLength = strlen( promptString ) ) > 0 ? promptStringLength : strlen( INPUT_OUTPUT_PROMPT_EMPTY_STRING ) ) - 1 );
							}

						while( leftWidth > NO_CENTER_WIDTH )
							{
							leftWidth--;
							currentPosition_++;
							strcat( outputString, SPACE_STRING );
							}

						if( i < printStringLength )
							{
							if( textString[i] == TAB_CHAR )
								{
								charString_[0] = textString[i];
								strcat( outputString, charString_ );
								i++;
								currentPosition_ += length;
								}
							else
								{
								do	{
									charString_[0] = textString[i];
									strcat( outputString, charString_ );

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
					strcat( outputString, SPACE_STRING );
					}

				addStringToOutput( isSkippingInTestFile, isError, true, outputString );
				}
			else
				{
				sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tThe given text string is empty.\n", moduleNameString_, functionNameString );
				addStringToOutput( false, true, true, tempString_ );
				globalVariables_->result = RESULT_ERROR;
				}
			}
		else
			{
			sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tThe given write string is undefined.\n", moduleNameString_, functionNameString );
			addStringToOutput( false, true, true, tempString_ );
			globalVariables_->result = RESULT_ERROR;
			}

		return globalVariables_->result;
		}

	signed char writeText( bool isSkippingInTestFile, bool isError, bool isFirstCharacterToUpperCase, bool isReturningToPosition, unsigned short promptTypeNr, size_t queryWidth, const char *promptString, const char *textString )
		{
		size_t leftWidth = NO_CENTER_WIDTH;
		size_t rightWidth = NO_CENTER_WIDTH;
		size_t textStringLength;
		size_t widthDifference;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeText";

		if( textString != NULL )
			{
			if( ( textStringLength = strlen( textString ) ) > 0 ||
			queryWidth > NO_CENTER_WIDTH )
				{
				clearProgress();
				clearProgressText();

				if( promptTypeNr == INPUT_OUTPUT_PROMPT_WARNING )
					globalVariables_->hasDisplayedWarning = true;
				else
					{
					if( promptTypeNr == INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY )
						globalVariables_->hasDisplayedIntegrityWarning = true;
					else
						globalVariables_->hasDisplayedMessage = true;
					}

				if( queryWidth > NO_CENTER_WIDTH &&
				queryWidth < textStringLength )
					textStringLength = queryWidth;

				if( queryWidth >= textStringLength )
					{
					widthDifference = ( queryWidth - textStringLength );
					leftWidth = ( widthDifference / 2 );
					rightWidth = ( widthDifference - leftWidth );
					}

				if( writeText( isSkippingInTestFile, isError, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, leftWidth, rightWidth, textStringLength, promptString, textString ) == RESULT_OK )
					lastUsedPromptTypeNr_ = ( promptTypeNr == INPUT_OUTPUT_PROMPT_WARNING_INDENTED ? INPUT_OUTPUT_PROMPT_WARNING : ( promptTypeNr == INPUT_OUTPUT_PROMPT_WRITE_INDENTED ? INPUT_OUTPUT_PROMPT_WRITE : promptTypeNr ) );
				else
					{
					sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the string.\n", moduleNameString_, functionNameString );
					addStringToOutput( false, true, true, tempString_ );
					}
				}
			else
				globalVariables_->result = displayError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The given write string is empty" );
			}
		else
			globalVariables_->result = displayError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The given write string is undefined" );

		return globalVariables_->result;
		}

	const char *getPromptString( unsigned short promptTypeNr )
		{
		switch( promptTypeNr )
			{
			case INPUT_OUTPUT_PROMPT_READ:
				return INPUT_OUTPUT_PROMPT_READ_STRING;

			case INPUT_OUTPUT_PROMPT_WRITE:
				return INPUT_OUTPUT_PROMPT_WRITE_STRING;

			case INPUT_OUTPUT_PROMPT_WRITE_INDENTED:
				return INPUT_OUTPUT_PROMPT_WRITE_INDENTED_STRING;

			case INPUT_OUTPUT_PROMPT_INFO:
				return INPUT_OUTPUT_PROMPT_INFO_STRING;

			case INPUT_OUTPUT_PROMPT_NOTIFICATION:
				return INPUT_OUTPUT_PROMPT_NOTIFICATION_STRING;

			case INPUT_OUTPUT_PROMPT_WARNING:
			case INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY:
				return INPUT_OUTPUT_PROMPT_WARNING_STRING;

			case INPUT_OUTPUT_PROMPT_WARNING_INDENTED:
				return INPUT_OUTPUT_PROMPT_WARNING_INDENTED_STRING;
			}

		return INPUT_OUTPUT_PROMPT_QUERY_STRING;
		}


	protected:
	// Constructor

	InputOutput( GlobalVariables *globalVariables, WordItem *adminWordItem )
		{
		// Private initialized variables

		if( ( globalVariables_ = globalVariables ) == NULL )
			{
			sprintf( tempString_, "\nClass:%s\nFunction:\t%s\nError:\tThe given global variables is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			addStringToOutput( false, true, true, tempString_ );
			}

		if( ( adminWordItem_ = adminWordItem ) == NULL )
			{
			sprintf( tempString_, "\nClass:%s\nFunction:\t%s\nError:\tThe given admin word Item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			addStringToOutput( false, true, true, tempString_ );
			}
		}


	// Protected functions

	void clearProgress()
		{
		if( progressBarLength_ > 0 )
			{
			addStringToOutput( true, false, false, CARRIAGE_RETURN_STRING );

			for( unsigned int i = 0; i + 1 < NUMBER_OF_CONSOLE_COLUMNS; i++ )
				addStringToOutput( true, false, false, SPACE_STRING );

			addStringToOutput( true, false, true, CARRIAGE_RETURN_STRING );

			progressBarLength_ = 0;
			currentPosition_ = 0;
			previousProgress_ = 0;
			strcpy( currentProgressString_, EMPTY_STRING );
			}
		}

	void redirectOutputToTestFile( FILE *testFile )
		{
		testFile_ = testFile;
		}

	void displayProgressBar( unsigned int currentProgress )
		{
		char outputString[SENTENCE_STRING_LENGTH];
		unsigned int progress;

		if( progressBarLength_ > 0 )
			{
			if( currentProgress > maximumProgress_ )
				currentProgress = maximumProgress_;

			progress = (unsigned int)( (long double)( progressBarLength_ ) * (long double)( currentProgress ) / maximumProgress_ );

			if( currentProgress == 0 ||
			progress < previousProgress_ )
				{
				previousProgress_ = 0;
				sprintf( outputString, "\r%s: ", currentProgressString_ );
				addStringToOutput( true, false, false, outputString );

				for( size_t i = 0; i < progressBarLength_; i++ )
					addStringToOutput( true, false, false, COLON_STRING );

				sprintf( outputString, "\r%s ", currentProgressString_ );
				addStringToOutput( true, false, ( progress <= previousProgress_ ), outputString );
				}

			// Only display changes
			if( progress > previousProgress_ )
				{
				for( unsigned int i = previousProgress_; i < progress; i++ )
					addStringToOutput( true, false, ( i + 1 == progress ), ASTERISK_STRING );
				}

			previousProgress_ = progress;
			}
		}

	void displayProgressText( const char *newProgressTextString )
		{
		size_t oldProgressTextString = progressTextStringLength_;
		char outputString[SENTENCE_STRING_LENGTH + 1];
		char progressTextString[SENTENCE_STRING_LENGTH];

		if( progressBarLength_ == 0 )
			{
			if( newProgressTextString == NULL )
				clearProgressText();
			else
				{
				if( ( progressTextStringLength_ = strlen( newProgressTextString ) ) < NUMBER_OF_CONSOLE_COLUMNS )
					strcpy( progressTextString, newProgressTextString );
				else
					{
					progressTextStringLength_ = ( NUMBER_OF_CONSOLE_COLUMNS - 1 );
					strcpy( progressTextString, EMPTY_STRING );
					strncat( progressTextString, newProgressTextString, progressTextStringLength_ );
					}

				// Different string
				if( strcmp( progressTextString, currentStatusString_ ) != 0 )
					{
					if( currentPosition_ > 0 )
						returnOutputToPosition( false, false, true );

					sprintf( outputString, "\r%s", progressTextString );
					addStringToOutput( true, false, false, outputString );

					for( size_t i = progressTextStringLength_; i < oldProgressTextString; i++ )
						addStringToOutput( true, false, false, SPACE_STRING );

					addStringToOutput( true, false, true, CARRIAGE_RETURN_STRING );
					strcpy( currentStatusString_, progressTextString );
					}
				}
			}
		}

	void startProgress( unsigned short interfaceParameter1, unsigned short shortNumber, unsigned short interfaceParameter2, unsigned int startProgress, unsigned int maxProgress )
		{
		char newProgressString[SENTENCE_STRING_LENGTH];

		if( progressTextStringLength_ > 0 )
			clearProgressText();

		sprintf( newProgressString, "%s%u%s", adminWordItem_->interfaceString( interfaceParameter1 ), shortNumber, adminWordItem_->interfaceString( interfaceParameter2 ) );
		displayProgressText( newProgressString );

		strcpy( currentProgressString_, EMPTY_STRING );
		strncat( currentProgressString_, newProgressString, ( NUMBER_OF_CONSOLE_COLUMNS - 4 ) );

		maximumProgress_ = maxProgress;
		progressBarLength_ = ( NUMBER_OF_CONSOLE_COLUMNS - 3 - strlen( currentProgressString_ ) );

		if( currentPosition_ > 0 )
			returnOutputToPosition( false, false, true );

		displayProgressBar( startProgress );
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

	signed char displayError( char functionListChar, const char *classNameString, const char *parentClassNameString, const char *wordNameString, const char *functionString, const char *errorString )
		{
		if( classNameString != NULL )
			{
			strcpy( tempString_, INPUT_OUTPUT_ERROR_CLASS_STRING );
			strcat( tempString_, classNameString );
			addStringToOutput( false, true, true, tempString_ );
			}

		if( parentClassNameString != NULL )
			{
			strcpy( tempString_, INPUT_OUTPUT_ERROR_PARENT_CLASS_STRING );
			strcat( tempString_, parentClassNameString );
			addStringToOutput( false, true, true, tempString_ );
			}

		if( functionString != NULL )
			{
			strcpy( tempString_, INPUT_OUTPUT_ERROR_FUNCTION_STRING );
			strcat( tempString_, functionString );
			addStringToOutput( false, true, true, tempString_ );
			}

		if( functionListChar != SYMBOL_QUESTION_MARK )
			{
			sprintf( tempString_, "%s%c%s", INPUT_OUTPUT_ERROR_FUNCTION_LIST_START_STRING, functionListChar, INPUT_OUTPUT_ERROR_FUNCTION_LIST_END_STRING );
			addStringToOutput( false, true, true, tempString_ );
			}

		if( wordNameString != NULL )
			{
			strcpy( tempString_, INPUT_OUTPUT_ERROR_FUNCTION_WORD_START_STRING );
			strcat( tempString_, wordNameString );
			strcat( tempString_, INPUT_OUTPUT_ERROR_FUNCTION_WORD_END_STRING );
			addStringToOutput( false, true, true, tempString_ );
			}

		if( errorString != NULL )
			{
			strcpy( tempString_, INPUT_OUTPUT_ERROR_STRING );
			strcat( tempString_, errorString );
			writeText( false, true, false, false, INPUT_OUTPUT_PROMPT_WARNING, NO_CENTER_WIDTH, INPUT_OUTPUT_PROMPT_WARNING_STRING, tempString_ );
			}

		addStringToOutput( false, true, true, COLON_STRING );

		return RESULT_ERROR;
		}

	signed char readLine( bool isDisplayingLine, bool isPassword, bool isQuestion, char *promptString, char *readString, FILE *readFile )
		{
		size_t passwordLength;
		size_t tempLength;
		char inputString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char passwordString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char outputString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
#ifdef _MSC_VER
		// MS Visual Studio
		wchar_t inputWideString[SENTENCE_STRING_LENGTH] = EMPTY_WIDE_STRING;
#endif
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readLine";

		hasReadLine_ = false;
		lastDisplayedInterfaceParameter_ = NO_INTERFACE_PARAMETER;

		if( readString != NULL )
			{
			if( isQuestion )
				strcat( outputString, INPUT_OUTPUT_PROMPT_WRITE_STRING );

			if( promptString != NULL )
				{
				tempLength = strlen( outputString );
				strcat( outputString, promptString );

				if( isQuestion &&
				strlen( promptString ) > 0 &&
				isalpha( promptString[0] ) )
					outputString[tempLength] = (char)toupper( outputString[tempLength] );
				}

			strcat( outputString, ( isQuestion ? INPUT_OUTPUT_PROMPT_QUERY_STRING : INPUT_OUTPUT_PROMPT_READ_STRING ) );

			if( currentPosition_ > 0 )
				returnOutputToPosition( false, false, true );

			if( isDisplayingExtraPromptLine_ )
				{
				if( writeText( false, false, true, true, lastUsedPromptTypeNr_, NO_CENTER_WIDTH, NULL, NEW_LINE_STRING ) == RESULT_OK )
					isDisplayingExtraPromptLine_ = false;
				else
					{
					sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the text.\n", moduleNameString_, functionNameString );
					addStringToOutput( false, true, true, tempString_ );
					}
				}

			if( globalVariables_->result == RESULT_OK )
				{
				if( isPassword ||
				readFile == NULL )
					{
					currentPosition_ = 0;
					addStringToOutput( false, false, true, outputString );

					if( isPassword )
						{
						passwordLength = 0;
						strcpy( readString, EMPTY_STRING );

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
						strncat( readString, passwordString, passwordLength );
						}
					else
						{
						// Input from keyboard
						if( fgets( inputString, SENTENCE_STRING_LENGTH, stdin ) != NULL )
							{
							hasReadLine_ = true;
							stripStartAndEndSpaces( inputString, readString );
							}
						else
							{
							sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to get an input string.\n", moduleNameString_, functionNameString );
							addStringToOutput( false, true, true, tempString_ );
							globalVariables_->result = RESULT_ERROR;
							}
						}
					}
				else
					{
					// Input from file
#ifdef _MSC_VER
					// MS Visual Studio
					if( fgetws( inputWideString, SENTENCE_STRING_LENGTH, readFile ) != NULL )
#else
					// For compilers other than MS Visual Studio
					if( fgets( inputString, SENTENCE_STRING_LENGTH, readFile ) != NULL )
#endif
						{
						hasReadLine_ = true;

#ifdef _MSC_VER
						// MS Visual Studio
						sprintf( inputString, "%S", &inputWideString );
#else
						// For compilers other than MS Visual Studio:
						// Check for possible BOM marker from first line
						if( strlen( inputString ) > 2 &&
						(unsigned short)inputString[0] == FILE_UTF_8_BOM_CHAR_1 &&
						(unsigned short)inputString[1] == FILE_UTF_8_BOM_CHAR_2 &&
						(unsigned short)inputString[2] == FILE_UTF_8_BOM_CHAR_3 )
							{
							// Remove BOM marker
							strcpy( tempString_, &inputString[FILE_UTF_8_BOM_LENGTH] );
							strcpy( inputString, tempString_ );
							}
#endif

						stripStartAndEndSpaces( inputString, readString );

						if( strncmp( readString, INPUT_OUTPUT_DEVELOPER_TAG_STRING, strlen( INPUT_OUTPUT_DEVELOPER_TAG_STRING ) ) == 0 )
							{
							// Strip Developer tag from line
							strcpy( tempString_, &readString[ strlen( INPUT_OUTPUT_DEVELOPER_TAG_STRING ) ] );
							strcpy( readString, tempString_ );
							}

						if( strncmp( readString, INPUT_OUTPUT_CPP_ACCEPT_TAG_STRING, strlen( INPUT_OUTPUT_CPP_ACCEPT_TAG_STRING ) ) == 0 )
							{
							// Strip C++ tag from line
							strcpy( tempString_, &readString[ strlen( INPUT_OUTPUT_CPP_ACCEPT_TAG_STRING ) ] );
							strcpy( readString, tempString_ );
							}

						if( isDisplayingLine &&
						// Skip Java comment line
						strncmp( readString, INPUT_OUTPUT_JAVA_SKIP_TAG_STRING, strlen( INPUT_OUTPUT_JAVA_SKIP_TAG_STRING ) ) != 0 &&
						writeText( false, false, true, true, INPUT_OUTPUT_PROMPT_READ, NO_CENTER_WIDTH, outputString, ( strlen( readString ) == 0 ? NEW_LINE_STRING : readString ) ) != RESULT_OK )
							{
							sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tI failed to write the text.\n", moduleNameString_, functionNameString );
							addStringToOutput( false, true, true, tempString_ );
							}
						}
					}
				}
			}
		else
			{
			sprintf( tempString_, "\nClass:\t%s\nFunction:\t%s\nError:\tThe given read string is undefined.\n", moduleNameString_, functionNameString );
			addStringToOutput( false, true, true, tempString_ );
			globalVariables_->result = RESULT_ERROR;
			}

		return globalVariables_->result;
		}

	signed char writeDiacriticalText( unsigned short promptTypeNr, const char *diacriticalTextString )
		{
		return writeDiacriticalText( true, true, promptTypeNr, diacriticalTextString );
		}

	signed char writeDiacriticalText( bool isFirstCharacterToUpperCase, bool isReturningToPosition, unsigned short promptTypeNr, const char *diacriticalTextString )
		{
		bool hasFoundNewLine = false;
		size_t position = 0;
		char textChar = SYMBOL_QUESTION_MARK;
		char textString[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeDiacriticalText";

		if( diacriticalTextString != NULL )
			{
			isDisplayingExtraPromptLine_ = true;

			if( diacriticalTextString[0] == QUERY_STRING_START_CHAR )
				position++;

			while( globalVariables_->result == RESULT_OK &&
			position < strlen( diacriticalTextString ) )
				{
				if( diacriticalTextString[position] == SYMBOL_BACK_SLASH )
					{
					if( ++position < strlen( diacriticalTextString ) )
						{
						if( ( textChar = convertDiacriticalChar( diacriticalTextString[position] ) ) == NEW_LINE_CHAR )
							hasFoundNewLine = true;
						}
					else
						globalVariables_->result = displayError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The text string ended with a diacritical sign" );
					}
				else
					textChar = diacriticalTextString[position];

				if( globalVariables_->result == RESULT_OK )
					{
					position++;
					charString_[0] = textChar;
					strcat( textString, charString_ );

					if( hasFoundNewLine ||
					strlen( textString ) + 1 == SENTENCE_STRING_LENGTH ||

					( strlen( textString ) > 0 &&
					position < strlen( diacriticalTextString ) &&
					diacriticalTextString[position] == QUERY_CHAR ) )
						{
						if( writeText( false, false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, NO_CENTER_WIDTH, NULL, textString ) == RESULT_OK )
							{
							hasFoundNewLine = false;
							strcpy( textString, EMPTY_STRING );
							}
						else
							{
							sprintf( tempString_, "\nClass:%s\nFunction:\t%s\nError:\tI failed to write a text string part.\n", moduleNameString_, functionNameString );
							addStringToOutput( false, true, true, tempString_ );
							}
						}
					}
				}

			if( globalVariables_->result == RESULT_OK &&
			strlen( textString ) > 0 &&
			writeText( false, false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, NO_CENTER_WIDTH, NULL, textString ) != RESULT_OK )
				{
				sprintf( tempString_, "\nClass:%s\nFunction:\t%s\nError:\tI failed to write the last text string part.\n", moduleNameString_, functionNameString );
				addStringToOutput( false, true, true, tempString_ );
				}
			}
		else
			globalVariables_->result = displayError( SYMBOL_QUESTION_MARK, moduleNameString_, NULL, NULL, functionNameString, "The given diacritical text string is undefined" );

		return globalVariables_->result;
		}

	signed char writeInterfaceText( bool isCheckingForDuplicateInterfaceParameter, unsigned short promptTypeNr, unsigned short interfaceParameter )
		{
		return writeInterfaceText( isCheckingForDuplicateInterfaceParameter, true, promptTypeNr, interfaceParameter );
		}

	signed char writeInterfaceText( bool isCheckingForDuplicateInterfaceParameter, bool isReturningToPosition, unsigned short promptTypeNr, unsigned short interfaceParameter )
		{
		if( !isCheckingForDuplicateInterfaceParameter ||
		interfaceParameter != lastDisplayedInterfaceParameter_ )
			{
			lastDisplayedInterfaceParameter_ = interfaceParameter;
			return writeDiacriticalText( true, isReturningToPosition, promptTypeNr, adminWordItem_->interfaceString( interfaceParameter ) );
			}

		return globalVariables_->result;
		}

	signed char writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, unsigned int intNumber, unsigned short interfaceParameter2 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		sprintf( interfaceString, "%s%u%s", adminWordItem_->interfaceString( interfaceParameter1 ), intNumber, adminWordItem_->interfaceString( interfaceParameter2 ) );
		return writeDiacriticalText( promptTypeNr, interfaceString );
		}

	signed char writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, unsigned int intNumber, unsigned short interfaceParameter2, char *textString, unsigned short interfaceParameter3 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		sprintf( interfaceString, "%s%u%s%s%s", adminWordItem_->interfaceString( interfaceParameter1 ), intNumber, adminWordItem_->interfaceString( interfaceParameter2 ), textString, adminWordItem_->interfaceString( interfaceParameter3 ) );
		return writeDiacriticalText( promptTypeNr, interfaceString );
		}

	signed char writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, unsigned int intNumber1, unsigned short interfaceParameter2, unsigned int intNumber2, unsigned short interfaceParameter3, char *textString, unsigned short interfaceParameter4 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		sprintf( interfaceString, "%s%u%s%u%s%s%s", adminWordItem_->interfaceString( interfaceParameter1 ), intNumber1, adminWordItem_->interfaceString( interfaceParameter2 ), intNumber2, adminWordItem_->interfaceString( interfaceParameter3 ), textString, adminWordItem_->interfaceString( interfaceParameter4 ) );
		return writeDiacriticalText( promptTypeNr, interfaceString );
		}

	signed char writeInterfaceText( bool isCheckingForDuplicateInterfaceParameter, unsigned short promptTypeNr, unsigned short interfaceParameter1, const char *textString, unsigned short interfaceParameter2 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		if( !isCheckingForDuplicateInterfaceParameter ||
		interfaceParameter1 != lastDisplayedInterfaceParameter_ )
			{
			lastDisplayedInterfaceParameter_ = interfaceParameter1;

			strcpy( interfaceString, adminWordItem_->interfaceString( interfaceParameter1 ) );
			strcat( interfaceString, textString );
			strcat( interfaceString, adminWordItem_->interfaceString( interfaceParameter2 ) );

			return writeDiacriticalText( promptTypeNr, interfaceString );
			}

		return globalVariables_->result;
		}

	signed char writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, char *textString, unsigned short interfaceParameter2, unsigned int longNumber, unsigned short interfaceParameter3 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		sprintf( interfaceString, "%s%s%s%u%s", adminWordItem_->interfaceString( interfaceParameter1 ), textString, adminWordItem_->interfaceString( interfaceParameter2 ), longNumber, adminWordItem_->interfaceString( interfaceParameter3 ) );
		return writeDiacriticalText( promptTypeNr, interfaceString );
		}

	signed char writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, const char *textString, unsigned short interfaceParameter2 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		strcpy( interfaceString, adminWordItem_->interfaceString( interfaceParameter1 ) );
		strcat( interfaceString, textString );
		strcat( interfaceString, adminWordItem_->interfaceString( interfaceParameter2 ) );

		return writeDiacriticalText( promptTypeNr, interfaceString );
		}

	signed char writeInterfaceText( unsigned short promptTypeNr, unsigned short interfaceParameter1, const char *textString1, unsigned short interfaceParameter2, const char *textString2, unsigned short interfaceParameter3 )
		{
		char interfaceString[SENTENCE_STRING_LENGTH];

		strcpy( interfaceString, adminWordItem_->interfaceString( interfaceParameter1 ) );
		strcat( interfaceString, textString1 );
		strcat( interfaceString, adminWordItem_->interfaceString( interfaceParameter2 ) );
		strcat( interfaceString, textString2 );
		strcat( interfaceString, adminWordItem_->interfaceString( interfaceParameter3 ) );

		return writeDiacriticalText( promptTypeNr, interfaceString );
		}

	signed char writeText( unsigned short promptTypeNr, const char *textString1, const char *textString2 )
		{
		isDisplayingExtraPromptLine_ = true;

		if( writeText( false, false, true, true, promptTypeNr, NO_CENTER_WIDTH, NULL, textString1 ) == RESULT_OK )
			{
			if( textString2 != NULL &&
			strlen( textString2 ) > 0 )
				return writeText( false, false, true, false, promptTypeNr, NO_CENTER_WIDTH, NULL, textString2 );
			}

		return globalVariables_->result;
		}

	signed char writeText( bool isReturningToPosition, unsigned short promptTypeNr, size_t queryWidth, const char *textString )
		{
		return writeText( false, false, true, isReturningToPosition, promptTypeNr, queryWidth, NULL, textString );
		}

	signed char writeText( bool isSkippingInTestFile, bool isReturningToPosition, unsigned short promptTypeNr, size_t queryWidth, const char *textString )
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
