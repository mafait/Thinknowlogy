/*	Class:			InterfaceItem
 *	Parent class:	Item
 *	Purpose:		To store info about the user-interface messages
 *					in the available languages
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

class InterfaceItem extends Item
	{
	// Private initialized variables

	private short interfaceParameter_ = Constants.NO_INTERFACE_PARAMETER;

	private String interfaceString_ = null;


	// Constructor

	protected InterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		interfaceParameter_ = interfaceParameter;

		// Checking private initialized variables

		if( interfaceString != null )
			interfaceString_ = interfaceString.substring( 0, interfaceStringLength );
		else
			startSystemError( 1, null, null, "The given interface string is undefined" );
		}


	// Protected virtual methods

	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( interfaceString_ != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( interfaceString_ );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( interfaceParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				interfaceParameter_ > Constants.NO_INTERFACE_PARAMETER ) );
		}

	protected String itemString()
		{
		return interfaceString_;
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( interfaceParameter_ > Constants.NO_INTERFACE_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "interfaceParameter:" + interfaceParameter_ );

		if( interfaceString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "interfaceString:" + Constants.QUERY_STRING_START_CHAR + interfaceString_ + Constants.QUERY_STRING_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected short interfaceParameter()
		{
		return interfaceParameter_;
		}

	protected String interfaceString()
		{
		return interfaceString_;
		}

	protected InterfaceItem nextInterfaceItem()
		{
		return (InterfaceItem)nextItem;
		}
	};

/*************************************************************************
 *	"O Lord, what are human beings that you should notice them,
 *	mere mortals that you should think about them?" (Psalm 144:3)
 *************************************************************************/
