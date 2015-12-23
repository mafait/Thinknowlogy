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

#include <time.h>
#include "AdminItem.h"
#include "FileList.cpp"
#include "Presentation.cpp"

class AdminReadFile
	{
	friend class AdminItem;

	// Private constructible variables

	unsigned short testFileNr_;
	clock_t startTime_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool hasFoundAnyUserSpecification()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( currentWordItem->hasFoundAnyUserSpecification() &&
				!currentWordItem->isNeedingAuthorizationForChanges() )
					return true;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return false;
		}

	bool isGrammarChar( char grammarChar )
		{
		return ( grammarChar == QUERY_WORD_TYPE_CHAR ||
				grammarChar == QUERY_PARAMETER_CHAR ||
				grammarChar == GRAMMAR_WORD_DEFINITION_CHAR );
		}

	bool isShowingLine()
		{
		if( !adminItem_->isSystemStartingUp() &&
		adminItem_->fileList != NULL )
			return adminItem_->fileList->isShowingLine();

		return false;
		}

	FileResultType openFile( bool isAddingSubPath, bool isInfoFile, bool isTestFile, bool isReportingErrorIfFileDoesNotExist, const char *defaultSubpathString, const char *fileNameString, const char *writeSubpathString, const char *referenceSubpathString )
		{
		FileResultType fileResult;
		FileList *fileList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "openFile";

		if( ( fileList = adminItem_->fileList ) != NULL )
			return fileList->openFile( isAddingSubPath, isInfoFile, isTestFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString, writeSubpathString, referenceSubpathString );

		fileResult.result = adminItem_->startError( functionNameString, moduleNameString_, "The file list isn't created yet" );
		return fileResult;
		}

	ResultType readLanguageFile( bool isGrammarFile, char *languageNameString )
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedLanguageFileItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readLanguageFile";

		if( languageNameString != NULL )
			{
			commonVariables_->presentation->showStatus( languageNameString );

			if( ( fileResult = openFile( true, false, false, false, ( isGrammarFile ? FILE_DATA_GRAMMAR_DIRECTORY_NAME_STRING : FILE_DATA_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString, NULL, NULL ) ).result == RESULT_OK )
				{
				if( ( openedLanguageFileItem = fileResult.createdFileItem ) != NULL )
					{
					if( adminItem_->createLanguage( languageNameString ) == RESULT_OK )
						{
						if( readAndExecute() != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened language file" );

						originalResult = commonVariables_->result;

						if( closeCurrentFileItem( openedLanguageFileItem ) != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the language file item" );

						if( originalResult != RESULT_OK )
							commonVariables_->result = originalResult;
						}
					else
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to create language: \"", languageNameString, "\"" );
					}
				else
					{
					// When reading special characters goes wrong, the wrong Zip file may be downloaded
					// The Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and further
					if( strcmp( languageNameString, FILE_WINDOWS_VERSION_STRING ) == 0 )
						return adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, "You have probably downloaded the Zip file for the wrong Windows version.\nPlease check if the downloaded file name matches your Windows version.\nThe Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and further" );

					return adminItem_->startError( functionNameString, moduleNameString_, ( isGrammarFile ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ), languageNameString, "\"" );
					}
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, ( isGrammarFile ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ), languageNameString, "\"" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given language name is undefined" );

		return RESULT_OK;
		}

	ResultType checkCurrentSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCurrentSentenceNr";

		if( commonVariables_->isDontIncrementCurrentSentenceNr )
			{
			commonVariables_->isDontIncrementCurrentSentenceNr = false;

			if( adminItem_->deleteSentences( true, commonVariables_->currentSentenceNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the current redo info" );
			}
		else
			{
			if( commonVariables_->currentSentenceNr < MAX_SENTENCE_NR )
				{
				commonVariables_->currentSentenceNr++;
				// Necessary after changing current sentence number
				adminItem_->setCurrentItemNr();
				}
			else
				return adminItem_->startSystemError( functionNameString, moduleNameString_, "Sentence number overflow! I can't except anymore input" );
			}

		return RESULT_OK;
		}

	ResultType executeLine( char *readString )
		{
		bool hasSwitchedLanguage = false;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeLine";

		if( readString != NULL )
			{
			// Skip empty line
			if( strlen( readString ) > 0 &&
			// and comment line
			readString[0] != COMMENT_CHAR )
				{
				commonVariables_->hasShownMessage = false;
				commonVariables_->isAssignmentChanged = false;

				if( adminItem_->cleanupDeletedItems() == RESULT_OK )
					{
					// Guide-by-grammar, grammar/language or query
					if( readString[0] == QUERY_CHAR )
						{
						// Guide-by-grammar
						if( strlen( readString ) == 1 )
							{
							if( checkCurrentSentenceNr() == RESULT_OK )
								{
								if( adminItem_->processReadSentence( NULL ) != RESULT_OK )
									adminItem_->addError( functionNameString, moduleNameString_, "I failed to process a read sentence" );
								}
							else
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to check to current sentence number" );
							}
						else
							{
							// Grammar/language
							if( isalpha( readString[1] ) )
								{
								if( adminItem_->isSystemStartingUp() )
									{
									if( readGrammarFileAndUserInterfaceFile( &readString[1] ) != RESULT_OK )
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to read the language" );
									}
								else
									{
									// Change language
									if( adminItem_->assignLanguage( &readString[1] ) == RESULT_OK )
										hasSwitchedLanguage = true;
									else
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language" );
									}
								}
							else
								{
								// Query
								if( readString[0] == QUERY_CHAR )
									{
									adminItem_->initializeQueryStringPosition();

									if( adminItem_->executeQuery( false, true, true, PRESENTATION_PROMPT_QUERY, readString ) != RESULT_OK )
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute query: \"", readString, "\"" );
									}
								}
							}
						}
					else
						{
						// Sentence or grammar definition
						if( checkCurrentSentenceNr() == RESULT_OK )
							{
							// Grammar definition
							if( isGrammarChar( readString[0] ) )
								{
								if( adminItem_->addGrammar( readString ) != RESULT_OK )
									adminItem_->addError( functionNameString, moduleNameString_, "I failed to add grammar: \"", readString, "\"" );
								}
							else
								{
								// Sentence
								if( adminItem_->processReadSentence( readString ) != RESULT_OK )
									adminItem_->addError( functionNameString, moduleNameString_, "I failed to process a read sentence" );
								}
							}
						else
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to check to current sentence number" );
						}

					if( commonVariables_->result != RESULT_SYSTEM_ERROR &&
					!adminItem_->hasRequestedRestart() )
						{
						if( commonVariables_->result == RESULT_OK &&
						!hasSwitchedLanguage &&
						!commonVariables_->hasShownWarning &&
						adminItem_->hasFoundAnyChangeMadeByThisSentence() &&

						// Execute selections after Undo or Redo
						( adminItem_->wasUndoOrRedo() ||
						!commonVariables_->hasShownMessage ) )
							{
							if( adminItem_->executeSelections() != RESULT_OK )
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}

						if( commonVariables_->result == RESULT_OK &&
						!hasSwitchedLanguage &&
						!adminItem_->hasFoundAnyChangeMadeByThisSentence() &&
						!adminItem_->isSystemStartingUp() &&
						!commonVariables_->hasShownMessage &&
						!commonVariables_->hasShownWarning )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != RESULT_OK )
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I know' interface notification" );
							}

						if( commonVariables_->result == RESULT_OK )
							{
							if( adminItem_->deleteAllTemporaryLists() != RESULT_OK )
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all temporary lists" );
							}
						}
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read string is undefined" );

		return RESULT_OK;
		}

	ResultType closeCurrentFileItem( FileItem *closeFileItem )
		{
		bool isTestFile;
		unsigned int testFileSentenceNr;
		FileList *fileList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "closeCurrentFileItem";

		if( ( fileList = adminItem_->fileList ) != NULL )
			{
			// Get info before closing the file
			isTestFile = fileList->isCurrentFileTestFile();
			testFileSentenceNr = fileList->currentFileSentenceNr();

			if( fileList->closeCurrentFile( closeFileItem ) == RESULT_OK )
				{
				if( isTestFile &&
				adminItem_->deleteSentences( false, testFileSentenceNr ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the previous test sentences" );
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to close a file" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The file list isn't created yet" );

		return RESULT_OK;
		}

	ResultType readGrammarFileAndUserInterfaceFile( char *readLanguageNameString )
		{
		char *startupLanguageNameString = adminItem_->startupLanguageNameString;
		WordItem *currentLanguageWordItem;
		WordItem *predefinedNounStartupLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readGrammarFileAndUserInterfaceFile";

		if( readLanguageNameString != NULL )
			{
			// Read the user-interface file
			if( readLanguageFile( false, readLanguageNameString ) == RESULT_OK )
				{
				// Read the grammar file
				if( readLanguageFile( true, readLanguageNameString ) == RESULT_OK )
					{
					if( startupLanguageNameString != NULL &&
					( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) != NULL &&
					( predefinedNounStartupLanguageWordItem = adminItem_->predefinedNounStartupLanguageWordItem() ) != NULL )
						{
						if( predefinedNounStartupLanguageWordItem->addSpecification( false, false, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NAME, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, currentLanguageWordItem, NULL, NULL, NULL ).result == RESULT_OK )
							{
							if( strcmp( startupLanguageNameString, currentLanguageWordItem->anyWordTypeString() ) == 0 )
								{
								if( predefinedNounStartupLanguageWordItem->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, currentLanguageWordItem, NULL, NULL ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the predefined noun startup language word" );
								}
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a predefined noun startup language specification" );
						}
					}
				else
					adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a grammar file" );
				}
			else
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to read an interface file" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read language name string is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminReadFile( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		testFileNr_ = 0;
		startTime_ = 0;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminReadFile" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType compareOutputFileAgainstReferenceFile( char *testFileNameString )
		{
		FileResultType fileResult;
		bool hasReadOutput;
		bool hasReadReference;
		bool isStop = false;
		unsigned int lineNr = 0;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char outputString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char referenceString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareOutputFileAgainstReferenceFile";

		if( ( fileResult = openFile( true, false, false, true, FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, testFileNameString, NULL, FILE_DATA_REGRESSION_TEST_REFERENCE_DIRECTORY_NAME_STRING ) ).result == RESULT_OK )
			{
			if( fileResult.outputFile != NULL )
				{
				if( fileResult.referenceFile != NULL )
					{
					do	{
						if( commonVariables_->presentation->readLine( false, false, false, false, NULL, outputString, fileResult.outputFile ) == RESULT_OK )
							{
							hasReadOutput = commonVariables_->presentation->hasReadLine();

							if( commonVariables_->presentation->readLine( false, false, false, false, NULL, referenceString, fileResult.referenceFile ) == RESULT_OK )
								{
								hasReadReference = commonVariables_->presentation->hasReadLine();

								if( hasReadOutput &&
								hasReadReference )
									{
									if( strcmp( outputString, referenceString ) == 0 )
										lineNr++;
									else
										{
										sprintf( errorString, "Line number %u is different:\n* test result:\t\t\"%s\"\n* against reference:\t\"%s\"", lineNr, outputString, referenceString );
										return adminItem_->startError( functionNameString, moduleNameString_, errorString );
										}
									}
								else
									{
									isStop = true;

									if( hasReadOutput )
										return adminItem_->startError( functionNameString, moduleNameString_, "The output file is longer than the reference file" );

									if( hasReadReference )
										return adminItem_->startError( functionNameString, moduleNameString_, "The output file is shorter than the reference file" );
									}
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a line from the reference file" );
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a line from the output file" );
						}
					while( !isStop );

						if( fileResult.outputFile != NULL )
							fclose( fileResult.outputFile );

						if( fileResult.referenceFile != NULL )
							fclose( fileResult.referenceFile );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The reference file is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The output file is undefined" );
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open a test file" );

		return RESULT_OK;
		}

	ResultType readAndExecute()
		{
		bool isLineExecuted;
		char readString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readAndExecute";

		do	{
			isLineExecuted = false;
			strcpy( readString, EMPTY_STRING );

			if( readLine( false, false, false, ( commonVariables_->isDontIncrementCurrentSentenceNr ? commonVariables_->currentSentenceNr : commonVariables_->currentSentenceNr + 1 ), adminItem_->currentUserName(), readString ) == RESULT_OK )
				{
				if( commonVariables_->presentation->hasReadLine() )
					{
					if( executeLine( readString ) == RESULT_OK )
						isLineExecuted = true;
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute the read line" );
					}
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a line" );
			}
		while( isLineExecuted &&
		!adminItem_->hasRequestedRestart() &&

		// Ignore warnings during testing
		( adminItem_->isTesting() ||
		!commonVariables_->hasShownWarning ) );

		if( adminItem_->isSystemStartingUp() &&
		commonVariables_->hasShownWarning )
			commonVariables_->result = RESULT_SYSTEM_ERROR;

		return RESULT_OK;
		}

	ResultType readExampleFile( char *exampleFileNameString )
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedExampleFileItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readExampleFile";

		if( exampleFileNameString != NULL )
			{
			if( adminItem_->isSystemStartingUp() )
				commonVariables_->presentation->showStatus( exampleFileNameString );

			if( ( fileResult = openFile( !adminItem_->isSystemStartingUp(), false, false, true, FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString, NULL, NULL ) ).result == RESULT_OK )
				{
				if( ( openedExampleFileItem = fileResult.createdFileItem ) != NULL )
					{
					if( readAndExecute() != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened example file" );

					originalResult = commonVariables_->result;

					if( closeCurrentFileItem( openedExampleFileItem ) != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the example file item" );

					if( originalResult != RESULT_OK )
						commonVariables_->result = originalResult;
					}
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open an example file" );
			}
		else
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	ResultType readLine( bool isPassword, bool isQuestion, bool isText, unsigned int promptSentenceNr, char *promptUserNameString, char *readString )
		{
		char promptString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readLine";

		if( promptSentenceNr > NO_SENTENCE_NR )
			sprintf( promptString, "%u", promptSentenceNr );

		if( promptUserNameString != NULL )
			{
			if( strlen( promptString ) > 0 )
				strcat( promptString, QUERY_SEPARATOR_STRING );

			strcat( promptString, promptUserNameString );
			}

		if( commonVariables_->presentation->readLine( isPassword, isQuestion, isText, isShowingLine(), promptString, readString, ( adminItem_->fileList == NULL ? NULL : adminItem_->fileList->currentReadFile() ) ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a line from a file or from input" );

		return RESULT_OK;
		}

	ResultType readStartupFile()
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedStartupFileItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readStartupFile";

		if( adminItem_->fileList == NULL )
			{
			// Create list
			if( ( adminItem_->fileList = new FileList( commonVariables_, adminItem_ ) ) != NULL )
				adminItem_->adminListArray[ADMIN_FILE_LIST] = adminItem_->fileList;
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create an admin file list" );
			}

		if( ( fileResult = openFile( true, false, false, true, FILE_DATA_STARTUP_DIRECTORY_NAME_STRING, FILE_STARTUP_NAME_STRING, NULL, NULL ) ).result == RESULT_OK )
			{
			if( ( openedStartupFileItem = fileResult.createdFileItem ) != NULL )
				{
				if( readAndExecute() != RESULT_OK )
					adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened startup file" );

				originalResult = commonVariables_->result;

				if( closeCurrentFileItem( openedStartupFileItem ) != RESULT_OK )
					adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the startup file item" );

				if( originalResult != RESULT_OK )
					commonVariables_->result = originalResult;
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open an startup file" );

		return RESULT_OK;
		}

	ResultType readTestFile( char *testFileNameString )
		{
		FileResultType fileResult;
		ResultType originalResult;
		bool isFirstTestFile = ( commonVariables_->currentSentenceNr == adminItem_->firstSentenceNrOfCurrentUser() );
		FileItem *testFileItem;
		char testString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "readTestFile";

		// Check if user has already entered sentences
		if( !hasFoundAnyUserSpecification() )
			{
			if( isFirstTestFile )
				{
				testFileNr_ = 0;
				startTime_ = clock();
				}
			else
				testFileNr_++;

			if( ( fileResult = openFile( true, false, true, true, FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, testFileNameString, FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, NULL ) ).result == RESULT_OK )
				{
				if( ( testFileItem = fileResult.createdFileItem ) != NULL )
					{
					sprintf( testString, "Testing: %u, %s.\n", testFileNr_, testFileItem->readFileNameString() );

					if( commonVariables_->presentation->writeText( true, true, PRESENTATION_PROMPT_NOTIFICATION, NO_CENTER_WIDTH, testString ) == RESULT_OK )
						{
						commonVariables_->presentation->redirectOutputToTestFile( testFileItem->writeFile() );

						if( readAndExecute() != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened test file" );

						if( ( originalResult = commonVariables_->result ) != RESULT_OK )
							commonVariables_->result = RESULT_OK;

						if( closeCurrentFileItem( testFileItem ) != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the test file item" );

						if( originalResult != RESULT_OK )
							commonVariables_->result = originalResult;
						else
							{
							if( isFirstTestFile )
								{
								sprintf( testString, "Done in: %.3f sec.\n", ( ( clock() - startTime_ ) / (double)CLOCKS_PER_SEC ) );

								if( commonVariables_->presentation->writeText( true, true, PRESENTATION_PROMPT_NOTIFICATION, NO_CENTER_WIDTH, testString ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a text" );
								}
							}
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a text" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The last created file item is undefined" );
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open a test file" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "Some knowledge is already entered. It may disturb the test results. Please, use restart button before testing to ensure no knowledge is present" );

		return RESULT_OK;
		}

	FileResultType readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString )
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedInfoFileItem;
		WordItem *currentLanguageWordItem = commonVariables_->currentLanguageWordItem;
		char infoPathString[MAX_SENTENCE_STRING_LENGTH] = FILE_DATA_INFO_DIRECTORY_NAME_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readInfoFile";

		if( infoFileNameString != NULL )
			{
			if( currentLanguageWordItem != NULL )
				{
				strcat( infoPathString, currentLanguageWordItem->anyWordTypeString() );
				strcat( infoPathString, SLASH_STRING );
				}

			if( ( fileResult = openFile( true, true, false, isReportingErrorIfFileDoesNotExist, infoPathString, infoFileNameString, NULL, NULL ) ).result == RESULT_OK )
				{
				if( ( openedInfoFileItem = fileResult.createdFileItem ) != NULL )
					{
					if( readAndExecute() != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened info file" );

					originalResult = commonVariables_->result;

					if( closeCurrentFileItem( openedInfoFileItem ) != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the info file item" );

					if( originalResult != RESULT_OK )
						commonVariables_->result = originalResult;
					}
				}
			else
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to open the info file" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given info file name string is undefined" );

		fileResult.result = commonVariables_->result;
		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
