/***************************************************************************
*                 Sample Program Using Rice Encoding Library
*
*   File    : sample.c
*   Purpose : Demonstrate usage of Rice encoding library
*   Author  : Michael Dipperstein
*   Date    : January 23, 2008
*
****************************************************************************
*
* SAMPLE: Sample usage of Rice Encoding Library
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

/***************************************************************************
*                             INCLUDED FILES
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "optlist.h"
#include "rice.h"

/***************************************************************************
*                               PROTOTYPES
***************************************************************************/
void ShowUsage(const char *progName);

/***************************************************************************
*                                FUNCTIONS
***************************************************************************/

/****************************************************************************
*   Function   : main
*   Description: This is the main function for this program, it validates
*                the command line input and, if valid, it will call
*                functions to encode or decode a file using the rice
*                coding algorithm.
*   Parameters : argc - number of parameters
*                argv - parameter list
*   Effects    : Encodes/Decodes input file
*   Returned   : 0 for success, errno for failure.
****************************************************************************/
int main(int argc, char *argv[])
{
    FILE *inFile;
    FILE *outFile;
    char encode;                    /* encode/decode */
    unsigned char k;
    option_t *optList;
    option_t *thisOpt;

    /* initialize variables */
    k = 0;
    encode = 1;
    inFile = NULL;
    outFile = NULL;

    /* parse command line */
    optList = GetOptList(argc, argv, "cdk:i:o:h?");
    thisOpt = optList;

    while (thisOpt != NULL)
    {
        switch(thisOpt->option)
        {
            case 'c':       /* compression mode */
                encode = 1;
                break;

            case 'd':       /* decompression mode */
                encode = 0;
                break;

            case 'k':       /* length of binary portion */
                k = (unsigned char)atoi(thisOpt->argument);
                break;

            case 'i':       /* input file name */
                if (inFile != NULL)
                {
                    fprintf(stderr, "Multiple input files not allowed.\n");
                    fclose(inFile);

                    if (outFile != NULL)
                    {
                        fclose(outFile);
                    }

                    FreeOptList(optList);
                    return EINVAL;
                }
                else if ((inFile = fopen(thisOpt->argument, "rb")) == NULL)
                {
                    perror("Opening Input File");

                    if (outFile != NULL)
                    {
                        fclose(outFile);
                    }

                    FreeOptList(optList);
                    return errno;
                }
                break;

            case 'o':       /* output file name */
                if (outFile != NULL)
                {
                    fprintf(stderr, "Multiple output files not allowed.\n");
                    fclose(outFile);

                    if (inFile != NULL)
                    {
                        fclose(inFile);
                    }

                    FreeOptList(optList);
                    return EINVAL;
                }
                else if ((outFile = fopen(thisOpt->argument, "wb")) == NULL)
                {
                    perror("Opening Output File");

                    if (inFile != NULL)
                    {
                        fclose(inFile);
                    }

                    FreeOptList(optList);
                    return errno;
                }
                break;

            case 'h':
            case '?':
                ShowUsage(argv[0]);
                FreeOptList(optList);
                return 0;
        }

        optList = thisOpt->next;
        free(thisOpt);
        thisOpt = optList;
    }

    /* validate command line */
    if (k < 1 || k > 7)
    {
        fprintf(stderr, "Error: k must be between 1 and 7.\n");
        ShowUsage(argv[0]);

        if (inFile != NULL)
        {
            free(inFile);
        }

        if (outFile != NULL)
        {
            free(outFile);
        }

        FreeOptList(optList);
        exit(EXIT_FAILURE);
    }

    if (inFile == NULL)
    {
        fprintf(stderr, "Input file must be provided\n");
        ShowUsage(argv[0]);

        if (outFile != NULL)
        {
            free(outFile);
        }

        exit(EXIT_FAILURE);
    }
    else if (outFile == NULL)
    {
        fprintf(stderr, "Output file must be provided\n");
        ShowUsage(argv[0]);

        if (inFile != NULL)
        {
            free(inFile);
        }

        exit(EXIT_FAILURE);
    }

    /* we have valid parameters encode or decode */
    if (encode)
    {
        RiceEncodeFile(inFile, outFile, k);
    }
    else
    {
        RiceDecodeFile(inFile, outFile, k);
    }

    fclose(inFile);
    fclose(outFile);
    return 0;
}

/****************************************************************************
*   Function   : ShowUsage
*   Description: This function sends instructions for using this program to
*                stdout.
*   Parameters : progName - the name of the executable version of this
*                           program.
*   Effects    : Usage instructions are sent to stdout.
*   Returned   : None
****************************************************************************/
void ShowUsage(const char *progName)
{
    printf("Usage: %s <options>\n\n", FindFileName(progName));
    printf("Options:\n");
    printf("  -c : Encode input file to output file.\n");
    printf("  -d : Decode input file to output file.\n");
    printf("  -k [1-7]: Length of binary portion.\n");
    printf("  -i <filename> : Name of input file.\n");
    printf("  -o <filename> : Name of output file.\n");
    printf("  -h | ?  : Print out command line options.\n\n");
}
