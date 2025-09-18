#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funjs.h"

double rtd(double rad)
{
    return rad * (180.0 / PI);
}
double dtr(double deg)
{
    return deg * (PI / 180.0);
}

// La funcion scani() sirve para crear cadenas de texto dinamicas de esta forma evitas tener problemas
// por falta de espacio en el arreglo. Es importante usar esta funcion con un char* y liberar este puntero
// al final del main    Requiere de stdio y stdlib
char* scani()
{
  char* str = malloc(sizeof(char));
  int n = 0;
  while((str[n] = getchar()) != '\n' && str[n] != EOF){
    n++;
    char* temp = realloc(str, n + 1);
    if(temp == NULL){
      free(str);
      printf("\nError de memoria");
      return NULL;
    }
    str = temp;
  }
  str[n] = '\0';
  return str;
}

char* scanif(FILE *docu)
{
  char* str = malloc(sizeof(char));
  int n = 0;
  while((str[n] = fgetc(docu)) != '\n' && str[n] != EOF){
    n++;
    char* temp = realloc(str, n + 1);
    if(temp == NULL){
      free(str);
      printf("\nError de memoria");
      return NULL;
    }
    str = temp;
  }
  str[n] = '\0';
  return str;
}

//Funcion split. Por el momento usar strtok. Mejorar despues
//Liberar unicamente el apuntador a la primera palabra 
char** split(char str[], char lim[], int *nPalabras)
{
  char* str2 = strdup(str);
  char** array = malloc(sizeof(char*) * 2);
  *nPalabras = 0;

  char *token = strtok(str2, lim);
  while (token != NULL) {
    array[*nPalabras] = mstrdup(token);
    (*nPalabras)++;
    array = realloc(array, sizeof(char*) * (*nPalabras + 1));
    token = strtok(NULL, lim);
  }
  free(str2);
  return array;
}

//Funcion trim 
//
char* trim(char* str)
{
  int fin = 0, inicio = 0, len = 0;
  
  while(str[len] != '\0')
    len++;
  while(str[inicio] == ' ' || str[inicio] == '\t')
    inicio++;
  fin = len - 1;
  while (str[fin] == ' ' || str[fin] == '\t') 
    fin--;

  char* nstr = malloc(sizeof(char) * (fin - inicio + 2));
  if(nstr == NULL)
    return NULL;

  for(int i = 0; i < fin - inicio + 1; i++)
    nstr[i] = str[inicio + i];

  nstr[fin - inicio + 1] = '\0';
  return nstr;
}

int digito(char str[])
{
  int n = 0;
  while (str[n] != '\0') {
    if((str[n] >= 48 && str[n] <= 57) || str[n] == 46 || str[n] == 45)
      n++;
    else 
      return 0;
  }
  return 1;
}

//Recibe una cadema de caracteres como unico parametro y convierte todas sus letras en minusculas.
//Por el momento no incluye validacion, por lo que antes de usarla, es necesario asegurarce que la cadena
//que pasas sean solo letras
char* lower( char* string)
{
  int letras; 

  letras = strlen(string);

  char *newString = malloc( sizeof(char) * (letras + 1));

  for(int i = 0; i < letras; i++)
    newString[i] = tolower(string[i]);
  newString[letras] = '\0';

  return newString;
}

void desorden(int *array, int length)
{
  int aleatorio;
  for(int i = length - 1; i > 0; i--){
    aleatorio = rand() % i ;
    array[i] = array[aleatorio] ^ array[i];
    array[aleatorio] = array[aleatorio] ^ array[i];
    array[i] = array[aleatorio] ^ array[i];
  }
}

void sort(int *array, int length)
{
  for(int j = 0; j < length; j++){
    for(int i = j; i < length; i++)
    if(array[i] < array[j]){
      array[j] = array[i] ^ array[j];
      array[i] = array[i] ^ array[j];
      array[j] = array[i] ^ array[j];
    }
  }
}

char* mstrdup(char* s){
  char* copy = malloc(strlen(s) + 1);
  if(copy != NULL)
    strcpy(copy, s);
  return copy;
}

//Busca una string especifica en un arreglo de strings. Requiere como primer parametro la string a buscar, el segundo
//parametro es donde la busca y el ultimo parametro es la medida del arreglo donde se buscara la string.
//Devuelve la posicion en el arreglo donde aparece la string buscada, si no esta devuelve -1. Solo devuelve la primera aparicion
/*int includesc(char* str, char** array, int length){
  int indice;
  for(int i = 0; i < length; i++){
    if(strcmp(lower(str), lower(array[i])) == 0){
      indice = i;
      return indice;
    }
  }
  return -1;
}*/