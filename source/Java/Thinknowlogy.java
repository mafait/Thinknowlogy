/*	Class:		Thinknowlogy
 *	Purpose:	Main class of the Thinknowlogy knowledge technology
 *	Version:	Thinknowlogy 2016r1 (Huguenot)
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

import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Toolkit;
import javax.swing.JFrame;

class Thinknowlogy
	{
	// Private static variables

	private static int titleBarHeight_;
	private static int windowBottomHeight_;
	
	// Private static methods

	private static void deriveCurrentPath()
		{
		int lastIndex;
		String absolutePath = Thinknowlogy.class.getProtectionDomain().getCodeSource().getLocation().getPath();

		if( absolutePath.endsWith( Constants.FILE_SOURCE_DIRECTORY_NAME_STRING ) )
			lastIndex = absolutePath.length() - Constants.FILE_SOURCE_DIRECTORY_NAME_STRING.length() - 1;
		else
			{
			if( ( lastIndex = ( absolutePath.lastIndexOf( Constants.SLASH_STRING ) ) ) < 0 )
				lastIndex = absolutePath.lastIndexOf( Constants.BACK_SLASH_STRING );
			}

		// If no absolute path is given - add it
		CommonVariables.currentPathStringBuffer = new StringBuffer( lastIndex < 0 ? absolutePath : absolutePath.substring( 0, lastIndex + 1 ) );

		// Replace all '%20' by spaces
		while( ( lastIndex = CommonVariables.currentPathStringBuffer.indexOf( "%20" ) ) > 0 )
			CommonVariables.currentPathStringBuffer.replace( lastIndex, lastIndex + 3, " " );
		}

	private static void createAndShowGUI()
		{
		// Setup screen and frame size for console
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension frameSize = new Dimension( ( screenSize.width > Constants.CONSOLE_MAXIMUM_FRAME_WIDTH_AT_STARTUP ? Constants.CONSOLE_MAXIMUM_FRAME_WIDTH_AT_STARTUP : screenSize.width ), ( screenSize.height - Constants.CONSOLE_TOP_BOTTOM_SPACE ) );

		// Create and set up the window.
		JFrame frame = new JFrame( Constants.PRODUCT_NAME + Constants.SPACE_STRING + Constants.VERSION_NAME );
		frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );

		// Set the initial position of the frame on the screen
		frame.setBounds( ( ( screenSize.width - frameSize.width ) / 2 ),
						( ( screenSize.height - frameSize.height ) / 2 ),
						frameSize.width,
						frameSize.height );

		// Create and set up the content pane.
		Console console = new Console();

		frame.add( console );
		frame.addComponentListener( console );
		frame.setVisible( true );
		Insets insets = frame.getInsets();
		titleBarHeight_ = insets.top;
		windowBottomHeight_ = insets.bottom;
		}

	private static void runAdmin()
		{
		byte result = Constants.RESULT_OK;
		boolean hasRequestedRestart = false;
		String startupLanguageNameString = null;
		AdminItem adminItem;

		do	{
			// Start the administrator
			Console.restart( titleBarHeight_, windowBottomHeight_ );
			adminItem = new AdminItem( startupLanguageNameString );

			// Interact with the administrator
			// until a restart or a system error occurs
			do	{
				result = adminItem.interact();
				hasRequestedRestart = adminItem.hasRequestedRestart();
				}
			while( result == Constants.RESULT_OK &&
			!hasRequestedRestart );

			if( result == Constants.RESULT_OK )
				startupLanguageNameString = new String( adminItem.currentLanguageNameString() );
			}
		while( result == Constants.RESULT_OK &&
		hasRequestedRestart );

		if( result != Constants.RESULT_OK )
			{
			Console.showProgressStatus( "System error: The administrator failed to start." );
			Console.showError();
			}
		}


	// Public static methods

	public static void main(String [] args)
		{
		deriveCurrentPath();
		createAndShowGUI();
		runAdmin();
		}
	};

/*************************************************************************
 *	"Because the Sovereign Lord helps me,
 *	I will not be disgraced.
 *	Therefore, I have set my face like a stone,
 *	determined to do his will.
 *	And I know that I will not be put to shame." (Psalm 50:7)
 *************************************************************************/
