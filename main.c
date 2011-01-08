/*
 * main.c
 *
 *  Created on: 03/01/2011
 *      Author: javier
 */
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
// First, I am going to put the main function, and then all the others by alphabetical order
int main(int argc, char *args[])
{
	int x; // Counters
	FILE *infile,*outfile;
	char *infilename,*outfilename;
	if (argc)
	{
		for(x=1;x<argc;x++)//1st argument is tha path name
		{
			if(args[x][0]=='-')
				switch(args[x][1])
				{
					case 'h':
						display_help();
						break;
					case 'i':
						strcpy(infilename=calloc(strlen(args[x+1]),sizeof(char)),args[x+1]);
					case 'o':
						strcpy(outfilename=calloc(strlen(args[x+1]),sizeof(char)),args[x+1]);
				}
		}
		readfile();
	}
	else
	{

	}

}

/*
 *  Other functions
 */

/*
 * void display_help () is a function that shows how the program works
 * It must be in alphabetical order (just to program easier).
 */


void display_help()
{
	printf("grub-melody: grub-melody [ -i <input_file> ] [-o <output_file>] [-h]");
	printf("grub-melody is used as follows:\n");
	printf("-h : To display this help");
	printf("-i <file in> : To use that file as input\n");
	printf("-o <file out> : Name of the file to write in the melody\n");

	return;
}

