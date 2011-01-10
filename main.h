/*
 * main.h
 *
 *  Created on: 03/01/2011
 *      Author: javier
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "structures.h"
#include "frecuency.h"
	void display_help();
	float frecuency(int);
	struct papersheet* readfile(char *,struct papersheet*,int*);
	void createfile(struct papersheet*, int);
#endif /* MAIN_H_ */
