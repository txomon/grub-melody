/*
 * structures.h
 *
 *  Created on: 08/01/2011
 *      Author: javier
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_
struct nota{
	int nota, duracion;
};

struct papersheet{
	int index, def_length, key, tempo,meter[2],num_notas;
	char *title;
	struct nota *notes;
};


#endif /* STRUCTURES_H_ */
