/*
 *	Class:			FileResultType
 *	Purpose:		To return file variables of a function
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
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

#ifndef FILERESULTTYPE
#define FILERESULTTYPE 1
#include "Item.h"
// Class declarations needed by some compilers
class FileItem;

class FileResultType
	{
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminReadFile;
	friend class FileList;
	protected:
	// Protected variables

	ResultType result;
	FileItem *createdFileItem;

	protected:
	// Constructor / deconstructor

	FileResultType()
		{
		result = RESULT_OK;
		createdFileItem = NULL;
		}
	};
#endif

/*************************************************************************
 *	"Come, everyone! Clap your hands!
 *	Shout to God with joyful praise!
 *	For the Lord Most High is awesome.
 *	He is the great King of all the earth." (Psalm 47:1-2)
 *************************************************************************/
