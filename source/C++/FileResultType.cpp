/*	Class:		FileResultType
 *	Purpose:	To return file variables,
 *				as the result of a function call
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

#ifndef FILERESULTTYPE
#define FILERESULTTYPE 1
#include "Item.h"
// Some compilers need these class declarations
class FileItem;

class FileResultType
	{
	friend class AdminImperative;
	friend class AdminReadFile;
	friend class FileList;
	friend class Item;
	friend class List;

	protected:
	// Protected variables

	ResultType result;
	FileItem *createdFileItem;
	FILE *outputFile;
	FILE *referenceFile;

	protected:
	// Constructor

	FileResultType()
		{
		result = RESULT_OK;
		createdFileItem = NULL;
		outputFile = NULL;
		referenceFile = NULL;
		}
	};
#endif

/*************************************************************************
 *	"Come, everyone! Clap your hands!
 *	Shout to God with joyful praise!
 *	For the Lord Most High is awesome.
 *	He is the great King of all the earth." (Psalm 47:1-2)
 *************************************************************************/
