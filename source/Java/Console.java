/*
 *	Class:		Console
 *	Purpose:	To create the GUI and to process the events
 *	Version:	Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *
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
	// Private static constants
	private static final long serialVersionUID = -2264899695119545018L;

	// Private static variables
	private static boolean hasSelectedAmbiguityBoston_;
	private static boolean hasSelectedAmbiguityPresidents_;
	private static boolean hasSelectedProgrammingConnect4_;
	private static boolean hasSelectedProgrammingTowerOfHanoi_;
	private static boolean hasSelectedReasoningACarHasAnEngine;
	private static boolean hasSelectedReasoningJohnWasTheFatherOfPaul_;
	private static boolean hasSelectedReasoningReadTheFileOnlyOptionLeftBoat_;
	private static boolean hasSelectedReasoningFamily_;

	private static boolean isActionPerformed_;
	private static boolean isStopCorrecting_;
	private static boolean isStopResizing_;

	private static short currentSubMenu_;
	private static short nSubMenuButtons_;

	private static int currentFrameHeight_;
	private static int heightCorrection_;
	private static int previousPreferredOutputScrollPaneSize_;
	private static int titleBarHeight_;
	private static int wantedPreferredOutputScrollPaneSize_;
	private static int windowBottomHeight_;

	private static String inputString_;
	private static String errorHeaderString_;
	private static StringBuffer errorStringBuffer_;

	private static WordItem currentInterfaceLanguageWordItem_;

	// Private static components - Output area
	private static JScrollPane outputScrollPane_;
	private static JTextArea outputArea_;

	// Private static components - Panels
	private static JPanel upperMenuPanel_;
	private static JPanel mainMenuPanel_;
	private static JPanel subMenuPanel_;
	private static JPanel inputPanel_;

	// Private static components - Upper menu
	private static JButton upperMenuClearYourMindButton_;
	private static JButton upperMenuRestartButton_;
	private static JButton upperMenuUndoButton_;
	private static JButton upperMenuRedoButton_;
	private static JButton upperMenuLoginAsExpertButton_;
	private static JButton upperMenuMoreExamplesButton_;

	private static JLabel upperMenuProgressLabel_;
	private static JProgressBar upperMenuProgressBar_;

	private static JFileChooser fileChooser_;

	// Private static components - Main menu
	private static JButton mainMenuAmbiguitySubMenuButton_;
	private static JButton mainMenuReadTheFileAmbiguityBostonButton_;
	private static JButton mainMenuReadTheFileAmbiguityPresidentsButton_;

	private static JButton mainMenuProgrammingSubMenuButton_;
	private static JButton mainMenuReadTheFileProgrammingConnect4Button_;
	private static JButton mainMenuReadTheFileProgrammingTowerOfHanoiButton_;

	private static JButton mainMenuReasoningSubMenuButton_;
	private static JButton mainMenuACarHasAnEngineButton_;
	private static JButton mainMenuJohnWasTheFatherOfPaulButton_;
	private static JButton mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_;

	private static JButton mainMenuReadTheFileReasoningFamilyButton_;
	private static JButton mainMenuFamilyDefinitionsSubMenuButton_;
	private static JButton mainMenuFamilyConflictsSubMenuButton_;
	private static JButton mainMenuFamilyJustificationSubMenuButton_;
	private static JButton mainMenuFamilyQuestionsSubMenuButton_;
	private static JButton mainMenuFamilyShowInformationSubMenuButton_;

	private static JButton mainMenuBackButton_;
	private static JButton mainMenuChangeLanguageButton_;
	private static JButton mainMenuHelpButton_;

	// Private static components - Sub menus
	private static JLabel initLabel_;

	private static JButton[] subMenuButtonArray_ = new JButton[Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS];

	// Private static components - input area
	private static JTextField inputField_;
	private static JPasswordField passwordField_;


	// Private static methods

	private static void addSubMenuButton( short interfaceParameter )
		{
		String interfaceString;

		if( nSubMenuButtons_ >= 0 &&
		nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS &&
		currentInterfaceLanguageWordItem_ != null &&
		( interfaceString = currentInterfaceLanguageWordItem_.interfaceString( interfaceParameter ) ) != null )
			{
			if( !interfaceString.equals( Constants.INTERFACE_STRING_NOT_AVAILABLE ) )
				subMenuButtonArray_[nSubMenuButtons_++].setText( interfaceString );
			}
		}

	private static void enableMenus( boolean isEnableNormalButtons, boolean enableSubMenuButtons )
		{
		boolean isChangeLanguage = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE );
		boolean isHelp = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_HELP );
		boolean isInit = ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_INIT );
		
		// Upper menu
		upperMenuClearYourMindButton_.setEnabled( isEnableNormalButtons );
		upperMenuRestartButton_.setEnabled( isEnableNormalButtons );
		upperMenuUndoButton_.setEnabled( isEnableNormalButtons );
		upperMenuRedoButton_.setEnabled( isEnableNormalButtons );
		upperMenuLoginAsExpertButton_.setEnabled( isEnableNormalButtons && !Presentation.isExpertUser() );
		upperMenuMoreExamplesButton_.setEnabled( isEnableNormalButtons );

		// Main menu
		mainMenuAmbiguitySubMenuButton_.setEnabled( isEnableNormalButtons );
		setButtonText( ( isChangeLanguage || isHelp || isInit ), Constants.INTERFACE_CONSOLE_MAIN_MENU_AMBIGUITY_SUBMENU, mainMenuAmbiguitySubMenuButton_ );

		mainMenuProgrammingSubMenuButton_.setEnabled( isEnableNormalButtons );
		setButtonText( ( isChangeLanguage || isHelp || isInit ), Constants.INTERFACE_CONSOLE_MAIN_MENU_PROGRAMMING_SUBMENU, mainMenuProgrammingSubMenuButton_ );

		mainMenuReasoningSubMenuButton_.setEnabled( isEnableNormalButtons );
		setButtonText( ( isChangeLanguage || isHelp || isInit ), Constants.INTERFACE_CONSOLE_MAIN_MENU_REASONING_SUBMENU, mainMenuReasoningSubMenuButton_ );

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_AMBIGUITY )
			{
			mainMenuReadTheFileAmbiguityBostonButton_.setEnabled( hasSelectedAmbiguityBoston_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_BOSTON, mainMenuReadTheFileAmbiguityBostonButton_ );

			mainMenuReadTheFileAmbiguityPresidentsButton_.setEnabled( hasSelectedAmbiguityPresidents_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_PRESIDENTS, mainMenuReadTheFileAmbiguityPresidentsButton_ );
			}
		else
			{
			mainMenuReadTheFileAmbiguityBostonButton_.setVisible( false );
			mainMenuReadTheFileAmbiguityPresidentsButton_.setVisible( false );
			}

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING_CONNECT4 ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING_GREETING ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_PROGRAMMING_TOWER_OF_HANOI )
			{
			mainMenuReadTheFileProgrammingConnect4Button_.setEnabled( hasSelectedProgrammingConnect4_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_CONNECT4, mainMenuReadTheFileProgrammingConnect4Button_ );

			mainMenuReadTheFileProgrammingTowerOfHanoiButton_.setEnabled( hasSelectedProgrammingTowerOfHanoi_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_TOWER_OF_HANOI, mainMenuReadTheFileProgrammingTowerOfHanoiButton_ );
			}
		else
			{
			mainMenuReadTheFileProgrammingConnect4Button_.setVisible( false );
			mainMenuReadTheFileProgrammingTowerOfHanoiButton_.setVisible( false );
			}

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING )
			{
			mainMenuACarHasAnEngineButton_.setEnabled( hasSelectedReasoningACarHasAnEngine ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_A_CAR_HAS_AN_ENGINE, mainMenuACarHasAnEngineButton_ );

			mainMenuJohnWasTheFatherOfPaulButton_.setEnabled( hasSelectedReasoningJohnWasTheFatherOfPaul_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_JOHN_WAS_THE_FATHER_OF_PAUL, mainMenuJohnWasTheFatherOfPaulButton_ );

			mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_.setEnabled( hasSelectedReasoningReadTheFileOnlyOptionLeftBoat_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_READ_THE_FILE_REASONING_ONLY_OPTION_LEFT_BOAT, mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_ );
			}
		else
			{
			mainMenuACarHasAnEngineButton_.setVisible( false );
			mainMenuJohnWasTheFatherOfPaulButton_.setVisible( false );
			mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_.setVisible( false );
			}

		if( currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS ||
		currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_SHOW_INFORMATION )
			{
			mainMenuReadTheFileReasoningFamilyButton_.setEnabled( hasSelectedReasoningFamily_ ? false : isEnableNormalButtons );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_REASONING_FAMILY, mainMenuReadTheFileReasoningFamilyButton_ );

			mainMenuFamilyDefinitionsSubMenuButton_.setEnabled( hasSelectedReasoningFamily_ && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS ? isEnableNormalButtons : false );
			setButtonText( hasSelectedReasoningFamily_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SUBMENU, mainMenuFamilyDefinitionsSubMenuButton_ );

			mainMenuFamilyConflictsSubMenuButton_.setEnabled( hasSelectedReasoningFamily_ && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS ? isEnableNormalButtons : false );
			setButtonText( hasSelectedReasoningFamily_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_CONFLICT_SUBMENU, mainMenuFamilyConflictsSubMenuButton_ );

			mainMenuFamilyJustificationSubMenuButton_.setEnabled( hasSelectedReasoningFamily_ && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT ? isEnableNormalButtons : false );
			setButtonText( hasSelectedReasoningFamily_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_JUSTIFICATION_REPORT_SUBMENU, mainMenuFamilyJustificationSubMenuButton_ );

			mainMenuFamilyQuestionsSubMenuButton_.setEnabled( hasSelectedReasoningFamily_ && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS ? isEnableNormalButtons : false );
			setButtonText( hasSelectedReasoningFamily_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_QUESTION_SUBMENU, mainMenuFamilyQuestionsSubMenuButton_ );

			mainMenuFamilyShowInformationSubMenuButton_.setEnabled( hasSelectedReasoningFamily_ && currentSubMenu_ != Constants.CONSOLE_SUBMENU_REASONING_FAMILY_SHOW_INFORMATION ? isEnableNormalButtons : false );
			setButtonText( hasSelectedReasoningFamily_, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SHOW_INFO_SUBMENU, mainMenuFamilyShowInformationSubMenuButton_ );
			}
		else
			{
			mainMenuReadTheFileReasoningFamilyButton_.setVisible( false );

			mainMenuFamilyDefinitionsSubMenuButton_.setVisible( false );
			mainMenuFamilyConflictsSubMenuButton_.setVisible( false );
			mainMenuFamilyJustificationSubMenuButton_.setVisible( false );
			mainMenuFamilyQuestionsSubMenuButton_.setVisible( false );
			mainMenuFamilyShowInformationSubMenuButton_.setVisible( false );
			}

		mainMenuBackButton_.setEnabled( isEnableNormalButtons );

		setButtonText( ( currentSubMenu_ != Constants.CONSOLE_SUBMENU_INIT && currentSubMenu_ != Constants.CONSOLE_SUBMENU_HELP ), Constants.INTERFACE_CONSOLE_MAIN_MENU_BACK, mainMenuBackButton_ );
		setButtonText( ( CommonVariables.interfaceLanguageStringBuffer == null || currentSubMenu_ == Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE ? false : isEnableNormalButtons ), Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_LANGUAGE, mainMenuChangeLanguageButton_ );
		setButtonText( isEnableNormalButtons, Constants.INTERFACE_CONSOLE_MAIN_MENU_HELP, mainMenuHelpButton_ );

		// Sub menu
		if( currentSubMenu_ != Constants.CONSOLE_SUBMENU_INIT &&
		nSubMenuButtons_ <= Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS )
			{
			for( short index = 0; index < nSubMenuButtons_; index++ )
				subMenuButtonArray_[index].setEnabled( enableSubMenuButtons );
			}

		inputField_.setEnabled( isEnableNormalButtons && Presentation.isExpertUser() );
		inputField_.requestFocus();
		}

	private static void goToEndOfOutputDocument()
		{
		Document document;

		if( ( document = outputArea_.getDocument() ) != null )
			{
			outputArea_.setCaretPosition( 0 );
			// Go to end of text in output area
			outputArea_.setCaretPosition( document.getLength() );
			}
		}

	private static void resizeFrame()
		{
		int difference;
		int buttonPaneSize = ( titleBarHeight_ + mainMenuPanel_.getHeight() + subMenuPanel_.getHeight() + windowBottomHeight_ + heightCorrection_ + ( 2 * Constants.CONSOLE_BUTTON_PANE_HEIGHT ) );
		int preferredOutputScrollPaneSize = ( currentFrameHeight_ - buttonPaneSize );

		if( !isStopResizing_ &&
		nSubMenuButtons_ > 0 &&
		nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS &&
		subMenuButtonArray_[nSubMenuButtons_].isVisible() )
			nSubMenuButtons_++;

		difference = preferredOutputScrollPaneSize - wantedPreferredOutputScrollPaneSize_;

		if( difference == 0 )
			{
			isStopCorrecting_ = true;

			if( nSubMenuButtons_ > 1 &&
			outputScrollPane_.getHeight() < buttonPaneSize )
				{
				isStopResizing_ = true;
				nSubMenuButtons_--;
				}
			}
		else
			{
			if( isActionPerformed_ )
				{
				wantedPreferredOutputScrollPaneSize_ = preferredOutputScrollPaneSize;
				goToEndOfOutputDocument();
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
				else
					{
					if( !isStopCorrecting_ &&
					heightCorrection_ < Constants.CONSOLE_BUTTON_PANE_HEIGHT &&
					previousPreferredOutputScrollPaneSize_ > preferredOutputScrollPaneSize )
						heightCorrection_++;
					}

				previousPreferredOutputScrollPaneSize_ = preferredOutputScrollPaneSize;
				}
			}

		mainMenuPanel_.setPreferredSize( new Dimension( 0, ( Constants.CONSOLE_BUTTON_PANE_HEIGHT + mainMenuHelpButton_.getY() ) ) );
		subMenuPanel_.setPreferredSize( new Dimension( 0, ( Constants.CONSOLE_BUTTON_PANE_HEIGHT + ( nSubMenuButtons_ > 0 && nSubMenuButtons_ <= Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS ? subMenuButtonArray_[nSubMenuButtons_ - 1].getY() : 0 ) ) ) );
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
		String interfaceString = ( currentInterfaceLanguageWordItem_ != null ? currentInterfaceLanguageWordItem_.interfaceString( interfaceParameter ) : null );

		if( menuButton != null )
			{
			isButtonInvisible = ( isInvisible &&
								interfaceString != null &&
								!interfaceString.equals( Constants.INTERFACE_STRING_NOT_AVAILABLE ) );

			menuButton.setVisible( isButtonInvisible );

			if( isButtonInvisible )
				menuButton.setText( interfaceString );
			}
		}

	private static void setInterfaceLanguage()
		{
		if( CommonVariables.currentInterfaceLanguageWordItem != null &&
		CommonVariables.currentInterfaceLanguageWordItem != currentInterfaceLanguageWordItem_ )
			{
			currentInterfaceLanguageWordItem_ = CommonVariables.currentInterfaceLanguageWordItem;
			
			// Upper panel buttons
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_CLEAR_YOUR_MIND, upperMenuClearYourMindButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_RESTART, upperMenuRestartButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_UNDO, upperMenuUndoButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_REDO, upperMenuRedoButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_LOGIN_AS_EXPERT, upperMenuLoginAsExpertButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_UPPER_MENU_MORE_EXAMPLES, upperMenuMoreExamplesButton_ );

			// Main menu buttons
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_AMBIGUITY_SUBMENU, mainMenuAmbiguitySubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_BOSTON, mainMenuReadTheFileAmbiguityBostonButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_AMBIGUITY_PRESIDENTS, mainMenuReadTheFileAmbiguityPresidentsButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_PROGRAMMING_SUBMENU, mainMenuProgrammingSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_CONNECT4, mainMenuReadTheFileProgrammingConnect4Button_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_TOWER_OF_HANOI, mainMenuReadTheFileProgrammingTowerOfHanoiButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_REASONING_SUBMENU, mainMenuReasoningSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_A_CAR_HAS_AN_ENGINE, mainMenuACarHasAnEngineButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_JOHN_WAS_THE_FATHER_OF_PAUL, mainMenuJohnWasTheFatherOfPaulButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_REASONING_READ_THE_FILE_REASONING_ONLY_OPTION_LEFT_BOAT, mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_REASONING_FAMILY, mainMenuReadTheFileReasoningFamilyButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SUBMENU, mainMenuFamilyDefinitionsSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_CONFLICT_SUBMENU, mainMenuFamilyConflictsSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_JUSTIFICATION_REPORT_SUBMENU, mainMenuFamilyJustificationSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_QUESTION_SUBMENU, mainMenuFamilyQuestionsSubMenuButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SHOW_INFO_SUBMENU, mainMenuFamilyShowInformationSubMenuButton_ );

			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_BACK, mainMenuBackButton_ );
			setButtonText( ( CommonVariables.interfaceLanguageStringBuffer != null ), Constants.INTERFACE_CONSOLE_MAIN_MENU_CHANGE_LANGUAGE, mainMenuChangeLanguageButton_ );
			setButtonText( true, Constants.INTERFACE_CONSOLE_MAIN_MENU_HELP, mainMenuHelpButton_ );

			// Sub menu
			setSubMenuButtonTexts();
			}
		}

	private static void setSubMenuButtonTexts()
		{
		int grammarLanguageStringBufferLength;
		int startPosition;
		int endPosition;
		String currentLanguageString;
		String newLanguageString;

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

				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_WOMAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_MAN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_WOMAN );
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

			case Constants.CONSOLE_SUBMENU_REASONING_FAMILY_SHOW_INFORMATION:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_FAMILY );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_PARENT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_CHILD );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_FATHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_MOTHER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_SON );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_DAUGHTER );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_JOHN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_ANN );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_PAUL );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_JOE );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_REASONING_FAMILY_SHOW_INFORMATION_ABOUT_LAURA );
				break;

			case Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE:
				if( CommonVariables.interfaceLanguageStringBuffer != null )
					{
					startPosition = 0;
					endPosition = 0;
					grammarLanguageStringBufferLength = CommonVariables.interfaceLanguageStringBuffer.length();

					if( currentInterfaceLanguageWordItem_ != null &&
					( currentLanguageString = currentInterfaceLanguageWordItem_.anyWordTypeString() ) != null )
						{
						while( startPosition < grammarLanguageStringBufferLength &&
						nSubMenuButtons_ < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS )
							{
							if( ( endPosition = CommonVariables.interfaceLanguageStringBuffer.substring( startPosition ).indexOf( Constants.QUERY_SEPARATOR_SPACE_STRING ) ) < 0 )
								endPosition = grammarLanguageStringBufferLength;			// Last language in string
							else
								endPosition += startPosition;
	
							if( ( newLanguageString = CommonVariables.interfaceLanguageStringBuffer.substring( startPosition, endPosition ) ) != null )
								{
								if( !newLanguageString.equals( currentLanguageString ) )	// Skip current language
									subMenuButtonArray_[nSubMenuButtons_++].setText( Constants.CHANGE_LANGUAGE_STRING + newLanguageString );
								}
	
							startPosition = endPosition + Constants.QUERY_SEPARATOR_SPACE_STRING.length();
							}
						}
					}

				break;

			case Constants.CONSOLE_SUBMENU_HELP:
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_GRAMMAR_LANGUAGES );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_INTERFACE_LANGUAGES );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_LISTS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_USERS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_INFO_ABOUT_THE_WORD_TYPES );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_QUERY_COMMANDS );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_COPYRIGHT );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_GPLv2_LICENSE );
				addSubMenuButton( Constants.INTERFACE_CONSOLE_HELP_SHOW_THE_WARRANTY );
				break;
			}
		}

	private static void setSubMenuVisible( boolean isSkipChangingCurrentSubMenuSetting, short subMenu )
		{
		if( !isSkipChangingCurrentSubMenuSetting )
			currentSubMenu_ = subMenu;

		setSubMenuButtonTexts();
		showProgressStatus( null );

		initLabel_.setVisible( nSubMenuButtons_ == 0 );

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuButtonArray_[index].setVisible( index < nSubMenuButtons_ );
		}

	private static void setUpperMenuVisible( boolean isVisible )
		{
		upperMenuClearYourMindButton_.setVisible( isVisible );
		upperMenuRestartButton_.setVisible( isVisible );
		upperMenuUndoButton_.setVisible( isVisible );
		upperMenuRedoButton_.setVisible( isVisible );
		upperMenuLoginAsExpertButton_.setVisible( isVisible );
		upperMenuMoreExamplesButton_.setVisible( isVisible );
		}

	private static void showConnectFourInterferenceNotification()
		{
		JTextArea errorTextArea = new JTextArea( "\n\tThe Connect-Four playing rules are still present in the system.\n\tThey may interfere with the current action. My advice:\n\tClear the Connect-Four playing rules from the system first,\n\tby using \"Clear your mind.\" or \"Restart.\". Then select this action again." );
		errorTextArea.setEditable( false );
		JScrollPane errorScrollPane = new JScrollPane( errorTextArea );
		errorScrollPane.setPreferredSize( new Dimension( Constants.CONSOLE_CONNECT_FOUR_INTERFERENCE_PANE_WIDTH, Constants.CONSOLE_CONNECT_FOUR_INTERFERENCE_PANE_HEIGHT ) );
		JOptionPane.showMessageDialog( null, errorScrollPane, Constants.CONSOLE_CONNECT_FOUR_INTERFERENCE_MESSAGE_STRING, JOptionPane.ERROR_MESSAGE );			
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

	public Console()
		{
		super( new GridBagLayout() );

		Border border = BorderFactory.createEmptyBorder( Constants.CONSOLE_BORDER_SIZE, Constants.CONSOLE_BORDER_SIZE, Constants.CONSOLE_BORDER_SIZE, Constants.CONSOLE_BORDER_SIZE );

		// Create output area
		outputArea_ = new JTextArea();
		outputArea_.setEditable( false );
		outputArea_.setLineWrap( true );
		outputArea_.setWrapStyleWord( true );

		// Set a mono-spaced (= non-proportional) font for output area
		outputArea_.setFont( new Font( Constants.CONSOLE_MONO_SPACED_FONT, Font.PLAIN, Constants.CONSOLE_FONT_SIZE ) );

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

		// Create Sub menu panel with border
		subMenuPanel_ = new JPanel();
		subMenuPanel_.setBorder( BorderFactory.createCompoundBorder( border, subMenuPanel_.getBorder() ) );

		// Create Input panel with border
		inputPanel_ = new JPanel();
		inputPanel_.setLayout( new GridBagLayout() );
		inputPanel_.setPreferredSize( new Dimension( 0, Constants.CONSOLE_BUTTON_PANE_HEIGHT ) );
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

		upperMenuMoreExamplesButton_ = new JButton();
		upperMenuMoreExamplesButton_.addActionListener( this );

		// Create the file chooser
		fileChooser_ = new JFileChooser( CommonVariables.currentPathStringBuffer + Constants.FILE_EXAMPLES_DIRECTORY_NAME_STRING );

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

		mainMenuACarHasAnEngineButton_ = new JButton();
		mainMenuACarHasAnEngineButton_.addActionListener( this );

		mainMenuJohnWasTheFatherOfPaulButton_ = new JButton();
		mainMenuJohnWasTheFatherOfPaulButton_.addActionListener( this );

		mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_ = new JButton();
		mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_.addActionListener( this );

		mainMenuReadTheFileReasoningFamilyButton_ = new JButton();
		mainMenuReadTheFileReasoningFamilyButton_.addActionListener( this );

		mainMenuFamilyDefinitionsSubMenuButton_ = new JButton();
		mainMenuFamilyDefinitionsSubMenuButton_.addActionListener( this );

		mainMenuFamilyConflictsSubMenuButton_ = new JButton();
		mainMenuFamilyConflictsSubMenuButton_.addActionListener( this );

		mainMenuFamilyJustificationSubMenuButton_ = new JButton();
		mainMenuFamilyJustificationSubMenuButton_.addActionListener( this );

		mainMenuFamilyQuestionsSubMenuButton_ = new JButton();
		mainMenuFamilyQuestionsSubMenuButton_.addActionListener( this );

		mainMenuFamilyShowInformationSubMenuButton_ = new JButton();
		mainMenuFamilyShowInformationSubMenuButton_.addActionListener( this );

		mainMenuBackButton_ = new JButton();
		mainMenuBackButton_.addActionListener( this );

		mainMenuChangeLanguageButton_ = new JButton();
		mainMenuChangeLanguageButton_.addActionListener( this );

		mainMenuHelpButton_ = new JButton();
		mainMenuHelpButton_.addActionListener( this );

		// Create of Sub menu
		initLabel_ = new JLabel();

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuButtonArray_[index] = new JButton();

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuButtonArray_[index].addActionListener( this );

		// Create Input field
		inputField_ = new JTextField();
		inputField_.setFont( new Font( Constants.CONSOLE_MONO_SPACED_FONT, Font.PLAIN, 13 ) );	// Set a mono-spaced (= non-proportional) font for Input field
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
		upperMenuPanel_.add( upperMenuMoreExamplesButton_ );

		// Add buttons to Main menu panel
		mainMenuPanel_.add( mainMenuAmbiguitySubMenuButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileAmbiguityBostonButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileAmbiguityPresidentsButton_ );

		mainMenuPanel_.add( mainMenuProgrammingSubMenuButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileProgrammingConnect4Button_ );
		mainMenuPanel_.add( mainMenuReadTheFileProgrammingTowerOfHanoiButton_ );

		mainMenuPanel_.add( mainMenuReasoningSubMenuButton_ );
		mainMenuPanel_.add( mainMenuACarHasAnEngineButton_ );
		mainMenuPanel_.add( mainMenuJohnWasTheFatherOfPaulButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_ );
		mainMenuPanel_.add( mainMenuReadTheFileReasoningFamilyButton_ );
		mainMenuPanel_.add( mainMenuFamilyDefinitionsSubMenuButton_ );
		mainMenuPanel_.add( mainMenuFamilyConflictsSubMenuButton_ );
		mainMenuPanel_.add( mainMenuFamilyJustificationSubMenuButton_ );
		mainMenuPanel_.add( mainMenuFamilyQuestionsSubMenuButton_ );
		mainMenuPanel_.add( mainMenuFamilyShowInformationSubMenuButton_ );

		mainMenuPanel_.add( mainMenuBackButton_ );
		mainMenuPanel_.add( mainMenuChangeLanguageButton_ );
		mainMenuPanel_.add( mainMenuHelpButton_ );

		// Add components to Sub menu panel
		subMenuPanel_.add( initLabel_ );

		for( short index = 0; index < Constants.CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS; index++ )
			subMenuPanel_.add( subMenuButtonArray_[index] );

		// Add Input field to Input panel
		inputPanel_.add( inputField_, remainderOrientedConstraints );
		inputPanel_.add( passwordField_, remainderOrientedConstraints );

		// Add pane and panels to the frame
		add( outputScrollPane_, remainderOrientedConstraints );
		add( upperMenuPanel_, remainderOrientedConstraints );
		add( mainMenuPanel_, remainderOrientedConstraints );
		add( subMenuPanel_, remainderOrientedConstraints );
		add( inputPanel_, remainderOrientedConstraints );
		}


	// Protected static methods

	protected static void restart( int titleBarHeight, int windowBottomHeight )
		{
		// Initialize private static variables
		hasSelectedAmbiguityBoston_ = false;
		hasSelectedAmbiguityPresidents_ = false;

		hasSelectedProgrammingConnect4_ = false;
		hasSelectedProgrammingTowerOfHanoi_ = false;

		hasSelectedReasoningACarHasAnEngine = false;
		hasSelectedReasoningJohnWasTheFatherOfPaul_ = false;
		hasSelectedReasoningReadTheFileOnlyOptionLeftBoat_ = false;
		hasSelectedReasoningFamily_ = false;

		isActionPerformed_ = false;
//		isStopCorrecting_ = false;
		isStopResizing_ = false;

		currentSubMenu_ = Constants.CONSOLE_SUBMENU_INIT;
		nSubMenuButtons_ = 0;

//		currentFrameHeight_ = 0;
		heightCorrection_ = 0;
		previousPreferredOutputScrollPaneSize_ = 0;
		titleBarHeight_ = titleBarHeight;
		wantedPreferredOutputScrollPaneSize_ = 0;
		windowBottomHeight_ = windowBottomHeight;

		errorHeaderString_ = null;
		errorStringBuffer_ = null;

		currentInterfaceLanguageWordItem_ = null;

		// Initialize Sub menus
		setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_INIT );

		// Disable menus
		enableMenus( false, false );

		// Clear screen
		outputArea_.setText( Constants.NEW_LINE_STRING );

		// Clear possible restart sentence in Input field
		inputField_.setText( Constants.EMPTY_STRING );

		// Disable Input field
		inputField_.setEnabled( false );
		}

	protected static void writeText( String textString )
		{
		if( textString != null )
			{
			// Show text in output area
			outputArea_.append( textString );
			goToEndOfOutputDocument();
			}
		else
			{
			addError( "Class Console;\nMethod: writeText;\nError: The given text string is undefined." );
			showError();
			}
		}

	protected static void showProgressStatus( String newStatusString )
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

	protected static void startProgress( int startProgress, int maxProgress, String progressString )
		{
		upperMenuProgressBar_.setValue( startProgress );
		upperMenuProgressBar_.setMaximum( maxProgress );

		showProgressStatus( progressString );
		upperMenuProgressBar_.setVisible( true );
		}

	protected static void showProgress( int currentProgress )
		{
		upperMenuProgressBar_.setValue( currentProgress );
		}

	protected static void clearProgress()
		{
		showProgressStatus( null );
		upperMenuProgressBar_.setVisible( false );
		}

	protected static void showError()
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

	protected static String getPassword()
		{
		if( errorStringBuffer_ != null )
			showError();

		// Prepare password field
		inputField_.setVisible( false );
		passwordField_.setVisible( true );
		// Clear previous password
		passwordField_.setText( Constants.EMPTY_STRING );
		passwordField_.requestFocus();

		inputString_ = getInputString();

		// Hide password field again
		passwordField_.setVisible( false );
		inputField_.setVisible( true );

		return inputString_;
		}

	protected static String readLine( boolean clearInputField )
		{
		if( errorStringBuffer_ != null )
			showError();

		// Set the language of the interface
		setInterfaceLanguage();
		initLabel_.setText( currentInterfaceLanguageWordItem_.interfaceString( Presentation.isExpertUser() ? Constants.INTERFACE_CONSOLE_START_MESSAGE_EXPERT : Constants.INTERFACE_CONSOLE_START_MESSAGE_GUEST ) );

		// Prepare Input field
		inputField_.setEnabled( Presentation.isExpertUser() );
		inputField_.requestFocus();

		if( clearInputField )
			// Select all text in Input field
			inputField_.selectAll();
		else
			{
			// Set Upper menu visible
			setUpperMenuVisible( true );

			// Enable menus again
			enableMenus( true, true );

			// Select all text in Input field
			inputField_.selectAll();
			}

		inputString_ = getInputString();

		// Disable Input field
		inputField_.setEnabled( false );

		return inputString_;
		}


	// Public non-static methods

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

	public void actionPerformed( ActionEvent actionEvent )
		{
		File selectedFile;
		File currentDirectory;
		Object actionSource;
		String actionCommandString;
		String currentDirectoryString;
		String currentLanguageString;
		String selectedFilePath;

		isActionPerformed_ = false;

		if( currentInterfaceLanguageWordItem_ != null )
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

								hasSelectedReasoningACarHasAnEngine = false;
								hasSelectedReasoningJohnWasTheFatherOfPaul_ = false;
								hasSelectedReasoningReadTheFileOnlyOptionLeftBoat_ = false;
								hasSelectedReasoningFamily_ = false;

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
									if( actionSource == upperMenuMoreExamplesButton_ )
										{
										if( hasSelectedProgrammingConnect4_ )
											showConnectFourInterferenceNotification();

										if( ( currentDirectory = fileChooser_.getCurrentDirectory() ) != null )
											{
											if( currentInterfaceLanguageWordItem_ != null &&
											( currentDirectoryString = currentDirectory.toString() ) != null &&
											( currentLanguageString = currentInterfaceLanguageWordItem_.anyWordTypeString() ) != null )
												{
												// No file selected yet with current language
												if( currentDirectoryString.indexOf( currentLanguageString ) < 0 ) 
													// Select current language in file chooser
													fileChooser_ = new JFileChooser( CommonVariables.currentPathStringBuffer + Constants.FILE_EXAMPLES_DIRECTORY_NAME_STRING + currentLanguageString );
												}
											} 

										if( fileChooser_.showOpenDialog( this ) == JFileChooser.APPROVE_OPTION )
											{
											if( ( selectedFile = fileChooser_.getSelectedFile() ) != null )
												{
												if( ( selectedFilePath = selectedFile.getPath() ) != null )
													inputString_ = Presentation.convertDiacriticalText( currentInterfaceLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_START ) ) + selectedFilePath + Presentation.convertDiacriticalText( currentInterfaceLanguageWordItem_.interfaceString( Constants.INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_END ) );
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
																if( hasSelectedProgrammingConnect4_ )
																	showConnectFourInterferenceNotification();

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
																	if( actionSource == mainMenuACarHasAnEngineButton_ )
																		{
																		hasSelectedReasoningACarHasAnEngine = true;
																		inputString_ = actionCommandString;
																		}
																	else
																		{
																		if( actionSource == mainMenuJohnWasTheFatherOfPaulButton_ )
																			{
																			hasSelectedReasoningJohnWasTheFatherOfPaul_ = true;
																			inputString_ = actionCommandString;
																			}
																		else
																			{
																			if( actionSource == mainMenuReadTheFileReasoningOnlyOptionLeftBoatButton_ )
																				{
																				hasSelectedReasoningReadTheFileOnlyOptionLeftBoat_ = true;
																				inputString_ = actionCommandString;
																				}
																			else
																				{
																				if( actionSource == mainMenuReadTheFileReasoningFamilyButton_ )
																					{
																					if( hasSelectedProgrammingConnect4_ )
																						showConnectFourInterferenceNotification();

																					hasSelectedReasoningFamily_ = true;
																					setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS );
																					inputString_ = actionCommandString;
																					}
																				else
																					{
																					if( actionSource == mainMenuFamilyDefinitionsSubMenuButton_ )
																						{
																						setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS );
																						enableMenus( true, true );
																						}
																					else
																						{
																						if( actionSource == mainMenuFamilyConflictsSubMenuButton_ )
																							{
																							setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS );
																							enableMenus( true, true );
																							}
																						else
																							{
																							if( actionSource == mainMenuFamilyJustificationSubMenuButton_ )
																								{
																								setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT );
																								enableMenus( true, true );
																								}
																							else
																								{
																								if( actionSource == mainMenuFamilyQuestionsSubMenuButton_ )
																									{
																									setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS );
																									enableMenus( true, true );
																									}
																								else
																									{
																									if( actionSource == mainMenuFamilyShowInformationSubMenuButton_ )
																										{
																										setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_REASONING_FAMILY_SHOW_INFORMATION );
																										enableMenus( true, true );
																										}
																									else
																										{
																										if( actionSource == mainMenuBackButton_ )
																											{
																											setSubMenuVisible( false, ( currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS || currentSubMenu_ == Constants.CONSOLE_SUBMENU_REASONING_FAMILY_SHOW_INFORMATION ? Constants.CONSOLE_SUBMENU_REASONING : Constants.CONSOLE_SUBMENU_INIT ) );
																											enableMenus( true, true );
																											}
																										else
																											{
																											if( actionSource == mainMenuChangeLanguageButton_ )
																												{
																												setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_CHANGE_LANGUAGE );
																												enableMenus( true, true );
																												}
																											else
																												{
																												if( actionSource == mainMenuHelpButton_ )
																													setSubMenuVisible( false, Constants.CONSOLE_SUBMENU_HELP );
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

						if( inputString_ != null )
							{
							if( actionSource != inputField_ )
								inputField_.setText( inputString_ );

							if( actionSource != upperMenuRestartButton_ )
								writeText( inputString_ + Constants.NEW_LINE_STRING );
							}
						}

					isActionPerformed_ = true;
					}
				else
					{
					addError( "Class Console;\nMethod: actionPerformed;\nError: The action command string is undefined." );
					showError();
					}
				}
			else
				{
				addError( "Class Console;\nMethod: actionPerformed;\nError: The action source is undefined." );
				showError();
				}
			}
		else
			{
			addError( "Class Console;\nMethod: actionPerformed;\nError: The current interface language word is undefined." );
			showError();
			}
		}
	};

/*************************************************************************
 *	"I will love the Lord because he hears my voice
 *	and my prayer for mercy.
 *	Because he bends down to listen,
 *	I will pray as long as I breath!" (Psalm 116:1-2)
 *************************************************************************/
