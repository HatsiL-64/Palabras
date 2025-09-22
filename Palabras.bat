@echo off
IF NOT EXIST programa.exe (
	cd src
	attrib +R * 
	attrib +H .palabras.txt
	cd ..\.biblioteca
	gcc -c funjs.c -o funjs.o
	cd ..
	gcc programa.c .biblioteca\funjs.o -o programa.exe -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf
	REM rd /Q /S .biblioteca
	del programa.c
)
IF EXIST programa.exe ./programa.exe
