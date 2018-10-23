/*	Class:		InputOutput
 *	Purpose:	To read and write user information
 *	Version:	Thinknowlogy 2018r3 (Deep Magic)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

class InputOutput
	{
	// Private static variables

	private static boolean hasReadLine_;
	private static boolean isDisplayingExtraPromptLine_;

	private static short lastShownInterfaceParameter_;
	private static short lastUsedPromptTypeNr_;

	private static int currentPosition_;
	private static int previousProgress_;

	private static BufferedWriter testFile_;

	private static String classNameString_ = "InputOutput";

	private static StringBuffer currentStatusStringBuffer_;
	private static StringBuffer outputStringBuffer_;


	// Private static methods

	private static void clearStatus()
		{
		currentStatusStringBuffer_ = null;
		Console.clearProgress();
		}

	private static void displayStatus( short interfaceParameter )
		{
		String newStatusString;
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;
		
		if( ( newStatusString = ( currentLanguageWordItem == null ? Constants.NO_LANGUAGE_WORD_FOUND_STRING : currentLanguageWordItem.interfaceString( interfaceParameter ) ) ) == null )
			clearStatus();
		else
			{
			if( currentStatusStringBuffer_ == null ||
			// Different string
			currentStatusStringBuffer_.indexOf( newStatusString ) == -1 )
				{
				currentStatusStringBuffer_ = new StringBuffer( newStatusString + ( newStatusString.endsWith( "." ) ? Constants.EMPTY_STRING : "..." ) );
				Console.displayProgressStatus( currentStatusStringBuffer_.toString() );
				}
			}
		}

	private static void returnOutputToPosition( boolean isSkippingInTestFile, boolean isWriting )
		{
		currentPosition_ = 0;
		addStringToOutput( isSkippingInTestFile, isWriting, Constants.CARRIAGE_RETURN_NEW_LINE_STRING );
		}

	private static void addStringToOutput( boolean isSkippingInTestFile, boolean isWriting, String writeString )
		{
		outputStringBuffer_.append( writeString );

		if( isWriting &&
		outputStringBuffer_.length() > 0 )
			{
			if( isSkippingInTestFile ||
			testFile_ == null )
				Console.writeText( outputStringBuffer_.toString() );
			else
				{
				try	{
					testFile_.write( outputStringBuffer_.toString() );
					}
				catch( IOException exception )
					{
					Console.writeText( "IOException: Failed to write to test file: \"" + outputStringBuffer_.toString() + "\".\n" );
					}
				}

			outputStringBuffer_ = new StringBuffer();
			}
		}

	private static byte writeDiacriticalText( boolean isFirstCharacterToUpperCase, boolean isReturningToPosition, short promptTypeNr, String diacriticalTextString )
		{
		boolean hasFoundNewLine = false;
		int position = 0;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer textStringBuffer = new StringBuffer();

		if( diacriticalTextString != null )
			{
			isDisplayingExtraPromptLine_ = true;

			while( GlobalVariables.result == Constants.RESULT_OK &&
			position < diacriticalTextString.length() )
				{
				if( diacriticalTextString.charAt( position ) == Constants.SYMBOL_BACK_SLASH )
					{
					if( ++position < diacriticalTextString.length() )
						{
						if( ( textChar = convertDiacriticalChar( diacriticalTextString.charAt( position ) ) ) == Constants.NEW_LINE_CHAR )
							hasFoundNewLine = true;
						}
					else
						{
						displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The text string ended with a diacritical sign" );
						GlobalVariables.result = Constants.RESULT_ERROR;
						}
					}
				else
					textChar = diacriticalTextString.charAt( position );

				if( GlobalVariables.result == Constants.RESULT_OK )
					{
					position++;
					textStringBuffer.append( Constants.EMPTY_STRING + textChar );
	
					if( hasFoundNewLine ||
	
					( textStringBuffer.length() > 0 &&
					position < diacriticalTextString.length() &&
					diacriticalTextString.charAt( position ) == Constants.QUERY_CHAR ) )
						{
						if( writeText( false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, Constants.NO_CENTER_WIDTH, null, textStringBuffer.toString() ) == Constants.RESULT_OK )
							{
							hasFoundNewLine = false;
							textStringBuffer = new StringBuffer();
							}
						else
							displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write a text string part" );
						}
					}
				}

			if( GlobalVariables.result == Constants.RESULT_OK &&
			textStringBuffer.length() > 0 &&
			writeText( false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, Constants.NO_CENTER_WIDTH, null, textStringBuffer.toString() ) != Constants.RESULT_OK )
				displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the last text string part" );
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given diacritical text string is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	private static byte writeText( boolean isSkippingInTestFile, boolean isFirstCharacterToUpperCase, boolean isReturningToPosition, short promptTypeNr, int leftWidth, int rightWidth, int printStringLength, String promptString, String writeString )
		{
		boolean startNewLine = false;
		int firstLetterPosition = 0;
		int i = 0;
		int length = 0;
		int promptStringLength;
		int wordPosition;
		String textString;
		StringBuffer writeStringBuffer = new StringBuffer();

		if( writeString != null )
			{
			if( printStringLength > 0 ||
			leftWidth > Constants.NO_CENTER_WIDTH ||
			rightWidth > Constants.NO_CENTER_WIDTH )
				{
				if( currentPosition_ > 0 &&
				isReturningToPosition )
					returnOutputToPosition( isSkippingInTestFile, false );

				// Find first letter character position
				while( firstLetterPosition < printStringLength &&
				!Character.isLetter( writeString.charAt( firstLetterPosition ) ) &&
				!Character.isWhitespace( writeString.charAt( firstLetterPosition ) ) )
					firstLetterPosition++;

				// Convert first letter character to upper case
				if( isFirstCharacterToUpperCase &&
				firstLetterPosition < printStringLength &&

				( !isReturningToPosition ||
				currentPosition_ == 0 ) &&

				( promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WRITE ||
				promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED ||
				promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WARNING ) )
					textString = writeString.substring( 0, firstLetterPosition ) + Character.toUpperCase( writeString.charAt( firstLetterPosition ) ) + ( firstLetterPosition + 1 < printStringLength ? writeString.substring( ( firstLetterPosition + 1 ), printStringLength ) : Constants.EMPTY_STRING );
				else
					textString = writeString.substring( 0, printStringLength );

				if( promptString == null )
					promptString = getPromptString( promptTypeNr );

				if( promptString != null )
					{
					while( i < printStringLength ||
					leftWidth > Constants.NO_CENTER_WIDTH )
						{
						if( currentPosition_ == 0 &&
						( promptStringLength = promptString.length() ) > 0 )
							{
							currentPosition_ = ( promptStringLength - 1 );
							writeStringBuffer.append( promptString );
							}
	
						if( i < printStringLength )
							{
							if( textString.charAt( i ) == Constants.TAB_CHAR )
								length = ( Constants.TAB_LENGTH - ( currentPosition_ + 1 ) % Constants.TAB_LENGTH );
							else
								{
								length = 0;
								wordPosition = i;
	
								do	{
									i++;
									length++;
									}
								while( i < printStringLength &&
								!Character.isWhitespace( textString.charAt( i ) ) );
	
								if( i < printStringLength &&

								( textString.charAt( i ) == Constants.NEW_LINE_CHAR ||
								textString.charAt( i ) == Constants.CARRIAGE_RETURN_CHAR ) )
									length--;
	
								i = wordPosition;
								}
	
							if( i < printStringLength &&

							( textString.charAt( i ) == Constants.NEW_LINE_CHAR ||
							textString.charAt( i ) == Constants.CARRIAGE_RETURN_CHAR ) )
								{
								startNewLine = true;
								i++;
								currentPosition_ = 0;
								writeStringBuffer.append( Constants.CARRIAGE_RETURN_NEW_LINE_STRING );
								}
							}
	
						if( startNewLine )
							startNewLine = false;
						else
							{
							while( leftWidth > Constants.NO_CENTER_WIDTH )
								{
								leftWidth--;
								currentPosition_++;
								writeStringBuffer.append( Constants.SPACE_STRING );
								}
	
							if( i < printStringLength )
								{
								if( textString.charAt( i ) == Constants.TAB_CHAR )
									{
									writeStringBuffer.append( textString.charAt( i ) );
									i++;
									currentPosition_ += length;
									}
								else
									{
									do	{
										writeStringBuffer.append( textString.charAt( i ) );
	
										if( textString.charAt( i ) == Constants.NEW_LINE_CHAR ||
										textString.charAt( i ) == Constants.CARRIAGE_RETURN_CHAR )
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
					}

				while( rightWidth-- > Constants.NO_CENTER_WIDTH )
					{
					currentPosition_++;
					writeStringBuffer.append( Constants.SPACE_STRING );
					}

				addStringToOutput( isSkippingInTestFile, true, writeStringBuffer.toString() );
				}
			else
				{
				displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given write string is empty" );
				GlobalVariables.result = Constants.RESULT_ERROR;
				}
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given write string is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	private static byte writeText( boolean isSkippingInTestFile, boolean isFirstCharacterToUpperCase, boolean isReturningToPosition, short promptTypeNr, int queryWidth, String promptString, String textString )
		{
		int leftWidth = Constants.NO_CENTER_WIDTH;
		int rightWidth = Constants.NO_CENTER_WIDTH;
		int textStringLength;
		int widthDifference;

		if( textString != null )
			{
			textStringLength = textString.length();

			if( textStringLength > 0 ||
			queryWidth > Constants.NO_CENTER_WIDTH )
				{
				clearStatus();
				clearProgress();

				if( promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WARNING )
					GlobalVariables.hasDisplayedWarning = true;
				else
					{
					if( promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY )
						GlobalVariables.hasDisplayedIntegrityWarning = true;
					else
						GlobalVariables.hasDisplayedMessage = true;
					}

				if( queryWidth > 0 &&
				queryWidth < textStringLength )
					textStringLength = queryWidth;

				if( queryWidth >= textStringLength )
					{
					widthDifference = ( queryWidth - textStringLength );
					leftWidth = ( widthDifference / 2 );
					rightWidth = ( widthDifference - leftWidth );
					}

				if( writeText( isSkippingInTestFile, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, leftWidth, rightWidth, textStringLength, promptString, textString ) == Constants.RESULT_OK )
					lastUsedPromptTypeNr_ = ( promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WARNING_INDENTED ? Constants.INPUT_OUTPUT_PROMPT_WARNING : ( promptTypeNr == Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED ? Constants.INPUT_OUTPUT_PROMPT_WRITE : promptTypeNr ) );
				else
					displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the string" );
				}
			else
				{
				displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string is empty" );
				GlobalVariables.result = Constants.RESULT_ERROR;
				}
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	private static String getPromptString( short promptTypeNr )
		{
		switch( promptTypeNr )
			{
			case Constants.INPUT_OUTPUT_PROMPT_READ:
				return Constants.INPUT_OUTPUT_PROMPT_READ_STRING;

			case Constants.INPUT_OUTPUT_PROMPT_WRITE:
				return Constants.INPUT_OUTPUT_PROMPT_WRITE_STRING;

			case Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED:
				return Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED_STRING;

			case Constants.INPUT_OUTPUT_PROMPT_INFO:
				return Constants.INPUT_OUTPUT_PROMPT_INFO_STRING;

			case Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION:
				return Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION_STRING;

			case Constants.INPUT_OUTPUT_PROMPT_WARNING:
			case Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY:
				return Constants.INPUT_OUTPUT_PROMPT_WARNING_STRING;

			case Constants.INPUT_OUTPUT_PROMPT_WARNING_INDENTED:
				return Constants.INPUT_OUTPUT_PROMPT_WARNING_INDENTED_STRING;
			}

		return Constants.INPUT_OUTPUT_PROMPT_QUERY_STRING;
		}


	// Constructor

	protected static void initialize()
		{
		// Private static variables

		hasReadLine_ = false;
		isDisplayingExtraPromptLine_ = false;

		lastShownInterfaceParameter_ = Constants.NO_INTERFACE_PARAMETER;
		lastUsedPromptTypeNr_ = Constants.INPUT_OUTPUT_PROMPT_QUERY;

		currentPosition_ = 0;
		previousProgress_ = 0;

		testFile_ = null;

		currentStatusStringBuffer_ = null;
		outputStringBuffer_ = new StringBuffer();
		}

	// Protected methods

	protected static void startProgress( short interfaceParameter1, short shortNumber, short interfaceParameter2, int startProgress, int maxProgress )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;
		Console.startProgress( startProgress, maxProgress, ( currentLanguageWordItem == null ? Constants.NO_LANGUAGE_WORD_FOUND_STRING : currentLanguageWordItem.interfaceString( interfaceParameter1 ) + shortNumber + currentLanguageWordItem.interfaceString( interfaceParameter2 ) + "  " + Constants.QUERY_ITEM_START_STRING + GlobalVariables.currentSentenceNr + ( GlobalVariables.currentSentenceItemNr == Constants.NO_SENTENCE_NR ? Constants.EMPTY_STRING : Constants.QUERY_SEPARATOR_STRING + GlobalVariables.currentSentenceItemNr ) + Constants.QUERY_ITEM_END_CHAR ) );
		}

	protected static void displayProgressBar( int currentProgress )
		{
		if( currentProgress != previousProgress_ )
			{
			previousProgress_ = currentProgress;
			Console.displayProgress( currentProgress );
			}
		}

	protected static void clearProgress()
		{
		previousProgress_ = 0;
		Console.clearProgress();

		if( currentStatusStringBuffer_ != null )
			Console.displayProgressStatus( currentStatusStringBuffer_.toString() );
		}

	protected static void redirectOutputToTestFile( BufferedWriter testFile )
		{
		testFile_ = testFile;
		}

	protected static void displayError( char methodListChar, String classNameString, String parentClassNameString, String wordNameString, int methodLevel, String errorString )
		{
		StackTraceElement[] elements;
		StringBuffer errorStringBuffer = new StringBuffer();
		Throwable t = new Throwable();

		// Show remaining output
		addStringToOutput( true, true, Constants.EMPTY_STRING );

		if( classNameString != null )
			errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_CLASS_STRING + classNameString );

		if( parentClassNameString != null )
			errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_PARENT_CLASS_STRING + parentClassNameString );

		if( ( elements = t.getStackTrace() ) != null )
			{
			if( elements.length > 0 )
				{
				// elements.[0] is this method
				if( methodLevel < elements.length )
					errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_METHOD_STRING + elements[methodLevel].getMethodName() );
				else
					errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_METHOD_STRING + "{Invalid method level}" );
				}
			else
				errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_METHOD_STRING + "{No stack information available}" );
			}
		else
			errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_METHOD_STRING + "{Failed to create Throwable or StackTrace}" );

		if( methodListChar != Constants.SYMBOL_QUESTION_MARK )
			errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_METHOD_LIST_START_STRING + methodListChar + Constants.INPUT_OUTPUT_ERROR_METHOD_LIST_END_STRING );

		if( wordNameString != null )
			errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_METHOD_WORD_START_STRING + wordNameString + Constants.INPUT_OUTPUT_ERROR_METHOD_WORD_END_STRING );

		if( errorString != null )
			errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_MESSAGE_START_STRING + errorString );

		errorStringBuffer.append( Constants.INPUT_OUTPUT_ERROR_MESSAGE_END_STRING );

		Console.addError( Constants.INPUT_OUTPUT_ERROR_CURRENT_ID_STRING + Constants.QUERY_ITEM_START_CHAR + GlobalVariables.currentSentenceNr + Constants.QUERY_SEPARATOR_CHAR + GlobalVariables.currentSentenceItemNr + Constants.QUERY_ITEM_END_CHAR, errorStringBuffer.toString() );
		}

	protected static boolean hasReadLine()
		{
		return hasReadLine_;
		}

	protected static char convertDiacriticalChar( char diacriticalChar )
		{
		switch( diacriticalChar )
			{
			case Constants.TEXT_TAB_CHAR:
				return Constants.TAB_CHAR;

			case Constants.TEXT_BELL_CHAR:
				return Constants.BELL_CHAR;

			case Constants.SYMBOL_DOUBLE_QUOTE:
				return Constants.SYMBOL_DOUBLE_QUOTE;

			case Constants.TEXT_BACK_SPACE_CHAR:
				return Constants.BACK_SPACE_CHAR;

			case Constants.TEXT_NEW_LINE_CHAR:
				return Constants.NEW_LINE_CHAR;

			case Constants.QUERY_CHAR:	// Avoid triggering of query
				return Constants.QUERY_CHAR;
			}

		return diacriticalChar;
		}

	protected static byte readLine( boolean isClearInputField, boolean isDeveloperTheCurrentUser, boolean isDisplayingLine, boolean isFirstLine, boolean isPassword, boolean isQuestion, String promptString, StringBuffer readStringBuffer, BufferedReader readFile )
		{
		int readStringLength;
		int startPosition = 0;
		String readString = new String();
		StringBuffer promptStringBuffer = new StringBuffer();

		hasReadLine_ = false;
		lastShownInterfaceParameter_ = Constants.NO_INTERFACE_PARAMETER;

		if( readStringBuffer != null )
			{
			if( isQuestion )
				promptStringBuffer.append( Constants.INPUT_OUTPUT_PROMPT_WRITE_STRING );

			if( promptString != null )
				promptStringBuffer.append( isQuestion && promptString.length() > 0 && Character.isLetter( promptString.charAt( 0 ) ) ? Character.toUpperCase( promptString.charAt( 0 ) ) + promptString.substring( 1 ) : promptString );

			promptStringBuffer.append( isQuestion ? Constants.INPUT_OUTPUT_PROMPT_QUERY_STRING : Constants.INPUT_OUTPUT_PROMPT_READ_STRING );

			if( currentPosition_ > 0 )
				returnOutputToPosition( false, true );

			if( isDisplayingExtraPromptLine_ )
				{
				if( writeText( false, true, true, lastUsedPromptTypeNr_, Constants.NO_CENTER_WIDTH, null, Constants.NEW_LINE_STRING ) == Constants.RESULT_OK )
					isDisplayingExtraPromptLine_ = false;
				else
					displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the line" );
				}

			if( GlobalVariables.result == Constants.RESULT_OK )
				{
				if( isPassword ||
				readFile == null )
					{
					currentPosition_ = 0;

					if( isPassword )
						{
						displayStatus( Constants.INTERFACE_CONSOLE_WAITING_FOR_SECURE_INPUT );
						addStringToOutput( false, true, ( promptStringBuffer.toString() + Constants.NEW_LINE_STRING ) );

						if( ( readString = Console.getPassword() ) != null )
							hasReadLine_ = true;
						else
							{
							displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The returned password string is undefined" );
							GlobalVariables.result = Constants.RESULT_ERROR;
							}
						}
					else
						{
						if( isClearInputField )
							displayStatus( Constants.INTERFACE_CONSOLE_WAITING_FOR_INPUT );
						else
							clearStatus();

						addStringToOutput( false, true, promptStringBuffer.toString() );

						if( ( readString = Console.readLine( isClearInputField, isQuestion ) ) != null )
							hasReadLine_ = true;
						else
							{
							displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The returned read line string is undefined" );
							GlobalVariables.result = Constants.RESULT_ERROR;
							}
						}
					}
				else
					{
					try	{
						if( ( readString = readFile.readLine() ) != null )
							{
							hasReadLine_ = true;

							// Check for possible UTF-8 BOM marker
							if( isFirstLine &&
							readString.length() > 0 &&

							( readString.charAt( 0 ) == Constants.FILE_UTF_8_BOM_CHAR ||
							readString.startsWith( Constants.FILE_UTF_8_BOM_STRING ) ) )
								// Remove UTF-8 BOM marker
								readString = readString.substring( 1 );

							if( readString.startsWith( Constants.INPUT_OUTPUT_DEVELOPER_TAG_STRING ) )
								{
								if( isDeveloperTheCurrentUser )
									// Strip Developer tag from line
									readString = readString.substring( Constants.INPUT_OUTPUT_DEVELOPER_TAG_STRING.length() );
								else
									isDisplayingLine = false;
								}

							if( readString.startsWith( Constants.INPUT_OUTPUT_JAVA_ACCEPT_TAG_STRING ) )
								// Strip Java tag from line
								readString = readString.substring( Constants.INPUT_OUTPUT_JAVA_ACCEPT_TAG_STRING.length() );

							if( isDisplayingLine &&
							// Skip C++ comment line
							!readString.startsWith( Constants.INPUT_OUTPUT_CPP_SKIP_TAG_STRING ) &&
							writeText( false, true, true, Constants.INPUT_OUTPUT_PROMPT_READ, Constants.NO_CENTER_WIDTH, promptStringBuffer.toString(), ( readString.length() == 0 ? Constants.NEW_LINE_STRING : readString ) ) != Constants.RESULT_OK )
								displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the text" );
							}
						}
					catch( IOException exception )
						{
						displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, exception.toString() );
						GlobalVariables.result = Constants.RESULT_ERROR;
						}
					}

				if( GlobalVariables.result == Constants.RESULT_OK &&
				hasReadLine_ &&
				readString != null &&
				( readStringLength = readString.length() ) > 0 )
					{
					// Skip spaces at the beginning of the input string
					// But don't skip spaces at the end
					// So, don't use 'trim'
					while( startPosition < readStringLength &&
					Character.isWhitespace( readString.charAt( startPosition ) ) )
						startPosition++;

					readStringBuffer.append( readString.substring( startPosition ) );
					}
				}
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given read string buffer is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	protected static byte writeDiacriticalText( short promptTypeNr, String diacriticalTextString )
		{
		return writeDiacriticalText( true, true, promptTypeNr, diacriticalTextString );
		}

	protected static byte writeInterfaceText( boolean isCheckingForDuplicateInterfaceParameter, short promptTypeNr, short interfaceParameter )
		{
		return writeInterfaceText( isCheckingForDuplicateInterfaceParameter, true, promptTypeNr, interfaceParameter );
		}

	protected static byte writeInterfaceText( boolean isCheckingForDuplicateInterfaceParameter, boolean isReturningToPosition, short promptTypeNr, short interfaceParameter )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			{
			if( !isCheckingForDuplicateInterfaceParameter ||
			interfaceParameter != lastShownInterfaceParameter_ )
				{
				lastShownInterfaceParameter_ = interfaceParameter;
				return writeDiacriticalText( true, isReturningToPosition, promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter ) );
				}
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber, short interfaceParameter2 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber + currentLanguageWordItem.interfaceString( interfaceParameter2 ) );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber, short interfaceParameter2, String textString, short interfaceParameter3 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber + currentLanguageWordItem.interfaceString( interfaceParameter2 ) + textString + currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber1, short interfaceParameter2, int intNumber2, short interfaceParameter3, String textString, short interfaceParameter4 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber1 + currentLanguageWordItem.interfaceString( interfaceParameter2 ) + intNumber2 + currentLanguageWordItem.interfaceString( interfaceParameter3 ) + textString + currentLanguageWordItem.interfaceString( interfaceParameter4 ) );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( boolean isCheckingForDuplicateInterfaceParameter, short promptTypeNr, short interfaceParameter1, String textString, short interfaceParameter2 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			{
			if( !isCheckingForDuplicateInterfaceParameter ||
			interfaceParameter1 != lastShownInterfaceParameter_ )
				{
				lastShownInterfaceParameter_ = interfaceParameter1;
				return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString + currentLanguageWordItem.interfaceString( interfaceParameter2 ) );
				}
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, String textString, short interfaceParameter2 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString + currentLanguageWordItem.interfaceString( interfaceParameter2 ) );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, String textString1, short interfaceParameter2, String textString2, short interfaceParameter3 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString1 + currentLanguageWordItem.interfaceString( interfaceParameter2 ) + textString2 + currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, String textString, short interfaceParameter2, int intNumber, short interfaceParameter3 )
		{
		WordItem currentLanguageWordItem = GlobalVariables.currentLanguageWordItem;

		if( currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString + currentLanguageWordItem.interfaceString( interfaceParameter2 ) + intNumber + currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeText( short promptTypeNr, StringBuffer textStringBuffer1, StringBuffer textStringBuffer2 )
		{
		if( textStringBuffer1 != null )
			{
			isDisplayingExtraPromptLine_ = true;

			if( writeText( false, true, true, promptTypeNr, Constants.NO_CENTER_WIDTH, null, textStringBuffer1.toString() ) == Constants.RESULT_OK )
				{
				if( textStringBuffer2 != null &&
				textStringBuffer2.length() > 0 )
					return writeText( false, true, false, promptTypeNr, Constants.NO_CENTER_WIDTH, null, textStringBuffer2.toString() );
				}
			}
		else
			{
			displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given first diacritical text string buffer is undefined" );
			GlobalVariables.result = Constants.RESULT_ERROR;
			}

		return GlobalVariables.result;
		}

	protected static byte writeText( boolean isReturningToPosition, short promptTypeNr, int queryWidth, StringBuffer textStringBuffer )
		{
		if( textStringBuffer != null )
			return writeText( false, true, isReturningToPosition, promptTypeNr, queryWidth, null, textStringBuffer.toString() );

		displayError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string buffer is undefined" );
		GlobalVariables.result = Constants.RESULT_ERROR;
		return GlobalVariables.result;
		}

	protected static byte writeText( boolean isSkippingInTestFile, boolean isReturningToPosition, short promptTypeNr, int queryWidth, String textString )
		{
		return writeText( isSkippingInTestFile, true, isReturningToPosition, promptTypeNr, queryWidth, null, textString );
		}
	};

/*************************************************************************
 *	"The voice of the Lord twists mighty oaks
 *	and strips the forest bare.
 *	In his temple everyone shouts, "Glory!"." (Psalm 29:9)
 *************************************************************************/
