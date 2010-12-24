#!bin/sh
#
#	The grub-melody project, aims to make easier to create your own melodies for Grub. 
#	
#	Released under the terms of the GPL-3 version; see
#	./GPL-3

. ./config
. ./functions

if [ -z $languaje ]; then
	languaje=spa;
fi

. ./lang/$languaje

case "$#" in
	"0")	
		presentacion;
		pedirnombre;
		pedirpartitura;;
		


	"2")
		parsepartitura;;


esac 
