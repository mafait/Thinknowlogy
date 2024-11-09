﻿/*	Class:		RelatedResultType
 *	Purpose:	To return related specification variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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

class RelatedResultType
	{
	// Protected variables

	protected byte result = Constants.RESULT_OK;

	protected boolean isFirstRelatedSpecification = false;
	protected boolean isLastRelatedSpecification = false;

	protected SpecificationItem relatedSpecificationItem = null;
	}

/*************************************************************************
 *	"He gives food to those who fear him;
 *	he always remembers his covenant." (Psalm 111:5)
 *************************************************************************/
