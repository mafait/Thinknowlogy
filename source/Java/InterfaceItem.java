/*
 *	Class:			InterfaceItem
 *	Parent class:	Item
 *	Purpose:		To store info about the interface messages
 *					in a certain language that can be shown to the user
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

class InterfaceItem extends Item
	{
	// Private loadable variables

	private short interfaceParameter_;

	private String interfaceString_;


	// Constructor / deconstructor

	protected InterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		interfaceParameter_ = interfaceParameter;
		interfaceString_ = null;

		if( interfaceString != null )
			interfaceString_ = interfaceString.substring( 0, interfaceStringLength );
		else
			startSystemError( 1, null, null, "The given interface string is undefined" );
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( interfaceString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( interfaceString_ );
			}
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( interfaceParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				interfaceParameter_ > Constants.NO_INTERFACE_PARAMETER ) );
		}

	protected String itemString()
		{
		return interfaceString_;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		baseToStringBuffer( queryWordTypeNr );

		if( interfaceParameter_ > Constants.NO_INTERFACE_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "interfaceParameter:" + interfaceParameter_ );

			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "interfaceString:" + Constants.QUERY_STRING_START_CHAR + interfaceString_ + Constants.QUERY_STRING_END_CHAR );

		return CommonVariables.queryStringBuffer;
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
