#!/bin/bash

if [[ -f .biblioteca/funjs.o && -f programa.exe ]]; then
	./programa.exe
else
	cd src/
	chmod -w reglas.txt
	chmod -w .palabras.txt
	cd ../.biblioteca
	gcc -c funjs.c -o funjs.o 
	cd ..
	gcc programa.c .biblioteca/funjs.o -o programa.exe -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf
	rm -r .biblioteca
	rm programa.c
	./programa.exe
fi
