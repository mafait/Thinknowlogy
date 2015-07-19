/*
 *	Class:		Presentation
 *	Purpose:	To format the information presented to the user
 *	Version:	Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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
import java.io.IOException;

class Presentation
	{
	// Private static variables

	private static boolean hasReadLine_;
	private static boolean isExpertUser_;
	private static boolean isFirstTimeAfterReadingFile_;
	private static boolean isShowingExtraPromptLine_;
	private static boolean wasPreviousPromptGuidedByGrammarInput_;

	private static short lastUsedPromptTypeNr_;
	private static short lastShownInterfaceParameter_;

	private static int currentPosition_;
	private static int previousProgress_;

	private static StringBuffer outputStringBuffer_;
	private static StringBuffer currentStatusStringBuffer_;

	private static String classNameString_ = "Presentation";


	// Private static methods

	private static void returnOutputToPosition( boolean isWriting )
		{
		currentPosition_ = 0;
		addStringToOutput( isWriting, Constants.CARRIAGE_RETURN_NEW_LINE_STRING );
		}

	private static void addStringToOutput( boolean isWriting, String writeString )
		{
		outputStringBuffer_.append( writeString );

		if( isWriting &&
		outputStringBuffer_.length() > 0 )
			{
				Console.writeText( outputStringBuffer_.toString() );
			outputStringBuffer_ = new StringBuffer();
			}
		}

	private static void showStatus( boolean isPassword, short interfaceParameter )
		{
		String newStatusString = ( CommonVariables.currentLanguageWordItem == null ? Constants.NO_LANGUAGE_WORD_FOUND : CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter ) );
		
		if( newStatusString == null )
			clearStatus();
		else
			{
			if( currentStatusStringBuffer_ == null ||
			currentStatusStringBuffer_.indexOf( newStatusString ) == -1 )	// Different string
				{
				currentStatusStringBuffer_ = new StringBuffer( newStatusString + ( newStatusString.endsWith( "." ) ? Constants.EMPTY_STRING : "..." ) );

				if( isPassword )
					Console.showProgressStatus( currentStatusStringBuffer_.toString() );
				}
			}
		}

	private static byte writeText( boolean isForcingEmptyString, boolean isFirstCharacterToUpperCase, boolean isReturningToPosition, short promptTypeNr, int leftWidth, int rightWidth, int printStringLength, String promptString, String writeString )
		{
		boolean startNewLine = false;
		int wordPosition;
		int i = 0;
		int length = 0;
		int firstLetterPosition = 0;
		String textString;

		if( writeString != null )
			{
			if( isForcingEmptyString ||
			printStringLength > 0 ||
			leftWidth > Constants.NO_CENTER_WIDTH ||
			rightWidth > Constants.NO_CENTER_WIDTH )
				{
				if( currentPosition_ > 0 &&

				( isReturningToPosition ||
				isFirstTimeAfterReadingFile_ ) )
					returnOutputToPosition( false );

				isFirstTimeAfterReadingFile_ = false;

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

				( promptTypeNr == Constants.PRESENTATION_PROMPT_WRITE ||
				promptTypeNr == Constants.PRESENTATION_PROMPT_WARNING ) )
					textString = writeString.substring( 0, firstLetterPosition ) + Character.toUpperCase( writeString.charAt( firstLetterPosition ) ) + ( firstLetterPosition + 1 < printStringLength ? writeString.substring( ( firstLetterPosition + 1 ), printStringLength ) : Constants.EMPTY_STRING );
				else
					textString = writeString.substring( 0, printStringLength );

				if( promptString == null )
					{
					promptString = getPromptString( promptTypeNr );

					if( isForcingEmptyString &&
					printStringLength == 0 &&
					promptString != null )
						{
						currentPosition_ = promptString.length() - 1;
						addStringToOutput( false, promptString );
						}
					}

				if( promptString != null )
					{
					while( i < printStringLength ||
					leftWidth > Constants.NO_CENTER_WIDTH )
						{
						if( currentPosition_ == 0 )
							{
							currentPosition_ = promptString.length() - 1;
							addStringToOutput( false, promptString );
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
								addStringToOutput( ( i + 1 == printStringLength ), Constants.CARRIAGE_RETURN_NEW_LINE_STRING );
	
								i++;
								currentPosition_ = 0;
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
								addStringToOutput( false, Constants.SPACE_STRING );
								}
	
							if( i < printStringLength )
								{
								if( textString.charAt( i ) == Constants.TAB_CHAR )
									{
									addStringToOutput( ( i + 1 == printStringLength ), Constants.EMPTY_STRING + textString.charAt( i ) );

									i++;
									currentPosition_ += length;
									}
								else
									{
									do	{
										addStringToOutput( ( i + 1 == printStringLength ), Constants.EMPTY_STRING + textString.charAt( i ) );
	
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
					addStringToOutput( ( ( i + 1 == printStringLength ) && rightWidth == Constants.NO_CENTER_WIDTH ), Constants.SPACE_STRING );
					}
				}
			else
				{
				showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given write string is empty" );
				CommonVariables.result = Constants.RESULT_ERROR;
				}
			}
		else
			{
			showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given write string is undefined" );
			CommonVariables.result = Constants.RESULT_ERROR;
			}

		return CommonVariables.result;
		}

	private static byte writeText( boolean isForcingEmptyString, boolean isFirstCharacterToUpperCase, boolean isReturningToPosition, short promptTypeNr, int queryWidth, String promptString, String textString )
		{
		int widthDifference;
		int leftWidth = Constants.NO_CENTER_WIDTH;
		int rightWidth = Constants.NO_CENTER_WIDTH;
		int textStringLength;

		if( textString != null )
			{
			if( isForcingEmptyString ||
			textString.length() > 0 ||
			queryWidth > Constants.NO_CENTER_WIDTH )
				{
				clearStatus();
				clearProgress();
				textStringLength = textString.length();

				if( promptTypeNr == Constants.PRESENTATION_PROMPT_WARNING )
					CommonVariables.hasShownWarning = true;
				else
					CommonVariables.hasShownMessage = true;

				if( queryWidth > 0 &&
				queryWidth < textStringLength )
					textStringLength = queryWidth;

				if( queryWidth >= textStringLength )
					{
					widthDifference = ( queryWidth - textStringLength );
					leftWidth = ( widthDifference / 2 );
					rightWidth = ( widthDifference - leftWidth );
					}

					if( writeText( isForcingEmptyString, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, leftWidth, rightWidth, textStringLength, promptString, textString ) == Constants.RESULT_OK )
					lastUsedPromptTypeNr_ = promptTypeNr;
				else
					showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the string" );
				}
			else
				{
				showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string is empty" );
				CommonVariables.result = Constants.RESULT_ERROR;
				}
			}
		else
			{
			showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string is undefined" );
			CommonVariables.result = Constants.RESULT_ERROR;
			}

		return CommonVariables.result;
		}

	private static String getPromptString( short promptTypeNr )
		{
		switch( promptTypeNr )
			{
			case Constants.PRESENTATION_PROMPT_READ:
				return Constants.PRESENTATION_PROMPT_READ_STRING;

			case Constants.PRESENTATION_PROMPT_WRITE:
				return Constants.PRESENTATION_PROMPT_WRITE_STRING;

			case Constants.PRESENTATION_PROMPT_INFO:
				return Constants.PRESENTATION_PROMPT_INFO_STRING;

			case Constants.PRESENTATION_PROMPT_NOTIFICATION:
				return Constants.PRESENTATION_PROMPT_NOTIFICATION_STRING;

			case Constants.PRESENTATION_PROMPT_WARNING:
				return Constants.PRESENTATION_PROMPT_WARNING_STRING;
			}

		return Constants.PRESENTATION_PROMPT_QUERY_STRING;
		}


	// Constructor initialization

	protected static void initialize()
		{
		hasReadLine_ = false;
		isExpertUser_ = false;
		isFirstTimeAfterReadingFile_ = false;
		isShowingExtraPromptLine_ = false;
		wasPreviousPromptGuidedByGrammarInput_ = false;

		currentPosition_ = 0;
		previousProgress_ = 0;
		lastUsedPromptTypeNr_ = Constants.PRESENTATION_PROMPT_QUERY;

		outputStringBuffer_ = new StringBuffer();
		currentStatusStringBuffer_ = null;
		}

	// Protected methods

	protected static void clearStatus()
		{
		currentStatusStringBuffer_ = null;
		Console.clearProgress();
		}

	protected static void clearProgress()
		{
		previousProgress_ = 0;
		Console.clearProgress();

		if( currentStatusStringBuffer_ != null )
			Console.showProgressStatus( currentStatusStringBuffer_.toString() );
		}

	protected static void showProgress( int currentProgress )
		{
		if( currentProgress != previousProgress_ )
			{
			previousProgress_ = currentProgress;
			Console.showProgress( currentProgress );
			}
		}

	protected static void startProgress( int startProgress, int maxProgress, short interfaceParameter1, short shortNumber, short interfaceParameter2 )
		{
		Console.startProgress( startProgress, maxProgress, ( CommonVariables.currentLanguageWordItem == null ? Constants.NO_LANGUAGE_WORD_FOUND : CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + shortNumber + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + "  " + Constants.QUERY_ITEM_START_STRING + CommonVariables.currentSentenceNr + ( CommonVariables.currentItemNr == Constants.NO_SENTENCE_NR ? Constants.EMPTY_STRING : Constants.QUERY_SEPARATOR_STRING + CommonVariables.currentItemNr ) + Constants.QUERY_ITEM_END_CHAR ) );
		}

	protected static void showError( char methodListChar, String classNameString, String subclassNameString, String wordNameString, int methodLevel, String errorString )
		{
		StringBuffer errorStringBuffer = new StringBuffer();
		StackTraceElement[] elements;
		Throwable t = new Throwable();

		addStringToOutput( true, Constants.EMPTY_STRING );	// Show remaining output

		if( classNameString != null )
			errorStringBuffer.append( Constants.PRESENTATION_ERROR_CLASS_STRING + classNameString );

		if( subclassNameString != null )
			errorStringBuffer.append( Constants.PRESENTATION_ERROR_SUPERCLASS_STRING + subclassNameString );

		if( ( elements = t.getStackTrace() ) != null )
			{
			if( elements.length > 0 )
				{
				if( methodLevel < elements.length )		// elements.[0] is this method
					errorStringBuffer.append( Constants.PRESENTATION_ERROR_METHOD_STRING + elements[methodLevel].getMethodName() );
				else
					errorStringBuffer.append( Constants.PRESENTATION_ERROR_METHOD_STRING + "{Invalid method level}" );
				}
			else
				errorStringBuffer.append( Constants.PRESENTATION_ERROR_METHOD_STRING + "{No stack information available}" );
			}
		else
			errorStringBuffer.append( Constants.PRESENTATION_ERROR_METHOD_STRING + "{Failed to create Throwable or StackTrace}" );

		if( methodListChar != Constants.SYMBOL_QUESTION_MARK )
			errorStringBuffer.append( Constants.PRESENTATION_ERROR_METHOD_LIST_START_STRING + methodListChar + Constants.PRESENTATION_ERROR_METHOD_LIST_END_STRING );

		if( wordNameString != null )
			errorStringBuffer.append( Constants.PRESENTATION_ERROR_METHOD_WORD_START_STRING + wordNameString + Constants.PRESENTATION_ERROR_METHOD_WORD_END_STRING );

		if( errorString != null )
			errorStringBuffer.append( Constants.PRESENTATION_ERROR_MESSAGE_START_STRING + errorString );

		errorStringBuffer.append( Constants.PRESENTATION_ERROR_MESSAGE_END_STRING );

		Console.addError( Constants.PRESENTATION_ERROR_CURRENT_ID_STRING + Constants.QUERY_ITEM_START_CHAR + CommonVariables.currentSentenceNr + Constants.QUERY_SEPARATOR_CHAR + CommonVariables.currentItemNr + Constants.QUERY_ITEM_END_CHAR, errorStringBuffer.toString() );
		}

	protected static boolean hasReadLine()
		{
		return hasReadLine_;
		}

	protected static boolean isExpertUser()
		{
		return isExpertUser_;
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

	protected static String convertDiacriticalText( String textString )
		{
		int position = 0;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer textStringBuffer = new StringBuffer();

		if( textString != null )
			{
			if( textString.charAt( 0 ) == Constants.SYMBOL_DOUBLE_QUOTE )
				position++;

			while( position < textString.length() &&
			textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE )
				{
				if( textString.charAt( position ) == Constants.TEXT_DIACRITICAL_CHAR )
					{
					if( ++position < textString.length() )
						{
						textChar = convertDiacriticalChar( textString.charAt( position ) );
						textStringBuffer.append( Constants.EMPTY_STRING + textChar );
						}
					}
				else
					{
					textChar = textString.charAt( position );
					textStringBuffer.append( Constants.EMPTY_STRING + textChar );
					}

				position++;
				}
			}

		return textStringBuffer.toString();
		}

	protected static byte writeText( boolean isReturningToPosition, short promptTypeNr, int queryWidth, StringBuffer textStringBuffer )
		{
		if( textStringBuffer != null )
			return writeText( false, true, isReturningToPosition, promptTypeNr, queryWidth, null, textStringBuffer.toString() );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string buffer is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeDiacriticalText( short promptTypeNr, String diacriticalTextString )
		{
		return writeDiacriticalText( true, true, promptTypeNr, diacriticalTextString );
		}

	protected static byte writeDiacriticalText( boolean isFirstCharacterToUpperCase, boolean isReturningToPosition, short promptTypeNr, String textString )
		{
		boolean hasFoundNewLine = false;
		int position = 0;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer textStringBuffer = new StringBuffer();

		if( textString != null )
			{
			isShowingExtraPromptLine_ = true;

			if( textString.charAt( 0 ) == Constants.SYMBOL_DOUBLE_QUOTE )
				position++;

			while( CommonVariables.result == Constants.RESULT_OK &&
			position < textString.length() &&
			textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE )
				{
				if( textString.charAt( position ) == Constants.TEXT_DIACRITICAL_CHAR )
					{
					if( ++position < textString.length() )
						{
						if( ( textChar = convertDiacriticalChar( textString.charAt( position ) ) ) == Constants.NEW_LINE_CHAR )
							hasFoundNewLine = true;
						}
					else
						{
						showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The text string ended with a diacritical sign" );
						CommonVariables.result = Constants.RESULT_ERROR;
						}
					}
				else
					textChar = textString.charAt( position );

				position++;
				textStringBuffer.append( Constants.EMPTY_STRING + textChar );

				if( hasFoundNewLine ||

				( textStringBuffer.length() > 0 &&
				position < textString.length() &&
				textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE &&
				textString.charAt( position ) == Constants.QUERY_CHAR ) )
					{
					if( writeText( false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, Constants.NO_CENTER_WIDTH, null, textStringBuffer.toString() ) == Constants.RESULT_OK )
						{
						hasFoundNewLine = false;
						textStringBuffer = new StringBuffer();
						}
					else
						showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write a character" );
					}
				}

			if( CommonVariables.result == Constants.RESULT_OK &&
			textStringBuffer.length() > 0 )
				{
				if( writeText( false, isFirstCharacterToUpperCase, isReturningToPosition, promptTypeNr, Constants.NO_CENTER_WIDTH, null, textStringBuffer.toString() ) != Constants.RESULT_OK )
					showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the last characters" );
				}
			}
		else
			{
			showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given text string is undefined" );
			CommonVariables.result = Constants.RESULT_ERROR;
			}

		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( boolean isCheckingForDuplicateInterfaceParameter, short promptTypeNr, short interfaceParameter )
		{
		return writeInterfaceText( isCheckingForDuplicateInterfaceParameter, true, promptTypeNr, interfaceParameter );
		}

	protected static byte writeInterfaceText( boolean isCheckingForDuplicateInterfaceParameter, boolean isReturningToPosition, short promptTypeNr, short interfaceParameter )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			{
			if( !isCheckingForDuplicateInterfaceParameter ||
			interfaceParameter != lastShownInterfaceParameter_ )
				{
				lastShownInterfaceParameter_ = interfaceParameter;
				return writeDiacriticalText( true, isReturningToPosition, promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter ) );
				}
			}
		else
			{
			showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
			CommonVariables.result = Constants.RESULT_ERROR;
			}

		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber, short interfaceParameter2 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber1, short interfaceParameter2, int intNumber2, short interfaceParameter3 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber1 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + intNumber2 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber, short interfaceParameter2, String textString, short interfaceParameter3 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + textString + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, int intNumber1, short interfaceParameter2, int intNumber2, short interfaceParameter3, String textString, short interfaceParameter4 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + intNumber1 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + intNumber2 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter3 ) + textString + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter4 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( boolean isCheckingForDuplicateInterfaceParameter, short promptTypeNr, short interfaceParameter1, String textString, short interfaceParameter2 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			{
			if( !isCheckingForDuplicateInterfaceParameter ||
			interfaceParameter1 != lastShownInterfaceParameter_ )
				{
				lastShownInterfaceParameter_ = interfaceParameter1;
				return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) );
				}
			}
		else
			{
			showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
			CommonVariables.result = Constants.RESULT_ERROR;
			}

		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, String textString1, short interfaceParameter2, String textString2, short interfaceParameter3 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString1 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + textString2 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, String textString1, short interfaceParameter2, String textString2, short interfaceParameter3, String textString3, short interfaceParameter4 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString1 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + textString2 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter3 ) + textString3 + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter4 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte writeInterfaceText( short promptTypeNr, short interfaceParameter1, String textString, short interfaceParameter2, int intNumber, short interfaceParameter3 )
		{
		if( CommonVariables.currentLanguageWordItem != null )
			return writeDiacriticalText( promptTypeNr, CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter1 ) + textString + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter2 ) + intNumber + CommonVariables.currentLanguageWordItem.interfaceString( interfaceParameter3 ) );

		showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The current interface language word is undefined" );
		CommonVariables.result = Constants.RESULT_ERROR;
		return CommonVariables.result;
		}

	protected static byte readLine( boolean isClearInputField, boolean isExpertUser, boolean isFirstLine, boolean isGuidedByGrammarPrompt, boolean isPassword, boolean isQuestion, boolean isText, boolean showline, String promptString, StringBuffer readStringBuffer, BufferedReader readFile )
		{
		int endPosition;
		int startPosition = 0;
		String readString = new String();
		StringBuffer promptStringBuffer = new StringBuffer();

		hasReadLine_ = false;
		lastShownInterfaceParameter_ = Constants.NO_INTERFACE_PARAMETER;

		if( readStringBuffer != null )
			{
			if( isText ||
			isQuestion )
				promptStringBuffer.append( Constants.PRESENTATION_PROMPT_WRITE_STRING );

			if( promptString != null )
				{
				if( isQuestion &&
				promptString.length() > 0 &&
				Character.isLetter( promptString.charAt( 0 ) ) )
					promptStringBuffer.append( Character.toUpperCase( promptString.charAt( 0 ) ) + promptString.substring( 1 ) );
				else
					promptStringBuffer.append( promptString );

				isExpertUser_ = isExpertUser;
				}

			promptStringBuffer.append( isQuestion ? Constants.PRESENTATION_PROMPT_QUERY_STRING : ( isText ? Constants.PRESENTATION_PROMPT_ENTER_STRING : Constants.PRESENTATION_PROMPT_READ_STRING ) );

			if( currentPosition_ > 0 )
				returnOutputToPosition( true );

			if( isShowingExtraPromptLine_ )
				{
				if( writeText( false, true, true, lastUsedPromptTypeNr_, Constants.NO_CENTER_WIDTH, null, Constants.NEW_LINE_STRING ) == Constants.RESULT_OK )
					isShowingExtraPromptLine_ = false;
				else
					showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the line" );
				}

			if( CommonVariables.result == Constants.RESULT_OK )
				{
				if( isPassword ||
				readFile == null )
					{
					currentPosition_ = 0;

					if( isPassword )
						{
						showStatus( true, Constants.INTERFACE_CONSOLE_WAITING_FOR_SECURE_INPUT );
						addStringToOutput( true, promptStringBuffer.toString() + Constants.NEW_LINE_STRING );

						if( ( readString = Console.getPassword() ) != null )
							hasReadLine_ = true;
						else
							{
							showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The returned password string is undefined" );
							CommonVariables.result = Constants.RESULT_ERROR;
							}
						}
					else
						{
						if( isClearInputField &&
						!wasPreviousPromptGuidedByGrammarInput_ )
							showStatus( false, Constants.INTERFACE_CONSOLE_WAITING_FOR_INPUT );
						else
							clearStatus();

						// Avoid extra prompt on the screen
						if( isGuidedByGrammarPrompt ||
						!wasPreviousPromptGuidedByGrammarInput_ )
							addStringToOutput( true, promptStringBuffer.toString() );

						if( ( readString = Console.readLine( isClearInputField ) ) != null )
							hasReadLine_ = true;
						else
							{
							showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The returned read line string is undefined" );
							CommonVariables.result = Constants.RESULT_ERROR;
							}
						}

					wasPreviousPromptGuidedByGrammarInput_ = isGuidedByGrammarPrompt;
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

							// Strip Java comment from line
							if( readString.startsWith( Constants.PRESENTATION_STRIP_COMMENT_STRING ) )
								readString = readString.substring( Constants.PRESENTATION_STRIP_COMMENT_STRING.length() );

							if( showline &&
							// Skip C++ comment line
							!readString.startsWith( Constants.PRESENTATION_SKIP_COMMENT_STRING ) )
								{
								if( writeText( false, true, true, Constants.PRESENTATION_PROMPT_READ, Constants.NO_CENTER_WIDTH, promptStringBuffer.toString(), ( readString.length() == 0 ? Constants.NEW_LINE_STRING : readString ) ) != Constants.RESULT_OK )
									showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "I failed to write the text" );
								}
							}
						}
					catch( IOException exception )
						{
						showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, exception.toString() );
						CommonVariables.result = Constants.RESULT_ERROR;
						}
					}

				if( CommonVariables.result == Constants.RESULT_OK &&
				hasReadLine_ &&
				readString != null &&
				readString.length() > 0 )
					{
					isFirstTimeAfterReadingFile_ = true;

					while( readString.substring( startPosition ).length() > 0 &&
					Character.isWhitespace( readString.charAt( startPosition ) ) )
						startPosition++;

					endPosition = readString.length();

					while( endPosition > startPosition &&
					Character.isWhitespace( readString.charAt( endPosition - 1 ) ) )
						endPosition--;

					readStringBuffer.append( readString.substring( startPosition, endPosition ) );
					}
				}
			}
		else
			{
			showError( Constants.SYMBOL_QUESTION_MARK, classNameString_, null, null, 1, "The given read string buffer is undefined" );
			CommonVariables.result = Constants.RESULT_ERROR;
			}

		return CommonVariables.result;
		}
	};

/*************************************************************************
 *	"The voice of the Lord twists mighty oaks
 *	and strips the forest bare.
 *	In his temple everyone shouts, "Glory!"." (Psalm 29:9)
 *************************************************************************/
