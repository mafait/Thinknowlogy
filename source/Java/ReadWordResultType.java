/*	Class:		ReadWordResultType
 *	Purpose:	To return read word variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2018r2 (Natural Intelligence)
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

class ReadWordResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundGrammarDefinition;

	protected int offset;
	protected int startWordPosition;
	protected int wordLength;

	// Constructor

	ReadWordResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundGrammarDefinition = false;

		offset = 0;
		startWordPosition = 0;
		wordLength = 0;
		}
	};

/*************************************************************************
 *	"As a deer longs for streams of water,
 *	so I long for you, O God.
 *	I thirst for God, the living God.
 *	When can I go and stand before him?" (Psalm 42:1-2)
 *************************************************************************/
