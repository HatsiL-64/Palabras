#ifndef FUNJS_H
#define FUNJS_H

#define PI 3.14159265358979323846

double rtd(double);

double dtr(double);

char* scani();

char* scanif(FILE*);

char** split(char[], char[], int*);

char* trim(char*);

int digito(char*);

char* lower(char*);

void desorden(int*, int);

void sort(int*, int);

char* mstrdup(char*);

//int includesc(char*, char**, int);

#endif
