/***************************************************************************
*                 Sample Program Using Rice Encoding Library
*
*   File    : sample.c
*   Purpose : Demonstrate usage of Rice encoding library
*   Author  : Michael Dipperstein
*   Date    : January 23, 2008
*
****************************************************************************
*   UPDATES
*
*   $Id: sample.c,v 1.2 2009/04/17 04:28:00 michael Exp $
*   $Log: sample.c,v $
*   Revision 1.2  2009/04/17 04:28:00  michael
*   Added log
*
****************************************************************************
*
* SAMPLE: Sample usage of Rice Encoding Library
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
#include "optlist.h"
#include "rice.h"

/***************************************************************************
*                               PROTOTYPES
***************************************************************************/
char *RemovePath(char *fullPath);
void ShowUsage(char *progName);

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
*   Returned   : EXIT_SUCCESS for success, otherwise EXIT_FAILURE.
****************************************************************************/
int main(int argc, char *argv[])
{
    char *inFile, *outFile;
    char encode;                    /* encode/decode */
    unsigned char k;
    option_t *optList, *thisOpt;

    /* initialize variables */
    k = 0;
    encode = TRUE;
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
                encode = TRUE;
                break;

            case 'd':       /* decompression mode */
                encode = FALSE;
                break;

            case 'k':       /* length of binary portion */
                k = (unsigned char)atoi(thisOpt->argument);
                break;

            case 'i':       /* input file name */
                if (inFile != NULL)
                {
                    fprintf(stderr, "Multiple input files not allowed.\n");
                    free(inFile);

                    if (outFile != NULL)
                    {
                        free(outFile);
                    }

                    FreeOptList(optList);
                    return EXIT_FAILURE;
                }
                else if ((inFile =
                    (char *)malloc(strlen(thisOpt->argument) + 1)) == NULL)
                {
                    perror("Memory allocation");

                    if (outFile != NULL)
                    {
                        free(outFile);
                    }

                    FreeOptList(optList);
                    return EXIT_FAILURE;
                }

                strcpy(inFile, thisOpt->argument);
                break;

            case 'o':       /* output file name */
                if (outFile != NULL)
                {
                    fprintf(stderr, "Multiple output files not allowed.\n");
                    free(outFile);

                    if (inFile != NULL)
                    {
                        free(inFile);
                    }

                    FreeOptList(optList);
                    return EXIT_FAILURE;
                }
                else if ((outFile =
                    (char *)malloc(strlen(thisOpt->argument) + 1)) == NULL)
                {
                    perror("Memory allocation");

                    if (inFile != NULL)
                    {
                        free(inFile);
                    }

                    FreeOptList(optList);
                    return EXIT_FAILURE;
                }

                strcpy(outFile, thisOpt->argument);
                break;

            case 'h':
            case '?':
                ShowUsage(RemovePath(argv[0]));
                FreeOptList(optList);
                return EXIT_SUCCESS;
        }

        optList = thisOpt->next;
        free(thisOpt);
        thisOpt = optList;
    }

    /* validate command line */
    if (k < 1 || k > 7)
    {
        fprintf(stderr, "Error: k must be between 1 and 7.\n");
        ShowUsage(RemovePath(argv[0]));

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
        fprintf(stderr, "Enter \"%s -?\" for help.\n", RemovePath(argv[0]));

        if (outFile != NULL)
        {
            free(outFile);
        }

        exit(EXIT_FAILURE);
    }
    else if (outFile == NULL)
    {
        fprintf(stderr, "Output file must be provided\n");
        fprintf(stderr, "Enter \"%s -?\" for help.\n", RemovePath(argv[0]));

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

    free(inFile);
    free(outFile);
    return EXIT_SUCCESS;
}

/****************************************************************************
*   Function   : RemovePath
*   Description: This is function accepts a pointer to the name of a file
*                along with path information and returns a pointer to the
*                character that is not part of the path.
*   Parameters : fullPath - pointer to an array of characters containing
*                           a file name and possible path modifiers.
*   Effects    : None
*   Returned   : Returns a pointer to the first character after any path
*                information.
****************************************************************************/
char *RemovePath(char *fullPath)
{
    int i;
    char *start, *tmp;                          /* start of file name */
    const char delim[3] = {'\\', '/', ':'};     /* path deliminators */

    start = fullPath;

    /* find the first character after all file path delimiters */
    for (i = 0; i < 3; i++)
    {
        tmp = strrchr(start, delim[i]);

        if (tmp != NULL)
        {
            start = tmp + 1;
        }
    }

    return start;
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
void ShowUsage(char *progName)
{
    printf("Usage: %s <options>\n\n", progName);
    printf("Options:\n");
    printf("  -c : Encode input file to output file.\n");
    printf("  -d : Decode input file to output file.\n");
    printf("  -k [1-7]: Length of binary portion.\n");
    printf("  -i <filename> : Name of input file.\n");
    printf("  -o <filename> : Name of output file.\n");
    printf("  -h | ?  : Print out command line options.\n\n");
}
