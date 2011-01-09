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
#include <math.h>
#include <stdlib.h>

// First, I am going to put the main function, and then all the others by alphabetical order

int main(int argc, char *args[])
{
	int x; // Counters
	int num;//number of papersheets
	struct papersheet *partitura=NULL;
	char *infilename;
	if (argc)
	{
		for(x=1;x<argc;x++)//1st argument is tha path name
		{
			if(args[x][0]=='-')
				switch(args[x][1])
				{
					case 'h':
						display_help();
						return 1;
						break;
					case 'i':
						strcpy(infilename=calloc(strlen(args[x+1]),sizeof(char)),args[x+1]);
						break;
				}
		}
		num=readfile(infilename,partitura);
	}
	else
	{

	}

	return 0;
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
	printf("\n\tThe file output will be <$X>-<$T>, where $X is the song's index and $T is the title of the song");
	return;
}

/*
 * Here comes the frequency conversor, as the louder note is C3 (C, in abc) I will use it to
 * calculate all the other frequencies depending on how much distance is between them.
 * so float frecuency(int x) will be float=> the frecuency, and x the distance.
 */

float frecuency(int x)
{
	if(x>=0)
		return C3*pow(BASE112,x);
	else
		return 0;
}

/*
 * Here is the most difficult part, reading abc notation and parse it to the papersheet structure
 */
int readfile(char *filename,struct papersheet* partitura)
{
	int num=0,a,b,super=0,nota,tempo;
	char temp[100],tem[10],natural,flat,sharp,cuidao=0;
	FILE *file=fopen(filename,"r");
	while(fgets(temp,100,file)!=NULL)
	{
		switch(temp[0])
		{
			case 'X':
				if(partitura==NULL)
				{
					partitura=calloc(sizeof(struct papersheet), num+1);
					super=0;
				}
				else
				{
					partitura[num].num_notas=super;
					num++;
					partitura=realloc(partitura, sizeof(struct papersheet)*(1+num));
					super=0;
				}
				sscanf(temp,"X:%d",&(partitura[num].index));
				break;
			case 'T':
				strcpy(partitura[num].title=calloc(sizeof(char),strlen(&temp[2])+1),&temp[2]);
				break;
			case 'M':
				sscanf(temp,"M:%d/%d",&a,&b);
				partitura[num].meter[0]=a;
				partitura[num].meter[1]=b;
				break;
			case 'L':
				sscanf(temp,"L:1/%d",&partitura[num].def_length);
				break;
			case 'Q':
				sscanf(temp,"Q:%s",tem);
				if(strstr(tem,"="))
				{
					sscanf(tem,"%d=1/%d",&partitura[num].tempo,&a);
					cuidao=1;
				}
				else
					sscanf(tem,"%d",&partitura[num].tempo);
			case 'K':
				sscanf(temp,"K:%s",tem);
				switch(tem[0])
				{
					case 'A':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=0;
						else
							partitura[num].key=1;

					case 'B':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=2;
						else
							partitura[num].key=3;
					case 'C':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=4;
						else
							partitura[num].key=5;
					case 'D':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=6;
						else
							partitura[num].key=7;

					case 'E':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=8;
						else
							partitura[num].key=9;

					case 'F':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=10;
						else
							partitura[num].key=11;

					case 'G':
						if(tem[1]=='m'||strstr(tem,"min")!=NULL)
							partitura[num].key=12;
						else
							partitura[num].key=13;
				}
				break;
			default:
				for(a=0;temp[a]!=0&&temp[a]!='\n';a++)
				{
					switch(temp[a])
					{
						case '|':
							break;
						case '=':
							natural=1;
							break;
						case '^':
							sharp++;
							break;
						case '_':
							flat--;
							break;
						case 'b':
							nota+=2;
						case 'a':
							nota+=2;
						case 'g':
							nota+=2;
						case 'f':
							nota++;
						case 'e':
							nota+=2;
						case 'd':
							nota+=2;
						case 'c':
							nota+=24;
							break;
						case '\'':
							nota+=12;
							break;
						case 'B':
							nota+=2;
						case 'A':
							nota+=2;
						case 'G':
							nota+=2;
						case 'F':
							nota++;
						case 'E':
							nota+=2;
						case 'D':
							nota+=2;
						case 'C':
							nota+=12;
							break;
						case ',':
							nota-=12;
							break;
						case 'z':
							nota=-1;
						case '/':
							if(temp[a+1]<'9'&&temp[a+1]>'1')
								tempo/=(temp[++a]-'0');
							else
								tempo/=2;
							break;
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							tempo*=(temp[a]-'0');
							break;
					}
					if((temp[a+1]>='A'&&temp[a+1]<='Z')||(temp[a+1]>='a'&&temp[a+1]<='z')||temp[a+1]=='|')
					{
						partitura[num].notes=realloc(partitura[num].notes,sizeof(struct nota)*(super+1));
						partitura[num].notes[super].duracion=tempo;
						if(natural==0)
						{
							if(sharp)
							{
								partitura[num].notes[super].nota=nota+sharp;
								sharp=0;
							}
							if(flat)
							{
								partitura[num].notes[super].nota=nota+sharp;
								flat=0;
							}
						}
						super++;
						nota=0;
						tempo=128;
					}
				}

		}
	}
	fclose(file);
	return num;
}
