/*	Class:			AdminReadFile
 *	Supports class:	AdminItem
 *	Purpose:		To read the lines from knowledge files
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

import java.io.IOException;
import java.util.Date;

class AdminReadFile
	{
	// Private constructed variables

	private boolean hasClosedFileDueToError_;
	private boolean hasFoundDifferentTestResult_;

	private short testFileNr_;

	long startTime_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean hasAnyUserSpecification()
		{
		WordItem currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = CommonVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem.hasAnyUserSpecification() &&
				!currentSpecificationWordItem.isAuthorizationRequiredForChanges() )
					return true;
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}

		return false;
		}

	private boolean isDisplayingLine()
		{
		if( !adminItem_.isSystemStartingUp() &&
		adminItem_.fileList != null )
			return adminItem_.fileList.isDisplayingLine();

		return false;
		}

	private static boolean isGrammarChar( char grammarChar )
		{
		return ( grammarChar == Constants.QUERY_WORD_TYPE_CHAR ||
				grammarChar == Constants.QUERY_PARAMETER_CHAR ||
				grammarChar == Constants.GRAMMAR_WORD_DEFINITION_CHAR );
		}

	private FileResultType openFile( boolean isAddingSubPath, boolean isInfoFile, boolean isTestFile, boolean isReportingErrorIfFileDoesNotExist, String defaultSubpathString, String fileNameString, String writeSubpathString, String referenceSubpathString )
		{
		FileResultType fileResult = new FileResultType();
		FileList fileList;

		if( ( fileList = adminItem_.fileList ) == null )
			{
			fileResult.result = adminItem_.startError( 1, moduleNameString_, "The file list isn't created yet" );
			return fileResult;
			}

		return fileList.openFile( isAddingSubPath, isInfoFile, isTestFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString, writeSubpathString, referenceSubpathString );
		}

	private byte readLanguageFile( boolean isGrammarFile, String languageNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedLanguageFileItem;

		if( languageNameString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given language name is undefined" );

		Console.displayProgressStatus( languageNameString );

		if( ( fileResult = openFile( true, false, false, false, ( isGrammarFile ? Constants.FILE_DATA_GRAMMAR_DIRECTORY_NAME_STRING : Constants.FILE_DATA_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, ( isGrammarFile ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ) + languageNameString + "\"" );

		if( ( openedLanguageFileItem = fileResult.createdFileItem ) != null )
			{
			if( adminItem_.createLanguage( languageNameString ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to create language: \"" + languageNameString + "\"" );

			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened language file" );

			originalResult = CommonVariables.result;

			if( closeCurrentFileItem( openedLanguageFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the language file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			}
		else
			{
			// When reading special characters goes wrong, the wrong Zip file may be downloaded
			// The Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and higher
			if( languageNameString.equals( Constants.FILE_WINDOWS_VERSION_STRING ) )
				return adminItem_.startSystemError( 1, moduleNameString_, "You have probably downloaded the Zip file for the wrong Windows version.\nPlease check if the downloaded file name matches your Windows version.\nThe Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and higher" );

			return adminItem_.startError( 1, moduleNameString_, ( isGrammarFile ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ) + languageNameString + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte incrementCurrentSentenceNr()
		{
		if( CommonVariables.currentSentenceNr >= Constants.MAX_SENTENCE_NR )
			return adminItem_.startSystemError( 1, moduleNameString_, "Sentence number overflow! I can't except anymore input" );

		CommonVariables.currentSentenceNr++;
		// Necessary after changing current sentence number
		adminItem_.setCurrentItemNr();

		return Constants.RESULT_OK;
		}

	private byte executeLine( StringBuffer readStringBuffer )
		{
		boolean wasQueryCommand = false;
		boolean hasSwitchedLanguage = false;
		boolean wasUndoOrRedoCommand;

		if( readStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read string buffer is undefined" );

		// Skip empty line
		if( readStringBuffer.length() > 0 &&
		// and comment line
		readStringBuffer.charAt( 0 ) != Constants.COMMENT_CHAR )
			{
			CommonVariables.hasDisplayedMessage = false;
			CommonVariables.hasDisplayedWarning = false;
			CommonVariables.isAssignmentChanged = false;

			if( !adminItem_.wasPreviousCommandUndoOrRedo() )
				{
				if( adminItem_.cleanupDeletedItems() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to cleanup the deleted items" );
				}

			hasClosedFileDueToError_ = false;

			// Guide by Grammar, grammar/language or query
			if( readStringBuffer.charAt( 0 ) == Constants.QUERY_CHAR )
				{
					// Grammar/language
					if( Character.isLetter( readStringBuffer.charAt( 1 ) ) )
						{
						if( adminItem_.isSystemStartingUp() )
							{
							if( readGrammarFileAndUserInterfaceFile( readStringBuffer.substring( 1 ) ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to read the language" );
							}
						else
							{
							// Change language
							if( adminItem_.assignLanguage( readStringBuffer.substring( 1 ) ) == Constants.RESULT_OK )
								hasSwitchedLanguage = true;
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );
							}
						}
					else
						{
						// Query
						if( readStringBuffer.charAt( 0 ) == Constants.QUERY_CHAR )
							{
							wasQueryCommand = true;
							adminItem_.initializeQueryStringPosition();

							if( adminItem_.executeQuery( false, true, true, Constants.PRESENTATION_PROMPT_QUERY, readStringBuffer.toString() ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to execute query: \"" + readStringBuffer + "\"" );
							}
						}
				}
			else
				{
				// Sentence or grammar definition
				if( incrementCurrentSentenceNr() != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to increment the current sentence number" );

				// Grammar definition
				if( isGrammarChar( readStringBuffer.charAt( 0 ) ) )
					{
					if( adminItem_.addGrammar( readStringBuffer.toString() ) != Constants.RESULT_OK )
						adminItem_.addError( 1, moduleNameString_, "I failed to add grammar: \"" + readStringBuffer + "\"" );
					}
				else
					{
					// Sentence
					if( adminItem_.processReadSentence( readStringBuffer.toString() ) != Constants.RESULT_OK )
						adminItem_.addError( 1, moduleNameString_, "I failed to process a read sentence" );
					}
				}

			if( CommonVariables.result != Constants.RESULT_SYSTEM_ERROR &&
			!adminItem_.hasRequestedRestart() )
				{
				wasUndoOrRedoCommand = adminItem_.wasUndoOrRedoCommand();

				if( CommonVariables.result == Constants.RESULT_OK &&
				!CommonVariables.hasDisplayedWarning )
					{
					if( adminItem_.hasAnyChangeBeenMadeByThisSentence() )
						{
						if( !hasSwitchedLanguage &&

						( wasUndoOrRedoCommand ||
						!CommonVariables.hasDisplayedMessage ) )
							{
							if( adminItem_.executeSelections() != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}
						}
					else
						{
						if( !hasSwitchedLanguage &&
						!CommonVariables.hasDisplayedMessage &&
						!adminItem_.wasLoginCommand() &&
						!adminItem_.isSystemStartingUp() )
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I know' interface notification" );
							}
						}
					}

				adminItem_.clearTemporaryAdminLists();

				if( !wasQueryCommand &&
				!wasUndoOrRedoCommand )
					{
					if( adminItem_.cleanupDeletedItems() != Constants.RESULT_OK )
						adminItem_.addError( 1, moduleNameString_, "I failed to cleanup the deleted items" );

					if( !adminItem_.isSystemStartingUp() )
						{
						// Check for empty sentence
						adminItem_.setCurrentItemNr();

						if( CommonVariables.currentItemNr == Constants.NO_ITEM_NR )
							adminItem_.decrementCurrentSentenceNr();
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte closeCurrentFileItem( FileItem closeFileItem )
		{
		boolean isTestFile;
		int testFileSentenceNr;
		FileList fileList;

		if( ( fileList = adminItem_.fileList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The file list isn't created yet" );

		// Get info before closing the file
		isTestFile = fileList.isCurrentFileTestFile();
		testFileSentenceNr = fileList.currentFileSentenceNr();

		if( fileList.closeCurrentFile( closeFileItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to close a file" );

		if( isTestFile )
			{
			if( adminItem_.deleteSentences( testFileSentenceNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the test sentences added during read the test file" );
			}
		else
			{
			if( CommonVariables.hasDisplayedWarning ||
			CommonVariables.result != Constants.RESULT_OK )
				{
				hasClosedFileDueToError_ = true;
				CommonVariables.hasDisplayedWarning = false;
				}
			}

		return Constants.RESULT_OK;
		}

	private byte readGrammarFileAndUserInterfaceFile( String readLanguageNameString )
		{
		String startupLanguageNameString = adminItem_.startupLanguageNameString;
		WordItem currentLanguageWordItem;
		WordItem predefinedNounStartupLanguageWordItem;

		if( readLanguageNameString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read language name string is undefined" );

		// Read the user-interface file
		if( readLanguageFile( false, readLanguageNameString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to read an interface file" );

		// Read the grammar file
		if( readLanguageFile( true, readLanguageNameString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to read a grammar file" );

		if( startupLanguageNameString != null &&
		( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null &&
		( predefinedNounStartupLanguageWordItem = adminItem_.predefinedNounStartupLanguageWordItem() ) != null )
			{
			if( predefinedNounStartupLanguageWordItem.addSpecificationInWord( false, false, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, currentLanguageWordItem, null, null, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a predefined noun startup language specification" );

			if( startupLanguageNameString.equals( currentLanguageWordItem.anyWordTypeString() ) )
				{
				if( predefinedNounStartupLanguageWordItem.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, currentLanguageWordItem, null, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign the predefined noun startup language word" );
				}
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminReadFile( AdminItem adminItem )
		{
		String errorString = null;

		hasClosedFileDueToError_ = false;
		hasFoundDifferentTestResult_ = false;

		testFileNr_ = 0;
		startTime_ = 0;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected boolean hasClosedFileDueToError()
		{
		return hasClosedFileDueToError_;
		}

	protected byte compareOutputFileAgainstReferenceFile( String testFileNameString )
		{
		FileResultType fileResult;
		boolean isStop = false;
		int lineNr = 0;
		String outputString = null;
		String referenceString = null;

		hasFoundDifferentTestResult_ = false;

		if( ( fileResult = openFile( true, false, false, true, Constants.FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, testFileNameString, null, Constants.FILE_DATA_REGRESSION_TEST_REFERENCE_DIRECTORY_NAME_STRING ) ).result != Constants.RESULT_OK )
			return adminItem_.startError( 1, moduleNameString_, "I failed to open a test file" );

		if( fileResult.outputFile == null )
			return adminItem_.startError( 1, moduleNameString_, "The output file is undefined" );

		if( fileResult.referenceFile == null )
			return adminItem_.startError( 1, moduleNameString_, "The reference file is undefined" );

		do	{
			isStop = true;
			hasFoundDifferentTestResult_ = true;

			try	{
				outputString = fileResult.outputFile.readLine();
				referenceString = fileResult.referenceFile.readLine();
				}
			catch( IOException exception )
				{
				return adminItem_.startError( 1, moduleNameString_, "I couldn't read from the output file or the reference file" );
				}

			if( outputString != null &&
			referenceString != null )
				{
				if( !outputString.equals( referenceString ) )
					return adminItem_.startError( 1, moduleNameString_, "Line number " + lineNr + " is different:\n* test result:\t\t\"" + outputString + "\"\n* against reference:\t\"" + referenceString + "\"" );

				isStop = false;
				lineNr++;
				}
			else
				{
				if( outputString != null )
					return adminItem_.startError( 1, moduleNameString_, "The output file is longer than the reference file" );

				if( referenceString != null )
					return adminItem_.startError( 1, moduleNameString_, "The output file is shorter than the reference file" );

				hasFoundDifferentTestResult_ = false;
				}
			}
		while( !isStop );

		try	{
			if( fileResult.outputFile != null )
				fileResult.outputFile.close();

			if( fileResult.referenceFile != null )
				fileResult.referenceFile.close();
			}
		catch( IOException exception )
			{
			return adminItem_.startError( 1, moduleNameString_, "I couldn't close the output file or the reference file" );
			}

		return Constants.RESULT_OK;
		}

	protected byte readAndExecute()
		{
		boolean isLineExecuted;
		boolean isFirstLine = true;
		StringBuffer readStringBuffer = new StringBuffer();

		hasFoundDifferentTestResult_ = false;

		do	{
			isLineExecuted = false;
			readStringBuffer = new StringBuffer();

			if( readLine( false, isFirstLine, false, false, false, false, ( CommonVariables.currentSentenceNr + 1 ), adminItem_.currentUserName(), readStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to read a line" );

			if( Presentation.hasReadLine() )
				{
				if( executeLine( readStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to execute the read line" );

				isFirstLine = false;
				isLineExecuted = true;
				}
			}
		while( isLineExecuted &&
		!hasFoundDifferentTestResult_ &&
		!adminItem_.hasRequestedRestart() &&
		!Console.isTestingCanceled() &&

		// Ignore warnings during testing
		( adminItem_.isCurrentlyTesting() ||
		!CommonVariables.hasDisplayedWarning ) );

		if( adminItem_.isSystemStartingUp() &&
		CommonVariables.hasDisplayedWarning )
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;

		return Constants.RESULT_OK;
		}

	protected byte readExampleFile( String exampleFileNameString )
		{
		FileResultType fileResult;
		byte originalResult;
		FileItem openedExampleFileItem;

		if( exampleFileNameString != null )
			{
			if( adminItem_.isSystemStartingUp() )
				Console.displayProgressStatus( exampleFileNameString );

			if( ( fileResult = openFile( !adminItem_.isSystemStartingUp(), false, false, true, Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString, null, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to open an example file" );

			if( ( openedExampleFileItem = fileResult.createdFileItem ) != null )
				{
				if( readAndExecute() != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened example file" );

				originalResult = CommonVariables.result;

				if( closeCurrentFileItem( openedExampleFileItem ) != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to close the example file item" );

				if( originalResult != Constants.RESULT_OK )
					CommonVariables.result = originalResult;
				}
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	protected byte readLine( boolean isClearInputField, boolean isFirstLine, boolean isGuideByGrammar, boolean isPassword, boolean isQuestion, boolean isText, int promptSentenceNr, String promptUserNameString, StringBuffer readStringBuffer )
		{
		StringBuffer promptStringBuffer = new StringBuffer();

		if( promptSentenceNr > Constants.NO_SENTENCE_NR )
			promptStringBuffer.append( promptSentenceNr );

		if( promptUserNameString != null )
			{
			if( promptStringBuffer.length() > 0 )
				promptStringBuffer.append( Constants.QUERY_SEPARATOR_STRING );

			promptStringBuffer.append( promptUserNameString );
			}

		if( Presentation.readLine( isClearInputField, isDisplayingLine(), isFirstLine, isGuideByGrammar, isPassword, isQuestion, isText, promptStringBuffer.toString(), readStringBuffer, ( adminItem_.fileList == null ? null : adminItem_.fileList.currentReadFile() ) ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to read a line from a file or from input" );

		return Constants.RESULT_OK;
		}

	protected byte readStartupFile()
		{
		FileResultType fileResult;
		byte originalResult;
		FileItem openedStartupFileItem;

		if( adminItem_.fileList == null )
			{
			// Create list
			if( ( adminItem_.fileList = new FileList( adminItem_ ) ) == null )
				return adminItem_.startError( 1, moduleNameString_, "I failed to create an admin file list" );

			adminItem_.adminListArray[Constants.ADMIN_FILE_LIST] = adminItem_.fileList;
			}

		if( ( fileResult = openFile( true, false, false, true, Constants.FILE_DATA_STARTUP_DIRECTORY_NAME_STRING, Constants.FILE_STARTUP_NAME_STRING, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to open an startup file" );

		if( ( openedStartupFileItem = fileResult.createdFileItem ) != null )
			{
			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened startup file" );

			originalResult = CommonVariables.result;

			if( closeCurrentFileItem( openedStartupFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the startup file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			}

		return Constants.RESULT_OK;
		}

	protected byte readTestFile( String testFileNameString )
		{
		FileResultType fileResult;
		byte originalResult;
		boolean isFirstTestFile = ( !adminItem_.isCurrentlyTesting() &&
									adminItem_.isCurrentUserDeveloper() );
		FileItem testFileItem;

		// Check if user has already entered sentences
		if( hasAnyUserSpecification() )
			return adminItem_.startError( 1, moduleNameString_, "Some knowledge is already entered. It may disturb the test results. Please, use restart button before testing to ensure no knowledge is present" );

		if( isFirstTestFile )
			{
			testFileNr_ = 0;
				startTime_ = new Date().getTime();
			}
		else
			testFileNr_++;

		if( ( fileResult = openFile( true, false, true, true, Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, testFileNameString, Constants.FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to open a test file" );

		if( ( testFileItem = fileResult.createdFileItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The last created file item is undefined" );

			if( Presentation.writeText( true, true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.NO_CENTER_WIDTH, "Test file #" + testFileNr_ + ": " + testFileItem.readFileNameString() + Constants.NEW_LINE_STRING ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a text" );

			Presentation.redirectOutputToTestFile( testFileItem.writeFile() );

			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened test file" );

			if( ( originalResult = CommonVariables.result ) != Constants.RESULT_OK )
				CommonVariables.result = Constants.RESULT_OK;

			if( closeCurrentFileItem( testFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the test file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			else
				{
				if( isFirstTestFile &&
				!Console.isTestingCanceled() )
					{
					if( Presentation.writeText( true, true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.NO_CENTER_WIDTH, "Done in: " + ( ( new Date().getTime() - startTime_ ) / 1000.0 ) + " sec.\n" ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write a text" );
					}
				}

		return Constants.RESULT_OK;
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedInfoFileItem;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;
		StringBuffer infoPathStringBuffer = new StringBuffer( Constants.FILE_DATA_INFO_DIRECTORY_NAME_STRING );

		if( infoFileNameString == null )
			return adminItem_.startFileResultError( 1, moduleNameString_, "The given info file name string is undefined" );

		if( currentLanguageWordItem != null )
			infoPathStringBuffer.append( currentLanguageWordItem.anyWordTypeString() + Constants.SYMBOL_SLASH );

		if( ( fileResult = openFile( true, true, false, isReportingErrorIfFileDoesNotExist, infoPathStringBuffer.toString(), infoFileNameString, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addFileResultError( 1, moduleNameString_, "I failed to open the info file" );

		if( ( openedInfoFileItem = fileResult.createdFileItem ) != null )
			{
			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened info file" );

			originalResult = CommonVariables.result;

			if( closeCurrentFileItem( openedInfoFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the info file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			}

		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
