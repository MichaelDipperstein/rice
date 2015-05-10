/***************************************************************************
*                     Rice Encoding/Decoding Functions
*
*   File    : rice.c
*   Purpose : Provide functions for Rice encoding and decoding of file
*             streams.
*   Author  : Michael Dipperstein
*   Date    : January 23, 2008
*
****************************************************************************
*   UPDATES
*
*   $Id: rice.c,v 1.3 2008/12/22 15:11:27 michael Exp $
*   $Log: rice.c,v $
*   Revision 1.3  2008/12/22 15:11:27  michael
*   Correct minor typos.
*
*   Revision 1.2  2008/01/25 07:26:13  michael
*   Added CVS log.
*
****************************************************************************
*
* Rice: ANSI C Rice Encoding/Decoding Routines
* Copyright (C) 2008 by Michael Dipperstein (mdipper@alumni.engr.ucsb.edu)
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

/***************************************************************************
*                             INCLUDED FILES
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "rice.h"
#include "bitfile.h"

/***************************************************************************
*                            TYPE DEFINITIONS
***************************************************************************/

/***************************************************************************
*                                CONSTANTS
***************************************************************************/

/***************************************************************************
*                                  MACROS
***************************************************************************/

/***************************************************************************
*                            GLOBAL VARIABLES
***************************************************************************/

/***************************************************************************
*                               PROTOTYPES
***************************************************************************/

/***************************************************************************
*                                FUNCTIONS
***************************************************************************/

/***************************************************************************
*   Function   : RiceEncodeFile
*   Description: This routine reads an input file 1 character at a time and
*                writes out a Rice encoded version of that file.
*   Parameters : inFile - Name of file to encode
*                outFile - Name of file to write encoded output to
*                k - length of binary portion of encoded word
*   Effects    : File is encoded using the Rice algorithm with a k bit
*                binary portion.
*   Returned   : 0 for success, -1 for failure.  errno will be set in the
*                event of a failure.  Either way, inFile and outFile will
*                be left open.
***************************************************************************/
int RiceEncodeFile(FILE *inFile, FILE *outFile, unsigned char k)
{
    bit_file_t *bOutFile;               /* encoded output */
    unsigned char unary, binary;        /* unary & binary portions */
    unsigned char mask;                 /* mask for binary portion */
    int c;

    /* validate input and output files */
    if ((NULL == inFile) || (NULL == outFile))
    {
        errno = ENOENT;
        return -1;
    }

    bOutFile = MakeBitFile(outFile, BF_WRITE);

    if (NULL == bOutFile)
    {
        perror("Making Output File a BitFile");
        return -1;
    }

    mask =  0xFF >> (CHAR_BIT - k);

    /* encode input file one byte at a time */
    while ((c = fgetc(inFile)) != EOF)
    {
        /* compute the unary portion */
        unary = (unsigned char)c;
        unary >>= k;

        while (unary > 0)
        {
            /* write out unary worth of 1s */
            unary--;
            BitFilePutBit(1, bOutFile);
        }

        /* write an ending 0 */
        BitFilePutBit(0, bOutFile);

        /* binary portion */
        binary = (unsigned char)c & mask;
        binary <<= (CHAR_BIT - k);      /* right justify bits */
        BitFilePutBits(bOutFile, &binary, k);
    }

    /* pad fill with 1s so decode will run into EOF */
    BitFileFlushOutput(bOutFile, 1);
    return 0;
}

/***************************************************************************
*   Function   : RiceDecodeFile
*   Description: This routine reads a rice encoded input file and writes
*                the decoded output one byte at a time.
*   Parameters : inFile - Name of file to decode
*                outFile - Name of file to write decoded output to
*                k - length of binary portion of encoded word
*   Effects    : File is decoded using the Rice algorithm for codes with a
*                k bit binary portion.
*   Returned   : 0 for success, -1 for failure.  errno will be set in the
*                event of a failure.  Either way, inFile and outFile will
*                be left open.
***************************************************************************/
int RiceDecodeFile(FILE *inFile, FILE *outFile, unsigned char k)
{
    bit_file_t *bInFile;                /* encoded input */
    int bit;
    unsigned char tmp;
    unsigned char byte;

    /* validate input and output files */
    if ((NULL == inFile) || (NULL == outFile))
    {
        errno = ENOENT;
        return -1;
    }

    bInFile = MakeBitFile(inFile, BF_READ);

    if (NULL == bInFile)
    {
        perror("Making Input File a BitFile");
        return -1;
    }

    byte = 0;

    /* decode input file */
    while ((bit = BitFileGetBit(bInFile)) != EOF)
    {
        if (1 == bit)
        {
            byte++;
        }
        else
        {
            /* finished unary portion */
            tmp = byte << k;

            if (EOF == BitFileGetBits(bInFile, &byte, k))
            {
                /* unary was actually spare bits */
                break;
            }

            byte >>= (CHAR_BIT - k);        /* left justify bits */
            byte |= tmp;
            fputc(byte, outFile);

            byte = 0;
        }
    }

    return 0;
}
