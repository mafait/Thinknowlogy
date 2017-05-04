/*	Class:		Thinknowlogy
 *	Purpose:	Main class of the Thinknowlogy knowledge technology
 *	Version:	Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#ifdef linux
// Necessary to make a Linux native C++ version
#include <stdlib.h>
#endif
#include "AdminItem.h"

int main()
	{
	signed char result = RESULT_OK;
	bool isRestart = false;
	char startupLanguageNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	AdminItem *adminItem = NULL;

	do	{
		// Start the administrator
		if( ( adminItem = new AdminItem( startupLanguageNameString ) ) != NULL )
			{
			// Interact with the administrator
			// until a restart or a system error occurs
			do	{
				result = adminItem->interact();
				isRestart = adminItem->hasRequestedRestart();
				}
			while( result == RESULT_OK &&
			!isRestart );

			if( result == RESULT_OK )
				strcpy( startupLanguageNameString, adminItem->currentLanguageNameString() );

			delete adminItem;
			}
		else
			{
			fprintf( stderr, "\n\nFunction:\tmain\nError:\t\tI failed to create the administrator.\n" );
			result = RESULT_SYSTEM_ERROR;
			}
		}
	while( result == RESULT_OK &&
	isRestart );

	// Don't close the console application on startup error,
	// because the user needs to read the error
	if( result != RESULT_OK )
		// MS Visual Studio
#ifdef _MSC_VER
		// If your compiler has problems with this statement,
		// try the next line or remove these few lines
		system( "pause" );
#else
		// Bad alternative
		while( true );
#endif

	return result;
	}

/*************************************************************************
 *	"Because the Sovereign Lord helps me,
 *	I will not be disgraced.
 *	Therefore, I have set my face like a stone,
 *	determined to do his will.
 *	And I know that I will not be put to shame." (Isaiah 50:7)
 *************************************************************************/
