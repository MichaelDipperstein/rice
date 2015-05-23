/***************************************************************************
*                Header for Rice Encoding and Decoding Library
*
*   File    : rice.h
*   Purpose : Provides prototypes for functions that use Rice coding to
*             encode/decode files.
*   Author  : Michael Dipperstein
*   Date    : January 23, 2008
*
****************************************************************************
*
* Rice: ANSI C Rice Encoding/Decoding Routines
* Copyright (C) 2008 - 2015 by
* Michael Dipperstein (mdipper@alumni.engr.ucsb.edu)
*
* This file is part of the rice library.
*
* The rice library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation; either version 3 of the
* License, or (at your option) any later version.
*
* The rice library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/

#ifndef _RICE_H_
#define _RICE_H_

/***************************************************************************
*                                CONSTANTS
***************************************************************************/

/***************************************************************************
*                               PROTOTYPES
***************************************************************************/
 /* encode inFile */
int RiceEncodeFile(FILE *inFile, FILE *outFile, const unsigned char k);

/* decode inFile*/
int RiceDecodeFile(FILE *inFile, FILE *outFile, const unsigned char k);

#endif  /* ndef _RICE_H_ */
