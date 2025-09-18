cd biblioteca
gcc -c funjs.c -o funjs.o
cd ..
gcc programa.c biblioteca/funjs.o -o programa.exe -lallegro -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lm
./programa.exe
