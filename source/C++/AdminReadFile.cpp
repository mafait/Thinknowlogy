/*
 *	Class:			AdminReadFile
 *	Supports class:	AdminItem
 *	Purpose:		To read the lines from knowledge files
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

#include "AdminItem.h"
#include "FileList.cpp"
#include "Presentation.cpp"

class AdminReadFile
	{
	friend class AdminItem;

	// Private constructible variables

	unsigned int previousSentenceNr_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isGrammarChar( char grammarChar )
		{
		return ( grammarChar == QUERY_WORD_TYPE_CHAR ||
				grammarChar == QUERY_PARAMETER_CHAR ||
				grammarChar == GRAMMAR_WORD_DEFINITION_CHAR );
		}

	bool showLine()
		{
		if( !adminItem_->isSystemStartingUp() &&
		adminItem_->fileList != NULL )
			return adminItem_->fileList->showLine();

		return false;
		}

	FileResultType openFile( bool isAddingSubPath, bool isInfoFile, bool isReportingErrorIfFileDoesNotExist, const char *defaultSubpathString, const char *fileNameString )
		{
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "openFile";

		if( adminItem_->fileList != NULL )
			return adminItem_->fileList->openFile( isAddingSubPath, isInfoFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString );

		fileResult.result = myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The file list isn't created yet" );
		return fileResult;
		}

	ResultType readLanguageFile( bool isGrammarLanguage, char *languageNameString )
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedLanguageFileItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readLanguageFile";

		if( languageNameString != NULL )
			{
			commonVariables_->presentation->showStatus( languageNameString );

			if( ( fileResult = openFile( true, false, false, ( isGrammarLanguage ? FILE_GRAMMAR_DIRECTORY_NAME_STRING : FILE_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString ) ).result == RESULT_OK )
				{
				if( ( openedLanguageFileItem = fileResult.createdFileItem ) != NULL )
					{
					if( isGrammarLanguage )
						{
						if( adminItem_->createGrammarLanguage( languageNameString ) != RESULT_OK )
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create grammar language: \"", languageNameString, "\"" );
						}
					else
						{
						if( adminItem_->createInterfaceLanguage( languageNameString ) != RESULT_OK )
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create interface language: \"", languageNameString, "\"" );
						}

					if( commonVariables_->result == RESULT_OK )
						{
						if( readAndExecute() != RESULT_OK )
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read and execute the opened language file" );

						originalResult = commonVariables_->result;

						if( closeCurrentFileItem( openedLanguageFileItem ) != RESULT_OK )
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to close the language file item" );

						if( originalResult != RESULT_OK )
							commonVariables_->result = originalResult;
						}
//Java
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, ( isGrammarLanguage ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ), languageNameString, "\"" );
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, ( isGrammarLanguage ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ), languageNameString, "\"" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given language name is undefined" );

		return RESULT_OK;
		}

	ResultType checkCurrentSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCurrentSentenceNr";

		if( commonVariables_->currentSentenceNr < MAX_SENTENCE_NR )
			{
			if( adminItem_->isDontIncrementCurrentSentenceNr() )
				{
				adminItem_->clearDontIncrementCurrentSentenceNr();
				previousSentenceNr_--;	// No increment is done. So, the previous sentence number should be one less
				}
			else
				{
				if( commonVariables_->currentSentenceNr + 1 >= adminItem_->firstUserSentenceNr() )	// Is my sentence
					{
					commonVariables_->currentSentenceNr++;
					adminItem_->setCurrentItemNr();		// Necessary after increment of current sentence number
					}
				else
					return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Integrity violation! The next sentence is already used by another user" );
				}

			if( commonVariables_->currentSentenceNr > NO_SENTENCE_NR &&
			commonVariables_->currentSentenceNr <= adminItem_->highestSentenceNr() )
				{
				if( adminItem_->deleteSentences( true, commonVariables_->currentSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the current redo info" );
				}
			}
		else
			return myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Sentence number overflow! I can't except anymore input" );

		return RESULT_OK;
		}

	ResultType executeLine( char *readString )
		{
		ResultType originalResult;
		bool hasSwitchedLanguage = false;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeLine";

		previousSentenceNr_ = MAX_SENTENCE_NR;

		if( readString != NULL )
			{
			if( strlen( readString ) > 0 &&		// Skip empty line
			readString[0] != COMMENT_CHAR )		// and comment line
				{
				commonVariables_->hasShownMessage = false;
				commonVariables_->isAssignmentChanged = false;

				if( adminItem_->cleanupDeletedItems() == RESULT_OK )
					{
					previousSentenceNr_ = commonVariables_->currentSentenceNr;

					if( readString[0] == QUERY_CHAR )					// Guide-by-grammar, grammar/interface language or query
						{
						if( strlen( readString ) == 1 )					// Guide-by-grammar
							{
							if( checkCurrentSentenceNr() == RESULT_OK )
								{
								if( adminItem_->processReadSentence( NULL ) != RESULT_OK )
									myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to process a read sentence" );
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check to current sentence number" );
							}
						else
							{
							if( isalpha( readString[1] ) )				// Grammar/interface language
								{
								if( adminItem_->isSystemStartingUp() )	// Read grammar and/or interface language file
									{
									// First try to read the user-interface language file
									if( readLanguageFile( false, &readString[1] ) == RESULT_OK )
										{
										// Now, try to read the grammar language file
										if( readLanguageFile( true, &readString[1] ) != RESULT_OK )
											myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a grammar file" );
										}
									else
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read an interface file" );
									}
								else									// Change interface language
									{
									if( adminItem_->assignGrammarAndInterfaceLanguage( &readString[1] ) == RESULT_OK )
										hasSwitchedLanguage = true;
									else
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign the grammar and interface language" );
									}
								}
							else
								{
								if( readString[0] == QUERY_CHAR )		// Query
									{
									adminItem_->initializeQueryStringPosition();

									if( adminItem_->executeQuery( false, true, true, PRESENTATION_PROMPT_QUERY, readString ) != RESULT_OK )
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute query: \"", readString, "\"" );
									}
								}
							}
						}
					else												// Sentence or grammar definition
						{
						if( checkCurrentSentenceNr() == RESULT_OK )
							{
							if( isGrammarChar( readString[0] ) )		// Grammar definition
								{
								if( adminItem_->addGrammar( readString ) != RESULT_OK )
									myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add grammar: \"", readString, "\"" );
								}
							else										// Sentence
								{
								if( adminItem_->processReadSentence( readString ) != RESULT_OK )
									myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to process a read sentence" );
								}
							}
						else
							myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check to current sentence number" );
						}

					if( commonVariables_->result != RESULT_SYSTEM_ERROR &&
					!adminItem_->hasRequestedRestart() )
						{
						if( commonVariables_->result == RESULT_OK &&
						!hasSwitchedLanguage &&
						adminItem_->hasFoundAnyChangeDuringThisSentence() &&

						( adminItem_->wasUndoOrRedo() ||				// Execute selections after Undo or Redo
						!commonVariables_->hasShownMessage ) &&
						!commonVariables_->hasShownWarning )
							{
							if( adminItem_->executeSelections() != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}

						if( commonVariables_->result == RESULT_OK &&
						!hasSwitchedLanguage &&
						!adminItem_->hasFoundAnyChangeDuringThisSentence() &&
						!adminItem_->isSystemStartingUp() &&
						!commonVariables_->hasShownMessage &&
						!commonVariables_->hasShownWarning )
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
							}

						if( commonVariables_->result == RESULT_OK )
							{
							if( adminItem_->deleteAllTemporaryLists() != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete all temporary lists" );
							}

						if( ( commonVariables_->hasShownWarning ||
						commonVariables_->result != RESULT_OK ) &&

						commonVariables_->currentSentenceNr == previousSentenceNr_ + 1 )	// This sentence has items
							{
							originalResult = commonVariables_->result;		// Remember original result
							commonVariables_->result = RESULT_OK;			// Clear current result

							// Deleted current redo info. After this, 'Redo' isn't possible
							if( adminItem_->deleteSentences( false, commonVariables_->currentSentenceNr ) != RESULT_OK )
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the current redo information" );

							if( originalResult != RESULT_OK )
								commonVariables_->result = originalResult;	// Restore original result
							}
						}
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given read string is undefined" );

		return RESULT_OK;
		}

	ResultType closeCurrentFileItem( FileItem *closeFileItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "closeCurrentFileItem";
		if( adminItem_->fileList != NULL )
			{
			if( adminItem_->fileList->closeCurrentFile( closeFileItem ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to close a file" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The file list isn't created yet" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminReadFile( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		previousSentenceNr_ = MAX_SENTENCE_NR;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminReadFile" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType readStartupFile()
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedStartupFileItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readStartupFile";

		if( adminItem_->fileList == NULL )
			{
			// Create list
			if( ( adminItem_->fileList = new FileList( commonVariables_, myWordItem_ ) ) != NULL )
				adminItem_->adminListArray[ADMIN_FILE_LIST] = adminItem_->fileList;
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create an admin file list" );
			}

		if( ( fileResult = openFile( true, false, true, FILE_STARTUP_DIRECTORY_NAME_STRING, FILE_STARTUP_NAME_STRING ) ).result == RESULT_OK )
			{
			if( ( openedStartupFileItem = fileResult.createdFileItem ) != NULL )
				{
				if( readAndExecute() != RESULT_OK )
					myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read and execute the opened startup file" );

				originalResult = commonVariables_->result;

				if( closeCurrentFileItem( openedStartupFileItem ) == RESULT_OK )
					{
					if( commonVariables_->currentUserNr == NO_USER_NR )
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "No user is logged in" );
					}
				else
					myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to close the startup file item" );

				if( originalResult != RESULT_OK )
					commonVariables_->result = originalResult;
				}
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to open an startup file" );

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

			if( ( fileResult = openFile( !adminItem_->isSystemStartingUp(), false, true, FILE_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString ) ).result == RESULT_OK )
				{
				if( ( openedExampleFileItem = fileResult.createdFileItem ) != NULL )
					{
					if( readAndExecute() != RESULT_OK )
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read and execute the opened example file" );

					originalResult = commonVariables_->result;

					if( closeCurrentFileItem( openedExampleFileItem ) != RESULT_OK )
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to close the example file item" );

					if( originalResult != RESULT_OK )
						commonVariables_->result = originalResult;
					}
//Java
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to open an example file" );
			}
		else
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

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

			if( readLine( false, false, false, ( adminItem_->isDontIncrementCurrentSentenceNr() ? commonVariables_->currentSentenceNr : commonVariables_->currentSentenceNr + 1 ), adminItem_->currentUserName(), readString ) == RESULT_OK )
				{
				if( commonVariables_->presentation->hasReadLine() )
					{
					if( executeLine( readString ) == RESULT_OK )
						isLineExecuted = true;
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute the read line" );
					}
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a line" );
			}
		while( commonVariables_->result == RESULT_OK &&
		isLineExecuted &&
		!adminItem_->hasRequestedRestart() &&
		!commonVariables_->hasShownWarning );

		if( adminItem_->isSystemStartingUp() &&
		commonVariables_->hasShownWarning )
			commonVariables_->result = RESULT_SYSTEM_ERROR;

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

		if( commonVariables_->presentation->readLine( isPassword, isQuestion, isText, showLine(), promptString, readString, ( adminItem_->fileList == NULL ? NULL : adminItem_->fileList->currentReadFile() ) ) != RESULT_OK )
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a line from a file or from input" );

		return RESULT_OK;
		}

	FileResultType readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString )
		{
		FileResultType fileResult;
		ResultType originalResult;
		FileItem *openedInfoFileItem;
		WordItem *currentGrammarLanguageWordItem = commonVariables_->currentGrammarLanguageWordItem;
		char infoPathString[MAX_SENTENCE_STRING_LENGTH] = FILE_INFO_DIRECTORY_NAME_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readInfoFile";

		if( infoFileNameString != NULL )
			{
			if( currentGrammarLanguageWordItem != NULL )
				{
				strcat( infoPathString, currentGrammarLanguageWordItem->anyWordTypeString() );
				strcat( infoPathString, SLASH_STRING );
				}

			if( ( fileResult = openFile( true, true, isReportingErrorIfFileDoesNotExist, infoPathString, infoFileNameString ) ).result == RESULT_OK )
				{
				if( ( openedInfoFileItem = fileResult.createdFileItem ) != NULL )
					{
					if( readAndExecute() != RESULT_OK )
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read and execute the opened info file" );

					originalResult = commonVariables_->result;

					if( closeCurrentFileItem( openedInfoFileItem ) != RESULT_OK )
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to close the info file item" );

					if( originalResult != RESULT_OK )
						commonVariables_->result = originalResult;
					}
				}
			else
				myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to open the info file" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given info file name string is undefined" );

		fileResult.result = commonVariables_->result;
		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
