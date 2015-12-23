/*
 *	Class:			AdminReadFile
 *	Supports class:	AdminItem
 *	Purpose:		To read the lines from knowledge files
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
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

import java.io.IOException;
import java.util.Date;

class AdminReadFile
	{
	// Private constructible variables

	private short testFileNr_;
	long startTime_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean hasFoundAnyUserSpecification()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.hasFoundAnyUserSpecification() &&
				!currentWordItem.isNeedingAuthorizationForChanges() )
					return true;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return false;
		}

	private static boolean isGrammarChar( char grammarChar )
		{
		return ( grammarChar == Constants.QUERY_WORD_TYPE_CHAR ||
				grammarChar == Constants.QUERY_PARAMETER_CHAR ||
				grammarChar == Constants.GRAMMAR_WORD_DEFINITION_CHAR );
		}

	private boolean isShowingLine()
		{
		if( !adminItem_.isSystemStartingUp() &&
		adminItem_.fileList != null )
			return adminItem_.fileList.isShowingLine();

		return false;
		}

	private FileResultType openFile( boolean isAddingSubPath, boolean isInfoFile, boolean isTestFile, boolean isReportingErrorIfFileDoesNotExist, String defaultSubpathString, String fileNameString, String writeSubpathString, String referenceSubpathString )
		{
		FileResultType fileResult = new FileResultType();
		FileList fileList;

		if( ( fileList = adminItem_.fileList ) != null )
			return fileList.openFile( isAddingSubPath, isInfoFile, isTestFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString, writeSubpathString, referenceSubpathString );

		fileResult.result = adminItem_.startError( 1, moduleNameString_, "The file list isn't created yet" );
		return fileResult;
		}

	private byte readLanguageFile( boolean isGrammarFile, String languageNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedLanguageFileItem;

		if( languageNameString != null )
			{
			Console.showProgressStatus( languageNameString );

			if( ( fileResult = openFile( true, false, false, false, ( isGrammarFile ? Constants.FILE_DATA_GRAMMAR_DIRECTORY_NAME_STRING : Constants.FILE_DATA_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString, null, null ) ).result == Constants.RESULT_OK )
				{
				if( ( openedLanguageFileItem = fileResult.createdFileItem ) != null )
					{
					if( adminItem_.createLanguage( languageNameString ) == Constants.RESULT_OK )
						{
						if( readAndExecute() != Constants.RESULT_OK )
							adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened language file" );

						originalResult = CommonVariables.result;

						if( closeCurrentFileItem( openedLanguageFileItem ) != Constants.RESULT_OK )
							adminItem_.addError( 1, moduleNameString_, "I failed to close the language file item" );

						if( originalResult != Constants.RESULT_OK )
							CommonVariables.result = originalResult;
						}
					else
						adminItem_.addError( 1, moduleNameString_, "I failed to create language: \"" + languageNameString + "\"" );
					}
				else
					{
					// When reading special characters goes wrong, the wrong Zip file may be downloaded
					// The Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and further
					if( languageNameString.equals( Constants.FILE_WINDOWS_VERSION_STRING ) )
						return adminItem_.startSystemError( 1, moduleNameString_, "You have probably downloaded the Zip file for the wrong Windows version.\nPlease check if the downloaded file name matches your Windows version.\nThe Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and further" );

					return adminItem_.startError( 1, moduleNameString_, ( isGrammarFile ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ) + languageNameString + "\"" );
					}
				}
			else
				return adminItem_.addError( 1, moduleNameString_, ( isGrammarFile ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ) + languageNameString + "\"" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given language name is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkCurrentSentenceNr()
		{
		if( CommonVariables.isDontIncrementCurrentSentenceNr )
			{
			CommonVariables.isDontIncrementCurrentSentenceNr = false;

			if( adminItem_.deleteSentences( true, CommonVariables.currentSentenceNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the current redo info" );
			}
		else
			{
			if( CommonVariables.currentSentenceNr < Constants.MAX_SENTENCE_NR )
				{
				CommonVariables.currentSentenceNr++;
				// Necessary after changing current sentence number
				adminItem_.setCurrentItemNr();
				}
			else
				return adminItem_.startSystemError( 1, moduleNameString_, "Sentence number overflow! I can't except anymore input" );
			}

		return Constants.RESULT_OK;
		}

	private byte executeLine( StringBuffer readStringBuffer )
		{
		boolean hasSwitchedLanguage = false;

		if( readStringBuffer != null )
			{
			// Skip empty line
			if( readStringBuffer.length() > 0 &&
			// and comment line
			readStringBuffer.charAt( 0 ) != Constants.COMMENT_CHAR )
				{
				CommonVariables.hasShownMessage = false;
				CommonVariables.isAssignmentChanged = false;

				if( adminItem_.cleanupDeletedItems() == Constants.RESULT_OK )
					{
					// Guide-by-grammar, grammar/language or query
					if( readStringBuffer.charAt( 0 ) == Constants.QUERY_CHAR )
						{
						// Guide-by-grammar
						if( readStringBuffer.length() == 1 )
							{
							if( checkCurrentSentenceNr() == Constants.RESULT_OK )
								{
								if( adminItem_.processReadSentence( null ) != Constants.RESULT_OK )
									adminItem_.addError( 1, moduleNameString_, "I failed to process a read sentence" );
								}
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to check to current sentence number" );
							}
						else
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
									adminItem_.initializeQueryStringPosition();

									if( adminItem_.executeQuery( false, true, true, Constants.PRESENTATION_PROMPT_QUERY, readStringBuffer.toString() ) != Constants.RESULT_OK )
										adminItem_.addError( 1, moduleNameString_, "I failed to execute query: \"" + readStringBuffer + "\"" );
									}
								}
							}
						}
					else
						{
						// Sentence or grammar definition
						if( checkCurrentSentenceNr() == Constants.RESULT_OK )
							{
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
						else
							adminItem_.addError( 1, moduleNameString_, "I failed to check to current sentence number" );
						}

					if( CommonVariables.result != Constants.RESULT_SYSTEM_ERROR &&
					!adminItem_.hasRequestedRestart() )
						{
						if( CommonVariables.result == Constants.RESULT_OK &&
						!hasSwitchedLanguage &&
						!CommonVariables.hasShownWarning &&
						adminItem_.hasFoundAnyChangeMadeByThisSentence() &&

						// Execute selections after Undo or Redo
						( adminItem_.wasUndoOrRedo() ||
						!CommonVariables.hasShownMessage ) )
							{
							if( adminItem_.executeSelections() != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!hasSwitchedLanguage &&
						!adminItem_.hasFoundAnyChangeMadeByThisSentence() &&
						!adminItem_.isSystemStartingUp() &&
						!CommonVariables.hasShownMessage &&
						!CommonVariables.hasShownWarning )
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I know' interface notification" );
							}

						if( CommonVariables.result == Constants.RESULT_OK )
							{
							if( adminItem_.deleteAllTemporaryLists() != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to delete all temporary lists" );
							}
						}
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to cleanup the deleted items" );
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given read string buffer is undefined" );

		return Constants.RESULT_OK;
		}

	private byte closeCurrentFileItem( FileItem closeFileItem )
		{
		boolean isTestFile;
		int testFileSentenceNr;
		FileList fileList;

		if( ( fileList = adminItem_.fileList ) != null )
			{
			// Get info before closing the file
			isTestFile = fileList.isCurrentFileTestFile();
			testFileSentenceNr = fileList.currentFileSentenceNr();

			if( fileList.closeCurrentFile( closeFileItem ) == Constants.RESULT_OK )
				{
				if( isTestFile &&
				adminItem_.deleteSentences( false, testFileSentenceNr ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete the previous test sentences" );
				}
			else
				return adminItem_.addError( 1, moduleNameString_, "I failed to close a file" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The file list isn't created yet" );

		return Constants.RESULT_OK;
		}

	private byte readGrammarFileAndUserInterfaceFile( String readLanguageNameString )
		{
		String startupLanguageNameString = adminItem_.startupLanguageNameString;
		WordItem currentLanguageWordItem;
		WordItem predefinedNounStartupLanguageWordItem;

		if( readLanguageNameString != null )
			{
			// Read the user-interface file
			if( readLanguageFile( false, readLanguageNameString ) == Constants.RESULT_OK )
				{
				// Read the grammar file
				if( readLanguageFile( true, readLanguageNameString ) == Constants.RESULT_OK )
					{
					if( startupLanguageNameString != null &&
					( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null &&
					( predefinedNounStartupLanguageWordItem = adminItem_.predefinedNounStartupLanguageWordItem() ) != null )
						{
						if( predefinedNounStartupLanguageWordItem.addSpecification( false, false, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, currentLanguageWordItem, null, null, null ).result == Constants.RESULT_OK )
							{
							if( startupLanguageNameString.equals( currentLanguageWordItem.anyWordTypeString() ) )
								{
								if( predefinedNounStartupLanguageWordItem.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, currentLanguageWordItem, null, null ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to assign the predefined noun startup language word" );
								}
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a predefined noun startup language specification" );
						}
					}
				else
					adminItem_.addError( 1, moduleNameString_, "I failed to read a grammar file" );
				}
			else
				adminItem_.addError( 1, moduleNameString_, "I failed to read an interface file" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given read language name string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminReadFile( AdminItem adminItem )
		{
		String errorString = null;

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

	protected byte compareOutputFileAgainstReferenceFile( String testFileNameString )
		{
		FileResultType fileResult;
		boolean isStop = false;
		int lineNr = 0;
		String outputString = null;
		String referenceString = null;

		if( ( fileResult = openFile( true, false, false, true, Constants.FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, testFileNameString, null, Constants.FILE_DATA_REGRESSION_TEST_REFERENCE_DIRECTORY_NAME_STRING ) ).result == Constants.RESULT_OK )
			{
			if( fileResult.outputFile != null )
				{
				if( fileResult.referenceFile != null )
					{
					do	{
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
									if( outputString.equals( referenceString ) )
										lineNr++;
									else
										return adminItem_.startError( 1, moduleNameString_, "Line number " + lineNr + " is different:\n* test result:\t\t\"" + outputString + "\"\n* against reference:\t\"" + referenceString + "\"" );
									}
								else
									{
									isStop = true;

									if( outputString != null )
										return adminItem_.startError( 1, moduleNameString_, "The output file is longer than the reference file" );

									if( referenceString != null )
										return adminItem_.startError( 1, moduleNameString_, "The output file is shorter than the reference file" );
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
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The reference file is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The output file is undefined" );
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to open a test file" );

		return Constants.RESULT_OK;
		}

	protected byte readAndExecute()
		{
		boolean isLineExecuted;
		boolean isFirstLine = true;
		StringBuffer readStringBuffer = new StringBuffer();

		do	{
			isLineExecuted = false;
			readStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

			if( readLine( false, isFirstLine, false, false, false, false, ( CommonVariables.isDontIncrementCurrentSentenceNr ? CommonVariables.currentSentenceNr : CommonVariables.currentSentenceNr + 1 ), adminItem_.currentUserName(), readStringBuffer ) == Constants.RESULT_OK )
				{
				if( Presentation.hasReadLine() )
					{
					if( executeLine( readStringBuffer ) == Constants.RESULT_OK )
						{
						isFirstLine = false;
						isLineExecuted = true;
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to execute the read line" );
					}
				}
			else
				return adminItem_.addError( 1, moduleNameString_, "I failed to read a line" );
			}
		while( isLineExecuted &&
		!adminItem_.hasRequestedRestart() &&
		!Console.isTestingCanceled() &&

		// Ignore warnings during testing
		( adminItem_.isTesting() ||
		!CommonVariables.hasShownWarning ) );

		if( adminItem_.isSystemStartingUp() &&
		CommonVariables.hasShownWarning )
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
				Console.showProgressStatus( exampleFileNameString );

			if( ( fileResult = openFile( !adminItem_.isSystemStartingUp(), false, false, true, Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString, null, null ) ).result == Constants.RESULT_OK )
				{
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
				return adminItem_.addError( 1, moduleNameString_, "I failed to open an example file" );
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	protected byte readLine( boolean isClearInputField, boolean isFirstLine, boolean isGuideByGrammarPrompt, boolean isPassword, boolean isQuestion, boolean isText, int promptSentenceNr, String promptUserNameString, StringBuffer readStringBuffer )
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

		if( Presentation.readLine( isClearInputField, adminItem_.isDeveloperUser(), adminItem_.isExpertUser(), isFirstLine, isGuideByGrammarPrompt, isPassword, isQuestion, isText, isShowingLine(), promptStringBuffer.toString(), readStringBuffer, ( adminItem_.fileList == null ? null : adminItem_.fileList.currentReadFile() ) ) != Constants.RESULT_OK )
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
			if( ( adminItem_.fileList = new FileList( adminItem_ ) ) != null )
				adminItem_.adminListArray[Constants.ADMIN_FILE_LIST] = adminItem_.fileList;
			else
				return adminItem_.startError( 1, moduleNameString_, "I failed to create an admin file list" );
			}

		if( ( fileResult = openFile( true, false, false, true, Constants.FILE_DATA_STARTUP_DIRECTORY_NAME_STRING, Constants.FILE_STARTUP_NAME_STRING, null, null ) ).result == Constants.RESULT_OK )
			{
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
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to open an startup file" );

		return Constants.RESULT_OK;
		}

	protected byte readTestFile( String testFileNameString )
		{
		FileResultType fileResult;
		byte originalResult;
		boolean isFirstTestFile = ( CommonVariables.currentSentenceNr == adminItem_.firstSentenceNrOfCurrentUser() );
		FileItem testFileItem;

		// Check if user has already entered sentences
		if( !hasFoundAnyUserSpecification() )
			{
			if( isFirstTestFile )
				{
				testFileNr_ = 0;
				startTime_ = new Date().getTime();
				}
			else
				testFileNr_++;

			if( ( fileResult = openFile( true, false, true, true, Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, testFileNameString, Constants.FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, null ) ).result == Constants.RESULT_OK )
				{
				if( ( testFileItem = fileResult.createdFileItem ) != null )
					{
					if( Presentation.writeText( true, true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.NO_CENTER_WIDTH, "Testing: " + testFileNr_ + ", " + testFileItem.readFileNameString() + Constants.NEW_LINE_STRING ) == Constants.RESULT_OK )
						{
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
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to write a text" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The last created file item is undefined" );
				}
			else
				return adminItem_.addError( 1, moduleNameString_, "I failed to open a test file" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "Some knowledge is already entered. It may disturb the test results. Please, use restart button before testing to ensure no knowledge is present" );

		return Constants.RESULT_OK;
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedInfoFileItem;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;
		StringBuffer infoPathStringBuffer = new StringBuffer( Constants.FILE_DATA_INFO_DIRECTORY_NAME_STRING );

		if( infoFileNameString != null )
			{
			if( currentLanguageWordItem != null )
				infoPathStringBuffer.append( currentLanguageWordItem.anyWordTypeString() + Constants.SYMBOL_SLASH );

			if( ( fileResult = openFile( true, true, false, isReportingErrorIfFileDoesNotExist, infoPathStringBuffer.toString(), infoFileNameString, null, null ) ).result == Constants.RESULT_OK )
				{
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
				}
			else
				adminItem_.addError( 1, moduleNameString_, "I failed to open the info file" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given info file name string is undefined" );

		fileResult.result = CommonVariables.result;
		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
