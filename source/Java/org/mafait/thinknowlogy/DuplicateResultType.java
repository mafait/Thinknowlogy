﻿/*	Class:		DuplicateResultType
 *	Purpose:	To return duplicate condition variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

package org.mafait.thinknowlogy;

class DuplicateResultType
	{
	// Protected variables

	protected byte result = Constants.RESULT_OK;
	protected boolean hasFoundDuplicateSelection = false;
	protected int duplicateConditionSentenceNr = Constants.NO_SENTENCE_NR;
	}

/*************************************************************************
 *	"The Lords protects them
 *	and keeps them alive.
 *	and rescues them from their enemies.
 *	The Lord nurses them when they are sick
 *	and restores them to health." (Psalm 41:2-3)
 *************************************************************************/
