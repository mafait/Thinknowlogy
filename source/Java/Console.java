/*	Class:		Console
 *	Purpose:	To create the GUI and to process the events
 *	Version:	Thinknowlogy 2016r2 (Restyle)
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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.io.File;

import javax.swing.border.Border;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ScrollPaneConstants;
import javax.swing.text.Document;

class Console extends JPanel implements ActionListener, ComponentListener
	{
	// Private constants
	private static final long serialVersionUID = -2264899695119545018L;

	// Private variables
	private static boolean hasSelectedAmbiguityBoston_;
	private static boolean hasSelectedAmbiguityPresidents_;
	private static boolean hasSelectedProgrammingConnect4_;
	private static boolean hasSelectedProgrammingTowerOfHanoi_;
	private static boolean hasSelectedReasoningScientificChallenge_;
	private static boolean hasSelectedReasoningFamilyDefinition_;
	private static boolean hasSelectedMoreExamples_;

	private static boolean isActionPerformed_;
	private static boolean isGUIavailable_;
	private static boolean isStopResizing_;
	private static boolean isTesting_;
	private static boolean isTestingCanceled_;

	private static short currentFontSize_;
	private static short currentSubMenu_;
	private static short nSubMenuButtons_;

	private static int currentFrameHeight_;
	private static int titleBarHeight_;
	private static int wantedPreferredOutputScrollPaneSize_;
	private static int windowBottomHeight_;

	private static String currentFontString_;
	private static String inputString_;
	private static String errorHeaderString_;
	private static StringBuffer errorStringBuffer_;

	private static AdminItem adminItem_;
	private static WordItem currentLanguageWordItem_;

	// Private components - Output area
	private static JScrollPane outputScrollPane_;
	private static JTextArea outputArea_;

	// Private components - Panels
	private static JPanel upperMenuPanel_;
	private static JPanel mainMenuPanel_;
	private static JPanel subMenuPanel_;
	private static JPanel inputPanel_;

	// Private components - Upper menu
	private static JButton upperMenuClearYourMindButton_;
	private static JButton upperMenuRestartButton_;
	private static JButton upperMenuUndoButton_;
	private static JButton upperMenuRedoButton_;
	private static JButton upperMenuLoginAsExpertButton_;
	private static JButton upperMenuLoginAsDeveloperButton_;
	private static JButton upperMenuReadTheTestFileRegressionTestUserButton_;
	private static JButton upperMenuReadTheTestFileRegressionTestDeepButton_;
	private static JButton upperMenuStopTestingButton_;
	private static JButton upperMenuMoreExamplesButton_;

	private static JLabel upperMenuProgressLabel_;
	private static JProgressBar upperMenuProgressBar_;

	private static JFileChooser fileChooser_;

	// Private components - Main menu
	private static JButton mainMenuAmbiguitySubMenuButton_;
	private static JButton mainMenuReadTheFileAmbiguityBostonButton_;
	private static JButton mainMenuReadTheFileAmbiguityPresidentsButton_;

	private static JButton mainMenuProgrammingSubMenuButton_;
	private static JButton mainMenuReadTheFileProgrammingConnect4Button_;
	private static JButton mainMenuReadTheFileProgrammingTowerOfHanoiButton_;

	private static JButton mainMenuReasoningSubMenuButton_;
	private static JButton mainMenuReasoningFamilyDefinitionsSubMenuButton_;
	private static JButton mainMenuReasoningFamilyConflictsSubMenuButton_;
	private static JButton mainMenuReasoningFamilyJustificationSubMenuButton_;
	private static JButton mainMenuReasoningFamilyQuestionsSubMenuButton_;
	private static JButton mainMenuReasoningFamilyDisplayInformationSubMenuButton_;

	private static JButton mainMenuReasoningReadTheFileScientificChallengeButton_;

	private static JButton mainMenuReasoningReadTheFileFamilyDefinitionButton_;

	private static JButton mainMenuBackButton_;
	private static JButton mainMenuChangeLanguageButton_;
	private static JButton mainMenuChangeFontButton_;
	private static JButton mainMenuHelpButton_;

	// Private components - sub-menus
	private static JLabel initialLabelDemoButtons_;
	private static JLabel initialLabelEnterSentence_;

	private static JButton[] subMenuButtonArray_ = new JButton[Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS];

	// Private components - input area
	private static JTextField inputField_;
	private static JPasswordField passwordField_;


	// Private methods

	private static void addSubMenuButton( short interfaceParameter )
		{
		String interfaceString;

		if( nSubMenuButtons_ >= 0 &&
		nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS &&
		currentLanguageWordItem_ != null &&
		( interfaceString = currentLanguageWordItem_.interfaceString( interfaceParameter ) ) != null )
			{
			if( !interfaceString.equals( Constants.INTERFACE_STRING_NOT_AVAILABLE ) )
				{
				subMenuButtonArray_[nSubMenuButtons_].setEnabled( true );
				subMenuButtonArray_[nSubMenuButtons_++].setText( interfaceString );
				}
			}
		}

	private static void addSubMenuFontButton( String fontString )
		{
		if( fontString != null &&
		nSubMenuButtons_ >= 0 &&
		nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS )
			{
			subMenuButtonArray_[nSubMenuButtons_].setEnabled( !fontString.equals( currentFontString_ ) );
			subMenuButtonArray_[nSubMenuButtons_++].setText( fontString );
			}
		}

	private static void changeFont( String actionCommandString )
		{
		boolean hasFoundAction = false;
		String interfaceString;

		if( actionCommandString != null )
			{
			if( actionCommandString.equals( Constants.CONSOLE_FONT_ARIEL ) ||
			actionCommandString.equals( Constants.CONSOLE_FONT_COURIER ) ||
			actionCommandString.equals( Constants.CONSOLE_FONT_COURIER_NEW ) ||
			actionCommandString.equals( Constants.CONSOLE_FONT_MICROSOFT_SANS_SERIF ) ||
			actionCommandString.equals( Constants.CONSOLE_FONT_MONOSPACED ) ||
			actionCommandString.equals( Constants.CONSOLE_FONT_TIMES_NEW_ROMAN ) ||
			actionCommandString.equals( Constants.CONSOLE_FONT_VERDANA ) )
				{
				hasFoundAction = true;
				currentFontString_ = actionCommandString;
				}
			else
				{
				if( currentLanguageWordItem_ != null )
					{
					interfaceString = currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_FONT_SIZE_DECREASE );
	
					if( interfaceString != null &&
					interfaceString.equals( actionCommandString ) )
						{
						hasFoundAction = true;
						currentFontSize_--;
						}
					else
						{
						interfaceString = currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_FONT_SIZE_INCREASE );
						
						if( interfaceString != null &&
						interfaceString.equals( actionCommandString ) )
							{
							hasFoundAction = true;
							currentFontSize_++;
							}
						}
					}
				}
			}

		if( hasFoundAction )
			{
			inputString_ = null;
			outputArea_.setFont( new Font( currentFontString_, Font.PLAIN, currentFontSize_ ) );
			enableMenus( true, true );
			setSubMenuButtonTexts();
			goToEndOfOutputDocument( true );
			}
		}

	private static void enableMenus( boolean isEnablingNormalButtons, boolean isEnablingSubMenuButtons )
		{
		boolean hasSelectedMainButton = ( hasSelectedProgrammingConnect4_ ||
										hasSelectedProgrammingTowerOfHanoi_ ||
										hasSelectedReasoningScientificChallenge_ ||
										hasSelectedReasoningFamilyDefinition_ ||
										hasSelectedMoreExamples_ );
		boolean isChangeFontMenu = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_CHANGE_FONT );
		boolean isChangeLanguageMenu = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE );
		boolean isCurrentUserExpert = ( adminItem_ != null &&
										adminItem_.isCurrentUserExpert() );
		boolean isCurrentUserDeveloper = ( adminItem_ != null &&
											adminItem_.isCurrentUserDeveloper() );
		boolean isHelpMenu = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_HELP );
		boolean isInitMenu = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_INIT );
		boolean isInvisible = ( isChangeFontMenu ||
								isChangeLanguageMenu ||
								isHelpMenu ||
								isInitMenu );

		// Upper menu
		upperMenuClearYourMindButton_.setEnabled( isEnablingNormalButtons );
		upperMenuRestartButton_.setEnabled( isEnablingNormalButtons );
		upperMenuUndoButton_.setEnabled( isEnablingNormalButtons );
		upperMenuRedoButton_.setEnabled( isEnablingNormalButtons );

		upperMenuLoginAsExpertButton_.setEnabled( isEnablingNormalButtons && !isCurrentUserExpert );
		upperMenuLoginAsDeveloperButton_.setEnabled( isEnablingNormalButtons && isCurrentUserExpert );
		upperMenuReadTheTestFileRegressionTestUserButton_.setEnabled( isEnablingNormalButtons && isCurrentUserDeveloper );
		upperMenuReadTheTestFileRegressionTestDeepButton_.setEnabled( isEnablingNormalButtons && isCurrentUserDeveloper );

		upperMenuMoreExamplesButton_.setEnabled( !hasSelectedMainButton && isEnablingNormalButtons );

		// Main menu
		mainMenuAmbiguitySubMenuButton_.setEnabled( isEnablingNormalButtons );
		setButtonText( isInvisible, Constants.INTERFACE_CONSOLE_MAIN_MENU_AMBIGUITY_SUBMENU, mainMenuAmbiguitySubMenuButton_ );

		mainMenuProgrammingSubMenuButton_.setEnabled( isEnablingNormalButtons );
		setButtonText( isInvisible, Constants.INTERFACE_CONSOLE_MAIN_MENU_PROGRAMMING_SUBMENU, mainMenuProgrammingSubMenuButton_ );

		mainMenuReasoningSubMenuButton_.setEnabled( isEnablingNormalButtons );
		setButtonText( isInvisible, Constants.INTERFACE_CONSOLE_MAIN_MENU_REASONING_SUBMENU, mainMenuReasoningSubMenuButton_ );
		
		mainMenuHelpButton_.setEnabled( isEnablingNormalButtons );

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_AMBIGUITY )
			{
			mainMenuReadTheFileAmbiguityBostonButton_.setEnabled( !hasSelectedAmbiguityBoston_ && isEnablingNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_BOSTON, mainMenuReadTheFileAmbiguityBostonButton_ );

			mainMenuReadTheFileAmbiguityPresidentsButton_.setEnabled( !hasSelectedAmbiguityPresidents_ && isEnablingNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_PRESIDENTS, mainMenuReadTheFileAmbiguityPresidentsButton_ );
			}
		else
			{
			mainMenuReadTheFileAmbiguityBostonButton_.setVisible( false );
			mainMenuReadTheFileAmbiguityPresidentsButton_.setVisible( false );
			}

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING_CONNECT4 ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING_TOWER_OF_HANOI )
			{
			mainMenuReadTheFileProgrammingConnect4Button_.setEnabled( !hasSelectedMainButton && isEnablingNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_CONNECT4, mainMenuReadTheFileProgrammingConnect4Button_ );

			mainMenuReadTheFileProgrammingTowerOfHanoiButton_.setEnabled( !hasSelectedMainButton && isEnablingNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_TOWER_OF_HANOI, mainMenuReadTheFileProgrammingTowerOfHanoiButton_ );
			}
		else
			{
			mainMenuReadTheFileProgrammingConnect4Button_.setVisible( false );
			mainMenuReadTheFileProgrammingTowerOfHanoiButton_.setVisible( false );
			}

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING )
			{
			mainMenuReasoningReadTheFileScientificChallengeButton_.setEnabled( !hasSelectedMainButton && isEnablingNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_READ_THE_FILE_SCIENTIFIC_CHALLENGE, mainMenuReasoningReadTheFileScientificChallengeButton_ );
			}
		else
			mainMenuReasoningReadTheFileScientificChallengeButton_.setVisible( false );

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DISPLAY_INFORMATION )
			{
			mainMenuReasoningReadTheFileFamilyDefinitionButton_.setEnabled( !hasSelectedMainButton && isEnablingNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_REASONING_FAMILY, mainMenuReasoningReadTheFileFamilyDefinitionButton_ );

			mainMenuReasoningFamilyDefinitionsSubMenuButton_.setEnabled( hasSelectedReasoningFamilyDefinition_ && isEnablingNormalButtons && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS );
			setButtonText( hasSelectedReasoningFamilyDefinition_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SUBMENU, mainMenuReasoningFamilyDefinitionsSubMenuButton_ );

			mainMenuReasoningFamilyConflictsSubMenuButton_.setEnabled( hasSelectedReasoningFamilyDefinition_ && isEnablingNormalButtons && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS );
			setButtonText( hasSelectedReasoningFamilyDefinition_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_CONFLICT_SUBMENU, mainMenuReasoningFamilyConflictsSubMenuButton_ );

			mainMenuReasoningFamilyJustificationSubMenuButton_.setEnabled( hasSelectedReasoningFamilyDefinition_ && isEnablingNormalButtons && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT );
			setButtonText( hasSelectedReasoningFamilyDefinition_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_JUSTIFICATION_REPORT_SUBMENU, mainMenuReasoningFamilyJustificationSubMenuButton_ );

			mainMenuReasoningFamilyQuestionsSubMenuButton_.setEnabled( hasSelectedReasoningFamilyDefinition_ && isEnablingNormalButtons && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS );
			setButtonText( hasSelectedReasoningFamilyDefinition_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_QUESTION_SUBMENU, mainMenuReasoningFamilyQuestionsSubMenuButton_ );

			mainMenuReasoningFamilyDisplayInformationSubMenuButton_.setEnabled( hasSelectedReasoningFamilyDefinition_ && isEnablingNormalButtons && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DISPLAY_INFORMATION );
			setButtonText( hasSelectedReasoningFamilyDefinition_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_DISPLAY_INFORMATION_SUBMENU, mainMenuReasoningFamilyDisplayInformationSubMenuButton_ );
			}
		else
			{
			mainMenuReasoningReadTheFileFamilyDefinitionButton_.setVisible( false );

			mainMenuReasoningFamilyDefinitionsSubMenuButton_.setVisible( false );
			mainMenuReasoningFamilyConflictsSubMenuButton_.setVisible( false );
			mainMenuReasoningFamilyJustificationSubMenuButton_.setVisible( false );
			mainMenuReasoningFamilyQuestionsSubMenuButton_.setVisible( false );
			mainMenuReasoningFamilyDisplayInformationSubMenuButton_.setVisible( false );
			}

		mainMenuBackButton_.setEnabled( isEnablingNormalButtons );
		setButtonText( ( !isInitMenu && !isChangeLanguageMenu && !isChangeFontMenu && !isHelpMenu ), Constants.INTERFACE_CONSOLE_MAIN_MENU_BACK, mainMenuBackButton_ );

		setButtonText( isEnablingNormalButtons, Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_LANGUAGE, mainMenuChangeLanguageButton_ );
		setButtonText( isEnablingNormalButtons, Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_FONT, mainMenuChangeFontButton_ );

		// Sub-menu
		if( !isInitMenu &&
		nSubMenuButtons_ <= Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS )
			{
			for( short index = 0; index < nSubMenuButtons_; index++ )
				subMenuButtonArray_[index].setEnabled( isEnablingSubMenuButtons );
			}

		inputPanel_.setPreferredSize( new Dimension( 0, ( isCurrentUserExpert || isCurrentUserDeveloper ? Constants.CONSOLE_BUTTON_PANE_HEIGHT : 0 ) ) );
		inputField_.requestFocus();
		}

	private static void goToEndOfOutputDocument( boolean isGoingBackFirst )
		{
		Document document;

		if( ( document = outputArea_.getDocument() ) != null )
			{
			if( isGoingBackFirst )
				outputArea_.setCaretPosition( 0 );

			// Go to end of text in output area
			outputArea_.setCaretPosition( document.getLength() );
			}
		}

	private static void resizeFrame()
		{
		int difference;
		int buttonPaneSize = ( titleBarHeight_ + mainMenuPanel_.getHeight() + subMenuPanel_.getHeight() + inputPanel_.getHeight() + windowBottomHeight_ + Constants.CONSOLE_BUTTON_PANE_HEIGHT );
		int preferredOutputScrollPaneSize = ( currentFrameHeight_ - buttonPaneSize );

		if( !isStopResizing_ &&
		nSubMenuButtons_ > 0 &&
		nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS &&
		subMenuButtonArray_[nSubMenuButtons_].isVisible() )
			nSubMenuButtons_++;

		difference = preferredOutputScrollPaneSize - wantedPreferredOutputScrollPaneSize_;

		if( difference == 0 )
			{
			if( nSubMenuButtons_ > 1 &&
			outputScrollPane_.getHeight() < buttonPaneSize )
				{
				isStopResizing_ = true;
				nSubMenuButtons_--;
				}
			else
				isGUIavailable_ = true;
			}
		else
			{
			if( isActionPerformed_ )
				{
				wantedPreferredOutputScrollPaneSize_ = preferredOutputScrollPaneSize;
				goToEndOfOutputDocument( true );
				}
			else
				{
				if( difference > 0 )
					{
					if( difference > 9 )
						wantedPreferredOutputScrollPaneSize_ += ( difference / 5 );
					else
						wantedPreferredOutputScrollPaneSize_++;

					}
				}
			}

		mainMenuPanel_.setPreferredSize( new Dimension( 0, ( Constants.CONSOLE_BUTTON_PANE_HEIGHT + mainMenuHelpButton_.getY() ) ) );
		subMenuPanel_.setPreferredSize( new Dimension( 0, ( Constants.CONSOLE_BUTTON_PANE_HEIGHT + ( nSubMenuButtons_ > 0 && nSubMenuButtons_ <= Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS ? subMenuButtonArray_[nSubMenuButtons_ - 1].getY() : ( initialLabelEnterSentence_.isVisible() ? initialLabelEnterSentence_.getY() : initialLabelDemoButtons_.getY() ) ) ) ) );
		outputScrollPane_.setPreferredSize( new Dimension( 0, preferredOutputScrollPaneSize ) );
		subMenuPanel_.revalidate();
		}

	private static void resizeFrameComponents( ComponentEvent componentEvent )
		{
		Component component;

		if( componentEvent != null &&
		( component = componentEvent.getComponent() ) != null )
			{
			isStopResizing_ = false;
			currentFrameHeight_ = component.getHeight();
			resizeFrame();
			}
		}

	private static void setButtonText( boolean isInvisible, short interfaceParameter, JButton menuButton )
		{
		boolean isButtonInvisible;
		String interfaceString;

		if( menuButton != null )
			{
			interfaceString = ( currentLanguageWordItem_ != null ? currentLanguageWordItem_.interfaceString( interfaceParameter ) : null );

			isButtonInvisible = ( isInvisible &&
								interfaceString != null &&
								// Don't display undefined buttons
								!interfaceString.equals( Constants.INTERFACE_STRING_NOT_AVAILABLE ) );

			// Change language button and Change font button are always visible
			if( interfaceParameter == Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_LANGUAGE ||
			interfaceParameter == Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_FONT )
				menuButton.setEnabled( isButtonInvisible && ( interfaceParameter == Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_FONT || CommonVariables.interfaceLanguageStringBuffer != null ) );
			else
				menuButton.setVisible( isButtonInvisible );

			if( isButtonInvisible )
				menuButton.setText( interfaceString );
			}
		}

	private static void setInterfaceLanguage()
		{
		if( CommonVariables.currentLanguageWordItem != null &&
		CommonVariables.currentLanguageWordItem != currentLanguageWordItem_ )
			{
			currentLanguageWordItem_ = CommonVariables.currentLanguageWordItem;
			
			// Upper panel buttons
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_CLEAR_YOUR_MIND, upperMenuClearYourMindButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_RESTART, upperMenuRestartButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_UNDO, upperMenuUndoButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_REDO, upperMenuRedoButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_LOGIN_AS_EXPERT, upperMenuLoginAsExpertButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_LOGIN_AS_DEVELOPER, upperMenuLoginAsDeveloperButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_REGRESSION_TEST_USER, upperMenuReadTheTestFileRegressionTestUserButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_REGRESSION_TEST_DEEP, upperMenuReadTheTestFileRegressionTestDeepButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_STOP_TESTING, upperMenuStopTestingButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_MORE_EXAMPLES, upperMenuMoreExamplesButton_ );

			// Main menu buttons
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_AMBIGUITY_SUBMENU, mainMenuAmbiguitySubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_BOSTON, mainMenuReadTheFileAmbiguityBostonButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_PRESIDENTS, mainMenuReadTheFileAmbiguityPresidentsButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_PROGRAMMING_SUBMENU, mainMenuProgrammingSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_CONNECT4, mainMenuReadTheFileProgrammingConnect4Button_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_TOWER_OF_HANOI, mainMenuReadTheFileProgrammingTowerOfHanoiButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_REASONING_SUBMENU, mainMenuReasoningSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_READ_THE_FILE_SCIENTIFIC_CHALLENGE, mainMenuReasoningReadTheFileScientificChallengeButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_REASONING_FAMILY, mainMenuReasoningReadTheFileFamilyDefinitionButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SUBMENU, mainMenuReasoningFamilyDefinitionsSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_CONFLICT_SUBMENU, mainMenuReasoningFamilyConflictsSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_JUSTIFICATION_REPORT_SUBMENU, mainMenuReasoningFamilyJustificationSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_QUESTION_SUBMENU, mainMenuReasoningFamilyQuestionsSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_DISPLAY_INFORMATION_SUBMENU, mainMenuReasoningFamilyDisplayInformationSubMenuButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_BACK, mainMenuBackButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_LANGUAGE, mainMenuChangeLanguageButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_FONT, mainMenuChangeFontButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_HELP, mainMenuHelpButton_ );

			// Sub-menu
			setSubMenuButtonTexts();

			// Initial labels
			initialLabelDemoButtons_.setText( currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_INITIAL_MESSAGE_DEMO_BUTTONS ) );
			initialLabelEnterSentence_.setText( currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_INITIAL_MESSAGE_ENTER_SENTENCE ) );
			}
		}

	private static void setSubMenuButtonTexts()
		{
		int endPosition;
		int grammarLanguageStringBufferLength;
		int startPosition = 0;
		String currentLanguageString;
		String languageString;

		isStopResizing_ = false;
		nSubMenuButtons_ = 0;

		switch( currentSubMenu_ )
			{
			case Constants.CONSOLE_SUBMENU_PROGRAMMING_CONNECT4:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_A );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_B );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_C );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_D );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_E );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_F );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_G );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_SHOW_INFO_ABOUT_THE_SET );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_THE_SOLVE_LEVEL_IS_LOW );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_THE_SOLVE_LEVEL_IS_HIGH );
				break;

			case Constants.CONSOLE_SUBMENU_PROGRAMMING_TOWER_OF_HANOI:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_TOWER_OF_HANOI_EVEN_NUMBER_OF_DISCS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_PROGRAMMING_TOWER_OF_HANOI_ODD_NUMBER_OF_DISCS );
				break;

			case Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_THE_FATHER_AND_ANN_IS_THE_MOTHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_THE_FATHER_OF_PAUL_JOE_AND_LAURA );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_THE_MOTHER_OF_PAUL_JOE_AND_LAURA );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_SON_OF_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_SON_OF_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_DAUGHTER_OF_JOHN_AND_ANN );

				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_FATHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_MOTHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_DAUGHTER );

				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_HAS_A_FATHER_CALLED_JOHN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_HAS_A_MOTHER_CALLED_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_HAS_A_FATHER_CALLED_JOHN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_HAS_A_MOTHER_CALLED_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_HAS_A_FATHER_CALLED_JOHN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_HAS_A_MOTHER_CALLED_ANN );

				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_PARENT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_PARENT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_PARENT_OF_PAUL_JOE_AND_LAURA );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_PARENT_OF_PAUL_JOE_AND_LAURA );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_HAS_2_PARENT_CALLED_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_HAS_2_PARENT_CALLED_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_HAS_2_PARENT_CALLED_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_CHILD_OF_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_CHILD_OF_JOHN_AND_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_CHILD_OF_JOHN_AND_ANN );

				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_WOMAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_WOMAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_CHILD );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_CHILD );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_CHILD );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_BOY );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_BOY );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_GIRL );
				break;

			case Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_JOHN_IS_A_WOMAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_JOHN_IS_THE_MOTHER_PETE );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_ANN_IS_A_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_PAUL_IS_A_DAUGHTER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_JOE_IS_A_MOTHER );
				break;

			case Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_FAMILY );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_PARENT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_CHILD );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_FATHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_MOTHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_DAUGHTER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_JOHN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_PAUL );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_JOE );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_JUSTIFICATION_REPORT_FOR_LAURA );
				break;

			case Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_A_FATHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_A_MOTHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_THE_FATHER_OF_PAUL );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_THE_MOTHER_OF_PAUL );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_WOMAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_MAN_OR_A_WOMAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_DAUGHTER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_SON_OR_A_DAUGHTER );
				break;

			case Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DISPLAY_INFORMATION:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_FAMILY );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_PARENT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_CHILD );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_FATHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_MOTHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_DAUGHTER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_JOHN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_PAUL );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_JOE );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_LAURA );
				break;

			case Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE:
				if( CommonVariables.interfaceLanguageStringBuffer != null )
					{
					grammarLanguageStringBufferLength = CommonVariables.interfaceLanguageStringBuffer.length();

					if( currentLanguageWordItem_ != null &&
					( currentLanguageString = currentLanguageWordItem_.anyWordTypeString() ) != null )
						{
						while( startPosition < grammarLanguageStringBufferLength &&
						nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS )
							{
							if( ( endPosition = CommonVariables.interfaceLanguageStringBuffer.substring( startPosition ).indexOf( Constants.QUERY_SEPARATOR_SPACE_STRING ) ) < 0 )
								// Last language in string
								endPosition = grammarLanguageStringBufferLength;
							else
								endPosition += startPosition;
	
							if( ( languageString = CommonVariables.interfaceLanguageStringBuffer.substring( startPosition, endPosition ) ) != null )
								{
								// Disable current language
								subMenuButtonArray_[nSubMenuButtons_].setEnabled( !languageString.equals( currentLanguageString ) );
								subMenuButtonArray_[nSubMenuButtons_++].setText( Constants.CHANGE_LANGUAGE_STRING + languageString );
								}
	
							// Prepare start position for next word
							startPosition = endPosition + Constants.QUERY_SEPARATOR_SPACE_STRING.length();
							}
						}
					}

				break;

			case Constants.CONSOLE_SUBMENU_CHANGE_FONT:
				// Decrease font size button
				if( currentFontSize_ <= Constants.CONSOLE_MINIMUM_FONT_SIZE )
					subMenuButtonArray_[nSubMenuButtons_].setEnabled( false );

				addSubMenuButton( Constants.INTERFACE_CONSOLE_FONT_SIZE_DECREASE );

				// Increase font size button
				if( currentFontSize_ >= Constants.CONSOLE_MAXIMUM_FONT_SIZE )
					subMenuButtonArray_[nSubMenuButtons_].setEnabled( false );

				addSubMenuButton( Constants.INTERFACE_CONSOLE_FONT_SIZE_INCREASE );

				addSubMenuFontButton( Constants.CONSOLE_FONT_ARIEL );
				addSubMenuFontButton( Constants.CONSOLE_FONT_COURIER );
				addSubMenuFontButton( Constants.CONSOLE_FONT_COURIER_NEW );
				addSubMenuFontButton( Constants.CONSOLE_FONT_MICROSOFT_SANS_SERIF );
				addSubMenuFontButton( Constants.CONSOLE_FONT_MONOSPACED );
				addSubMenuFontButton( Constants.CONSOLE_FONT_TIMES_NEW_ROMAN );
				addSubMenuFontButton( Constants.CONSOLE_FONT_VERDANA );
				break;

			case Constants.CONSOLE_SUBMENU_HELP:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_LANGUAGES );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_LISTS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_USERS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_WORD_TYPES );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_QUERY_COMMANDS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_COPYRIGHT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_GPLv2_LICENSE );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_WARRANTY );
				break;
			}

		initialLabelEnterSentence_.setVisible( nSubMenuButtons_ == 0 && adminItem_ != null && adminItem_.isCurrentUserDeveloperOrExpert() );
		}

	private static void setSubMenuVisible( boolean isClearYourMind, short subMenu )
		{
		boolean hasNoSubMenuButtons;

		if( !isClearYourMind )
			currentSubMenu_ = subMenu;

			setSubMenuButtonTexts();

		displayProgressStatus( null );

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuButtonArray_[index].setVisible( index < nSubMenuButtons_ );

		hasNoSubMenuButtons = ( nSubMenuButtons_ == 0 );

		initialLabelDemoButtons_.setVisible( hasNoSubMenuButtons );
		initialLabelEnterSentence_.setVisible( hasNoSubMenuButtons && adminItem_ != null && adminItem_.isCurrentUserDeveloperOrExpert() );
		}

	private static void setUpperMenuVisible( boolean isVisible )
		{
		boolean isCurrentUserExpert = ( adminItem_ != null &&
										adminItem_.isCurrentUserExpert() );
		boolean isCurrentUserDeveloper = ( adminItem_ != null &&
										adminItem_.isCurrentUserDeveloper() );

		upperMenuClearYourMindButton_.setVisible( isVisible );
		upperMenuRestartButton_.setVisible( isVisible );
		upperMenuUndoButton_.setVisible( isVisible );
		upperMenuRedoButton_.setVisible( isVisible );

		upperMenuLoginAsExpertButton_.setVisible( isVisible && !isCurrentUserExpert );
		upperMenuLoginAsDeveloperButton_.setVisible( isVisible && isCurrentUserExpert );
		upperMenuReadTheTestFileRegressionTestUserButton_.setVisible( isVisible && isCurrentUserDeveloper );
		upperMenuReadTheTestFileRegressionTestDeepButton_.setVisible( isVisible && isCurrentUserDeveloper );
		upperMenuStopTestingButton_.setVisible( isVisible && isTesting_ );

		upperMenuMoreExamplesButton_.setVisible( isVisible );
		}

	private static String convertDiacriticalText( String textString )
		{
		int position = 0;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer textStringBuffer = new StringBuffer();

		if( textString != null )
			{
			if( textString.charAt( 0 ) == Constants.QUERY_STRING_START_CHAR )
				position++;

			while( position < textString.length() &&
			textString.charAt( position ) != Constants.QUERY_STRING_END_CHAR )
				{
				if( textString.charAt( position ) == Constants.SYMBOL_BACK_SLASH )
					{
					if( ++position < textString.length() )
						{
						textChar = Presentation.convertDiacriticalChar( textString.charAt( position ) );
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

	private static String getInputString()
		{
		inputString_ = null;

		do	{
			resizeFrame();

			try {
				Thread.sleep( Constants.CONSOLE_SLEEP_TIME );
				}
			catch( InterruptedException exception )
				{
				exception.printStackTrace();
				}
			}
		while( !isActionPerformed_ ||
		inputString_ == null );

		return inputString_;
		}


	// Constructor

	protected Console()
		{
		super( new GridBagLayout() );

		Border border = BorderFactory.createEmptyBorder( Constants.CONSOLE_BORDER_SIZE, Constants.CONSOLE_BORDER_SIZE, Constants.CONSOLE_BORDER_SIZE, Constants.CONSOLE_BORDER_SIZE );

		// Create output area
		outputArea_ = new JTextArea();
		outputArea_.setEditable( false );
		outputArea_.setLineWrap( true );
		outputArea_.setWrapStyleWord( true );

		// Set font for output area
		currentFontSize_ = Constants.CONSOLE_DEFAULT_FONT_SIZE;
		currentFontString_ = Constants.CONSOLE_FONT_COURIER_NEW;
		outputArea_.setFont( new Font( currentFontString_, Font.PLAIN, currentFontSize_ ) );

		// Create scroll pane with border for output area
		outputScrollPane_ = new JScrollPane( outputArea_ );
		outputScrollPane_.setVerticalScrollBarPolicy( ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS );
		outputScrollPane_.setBorder( BorderFactory.createCompoundBorder( border, outputScrollPane_.getBorder() ) );

		// Create Upper panel with border
		upperMenuPanel_ = new JPanel();
		upperMenuPanel_.setLayout( new GridBagLayout() );
		upperMenuPanel_.setPreferredSize( new Dimension( 0, Constants.CONSOLE_BUTTON_PANE_HEIGHT ) );
		upperMenuPanel_.setBorder( BorderFactory.createCompoundBorder( border, upperMenuPanel_.getBorder() ) );

		// Create Main menu with border
		mainMenuPanel_ = new JPanel();
		mainMenuPanel_.setBorder( BorderFactory.createCompoundBorder( border, mainMenuPanel_.getBorder() ) );

		// Create sub-menu panel with border
		subMenuPanel_ = new JPanel();
		subMenuPanel_.setBorder( BorderFactory.createCompoundBorder( border, subMenuPanel_.getBorder() ) );

		// Create input panel with border
		inputPanel_ = new JPanel();
		inputPanel_.setLayout( new GridBagLayout() );
		inputPanel_.setBorder( BorderFactory.createCompoundBorder( border, inputPanel_.getBorder() ) );

		// Create Upper panel buttons
		upperMenuClearYourMindButton_ = new JButton();
		upperMenuClearYourMindButton_.addActionListener( this );

		upperMenuRestartButton_ = new JButton();
		upperMenuRestartButton_.addActionListener( this );

		upperMenuUndoButton_ = new JButton();
		upperMenuUndoButton_.addActionListener( this );

		upperMenuRedoButton_ = new JButton();
		upperMenuRedoButton_.addActionListener( this );

		upperMenuLoginAsExpertButton_ = new JButton();
		upperMenuLoginAsExpertButton_.addActionListener( this );

		upperMenuLoginAsDeveloperButton_ = new JButton();
		upperMenuLoginAsDeveloperButton_.addActionListener( this );

		upperMenuReadTheTestFileRegressionTestUserButton_ = new JButton();
		upperMenuReadTheTestFileRegressionTestUserButton_.addActionListener( this );

		upperMenuReadTheTestFileRegressionTestDeepButton_ = new JButton();
		upperMenuReadTheTestFileRegressionTestDeepButton_.addActionListener( this );

		upperMenuStopTestingButton_ = new JButton();
		upperMenuStopTestingButton_.addActionListener( this );

		upperMenuMoreExamplesButton_ = new JButton();
		upperMenuMoreExamplesButton_.addActionListener( this );

		// Create the file chooser
		fileChooser_ = new JFileChooser( CommonVariables.currentPathStringBuffer + Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING );

		// Create progress bar
		upperMenuProgressLabel_ = new JLabel();

		upperMenuProgressBar_ = new JProgressBar();
		upperMenuProgressBar_.setVisible( false );
		upperMenuProgressBar_.setStringPainted( true );

		// Create the Main menu buttons
		mainMenuAmbiguitySubMenuButton_ = new JButton();
		mainMenuAmbiguitySubMenuButton_.addActionListener( this );

		mainMenuReadTheFileAmbiguityBostonButton_ = new JButton();
		mainMenuReadTheFileAmbiguityBostonButton_.addActionListener( this );

		mainMenuReadTheFileAmbiguityPresidentsButton_ = new JButton();
		mainMenuReadTheFileAmbiguityPresidentsButton_.addActionListener( this );

		mainMenuProgrammingSubMenuButton_ = new JButton();
		mainMenuProgrammingSubMenuButton_.addActionListener( this );

		mainMenuReadTheFileProgrammingConnect4Button_ = new JButton();
		mainMenuReadTheFileProgrammingConnect4Button_.addActionListener( this );

		mainMenuReadTheFileProgrammingTowerOfHanoiButton_ = new JButton();
		mainMenuReadTheFileProgrammingTowerOfHanoiButton_.addActionListener( this );

		mainMenuReasoningSubMenuButton_ = new JButton();
		mainMenuReasoningSubMenuButton_.addActionListener( this );

		mainMenuReasoningReadTheFileScientificChallengeButton_ = new JButton();
		mainMenuReasoningReadTheFileScientificChallengeButton_.addActionListener( this );

		mainMenuReasoningReadTheFileFamilyDefinitionButton_ = new JButton();
		mainMenuReasoningReadTheFileFamilyDefinitionButton_.addActionListener( this );

		mainMenuReasoningFamilyDefinitionsSubMenuButton_ = new JButton();
		mainMenuReasoningFamilyDefinitionsSubMenuButton_.addActionListener( this );

		mainMenuReasoningFamilyConflictsSubMenuButton_ = new JButton();
		mainMenuReasoningFamilyConflictsSubMenuButton_.addActionListener( this );

		mainMenuReasoningFamilyJustificationSubMenuButton_ = new JButton();
		mainMenuReasoningFamilyJustificationSubMenuButton_.addActionListener( this );

		mainMenuReasoningFamilyQuestionsSubMenuButton_ = new JButton();
		mainMenuReasoningFamilyQuestionsSubMenuButton_.addActionListener( this );

		mainMenuReasoningFamilyDisplayInformationSubMenuButton_ = new JButton();
		mainMenuReasoningFamilyDisplayInformationSubMenuButton_.addActionListener( this );

		mainMenuBackButton_ = new JButton();
		mainMenuBackButton_.addActionListener( this );

		mainMenuChangeLanguageButton_ = new JButton();
		mainMenuChangeLanguageButton_.addActionListener( this );

		mainMenuChangeFontButton_ = new JButton();
		mainMenuChangeFontButton_.addActionListener( this );

		mainMenuHelpButton_ = new JButton();
		mainMenuHelpButton_.addActionListener( this );

		initialLabelDemoButtons_ = new JLabel();
		initialLabelEnterSentence_ = new JLabel();

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuButtonArray_[index] = new JButton();

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuButtonArray_[index].addActionListener( this );

		// Create input field
		inputField_ = new JTextField();
		inputField_.setFont( new Font( Constants.CONSOLE_FONT_COURIER_NEW, Font.PLAIN, Constants.CONSOLE_DEFAULT_FONT_SIZE ) );
		inputField_.addActionListener( this );

		// Create password field
		passwordField_ = new JPasswordField();
		passwordField_.setVisible( false );
		passwordField_.addActionListener( this );

		// Create grid bag constraints
		GridBagConstraints westOrientedConstraints = new GridBagConstraints();
		westOrientedConstraints.gridwidth = GridBagConstraints.WEST;
		westOrientedConstraints.fill = GridBagConstraints.BOTH;
		westOrientedConstraints.weightx = 1.0;
		westOrientedConstraints.weighty = 1.0;

		GridBagConstraints eastOrientedConstraints = new GridBagConstraints();
		eastOrientedConstraints.gridwidth = GridBagConstraints.EAST;
		eastOrientedConstraints.fill = GridBagConstraints.BOTH;
		eastOrientedConstraints.weightx = 1.0;
		eastOrientedConstraints.weighty = 1.0;

		GridBagConstraints remainderOrientedConstraints = new GridBagConstraints();
		remainderOrientedConstraints.gridwidth = GridBagConstraints.REMAINDER;
		remainderOrientedConstraints.fill = GridBagConstraints.BOTH;
		remainderOrientedConstraints.weightx = 1.0;
		remainderOrientedConstraints.weighty = 1.0;

		// Add fields to Upper panel
		upperMenuPanel_.add( upperMenuProgressLabel_, westOrientedConstraints );
		upperMenuPanel_.add( upperMenuProgressBar_, eastOrientedConstraints );

		upperMenuPanel_.add( upperMenuClearYourMindButton_ );
		upperMenuPanel_.add( upperMenuRestartButton_ );
		upperMenuPanel_.add( upperMenuUndoButton_ );
		upperMenuPanel_.add( upperMenuRedoButton_ );

		upperMenuPanel_.add( upperMenuLoginAsExpertButton_ );
		upperMenuPanel_.add( upperMenuLoginAsDeveloperButton_ );
		upperMenuPanel_.add( upperMenuReadTheTestFileRegressionTestUserButton_ );
		upperMenuPanel_.add( upperMenuReadTheTestFileRegressionTestDeepButton_ );
		upperMenuPanel_.add( upperMenuStopTestingButton_ );

		upperMenuPanel_.add( upperMenuMoreExamplesButton_ );

		// Add buttons to Main menu panel
		mainMenuPanel_.add( mainMenuAmbiguitySubMenuButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileAmbiguityBostonButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileAmbiguityPresidentsButton_ );

		mainMenuPanel_.add( mainMenuProgrammingSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileProgrammingConnect4Button_ );
		mainMenuPanel_.add( mainMenuReadTheFileProgrammingTowerOfHanoiButton_ );

		mainMenuPanel_.add( mainMenuReasoningSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReasoningReadTheFileScientificChallengeButton_ );
		mainMenuPanel_.add( mainMenuReasoningReadTheFileFamilyDefinitionButton_ );
		mainMenuPanel_.add( mainMenuReasoningFamilyDefinitionsSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReasoningFamilyConflictsSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReasoningFamilyJustificationSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReasoningFamilyQuestionsSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReasoningFamilyDisplayInformationSubMenuButton_ );

		mainMenuPanel_.add( mainMenuBackButton_ );
		mainMenuPanel_.add( mainMenuChangeLanguageButton_ );
		mainMenuPanel_.add( mainMenuChangeFontButton_ );
		mainMenuPanel_.add( mainMenuHelpButton_ );

		// Add components to sub-menu panel
		subMenuPanel_.add( initialLabelDemoButtons_ );
		subMenuPanel_.add( initialLabelEnterSentence_ );

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuPanel_.add( subMenuButtonArray_[index] );

		// Add input field to input panel
		inputPanel_.add( inputField_, remainderOrientedConstraints );
		inputPanel_.add( passwordField_, remainderOrientedConstraints );

		// Add pane and panels to the frame
		add( outputScrollPane_, remainderOrientedConstraints );
		add( upperMenuPanel_, remainderOrientedConstraints );
		add( mainMenuPanel_, remainderOrientedConstraints );
		add( subMenuPanel_, remainderOrientedConstraints );
		add( inputPanel_, remainderOrientedConstraints );
		}


	// Protected methods

	protected static void addError( String newErrorString )
		{
		if( errorStringBuffer_ == null )
			errorStringBuffer_ = new StringBuffer( newErrorString );
		else
			errorStringBuffer_.append( newErrorString ); 
		}

	protected static void addError( String newHeaderString, String newErrorString )
		{
		errorHeaderString_ = newHeaderString;
		addError( newErrorString ); 
		}

	protected static void clearProgress()
		{
		displayProgressStatus( null );
		upperMenuProgressBar_.setVisible( false );
		setUpperMenuVisible( true );
		}

	protected static void displayError()
		{
		if( errorStringBuffer_ != null )
			{
			JTextArea errorTextArea = new JTextArea( errorStringBuffer_.toString() );
			errorTextArea.setEditable( false );
			JScrollPane errorScrollPane = new JScrollPane( errorTextArea );
			errorScrollPane.setPreferredSize( new Dimension( Constants.CONSOLE_ERROR_PANE_WIDTH, Constants.CONSOLE_ERROR_PANE_HEIGHT ) );
			JOptionPane.showMessageDialog( null, errorScrollPane, ( errorHeaderString_ == null ? Constants.PRESENTATION_ERROR_INTERNAL_TITLE_STRING : Constants.PRESENTATION_ERROR_INTERNAL_TITLE_STRING + errorHeaderString_ ), JOptionPane.ERROR_MESSAGE );			
			errorHeaderString_ = null;
			errorStringBuffer_ = null;
			}
		}

	protected static void displayProgress( int currentProgress )
		{
		upperMenuProgressBar_.setValue( currentProgress );
		}

	protected static void displayProgressStatus( String newStatusString )
		{
		if( newStatusString == null )
			upperMenuProgressLabel_.setVisible( false );
		else
			{
			setUpperMenuVisible( false );

			upperMenuProgressLabel_.setText( newStatusString );
			upperMenuProgressLabel_.setVisible( true );
			}
		}

	protected static void restart( int titleBarHeight, int windowBottomHeight )
		{
		// Initialize private variables
		hasSelectedAmbiguityBoston_ = false;
		hasSelectedAmbiguityPresidents_ = false;
		hasSelectedProgrammingConnect4_ = false;
		hasSelectedProgrammingTowerOfHanoi_ = false;
		hasSelectedReasoningScientificChallenge_ = false;
		hasSelectedReasoningFamilyDefinition_ = false;
		hasSelectedMoreExamples_ = false;

		isActionPerformed_ = false;
		isGUIavailable_ = false;
		isStopResizing_ = false;
		isTesting_ = false;
		isTestingCanceled_ = false;

		currentSubMenu_ = Constants.CONSOLE_SUBMENU_INIT;
		nSubMenuButtons_ = 0;

//		currentFrameHeight_ = 0;
		titleBarHeight_ = titleBarHeight;
		wantedPreferredOutputScrollPaneSize_ = 0;
		windowBottomHeight_ = windowBottomHeight;

		errorHeaderString_ = null;
		errorStringBuffer_ = null;

		adminItem_ = null;
		currentLanguageWordItem_ = null;

		// Initialize sub-menus
		setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_INIT );

		// Disable menus
		enableMenus( false, false );

		// Clear screen
		outputArea_.setText( Constants.NEW_LINE_STRING );

		// Clear fields
		inputField_.setVisible( false );
		inputField_.setText( Constants.EMPTY_STRING );

		do	resizeFrame();
		while( !isGUIavailable_ );
		}

	protected static void setAdminItem( AdminItem adminItem )
		{
		adminItem_ = adminItem;
		}

	protected static void startProgress( int startProgress, int maxProgress, String progressString )
		{
		upperMenuProgressBar_.setValue( startProgress );
		upperMenuProgressBar_.setMaximum( maxProgress );

		displayProgressStatus( progressString );
		upperMenuProgressBar_.setVisible( true );
		}

	protected static void writeText( String textString )
		{
		if( textString != null )
			{
			// Show text in output area
			outputArea_.append( textString );
			goToEndOfOutputDocument( false );
			}
		else
			{
			addError( "Class Console;\nMethod: writeText;\nError: The given text string is undefined." );
			displayError();
			}
		}

	protected static boolean isTestingCanceled()
		{
		return isTestingCanceled_;
		}

	protected static short nSubMenuButtons()
		{
		return nSubMenuButtons_;
		}

	protected static String getPassword()
		{
		if( errorStringBuffer_ != null )
			displayError();

		// Switch input field for password field
		inputField_.setText( Constants.EMPTY_STRING );
		inputField_.setVisible( false );
		passwordField_.setVisible( true );
		passwordField_.setText( Constants.EMPTY_STRING );
		passwordField_.requestFocus();

		inputString_ = getInputString();

		// Hide password field afterwards
		passwordField_.setVisible( false );

		return inputString_;
		}

	protected static String readLine( boolean isClearInputField )
		{
		boolean isSystemStartingUp = ( adminItem_ == null );

		isTesting_ = false;
		isTestingCanceled_ = false;

		if( errorStringBuffer_ != null )
			displayError();

		// Set the language of the interface
		setInterfaceLanguage();

		if( isClearInputField )
			inputField_.setText( Constants.EMPTY_STRING );

		// Set Upper menu visible
		setUpperMenuVisible( true );

		// Enable menus again
		enableMenus( !isSystemStartingUp, !isSystemStartingUp );
		setSubMenuButtonTexts();

		// Prepare input field
		inputField_.setEnabled( true );
		inputField_.setVisible( adminItem_ != null && adminItem_.isCurrentUserDeveloperOrExpert() );
		inputField_.requestFocus();

		// Select all text in input field
		inputField_.selectAll();
		inputString_ = getInputString();

		// Disable input field
		inputField_.setEnabled( false );

		return inputString_;
		}


	// Public methods

	public void actionPerformed( ActionEvent actionEvent )
		{
		File selectedFile;
		File currentDirectory;
		Object actionSource;
		String actionCommandString;
		String currentDirectoryString;
		String currentLanguageString;
		String currentUserNounString;
		String selectedFilePath;

		isActionPerformed_ = false;

		if( currentLanguageWordItem_ == null )
			setInterfaceLanguage();

		if( currentLanguageWordItem_ != null )
			{
			if( ( actionSource = actionEvent.getSource() ) != null )
				{
				if( ( actionCommandString = actionEvent.getActionCommand() ) != null )
					{
					// Disable menus during action
					enableMenus( false, false );

					if( actionSource == passwordField_ )
						inputString_ = String.valueOf( passwordField_.getPassword() );
					else
						{
						if( actionSource == inputField_ )
							inputString_ = inputField_.getText();
						else
							{
							if( actionSource == upperMenuClearYourMindButton_ )
								{
								hasSelectedAmbiguityBoston_ = false;
								hasSelectedAmbiguityPresidents_ = false;
								hasSelectedProgrammingConnect4_ = false;
								hasSelectedProgrammingTowerOfHanoi_ = false;
								hasSelectedReasoningScientificChallenge_ = false;
								hasSelectedReasoningFamilyDefinition_ = false;
								hasSelectedMoreExamples_ = false;

								setSubMenuVisible( true, Constants.CONSOLE_SUBMENU_INIT );

								inputString_ = actionCommandString;
								}
							else
								{
								if( actionSource == upperMenuRestartButton_ )
									{
									inputString_ = actionCommandString;
									restart( titleBarHeight_, windowBottomHeight_ );
									}
								else
									{
									if( actionSource == upperMenuReadTheTestFileRegressionTestUserButton_ ||
									actionSource == upperMenuReadTheTestFileRegressionTestDeepButton_ )
										{
										isTesting_ = true;
										inputString_ = ( currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_UPPER_MENU_READ_THE_TEST_FILE ) + currentLanguageWordItem_.anyWordTypeString() + Constants.SYMBOL_SLASH + actionCommandString + convertDiacriticalText( currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_END ) ) );
										}
									else
										{
										if( actionSource == upperMenuStopTestingButton_ )
											{
											isTesting_ = false;
											isTestingCanceled_ = true;
											}
										else
											{
											if( actionSource == upperMenuMoreExamplesButton_ )
												{
												if( ( currentDirectory = fileChooser_.getCurrentDirectory() ) != null )
													{
													if( adminItem_ != null &&
													currentLanguageWordItem_ != null &&
													( currentDirectoryString = currentDirectory.toString() ) != null &&
													( currentLanguageString = currentLanguageWordItem_.anyWordTypeString() ) != null )
														{
														currentUserNounString = ( adminItem_.isCurrentUserDeveloper() ||
																				CommonVariables.predefinedNounUserWordItem == null ? null : CommonVariables.predefinedNounUserWordItem.wordTypeString( false, Constants.WORD_TYPE_NOUN_SINGULAR ) );

														// No file selected yet with current language
														if( currentDirectoryString.indexOf( currentLanguageString ) < 0 ) 
															// Select current language in file chooser
															fileChooser_ = new JFileChooser( CommonVariables.currentPathStringBuffer + Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING + currentLanguageString + ( currentUserNounString == null ? Constants.EMPTY_STRING : ( Constants.SLASH_STRING + currentUserNounString ) ) );
														}
													} 
		
												if( fileChooser_.showOpenDialog( this ) == JFileChooser.APPROVE_OPTION )
													{
													if( ( selectedFile = fileChooser_.getSelectedFile() ) != null )
														{
														if( ( selectedFilePath = selectedFile.getPath() ) != null )
															{
															hasSelectedMoreExamples_ = true;
															inputString_ = convertDiacriticalText( currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_START ) ) + selectedFilePath + convertDiacriticalText( currentLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_END ) );
															}
														}
													}
												else
													// If canceled
													enableMenus( true, true );
												}
											else
												{
												if( actionSource == mainMenuAmbiguitySubMenuButton_ )
													{
													setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_AMBIGUITY );
													enableMenus( true, true );
													}
												else
													{
													if( actionSource == mainMenuReadTheFileAmbiguityBostonButton_ )
														{
														hasSelectedAmbiguityBoston_ = true;
														inputString_ = actionCommandString;
														}
													else
														{
														if( actionSource == mainMenuReadTheFileAmbiguityPresidentsButton_ )
															{
															hasSelectedAmbiguityPresidents_ = true;
															inputString_ = actionCommandString;
															}
														else
															{
															if( actionSource == mainMenuProgrammingSubMenuButton_ )
																{
																setSubMenuVisible( false, ( hasSelectedProgrammingConnect4_ ? Constants.CONSOLE_SUBMENU_PROGRAMMING_CONNECT4 : Constants.CONSOLE_SUBMENU_PROGRAMMING ) );
																enableMenus( true, true );
																}
															else
																{
																if( actionSource == mainMenuReadTheFileProgrammingConnect4Button_ )
																	{
																	hasSelectedProgrammingConnect4_ = true;
																	setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_PROGRAMMING_CONNECT4 );
																	inputString_ = actionCommandString;
																	}
																else
																	{
																	if( actionSource == mainMenuReadTheFileProgrammingTowerOfHanoiButton_ )
																		{
																		hasSelectedProgrammingTowerOfHanoi_ = true;
																		setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_PROGRAMMING_TOWER_OF_HANOI );
																		inputString_ = actionCommandString;
																		}
																	else
																		{
																		if( actionSource == mainMenuReasoningSubMenuButton_ )
																			{
																			setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING );
																			enableMenus( true, true );
																			}
																		else
																			{
																			if( actionSource == mainMenuReasoningReadTheFileScientificChallengeButton_ )
																				{
																				hasSelectedReasoningScientificChallenge_ = true;
																				inputString_ = actionCommandString;
																				}
																			else
																				{
																				if( actionSource == mainMenuReasoningReadTheFileFamilyDefinitionButton_ )
																					{
																					hasSelectedReasoningFamilyDefinition_ = true;
																					setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS );
																					inputString_ = actionCommandString;
																					}
																				else
																					{
																					if( actionSource == mainMenuReasoningFamilyDefinitionsSubMenuButton_ )
																						{
																						setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS );
																						enableMenus( true, true );
																						}
																					else
																						{
																						if( actionSource == mainMenuReasoningFamilyConflictsSubMenuButton_ )
																							{
																							setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS );
																							enableMenus( true, true );
																							}
																						else
																							{
																							if( actionSource == mainMenuReasoningFamilyJustificationSubMenuButton_ )
																								{
																								setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT );
																								enableMenus( true, true );
																								}
																							else
																								{
																								if( actionSource == mainMenuReasoningFamilyQuestionsSubMenuButton_ )
																									{
																									setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS );
																									enableMenus( true, true );
																									}
																								else
																									{
																									if( actionSource == mainMenuReasoningFamilyDisplayInformationSubMenuButton_ )
																										{
																										setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DISPLAY_INFORMATION );
																										enableMenus( true, true );
																										}
																									else
																										{
																										if( actionSource == mainMenuBackButton_ )
																											{
																											setSubMenuVisible( false, ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DISPLAY_INFORMATION ? Constants.CONSOLE_SUBMENU_REASONING : Constants.CONSOLE_SUBMENU_INIT ) );
																											enableMenus( true, true );
																											}
																										else
																											{
																											if( actionSource == mainMenuChangeLanguageButton_ )
																												{
																												setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE );
																												enableMenus( true, true );
																												setSubMenuButtonTexts();
																												}
																											else
																												{
																												if( actionSource == mainMenuChangeFontButton_ )
																													{
																													setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_CHANGE_FONT );
																													enableMenus( true, true );
																													setSubMenuButtonTexts();
																													}
																												else
																													{
																													if( actionSource == mainMenuHelpButton_ )
																														{
																														setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_HELP );
																														inputString_ = actionCommandString;
																														}
																													else
																															inputString_ = actionCommandString;
																													}
																												}
																											}
																										}
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}

						if( inputString_ != null )
							{
							if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_CHANGE_FONT )
								changeFont( actionCommandString );
							else
								{
								if( actionSource != inputField_ )
									inputField_.setText( inputString_ );
	
								if( actionSource != upperMenuRestartButton_ )
									writeText( inputString_ + Constants.NEW_LINE_STRING );
								}
							}
						}

					isActionPerformed_ = true;
					}
				else
					{
					addError( "Class Console;\nMethod: actionPerformed;\nError: The action command string is undefined." );
					displayError();
					}
				}
			else
				{
				addError( "Class Console;\nMethod: actionPerformed;\nError: The action source is undefined." );
				displayError();
				}
			}
		else
			{
			addError( "Class Console;\nMethod: actionPerformed;\nError: The current interface language word is undefined." );
			displayError();
			}
		}

	public void componentHidden( ComponentEvent componentEvent )
		{
		/* This method needs to exist for ComponentListener */
		}

	public void componentMoved( ComponentEvent componentEvent )
		{
		resizeFrameComponents( componentEvent );
		}

	public void componentResized( ComponentEvent componentEvent )
		{
		resizeFrameComponents( componentEvent );
		}

	public void componentShown( ComponentEvent componentEvent )
		{
		/* This method needs to exist for ComponentListener */
		}
	};

/*************************************************************************
 *	"I will love the Lord because he hears my voice
 *	and my prayer for mercy.
 *	Because he bends down to listen,
 *	I will pray as long as I breath!" (Psalm 116:1-2)
 *************************************************************************/
