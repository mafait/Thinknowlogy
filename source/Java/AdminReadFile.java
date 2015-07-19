/*
 *	Class:			AdminReadFile
 *	Supports class:	AdminItem
 *	Purpose:		To read the lines from knowledge files
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
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


class AdminReadFile
	{
	// Private constructible variables

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

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

	private FileResultType openFile( boolean isAddingSubPath, boolean isInfoFile, boolean isReportingErrorIfFileDoesNotExist, String defaultSubpathString, String fileNameString )
		{
		FileResultType fileResult = new FileResultType();
		FileList fileList;

		if( ( fileList = adminItem_.fileList ) != null )
			return fileList.openFile( isAddingSubPath, isInfoFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString );

		fileResult.result = adminItem_.startErrorInItem( 1, moduleNameString_, "The file list isn't created yet" );
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

			if( ( fileResult = openFile( true, false, false, ( isGrammarFile ? Constants.FILE_GRAMMAR_DIRECTORY_NAME_STRING : Constants.FILE_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString ) ).result == Constants.RESULT_OK )
				{
				if( ( openedLanguageFileItem = fileResult.createdFileItem ) != null )
					{
					if( adminItem_.createLanguage( languageNameString ) == Constants.RESULT_OK )
						{
						if( readAndExecute() != Constants.RESULT_OK )
							adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read and execute the opened language file" );

						originalResult = CommonVariables.result;

						if( closeCurrentFileItem( openedLanguageFileItem ) != Constants.RESULT_OK )
							adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to close the language file item" );

						if( originalResult != Constants.RESULT_OK )
							CommonVariables.result = originalResult;
						}
					else
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create language: \"" + languageNameString + "\"" );
					Console.clearProgress();
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, ( isGrammarFile ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ) + languageNameString + "\"" );
				}
			else
				return adminItem_.addErrorInItem( 1, moduleNameString_, ( isGrammarFile ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ) + languageNameString + "\"" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given language name is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkCurrentSentenceNr()
		{
		if( CommonVariables.isDontIncrementCurrentSentenceNr )
			{
			CommonVariables.isDontIncrementCurrentSentenceNr = false;

			if( adminItem_.deleteSentences( true, CommonVariables.currentSentenceNr ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the current redo info" );
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
				return adminItem_.startSystemErrorInItem( 1, moduleNameString_, "Sentence number overflow! I can't except anymore input" );
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
									adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to process a read sentence" );
								}
							else
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to check to current sentence number" );
							}
						else
							{
							// Grammar/language
							if( Character.isLetter( readStringBuffer.charAt( 1 ) ) )
								{
								if( adminItem_.isSystemStartingUp() )
									{
									if( readGrammarFileAndUserInterfaceFile( readStringBuffer.substring( 1 ) ) != Constants.RESULT_OK )
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read the language" );
									}
								else
									{
									// Change language
									if( adminItem_.assignLanguage( readStringBuffer.substring( 1 ) ) == Constants.RESULT_OK )
										hasSwitchedLanguage = true;
									else
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the language" );
									}
								}
							else
								{
								// Query
								if( readStringBuffer.charAt( 0 ) == Constants.QUERY_CHAR )
									{
									adminItem_.initializeQueryStringPosition();

									if( adminItem_.executeQuery( false, true, true, Constants.PRESENTATION_PROMPT_QUERY, readStringBuffer.toString() ) != Constants.RESULT_OK )
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute query: \"" + readStringBuffer + "\"" );
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
									adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add grammar: \"" + readStringBuffer + "\"" );
								}
							else
								{
								// Sentence
								if( adminItem_.processReadSentence( readStringBuffer.toString() ) != Constants.RESULT_OK )
									adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to process a read sentence" );
								}
							}
						else
							adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to check to current sentence number" );
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
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!hasSwitchedLanguage &&
						!adminItem_.hasFoundAnyChangeMadeByThisSentence() &&
						!adminItem_.isSystemStartingUp() &&
						!CommonVariables.hasShownMessage &&
						!CommonVariables.hasShownWarning )
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
							}

						if( CommonVariables.result == Constants.RESULT_OK )
							{
							if( adminItem_.deleteAllTemporaryLists() != Constants.RESULT_OK )
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete all temporary lists" );
							}
						}
					}
				else
					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to cleanup the deleted items" );
				}
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given read string buffer is undefined" );

		return Constants.RESULT_OK;
		}

	private byte closeCurrentFileItem( FileItem closeFileItem )
		{
		FileList fileList;
		if( ( fileList = adminItem_.fileList ) != null )
			{
			if( fileList.closeCurrentFile( closeFileItem ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to close a file" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The file list isn't created yet" );

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
						if( predefinedNounStartupLanguageWordItem.addSpecification( false, false, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, currentLanguageWordItem, null, null, null ).result == Constants.RESULT_OK )
							{
							if( startupLanguageNameString.equals( currentLanguageWordItem.anyWordTypeString() ) )
								{
								if( predefinedNounStartupLanguageWordItem.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, currentLanguageWordItem, null, null ).result != Constants.RESULT_OK )
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the predefined noun startup language word" );
								}
							}
						else
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a predefined noun startup language specification" );
						}
					}
				else
					adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a grammar file" );
				}
			else
				adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read an interface file" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given read language name string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminReadFile( AdminItem adminItem )
		{
		String errorString = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

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
				return adminItem_.startErrorInItem( 1, moduleNameString_, "I failed to create an admin file list" );
			}

		if( ( fileResult = openFile( true, false, true, Constants.FILE_STARTUP_DIRECTORY_NAME_STRING, Constants.FILE_STARTUP_NAME_STRING ) ).result == Constants.RESULT_OK )
			{
			if( ( openedStartupFileItem = fileResult.createdFileItem ) != null )
				{
				if( readAndExecute() != Constants.RESULT_OK )
					adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read and execute the opened startup file" );

				originalResult = CommonVariables.result;

				if( closeCurrentFileItem( openedStartupFileItem ) != Constants.RESULT_OK )
					adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to close the startup file item" );

				if( originalResult != Constants.RESULT_OK )
					CommonVariables.result = originalResult;
				}
			}
		else
			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to open an startup file" );

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

			if( ( fileResult = openFile( !adminItem_.isSystemStartingUp(), false, true, Constants.FILE_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString ) ).result == Constants.RESULT_OK )
				{
				if( ( openedExampleFileItem = fileResult.createdFileItem ) != null )
					{
					if( readAndExecute() != Constants.RESULT_OK )
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read and execute the opened example file" );

					originalResult = CommonVariables.result;

					if( closeCurrentFileItem( openedExampleFileItem ) != Constants.RESULT_OK )
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to close the example file item" );

					if( originalResult != Constants.RESULT_OK )
						CommonVariables.result = originalResult;
					}
				Console.clearProgress();
				}
			else
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to open an example file" );
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != Constants.RESULT_OK )
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
			}

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
						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to execute the read line" );
					}
				}
			else
				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a line" );
			}
		while( CommonVariables.result == Constants.RESULT_OK &&
		isLineExecuted &&
		!adminItem_.hasRequestedRestart() &&
		!CommonVariables.hasShownWarning );

		if( adminItem_.isSystemStartingUp() &&
		CommonVariables.hasShownWarning )
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;

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

		if( Presentation.readLine( isClearInputField, adminItem_.isExpertUser(), isFirstLine, isGuideByGrammarPrompt, isPassword, isQuestion, isText, isShowingLine(), promptStringBuffer.toString(), readStringBuffer, ( adminItem_.fileList == null ? null : adminItem_.fileList.currentReadFile() ) ) != Constants.RESULT_OK )
			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a line from a file or from input" );

		return Constants.RESULT_OK;
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedInfoFileItem;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;
		StringBuffer infoPathStringBuffer = new StringBuffer( Constants.FILE_INFO_DIRECTORY_NAME_STRING );

		if( infoFileNameString != null )
			{
			if( currentLanguageWordItem != null )
				infoPathStringBuffer.append( currentLanguageWordItem.anyWordTypeString() + Constants.SYMBOL_SLASH );

			if( ( fileResult = openFile( true, true, isReportingErrorIfFileDoesNotExist, infoPathStringBuffer.toString(), infoFileNameString ) ).result == Constants.RESULT_OK )
				{
				if( ( openedInfoFileItem = fileResult.createdFileItem ) != null )
					{
					if( readAndExecute() != Constants.RESULT_OK )
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read and execute the opened info file" );

					originalResult = CommonVariables.result;

					if( closeCurrentFileItem( openedInfoFileItem ) != Constants.RESULT_OK )
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to close the info file item" );

					if( originalResult != Constants.RESULT_OK )
						CommonVariables.result = originalResult;
					}
				}
			else
				adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to open the info file" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given info file name string is undefined" );

		fileResult.result = CommonVariables.result;
		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
