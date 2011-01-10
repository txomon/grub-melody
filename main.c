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
#ifdef DEBUG
	fprintf(stderr,"[DEBUG] argc:%d\n",argc);
	for(x=0;x<argc;x++)
		fprintf(stderr,"[DEBUG] args[%d]=> %s\n",x,args[x]);
#endif
	if (argc>1)
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
						x++;
						break;
				}
		}
		partitura=readfile(infilename,partitura,&num);
	}
	else
	{
		display_help(args[0]);
		return 0;
	}
#ifdef DEBUG
	fprintf(stderr,"\n[DEBUG] num of papersheets= %d\n",num);
	fprintf(stderr,"[DEBUG] a ver...");
#endif
	if(partitura!=NULL)
		createfile(partitura,num);
	return 0;
}

/*
 *  Other functions
 */

/*
 * void display_help () is a function that shows how the program works
 * It must be in alphabetical order (just to program easier).
 */


void display_help(char *basepath)
{
	printf("\ngrub-melody: %s [ -i <input_file> ] [-o <output_file>] [-h]\n",basepath);
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
struct papersheet* readfile(char *filename,struct papersheet* partitura,int *number)
{
	int num=0,a,b,super=0,nota,tempo;
	char temp[100],tem[10],natural,flat,sharp,cuidao=0;
	FILE *fich=fopen(filename,"r");
	if(fich==NULL)
	{
		fprintf(stderr,"File not found");
		return NULL;
	}
	while(fgets(temp,99,fich)!=NULL)
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
					num++;
					partitura=realloc(partitura, sizeof(struct papersheet)*(1+num));
					super=0;
				}
				sscanf(temp,"X:%d",&(partitura[num].index));
				break;
			case 'T':
				strcpy(partitura[num].title=calloc(sizeof(char),strlen(&temp[2])+1),&temp[2]);
				if(partitura[num].title[strlen(partitura[num].title)-1]=='\n')
					partitura[num].title[strlen(partitura[num].title)-1]='\0';
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
					sscanf(tem,"%d=1/%d",&partitura[num].tempo,&partitura[num].tempomeasure);
				}
				else
				{
					sscanf(tem,"%d",&partitura[num].tempo);
					partitura[num].tempomeasure=partitura[num].def_length;
				}

			case 'K':
				sscanf(temp,"K:%s",partitura[num].key);

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
						partitura[num].notes[super].nota=nota+sharp+flat;
						partitura[num].num_notas=++super;
						nota=0;
						tempo=128;
						sharp=0;
						nota=0;
						flat=0;
					}
				}

		}
	}
	fclose(fich);
	*number=num+1;
	return partitura;
}


/*
 * And now comes writing the file, where I just have to make the conversion to Hz and time.
 */

void createfile(struct papersheet* partitura,int num)
{
	int x,y,basetempo;
	FILE *fich;

	for(x=0;x<num;x++)
	{
		fich=fopen(partitura[x].title,"w");
		basetempo=partitura[x].tempo*128*(partitura[x].def_length/partitura[x].tempomeasure);
		fprintf(fich,"%d ",basetempo);
		for(y=0;y<partitura[x].num_notas;y++)
		{
			fprintf(fich,"%d %d ",(int)frecuency(partitura[x].notes[y].nota),partitura[x].notes[y].duracion);
		}
		fclose(fich);
	}
}
